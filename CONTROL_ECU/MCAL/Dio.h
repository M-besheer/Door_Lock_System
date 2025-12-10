#ifndef DIO_H
#define DIO_H

#include "std_types.h"

/* ============================================================= */
/* PIN INDEX DEFINITIONS                       */
/* ============================================================= */
#define PIN_0    0
#define PIN_1    1
#define PIN_2    2
#define PIN_3    3
#define PIN_4    4
#define PIN_5    5
#define PIN_6    6
#define PIN_7    7

/* ============================================================= */
/* PORT INDEX DEFINITIONS                      */
/* ============================================================= */
#define PORT_A   0
#define PORT_B   1
#define PORT_C   2
#define PORT_D   3
#define PORT_E   4
#define PORT_F   5


/*
 * Direction Definitions
 * Used to set pin direction as input or output.
 */
#define INPUT       0
#define OUTPUT      1

/*
 * Pin Level Definitions
 * Used to set or read pin state (HIGH/LOW).
 */
#define LOW         0
#define HIGH        1

/*
 * Enable/Disable Options
 * Used for enabling/disabling pull-up/pull-down resistors.
 */
#define ENABLE      1
#define DISABLE     0

/* Function Prototypes */
void Dio_Init(u8 PortId, u8 PinId, u8 Direction);
void Dio_WriteChannel(u8 PortId, u8 PinId, u8 Level);
u8   Dio_ReadChannel(u8 PortId, u8 PinId);        // <--- NEW: Read Function
void Dio_FlipChannel(u8 PortId, u8 PinId);        // <--- OPTIONAL: Toggle
void Dio_SetPUR(u8 port, u8 pin, u8 enable);
void Dio_SetPDR(u8 port, u8 pin, u8 enable);
#endif