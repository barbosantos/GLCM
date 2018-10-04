/*********************************************************************/
/** Aluno : Gabriel Santos Barbosa 								    **/
/** M a t r i c u l a : 20142045050156								**/
/** 																**/
/** Aluno : Willian Bastos Dias										**/
/** M a t r i c u l a : 20142045050270								**/
/**                                                                 **/
/** Trabalho Final	- Laboratorio de programaçao				   	**/
/** 					   											**/
/** Prof. Daniel Ferreira                                           **/
/** Versão do compilador: gcc (Ubuntu 4.8.4-2ubuntu1~14.04.1) 4.8.4 **/
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "func.h"

/*FUNCOES*/
void ler_cabecalho(IMAGEM *img, FILE *lena)//função para ler e armazenar as informaçoes do cabeçalho nos membros da estrutura
{		
	fscanf(lena, "%s %d %d %d", img->codigo, &img->l, &img->c, &img->lum );
	img->codigo[2] = '\0';
	//printf("%s %d %d %d\n\n", img->codigo, img->l, img->c, img->lum );
}

void cria_matriz(IMAGEM *img, FILE *lena)//funçao para ler e armazenar os pixels da imagem em uma matriz (pdata)
{   
	register unsigned int i;
    
    fseek(lena, 1, SEEK_CUR);
	for (i = 0; i < TOTALPIXELS; i++)
		fread((img->pdata) + i, sizeof(unsigned char), 1, lena);	
}

void quantizacao(MATRICES *m, IMAGEM *img)//quantizacao da matriz
{
	register unsigned int i;

	for (i = 0; i < TOTALPIXELS; i++)
		*(m->M_quantization + i) = *(img->pdata + i)/LC;
}

void co_ocorrencia(MATRICES *m)//calculo da co-ocorrencia
{
	register unsigned int i;
			
	for (i = 0; i < TOTALPIXELS; i++){
		if ((i + 1) % LINHAS != 0)
			*(m->Co_Occurrence_Matrix + ((*(m->M_quantization + i) * LC) + (*(m->M_quantization + i + 1) ))) += 1;
	}
}

void normalizacao(MATRICES *m)//normalizaçao da matriz de co-ocorrencia
{
	unsigned int somatorio = 0;
	register unsigned short int i;

	for (i = 0; i < LUM ; i++)
		somatorio += *(m->Co_Occurrence_Matrix + i);
	
	for (i = 0; i < LUM ; i++)
		*(m->M_normalization + i) = (float)*(m->Co_Occurrence_Matrix + i)/(float)somatorio;	
}

void contraste(MATRICES *m, INFO *r)//calculo do contraste
{
	float contrast = 0;
	register unsigned short int i;

	for (i = 0; i < LUM ; i++)
		contrast += (*(m->M_normalization + i) * pow(((i/LC) - (i%LC)), 2));

	//printf("\nlum: %d", LUM );
	r->contraste = contrast;//salvando informaçoes nos membros da estrutura INFO
}

void energia(MATRICES *m, INFO *r)//calculo da energia
{
	float energy = 0;
	register unsigned short int i;
	
	for (i = 0; i < LUM ; i++)
		energy += pow(*(m->M_normalization + i), 2);

	//printf("Energy: %.4f\n", energy);
	r->energia = energy;//salvando informaçoes nos membros da estrutura INFO
}

void homogeneidade(MATRICES *m, INFO *r)//calculo da homogeneidade
{
	float homogeneity = 0;
	register unsigned short int i;

	for (i = 0; i < LUM ; i++)
		homogeneity += (*(m->M_normalization + i)/(float)(1 + abs((i/LC) - (i%LC))));

	//printf("Homogeneity: %.4f\n", homogeneity);
	r->homogeneidade = homogeneity;//salvando informaçoes nos membros da estrutura INFO
}

void correlacao(MATRICES *m, INFO *r)//calculo da correlacao
{
	float correlation = 0, mi_i = 0, mi_j = 0, r_i = 0, r_j = 0;
	register unsigned short int i;

	for (i = 0; i < LUM ; i++){
		mi_i += ((i / LC) * *(m->M_normalization + i));
		mi_j += ((i % LC) * *(m->M_normalization + i));
	}
	
	for (i = 0; i < LUM ; i++){
		r_i += *(m->M_normalization + i) * pow(((i / LC) - mi_i), 2);
		r_j += *(m->M_normalization + i) * pow(((i % LC) - mi_j), 2);
	}
	
	//tirando a raiz quadrada dos elementos r_i e r_j para aplicar na formula de correlacao
	r_i = sqrt(r_i);
	r_j = sqrt(r_j);
	
	for (i = 0; i < LUM ; i++)//formula de correlacao
		correlation += ((((float)(i / LC)) - mi_i) * (((float)(i % LC)) - mi_j) * *(m->M_normalization + i)) / (r_i * r_j);

	r->correlacao = correlation;//salvando informaçoes nos membros da estrutura INFO
	//printf("Correlation: %.4f\n", correlation);			
}

void resultados(FILE *pOut, INFO *informations, double a_t)//funcao que escreve os resultados do algoritmo em um arquivo
{
	fprintf(pOut, "%s\n", "Equipe \n\nGabriel Santos Barbosa\t Matrícula: 20**204**50*56 \nWillian Bastos Dias\t Matrícula: 2014**450**270\n\n ");
	fprintf(pOut, "%s%s %.4f\n", "Atributos\n\n", "Contrast: ", informations->contraste);
	fprintf(pOut, "%s %.4f\n", "Correlation: ", informations->correlacao);
	fprintf(pOut, "%s %.4f\n", "Energy: ", informations->energia);
	fprintf(pOut, "%s %.4f\n", "Homogeneity: ", informations->homogeneidade);
	fprintf(pOut, "\n%s\t%f", "Tempo médio de execução:", a_t/TIMES_EXEC);
}
