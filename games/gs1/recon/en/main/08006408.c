#include "serial_runtime_family.h"

s32 Func_08006408(s32 value)
{
    struct SerialTransferState *state;
    volatile s32 *active;
    u32 saved_interrupt_master;
    s32 current;
    s32 result;

    active = (volatile s32 *)0x020023ac;
    current = *active;
    state = SERIAL_TRANSFER;
    if (current == 0)
        goto start_transfer;
    result = -1;
    goto finish;
start_transfer:
    saved_interrupt_master = Data_04000208;
    Data_04000208 = (u16)&Data_04000208;
    do {
        state->status = 0x81;
        SERIAL_VALUE_B = current;
        state->active = 1;
        *active = value;
        SERIAL_RESULT = (u32)Data_00000000;
    } while (0);
    Data_04000208 = saved_interrupt_master;
    result = 0;
finish:
    return result;
}
