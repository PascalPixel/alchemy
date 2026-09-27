#include "TYPES.H"
#include "FIELD_EVENT.H"


void ArutinYama_RunRollingObject(s32 id, s32 heading);
void Battle_ResetEffectCounterFar(void);
void Main_0808a5e8(void);

/* NONMATCHING: 188 of 188 bytes, 2 halfword edits (2026-09-24). sched2 orders the two
 * shifts of ((d >> 14) << 14) + 0x40000 the other way round: the reference
 * shifts d before it finishes building 0x40000.
 * 2026-09-26: allocator/sched2 dumps show reload insn 255 wins the ready-list
 * tie over shift insn 102. Quantization in a one-pass block moved the shift
 * too early (still two edits); a signed mask added an AND and pool word
 * (192 bytes, 11 edits); wrapping the store left the original two edits.
 * Retain the original ordinary source; this scheduling sweep is timeboxed.
 * 2026-09-27 interface transfer from exact ARUTIN_YAMA/SETTLE_MOUNT.C
 * and ROLL_OBJECT.C: both lookup results are FieldActor pointers, +0x0c
 * is fixed-point y, +0x28 is vertical velocity and sprite byte 9 OR 12
 * sets FieldSprite.priority to 3. Preserve the quantization expression and
 * all calls; the complete typed 188-byte candidate is byte-identical to
 * the baseline, including the trailing alignment and two pool words.
 * The only residual remains the shift order at 02000316/02000318.
 * This falsifies an actor/sprite alias-interface cause for that hunk;
 * no new scheduling trial is justified. Keep the proven field interfaces,
 * stop this axis, and do not retry the earlier quantization spellings.
 * Full normalized diff read; no direct C caller found in the exact scene
 * files, and the own-ROM callee at 02003850 is the exact rolling driver.
 * Complete service follow-up: all 18 call sites now name canonical
 * FIELD_EVENT services, the exact ArutinYama_RunRollingObject driver,
 * Battle_ResetEffectCounterFar and the unresolved-role Main_0808a5e8.
 * Each binding was checked against this overlay's import table; registered
 * main names use the existing veneer resolver. Registered the complete
 * [020002cc,02000388) owner as not-yet-c, including both pool words.
 * Result is again 188/188, two halfwords/two edits, topology equal and
 * every other instruction exact. The service ABI is not the shift cause.
 * No adoption or new DONE bytes; stop after this complete interface model. */
void ArutinYama_BeginRollingRide(s32 a0)
{
    u32 i;
    struct FieldActor *rec7;
    struct FieldActor *rec8;
    s32 record;
    s32 v3;

    rec8 = Engine_ActorGet(0);
    rec7 = Engine_ActorGet(8);
    Battle_ResetEffectCounterFar();
    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 22);
    Engine_EventWait(10);
    Engine_AudioPlayCue(152);
    Engine_ActorSetSpeed(0, 0x33333, 0x19999);
    v3 = rec7->y.fixed - rec8->y.fixed;
    if ((rec7->y.fixed - rec8->y.fixed) < 0) {
        v3 = rec8->y.fixed - rec7->y.fixed;
    }
    rec8->velocity_y = 0x40000 + ((v3 >> 14) << 14);
    Engine_ActorSetAnimation(0, 7);
    Engine_ObjectSetPosition(rec8, rec7->x.fixed, rec7->y.fixed, rec7->z.fixed);
    Engine_TaskWait(10);
    rec8->sprite->priority = 3;
    Engine_ActorWaitForMove(0);
    for (;;) {
        if (!((rec7->y.fixed >> 14) < (rec8->y.fixed >> 14))) break;
        Engine_TaskWait(1);
    }
    Engine_EventEnd();
    Engine_AudioPlayCue(159);
    ArutinYama_RunRollingObject(a0, 0);
    Engine_TaskWait(20);
    Main_0808a5e8();
}
