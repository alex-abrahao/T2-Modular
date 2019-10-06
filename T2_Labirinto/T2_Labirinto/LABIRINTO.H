#if ! defined( LABIRINTO_ )
#define LABIRINTO_
/***************************************************************************
*
*  $MCD Módulo de definição: Módulo labirinto
*
*  Arquivo gerado:              LABIRINTO.H
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
*       1.01   aaf   30/09/2019 Documentação inicial
*       1.02   aaf   30/09/2019 Assertivas de entrada e saída
*
*  $ED Descrição do módulo
*     Descreve funções de criação e manipulação de labirintos.
*     Podem existir n labirintos em operação simultaneamente.
*     Os labirintos possuem uma cabeça encapsulando o seu estado.
*
*     Cada labirinto armazena em cada posição um único valor do tipo
*        LAB_tpElemCasa, que representa o conteúdo da posição.
*
*     Cada labirinto tem sempre apenas uma saída e uma entrada, que não podem
*        estar na mesma posição.
*
*     Na criação de um labirinto de tamanho n:
*        - a entrada é posta na posição (0, 0);
*        - a saída é posta na posição (n - 1, n - 1);
*        - as demais casas são preenchidas com elementos vazios.
*
*     O usuário pode andar livremente por qualquer posição dentro do labirinto,
*       alterando os elementos na posição corrente.
*
*     O labirinto pode não ter solução.
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

		 LAB_CondRetNaoTemSolucao = 5 ,
			         /* Labirinto não tem solução */

         LAB_CondRetElementoInvalido = 6
               /* Elemento inserido na posição é inválido */

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
 *  $TC Tipo de dados: LAB Elementos da casa
 *
 *
 ***********************************************************************/

    typedef enum {
        
        LAB_ElemVazio = 0 ,
            /* Elemento que representa um espaço vazio */

        LAB_ElemParede = 1 ,
            /* Elemento que representa uma parede */

        LAB_ElemEntrada = 2 ,
            /* Elemento que representa a entrada do labirinto */

        LAB_ElemSaida = 3 ,
            /* Elemento que representa a saída do labirinto */

    } LAB_tpElemCasa ;

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
*  Condições de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     ppLab não contém lixo
*     tam > 1
*
*  $EAS Assertivas de saída esperadas
*     Valem as assertivas estruturais do Labirinto
*     A entrada do labirinto estará em (0, 0)
*     A posição corrente do labirinto estará em (0, 0)
*     A saída do labirinto estará em (tam - 1, tam - 1)
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
*  Condições de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     ppLab não contém lixo
*     Valem as assertivas estruturais do Labirinto
*
*  $EAS Assertivas de saída esperadas
*     *ppLab = NULL
*     Todos os elementos dentro casas estarão liberados
*     A estrutura de labirinto estará liberada
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
*  Condições de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     Valem as assertivas estruturais do Labirinto
*
*  $EAS Assertivas de saída esperadas
*     Valem as assertivas estruturais do Labirinto
*     Se a direção for inválida ou sair do labirinto, a posição corrente se manterá na última posição válida
*     Se a direção for válida, a posição corrente estará movida na direção especificada
*
***********************************************************************/

LAB_tpCondRet LAB_AndarDirecao( LAB_tppLabirinto pLab, LAB_tpDirecao direcao ) ;

/***********************************************************************
*
*  $FC Função: LAB Inserir elemento
*
*  $ED Descrição da função
*     Muda o elemento presente na casa corrente do labirinto.
*     Não faz nada caso o elemento inserido seja igual ao da posição corrente ou esteja sobrescrevendo
*     uma saída ou entrada.
*
*  $EP Parâmetros
*     $P pLab - Ponteiro para um labirinto
*     $P elemento - Um elemento a ser posto na casa corrente
*
*  $FV Valor retornado
*     LAB_CondRetOK
*     LAB_CondRetLabirintoNaoExiste
*     LAB_CondRetElementoInvalido
*     LAB_CondRetErroEstrutura   - Caso não consiga retirar a entrada ou saida anterior
*
*  Condições de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     Valem as assertivas estruturais do Labirinto
*     Se o elemento presente na posição corrente for uma entrada ou saída, ele não será sobrescrito
*
*  $EAS Assertivas de saída esperadas
*     Valem as assertivas estruturais do Labirinto
*     Se o elemento for inválido, a posição corrente manterá o último elemento válido
*     Se o elemento for válido, o elemento da posição corrente estará sobrescrito com o novo
*     Se o elemento for uma entrada, a entrada antiga irá virar uma posição vazia
*     Se o elemento for uma saída, a saída antiga irá virar uma posição vazia
*
***********************************************************************/

LAB_tpCondRet LAB_InserirElemento( LAB_tppLabirinto pLab, LAB_tpElemCasa elemento ) ;

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
*  Condições de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     Valem as assertivas estruturais do Labirinto
*
*  $EAS Assertivas de saída esperadas
*     Valem as assertivas estruturais do Labirinto
*
***********************************************************************/

LAB_tpCondRet LAB_ImprimirLabirinto( LAB_tppLabirinto pLab ) ;

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
*  Condições de acoplamento
*
*  $EAE Assertivas de entradas esperadas
*     Valem as assertivas estruturais do Labirinto
*
*  $EAS Assertivas de saída esperadas
*     Valem as assertivas estruturais do Labirinto
*
***********************************************************************/

int LAB_ExisteSolucao( LAB_tppLabirinto pLab ) ;

#undef LAB_EXT

/********** Fim do módulo de definição: Módulo matriz **********/

#else
#endif
