/***************************************************************************
*  $MCI M�dulo de implementa��o: M�dulo de teste espec�fico
*
*  Arquivo gerado:              TESTLAB.C
*  Letras identificadoras:      TLAB
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
*     m�dulo labirinto. Ilustra como redigir um interpretador de comandos
*     de teste espec�ficos utilizando o arcabou�o de teste para C.
*
*  $EIU Interface com o usu�rio pessoa
*     Comandos de teste espec�ficos para testar o m�dulo labirinto:
*
*     "=criar"  <Int> <Int>
*                   - chama a fun��o LAB_CriarLabirinto( )
*     "=inselm" <Int> <Int>
*                   - chama a fun��o LAB_InserirElemento( <Int> )
*                     Obs. nota��o: <Int> � o valor do par�metro
*                     que se encontra no comando de teste.
*     "=andar" <Int>    
*                   - chama a fun��o LAB_AndarDirecao( )
*     "=destroi" <Int>
*                   - chama a fun��o LAB_DestruirLabirinto( )
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "TST_ESPC.H"
#include    "generico.h"
#include    "lerparm.h"

#include    "LABIRINTO.h"

/* Tabela dos nomes dos comandos de teste espec�ficos */

#define     CRIAR_LAB_CMD       "=criar"
#define     INS_ELM_CMD         "=inselm"
#define     ANDAR_CMD           "=andar"
#define     DESTROI_CMD         "=destruir"

/*****  Dados encapsulados no m�dulo  *****/

static LAB_tppLabirinto labirintoTeste = NULL ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: TLAB Efetuar opera��es de teste espec�ficas para o labirinto
*
*  $ED Descri��o da fun��o
*     Efetua os diversos comandos de teste espec�ficos para o m�dulo
*     labirinto sendo testado.
*
*  $EP Par�metros
*     $P ComandoTeste - String contendo o comando
*
*  $FV Valor retornado
*     Ver TST_tpCondRet definido em TST_ESPC.H
*
***********************************************************************/

TST_tpCondRet TST_EfetuarComando( char * ComandoTeste ) {

    LAB_tpCondRet CondRetObtido   = LAB_CondRetOK ;
    LAB_tpCondRet CondRetEsperada = LAB_CondRetFaltouMemoria ;
                                      /* inicializa para qualquer coisa */

    int ValorEsperado = 0  ;
    int ValorObtido   = 1  ;
    LAB_tpElemCasa ValorDado ;

	int  NumLidos = -1 ;

      /* Testar LAB Criar labirinto */

    if ( strcmp( ComandoTeste , CRIAR_LAB_CMD ) == 0 ) {

        int dim;

        NumLidos = LER_LerParametros( "ii" ,
            &dim, &CondRetEsperada ) ;
        if ( NumLidos != 2 ) {
            return TST_CondRetParm ;
        } /* if */

		CondRetObtido = LAB_CriarLabirinto( &labirintoTeste , dim ) ;

        return TST_CompararInt( CondRetEsperada , CondRetObtido ,
            "Retorno errado ao criar labirinto." );

    } /* fim ativa: Testar LAB Criar labirinto */

    /* Testar LAB Inserir elemento na casa corrente */
    else if ( strcmp( ComandoTeste , INS_ELM_CMD ) == 0 ) {

        NumLidos = LER_LerParametros( "ii" ,
            &ValorDado , &CondRetEsperada ) ;
        if ( NumLidos != 2 ) {
            return TST_CondRetParm ;
        } /* if */

        CondRetObtido = LAB_InserirElemento( labirintoTeste , ValorDado ) ;

        return TST_CompararInt( CondRetEsperada , CondRetObtido ,
            "Retorno errado inserir o elemento." );

    } /* fim ativa: Inserir elemento na casa corrente */

    /* Testar LAB Andar em Dire��o */
    else if ( strcmp( ComandoTeste , ANDAR_CMD ) == 0 ) {
        
        LAB_tpDirecao dir;

        NumLidos = LER_LerParametros( "ii" ,
            &dir, &CondRetEsperada ) ;
        if ( NumLidos != 2 ) {
            return TST_CondRetParm ;
        } /* if */

        CondRetObtido = LAB_AndarDirecao( labirintoTeste , dir ) ;

        return TST_CompararInt( CondRetEsperada , CondRetObtido ,
            "Retorno errado ao andar." );

    } /* fim ativa: Testar LAB Andar em Dire��o */

    /* Testar LAB Destruir Labirinto */
    else if ( strcmp( ComandoTeste , DESTROI_CMD ) == 0 ) {

        NumLidos = LER_LerParametros( "i" ,
            &CondRetEsperada ) ;
        if ( NumLidos != 1 ) {
            return TST_CondRetParm ;
        } /* if */

        CondRetObtido = LAB_DestruirLabirinto( &labirintoTeste ) ;
        labirintoTeste = NULL;

        return TST_CompararInt( CondRetEsperada , CondRetObtido ,
            "Retorno errado ao destruir labirinto." );

    } /* fim ativa: Testar LAB Destruir Labirinto */

    return TST_CondRetNaoConhec ;

} /* Fim fun��o: TLAB Efetuar opera��es de teste espec�ficas para labirinto */

/********** Fim do m�dulo de implementa��o: M�dulo de teste espec�fico **********/