typedef signed int s32;

/*
 * resource_3bb owner at 0x02000b38, 154 bytes (0x02000b38-0x02000bd1), all
 * code: the body loads no literal pool and no pool sits inside the span.
 *
 * Prologue `push {r5, lr}` at 0x02000b38, epilogue
 * `pop {r5} / pop {r0} / bx r0`: r0 holds the popped return address, so the
 * owner is void.  r0 is copied into r5 in the first instruction and is live to
 * the end, so this owner takes one argument; it is tested with `bge`, so it is
 * signed.
 *
 * All 15 `bl` sites are placed and reach nine distinct callees, matching the
 * inventory row's calls=15.  Targets come from tools/overlay_call_targets.ts
 * (target offset = stored displacement + 2), never from the disassembler's
 * annotations - it prints the repeated Func_0808a100 and Func_0808a148
 * halfwords as different callees.  Fourteen sites reach the import veneer
 * table (0x3f58 -> Func_0808a088, 0x3f28 -> Func_08077260, 0x3f38 ->
 * Func_0808a018, 0x3f90 -> Func_0808a0f0, 0x3fb8 -> Func_0808a148, 0x3f98 ->
 * Func_0808a100, 0x3da0 -> Func_080000c0, 0x4020 -> Func_0808a210, 0x3f40 ->
 * Func_0808a020) and one reaches this overlay's own prologue at file offset
 * 0x2adc, which receives the owner's argument unchanged.
 *
 * Shape: bring in participants 40 and 41, set the scene up, place participants
 * 8 and 0 and face them, then pick one of two pose pairs on the sign of the
 * argument, wait a frame, run the transition and hand the argument on to
 * Func_02002adc before closing the scene.
 *
 * The coordinates are built by shifting small immediates; they are written out
 * in full below (176 << 15, 128 << 17, 240 << 15, 128 << 7, 208 << 15,
 * 192 << 16).
 *
 * Uncertainty: what the argument selects is not established here beyond its
 * sign choosing between the pose pair (10, 35) and (8, 28).
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_0808a088();
void Func_08077260();
void Func_0808a018();
void Func_0808a0f0();
void Func_0808a148();
void Func_0808a100();
void Func_080000c0();
void Func_0808a210();
void Func_0808a020();

/* This overlay's own routine at file offset 0x2adc. */
void Func_02002adc(s32);

void Func_02000b38(s32 variant)
{
    Func_0808a088(40);
    Func_0808a088(41);
    Func_08077260(1);
    Func_0808a018();

    Func_0808a0f0(8, 0x580000, 0x1000000);      /* 176 << 15, 128 << 17 */
    Func_0808a0f0(0, 0x780000, 0x1000000);      /* 240 << 15 */
    Func_0808a148(8, 0x4000, 0);                /* 128 << 7 */
    Func_0808a148(0, 0x4000, 0);

    if (variant < 0) {
        Func_0808a100(8, 10);
        Func_0808a100(0, 35);
    } else {
        Func_0808a100(8, 8);
        Func_0808a100(0, 28);
    }

    Func_080000c0(1);
    Func_0808a210(0x680000, 0, 0xc00000, 0);    /* 208 << 15, 192 << 16 */

    Func_02002adc(variant);
    Func_0808a020();
}
