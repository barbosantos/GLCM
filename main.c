/*********************************************************************/
/** Aluno : Gabriel Santos Barbosa 								    **/
/** M a t r i c u l a : 201****5050156								**/
/** 																**/
/** Aluno : Willian Bastos Dias										**/
/** M a t r i c u l a : 201****5050270								**/
/**                                                                 **/
/** Trabalho Final	- Laboratorio de programaçao				   	**/
/** 					   											**/
/** Prof. Daniel Ferreira                                           **/
/** Versão do compilador: gcc (Ubuntu 4.8.4-2ubuntu1~14.04.1) 4.8.4 **/
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "func.h"

int main(int argc, char **argv)
{
	//variaveis de mediçao do tempo
	clock_t ini, fin;
	double t, tt=0;
	register unsigned int i;

	//declaraçao dos tipos de estruturas
	IMAGEM img;
    MATRICES matrizes;
	INFO results;		
 
	FILE *lena = fopen(*(argv + 1), "rb");//nome do arquivo passado na linha de comando
	FILE *pOut = fopen("Resultados.txt", "w");//arquivo que guardara as informacoes	

	if (!lena){
		puts("Arquivo inexistente!");
		exit(1);
	}	

	if (argc != 2){
		puts("Nome do arquivo nao foi passado na linha de comando!");
		exit(1);
	}

	//chamada da funçao que ler e armazena as informaçoes do cabeçalho na estrutura imagem
	ler_cabecalho(&img, lena);

	img.pdata = (unsigned char *)malloc((img.l) * (img.c) * sizeof(unsigned char));//alocando matriz pdata
	if (!img.pdata){
		puts("Sem memoria!");
		exit(1);
	}
    
	//chamada da funçao que armazenara os dados da imagem na matriz pdata
	cria_matriz(&img, lena);

	matrizes.M_quantization = (unsigned char *)malloc((img.l) * (img.c) * sizeof(unsigned char));//alocando matriz de quantizaçao	
	if (!matrizes.M_quantization){
		puts("Sem memoria!");
		exit(1);
	}

	matrizes.Co_Occurrence_Matrix = (unsigned int *)calloc((img.lum), sizeof(unsigned int));//alocando matriz de co-ocorrencia
	if (!matrizes.Co_Occurrence_Matrix){
		puts("Sem memoria!");
		exit(1);
	}

	matrizes.M_normalization = (float *)malloc((img.lum) * sizeof(float));//alocando matriz de normalizaçao
	if (!matrizes.M_normalization){
		puts("Sem memoria!");
		exit(1);
	}

	//mediçao do tempo	
	for (i = 0; i < TIMES_EXEC; i++){		
		ini=clock();
		
		quantizacao(&matrizes, &img);
		co_ocorrencia(&matrizes);
		normalizacao(&matrizes);
		contraste(&matrizes, &results);
		correlacao(&matrizes, &results);
		energia(&matrizes, &results);
		homogeneidade(&matrizes, &results);
		
		fin=clock();
		
		t = ((double)(fin-ini))/CLOCKS_PER_SEC;
		tt+=t;
	}		
	
	resultados(pOut, &results, tt);//chamada da funcao que escrevera os resultados do algoritimo em um arquivo

	//salvando arquivos
	fclose(lena);
	fclose(pOut);
	
	//desalocando memoria
	free(img.pdata);
	free(matrizes.M_quantization);
	free(matrizes.Co_Occurrence_Matrix);
	free(matrizes.M_normalization);
	return 0;	
}
