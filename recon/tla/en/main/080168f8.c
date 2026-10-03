/* 2026-10-03: uses the canonical void WaitFrames declaration; body unchanged.
 * Complete candidate/reference extents are 56/52 bytes, including the pool.
 * Score 180 (4 operand, 1 inserted): three cursor loads have unresolved
 * gSerialSendSource/gSerialReceiveDest names; the entry branch target and
 * an extra cursor dereference also differ. No reshaping or adoption. */
#include "TYPES.H"
#include "SYSTEM.H"
extern volatile s32 gSerialReceiveDest;
extern volatile s32 gSerialSendSource;

void SerialRuntime_WaitForTransfers(void)
{
    u32 count;

    count = 0;
    if (*(volatile s32 *)&gSerialSendSource != 0)
    {
        goto loop;
    }
    if (*(volatile s32 *)&gSerialReceiveDest != 0)
    {
        goto loop;
    }
    return;
loop:
    WaitFrames(1);
    count++;
    if (count > 0x000927BF)
    {
        return;
    }
    if (*(volatile s32 *)&gSerialSendSource != 0)
    {
        goto loop;
    }
    if (*(volatile s32 *)&gSerialReceiveDest != 0)
    {
        goto loop;
    }
}
