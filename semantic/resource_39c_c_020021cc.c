#include "types.h"

/*
 * resource_39c owner at 0x020021cc, 248 bytes: an event-flag gated dialogue
 * check -- picks one of four possible messages by two fixed-point values
 * read from two looked-up objects and a handful of story-flag reads, shows
 * at most one of them, and reports whether the "success" variant fired.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020021cc through `pop {r5, r6} /
 * pop {r1} / bx r1` at 0x200229a-0x200229e -- the popped word goes to r1,
 * not r0, so r0 (last set to 0 or 1 on every path) IS the result, matching
 * this project's own documented convention for a real-return-value owner
 * (resource_3cb_c_02000580.c's identical `pop {r1} / bx r1` tail). The
 * trailing nine-word literal pool runs 0x020022a0-0x020022c3; the next
 * owner's prologue is at 0x020022c4.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * `Fixed20()` below is the repeated `if (raw < 0) raw += 0xfffff; raw >> 20`
 * idiom read twice (0x020021d4-0x020021e2 and 0x020021e6-0x020021f8) -- the
 * standard truncate-toward-zero correction for an arithmetic right shift of
 * a value that can be negative.
 *
 * Uncertainty: none of the seven resolved callees are identified beyond their
 * call shape and argument count; the two fixed-point values (from field 8 of
 * one looked-up object and field 16 of another, both looked up with the
 * same constant argument 9) are read but their unit/meaning is not
 * established; the four 0x0200dfXX pool words are treated as opaque
 * message-id pointers, and the 793/794/795/2167 constants as opaque flag
 * ids. The asymmetry between the two "success" tails -- one (messages at
 * 0x0200df20/0x0200df60/0x0200dff0) still falls through to the "default"
 * call and returns 0, the other (message at 0x0200dfa8) takes two
 * different follow-up calls and returns 1 -- is preserved exactly as
 * compiled, not smoothed into a single shape.
 */

extern void *Func_0808a080(s32 arg0);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32, s32, s32, s32);
extern void Func_0808a218(void);
extern s32 Func_080770c0(s32 flagId);
extern void Func_0808a098(s32 arg0, void *msg);
extern void Func_0808a010(s32 arg0);

static s32 Fixed20(s32 raw)
{
    if (raw < 0) {
        raw += 0xfffff;
    }
    return raw >> 20;
}

s32 Func_020021cc(void)
{
    s32 valA = Fixed20(*(s32 *)((u8 *)Func_0808a080(9) + 8));
    s32 valB = Fixed20(*(s32 *)((u8 *)Func_0808a080(9) + 16));
    void *msg;

    Func_0808a208(0x50000, 0xa000);
    Func_0808a210(0x3300000, -1, 0x2c80000, 1);
    Func_0808a218();

    if (Func_080770c0(2167) != 0) {
        goto negative;
    }

    if (valA == 50) {
        if (Func_080770c0(793) == 0) {
            goto negative;
        }
        msg = (void *)0x0200df20;
        goto message;
    }

    if (valA != 49) {
        goto negative;
    }

    if (valB == 44) {
        if (Func_080770c0(793) == 0 && Func_080770c0(794) == 0 && Func_080770c0(795) == 0) {
            msg = (void *)0x0200df60;
            goto message;
        }
        if (Func_080770c0(793) != 0) {
            msg = (void *)0x0200dff0;
            goto message;
        }
        goto check_success;
    }

    goto check_success;

message:
    Func_0808a098(9, msg);
    goto negative;

check_success:
    if (valB == 46 && Func_080770c0(794) != 0) {
        Func_0808a098(9, (void *)0x0200dfa8);
        Func_0808a010(30);
        return 1;
    }

negative:
    Func_0808a010(30);
    return 0;
}
