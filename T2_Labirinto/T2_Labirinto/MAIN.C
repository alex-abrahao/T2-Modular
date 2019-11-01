#include "LABIRINTO.H"
#include "TST_ESPC.H"
#include <stdio.h>
#include <ctype.h>

int main(void) {
	
	char comando=0, parametro; //inicializa a variável comando para entrar no while
	int tam = 3;
	LAB_tpDirecao dir; //variavel pra andar
	//LAB_tpCondRet CondRetObtido; //variavel que recebe o condret da funcao (ainda nao e utilizada)
	LAB_tpElemCasa parametroENUM; //variavel que converte o caractere digitado para o o tipo de elemento que sera colocado na casa
	LAB_tppLabirinto labTeste = NULL; // iniciação da variável

	printf("========================= CRIACAO DO LABIRINTO =========================\n\n");
	printf("Primeiramente, escolha o tamanho que sera utilizado na criacao do labirinto: ");                  /* TUTORIAL ESTÁ EM WIP */
	scanf(" %d", &tam);
	LAB_CriarLabirinto(&labTeste, tam); // criação do labirinto
	while ((getchar()) != '\n'); //limpa o buffer do scanf

	printf("\n\n======================= MANIPULACAO DO LABIRINTO =======================\n");
	printf("\n\nOS SEGUINTES COMANDOS ESTAO DISPONIBILIZADOS PARA MANIPULACAO DO LABIRINTO: \n\n");

	printf("'i', PARA INSERIR UM ELEMENTO NA CASA CORRENTE DO LABIRINTO, SEGUIDO DO SEU PARAMETRO:\n-'p', para inserir uma parede\n-'v', para uma casa vazia\n-'e', para setar a entrada da matriz\n-'s', para setar a saida da matriz\n\n'a' PARA ANDAR PARA A PROXIMA CASA DO LABIRINTO\n-'n', para andar na direcao norte\n-'l', para andar na direcao leste\n-'s', para andar na direcao sul\n-'o', para andar na direcao oeste\n\n'd' PARA DESTRUIR O LABIRINTO\n'm' PARA MOSTRAR O LABIRINTO\n'r' PARA RESOLVER O LABIRINTO\n't' PARA REVER O TUTORIAL\n'q' PARA FECHAR O PROGRAMA, MOSTRANDO SER FRACO E NAO TER A CAPACIDADE DE MANUSEAR UM LABIRINTO\n\n");
		
	while(comando != 'q') { 
		printf("Digite o comando desejado seguido do parametro, caso necessario: ");
		scanf(" %c", &comando);

		//as funcoes ainda precisam receber os condrets para saber se tudo funcionou corretamente
		if (comando == 'i') { //inserir elemento
			scanf(" %c", &parametro);
			if (parametro == 'p' || parametro == 'v' || parametro == 'e' || parametro == 's'){
				if(parametro == 'p')
					parametroENUM = LAB_ElemParede;
				else if(parametro == 'v')
					parametroENUM = LAB_ElemVazio;
				else if(parametro == 'e')
					parametroENUM = LAB_ElemEntrada;
				else if(parametro == 's')
					parametroENUM = LAB_ElemSaida;
				LAB_InserirElemento( labTeste, parametroENUM );
				printf("Elemento inserido com sucesso!\n");
			}
			else
				printf("Parametro invalido!\n");
		}

		else if (comando == 'a'){ //assumindo como placeholder q andou com sucesso, msm q isso n seja verdade
			scanf(" %c", &parametro);
			if (parametro == 'n' || parametro == 'l' || parametro == 's' || parametro == 'o'){
				if(parametro == 'n')
					dir=LAB_DirNorte;
				else if(parametro == 'l')
					dir=LAB_DirLeste;
				else if(parametro == 's')
					dir=LAB_DirSul;
				else if(parametro == 'o')
					dir=LAB_DirOeste;
				LAB_AndarDirecao( labTeste, dir );
				printf("Andou com sucesso! Agora na coordenada: (x,y)\n");
			}
			else
				printf("Parametro invalido!\n");
		}

		else if (comando == 'd'){ //Deixando aqui ja pronto, mas se vc tem uma funcao de destruir labirinto, o usuario tem q ter uma opcao para criar um novo labirinto, pq apos destruir o programa se tornaria inutil
			LAB_DestruirLabirinto( &labTeste );
			printf("Labirinto destruido com sucesso!\n");
		}

		else if (comando == 'm')
			LAB_ImprimirLabirinto( labTeste );

		else if (comando == 'r')
			LAB_ExibeSolucao( labTeste ); //N sei se essa funcao vai ser a q acha a solucao, mas e a mais proxima do q a gnt tem, entao botei como placeholder

		else if (comando == 't')
			printf("\n'i', PARA INSERIR UM ELEMENTO NA CASA CORRENTE DO LABIRINTO, SEGUIDO DO SEU PARAMETRO:\n-'p', para inserir uma parede\n-'v', para uma casa vazia\n-'e', para setar a entrada da matriz\n-'s', para setar a saida da matriz\n\n'a' PARA ANDAR PARA A PROXIMA CASA DO LABIRINTO\n-'n', para andar na direcao norte\n-'l', para andar na direcao leste\n-'s', para andar na direcao sul\n-'o', para andar na direcao oeste\n\n'd' PARA DESTRUIR O LABIRINTO\n'm' PARA MOSTRAR O LABIRINTO\n'r' PARA RESOLVER O LABIRINTO\n't' PARA REVER O TUTORIAL\n'q' PARA FECHAR O PROGRAMA, MOSTRANDO SER FRACO E NAO TER A CAPACIDADE DE MANUSEAR UM LABIRINTO\n\n");

		else if (comando != 'q')
			printf("COMANDO INVALIDO!\n");

		while ((getchar()) != '\n'); //limpa o buffer do scanf
	}

	return 0;
}