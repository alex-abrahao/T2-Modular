/***************************************************************************
*  $MCI Módulo de implementação: Módulo labirinto
*
*  Arquivo gerado:              MAIN.C
*  Letras identificadoras:      MLAB
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
*       1.00   jmb   30/10/2019 Início do desenvolvimento
*       1.10   phd   31/10/2019 Todas funções estao disponiveis
*       1.20   jmb   02/11/2019 Diversas otimizacoes
*		1.30   phd   03/11/2019 Respostas adequadas aos CondRets recebidos
*
***************************************************************************/

#include "LABIRINTO.H"
#include <stdio.h>

void exibeTutorial(void) {
	printf("->'i', PARA INSERIR UM ELEMENTO NA CASA CORRENTE DO LABIRINTO, SEGUIDO DO SEU PARAMETRO APOS TER APERTADO A TECLA 'ENTER' OU TER DADO UM ESPACO ENTRE AS LETRAS:\n%c'p', para inserir uma parede\n%c'v', para uma casa vazia\n%c'e', para setar a entrada da matriz\n%c's', para setar a saida da matriz\n\n\n->'a' PARA ANDAR PARA A PROXIMA CASA DO LABIRINTO, SEGUIDO DO SEU PARAMETRO APOS TER APERTADO A TECLA 'ENTER' OU TER DADO UM ESPACO ENTRE AS LETRAS:\n%c'n', para andar na direcao norte\n%c'l', para andar na direcao leste\n%c's', para andar na direcao sul\n%c'o', para andar na direcao oeste\n\n->'d' PARA DESTRUIR O LABIRINTO\n->'m' PARA MOSTRAR O LABIRINTO\n->'r' PARA RESOLVER O LABIRINTO\n->'t' PARA REVER O TUTORIAL\n->'q' PARA FINALIZAR O PROGRAMA\n\n", 250, 250, 250, 250, 250, 250, 250, 250);
}

char paraMinuscula(char letra) {
	if (letra >= 65 && letra <= 90) // transforma letras maiusculas em minusculas
		letra += 32;
	return letra;
}

int main(void) {
	
	char comando=0, parametro; // inicializa as variáveis comando e parametro para entrar no while
	int tam = 3, x, y; // inicializa as variáveis de tamanho e das coordenadas x e y
	LAB_tpDirecao dir; // variavel pra andar
	LAB_tpCondRet CondRetObtido; // variavel que recebe o condret da funcao
	LAB_tpElemCasa parametroENUM; // variavel que converte o caractere digitado para o o tipo de elemento que sera colocado na casa
	LAB_tppLabirinto labTeste = NULL; // iniciação do labirinto

	printf("========================= CRIACAO DO LABIRINTO =========================\n\n");
	printf("Primeiramente, escolha o tamanho que sera utilizado na criacao do labirinto: ");
	scanf(" %d", &tam); // recebe o tamanho desejado
	LAB_CriarLabirinto(&labTeste, tam); // criação do labirinto
	while ((getchar()) != '\n'); // limpa o buffer do scanf

	printf("\n\n======================= MANIPULACAO DO LABIRINTO =======================\n");
	printf("\nOS SEGUINTES COMANDOS ESTAO DISPONIBILIZADOS PARA MANIPULACAO DO LABIRINTO: \n\n");

	exibeTutorial();
		
	while(comando != 'q') { // while q so termina qnd o usuario digitar 'q'
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
			printf("Labirinto destruido com sucesso!\n");
			printf("Agora que o labirinto foi destruido, voce pode escolher entre finalizar o programa, digitando 'q', ou recriar o labirinto, digitando 'c'!\n");
			
			while(comando != 'q' && comando != 'c'){
				printf("Digite o comando desejado: ");
				scanf(" %c", &comando);
				if (comando == 'c'){
					printf("Escolha o tamanho do labirinto: ");
					scanf(" %d", &tam);
					LAB_CriarLabirinto(&labTeste, tam);
				}
				else 
					if(comando != 'q')
						printf("COMANDO INVALIDO!\n");
			}
		}

		else if (comando == 'm'){ // imprimir labirinto
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
			printf("\n'i', PARA INSERIR UM ELEMENTO NA CASA CORRENTE DO LABIRINTO, SEGUIDO DO SEU PARAMETRO:\n-'p', para inserir uma parede\n-'v', para uma casa vazia\n-'e', para setar a entrada da matriz\n-'s', para setar a saida da matriz\n\n'a' PARA ANDAR PARA A PROXIMA CASA DO LABIRINTO\n-'n', para andar na direcao norte\n-'l', para andar na direcao leste\n-'s', para andar na direcao sul\n-'o', para andar na direcao oeste\n\n'd' PARA DESTRUIR O LABIRINTO\n'm' PARA MOSTRAR O LABIRINTO\n'r' PARA RESOLVER O LABIRINTO\n't' PARA REVER O TUTORIAL\n'q' PARA FINALIZAR O PROGRAMA\n\n", 250, 250, 250, 250, 250, 250, 250, 250);

		else if (comando != 'q') // fechar o programa
			printf("COMANDO INVALIDO!\n");

		while ((getchar()) != '\n'); // limpa o buffer do scanf
	}

	LAB_DestruirLabirinto( &labTeste );

	return 0;
}