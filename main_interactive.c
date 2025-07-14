/**
 * @file main_interactive.c
 * @brief Shell Interativo para testar o sistema GameHouse dinamicamente.
 *
 * Este programa permite ao usuário interagir com a implementação do GameHouse
 * por meio de comandos de texto no terminal.
 *
 * Comandos disponíveis:
 * - add_tipo <id>
 * - add_disp <id> <tipo_id>
 * - add_jogo <id> <tipo_id>
 * - add_cliente <id>
 * - creditos <cliente_id> <qtd>
 * - instalar <disp_id> <jogo_id>
 * - fila_entrar <cliente_id> <disp_id> <jogo_id>
 * - sessao_iniciar <disp_id>
 * - sessao_encerrar <cliente_id>
 * - estado            (mostra o estado atual do sistema)
 * - help              (mostra esta ajuda)
 * - sair              (encerra o programa)
 *
 * Para compilar:
 * gcc -o gamehouse_cli main_interactive.c GameHouse.c -I.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Para atoi (converter string para inteiro)
#include "GameHouse.h"
#include <limits.h>
// Protótipos das funções auxiliares
void print_help();
void display_enabled_operations();
void print_system_state_interactive();

int main() {
    char input_buffer[256];
    char* command;
    char* arg1, *arg2, *arg3;
    bool ok;

    // Inicializa o sistema uma vez no início
    GameHouse__INITIALISATION();
    printf("Bem-vindo ao Shell Interativo do GameHouse!\n");
    printf("Digite 'help' para ver a lista de comandos.\n");

    while (1) {
        printf("\nGameHouse > ");
        // Lê uma linha inteira do usuário de forma segura
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break; // Sai em caso de erro ou fim de arquivo (Ctrl+D)
        }
        
        // Remove a quebra de linha do final do buffer, se houver
        input_buffer[strcspn(input_buffer, "\n")] = 0;

        // Pega o primeiro token (o comando)
        command = strtok(input_buffer, " ");
        if (command == NULL) {
            continue; // Se o usuário só apertou Enter, continua o loop
        }

        // --- BLOCO DE COMANDOS ---
        if (strcmp(command, "sair") == 0) {
            printf("Encerrando...\n");
            break;
        } 
        else if (strcmp(command, "help") == 0) {
            print_help();
        }
        else if (strcmp(command, "estado") == 0) {
            print_system_state_interactive();
        }
        else if (strcmp(command, "comandos_validos") == 0) {
            display_enabled_operations();
        }
        // --- Operações de Adição ---
        else if (strcmp(command, "add_cliente") == 0) {
            arg1 = strtok(NULL, " ");
            if (arg1 == NULL) {
                printf("ERRO: Uso: add_cliente <id_cliente>\n");
            } else {
                GameHouse__CLIENTES id = atoi(arg1);
                GameHouse__pre_add_cliente(id, &ok);
                if(ok) {
                    GameHouse__add_cliente(id);
                    printf("SUCESSO: Cliente %d adicionado.\n", id);
                } else {
                    printf("FALHA: Pré-condição não satisfeita (cliente já existe ou limite atingido).\n");
                }
            }
        }
        else if (strcmp(command, "add_tipo") == 0) {
            arg1 = strtok(NULL, " ");
             if (arg1 == NULL) {
                printf("ERRO: Uso: add_tipo <id_tipo>\n");
            } else {
                GameHouse__TIPOS_DISPOSITIVO id = atoi(arg1);
                GameHouse__pre_add_tipo_dispositivo(id, &ok);
                 if(ok) {
                    GameHouse__add_tipo_dispositivo(id);
                    printf("SUCESSO: Tipo de dispositivo %d adicionado.\n", id);
                } else {
                    printf("FALHA: Pré-condição não satisfeita (tipo já existe ou limite atingido).\n");
                }
            }
        }
        else if (strcmp(command, "add_disp") == 0) {
            arg1 = strtok(NULL, " ");
            arg2 = strtok(NULL, " ");
             if (arg1 == NULL || arg2 == NULL) {
                printf("ERRO: Uso: add_disp <id> <tipo_id>\n");
            } else {
                GameHouse__DISPOSITIVOS id = atoi(arg1);
                GameHouse__TIPOS_DISPOSITIVO td = atoi(arg2);
                GameHouse__pre_add_dispositivo(id, td, &ok);
                 if(ok) {
                    GameHouse__add_dispositivo(id, td);
                    printf("SUCESSO: Dispositivo %d adicionado.\n", id);
                } else {
                    printf("FALHA: Pré-condição não satisfeita (Dispositivo já existe ou limite atingido).\n");
                }
            }
        }
        else if (strcmp(command, "creditos") == 0) {
            arg1 = strtok(NULL, " ");
            arg2 = strtok(NULL, " ");
             if (arg1 == NULL || arg2 == NULL) {
                printf("ERRO: Uso: creditos <id_cliente> <quantidade>\n");
            } else {
                GameHouse__CLIENTES id = atoi(arg1);
                int qtd = atoi(arg2);
                // A pré-condição real é mais complexa, mas aqui simplificamos
                GameHouse__comprar_creditos(id, qtd);
                printf("SUCESSO: %d créditos adicionados ao cliente %d.\n", qtd, id);
            }
        }
        // --- Operações de Fluxo ---
        else if (strcmp(command, "fila_entrar") == 0) {
            arg1 = strtok(NULL, " "); // cliente
            arg2 = strtok(NULL, " "); // dispositivo
            arg3 = strtok(NULL, " "); // jogo
             if (arg1 == NULL || arg2 == NULL || arg3 == NULL) {
                printf("ERRO: Uso: fila_entrar <id_cliente> <id_disp> <id_jogo>\n");
            } else {
                GameHouse__CLIENTES c_id = atoi(arg1);
                GameHouse__DISPOSITIVOS d_id = atoi(arg2);
                GameHouse__JOGOS j_id = atoi(arg3);
                // Aqui você chamaria a pré-condição real
                GameHouse__entrar_fila_dispositivo(c_id, d_id, j_id);
                printf("SUCESSO: Cliente %d entrou na fila para o dispositivo %d.\n", c_id, d_id);
            }
        }
        else if (strcmp(command, "sessao_iniciar") == 0) {
             arg1 = strtok(NULL, " ");
             if (arg1 == NULL) {
                printf("ERRO: Uso: sessao_iniciar <id_disp>\n");
            } else {
                GameHouse__DISPOSITIVOS d_id = atoi(arg1);
                GameHouse__iniciar_sessao_fila(d_id);
                printf("SUCESSO: Sessão iniciada no dispositivo %d.\n", d_id);
            }
        }
        else if (strcmp(command, "sessao_encerrar") == 0) {
             arg1 = strtok(NULL, " ");
             if (arg1 == NULL) {
                printf("ERRO: Uso: sessao_encerrar <id_cliente>\n");
            } else {
                GameHouse__CLIENTES c_id = atoi(arg1);
                GameHouse__encerrar_sessao(c_id);
                printf("SUCESSO: Sessão do cliente %d encerrada.\n", c_id);
            }
        }
        else {
            printf("ERRO: Comando '%s' desconhecido. Digite 'help' para ajuda.\n", command);
        }
    }

    return 0;
}

// Implementação das funções auxiliares
void print_help() {
    printf("\n--- Ajuda: Lista Completa de Comandos ---\n");
    printf("  add_tipo <id>                   - Adiciona um novo tipo de dispositivo.\n");
    printf("  add_disp <id> <tipo_id>         - Adiciona um novo dispositivo de um tipo existente.\n");
    printf("  add_jogo <id> <tipo_id>         - Adiciona um novo jogo para um tipo de dispositivo.\n");
    printf("  add_cliente <id>                - Adiciona um novo cliente ao sistema.\n");
    printf("  creditos <cliente_id> <qtd>     - Adiciona créditos a um cliente.\n");
    printf("  instalar <disp_id> <jogo_id>    - Instala um jogo em um dispositivo compatível.\n");
    printf("  fila_entrar <c_id> <d_id> <j_id> - Coloca um cliente na fila de um dispositivo.\n");
    printf("  sessao_iniciar <disp_id>        - Inicia a sessão para o primeiro cliente na fila.\n");
    printf("  sessao_encerrar <cliente_id>    - Encerra a sessão de um cliente.\n");
    
    printf("\n-------- Comandos de Utilitário ---------\n");
    printf("  comandos_validos                - Mostra os comandos válidos no estado atual.\n");
    printf("  estado                          - Exibe o estado atual de clientes e dispositivos.\n");
    printf("  help                            - Mostra esta mensagem de ajuda.\n");
    printf("  sair                            - Encerra o programa.\n");
    printf("-------------------------------------------\n");
}

void display_enabled_operations() {
    bool ok;
    printf("\n--- Comandos Válidos no Estado Atual ---\n");

    // Verifica se é possível adicionar novas entidades
    GameHouse__pre_add_tipo_dispositivo(0, &ok);
    if (ok) printf("  > add_tipo <id>\n");

    GameHouse__pre_add_cliente(0, &ok);
    if (ok) printf("  > add_cliente <id>\n");
    
    // Verifica operações em entidades existentes.
    // Para um exemplo mais completo, seria necessário iterar sobre clientes, dispositivos, etc.
    // e chamar a pré-condição para cada um.
    // Exemplo simplificado:
    printf("  > creditos <cliente_id> <qtd> (verificar pré-condição manualmente)\n");


    // Exemplo de verificação em entidade existente:
    // Tenta encontrar um cliente que possa encerrar a sessão.
    // NOTA: Requer que a função de iterador de clientes esteja implementada em GameHouse.c
    GameHouse__CLIENTES cc;
    GameHouse__STATUS_CLIENTE sc;
    int32_t cr;
    bool sessao_pode_encerrar = false;
    GameHouse__init_clientes_info();
    GameHouse__has_more_clientes_info(&ok);
    while(ok) {
        GameHouse__get_next_cliente_info(&cc, &sc, &cr);
        bool pre_ok;
        GameHouse__pre_encerrar_sessao(cc, &pre_ok);
        if(pre_ok) {
            printf("  > sessao_encerrar %d\n", cc);
            sessao_pode_encerrar = true;
        }
        GameHouse__has_more_clientes_info(&ok);
    }
    if(!sessao_pode_encerrar) {
        printf("  (Nenhuma sessão pode ser encerrada no momento)\n");
    }


    printf("--------------------------------------\n");
}

// Versão da função de impressão de estado para o modo interativo
void print_system_state_interactive() {
    GameHouse__DISPOSITIVOS dd;
    GameHouse__TIPOS_DISPOSITIVO td;
    GameHouse__STATUS_DISPOSITIVO ss_d;
    GameHouse__CLIENTES cc;
    GameHouse__STATUS_CLIENTE ss_c;
    int32_t cr;
    bool more;

    printf("\n--- ESTADO ATUAL DO SISTEMA ---\n");
    printf("Dispositivos:\n");
    GameHouse__init_devices_info();
    GameHouse__has_more_devices_info(&more);
    if (!more) { printf("  (Nenhum dispositivo cadastrado)\n"); }
    while (more) {
        GameHouse__get_next_device_info(&dd, &td, &ss_d);
        // A função get_status_dispositivo_str precisa ser definida ou adaptada
        printf("  - ID: %-5d | Tipo: %-5d | Status: %d\n", dd, td, ss_d);
        GameHouse__has_more_devices_info(&more);
    }

    printf("\nClientes:\n");
    GameHouse__init_clientes_info();
    GameHouse__has_more_clientes_info(&more);
    if (!more) { printf("  (Nenhum cliente cadastrado)\n"); }
    while (more) {
        GameHouse__get_next_cliente_info(&cc, &ss_c, &cr);
        // A função get_status_cliente_str precisa ser definida ou adaptada
        printf("  - ID: %-5d | Créditos: %-5d | Status: %d\n", cc, cr, ss_c);
        GameHouse__has_more_clientes_info(&more);
    }
    printf("--------------------------------\n");
}