/***************************************************************************
*  $MCI M�dulo de implementa��o: M�dulo labirinto
*
*  Arquivo gerado:              LABIRINTO.C
*  Letras identificadoras:      LAB
*
*  Nome da base de software:    Exemplo de teste automatizado
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\SIMPLES.BSW
*
*  Projeto: Disciplinas INF 1628 / 1301
*  Gestor:  DI/PUC-Rio
*  Autores: aaf - Alexandre Abrah�o Ferreira
*           jmb - Jo�o Marcello Bessa
*           phd - Pedro Henrique Dias
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*       1.00   phd   26/09/2019 In�cio do desenvolvimento
*
***************************************************************************/

#include   <malloc.h>
#include   <stdio.h>

#include "MATRIZ.H"

#define LABIRINTO_OWN
#include "LABIRINTO.H"
#undef LABIRINTO_OWN


/***********************************************************************
*
*  $TC Tipo de dados: LAB Descritor da cabe�a de um labirinto
*
*
*  $ED Descri��o do tipo
*     A cabe�a do labirinto � o ponto de acesso para um determinado labirinto.
*	  Cont�m a refer�ncia para uma matriz que serve como base para andar pelo labirinto.
*
***********************************************************************/

typedef struct tgLabirinto {
   
	MTZ_tppMatriz pMatriz ;
        	/* Ponteiro para a matriz base para o labirinto */

	char id;
        	/* Caracter identificador do labirinto (opcional) */

	int posXEntrada;
        	/* Posi��o X da entrada do labirinto */

	int posYEntrada;
        	/* Posi��o Y da entrada do labirinto */

	int posXSaida;
        	/* Posi��o X da sa�da do labirinto */

	int posYSaida;
        	/* Posi��o Y da sa�da do labirinto */

	int posXCorrente;
        	/* Posi��o X corrente do labirinto */

	int posYCorrente;
        	/* Posi��o Y corrente do labirinto */

} tpLabirinto ;

/*****  Dados encapsulados no m�dulo  *****/

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

static void ExcluirValor( void * pValor );

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: LAB Criar labirinto
*  ****/

LAB_tpCondRet LAB_CriarLabirinto( LAB_tppLabirinto * ppLab, int tam ) {

	LAB_tpElemCasa * pElementoAux;
	int linhaAtual, colunaAtual;

	if (ppLab == NULL || tam <= 1) return LAB_CondRetErroEstrutura;
	if (*ppLab ! NULL) LAB_DestruirLabirinto( LAB_tppLabirinto * ppLab );

	// Alocar espa�o
	*ppLab = (LAB_tppLabirinto) malloc(sizeof(tpLabirinto));

	if (*ppLab == NULL) return LAB_CondRetFaltouMemoria;

	// Alocar matriz
	if (MTZ_CriarMatriz( &((*ppLab)->pMatriz), tam, ExcluirValor) == MTZ_CondRetFaltouMemoria ) {
		free(*ppLab);
		*ppLab = NULL;
		return LAB_CondRetFaltouMemoria;
	}

	// Inicializa os valores segundo o padr�o
	(*ppLab)->id = 0;
	(*ppLab)->posXEntrada = 0;
	(*ppLab)->posYEntrada = 0;
	(*ppLab)->posXSaida = tam - 1;
	(*ppLab)->posYSaida = tam - 1;
	(*ppLab)->posXCorrente = 0;
	(*ppLab)->posYCorrente = 0;

	// Todos os espa�os para os elementos s�o alocados nessa fun��o

	// Preenchimento inicial
	for (linhaAtual = 0; linhaAtual < tam; linhaAtual++) {

		for (colunaAtual = 0; colunaAtual < tam; colunaAtual++) {
			
			pElementoAux = (LAB_tpElemCasa *) malloc(sizeof(LAB_tpElemCasa));
			if (pElementoAux == NULL) {
				LAB_DestruirLabirinto( ppLab );
				return LAB_CondRetFaltouMemoria;
			}
			// Preencher a casa (0, 0) com a entrada
			if (colunaAtual == 0 && linhaAtual == 0)
				*pElementoAux = LAB_ElemEntrada;

			// Preencher a sa�da em (tam - 1, tam - 1)
			else if (colunaAtual == tam - 1 && linhaAtual == tam - 1)
				*pElementoAux = LAB_ElemSaida;
			// Preencher as demais casas com espa�os vazios
			else
				*pElementoAux = LAB_ElemVazio;

			// Insere o elemento na matriz
			MTZ_InserirElementoNaCasaCorrente( (*ppLab)->pMatriz, pElementoAux ) ;
		}
	}
	
	// Retornar o ponteiro da matriz para o in�cio
	MTZ_VoltarParaPrimeiro( (*ppLab)->pMatriz );

	return LAB_CondRetOK;

} /* Fim fun��o: LAB Criar labirinto */

/***************************************************************************
*
*  Fun��o: LAB Destruir labirinto
*  ****/

LAB_tpCondRet LAB_DestruirLabirinto( LAB_tppLabirinto * ppLab ) {

	if (ppLab == NULL || *ppLab == NULL) return LAB_CondRetLabirintoNaoExiste;

	MTZ_DestruirMatriz(&((*ppLab)->pMatriz));

	free(*ppLab);
	*ppLab = NULL;

	return LAB_CondRetOK;

} /* Fim fun��o: LAB Destruir labirinto */

/***************************************************************************
*
*  Fun��o: LAB Andar em Dire��o
*  ****/

LAB_tpCondRet LAB_AndarDirecao( LAB_tppLabirinto pLab, LAB_tpDirecao direcao ) {
  
	if (pLab == NULL) return LAB_CondRetLabirintoNaoExiste;

	// WIP: Implementar

	return LAB_CondRetOK;

} /* Fim fun��o: LAB Andar em Dire��o */

/***************************************************************************
*
*  Fun��o: LAB Inserir elemento
*  ****/

LAB_tpCondRet LAB_InserirElemento( LAB_tppLabirinto pLab, LAB_tpElemCasa elemento ) {

	LAB_tpElemCasa * pElementoPresente;

	if (pLab == NULL) return LAB_CondRetLabirintoNaoExiste;

	// Verificar se o elemento � v�lido
	if (elemento > 3) return LAB_CondRetElementoInvalido;

	// Pegar o ponteiro para o elemento da posi��o corrente
	MTZ_ObterValorCorrente( pLab->pMatri, &pElementoPresente ) ;

	// Se era uma entrada ou saida, ela ter� que ser substituida por um elemento vazio
	// WIP: Implementar

	// Inserir o elemento na matriz
	*pElementoPresente = elemento;

	return LAB_CondRetOK;

} /* Fim fun��o: LAB Inserir elemento */

/***************************************************************************
*
*  Fun��o: LAB Imprimir labirinto
*  ****/

LAB_tpCondRet LAB_ImprimirLabirinto( LAB_tppLabirinto pLab ) {

	if (pLab == NULL) return LAB_CondRetLabirintoNaoExiste;

	// WIP: Implementar

	return LAB_CondRetOK;

} /* Fim fun��o: LAB Imprimir labirinto */

/***************************************************************************
*
*  Fun��o: LAB Existe solu��o
*  ****/

int LAB_ExisteSolucao( LAB_tppLabirinto pLab ) {

	if (pLab == NULL) return 0;

	// WIP: Implementar

	return 0;

} /* Fim fun��o: LAB Existe solu��o */


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/

/***********************************************************************
*
*  $FC Fun��o: TMTZ Excluir valor
*
***********************************************************************/

void ExcluirValor( void * pValor ) {

	// WIP: Implementar para excluir o que tiver dentro da matriz
    if (pValor != NULL)
        free( pValor ) ;

} /* Fim fun��o: TMTZ Excluir caracter */

/********** Fim do m�dulo de implementa��o: M�dulo labirinto **********/