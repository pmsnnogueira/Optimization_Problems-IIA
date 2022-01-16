#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

#define DEFAULT_RUNS	10

int main(int argc, char *argv[])
{
	char        nome_fich[100];
	struct info EA_param;
	pchrom      pop = NULL, parents = NULL;
	chrom       best_run, best_ever;
	int         gen_actual, r, runs, i, inv;
	float       mbf = 0.0;

	//matriz
	int *matriz;


    // L� os argumentos de entrada
	if (argc == 3)
	{
		runs = atoi(argv[2]);
		strcpy(nome_fich, argv[1]);
	}
	else
        // Se o n�mero de execu��es do processo n�o for colocado nos argumentos de entrada, define-o com um valor por defeito
        if (argc == 2)
        {
            runs = DEFAULT_RUNS;
            strcpy(nome_fich, argv[1]);
        }
        // Se o nome do ficheiro de informa��es n�o for colocado nos argumentos de entrada, pede-o novamente
        else
        {
            runs = DEFAULT_RUNS;
            printf("Nome do Ficheiro: ");
            scanf("%s",nome_fich);
        }
    // Se o n�mero de execu��es do processo for menor ou igual a 0, termina o programa
	if (runs <= 0)
		return 0;
    //Inicializa a gera��o dos n�meros aleat�rios
	init_rand();
    // Preenche a matriz com dados dos objectos (peso e valor) e a estrutura EA_param que foram definidos no ficheiro de input
	matriz = lerFicheiro(nome_fich , &EA_param);

	printf("\n=========== Inicio ============\n");
	// Faz um ciclo com o n�mero de execu��es definidas

	
		for (r=0; r<runs; r++)
		{
        // Gera��o da popula��o inicial	
		pop = init_pop(EA_param); // pop tem um array de soluções (structs chrom), que dentro delas têm arrays de 1s e zeros
		
		// Avalia a popula��o inicial
		evaluate(pop, EA_param , matriz); // mat tem os dados direitinhos do ficheiro de texto
		// Como ainda n�o existe, escolhe-se como melhor solu��o a primeira da popula��o (poderia ser outra qualquer)
		best_run = pop[0];

        // Encontra-se a melhor solu��o dentro de toda a popula��o
		best_run = get_best(pop, EA_param, best_run);

        // Reserva espa�o para os pais da popula��o seguinte
		parents = malloc(sizeof(chrom)*EA_param.popsize);
		if (parents==NULL){
			printf("Erro na alocacao de memoria\n");
			exit(1);
		}

		// Ciclo de optimiza��o
		gen_actual = 1;
		while (gen_actual <= EA_param.numGenerations)
		{
            // Torneio bin�rio para encontrar os progenitores (ficam armazenados no vector parents)
			tournament(pop, EA_param, parents); // parents fica com uma série de soluções um pouco melhores que as anteriores, vai haver soluções repetidas, vão se perder algumas das soluções piores
            
			// Aplica os operadores gen�ticos aos pais (os descendentes ficam armazenados na estrutura pop)
			genetic_operators(parents, EA_param, pop); // pop fica com o crossover dos parents e da propria pop, e é aplicada mutação à pop
           
		    // Avalia a nova popula��o (a dos filhos)
			evaluate(pop, EA_param, matriz); // calcula se é válida e o fitness de cada solução
		avaliaIndividual_1(solucaovalida,);
            // Actualiza a melhor solu��o encontrada
			best_run = get_best(pop, EA_param, best_run);
			gen_actual++;
		}
		// Contagem das soluções inválidas
		for (inv=0, i=0; i<EA_param.popsize; i++)
			if (pop[i].valido == 0)
				inv++;
				
		// Escreve resultados da repeti��o que terminou
		printf("\nRepeticao %d:", r);
		write_best(best_run, EA_param);
		printf("Percentagem Invalidos: %.2f\n", 100*(float)inv/EA_param.popsize);
		printf("\n-----------\n");
		mbf += best_run.fitness;
		if (r==0 || best_run.fitness > best_ever.fitness)
			best_ever = best_run;
        // Liberta a mem�ria usada
		free(parents);
		free(pop);
}
	// Escreve resultados globais
	printf("\n\n=============== RESULTADOS GLOBAIS ===============");
	printf("\nMBF: %f\n", mbf/r);			//Melhor fitness
	printf("\nMelhor solucao encontrada");
	write_best(best_ever, EA_param);
	return 0;
}
