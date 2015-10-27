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
            printf("itinerario: %c", i[indice].codigo);
            getchar();
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
        printf(" %c - %s x %s :%.2f Placa: %s\tQtde. Poltronas: %d\n",
        i[n].codigo,
        i[n].origem,
        i[n].destino,
        i[n].valor,
        i[n].onibus.placa,
        sizeof(i[n].onibus.poltronas)/sizeof(int)
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

void itinerarios_disponiveis(itinerario *itinerarios) {
    itinerarios[0].codigo = '1';
    strcpy(itinerarios[0].origem, "Campinas");
    strcpy(itinerarios[0].destino, "Santos");
    strcpy(itinerarios[0].data, "26/10/2015");
    itinerarios[0].valor = 55.00;
    itinerarios[0].onibus = (onibus){ "AAB-1986" }  ;
    
    itinerarios[1].codigo = '2';
    strcpy(itinerarios[1].origem, "Campinas");
    strcpy(itinerarios[1].destino, "Jundiai");
    strcpy(itinerarios[1].data, "26/10/2015");
    itinerarios[1].valor = 45.00;
    itinerarios[1].onibus = (onibus){ "JWS-1982" }  ;
    
    itinerarios[2].codigo = '3';
    strcpy(itinerarios[2].origem, "Campinas");
    strcpy(itinerarios[2].destino, "São Paulo");
    strcpy(itinerarios[2].data, "26/10/2015");
    itinerarios[2].valor = 36.00;
    itinerarios[2].onibus = (onibus){ "AEJ-435" }  ;
}

