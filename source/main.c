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

#define COMUM       1
#define ESTUDANTE   2
#define IDOSO       3

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
    int tipo;   
    int numero_poltrona;
    int valor_pago;
} bilhete;

//==============================================================================
//    Estrutura de dados para armezenar dados de um itinerário.
//==============================================================================
typedef struct {
    char    codigo;
    char    origem[10];
    char    destino[10];
    char    data[10];
    double  valor;
    bilhete bilhetes[QTDE_POLTRONAS];
} itinerario ;


//==============================================================================
//    Declaração de funções auxiliares.
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
itinerario criar_itinerario(int codigo, char origem[10], char destino[10], char data_partida[10], float valor);
void exibir_poltrona(bilhete bilhete);
void exibir_legenda_acentos();

//==============================================================================
//    Função principal.
//==============================================================================
int main(int argc, char *argv[]) {

    //argc      = Quantidade de argumentos passados via linha de comando.
    //argv[0]   = Nome do programa (caminho absoluto)
    //argv[1]   = Primeiro argumento passado via linha de comando
    //arg[n]    = N argumento passado via linha de comando, sendo N diferente de zero
    
    int opcao;
    
    itinerario itinerarios[3];
    itinerarios_disponiveis(itinerarios);
    
	/* executa até o usuário escolher SAIR */
    do {
		exibir_menu_principal();
		
		printf(" Escolha a op%c%co: ", CEDILHA, ATIL);
        scanf("%d", &opcao);

        if (opcao == 1) {
            vender_passagem(itinerarios);
        } else if (opcao == 2) {
            consultar_caixa();
        } else if (opcao == 3) {
            limpar_console();
            //printf("\n Saindo...\n");
        } else {
           opcao_invalida();
		}
	} while(opcao != 3);

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

itinerario criar_itinerario(int codigo, char origem[10], char destino[10], char data_partida[10], float valor) {
    itinerario novo_itinerario;
    
    novo_itinerario.codigo = codigo;
    strcpy(novo_itinerario.origem, origem);
    strcpy(novo_itinerario.destino, destino);
    strcpy(novo_itinerario.data, data_partida);
    novo_itinerario.valor = valor;
    
    int indice;
    
    for(indice = 0; indice < QTDE_POLTRONAS; indice++) {
        int numero_poltrona = indice + 1;
        novo_itinerario.bilhetes[indice].numero_poltrona = numero_poltrona;
        novo_itinerario.bilhetes[indice].tipo = 0;
        novo_itinerario.bilhetes[indice].valor_pago = 0;
    }
    
    return novo_itinerario;
}

/*
    Função para apresentar os itinerários
*/
void exibir_itinerarios(itinerario *itinerarios) {
    limpar_console();
    
    printf("\n");
	printf(" Itiner%crios\n\n", AAGUDO);

    int n;
    for(n = 0; n < 3; n++) {
        printf(" %d - %s x %s\n", itinerarios[n].codigo, itinerarios[n].origem, itinerarios[n].destino);
    }

    printf(" 4 - Voltar ao menu anterior\n\n"); // deveria ser um número que não coincida com o código do itinerário
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
    bilhete bilhete;
    limpar_console();
    
    // TODO: tranferir este trecho para uma function - início
    // lateral direita do ônibus - início
    recuo_margem_esquerda();
    printf("%c", ARESTA_SUPERIOR_ESQUERDA);

    int i;
    for(i = 0; i < QTDE_POLTRONAS - COMPENSACAO; i++){
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

    for(i = 2; i < QTDE_POLTRONAS; (i+=4)) {
        bilhete = itinerario.bilhetes[i];
        if(bilhete.valor_pago == 0) {
            printf("%02d ", bilhete.numero_poltrona);
        } else {
            printf("%c%c%d", VAZIO, VAZIO, bilhete.tipo);
            // vendida
        }
		
	}

    printf("%c\n", FRONTAL);
    // poltronas ímpares janela da direita - fim

    // poltronas pares corredor, lado direito - início
    recuo_margem_esquerda();
    printf("%c ", FRONTAL);

    for(i = 3; i < QTDE_POLTRONAS; (i+=4)){
        bilhete = itinerario.bilhetes[i];
		if(bilhete.valor_pago == 0) {
            printf("%02d ", bilhete.numero_poltrona);
        } else {
            printf("%c%c%d", VAZIO, VAZIO, bilhete.tipo);
            // vendida
        }
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
		bilhete = itinerario.bilhetes[i];
		exibir_poltrona(bilhete);
	}

    printf("%c\n", FRONTAL);
    // poltronas pares corredor, lado esquerdo - fim

    // poltronas ímpares janela da esquerda - início
    recuo_margem_esquerda();
    printf("%c ", FRONTAL);

    for(i = 0; i < QTDE_POLTRONAS; (i+=4)){
		bilhete = itinerario.bilhetes[i];
		bilhete.valor_pago = 1; bilhete.tipo = 3;
		exibir_poltrona(bilhete);
	}

    printf("%c\n", FRONTAL);
    // poltronas ímpares janela da esquerda - fim

    // lateral direita do ônibus - início
    recuo_margem_esquerda();
    printf("%c", ARESTA_INFERIOR_ESQUERDA);

    for(i = 0; i < QTDE_POLTRONAS - COMPENSACAO; i++){
		if(i % 3 == 0) {
            printf("%c", JANELA);  
        } else {
            printf("%c", LATERAL);
        }
	}

    printf("%c", ARESTA_INFERIOR_DIREITA);
    printf("\n");
    // lateral direita do ônibus - fim
    
    exibir_legenda_acentos();
    
    printf("\n");
    recuo_margem_esquerda();
    printf("%s x %s", itinerario.origem, itinerario.destino);
    printf("\n");
    recuo_margem_esquerda();
    printf("Data: %s", itinerario.data);
    printf("\n");
    recuo_margem_esquerda();
    printf("Valor: %.2f", itinerario.valor);
	getchar();
}

void exibir_legenda_acentos(){
    printf("\n");
    recuo_margem_esquerda();
    printf("Verde: acentos livres.");
    printf("\n");
    recuo_margem_esquerda();
    printf("Vermelho: acentos ocupados.");
    printf("\n");
}

void itinerarios_disponiveis(itinerario *itinerarios) {
  
    itinerarios[0] = criar_itinerario(1, "Campinas", "Santos", "26/10/2015", 55.00);
    itinerarios[1] = criar_itinerario(2, "Campinas", "Jundiai", "26/10/2015", 45.00);
    itinerarios[2] = criar_itinerario(3, "Campinas", "Sao Paulo", "26/10/2015", 36.00);
    
}

void exibir_poltrona(bilhete bilhete) {
    HANDLE h = GetStdHandle (STD_OUTPUT_HANDLE);
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    
    /* Primeiro, salvamos a atual configuração de cores do console */
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;
    
    if(bilhete.valor_pago == 0) {
        /* Definimos as novas informações de cores do console */
        /* FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY	*/
        SetConsoleTextAttribute (h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("%02d ", bilhete.numero_poltrona);
    } else {
        /* Definimos as novas informações de cores do console */
        /* FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY	*/
        SetConsoleTextAttribute (h, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("%02d ", bilhete.numero_poltrona);
    }

    /* Restaurando as cores originais */
    SetConsoleTextAttribute (h, wOldColorAttrs);

}

