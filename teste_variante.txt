/**
 * @file main.c
 * @brief Suíte de Testes de Diagnóstico e Invariantes para o sistema GameHouse.
 *
 * Este arquivo testa de forma exaustiva as operações e, mais importante,
 * as regras de negócio (invariantes) do sistema GameHouse. Ele tenta ativamente
 * criar estados inválidos para garantir que as pré-condições do sistema
 * estão protegendo sua integridade.
 *
 * Para compilar (Linux/macOS):
 * gcc -o teste_gamehouse main.c GameHouse.c -I.
 * Para compilar (Windows com MinGW):
 * gcc -o teste_gamehouse.exe main.c GameHouse.c -I.
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "GameHouse.h"

// =================================================================
// PROTÓTIPOS E IDs DE TESTE
// =================================================================

// Funções de ajuda
void check_test(const char *scenario, bool actual_ok, bool expected_ok);
void print_system_state();
void setup_initial_state();
void print_queue_for_device(GameHouse__DISPOSITIVOS dd); // Protótipo corrigido

// IDs globais para facilitar o acesso nos testes
const GameHouse__TIPOS_DISPOSITIVO PC = 1, CONSOLE = 2, VR = 3;
const GameHouse__DISPOSITIVOS PC_01 = 101, PC_02 = 102, CONSOLE_01 = 201;
const GameHouse__JOGOS JOGO_A_PC = 1001, JOGO_B_PC = 1002, JOGO_C_CONSOLE = 2001;
const GameHouse__CLIENTES CLIENTE_1 = 1, CLIENTE_2 = 2, CLIENTE_3 = 3;

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
    printf("\n--- INICIANDO SUÍTE DE TESTES DE DIAGNÓSTICO PARA GAMEHOUSE ---\n");
    bool ok;

    // =================================================================
    // INVARIANTE: UNICIDADE DE ENTIDADES
    // Regra: Não podem existir duas entidades (tipos, dispositivos, jogos, clientes) com o mesmo ID.
    // =================================================================
    printf("\n\n####################################################\n");
    printf("## Testando Invariante: UNICIDADE DE ENTIDADES    ##\n");
    printf("####################################################\n");

    setup_initial_state();
    printf("\nCenário: Sistema inicializado com entidades (PC:1, JOGO:1001, CLIENTE:1, etc).\n");

    printf("\nTentativa de violação: Adicionar entidades com IDs já existentes.\n");
    GameHouse__pre_add_tipo_dispositivo(PC, &ok);
    check_test("Não se pode adicionar tipo de dispositivo duplicado", ok, false);

    GameHouse__pre_add_dispositivo(PC_01, PC, &ok);
    check_test("Não se pode adicionar dispositivo duplicado", ok, false);

    GameHouse__pre_add_jogo(JOGO_A_PC, PC, &ok);
    check_test("Não se pode adicionar jogo duplicado", ok, false);

    GameHouse__pre_add_cliente(CLIENTE_1, &ok);
    check_test("Não se pode adicionar cliente duplicado", ok, false);
    printf("------------------------------------------------------------------------\n");


    // =================================================================
    // INVARIANTE: CONSISTÊNCIA DE STATUS (Cliente/Dispositivo <-> Sessão)
    // Regra: Um cliente/dispositivo só pode estar 'ativo'/'em_uso' se estiver em uma sessão.
    // =================================================================
    printf("\n\n#################################################################\n");
    printf("## Testando Invariante: CONSISTÊNCIA DE STATUS (ATIVO/EM_USO) ##\n");
    printf("#################################################################\n");

    setup_initial_state();
    printf("\nCenário: Cliente 1 entra na fila do PC_01.\n");
    GameHouse__entrar_fila_dispositivo(CLIENTE_1, PC_01, JOGO_A_PC);

    printf("\nAção: Iniciar a sessão. Status devem mudar para Ativo/Em Uso.\n");
    GameHouse__iniciar_sessao_fila(PC_01);
    print_system_state();

    printf("\nTentativa de violação: Cliente em sessão tenta entrar em outra fila (deve ser bloqueado).\n");
    GameHouse__pre_entrar_fila_dispositivo(CLIENTE_1, CONSOLE_01, JOGO_C_CONSOLE, &ok);
    check_test("Cliente em sessão não pode entrar em fila", ok, false);

    printf("\nAção: Encerrando a sessão. Status devem ser revertidos para Inativo/Disponível.\n");
    GameHouse__encerrar_sessao(CLIENTE_1);

    printf("\nVerificação: O estado do sistema após o ciclo completo da sessão.\n");
    print_system_state();
    printf("------------------------------------------------------------------------\n");


    // =================================================================
    // INVARIANTE: INTEGRIDADE REFERENCIAL (Não remover entidades em uso)
    // Regra: O sistema deve impedir a remoção de entidades que estão sendo referenciadas.
    // =================================================================
    printf("\n\n###########################################################\n");
    printf("## Testando Invariante: INTEGRIDADE REFERENCIAL      ##\n");
    printf("###########################################################\n");

    setup_initial_state();
    GameHouse__entrar_fila_dispositivo(CLIENTE_1, PC_01, JOGO_A_PC);
    GameHouse__iniciar_sessao_fila(PC_01);
    printf("\nCenário: Cliente 1 está em uma sessão no PC_01, jogando o Jogo A.\n");
    print_system_state();

    printf("\nTentativa de violação 1: Remover o dispositivo PC_01 que está em uso.\n");
    GameHouse__pre_remove_dispositivo(PC_01, &ok);
    check_test("Não se pode remover dispositivo em uso", ok, false);

    printf("\nTentativa de violação 2: Remover o Jogo A, que está sendo jogado.\n");
    GameHouse__pre_remover_jogo(JOGO_A_PC, &ok);
    check_test("Não se pode remover jogo em uso", ok, false);

    printf("\nTentativa de violação 3: Banir o Cliente 1, que está em uma sessão.\n");
    GameHouse__pre_banir_cliente(CLIENTE_1, &ok);
    check_test("Não se pode banir cliente em sessão", ok, false);

    printf("\nTentativa de violação 4: Remover o tipo PC, que está associado ao PC_01.\n");
    GameHouse__pre_remove_tipo_dispositivo(PC, &ok);
    check_test("Não se pode remover tipo em uso por dispositivo", ok, false);

    printf("\nAção: Encerrando a sessão para liberar as entidades.\n");
    GameHouse__encerrar_sessao(CLIENTE_1);

    printf("\nVerificação Pós-Sessão: Agora que a sessão acabou, as remoções devem ser permitidas.\n");
    GameHouse__pre_remover_jogo(JOGO_A_PC, &ok);
    check_test("Remoção de jogo (agora inativo) é permitida", ok, true);

    GameHouse__pre_remove_dispositivo(PC_01, &ok);
    check_test("Remoção de dispositivo (agora inativo) é permitida", ok, true);
    printf("------------------------------------------------------------------------\n");


    // =================================================================
    // INVARIANTE: LÓGICA DE DEPENDÊNCIAS (Tipos -> Jogos/Dispositivos)
    // Regra: Não se pode remover um Tipo se ele ainda é referenciado por um Jogo ou Dispositivo.
    // =================================================================
    printf("\n\n###########################################################\n");
    printf("## Testando Invariante: REMOÇÃO DE DEPENDÊNCIAS      ##\n");
    printf("###########################################################\n");
    setup_initial_state();
    printf("\nCenário: Tipo PC é usado pelo Dispositivo PC_01, PC_02 e pelos Jogos JOGO_A_PC, JOGO_B_PC.\n");

    printf("\nTentativa de violação 1: Remover o tipo PC (em uso por dispositivos e jogos).\n");
    GameHouse__pre_remove_tipo_dispositivo(PC, &ok);
    check_test("Não se pode remover tipo em uso por dispositivo/jogo", ok, false);

    printf("\nAção: Removendo os dispositivos PC_01 e PC_02.\n");
    GameHouse__remove_dispositivo(PC_01);
    GameHouse__remove_dispositivo(PC_02);

    printf("\nTentativa de violação 2: Remover o tipo PC (ainda em uso por jogos).\n");
    GameHouse__pre_remove_tipo_dispositivo(PC, &ok);
    check_test("Não se pode remover tipo em uso por jogo", ok, false);

    printf("\nAção: Removendo os jogos para liberar o tipo PC.\n");
    GameHouse__remove_jogo(JOGO_A_PC);
    // CORREÇÃO: Remover o JOGO_B_PC que também dependia do tipo PC.
    GameHouse__remove_jogo(JOGO_B_PC);
    printf("Jogos JOGO_A_PC e JOGO_B_PC removidos.\n");

    printf("\nVerificação: Agora que não há mais dependências, a remoção do tipo deve ser permitida.\n");
    GameHouse__pre_remove_tipo_dispositivo(PC, &ok);
    check_test("Remoção de tipo (agora livre) é permitida", ok, true);
    if(ok) GameHouse__remove_tipo_dispositivo(PC);
    print_system_state();
    printf("------------------------------------------------------------------------\n");


    printf("\n\n--- SUÍTE DE TESTES DE INVARIANTES FINALIZADA COM SUCESSO ---\n");
    return 0;
}


// =================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES AUXILIARES
// =================================================================

/**
 * @brief Prepara o sistema com um estado inicial conhecido para os testes.
 */
void setup_initial_state() {
    GameHouse__INITIALISATION();
    // Tipos
    GameHouse__add_tipo_dispositivo(PC);
    GameHouse__add_tipo_dispositivo(CONSOLE);
    // Jogos
    GameHouse__add_jogo(JOGO_A_PC, PC);
    GameHouse__add_jogo(JOGO_B_PC, PC);
    GameHouse__add_jogo(JOGO_C_CONSOLE, CONSOLE);
    // Dispositivos
    GameHouse__add_dispositivo(PC_01, PC);
    GameHouse__add_dispositivo(PC_02, PC);
    GameHouse__add_dispositivo(CONSOLE_01, CONSOLE);
    // Instalações
    GameHouse__instalar_jogo(PC_01, JOGO_A_PC);
    GameHouse__instalar_jogo(PC_01, JOGO_B_PC);
    GameHouse__instalar_jogo(PC_02, JOGO_A_PC);
    GameHouse__instalar_jogo(CONSOLE_01, JOGO_C_CONSOLE);
    // Clientes
    GameHouse__add_cliente(CLIENTE_1);
    GameHouse__add_cliente(CLIENTE_2);
    GameHouse__add_cliente(CLIENTE_3);
    // Créditos
    // CORREÇÃO AQUI: 'GameHouse' com 'H' maiúsculo.
    GameHouse__comprar_creditos(CLIENTE_1, 10);
    GameHouse__comprar_creditos(CLIENTE_2, 10);
}

/**
 * @brief Checa o resultado de um teste e imprime o resultado formatado.
 */
void check_test(const char *scenario, bool actual_ok, bool expected_ok) {
    printf("  -> TESTE: %-55s -> ", scenario);
    if (actual_ok == expected_ok) {
        // Cor verde para PASS
        printf("\x1b[32mPASS\x1b[0m (Esperado: %s, Obtido: %s)\n", 
               expected_ok ? "permitido" : "bloqueado", actual_ok ? "permitido" : "bloqueado");
    } else {
        // Cor vermelha para FAIL
        printf("\x1b[31mFAIL\x1b[0m (Esperado: %s, Obtido: %s)\n", 
               expected_ok ? "permitido" : "bloqueado", actual_ok ? "permitido" : "bloqueado");
    }
}

/**
 * @brief Imprime um resumo completo do estado atual do sistema.
 */
void print_system_state() {
    printf("   ...................[ ESTADO ATUAL DO SISTEMA ]...................\n");
    GameHouse__DISPOSITIVOS dd;
    GameHouse__TIPOS_DISPOSITIVO td;
    GameHouse__STATUS_DISPOSITIVO ss_d;
    GameHouse__CLIENTES cc;
    GameHouse__STATUS_CLIENTE ss_c;
    int32_t cr;
    bool more;

    printf("   | Dispositivos:\n");
    GameHouse__init_devices_info();
    GameHouse__has_more_devices_info(&more);
    if (!more) { printf("   |   (Nenhum)\n"); }
    while (more) {
        GameHouse__get_next_device_info(&dd, &td, &ss_d);
        printf("   |   - ID: %-5d | Tipo: %-5d | Status: %s\n", dd, td, get_status_dispositivo_str(ss_d));
        GameHouse__has_more_devices_info(&more);
    }

    printf("   | Clientes:\n");
    GameHouse__init_clientes_info();
    GameHouse__has_more_clientes_info(&more);
    if (!more) { printf("   |   (Nenhum)\n"); }
    while (more) {
        GameHouse__get_next_cliente_info(&cc, &ss_c, &cr);
        printf("   |   - ID: %-5d | Créditos: %-5d | Status: %s\n", cc, cr, get_status_cliente_str(ss_c));
        GameHouse__has_more_clientes_info(&more);
    }
    printf("   .................................................................\n");
}

/**
 * @brief Imprime a fila de um dispositivo específico.
 */
void print_queue_for_device(GameHouse__DISPOSITIVOS dd) {
    printf("   > Fila para o Dispositivo %d:\n", dd);
    int32_t pos;
    bool fila_tem_gente = false;

    // Itera por todos os clientes conhecidos para ver se estão nesta fila
    GameHouse__CLIENTES clientes_conhecidos[] = {CLIENTE_1, CLIENTE_2, CLIENTE_3};
    for (int i = 0; i < 3; i++) {
        GameHouse__posicao_fila(clientes_conhecidos[i], dd, &pos);
        if (pos > 0) {
            printf("     - Cliente %d está na posição %d\n", clientes_conhecidos[i], pos);
            fila_tem_gente = true;
        }
    }

    if (!fila_tem_gente) {
        printf("     (Fila vazia)\n");
    }
}