#include "serial_runtime.h"
#include "io_reg.h"

void SerialRuntime_EnableTransferTimer(void)
{
    struct SerialRuntime *state = &gSerialRuntime;

    if (state->mode != 0)
        state->transfer_enabled = 1;
}

void SerialRuntime_DisableTransfer(void)
{
    REG_IME = 0;
    REG_IE = REG_IE & 0xff3f;
    REG_IME = 1;
    REG_SIOCNT16 = 0x2003;
    REG_TM3CNT = 0x0000c963;
    REG_IF = 0xc0;
    gSerialRuntime.transfer_enabled = 0;
}
