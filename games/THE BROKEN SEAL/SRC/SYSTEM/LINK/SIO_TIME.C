#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

extern struct SerialRuntime gSerialRuntime;

/* link/serial/enable_transfer_timer.c */
void SerialRuntime_EnableTransferTimer(void)
{
    s32 state = (u32)&gSerialRuntime;
    if (FIELD_AT_OFFSET((void *)state, u8 *, 0) != 0)
        FIELD_AT_OFFSET((void *)state, s8 *, 8) = 1;
}

extern struct SerialRuntime gSerialRuntime;

void SerialRuntime_DisableTransfer(void)
{
    s32 base;

    *(volatile u16 *)0x04000208 = 0;
    *(u16 *)0x04000200 = *(u16 *)0x04000200 & 0xff3f;
    *(volatile u16 *)0x04000208 = 1;
    *(u16 *)0x04000128 = 0x2003;
    *(u32 *)0x0400010c = 0x0000c963;
    *(u16 *)0x04000202 = 0xc0;
    base = (u32)&gSerialRuntime;
    *((u8 *)base + 8) = 0;
}
