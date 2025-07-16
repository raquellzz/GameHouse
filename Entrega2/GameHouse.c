#include "GameHouse.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


void clear_screen(void) {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void clean_stdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

const char *status_cliente_to_string(GameHouse__STATUS_CLIENTE s) {
  switch (s) {
  case GameHouse__ativo:
    return "Ativo";
  case GameHouse__inativo:
    return "Inativo";
  case GameHouse__em_espera:
    return "Em Espera";
  default:
    return "Desconhecido";
  }
}

const char *status_dispositivo_to_string(GameHouse__STATUS_DISPOSITIVO s) {
  switch (s) {
  case GameHouse__disponivel:
    return "Disponivel";
  case GameHouse__em_uso:
    return "Em Uso";
  case GameHouse__manutencao:
    return "Manutencao";
  default:
    return "Desconhecido";
  }
}

void print_menu() {
  printf("\n================================================================================\n");
    printf(" ------------------------ Sistema Interativo GameHouse ------------------------- \n");
    printf("================================================================================\n");

    // Seção de Cadastro organizada em duas colunas
    printf("\n ---------------------------- Cadastro e Instalação ---------------------------- \n");
    printf("\n");
    printf("   %-40s %s\n", "1. Adicionar Tipo de Dispositivo", "2. Adicionar Dispositivo");
    printf("   %-40s %s\n", "3. Adicionar Jogo", "4. Instalar Jogo em Dispositivo");
    printf("   %-40s %s\n", "5. Adicionar Cliente", "6. Comprar Créditos");

    // Seção de Fluxo Principal
    printf("\n ------------------------------- Fluxo Principal ------------------------------- \n");
    printf("\n");
    printf("   %-40s %s\n", "7. Cliente Entrar na Fila", "8. Iniciar Sessão do Próximo");
    printf("   %-40s %s\n", "9. Encerrar Sessão de Cliente", "10. Cliente Sair da Fila");

    // Seção de Consultas e Relatórios
    printf("\n --------------------------- Consultas e Relatórios ---------------------------- \n");
    printf("\n");
    printf("   %-40s %s\n", "11. Listar Dispositivos Disponíveis", " 12. Listar Jogos por Tipo");
    printf("   %-40s %s\n", "13. Listar Jogos de um Dispositivo", "14. Listar Todos os Clientes");
    printf("   %-40s %s\n", "15. Consultar Posição na Fila", "  16. Mudar Status de Dispositivo");
    printf("   %-40s %s\n", "17. Listar Tipos de Dispositivos", "18. Relatório Completo de Dispositivos");

    // Seção de Remoção e Gerenciamento
    printf("\n --------------------------- Remoção e Gerenciamento --------------------------- \n");
    printf("\n");
    printf("   %-40s %s\n", "21. Remover Tipo de Dispositivo", "22. Remover Dispositivo");
    printf("   %-40s %s\n", "23. Remover Jogo", "24. Desinstalar Jogo de Dispositivo");
    printf("   %-40s\n", "25. Banir Cliente");


    // Linha final e opção de saída
    printf("--------------------------------------------------------------------------------\n");
    printf("   %-40s\n", "0. Sair");
    printf("================================================================================\n");
    printf("\nEscolha uma opcao: ");
}

int main(void) {
  int option = -1;
  bool ok;

  GameHouse__INITIALISATION();

  do {
    clear_screen();
    print_menu();

    int scanf_result = scanf("%d", &option);
    if (scanf_result != 1) {
      printf("Entrada invalida. Por favor, digite um numero.\n");
      clean_stdin();
      option = -1;
      continue;
    }

    switch (option) {
    case 1: {
      GameHouse__TIPOS_DISPOSITIVO td;
      printf("Digite o ID do novo tipo de dispositivo: ");
      scanf("%d", &td);
      GameHouse__pre_add_tipo_dispositivo(td, &ok);
      
      if (ok) {
        GameHouse__add_tipo_dispositivo(td);
        printf(">>> SUCESSO: Tipo de dispositivo %d adicionado.\n", td);
      } else {
        printf(">>> ERRO: Precondicao falhou (tipo ja existe ou limite "
               "atingido).\n");
      }
      break;
    }
    case 2: {
      GameHouse__DISPOSITIVOS dd;
      GameHouse__TIPOS_DISPOSITIVO td;
      printf("Digite o ID do novo dispositivo: ");
      scanf("%d", &dd);
      printf("Digite o ID do tipo para este dispositivo: ");
      scanf("%d", &td);
      GameHouse__pre_add_dispositivo(dd, td, &ok);
      if (ok) {
        GameHouse__add_dispositivo(dd, td);
        printf(">>> SUCESSO: Dispositivo %d (tipo %d) adicionado.\n", dd, td);
      } else {
        printf(">>> ERRO: Precondicao falhou (dispositivo ja existe, tipo "
               "invalido ou limite atingido).\n");
      }
      break;
    }
    case 3: {
      GameHouse__JOGOS jj;
      GameHouse__TIPOS_DISPOSITIVO td;
      printf("Digite o ID do novo jogo: ");
      scanf("%d", &jj);
      printf("Digite o ID do tipo de dispositivo para este jogo: ");
      scanf("%d", &td);
      GameHouse__pre_add_jogo(jj, td, &ok);
      if (ok) {
        GameHouse__add_jogo(jj, td);
        printf(">>> SUCESSO: Jogo %d (tipo %d) adicionado.\n", jj, td);
      } else {
        printf(">>> ERRO: Precondicao falhou (jogo ja existe ou tipo "
               "invalido).\n");
      }
      break;
    }
    case 4: {
      GameHouse__DISPOSITIVOS dd;
      GameHouse__JOGOS jj;
      printf("Digite o ID do dispositivo: ");
      scanf("%d", &dd);
      printf("Digite o ID do jogo a ser instalado: ");
      scanf("%d", &jj);
      GameHouse__pre_instalar_jogo(dd, jj, &ok);
      if (ok) {
        GameHouse__instalar_jogo(dd, jj);
        printf(">>> SUCESSO: Jogo %d instalado no dispositivo %d.\n", jj, dd);
      } else {
        printf(">>> ERRO: Precondicao falhou (tipos incompativeis, ja "
               "instalado, etc.).\n");
      }
      break;
    }
    case 5: {
      GameHouse__CLIENTES cc;
      printf("Digite o ID do novo cliente: ");
      scanf("%d", &cc);
      GameHouse__pre_add_cliente(cc, &ok);
      if (ok) {
        GameHouse__add_cliente(cc);
        printf(">>> SUCESSO: Cliente %d adicionado.\n", cc);
      } else {
        printf(">>> ERRO: Precondicao falhou (cliente ja existe ou limite "
               "atingido).\n");
      }
      break;
    }
    case 6: {
      GameHouse__CLIENTES cc;
      int32_t qtd;
      printf("Digite o ID do cliente: ");
      scanf("%d", &cc);
      printf("Digite a quantidade de creditos (1-3): ");
      scanf("%d", &qtd);
      GameHouse__pre_comprar_creditos(cc, qtd, &ok);
      if (ok) {
        GameHouse__comprar_creditos(cc, qtd);
        printf(">>> SUCESSO: Creditos comprados para o cliente %d.\n", cc);
      } else {
        printf(">>> ERRO: Precondicao falhou (cliente invalido, quantidade "
               "invalida ou excede o maximo).\n");
      }
      break;
    }
    case 7: {
      GameHouse__CLIENTES cc;
      GameHouse__DISPOSITIVOS dd;
      GameHouse__JOGOS jj;
      printf("Digite o ID do cliente: ");
      scanf("%d", &cc);
      printf("Digite o ID do dispositivo desejado: ");
      scanf("%d", &dd);
      printf("Digite o ID do jogo desejado: ");
      scanf("%d", &jj);
      GameHouse__pre_entrar_fila_dispositivo(cc, dd, jj, &ok);
      if (ok) {
        GameHouse__entrar_fila_dispositivo(cc, dd, jj);
        printf(">>> SUCESSO: Cliente %d entrou na fila do dispositivo %d para "
               "jogar %d.\n",
               cc, dd, jj);
      } else {
        printf(">>> ERRO: Precondicao falhou (verifique creditos, status, se "
               "ja esta em fila, etc.).\n");
      }
      break;
    }
    case 8: {
      GameHouse__DISPOSITIVOS dd;
      printf("Digite o ID do dispositivo para iniciar a sessao do proximo da "
             "fila: ");
      scanf("%d", &dd);
      GameHouse__pre_iniciar_sessao(dd, &ok);
      if (ok) {
        GameHouse__iniciar_sessao_fila(dd);
        printf(">>> SUCESSO: Sessao iniciada no dispositivo %d.\n", dd);
      } else {
        printf(">>> ERRO: Precondicao falhou (dispositivo nao esta disponivel "
               "ou fila esta vazia).\n");
      }
      break;
    }
    case 9: {
      GameHouse__CLIENTES cc;
      printf("Digite o ID do cliente para encerrar a sessao: ");
      scanf("%d", &cc);
      GameHouse__pre_encerrar_sessao(cc, &ok);
      if (ok) {
        GameHouse__encerrar_sessao(cc);
        printf(">>> SUCESSO: Sessao do cliente %d encerrada.\n", cc);
      } else {
        printf(">>> ERRO: Precondicao falhou (cliente nao esta em sessao).\n");
      }
      break;
    }
    case 10: {
      GameHouse__CLIENTES cc;
      GameHouse__DISPOSITIVOS dd;
      printf("Digite o ID do cliente: ");
      scanf("%d", &cc);
      printf("Digite o ID do dispositivo da fila: ");
      scanf("%d", &dd);
      GameHouse__pre_sair_fila_dispositivo(cc, dd, &ok);
      if (ok) {
        GameHouse__sair_fila_dispositivo(cc, dd);
        printf(">>> SUCESSO: Cliente %d saiu da fila do dispositivo %d.\n", cc,
               dd);
      } else {
        printf(">>> ERRO: Precondicao falhou (cliente nao esta nesta fila).\n");
      }
      break;
    }
    case 11: {
      bool more;
      GameHouse__DISPOSITIVOS current_dd;
      printf("\n--- Dispositivos Disponiveis ---\n");
      GameHouse__init_disp_disponiveis();
      GameHouse__has_more_disp_disponiveis(&more);
      if (!more) {
        printf("  (Nenhum)\n");
      }
      while (more) {
        GameHouse__get_next_disp_disponivel(&current_dd);
        printf("  - Dispositivo ID: %d\n", current_dd);
        GameHouse__has_more_disp_disponiveis(&more);
      }
      printf("---------------------------------\n");
      break;
    }
    case 12: {
      bool more;
      GameHouse__TIPOS_DISPOSITIVO td;
      GameHouse__JOGOS current_jj;
      printf("Digite o ID do tipo de dispositivo para listar os jogos: ");
      scanf("%d", &td);
      printf("\n--- Jogos do Tipo %d ---\n", td);
      GameHouse__init_jogos_por_tipo();
      GameHouse__has_more_jogos_por_tipo(td, &more);
      if (!more) {
        printf("  (Nenhum jogo encontrado para este tipo)\n");
      }
      while (more) {
        GameHouse__get_next_jogo_por_tipo(td, &current_jj);
        printf("  - Jogo ID: %d\n", current_jj);
        GameHouse__has_more_jogos_por_tipo(td, &more);
      }
      printf("-----------------------\n");
      break;
    }
    case 13: {
      bool more;
      GameHouse__DISPOSITIVOS dd;
      GameHouse__JOGOS current_jj;
      printf("Digite o ID do dispositivo para listar os jogos instalados: ");
      scanf("%d", &dd);
      printf("\n--- Jogos Instalados no Dispositivo %d ---\n", dd);
      GameHouse__init_jogos_dispositivo();
      GameHouse__has_more_jogos_dispositivo(dd, &more);
      if (!more) {
        printf("  (Nenhum jogo instalado neste dispositivo)\n");
      }
      while (more) {
        GameHouse__get_next_jogo_dispositivo(dd, &current_jj);
        printf("  - Jogo ID: %d\n", current_jj);
        GameHouse__has_more_jogos_dispositivo(dd, &more);
      }
      printf("-----------------------------------------\n");
      break;
    }
    case 14: {
      bool more;
      GameHouse__CLIENTES cc;
      GameHouse__STATUS_CLIENTE ss;
      int32_t cr;
      printf("\n--- Informacoes dos Clientes ---\n");
      GameHouse__init_clientes_info();
      GameHouse__has_more_clientes_info(&more);
      if (!more) {
        printf("  (Nenhum cliente cadastrado)\n");
      }
      while (more) {
        GameHouse__get_next_cliente_info(&cc, &ss, &cr);
        printf("  - Cliente: %-3d | Status: %-10s | Creditos: %d\n", cc,
               status_cliente_to_string(ss), cr);
        GameHouse__has_more_clientes_info(&more);
      }
      printf("--------------------------------\n");
      break;
    }
    case 15: {
      GameHouse__CLIENTES cc;
      GameHouse__DISPOSITIVOS dd;
      int32_t pos = 0;
      printf("Digite o ID do cliente: ");
      scanf("%d", &cc);
      printf("Digite o ID do dispositivo da fila: ");
      scanf("%d", &dd);
      GameHouse__posicao_fila(cc, dd, &pos);
      if (pos > 0) {
        printf(">>> Posicao do cliente %d na fila do dispositivo %d: %d\n", cc,
               dd, pos);
      } else {
        printf(">>> Cliente %d nao encontrado na fila do dispositivo %d.\n", cc,
               dd);
      }
      break;
    }
    case 16: {
      GameHouse__DISPOSITIVOS dd;
      int status_in;
      GameHouse__STATUS_DISPOSITIVO ss;
      printf("Digite o ID do dispositivo: ");
      scanf("%d", &dd);
      printf("Digite o novo status (0=Disponivel, 1=Em Uso, 2=Manutencao): ");
      scanf("%d", &status_in);

      if (status_in >= 0 && status_in <= 2) {
        ss = (GameHouse__STATUS_DISPOSITIVO)status_in;
        GameHouse__pre_set_status_dispositivo(dd, ss, &ok);
        if (ok) {
          GameHouse__set_status_dispositivo(dd, ss);
          printf(">>> SUCESSO: Status do dispositivo %d alterado.\n", dd);
        } else {
          printf(
              ">>> ERRO: Precondicao falhou (mudanca de status invalida).\n");
        }
      } else {
        printf(">>> ERRO: Status invalido.\n");
      }
      break;
    }
    case 17: {
      bool more;
      GameHouse__TIPOS_DISPOSITIVO current_td;
      printf("\n--- Tipos de Dispositivos Cadastrados ---\n");
      GameHouse__init_tipos_disponiveis();
      GameHouse__has_more_tipos_disponiveis(&more);
      if (!more) {
        printf("  (Nenhum tipo de dispositivo cadastrado)\n");
      }
      while (more) {
        GameHouse__get_next_tipo_dispositivo(&current_td);
        printf("  - Tipo de Dispositivo ID: %d\n", current_td);
        GameHouse__has_more_tipos_disponiveis(&more);
      }
      printf("-----------------------------------------\n");
      break;
    }
    case 18: {
      bool has_devices;
      GameHouse__DISPOSITIVOS dev_id;
      GameHouse__TIPOS_DISPOSITIVO dev_tipo;
      GameHouse__STATUS_DISPOSITIVO dev_status;

      printf("\n--- Relatorio Completo de Dispositivos ---\n");
      GameHouse__init_devices_info();
      GameHouse__has_more_devices_info(&has_devices);

      if (!has_devices) {
        printf("  (Nenhum dispositivo cadastrado)\n");
      }

      while (has_devices) {
        GameHouse__get_next_device_info(&dev_id, &dev_tipo, &dev_status);
        printf("\n* Dispositivo ID: %d\n", dev_id);
        printf("  - Tipo: %d\n", dev_tipo);
        printf("  - Status: %s\n", status_dispositivo_to_string(dev_status));
        printf("  - Jogos Instalados:\n");

        bool has_games;
        GameHouse__JOGOS game_id;
        GameHouse__init_jogos_dispositivo();
        GameHouse__has_more_jogos_dispositivo(dev_id, &has_games);
        if (!has_games) {
          printf("    (Nenhum)\n");
        }
        while (has_games) {
          GameHouse__get_next_jogo_dispositivo(dev_id, &game_id);
          printf("    - Jogo ID: %d\n", game_id);
          GameHouse__has_more_jogos_dispositivo(dev_id, &has_games);
        }

        GameHouse__has_more_devices_info(&has_devices);
      }
      printf("-----------------------------------------\n");
      break;
    }
    case 21: {
      GameHouse__TIPOS_DISPOSITIVO td;
      printf("Digite o ID do tipo de dispositivo a ser removido: ");
      scanf("%d", &td);
      GameHouse__pre_remove_tipo_dispositivo(td, &ok);
      if (ok) {
        GameHouse__remove_tipo_dispositivo(td);
        printf(">>> SUCESSO: Tipo de dispositivo %d removido.\n", td);
      } else {
        printf(">>> ERRO: Precondicao falhou (tipo em uso ou inexistente).\n");
      }
      break;
    }
    case 22: {
      GameHouse__DISPOSITIVOS dd;
      printf("Digite o ID do dispositivo a ser removido: ");
      scanf("%d", &dd);
      GameHouse__pre_remove_dispositivo(dd, &ok);
      if (ok) {
        GameHouse__remove_dispositivo(dd);
        printf(">>> SUCESSO: Dispositivo %d removido.\n", dd);
      } else {
        printf(">>> ERRO: Precondicao falhou (dispositivo em uso, na fila ou "
               "inexistente).\n");
      }
      break;
    }
    case 23: {
      GameHouse__JOGOS jj;
      printf("Digite o ID do jogo a ser removido: ");
      scanf("%d", &jj);
      GameHouse__pre_remover_jogo(jj, &ok);
      if (ok) {
        GameHouse__remove_jogo(jj);
        printf(">>> SUCESSO: Jogo %d removido.\n", jj);
      } else {
        printf(">>> ERRO: Precondicao falhou (jogo em uso ou inexistente).\n");
      }
      break;
    }
    case 24: {
      GameHouse__DISPOSITIVOS dd;
      GameHouse__JOGOS jj;
      printf("Digite o ID do dispositivo: ");
      scanf("%d", &dd);
      printf("Digite o ID do jogo a ser desinstalado: ");
      scanf("%d", &jj);
      GameHouse__pre_desinstalar_jogo(dd, jj, &ok);
      if (ok) {
        GameHouse__desinstalar_jogo(dd, jj);
        printf(">>> SUCESSO: Jogo %d desinstalado do dispositivo %d.\n", jj,
               dd);
      } else {
        printf(">>> ERRO: Precondicao falhou (jogo ou dispositivo invalido, "
               "jogo nao esta instalado).\n");
      }
      break;
    }
    case 25: {
      GameHouse__CLIENTES cc;
      printf("Digite o ID do cliente a ser banido: ");
      scanf("%d", &cc);
      GameHouse__pre_banir_cliente(cc, &ok);
      if (ok) {
        GameHouse__banir_cliente(cc);
        printf(">>> SUCESSO: Cliente %d banido.\n", cc);
      } else {
        printf(">>> ERRO: Precondicao falhou (cliente em sessao, na fila ou "
               "inexistente).\n");
      }
      break;
    }

    case 0:
      printf("Encerrando o programa.\n");
      break;
    default:
      printf(">>> ERRO: Opcao invalida. Tente novamente.\n");
      break;
    }

    if (option != 0) {
      printf("\nPressione Enter para voltar ao menu...");
      clean_stdin();
      getchar();
    }

  } while (option != 0);

  return 0;
}

