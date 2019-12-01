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
#include "CESPDIN.H"
#include "CONTA.H"
#endif

typedef struct tgMatriz tpMatriz;

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
         char tipoConteudo[20];
               /* Tipo do conteúdo presente na casa */

         tpMatriz * pCabeca;
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

   struct tgMatriz {
       
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

   } ;

/*****  Dados encapsulados no módulo  *****/

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

      #ifdef _DEBUG
      CNT_Contar("MTZ_CriarMatriz, inicio", __LINE__); 
      #endif

      if (n <= 0) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_CriarMatriz, tam invalido", __LINE__); 
         #endif
         return MTZ_CondRetErroEstrutura;
      }

      #ifdef _DEBUG
      CNT_Contar("MTZ_CriarMatriz, tam valido", __LINE__); 
      #endif

      if (ppMtz == NULL) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_CriarMatriz, ponteiro para matriz inexistente", __LINE__); 
         #endif
         return MTZ_CondRetErroEstrutura;
      }

      #ifdef _DEBUG
      CNT_Contar("MTZ_CriarMatriz, ponteiro ppMtz ok", __LINE__); 
      #endif

      // Se já havia uma matriz anteriormente, destrua-a primeiro
      if (*ppMtz != NULL) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_CriarMatriz, ja havia matriz em ppMtz", __LINE__); 
         #endif
         MTZ_DestruirMatriz(ppMtz);
      }
      #ifdef _DEBUG
      else
         CNT_Contar("MTZ_CriarMatriz, ppMtz nao continha matriz", __LINE__); 
      #endif

      // Alocar espaço para a head
      #ifdef _DEBUG
      *ppMtz = ( tpMatriz * ) CED_Malloc(sizeof( tpMatriz ), __LINE__, __FILE__);
      #else
      *ppMtz = ( tpMatriz * ) malloc( sizeof( tpMatriz )) ;
      #endif

      if (*ppMtz == NULL) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_CriarMatriz, faltou memoria", __LINE__); 
         #endif
         return MTZ_CondRetFaltouMemoria;
      }
      #ifdef _DEBUG
      CNT_Contar("MTZ_CriarMatriz, espaco para matriz ok", __LINE__); 
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
         CNT_Contar("MTZ_CriarMatriz, nao teve espaco para primeira casa", __LINE__);
         CED_Free(*ppMtz);
         #else
         free(*ppMtz);
         #endif
         *ppMtz = NULL;
         return MTZ_CondRetFaltouMemoria;
      }
      #ifdef _DEBUG
      CNT_Contar("MTZ_CriarMatriz, espaco para primeira casa ok", __LINE__);
      (*ppMtz)->numCasas++; 
      #endif

      for (linha = 0; linha < n; linha++) {

         if (linha == 0) {
            #ifdef _DEBUG
            CNT_Contar("MTZ_CriarMatriz, primeira linha da matriz", __LINE__); 
            #endif
            pCasaInicioLinha = (*ppMtz)->pPrimeiro;
            pCasaNorte = NULL;            

         } else {
            #ifdef _DEBUG
            CNT_Contar("MTZ_CriarMatriz, nao eh primeira linha da matriz", __LINE__); 
            #endif
            pCasaAtual = CriarCasa();

            if (pCasaAtual == NULL) {
               #ifdef _DEBUG
               CNT_Contar("MTZ_CriarMatriz, faltou memoria para casa", __LINE__); 
               #endif
               MTZ_DestruirMatriz(ppMtz);
               return MTZ_CondRetFaltouMemoria;
            }
            #ifdef _DEBUG
            CNT_Contar("MTZ_CriarMatriz, primeira casa da linha alocada", __LINE__);
            (*ppMtz)->numCasas++;
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
               #ifdef _DEBUG
               CNT_Contar("MTZ_CriarMatriz, faltou memoria para casa", __LINE__); 
               #endif
               MTZ_DestruirMatriz(ppMtz);
               return MTZ_CondRetFaltouMemoria;
            }
            #ifdef _DEBUG
            CNT_Contar("MTZ_CriarMatriz, casa alocada na coluna", __LINE__);
            (*ppMtz)->numCasas++;
            #endif

            if (pCasaNorte != NULL) {
               #ifdef _DEBUG
               CNT_Contar("MTZ_CriarMatriz, tem casa a norte", __LINE__); 
               #endif
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
                  #ifdef _DEBUG
                  CNT_Contar("MTZ_CriarMatriz, tem nordeste", __LINE__); 
                  #endif
               }
               #ifdef _DEBUG
               else
                  CNT_Contar("MTZ_CriarMatriz, nao tem nordeste", __LINE__); 
               #endif
            }
            #ifdef _DEBUG
            else
               CNT_Contar("MTZ_CriarMatriz, casa na primeira linha", __LINE__); 
            #endif
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

      #ifdef _DEBUG
      CNT_Contar("MTZ_DestruirMatriz, inicio", __LINE__); 
      #endif

      if ( ppMtz == NULL ) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_DestruirMatriz, matriz inexistente", __LINE__); 
         #endif
         return MTZ_CondRetMatrizNaoExiste;
      }
      #ifdef _DEBUG
      CNT_Contar("MTZ_DestruirMatriz, matriz existe", __LINE__); 
      #endif
      if ( *ppMtz != NULL ) {
         if ( (*ppMtz)->pPrimeiro != NULL ) {
            #ifdef _DEBUG
            CNT_Contar("MTZ_DestruirMatriz, matriz inexistente", __LINE__); 
            #endif
            DestroiMatriz( *ppMtz ) ;
         } /* if */
         #ifdef _DEBUG
         else
            CNT_Contar("MTZ_DestruirMatriz, pPrimeiro eh NULL", __LINE__); 

         CED_Free(*ppMtz);
         #else
         free(*ppMtz);
         #endif
         *ppMtz = NULL ;
         return MTZ_CondRetOK;
      } /* if */
      #ifdef _DEBUG
      CNT_Contar("MTZ_DestruirMatriz, pPrimeiro eh NULL", __LINE__); 
      #endif
      return MTZ_CondRetMatrizNaoExiste;

   } /* Fim função: MTZ Destruir matriz */

/***************************************************************************
*
*  Função: MTZ Andar em Direção
*  ****/

   MTZ_tpCondRet MTZ_AndarDirecao( MTZ_tppMatriz pMtz, MTZ_tpDirecao direcao ) {
      
      #ifdef _DEBUG
      CNT_Contar("MTZ_AndarDirecao, inicio", __LINE__);
      #endif
      // Tratar erro na estrutura
      if (pMtz == NULL) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_AndarDirecao, matriz inexistente", __LINE__);
         #endif
         return MTZ_CondRetMatrizNaoExiste;
      }
      #ifdef _DEBUG
      CNT_Contar("MTZ_AndarDirecao, matriz existe", __LINE__);
      #endif

      // Tratar erro na direção
      if (direcao < 0 || direcao > 7) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_AndarDirecao, direcao inexistente", __LINE__);
         #endif
         return MTZ_CondRetDirecaoNaoExisteOuInvalida;
      }
      #ifdef _DEBUG
      CNT_Contar("MTZ_AndarDirecao, direcao existe", __LINE__);
      #endif

      // Tratar se está andando para uma direção que contém nulo
      if (pMtz->pCasaCorr->pCasasAdjacentes[direcao] == NULL) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_AndarDirecao, direcao invalida", __LINE__);
         #endif
         return MTZ_CondRetDirecaoNaoExisteOuInvalida;
      }
      #ifdef _DEBUG
      CNT_Contar("MTZ_AndarDirecao, direcao valida", __LINE__);
      #endif

      pMtz->pCasaCorr = pMtz->pCasaCorr->pCasasAdjacentes[direcao];
      return MTZ_CondRetOK;

   } /* Fim função: MTZ Andar em Direção */

/***************************************************************************
*
*  Função: MTZ Inserir elemento na casa corrente
*  ****/

   MTZ_tpCondRet MTZ_InserirElementoNaCasaCorrente( MTZ_tppMatriz pMtz, void * pElemento ) {

      #ifdef _DEBUG
      CNT_Contar("MTZ_InserirElementoNaCasaCorrente, inicio", __LINE__);
      #endif
      // Tratar se a matriz existe
      if (pMtz == NULL) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_InserirElementoNaCasaCorrente, matriz inexistente", __LINE__);
         #endif
         return MTZ_CondRetMatrizNaoExiste;
      }
      #ifdef _DEBUG
      CNT_Contar("MTZ_InserirElementoNaCasaCorrente, matriz existe", __LINE__);
      #endif
      // Tratar se o ponteiro para o elemento é nulo
      if (pElemento == NULL) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_InserirElementoNaCasaCorrente, ponteiro para elemento eh nulo", __LINE__);
         #endif
         return MTZ_CondRetErroEstrutura;
      }
      #ifdef _DEBUG
      CNT_Contar("MTZ_InserirElementoNaCasaCorrente, pElemento tem conteudo", __LINE__);
      #endif
      if (pMtz->pCasaCorr->conteudo != NULL){
         #ifdef _DEBUG
         CNT_Contar("MTZ_InserirElementoNaCasaCorrente, casa corrente possuia conteudo", __LINE__);
         #endif
         pMtz->ExcluirValor(pMtz->pCasaCorr->conteudo);
      }
      #ifdef _DEBUG
      else
         CNT_Contar("MTZ_InserirElementoNaCasaCorrente, casa corrente previamente vazia", __LINE__);
      #endif

      pMtz->pCasaCorr->conteudo = pElemento;

      return MTZ_CondRetOK;
   } /* Fim função: MTZ Inserir elemento na casa corrente */

/***************************************************************************
*
*  Função: MTZ Obter valor corrente
*  ****/

   MTZ_tpCondRet MTZ_ObterValorCorrente( MTZ_tppMatriz pMtz, void ** valor ) {

      #ifdef _DEBUG
      CNT_Contar("MTZ_ObterValorCorrente, inicio", __LINE__);
      #endif

	   if (pMtz == NULL) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_ObterValorCorrente, matriz inexistente", __LINE__);
         #endif
         return MTZ_CondRetMatrizNaoExiste;
      }
      #ifdef _DEBUG
      CNT_Contar("MTZ_ObterValorCorrente, matriz existe", __LINE__);
      #endif

      if (valor == NULL) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_ObterValorCorrente, ponteiro para valor inexistente", __LINE__);
         #endif
         return MTZ_CondRetErroEstrutura;
      }
      #ifdef _DEBUG
      CNT_Contar("MTZ_ObterValorCorrente, ponteiro para valor existe", __LINE__);
      #endif

	   if(pMtz->pCasaCorr->conteudo == NULL) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_ObterValorCorrente, casa corrente sem conteudo", __LINE__);
         #endif
         return MTZ_CondRetCasaVazia;
      }
      #ifdef _DEBUG
      CNT_Contar("MTZ_ObterValorCorrente, casa corrente possui conteudo", __LINE__);
      #endif

	   *valor = pMtz->pCasaCorr->conteudo; 

	   return MTZ_CondRetOK;

   } /* Fim função: MTZ Obter valor corrente */

/***************************************************************************
*
*  Função: MTZ Voltar para o primeiro
*  ****/

   MTZ_tpCondRet MTZ_VoltarParaPrimeiro( MTZ_tppMatriz pMtz ) {

      #ifdef _DEBUG
      CNT_Contar("MTZ_VoltarParaPrimeiro, inicio", __LINE__); 
      #endif

      if (pMtz == NULL) {
         #ifdef _DEBUG
         CNT_Contar("MTZ_VoltarParaPrimeiro, matriz inexistente", __LINE__); 
         #endif
         return MTZ_CondRetMatrizNaoExiste;
      }
      #ifdef _DEBUG
      CNT_Contar("MTZ_VoltarParaPrimeiro, matriz existe", __LINE__); 
      #endif

      pMtz->pCasaCorr = pMtz->pPrimeiro; 

      return MTZ_CondRetOK;

   } /* Fim função: MTZ Voltar para o primeiro */


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
      CNT_Contar("CriarCasa, inicio", __LINE__);
      pCasa = ( tpCasaMatriz * ) CED_Malloc(sizeof( tpCasaMatriz ), __LINE__, __FILE__);
      #else
      pCasa = ( tpCasaMatriz * ) malloc( sizeof( tpCasaMatriz )) ;
      #endif
      
      if ( pCasa == NULL ) {
         #ifdef _DEBUG
         CNT_Contar("CriarCasa, faltou memoria para casa", __LINE__); 
         #endif
         return NULL ;
      } /* if */
      #ifdef _DEBUG
      CNT_Contar("CriarCasa, memoria para casa ok", __LINE__); 

      pCasa->pCasasAdjacentes = ( tpCasaMatriz ** ) CED_Malloc(8 * sizeof( tpCasaMatriz * ), __LINE__, __FILE__);
      #else
      pCasa->pCasasAdjacentes = ( tpCasaMatriz ** ) malloc(8 * sizeof(tpCasaMatriz *));
      #endif

      if ( pCasa->pCasasAdjacentes == NULL ) {
         #ifdef _DEBUG
         CNT_Contar("CriarCasa, faltou memoria para casa", __LINE__);
         CED_Free(pCasa); 
         #else
         free(pCasa);
         #endif
         return NULL ;
      } /* if */
      #ifdef _DEBUG
      CNT_Contar("CriarCasa, memoria para adjacentes ok", __LINE__);
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

      #ifdef _DEBUG
      CNT_Contar("DestroiMatriz, inicio", __LINE__);
      #endif

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
               #ifdef _DEBUG
               CNT_Contar("DestroiMatriz, ponteiro a destruir existe", __LINE__);
               #endif
               DestroiCasa(pDestruir, pMtz->ExcluirValor);
            }
            #ifdef _DEBUG
            else
               CNT_Contar("DestroiMatriz, ponteiro a destruir nulo", __LINE__);
            #endif
            // Marca a casa para ser destruída na próxima iteração ou no fim da coluna
            pDestruir = pMtz->pCasaCorr;
         }
		    
         // Destroi se houver uma marcada
         if (pDestruir != NULL) {
            #ifdef _DEBUG
            CNT_Contar("DestroiMatriz, ponteiro a destruir existe", __LINE__);
            #endif
            DestroiCasa(pDestruir, pMtz->ExcluirValor);
         }
         #ifdef _DEBUG
         else
            CNT_Contar("DestroiMatriz, ponteiro a destruir nulo", __LINE__);
         #endif
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

      #ifdef _DEBUG
      CNT_Contar("DestroiCasa, inicio", __LINE__);
      #endif
      if (pCasa->conteudo != NULL) {
         #ifdef _DEBUG
         CNT_Contar("DestroiCasa, conteudo nao nulo sera destruido", __LINE__);
         #endif
         ExcluirValor(pCasa->conteudo);
      }
      #ifdef _DEBUG
      else
         CNT_Contar("DestroiCasa, conteudo nulo", __LINE__);
      CED_Free(pCasa->pCasasAdjacentes);
      CED_Free(pCasa);
      #else
      free(pCasa->pCasasAdjacentes);
      free(pCasa);
      #endif
      pCasa = NULL;

   } /* Fim função: MTZ Destruir uma casa da matriz */

/********** Fim do módulo de implementação: Módulo matriz **********/

