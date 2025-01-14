#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/temporal.h>
#include <semaphore.h>
#include <pthread.h>
#include <libgen.h>

#include "../../utils/include/utils.h"
#include "../../utils/include/registros.h"
#include "../../utils/include/conexiones.h"
#include "../../utils/include/serializacion.h"
#include "../../utils/include/instrucciones.h"

typedef t_list t_lista_recursos;

typedef struct
{
	char *nombreEstado;
	t_list *estado;
	pthread_mutex_t *mutex_estado;
	sem_t *contador;

} colaEstado;

typedef struct
{
	char *nombre;
	sem_t *instancias;
	colaEstado *cola_bloqueados;
} t_recurso;

typedef struct
{
	t_config *config;
	int puerto_escucha;
	char *ip_memoria;
	int puerto_memoria;
	char *ip_cpu;
	int puerto_cpu_dispatch;
	int puerto_cpu_interrupt;
	char *algoritmo_planificacion;
	int quantum;
	t_lista_recursos *recursos;
	int grado_multiprogramacion;

} config_kernel;

typedef struct
{
	t_cde *cde;
	t_tipoDeInstruccion instruccion;
	int unidades_trabajo;
	int pid;
	int tamanio_reg;
	int dir_fisica;
	int tamanio_marco;
	char *nombre_archivo;
	uint32_t marco;
	uint32_t tamanio;
	uint32_t direccion_fisica;
	uint32_t puntero_archivo;
} t_struct_io;

typedef struct
{
	int cliente_io;
	char *nombre_io;
	enum_interfaz tipo_IO;
	t_list *procesos_espera;	//va a tener t_struct_io
	sem_t* contador_espera;
} t_infoIO;

// VARIABLES
extern uint32_t PID_GLOBAL;
extern t_log *logger;
extern sem_t *GRADO_MULTIPROGRAMACION;
extern sem_t *sem_agregar_a_estado;

extern int QUANTUM;
extern pthread_t hiloQuantum;
extern t_cde *cde_interrumpido;
// Binarios
extern sem_t *sem_quantum;
extern sem_t *binario_menu_lp;
extern sem_t *b_largo_plazo_exit;
extern sem_t *b_exec_libre;
extern sem_t *b_transicion_exec_blocked;

extern sem_t *b_reanudar_largo_plazo;
extern sem_t *b_reanudar_corto_plazo;
extern sem_t *b_reanudar_exit_largo;
extern sem_t *b_reanudar_exec_blocked;
extern sem_t *b_reanudar_exec_ready;
extern sem_t *b_reanudar_blocked_ready;

extern sem_t *b_transicion_exec_ready;
extern sem_t *b_transicion_blocked_ready;
extern sem_t *b_detener_planificacion;

extern sem_t *manejo_grado;

extern sem_t *cant_procesos_en_new;

extern int habilitar_planificadores;
extern config_kernel *valores_config;
extern int socket_memoria;
extern int socket_cpu_dispatch;
extern sem_t *sem_finalizar_proceso;
extern int socket_cpu_interrupt;

// DECLARACION VARIABLES GLOBALES

// INTERRUPTORES
extern sem_t *b_switch_readys;
extern int interruptor_switch_readys;

extern sem_t *contador_readys;

extern colaEstado *cola_new_global;
extern colaEstado *cola_ready_global;
extern colaEstado *cola_ready_plus;
extern colaEstado *cola_exec_global;
extern colaEstado *cola_bloqueado_global;
extern colaEstado *cola_exit_global;

extern sem_t *bloquearReady;
extern sem_t *bloquearReadyPlus;

extern int valorSem;
extern int valor_grado_a_modificar;

extern int pid_a_desbloquear;
extern int vieneDeIO;



extern config_kernel *configuracion;
// FUNCIONES
void crear_hilos();
void proceso_estado();
void conexionAMemoria();
void iniciar_consola_interactiva();
void iniciar_consola_interactiva2();
void gestionar_peticiones_memoria();
void gestionar_peticiones_interfaces();
void planificar_ejecucion_procesos();
void inicializarEstados();
colaEstado *constructorColaEstado(char *nombre);
config_kernel *inicializar_config_kernel();

void agregar_a_estado(t_pcb *pcb, colaEstado *cola_estado);
t_pcb *sacar_procesos_cola(colaEstado *cola_estado, char *planificacion);
t_pcb *sacar_procesos_cola_basico(colaEstado *cola_estado, char *planificacion);

t_pcb *transicion_generica(colaEstado *colaEstadoInicio, colaEstado *colaEstadoFinal, char *planificacion);
void evaluar_planificacion(char *planificador);

void iniciar_semaforos();

// FUNCIONES DE LEVANTAR MODULOS
void levantar_CPU_Dispatch();
void levantar_CPU_Interrupt();
void *conectar_interfaces();
void iniciar_hilos();

// planificadores

void *largo_plazo();
void *transicion_exit_largo_plazo();
void *transicion_exec_ready();
void *corto_plazo();
void *hilo_quantum();
void *transicion_exec_blocked();
void *transicion_blocked_ready();
void iniciar_kernel();
void levantar_servidores();

bool existe_recurso(char *nombre_recurso);

void wait_instancia_recurso(t_recurso *recurso);
_Bool ya_tiene_instancias_del_recurso(t_recurso *recurso_proceso);

void signal_instancia_recurso(t_recurso *recurso);
void interfaz_no_conectada(int pid, t_infoIO *informacion_interfaz);
void eliminar_proceso(t_pcb *proceso);
t_pcb *buscar_pcb_en_colas(int pid);
t_pcb *buscarProceso(uint32_t pid);
void finalizar_proceso(uint32_t PID, motivoFinalizar motivo);
void interfaz_conectada();
// FUNCIONES DE ENTRADA/SALIDA

char *obtener_interfaz(enum_interfaz interfaz);
_Bool interfaz_esta_en_lista(void *ptr);
void recibir_orden_interfaces_de_cpu();
_Bool interfaz_no_esta_conectada(t_infoIO *informacion_interfaz);
t_cde *iniciar_cde(char *PATH);
void interfaz_conectada_stdin(t_tipoDeInstruccion instruccion_a_ejecutar, int tamanio_reg, int tamanio_marco, int dir_fisica, t_infoIO *io, int pid);
void interfaz_conectada_stdout(t_tipoDeInstruccion instruccion_a_ejecutar, int tamanio_reg, int dir_fisica, t_infoIO *io, int pid);
void interfaz_conectada_generica(int unidades_trabajo, t_tipoDeInstruccion instruccion_a_ejecutar, t_infoIO *io, int pid);
void atender_interrupciones();
char *buscar_recurso(char *recurso, int *posicion);
void fs_create_delete(char *nombre_archivo, t_tipoDeInstruccion instruccion_interfaz, t_infoIO *io, t_struct_io *informacion_buffer);
void fs_truncate(char *nombre_archivo, t_tipoDeInstruccion instruccion_interfaz, uint32_t tamanio, t_infoIO *io, t_struct_io *informacion_buffer);
void fs_write_read(char *nombre_archivo, t_tipoDeInstruccion instruccion_interfaz, uint32_t marco, uint32_t tamanio, uint32_t reg_direccion, uint32_t puntero_archivo, t_infoIO *io, t_struct_io *informacion_buffer);
int hayProcesosEnEstado(colaEstado *cola_estado);

void bloquearSiReadysVacios();
void desbloquearSiReadysVacios();

void waitInterruptor(int valor_interruptor, sem_t *interruptorSemaforo);

void signalInterruptor(int valor_interruptor, sem_t *interruptorSemaforo); // no se usa

int valorSemaforo(sem_t *semaforo);
void asignar_recurso(t_recurso *recurso, t_pcb *cde);
bool existe_recurso2(char *nombre_recurso);
void manejarGrado();
void *manejar_conexiones_io();
void *conectar_interfaz();
t_recurso *obtener_recurso(char *nombre_recurso);
t_pcb *sacar_procesos_criterio_cola(colaEstado *cola_estado, char *planificacion, bool (*condicion)(void*));




#endif