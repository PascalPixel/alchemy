#include "serial_runtime_family.h"

void Func_0800651c(void)
{
    volatile u16 *interrupt_master;
    u32 saved_interrupt_master;
    u32 zero;

    do {
        interrupt_master = &Data_04000208;
        zero = 0;
    } while (0);
    do {
        saved_interrupt_master = *interrupt_master;
        *interrupt_master = (u16)interrupt_master;
        do {
            Data_02002220.status = 0x80;
            Data_02002080 = zero;
            Data_02002008 = zero;
            Data_020023ac = zero;
            Data_02002220.peer_flags = zero;
            Data_02002220.flags = zero;
            Data_02002238 = zero;
        } while (0);
        *interrupt_master = saved_interrupt_master;
    } while (0);
}
