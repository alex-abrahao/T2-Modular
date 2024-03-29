/***************************************************************************
*  $MCI M�dulo de implementa��o: M�dulo matriz
*
*  Arquivo gerado:              MATRIZ.C
*  Letras identificadoras:      MTZ
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
*       2.00   aaf   30/11/2019 In�cio da instrumenta��o
*
***************************************************************************/

#include   <malloc.h>
#include   <stdio.h>

#define MATRIZ_OWN
#include "MATRIZ.H"
#undef MATRIZ_OWN

#ifdef _DEBUG
#include "GENERICO.H"
#include "TST_ESPC.H"
#include "CESPDIN.H"
#include "CONTA.H"
#include "TiposEspacosMatriz.def"
#include <string.h>
#define TAM_CONTEUDO 20
#endif

/***********************************************************************
*
*  $TC Tipo de dados: MTZ Descritor do casa da matriz
*
*
*  $ED Descri��o do tipo
*     Descreve a organiza��o de uma casa da matriz
*
***********************************************************************/

   typedef struct tgCasaMatriz {

         struct tgCasaMatriz ** pCasasAdjacentes ;
               /* Vetor de ponteiros para as casas adjacentes */
       
         void * conteudo ;
               /* Conte�do da casa */

         #ifdef _DEBUG
         char tipoConteudo[TAM_CONTEUDO];
               /* Tipo do conte�do presente na casa */

         MTZ_tppMatriz pCabeca;
               /* Ponteiro para cabe�a da matriz */

         int tamBytes;
               /* Tamanho total do conte�do da casa, em bytes */
         #endif
       
   } tpCasaMatriz ;

/***********************************************************************
*
*  $TC Tipo de dados: MTZ Descritor da cabe�a de uma matriz
*
*
*  $ED Descri��o do tipo
*     A cabe�a da matriz � o ponto de acesso para uma determinada matriz.
*     Por interm�dio da refer�ncia para a casa corrente pode-se navegar
*     pela matriz.
*     Cont�m a refer�ncia para a fun��o que destroi o conteudo de uma casa.
*
***********************************************************************/

   typedef struct tgMatriz {
       
         tpCasaMatriz * pPrimeiro ;
               /* Ponteiro para a casa (0, 0) da matriz */

         tpCasaMatriz * pCasaCorr ;
               /* Ponteiro para o casa corrente da matriz */
       
         char id;
               /* Caracter identificador da matriz (opcional) */

         void ( * ExcluirValor ) ( void * pValor ) ;
               /* Ponteiro para a fun��o de destrui��o do valor contido em um elemento */

         #ifdef _DEBUG
         int numCasas;
               /* N�mero de casas alocadas na matriz */

         int tamBytes;
               /* Tamanho total da estrutura, em bytes */
         #endif

   } tpMatriz ;

/*****  Dados encapsulados no m�dulo  *****/

#ifdef _DEBUG

static char EspacoLixo[ 256 ] =
       "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" ;
      /* Espa�o de dados lixo usado ao testar */
#endif

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static tpCasaMatriz * CriarCasa( ) ;

   static void DestroiMatriz( MTZ_tppMatriz pMtz ) ;

   static void DestroiCasa( tpCasaMatriz * pCasa, void ( * ExcluirValor ) ( void * pValor ) ) ;

   #ifdef _DEBUG

   static int TamConteudo(const char * nome) ;

   #endif

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: MTZ Criar matriz
*  ****/

   MTZ_tpCondRet MTZ_CriarMatriz(MTZ_tppMatriz * ppMtz, int n, void ( * ExcluirValor ) ( void * pValor ) ) {

      int linha, coluna;
      // S� � preciso setar os ponteiros para as casas oeste, norte, noroeste e nordeste, assim como
      // setar nessas casas as direcoes inversas para o ponteiro da casa atual que est� sendo criada.
      tpCasaMatriz * pCasaOeste, * pCasaInicioLinha, * pCasaAtual, * pCasaNorte;

      if (n <= 0) return MTZ_CondRetErroEstrutura;

      if (ppMtz == NULL) return MTZ_CondRetErroEstrutura;

      // Se j� havia uma matriz anteriormente, destrua-a primeiro
      if (*ppMtz != NULL) {
         MTZ_DestruirMatriz(ppMtz);
      }

      // Alocar espa�o para a head
      #ifdef _DEBUG
      *ppMtz = ( tpMatriz * ) CED_Malloc(sizeof( tpMatriz ), __LINE__, __FILE__);
      #else
      *ppMtz = ( tpMatriz * ) malloc( sizeof( tpMatriz )) ;
      #endif

      if (*ppMtz == NULL) return MTZ_CondRetFaltouMemoria;

      #ifdef _DEBUG
      CED_DefinirTipoEspaco( *ppMtz , MTZ_TipoEspacoCabeca ) ;
      CED_MarcarEspacoAtivo( *ppMtz );
      #endif
      // Setup da head
      (*ppMtz)->id = 0;
      (*ppMtz)->ExcluirValor = ExcluirValor;
      (*ppMtz)->pCasaCorr = NULL;
      #ifdef _DEBUG
      (*ppMtz)->numCasas = 0;
      (*ppMtz)->tamBytes = 0;
      #endif

      // Criar a primeira casa
      (*ppMtz)->pPrimeiro = CriarCasa();
      if ((*ppMtz)->pPrimeiro == NULL) {
         // Libera a head
         #ifdef _DEBUG
         CED_MarcarEspacoNaoAtivo( *ppMtz ) ;
         CED_Free(*ppMtz);
         #else
         free(*ppMtz);
         #endif
         *ppMtz = NULL;
         return MTZ_CondRetFaltouMemoria;
      }
      #ifdef _DEBUG
      (*ppMtz)->numCasas++;
      (*ppMtz)->tamBytes += sizeof(tpCasaMatriz);
      (*ppMtz)->pPrimeiro->pCabeca = (*ppMtz);
      #endif

      for (linha = 0; linha < n; linha++) {

         if (linha == 0) {

            pCasaInicioLinha = (*ppMtz)->pPrimeiro;
            pCasaNorte = NULL;

         } else {
            pCasaAtual = CriarCasa();

            if (pCasaAtual == NULL) {
               MTZ_DestruirMatriz(ppMtz);
               return MTZ_CondRetFaltouMemoria;
            }
            #ifdef _DEBUG
            (*ppMtz)->numCasas++;
            pCasaAtual->pCabeca = (*ppMtz);
            (*ppMtz)->tamBytes += sizeof(tpCasaMatriz);
            #endif
            // Apontar a linha anterior como o norte da casa de inicio da nova linha, e vice-versa
            pCasaInicioLinha->pCasasAdjacentes[MTZ_DirSul] = pCasaAtual;
            pCasaAtual->pCasasAdjacentes[MTZ_DirNorte] = pCasaInicioLinha;
            // Apontar nordeste (e sudeste da linha de cima)
            // N�o ser� nulo, pois se passou para a segunda linha existe pelo menos 2 casas na linha de cima
            pCasaAtual->pCasasAdjacentes[MTZ_DirNordeste] = pCasaInicioLinha->pCasasAdjacentes[MTZ_DirLeste];
            pCasaInicioLinha->pCasasAdjacentes[MTZ_DirLeste]->pCasasAdjacentes[MTZ_DirSudoeste] = pCasaAtual;
            // A casa norte no inicio do loop de coluna ser� o norte da casa anterior, ou seja, o noroeste da nova casa a ser criada
            pCasaNorte = pCasaInicioLinha;
            // Agora o inicio da linha � a casa que acabou de ser criada
            pCasaInicioLinha = pCasaAtual;
         }
         pCasaOeste = pCasaInicioLinha;

         // Para cada coluna (elemento da primeira coluna j� foi criado)
         for (coluna = 1; coluna < n; coluna++) {

            pCasaAtual = CriarCasa();
            if (pCasaAtual == NULL) {
               MTZ_DestruirMatriz(ppMtz);
               return MTZ_CondRetFaltouMemoria;
            }
            #ifdef _DEBUG
            (*ppMtz)->numCasas++;
            pCasaAtual->pCabeca = (*ppMtz);
            (*ppMtz)->tamBytes += sizeof(tpCasaMatriz);
            #endif

            if (pCasaNorte != NULL) {
               // Preenche o noroeste
               pCasaAtual->pCasasAdjacentes[MTZ_DirNoroeste] = pCasaNorte;
               pCasaNorte->pCasasAdjacentes[MTZ_DirSudeste] = pCasaAtual;
               // Atualiza o norte e preenche (sempre dever� existir o leste do norte atual)
               pCasaNorte = pCasaNorte->pCasasAdjacentes[MTZ_DirLeste];
               pCasaNorte->pCasasAdjacentes[MTZ_DirSul] = pCasaAtual;
               pCasaAtual->pCasasAdjacentes[MTZ_DirNorte] = pCasaNorte;
               // Preenche a casa nordeste
               if (pCasaNorte->pCasasAdjacentes[MTZ_DirLeste] != NULL) {
                  tpCasaMatriz * pCasaNordeste = pCasaNorte->pCasasAdjacentes[MTZ_DirLeste];
                  pCasaNordeste->pCasasAdjacentes[MTZ_DirSudoeste] = pCasaAtual;
                  pCasaAtual->pCasasAdjacentes[MTZ_DirNordeste] = pCasaNordeste;
               }
            }
            // Preenche a casa oeste
            pCasaAtual->pCasasAdjacentes[MTZ_DirOeste] = pCasaOeste;
            pCasaOeste->pCasasAdjacentes[MTZ_DirLeste] = pCasaAtual;
            // Atualiza a casa oeste para a proxima itera��o
            pCasaOeste = pCasaAtual;
         }
      }

      // Apontar o ponteiro corrente para a primeira casa
      (*ppMtz)->pCasaCorr = (*ppMtz)->pPrimeiro;

      return MTZ_CondRetOK ;

   } /* Fim fun��o: MTZ Criar matriz */

/***************************************************************************
*
*  Fun��o: MTZ Destruir matriz
*  ****/

   MTZ_tpCondRet MTZ_DestruirMatriz( MTZ_tppMatriz * ppMtz ) {

      if ( ppMtz == NULL ) return MTZ_CondRetMatrizNaoExiste;
      
      if ( *ppMtz != NULL ) {
         if ( (*ppMtz)->pPrimeiro != NULL ) {
            DestroiMatriz( *ppMtz ) ;
         } /* if */

         #ifdef _DEBUG
         CED_Free(*ppMtz);
         #else
         free(*ppMtz);
         #endif
         *ppMtz = NULL ;
         return MTZ_CondRetOK;
      } /* if */

      return MTZ_CondRetMatrizNaoExiste;

   } /* Fim fun��o: MTZ Destruir matriz */

/***************************************************************************
*
*  Fun��o: MTZ Andar em Dire��o
*  ****/

   MTZ_tpCondRet MTZ_AndarDirecao( MTZ_tppMatriz pMtz, MTZ_tpDirecao direcao ) {
      
      // Tratar erro na estrutura
      if (pMtz == NULL) return MTZ_CondRetMatrizNaoExiste;

      // Tratar erro na dire��o
      if (direcao < 0 || direcao > 7) return MTZ_CondRetDirecaoNaoExisteOuInvalida;

      // Tratar se est� andando para uma dire��o que cont�m nulo
      if (pMtz->pCasaCorr->pCasasAdjacentes[direcao] == NULL) return MTZ_CondRetDirecaoNaoExisteOuInvalida;

      pMtz->pCasaCorr = pMtz->pCasaCorr->pCasasAdjacentes[direcao];
      return MTZ_CondRetOK;

   } /* Fim fun��o: MTZ Andar em Dire��o */

/***************************************************************************
*
*  Fun��o: MTZ Inserir elemento na casa corrente
*  ****/

   MTZ_tpCondRet MTZ_InserirElementoNaCasaCorrente( MTZ_tppMatriz pMtz, void * pElemento
                                                                                       #ifdef _DEBUG
                                                                                       , char * tipoConteudo, int tamBytes
                                                                                       #endif
                                                   ) {

      // Tratar se a matriz existe
      if (pMtz == NULL) return MTZ_CondRetMatrizNaoExiste;

      // Tratar se o ponteiro para o elemento � nulo
      if (pElemento == NULL) return MTZ_CondRetErroEstrutura;

      if (pMtz->pCasaCorr->conteudo != NULL){
         pMtz->ExcluirValor(pMtz->pCasaCorr->conteudo);
      }

      #ifdef _DEBUG
      strcpy_s(pMtz->pCasaCorr->tipoConteudo, TAM_CONTEUDO, tipoConteudo);
      pMtz->pCasaCorr->tamBytes = tamBytes;
      #endif

      pMtz->pCasaCorr->conteudo = pElemento;

      return MTZ_CondRetOK;

   } /* Fim fun��o: MTZ Inserir elemento na casa corrente */

/***************************************************************************
*
*  Fun��o: MTZ Obter valor corrente
*  ****/

   MTZ_tpCondRet MTZ_ObterValorCorrente( MTZ_tppMatriz pMtz, void ** valor ) {

	   if (pMtz == NULL) return MTZ_CondRetMatrizNaoExiste;

      if (valor == NULL) return MTZ_CondRetErroEstrutura;

	   if(pMtz->pCasaCorr->conteudo == NULL) return MTZ_CondRetCasaVazia;

	   *valor = pMtz->pCasaCorr->conteudo; 

	   return MTZ_CondRetOK;

   } /* Fim fun��o: MTZ Obter valor corrente */

/***************************************************************************
*
*  Fun��o: MTZ Voltar para o primeiro
*  ****/

   MTZ_tpCondRet MTZ_VoltarParaPrimeiro( MTZ_tppMatriz pMtz ) {

      if (pMtz == NULL) return MTZ_CondRetMatrizNaoExiste;

      pMtz->pCasaCorr = pMtz->pPrimeiro; 

      return MTZ_CondRetOK;

   } /* Fim fun��o: MTZ Voltar para o primeiro */

#ifdef _DEBUG

/***************************************************************************
*
*  Fun��o: MTZ Verificar uma matriz
*  ****/

   int MTZ_VerificarMatriz( void * pMatrizParm ) {

      tpMatriz * pMatriz = NULL ;
      tpCasaMatriz * pCasaInicioLinha = NULL;
      tpCasaMatriz * pCasaAtual = NULL;
      MTZ_tpCondRet condRetObtida = MTZ_CondRetOK;
      int linha = 0, coluna = 0;
      int contadorErros = 0;

      // Verifica cabe�a
      if (pMatrizParm == NULL) {
         contadorErros++;
         // N�o tem sentido continuar se o ponteiro cabe�a � NULL (vai voar)
         CNT_CONTAR( "MTZ_VerificarMatriz, pMatrizParm nulo" );
         return contadorErros;
      }

      CNT_CONTAR( "MTZ_VerificarMatriz, pMatrizParm ok" );

      pMatriz = ( tpMatriz * ) ( pMatrizParm ) ;

      if (pMatriz->tamBytes == 0) {
         contadorErros++;
         CNT_CONTAR( "MTZ_VerificarMatriz, tamBytes eh zero" );
      } else {
         CNT_CONTAR( "MTZ_VerificarMatriz, tamBytes maior que zero" );
      }

      if (pMatriz->numCasas == 0) {
         contadorErros++;
         CNT_CONTAR( "MTZ_VerificarMatriz, numCasas eh zero" );
      } else {
         CNT_CONTAR( "MTZ_VerificarMatriz, numCasas maior que zero" );
      }

      /* Verifica corrente */

      if ( pMatriz->pCasaCorr == NULL ) {
         contadorErros++;
         CNT_CONTAR( "MTZ_VerificarMatriz, pCasaCorr nulo" );
      } else {
         CNT_CONTAR( "MTZ_VerificarMatriz, pCasaCorr existe" );
      } /* if */

      /* Verifica primeiro da matriz */

      if ( pMatriz->pPrimeiro != NULL ) {
         CNT_CONTAR( "MTZ_VerificarMatriz, pPrimeiro existe" );

         // pr�ximos casos s� tem sentido se pPrimeiro existe, caso contrario voa
         if (pMatriz->pPrimeiro->pCasasAdjacentes[MTZ_DirNordeste] ||
               pMatriz->pPrimeiro->pCasasAdjacentes[MTZ_DirNorte] ||
               pMatriz->pPrimeiro->pCasasAdjacentes[MTZ_DirNoroeste] ||
               pMatriz->pPrimeiro->pCasasAdjacentes[MTZ_DirOeste] ||
               pMatriz->pPrimeiro->pCasasAdjacentes[MTZ_DirSudoeste] ) {
            contadorErros++;
            CNT_CONTAR( "MTZ_VerificarMatriz, pPrimeiro posicao errada" );
         } else {
            CNT_CONTAR( "MTZ_VerificarMatriz, pPrimeiro posicao ok" );
         }
         
         pCasaInicioLinha = pMatriz->pPrimeiro;
         // Percorrer todas as casas da linha
         do {
            pCasaAtual = pCasaInicioLinha;
            coluna = 0;
            // Percorrer todas as casas da coluna
            do {
               contadorErros += MTZ_VerificarElem( (void *) pCasaAtual );
               pCasaAtual = pCasaAtual->pCasasAdjacentes[MTZ_DirLeste];
               coluna++;
            } while (pCasaAtual != NULL) ;

            linha++;
            pCasaInicioLinha = pCasaInicioLinha->pCasasAdjacentes[MTZ_DirSul];
         } while (pCasaInicioLinha != NULL) ;

         // Verificar se tamanho condiz
         if (pMatriz->numCasas != linha*coluna) {
            contadorErros++;
            CNT_CONTAR( "MTZ_VerificarMatriz, numCasas diferente do esperado" );
         } else {
            CNT_CONTAR( "MTZ_VerificarMatriz, numCasas ok" );
         }

         if (pMatriz->tamBytes != linha*coluna*sizeof(tpCasaMatriz)) {
            contadorErros++;
            CNT_CONTAR( "MTZ_VerificarMatriz, tamBytes diferente do esperado" );
         } else {
            CNT_CONTAR( "MTZ_VerificarMatriz, tamBytes ok" );
         }

      } else {
         contadorErros++;
         CNT_CONTAR( "MTZ_VerificarMatriz, pPrimeiro nulo" );
      } /* if */

      return contadorErros;

   } /* Fim fun��o: MTZ Verificar uma matriz */

/***************************************************************************
*
*  Fun��o: MTZ Verificar uma casa de estrutura
*  ****/

   int MTZ_VerificarElem( void * pElemParm ) {

      tpCasaMatriz * pCasa = NULL ;
      tpMatriz * pMatriz = NULL ;
      int contadorErros = 0;

      /* Verificar se � casa estrutural */

      if ( pElemParm == NULL ) {
         contadorErros++;
         CNT_CONTAR( "MTZ_VerificarElem, pElemParm nulo" );
         // N�o tem sentido continuar se o ponteiro da casa � NULL (vai voar)
         return contadorErros ;
      } /* if */

      CNT_CONTAR( "MTZ_VerificarElem, pElemParm ok" );

      pCasa   = ( tpCasaMatriz * )( pElemParm ) ;
      pMatriz = pCasa->pCabeca ;

      /* Verificar cabe�a */

      if ( pCasa->pCabeca == pMatriz->pPrimeiro->pCabeca ) {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa eh da matriz" );
      } else {
         contadorErros++;
         CNT_CONTAR( "MTZ_VerificarElem, pCasa nao eh da matriz" );
      }

      /* Verificar norte */

      if ( pCasa->pCasasAdjacentes[MTZ_DirNorte] != NULL ) {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa tem norte" );
         if (pCasa != pCasa->pCasasAdjacentes[MTZ_DirNorte]->pCasasAdjacentes[MTZ_DirSul] ) {
            contadorErros++;
            CNT_CONTAR( "MTZ_VerificarElem, pCasa tem erro no norte" );
         } else {
            CNT_CONTAR( "MTZ_VerificarElem, pCasa norte ok" );
         }
      } else {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa nao tem norte" );
      } /* if */

      /* Verificar noroeste */

      if ( pCasa->pCasasAdjacentes[MTZ_DirNoroeste] != NULL ) {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa tem noroeste" );
         if (pCasa != pCasa->pCasasAdjacentes[MTZ_DirNoroeste]->pCasasAdjacentes[MTZ_DirSudeste] ) {
            contadorErros++;
            CNT_CONTAR( "MTZ_VerificarElem, pCasa tem erro no noroeste" );
         } else {
            CNT_CONTAR( "MTZ_VerificarElem, pCasa noroeste ok" );
         }
      } else {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa nao tem noroeste" );
      } /* if */

      /* Verificar oeste */

      if ( pCasa->pCasasAdjacentes[MTZ_DirOeste] != NULL ) {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa tem oeste" );
         if (pCasa != pCasa->pCasasAdjacentes[MTZ_DirOeste]->pCasasAdjacentes[MTZ_DirLeste] ) {
            contadorErros++;
            CNT_CONTAR( "MTZ_VerificarElem, pCasa tem erro no oeste" );
         } else {
            CNT_CONTAR( "MTZ_VerificarElem, pCasa oeste ok" );
         }
      } else {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa nao tem oeste" );
      } /* if */

      /* Verificar sul */

      if ( pCasa->pCasasAdjacentes[MTZ_DirSul] != NULL ) {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa tem sul" );
         if (pCasa != pCasa->pCasasAdjacentes[MTZ_DirSul]->pCasasAdjacentes[MTZ_DirNorte] ) {
            contadorErros++;
            CNT_CONTAR( "MTZ_VerificarElem, pCasa tem erro no sul" );
         } else {
            CNT_CONTAR( "MTZ_VerificarElem, pCasa sul ok" );
         }
      } else {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa nao tem sul" );
      } /* if */

      /* Verificar leste */

      if ( pCasa->pCasasAdjacentes[MTZ_DirLeste] != NULL ) {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa tem leste" );
         if (pCasa != pCasa->pCasasAdjacentes[MTZ_DirLeste]->pCasasAdjacentes[MTZ_DirOeste] ) {
            contadorErros++;
            CNT_CONTAR( "MTZ_VerificarElem, pCasa tem erro no leste" );
         } else {
            CNT_CONTAR( "MTZ_VerificarElem, pCasa leste ok" );
         }
      } else {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa nao tem leste" );
      } /* if */

      /* Verificar nordeste */

      if ( pCasa->pCasasAdjacentes[MTZ_DirNordeste] != NULL ) {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa tem nordeste" );
         if (pCasa != pCasa->pCasasAdjacentes[MTZ_DirNordeste]->pCasasAdjacentes[MTZ_DirSudoeste] ) {
            contadorErros++;
            CNT_CONTAR( "MTZ_VerificarElem, pCasa tem erro no nordeste" );
         } else {
            CNT_CONTAR( "MTZ_VerificarElem, pCasa nordeste ok" );
         }
      } else {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa nao tem nordeste" );
      } /* if */

      /* Verificar sudeste */

      if ( pCasa->pCasasAdjacentes[MTZ_DirSudeste] != NULL ) {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa tem sudeste" );
         if (pCasa != pCasa->pCasasAdjacentes[MTZ_DirSudeste]->pCasasAdjacentes[MTZ_DirNoroeste] ) {
            contadorErros++;
            CNT_CONTAR( "MTZ_VerificarElem, pCasa tem erro no sudeste" );
         } else {
            CNT_CONTAR( "MTZ_VerificarElem, pCasa sudeste ok" );
         }
      } else {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa nao tem sudeste" );
      } /* if */

      /* Verificar sudoeste */

      if ( pCasa->pCasasAdjacentes[MTZ_DirSudoeste] != NULL ) {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa tem sudoeste" );
         if (pCasa != pCasa->pCasasAdjacentes[MTZ_DirSudoeste]->pCasasAdjacentes[MTZ_DirNordeste] ) {
            contadorErros++;
            CNT_CONTAR( "MTZ_VerificarElem, pCasa tem erro no sudoeste" );
         } else {
            CNT_CONTAR( "MTZ_VerificarElem, pCasa sudoeste ok" );
         }
      } else {
         CNT_CONTAR( "MTZ_VerificarElem, pCasa nao tem sudoeste" );
      } /* if */

      // Verificar tipo conteudo
      if (pCasa->tamBytes != TamConteudo(pCasa->tipoConteudo)) {
         contadorErros++;
         CNT_CONTAR( "MTZ_VerificarElem, tipoConteudo errado" );
      } else {
         CNT_CONTAR( "MTZ_VerificarElem, tipoConteudo ok" );
      }

      return contadorErros;
   } /* Fim fun��o: MTZ Verificar uma casa de estrutura */

/***************************************************************************
*
*  Fun��o: MTZ Deturpar matriz
*  ****/

   void MTZ_Deturpar( void * pMatrizParm , MTZ_tpModosDeturpacao ModoDeturpar ) {

      tpMatriz * pMatriz = NULL ;
      int i;

      if ( pMatrizParm == NULL ) { return ; } /* if */

      pMatriz = ( tpMatriz * )( pMatrizParm ) ;

      switch ( ModoDeturpar ) {

      /* Elimina corrente */

         case DeturpaEliminaCorrente : {

            CED_Free(pMatriz->pCasaCorr) ;
            break ;
         } /* fim ativa: Elimina corrente */

      /* Anula ponteiro corrente */

         case DeturpaPtrCorrNulo : {

            pMatriz->pCasaCorr = NULL ;
            break ;
         } /* fim ativa: Anula ponteiro raiz */

      /* Anula ponteiro primeiro */

         case DeturpaPtrPrimNulo : {

            pMatriz->pPrimeiro = NULL ;
            break ;
         } /* fim ativa: Anula ponteiro corrente */

      /* Deturpa casa */

         default :

         if ( pMatriz->pCasaCorr != NULL ) {

            switch ( ModoDeturpar ) {

            /* Anula norte */

               case DeturpaPtrNorteNulo : {

                  pMatriz->pCasaCorr->pCasasAdjacentes[MTZ_DirNorte] = NULL ;
                  break ;
               } /* fim ativa: Anula norte */

            /* Anula sudeste */

               case DeturpaPtrSudesteNulo : {

                  pMatriz->pCasaCorr->pCasasAdjacentes[MTZ_DirSudeste] = NULL ;
                  break ;
               } /* fim ativa: Anula sudeste */

            /* Faz ponteiro norte apontar para lixo */

               case DeturpaPtrNorteLixo : {

                  pMatriz->pCasaCorr->pCasasAdjacentes[MTZ_DirNorte] = ( tpCasaMatriz * )( EspacoLixo ) ;
                  break ;
               } /* fim ativa: Faz ponteiro norte apontar para lixo */

            /* Faz ponteiro sudeste apontar para lixo */

               case DeturpaPtrSudesteLixo : {

                  pMatriz->pCasaCorr->pCasasAdjacentes[MTZ_DirSudeste] = ( tpCasaMatriz * )( EspacoLixo ) ;
                  break ;
               } /* fim ativa: Faz ponteiro sudeste apontar para lixo */

            /* Altera conteudo */

               case DeturpaAlteraTipoEstrutura : {

                  strcpy_s(pMatriz->pCasaCorr->tipoConteudo, 3, "INT") ;
                  break ;
               } /* fim ativa: Altera conteudo */

            /* Faz ponteiro sudeste apontar para lixo */

               case DeturpaDesencadeiaCasa : {

                  for (i = 0; i < 8; i++)
                     pMatriz->pCasaCorr->pCasasAdjacentes[i] = NULL ;
                  break ;
               } /* fim ativa: Faz ponteiro sudeste apontar para lixo */

            } /* fim seleciona: Deturpa n� */

            break ;

         } /* fim ativa: Deturpa n� */

      } /* fim seleciona: Raiz de ARV  &Deturpar �rvore */
   } /* Fim fun��o: MTZ Deturpar matriz */

#endif

/*****  C�digo das fun��es encapsuladas no m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: MTZ Criar casa da matriz
*
*  $FV Valor retornado
*     Ponteiro para a casa criada.
*     Ser� NULL caso a mem�ria tenha se esgotado.
*     Os ponteiros de dire��o da casa criada e o conteudo estar�o nulos.
*
***********************************************************************/

   tpCasaMatriz * CriarCasa(  ) {

	   int i = 0;
      tpCasaMatriz * pCasa ;

      #ifdef _DEBUG
      pCasa = ( tpCasaMatriz * ) CED_Malloc(sizeof( tpCasaMatriz ), __LINE__, __FILE__);
      #else
      pCasa = ( tpCasaMatriz * ) malloc( sizeof( tpCasaMatriz )) ;
      #endif
      
      if ( pCasa == NULL ) return NULL ;

      #ifdef _DEBUG
      CED_DefinirTipoEspaco( pCasa , MTZ_TipoEspacoCasa ) ;
      CED_MarcarEspacoAtivo( pCasa );
      pCasa->pCasasAdjacentes = ( tpCasaMatriz ** ) CED_Malloc(8 * sizeof( tpCasaMatriz * ), __LINE__, __FILE__);
      #else
      pCasa->pCasasAdjacentes = ( tpCasaMatriz ** ) malloc(8 * sizeof(tpCasaMatriz *));
      #endif

      if ( pCasa->pCasasAdjacentes == NULL ) {
         #ifdef _DEBUG
         CED_MarcarEspacoNaoAtivo(pCasa);
         CED_Free(pCasa); 
         #else
         free(pCasa);
         #endif
         return NULL ;
      } /* if */
      #ifdef _DEBUG
      CED_DefinirTipoEspaco( pCasa->pCasasAdjacentes , MTZ_TipoEspacoVetorCasas ) ;
      CED_MarcarEspacoAtivo(pCasa->pCasasAdjacentes);
      pCasa->tipoConteudo[0] = '\0';
      pCasa->tamBytes = 0;
      #endif

      // Preenche os ponteiros com nulos
      for (; i < 8; i++) {
         pCasa->pCasasAdjacentes[i] = NULL;
      }
      pCasa->conteudo = NULL ;
      return pCasa ;

   } /* Fim fun��o: MTZ Criar casa da matriz */


/***********************************************************************
*
*  $FC Fun��o: MTZ Destruir a estrutura da matriz
*
*  $EAE Assertivas de entradas esperadas
*     pMtz != NULL
*
***********************************************************************/

   void DestroiMatriz( MTZ_tppMatriz pMtz ) {

      tpCasaMatriz * pDestruir = NULL;

      // Destruir linha a linha
      while (pMtz->pPrimeiro != NULL) {
         // Marca que n�o h� casas a serem destru�das
         pDestruir = NULL;

         // Atualiza a casa corrente
         pMtz->pCasaCorr = pMtz->pPrimeiro;

         // Destruir cada coluna, exceto a primeira
         while(MTZ_AndarDirecao(pMtz, MTZ_DirLeste) != MTZ_CondRetDirecaoNaoExisteOuInvalida) {

            // Destroi se houver casa marcada
            if (pDestruir != NULL) {
               DestroiCasa(pDestruir, pMtz->ExcluirValor);
            }

            // Marca a casa para ser destru�da na pr�xima itera��o ou no fim da coluna
            pDestruir = pMtz->pCasaCorr;
         }
		    
         // Destroi se houver uma marcada
         if (pDestruir != NULL) {
            DestroiCasa(pDestruir, pMtz->ExcluirValor);
         }

         // Atualiza para come�ar a destruir a pr�xima linha e destroi a primeira casa da coluna
         pDestruir = pMtz->pPrimeiro;
		 
         pMtz->pPrimeiro = pMtz->pPrimeiro->pCasasAdjacentes[MTZ_DirSul];
         
         DestroiCasa(pDestruir, pMtz->ExcluirValor);
      }

   } /* Fim fun��o: MTZ Destruir a estrutura da matriz */

/***********************************************************************
*
*  $FC Fun��o: MTZ Destruir uma casa da matriz
*
*  $EAE Assertivas de entradas esperadas
*     pCasa != NULL
*     ExcluirValor != NULL
*
***********************************************************************/

   void DestroiCasa( tpCasaMatriz * pCasa, void ( * ExcluirValor ) ( void * pValor ) ) {

      if (pCasa->conteudo != NULL) {
         ExcluirValor(pCasa->conteudo);
      }
      #ifdef _DEBUG
      CED_MarcarEspacoNaoAtivo(pCasa->pCasasAdjacentes);
      CED_MarcarEspacoNaoAtivo(pCasa);
      CED_Free(pCasa->pCasasAdjacentes);
      CED_Free(pCasa);
      #else
      free(pCasa->pCasasAdjacentes);
      free(pCasa);
      #endif
      pCasa = NULL;

   } /* Fim fun��o: MTZ Destruir uma casa da matriz */

#ifdef _DEBUG

/***********************************************************************
*
*  $FC Fun��o: MTZ Tamanho de um conte�do
*
***********************************************************************/

int TamConteudo(const char * nome) {

   if (strcmp( nome, "INT" ) == 0) {
      return sizeof(int);
   } else if (strcmp( nome, "CHAR" ) == 0) {
      return sizeof(char);
   } else if (strcmp( nome, "SHORT" ) == 0) {
      return sizeof(short);
   } else if (strcmp( nome, "LONG" ) == 0) {
      return sizeof(long);
   } else {
      return 0;
   }
} /* Fim fun��o: MTZ Tamanho de um conte�do */

#endif

/********** Fim do m�dulo de implementa��o: M�dulo matriz **********/

