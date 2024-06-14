#include "../include/largoPlazo.h"

void *largo_plazo()
{
    while (1)
    {
        //sem_wait(b_reanudar_largo_plazo);

        sem_wait(binario_menu_lp); // Se bloquea esperando al menu

        sem_wait(GRADO_MULTIPROGRAMACION);
        t_pcb *proceso = malloc(sizeof(t_pcb));

        proceso = transicion_new_a_ready(); // lo saca de new y lo mete a ready

        log_info(logger, "PID: %d - Estado Anterior: NEW - Estado Actual: READY \n", proceso->cde->pid); // Log pedido de cambio de estado

        
        //sem_post(b_reanudar_largo_plazo);
    }
}





t_pcb *transicion_new_a_ready()
{
    t_pcb *proceso = sacar_procesos_cola(cola_new_global);
    agregar_a_estado(proceso, cola_ready_global);
    return proceso;
}

void *transicion_exit_largo_plazo()
{
    while (1)
    {
        sem_wait(b_largo_plazo_exit);

        t_pcb *proceso = sacar_procesos_cola(cola_exec_global);
        agregar_a_estado(proceso, cola_exit_global);

        sem_post(GRADO_MULTIPROGRAMACION);
        sem_post(b_exec_libre);

        log_info(logger, "Finaliza el proceso %d - Motivo:", proceso->cde->pid);
    }
}




