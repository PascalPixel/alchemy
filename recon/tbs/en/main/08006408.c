/* 2026-10-01 (matcher 3): why the zero is pooled. agscc loads every
   HImode constant from the literal pool (*thumb_movhi_insn lists "mn"
   before "I"), so the reference's `ldr r0, [pc]` is a halfword zero that
   CSE shares into the byte store. The plain body (state set up front, the
   test on SERIAL_ACTIVE_B, the six stores in order, no blocks) already
   pools it and scores 315: its HImode zero is set at SERIAL_VALUE_B's
   store, lives across `SERIAL_ACTIVE_B = value` and pushes value into r5.
   local-alloc's update_equiv_regs moves a set-once, used-once REG_EQUIV
   constant to just before its use only when the two sit in different basic
   blocks and the use is outside any loop notes (do/while(0) blocks count),
   which is how ResetSceneTransitionEffect (EFFECT38.C) gets its late pooled
   zero: an s16 field store's store_bit_field mask makes the HImode zero in
   an earlier block. Nothing in this body gives the zero an earlier block
   yet: u16/s16 zero locals are promoted to SImode, a struct-field form of
   SERIAL_VALUE_B folds into the state pointer, and a block around the first
   stores lets CSE use busy (r4) again. The permuter's 190 changed what is
   stored, so it was discarded. */
/* 2026-09-30 (Mercury): this A-shaped body (the sibling
   SerialRuntime_BeginTransferA's goto layout and one-pass block, with the
   sequence clear after the block) matches every instruction but one: the
   reference stores gSerialBlockSequence from a pooled halfword zero loaded
   right before the strb (ldr r0, [pc, #12]), where this build stores busy's
   known zero (strb r4), so it is 72 of 80 bytes. The pooled zero is the
   halfword constant of the SERIAL_VALUE_B = 0 store, which CSE shares with
   the byte store; the reference then rematerialises it at its use
   (local-alloc moves a REG_EQUIV constant next to its single use only when
   the two sit in different basic blocks, or reload does when it gets no
   register). With the clear inside the block the shared zero stays live
   across value's store and takes r0 (st4: 28). */
#include "SERIAL_RUNTIME.H"

s32 SerialRuntime_BeginTransferB(s32 value)
{
    volatile s32 *active;
    struct SerialTransferState *state;
    volatile u16 *ime;
    u32 saved_interrupt_master;
    s32 busy;

    active = &SERIAL_ACTIVE_B;
    busy = *active;
    do {
        state = &gSerialTransfer;
    } while (0);
    active = &SERIAL_ACTIVE_B;
    if (busy == 0)
        goto begin_transfer;
    value = -1;
    goto transfer_complete;

begin_transfer:
    ime = &REG_IME;
    saved_interrupt_master = *ime;
    *ime = (u16)(u32)ime;
    do {
        state->status = 0x81;
        SERIAL_VALUE_B = 0;
        state->active = 1;
        *active = value;
    } while (0);
    gSerialBlockSequence = 0;
    *ime = saved_interrupt_master;
    value = 0;

transfer_complete:
    return value;
}
