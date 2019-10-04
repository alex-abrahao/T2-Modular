#if ! defined( LABIRINTO_ )
#define LABIRINTO_
/***************************************************************************
*
*  $MCD M�dulo de defini��o: M�dulo labirinto
*
*  Arquivo gerado:              LAB.H
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
*       1.01   aaf   30/09/2019 Documenta��o inicial
*       1.02   aaf   30/09/2019 Assertivas de entrada e sa�da
*
*  $ED Descri��o do m�dulo
*
*
***************************************************************************/

#if defined( LABIRINTO_OWN )
   #define LABIRINTO_EXT
#else
   #define LABIRINTO_EXT extern
#endif

/***** Declara��es exportadas pelo m�dulo *****/

/* Tipo refer�ncia para um labirinto */

typedef struct tgLabirinto * LAB_tppLabirinto ;

/***********************************************************************
*
*  $TC Tipo de dados: LAB Condi�oes de retorno
*
*
***********************************************************************/

   typedef enum {

         LAB_CondRetOK = 0 ,
               /* Executou correto */

         LAB_CondRetErroEstrutura = 1 ,
               /* Estrutura do labirinto est� errada */

         LAB_CondRetDirecaoNaoExisteOuInvalida = 2 ,
               /* Direcao nao existe ou � invalida (sai do labirinto) */

         LAB_CondRetLabirintoNaoExiste = 3 ,
               /* Labirinto n�o existe */

         LAB_CondRetFaltouMemoria = 4 ,
               /* Faltou mem�ria ao alocar dados */

		     LAB_CondRetNaoTemSolucao = 5 ,
			         /* Labirinto n�o tem solu��o */

         LAB_CondRetElementoInvalido = 6
               /* Elemento inserido na posi��o � inv�lido */

   } LAB_tpCondRet ;

/***********************************************************************
 *
 *  $TC Tipo de dados: LAB Dire��es
 *
 *
 ***********************************************************************/

    typedef enum {
        
        LAB_DirNorte = 0 ,
            /* Andar na dire��o Norte */

        LAB_DirLeste = 1 ,
            /* Andar na dire��o Leste */

        LAB_DirSul = 2 ,
            /* Andar na dire��o Sul */

        LAB_DirOeste = 3 ,
            /* Andar na dire��o Oeste */

    } LAB_tpDirecao ;

/***********************************************************************
 *
 *  $TC Tipo de dados: LAB Elementos da casa
 *
 *
 ***********************************************************************/

    typedef enum {
        
        LAB_ElemVazio = 0 ,
            /* Elemento que representa um espa�o vazio */

        LAB_ElemParede = 1 ,
            /* Elemento que representa uma parede */

        LAB_ElemEntrada = 2 ,
            /* Elemento que representa a entrada do labirinto */

        LAB_ElemSaida = 3 ,
            /* Elemento que representa a sa�da do labirinto */

    } LAB_tpElemCasa ;

/***********************************************************************
*
*  $FC Fun��o: LAB Criar labirinto
*
*  $ED Descri��o da fun��o
*     Cria um novo labirinto n x n.
*     Caso j� exista um labirinto no ponteiro especificado, este ser� destru�do.
*
*  $EP Parametros
*     $P ppLab - Ponteiro de ponteiro para um labirinto
*     $P tam - Tamanho do labirinto (dimens�o n x n)
*
*  $FV Valor retornado
*     LAB_CondRetOK
*     LAB_CondRetFaltouMemoria
*     LAB_CondRetErroEstrutura   - Caso a dimens�o do labirinto seja menor que zero ou o ponteiro seja nulo
*
*  Condi��es de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     ppLab n�o cont�m lixo
*     tam > 1
*
*  $EAS Assertivas de sa�da esperadas
*     Valem as assertivas estruturais do Labirinto
*     A entrada do labirinto estar� em (0, 0)
*     A posi��o corrente do labirinto estar� em (0, 0)
*     A sa�da do labirinto estar� em (tam - 1, tam - 1)
*
***********************************************************************/

LAB_tpCondRet LAB_CriarLabirinto( LAB_tppLabirinto * ppLab, int tam ) ;

/***********************************************************************
*
*  $FC Fun��o: LAB Destruir labirinto
*
*  $ED Descri��o da fun��o
*     Destr�i o labirinto.
*     Faz nada caso o ponteiro nao contenha um labirinto.
*
*  $EP Parametros
*     $P ppLab - Ponteiro de ponteiro para um labirinto
*
*  $FV Valor retornado
*     LAB_CondRetOK
*     LAB_CondRetLabirintoNaoExiste   - Caso o ponteiro seja nulo
*
*  Condi��es de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     ppLab n�o cont�m lixo
*     Valem as assertivas estruturais do Labirinto
*
*  $EAS Assertivas de sa�da esperadas
*     *ppLab = NULL
*     Todos os elementos dentro casas estar�o liberados
*     A estrutura de labirinto estar� liberada
*
***********************************************************************/

LAB_tpCondRet LAB_DestruirLabirinto( LAB_tppLabirinto * ppLab ) ;

/***********************************************************************
*
*  $FC Fun��o: LAB Andar em Dire��o
*
*  $ED Descri��o da fun��o
*     Anda na dire��o especificada.
*
*  $EP Par�metros
*     $P pLab - Ponteiro para um labirinto
*     $P direcao - Dire��o a andar
*
*  $FV Valor retornado
*     LAB_CondRetOK
*     LAB_CondRetDirecaoNaoExisteOuInvalida
*     LAB_CondRetLabirintoNaoExiste
*
*  Condi��es de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     Valem as assertivas estruturais do Labirinto
*
*  $EAS Assertivas de sa�da esperadas
*     Valem as assertivas estruturais do Labirinto
*     Se a dire��o for inv�lida ou sair do labirinto, a posi��o corrente se manter� na �ltima posi��o v�lida
*     Se a dire��o for v�lida, a posi��o corrente estar� movida na dire��o especificada
*
***********************************************************************/

LAB_tpCondRet LAB_AndarDirecao( LAB_tppLabirinto pLab, LAB_tpDirecao direcao ) ;

/***********************************************************************
*
*  $FC Fun��o: LAB Inserir elemento
*
*  $ED Descri��o da fun��o
*     Muda o elemento presente na casa corrente do labirinto.
*
*  $EP Par�metros
*     $P pLab - Ponteiro para um labirinto
*     $P elemento - Um elemento a ser posto na casa corrente
*
*  $FV Valor retornado
*     LAB_CondRetOK
*     LAB_CondRetLabirintoNaoExiste
*     LAB_CondRetElementoInvalido
*
*  Condi��es de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     Valem as assertivas estruturais do Labirinto
*     Se o elemento presente na posi��o corrente for uma entrada ou sa�da, ele n�o ser� sobrescrito
*
*  $EAS Assertivas de sa�da esperadas
*     Valem as assertivas estruturais do Labirinto
*     Se o elemento for inv�lido, a posi��o corrente manter� o �ltimo elemento v�lido
*     Se o elemento for v�lido, o elemento da posi��o corrente estar� sobrescrito com o novo
*     Se o elemento for uma entrada, a entrada antiga ir� virar uma posi��o vazia
*     Se o elemento for uma sa�da, a sa�da antiga ir� virar uma posi��o vazia
*
***********************************************************************/

LAB_tpCondRet LAB_InserirElemento( LAB_tppLabirinto pLab, LAB_tpElemCasa elemento ) ;

/***********************************************************************
*
*  $FC Fun��o: LAB Imprimir labirinto
*
*  $ED Descri��o da fun��o
*     Imprime na tela o labirinto apontado, caso exista.
*
*  $EP Par�metros
*     $P pLab - Ponteiro para um labirinto
*
*  $FV Valor retornado
*     LAB_CondRetOK
*     LAB_CondRetLabirintoNaoExiste
*
*  Condi��es de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     Valem as assertivas estruturais do Labirinto
*
*  $EAS Assertivas de sa�da esperadas
*     Valem as assertivas estruturais do Labirinto
*
***********************************************************************/

LAB_tpCondRet LAB_ImprimirLabirinto( LAB_tppLabirinto pLab ) ;

/***********************************************************************
*
*  $FC Fun��o: LAB Existe solu��o
*
*  $ED Descri��o da fun��o
*     Verifica se o labirinto possui uma solu��o
*
*  $EP Parametros
*     $P pLab - Ponteiro para um labirinto
*
*  $FV Valor retornado
*     0 - N�o existe solu��o ou pLab � NULL
*     1 - Existe solu��o
*
*  Condi��es de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     Valem as assertivas estruturais do Labirinto
*
*  $EAS Assertivas de sa�da esperadas
*     Valem as assertivas estruturais do Labirinto
*
***********************************************************************/

int LAB_ExisteSolucao( LAB_tppLabirinto pLab ) ;

#undef LAB_EXT

/********** Fim do m�dulo de defini��o: M�dulo matriz **********/

#else
#endif