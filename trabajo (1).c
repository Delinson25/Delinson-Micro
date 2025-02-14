#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // Para Sleep()

// Definición de estados
#define ESTADO_INICIAL 0
#define ESTADO_ERROR 1
#define ESTADO_ABRIENDO 2
#define ESTADO_CERRANDO 3
#define ESTADO_ABIERTO 4
#define ESTADO_CERRADO 5
#define ESTADO_DETENIDO 6
#define ESTADO_DESCONOCIDO 7

// Definición de sensores y actuadores
#define LM_ACTIVO 1
#define LM_NOACTIVO 0
#define MOTOR_OFF 0
#define MOTOR_ON 1
#define LAMP_OFF 0
#define LAMP_ON 1

// Variables de estado
int ESTADO_SIGUIENTE = ESTADO_INICIAL;
int ESTADO_ANTERIOR = ESTADO_INICIAL;
int ESTADO_ACTUAL = ESTADO_INICIAL;

// Estructura de entradas/salidas
struct SYSTEM_IO
{
    unsigned int lsc : 1;  // Sensor de puerta cerrada
    unsigned int lsa : 1;  // Sensor de puerta abierta
    unsigned int ma : 1;   // Motor abriendo
    unsigned int mc : 1;   // Motor cerrando
    unsigned int lamp : 1; // Lámpara indicadora
    unsigned int keya : 1; // Botón abrir
    unsigned int keyc : 1; // Botón cerrar
} io;

// Prototipos de funciones
int Func_ESTADO_INICIAL(void);
int Func_ESTADO_ERROR(void);
int Func_ESTADO_ABRIENDO(void);
int Func_ESTADO_ABIERTO(void);
int Func_ESTADO_CERRANDO(void);
int Func_ESTADO_CERRADO(void);
int Func_ESTADO_DETENIDO(void);
int Func_ESTADO_DESCONOCIDO(void);

// Simulación de cambio de sensores
void simular_sensores()
{
    static int counter = 0;
    counter++;

    if (counter == 3)
    {
        io.keya = 1;
    } // Simula que alguien presiona "abrir" en el tercer ciclo
    if (counter == 8)
    {
        io.lsa = 1;
        io.ma = MOTOR_OFF;
    } // Simula que la puerta se abre completamente
    if (counter == 10)
    {
        io.keyc = 1;
    } // Simula que alguien presiona "cerrar"
    if (counter == 15)
    {
        io.lsc = 1;
        io.mc = MOTOR_OFF;
    } // Simula que la puerta se cierra completamente
}

int main()
{
    for (;;)
    {
        simular_sensores(); // Simula cambios en sensores

        switch (ESTADO_SIGUIENTE)
        {
        case ESTADO_INICIAL:
            ESTADO_SIGUIENTE = Func_ESTADO_INICIAL();
            break;
        case ESTADO_ERROR:
            ESTADO_SIGUIENTE = Func_ESTADO_ERROR();
            break;
        case ESTADO_ABRIENDO:
            ESTADO_SIGUIENTE = Func_ESTADO_ABRIENDO();
            break;
        case ESTADO_ABIERTO:
            ESTADO_SIGUIENTE = Func_ESTADO_ABIERTO();
            break;
        case ESTADO_CERRANDO:
            ESTADO_SIGUIENTE = Func_ESTADO_CERRANDO();
            break;
        case ESTADO_CERRADO:
            ESTADO_SIGUIENTE = Func_ESTADO_CERRADO();
            break;
        case ESTADO_DETENIDO:
            ESTADO_SIGUIENTE = Func_ESTADO_DETENIDO();
            break;
        case ESTADO_DESCONOCIDO:
            ESTADO_SIGUIENTE = Func_ESTADO_DESCONOCIDO();
            break;
        }

        Sleep(500); // Pausa de 0.5 segundos para simular tiempo real (reemplaza usleep)
    }
    return 0;
}

// Implementación de funciones para cada estado
int Func_ESTADO_INICIAL(void)
{
    printf("Estado: INICIAL\n");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_INICIAL;
    io.ma = MOTOR_OFF;
    io.mc = MOTOR_OFF;
    io.lamp = LAMP_OFF;

    if (io.lsa && io.lsc)
        return ESTADO_ERROR;
    if (io.lsa && !io.lsc)
        return ESTADO_ABIERTO;
    if (!io.lsa && !io.lsc)
        return ESTADO_DESCONOCIDO;
    return ESTADO_INICIAL;
}

int Func_ESTADO_ERROR(void)
{
    printf("Estado: ERROR\n");
    io.lamp = LAMP_ON;
    return ESTADO_INICIAL; // Reinicia el sistema
}

int Func_ESTADO_ABRIENDO(void)
{
    printf("Estado: ABRIENDO\n");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ABRIENDO;
    io.ma = MOTOR_ON;
    io.mc = MOTOR_OFF;

    if (io.lsa)
        return ESTADO_ABIERTO;
    return ESTADO_ABRIENDO;
}

int Func_ESTADO_ABIERTO(void)
{
    printf("Estado: ABIERTO\n");
    io.ma = MOTOR_OFF;
    io.mc = MOTOR_OFF;

    if (io.keyc)
        return ESTADO_CERRANDO;
    return ESTADO_ABIERTO;
}

int Func_ESTADO_CERRANDO(void)
{
    printf("Estado: CERRANDO\n");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_CERRANDO;
    io.ma = MOTOR_OFF;
    io.mc = MOTOR_ON;

    if (io.lsc)
        return ESTADO_CERRADO;
    return ESTADO_CERRANDO;
}

int Func_ESTADO_CERRADO(void)
{
    printf("Estado: CERRADO\n");
    io.ma = MOTOR_OFF;
    io.mc = MOTOR_OFF;

    if (io.keya)
        return ESTADO_ABRIENDO;
    return ESTADO_CERRADO;
}

int Func_ESTADO_DETENIDO(void)
{
    printf("Estado: DETENIDO\n");
    io.ma = MOTOR_OFF;
    io.mc = MOTOR_OFF;
    return ESTADO_DETENIDO;
}

int Func_ESTADO_DESCONOCIDO(void)
{
    printf("Estado: DESCONOCIDO\n");
    io.lamp = LAMP_ON;
    return ESTADO_INICIAL;
}
