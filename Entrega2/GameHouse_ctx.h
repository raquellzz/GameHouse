#ifndef _GameHouse_ctx_h
#define _GameHouse_ctx_h

#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define GameHouse_ctx__DISPOSITIVOS__max GameHouse_ctx__MAX_DISPOSITIVOS
#define GameHouse_ctx__CLIENTES__max GameHouse_ctx__MAX_CLIENTES
#define GameHouse_ctx__TIPOS_DISPOSITIVO__max GameHouse_ctx__MAX_TIPOS_DISPOSITIVO
#define GameHouse_ctx__JOGOS__max GameHouse_ctx__MAX_JOGOS
/* Clause SETS */
typedef int GameHouse_ctx__DISPOSITIVOS;
typedef int GameHouse_ctx__CLIENTES;
typedef int GameHouse_ctx__TIPOS_DISPOSITIVO;
typedef int GameHouse_ctx__JOGOS;
typedef enum
{
    GameHouse_ctx__ativo,
    GameHouse_ctx__inativo,
    GameHouse_ctx__em_espera
    
} GameHouse_ctx__STATUS_CLIENTE;
#define GameHouse_ctx__STATUS_CLIENTE__max 3
typedef enum
{
    GameHouse_ctx__disponivel,
    GameHouse_ctx__em_uso,
    GameHouse_ctx__manutencao
    
} GameHouse_ctx__STATUS_DISPOSITIVO;
#define GameHouse_ctx__STATUS_DISPOSITIVO__max 3

/* Clause CONCRETE_CONSTANTS */
/* Basic constants */
#define GameHouse_ctx__MAX_DISPOSITIVOS 12
#define GameHouse_ctx__MAX_CREDITOS 3
#define GameHouse_ctx__MAX_TIPOS_DISPOSITIVO 5
#define GameHouse_ctx__MAX_JOGOS 35
#define GameHouse_ctx__MAX_CLIENTES 30
#define GameHouse_ctx__MAX_JOGOS_POR_DISPOSITIVO 5
#define GameHouse_ctx__MAX_SESSOES 12
#define GameHouse_ctx__MAX_ENTRADAS_FILA 7
/* Array and record constants */









/* Clause CONCRETE_VARIABLES */

extern void GameHouse_ctx__INITIALISATION(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _GameHouse_ctx_h */
