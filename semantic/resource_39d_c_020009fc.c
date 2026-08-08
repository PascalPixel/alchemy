#include "types.h"

/*
 * Resource 39d scripted recoil cutscene at 0x020009fc (198 bytes, 10 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x020009fc, the single conditional
 * branch (`beq` at 0x02000a14) lands on 0x02000ab8, which is inside the body,
 * and the walk stops at the interworking return `pop {r5,r6,r7} / pop {r0} /
 * bx r0` at 0x02000abc-0x02000ac0.  So the executable extent is exactly
 * 0x020009fc-0x02000ac1 (198 bytes).  The popped register is r0, so the popped
 * value is the return address and the owner is `void`.
 *
 * Pool map — the halfword at 0x02000ac2 is `0x0000` alignment and the three
 * words after it are literals the walk never reaches as instructions:
 *   0x02000ac4  0x0200bc50   the handle slot, in-image 0x02003c50
 *   0x02000ac8  0xfffd0000   -3.0 in 16.16
 *   0x02000acc  0xfffe0000   -2.0 in 16.16
 * The next prologue (`push {r5, lr}`) begins at 0x02000ad0, the record-flag
 * initialiser already converted as resource_39d_c_02000ad0.c.
 *
 * Link base proof, this overlay: the image is 0x3c80 bytes, so the pool word
 * 0x0200bc50 is outside it as written and lands at 0x02003c50 under
 * `pool_word - 0x8000`, inside the image and even — data, not a Thumb entry.
 * The overlay's data is writable EWRAM, so the slot is spelled as an ordinary
 * variable and stored to.
 *
 * TRANSPOSITION: `resource_3c9:05ec` is the same routine.  Over all 198 bytes
 * exactly 29 differ, and every one of them is inside one of the ten `bl`
 * halfword pairs (+0x04/+0x06, +0x0a/+0x0c, +0x10, +0x1e/+0x20, +0x24/+0x26,
 * +0x54/+0x56, +0x6e/+0x70, +0x8a/+0x8c, +0xa2/+0xa4, +0xbc/+0xbe) — which is
 * the only thing that *can* differ, because each overlay's veneer bank and its
 * own local callee sit at different image offsets.  Every `movs` immediate,
 * every `ldr [pc]` displacement and every field offset is identical.
 * `resource_3c9:05ec` is likewise absent from the inventory; it is not in this
 * reconstruction's set but it is a free conversion for whoever takes 3c9.
 *
 * All ten call sites were resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_39d 09fc ac2`, and the
 * per-target histogram is Func_0808a080 x2, Func_0808a018 x1, Func_020008cc
 * x1, Func_080770c8 x1, Func_080000c0 x4, Func_0808a020 x1:
 *   0x02000a00 -> veneer 0x036f0 -> Func_0808a080   record fetch, index 0
 *   0x02000a06 -> veneer 0x036d8 -> Func_0808a018   scripted-section open
 *   0x02000a0a -> prologue 0x008cc -> Func_020008cc  local, returns the handle
 *   0x02000a1a -> veneer 0x036b8 -> Func_080770c8   one-argument cue, 592
 *   0x02000a20 -> veneer 0x036f0 -> Func_0808a080   record fetch, the handle
 *   0x02000a50 -> veneer 0x03630 -> Func_080000c0   yield 2
 *   0x02000a6a -> veneer 0x03630 -> Func_080000c0   yield 10
 *   0x02000a86 -> veneer 0x03630 -> Func_080000c0   yield 4
 *   0x02000a9e -> veneer 0x03630 -> Func_080000c0   yield 4
 *   0x02000ab8 -> veneer 0x036e0 -> Func_0808a020   scripted-section close
 * `overlay_show`'s printed targets (0x20040f2, 0x20040e0, ...) are the
 * pc-relative misreading: four of them differ from each other despite all four
 * being the same Func_080000c0 veneer.
 *
 * Func_020008cc is called with r0 still holding record 0, but it overwrites r0
 * with `movs r0,#0` before its first use, so it takes no argument.  The same
 * is true of Func_0808a018, which the 373/375/377 scripted owners already
 * document as an argument-free bracket.
 *
 * THE CLOSE IS SHARED between the taken and the not-taken side of the `beq`,
 * so it is reached with a `goto` rather than written into both — a per-arm copy
 * would put two Func_0808a020 calls in the C against one in the assembly.
 *
 * The body is a five-step tween on two 16.16 fields.  The same delta is added
 * to +12 of both objects and to +20 of the acting record, in the order
 * -3.0, -2.0, +2.0, +2.0, +1.0 with scheduler yields of 2, 10, 4 and 4 frames
 * between them.  THE FIVE DELTAS SUM TO EXACTLY ZERO, so the sequence is a
 * recoil that returns both objects to where they started; reading any step in
 * isolation makes it look like a permanent displacement.  The last two deltas
 * are materialised as `movs #128 / lsls #10` and `movs #128 / lsls #9` —
 * values, not displacements — and the third is held in r5 across two steps,
 * which is the evidence that steps three and four really are the same delta.
 *
 * The byte at +0x55 is the record flag byte the neighbouring 0x02000ad0
 * initialiser writes wholesale and the 0x02000b24 owner masks; here the fetched
 * object's copy is zeroed outright and the acting record's bit 0 is cleared.
 */

u8 *Func_0808a080();           /* record fetch, returns the record */
void Func_0808a018();          /* scripted-section open  */
void Func_0808a020();          /* scripted-section close */
void Func_080000c0();          /* advance the task scheduler */
void Func_080770c8();          /* one-argument story cue */
s32 Func_020008cc(void);       /* local: builds the scene and returns its handle */

extern s32 Data_0200bc50;      /* in-image 0x02003c50 */

void Func_020009fc(void)
{
    u8 *actor = Func_0808a080(0);
    u8 *object;
    s32 delta;

    Func_0808a018();
    Data_0200bc50 = Func_020008cc();
    if (Data_0200bc50 == 0) goto close;

    Func_080770c8(592);                        /* 148 << 2 */
    object = Func_0808a080(Data_0200bc50);

    object[0x55] = 0;
    actor[0x55] &= 0xfe;

    delta = -0x30000;                          /* -3.0 */
    *(s32 *)(object + 12) += delta;
    *(s32 *)(actor + 12) += delta;
    *(s32 *)(actor + 20) += delta;
    Func_080000c0(2);

    delta = -0x20000;                          /* -2.0 */
    *(s32 *)(object + 12) += delta;
    *(s32 *)(actor + 12) += delta;
    *(s32 *)(actor + 20) += delta;
    Func_080000c0(10);

    delta = 0x20000;                           /* +2.0, held in r5 */
    *(s32 *)(object + 12) += delta;
    *(s32 *)(actor + 12) += delta;
    *(s32 *)(actor + 20) += delta;
    Func_080000c0(4);

    *(s32 *)(object + 12) += delta;            /* +2.0 again, same register */
    *(s32 *)(actor + 12) += delta;
    *(s32 *)(actor + 20) += delta;
    Func_080000c0(4);

    delta = 0x10000;                           /* +1.0; the five sum to zero */
    *(s32 *)(object + 12) += delta;
    *(s32 *)(actor + 12) += delta;
    *(s32 *)(actor + 20) += delta;

close:
    Func_0808a020();
}
