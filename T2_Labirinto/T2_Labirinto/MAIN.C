/***************************************************************************
*  $MCI M�dulo de implementa��o: M�dulo main
*
*  Arquivo gerado:              MAIN.C
*  Letras identificadoras:      MLAB
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
*       1.00   jmb   30/10/2019 In�cio do desenvolvimento
*       1.10   phd   31/10/2019 Todas fun��es estao disponiveis
*       1.20   jmb   02/11/2019 Otimiza��es
*		1.30   phd   03/11/2019 Respostas adequadas aos CondRets recebidos
*		1.31   aaf   06/11/2019 Revis�o do tutorial
*
***************************************************************************/

#include "LABIRINTO.H"
#include <stdio.h>

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

static char paraMinuscula(char letra);

static void exibeTutorial(void);

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***********************************************************************
*
*  $FC Fun��o: MLAB Main
*
*  $ED Descri��o da fun��o
*     Main que contem as fun��es do labirinto disponiveis para o usuario
*     criar e testar labirintos personalizados.
*
*  $FV Valor retornado
*     Int: 0
*
***********************************************************************/

int main(void) {
	
	char comando = 0, parametro; // inicializa as vari�veis comando e parametro para entrar no while
	int tam = 0, x, y; // inicializa as vari�veis de tamanho e das coordenadas x e y
	LAB_tpDirecao dir; // variavel pra andar
	LAB_tpCondRet CondRetObtido; // variavel que recebe o condret da funcao
	LAB_tpElemCasa parametroENUM; // variavel que converte o caractere digitado para o o tipo de elemento que sera colocado na casa
	LAB_tppLabirinto labTeste = NULL; // inicializa��o do labirinto

	printf("========================= CRIACAO DO LABIRINTO =========================\n\n");
	printf("Primeiramente, escolha o tamanho que sera utilizado na criacao do labirinto: ");
	scanf(" %d", &tam); // recebe o tamanho desejado
	
	while ((getchar()) != '\n'); // limpa o buffer do scanf

	while(LAB_CriarLabirinto(&labTeste, tam /* cria��o do labirinto */) != LAB_CondRetOK) {
		printf("Tamanho invalido, digite novamente: ");
		scanf(" %d", &tam); // recebe o tamanho desejado
		while ((getchar()) != '\n'); // limpa o buffer do scanf
	}

	printf("\n\n======================= MANIPULACAO DO LABIRINTO =======================\n");
	printf("\nOS SEGUINTES COMANDOS ESTAO DISPONIBILIZADOS PARA MANIPULACAO DO LABIRINTO: \n\n");

	exibeTutorial();
		
	while(comando != 'q') { // Enquanto o user n�o quiser terminar
		printf("Digite o comando desejado seguido do parametro, caso necessario: ");
		scanf("%c", &comando); // recebe o comando desejado

		comando = paraMinuscula(comando);

		if (comando == 'i') { // inserir elemento
			scanf(" %c", &parametro); // recebe o parametro que ja foi dado junto ao comando

			parametro = paraMinuscula(parametro); // transforma letras maiusculas em minusculas
			
			if (parametro == 'p' || parametro == 'v' || parametro == 'e' || parametro == 's'){ // determina se o parametro e valido
				if(parametro == 'p')
					parametroENUM = LAB_ElemParede;
				else if(parametro == 'v')
					parametroENUM = LAB_ElemVazio;
				else if(parametro == 'e')
					parametroENUM = LAB_ElemEntrada;
				else
					parametroENUM = LAB_ElemSaida;
				CondRetObtido = LAB_InserirElemento( labTeste, parametroENUM );

				if (CondRetObtido == LAB_CondRetLabirintoNaoExiste)
					printf("Ocorreu um problema durante a criacao do labirinto, pois o labirinto nao existe!\n");
				else if (CondRetObtido == LAB_CondRetElementoInvalido)
					printf("Nao se pode inserir elementos no lugar da Saida ou da Entrada!\n");
				else
					printf("Elemento inserido com sucesso!\n");
			}
			else
				printf("Parametro invalido!\n");
		}

		else if (comando == 'a'){ // andar para uma direcao
			scanf(" %c", &parametro);
			
			parametro = paraMinuscula(parametro);

			if (parametro == 'n' || parametro == 'l' || parametro == 's' || parametro == 'o'){
				if(parametro == 'n')
					dir=LAB_DirNorte;
				else if(parametro == 'l')
					dir=LAB_DirLeste;
				else if(parametro == 's')
					dir=LAB_DirSul;
				else if(parametro == 'o')
					dir=LAB_DirOeste;
				CondRetObtido = LAB_AndarDirecao( labTeste, dir );

				if (CondRetObtido == LAB_CondRetLabirintoNaoExiste)
					printf("Ocorreu um problema durante a criacao do labirinto, pois o labirinto nao existe!\n");
				else if (CondRetObtido == LAB_CondRetDirecaoNaoExisteOuInvalida)
					printf("Nao se pode andar para fora do labirinto!\n");
				else {
					LAB_ReceberCoordenadas( labTeste, &x, &y );
					printf("Andou com sucesso! Agora na coordenada: (%d,%d)\n", x, y);
				}
			}
			else
				printf("Parametro invalido!\n");
		}

		else if (comando == 'd'){ // destroi funcao
			CondRetObtido = LAB_DestruirLabirinto( &labTeste );

			if (CondRetObtido == LAB_CondRetLabirintoNaoExiste)
				printf("Ocorreu um problema durante a criacao do labirinto, pois o labirinto nao existe!\n");
			else {
				printf("Labirinto destruido com sucesso!\n");
				printf("Agora que o labirinto foi destruido, voce pode escolher entre finalizar o programa, digitando 'q', ou recriar o labirinto, digitando 'c'!\n");
			
				while(comando != 'q' && comando != 'c'){
					printf("Digite o comando desejado: ");
					scanf(" %c", &comando);
					if (comando == 'c'){
						tam = 0;
						printf("Escolha o tamanho do labirinto: ");
						scanf(" %d", &tam); // recebe o tamanho desejado
						while(LAB_CriarLabirinto(&labTeste, tam /* cria��o do labirinto */) != LAB_CondRetOK) {
							printf("Tamanho invalido, digite novamente: ");
							scanf(" %d", &tam); // recebe o tamanho desejado
							while ((getchar()) != '\n'); // limpa o buffer do scanf
						}
					
					}
					else {
						if(comando != 'q')
							printf("COMANDO INVALIDO!\n");
					}
				}
			}
		}

		else if (comando == 'm'){ // imprimir labirinto
			printf("\n'C' simboliza a casa corrente do labirinto\n");
			CondRetObtido = LAB_ImprimirLabirinto( labTeste );

			if (CondRetObtido == LAB_CondRetLabirintoNaoExiste)
				printf("Ocorreu um problema durante a criacao do labirinto, pois o labirinto nao existe!\n");
		}

		else if (comando == 'r'){ // resolver labirinto
			CondRetObtido = LAB_ExibeSolucao( labTeste );

			if (CondRetObtido == LAB_CondRetLabirintoNaoExiste)
				printf("Ocorreu um problema durante a criacao do labirinto, pois o labirinto nao existe!\n");
			else if (CondRetObtido == LAB_CondRetNaoTemSolucao)
				printf("O labirinto nao tem solucao!\n");
		}

		else if (comando == 't') // imprimir o tutorial
			exibeTutorial();
		else if (comando != 'q') // fechar o programa
			printf("COMANDO INVALIDO!\n");

		while ((getchar()) != '\n'); // limpa o buffer do scanf
	}

	LAB_DestruirLabirinto( &labTeste );

	return 0;
} /* Fim fun��o: MLAB Main */

/***********************************************************************
*
*  $FC Fun��o: Torna as letras maiusculas em minusculas
*
***********************************************************************/
char paraMinuscula(char letra) {
	if (letra >= 65 && letra <= 90)
		letra += 32;
	return letra;
} /* Fim fun��o: Torna as letras maiusculas em minusculas  */

/***********************************************************************
*
*  $FC Fun��o: Exibe o tutorial da main
*
***********************************************************************/
void exibeTutorial(void) {
	printf("->'i', PARA INSERIR UM ELEMENTO NA CASA CORRENTE DO LABIRINTO, SEGUIDO DO SEU PARAMETRO:\n\
	PARAMETROS PARA INSERIR:\n\
	%c'p', para inserir uma parede\n\
	%c'v', para uma casa vazia\n\
	%c'e', para setar a entrada do labirinto\n\
	%c's', para setar a saida do labirinto\n\
		Exemplo: i p  ----> Insere uma parede na casa corrente\n\n\
->'a' PARA ANDAR PARA A PROXIMA CASA DO LABIRINTO, SEGUIDO DO SEU PARAMETRO:\n\
	PARAMETROS PARA ANDAR:\n\
	%c'n', para andar na direcao norte\n\
	%c'l', para andar na direcao leste\n\
	%c's', para andar na direcao sul\n\
	%c'o', para andar na direcao oeste\n\
		Exemplo: a l  ----> Anda na direcao leste\n\n\
->'d' PARA DESTRUIR O LABIRINTO\n\
->'m' PARA MOSTRAR O LABIRINTO\n\
->'r' PARA RESOLVER O LABIRINTO\n\
->'t' PARA REVER O TUTORIAL\n\
->'q' PARA FINALIZAR O PROGRAMA\n\n", 250, 250, 250, 250, 250, 250, 250, 250);
} /* Fim fun��o: Exibe o tutorial da main  */

/********** Fim do m�dulo de implementa��o: Main **********/
