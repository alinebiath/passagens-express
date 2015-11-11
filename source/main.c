#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define CEDILHA 135
#define ATIL    198
#define AAGUDO  160
#define OTIL    228

#define QTDE_POLTRONAS 40

#define RECUO_MARGEM_ESQUERDA   24
#define VAZIO                   32

#define ARESTA_SUPERIOR_ESQUERDA    201
#define ARESTA_SUPERIOR_DIREITA     187
#define LATERAL                     205
#define ARESTA_INFERIOR_ESQUERDA    200
#define ARESTA_INFERIOR_DIREITA     188
#define FRONTAL                     186
#define COMPENSACAO                 (QTDE_POLTRONAS / 4 - 1)
#define CORREDOR                    32 // try 32, 176
#define JANELA                      254

#define LIVRE               0
#define COMUM               1
#define ESTUDANTE           2
#define IDOSO               3

#define SALTO_POLTRONA      4

#define SAIR				0
#define VENDER_PASSAGEM		1
#define CONSULTAR_CAIXA		2

typedef struct Bilhete Bilhete;
typedef struct Itinerario Itinerario;

//==============================================================================
//  Estrutura de dados para armezenar o tipo do bilhete (COMUM, ESTUDANTE, 
//  IDOSO) , n�mero da poltrona e valor pago pela passagem.
//==============================================================================
struct Bilhete {
    int tipo;   
    int numero_poltrona;
    int valor_pago;
    Itinerario *itinerario;
};

//==============================================================================
//  Estrutura de dados para armezenar dados de um itiner�rio.
//==============================================================================
struct Itinerario {
    char    codigo;
    char    origem[10];
    char    destino[10];
    char    data[10];
    double  valor;
    int     numero_onibus;
    Bilhete bilhetes[QTDE_POLTRONAS];
};

//==============================================================================
//  Declara��o de fun��es auxiliares.
//==============================================================================
void limpar_console();
void vender_passagem(Itinerario *var_itinerario);
void consultar_caixa();
void exibir_itinerarios(Itinerario *vetor_itinerarios);
void exibir_menu_principal();
void limpar_buffer_entrada();
void opcao_invalida();
void exibir_poltronas(Itinerario itinerario);
void exibir_poltrona(Bilhete bilhete, int *poltronas_disponiveis);
void exibir_legenda_poltronas();
int calcular_poltronas_livres(Bilhete *vetor_bilhetes);
Itinerario* obter_itinerarios_disponiveis();
Itinerario criar_itinerario(int codigo_itinerario, 
                            int numero_onibus,
                            char *origem, 
                            char *destino, 
                            char *data_partida, 
                            float valor);

//==============================================================================
//  Fun��o principal.
//==============================================================================
int main(int argc, char *argv[]) {

    //argc      = Quantidade de argumentos passados via linha de comando.
    //argv[0]   = Nome do programa (caminho absoluto)
    //argv[1]   = Primeiro argumento passado via linha de comando
    //arg[n]    = N argumento passado via linha de comando, sendo N diferente de zero
    
    int opcao = -1;
    
    Itinerario *vetor_itinerarios = obter_itinerarios_disponiveis();
    
	/* executa at� o usu�rio escolher SAIR */
    do {
		exibir_menu_principal();
		
		printf(" Escolha a op%c%co: ", CEDILHA, ATIL);
        scanf("%d", &opcao);

        if (opcao == VENDER_PASSAGEM) {
            vender_passagem(vetor_itinerarios);
        } else if (opcao == CONSULTAR_CAIXA) {
            consultar_caixa();
        } else if (opcao == SAIR) {
            limpar_console();
            //printf("\n Saindo...\n");
        } else {
           opcao_invalida();
		}
	} while (opcao != SAIR);

    getchar();

    free(vetor_itinerarios);

    return 0;
}

//==============================================================================
//  Fun��o para limpar o console.
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

void vender_passagem(Itinerario *vetor_itinerarios) {

    int codigo_itinerario;
    
    /* executa at� o usu�rio escolher voltar */
    do {
        exibir_itinerarios(vetor_itinerarios);
        printf(" Escolha o itiner%crio: ", AAGUDO);        

        scanf("%d", &codigo_itinerario);
        
        if (codigo_itinerario >= 1 && codigo_itinerario <= 3) {
            int indice = codigo_itinerario - 1;
            exibir_poltronas(vetor_itinerarios[indice]);
            getchar();
        } else if(codigo_itinerario == 4) {
        } else {
            opcao_invalida();				
        }
	} while(codigo_itinerario != 4);
}

void consultar_caixa() {
    limpar_console();
    printf("\n");
	printf(" Consultando Caixa\n\n");
	fflush(stdin);
	getchar();
}

/*
    Retorna um <itinerario> conforme par�metros fornecidos.
*/
Itinerario criar_itinerario(int codigo_itinerario,
                            int numero_onibus,
                            char *origem, 
                            char *destino, 
                            char *data_partida, 
                            float valor) {
    Itinerario novo_itinerario;
    
    novo_itinerario.codigo = codigo_itinerario;
    novo_itinerario.numero_onibus = numero_onibus;
    strcpy(novo_itinerario.origem, origem);
    strcpy(novo_itinerario.destino, destino);
    strcpy(novo_itinerario.data, data_partida);
    novo_itinerario.valor = valor;
    
    int indice;
    
    for (indice = 0; indice < QTDE_POLTRONAS; indice++) {
        int numero_poltrona = indice + 1;
        novo_itinerario.bilhetes[indice].numero_poltrona = numero_poltrona;
        novo_itinerario.bilhetes[indice].tipo = LIVRE;
        novo_itinerario.bilhetes[indice].valor_pago = 0;
        novo_itinerario.bilhetes[indice].itinerario = &novo_itinerario;
    }
    
    return novo_itinerario;
}

/*
    Fun��o para apresentar os itiner�rios
*/
void exibir_itinerarios(Itinerario *itinerarios) {
    limpar_console();
    
    printf("\n");
	printf(" Itiner%crios\n\n", AAGUDO);

    int n;
    for(n = 0; n < 3; n++) {
        printf(" %d - %s x %s\n", 
                itinerarios[n].codigo,
                itinerarios[n].origem, 
                itinerarios[n].destino);
    }

    printf(" 4 - Voltar ao menu anterior\n\n"); // deveria ser um n�mero que n�o coincida com o c�digo do itiner�rio
}

/*
    Fun��o para apresentar o menu principal no console.
*/
void exibir_menu_principal() {
    limpar_console();

	printf("\n");
	printf(" Menu de Op%c%ces\n\n", CEDILHA, OTIL);
    printf(" %d - Vender Passagem\n", VENDER_PASSAGEM);
    printf(" %d - Consultar Caixa\n", CONSULTAR_CAIXA);
    printf(" %d - Sair\n\n", SAIR);
}

void limpar_buffer_entrada() {
    fflush(stdin);
    //fseek(stdin, 0, SEEK_END);
}

void opcao_invalida() {
    printf("\n Op%c%co inv%clida!", CEDILHA, ATIL, AAGUDO);
    fflush(stdin);
    getchar();
}

void recuo_margem_esquerda() {
    int i;
    for (i = 0; i < RECUO_MARGEM_ESQUERDA; i++) {
        printf("%c", VAZIO);
	}
}

void exibir_poltronas(Itinerario itinerario) {
    int total_poltronas_disponiveis = 0;
    Bilhete bilhete;
    limpar_console();
    
    // TODO: tranferir este trecho para uma function - in�cio
    // lateral direita do �nibus - in�cio
    recuo_margem_esquerda();
    printf("%c", ARESTA_SUPERIOR_ESQUERDA);

    int i;
    for (i = 0; i < QTDE_POLTRONAS - COMPENSACAO; i++) {
        if (i % 3 == 0) {
          printf("%c", JANELA);  
        } else {
            printf("%c", LATERAL);
        }
	}

    printf("%c", ARESTA_SUPERIOR_DIREITA);
    printf("\n");
    // lateral direita do �nibus - fim
    // TODO: tranferir este trecho para uma function - fim
    
    // poltronas �mpares janela da direita - in�cio
    recuo_margem_esquerda();
    printf("%c ", FRONTAL);

    for (i = 2; i < QTDE_POLTRONAS; (i += SALTO_POLTRONA)) {
        bilhete = itinerario.bilhetes[i];
        exibir_poltrona(bilhete, &total_poltronas_disponiveis);
	}

    printf("%c\n", FRONTAL);
    // poltronas �mpares janela da direita - fim

    // poltronas pares corredor, lado direito - in�cio
    recuo_margem_esquerda();
    printf("%c ", FRONTAL);

    for (i = 3; i < QTDE_POLTRONAS; (i += SALTO_POLTRONA)) {
        bilhete = itinerario.bilhetes[i];
		exibir_poltrona(bilhete, &total_poltronas_disponiveis);
	}

    printf("%c\n", FRONTAL);
    // poltronas pares corredor, lado direito - fim

    // corredor - in�cio
    recuo_margem_esquerda();
    printf("%c%c", FRONTAL, CORREDOR);

    for (i = 3; i < QTDE_POLTRONAS; (i += SALTO_POLTRONA)) {
		printf("%c%c%c", CORREDOR, CORREDOR, CORREDOR);
	}

    printf("%c\n", FRONTAL);
    // corredor - fim

    // poltronas pares corredor, lado esquero - in�cio
    recuo_margem_esquerda();
    printf("%c ", FRONTAL);

    for (i = 1; i < QTDE_POLTRONAS; (i += SALTO_POLTRONA)) {
		bilhete = itinerario.bilhetes[i];
		exibir_poltrona(bilhete, &total_poltronas_disponiveis);
	}

    printf("%c\n", FRONTAL);
    // poltronas pares corredor, lado esquerdo - fim

    // poltronas �mpares janela da esquerda - in�cio
    recuo_margem_esquerda();
    printf("%c ", FRONTAL);
    
    int qtde_poltronas_livres;

    for (i = 0; i < QTDE_POLTRONAS; (i += SALTO_POLTRONA)) {
		itinerario.bilhetes[i].tipo = IDOSO;
		bilhete = itinerario.bilhetes[i];
		exibir_poltrona(bilhete, &total_poltronas_disponiveis);
	}

    printf("%c\n", FRONTAL);
    // poltronas �mpares janela da esquerda - fim

    // lateral direita do �nibus - in�cio
    recuo_margem_esquerda();
    printf("%c", ARESTA_INFERIOR_ESQUERDA);

    for (i = 0; i < QTDE_POLTRONAS - COMPENSACAO; i++) {
		if (i % 3 == 0) {
            printf("%c", JANELA);  
        } else {
            printf("%c", LATERAL);
        }
	}

    printf("%c", ARESTA_INFERIOR_DIREITA);
    printf("\n");
    // lateral direita do �nibus - fim
    
    exibir_legenda_poltronas();
    
    printf("\n");
    recuo_margem_esquerda();
    printf("%s x %s", itinerario.origem, itinerario.destino);
    printf("\n");
    recuo_margem_esquerda();
    printf("Data: %s", itinerario.data);
    printf("\n");
    recuo_margem_esquerda();
    printf("Valor: %.2f", itinerario.valor);
    
    printf("\n\n Poltronas livres: %d", total_poltronas_disponiveis);
	getchar();
}

void exibir_legenda_poltronas() {
    printf("\n");
    recuo_margem_esquerda();
    printf("Verde: acentos livres.");
    printf("\n");
    recuo_margem_esquerda();
    printf("Vermelho: acentos ocupados.");
    printf("\n");
}

/*
    Retorna um vetor com 3 elementos do tipo <itinerario>
*/
Itinerario * obter_itinerarios_disponiveis() {
    int numero_de_elementos = 3;
    
    Itinerario *vetor_itinerarios = malloc(numero_de_elementos * sizeof(Itinerario));
    vetor_itinerarios[0] = criar_itinerario(1, 100, "Campinas", "Santos", "26/10/2015", 55.00);
    vetor_itinerarios[1] = criar_itinerario(2, 200, "Campinas", "Jundiai", "26/10/2015", 45.00);
    vetor_itinerarios[2] = criar_itinerario(3, 300, "Campinas", "Sao Paulo", "26/10/2015", 36.00);

    return vetor_itinerarios;
}

void exibir_poltrona(Bilhete var_bilhete, int *poltronas_disponives) {
    HANDLE h = GetStdHandle (STD_OUTPUT_HANDLE);
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    
    /* Primeiro, salvamos a atual configura��o de cores do console */
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;
    
    if (var_bilhete.tipo == LIVRE) {
        /* Definimos as novas informa��es de cores do console */
        /* FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY	*/
        SetConsoleTextAttribute (h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("%02d ", var_bilhete.numero_poltrona);
        (*poltronas_disponives)++;
    } else {
        /* Definimos as novas informa��es de cores do console */
        /* FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY	*/
        SetConsoleTextAttribute (h, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("%02d ", var_bilhete.numero_poltrona);
    }

    /* Restaurando as cores originais */
    SetConsoleTextAttribute (h, wOldColorAttrs);

}

