#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define CEDILHA 135
#define ATIL    198
#define AAGUDO  160
#define OTIL    228

#define VENDER_PASSAGEM 1
#define CONSULTAR_CAIXA 2
#define SAIR            3

#define QTDE_POLTRONAS 36

#define RECUO_MARGEM_ESQUERDA   24
#define VAZIO                   32

#define ARESTA_SUPERIOR_ESQUERDA    201
#define ARESTA_SUPERIOR_DIREITA     187
#define LATERAL                     205
#define ARESTA_INFERIOR_ESQUERDA    200
#define ARESTA_INFERIOR_DIREITA     188
#define FRONTAL                     186
#define OFF_SET                     8
#define CORREDOR                    32 // try 32, 176
#define JANELA                      254

//==============================================================================
//    Estrutura de dados para armezenar caracteres ascii acentuados que serão 
//    utilizados nas mensagens.
//==============================================================================
typedef struct {
    char cedilha;
    char atil;
    char aagudo;
    char otil;
} caracteres_acentuados;

typedef struct {
    char    placa[7];
    int     poltronas[QTDE_POLTRONAS];
} onibus;

//==============================================================================
//    Estrutura de dados para armezenar dados de um itinerário.
//==============================================================================
typedef struct {
    char    codigo;
    char    origem[10];
    char    destino[10];
    char    data[10];
    double  valor;
    onibus  onibus;
} itinerario ;


//==============================================================================
//    Declaração de funções.
//==============================================================================
void limpar_console();
void vender_passagem(itinerario *i);
void consultar_caixa();
void exibir_itinerarios(itinerario *itinerarios);
void exibir_menu_principal();
void limpar_buffer_entrada();
void opcao_invalida();
void itinerarios_disponiveis(itinerario itinerarios[3]);
void exibir_poltronas(itinerario itinerario);

//==============================================================================
//    Função principal.
//==============================================================================
int main(int argc, char *argv[]) {

    int opcao;
    
    itinerario itinerarios[3];
    itinerarios_disponiveis(itinerarios);
    
    
	/* executa até o usuário escolher SAIR */
    do {
		exibir_menu_principal();
		
		printf(" Escolha a op%c%co: ", CEDILHA, ATIL);
        scanf("%d", &opcao);

        if (opcao == VENDER_PASSAGEM) {
            vender_passagem(itinerarios);
        } else if (opcao == CONSULTAR_CAIXA) {
            consultar_caixa();
        } else if (opcao == SAIR) {
            limpar_console();
            printf("\n Saindo...\n");
        } else {
           opcao_invalida();
		}
	} while(opcao != SAIR);

    getchar();

    return 0;
}

//==============================================================================
//    Função para limpar o console.
//==============================================================================
void limpar_console(){

    DWORD n;                            // Number of characters written
    DWORD size;                        // Number of visible characters
	COORD coord = {0};                 // Top left screen position
	CONSOLE_SCREEN_BUFFER_INFO csbi;   // Structure 

	// Get a handle to the console
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    // Fill the structure csbi
	GetConsoleScreenBufferInfo(h, &csbi);

	// Find the number of characters to overwrite
	size = csbi.dwSize.X * csbi.dwSize.Y;

	// Overwrite the screen buffer with whitespace
	FillConsoleOutputCharacter(h, TEXT (' '), size, coord, &n);
	
	// Reset the cursor to the top left position
	SetConsoleCursorPosition(h, coord);
}

void vender_passagem(itinerario *i) {
    	
	/* executa até o usuário escolher voltar */
    int itinerario;

    do {
        exibir_itinerarios(i);
        printf(" Escolha o itiner%crio: ", AAGUDO);        

        scanf("%d", &itinerario);
        
        if(itinerario >= 1 && itinerario <= 3) {
            int indice = itinerario - 1;
            exibir_poltronas(i[indice]);
            //printf("itinerario: %c", i[indice].codigo);
            //getchar();
            getchar();
        } else if(itinerario == 4) {
        } else {
            opcao_invalida();				
        }
	} while(itinerario != 4);
}

void consultar_caixa() {
    limpar_console();
    printf("\n");
	printf(" Consultando Caixa\n\n");
	fflush(stdin);
	getchar();
}
/*
    Função para apresentar os itinerários
*/
void exibir_itinerarios(itinerario *i) {
    limpar_console();
    
    printf("\n");
	printf(" Itiner%crios\n\n", AAGUDO);

    int n;
    for(n = 0; n < 3; n++) {
        
        printf(
            " %c - %s x %s\n",
            i[n].codigo,
            i[n].origem,
            i[n].destino
        );
        
    }

    printf(" 4 - Voltar ao menu anterior\n\n");
}

/*
    Função para apresentar o menu principal no console.
*/
void exibir_menu_principal() {
    limpar_console();

	printf("\n");
	printf(" Menu de Op%c%ces\n\n", CEDILHA, OTIL);
    printf(" 1 - Vender Passagem\n");
    printf(" 2 - Consultar Caixa\n");
    printf(" 3 - Sair\n\n");
}

void limpar_buffer_entrada(){
    fflush(stdin);
    //fseek(stdin, 0, SEEK_END);
}

void opcao_invalida(){
    printf("\n Op%c%co inv%clida!", CEDILHA, ATIL, AAGUDO);
    fflush(stdin);
    getchar();
}

void recuo_margem_esquerda(){
    int i;
    for(i = 0; i < RECUO_MARGEM_ESQUERDA; i++){
        printf("%c", VAZIO);
	}
}

void exibir_poltronas(itinerario itinerario) {
    limpar_console();
    
    // TODO: tranferir este trecho para uma function - início
    // lateral direita do ônibus - início
    recuo_margem_esquerda();
    printf("%c", ARESTA_SUPERIOR_ESQUERDA);

    int i;
    for(i = 0; i < QTDE_POLTRONAS - OFF_SET; i++){
        if(i % 3 == 0) {
          printf("%c", JANELA);  
        } else {
            printf("%c", LATERAL);
        }
	}

    printf("%c", ARESTA_SUPERIOR_DIREITA);
    printf("\n");
    // lateral direita do ônibus - fim
    // TODO: tranferir este trecho para uma function - fim
    
    // poltronas ímpares janela da direita - início
    recuo_margem_esquerda();
    printf("%c ", FRONTAL);

    for(i = 2; i < QTDE_POLTRONAS; (i+=4)){
		printf("%02d ", itinerario.onibus.poltronas[i]);
	}

    printf("%c\n", FRONTAL);
    // poltronas ímpares janela da direita - fim

    // poltronas pares corredor, lado direito - início
    recuo_margem_esquerda();
    printf("%c ", FRONTAL);

    for(i = 3; i < QTDE_POLTRONAS; (i+=4)){
		printf("%02d ", itinerario.onibus.poltronas[i]);
	}

    printf("%c\n", FRONTAL);
    // poltronas pares corredor, lado direito - fim

    // corredor - início
    recuo_margem_esquerda();
    printf("%c%c", FRONTAL, CORREDOR);

    for(i = 3; i < QTDE_POLTRONAS; (i+=4)){
		printf("%c%c%c", CORREDOR, CORREDOR, CORREDOR);
	}

    printf("%c\n", FRONTAL);
    // corredor - fim

    // poltronas pares corredor, lado esquero - início
    recuo_margem_esquerda();
    printf("%c ", FRONTAL);

    for(i = 1; i < QTDE_POLTRONAS; (i+=4)){
		printf("%02d ", itinerario.onibus.poltronas[i]);
	}

    printf("%c\n", FRONTAL);
    // poltronas pares corredor, lado esquerdo - fim

    // poltronas ímpares janela da esquerda - início
    recuo_margem_esquerda();
    printf("%c ", FRONTAL);

    for(i = 0; i < QTDE_POLTRONAS; (i+=4)){
		printf("%02d ", itinerario.onibus.poltronas[i]);
	}

    printf("%c\n", FRONTAL);
    // poltronas ímpares janela da esquerda - fim

    // lateral direita do ônibus - início
    recuo_margem_esquerda();
    printf("%c", ARESTA_INFERIOR_ESQUERDA);

    for(i = 0; i < QTDE_POLTRONAS - OFF_SET; i++){
		if(i % 3 == 0) {
            printf("%c", JANELA);  
        } else {
            printf("%c", LATERAL);
        }
	}

    printf("%c", ARESTA_INFERIOR_DIREITA);
    printf("\n");
    // lateral direita do ônibus - fim
    
    /*int i;
    for(i = 0; i < QTDE_POLTRONAS; i++){
		printf("%02d ", itinerario.onibus.poltronas[i]);
	}*/
	getchar();
}

void itinerarios_disponiveis(itinerario *itinerarios) {
    
    itinerarios[0].codigo = '1';
    strcpy(itinerarios[0].origem, "Campinas");
    strcpy(itinerarios[0].destino, "Santos");
    strcpy(itinerarios[0].data, "26/10/2015");
    itinerarios[0].valor = 55.00;
    strcpy(itinerarios[0].onibus.placa, "AAB-1986");
    int i = 0;
    for(i = 0; i < QTDE_POLTRONAS; i++) {
        itinerarios[0].onibus.poltronas[i] = i+1;
    }
    
    itinerarios[1].codigo = '2';
    strcpy(itinerarios[1].origem, "Campinas");
    strcpy(itinerarios[1].destino, "Jundiai");
    strcpy(itinerarios[1].data, "26/10/2015");
    itinerarios[1].valor = 45.00;
    strcpy(itinerarios[1].onibus.placa,"JWS-1982");
    for(i = 0; i < QTDE_POLTRONAS; i++) {
        itinerarios[1].onibus.poltronas[i] = i+1;
    }
    
    itinerarios[2].codigo = '3';
    strcpy(itinerarios[2].origem, "Campinas");
    strcpy(itinerarios[2].destino, "São Paulo");
    strcpy(itinerarios[2].data, "26/10/2015");
    itinerarios[2].valor = 36.00;
    strcpy(itinerarios[2].onibus.placa, "AEJ-435");
    for(i = 0; i < QTDE_POLTRONAS; i++) {
        itinerarios[2].onibus.poltronas[i] = i+1;
    }
}

