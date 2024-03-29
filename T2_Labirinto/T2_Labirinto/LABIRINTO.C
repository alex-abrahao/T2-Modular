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
*       1.10   aaf   03/10/2019 T�rmino da implementa��o das fun��es do T2
*       1.20   aaf   28/10/2019 In�cio da implementa��o das fun��es do T3
*       1.21   aaf   04/11/2019 Implementado o desvendador do labirinto
*
***************************************************************************/

#include   <malloc.h>
#include   <stdio.h>
#include   <limits.h>

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

	int tam;
        	/* Tamanho do labirinto */

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

/***********************************************************************
*
*  $TC Tipo de dados: LAB Descritor do dado armazenado na posi��o do labirinto
*
*
*  $ED Descri��o do tipo
*     Cont�m as informa��es necess�rias para definir as posi��es do labirinto 
*	  e os dados para o algoritmo de resolu��o.
*
***********************************************************************/

typedef struct tgConteudoPosicao {
   
	LAB_tpElemCasa elemento ;
        	/* Ponteiro para a matriz base para o labirinto */

	int numPassagens;
        	/* N�mero de vezes que o algoritmo de resolu��o passou pela posi��o */

	LAB_tpDirecao direcaoVolta;
        	/* Dire��o para seguir na volta do algoritmo de resolu��o.
        	   Caso seja -1, n�o � uma casa que passou pelo algoritmo. */

} tpConteudoPosicao ;

/*****  Dados encapsulados no m�dulo  *****/

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

static void ExcluirValor( void * pValor );

static MTZ_tpDirecao ConverterDirecao( LAB_tpDirecao direcao );

static void IrEntradaOuSaida( LAB_tppLabirinto pLab, LAB_tpElemCasa tipoElem );

static int RetirarEntradaOuSaida( LAB_tppLabirinto pLab, LAB_tpElemCasa tipoElem );

static void ImprimirTudo( LAB_tppLabirinto pLab, int solucao );

static void ImprimirCasa( LAB_tpElemCasa elemento, int corrente, int solucao, LAB_tpDirecao direcao );

static LAB_tpDirecao MenorDirecao( MTZ_tppMatriz pMtz );

static void VerificaDirecao( MTZ_tppMatriz pMtz, MTZ_tpDirecao direcao, int * adjacentes );

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: LAB Criar labirinto
*  ****/

LAB_tpCondRet LAB_CriarLabirinto( LAB_tppLabirinto * ppLab, int tam ) {

	tpConteudoPosicao * pConteudoAux;
	int linhaAtual, colunaAtual;

	if (ppLab == NULL || tam <= 1) return LAB_CondRetErroEstrutura;
	if (*ppLab != NULL) LAB_DestruirLabirinto( ppLab );

	// Alocar espa�o
	*ppLab = (LAB_tppLabirinto) malloc(sizeof(tpLabirinto));

	if (*ppLab == NULL) return LAB_CondRetFaltouMemoria;

	// Alocar matriz
	(*ppLab)->pMatriz = NULL;
	if (MTZ_CriarMatriz( &((*ppLab)->pMatriz), tam, ExcluirValor) == MTZ_CondRetFaltouMemoria ) {
		free(*ppLab);
		*ppLab = NULL;
		return LAB_CondRetFaltouMemoria;
	}

	// Inicializa os valores segundo o padr�o
	(*ppLab)->id = 0;
	(*ppLab)->tam = tam;
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
			
			pConteudoAux = (tpConteudoPosicao *) malloc(sizeof(tpConteudoPosicao));
			if (pConteudoAux == NULL) {
				LAB_DestruirLabirinto( ppLab );
				return LAB_CondRetFaltouMemoria;
			}

			// Preenche os campos padr�es
			pConteudoAux->numPassagens = 0;
			pConteudoAux->direcaoVolta = -1;
			// Preencher a casa (0, 0) com a entrada
			if (colunaAtual == 0 && linhaAtual == 0)
				pConteudoAux->elemento = LAB_ElemEntrada;

			// Preencher a sa�da em (tam - 1, tam - 1)
			else if (colunaAtual == tam - 1 && linhaAtual == tam - 1)
				pConteudoAux->elemento = LAB_ElemSaida;
			// Preencher as demais casas com espa�os vazios
			else
				pConteudoAux->elemento = LAB_ElemVazio;

			// Insere o conteudo na matriz
			MTZ_InserirElementoNaCasaCorrente( (*ppLab)->pMatriz, pConteudoAux ) ;

			// Anda para direita
			MTZ_AndarDirecao( (*ppLab)->pMatriz, MTZ_DirLeste) ;
		}
		// Volta para a primeira coluna
		while (MTZ_AndarDirecao( (*ppLab)->pMatriz, MTZ_DirOeste) != MTZ_CondRetDirecaoNaoExisteOuInvalida);

		// Anda para baixo
		MTZ_AndarDirecao( (*ppLab)->pMatriz, MTZ_DirSul) ;
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
  	
  	MTZ_tpDirecao dirMatriz;

	if (pLab == NULL) return LAB_CondRetLabirintoNaoExiste;
	if (direcao < 0 || direcao > 3) return LAB_CondRetDirecaoNaoExisteOuInvalida;

	// Traduzir a dire��o de labirinto para matriz
	dirMatriz = ConverterDirecao( direcao );

	// Verificar se n�o anda para fora do labirinto
	if (MTZ_AndarDirecao( pLab->pMatriz, dirMatriz ) == MTZ_CondRetDirecaoNaoExisteOuInvalida)
		return LAB_CondRetDirecaoNaoExisteOuInvalida;

	// Atualizar as coordenadas da posi��o corrente
	if (direcao == LAB_DirLeste || direcao == LAB_DirOeste)
		pLab->posXCorrente += (direcao == LAB_DirLeste) ? 1 : -1;
	else
		pLab->posYCorrente += (direcao == LAB_DirSul) ? 1 : -1;

	return LAB_CondRetOK;

} /* Fim fun��o: LAB Andar em Dire��o */

/***************************************************************************
*
*  Fun��o: LAB Inserir elemento
*  ****/

LAB_tpCondRet LAB_InserirElemento( LAB_tppLabirinto pLab, LAB_tpElemCasa elemento ) {

	tpConteudoPosicao * pConteudoPresente;

	if (pLab == NULL) return LAB_CondRetLabirintoNaoExiste;

	// Verificar se o elemento � v�lido
	if (elemento > 3) return LAB_CondRetElementoInvalido;

	// Pegar o ponteiro para o elemento da posi��o corrente
	MTZ_ObterValorCorrente(pLab->pMatriz, (void **) &pConteudoPresente);
	if (pConteudoPresente->elemento == elemento) return LAB_CondRetOK;

	// Se existe na posi��o corrente uma entrada ou saida, n�o poder� ser sobrescrita
	if (pConteudoPresente->elemento == LAB_ElemEntrada || pConteudoPresente->elemento == LAB_ElemSaida)
		return LAB_CondRetElementoInvalido;

	// Se o novo elemento � uma entrada ou saida, a antiga ter� que ser substituida por um elemento vazio e voltar para a corrente
	if (elemento == LAB_ElemEntrada || elemento == LAB_ElemSaida) {

		if (RetirarEntradaOuSaida( pLab, elemento ) == 1)
			return LAB_CondRetErroEstrutura;
	}

	// Inserir o elemento na matriz
	pConteudoPresente->elemento = elemento;

	// Atualizar nova entrada ou saida
	if (elemento == LAB_ElemEntrada) {
		pLab->posXEntrada = pLab->posXCorrente;
		pLab->posYEntrada = pLab->posYCorrente;
	} else if (elemento == LAB_ElemSaida) {
		pLab->posXSaida = pLab->posXCorrente;
		pLab->posYSaida = pLab->posYCorrente;
	}

	return LAB_CondRetOK;

} /* Fim fun��o: LAB Inserir elemento */

/***************************************************************************
*
*  Fun��o: LAB Imprimir labirinto
*  ****/

LAB_tpCondRet LAB_ImprimirLabirinto( LAB_tppLabirinto pLab ) {

	if (pLab == NULL) return LAB_CondRetLabirintoNaoExiste;

	ImprimirTudo(pLab, 0);

	return LAB_CondRetOK;

} /* Fim fun��o: LAB Imprimir labirinto */

/***************************************************************************
*
*  Fun��o: LAB Exibe solu��o
*  ****/

LAB_tpCondRet LAB_ExibeSolucao( LAB_tppLabirinto pLab ) {

	int numIteracoes = 0, limiteIteracoes;
	int contLinha = 0, contColuna = 0;
	// Dire��o = -1 significa dire��o inv�lida
	LAB_tpDirecao ultimaDirecao = -1, direcaoAux;
	tpConteudoPosicao * pConteudoAux = NULL;

	if (pLab == NULL) return LAB_CondRetLabirintoNaoExiste;

	// Defini��o do limite de itera��es como tam^4
	limiteIteracoes = pLab->tam * pLab->tam * pLab->tam * pLab->tam;

	// Limpar as casas primeiro (colocar -1 na direcao de volta e 0 nas passagens, caso o lab tenha sido solucionado previamente e o user alterou depois)
	MTZ_VoltarParaPrimeiro(pLab->pMatriz);
	do {
		contLinha = 0;
		do {
			MTZ_ObterValorCorrente(pLab->pMatriz, (void **) &pConteudoAux);
			pConteudoAux->direcaoVolta = -1;
			pConteudoAux->numPassagens = 0;
			contLinha++;
		} while (MTZ_AndarDirecao(pLab->pMatriz, MTZ_DirLeste) != MTZ_CondRetDirecaoNaoExisteOuInvalida);

		// Volta para o inicio da linha
		while (MTZ_AndarDirecao(pLab->pMatriz, MTZ_DirOeste) != MTZ_CondRetDirecaoNaoExisteOuInvalida);
		contColuna++;

	} while (MTZ_AndarDirecao(pLab->pMatriz, MTZ_DirSul) != MTZ_CondRetDirecaoNaoExisteOuInvalida);
	MTZ_VoltarParaPrimeiro(pLab->pMatriz);
	pLab->posXCorrente = 0;
	pLab->posYCorrente = 0;

	// Andar at� a entrada
	IrEntradaOuSaida( pLab, LAB_ElemEntrada );
	pLab->posXCorrente = pLab->posXEntrada;
	pLab->posYCorrente = pLab->posYEntrada;

	// Para cada itera��o, at� o limite de execu��es
	while (numIteracoes < limiteIteracoes) {

		// Anda para as posi��es adjacentes e volta, armazenando cada um dos valores
		// Exceto para posi��es j� visitadas ou inv�lidas
		ultimaDirecao = MenorDirecao(pLab->pMatriz);
		// Caso todas as dire��es sejam inv�lidas, sai do loop e volta para a entrada
		while (ultimaDirecao != -1) {

			// Anda para a casa determinada
			LAB_AndarDirecao( pLab, ultimaDirecao );
			MTZ_ObterValorCorrente( pLab->pMatriz, (void **) &pConteudoAux );
			
			// Se a casa for a sa�da, encontrou o caminho
			if (pConteudoAux->elemento == LAB_ElemSaida) {
				// Coloca o labirinto no (0, 0)
				MTZ_VoltarParaPrimeiro(pLab->pMatriz);
				pLab->posXCorrente = 0;
				pLab->posYCorrente = 0;
				// Exibe na tela a solu��o
				ImprimirTudo(pLab, 1);
				printf("Numero total de tentativas ate encontrar: %d\n", numIteracoes + 1);
				// Retorna OK
				return LAB_CondRetOK;
			}

			// Sen�o, anda para a dire��o com menor n�mero armazenado
			// e coloca nessa casa a dire��o para voltar no fim dessa passada
			switch (ultimaDirecao) {
		    case LAB_DirNorte:
		        pConteudoAux->direcaoVolta = LAB_DirSul;
				break;
		    case LAB_DirLeste:
		        pConteudoAux->direcaoVolta = LAB_DirOeste;
				break;
		    case LAB_DirSul:
		        pConteudoAux->direcaoVolta = LAB_DirNorte;
				break;
		    default:
		        pConteudoAux->direcaoVolta = LAB_DirLeste;
				break;
		    }

			ultimaDirecao = MenorDirecao(pLab->pMatriz);
		}

		MTZ_ObterValorCorrente( pLab->pMatriz, (void **) &pConteudoAux );
		while (pConteudoAux->elemento != LAB_ElemEntrada) {
			// Volta incrementando 1 no n�mero de passagens
			pConteudoAux->numPassagens += 1;
			// e removendo a dire��o de volta
			direcaoAux = pConteudoAux->direcaoVolta;
			pConteudoAux->direcaoVolta = -1;
			LAB_AndarDirecao( pLab, direcaoAux );
			MTZ_ObterValorCorrente( pLab->pMatriz, (void **) &pConteudoAux );
		}

		// Incrementa o contador
		numIteracoes++;
	}

	// Coloca o labirinto no (0, 0)
	MTZ_VoltarParaPrimeiro(pLab->pMatriz);
	pLab->posXCorrente = 0;
	pLab->posYCorrente = 0;

	// Se chegou at� aqui, n�o achou solu��o
	printf("\nNumero total de tentativas sem solucao: %d\n", numIteracoes);
	return LAB_CondRetNaoTemSolucao;

} /* Fim fun��o: LAB Exibe solu��o */

/***************************************************************************
*
*  Fun��o: LAB Receber coordenadas
*  ****/

LAB_tpCondRet LAB_ReceberCoordenadas( LAB_tppLabirinto pLab, int * x, int * y ) {

	if (pLab == NULL) return LAB_CondRetLabirintoNaoExiste;

	*x = pLab->posXCorrente;
	*y = pLab->posYCorrente;

	return LAB_CondRetOK;

} /* Fim fun��o: LAB Receber coordenadas */


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/

/***********************************************************************
*
*  $FC Fun��o: LAB Excluir valor
*
***********************************************************************/

void ExcluirValor( void * pValor ) {

    if (pValor != NULL)
        free( (tpConteudoPosicao *) pValor ) ;

} /* Fim fun��o: LAB Excluir valor */

/***********************************************************************
*
*  $FC Fun��o: LAB Converter dire��o
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

} /* Fim fun��o: LAB Converter dire��o */

/***********************************************************************
*
*  $FC Fun��o: LAB Ir entrada ou sa�da
*
***********************************************************************/

void IrEntradaOuSaida( LAB_tppLabirinto pLab, LAB_tpElemCasa tipoElem ) {

	MTZ_tpDirecao direcaoX, direcaoY;
	int i = 0, qtdIteracoesX, qtdIteracoesY, posXRetirar, posYRetirar;

	// Tratar se procura entrada ou sa�da
	if (tipoElem == LAB_ElemEntrada) {
		posXRetirar = pLab->posXEntrada;
		posYRetirar = pLab->posYEntrada;
	} else {
		posXRetirar = pLab->posXSaida;
		posYRetirar = pLab->posYSaida;
	}

	// Se o elemento estiver � esquerda da corrente, anda para oeste, sen�o anda para leste
	direcaoX = (posXRetirar < pLab->posXCorrente) ? MTZ_DirOeste : MTZ_DirLeste;
	qtdIteracoesX = posXRetirar - pLab->posXCorrente;
	if (qtdIteracoesX < 0) qtdIteracoesX = -qtdIteracoesX;

	// Se o elemento estiver acima da corrente, anda para norte, sen�o anda para sul
	direcaoY = (posYRetirar < pLab->posYCorrente) ? MTZ_DirNorte : MTZ_DirSul;
	qtdIteracoesY = posYRetirar - pLab->posYCorrente;
	if (qtdIteracoesY < 0) qtdIteracoesY = -qtdIteracoesY;

	// Anda at� o elemento
	for (i = 0; i < qtdIteracoesX; i++)
		MTZ_AndarDirecao( pLab->pMatriz, direcaoX );

	for (i = 0; i < qtdIteracoesY; i++)
		MTZ_AndarDirecao( pLab->pMatriz, direcaoY );

} /* Fim fun��o: LAB Ir entrada ou sa�da */

/***********************************************************************
*
*  $FC Fun��o: LAB Retirar entrada ou sa�da
*
*  $FV Valor retornado
*     0 - Caso tenha executado corretamente
*     1 - Caso tenha ocorrido problema ao executar
*
*  $EAE Assertivas de entradas esperadas
*     pLab != NULL
*     tipoElem � Entrada ou Saida
*
*  $EAE Assertivas de sa�da esperadas
*     A entrada ou sa�da ter� sido transformada em elemento vazio (caso n�o haja erro)
*
***********************************************************************/

int RetirarEntradaOuSaida( LAB_tppLabirinto pLab, LAB_tpElemCasa tipoElem ) {

	MTZ_tpDirecao direcaoX, direcaoY;
	int i = 0, qtdIteracoesX, qtdIteracoesY, retorno = 0, posXRetirar, posYRetirar;
	tpConteudoPosicao * pConteudoPresente;

	// Tratar se procura entrada ou sa�da
	if (tipoElem == LAB_ElemEntrada) {
		posXRetirar = pLab->posXEntrada;
		posYRetirar = pLab->posYEntrada;
	} else {
		posXRetirar = pLab->posXSaida;
		posYRetirar = pLab->posYSaida;
	}

	// Se o elemento estiver � esquerda da corrente, anda para oeste, sen�o anda para leste
	direcaoX = (posXRetirar < pLab->posXCorrente) ? MTZ_DirOeste : MTZ_DirLeste;
	qtdIteracoesX = posXRetirar - pLab->posXCorrente;
	if (qtdIteracoesX < 0) qtdIteracoesX = -qtdIteracoesX;

	// Se o elemento estiver acima da corrente, anda para norte, sen�o anda para sul
	direcaoY = (posYRetirar < pLab->posYCorrente) ? MTZ_DirNorte : MTZ_DirSul;
	qtdIteracoesY = posYRetirar - pLab->posYCorrente;
	if (qtdIteracoesY < 0) qtdIteracoesY = -qtdIteracoesY;

	// Anda at� o elemento
	for (i = 0; i < qtdIteracoesX; i++)
		MTZ_AndarDirecao( pLab->pMatriz, direcaoX );

	for (i = 0; i < qtdIteracoesY; i++)
		MTZ_AndarDirecao( pLab->pMatriz, direcaoY );

	// Verifica se o elemento � o procurado (sen�o da erro, retorna 1)
	MTZ_ObterValorCorrente( pLab->pMatriz, (void **) &pConteudoPresente) ;
	if (pConteudoPresente->elemento != tipoElem)
		retorno = 1;

	// Troca o elemento para casa vazia se nao tiver dado erro
	if (retorno == 0)
		pConteudoPresente->elemento = LAB_ElemVazio;

	// Volta para a casa corrente fazendo o passo oposto da ida at� a entrada
	direcaoX = (direcaoX == MTZ_DirOeste) ? MTZ_DirLeste : MTZ_DirOeste;
	direcaoY = (direcaoY == MTZ_DirNorte) ? MTZ_DirSul : MTZ_DirNorte;

	for (i = 0; i < qtdIteracoesX; i++)
		MTZ_AndarDirecao( pLab->pMatriz, direcaoX );

	for (i = 0; i < qtdIteracoesY; i++)
		MTZ_AndarDirecao( pLab->pMatriz, direcaoY );

	// Se executou certo, retorna 0
	return retorno;
} /* Fim fun��o: LAB Retirar entrada ou sa�da */

/***************************************************************************
*
*  Fun��o: LAB Imprimir tudo
*  ****/

void ImprimirTudo( LAB_tppLabirinto pLab, int solucao ) {

	tpConteudoPosicao * pConteudoPresente = NULL;

	int contLinha = 0, contColuna = 0;

	printf("\n");

	// Andar para o inicio sem alterar a marca��o X e Y da posi��o corrente
	MTZ_VoltarParaPrimeiro(pLab->pMatriz);

	// Imprimir tudo
	do {
		contLinha = 0;
		do {
			MTZ_ObterValorCorrente(pLab->pMatriz, (void **) &pConteudoPresente);
			ImprimirCasa(pConteudoPresente->elemento, (contLinha == pLab->posXCorrente && contColuna == pLab->posYCorrente), (solucao && pConteudoPresente->direcaoVolta != -1), pConteudoPresente->direcaoVolta);
			contLinha++;
		} while (MTZ_AndarDirecao(pLab->pMatriz, MTZ_DirLeste) != MTZ_CondRetDirecaoNaoExisteOuInvalida);

		// Volta para o inicio da linha
		while (MTZ_AndarDirecao(pLab->pMatriz, MTZ_DirOeste) != MTZ_CondRetDirecaoNaoExisteOuInvalida);
		printf("\n\n");
		contColuna++;

	} while (MTZ_AndarDirecao(pLab->pMatriz, MTZ_DirSul) != MTZ_CondRetDirecaoNaoExisteOuInvalida);

	// Voltar para a casa corrente certa
	MTZ_VoltarParaPrimeiro(pLab->pMatriz);
	for (contLinha = 0; contLinha < pLab->posXCorrente; contLinha++)
		MTZ_AndarDirecao(pLab->pMatriz, MTZ_DirLeste);
	for (contColuna = 0; contColuna < pLab->posYCorrente; contColuna++)
		MTZ_AndarDirecao(pLab->pMatriz, MTZ_DirSul);

} /* Fim fun��o: LAB Imprimir tudo */

/***********************************************************************
*
*  $FC Fun��o: LAB Imprimir Casa
*
***********************************************************************/

void ImprimirCasa( LAB_tpElemCasa elemento, int corrente, int solucao, LAB_tpDirecao direcao ) {

	char c, aux;

	if (solucao) {
		switch (direcao) {
	    case LAB_DirNorte:
	    	c = 'n';
	        break;
	    case LAB_DirSul:
	    	c = 's';
	        break;
	    case LAB_DirLeste:
	        c = 'l';
	        break;
	    default:
	    	c = 'o';
	        break;
	    }
		aux = c;

	} else {
		switch (elemento) {
	    case LAB_ElemParede:
	    	c = 219;
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

		if (corrente)
			aux = 'C';
		else
			aux = c;
	}

    printf("%c%c  ", c, aux);
    return;

} /* Fim fun��o: LAB Imprimir Casa */

/***********************************************************************
*
*  $FC Fun��o: LAB Menor dire��o
*
***********************************************************************/

LAB_tpDirecao MenorDirecao( MTZ_tppMatriz pMtz ) {

	// Inicializa tudo com INT_MAX para caso haja posi��es adjacentes inv�lidas
	int i = 0, menorIndice = 0;
	int adjacentes[4] = {INT_MAX, INT_MAX, INT_MAX, INT_MAX};

	// Verifica leste
	VerificaDirecao( pMtz, LAB_DirLeste, adjacentes );

	// Verifica oeste
	VerificaDirecao( pMtz, LAB_DirOeste, adjacentes );

	// Verifica norte
	VerificaDirecao( pMtz, LAB_DirNorte, adjacentes );

	// Verifica sul
	VerificaDirecao( pMtz, LAB_DirSul, adjacentes );

	// Verifica qual o menor (assume de in�cio que o 0 (norte) � o menor)
	while (i < 4) {
		if (adjacentes[i] < adjacentes[menorIndice])
			menorIndice = i;
		i++;
	}

	// Se o �ndice apontar para INT_MAX � porque todas as dire��es s�o inv�lidas
	if (adjacentes[menorIndice] == INT_MAX)
		return -1;
	else
		return (LAB_tpDirecao) menorIndice;

} /* Fim fun��o: LAB Menor dire��o */

/***********************************************************************
*
*  $FC Fun��o: LAB Verifica dire��o
*
***********************************************************************/

void VerificaDirecao( MTZ_tppMatriz pMtz, LAB_tpDirecao direcao, int * adjacentes ) {

	tpConteudoPosicao * pConteudoAux = NULL;
	MTZ_tpDirecao direcaoOposta;
	MTZ_tpDirecao direcaoConvertida;

	direcaoConvertida = ConverterDirecao( direcao );

	switch(direcaoConvertida) {
	case MTZ_DirNorte:
		direcaoOposta = MTZ_DirSul;
		break;
	case MTZ_DirSul:
		direcaoOposta = MTZ_DirNorte;
		break;
	case MTZ_DirLeste:
		direcaoOposta = MTZ_DirOeste;
		break;
	default:
		direcaoOposta = MTZ_DirLeste;
		break;
	}

	// Verifica a direcao se ela existir
	if (MTZ_AndarDirecao( pMtz, direcaoConvertida ) == MTZ_CondRetOK) {
		MTZ_ObterValorCorrente( pMtz, (void **) &pConteudoAux );
		if ((pConteudoAux->elemento == LAB_ElemVazio || pConteudoAux->elemento == LAB_ElemSaida) && pConteudoAux->direcaoVolta == -1)
			adjacentes[direcao] = pConteudoAux->numPassagens;
		// Volta para a casa anterior
		MTZ_AndarDirecao( pMtz, direcaoOposta );
	}

} /* Fim fun��o: LAB Verifica dire��o */

/********** Fim do m�dulo de implementa��o: M�dulo labirinto **********/
