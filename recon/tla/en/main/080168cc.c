/* 2026-10-03: physical cursor names now resolve. Complete extents are
 * 44/44 bytes, including the pool. Score 60: count initialization moves.
 * Returning after 600,000 frame waits does not establish completion.
 * Not adopted. */
#include "SERIAL_RUNTIME.H"

void SerialRuntime_WaitForTransferB(void)
{
    u32 count = 0;

    if (gSerialReceiveDest != 0) {
        do {
            WaitFrames(1);
            count++;
        } while (count <= 0x927bf && gSerialReceiveDest != 0);
    }
}
