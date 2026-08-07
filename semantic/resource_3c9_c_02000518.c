#include "types.h"

/*
 * resource_3c9 owner at 0x02000518, 212 bytes: a probe-then-act step on
 * scene record 0. It builds a three-word candidate position in a stack
 * scratch buffer -- record 0's own +8/+12/+16 triple, with the first
 * and third snapped to a coarse grid -- offsets that candidate by a
 * quantised heading taken from the record's +6 halfword, and hands the
 * buffer to Func_080091d8. When that probe returns ZERO the row runs a
 * fixed act sequence; when it returns nonzero the row does nothing at
 * all.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + `mov r7, r8 / push {r7}` +
 * `sub sp, #12` at 0x02000518 through the single epilogue `add sp, #12
 * / pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x020005dc-0x020005e6, then a one-word literal pool at 0x020005e8
 * (0xfff00000) filling the span exactly to 0x020005ec, where the
 * exact-C owner 0x020005ec begins. No alignment halfword. Measured to
 * the epilogue.
 *
 * `arg0` is never read before being overwritten (r0 takes the constant
 * 0 at 0x0200051e), so void in, void out.
 *
 * Call sites resolved with `bun tools/lib/overlay_call_targets.ts
 * resource_3c9 518 5ec`'s `+2` rule: `sites=13 distinct_targets=10
 * veneer=12 prologue=1`, and thirteen are transcribed. The one
 * non-veneer site is an in-overlay call to this overlay's own exact-C
 * owner Func_020006c0.
 *
 * Published, not called: the address appears as the in-image spelling
 * 0x02008518 in two words, at overlay offsets 0x66f4 and 0x670c. Both
 * lie in the data tables past the veneer bank, not inside any owner --
 * so unlike every other published owner in this overlay, this one is
 * installed from a TABLE rather than by a spawner, and no drafted row
 * points at it.
 *
 * The +85 byte is SAVED on entry and RESTORED on the way out, and in
 * between it is masked with 126 -- clearing bits 0 and 7 for the
 * duration of the act sequence and then putting the original byte back
 * verbatim. That save/restore is the clearest single fact about the
 * row: whatever bits 0 and 7 gate, this sequence must run with them
 * clear and must not be observed to have changed them.
 *
 * Uncertainties:
 *   - `& 0xfff00000` then `+ 0x80000` on the +8 and +16 fields is a
 *     snap to a 0x100000 grid landing on cell centres; the +12 field is
 *     copied unsnapped. Which axis is which is not established.
 *   - Func_08000128 takes (0x200000, heading, buffer) and the buffer is
 *     read back afterwards at +2 and +10 as SIGNED halfwords, so it is
 *     in-out; nothing here establishes what it computes.
 *   - Func_080091d8's return is treated only as a boolean gate. Whether
 *     zero means "clear" or "failed" is not established, and the row is
 *     written to say only which way the branch goes.
 *   - the constants 592, 6, 7, 152, 0x30000, 0x20000, 0x40000 are
 *     transcribed in the shifted form the row builds them in.
 */

extern u8 *Func_0808a080(s32 index);   /* scene-record accessor */
extern void Func_08000128(s32 scale, s32 heading, s32 *buffer);
extern s32 Func_080091d8(u8 *record, s32 *buffer);
extern void Func_080770d0(s32 id);
extern void Func_020006c0(void);
extern void Func_08009080(u8 *record, s32 mode);
extern void Func_080000c0(s32 id);
extern void Func_080f9010(s32 id);
extern void Func_080091e0(u8 *record, s32 mode);
extern void Func_0808a0c0(s32 index, s32 a, s32 b);

void Func_02000518(void)
{
    s32 probe[3];
    u8 *record = Func_0808a080(0);
    u8 *flags = record + 85;
    s32 heading = (*(u16 *)(record + 6) + (128 << 5)) & (224 << 8);
    u8 saved = *flags;

    probe[0] = (*(s32 *)(record + 8) & (s32)0xfff00000) + (128 << 12);
    probe[1] = *(s32 *)(record + 12);
    probe[2] = (*(s32 *)(record + 16) & (s32)0xfff00000) + (128 << 12);

    Func_08000128(128 << 14, heading, probe);

    if (Func_080091d8(record, probe) != 0) {
        return;
    }

    Func_080770d0(148 << 2);
    Func_020006c0();
    Func_08009080(record, 6);
    Func_080000c0(6);
    Func_08009080(record, 7);

    *(s32 *)(record + 48) = 192 << 10;
    *(s32 *)(record + 52) = 128 << 10;

    Func_080f9010(152);

    *(s32 *)(record + 40) = 128 << 11;
    *flags = (u8)(*flags & 126);

    Func_080091e0(record, 0);
    Func_0808a0c0(0, *((s16 *)probe + 1), *((s16 *)probe + 5));
    Func_08009080(record, 6);
    Func_080091e0(record, 1);

    *flags = saved;
}
