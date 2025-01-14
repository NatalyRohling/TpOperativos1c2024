#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <semaphore.h>
#include <commons/collections/list.h>
#include <assert.h>

#include "registros.h"
#include "sockets.h"
#include "printf_color.h"

typedef t_list t_lista_recursos;
typedef struct
{
	t_config *config;
	char *ip_memoria;
	char *puerto_memoria;
	int tam_memoria;
	int tam_pagina;
	char *path_instrucciones;
	int retardo_respuesta;
} config_memoria;

typedef enum
{
	SUCCESS,
	INVALID_RESOURCE,
	INVALID_INTERFACE,
	OUT_OF_MEMORY_END,
	INTERRUPTED_BY_USER
} motivoFinalizar;

typedef enum
{
	STRING,
	UINT32_T,
	UINT8_T,
	INTEGER
} tipoDato;

typedef enum
{
	EJECUTAR_SCRIPT,
	INICIAR_PROCESO,
	FINALIZAR_PROCESO_MENU,
	INICIAR_PLANIFICACION,
	DETENER_PLANIFICACION,
	MULTIPROGRAMACION,
	LISTAR_ESTADOS,
} opciones_menu;

typedef enum
{
	SOLICITUD_INICIAR_PROCESO,
	INICIAR_PROCESO_CORRECTO,
	ERROR_INICIAR_PROCESO,
	SOLICITUD_FINALIZAR_PROCESO,
	FINALIZAR_PROCESO,
	PEDIDO_INSTRUCCION,
	PEDIDO_PCB,
	EJECUTAR_PROCESO, // CPU
	SOLICITUD_INTERFAZ_GENERICA,
	NO_ESTOY_LIBRE,
	CONCLUI_OPERACION,
	ESTOY_LIBRE,
	CONSULTAR_DISPONIBILDAD,
	SOLICITUD_EXIT, // cortoPlazo
	SOLICITUD_INTERFAZ_STDIN,
	SOLICITUD_INTERFAZ_STDOUT,
	SOLICITUD_INTERFAZ_DIALFS,
	ENVIAR_INSTRUCCION_CORRECTO,
	FIN_DE_QUANTUM, // cpu, kernel
	SOLICITUD_CONEXION_IO,
	PROCESO_INTERRUMPIDO_QUANTUM,
	INSTRUCCION_INTERFAZ,
	ESTABA_CONECTADO,
	ACCESO_ESPACIO_USUARIO,
	NO_ESTABA_CONECTADO,
	PEDIDO_ESCRITURA,
	ERROR_PEDIDO_ESCRITURA,
	PEDIDO_LECTURA,
	ERROR_PEDIDO_LECTURA,
	OUT_OF_MEMORY,
	RESIZE_OP,
	OK,
	SIGNAL_RECURSO,
	WAIT_RECURSO,
	RESIZE_EXITOSO,
	ERROR_CREAR_ARCHIVO_OK,
	PEDIDO_FRAME,
	PEDIDO_FRAME_CORRECTO,
	PEDIDO_FRAME_INCORRECTO,
	CONFIRMAR_CONEXION,
	INICIAR_OPERACION
} op_code;

typedef enum
{
	GENERICA,
	STDIN,
	STDOUT,
	DIALFS,
} enum_interfaz;

typedef enum
{
	F_CREATE,
	F_DELETE,
	F_TRUNCATE,
	F_READ,
	F_WRITE
} operacion_dialfs;
typedef struct
{
	enum_interfaz tipo_interfaz;
	int tiempo_unidad_trabajo;
	char *ip_kernel;
	int puerto_kernel;
	char *ip_memoria;
	int puerto_memoria;
	char *path_base_dialfs;
	int block_size;
	int block_count;
	int retraso_compactacion;
	char *nombre_interfaz;
} t_interfaz;

typedef enum
{
	NEW,
	READY,
	READY_PLUS,
	EXEC,
	BLOCKED,
	FINISHED
} t_estados;
typedef struct
{
	t_cde *cde;		  // contexto de ejecucion
	t_estados estado; // estado del proceso
	t_list *recursosAsignados;
	int quantum;
} t_pcb;

// VARIABLES
extern t_log *logger;

// FUNCIONES

void sleep_ms(int milliseconds);
void terminar_programa(int, t_log *, t_config *);
void destruirLog(t_log *logger);
void destruirConfig(t_config *config);
void liberarConexion(int conexion);
void enviar_op_code(int socket_servidor, uint32_t cod);
op_code recibir_op_code(int socket_cliente);
t_log *iniciar_logger(char *path_log, char *nombre_log);
t_config *iniciar_config(char *config_path);

#endif