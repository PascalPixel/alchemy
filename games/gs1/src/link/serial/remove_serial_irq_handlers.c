#include "types.h"
#include "global_cells.h"

typedef void (*InterruptHandler)(void);

void Func_0800307c(s32, s32, InterruptHandler);

#define SerialRuntime_RemoveIrqHandlers Func_08006358

void SerialRuntime_RemoveIrqHandlers(void)
{
    s16 *work;
    s32 handler;

    work = (s16 *)ADDR_03001CB0;
    do {
        do {
        } while (0);
        *work = 0;
        Func_0800307c(7, 0, (InterruptHandler)(handler = 0));
    } while (0);
    handler = 6;
    Func_0800307c(handler, 0, 0);
}
