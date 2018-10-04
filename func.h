/*********************************************************************/
/** Aluno : Gabriel Santos Barbosa 								    **/
/** M a t r i c u l a : 2****045050156								**/
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

#define LINHAS 512
#define COLUNAS 512
#define TOTALPIXELS 262144
#define LUM 255
#define LC 16 //ordem das matrizes de quantizaçao, co - ocorrencia e normalizao; equivale a (sqrt(img->lum))
#define TIMES_EXEC 10000 //numero de execuçoes para mediçao do tempo de processamento

/*declaracao das estruturas*/
typedef struct{
	char codigo[3];
	unsigned int c, l, lum;
	unsigned char *pdata;		
}IMAGEM;

typedef struct{
    unsigned char *M_quantization;
	unsigned int *Co_Occurrence_Matrix;
	float *M_normalization;
}MATRICES;

typedef struct{
	float contraste, homogeneidade, correlacao, energia;
}INFO;

//prototipos
void ler_cabecalho(IMAGEM *img, FILE *lena);
void cria_matriz(IMAGEM *img, FILE *lena);
void quantizacao(MATRICES *m, IMAGEM *img);
void co_ocorrencia(MATRICES *m);
void normalizacao(MATRICES *m);
void contraste(MATRICES *m, INFO *r);
void energia(MATRICES *m, INFO *r);
void homogeneidade(MATRICES *m, INFO *r);
void resultados(FILE *resultado, INFO *informations, double a_t);
void correlacao(MATRICES *m, INFO *r);
