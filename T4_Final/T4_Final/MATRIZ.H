#if ! defined( MATRIZ_ )
#define MATRIZ_
/***************************************************************************
*
*  $MCD Módulo de definição: Módulo matriz
*
*  Arquivo gerado:              MATRIZ.H
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
*       1.01   aaf   02/10/2019 Assertivas de entrada e saída
*
*  $ED Descrição do módulo
*     Implementa matrizes genéricas com encadeamento nas oito direções.
*     Podem existir n matrizes em operação simultaneamente.
*     As matrizes possuem uma cabeça encapsulando o seu estado.
*
*     Cada matriz é homogênea quanto ao tipo dos dados que armazena.
*     Cada casa da matriz referencia o valor que contém.
*
*     Os ponteiros para os dados são copiados para elementos da matriz.
*        Não é copiado o valor apontado por estes ponteiros.
*
*     O controle da destruição do valor de um elemento a ser excluído
*        é realizado por uma função fornecida pelo usuário.
*
*     Cada matriz referencia uma função que determina como devem ser
*        desalocados os dados nela contidos.
*
*     A função de liberação dos valores contidos nos elementos deve
*        assegurar a liberação de todos os espaços referênciados pelo
*        valor contido em um elemento.
*        Esta função é chamada antes de se desalocar uma casa
*        de uma matriz.
*        Caso não seja necessário desalocar o valor referenciado pelo
*        elemento, o ponteiro para a função de liberação poderá ser NULL .
*        Caso o elemento da matriz seja a única âncora do valor referenciado,
*        esta função deve promover a destruição (free) desse valor e
*        de todos os dados nele ancorados.
*
***************************************************************************/
 
#if defined( MATRIZ_OWN )
   #define MATRIZ_EXT
#else
   #define MATRIZ_EXT extern
#endif


/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para uma matriz */

typedef struct tgMatriz * MTZ_tppMatriz ;


/***********************************************************************
*
*  $TC Tipo de dados: MTZ Condiçoes de retorno
*
*
***********************************************************************/

   typedef enum {

         MTZ_CondRetOK = 0 ,
               /* Executou correto */

         MTZ_CondRetNaoCriouHead = 1 ,
               /* Não criou a cabeça */

         MTZ_CondRetErroEstrutura = 2 ,
               /* Estrutura da matriz está errada */

         MTZ_CondRetDirecaoNaoExisteOuInvalida = 3 ,
               /* Direcao nao existe ou é invalida (sai da matriz) */

         MTZ_CondRetMatrizNaoExiste = 4 ,
               /* Matriz não existe */

         MTZ_CondRetCasaVazia = 5 ,
               /* Casa da matriz está vazia */

         MTZ_CondRetFaltouMemoria = 6
               /* Faltou memória ao alocar dados */

   } MTZ_tpCondRet ;


/***********************************************************************
 *
 *  $TC Tipo de dados: MTZ Direções
 *
 *
 ***********************************************************************/

    typedef enum {
        
        MTZ_DirNorte = 0 ,
            /* Andar na direção Norte */
        
        MTZ_DirNordeste = 1 ,
            /* Andar na direção Nordeste */
        
        MTZ_DirLeste = 2 ,
            /* Andar na direção Leste */
        
        MTZ_DirSudeste = 3 ,
            /* Andar na direção Sudeste */
        
        MTZ_DirSul = 4 ,
            /* Andar na direção Sul */
        
        MTZ_DirSudoeste = 5 ,
            /* Andar na direção Sudoeste */
        
        MTZ_DirOeste = 6 ,
            /* Andar na direção Oeste */
        
        MTZ_DirNoroeste = 7
            /* Andar na direção Noroeste */
        
    } MTZ_tpDirecao ;


#ifdef _DEBUG

   typedef enum {

        DeturpaEliminaCorrente = 1 ,
            /* Elimina o elemento corrente. */

        DeturpaPtrNorteNulo = 2 ,
            /* Atribui NULL ao ponteiro para o elemento norte. */

        DeturpaPtrSudesteNulo = 3 ,
            /* Atribui lixo ao ponteiro para o elemento leste. */

		    DeturpaPtrNorteLixo = 4 ,
            /* Atribui lixo ao ponteiro para o elemento norte. */

        DeturpaPtrSudesteLixo = 5 ,
            /* Atribui lixo ao ponteiro para o elemento sudeste. */

		    DeturpaConteudoPtrNulo = 6 ,
		        /* Atribui NULL ao ponteiro para o conteúdo da casa. */

		    DeturpaAlteraTipoEstrutura = 7 ,
            /* Altera tipo de estrutura apontado na casa. */

		    DeturpaDesencadeiaCasa = 8 ,
            /* Desencadeia casa sem liberá-la com free. */

		    DeturpaPtrCorrNulo = 9 ,
            /* Atribui NULL ao ponteiro corrente. */

        DeturpaPtrPrimNulo = 10 ,
            /* Atribui NULL ao ponteiro primeiro. */

   } MTZ_tpModosDeturpacao ;

#endif

/***********************************************************************
*
*  $FC Função: MTZ Criar matriz
*
*  $ED Descrição da função
*     Cria uma nova matriz n x n.
*     Caso já exista uma matriz no ponteiro especificado, esta será destruída.
*
*  $EP Parametros
*     $P ppMtz - Ponteiro para o ponteiro para uma matriz
*     $P n - Dimensão n x n da matriz quadrada
*     $P ExcluirValor - ponteiro para a função que processa a
*                       exclusão do valor referenciado pelo elemento
*                       a ser excluído.
*
*  $FV Valor retornado
*     MTZ_CondRetOK
*     MTZ_CondRetFaltouMemoria
*     MTZ_CondRetErroEstrutura   - Caso a dimensão da matriz seja menor que zero ou o ponteiro seja nulo
*
*  Condições de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     ppMtz não contém lixo
*     ExcluirValor libera devidamente toda a área de memória alocada no conteúdo que será posto em cada casa
*
*  $EAS Assertivas de saída esperadas
*     Valem as assertivas estruturais da Matriz Genérica com Cabeça
*     A posição corrente da matriz estará em (0, 0)
*
***********************************************************************/

MTZ_tpCondRet MTZ_CriarMatriz(MTZ_tppMatriz * ppMtz, int n, void ( * ExcluirValor ) ( void * pValor ) ) ;


/***********************************************************************
*
*  $FC Função: MTZ Destruir matriz
*
*  $ED Descrição da função
*     Destrói a cabeça da matriz, anulando a matriz especificada.
*     Faz nada caso o ponteiro nao contenha uma matriz.
*
*  $EP Parametros
*     $P ppMtz - Ponteiro para uma matriz
*
*  $FV Valor retornado
*     MTZ_CondRetOK
*     MTZ_CondRetMatrizNaoExiste   - Caso o ponteiro seja nulo
*
*  Condições de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     ppMtz não contém lixo
*     Valem as assertivas estruturais da Matriz Genérica com Cabeça
*
*  $EAS Assertivas de saída esperadas
*     *ppMtz = NULL
*     Todos os elementos dentro casas estarão liberados
*     A estrutura de matriz estará liberada
*
***********************************************************************/

   MTZ_tpCondRet MTZ_DestruirMatriz( MTZ_tppMatriz * ppMtz ) ;


/***********************************************************************
*
*  $FC Função: MTZ Andar em Direção
*
*  $ED Descrição da função
*     Anda na direção especificada.
*
*  $EP Parâmetros
*     $P pMtz - Ponteiro para uma matriz
*     $P direcao - Direção a andar
*
*  $FV Valor retornado
*     MTZ_CondRetOK
*     MTZ_CondRetDirecaoNaoExisteOuInvalida
*     MTZ_CondRetMatrizNaoExiste
*
*  Condições de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     Valem as assertivas estruturais da Matriz Genérica com Cabeça
*
*  $EAS Assertivas de saída esperadas
*     Valem as assertivas estruturais da Matriz Genérica com Cabeça
*     Se a direção for inválida ou sair da matriz, a casa corrente se manterá na última posição válida
*     Se a direção for válida, a casa corrente estará movida na direção especificada
*
***********************************************************************/

   MTZ_tpCondRet MTZ_AndarDirecao( MTZ_tppMatriz pMtz, MTZ_tpDirecao direcao ) ;


/***********************************************************************
*
*  $FC Função: MTZ Inserir elemento na casa corrente
*
*  $ED Descrição da função
*     Insere um elemento na casa corrente da matriz.
*     Se já houver um elemento na casa corrente, este será liberado e sustituído.
*
*  $EP Parâmetros
*     $P pMtz - Ponteiro para uma matriz
*     $P pElemento - Ponteiro para valor a ser inserido na casa corrente
*     $P tipoConteudo - Debug: String com o nome do tipo do conteudo
*     $P tamBytes - Debug: Tamanho do conteúdo em bytes
*
*  $FV Valor retornado
*     MTZ_CondRetOK
*     MTZ_CondRetErroEstrutura
*	    MTZ_CondRetMatrizNaoExiste
*
*  Condições de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     Valem as assertivas estruturais da Matriz Genérica com Cabeça
*
*  $EAS Assertivas de saída esperadas
*     Valem as assertivas estruturais da Matriz Genérica com Cabeça
*     A casa corrente conterá o ponteiro passado em pElemento
*
***********************************************************************/

   MTZ_tpCondRet MTZ_InserirElementoNaCasaCorrente( MTZ_tppMatriz pMtz, void * pElemento
                                                                                       #ifdef _DEBUG
                                                                                       , char * tipoConteudo, int tamBytes
                                                                                       #endif
                                                   ) ;

/***********************************************************************
*
*  $FC Função: MTZ Obter valor corrente
*
*  $ED Descrição da função
*     Obtem o valor presente na casa corrente da matriz selecionada.
*
*  $EP Parâmetros
*     $P pMtz - Ponteiro para uma matriz
*     $P valor - Ponteiro de ponteiro para o valor a ser obtido
*
*  $FV Valor retornado
*     MTZ_CondRetOK
*     MTZ_CondRetMatrizNaoExiste
*     MTZ_CondRetCasaVazia
*     MTZ_CondRetErroEstrutura   - Caso o ponteiro para valor seja nulo
*
*  Condições de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     Valem as assertivas estruturais da Matriz Genérica com Cabeça
*
*  $EAS Assertivas de saída esperadas
*     Valem as assertivas estruturais da Matriz Genérica com Cabeça
*     O conteúdo do ponteiro valor será o ponteiro para o conteúdo da casa corrente
*
***********************************************************************/

   MTZ_tpCondRet MTZ_ObterValorCorrente( MTZ_tppMatriz pMtz, void ** valor ) ;

/***********************************************************************
*
*  $FC Função: MTZ Voltar para o primeiro
*
*  $ED Descrição da função
*     Volta o ponteiro corrente para o primeiro, ou seja, (0, 0).
*
*  $EP Parâmetros
*     $P pMtz - Ponteiro para uma matriz
*
*  $FV Valor retornado
*     MTZ_CondRetOK
*     MTZ_CondRetMatrizNaoExiste
*
*  Condições de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     Valem as assertivas estruturais da Matriz Genérica com Cabeça
*
*  $EAE Assertivas de saída esperadas
*     Valem as assertivas estruturais da Matriz Genérica com Cabeça
*     A casa corrente da matriz estará posta em (0,0)
*
***********************************************************************/

   MTZ_tpCondRet MTZ_VoltarParaPrimeiro( MTZ_tppMatriz pMtz ) ;


#ifdef _DEBUG

/***********************************************************************
*
*  $FC Função: MTZ  Verificar uma matriz
*
*  $ED Descrição da função
*     Função da interface de teste.
*     Verifica completamente uma determinada matriz.
*     Também marca todos os espaços por ela ocupados.
*
*  $FV Valor retornado
*     Número de erros encontrados, 0 se não encontrar nenhum.
*
***********************************************************************/

   int MTZ_VerificarMatriz( void * pMatrizParm );

/***********************************************************************
*
*  $FC Função: MTZ  Verificar uma casa de estrutura
*
*  $ED Descrição da função
*     Função da interface de teste.
*     Verifica a integridade de uma casa estrutural da matriz.
*
*  $EP Parâmetros
*     $P pElemParm - ponteiro para espaço que deveria ser uma casa estrutural.
*
*  $FV Valor retornado
*     Número de erros encontrados, 0 se não encontrar nenhum.
*
***********************************************************************/

   int MTZ_VerificarElem( void * pElemParm );

/***********************************************************************
*
*  $FC Função: MTZ  Deturpar matriz
*
*  $ED Descrição da função
*     Função da interface de teste.
*     Corrompe elementos específicos da estrutura da matriz.
*     Essa função destina-se a preparar os cenários de teste dos
*     casos de teste utilizados ao testar os verificadores estruturais
*     da matriz.
*     Esta função não tem proteção contra erros de uso, consequentemente
*     poderá levar o programa a ser cancelado pelo sistema operacional.
*
*  $EP Parâmetros
*     $P pMatrizParm  - matriz a ser deturpada
*     $P ModoDeturpar - identifica como deve ser feita a deturpação
*                       MTZ_tpModosDeturpacao identifica os modos de
*                       deturpação conhecidos
*
***********************************************************************/

   void MTZ_Deturpar( void * pMatrizParm , MTZ_tpModosDeturpacao ModoDeturpar ); 

#endif

#undef MATRIZ_EXT

/********** Fim do módulo de definição: Módulo matriz **********/

#else
#endif
