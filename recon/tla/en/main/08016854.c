/*
 * SerialRuntime_BeginTransferB: receive destination and zero transferred bytes.
 * Reference: 76 bytes, including its literal pool and alignment.
 * Ordinary-C baseline: score 540; complete object extent 76/76 bytes.
 * Remaining: 10 register-only, 3 operand, 2 reordered, 2 inserted,
 * 1 deleted differences. The destination moves out of r0, an extra r7
 * is saved, and the status store moves before the IME read/disable.
 * Refreshed against the rebuilt English ELF: every physical name resolves;
 * no unresolved-symbol comparisons remain.
 * One natural early-busy-guard trial; no compiler devices or equivalent
 * expression trial. Canonical-name closure preserves this complete
 * object after normalizing only the two renamed relocation targets.
 * Register and ordering differences still prevent adoption.
 */
#include "SERIAL_RUNTIME.H"

s32 SerialRuntime_BeginTransferB(void *destination)
{
    u32 saved;

    if (gSerialReceiveDest != 0)
        return -1;

    saved = REG_IME;
    REG_IME = 0;
    gSerialTransfer.status = 0x81;
    gSerialReceiveDest = (s32)destination;
    gSerialReceivedSize = 0;
    gSerialTransfer.active = 1;
    gSerialBlockSequence = 0;
    REG_IME = saved;
    return 0;
}
