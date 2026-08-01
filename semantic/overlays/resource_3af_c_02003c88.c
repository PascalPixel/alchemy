typedef unsigned char u8;
typedef int s32;

/*
 * resource_3af owner at 0x02003c88, 680 bytes: a party-facing dialogue
 * scene with a repeat-prompt loop. It sets story flag 0x903 on the way
 * out.
 *
 * THIS OWNER IS IN NO LIST AT ALL -- not a queue row, not in
 * assets/code, not in out/decomp/overlays.json, and not reachable by
 * any `bl` in the overlay. It is Ivan's resource_380 lesson repeating:
 * an owner can be absent from every inventory. Here is how it was
 * bounded, since the method is the transferable part:
 *
 *   1. The 0x02003a0c draft's pool ended at 0x02003c87 and the next
 *      halfword was `push {r5, r6, lr}` -- a prologue with no owner.
 *   2. A whole-image sweep of every BL, resolved with the +2 rule and
 *      classified, found exactly ONE prologue target not already owned
 *      (0x02002c84). So this function is NOT called from within the
 *      overlay, which is why no call-graph walk finds it.
 *   3. Searching the image for its linked address instead --
 *      0x02003c88 + 0x8000 with the Thumb bit, i.e. the word
 *      0x0200bc89 -- found it PUBLISHED at 0x02005a98 and 0x02005ab0,
 *      two entries of a script-record table near the image end. It is a
 *      script callback, installed by data rather than called by code.
 *
 * The same sweep bounds the rest of the overlay: above 0x02004218 there
 * is exactly one prologue-shaped halfword in the whole remaining
 * 0x1914 bytes (0x02004218 itself, already banked), so the tail is
 * veneers, pointer tables and data with no further owners hiding in it.
 *
 * Structure. Opens the scripted-scene bracket, dresses slot 0, then
 * places slots 1, 2 and 3 at slot 0's own position fields through three
 * copies of the null-guarded idiom this overlay uses in 0x02002618 and
 * 0x02002c84 as well:
 *
 *     record = Func_0808a080(0);
 *     if (record != 0)
 *         Func_0808a0f0(n, *(s32 *)(record + 8), *(s32 *)(record + 16));
 *
 * The record is re-fetched and the guard re-evaluated for each n; it is
 * not hoisted.
 *
 * The middle is a NESTED skip-beat construct, and it is the reason to
 * read this one carefully:
 *
 *     if (Func_0808a070(0, 0) == 1) {
 *         ... one block of party reactions ...
 *         while (Func_0808a070(0, 0) == 1) {
 *             Func_0808a138(2, 1);
 *             Func_0808a170(0x1f53);
 *             Func_0808a178(2, 0);
 *         }
 *     }
 *
 * Both tests are the same call with the same arguments, and both
 * failures land on the same address (0x02003e1e). The inner one is a
 * genuine loop -- the `b.n 0x02003dfa` at 0x02003e1c is backward, to
 * the test itself -- so it is a repeat-prompt poll of the kind first
 * seen in resource_39e's 0x27ec, here re-asking until the player stops
 * choosing the same option. The outer one is an ordinary guard. Read
 * only the two `bne`s and it looks like two independent guards; the
 * back edge is what distinguishes them.
 *
 * The close pairs two read-modify-write bytes per actor on record +90:
 * clear bit 0, wait one frame, set bit 0 -- done for actor 22 and then
 * actor 21, each with a Func_0808a0d0 between. The masks come from
 * `movs r5,#254` and `movs r6,#1`, and both registers are then reused
 * as the accumulators for the second actor's pair, so r5 stops holding
 * 0xfe partway through. Then three Func_0808a0c8/Func_0808a0d0 calls
 * and Func_0808a088 on slots 1, 2 and 3, story flag 0x903 set, and
 * Func_0808a020 closes the bracket.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02003c88 through
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x02003f0a-0x02003f0e; eight
 * trailing pool words (0x0000028e, 0x00013333, 0x00009999, 0x00001f55,
 * 0x00001f53, 0x00001f5b, 0x0000027a, 0x00000903) end at 0x02003f2f,
 * and the next owner prologue (`push {r5, r6, lr}` plus its r8 spill,
 * the drafted 0x02003f30) begins at 0x02003f30.
 *
 * Second read done (over 400 bytes): the correction was the nested
 * construct above. A first pass read the two `bne`s as a flat
 * `if (a) {...} else if (b) {...}` chain because both branch to the
 * same target, and missed that the second test is a loop head with a
 * back edge into it.
 *
 * Uncertainty: slot ids and argument roles by shape; the four dialogue
 * ids and the record +90 bit are transcribed, meanings unknown.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a010(s32 frames);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a088(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_080770c8(s32 flagId);
extern void Func_02003a00(s32 id, s32 arg1);
extern void Func_020039ec(s32 packedId);

void Func_02003c88(void)
{
    u8 *record;

    Func_0808a018();
    Func_0808a090(0, 128 << 9, 128 << 8);
    Func_0808a0d0(0, 180, 0x28e);
    Func_0808a1b8(0, 128 << 8, 0);

    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }

    Func_0808a090(1, 0x13333, 0x9999);
    Func_0808a090(2, 128 << 9, 128 << 8);
    Func_0808a090(3, 0x13333, 0x9999);
    Func_0808a0c8(1, 194, 160 << 2);
    Func_0808a0c8(2, 198, 0x28e);
    Func_0808a0d0(3, 194, 168 << 2);
    Func_0808a100(1, 1);
    Func_0808a100(2, 1);
    Func_0808a010(10);
    Func_0808a1b8(1, 128 << 8, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_02003a00(3, 128 << 8);

    Func_02003a00(22, 0);
    Func_0808a170(0x1f55);
    Func_020039ec(22);
    Func_02003a00(21, 208 << 8);
    Func_0808a188(21, 0, 40);
    Func_0808a1e8(22, 128 << 1, 20);
    Func_0808a138(22, 1);
    Func_0808a178(22, 0);

    if (Func_0808a070(0, 0) == 1) {
        Func_0808a110(2, 4);
        Func_020039ec(2);
        Func_02003a00(3, 160 << 8);
        Func_0808a100(3, 3);
        Func_020039ec(3);
        Func_02003a00(1, 192 << 7);
        Func_0808a138(1, 1);
        Func_0808a178(1, 0);

        while (Func_0808a070(0, 0) == 1) {
            Func_0808a138(2, 1);
            Func_0808a170(0x1f53);
            Func_0808a178(2, 0);
        }
    }

    Func_0808a010(20);
    Func_0808a110(22, 3);
    Func_0808a170(0x1f5b);
    Func_020039ec(22);
    Func_0808a090(22, 128 << 9, 128 << 8);
    Func_0808a090(21, 128 << 9, 128 << 8);

    record = Func_0808a080(22);
    record[90] &= 0xfe;
    Func_0808a0d0(22, 162, 0x27a);
    Func_0808a010(1);
    record = Func_0808a080(22);
    record[90] |= 1;

    record = Func_0808a080(21);
    record[90] &= 0xfe;
    Func_0808a0d0(21, 162, 169 << 2);
    Func_0808a010(1);
    record = Func_0808a080(21);
    record[90] |= 1;

    Func_0808a1b8(22, 192 << 6, 0);
    Func_02003a00(21, 208 << 8);
    Func_020039ec(22);
    Func_0808a0c8(1, 180, 0x28e);
    Func_0808a0c8(2, 180, 0x28e);
    Func_0808a0d0(3, 180, 0x28e);
    Func_0808a088(1);
    Func_0808a088(2);
    Func_0808a088(3);
    Func_080770c8(0x903);
    Func_0808a020();
}
