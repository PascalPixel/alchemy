/* 2026-10-03: uses the canonical void WaitFrames declaration; body unchanged.
 * Complete candidate/reference extents are 44/44 bytes, including the pool.
 * Score 80 (1 operand, 1 reordered): gSerialSendSource is unresolved in the
 * current TLA ELF, and the count's zero initialization moves. Not adopted. */
#include "TYPES.H"
#include "SYSTEM.H"
extern volatile s32 gSerialReceiveDest;
extern volatile s32 gSerialSendSource;

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
