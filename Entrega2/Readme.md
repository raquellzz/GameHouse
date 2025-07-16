# Projeto GameHouse - Teste Interativo em C

## Descrição

Este projeto é uma interface de linha de comando (CLI) interativa para testar a implementação em C do sistema `GameHouse`.

O núcleo lógico do sistema (`GameHouse_i.c`) foi gerado automaticamente a partir de um modelo formal escrito em Método B, garantindo que as regras de negócio sejam robustas e corretas. Esta interface de console serve como a "casca" ou a "camada de apresentação" para interagir com esse núcleo lógico verificado.

## Estrutura dos Arquivos

-   `README.md`: Este arquivo.
-   `GameHouse.h`: A interface pública (protótipos de função) da máquina principal.
-   `GameHouse_ctx.h`: As definições de constantes e tipos do contexto B.
-   `GameHouse_ctx_i.c`: A implementação em C da máquina contexto, gerada pela ferramenta B.
-   `GameHouse_imp.c`: A implementação em C do núcleo lógico, gerada pela ferramenta B.
-   `GameHouse.c`: O programa principal que fornece o shell interativo para o usuário.

-   `GameHouse_ctx.mch`: A máquina asbtrata de contexto de Gamehouse, que contém sets e constants.
-   `GameHouse_ctx_i.imp`: A implementação máquina asbtrata de contexto de Gamehouse, que dá valor aos sets e constants da abstrata.
-   `GameHouse.mch`: A máquina asbtrata principal, que fornece todas as operações usadas.
-   `GameHouse_i.imp`: A implementação da máquina asbtrata principal, que transforma as operações abstratas em concretas.

## Como Compilar e Executar

Este projeto foi testado com o compilador GCC em um ambiente Linux.

### Pré-requisitos

-   Um compilador C (como GCC ou Clang) instalado.

### 1. Comando de Compilação

Para compilar todos os arquivos e gerar o executável `gamehouse`, navegue até a pasta do projeto no seu terminal e execute o seguinte comando:

```bash
gcc -o gamehouse_cli GameHouse.c GameHouse_i.c -I.
```

-   **`gcc`**: Chama o compilador.
-   **`-o gamehouse_cli`**: Define o nome do arquivo de saída (o executável).
-   **`GameHouse.c GameHouse_i.c`**: Os arquivos fonte C a serem compilados.
-   **`-I.`**: Diz ao compilador para procurar por arquivos de cabeçalho (`.h`) no diretório atual.

### 2. Comando para Executar

Após a compilação bem-sucedida, execute o programa com o comando:

```bash
./gamehouse_cli
```

## Uso Básico

Ao iniciar o programa, um prompt `GameHouse >` será exibido.

-   Os comandos disponíveis estarão visíveis.
-   Digite `0` para encerrar a aplicação.