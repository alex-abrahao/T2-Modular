/***************************************************************************
*  $MCI Módulo de implementação: Módulo labirinto
*
*  Arquivo gerado:              LABIRINTO.C
*  Letras identificadoras:      LAB
*
*  Nome da base de software:    Exemplo de teste automatizado
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\SIMPLES.BSW
*
*  Projeto: Disciplinas INF 1628 / 1301
*  Gestor:  DI/PUC-Rio
*  Autores: aaf - Alexandre Abrahão Ferreira
*           jmb - João Marcello Bessa
*           phd - Pedro Henrique Dias
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*       1.00   phd   26/09/2019 Início do desenvolvimento
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
*  $TC Tipo de dados: LAB Descritor da cabeça de um labirinto
*
*
*  $ED Descrição do tipo
*     A cabeça do labirinto é o ponto de acesso para um determinado labirinto.
*	  Contém a referência para uma matriz que serve como base para andar pelo labirinto.
*
***********************************************************************/

typedef struct tgLabirinto {
   
	MTZ_tppMatriz pMatriz ;
        	/* Ponteiro para a matriz base para o labirinto */

	char id;
        	/* Caracter identificador do labirinto (opcional) */

	int posXEntrada;
        	/* Posição X da entrada do labirinto */

	int posYEntrada;
        	/* Posição Y da entrada do labirinto */

	int posXSaida;
        	/* Posição X da saída do labirinto */

	int posYSaida;
        	/* Posição Y da saída do labirinto */

	int posXCorrente;
        	/* Posição X corrente do labirinto */

	int posYCorrente;
        	/* Posição Y corrente do labirinto */

} tpLabirinto ;

/*****  Dados encapsulados no módulo  *****/

/***** Protótipos das funções encapuladas no módulo *****/

static void ExcluirValor( void * pValor );

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: LAB Criar labirinto
*  ****/

LAB_tpCondRet LAB_CriarLabirinto( LAB_tppLabirinto * ppLab, int tam ) {

	LAB_tpElemCasa * pElementoAux;
	int linhaAtual, colunaAtual;

	if (ppLab == NULL || tam <= 1) return LAB_CondRetErroEstrutura;
	if (*ppLab != NULL) LAB_DestruirLabirinto( ppLab );

	// Alocar espaço
	*ppLab = (LAB_tppLabirinto) malloc(sizeof(tpLabirinto));

	if (*ppLab == NULL) return LAB_CondRetFaltouMemoria;

	// Alocar matriz
	if (MTZ_CriarMatriz( &((*ppLab)->pMatriz), tam, ExcluirValor) == MTZ_CondRetFaltouMemoria ) {
		free(*ppLab);
		*ppLab = NULL;
		return LAB_CondRetFaltouMemoria;
	}

	// Inicializa os valores segundo o padrão
	(*ppLab)->id = 0;
	(*ppLab)->posXEntrada = 0;
	(*ppLab)->posYEntrada = 0;
	(*ppLab)->posXSaida = tam - 1;
	(*ppLab)->posYSaida = tam - 1;
	(*ppLab)->posXCorrente = 0;
	(*ppLab)->posYCorrente = 0;

	// Todos os espaços para os elementos são alocados nessa função

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

			// Preencher a saída em (tam - 1, tam - 1)
			else if (colunaAtual == tam - 1 && linhaAtual == tam - 1)
				*pElementoAux = LAB_ElemSaida;
			// Preencher as demais casas com espaços vazios
			else
				*pElementoAux = LAB_ElemVazio;

			// Insere o elemento na matriz
			MTZ_InserirElementoNaCasaCorrente( (*ppLab)->pMatriz, pElementoAux ) ;
		}
	}
	
	// Retornar o ponteiro da matriz para o início
	MTZ_VoltarParaPrimeiro( (*ppLab)->pMatriz );

	return LAB_CondRetOK;

} /* Fim função: LAB Criar labirinto */

/***************************************************************************
*
*  Função: LAB Destruir labirinto
*  ****/

LAB_tpCondRet LAB_DestruirLabirinto( LAB_tppLabirinto * ppLab ) {

	if (ppLab == NULL || *ppLab == NULL) return LAB_CondRetLabirintoNaoExiste;

	MTZ_DestruirMatriz(&((*ppLab)->pMatriz));

	free(*ppLab);
	*ppLab = NULL;

	return LAB_CondRetOK;

} /* Fim função: LAB Destruir labirinto */

/***************************************************************************
*
*  Função: LAB Andar em Direção
*  ****/

LAB_tpCondRet LAB_AndarDirecao( LAB_tppLabirinto pLab, LAB_tpDirecao direcao ) {
  
	if (pLab == NULL) return LAB_CondRetLabirintoNaoExiste;

	// WIP: Implementar

	return LAB_CondRetOK;

} /* Fim função: LAB Andar em Direção */

/***************************************************************************
*
*  Função: LAB Inserir elemento
*  ****/

LAB_tpCondRet LAB_InserirElemento( LAB_tppLabirinto pLab, LAB_tpElemCasa elemento ) {

	LAB_tpElemCasa * pElementoPresente;

	if (pLab == NULL) return LAB_CondRetLabirintoNaoExiste;

	// Verificar se o elemento é válido
	if (elemento > 3) return LAB_CondRetElementoInvalido;

	// Pegar o ponteiro para o elemento da posição corrente
	MTZ_ObterValorCorrente( pLab->pMatriz, (void **) &pElementoPresente ) ;

	// Se era uma entrada ou saida, ela terá que ser substituida por um elemento vazio
	// WIP: Implementar

	// Inserir o elemento na matriz
	*pElementoPresente = elemento;

	return LAB_CondRetOK;

} /* Fim função: LAB Inserir elemento */

/***************************************************************************
*
*  Função: LAB Imprimir labirinto
*  ****/

LAB_tpCondRet LAB_ImprimirLabirinto( LAB_tppLabirinto pLab ) {

	if (pLab == NULL) return LAB_CondRetLabirintoNaoExiste;

	// WIP: Implementar

	return LAB_CondRetOK;

} /* Fim função: LAB Imprimir labirinto */

/***************************************************************************
*
*  Função: LAB Existe solução
*  ****/

int LAB_ExisteSolucao( LAB_tppLabirinto pLab ) {

	if (pLab == NULL) return 0;

	// WIP: Implementar

	return 0;

} /* Fim função: LAB Existe solução */


/*****  Código das funções encapsuladas no módulo  *****/

/***********************************************************************
*
*  $FC Função: TMTZ Excluir valor
*
***********************************************************************/

void ExcluirValor( void * pValor ) {

	// WIP: Implementar para excluir o que tiver dentro da matriz
    if (pValor != NULL)
        free( pValor ) ;

} /* Fim função: TMTZ Excluir caracter */

/********** Fim do módulo de implementação: Módulo labirinto **********/
