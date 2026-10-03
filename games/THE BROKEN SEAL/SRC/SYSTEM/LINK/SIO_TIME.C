#include "SERIAL_RUNTIME.H"

/* link/serial/enable_transfer_timer.c */
void SerialRuntime_EnableTransferTimer(void)
{
    struct SerialRuntime *state = &gSerialRuntime;

    if (state->mode != 0)
        state->transfer_enabled = 1;
}

void SerialRuntime_DisableTransfer(void)
{
    struct SerialRuntime *state;

    *(volatile u16 *)0x04000208 = 0;
    *(u16 *)0x04000200 = *(u16 *)0x04000200 & 0xff3f;
    *(volatile u16 *)0x04000208 = 1;
    *(u16 *)0x04000128 = 0x2003;
    *(u32 *)0x0400010c = 0x0000c963;
    *(u16 *)0x04000202 = 0xc0;
    state = &gSerialRuntime;
    state->transfer_enabled = 0;
}
