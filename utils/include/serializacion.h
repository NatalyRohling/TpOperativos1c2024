#ifndef SERIALIZACION_H_
#define SERIALIZACION_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <commons/collections/list.h>

#include "instrucciones.h"
#include "registros.h"
#include "utils.h"

typedef struct
{
    uint32_t size;
    uint32_t offset;
    void *stream;

} tipo_buffer;

// ENVIAR Y RECIBIR CODIGOS OP_CODE
void enviar_cod_enum(int socket_servidor, uint32_t cod);
op_code recibir_operacion(int socket_cliente);

// BUFFERS
tipo_buffer *crear_buffer();
tipo_buffer *recibir_buffer(int socket);
void destruir_buffer(tipo_buffer *buffer);
void agregar_cde_buffer(tipo_buffer *buffer, t_cde *cde);
void agregar_buffer_para_enterosUint32(tipo_buffer *buffer, uint32_t entero);
void agregar_buffer_para_enterosUint8(tipo_buffer *buffer, uint8_t entero);
void agregar_buffer_para_string(tipo_buffer *buffer, char *args);
void agregar_buffer_para_registros(tipo_buffer *buffer, t_registros *registros);
void enviar_buffer(tipo_buffer *buffer, int socket);

uint32_t leer_buffer_enteroUint32(tipo_buffer *buffer);
uint8_t leer_buffer_enteroUint8(tipo_buffer *buffer);
char *leer_buffer_string(tipo_buffer *buffer);
t_registros *leer_buffer_registros(tipo_buffer *buffer);
t_cde *leer_cde(tipo_buffer *buffer);

#endif