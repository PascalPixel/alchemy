/* 2026-10-03: canonical declarations and one shared cursor check replace
 * duplicated entry/exit tests. Complete extents: 52/52 bytes, with pools.
 * Score 120: one branch operand and one missing cursor-address load.
 * A structured while trial worsened the score to 1195; shared check kept
 * the score at 120 and reduced the prior 56-byte draft to 52 bytes.
 * Returning after 600,000 frame waits does not establish completion.
 * Not adopted; no compiler devices. */
#include "SERIAL_RUNTIME.H"

void SerialRuntime_WaitForTransfers(void)
{
    u32 count = 0;

    goto check;
loop:
    WaitFrames(1);
    count++;
    if (count > 0x927bf)
        return;
check:
    if (gSerialSendSource != 0 || gSerialReceiveDest != 0)
        goto loop;
}
