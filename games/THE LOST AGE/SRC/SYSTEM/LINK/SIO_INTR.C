#include "TYPES.H"
#include "IO_REG.H"
#include "SERIAL_RUNTIME.H"

/* ☀️'s SYSTEM/LINK/SIO_INTR.C after its transfer interrupt, which is still
   disassembly here. ⚓️'s compiler needs none of ☀️'s steering in the
   handler remover. */
void SerialRuntime_RemoveIrqHandlers(void)
{
    gSerialExchangeActive = 0;
    Runtime_SetIrqHandler(7, 0, 0);
    Runtime_SetIrqHandler(6, 0, 0);
}

extern volatile u16 gLinkStatus;

u32 SerialRuntime_WaitForStatusMask(s32 mask)
{
    if ((mask & gLinkStatus) != mask) {
        do {
            WaitFrames(1);
        } while ((mask & gLinkStatus) != mask);
    }
    /* SIOCNT bits 4-5: this unit's multiplayer ID. */
    return (REG_SIOCNT << 0x1A) >> 0x1E;
}
