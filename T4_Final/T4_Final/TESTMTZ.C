/***************************************************************************
*  $MCI M�dulo de implementa��o: M�dulo de teste espec�fico
*
*  Arquivo gerado:              TESTMTZ.C
*  Letras identificadoras:      TMTZ
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
*       1.00   aaf   26/08/2019 In�cio do desenvolvimento
*       1.01   aaf   30/09/2019 Altera��o para n�o usar listas nem estruturas est�ticas
*
*  $ED Descri��o do m�dulo
*     Este m�dulo cont�m as fun��es espec�ficas para o teste do
*     m�dulo matriz. Ilustra como redigir um interpretador de comandos
*     de teste espec�ficos utilizando o arcabou�o de teste para C.
*
*  $EIU Interface com o usu�rio pessoa
*     Comandos de teste espec�ficos para testar o m�dulo matriz:
*
*     "=criar"  <Int> <Int>
*                   - chama a fun��o MTZ_CriarMatriz( )
*     "=inselm" <Int> <Char>
*                   - chama a fun��o MTZ_InserirElementoNaCasaCorrente( <Char> )
*                     Obs. nota��o: <Char> � o valor do par�metro
*                     que se encontra no comando de teste.
*     "=andar" <Int>    
*                   - chama a fun��o MTZ_AndarDirecao( )
*     "=obter" <Int> <Char>
*                   - chama a fun��o MTZ_ObterValorCorrente( ) e compara
*                     o valor retornado com o valor <Char>
*     "=destroi" <Int>
*                   - chama a fun��o MTZ_DestruirMatriz( )
*     "=primeiro" <Int>
*                   - chama a fun��o MTZ_VoltarParaPrimeiro( )
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "TST_ESPC.H"
#include    "generico.h"
#include    "lerparm.h"
#include    "CESPDIN.H"
#include    "CONTA.H"

#include    "MATRIZ.h"

/* Tabela dos nomes dos comandos de teste espec�ficos */

#define     CRIAR_MTZ_CMD       "=criar"
#define     INS_ELM_CMD         "=inselm"
#define     ANDAR_CMD           "=andar"
#define     OBTER_VAL_CMD       "=obter"
#define     DESTROI_CMD         "=destruir"
#define     PRIMEIRO_CMD        "=primeiro"

/* Comandos que somente operam em modo _DEBUG */

#ifdef _DEBUG
#define VERIFICA_CMD			"=verifica" 
#define DETURPAR_CMD			"=deturpar"
#endif

/*****  Dados encapsulados no m�dulo  *****/

static MTZ_tppMatriz matriz0 = NULL ;
static MTZ_tppMatriz matriz1 = NULL ;
static MTZ_tppMatriz matriz2 = NULL ;

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

static void ExcluirCaracter( void * pValor ) ;

static MTZ_tppMatriz EncontrarMatriz( int indice ) ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: TMTZ Efetuar opera��es de teste espec�ficas para matriz
*
*  $ED Descri��o da fun��o
*     Efetua os diversos comandos de teste espec�ficos para o m�dulo
*     matriz sendo testado.
*
*  $EP Par�metros
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

    #ifdef _DEBUG
    int ErrosEsperados = 0;
    int ErrosObtidos = 0;
    #endif

    int  NumLidos = -1 ;

    TST_tpCondRet Ret ;

    int indiceMtz, modoDeturpar;

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
        #ifdef _DEBUG
        pChar = (char *) CED_Malloc(sizeof(char), __LINE__, __FILE__);
        #else
        pChar = (char *) malloc(sizeof(char));
        #endif

        if ( pChar == NULL ) {
            return TST_CondRetMemoria ;
        } /* if */
        #ifdef _DEBUG
        CED_MarcarEspacoAtivo( pChar );
        #endif
        *pChar = ValorDado;

        CondRetObtido = MTZ_InserirElementoNaCasaCorrente( matDada , (void *) pChar 
                                                                                #ifdef _DEBUG
                                                                                , "CHAR", sizeof(char)
                                                                                #endif
                                                                                ) ;

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
            "O valor est� errado.") ;

     } /* fim ativa: Testar MTZ Obter valor corrente */

    /* Testar MTZ Andar em Dire��o */
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

    } /* fim ativa: Testar MTZ Andar em Dire��o */

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

	/* Verificar estrutura */
	#ifdef _DEBUG

	else if ( strcmp( ComandoTeste, VERIFICA_CMD) == 0) {
		NumLidos = LER_LerParametros( "ii" , 
									&indiceMtz, &ErrosEsperados);
		if (NumLidos != 2) {
			return TST_CondRetParm;
		}

		matDada = EncontrarMatriz( indiceMtz );
		ErrosObtidos = MTZ_VerificarMatriz( matDada );

        return TST_CompararInt( ErrosEsperados , ErrosObtidos ,
            "Retorno errado no num de erros ao verificar a matriz" );
	} /* fim ativa: Verificar estrutura */

	/* Deturpar uma matriz */
	else if ( strcmp( ComandoTeste , DETURPAR_CMD ) == 0 ) {

        NumLidos = LER_LerParametros( "ii" ,
                           &indiceMtz , &modoDeturpar ) ;

        if ( ( NumLidos != 2 )) {
           return TST_CondRetParm ;
        } /* if */

		matDada = EncontrarMatriz( indiceMtz );

        MTZ_Deturpar ( matDada , modoDeturpar ) ;

        return TST_CondRetOK ;

    } /* fim ativa: Deturpar uma matriz */

    #endif
    
    return TST_CondRetNaoConhec;
	  
} /* Fim fun��o: TMTZ Efetuar opera��es de teste espec�ficas para matriz */

/*****  C�digo das fun��es encapsuladas no m�dulo  *****/

/***********************************************************************
*
*  $FC Fun��o: TMTZ Excluir caracter
*
***********************************************************************/

void ExcluirCaracter( void * pValor ) {

    if (pValor != NULL) {
        #ifdef _DEBUG
        CED_MarcarEspacoNaoAtivo(pValor);
        CED_Free(pValor);
        #else
        free( pValor ) ;
        #endif
    }

} /* Fim fun��o: TMTZ Excluir caracter */

/***********************************************************************
*
*  $FC Fun��o: TMTZ Encontrar matriz
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

} /* Fim fun��o: TMTZ Encontrar matriz */

/********** Fim do m�dulo de implementa��o: M�dulo de teste espec�fico **********/
