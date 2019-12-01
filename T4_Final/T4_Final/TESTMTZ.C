/***************************************************************************
*  $MCI Módulo de implementação: Módulo de teste específico
*
*  Arquivo gerado:              TESTMTZ.C
*  Letras identificadoras:      TMTZ
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
*       1.00   aaf   26/08/2019 Início do desenvolvimento
*       1.01   aaf   30/09/2019 Alteração para não usar listas nem estruturas estáticas
*
*  $ED Descrição do módulo
*     Este módulo contém as funções específicas para o teste do
*     módulo matriz. Ilustra como redigir um interpretador de comandos
*     de teste específicos utilizando o arcabouço de teste para C.
*
*  $EIU Interface com o usuário pessoa
*     Comandos de teste específicos para testar o módulo matriz:
*
*     "=criar"  <Int> <Int>
*                   - chama a função MTZ_CriarMatriz( )
*     "=inselm" <Int> <Char>
*                   - chama a função MTZ_InserirElementoNaCasaCorrente( <Char> )
*                     Obs. notação: <Char> é o valor do parâmetro
*                     que se encontra no comando de teste.
*     "=andar" <Int>    
*                   - chama a função MTZ_AndarDirecao( )
*     "=obter" <Int> <Char>
*                   - chama a função MTZ_ObterValorCorrente( ) e compara
*                     o valor retornado com o valor <Char>
*     "=destroi" <Int>
*                   - chama a função MTZ_DestruirMatriz( )
*     "=primeiro" <Int>
*                   - chama a função MTZ_VoltarParaPrimeiro( )
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "TST_ESPC.H"
#include    "generico.h"
#include    "lerparm.h"

#include    "MATRIZ.h"

/* Tabela dos nomes dos comandos de teste específicos */

const char	   RESET_MATRIZ_CMD [ ] = "resetmatriz"   ;
const char     CRIAR_MTZ_CMD    [ ] = "=criar"        ;
const char     INS_ELM_CMD      [ ] = "=inselm"       ; 
const char     ANDAR_CMD        [ ] = "=andar"        ;
const char     OBTER_VAL_CMD    [ ] = "=obter"        ;
const char     DESTROI_CMD      [ ] = "=destruir"     ;
const char     PRIMEIRO_CMD     [ ] = "=primeiro"     ;

/* Comandos que somente operam em modo _DEBUG */

const char VER_CABECA_CMD[ ] = "=verificarcabeca" ;
const char VER_MATRIZ_CMD[ ] = "=verificararvore" ;
const char VER_MEMORIA_CMD[ ] = "=verificarmemoria" ;
const char DETURPAR_CMD[ ]   = "=deturpar" ;

/*****  Dados encapsulados no módulo  *****/

static MTZ_tppMatriz matriz0 = NULL ;
static MTZ_tppMatriz matriz1 = NULL ;
static MTZ_tppMatriz matriz2 = NULL ;

/***** Protótipos das funções encapuladas no módulo *****/

static void ExcluirCaracter( void * pValor ) ;

static MTZ_tppMatriz EncontrarMatriz( int indice ) ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TMTZ Efetuar operações de teste específicas para matriz
*
*  $ED Descrição da função
*     Efetua os diversos comandos de teste específicos para o módulo
*     matriz sendo testado.
*
*  $EP Parâmetros
*     $P ComandoTeste - String contendo o comando
*
*  $FV Valor retornado
*     Ver TST_tpCondRet definido em TST_ESPC.H
*
***********************************************************************/

TST_tpCondRet TST_EfetuarComando( char * ComandoTeste ) {

    MTZ_tpCondRet CondRetObtido   = MTZ_CondRetOK ;
    MTZ_tpCondRet CondRetEsperada = MTZ_CondRetFaltouMemoria ;
                                      /* inicializa para qualquer coisa */

    char ValorEsperado = '?'  ;
    char ValorObtido   = '!'  ;
    char ValorDado     = '\0' ;
    char * pChar = NULL;
    MTZ_tppMatriz matDada;

    int  NumLidos = -1 ;

    TST_tpCondRet Ret ;

    int indiceMtz;

      /* Testar MTZ Criar matriz */

    if ( strcmp( ComandoTeste , CRIAR_MTZ_CMD ) == 0 ) {

        int dim;

        NumLidos = LER_LerParametros( "iii" ,
            &indiceMtz, &dim, &CondRetEsperada ) ;
        if ( NumLidos != 3 ) {
            return TST_CondRetParm ;
        } /* if */

        if (indiceMtz == 0) {
            CondRetObtido = MTZ_CriarMatriz( &matriz0 , dim , ExcluirCaracter ) ;
        } else if (indiceMtz == 1) {
            CondRetObtido = MTZ_CriarMatriz( &matriz1 , dim , ExcluirCaracter ) ; 
        } else if (indiceMtz == 2) {
            CondRetObtido = MTZ_CriarMatriz( &matriz2 , dim , ExcluirCaracter ) ; 
        } else
            return TST_CondRetNaoConhec;

        return TST_CompararInt( CondRetEsperada , CondRetObtido ,
            "Retorno errado ao criar matriz." );

    } /* fim ativa: Testar MTZ Criar matriz */

    /* Testar MTZ Inserir elemento na casa corrente */
    else if ( strcmp( ComandoTeste , INS_ELM_CMD ) == 0 ) {

        NumLidos = LER_LerParametros( "ici" ,
            &indiceMtz, &ValorDado , &CondRetEsperada ) ;
        if ( NumLidos != 3 ) {
            return TST_CondRetParm ;
        } /* if */

        matDada = EncontrarMatriz(indiceMtz);
        pChar = (char *) malloc(sizeof(char));

        if ( pChar == NULL ) {
            return TST_CondRetMemoria ;
        } /* if */

        *pChar = ValorDado;

        CondRetObtido = MTZ_InserirElementoNaCasaCorrente( matDada , (void *) pChar ) ;

        return TST_CompararInt( CondRetEsperada , CondRetObtido ,
            "Retorno errado inserir o elemento." );

    } /* fim ativa: Inserir elemento na casa corrente */

    /* Testar MTZ Obter valor corrente */
    else if ( strcmp( ComandoTeste , OBTER_VAL_CMD ) == 0 ) {

        NumLidos = LER_LerParametros( "ici" ,
            &indiceMtz, &ValorEsperado , &CondRetEsperada ) ;
        if ( NumLidos != 3 ) {
            return TST_CondRetParm ;
        } /* if */

        matDada = EncontrarMatriz(indiceMtz);
        CondRetObtido = MTZ_ObterValorCorrente( matDada, (void **) &pChar ) ;

        Ret = TST_CompararInt( CondRetEsperada , CondRetObtido ,
            "Retorno errado ao obter valor corrente.");

        if ( Ret != TST_CondRetOK ) {
            return Ret ;
        } /* if */

        if ( CondRetObtido == MTZ_CondRetMatrizNaoExiste && matDada == NULL ) {
            return TST_CondRetOK;
        }

        if ( CondRetObtido == MTZ_CondRetCasaVazia && pChar == NULL ) {
            return TST_CondRetOK;
        }

        Ret = TST_CompararPonteiroNulo( 1 , pChar,
            "Retorno errado: Ponteiro para char nulo") ;

        if ( Ret != TST_CondRetOK ) {
            return Ret ;
        } /* if */

        ValorObtido = *pChar;

        return TST_CompararChar( ValorEsperado , ValorObtido ,
            "O valor está errado.") ;

     } /* fim ativa: Testar MTZ Obter valor corrente */

    /* Testar MTZ Andar em Direção */
    else if ( strcmp( ComandoTeste , ANDAR_CMD ) == 0 ) {
        
        MTZ_tpDirecao dir;

        NumLidos = LER_LerParametros( "iii" ,
            &indiceMtz, &dir, &CondRetEsperada ) ;
        if ( NumLidos != 3 ) {
            return TST_CondRetParm ;
        } /* if */

        matDada = EncontrarMatriz(indiceMtz);
        CondRetObtido = MTZ_AndarDirecao( matDada , dir ) ;

        return TST_CompararInt( CondRetEsperada , CondRetObtido ,
            "Retorno errado ao andar." );

    } /* fim ativa: Testar MTZ Andar em Direção */

    /* Testar MTZ Destruir Matriz */
    else if ( strcmp( ComandoTeste , DESTROI_CMD ) == 0 ) {

        NumLidos = LER_LerParametros( "ii" ,
            &indiceMtz, &CondRetEsperada ) ;
        if ( NumLidos != 2 ) {
            return TST_CondRetParm ;
        } /* if */

        if (indiceMtz == 0) {
            CondRetObtido = MTZ_DestruirMatriz( &matriz0 ) ;
            matriz0 = NULL;
        } else if (indiceMtz == 1) {
            CondRetObtido = MTZ_DestruirMatriz( &matriz1 ) ;
            matriz1 = NULL;
        } else if (indiceMtz == 2) {
            CondRetObtido = MTZ_DestruirMatriz( &matriz2 ) ;
            matriz2 = NULL;
        } else
            return TST_CondRetParm;

        return TST_CompararInt( CondRetEsperada , CondRetObtido ,
            "Retorno errado ao destruir matriz." );

    } /* fim ativa: Testar MTZ Destruir Matriz */

    /* Testar MTZ Voltar primeiro */
    else if ( strcmp( ComandoTeste , PRIMEIRO_CMD ) == 0 ) {

        NumLidos = LER_LerParametros( "ii" ,
            &indiceMtz, &CondRetEsperada ) ;
        if ( NumLidos != 2 ) {
            return TST_CondRetParm ;
        } /* if */

        matDada = EncontrarMatriz(indiceMtz);

        CondRetObtido = MTZ_VoltarParaPrimeiro( matDada ) ;

        return TST_CompararInt( CondRetEsperada , CondRetObtido ,
            "Retorno errado ao destruir matriz." );

    } /* fim ativa: Testar MTZ Voltar primeiro */

    return TST_CondRetNaoConhec ;

} /* Fim função: TMTZ Efetuar operações de teste específicas para matriz */

/*****  Código das funções encapsuladas no módulo  *****/

/***********************************************************************
*
*  $FC Função: TMTZ Excluir caracter
*
***********************************************************************/

void ExcluirCaracter( void * pValor ) {

    if (pValor != NULL)
        free( pValor ) ;

} /* Fim função: TMTZ Excluir caracter */

/***********************************************************************
*
*  $FC Função: TMTZ Encontrar matriz
*
***********************************************************************/

MTZ_tppMatriz EncontrarMatriz( int indice ) {

    switch (indice) {
    case 0:
        return matriz0;
    case 1:
        return matriz1;
    case 2:
        return matriz2;
    default:
        return NULL;
    }

} /* Fim função: TMTZ Encontrar matriz */

/********** Fim do módulo de implementação: Módulo de teste específico **********/
