#include "../include/utils.h"

void *serializar_paquete(t_paquete *paquete, int bytes)
{
	void *magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento += paquete->buffer->size;

	return magic;
}

void enviar_mensaje(char *mensaje, int socket_cliente)
{
	t_paquete *paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(tipo_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2 * sizeof(int);

	void *a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}

t_paquete *crear_paquete(void)
{
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete *paquete, void *valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete *paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2 * sizeof(int);
	void *a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete *paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

// UTILS DE SERVIDOR
/*
tipo_buffer *recibir_buffer_propio(int socket)
{
	tipo_buffer *buffer = crear_buffer();
	if (buffer == NULL)
	{
		// Manejar error de creación de buffer
		return NULL;
	}
	// Recibo el tamaño del buffer y reservo espacio en memoria
	recv(socket, &(buffer->size), sizeof(uint32_t), MSG_WAITALL);
	if (buffer->size > 0)
	{
		buffer->stream = malloc(buffer->size);
		if (buffer->stream == NULL)
		{
			// Manejar error de malloc
			destruir_buffer(buffer);
			return NULL;
		}
		// Recibo stream del buffer
		recv(socket, buffer->stream, buffer->size, MSG_WAITALL);
	}
	return buffer;
}
*/
void recibir_mensaje(int socket_cliente)
{
	tipo_buffer *buffer = recibir_buffer_propio(socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list *recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	tipo_buffer *buffer;
	t_list *valores = list_create();
	int tamanio;

	buffer = recibir_buffer_propio(socket_cliente);
	while (desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);
		char *valor = malloc(tamanio);
		memcpy(valor, buffer + desplazamiento, tamanio);
		desplazamiento += tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}

// UTILS GENERALES

t_log *iniciar_logger(char *path_log, char *nombre_log)
{
	t_log *nuevo_logger;
	nuevo_logger = log_create(path_log, nombre_log, 1, LOG_LEVEL_INFO);
	if (nuevo_logger == NULL)
	{
		return NULL;
	};
	return nuevo_logger;
}

t_config *iniciar_config(char *config_path)
{
	t_config *nuevo_config;
	nuevo_config = config_create(config_path);
	if (nuevo_config == NULL)
	{
		printf("\nNo se pudo leer la config\n");
		exit(2);
	}
	return nuevo_config;
}
// TERMINAR PROGRAMA
void terminar_programa(int conexion, t_log *logger, t_config *config)
{
	destruirLog(logger);
	destruirConfig(config);
	liberar_conexion(conexion);
}

void destruirLog(t_log *logger)
{
	if (logger != NULL)
	{
		log_destroy(logger);
	}
}
void destruirConfig(t_config *config)
{
	if (config != NULL)
	{
		config_destroy(config);
	}
}
void liberarConexion(int conexion)
{
	if (conexion != 0)
	{
		liberar_conexion(conexion);
	}
}
void paquete(int conexion)
{
	char *leido = NULL;
	t_paquete *paquete = crear_paquete();

	leido = readline(">");
	while (strcmp(leido, "") != 0)
	{
		agregar_a_paquete(paquete, leido, strlen(leido) - 1);
		free(leido);
		leido = readline("> ");
	}
	free(leido);
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}
