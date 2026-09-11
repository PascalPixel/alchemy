#include "types.h"
#include "scene.h"
#include "abi/link/serial/remove_serial_irq_handlers.h"
#include "global_cells.h"

#ifndef SERIAL_RUNTIME_TU
typedef void (*InterruptHandler)(void);

#endif

void SerialRuntime_RemoveIrqHandlers(void)
{
    s16 *work;
    s32 handler;

    work = (s16 *)ADDR_03001CB0;
    do {
        do {
        } while (0);
        *work = 0;
        Sys_Place(7, 0, (InterruptHandler)(handler = 0));
    } while (0);
    handler = 6;
    Sys_Place(handler, 0, 0);
}
