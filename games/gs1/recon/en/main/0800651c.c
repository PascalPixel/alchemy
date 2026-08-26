#include "serial_runtime_family.h"

void Func_0800651c(void)
{
    struct SerialTransferState *state;
    volatile u16 *interrupt_master;
    u32 saved_interrupt_master;
    u32 zero;

    do {
        interrupt_master = &Data_04000208;
        state = &Data_02002220;
        zero = 0;
    } while (0);
    do {
        saved_interrupt_master = *interrupt_master;
        *interrupt_master = (u16)interrupt_master;
        do {
            state->status = 0x80;
            Data_02002080 = zero;
            Data_02002008 = zero;
            Data_020023ac = zero;
            state->peer_flags = zero;
            state->flags = zero;
            Data_02002238 = zero;
        } while (0);
        *interrupt_master = saved_interrupt_master;
    } while (0);
}
