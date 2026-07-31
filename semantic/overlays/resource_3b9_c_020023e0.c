typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;

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
 * 248 bytes. Recorded here per this session's recurring "confirm the
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
 * Func_0808a0b8 and Func_0808a0c0 alternate between id 0 and id 11 with
 * a shared second argument (588 = 147<<2) and a climbing first argument
 * (780, 812, 880, 912, 980, 1012) -- the same "three near-identical
 * calls with shifted-constant pairs" shape resource_3b9_c_02002668.c's
 * header already named for a different callee pair.
 *
 * Uncertainty: none of the eleven callees are identified beyond call
 * shape; every id/constant is recorded as a passed value.
 */

extern void Func_0808a018(void);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern s32 Func_080770c0(s32 flag_id);
extern void Func_080770c8(s32 flag_id);
extern void Func_08077150(s32 arg0);
extern void Func_08077168(s32 arg0);
extern void Func_080000c0(s32 arg0);
extern u8 *Func_0808a080();
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a200(s32 id, s32 arg1);
extern void Func_0808a248(s32 arg0);

void Func_020023e0(void)
{
    u8 *record11;

    if (Func_080770c0(5) != 0) {
        Func_080770c8(365);
        Func_08077168(5);
        Func_08077150(3);
    }

    Func_0808a018();
    Func_0808a0f0(11, 178 << 18, 147 << 18);
    Func_080000c0(1);
    Func_0808a200(11, 1);
    Func_0808a090(11, 0x19999, 0xcccc);
    Func_0808a090(0, 0x19999, 0xcccc);

    record11 = Func_0808a080(11);
    *(u16 *)(record11 + 6) = 0;

    Func_0808a360();
    Func_0808a100(0, 2);
    Func_0808a100(11, 2);
    Func_0808a0b8(0, 195 << 2, 147 << 2);
    Func_0808a0c0(11, 203 << 2, 147 << 2);
    Func_0808a0b8(0, 220 << 2, 147 << 2);
    Func_0808a0c0(11, 228 << 2, 147 << 2);
    Func_0808a0b8(0, 245 << 2, 147 << 2);
    Func_0808a0c0(11, 253 << 2, 147 << 2);
    Func_0808a368();
    Func_0808a370();

    if (Func_080770c0(0x90f) != 0) {
        Func_0808a248(31);
    } else {
        Func_0808a248(65);
    }
}
