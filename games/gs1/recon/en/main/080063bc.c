#include "serial_runtime_family.h"

s32 Func_080063bc(s32 value, s32 transfer_value)
{
    struct SerialTransferState *state;
    u32 saved_interrupt_master;
    s32 result;
    if (SERIAL_ACTIVE_A != 0)
        return -1;

    state = SERIAL_TRANSFER;
    do {
        saved_interrupt_master = Data_04000208;
        Data_04000208 = (u16)&Data_04000208;
        do {
            state->status = 0x80;
            SERIAL_VALUE_A = transfer_value;
            SERIAL_RESULT = 0;
            SERIAL_ACTIVE_A = value;
            state->active = 1;
        } while (0);
        Data_04000208 = saved_interrupt_master;
        result = 0;
    } while (0);
    return result;
}
