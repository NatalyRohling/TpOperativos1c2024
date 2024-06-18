#include "../include/operaciones_es.h"

int conexion_kernel, conexion_memoria, estoy_libre;

void realizar_operacion_gen(t_interfaz *interfaz)
{
	sleep_ms(interfaz->tiempo_unidad_trabajo);
	tipo_buffer *buffer_gen = recibir_buffer(conexion_kernel);
	t_tipoDeInstruccion instruccion = leer_buffer_enteroUint32(buffer_gen);
	int unidades_tiempo = leer_buffer_enteroUint32(buffer_gen);
	int pid = leer_buffer_enteroUint32(buffer_gen);
	destruir_buffer(buffer_gen);

	if (instruccion == IO_GEN_SLEEP)
	{
		sleep_ms(unidades_tiempo);
		log_info(logger, "PID: <%d> - Operacion: <IO_GEN_SLEEP>", pid);
	}
	else
	{
		log_error(logger, "ERROR - INSTRUCCION INCORRECTA");
	}

	estoy_libre = 1;
}

void realizar_operacion_stdin(t_interfaz *interfaz)
{
	sleep_ms(interfaz->tiempo_unidad_trabajo);
	tipo_buffer *buffer_stdin = recibir_buffer(conexion_kernel);
	t_tipoDeInstruccion instruccion = leer_buffer_enteroUint32(buffer_stdin);
	int tamanio = leer_buffer_enteroUint32(buffer_stdin);
	int direccion_fisica = leer_buffer_enteroUint32(buffer_stdin);
	int pid = leer_buffer_enteroUint32(buffer_stdin);
	destruir_buffer(buffer_stdin);

	if (instruccion == IO_STDIN_READ)
	{
		char *texto_ingresado = readline("Ingrese un texto por teclado: ");
		t_list *lista_enteros = truncar_y_convertir(texto_ingresado, pid, tamanio);
		int cant_enteros = list_size(lista_enteros);

		enviar_cod_enum(conexion_memoria, ACCESO_ESPACIO_USUARIO);
		enviar_cod_enum(conexion_memoria, PEDIDO_ESCRITURA);
		enviar_cod_enum(conexion_memoria, SOLICITUD_INTERFAZ_STDIN);

		enviar_buffer_stdin_memoria(direccion_fisica, pid, cant_enteros, lista_enteros);

		op_code codigo_memoria = recibir_operacion(conexion_memoria);
		if (codigo_memoria == OK)
		{

			log_info(logger, "PID: <%d> - Operacion: <IO_STDIN_READ>", pid);
		}
		else if (codigo_memoria == ERROR_PEDIDO_ESCRITURA)
		{
			log_error(logger, "PID: <%d> - ERROR Operacion: <IO_STDIN_READ>", pid);
		}
	}
	else
	{
		log_error(logger, "ERROR - INSTRUCCION INCORRECTA");
	}
	estoy_libre = 1;
}

void realizar_operacion_stdout(t_interfaz *interfaz)
{
	sleep_ms(interfaz->tiempo_unidad_trabajo);
	tipo_buffer *buffer_sol_operacion = recibir_buffer(conexion_kernel);
	t_tipoDeInstruccion sol_operacion = leer_buffer_enteroUint32(buffer_sol_operacion);
	int limitante_cadena = leer_buffer_enteroUint32(buffer_sol_operacion); // con este valor, se lo envio a la memoria para que
	log_info(logger, "TAMANIO A LEER %d", limitante_cadena);
	int direccion_fisica = leer_buffer_enteroUint32(buffer_sol_operacion); // donde voy a pedirle a memoria que busque el dato
	int pid = leer_buffer_enteroUint32(buffer_sol_operacion);
	if (sol_operacion == IO_STDOUT_WRITE)
	{
		enviar_cod_enum(conexion_memoria, ACCESO_ESPACIO_USUARIO);
		enviar_cod_enum(conexion_memoria, PEDIDO_LECTURA);
		enviar_cod_enum(conexion_memoria, SOLICITUD_INTERFAZ_STDOUT);

		tipo_buffer *buffer_a_memoria = crear_buffer();
		agregar_buffer_para_enterosUint32(buffer_a_memoria, direccion_fisica);
		agregar_buffer_para_enterosUint32(buffer_a_memoria, pid);
		agregar_buffer_para_enterosUint32(buffer_a_memoria, limitante_cadena);
		enviar_buffer(buffer_a_memoria, conexion_memoria);

		destruir_buffer(buffer_a_memoria);

		op_code codigo_memoria = recibir_operacion(conexion_memoria);
		if (codigo_memoria == OK)
		{
			char *texto_reconstruido = malloc(limitante_cadena);
			tipo_buffer *lectura = recibir_buffer(conexion_memoria);
			for (int i = 0; i < limitante_cadena; i++)
			{
				int entero_valor = leer_buffer_enteroUint32(lectura);
				int_a_char_y_concatenar_a_string(entero_valor, texto_reconstruido);
				log_info(logger, "Valor hallado en Direccion Fisica <%d> : %s", direccion_fisica, string_itoa(entero_valor));
			}
			log_info(logger, "VALOR: %s", texto_reconstruido);
			destruir_buffer(lectura);

			log_info(logger, "PID: <%d> - Operacion: <IO_STDOUT_WRITE>", pid);
		}
		else if (codigo_memoria == ERROR_PEDIDO_LECTURA)
		{
			log_error(logger, "PID: <%d> - ERROR Operacion: <IO_STDOUT_WRITE>", pid);
		}
	}
	else
	{
		log_info(logger, "Hubo un error al traer la operacion IO_STDOUT_WRITE");
	}
	estoy_libre = 1;
	destruir_buffer(buffer_sol_operacion);
}

void realizar_operacion_dialfs(t_interfaz *interfaz) // IMPLEMENTAR
{
	// FALTAN ESTOS LOGS
	// log_info(logger, "PID: <%d> - Inicio Compactacion");
	// log_info(logger, "PID: <%d> - FIn Compactacion");
	sleep_ms(interfaz->tiempo_unidad_trabajo);
	tipo_buffer *buffer_sol_operacion = recibir_buffer(conexion_kernel);
	t_tipoDeInstruccion sol_operacion = leer_buffer_enteroUint32(buffer_sol_operacion);
	int pid = leer_buffer_enteroUint32(buffer_sol_operacion);
	switch (sol_operacion)
	{
	case IO_FS_CREATE:
		log_info(logger, "PID: <%d> - Operacion: <IO_FS_CREATE>", pid);
		log_info(logger, "PID: <%d> - Crear Archivo: <%s>", pid, "NOMBRE ARCHIVO");
		break;
	case IO_FS_DELETE:
		log_info(logger, "PID: <%d> - Operacion: <IO_FS_DELETE>", pid);
		log_info(logger, "PID: <%d> - Eliminar Archivo: <%s>", pid, "NOMBRE ARCHIVO");
		break;
	case IO_FS_TRUNCATE:
		log_info(logger, "PID: <%d> - Operacion: <IO_FS_TRUNCATE>", pid);
		log_info(logger, "PID: <%d> - Truncar Archivo: <%s> - Tamaño: <%s>", pid, "NOMBRE ARCHIVO", "TAMAÑO");
		break;
	case IO_FS_READ:
		log_info(logger, "PID: <%d> - Operacion: <IO_FS_READ>", pid);
		log_info(logger, "PID: <%d> - Leer Archivo: <%s> - Tamaño a Leer: <%s> - Puntero Archivo: <%s>", pid, "NOMBRE ARCHIVO", "TAMAÑO", "PUNTERO_ARCHIVO");
		break;
	case IO_FS_WRITE:
		log_info(logger, "PID: <%d> - Operacion: <IO_FS_WRITE>", pid);
		log_info(logger, "PID: <%d> - Escribir Archivo: <%s> - Tamaño a Escribir: <%s> - Puntero Archivo: <%s>", pid, "NOMBRE ARCHIVO", "TAMAÑO", "PUNTERO_ARCHIVO");
		break;
	default:
		break;
	}
}

t_list *convertir_a_numeros(char *texto)
{
	int i;
	t_list *lista = list_create();
	for (i = 0; texto[i] != '\0'; i++)
	{
		int numero = (int)texto[i];
		list_add(lista, &numero);
	}
	return lista;
}

void truncar_valor(char **valor_nuevo, char *valor_viejo, int limitante)
{
	*valor_nuevo = malloc(limitante * sizeof(char));

	if (*valor_nuevo == NULL)
	{
		return;
	}

	for (int i = 0; i < limitante; i++)
	{
		(*valor_nuevo)[i] = valor_viejo[i];
	}
	(*valor_nuevo)[limitante] = '\0';
	if (valor_viejo != NULL && valor_viejo[0] != '\0')
	{
		free(valor_viejo);
	}
}

void int_a_char_y_concatenar_a_string(int valor, char *cadena)
{
	char caracter = (char)valor;
	int longitud_cadena = strlen(cadena);
	cadena[longitud_cadena] = caracter;
	cadena[longitud_cadena + 1] = '\0';
}

t_list *truncar_y_convertir(char *texto_ingresado, int pid, int tamanio)
{
	char *nuevo_texto;
	truncar_valor(&nuevo_texto, texto_ingresado, tamanio);
	log_info(logger, "PID: <%d> - Texto Truncado: '%s'", pid, nuevo_texto);
	return convertir_a_numeros(nuevo_texto);
}

void enviar_buffer_stdin_memoria(int direccion_fisica, int pid, int cant_enteros, t_list *lista_enteros)
{
	tipo_buffer *buffer_stdin_memoria = crear_buffer();
	agregar_buffer_para_enterosUint32(buffer_stdin_memoria, direccion_fisica);
	agregar_buffer_para_enterosUint32(buffer_stdin_memoria, pid);
	agregar_buffer_para_enterosUint32(buffer_stdin_memoria, cant_enteros);
	for (int i = 0; i < cant_enteros; i++)
	{
		int *valor = list_get(lista_enteros, i);
		agregar_buffer_para_enterosUint32(buffer_stdin_memoria, *valor);
	}
	enviar_buffer(buffer_stdin_memoria, conexion_memoria);
	destruir_buffer(buffer_stdin_memoria);
}