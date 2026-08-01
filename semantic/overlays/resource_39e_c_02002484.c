typedef int s32;

/*
 * resource_39e owner at 0x02002484, 132 bytes: a two-flag gate that either
 * stages a short scene or hands off to Func_08015040.
 *
 * Complete owner: `push {lr}` at 0x02002484 through `pop {r0} / bx r0` at
 * 0x020024f2-0x020024f4, a zero alignment halfword at 0x020024f6, then the
 * five-word literal pool 0x020024f8-0x02002507 (0x89a, 0x895, 0x18ad,
 * 0x0200c77a). The next owner's prologue is exactly at 0x02002508.
 * **132 bytes**, measured to the epilogue.
 *
 * Not found by the structural inventory walk (unindexed): reached only as
 * a published pointer. All 13 `bl` targets resolved with
 * `bun tools/overlay_call_targets.ts resource_39e 2484 2508` under the
 * `+2` rule.
 *
 * THIS ROW IS THE READ END OF resource_39e's FLAG CHAIN. It tests 0x89a
 * and 0x895, and neither is written here. 0x89a is set by the drafted
 * 0x02001494; 0x895 is set by the still-undrafted 0x02002ad0 at its tail.
 * So three published owners are ordered by state alone, with no call
 * between them -- the H3 case, and the only instrument for it is grepping
 * story-flag ids across the overlay. Both ids arrive as pool words here,
 * where 0x02001494 builds its set with a movs/lsls pair; same id, two
 * encodings, because the compiler picks per VALUE.
 *
 * THE GATE IS A SHORT-CIRCUIT OR, READ FROM THE BRANCHES. 0x02002492 is
 * `cmp r0,#0 / bne.n 0x020024ac` and 0x0200249c is the same test on the
 * second flag jumping to the same label, with the fall-through at
 * 0x0200249e being the path where BOTH are zero. So the staged scene runs
 * when EITHER flag is set, and the second Func_080770c0 is genuinely
 * skipped when the first is non-zero. Writing this as `&&` over the zero
 * tests would invert it.
 *
 * Both arms end in Func_0808a020, emitted twice (0x020024a6 and
 * 0x020024ee) because the first arm branches over the second to the
 * epilogue. It runs exactly once on either path, so it is hoisted below
 * the if/else here; that is the one place the call census legitimately
 * reads two sites against one statement.
 *
 * 0x0200c77a is an in-image data address and is EVEN, so a descriptor and
 * not a published function pointer -- do not feed it to the published
 * sweep.
 *
 * Uncertainty: none of the eight callees are identified beyond call shape.
 * What the two flags mean is not established; only that this row reads
 * them and does not write them.
 */

extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a248(s32 arg0);
extern void Func_080f9010(s32 arg0);
extern void Func_08009178(void *data, s32 arg1, s32 arg2);
extern void Func_08015040(s32 arg0, s32 arg1);
extern s32 Func_080770c0(s32 flagId);

void Func_02002484(void)
{
    Func_0808a018();

    if (Func_080770c0(0x89a) != 0 || Func_080770c0(0x895) != 0) {
        Func_080f9010(158);
        Func_08009178((void *)0x0200c77a, 78, 13);
        Func_0808a090(0, 128 << 8, 128 << 7);
        Func_0808a0d0(0, 153 << 1, 248);
        Func_0808a0c8(0, 152 << 1, 216);
        Func_0808a010(20);
        Func_0808a248(4);
    } else {
        Func_08015040(0x18ad, 1);
    }

    Func_0808a020();
}
