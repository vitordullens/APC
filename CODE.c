/* Universidade de Brasilia
Instituto de Ciencias Exatas
Departamento de Ciencia da Computacao
Algoritmos e Programação de Computadores – 2/2016
Aluno(a): Vitor Fernandes Dullens
Matricula: 160148260
Turma: A
Versão do compilador: gcc version 5.3.0
Descricao: Simulador de Restaurante */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ANIMACAO_DELAY 86 /*define o tempo de intervalo entre as exibições da tela (milissegundos)*/
#define MEDIA_TEMPO 60 /*valor médio da medida de tempo de estadia no restaurante (segundos)*/
#define ERRO_TEMPO 20 /* erro da medida de tempo de estadia no restaurante (segundos)*/
#define RAND(M, E) (M + (rand()%(2*E + 1) - E)) /* expressão que gera um número aleatório no intervalo [M - E, M + E]*/

/*limpar tela no linux ou no windows*/
#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

/*variaveis globais*/
char restaurante[17][41]; /*matriz que salva o arquivo tipo texto*/
long long int tempo_estadia[20]; /*vetor que armazena o tempo que cada mesa será ocupada*/
int s = 0, v = 0; /*o numero da mesa. S para salão e V para varanda. */
int varanda[4], salao[12]; /*para armazenar se o local esta ocupado, vetor recebe 1, ou vazio, vetor recebe 0*/

void Zerar_Vetor(){ /*deixa os vetores com os tamanhos padrões para os testes */

	int i=0;

	for (i = 0; i < 12; ++i){
		salao[i] = 0; /*zera cada mesa, ou seja, a mesa esta desocupada*/
	}
	for (i = 0; i < 4; ++i){
		varanda[i] = 0; /*zera cada mesa, ou seja, a mesa esta desocupada*/
	}
	for(i=0;i<20;i++){
		tempo_estadia[i] = 10000000000000; /*valor muito grande para n comprometer na manipulação do vetor posteriormente*/
	} 
	
}

void ler_arquivo(){ /*le o arquivo texto e coloca ele na matriz*/
    int i;
    FILE *fp;
    fp = fopen("matriz_restaurante.txt", "r");
    for(i=0;i<17;i++){
        fscanf(fp, "%[^\n]%*c", &restaurante[i][0]);
    }
    fclose(fp);

}

void sleep(int ms) { /*funcao utilizada para dar um delay e a animacao fluir*/
	clock_t limit = clock() + ms*(CLOCKS_PER_SEC/1000);
	while (limit > clock()) {}
}

void mostrar_arquivo(){ /*escreve na tela a matriz*/
    int i, j;
    printf("\n\t\t\tR E S T A U R A N T E    D O    D U L L E N S\n\n");
    for(i=0;i<17;i++){
        for(j=0;j<41;j++){
            printf("%c", restaurante[i][j]);
        if(j==40){
                printf("\n");
            }
       }
    }
    printf("\n");/*mostrar a matriz restaurante*/
}

void limpar_tela(){ /* comando para limpar a tela*/
    
    system(CLEAR);
}


char sim_nao(char letra){ /*verifica se o usuario digitou o comando adequado*/
    while(letra != 'S' && letra != 's' && letra != 'N' && letra != 'n'){
        printf("\t\nCOMANDO INAVALIDO\n\n");
        printf("Digite 's' ou 'S' para se sim, ou 'n' ou 'N' para se nao: ");
        scanf(" %c", &letra);
    }
    return letra;
}

int um_quatro(int qnt){ /*verifica se o usuario digitou o comando adeguado*/
    while(qnt<1 || qnt>4){
        printf("\t\nCOMANDO INAVALIDO\n\n");
        printf("Cada mesa do restaurante consegue armazenar de 1 a 4 pessoas.\n");
        printf("A mesa eh para quantas pessoas (1 a 4) ? ");
        scanf("%d", &qnt);
    }
    return qnt;
}

int controle_restaurante(char continuar){ /* saber se o usuario deseja continuar no controle do restaurante */
    char aux;
    aux = sim_nao(continuar);
    if(aux == 'n' || aux == 'N'){
        return 0;
    }
    else{
        return 1;
    }

}

int um_dois(int mesa){ /*verifica se o usuario digitou o comando adequado*/
    while(mesa != 1 && mesa != 2){
        printf("\t\nCOMANDO INAVALIDO\n\n");
        printf("Digite 1 para SALAO\n");
        printf("Digite 2 para VARANDA\n\n");
        printf("Qual a sua opcao de mesa: ");
        scanf("%d", &mesa);
    }
    return mesa;
}

int Check_Full(int mesa){ /*verifica se o local esta ocupado e se as areas estao ocupadas*/

	int i=0;

	if(mesa==1){
		while(salao[i]==1 && i<=12){
			i++; /*verifica até qual mesa esta lotado*/
		}
		
		if(i<=12){
			s=i; /* o valor da mesa recebe i, pois a posicao i é uma mesa vazia, ou seja, é possivel colocar alguem nela. Salao o maximo são 12 mesas*/
			
		}
		else{
			printf("\nO SALAO ESTA LOTADO\n\n" ); /*se o valor passa o maximo do local, ele esta lotado*/
			return 1;
		}
	}
	if(mesa==2){
		while(varanda[i]==1 && i<=4){
			i++; /*verifica até qual mesa esta lotado*/

		}
		if(i<=4){
			v=i; /* o valor da mesa recebe i, pois a posicao i é uma mesa vazia, ou seja, é possivel colocar alguem nela. Varanda o maximo são 4 mesas*/

		}
		else{
			printf("\nA VARANDA ESTA LOTADA\n\n"); /*se o valor passa o maximo do local, ele esta lotado*/
			return 2;


		}
	}

	return 0;	

}

void desocupar(int mesa){ /*verifica se o tempo de estadia foi alcançado e libera os lugares */

    int i;

    if(mesa == 1){ /*cada mesa recebe seu tempo de estadia. 1 = salao */
        tempo_estadia[s] = time(0) + RAND(MEDIA_TEMPO, ERRO_TEMPO); /*s é o numero da mesa*/
    }
    if(mesa == 2){ /*cada mesa recebe seu tempo de estadia. 2 = varanda */
        tempo_estadia[v+12] = time(0) + RAND(MEDIA_TEMPO, ERRO_TEMPO); /*v+12 é o numero da mesa, pois v, vai de 1 a 4, e nesse caso o vetor é unico, e v são as ultimas 4 mesas*/
    }
    for(i=1; i<20; i++){
        if(time(0) >= tempo_estadia[i]){

            if(i >= 1 && i <= 4){
                printf("\nA %d MESA DO SALAO FOI DESOCUPADA\n\n", i);
                sleep(3000); /*delay para mostrar a informação*/
                salao[i] = 0; /*local agora esta desocupado, vazio*/
                tempo_estadia[i] = 10000000000000; /*volta ao tempo padrão*/
                restaurante[1][17 +( (i - 1) * 3 )] = '/';
                restaurante[1][18 +( (i - 1) * 3 )] = '\\';
                restaurante[2][17 +( (i - 1) * 3 )] = '\\';
                restaurante[2][18 +( (i - 1) * 3 )] = '/';
                limpar_tela();
                mostrar_arquivo();
            }
         
            if(i>=5 && i <=8){
                printf("\nA %d MESA DO SALAO FOI DESOCUPADA\n\n", i);
                sleep(3000);
                salao[i] = 0;
                tempo_estadia[i] = 10000000000000;
                restaurante[5][17+( (i - 5) * 3 )] = '/';
                restaurante[5][18+( (i - 5) * 3 )] = '\\';
                restaurante[6][17+( (i - 5) * 3 )] = '\\';
                restaurante[6][18+( (i - 5) * 3 )] = '/';
                limpar_tela();
                mostrar_arquivo();
            }
            
            if(i>=9 && i <= 12){
                printf("\nA %d MESA DO SALAO FOI DESOCUPADA\n\n", i);
                sleep(3000);
                salao[i] = 0;
                tempo_estadia[i] = 10000000000000;
                restaurante[9][17+( (i - 9) * 3 )] = '/';
                restaurante[9][18+( (i - 9) * 3 )] = '\\';
                restaurante[10][17+( (i - 9) * 3 )] = '\\';
                restaurante[10][18+( (i - 9) * 3 )] = '/';
                limpar_tela();
                mostrar_arquivo();
            }
            
            if((i-12)>=1 && (i-12)<=4){
                printf("\nA %d MESA DA VARANDA FOI DESOCUPADA\n\n", i-12);
                sleep(3000);
                varanda[i-12] = 0;
                tempo_estadia[i] = 10000000000000;
                restaurante[13][17+( (i - 13) * 3 )] = '/';
                restaurante[13][18+( (i - 13) * 3 )] = '\\';
                restaurante[14][17+( (i - 13) * 3 )] = '\\';
                restaurante[14][18+( (i - 13) * 3 )] = '/';
                limpar_tela();
                mostrar_arquivo();
            }
            
        }
    }

}


int lotacao_restaurante(int mesa){ /*coloca as pessoas nas suas respectivas mesas*/

    if(mesa == 1) /*salao*/ {
      	salao[s] = 1; /*cada mesa recebe 1, o que quer dizer que ela esta ocupada*/
   		s++;

    }
    if(mesa == 2){ /*varanda*/
     	varanda[v] = 1; /*cada mesa recebe 1, o que quer dizer que ela esta ocupada*/
        v++;
       
    }

    return 0;

}


void pessoas_mesa(int qnt, int mesa){

    /* VARANDA */
    if(mesa==2){
	    if(v==1){ /*primeira mesa*/
	        if(qnt==1){
	            restaurante[13][17] = '*';
	            restaurante[13][18] = '-';
	            restaurante[14][17] = '-';
	            restaurante[14][18] = '-';
	        }
	        if(qnt==2){
	            restaurante[13][17] = '*';
	            restaurante[13][18] = '*';
	            restaurante[14][17] = '-';
	            restaurante[14][18] = '-';
	        }
	        if(qnt==3){
	            restaurante[13][17] = '*';
	            restaurante[13][18] = '*';
	            restaurante[14][17] = '*';
	            restaurante[14][18] = '-';
	        }
	        if(qnt==4){
	            restaurante[13][17] = '*';
	            restaurante[13][18] = '*';
	            restaurante[14][17] = '*';
	            restaurante[14][18] = '*';
	        }
	    }
	    if(v==2){ /*segunda mesa*/
	        if(qnt==1){
	            restaurante[13][20] = '*';
	            restaurante[13][21] = '-';
	            restaurante[14][20] = '-';
	            restaurante[14][21] = '-';
	        }
	        if(qnt==2){
	            restaurante[13][20] = '*';
	            restaurante[13][21] = '*';
	            restaurante[14][20] = '-';
	            restaurante[14][21] = '-';
	        }
	        if(qnt==3){
	            restaurante[13][20] = '*';
	            restaurante[13][21] = '*';
	            restaurante[14][20] = '*';
	            restaurante[14][21] = '-';
	        }
	        if(qnt==4){
	            restaurante[13][20] = '*';
	            restaurante[13][21] = '*';
	            restaurante[14][20] = '*';
	            restaurante[14][21] = '*';
	        }
	    }
	    if(v==3){/*terceira mesa*/
	        if(qnt==1){
	            restaurante[13][23] = '*';
	            restaurante[13][24] = '-';
	            restaurante[14][23] = '-';
	            restaurante[14][24] = '-';
	        }
	        if(qnt==2){
	            restaurante[13][23] = '*';
	            restaurante[13][24] = '*';
	            restaurante[14][23] = '-';
	            restaurante[14][24] = '-';
	        }
	        if(qnt==3){
	            restaurante[13][23] = '*';
	            restaurante[13][24] = '*';
	            restaurante[14][23] = '*';
	            restaurante[14][24] = '-';
	        }
	        if(qnt==4){
	            restaurante[13][23] = '*';
	            restaurante[13][24] = '*';
	            restaurante[14][23] = '*';
	            restaurante[14][24] = '*';
	        }
	    }
	    if(v==4){/*quarta mesa*/
	        if(qnt==1){
	            restaurante[13][26] = '*';
	            restaurante[13][27] = '-';
	            restaurante[14][26] = '-';
	            restaurante[14][27] = '-';
	        }
	        if(qnt==2){
	            restaurante[13][26] = '*';
	            restaurante[13][27] = '*';
	            restaurante[14][26] = '-';
	            restaurante[14][27] = '-';
	        }
	        if(qnt==3){
	            restaurante[13][26] = '*';
	            restaurante[13][27] = '*';
	            restaurante[14][26] = '*';
	            restaurante[14][27] = '-';
	        }
	        if(qnt==4){
	            restaurante[13][26] = '*';
	            restaurante[13][27] = '*';
	            restaurante[14][26] = '*';
	            restaurante[14][27] = '*';
	        }
    	}
    }

    /*SALAO */
    if(mesa==1){
	    if(s==1){
	        if(qnt==1){
	            restaurante[1][17] = '*';
	            restaurante[1][18] = '-';
	            restaurante[2][17] = '-';
	            restaurante[2][18] = '-';
	        }
	        if(qnt==2){
	            restaurante[1][17] = '*';
	            restaurante[1][18] = '*';
	            restaurante[2][17] = '-';
	            restaurante[2][18] = '-';
	        }
	        if(qnt==3){
	            restaurante[1][17] = '*';
	            restaurante[1][18] = '*';
	            restaurante[2][17] = '*';
	            restaurante[2][18] = '-';
	        }
	        if(qnt==4){
	            restaurante[1][17] = '*';
	            restaurante[1][18] = '*';
	            restaurante[2][17] = '*';
	            restaurante[2][18] = '*';
	        }
	    }
	    if(s==2){
	        if(qnt==1){
	            restaurante[1][17+3] = '*';
	            restaurante[1][18+3] = '-';
	            restaurante[2][17+3] = '-';
	            restaurante[2][18+3] = '-';
	        }
	        if(qnt==2){
	            restaurante[1][17+3] = '*';
	            restaurante[1][18+3] = '*';
	            restaurante[2][17+3] = '-';
	            restaurante[2][18+3] = '-';
	        }
	        if(qnt==3){
	            restaurante[1][17+3] = '*';
	            restaurante[1][18+3] = '*';
	            restaurante[2][17+3] = '*';
	            restaurante[2][18+3] = '-';
	        }
	        if(qnt==4){
	            restaurante[1][17+3] = '*';
	            restaurante[1][18+3] = '*';
	            restaurante[2][17+3] = '*';
	            restaurante[2][18+3] = '*';
	        }
	    }
	    if(s==3){
	        if(qnt==1){
	            restaurante[1][17+6] = '*';
	            restaurante[1][18+6] = '-';
	            restaurante[2][17+6] = '-';
	            restaurante[2][18+6] = '-';
	        }
	        if(qnt==2){
	            restaurante[1][17+6] = '*';
	            restaurante[1][18+6] = '*';
	            restaurante[2][17+6] = '-';
	            restaurante[2][18+6] = '-';
	        }
	        if(qnt==3){
	            restaurante[1][17+6] = '*';
	            restaurante[1][18+6] = '*';
	            restaurante[2][17+6] = '*';
	            restaurante[2][18+6] = '-';
	        }
	        if(qnt==4){
	            restaurante[1][17+6] = '*';
	            restaurante[1][18+6] = '*';
	            restaurante[2][17+6] = '*';
	            restaurante[2][18+6] = '*';
	        }
	    }
	    if(s==4){
	        if(qnt==1){
	            restaurante[1][17+9] = '*';
	            restaurante[1][18+9] = '-';
	            restaurante[2][17+9] = '-';
	            restaurante[2][18+9] = '-';
	        }
	        if(qnt==2){
	            restaurante[1][17+9] = '*';
	            restaurante[1][18+9] = '*';
	            restaurante[2][17+9] = '-';
	            restaurante[2][18+9] = '-';
	        }
	        if(qnt==3){
	            restaurante[1][17+9] = '*';
	            restaurante[1][18+9] = '*';
	            restaurante[2][17+9] = '*';
	            restaurante[2][18+9] = '-';
	        }
	        if(qnt==4){
	            restaurante[1][17+9] = '*';
	            restaurante[1][18+9] = '*';
	            restaurante[2][17+9] = '*';
	            restaurante[2][18+9] = '*';
	        }
	    }
	    if(s==5){
	        if(qnt==1){
	            restaurante[5][17] = '*';
	            restaurante[5][18] = '-';
	            restaurante[6][17] = '-';
	            restaurante[6][18] = '-';
	        }
	        if(qnt==2){
	            restaurante[5][17] = '*';
	            restaurante[5][18] = '*';
	            restaurante[6][17] = '-';
	            restaurante[6][18] = '-';
	        }
	        if(qnt==3){
	            restaurante[5][17] = '*';
	            restaurante[5][18] = '*';
	            restaurante[6][17] = '*';
	            restaurante[6][18] = '-';
	        }
	        if(qnt==4){
	            restaurante[5][17] = '*';
	            restaurante[5][18] = '*';
	            restaurante[6][17] = '*';
	            restaurante[6][18] = '*';
	        }
	    }
	    if(s==6){
	        if(qnt==1){
	            restaurante[5][17+3] = '*';
	            restaurante[5][18+3] = '-';
	            restaurante[6][17+3] = '-';
	            restaurante[6][18+3] = '-';
	        }
	        if(qnt==2){
	            restaurante[5][17+3] = '*';
	            restaurante[5][18+3] = '*';
	            restaurante[6][17+3] = '-';
	            restaurante[6][18+3] = '-';
	        }
	        if(qnt==3){
	            restaurante[5][17+3] = '*';
	            restaurante[5][18+3] = '*';
	            restaurante[6][17+3] = '*';
	            restaurante[6][18+3] = '-';
	        }
	        if(qnt==4){
	            restaurante[5][17+3] = '*';
	            restaurante[5][18+3] = '*';
	            restaurante[6][17+3] = '*';
	            restaurante[6][18+3] = '*';
	        }
	    }
	    if(s==7){
	        if(qnt==1){
	            restaurante[5][17+6] = '*';
	            restaurante[5][18+6] = '-';
	            restaurante[6][17+6] = '-';
	            restaurante[6][18+6] = '-';
	        }
	        if(qnt==2){
	            restaurante[5][17+6] = '*';
	            restaurante[5][18+6] = '*';
	            restaurante[6][17+6] = '-';
	            restaurante[6][18+6] = '-';
	        }
	        if(qnt==3){
	            restaurante[5][17+6] = '*';
	            restaurante[5][18+6] = '*';
	            restaurante[6][17+6] = '*';
	            restaurante[6][18+6] = '-';
	        }
	        if(qnt==4){
	            restaurante[5][17+6] = '*';
	            restaurante[5][18+6] = '*';
	            restaurante[6][17+6] = '*';
	            restaurante[6][18+6] = '*';
	        }
	    }
	    if(s==8){
	       if(qnt==1){
	            restaurante[5][17+9] = '*';
	            restaurante[5][18+9] = '-';
	            restaurante[6][17+9] = '-';
	            restaurante[6][18+9] = '-';
	        }
	        if(qnt==2){
	            restaurante[5][17+9] = '*';
	            restaurante[5][18+9] = '*';
	            restaurante[6][17+9] = '-';
	            restaurante[6][18+9] = '-';
	        }
	        if(qnt==3){
	            restaurante[5][17+9] = '*';
	            restaurante[5][18+9] = '*';
	            restaurante[6][17+9] = '*';
	            restaurante[6][18+9] = '-';
	        }
	        if(qnt==4){
	            restaurante[5][17+9] = '*';
	            restaurante[5][18+9] = '*';
	            restaurante[6][17+9] = '*';
	            restaurante[6][18+9] = '*';
	        }
	    }
	    if(s==9){
	        if(qnt==1){
	            restaurante[9][17] = '*';
	            restaurante[9][18] = '-';
	            restaurante[10][17] = '-';
	            restaurante[10][18] = '-';
	        }
	        if(qnt==2){
	            restaurante[9][17] = '*';
	            restaurante[9][18] = '*';
	            restaurante[10][17] = '-';
	            restaurante[10][18] = '-';
	        }
	        if(qnt==3){
	            restaurante[9][17] = '*';
	            restaurante[9][18] = '*';
	            restaurante[10][17] = '*';
	            restaurante[10][18] = '-';
	        }
	        if(qnt==4){
	            restaurante[9][17] = '*';
	            restaurante[9][18] = '*';
	            restaurante[10][17] = '*';
	            restaurante[10][18] = '*';
	        }
	    }
	    if(s==10){
	        if(qnt==1){
	            restaurante[9][17+3] = '*';
	            restaurante[9][18+3] = '-';
	            restaurante[10][17+3] = '-';
	            restaurante[10][18+3] = '-';
	        }
	        if(qnt==2){
	            restaurante[9][17+3] = '*';
	            restaurante[9][18+3] = '*';
	            restaurante[10][17+3] = '-';
	            restaurante[10][18+3] = '-';
	        }
	        if(qnt==3){
	            restaurante[9][17+3] = '*';
	            restaurante[9][18+3] = '*';
	            restaurante[10][17+3] = '*';
	            restaurante[10][18+3] = '-';
	        }
	        if(qnt==4){
	            restaurante[9][17+3] = '*';
	            restaurante[9][18+3] = '*';
	            restaurante[10][17+3] = '*';
	            restaurante[10][18+3] = '*';
	        }
	    }
	    if(s==11){
	        if(qnt==1){
	            restaurante[9][17+6] = '*';
	            restaurante[9][18+6] = '-';
	            restaurante[10][17+6] = '-';
	            restaurante[10][18+6] = '-';
	        }
	        if(qnt==2){
	            restaurante[9][17+6] = '*';
	            restaurante[9][18+6] = '*';
	            restaurante[10][17+6] = '-';
	            restaurante[10][18+6] = '-';
	        }
	        if(qnt==3){
	            restaurante[9][17+6] = '*';
	            restaurante[9][18+6] = '*';
	            restaurante[10][17+6] = '*';
	            restaurante[10][18+6] = '-';
	        }
	        if(qnt==4){
	            restaurante[9][17+6] = '*';
	            restaurante[9][18+6] = '*';
	            restaurante[10][17+6] = '*';
	            restaurante[10][18+6] = '*';
	        }
	    }
	    if(s==12){
	        if(qnt==1){
	            restaurante[9][17+9] = '*';
	            restaurante[9][18+9] = '-';
	            restaurante[10][17+9] = '-';
	            restaurante[10][18+9] = '-';
	        }
	        if(qnt==2){
	            restaurante[9][17+9] = '*';
	            restaurante[9][18+9] = '*';
	            restaurante[10][17+9] = '-';
	            restaurante[10][18+9] = '-';
	        }
	        if(qnt==3){
	            restaurante[9][17+9] = '*';
	            restaurante[9][18+9] = '*';
	            restaurante[10][17+9] = '*';
	            restaurante[10][18+9] = '-';
	        }
	        if(qnt==4){
	            restaurante[9][17+9] = '*';
	            restaurante[9][18+9] = '*';
	            restaurante[10][17+9] = '*';
	            restaurante[10][18+9] = '*';
	        }
	    }
	}
}

void animacao(int mesa){
    int i, k=36, l;

    for(i = 0;i < 7; i++){ /*boneco ir ate o meio*/
        restaurante[9][k] = '0';
        if(i > 0){
            restaurante[9][k+1] = ' ';
        }
        sleep(ANIMACAO_DELAY);
        limpar_tela();
        mostrar_arquivo();
        k--;
    }
    k = 30;
    l = 9;
    if (mesa==1 && s<=4) { /* primeira fileira do salao*/
        for(i=0;i<7;i++){
            restaurante[l][k] = '0';
            if(i>0){
                restaurante[l+1][k] = ' ';
            }
            sleep(ANIMACAO_DELAY);
            limpar_tela();
            mostrar_arquivo();
            l--;
        }
        l=3;
        for (i = 0; i < 13; i++){
            restaurante[l][k] = '0';
             if(i>0){
               restaurante[l][k+1] = ' ';         
            }
            if(s==1 && k==12){ /* posicao 1*/
                break;
            }
            if(s==2 && k==19) {/* posicao 2*/
                break;
            }
            if(s==3 && k==22){ /* posicao 3*/
                break;
            }
            if(s==4 && k==26){ /* posicao 4*/
               break;
            }

            sleep(ANIMACAO_DELAY);
            limpar_tela();
            mostrar_arquivo();
            k--;
        }
        
    }  
    else if(mesa==1 && s>4 && s<=8){ /*segunda fileira do salao*/
        for(i=0;i<3;i++){
            restaurante[l][k] = '0';
            if(i>0){
                restaurante[l+1][k] = ' ';
            }
            sleep(ANIMACAO_DELAY);
            limpar_tela();
            mostrar_arquivo();
            l--;
        }
        l=7;
        for (i = 0; i < 13; i++){
            restaurante[l][k] = '0';
             if(i>0){
               restaurante[l][k+1] = ' ';         
            }
            if(s==5 && k==12){
                break;
            }
            if(s==6 && k==19){
                break;
            }
            if(s==7 && k==22){
                break;
            }
            if(s==8 && k==26){
               break;
            }
            sleep(86);
            limpar_tela();
            mostrar_arquivo();
            k--;
        }

    } 
    else if(mesa==1 && s>8 && s<=12){ /*terceira fileira do salao*/
        for(i=0;i<3;i++){
            restaurante[l][k] = '0';
            if(i>0){
                restaurante[l-1][k] = ' ';
            }
            sleep(ANIMACAO_DELAY);
            limpar_tela();
            mostrar_arquivo();
            l++;
        }
        l=11;
        for (i = 0; i < 13; i++){
            restaurante[l][k] = '0';
             if(i>0){
               restaurante[l][k+1] = ' ';         
            }
            if(s==9 && k==12){
                break;
            }
            if(s==10 && k==19){
                break;
            }
            if(s==11 && k==22){
                break;
            }
            if(s==12 && k==26){
               break;
            }
            sleep(86);
            limpar_tela();
            mostrar_arquivo();
            k--;
        }
    }
    if (mesa==2 && v<=4){ /*fileira unica da varanda*/

        for(i=0;i<7;i++){
            restaurante[l][k] = '0';
            if(i>0){
                restaurante[l-1][k] = ' ';
            }
            sleep(ANIMACAO_DELAY);
            limpar_tela();
            mostrar_arquivo();
            l++;
        }
        l=15;
        for (i = 0; i < 13; i++){
            restaurante[l][k] = '0';
             if(i>0){
               restaurante[l][k+1] = ' ';         
            }
            if(v==1 && k==12){
                break;
            }
            if(v==2 && k==19){
                break;
            }
            if(v==3 && k==22){
                break;
            }
            if(v==4 && k==26){
               break;
            }

            sleep(86);
            limpar_tela();
            mostrar_arquivo();
            k--;
        }
    }    
        
    restaurante[l][k] = ' '; /*faz com que o personagem suma*/
    restaurante[l][k+1] = ' '; /*faz com que o personagem suma*/

}


int main() {
    srand(time(0));
    ler_arquivo();

    char continuar, troca_mesa;
    int controle, qnt_pessoas/*pessoas na mesa*/, opc_mesa/*salao ou varanda*/, lotacao/*armazena o resultado da CheckFull, e utiliza-o */, pula=0;

    printf("Deseja entrar no controle do restaurante (S/N) ? ");
    scanf(" %c", &continuar);
    controle = controle_restaurante(continuar);

    if(controle == 0){ /*se ele nao deseja controlar o restaurante, sair do programa */
        return 0;
    }

    limpar_tela();

    mostrar_arquivo();

    Zerar_Vetor();

    while(controle == 1){ /*grande loop, quando a pessoa aceita continuar no controle do restaurante*/

    		

        printf("A mesa eh para quantas pessoas (1 a 4) ? ");
        scanf("%d", &qnt_pessoas);
        
        qnt_pessoas = um_quatro(qnt_pessoas);

        printf("\nDigite 1 para SALAO\n");
        printf("Digite 2 para VARANDA\n\n");
        printf("Qual a sua opcao de mesa? ");
        scanf("%d", &opc_mesa);
        getchar();

        opc_mesa = um_dois(opc_mesa);

        lotacao_restaurante(opc_mesa);

        lotacao = Check_Full(opc_mesa);

        if(lotacao == 1 || lotacao == 2){ /*se a opcao escolhida esta lotada */ 
            printf("A opcao escolhida esta lotada, deseja alterar a opcao (S/N) ? ");
            scanf(" %c", &troca_mesa);
            getchar();
            troca_mesa = sim_nao(troca_mesa);

            if(troca_mesa == 'n' || troca_mesa == 'N'){
                printf("\nObrigado, volte sempre\n");
           		pula=1; /*flag*/
           		sleep(1500);

            }
            if(troca_mesa == 's' || troca_mesa == 'S'){
                if(opc_mesa == 1){ /*troca mesa salao para varanda*/
            		opc_mesa = 2;

            		lotacao_restaurante(opc_mesa);
                    lotacao = Check_Full(opc_mesa);

                    if(lotacao == 2) { /*tudo cheio*/
                        printf("\nDesculpe, o restaurante esta completamente lotado, volte sempre\n");
                        pula = 1; /*flag*/
                        sleep(1500);
                    }
                }
                if(opc_mesa == 2){ /*troca mesa varanda para salao*/
                	opc_mesa = 1;

                	lotacao_restaurante(opc_mesa);
                    lotacao = Check_Full(opc_mesa);

                    if(lotacao == 1) { /*tudo cheio*/
                        printf("\nDesculpe, o restaurante esta completamente lotado, volte sempre\n");
                        pula = 1; /*flag*/
                        sleep(1500);
                    }
                }
            }
           
        }
        if(pula != 1){
        	animacao(opc_mesa);

        	pessoas_mesa(qnt_pessoas, opc_mesa);

        	limpar_tela();
       	 	mostrar_arquivo();
        
        
     	   	printf("Deseja continuar no controle do restaurante (S/N) ? ");
     	   	
        	scanf(" %c", &continuar);
        	getchar();
        	controle = controle_restaurante(continuar);

        }
        else{ /*utilizada se todo o restaurante estiver cheio, é pulado algumas etapas desnecessarias*/

       		limpar_tela();
       	 	mostrar_arquivo();
        
        
     	   	printf("Deseja continuar no controle do restaurante (S/N) ? ");
       
        	scanf(" %c", &continuar);
        	getchar();

        	controle = controle_restaurante(continuar);
    	}
    	pula = 0;

    	desocupar(opc_mesa); 
    }
    
    return 0;
}
