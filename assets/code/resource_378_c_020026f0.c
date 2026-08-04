typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 378 orbit step at 0x020026f0 (94 bytes, 2 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x020026f0 — `push {r5, r6, lr}`
 * followed by r8 and sl through r5,r6 — the body is straight-line with no
 * conditional branch, and the walk stops at the matching high-register unwind
 * `pop {r3, r5} / mov r8,r3 / mov sl,r5 / pop {r5, r6} / pop {r0} / bx r0` at
 * 0x02002742-0x0200274c.  So the executable extent is exactly
 * 0x020026f0-0x0200274d (94 bytes).  The popped register is r0, so the popped
 * value is the return address and the owner is `void`.
 *
 * Pool map: none — this owner loads no literal at all; both displacements
 * (100, 102) and both scale factors are built from immediates.  The halfword
 * at 0x0200274e is `0x0000` alignment before the next prologue at 0x02002750,
 * and the walk never reaches it as an instruction.
 *
 * Both call sites were resolved with
 * `bun tools/overlay_call_targets.ts resource_378 26f0 274e`, assigning the
 * i-th occurrence in the source to the i-th site in address order:
 *   0x02002708 -> veneer 0x02003548 -> Func_08000120
 *   0x0200271c -> veneer 0x02003540 -> Func_08000118
 * Those are the ROM's fixed dispatch stubs at 0x080000c0 (8 bytes per entry,
 * `ldr r4,1f / bx r4 / .word target+1`): entry 12 -> Func_0800231c and entry
 * 11 -> Func_08002322, the same pair resource_39f_c_02000e18.c reaches.  They
 * are called with the same angle and their results scaled by two different
 * constants into two different axes, which is the shape of a cosine/sine pair;
 * which of the two is which is not settled here.
 *
 * `lsls #3 / subs / lsls #1` is *14 and `lsls #2 / adds / lsls #1` is *10 —
 * the two radii of an ellipse, not a fixed-point normalisation.  The anchor is
 * the pointer at +104, whose +8 and +16 are the centre the offsets are added
 * to; the result is published both to the object's own +8/+16 and to its
 * +56/+64 shadow pair.
 *
 * The angle lives at +100 and its per-frame step at +102, both u16, and the
 * step is added at the end — so the two halfwords are an angle/velocity pair
 * in one word and must not be read as one 32-bit field.
 */

s32 Func_02005c52();           /* site veneer -> ROM dispatch[12] -> Func_0800231c */
s32 Func_02005c5e();           /* site veneer -> ROM dispatch[11] -> Func_08002322 */

void Func_020026f0(u8 *object)
{
    u8 *anchor = *(u8 **)(object + 104);
    u16 *angle = (u16 *)(object + 100);
    u16 theta = *angle;
    s32 along;
    s32 across;
    s32 echoed;

    along = *(s32 *)(anchor + 8) + Func_02005c52(theta) * 14;
    *(s32 *)(object + 8) = along;

    across = *(s32 *)(anchor + 16) + Func_02005c5e(theta) * 10;
    echoed = *(s32 *)(object + 8);
    *(s32 *)(object + 16) = across;
    *(s32 *)(object + 64) = across;
    *(s32 *)(object + 56) = echoed;

    *angle = (u16)(*angle + *(u16 *)(object + 102));
}
