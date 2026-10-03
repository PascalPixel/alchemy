/* 2026-10-03: corrected the duplicated send-wait draft to receive-wait B,
 * as recon/tla/raw/080168cc.s proves; uses canonical void WaitFrames.
 * Complete candidate/reference extents are 44/44 bytes, including the pool.
 * Score 80 (1 operand, 1 reordered): gSerialReceiveDest is unresolved in the
 * current TLA ELF, and the count's zero initialization moves. Not adopted. */
#include "TYPES.H"
#include "SYSTEM.H"

extern volatile s32 gSerialReceiveDest;

void SerialRuntime_WaitForTransferB(void)
{
    u32 count = 0;

    if (*(volatile s32 *)&gSerialReceiveDest != 0) {
        do {
            WaitFrames(1);
            count++;
        } while (count <= 0x927BF && *(volatile s32 *)&gSerialReceiveDest != 0);
    }
}
