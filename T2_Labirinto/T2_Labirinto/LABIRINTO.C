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
*       1.10   aaf   03/10/2019 Término da implementação das funções do T2
*       1.10   aaf   28/10/2019 Início da implementação das funções do T3
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

static MTZ_tpDirecao ConverterDirecao( LAB_tpDirecao direcao );

static int RetirarEntradaOuSaida( LAB_tppLabirinto pLab, LAB_tpElemCasa tipoElem );

static void ImprimirCasa( LAB_tpElemCasa elemento );

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
	(*ppLab)->pMatriz = NULL;
	if (MTZ_CriarMatriz( &((*ppLab)->pMatriz), tam, ExcluirValor) == MTZ_CondRetFaltouMemoria ) {
		free(*ppLab);
		*ppLab = NULL;
		return LAB_CondRetFaltouMemoria;
	}

	// // Inicializa os valores segundo o padrão
	(*ppLab)->id = 0;
	(*ppLab)->posXEntrada = 0;
	(*ppLab)->posYEntrada = 0;
	(*ppLab)->posXSaida = tam - 1;
	(*ppLab)->posYSaida = tam - 1;
	(*ppLab)->posXCorrente = 0;
	(*ppLab)->posYCorrente = 0;

	// // Todos os espaços para os elementos são alocados nessa função

	// // Preenchimento inicial
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

			// Anda para direita
			MTZ_AndarDirecao( (*ppLab)->pMatriz,  MTZ_DirLeste) ;
		}
		// Volta para a primeira coluna
		while (MTZ_AndarDirecao( (*ppLab)->pMatriz,  MTZ_DirOeste) != MTZ_CondRetDirecaoNaoExisteOuInvalida);

		// Anda para baixo
		MTZ_AndarDirecao( (*ppLab)->pMatriz,  MTZ_DirSul) ;
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
  	
  	MTZ_tpDirecao dirMatriz;

	if (pLab == NULL) return LAB_CondRetLabirintoNaoExiste;
	if (direcao < 0 || direcao > 3) return LAB_CondRetDirecaoNaoExisteOuInvalida;

	// Traduzir a direção de labirinto para matriz
	dirMatriz = ConverterDirecao( direcao );

	// Verificar se não anda para fora do labirinto
	if (MTZ_AndarDirecao( pLab->pMatriz, dirMatriz ) == MTZ_CondRetDirecaoNaoExisteOuInvalida)
		return LAB_CondRetDirecaoNaoExisteOuInvalida;

	// Atualizar as coordenadas da posição corrente
	if (direcao == LAB_DirLeste || direcao == LAB_DirOeste)
		pLab->posXCorrente += (direcao == LAB_DirLeste) ? 1 : -1;
	else
		pLab->posYCorrente += (direcao == LAB_DirSul) ? 1 : -1;

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
	if (*pElementoPresente == elemento) return LAB_CondRetOK;

	// Se existe na posição corrente uma entrada ou saida, não poderá ser sobrescrita
	if (*pElementoPresente == LAB_ElemEntrada || *pElementoPresente == LAB_ElemSaida)
		return LAB_CondRetElementoInvalido;

	// Se o novo elemento é uma entrada ou saida, a antiga terá que ser substituida por um elemento vazio e voltar para a corrente
	if (elemento == LAB_ElemEntrada || elemento == LAB_ElemSaida) {

		if (RetirarEntradaOuSaida( pLab, elemento ) == 1)
			return LAB_CondRetErroEstrutura;
	}

	// Inserir o elemento na matriz
	*pElementoPresente = elemento;

	// Atualizar nova entrada ou saida
	if (elemento == LAB_ElemEntrada) {
		pLab->posXEntrada = pLab->posXCorrente;
		pLab->posYEntrada = pLab->posYCorrente;
	} else if (elemento == LAB_ElemSaida) {
		pLab->posXSaida = pLab->posXCorrente;
		pLab->posYSaida = pLab->posYCorrente;
	}

	return LAB_CondRetOK;

} /* Fim função: LAB Inserir elemento */

/***************************************************************************
*
*  Função: LAB Imprimir labirinto
*  ****/

LAB_tpCondRet LAB_ImprimirLabirinto( LAB_tppLabirinto pLab ) {

	LAB_tpElemCasa * pElementoCorrente = NULL;
	int contLinha = 0, contColuna = 0;

	if (pLab == NULL) return LAB_CondRetLabirintoNaoExiste;

	printf("\n\n");

	// Andar para o inicio sem alterar a marcação X e Y da posição corrente
	MTZ_VoltarParaPrimeiro(pLab->pMatriz);

	// Imprimir tudo
	do {

		do {
			MTZ_ObterValorCorrente(pLab->pMatriz, (void **) &pElementoCorrente);
			ImprimirCasa(*pElementoCorrente);
		} while (MTZ_AndarDirecao(pLab->pMatriz, MTZ_DirLeste) != MTZ_CondRetDirecaoNaoExisteOuInvalida);

		// Volta para o inicio da linha
		while (MTZ_AndarDirecao(pLab->pMatriz, MTZ_DirOeste) != MTZ_CondRetDirecaoNaoExisteOuInvalida);
		printf("\n");

	} while (MTZ_AndarDirecao(pLab->pMatriz, MTZ_DirSul) != MTZ_CondRetDirecaoNaoExisteOuInvalida);

	// Voltar para a casa corrente certa
	MTZ_VoltarParaPrimeiro(pLab->pMatriz);
	for (contLinha = 0; contLinha < pLab->posXCorrente; contLinha++)
		MTZ_AndarDirecao(pLab->pMatriz, MTZ_DirLeste);
	for (contColuna = 0; contColuna < pLab->posYCorrente; contColuna++)
		MTZ_AndarDirecao(pLab->pMatriz, MTZ_DirSul);

	printf("\n");

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
*  $FC Função: LAB Excluir valor
*
***********************************************************************/

void ExcluirValor( void * pValor ) {

    if (pValor != NULL)
        free( pValor ) ;

} /* Fim função: LAB Excluir valor */

/***********************************************************************
*
*  $FC Função: LAB Converter direção
*
***********************************************************************/

MTZ_tpDirecao ConverterDirecao( LAB_tpDirecao direcao ) {

	switch (direcao) {
    case LAB_DirNorte:
        return MTZ_DirNorte;
    case LAB_DirLeste:
        return MTZ_DirLeste;
    case LAB_DirSul:
        return MTZ_DirSul;
    default:
        return MTZ_DirOeste;
    }

} /* Fim função: LAB Converter direção */

/***********************************************************************
*
*  $FC Função: LAB Retirar entrada ou saída
*
*  $FV Valor retornado
*     0 - Caso tenha executado corretamente
*     1 - Caso tenha ocorrido problema ao executar
*
*  $EAE Assertivas de entradas esperadas
*     pLab != NULL
*     tipoElem é Entrada ou Saida
*
*  $EAE Assertivas de saída esperadas
*     A entrada ou saída terá sido transformada em elemento vazio (caso não haja erro)
*
***********************************************************************/

int RetirarEntradaOuSaida( LAB_tppLabirinto pLab, LAB_tpElemCasa tipoElem ) {

	MTZ_tpDirecao direcaoX, direcaoY;
	int i = 0, qtdIteracoesX, qtdIteracoesY, retorno = 0, posXRetirar, posYRetirar;
	LAB_tpElemCasa * pElementoPresente;

	// Tratar se procura entrada ou saída
	if (tipoElem == LAB_ElemEntrada) {
		posXRetirar = pLab->posXEntrada;
		posYRetirar = pLab->posYEntrada;
	} else {
		posXRetirar = pLab->posXSaida;
		posYRetirar = pLab->posYSaida;
	}

	// Se o elemento estiver à esquerda da corrente, anda para oeste, senão anda para leste
	direcaoX = (posXRetirar < pLab->posXCorrente) ? MTZ_DirOeste : MTZ_DirLeste;
	qtdIteracoesX = posXRetirar - pLab->posXCorrente;
	if (qtdIteracoesX < 0) qtdIteracoesX = -qtdIteracoesX;

	// Se o elemento estiver acima da corrente, anda para norte, senão anda para sul
	direcaoY = (posYRetirar < pLab->posYCorrente) ? MTZ_DirNorte : MTZ_DirSul;
	qtdIteracoesY = posYRetirar - pLab->posYCorrente;
	if (qtdIteracoesY < 0) qtdIteracoesY = -qtdIteracoesY;

	// Anda até o elemento
	for (i = 0; i < qtdIteracoesX; i++)
		MTZ_AndarDirecao( pLab->pMatriz, direcaoX );

	for (i = 0; i < qtdIteracoesY; i++)
		MTZ_AndarDirecao( pLab->pMatriz, direcaoY );

	// Verifica se o elemento é o procurado (senão da erro, retorna 1)
	MTZ_ObterValorCorrente( pLab->pMatriz, (void **) &pElementoPresente ) ;
	if (*pElementoPresente != tipoElem)
		retorno = 1;

	// Troca o elemento para casa vazia se nao tiver dado erro
	if (retorno == 0)
		*pElementoPresente = LAB_ElemVazio;

	// Volta para a casa corrente fazendo o passo oposto da ida até a entrada
	direcaoX = (direcaoX == MTZ_DirOeste) ? MTZ_DirLeste : MTZ_DirOeste;
	direcaoY = (direcaoY == MTZ_DirNorte) ? MTZ_DirSul : MTZ_DirNorte;

	for (i = 0; i < qtdIteracoesX; i++)
		MTZ_AndarDirecao( pLab->pMatriz, direcaoX );

	for (i = 0; i < qtdIteracoesY; i++)
		MTZ_AndarDirecao( pLab->pMatriz, direcaoY );

	// Se executou certo, retorna 0
	return retorno;
} /* Fim função: LAB Retirar entrada ou saída */

/***********************************************************************
*
*  $FC Função: LAB Imprimir Casa
*
***********************************************************************/

void ImprimirCasa( LAB_tpElemCasa elemento ) {

	char c;

	switch (elemento) {
    case LAB_ElemParede:
    	c = 254;
        break;
    case LAB_ElemEntrada:
    	c = 'E';
        break;
    case LAB_ElemSaida:
        c = 'S';
        break;
    default:
    	c = 176;
        break;
    }

    printf("%c", c);
    return;

} /* Fim função: LAB Imprimir Casa */

/********** Fim do módulo de implementação: Módulo labirinto **********/
