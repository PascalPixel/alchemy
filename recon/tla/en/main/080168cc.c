/*
 * Draft: SerialRuntime_WaitForTransferA does not yet match; 4 halfwords differ from ☀️'s C, first at +0x4 (movs r5, #0).
 * Links as recon/tla/raw/080167d8.s.
 */
#include "TYPES.H"

extern volatile s32 gSerialSendSource;

s32 WaitFrames(s32);

void SerialRuntime_WaitForTransferA(void)
{
    u32 count = 0;

    if (*(volatile s32 *)&gSerialSendSource != 0) {
        do {
            WaitFrames(1);
            count++;
        } while (count <= 0x927BF && *(volatile s32 *)&gSerialSendSource != 0);
    }
}
