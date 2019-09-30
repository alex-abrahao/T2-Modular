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
*
***************************************************************************/

#include   <malloc.h>
#include   <stdio.h>

#define MATRIZ_OWN
#include "MATRIZ.H"
#undef MATRIZ_OWN

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
       
   } tpCasaMatriz ;

/***********************************************************************
*
*  $TC Tipo de dados: MTZ Descritor da cabeça de uma matriz
*
*
*  $ED Descrição do tipo
*     A cabeça da matriz é o ponto de acesso para uma determinada matriz.
*     Por intermédio da referência para a casa corrente pode-se navegar
*	  pela matriz.
*	  Contém a referência para a função que destroi o conteudo de uma casa.
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

   } tpMatriz ;

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

      if (n <= 0) return MTZ_CondRetErroEstrutura;

      if (ppMtz == NULL)
         return MTZ_CondRetErroEstrutura;

      // Se já havia uma matriz anteriormente, destrua-a primeiro
      if (*ppMtz != NULL)
         MTZ_DestruirMatriz(ppMtz);

      // Alocar espaço para a head
      *ppMtz = ( tpMatriz * ) malloc( sizeof( tpMatriz )) ;
      if (*ppMtz == NULL)
         return MTZ_CondRetFaltouMemoria;

      // Setup da head
      (*ppMtz)->id = 0;
      (*ppMtz)->ExcluirValor = ExcluirValor;
      (*ppMtz)->pCasaCorr = NULL;

      // Criar a primeira casa
      (*ppMtz)->pPrimeiro = CriarCasa();
      if ((*ppMtz)->pPrimeiro == NULL) {
         // Libera a head
         free(*ppMtz);
         *ppMtz = NULL;
         return MTZ_CondRetFaltouMemoria;
      }

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
         free( *ppMtz ) ;
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
      if (pMtz == NULL)
         return MTZ_CondRetMatrizNaoExiste;

      // Tratar erro na direção
      if (direcao < 0 || direcao > 7)
         return MTZ_CondRetDirecaoNaoExisteOuInvalida;

      // Tratar se está andando para uma direção que contém nulo
      if (pMtz->pCasaCorr->pCasasAdjacentes[direcao] == NULL)
         return MTZ_CondRetDirecaoNaoExisteOuInvalida;

      pMtz->pCasaCorr = pMtz->pCasaCorr->pCasasAdjacentes[direcao];
      return MTZ_CondRetOK;

   } /* Fim função: MTZ Andar em Direção */

/***************************************************************************
*
*  Função: MTZ Inserir elemento na casa corrente
*  ****/


   MTZ_tpCondRet MTZ_InserirElementoNaCasaCorrente( MTZ_tppMatriz pMtz, void * pElemento ) {

      // Tratar se a matriz existe
      if (pMtz == NULL)
         return MTZ_CondRetMatrizNaoExiste;
      // Tratar se o ponteiro para o elemento é nulo
      if (pElemento == NULL)
         return MTZ_CondRetErroEstrutura;
      if (pMtz->pCasaCorr->conteudo != NULL)
         pMtz->ExcluirValor(pMtz->pCasaCorr->conteudo);
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

      pCasa = ( tpCasaMatriz * ) malloc( sizeof( tpCasaMatriz )) ;
      if ( pCasa == NULL ) {
         return NULL ;
      } /* if */

	  pCasa->pCasasAdjacentes = ( tpCasaMatriz ** ) malloc(8 * sizeof(tpCasaMatriz *));

	  if ( pCasa->pCasasAdjacentes == NULL ) {
		 free(pCasa);
         return NULL ;
      } /* if */

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
            if (pDestruir != NULL)
               DestroiCasa(pDestruir, pMtz->ExcluirValor);

            // Marca a casa para ser destruída na próxima iteração ou no fim da coluna
            pDestruir = pMtz->pCasaCorr;
         }
		    
         // Destroi se houver uma marcada
         if (pDestruir != NULL)
          DestroiCasa(pDestruir, pMtz->ExcluirValor);

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

	  if (pCasa->conteudo != NULL)
		ExcluirValor(pCasa->conteudo);
	  free(pCasa->pCasasAdjacentes);
      free(pCasa);
      pCasa = NULL;

   } /* Fim função: MTZ Destruir uma casa da matriz */

/********** Fim do módulo de implementação: Módulo matriz **********/

