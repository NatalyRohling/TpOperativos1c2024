#include "../include/cicloinstruccion.h"

extern t_registros *registros;

// INSTRUCCIONES DE REGISTROS

void exec_set(char *registro, uint32_t valor)
{
    if (strcmp(registro, "AX") == 0)
    {
        registros->AX = valor;
    }
    else if (strcmp(registro, "BX") == 0)
    {
        registros->BX = valor;
    }
    else if (strcmp(registro, "CX") == 0)
    {
        registros->CX = valor;
    }
    else if (strcmp(registro, "DX") == 0)
    {
        registros->DX = valor;
    }
    else if (strcmp(registro, "EAX") == 0)
    {
        registros->EAX = valor;
    }
    else if (strcmp(registro, "EBX") == 0)
    {
        registros->EBX = valor;
    }
    else if (strcmp(registro, "ECX") == 0)
    {
        registros->ECX = valor;
    }
    else if (strcmp(registro, "EDX") == 0)
    {
        registros->EDX = valor;
    }
    else if (strcmp(registro, "PC") == 0)
    {
        cde_recibido->PC = valor;
    }
    else if (strcmp(registro, "DI") == 0)
    {
        registros->DI = valor;
    }
    else if (strcmp(registro, "SI") == 0)
    {
        registros->SI = valor;
    }
    else
    {
        log_error(logger, "No es un registro o el PC");
    }
}

void exec_sum(char *destino, char *origen)
{
    uint32_t valor_origen = obtener_valor(origen);
    if (strcmp(destino, "AX") == 0)
    {
        registros->AX = registros->AX + valor_origen;
    }
    if (strcmp(destino, "BX") == 0)
    {
        registros->BX = registros->BX + valor_origen;
    }
    if (strcmp(destino, "CX") == 0)
    {
        registros->CX = registros->CX + valor_origen;
    }
    if (strcmp(destino, "DX") == 0)
    {
        registros->DX = registros->DX + valor_origen;
    }
    if (strcmp(destino, "EAX") == 0)
    {
        registros->EAX = registros->EAX + valor_origen;
    }
    if (strcmp(destino, "EBX") == 0)
    {
        registros->EBX = registros->EBX + valor_origen;
    }
    if (strcmp(destino, "ECX") == 0)
    {
        registros->ECX = registros->ECX + valor_origen;
    }
    if (strcmp(destino, "EDX") == 0)
    {
        registros->EDX = registros->EDX + valor_origen;
    }
}

void exec_sub(char *destino, char *origen)
{
    uint32_t valor_origen = obtener_valor(origen);
    if (strcmp(destino, "AX") == 0)
    {
        registros->AX = registros->AX - valor_origen;
    }
    if (strcmp(destino, "BX") == 0)
    {
        registros->BX = registros->BX - valor_origen;
    }
    if (strcmp(destino, "CX") == 0)
    {
        registros->CX = registros->CX - valor_origen;
    }
    if (strcmp(destino, "DX") == 0)
    {
        registros->DX = registros->DX - valor_origen;
    }
    if (strcmp(destino, "EAX") == 0)
    {
        registros->EAX = registros->EAX - valor_origen;
    }
    if (strcmp(destino, "EBX") == 0)
    {
        registros->EBX = registros->EBX - valor_origen;
    }
    if (strcmp(destino, "ECX") == 0)
    {
        registros->ECX = registros->ECX - valor_origen;
    }
    if (strcmp(destino, "EDX") == 0)
    {
        registros->EDX = registros->EDX - valor_origen;
    }
}

void exec_jnz(char *registro, uint32_t numeroInstruccion, t_cde *cde)
{
    if (strcmp(registro, "AX") == 0)
    {
        if (registros->AX != 0)
        {
            registros->AX = numeroInstruccion;
        }
    }
    if (strcmp(registro, "BX") == 0)
    {
        if (registros->BX != 0)
        {
            registros->BX = numeroInstruccion;
        }
    }
    if (strcmp(registro, "CX") == 0)
    {
        if (registros->CX != 0)
        {
            registros->CX = numeroInstruccion;
        }
    }
    if (strcmp(registro, "DX") == 0)
    {
        if (registros->DX != 0)
        {
            registros->DX = numeroInstruccion;
        }
    }
    if (strcmp(registro, "EAX") == 0)
    {
        if (registros->EAX != 0)
        {
            registros->EAX = numeroInstruccion;
        }
    }
    if (strcmp(registro, "EBX") == 0)
    {
        if (registros->EBX != 0)
        {
            registros->EBX = numeroInstruccion;
        }
    }
    if (strcmp(registro, "ECX") == 0)
    {
        if (registros->ECX != 0)
        {
            registros->ECX = numeroInstruccion;
        }
    }
    if (strcmp(registro, "EDX") == 0)
    {
        if (registros->EDX != 0)
        {
            registros->EDX = numeroInstruccion;
        }
    }
}

// INSTRUCCIONES DE MEMORIA

void exec_mov_in(char *datos, char *direccion, t_cde *cde)
{
    uint32_t direccion_logica = obtener_valor(direccion);
    uint32_t direccion_fisica = traducir_direccion_mmu(direccion_logica);
    enviar_op_code(socket_memoria, ACCESO_ESPACIO_USUARIO);
    enviar_op_code(socket_memoria, PEDIDO_LECTURA);

    uint8_t valor8;
    uint32_t valor32;
    size_t int_tamanio = 1;
    tipo_buffer *buffer = crear_buffer();
    agregar_buffer_para_enterosUint32(buffer, direccion_fisica);
    agregar_buffer_para_enterosUint32(buffer, cde->pid);
    agregar_buffer_para_enterosUint32(buffer, int_tamanio);
    agregar_buffer_para_enterosUint32(buffer, INTEGER);
    enviar_buffer(buffer, socket_memoria);

    op_code lectura_memoria = recibir_op_code(socket_memoria);
    if (lectura_memoria == OK)
    {
        tipo_buffer *buffer_valor = recibir_buffer(socket_memoria);
        uint32_t valor = leer_buffer_enteroUint8(buffer_valor);
        exec_set(datos, valor);
        destruir_buffer(buffer_valor);
        // log obligatorio
        log_info(logger, "PID: <%d> - Accion: <LEER> - Direccion Fisica: <%d> - Valor: <%d>", cde->pid, direccion_fisica, valor);
    }
    else
    {
        log_error(logger, "DIRECCION INCORRECTA");
    }

    destruir_buffer(buffer);
}

void exec_mov_out(char *direccion, char *datos, t_cde *cde)
{
    void *reg_valor = obtener_valor(datos);
    uint32_t direccion_logica = obtener_valor(direccion);
    uint32_t direccion_fisica = escribir_dato_memoria(direccion_logica, &reg_valor, 1, cde->pid);

    if (direccion_fisica != 1)
    {
        // log obligatorio
        log_info(logger, "PID: <%d> - Accion: <ESCRIBIR> - Direccion Fisica: <%d> - Valor: <%d>", cde->pid, direccion_fisica, reg_valor);
    }
}

void exec_resize(char *tamanio, t_cde *cde)
{
    enviar_op_code(socket_memoria, RESIZE_OP);
    uint32_t tamanioValor = atoi(tamanio);
    tipo_buffer *buffer = crear_buffer();

    agregar_buffer_para_enterosUint32(buffer, tamanioValor);
    agregar_cde_buffer(buffer, cde);

    enviar_buffer(buffer, socket_memoria);
    destruir_buffer(buffer);
    op_code resize_memoria = recibir_op_code(socket_memoria);
    if (resize_memoria != RESIZE_EXITOSO)
    {
        salida_exit = 0;
        enviar_op_code(socket_kernel_dispatch, FINALIZAR_PROCESO);
        tipo_buffer *buffer_out_memory = crear_buffer();
        agregar_cde_buffer(buffer_out_memory, cde);
        agregar_buffer_para_enterosUint32(buffer_out_memory, OUT_OF_MEMORY_END);
        enviar_buffer(buffer_out_memory, socket_kernel_dispatch);
        destruir_buffer(buffer_out_memory);
    }
}

void exec_copy_string(char *tamanio, t_cde *cde)
{
    // LEER STRING

    uint32_t direccion_logica_SI = registros->SI;
    uint32_t direccion_fisica_SI = traducir_direccion_mmu(direccion_logica_SI);

    enviar_op_code(socket_memoria, ACCESO_ESPACIO_USUARIO);
    enviar_op_code(socket_memoria, PEDIDO_LECTURA);

    tipo_buffer *buffer_copy_string = crear_buffer();
    agregar_buffer_para_enterosUint32(buffer_copy_string, direccion_fisica_SI);
    agregar_buffer_para_enterosUint32(buffer_copy_string, cde->pid);
    agregar_buffer_para_enterosUint32(buffer_copy_string, atoi(tamanio));
    agregar_buffer_para_enterosUint32(buffer_copy_string, STRING);

    enviar_buffer(buffer_copy_string, socket_memoria);
    destruir_buffer(buffer_copy_string);
    char *valor = (char *)calloc(1, atoi(tamanio));
    op_code lectura_memoria = recibir_op_code(socket_memoria);
    if (lectura_memoria == OK)
    {
        tipo_buffer *buffer_valor = recibir_buffer(socket_memoria);
        valor = leer_buffer_string(buffer_valor);
        destruir_buffer(buffer_valor);
        // log obligatorio
        log_info(logger, "PID: <%d> - Accion: <LEER> - Direccion Fisica: <%d> - Valor: <%s>", cde->pid, direccion_fisica_SI, valor);
    }
    else
    {
        log_error(logger, "DIRECCION INCORRECTA");
    }

    uint32_t tamanio_string = atoi(tamanio);
    uint32_t direccion_logica_DI = registros->DI;
    uint32_t direccion_fisica = escribir_dato_memoria(direccion_logica_DI, valor, tamanio_string, cde->pid);

    if (direccion_fisica != 1)
    {

        // log obligatorio
        log_info(logger, "PID: <%d> - Accion: <ESCRIBIR> - Direccion Fisica: <%d> - Valor: <%s>", cde->pid, direccion_fisica, valor);
    }
    free(valor);
}

// INSTRUCCIONES MANEJO DE RECURSOS

void exec_wait(char *recurso, t_cde *cde)
{
    enviar_op_code(socket_kernel_dispatch, WAIT_RECURSO);

    tipo_buffer *buffer_recurso;
    buffer_recurso = crear_buffer();
    desalojo_wait = 1;
    agregar_buffer_para_string(buffer_recurso, recurso);
    enviar_buffer(buffer_recurso, socket_kernel_dispatch);

    destruir_buffer(buffer_recurso);
}

void exec_signal(char *recurso, t_cde *cde)
{
    enviar_op_code(socket_kernel_dispatch, SIGNAL_RECURSO);

    tipo_buffer *buffer_recurso;
    buffer_recurso = crear_buffer();
    desalojo_signal = 1;
    agregar_buffer_para_string(buffer_recurso, recurso);
    enviar_buffer(buffer_recurso, socket_kernel_dispatch);
    destruir_buffer(buffer_recurso);
}

// INSTRUCCIONES DE ENTRADA/SALIDA

void exec_io_gen_sleep(char *nombre_interfaz, uint32_t unidades_trabajo)
{
    interrupcion_io = 1;
    tipo_buffer *buffer_instruccion_io = crear_buffer();
    enviar_op_code(socket_kernel_dispatch, INSTRUCCION_INTERFAZ);

    agregar_buffer_para_enterosUint32(buffer_instruccion_io, SOLICITUD_INTERFAZ_GENERICA);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, IO_GEN_SLEEP);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, unidades_trabajo);
    agregar_buffer_para_string(buffer_instruccion_io, nombre_interfaz);
    enviar_buffer(buffer_instruccion_io, socket_kernel_dispatch);
    destruir_buffer(buffer_instruccion_io);
}

void exec_io_stdin_read(char *interfaz, char *reg_direccion, char *reg_tamanio, t_cde *cde)
{
    interrupcion_io = 1;
    uint32_t direccion_logica = obtener_valor(reg_direccion);
    uint32_t direccion_fisica = traducir_direccion_mmu(direccion_logica);
    uint32_t tamanio_registro = obtener_valor(reg_tamanio);

    tipo_buffer *buffer_instruccion_io = crear_buffer();
    enviar_op_code(socket_kernel_dispatch, INSTRUCCION_INTERFAZ);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, SOLICITUD_INTERFAZ_STDIN);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, IO_STDIN_READ);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, tamanio_registro);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, tamanio_pagina);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, direccion_fisica);

    agregar_buffer_para_string(buffer_instruccion_io, interfaz);
    enviar_buffer(buffer_instruccion_io, socket_kernel_dispatch);
    destruir_buffer(buffer_instruccion_io);
}

void exec_io_stdout_write(char *interfaz, char *reg_direccion, char *reg_tamanio, t_cde *cde)
{
    interrupcion_io = 1;
    uint32_t direccion_logica = obtener_valor(reg_direccion);
    uint32_t direccion_fisica = traducir_direccion_mmu(direccion_logica);
    uint32_t tamanio = obtener_valor(reg_tamanio);

    tipo_buffer *buffer_instruccion_io = crear_buffer();

    enviar_op_code(socket_kernel_dispatch, INSTRUCCION_INTERFAZ);

    agregar_buffer_para_enterosUint32(buffer_instruccion_io, SOLICITUD_INTERFAZ_STDOUT);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, IO_STDOUT_WRITE);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, tamanio);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, direccion_fisica);
    agregar_buffer_para_string(buffer_instruccion_io, interfaz);
    enviar_buffer(buffer_instruccion_io, socket_kernel_dispatch);
    destruir_buffer(buffer_instruccion_io);
}

// INSTRUCCIONES DE FILE SYSTEM

void exec_io_fs_create(char *nombre_interfaz, char *nombre_archivo, t_cde *cde)
{
    interrupcion_fs = 1;
    nombre_archivo_a_enviar = nombre_archivo;
    tipo_buffer *buffer_instruccion_io = crear_buffer();
    enviar_op_code(socket_kernel_dispatch, INSTRUCCION_INTERFAZ);

    agregar_buffer_para_enterosUint32(buffer_instruccion_io, SOLICITUD_INTERFAZ_DIALFS); // enviar como buffer para que no haya problemas de sincronizacion
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, IO_FS_CREATE);
    agregar_buffer_para_string(buffer_instruccion_io, nombre_interfaz);
    enviar_buffer(buffer_instruccion_io, socket_kernel_dispatch);
    destruir_buffer(buffer_instruccion_io);
}

void exec_io_fs_delete(char *nombre_interfaz, char *nombre_archivo, t_cde *cde)
{
    interrupcion_fs = 1;
    nombre_archivo_a_enviar = nombre_archivo;
    tipo_buffer *buffer_instruccion_io = crear_buffer();
    enviar_op_code(socket_kernel_dispatch, INSTRUCCION_INTERFAZ);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, SOLICITUD_INTERFAZ_DIALFS);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, IO_FS_DELETE);
    agregar_buffer_para_string(buffer_instruccion_io, nombre_interfaz);
    enviar_buffer(buffer_instruccion_io, socket_kernel_dispatch);
    destruir_buffer(buffer_instruccion_io);
}

void exec_io_fs_truncate(char *nombre_interfaz, char *nombre_archivo, char *reg_tamanio, t_cde *cde)
{
    interrupcion_fs = 1;
    nombre_archivo_a_enviar = nombre_archivo;
    tipo_buffer *buffer_instruccion_io = crear_buffer();
    enviar_op_code(socket_kernel_dispatch, INSTRUCCION_INTERFAZ);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, SOLICITUD_INTERFAZ_DIALFS);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, IO_FS_TRUNCATE);
    uint32_t tamanio = obtener_valor(reg_tamanio);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, tamanio);
    agregar_buffer_para_string(buffer_instruccion_io, nombre_interfaz);
    enviar_buffer(buffer_instruccion_io, socket_kernel_dispatch);
    destruir_buffer(buffer_instruccion_io);
}

void exec_io_fs_write(char *nombre_interfaz, char *nombre_archivo, char *reg_direccion, char *reg_tamanio, char *puntero_archivo, t_cde *cde)
{
    interrupcion_fs = 1;
    nombre_archivo_a_enviar = nombre_archivo;
    uint32_t direccion_logica = obtener_valor(reg_direccion);
    uint32_t direccion_fisica = traducir_direccion_mmu(direccion_logica);
    tipo_buffer *buffer_instruccion_io = crear_buffer();
    enviar_op_code(socket_kernel_dispatch, INSTRUCCION_INTERFAZ);

    agregar_buffer_para_enterosUint32(buffer_instruccion_io, SOLICITUD_INTERFAZ_DIALFS);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, IO_FS_WRITE);

    uint32_t tamanio = obtener_valor(reg_tamanio);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, tamanio);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, direccion_fisica);
    uint32_t int_puntero = obtener_valor(puntero_archivo);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, int_puntero);
    agregar_buffer_para_string(buffer_instruccion_io, nombre_interfaz);
    enviar_buffer(buffer_instruccion_io, socket_kernel_dispatch);
    destruir_buffer(buffer_instruccion_io);
}

void exec_io_fs_read(char *nombre_interfaz, char *nombre_archivo, char *reg_direccion, char *reg_tamanio, char *puntero_archivo, t_cde *cde)
{
    interrupcion_fs = 1;
    nombre_archivo_a_enviar = nombre_archivo;
    uint32_t direccion_logica = obtener_valor(reg_direccion);
    uint32_t direccion_fisica = traducir_direccion_mmu(direccion_logica);
    tipo_buffer *buffer_instruccion_io = crear_buffer();
    enviar_op_code(socket_kernel_dispatch, INSTRUCCION_INTERFAZ);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, SOLICITUD_INTERFAZ_DIALFS);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, IO_FS_READ);
    uint32_t tamanio = obtener_valor(reg_tamanio);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, tamanio_pagina);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, tamanio);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, direccion_fisica);
    uint32_t puntero = obtener_valor(puntero_archivo);
    agregar_buffer_para_enterosUint32(buffer_instruccion_io, puntero);
    agregar_buffer_para_string(buffer_instruccion_io, nombre_interfaz);
    enviar_buffer(buffer_instruccion_io, socket_kernel_dispatch);
    destruir_buffer(buffer_instruccion_io);
}

// INSTRUCCION EXIT

void exec_exit(t_cde *cde, motivoFinalizar motivo)
{
    salida_exit = 0;
    enviar_op_code(socket_kernel_dispatch, FINALIZAR_PROCESO);
    tipo_buffer *buffer = crear_buffer();
    agregar_cde_buffer(buffer, cde);
    agregar_buffer_para_enterosUint32(buffer, motivo);
    enviar_buffer(buffer, socket_kernel_dispatch);
    destruir_buffer(buffer);
}

// FUNCIONES AUXILIARES

void *obtener_valor(char *origen)
{
    if (strcmp(origen, "AX") == 0)
    {
        return registros->AX;
    }
    else if (strcmp(origen, "BX") == 0)
    {
        return registros->BX;
    }
    else if (strcmp(origen, "CX") == 0)
    {
        return registros->CX;
    }
    else if (strcmp(origen, "DX") == 0)
    {
        return registros->DX;
    }
    else if (strcmp(origen, "SI") == 0)
    {
        return registros->SI;
    }
    else if (strcmp(origen, "DI") == 0)
    {
        return registros->DI;
    }
    else if (strcmp(origen, "EAX") == 0)
    {
        return registros->EAX;
    }
    else if (strcmp(origen, "EBX") == 0)
    {
        return registros->EBX;
    }
    else if (strcmp(origen, "ECX") == 0)
    {
        return registros->ECX;
    }
    else if (strcmp(origen, "EDX") == 0)
    {
        return registros->EDX;
    }
    return NULL;
}

uint32_t leer_dato_memoria(uint32_t direccion_logica, int size, int pid)
{
    uint32_t dl_del_byte_inicial_dato = direccion_logica;
    uint32_t dl_del_byte_final_dato = direccion_logica + size - 1;
    int pagina_del_byte_inicial = calcular_pagina(direccion_logica);
    int pagina_del_byte_final = calcular_pagina(dl_del_byte_final_dato);
    uint32_t direccion_fisica;

    if (pagina_del_byte_inicial == pagina_del_byte_final)
    {
        // Si el dato cabe en una sola página, se lee directamente.
        direccion_fisica = traducir_direccion_mmu(direccion_logica);
        if (direccion_fisica == -1)
        {
            log_error(logger, "DIRECCION INCORRECTA");
            // interrupcion_exit = 1;
            return -1;
        }
        return leer_memoria(direccion_fisica, size, pid);
    }
    else
    {
        // Si el dato cruza el límite de la página, se divide la lectura.
        uint32_t dl_byte_final_pagina = (pagina_del_byte_inicial + 1) * tamanio_pagina - 1;
        int cant_bytes = dl_byte_final_pagina - dl_del_byte_inicial_dato + 1;

        // Leer la primera parte del dato que cabe en la página actual.
        direccion_fisica = traducir_direccion_mmu(dl_del_byte_inicial_dato);
        if (direccion_fisica == -1)
        {
            log_error(logger, "DIRECCION INCORRECTA");
            // interrupcion_exit = 1;
            return -1;
        }
        uint32_t valor = leer_memoria(direccion_fisica, cant_bytes, pid);
        // Llamada recursiva para leer el resto del dato en la siguiente página.
        // Nota: se usar (char*) por una cuestion aritmetica de punteros, para sumar de a bytes
        return valor + leer_dato_memoria(dl_byte_final_pagina + 1, size - cant_bytes, pid); // Ejemplo de como va leyendo: 0-1023 | 1024-2047 | 2048-3071
    }
}

uint32_t leer_memoria(uint32_t direccion_fisica, int size, int pid)
{

    enviar_op_code(socket_memoria, ACCESO_ESPACIO_USUARIO);
    enviar_op_code(socket_memoria, PEDIDO_LECTURA);
    tipo_buffer *buffer = crear_buffer();
    agregar_buffer_para_enterosUint32(buffer, direccion_fisica);
    agregar_buffer_para_enterosUint32(buffer, pid);
    agregar_buffer_para_enterosUint32(buffer, size);
    agregar_buffer_para_enterosUint32(buffer, INTEGER);
    enviar_buffer(buffer, socket_memoria);
    op_code lectura_memoria = recibir_op_code(socket_memoria);
    uint32_t valor;
    if (lectura_memoria == OK)
    {
        tipo_buffer *buffer_valor = recibir_buffer(socket_memoria);
        valor = leer_buffer_enteroUint32(buffer_valor);
        destruir_buffer(buffer_valor);
        log_info(logger, "PID: <%d> - Accion: <LEER> - Direccion Fisica: <%d> - Valor: <%d>", pid, direccion_fisica, valor);
        return valor;
    }
    else
    {
        log_error(logger, "DIRECCION INCORRECTA");
        return -1;
    }
}

// FUNCIONES PARA ESCRIBIR

uint32_t escribir_dato_memoria(uint32_t direccion_logica, void *dato, int size, int pid)
{
    uint32_t dl_del_byte_inicial_dato = direccion_logica;
    uint32_t dl_del_byte_final_dato = direccion_logica + size - 1;
    int pagina_del_byte_inicial = calcular_pagina(direccion_logica);
    int pagina_del_byte_final = calcular_pagina(dl_del_byte_final_dato);
    uint32_t direccion_fisica;
    if (pagina_del_byte_inicial == pagina_del_byte_final)
    {
        // Si el dato cabe en una sola página, se escribe directamente.
        direccion_fisica = traducir_direccion_mmu(direccion_logica);
        if (direccion_fisica == -1)
        {
            log_error(logger, "DIRECCION INCORRECTA");
            // interrupcion_exit = 1;
            return -1;
        }
        escribir_memoria(socket_memoria, direccion_fisica, size, dato, pid);
    }
    else
    {
        // Si el dato cruza el límite de la página, se divide la escritura.
        uint32_t dl_byte_final_pagina = (pagina_del_byte_inicial + 1) * tamanio_pagina - 1;
        int cant_bytes = dl_byte_final_pagina - dl_del_byte_inicial_dato + 1;

        // Escribir la primera parte del dato que cabe en la página actual.
        direccion_fisica = traducir_direccion_mmu(dl_del_byte_inicial_dato);
        if (direccion_fisica == -1)
        {
            log_error(logger, "DIRECCION INCORRECTA");
            // interrupcion_exit = 1;
            return -1;
        }
        escribir_memoria(socket_memoria, direccion_fisica, cant_bytes, dato, pid);
        // Llamada recursiva para escribir el resto del dato en la siguiente página.
        // Nota: se usar (char*) por una cuestion aritmetica de punteros, para sumar de a bytes
        escribir_dato_memoria(dl_byte_final_pagina + 1, dato + cant_bytes, size - cant_bytes, pid);
    }

    return direccion_fisica;
}