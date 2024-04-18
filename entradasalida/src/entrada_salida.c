#include "../include/entrada_salida.h"

int main(int argc, char *argv[])
{
	struct config_io *valores_config = config_io();
	logger = iniciar_logger("entrada_salida.log", "ENTRADA_SALIDA");

	levantarCliente(conexion_memoria, logger, "Servidor Memoria", valores_config->ip_memoria, valores_config->puerto_memoria, "ENTRADA/SALIDA se conecta a Memoria");
	levantarCliente(conexion_kernel, logger, "Servidor Kernel", valores_config->ip_kernel, valores_config->puerto_kernel, "ENTRADA/SALIDA se conecta a Kernel");

	terminar_programa(conexion_memoria, logger, valores_config->config);
	terminar_programa(conexion_kernel, logger, valores_config->config);
}

// FUNCIONES

void leer_consola(t_log *logger)
{
	char *leido;
	leido = readline("> ");
	log_info(logger, "%s", leido);
	while (strcmp(leido, "") != 0)
	{
		free(leido);
		leido = readline("> ");
		log_info(logger, "%s", leido);
	}
	free(leido);
}
