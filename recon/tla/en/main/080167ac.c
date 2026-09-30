/*
 * Draft: SerialRuntime_RemoveIrqHandlers does not yet match; 3 halfwords differ from ☀️'s C, first at +0x26 (data).
 * Links as recon/tla/raw/080164e8.s.
 */
#include "SERIAL_RUNTIME.H"

extern u8 gSerialExchangeActive[];

void Runtime_SetIrqHandler(s32, s32, InterruptHandler);

void SerialRuntime_RemoveIrqHandlers(void)
{
    s16 *work;
    s32 handler;

    work = (s16 *)((u32)&gSerialExchangeActive);
    do {
        do {
        } while (0);
        *work = 0;
        Runtime_SetIrqHandler(7, 0, (InterruptHandler)(handler = 0));
    } while (0);
    handler = 6;
    Runtime_SetIrqHandler(handler, 0, 0);
}
