// Arquivo: teste_gamehouse.c
#include <stdio.h>
#include <assert.h> // Essencial para verificar condições. O programa para se uma asserção falhar.
#include <stdbool.h>

// Inclua o header da sua máquina e do contexto
#include "GameHouse.h"
#include "GameHouse_ctx.h" 

/* --- Protótipos das Funções de Teste --- */
void cenario_cliente_entra_na_fila_com_sucesso();
void cenario_iniciar_e_encerrar_sessao();

/* --- Função Principal que executa todos os testes --- */
int main() {
    printf("🚀 Iniciando suíte de testes para a GameHouse...\n\n");
    
    // Chame aqui todas as suas funções de cenário de teste
    cenario_cliente_entra_na_fila_com_sucesso();
    cenario_iniciar_e_encerrar_sessao();

    printf("🎉 Suíte de testes concluída com sucesso!\n");
    return 0;
}

/* --- Cenário de Teste 1: Fluxo Básico de Cliente e Fila --- */
void cenario_cliente_entra_na_fila_com_sucesso() {
    printf("Executando Cenário 1: Cliente entra na fila com sucesso...\n");

    // 1. Arrange (Preparar)
    // --------------------------------------------------------------------
    GameHouse__INITIALISATION();
    printf("  [PREPARAR] Máquina inicializada.\n");

    GameHouse__CLIENTES cliente1_id = 10;
    GameHouse__DISPOSITIVOS pc1_id = 1;
    GameHouse__TIPOS_DISPOSITIVO tipo_pc_id = 5;
    GameHouse__JOGOS jogo1_id = 100;
    bool pre_condicao_ok;

    GameHouse__add_tipo_dispositivo(tipo_pc_id);
    printf("  [PREPARAR] Tipo de dispositivo 'PC' (ID %d) adicionado.\n", tipo_pc_id);

    GameHouse__add_dispositivo(pc1_id, tipo_pc_id);
    printf("  [PREPARAR] Dispositivo 'PC_01' (ID %d) adicionado.\n", pc1_id);

    // [AJUSTE] Adicionando o jogo e instalando no dispositivo.
    GameHouse__add_jogo(jogo1_id, tipo_pc_id);
    printf("  [PREPARAR] Jogo (ID %d) adicionado.\n", jogo1_id);
    GameHouse__instalar_jogo(pc1_id, jogo1_id);
    printf("  [PREPARAR] Jogo (ID %d) instalado no Dispositivo (ID %d).\n", jogo1_id, pc1_id);

    GameHouse__pre_add_cliente(cliente1_id, &pre_condicao_ok);
    assert(pre_condicao_ok == true);
    GameHouse__add_cliente(cliente1_id);
    printf("  [PREPARAR] Cliente (ID %d) adicionado.\n", cliente1_id);

    // [AJUSTE] Corrigido o valor dos créditos para um valor válido (3).
    GameHouse__pre_comprar_creditos(cliente1_id, 3, &pre_condicao_ok);
    assert(pre_condicao_ok == true);
    GameHouse__comprar_creditos(cliente1_id, 3);
    printf("  [PREPARAR] 3 créditos adicionados ao cliente %d.\n", cliente1_id);

    // 2. Act (Agir)
    // --------------------------------------------------------------------
    printf("  [AGIR] Cliente %d tentando entrar na fila para o dispositivo %d...\n", cliente1_id, pc1_id);
    GameHouse__pre_entrar_fila_dispositivo(cliente1_id, pc1_id, jogo1_id, &pre_condicao_ok);
    assert(pre_condicao_ok == true);
    GameHouse__entrar_fila_dispositivo(cliente1_id, pc1_id, jogo1_id);

    // 3. Assert (Verificar)
    // --------------------------------------------------------------------
    int32_t posicao_na_fila;
    GameHouse__posicao_fila(cliente1_id, pc1_id, &posicao_na_fila);
    assert(posicao_na_fila == 1);
    printf("  [VERIFICAR] ✅ SUCESSO! Cliente %d está na posição 1 da fila.\n", cliente1_id);
    
    GameHouse__pre_entrar_fila_dispositivo(cliente1_id, pc1_id, jogo1_id, &pre_condicao_ok);
    assert(pre_condicao_ok == false);
    printf("  [VERIFICAR] ✅ SUCESSO! Pré-condição para entrar na fila novamente é falsa, como esperado.\n\n");
}


/* --- Cenário de Teste 2: Inicia e Encerra uma Sessão --- */
void cenario_iniciar_e_encerrar_sessao() {
    printf("Executando Cenário 2: Inicia e Encerra uma Sessão...\n");

    // 1. Arrange (Preparar) - Estado similar ao cenário 1, com cliente na fila
    // --------------------------------------------------------------------
    GameHouse__INITIALISATION();
    GameHouse__CLIENTES cliente1_id = 10;
    GameHouse__DISPOSITIVOS pc1_id = 1;
    GameHouse__TIPOS_DISPOSITIVO tipo_pc_id = 5;
    GameHouse__JOGOS jogo1_id = 100;
    bool pre_condicao_ok;

    GameHouse__add_tipo_dispositivo(tipo_pc_id);
    GameHouse__add_dispositivo(pc1_id, tipo_pc_id);
    GameHouse__add_jogo(jogo1_id, tipo_pc_id);
    GameHouse__instalar_jogo(pc1_id, jogo1_id);
    GameHouse__add_cliente(cliente1_id);
    GameHouse__comprar_creditos(cliente1_id, 3);
    GameHouse__entrar_fila_dispositivo(cliente1_id, pc1_id, jogo1_id);
    printf("  [PREPARAR] Estado inicial criado: Cliente %d na fila do dispositivo %d.\n", cliente1_id, pc1_id);

    // 2. Act & Assert (Iniciar Sessão)
    // --------------------------------------------------------------------
    printf("  [AGIR] Iniciando sessão para o primeiro da fila do dispositivo %d...\n", pc1_id);
    GameHouse__pre_iniciar_sessao(pc1_id, &pre_condicao_ok);
    assert(pre_condicao_ok == true);
    GameHouse__iniciar_sessao_fila(pc1_id);

    GameHouse__pre_encerrar_sessao(cliente1_id, &pre_condicao_ok);
    assert(pre_condicao_ok == true); // Agora a pré-condição para encerrar a sessão dele deve ser verdadeira
    printf("  [VERIFICAR] ✅ SUCESSO! Sessão iniciada para o cliente %d.\n", cliente1_id);

    // 3. Act & Assert (Encerrar Sessão)
    // --------------------------------------------------------------------
    printf("  [AGIR] Encerrando a sessão do cliente %d...\n", cliente1_id);
    GameHouse__encerrar_sessao(cliente1_id);
    
    GameHouse__pre_encerrar_sessao(cliente1_id, &pre_condicao_ok);
    assert(pre_condicao_ok == false); // Pré-condição para encerrar deve ser falsa, pois ele não está mais em sessão.
    printf("  [VERIFICAR] ✅ SUCESSO! Sessão do cliente %d encerrada corretamente.\n\n", cliente1_id);
}