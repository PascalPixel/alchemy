typedef int s32;
typedef unsigned char u8;

/*
 * resource_380 owner at 0x02002400, 628 bytes: the scene-setup driver
 * that runs before the per-actor callback bank.
 *
 * PUBLISHED, NOT CALLED. No `bl` in the image reaches it; its Thumb
 * pointer is written into a script record, so both the whole-image
 * call-graph sweep and `tools/overlay_unindexed.ts` are blind to it.
 * Found by `bun tools/overlay_published.ts resource_380`, which also
 * flags it under sweep class C at "nearest owner 0x0200227c +388".
 *
 * SPAN MEASURED, NOT INFERRED. The queue's 628 bytes was a
 * prologue-to-next-known-start upper bound and is recorded here only
 * because the epilogue was then found and it agreed. The real end is
 * `add sp,#8 / pop {r5} / pop {r0} / bx r0` at
 * 0x02002646-0x0200264c, a zero alignment halfword at 0x0200264e, then
 * the nine-word literal pool 0x02002650-0x02002673; the next owner's
 * prologue is at 0x02002674. So 0x02002674 - 0x02002400 = 628, and the
 * bound happened to be tight. It was not safe to assume so: in this
 * same overlay family four different pool words have posed as branches
 * and made spans read short.
 *
 * PLACE IN THE OVERLAY. This is the third member of a group that only
 * makes sense read together:
 *   0x0200227c   wiring   -- enables and places six actors, 5/9/11/10/14/13
 *   0x02002400   this row -- the cinematic that plays over that wiring
 *   0x02002674+  callbacks -- one talk handler per actor
 * None of the three is reachable from either of the others by a call.
 * The corroboration is the actor id list: this driver repositions five
 * of the same six ids (5, 9, 11, 10, 14) at one shared coordinate pair,
 * and the callback bank hands a handler to all six.
 *
 * ACTOR 13 IS ABSENT from the reposition run, and that is a real
 * observation rather than a transcription slip -- the five
 * Func_0808a0f0 calls at 0x020024b4 through 0x020024dc were read
 * individually. Whether 13 keeps the position 0x0200227c gave it, or
 * is placed somewhere this owner does not reach, is not established.
 *
 * Shape, in order:
 *   1. Scene bracket opens; a six-iteration counted loop (`r5` masked
 *      back to a byte each turn, `cmp r5,#6`) alternates two
 *      Func_0808a330/Func_0808a348 pairs with 8-frame waits, and on the
 *      SECOND iteration only (`cmp r5,#1`) inserts one Func_080091f0.
 *      The odd loop constant 0x004039d2 is a pool word, transcribed.
 *   2. A framing block: Func_080091f0(-1, -1, 0xe666) with both first
 *      arguments built by `movs #1 / negs`, so they are genuine -1 and
 *      not large unsigned values, then the six-argument Func_08009180
 *      whose fifth and sixth arguments are passed on the stack via
 *      `str r3,[sp,#0]` / `[sp,#4]`.
 *   3. Func_02004260(222, ...) spawns something and its return is kept
 *      in r5 across a 40-frame wait before being handed to
 *      Func_08015120. The sibling call in
 *      resource_380_c_02000a98.c is Func_02004260(223, 0xe80000,
 *      0x100000, 0x1d00000) -- same first-argument neighbourhood, same
 *      second and third arguments, only the fourth differs. Two
 *      independent readings agreeing on that signature is why it is
 *      typed (s32,s32,s32,s32) here rather than re-derived.
 *   4. The five repositions, then two id-0/id-1 configuration blocks
 *      with a `Func_0808a080(0)` null check between them.
 *   5. A long fade/parameter sequence: alternating Func_080f9010,
 *      Func_080091f0, Func_0808a1b8 and Func_0808a128 triples.
 *   6. The workspace write and close.
 *
 * The workspace write at 0x0200262a is the known additive/subtractive
 * idiom in its SUBTRACTIVE form: `movs r2,#224 / lsls r2,#1` builds the
 * displacement 448, and `subs r2,#192` turns the same register into the
 * value 256 before the `str`. Transcribed as arithmetic rather than
 * assumed to be the canonical 513 -- in resource_3af alone this slot
 * took 256, 513, 514, 521 and 0x202 in different owners.
 *
 * All 57 `bl` targets resolved through the overlay's import-veneer
 * table under the +2 rule (tools/overlay_call_targets.ts). Two are
 * direct intra-image branches rather than veneer calls
 * (Func_02004260), and that distinction is only visible in the
 * resolved target.
 *
 * Uncertainty: the roles of Func_0808a330, Func_0808a348,
 * Func_0808a1b8 and Func_0808a128 are open; the fixed-point constants
 * are transcribed, not interpreted; and the six-iteration loop's
 * purpose is inferred from its call mix, which is weak evidence.
 */

extern void Func_08009180(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_08015040(s32 arg0, s32 arg1);
extern void Func_08015120(s32 arg0, s32 arg1);
extern void Func_080f9010(s32 arg0);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern s32 Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a248(s32 arg0);
extern void Func_0808a330(s32 arg0, s32 arg1);
extern void Func_0808a348(s32 arg0);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern s32 Func_02004260(s32 template_id, s32 x, s32 y, s32 z);

extern u8 *Data_03001ebc;

void Func_02002400(void)
{
    s32 step;
    s32 object;
    s32 record;

    Func_0808a018();
    Func_080f9010(141);

    step = 0;
    do {
        Func_0808a330(0x004039d2, 1);
        Func_0808a348(8);
        Func_0808a010(8);
        Func_0808a330(0x10000, 1);
        Func_0808a348(8);
        Func_0808a010(8);
        if (step == 1)
            Func_080091f0(0x10000, 0x10000, 0x10000);
        step = (u8)(step + 1);
    } while (step != 6);

    Func_080f9010(0x121);
    Func_080091f0(-1, -1, 0xe666);
    Func_08009180(0, 40, 13, 46, 3, 3);
    Func_0808a010(20);

    object = Func_02004260(222, 0xe80000, 0x100000, 0x900000);
    Func_0808a010(40);
    Func_08015120(object, 1);
    Func_08015040(0x1078, 1);

    Func_0808a0f0(5, 0x1330000, 0x1150000);
    Func_0808a0f0(9, 0x1330000, 0x1150000);
    Func_0808a0f0(11, 0x1330000, 0x1150000);
    Func_0808a0f0(10, 0x1330000, 0x1150000);
    Func_0808a0f0(14, 0x1330000, 0x1150000);

    Func_0808a090(0, 0x13333, 0x9999);
    Func_0808a0d0(0, 232, 156);
    Func_0808a010(10);

    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));

    Func_0808a090(1, 0x13333, 0x9999);
    Func_0808a0d0(1, 218, 172);
    Func_0808a150(1, 0, 0);
    Func_0808a010(20);

    Func_080f9010(145);
    Func_080091f0(0x40000, 0x40000, 0x10000);
    Func_0808a010(20);
    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a010(40);
    Func_0808a1b8(0, 0xd000, 0);
    Func_0808a1b8(1, 0x5000, 50);

    Func_080f9010(144);
    Func_080091f0(0x30000, 0x30000, 0x10000);
    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a1b8(1, 0, 50);
    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(1, 0x8000, 50);
    Func_0808a1b8(0, 0xb000, 0);
    Func_0808a1b8(1, 0xd000, 0);

    Func_080f9010(144);
    Func_080091f0(0x30000, 0x30000, 0x10000);
    Func_0808a010(30);
    Func_0808a128(0, 2, 0);
    Func_0808a128(1, 2, 20);
    Func_0808a128(0, 6, 0);
    Func_0808a128(1, 6, 40);

    *(s32 *)(Data_03001ebc + 448) = 256;

    Func_0808a368();
    Func_0808a370();
    Func_0808a248(2);
}
