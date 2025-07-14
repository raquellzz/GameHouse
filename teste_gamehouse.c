#include <stdio.h>
#include <stdbool.h> // Usar stdbool para bool, true, false
#include <string.h>
#include "GameHouse.h"

// =================================================================
// PROTÓTIPOS E VARIÁVEIS GLOBAIS DE TESTE
// =================================================================

// Funções de ajuda para impressão do estado do sistema
void print_all_device_types();
void print_all_devices();
void print_all_games();
void print_installed_games(GameHouse__DISPOSITIVOS dd);
void print_all_clients();
void print_queue_for_device(GameHouse__DISPOSITIVOS dd);

// Nova função de teste mais detalhada
void check_test(const char *test_name, bool actual_ok, bool expected_ok);

// Arrays para guardar os IDs usados no teste, para funções de impressão mais inteligentes
GameHouse__CLIENTES todos_os_clientes[] = {1, 2, 3};
const int NUM_TOTAL_CLIENTES = 3;

// =================================================================
// FUNÇÕES DE MAPEAMENTO DE ENUM PARA STRING
// =================================================================

const char* get_status_cliente_str(GameHouse__STATUS_CLIENTE s) {
    switch (s) {
        case GameHouse__ativo: return "Ativo";
        case GameHouse__inativo: return "Inativo";
        case GameHouse__em_espera: return "Em Espera";
        default: return "Desconhecido";
    }
}

const char* get_status_dispositivo_str(GameHouse__STATUS_DISPOSITIVO s) {
    switch (s) {
        case GameHouse__disponivel: return "Disponível";
        case GameHouse__em_uso: return "Em Uso";
        case GameHouse__manutencao: return "Manutenção";
        default: return "Desconhecido";
    }
}

// =================================================================
// FUNÇÃO PRINCIPAL
// =================================================================

int main() {
    printf("--- INICIANDO SUÍTE DE TESTES DE DIAGNÓSTICO PARA GAMEHOUSE ---\n");
    GameHouse__INITIALISATION();

    // --- Definição dos IDs para os testes ---
    GameHouse__TIPOS_DISPOSITIVO PC = 1, CONSOLE = 2, VR = 3;
    GameHouse__DISPOSITIVOS PC_01 = 101, PC_02 = 102, CONSOLE_01 = 201;
    GameHouse__JOGOS JOGO_A_PC = 1001, JOGO_B_PC = 1002, JOGO_C_CONSOLE = 2001;
    GameHouse__CLIENTES CLIENTE_1 = 1, CLIENTE_2 = 2, CLIENTE_3 = 3;
    bool ok;
    int32_t pos;

    // =================================================================
    // 1. FASE DE CONFIGURAÇÃO (ADMIN)
    // =================================================================
    printf("\n\n#####################################################\n");
    printf("## 1. TESTES DE CONFIGURAÇÃO DO AMBIENTE (ADMIN) ##\n");
    printf("#####################################################\n");

    // --- Testes de Tipo de Dispositivo ---
    printf("\n--- Verificando adição e duplicação de Tipos de Dispositivo ---\n");
    GameHouse__pre_add_tipo_dispositivo(PC, &ok);
    check_test("Adicionar tipo PC (1)", ok, true);
    if(ok) GameHouse__add_tipo_dispositivo(PC);

    GameHouse__pre_add_tipo_dispositivo(CONSOLE, &ok);
    check_test("Adicionar tipo CONSOLE (2)", ok, true);
    if(ok) GameHouse__add_tipo_dispositivo(CONSOLE);

    printf("Tentando adicionar um tipo duplicado (deve falhar):\n");
    GameHouse__pre_add_tipo_dispositivo(PC, &ok);
    check_test("Tentar adicionar tipo PC (1) novamente", ok, false);
    print_all_device_types();

    // --- Testes de Jogos ---
    printf("\n--- Verificando adição de Jogos e compatibilidade de tipos ---\n");
    GameHouse__pre_add_jogo(JOGO_A_PC, PC, &ok);
    check_test("Adicionar Jogo A para tipo PC", ok, true);
    if(ok) GameHouse__add_jogo(JOGO_A_PC, PC);

    GameHouse__pre_add_jogo(JOGO_B_PC, PC, &ok);
    check_test("Adicionar Jogo B para tipo PC", ok, true);
    if(ok) GameHouse__add_jogo(JOGO_B_PC, PC);

    GameHouse__pre_add_jogo(JOGO_C_CONSOLE, CONSOLE, &ok);
    check_test("Adicionar Jogo C para tipo Console", ok, true);
    if(ok) GameHouse__add_jogo(JOGO_C_CONSOLE, CONSOLE);

    printf("Tentando adicionar um jogo para um tipo que não existe (deve falhar):\n");
    GameHouse__pre_add_jogo(9999, VR, &ok);
    check_test("Tentar adicionar jogo para tipo VR (inexistente)", ok, false);
    print_all_games();

    // --- Testes de Dispositivos ---
    printf("\n--- Verificando adição e duplicação de Dispositivos ---\n");
    GameHouse__pre_add_dispositivo(PC_01, PC, &ok);
    check_test("Adicionar dispositivo PC_01", ok, true);
    if(ok) GameHouse__add_dispositivo(PC_01, PC);

    GameHouse__pre_add_dispositivo(PC_02, PC, &ok);
    check_test("Adicionar dispositivo PC_02", ok, true);
    if(ok) GameHouse__add_dispositivo(PC_02, PC);

    GameHouse__pre_add_dispositivo(CONSOLE_01, CONSOLE, &ok);
    check_test("Adicionar dispositivo CONSOLE_01", ok, true);
    if(ok) GameHouse__add_dispositivo(CONSOLE_01, CONSOLE);

    printf("Tentando adicionar um dispositivo duplicado (deve falhar):\n");
    GameHouse__pre_add_dispositivo(PC_01, PC, &ok);
    check_test("Tentar adicionar PC_01 novamente", ok, false);
    print_all_devices();

    // --- Testes de Instalação de Jogos ---
    printf("\n--- Verificando instalação de jogos e compatibilidade ---\n");
    GameHouse__pre_instalar_jogo(PC_01, JOGO_A_PC, &ok);
    check_test("Instalar Jogo A (PC) no PC_01", ok, true);
    if(ok) GameHouse__instalar_jogo(PC_01, JOGO_A_PC);

    GameHouse__pre_instalar_jogo(PC_01, JOGO_B_PC, &ok);
    check_test("Instalar Jogo B (PC) no PC_01", ok, true);
    if(ok) GameHouse__instalar_jogo(PC_01, JOGO_B_PC);

    GameHouse__pre_instalar_jogo(PC_02, JOGO_A_PC, &ok);
    check_test("Instalar Jogo A (PC) no PC_02", ok, true);
    if(ok) GameHouse__instalar_jogo(PC_02, JOGO_A_PC);

    GameHouse__pre_instalar_jogo(CONSOLE_01, JOGO_C_CONSOLE, &ok);
    check_test("Instalar Jogo C (Console) no CONSOLE_01", ok, true);
    if(ok) GameHouse__instalar_jogo(CONSOLE_01, JOGO_C_CONSOLE);

    printf("Tentando instalar um jogo em um dispositivo de tipo incompatível (deve falhar):\n");
    GameHouse__pre_instalar_jogo(PC_01, JOGO_C_CONSOLE, &ok);
    check_test("Tentar instalar Jogo C (Console) no PC_01", ok, false);

    printf("Estado atual das instalações:\n");
    print_installed_games(PC_01);
    print_installed_games(PC_02);
    print_installed_games(CONSOLE_01);

    // =================================================================
    // 2. FASE DE GERENCIAMENTO DE CLIENTES
    // =================================================================
    printf("\n\n###################################################\n");
    printf("## 2. TESTES DE GERENCIAMENTO DE CLIENTES      ##\n");
    printf("###################################################\n");

    GameHouse__pre_add_cliente(CLIENTE_1, &ok);
    check_test("Adicionar Cliente 1", ok, true);
    if(ok) GameHouse__add_cliente(CLIENTE_1);

    GameHouse__pre_add_cliente(CLIENTE_2, &ok);
    check_test("Adicionar Cliente 2", ok, true);
    if(ok) GameHouse__add_cliente(CLIENTE_2);

    GameHouse__pre_add_cliente(CLIENTE_3, &ok);
    check_test("Adicionar Cliente 3", ok, true);
    if(ok) GameHouse__add_cliente(CLIENTE_3);

    printf("\n--- Comprando Créditos ---\n");
    GameHouse__pre_comprar_creditos(CLIENTE_1, 5, &ok);
    check_test("Cliente 1 compra 5 créditos", ok, true);
    if(ok) GameHouse__comprar_creditos(CLIENTE_1, 5);

    GameHouse__pre_comprar_creditos(CLIENTE_2, 2, &ok);
    check_test("Cliente 2 compra 2 créditos", ok, true);
    if(ok) GameHouse__comprar_creditos(CLIENTE_2, 2);

    printf("Estado inicial dos clientes:\n");
    print_all_clients();

    // =================================================================
    // 3. FASE OPERACIONAL (FILAS E SESSÕES)
    // =================================================================
    printf("\n\n#################################################\n");
    printf("## 3. TESTES OPERACIONAIS (FILAS E SESSÕES) ##\n");
    printf("#################################################\n");

    printf("\n--- Clientes tentam entrar na fila ---\n");
    printf("Cliente 1 (com créditos) tenta entrar em fila para jogo válido:\n");
    GameHouse__pre_entrar_fila_dispositivo(CLIENTE_1, PC_01, JOGO_A_PC, &ok);
    check_test("Cliente 1 entra na fila do PC_01 para Jogo A", ok, true);
    if(ok) GameHouse__entrar_fila_dispositivo(CLIENTE_1, PC_01, JOGO_A_PC);

    printf("Cliente 2 (com créditos) entra na mesma fila:\n");
    GameHouse__pre_entrar_fila_dispositivo(CLIENTE_2, PC_01, JOGO_B_PC, &ok);
    check_test("Cliente 2 entra na fila do PC_01 para Jogo B", ok, true);
    if(ok) GameHouse__entrar_fila_dispositivo(CLIENTE_2, PC_01, JOGO_B_PC);

    printf("Cliente 3 (sem créditos) tenta entrar (deve falhar):\n");
    GameHouse__pre_entrar_fila_dispositivo(CLIENTE_3, PC_01, JOGO_A_PC, &ok);
    check_test("Cliente 3 (sem créditos) tenta entrar na fila", ok, false);

    printf("Cliente 1 tenta entrar em outra fila ao mesmo tempo (deve falhar):\n");
    GameHouse__pre_entrar_fila_dispositivo(CLIENTE_1, CONSOLE_01, JOGO_C_CONSOLE, &ok);
    check_test("Cliente 1 tenta entrar na fila do CONSOLE_01", ok, false);

    printf("Estado atual das filas e clientes:\n");
    print_all_clients();
    print_queue_for_device(PC_01);
    GameHouse__posicao_fila(CLIENTE_2, PC_01, &pos);
    printf("Verificação explícita: Posição do Cliente 2 na fila do PC_01 é %d (esperado: 2)\n", pos);


    printf("\n--- Iniciando a primeira sessão a partir da fila ---\n");
    GameHouse__pre_iniciar_sessao(PC_01, &ok);
    check_test("Pré-condição para iniciar sessão no PC_01", ok, true);
    if(ok) GameHouse__iniciar_sessao_fila(PC_01);

    printf("Estado após Cliente 1 (primeiro da fila) iniciar a sessão:\n");
    print_all_clients();
    print_all_devices();
    print_queue_for_device(PC_01);
    GameHouse__posicao_fila(CLIENTE_2, PC_01, &pos);
    printf("Verificação explícita: Posição do Cliente 2 agora é %d (esperado: 1)\n", pos);

    printf("\n--- Cliente 2 (em espera) decide sair da fila ---\n");
    GameHouse__pre_sair_fila_dispositivo(CLIENTE_2, PC_01, &ok);
    check_test("Pré-condição para Cliente 2 sair da fila", ok, true);
    if(ok) GameHouse__sair_fila_dispositivo(CLIENTE_2, PC_01);

    printf("Estado após Cliente 2 sair da fila:\n");
    print_all_clients();
    print_queue_for_device(PC_01);

    printf("\n--- Encerrando a sessão do Cliente 1 ---\n");
    GameHouse__pre_encerrar_sessao(CLIENTE_1, &ok);
    check_test("Pré-condição para encerrar sessão do Cliente 1", ok, true);
    if(ok) GameHouse__encerrar_sessao(CLIENTE_1);

    printf("Estado após Cliente 1 encerrar sua sessão:\n");
    print_all_clients();
    print_all_devices();

    // =================================================================
    // 4. FASE DE DESMONTE (REMOÇÕES)
    // =================================================================
    printf("\n\n#############################################\n");
    printf("## 4. TESTES DE DESMONTE DO AMBIENTE     ##\n");
    printf("#############################################\n");

    printf("\n--- Testando Banimento de Cliente ---\n");
    GameHouse__pre_banir_cliente(CLIENTE_1, &ok);
    check_test("Banir Cliente 1 (inativo)", ok, true);
    if(ok) GameHouse__banir_cliente(CLIENTE_1);

    printf("Cliente 2 entra na fila para testar proteção contra banimento (deve falhar):\n");
    GameHouse__pre_entrar_fila_dispositivo(CLIENTE_2, CONSOLE_01, JOGO_C_CONSOLE, &ok);
    if(ok) GameHouse__entrar_fila_dispositivo(CLIENTE_2, CONSOLE_01, JOGO_C_CONSOLE);
    GameHouse__pre_banir_cliente(CLIENTE_2, &ok);
    check_test("Tentar banir Cliente 2 (em espera)", ok, false);

    printf("Cliente 2 sai da fila e é banido (deve funcionar):\n");
    GameHouse__sair_fila_dispositivo(CLIENTE_2, CONSOLE_01);
    GameHouse__pre_banir_cliente(CLIENTE_2, &ok);
    check_test("Banir Cliente 2 (agora inativo)", ok, true);
    if(ok) GameHouse__banir_cliente(CLIENTE_2);

    printf("Estado final dos clientes:\n");
    print_all_clients();

    printf("\n--- Testando Remoção de Dispositivos e Tipos (Ordem de Dependência) ---\n");
    GameHouse__pre_remove_dispositivo(PC_01, &ok);
    check_test("Remover dispositivo PC_01", ok, true);
    if(ok) GameHouse__remove_dispositivo(PC_01);

    printf("Estado após remover PC_01:\n");
    print_all_devices();
    print_installed_games(PC_01);

    printf("Tentando remover o tipo PC enquanto PC_02 ainda existe (deve falhar):\n");
    GameHouse__pre_remove_tipo_dispositivo(PC, &ok);
    check_test("Tentar remover tipo PC (em uso por PC_02)", ok, false);

    printf("Removendo o último dispositivo do tipo PC:\n");
    GameHouse__pre_remove_dispositivo(PC_02, &ok);
    check_test("Remover dispositivo PC_02", ok, true);
    if(ok) GameHouse__remove_dispositivo(PC_02);

    printf("Tentando remover o tipo PC enquanto jogos desse tipo ainda existem (deve falhar):\n");
    GameHouse__pre_remove_tipo_dispositivo(PC, &ok);
    check_test("Tentar remover tipo PC (em uso por Jogo A/B)", ok, false);

    printf("Removendo os jogos para liberar o tipo PC:\n");
    GameHouse__pre_remover_jogo(JOGO_A_PC, &ok);
    check_test("Remover Jogo A (PC)", ok, true);
    if(ok) GameHouse__remove_jogo(JOGO_A_PC);

    GameHouse__pre_remover_jogo(JOGO_B_PC, &ok);
    check_test("Remover Jogo B (PC)", ok, true);
    if(ok) GameHouse__remove_jogo(JOGO_B_PC);

    printf("Agora que não há dispositivos nem jogos do tipo PC, a remoção deve funcionar:\n");
    GameHouse__pre_remove_tipo_dispositivo(PC, &ok);
    check_test("Remover tipo PC (agora livre)", ok, true);
    if(ok) GameHouse__remove_tipo_dispositivo(PC);

    printf("Estado final dos tipos de dispositivo:\n");
    print_all_device_types();
    printf("Estado final dos jogos:\n");
    print_all_games();

    printf("\n\n--- SUÍTE DE TESTES FINALIZADA ---\n");
    return 0;
}

// =================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES AUXILIARES
// =================================================================

void check_test(const char *test_name, bool actual_ok, bool expected_ok) {
    printf("TESTE: %-55s -> ", test_name);
    if (actual_ok == expected_ok) {
        printf("\x1b[32mPASS\x1b[0m (Esperado: %s, Obtido: %s)\n", 
               expected_ok ? "true" : "false", actual_ok ? "true" : "false");
    } else {
        printf("\x1b[31mFAIL\x1b[0m (Esperado: %s, Obtido: %s)\n", 
               expected_ok ? "true" : "false", actual_ok ? "true" : "false");
    }
}

void print_all_device_types() {
    GameHouse__TIPOS_DISPOSITIVO td;
    bool more;
    printf(" > Tipos de Dispositivo Atuais: [ ");
    GameHouse__init_tipos_disponiveis();
    GameHouse__has_more_tipos_disponiveis(&more);
    while (more) {
        GameHouse__get_next_tipo_dispositivo(&td);
        printf("%d ", td);
        GameHouse__has_more_tipos_disponiveis(&more);
    }
    printf("]\n");
}

void print_all_devices() {
    GameHouse__DISPOSITIVOS dd;
    GameHouse__TIPOS_DISPOSITIVO td;
    GameHouse__STATUS_DISPOSITIVO ss;
    bool more;
    printf(" > Lista de Dispositivos Registrados:\n");
    GameHouse__init_devices_info();
    GameHouse__has_more_devices_info(&more);
    if (!more) {
        printf("   (Nenhum dispositivo cadastrado)\n");
        return;
    }
    while (more) {
        GameHouse__get_next_device_info(&dd, &td, &ss);
        printf("   - ID: %-5d | Tipo: %-5d | Status: %s\n", dd, td, get_status_dispositivo_str(ss));
        GameHouse__has_more_devices_info(&more);
    }
}

void print_all_games() {
    // Como não há uma função para listar TODOS os jogos, vamos iterar por tipos conhecidos.
    GameHouse__TIPOS_DISPOSITIVO tipos[] = {1, 2, 3}; // PC, CONSOLE, VR
    printf(" > Lista de Jogos Registrados (por tipo):\n");
    for (int i = 0; i < 3; i++) {
        GameHouse__JOGOS jj;
        bool more;
        printf("   - Tipo %d: [ ", tipos[i]);
        GameHouse__init_jogos_por_tipo();
        GameHouse__has_more_jogos_por_tipo(tipos[i], &more);
        while(more) {
            GameHouse__get_next_jogo_por_tipo(tipos[i], &jj);
            printf("%d ", jj);
            GameHouse__has_more_jogos_por_tipo(tipos[i], &more);
        }
        printf("]\n");
    }
}

void print_installed_games(GameHouse__DISPOSITIVOS dd) {
    GameHouse__JOGOS jj;
    bool more;
    printf("   > Jogos instalados no Dispositivo %d: [ ", dd);
    GameHouse__init_jogos_dispositivo();
    GameHouse__has_more_jogos_dispositivo(dd, &more);
    if (!more) {
        printf("(Nenhum jogo instalado) ");
    }
    while (more) {
        GameHouse__get_next_jogo_dispositivo(dd, &jj);
        printf("%d ", jj);
        GameHouse__has_more_jogos_dispositivo(dd, &more);
    }
    printf("]\n");
}

void print_all_clients() {
    GameHouse__CLIENTES cc;
    GameHouse__STATUS_CLIENTE ss;
    int32_t cr;
    bool more;
    printf(" > Lista de Clientes Registrados:\n");
    GameHouse__init_clientes_info();
    GameHouse__has_more_clientes_info(&more);
    if (!more) {
        printf("   (Nenhum cliente cadastrado)\n");
        return;
    }
    while (more) {
        GameHouse__get_next_cliente_info(&cc, &ss, &cr);
        printf("   - ID: %-5d | Créditos: %-5d | Status: %s\n", cc, cr, get_status_cliente_str(ss));
        GameHouse__has_more_clientes_info(&more);
    }
}

void print_queue_for_device(GameHouse__DISPOSITIVOS dd) {
    printf(" > Fila para o Dispositivo %d:\n", dd);
    int32_t pos;
    bool fila_tem_gente = false;

    // Itera por todos os clientes conhecidos para ver se estão nesta fila
    for (int i = 0; i < NUM_TOTAL_CLIENTES; i++) {
        GameHouse__CLIENTES cliente_atual = todos_os_clientes[i];
        GameHouse__posicao_fila(cliente_atual, dd, &pos);
        if (pos > 0) {
            printf("   - Cliente %d está na posição %d\n", cliente_atual, pos);
            fila_tem_gente = true;
        }
    }

    if (!fila_tem_gente) {
        printf("   (Fila vazia)\n");
    }
}