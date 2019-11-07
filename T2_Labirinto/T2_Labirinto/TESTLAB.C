/***************************************************************************
*  $MCI Módulo de implementação: Módulo de teste específico
*
*  Arquivo gerado:              TESTLAB.C
*  Letras identificadoras:      TLAB
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
*       1.00   phd   03/10/2019 Início do desenvolvimento
*       1.01   aaf   03/10/2019 Alteração no teste de inserir elemento
*		1.02   phd	 05/10/2019 Documentação
*		1.10   jmb   04/11/2019 Inserir teste da função ReceberCoordenadas
*       1.20   jmb   05/11/2019 Inserir teste da função Imprimir Labirinto e Receber Solução
*		1.21   phd	 06/11/2019 Documentação
*
*  $ED Descrição do módulo
*     Este módulo contém as funções específicas para o teste do
*     módulo labirinto. Ilustra como redigir um interpretador de comandos
*     de teste específicos utilizando o arcabouço de teste para C.
*
*  $EIU Interface com o usuário pessoa
*     Comandos de teste específicos para testar o módulo labirinto:
*
*     "=criar"  <Int> <Int>
*                   - chama a função LAB_CriarLabirinto( <Int> )
*                     Obs. notação: <Int> é o tamanho do labirinto a ser criado.
*     "=inselm" <Int> <Int>
*                   - chama a função LAB_InserirElemento( <Int> )
*                     Obs. notação: <Int> é o valor do parâmetro.
*                     que se encontra no comando de teste.
*     "=andar" <Int>    
*                   - chama a função LAB_AndarDirecao( )
*     "=destroi" <Int>
*                   - chama a função LAB_DestruirLabirinto( )
*
*	  "=receber" <Int> <Int>
*					- chama a função LAB_ReceberCoordenadas ( <Int> )
*					  Obs. notação: <Int> são as coordenadas x e y da casa corrente.
*	  "=exibir" <Int>
*					- chama a função LAB_ImprimirLabirinto ( )
*	  "=resolver" <Int> 
*					- chama a função LAB_ExibeSolucao ( )
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "TST_ESPC.H"
#include    "generico.h"
#include    "lerparm.h"

#include    "LABIRINTO.h"

/* Tabela dos nomes dos comandos de teste específicos */

#define     CRIAR_LAB_CMD       "=criar"
#define     INS_ELM_CMD         "=inselm"
#define     ANDAR_CMD           "=andar"
#define     DESTROI_CMD         "=destruir"
#define		RECEBER_CMD			"=receber"
#define		IMPRIMIR_CMD		"=imprimir"
#define		RESOLVER_CMD		"=resolver"

/*****  Dados encapsulados no módulo  *****/

static LAB_tppLabirinto labirintoTeste = NULL ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TLAB Efetuar operações de teste específicas para o labirinto
*
*  $ED Descrição da função
*     Efetua os diversos comandos de teste específicos para o módulo
*     labirinto sendo testado.
*
*  $EP Parâmetros
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

    /* Testar LAB Andar em Direção */
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

    } /* fim ativa: Testar LAB Andar em Direção */

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
	
	/* Testar LAB Receber Coordenadas */
	else if ( strcmp( ComandoTeste , RECEBER_CMD ) == 0 ) {
        
        int x, y;

        NumLidos = LER_LerParametros( "iii" ,
			&x, &y, &CondRetEsperada ) ;
        if ( NumLidos != 3 ) {
            return TST_CondRetParm ;
        } /* if */

        CondRetObtido = LAB_ReceberCoordenadas(labirintoTeste, &x, &y);

        return TST_CompararInt( CondRetEsperada , CondRetObtido ,
            "Retorno errado ao andar." );
	} /* fim ativa: Testar LAB Receber Coordenadas */

	/* Testar LAB Imprimir Labirinto */
	else if ( strcmp( ComandoTeste , IMPRIMIR_CMD ) == 0 ) {

        NumLidos = LER_LerParametros( "i" ,
			&CondRetEsperada ) ;
        if ( NumLidos != 1 ) {
            return TST_CondRetParm ;
        } /* if */

        CondRetObtido = LAB_ImprimirLabirinto(labirintoTeste);

        return TST_CompararInt( CondRetEsperada , CondRetObtido ,
            "Retorno errado ao andar." );
	} /* fim ativa: Testar LAB Imprimir Labirinto */

	/* Testar LAB Exibe Solução */
	else if ( strcmp( ComandoTeste , RESOLVER_CMD ) == 0 ) {

        NumLidos = LER_LerParametros( "i" ,
			&CondRetEsperada ) ;
        if ( NumLidos != 1 ) {
            return TST_CondRetParm ;
        } /* if */

        CondRetObtido = LAB_ExibeSolucao(labirintoTeste);

        return TST_CompararInt( CondRetEsperada , CondRetObtido ,
            "Retorno errado ao andar." );
		} /* fim ativa: Testar LAB Exibe Solução */


    return TST_CondRetNaoConhec ;

} /* Fim função: TLAB Efetuar operações de teste específicas para labirinto */

/********** Fim do módulo de implementação: Módulo de teste específico **********/
