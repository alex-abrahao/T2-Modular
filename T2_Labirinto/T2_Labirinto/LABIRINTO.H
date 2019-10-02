#if ! defined( LABIRINTO_ )
#define LABIRINTO_
/***************************************************************************
*
*  $MCD Módulo de definição: Módulo labirinto
*
*  Arquivo gerado:              LAB.H
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
*  $ED Descrição do módulo
*
*
***************************************************************************/

#if defined( LABIRINTO_OWN )
   #define LABIRINTO_EXT
#else
   #define LABIRINTO_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para um labirinto */

typedef struct tgLabirinto * LAB_tppLabirinto ;

/***********************************************************************
*
*  $TC Tipo de dados: LAB Condiçoes de retorno
*
*
***********************************************************************/

   typedef enum {

         LAB_CondRetOK = 0 ,
               /* Executou correto */

         LAB_CondRetErroEstrutura = 1 ,
               /* Estrutura do labirinto está errada */

         LAB_CondRetDirecaoNaoExisteOuInvalida = 2 ,
               /* Direcao nao existe ou é invalida (sai do labirinto) */

         LAB_CondRetLabirintoNaoExiste = 3 ,
               /* Labirinto não existe */

         LAB_CondRetFaltouMemoria = 4 ,
               /* Faltou memória ao alocar dados */

		     LAB_CondRetNaoTemSolucao = 5
			         /* Labirinto não tem solução */

   } LAB_tpCondRet ;

/***********************************************************************
 *
 *  $TC Tipo de dados: LAB Direções
 *
 *
 ***********************************************************************/

    typedef enum {
        
        LAB_DirNorte = 0 ,
            /* Andar na direção Norte */

        LAB_DirLeste = 1 ,
            /* Andar na direção Leste */

        LAB_DirSul = 2 ,
            /* Andar na direção Sul */

        LAB_DirOeste = 3 ,
            /* Andar na direção Oeste */

    } LAB_tpDirecao ;

/***********************************************************************
*
*  $FC Função: LAB Criar labirinto
*
*  $ED Descrição da função
*     Cria um novo labirinto n x n.
*     Caso já exista um labirinto no ponteiro especificado, este será destruído.
*
*  $EP Parametros
*     $P ppLab - Ponteiro de ponteiro para um labirinto
*     $P tam - Tamanho do labirinto (dimensão n x n)
*
*  $FV Valor retornado
*     LAB_CondRetOK
*     LAB_CondRetFaltouMemoria
*     LAB_CondRetErroEstrutura   - Caso a dimensão do labirinto seja menor que zero ou o ponteiro seja nulo
*
***********************************************************************/

LAB_tpCondRet LAB_CriarLabirinto( LAB_tppLabirinto * ppLab, int tam ) ;

/***********************************************************************
*
*  $FC Função: LAB Destruir labirinto
*
*  $ED Descrição da função
*     Destrói o labirinto.
*     Faz nada caso o ponteiro nao contenha um labirinto.
*
*  $EP Parametros
*     $P ppLab - Ponteiro de ponteiro para um labirinto
*
*  $FV Valor retornado
*     LAB_CondRetOK
*     LAB_CondRetLabirintoNaoExiste   - Caso o ponteiro seja nulo
*
***********************************************************************/

LAB_tpCondRet LAB_DestruirLabirinto( LAB_tppLabirinto * ppLab ) ;

/***********************************************************************
*
*  $FC Função: LAB Andar em Direção
*
*  $ED Descrição da função
*     Anda na direção especificada.
*
*  $EP Parâmetros
*     $P pLab - Ponteiro para um labirinto
*     $P direcao - Direção a andar
*
*  $FV Valor retornado
*     LAB_CondRetOK
*     LAB_CondRetDirecaoNaoExisteOuInvalida
*     LAB_CondRetLabirintoNaoExiste
*
***********************************************************************/

LAB_tpCondRet LAB_AndarDirecao( LAB_tppLabirinto pLab, LAB_tpDirecao direcao ) ;

/***********************************************************************
*
*  $FC Função: LAB Imprimir labirinto
*
*  $ED Descrição da função
*     Imprime na tela o labirinto apontado, caso exista.
*
*  $EP Parâmetros
*     $P pLab - Ponteiro para um labirinto
*
*  $FV Valor retornado
*     LAB_CondRetOK
*     LAB_CondRetLabirintoNaoExiste
*
***********************************************************************/

LAB_tpCondRet LAB_ImprimirLabirinto( LAB_tppLabirinto pLab ) ;

/***********************************************************************
*
*  $FC Função: LAB Voltar para a entrada
*
*  $ED Descrição da função
*     Volta o ponteiro corrente para a entrada do labirinto.
*
*  $EP Parametros
*     $P pLab - Ponteiro para um labirinto
*
*  $FV Valor retornado
*     LAB_CondRetOK
*     LAB_CondRetLabirintoNaoExiste
*
***********************************************************************/

LAB_tpCondRet LAB_VoltarParaEntrada( LAB_tppLabirinto pLab ) ;

/***********************************************************************
*
*  $FC Função: LAB Existe solução
*
*  $ED Descrição da função
*     Verifica se o labirinto possui uma solução
*
*  $EP Parametros
*     $P pLab - Ponteiro para um labirinto
*
*  $FV Valor retornado
*     0 - Não existe solução ou pLab é NULL
*     1 - Existe solução
*
***********************************************************************/

int LAB_ExisteSolucao( LAB_tppLabirinto pLab ) ;

#undef LAB_EXT

/********** Fim do módulo de definição: Módulo matriz **********/

#else
#endif
