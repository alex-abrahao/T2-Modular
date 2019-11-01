#include "LABIRINTO.H"
#include "TST_ESPC.H"
#include <stdio.h>
#include <ctype.h>

int main(void) {
	
	char comando = 0, parametro; //inicializa a variável comando para entrar no while
	int tam = 3;
	LAB_tpElemCasa parametroENUM; //variavel que converte o caractere digitado para o o tipo de elemento que sera colocado na casa
	LAB_tppLabirinto labTeste = NULL; // iniciação da variável

	printf("========================= CRIACAO DO LABIRINTO =========================\n\n");
	printf("Primeiramente, escolha o tamanho que sera utilizado na criacao do labirinto: ");                  /* TUTORIAL ESTÁ EM WIP */
	scanf("%d", &tam);
	LAB_CriarLabirinto(&labTeste, tam); // criação do labirinto

	printf("\n\n======================= MANIPULACAO DO LABIRINTO =======================\n");
	printf("\n\nOS SEGUINTES COMANDOS ESTAO DISPONIBILIZADOS PARA MANIPULACAO DO LABIRINTO: \n\n");

	printf("'i', PARA INSERIR UM ELEMENTO NA CASA CORRENTE DO LABIRINTO, SEGUIDO DO SEU PARAMETRO:\n-'p', para inserir uma parede\n-'v', para uma casa vazia\n-'e', para setar a entrada da matriz\n's', para setar a sa'ida da matriz\n\n'a' PARA ANDAR PARA A PROXIMA CASA DO LABIRINTO\n'd' PARA DESTRUIR O LABIRINTO\n'm' PARA MOSTRAR O LABIRINTO\n'r' PARA RESOLVER O LABIRINTO\n'q' PARA FECHAR O PROGRAMA, MOSTRANDO SER FRACO E NÃO TER A CAPACIDADE DE MANUSEAR UM LABIRINTO\n\n");
		
	while(comando != 'q') {
		printf("Digite o comando desejado seguido do parametro, caso necessario: ");
		scanf("%c", &comando);
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
				LAB_InserirElemento(labTeste, parametroENUM);
			}
			else
				printf("Parametro invalido!\n\n");
			
		}
		/*else if (comando == 'a')

		else if (comando == 'd')

		else if (comando == 'm')

		else if(comando == 'r')

		else
			printf("COMANDO INVALIDO");*/


	}

	return 0;
}