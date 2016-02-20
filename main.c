/*
   Projecto IAED - 1
   Grupo TG020 

   Joao Leite - 77907
   Joao Tomazio - 78039
   Tiago Fernandes - 77896
*/

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>

/* Diretivas de Pre-Compilador */
#define N_ESTADOS 28
#define N_PPE 12

/* Prototipos */
void SetupMatriz();
void AdicionarVotos();
void CalcularMandatos();
void PrintCabecalho();
void SetupTotal();
void PrintTotal();

/* Vectores e Variaveis Globais */
char paises[N_ESTADOS][3] = {"DE","AT","BE","BG","CY","HR","DK","SK","SI","ES","EE","FI","FR","GR","HU","IE","IT","LV","LT","LU","MT","NL","PL","PT","GB","CZ","RO","SE"};
char ppes[N_PPE][4] = {"EPP","PES","LDE","EGP","ECR","ELP","ELD","EDP","EAF","EMN","ECP","AED"};
int mandatos[N_ESTADOS] = {96,18,21,17,6,11,13,13,8,54,6,13,74,21,21,11,73,8,11,6,6,26,51,21,73,21,32,20}; /* Total de mandatos por pais */
int matriz[N_ESTADOS][N_PPE]; /* Matriz de Votos */
int total[N_PPE]; /* Linha TE */

/* MAIN */
int main()
{
    char command;
    int estado;  
    SetupMatriz();

    while(1)
    {
        command = getchar();

        switch(command)
        {
        	case '+':
            	AdicionarVotos();
            	break;

        	case 'm':
        		scanf("%d", &estado); /* Identifica o estado */
        		PrintCabecalho();
        		CalcularMandatos(estado - 1); /* Executa metodo D'Hont para um dado estado, com correccao de indice do vetor */
        		break;

        	case 'e':
        		SetupTotal(); /* Inicializa a 0 a linha TE */
            	PrintCabecalho();

        		for(estado = 0; estado < N_ESTADOS; estado++) /* Metodo D'Hont a todos os estados */
        			CalcularMandatos(estado);

        		PrintTotal();
				break;

        	case 'x':
            	return EXIT_SUCCESS; /* Termina o programa */
        
        	default:
            	printf("ERRO: Comando desconhecido\n");
        }

        getchar();
    }

    return EXIT_FAILURE;
}

/* Funcoes */

void SetupMatriz() /* Inicializa todas as entradas da matriz a 0 */
{
	int estado, ppe;

	for(estado = 0; estado < N_ESTADOS; estado++)
	{
		for(ppe = 0; ppe < N_PPE; ppe++)
			matriz[estado][ppe] = 0;
	}
		
}

void SetupTotal() /* Inicializa todas as entradas do vector "total" a 0, linha TE */
{
	int i;

	for(i = 0; i < N_PPE; i++)
		total[i] = 0;		
}

void PrintCabecalho() /* Desenha o cabecalho da tabela de mandatos */
{
	int i;

	printf("  ");

	for(i = 0; i < N_PPE; i++)
		printf(" %s", ppes[i]); /* Imprime o codigo de cada partido */

	printf("\n");
}

void AdicionarVotos() /* Adiciona votos a matriz */
{
	int estado, ppe, votos;

	scanf("%d", &estado); /* Le o estado a adicionar votos */
	scanf("%d", &ppe); /* Le o partido a adicionar votos */
	scanf("%d", &votos); /* Le o numero de votos a adicionar */

	matriz[estado - 1][ppe - 1] += votos; /* Adiciona */
}

void CalcularMandatos(int estado) /* Aplica o metodo D'Hont */
{
	int resultados[N_PPE], ind_maior, i, caso_zero, eleitos;
	float actual[N_PPE]; /* Float para fazer a divisao real */

	caso_zero = 0; /* Numero de partidos sem votos */

	for(i = 0; i < N_PPE ;i++) /* Inicializacao de vectores */
	{
		actual[i] = matriz[estado][i]; /* Para poder trabalhar a linha, fazendo as divisoes */
		resultados[i] = 0; /* Reset resultados de mandatos do estado membro */

		if(actual[i] == 0) /* Se partido nao tiver votos */
			caso_zero++;
	}

	eleitos = 0; /* Inicializa numero de mandatos atribuidos ao pais */

	while(eleitos < mandatos[estado]) /* Enquanto houver mandatos a atribuir */
	{
		ind_maior = 0; /* Indice de partido com mais votos na iteracao */

		for(i = 1; i < N_PPE ; i++)
		{
			if(actual[i] > actual[ind_maior]) /* Encontrar o partido com mais votos na interacao actual */
				ind_maior = i;
			
			else if(actual[i] == actual[ind_maior]) /* Caso de empate */
			{
				if(matriz[estado][i] < matriz[estado][ind_maior]) /* Favorecer as minorias */
					ind_maior = i;
			}
		}

		actual[ind_maior] = (actual[ind_maior] * (resultados[ind_maior] + 1)) / (resultados[ind_maior] + 2); /* Divisao segundo o metodo D'Hondt */

		if(caso_zero != N_PPE) /* Se houver votos, atribui mandato ao partido com mais votos na iteracao */
			resultados[ind_maior]++;

		eleitos++; /* Foi atribuido mais um mandato no estado */
	}

	printf("%s", paises[estado]); /* Imprime codigo do estado */

	for(i = 0; i < N_PPE; i++) /* Imprime valores de todos os partidos */
	{
		printf(" %3d", resultados[i]);
		total[i] += resultados[i]; /* Soma para o total de cada ppe a nivel europeu, linha TE */
	}

	printf("\n");
}

void PrintTotal() /* Imprime linha TE */
{
	int i;

	printf("TE");

	for(i = 0; i < N_PPE; i++)
		printf(" %3d", total[i]);

	printf("\n");
}
