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

#define QTDE_ITINERARIOS	2

#define SIM					1
#define NAO					2

#define POLTRONA_01			0
#define POLTRONA_02			1
#define POLTRONA_03			2
#define POLTRONA_04			3

#define VOLTAR				0

typedef struct Bilhete Bilhete;
typedef struct Itinerario Itinerario;

//==============================================================================
//  Estrutura de dados para armezenar o tipo do bilhete (COMUM, ESTUDANTE, 
//  IDOSO) , número da poltrona e valor pago pela passagem.
//==============================================================================
struct Bilhete {
    int tipo;   
    int numero_poltrona;
    int valor_pago;
    Itinerario *itinerario;
};

//==============================================================================
//  Estrutura de dados para armezenar dados de um itinerário.
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
//  Declaração de funções auxiliares.
//==============================================================================
void limpar_console();
void vender_passagem(Itinerario *var_itinerario);
void consultar_caixa();
void exibir_itinerarios(Itinerario *vetor_itinerarios);
void exibir_menu_principal();
void limpar_buffer_entrada();
void opcao_invalida();
int exibir_poltronas(Itinerario *itinerario);
void exibir_poltrona(Bilhete *bilhete, int *poltronas_disponiveis);
void exibir_legenda_poltronas();
void desenhar_lateral_onibus();
void desenhar_fileira_poltronas(Itinerario *itinerario, 
								int primeira_poltrona, 
								int *total_poltronas_disponiveis);
void desenhar_corredor();
void processar_venda();
int escolher_tipo_bilhete();
int confirmar_itinerario_selecionado(Itinerario *itinerario);
int calcular_poltronas_livres(Bilhete *vetor_bilhetes);
Itinerario* obter_itinerarios_disponiveis();
void criar_itinerario(int codigo_itinerario, 
                            int numero_onibus,
                            char *origem, 
                            char *destino, 
                            char *data_partida, 
                            float valor,
							Itinerario *novo_itinerario);

void pointer(char* message, Itinerario* itinerario) {
	printf(" %s > *p-> %p\n", message, itinerario);
	getchar();
}
//==============================================================================
//  Função principal.
//==============================================================================
int main(int argc, char *argv[]) {

    //argc      = Quantidade de argumentos passados via linha de comando.
    //argv[0]   = Nome do programa (caminho absoluto)
    //argv[1]   = Primeiro argumento passado via linha de comando
    //arg[n]    = N argumento passado via linha de comando, sendo N diferente de zero
    
    int opcao = -1;
    
    Itinerario *vetor_itinerarios = obter_itinerarios_disponiveis();
    pointer("main", vetor_itinerarios);
    
	/* executa até o usuário escolher SAIR */
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
//  Função para limpar o console.
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

    int codigo_itinerario = -1;
    pointer("vender_passagem", vetor_itinerarios);
    
    /* executa até o usuário escolher voltar */
    do {
        printf(" Escolha o itiner%crio: ", AAGUDO);
		exibir_itinerarios(vetor_itinerarios);

        scanf("%d", &codigo_itinerario);
        
        if (codigo_itinerario >= 1 && codigo_itinerario <= 3) {
			int indice = codigo_itinerario - 1;
			
			Itinerario *itinerario = &vetor_itinerarios[indice];
			pointer("vender_passagem - do while", itinerario);
			getchar();
			
			int confirmacao = confirmar_itinerario_selecionado(itinerario);
			
			if (confirmacao == SIM) {
				pointer("antes exibir poltronas: %p", itinerario);
				int poltronas = exibir_poltronas(itinerario);
				pointer("depois exibir poltronas: %p", itinerario);
				if(poltronas > 0) {
					processar_venda(itinerario);
				}
				
			}
        } else if (codigo_itinerario == SAIR) {
			
        } else {
            opcao_invalida();				
        }
	} while (codigo_itinerario != SAIR);
}

void separador_linha() {
	int indice = 0;
	int tamanho_margem = 80 - 2;
	
	printf(" ");
	for (indice = 0; indice < tamanho_margem; indice++) {
		printf("=");
	}
	printf("\n");
}

int confirmar_itinerario_selecionado(Itinerario *itinerario) {
	pointer("confirmar_itinerario_selecionado > ", itinerario);
	int confirmacao = -1;
    
    do {
    	//limpar_console();
		separador_linha();
		printf(" Dados do Itiner%crio\n\n", AAGUDO, (*itinerario).codigo);
		printf(" Origem:\t%s\n", (*itinerario).origem);
		printf(" Destino:\t%s\n", (*itinerario).destino);
		printf(" Data:\t\t%s\n", (*itinerario).data);	
		printf(" Valor:\t\t%.2f\n", (*itinerario).valor);
		separador_linha();
	
	
		printf(" Confirma a escolha do itiner%crio?\n", AAGUDO);
	    printf(" %d - Sim\n", SIM);
		printf(" %d - N%co\n", NAO, ATIL);
		printf(" ");

        scanf("%d", &confirmacao);
        
        switch (confirmacao) {
        	case SIM:
        		printf("Confirmado Sim\n");
        		getchar();
        		break;
        	case NAO:
        		printf("Confirmado Nao\n");
        		getchar();
        		break;
        	default:
        		opcao_invalida();
		}
	} while (confirmacao < SIM || confirmacao > NAO);

	getchar();	
	return confirmacao;
}

void consultar_caixa() {
    limpar_console();
    printf("\n");
	printf(" Consultando Caixa\n\n");
	fflush(stdin);
	getchar();
}

/*
    Retorna um <itinerario> conforme parâmetros fornecidos.
*/
void criar_itinerario(int codigo_itinerario,
                            int numero_onibus,
                            char *origem, 
                            char *destino, 
                            char *data_partida, 
                            float valor,
							Itinerario *novo_itinerario) {
    
    (*novo_itinerario).codigo = codigo_itinerario;
    (*novo_itinerario).numero_onibus = numero_onibus;
    strcpy((*novo_itinerario).origem, origem);
    strcpy((*novo_itinerario).destino, destino);
    strcpy((*novo_itinerario).data, data_partida);
    (*novo_itinerario).valor = valor;
    
    int indice;
    
    for (indice = 0; indice < QTDE_POLTRONAS; indice++) {
        int numero_poltrona = indice + 1;
        (*novo_itinerario).bilhetes[indice].numero_poltrona = numero_poltrona;
        (*novo_itinerario).bilhetes[indice].tipo = LIVRE;
        (*novo_itinerario).bilhetes[indice].valor_pago = 0;
        (*novo_itinerario).bilhetes[indice].itinerario = novo_itinerario;
    }
    
	printf("criar_itinerario > itinerario: %p\n", novo_itinerario);			
}

/*
    Função para apresentar os itinerários
*/
void exibir_itinerarios(Itinerario *itinerarios) {
	
    //limpar_console();
    
    printf("\n");
	printf(" Itiner%crios\n\n", AAGUDO);

    int index;
    for(index = 0; index < QTDE_ITINERARIOS; index++) {
    	//printf("exibir_itinerarios > itinerarios: %p\n", itinerarios);
        printf(" %d - %s x %s *p->%p\n", 
                (*itinerarios).codigo,
                (*itinerarios).origem, 
                (*itinerarios).destino,
				itinerarios);
        itinerarios++;
    }
	itinerarios -= QTDE_ITINERARIOS;
	//itinerarios = &itinerarios[0];
	pointer("exibir_itinerarios > saindo >", itinerarios);
    printf(" %d - Voltar ao menu anterior\n\n", SAIR);
}

/*
    Função para apresentar o menu principal no console.
*/
void exibir_menu_principal() {
    //limpar_console();

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

void desenhar_lateral_onibus() {
    recuo_margem_esquerda();
    printf("%c", ARESTA_SUPERIOR_ESQUERDA);

    int i;
    for (i = 0; i < QTDE_POLTRONAS - COMPENSACAO; i++) {
    	// lógica para imprimir o caracter que representa a janela ou não
        if (i % 3 == 0) {
          printf("%c", JANELA);  
        } else {
            printf("%c", LATERAL);
        }
	}

    printf("%c", ARESTA_SUPERIOR_DIREITA);
    printf("\n");
}

void desenhar_fileira_poltronas(Itinerario *itinerario, 
								int primeira_poltrona, 
								int *total_poltronas_disponiveis) {
    printf("desenhar_fileira_poltronas > itinerario: %p\n", itinerario);
	recuo_margem_esquerda();
    printf("%c ", FRONTAL);

	int i;
	Bilhete *vetor_bilhetes = (*itinerario).bilhetes;
	Bilhete *bilhete;
	
    for (i = primeira_poltrona; i < QTDE_POLTRONAS; (i += SALTO_POLTRONA)) {
        bilhete = &vetor_bilhetes[i];
        exibir_poltrona(bilhete, total_poltronas_disponiveis);
	}

    printf("%c\n", FRONTAL);
}

void desenhar_corredor() {
	int i;

    recuo_margem_esquerda();
    printf("%c%c", FRONTAL, CORREDOR);

    for (i = 3; i < QTDE_POLTRONAS; (i += SALTO_POLTRONA)) {
		printf("%c%c%c", CORREDOR, CORREDOR, CORREDOR);
	}

    printf("%c\n", FRONTAL);
}

void desenhar_onibus(Itinerario *itinerario, int *total_poltronas_disponiveis) {
	printf("desenhar_onibus > itinerario: %p\n", itinerario);
	// disposição dos assentos seguindo o modelo de empresa Cometa
	desenhar_lateral_onibus();
    desenhar_fileira_poltronas(itinerario, POLTRONA_03, total_poltronas_disponiveis);
    desenhar_fileira_poltronas(itinerario, POLTRONA_04, total_poltronas_disponiveis);
    desenhar_corredor();
	desenhar_fileira_poltronas(itinerario, POLTRONA_02, total_poltronas_disponiveis);
	desenhar_fileira_poltronas(itinerario, POLTRONA_01, total_poltronas_disponiveis);
	desenhar_lateral_onibus();
}

int exibir_poltronas(Itinerario* itinerario) {
	printf("exibir_poltronas > itinerario: %p\n", itinerario);
    int total_poltronas_disponiveis = 0;
    
    //limpar_console();
    
    desenhar_onibus(itinerario, &total_poltronas_disponiveis);
    
    exibir_legenda_poltronas();
    
    printf("\n");
    recuo_margem_esquerda();
    printf("%s x %s", (*itinerario).origem, (*itinerario).destino);
    printf("\n");
    recuo_margem_esquerda();
    printf("Data: %s", (*itinerario).data);
    printf("\n");
    recuo_margem_esquerda();
    printf("Valor: %.2f", (*itinerario).valor);
    
    if (total_poltronas_disponiveis == 0) {
    	printf("\n\n N%co h%c mais poltronas livres!", ATIL, AAGUDO);
    	getchar();
	} else {
		printf("\n\n Total de poltronas livres: %d", total_poltronas_disponiveis);
		getchar();
	}
	
	return total_poltronas_disponiveis;
	
}

void processar_venda(Itinerario *itinerario) {
    
    int tipo_bilhete = escolher_tipo_bilhete(itinerario);
    printf(" %d - TIPO\n", tipo_bilhete);
    getchar();
    getchar();
    
	if (tipo_bilhete = -1) {
        processar_venda(itinerario);
    }
    
    
}

int escolher_poltrona(Bilhete *vetor_bilhetes, int tipo_bilhete) {
	int poltrona_escolhida = -1;
	
	do {
    	//limpar_console();
		printf("\n\n");
		separador_linha();
		printf(" Informe o no. da poltrona desejada\n");
    	printf(" %d - Voltar\n\n", 0);

        scanf("%d", &poltrona_escolhida);
        
        if (poltrona_escolhida == 0) {
        	// just return zero
		} else {
			if (tipo_bilhete == IDOSO) {
						
			}
			
		}
        
        switch (tipo_bilhete) {
        	case IDOSO:
        		//aplicar desconto de 100%
        		break;
        	case ESTUDANTE:
        		//aplicar desconto de 50%
        		break;
        	case COMUM:
        		break;
        	case SAIR:
	       		break;
        	default:
        		opcao_invalida();
		}
	} while (tipo_bilhete != SAIR && tipo_bilhete != COMUM && 
			tipo_bilhete != ESTUDANTE && tipo_bilhete != IDOSO);
	
}

int escolher_tipo_bilhete(Itinerario *itinerario) {
	int tipo_bilhete = -1;
    pointer("escolher tipo bilhete: %p", itinerario);
    Bilhete *vetor_bilhetes = (*itinerario).bilhetes;
    do {
    	//limpar_console();
		printf("\n\n");
		separador_linha();
		printf(" Informe o tipo da passagem\n");
		printf(" %d - COMUM\n", COMUM);
    	printf(" %d - ESTUDANTE\n", ESTUDANTE);
    	printf(" %d - IDOSO\n", IDOSO);
    	printf(" %d - Voltar\n\n", VOLTAR);
		
		printf(" Tipo bilhete: ");

        scanf("%d", &tipo_bilhete);
        
        int i, poltronas_idoso_ocupadas = 0;
        
        switch (tipo_bilhete) {
        	case IDOSO:
				for (i = 0; i < QTDE_POLTRONAS; i++) {
					Bilhete bilhete = vetor_bilhetes[i];
					if (bilhete.tipo == IDOSO) {
						poltronas_idoso_ocupadas++;
						if (poltronas_idoso_ocupadas == 2) {
							printf(" Limite de passagens para idosos excedido!\n");
							tipo_bilhete = 0;
							getchar();
							break;
						}
					}
				}
        		break;
        	case ESTUDANTE:
        	case COMUM:
        	case SAIR:
	       		break;
        	default:
        		opcao_invalida();
        		pointer("saindo escolher tipo bilhete: %p", itinerario);
        		exibir_poltronas(itinerario);
		}
	} while (tipo_bilhete != SAIR && tipo_bilhete != COMUM && 
			tipo_bilhete != ESTUDANTE && tipo_bilhete != IDOSO);
    
    return tipo_bilhete;
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
    Retorna um vetor com QTDE_ITINERARIOS elementos do tipo <itinerario>
*/
Itinerario * obter_itinerarios_disponiveis() {
    
    Itinerario *vetor_itinerarios = (Itinerario*) malloc(QTDE_ITINERARIOS * sizeof(Itinerario));
    criar_itinerario(1, 100, "Campinas", "Santos", "26/10/2015", 55.00, &vetor_itinerarios[0]);
    criar_itinerario(2, 200, "Campinas", "Cerquilho", "26/10/2015", 45.00, &vetor_itinerarios[1]);

	pointer("obter_itinerarios_disponiveis > vetor 0: ", &vetor_itinerarios[0]);
	pointer("obter_itinerarios_disponiveis > vetor 1: ", &vetor_itinerarios[1]);
	
    return vetor_itinerarios;
}

void exibir_poltrona(Bilhete *var_bilhete, int *poltronas_disponives) {
    HANDLE h = GetStdHandle (STD_OUTPUT_HANDLE);
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    
    /* Primeiro, salvamos a atual configuração de cores do console */
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;

    
    if ((*var_bilhete).tipo == LIVRE) {
        /* Definimos as novas informações de cores do console */
        /* FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY	*/
        SetConsoleTextAttribute (h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("%02d ", (*var_bilhete).numero_poltrona);
        (*poltronas_disponives)++;
    } else {
        /* Definimos as novas informações de cores do console */
        /* FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY	*/
        SetConsoleTextAttribute (h, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("%02d ", (*var_bilhete).numero_poltrona);
    }

    /* Restaurando as cores originais */
    SetConsoleTextAttribute (h, wOldColorAttrs);

}

