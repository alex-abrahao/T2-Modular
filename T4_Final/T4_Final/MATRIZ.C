/***************************************************************************
*  $MCI Módulo de implementação: Módulo matriz
*
*  Arquivo gerado:              MATRIZ.C
*  Letras identificadoras:      MTZ
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
*       2.00   aaf   30/11/2019 Início da instrumentação
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
*  $ED Descrição do tipo
*     Descreve a organização de uma casa da matriz
*
***********************************************************************/

   typedef struct tgCasaMatriz {

         struct tgCasaMatriz ** pCasasAdjacentes ;
               /* Vetor de ponteiros para as casas adjacentes */
       
         void * conteudo ;
               /* Conteúdo da casa */

         #ifdef _DEBUG
         char tipoConteudo[TAM_CONTEUDO];
               /* Tipo do conteúdo presente na casa */

         MTZ_tppMatriz pCabeca;
               /* Ponteiro para cabeça da matriz */

         int tamBytes;
               /* Tamanho total do conteúdo da casa, em bytes */
         #endif
       
   } tpCasaMatriz ;

/***********************************************************************
*
*  $TC Tipo de dados: MTZ Descritor da cabeça de uma matriz
*
*
*  $ED Descrição do tipo
*     A cabeça da matriz é o ponto de acesso para uma determinada matriz.
*     Por intermédio da referência para a casa corrente pode-se navegar
*     pela matriz.
*     Contém a referência para a função que destroi o conteudo de uma casa.
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
               /* Ponteiro para a função de destruição do valor contido em um elemento */

         #ifdef _DEBUG
         int numCasas;
               /* Número de casas alocadas na matriz */

         int tamBytes;
               /* Tamanho total da estrutura, em bytes */
         #endif

   } tpMatriz ;

/*****  Dados encapsulados no módulo  *****/

#ifdef _DEBUG

static char EspacoLixo[ 256 ] =
       "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" ;
      /* Espaço de dados lixo usado ao testar */
#endif

/***** Protótipos das funções encapuladas no módulo *****/

   static tpCasaMatriz * CriarCasa( ) ;

   static void DestroiMatriz( MTZ_tppMatriz pMtz ) ;

   static void DestroiCasa( tpCasaMatriz * pCasa, void ( * ExcluirValor ) ( void * pValor ) ) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: MTZ Criar matriz
*  ****/

   MTZ_tpCondRet MTZ_CriarMatriz(MTZ_tppMatriz * ppMtz, int n, void ( * ExcluirValor ) ( void * pValor ) ) {

      int linha, coluna;
      // Só é preciso setar os ponteiros para as casas oeste, norte, noroeste e nordeste, assim como
      // setar nessas casas as direcoes inversas para o ponteiro da casa atual que está sendo criada.
      tpCasaMatriz * pCasaOeste, * pCasaInicioLinha, * pCasaAtual, * pCasaNorte;

      if (n <= 0) return MTZ_CondRetErroEstrutura;

      if (ppMtz == NULL) return MTZ_CondRetErroEstrutura;

      // Se já havia uma matriz anteriormente, destrua-a primeiro
      if (*ppMtz != NULL) {
         MTZ_DestruirMatriz(ppMtz);
      }

      // Alocar espaço para a head
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
            // Não será nulo, pois se passou para a segunda linha existe pelo menos 2 casas na linha de cima
            pCasaAtual->pCasasAdjacentes[MTZ_DirNordeste] = pCasaInicioLinha->pCasasAdjacentes[MTZ_DirLeste];
            pCasaInicioLinha->pCasasAdjacentes[MTZ_DirLeste]->pCasasAdjacentes[MTZ_DirSudoeste] = pCasaAtual;
            // A casa norte no inicio do loop de coluna será o norte da casa anterior, ou seja, o noroeste da nova casa a ser criada
            pCasaNorte = pCasaInicioLinha;
            // Agora o inicio da linha é a casa que acabou de ser criada
            pCasaInicioLinha = pCasaAtual;
         }
         pCasaOeste = pCasaInicioLinha;

         // Para cada coluna (elemento da primeira coluna já foi criado)
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
               // Atualiza o norte e preenche (sempre deverá existir o leste do norte atual)
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
            // Atualiza a casa oeste para a proxima iteração
            pCasaOeste = pCasaAtual;
         }
      }

      // Apontar o ponteiro corrente para a primeira casa
      (*ppMtz)->pCasaCorr = (*ppMtz)->pPrimeiro;

      return MTZ_CondRetOK ;

   } /* Fim função: MTZ Criar matriz */

/***************************************************************************
*
*  Função: MTZ Destruir matriz
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

   } /* Fim função: MTZ Destruir matriz */

/***************************************************************************
*
*  Função: MTZ Andar em Direção
*  ****/

   MTZ_tpCondRet MTZ_AndarDirecao( MTZ_tppMatriz pMtz, MTZ_tpDirecao direcao ) {
      
      // Tratar erro na estrutura
      if (pMtz == NULL) return MTZ_CondRetMatrizNaoExiste;

      // Tratar erro na direção
      if (direcao < 0 || direcao > 7) return MTZ_CondRetDirecaoNaoExisteOuInvalida;

      // Tratar se está andando para uma direção que contém nulo
      if (pMtz->pCasaCorr->pCasasAdjacentes[direcao] == NULL) return MTZ_CondRetDirecaoNaoExisteOuInvalida;

      pMtz->pCasaCorr = pMtz->pCasaCorr->pCasasAdjacentes[direcao];
      return MTZ_CondRetOK;

   } /* Fim função: MTZ Andar em Direção */

/***************************************************************************
*
*  Função: MTZ Inserir elemento na casa corrente
*  ****/

   MTZ_tpCondRet MTZ_InserirElementoNaCasaCorrente( MTZ_tppMatriz pMtz, void * pElemento
                                                                                       #ifdef _DEBUG
                                                                                       , char * tipoConteudo, int tamBytes
                                                                                       #endif
                                                   ) {

      // Tratar se a matriz existe
      if (pMtz == NULL) return MTZ_CondRetMatrizNaoExiste;

      // Tratar se o ponteiro para o elemento é nulo
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

   } /* Fim função: MTZ Inserir elemento na casa corrente */

/***************************************************************************
*
*  Função: MTZ Obter valor corrente
*  ****/

   MTZ_tpCondRet MTZ_ObterValorCorrente( MTZ_tppMatriz pMtz, void ** valor ) {

	   if (pMtz == NULL) return MTZ_CondRetMatrizNaoExiste;

      if (valor == NULL) return MTZ_CondRetErroEstrutura;

	   if(pMtz->pCasaCorr->conteudo == NULL) return MTZ_CondRetCasaVazia;

	   *valor = pMtz->pCasaCorr->conteudo; 

	   return MTZ_CondRetOK;

   } /* Fim função: MTZ Obter valor corrente */

/***************************************************************************
*
*  Função: MTZ Voltar para o primeiro
*  ****/

   MTZ_tpCondRet MTZ_VoltarParaPrimeiro( MTZ_tppMatriz pMtz ) {

      if (pMtz == NULL) return MTZ_CondRetMatrizNaoExiste;

      pMtz->pCasaCorr = pMtz->pPrimeiro; 

      return MTZ_CondRetOK;

   } /* Fim função: MTZ Voltar para o primeiro */

#ifdef _DEBUG

/***************************************************************************
*
*  Função: MTZ Verificar uma matriz
*  ****/

   int MTZ_VerificarMatriz( void * pMatrizParm ) {

      tpMatriz * pMatriz = NULL ;
      tpCasaMatriz * pCasaInicioLinha = NULL;
      tpCasaMatriz * pCasaAtual = NULL;
      MTZ_tpCondRet condRetObtida = MTZ_CondRetOK;
      int linha = 0, coluna = 0;
      int contadorErros = 0;

      // Verifica cabeça
      if (pMatrizParm == NULL) {
         contadorErros++;
         // Não tem sentido continuar se o ponteiro cabeça é NULL (vai voar)
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

         // próximos casos só tem sentido se pPrimeiro existe, caso contrario voa
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

   } /* Fim função: MTZ Verificar uma matriz */

/***************************************************************************
*
*  Função: MTZ Verificar uma casa de estrutura
*  ****/

   int MTZ_VerificarElem( void * pElemParm ) {

      tpCasaMatriz * pCasa = NULL ;
      tpMatriz * pMatriz = NULL ;
      int contadorErros = 0;

      /* Verificar se é casa estrutural */

      if ( pElemParm == NULL ) {
         contadorErros++;
         CNT_CONTAR( "MTZ_VerificarElem, pElemParm nulo" );
         // Não tem sentido continuar se o ponteiro da casa é NULL (vai voar)
         return contadorErros ;
      } /* if */

      CNT_CONTAR( "MTZ_VerificarElem, pElemParm ok" );

      pCasa   = ( tpCasaMatriz * )( pElemParm ) ;
      pMatriz = pCasa->pCabeca ;

      /* Verificar cabeça */

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

      return contadorErros;
   } /* Fim função: MTZ Verificar uma casa de estrutura */

/***************************************************************************
*
*  Função: MTZ Deturpar matriz
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

            } /* fim seleciona: Deturpa nó */

            break ;

         } /* fim ativa: Deturpa nó */

      } /* fim seleciona: Raiz de ARV  &Deturpar árvore */
   } /* Fim função: MTZ Deturpar matriz */

#endif

/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: MTZ Criar casa da matriz
*
*  $FV Valor retornado
*     Ponteiro para a casa criada.
*     Será NULL caso a memória tenha se esgotado.
*     Os ponteiros de direção da casa criada e o conteudo estarão nulos.
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

   } /* Fim função: MTZ Criar casa da matriz */


/***********************************************************************
*
*  $FC Função: MTZ Destruir a estrutura da matriz
*
*  $EAE Assertivas de entradas esperadas
*     pMtz != NULL
*
***********************************************************************/

   void DestroiMatriz( MTZ_tppMatriz pMtz ) {

      tpCasaMatriz * pDestruir = NULL;

      // Destruir linha a linha
      while (pMtz->pPrimeiro != NULL) {
         // Marca que não há casas a serem destruídas
         pDestruir = NULL;

         // Atualiza a casa corrente
         pMtz->pCasaCorr = pMtz->pPrimeiro;

         // Destruir cada coluna, exceto a primeira
         while(MTZ_AndarDirecao(pMtz, MTZ_DirLeste) != MTZ_CondRetDirecaoNaoExisteOuInvalida) {

            // Destroi se houver casa marcada
            if (pDestruir != NULL) {
               DestroiCasa(pDestruir, pMtz->ExcluirValor);
            }

            // Marca a casa para ser destruída na próxima iteração ou no fim da coluna
            pDestruir = pMtz->pCasaCorr;
         }
		    
         // Destroi se houver uma marcada
         if (pDestruir != NULL) {
            DestroiCasa(pDestruir, pMtz->ExcluirValor);
         }

         // Atualiza para começar a destruir a próxima linha e destroi a primeira casa da coluna
         pDestruir = pMtz->pPrimeiro;
		 
         pMtz->pPrimeiro = pMtz->pPrimeiro->pCasasAdjacentes[MTZ_DirSul];
         
         DestroiCasa(pDestruir, pMtz->ExcluirValor);
      }

   } /* Fim função: MTZ Destruir a estrutura da matriz */

/***********************************************************************
*
*  $FC Função: MTZ Destruir uma casa da matriz
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

   } /* Fim função: MTZ Destruir uma casa da matriz */

/********** Fim do módulo de implementação: Módulo matriz **********/

