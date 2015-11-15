#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define COLUNAS 80
#define LINHAS  45

#define CEDILHA 135
#define ATIL    198
#define AAGUDO  160
#define OTIL    228
#define IAGUDO  161

#define QTDE_POLTRONAS 8

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
#define CANCELAR            0
#define OPCAO_INVALIDA      -1

typedef struct Bilhete Bilhete;
typedef struct Itinerario Itinerario;

//==============================================================================
//  Estrutura de dados para armezenar o tipo do bilhete (COMUM, ESTUDANTE, 
//  IDOSO) , número da poltrona e valor pago pela passagem.
//==============================================================================
struct Bilhete {
    int tipo;   
    int numero_poltrona;
    float valor_pago;
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
void aux_configurar_janela_console();
void aux_limpar_janela_console();
void aux_recuar_margem_esquerda();
void aux_imprimir_titulo();
void aux_imprimir_funcionalidade(char *funcionalidade);
void aux_imprimir_itinerarios(Itinerario *vetor_itinerarios);
void aux_imprimir_menu_principal();
void aux_imprimir_opcao_invalida();
void aux_imprimir_separador_linha();
void aux_exibir_poltronas(Itinerario *itinerario);
void aux_desenhar_onibus(Itinerario *itinerario);
void aux_imprimir_poltrona(Bilhete *bilhete);
void aux_desenhar_lateral_direita_onibus();
void aux_desenhar_lateral_esquerda_onibus();
void aux_desenhar_fileira_poltronas(Itinerario *itinerario, int primeira_poltrona);
void aux_desenhar_corredor();
void aux_finalizar_venda(Bilhete *bilhete);
void aux_finalizar_aplicativo();
void aux_criar_itinerario(int codigo_itinerario, 
                            int numero_onibus,
                            char *origem, 
                            char *destino, 
                            char *data_partida, 
                            float valor,
							Itinerario *novo_itinerario);


void vender_passagem(Itinerario *var_itinerario);
void consultar_caixa(Itinerario *tinerarios);
void processar_venda(Itinerario *itinerario, Itinerario *vetor_itinerarios);

int aux_verificar_poltronas_livres(Itinerario *itinerario);
int aux_escolher_tipo_bilhete(Itinerario *itinerario);
int aux_escolher_poltrona(Itinerario *itinerario, int tipo_bilhete);
int aux_confirmar_itinerario(Itinerario *itinerario);


Itinerario* aux_carregar_itinerarios();

void pointer(char* message, Itinerario* itinerario) {
	printf(" %s > %x\n", message, itinerario);
}


//==============================================================================
//  Função principal.
//==============================================================================
int main(int argc, char *argv[]) {

    //argc      = Quantidade de argumentos passados via linha de comando.
    //argv[0]   = Nome do programa (caminho absoluto)
    //argv[1]   = Primeiro argumento passado via linha de comando
    //arg[n]    = N argumento passado via linha de comando, sendo N diferente de zero
    aux_configurar_janela_console();
    
    int opcao;
    
    Itinerario *vetor_itinerarios = aux_carregar_itinerarios();
    //pointer("1 - main", vetor_itinerarios);
    
	/* executa até o usuário escolher SAIR */
    do {
        //limpar_console();
        //separador_linha();
        opcao = OPCAO_INVALIDA;
		aux_imprimir_menu_principal();
        //separador_linha();
		printf("\n Escolha a op%c%co: ", CEDILHA, ATIL);
        scanf("%d", &opcao);

        if (opcao == VENDER_PASSAGEM) {
            vender_passagem(vetor_itinerarios);
        } else if (opcao == CONSULTAR_CAIXA) {
            consultar_caixa(vetor_itinerarios);
        } else if (opcao == SAIR) {
            aux_finalizar_aplicativo();
        } else {
            aux_imprimir_opcao_invalida();
		}
	} while (opcao != SAIR);

    getchar();

    free(vetor_itinerarios);

    return 0;
}

void vender_passagem(Itinerario *vetor_itinerarios) {
    int codigo_itinerario;
    
    /* executa até o usuário escolher voltar */
    do {
        codigo_itinerario = OPCAO_INVALIDA;
        aux_imprimir_funcionalidade("Vender Passagem > Selecionar Itinerario");
		aux_imprimir_itinerarios(vetor_itinerarios);
		printf(" %d - Voltar ao menu principal\n", VOLTAR);
		aux_imprimir_separador_linha();
		printf(" \n Informe o itiner%crio desejado: ", AAGUDO);
        scanf("%d", &codigo_itinerario);
        
        Itinerario *p = vetor_itinerarios;
        Itinerario *selecionado = NULL;
        int i;
        
        if (codigo_itinerario == OPCAO_INVALIDA) {
            aux_imprimir_opcao_invalida();
        } else if (codigo_itinerario != SAIR) {
            
           for (i = 0; i < QTDE_ITINERARIOS; i++) {
                if ((*p).codigo == codigo_itinerario) {
                    selecionado = p;
                    i = QTDE_ITINERARIOS;
                }
                p++;
            }
            
            if (selecionado != NULL) {
                int confirmacao = aux_confirmar_itinerario(selecionado);
            
                if (confirmacao == SIM) {
    				int poltronas_livres = aux_verificar_poltronas_livres(selecionado);
    				//printf("livres: %d\n ", poltronas_livres); getchar();
    				
    				if(poltronas_livres > 0) {
    					processar_venda(selecionado, vetor_itinerarios);
    				} else {
    				    aux_exibir_poltronas(selecionado);
    				    printf("\n N%co h%c poltronas dispon%cveis para este itinerario!", 
                        ATIL, AAGUDO, IAGUDO, AAGUDO);
                        getchar();
                        getchar();
                    }
    			}
            }
        }
	} while (codigo_itinerario != SAIR);
}


void consultar_caixa(Itinerario *itinerarios) {
    aux_imprimir_funcionalidade("Consultar Caixa");
	getchar();
}

void processar_venda(Itinerario *itinerario, Itinerario *vetor_itinerarios) {
    int tipo_bilhete;
    int poltrona_escolhida;
    
    do {
        tipo_bilhete = OPCAO_INVALIDA;
        poltrona_escolhida = OPCAO_INVALIDA;
    
        tipo_bilhete = aux_escolher_tipo_bilhete(itinerario);
        
        switch (tipo_bilhete) {
            case IDOSO:
            case ESTUDANTE:
            case COMUM:
            case CANCELAR:
                break;
            default:
                aux_imprimir_opcao_invalida();
        }
    } while (tipo_bilhete == OPCAO_INVALIDA);
    
    if (tipo_bilhete != CANCELAR) {
        do {
            poltrona_escolhida = aux_escolher_poltrona(itinerario, tipo_bilhete);
            if (poltrona_escolhida != VOLTAR) {
                int indice = poltrona_escolhida - 1;
                Bilhete *bilhete = &(*itinerario).bilhetes[indice];
                (*bilhete).tipo = tipo_bilhete;
                
                switch (tipo_bilhete) {
                    case COMUM:
                        (*bilhete).valor_pago = (*itinerario).valor;
                        break;
                    case ESTUDANTE:
                        (*bilhete).valor_pago = (*itinerario).valor * 0.5;
                        break;
                    case IDOSO:
                        (*bilhete).valor_pago = 0;
                        break;
                }
                
                printf(
                    "Poltrona: %d - tipo: %d - valor pago: %.2f\n", 
                    (*bilhete).numero_poltrona, 
                    (*bilhete).tipo,
                    (*bilhete).valor_pago
                );
                getchar();
                getchar();
                // confirmar a venda
            }
            
        } while (poltrona_escolhida == OPCAO_INVALIDA);
    }
   
    //vender_passagem(vetor_itinerarios);
}

//==============================================================================
//  Funções Auxiliares
//==============================================================================

void aux_finalizar_aplicativo() {
    aux_limpar_janela_console();
    printf("\n Finalizando aplicativo... [Pressione qualquer tecla]");
    getchar();
}

int aux_escolher_poltrona(Itinerario *itinerario, int tipo_bilhete) {
	
    int poltrona_escolhida;
	
	do {
	    poltrona_escolhida = OPCAO_INVALIDA;
	    aux_exibir_poltronas(itinerario);
		
    	printf(" %d - Cancelar\n", CANCELAR);
    	aux_imprimir_separador_linha();
        printf("\n Informe o n%c da poltrona desejada: ", 167);

        scanf("%d", &poltrona_escolhida);
        
        if (poltrona_escolhida < 0 || poltrona_escolhida > QTDE_POLTRONAS) {
            aux_imprimir_opcao_invalida();
            poltrona_escolhida = OPCAO_INVALIDA;
		} else {
		    
        }
	} while (poltrona_escolhida == OPCAO_INVALIDA);
			
	return poltrona_escolhida;
}

//http://stackoverflow.com/questions/17160553/making-the-console-window-bigger-in-c
void aux_configurar_janela_console() {
    SMALL_RECT consoleWindowSize; 

    HANDLE hStdout;
    COORD coord;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    coord.X = COLUNAS;
    coord.Y = LINHAS;
    SetConsoleScreenBufferSize(hStdout, coord);

    consoleWindowSize.Left = 0;
    consoleWindowSize.Top = 0;
    consoleWindowSize.Right = coord.X - 1;
    consoleWindowSize.Bottom = coord.Y - 1;

    SetConsoleWindowInfo(hStdout, TRUE, &consoleWindowSize);
}

void aux_limpar_janela_console(){

    DWORD n;                            // Number of characters written
    DWORD size;                        // Number of visible characters
	COORD coord = {0, 0};                 // Top left screen position
	CONSOLE_SCREEN_BUFFER_INFO csbi;   // Structure 

    SetConsoleTitle("Passagens Express");

	// Get a handle to the console
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	
    SMALL_RECT windowSize = {0, 80, 0, 30};
    //SetConsoleWindowInfo(h, 1, &windowSize);

    // Fill the structure csbi
	GetConsoleScreenBufferInfo(h, &csbi);

	// Find the number of characters to overwrite
	size = csbi.dwSize.X * csbi.dwSize.Y;

	// Overwrite the screen buffer with whitespace
	FillConsoleOutputCharacter(h, TEXT (' '), size, coord, &n);
	
	// Reset the cursor to the top left position
	SetConsoleCursorPosition(h, coord);
}

void aux_imprimir_menu_principal() {
    aux_imprimir_funcionalidade("Menu Principal");
    //printf(" Menu Principal\n");
    //separador_linha();
    printf(" %d - Vender Passagem\n", VENDER_PASSAGEM);
    printf(" %d - Consultar Caixa\n", CONSULTAR_CAIXA);
    printf(" %d - Sair\n", SAIR);
    aux_imprimir_separador_linha();
}

void aux_imprimir_separador_linha() {
	int indice = 0;
	int espaco_margem_esquerda_direita = 2;
	int tamanho_margem = COLUNAS - espaco_margem_esquerda_direita;
	
	printf(" ");
	for (indice = 0; indice < tamanho_margem; indice++) {
		printf("=");
	}
	printf("\n");
}

void aux_imprimir_itinerarios(Itinerario *itinerarios) {
    int index;
    for(index = 0; index < QTDE_ITINERARIOS; index++) {
    	
        printf(" %d - %s x %s\n", 
                (*itinerarios).codigo,
                (*itinerarios).origem, 
                (*itinerarios).destino);
        itinerarios++;
    }
	itinerarios -= QTDE_ITINERARIOS;
	//itinerarios = &itinerarios[0];
}

void aux_recuar_margem_esquerda() {
    int i;
    for (i = 0; i < RECUO_MARGEM_ESQUERDA; i++) {
        printf("%c", VAZIO);
	}
}

void aux_imprimir_opcao_invalida() {
    printf("\n\n Op%c%co inv%clida!", CEDILHA, ATIL, AAGUDO);
    fflush(stdin);
    getchar();
}

void aux_exibir_poltronas(Itinerario* itinerario) {
	//printf("1 - exibir_poltronas", itinerario);
    aux_imprimir_funcionalidade("Vender Passagem > Selecionar Poltrona");
    aux_desenhar_onibus(itinerario);
    aux_imprimir_separador_linha();
}

void aux_desenhar_poltrona(Bilhete *var_bilhete) {
   
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
    } else {
        /* Definimos as novas informações de cores do console */
        /* FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY	*/
        SetConsoleTextAttribute (h, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("%02d ", (*var_bilhete).numero_poltrona);
    }

    /* Restaurando as cores originais */
    SetConsoleTextAttribute (h, wOldColorAttrs);
}

Itinerario *aux_carregar_itinerarios() {
    
    Itinerario *vetor_itinerarios = (Itinerario*) malloc(QTDE_ITINERARIOS * sizeof(Itinerario));
    //pointer("1 - obter_itinerarios_disponiveis", vetor_itinerarios);
    aux_criar_itinerario(1, 100, "Campinas", "Santos", "26/10/2015", 55.33, vetor_itinerarios);
    
    vetor_itinerarios++;
    
    //pointer("2 - obter_itinerarios_disponiveis", vetor_itinerarios);
    aux_criar_itinerario(2, 200, "Campinas", "Cerquilho", "26/10/2015", 45.99, vetor_itinerarios);
    
    vetor_itinerarios--;
	
    return vetor_itinerarios;
}


int aux_confirmar_itinerario(Itinerario *itinerario) {
	
	//pointer("1 - confirmar_itinerario", itinerario);
	int confirmacao;
    
    do {
        confirmacao = OPCAO_INVALIDA;
    	//limpar_console();
		//separador_linha();
		aux_imprimir_funcionalidade("Vender Passagem > Confirmar Itinerario");
		//printf(" Dados do Itiner%crio\n\n", AAGUDO, (*itinerario).codigo);
		printf(" Origem:\t%s\n", (*itinerario).origem);
		printf(" Destino:\t%s\n", (*itinerario).destino);
		printf(" Data:\t\t%s\n", (*itinerario).data);	
		printf(" Valor:\t\t%.2f\n", (*itinerario).valor);
		aux_imprimir_separador_linha();

	    printf(" %d - Sim\n", SIM);
		printf(" %d - N%co\n", NAO, ATIL);
		aux_imprimir_separador_linha();
		printf(" Confirma? : ");

        scanf("%d", &confirmacao);
        
        if (confirmacao < SIM || confirmacao > NAO) {
            aux_imprimir_opcao_invalida();
        }
		
	} while (confirmacao < SIM || confirmacao > NAO);

	//getchar();	
	return confirmacao;
}

void aux_criar_itinerario(int codigo_itinerario,
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
    
	//pointer("1 - aux_criar_itinerario", novo_itinerario);
}

void aux_imprimir_titulo() {
    aux_limpar_janela_console();
    aux_imprimir_separador_linha();
	aux_recuar_margem_esquerda();
	printf(" ** Passagens  Express **\n");
    aux_imprimir_separador_linha();
}

void aux_imprimir_funcionalidade(char *funcionalidade) {
    aux_imprimir_titulo();
    printf(" %s\n", funcionalidade);
    aux_imprimir_separador_linha();
}

void aux_desenhar_onibus(Itinerario *itinerario) {
	//printf("1 - desenhar_onibus", itinerario);
	// disposição dos assentos seguindo o modelo de empresa Cometa
	aux_desenhar_lateral_direita_onibus();
    aux_desenhar_fileira_poltronas(itinerario, POLTRONA_03);
    aux_desenhar_fileira_poltronas(itinerario, POLTRONA_04);
    aux_desenhar_corredor();
	aux_desenhar_fileira_poltronas(itinerario, POLTRONA_02);
	aux_desenhar_fileira_poltronas(itinerario, POLTRONA_01);
	aux_desenhar_lateral_esquerda_onibus();
}

void aux_desenhar_lateral_direita_onibus() {
    aux_recuar_margem_esquerda();
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

void aux_desenhar_lateral_esquerda_onibus() {
    // lateral direita do ônibus - início
    aux_recuar_margem_esquerda();
    printf("%c", ARESTA_INFERIOR_ESQUERDA);
    int i;
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
}

void aux_desenhar_fileira_poltronas(Itinerario *itinerario, int primeira_poltrona) {
    //pointer("1 - desenhar_fileira_poltronas", itinerario);
	aux_recuar_margem_esquerda();
    printf("%c ", FRONTAL);

	int i;
	Bilhete *vetor_bilhetes = (*itinerario).bilhetes;
	Bilhete *bilhete;
	
    for (i = primeira_poltrona; i < QTDE_POLTRONAS; (i += SALTO_POLTRONA)) {
        bilhete = &vetor_bilhetes[i];
        aux_desenhar_poltrona(bilhete);
	}

    printf("%c\n", FRONTAL);
}

void aux_desenhar_corredor() {
	int i;

    aux_recuar_margem_esquerda();
    printf("%c%c", FRONTAL, CORREDOR);

    for (i = 3; i < QTDE_POLTRONAS; (i += SALTO_POLTRONA)) {
		printf("%c%c%c", CORREDOR, CORREDOR, CORREDOR);
	}

    printf("%c\n", FRONTAL);
}

int aux_verificar_poltronas_livres(Itinerario *itinerario) {
	int i;
	Bilhete *bilhetes = (*itinerario).bilhetes;
	int qtde_poltronas_livres = 0;
	
	for(i = 0; i < QTDE_POLTRONAS; i++) {
	    if (bilhetes[i].tipo == LIVRE) {
	        qtde_poltronas_livres++;
        }
    }
	
	return qtde_poltronas_livres;
}

int aux_escolher_tipo_bilhete(Itinerario *itinerario) {
	int tipo_bilhete;
    
    Bilhete *vetor_bilhetes = (*itinerario).bilhetes;
    do {
        
        aux_imprimir_funcionalidade("Vender Passagem > Selecionar Tipo Passagem");
        
    	tipo_bilhete = OPCAO_INVALIDA;

		printf(" %d - COMUM\n", COMUM);
    	printf(" %d - ESTUDANTE\n", ESTUDANTE);
    	printf(" %d - IDOSO\n", IDOSO);
    	printf(" %d - Cancelar\n", VOLTAR);
		aux_imprimir_separador_linha();
		printf("\n Tipo da Passagem: ");

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
        		aux_imprimir_opcao_invalida();
        		//aux_exibir_poltronas(itinerario);
		}
	} while (tipo_bilhete != SAIR && tipo_bilhete != COMUM && 
			tipo_bilhete != ESTUDANTE && tipo_bilhete != IDOSO);
    
    return tipo_bilhete;
}

void aux_finalizar_venda(Bilhete *bilhete) {
    
}

