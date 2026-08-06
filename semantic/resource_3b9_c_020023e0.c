#include "types.h"

/*
 * resource_3b9 owner at 0x020023e0, 248 bytes: a flag-gated prelude
 * (three story-flag-family calls, only when flag 5 is set) followed by
 * an unconditional scripted-scene bracket, then a closing story-flag
 * test that picks one of two Func_0808a248 arguments.
 *
 * Complete owner: `push {lr}` at 0x020023e0 through `pop {r0} / bx r0`
 * at 0x020024c4-0x020024c6 (HANDOVER's void epilogue shape -- the
 * popped word is r0 itself), followed by the four-word literal pool
 * 0x020024c8-0x020024d7; the next owner's prologue is exactly at
 * 0x020024d8 (this overlay's own resource_3b9_c_020024d8.c). No
 * incoming arguments are read before being overwritten, so `void`.
 *
 * SPAN CORRECTION: earlier queue notes estimated 232 bytes from a
 * partial read; the disassembly's own literal pool runs through
 * 0x020024d7 with no gap before the next prologue, so the real span is
 * 248 bytes. Recorded here per the reconstruction's recurring "confirm the
 * next owner's prologue byte, don't trust the first `bx r0`" lesson.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2`
 * rule (`bun tools/overlay_call_targets.ts resource_3b9 23e0 24d8`).
 *
 * SHARED IDIOMS: Func_0808a018 is the scripted-scene bracket open (no
 * matching close in this owner -- it is left open, as
 * resource_38d/resource_389's single-direction bracket owners also do);
 * Func_080770c0/Func_080770c8 are the story-flag test/set pair; both
 * cited from resource_3b9_c_0200160c.c's header rather than re-derived.
 * Func_08077150 and Func_08077168 are new to this mandate's sweep --
 * same single-id-argument call shape as Func_080770c8, so plausibly the
 * same flag family under a different veneer, but not confirmed.
 *
 * Func_0808a0b8 and Func_0808a0c0 share a second argument
 * (588 = 147<<2) while the first argument climbs through 780, 812, 880,
 * 912, 980 and 1012. The final id-11 site resolves to Func_0808a0b8 too,
 * so the sequence is not a perfectly alternating pair.
 *
 * Uncertainty: none of the eleven callees are identified beyond call
 * shape; every id/constant is recorded as a passed value.
 */



















extern s32 Func_02004fa2(s32 flag_id);
extern void Func_02004fb4(s32 flag_id);
extern void Func_02004fd2(s32 arg0);
extern void Func_02004fd0(s32 arg0);
extern void Func_02004ff4(void);
extern void Func_02005062(s32 id, s32 arg1, s32 arg2);
extern void Func_02004fa0(s32 arg0);
extern void Func_020050f0(s32 id, s32 arg1);
extern void Func_0200503a(s32 id, s32 arg1, s32 arg2);
extern void Func_02005044(s32 id, s32 arg1, s32 arg2);
extern u8 * Func_02005042();
extern void Func_0200515a(void);
extern void Func_020050a2(s32 id, s32 arg1);
extern void Func_020050aa(s32 id, s32 arg1);
extern void Func_02005090(s32 id, s32 arg1, s32 arg2);
extern void Func_020050a6(s32 id, s32 arg1, s32 arg2);
extern void Func_020050ac(s32 id, s32 arg1, s32 arg2);
extern void Func_020050c2(s32 id, s32 arg1, s32 arg2);
extern void Func_020050c8(s32 id, s32 arg1, s32 arg2);
extern void Func_020050d6(s32 id, s32 arg1, s32 arg2);
extern void Func_020051ca(void);
extern void Func_020051d6(void);
extern s32 Func_0200506c(s32 flag_id);
extern void Func_020051b6(s32 arg0);
extern void Func_020051be(s32 arg0);
void Func_020023e0(void)
{
    u8 *record11;

    if (Func_02004fa2(5) != 0) {
        Func_02004fb4(365);
        Func_02004fd2(5);
        Func_02004fd0(3);
    }

    Func_02004ff4();
    Func_02005062(11, 178 << 18, 147 << 18);
    Func_02004fa0(1);
    Func_020050f0(11, 1);
    Func_0200503a(11, 0x19999, 0xcccc);
    Func_02005044(0, 0x19999, 0xcccc);

    *(u16 *)(record11 + 6) = 0;
    record11 = Func_02005042(11);

    Func_0200515a();
    Func_020050a2(0, 2);
    Func_020050aa(11, 2);
    Func_02005090(0, 195 << 2, 147 << 2);
    Func_020050a6(11, 203 << 2, 147 << 2);
    Func_020050ac(0, 220 << 2, 147 << 2);
    Func_020050c2(11, 228 << 2, 147 << 2);
    Func_020050c8(0, 245 << 2, 147 << 2);
    Func_020050d6(11, 253 << 2, 147 << 2);
    Func_020051ca();
    Func_020051d6();

    if (Func_0200506c(0x90f) != 0) {
        Func_020051b6(31);
    } else {
        Func_020051be(65);
    }
}
