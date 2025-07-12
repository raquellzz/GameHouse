#ifndef _GameHouse_ctx_h
#define _GameHouse_ctx_h

#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* TODO : #define GameHouse_ctx__DISPOSITIVOS__max */
/* TODO : #define GameHouse_ctx__CLIENTES__max */
/* TODO : #define GameHouse_ctx__TIPOS_DISPOSITIVO__max */
/* TODO : #define GameHouse_ctx__JOGOS__max */
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
/* TO DO: #define GameHouse_ctx__MAX_DISPOSITIVOS */
/* TO DO: #define GameHouse_ctx__MAX_CREDITOS */
/* TO DO: #define GameHouse_ctx__MAX_TIPOS_DISPOSITIVO */
/* TO DO: #define GameHouse_ctx__MAX_JOGOS */
/* TO DO: #define GameHouse_ctx__MAX_CLIENTES */
/* TO DO: #define GameHouse_ctx__MAX_JOGOS_POR_DISPOSITIVO */
/* TO DO: #define GameHouse_ctx__MAX_SESSOES */
/* TO DO: #define GameHouse_ctx__MAX_ENTRADAS_FILA */
/* Array and record constants */
#define GameHouse__MAX_DISPOSITIVOS 12
#define GameHouse__MAX_CLIENTES 30
#define GameHouse__MAX_CREDITOS 3
#define GameHouse__MAX_JOGOS 35
#define GameHouse__MAX_TIPOS_DISPOSITIVO 5
#define GameHouse__MAX_JOGOS_POR_DISPOSITIVO 5
#define GameHouse__MAX_SESSOES 12
#define GameHouse__MAX_ENTRADAS_FILA 7










/* Clause CONCRETE_VARIABLES */

extern void GameHouse_ctx__INITIALISATION(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _GameHouse_ctx_h */
