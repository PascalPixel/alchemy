#include "GLOBAL.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "SERIAL_RUNTIME.H"

void SerialRuntime_RemoveIrqHandlers(void)
{
    gSerialExchangeActive = 0;
    Runtime_SetIrqHandler(7, 0, 0);
    Runtime_SetIrqHandler(6, 0, 0);
}


u32 SerialRuntime_WaitForStatusMask(s32 mask)
{
    if ((mask & gLinkStatus) != mask) {
        do {
            WaitFrames(1);
        } while ((mask & gLinkStatus) != mask);
    }
    return (REG_SIOCNT << 0x1A) >> 0x1E;
}
