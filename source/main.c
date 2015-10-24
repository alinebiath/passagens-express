#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/*
    TODO: 
    - ver guia de estilo de comentário em C
*/

/*
================================================================================
    Estruturas de dados
================================================================================
*/

/*
    Estrutura de dados para armezenar caracteres ascii acentuados que serão 
    utilizados nas mensagens.
*/ 
typedef struct {
    char cedilha;
    char atil;
    char aagudo;
    char otil;
} caracteres_acentuados;
/*
================================================================================
*/

/*
================================================================================
    Funções auxiliares
================================================================================
    Função para limpar console
*/
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
/*
================================================================================
    Função que obtém uma estrutura do tipo "caracteres_acentuados"
*/
caracteres_acentuados obter_caracteres_acentuados(){

    caracteres_acentuados caracteres;

    caracteres.cedilha  = 135;
    caracteres.atil     = 198; 
    caracteres.aagudo   = 160;
    caracteres.otil     = 228;
    
    return caracteres;
}

/*
    Função para apresentar o menu principal no console.
*/
void exibir_menu_principal() {

    limpar_console();

    caracteres_acentuados caracteres = obter_caracteres_acentuados();

	printf("\n");
	printf(" Menu de Op%c%ces\n\n", caracteres.cedilha, caracteres.otil);
    printf(" 1 - Vender Passagem\n");
    printf(" 2 - Consultar Caixa\n");
    printf(" 3 - Sair\n\n");
    printf(" Escolha a op%c%co: ", caracteres.cedilha, caracteres.atil);
}

/*
    Função para apresentar os itinerários
*/
void exibir_itinerarios() {
    limpar_console();
    caracteres_acentuados caracteres = obter_caracteres_acentuados();

    printf("\n");
	printf(" Itinerarios\n\n");
    printf(" 1 - Campinas x Santos\n");
    printf(" 2 - Campinas x Jundiai\n");
    printf(" 3 - Campinas x São Paulo\n");
    printf(" 4 - Voltar ao menu anterior\n\n");
    printf(" Escolha o itinerario: ");
}


void vender_passagem() {
    exibir_itinerarios();
}

/*
================================================================================
*/

int main(int argc, char *argv[]) {

    char opcao = '0';
    caracteres_acentuados caracteres = obter_caracteres_acentuados();
	
	/* executa até o usuário escolher SAIR */
	while(opcao != '3') {
		exibir_menu_principal();

        fgets(&opcao, 2, stdin);
		fseek(stdin, 0, SEEK_END);	 	

        switch (opcao) { 
            case '\n':
                printf("\n Pressionou ENTER.");
                getchar();
            break;
    
    	    case '1':
                vender_passagem();
                getchar();
	        break;

            case '2':
                printf("\n Pressionou 2.");
                getchar();
    		break;

            case '3':        
                printf("\n Saindo...\n");
    		break;

            default: 
                //opção inválida
                printf("\n Op%c%co inv%clida!\n Pressione ENTER.", caracteres.cedilha, caracteres.atil, caracteres.aagudo);
                //fseek(stdin, 0, SEEK_END);	 
                //getchar();				
			break;
		}
	}

    getchar();
    return 0;
}
