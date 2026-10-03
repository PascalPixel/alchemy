/*
 * SerialRuntime_BeginTransferB: receive destination and zero transferred bytes.
 * Reference: 76 bytes, including its literal pool and alignment.
 * Early-busy-guard baseline: score 540, complete object 76/76 bytes;
 * the nonvolatile status store moved before the IME read/disable.
 * IRQ-shared volatile-view trial: score 450, complete object 80/76 bytes.
 * Remaining: 9 register-only, 2 operand, 1 reordered, 3 inserted differences.
 * The destination still moves out of r0 and an extra r7 is saved. GCC adds
 * a byte read before each volatile struct-field write, unlike the reference.
 * The trial retains the status/cursor/count/active writes inside IME disable.
 * Current raw 0801319c installs 0801399c in VBlank; that handler calls
 * 08016990, which reads and writes these same active/status control bytes.
 * This supports the local access contract without changing the shared
 * declaration or claiming recovery of the original qualifier spelling.
 * All names resolve against the rebuilt English ELF. Two bounded trials
 * are recorded here; the volatile-view trial is retained, with no devices.
 */
#include "SERIAL_RUNTIME.H"

s32 SerialRuntime_BeginTransferB(void *destination)
{
    volatile struct SerialTransferState *state = &gSerialTransfer;
    u32 saved;

    if (gSerialReceiveDest != 0)
        return -1;

    saved = REG_IME;
    REG_IME = 0;
    /* VBlank's block-transfer step reads and updates these control bytes. */
    state->status = 0x81;
    gSerialReceiveDest = (s32)destination;
    gSerialReceivedSize = 0;
    state->active = 1;
    gSerialBlockSequence = 0;
    REG_IME = saved;
    return 0;
}
