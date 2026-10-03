/* 2026-10-03: physical cursor names now resolve. Complete extents are
 * 44/44 bytes, including the pool. Score 60: count initialization moves.
 * A named cursor pointer trial worsened 60 to 170; plain globals retained.
 * Returning after 600,000 frame waits does not establish completion.
 * Not adopted. */
#include "SERIAL_RUNTIME.H"

void SerialRuntime_WaitForTransferA(void)
{
    u32 count = 0;

    if (gSerialSendSource != 0) {
        do {
            WaitFrames(1);
            count++;
        } while (count <= 0x927bf && gSerialSendSource != 0);
    }
}
