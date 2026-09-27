#include "TYPES.H"
#include "FIELD_EVENT.H"


void Func_02003bbc();
void Func_02003dae();
void Func_02003dc6();
void Func_02003dea();
void Func_02003e28();
void Func_02003e56();
void Func_02003e5c();
struct FieldActor *Func_02003e6c(s32 actor);
struct FieldActor *Func_02003e74(s32 actor);
void Func_02003ea4();
void Func_02003ed6();
void Func_02003ede();
void Func_02003f14();
void Func_02003f26();
void Func_02003fe2();
void Func_0200401a();
void Func_02004084();
void Func_0200408e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

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
 * No adoption or new DONE bytes. */
void Func_020002cc(s32 a0)
{
    u32 i;
    struct FieldActor *rec7;
    struct FieldActor *rec8;
    s32 record;
    s32 v3;

    rec8 = Func_02003e6c(0);
    rec7 = Func_02003e74(8);
    Func_02003fe2();
    Func_02003e56();
    Func_02003ede(0, 22);
    Func_02003e5c(10);
    Func_0200401a(152);
    Func_02003ea4(0, 0x33333, 0x19999);
    v3 = rec7->y.fixed - rec8->y.fixed;
    if ((rec7->y.fixed - rec8->y.fixed) < 0) {
        v3 = rec8->y.fixed - rec7->y.fixed;
    }
    rec8->velocity_y = 0x40000 + ((v3 >> 14) << 14);
    Func_02003f14(0, 7);
    Func_02003e28(rec8, rec7->x.fixed, rec7->y.fixed, rec7->z.fixed);
    Func_02003dae(10);
    rec8->sprite->priority = 3;
    Func_02003f26(0);
    for (;;) {
        if (!((rec7->y.fixed >> 14) < (rec8->y.fixed >> 14))) break;
        Func_02003dc6(1);
    }
    Func_02003ed6();
    Func_02004084(159);
    Func_02003bbc(a0, 0);
    Func_02003dea(20);
    Func_0200408e();
}
