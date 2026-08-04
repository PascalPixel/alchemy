#include "types.h"

/*
 * resource_3a4 owner at 0x02002f10, 236 bytes: one straight-line cutscene
 * beat with no branches at all -- open the scene, place and aim the camera,
 * position two actors, play a sound, install a per-frame task, tint,
 * wait 60 frames, set a loader byte, then arm the next scene and run it.
 * Nineteen calls and not one conditional.
 *
 * A PUBLISHED CALLBACK (sweep B): no `bl` in the image reaches it, so it
 * is invisible to a whole-image call-graph walk and to overlay_unindexed.
 *
 * Complete owner: `push {lr}` at 0x02002f10 through the interworking
 * return `pop {r0} / bx r0` at 0x02002fd4-0x02002fd6, followed by the
 * nine-word literal pool 0x02002fd8-0x02002ffb; the next owner is the
 * already-drafted 0x02002ffc. measureSpan reports 200, undersizing by the
 * pool's 36 bytes -- the eighth row on this overlay to run past it.
 * No arguments, void: r0 is not read before its first store.
 *
 * INSTALLED POINTER, and it lands inside this same overlay. The pool word
 * at 0x02002fe4 is 0x0200aeed, which is image offset 0x2eec with the Thumb
 * bit -- resource_3a4's own 0x02002eec, already drafted as
 * resource_3a4_c_02002eec.c. This row hands it to Func_080000d0 with
 * 0xc80, the standard 200-frame rate resource_3a4_c_02002bb0.c also uses.
 * So 0x02002eec's own membership in sweep B is explained by this row: it
 * is installed, never called. Same mechanism as 0x02002b58 publishing
 * 0x02002a48, a second instance of it inside one overlay.
 *
 * Nineteen call sites, every one resolved with the `+2` rule via
 * `bun tools/overlay_call_targets.ts resource_3a4 0x2f10 0x2ffc` and none
 * read from the listing's arrows, which print nineteen nonexistent
 * 0x02006xxx addresses. They collapse onto sixteen veneers, in body order:
 *   0x2002f12 -> Func_0808a018()                       no arguments materialised
 *   0x2002f1a -> Func_0808a208(0x9999, 0x1333)
 *   0x2002f2c -> Func_0808a210(0x1480000, -1, 0x570000, 1)
 *   0x2002f36 -> Func_0808a090(0, 0x9999, 0x4ccc)
 *   0x2002f42 -> Func_0808a0d0(0, 328, 116)
 *   0x2002f48 -> Func_080f9010(148)                    established, a sound id
 *   0x2002f52 -> Func_080000d0(0x0200aeed, 0xc80)      the installed pointer above
 *   0x2002f62 -> Func_080091f0(0x10000, 0x10000, 0x10000)
 *   0x2002f6c -> Func_0808a090(8, 0x1999, 0xccc)
 *   0x2002f76 -> Func_0808a090(9, 0x1999, 0xccc)
 *   0x2002f7e -> Func_0808a100(8, 2)
 *   0x2002f8a -> Func_0808a0b8(8, 328, 104)
 *   0x2002f96 -> Func_0808a0b8(9, 328, 108)
 *   0x2002f9c -> Func_0808a010(60)                     established (frames)
 *   0x2002fa8 -> Func_0808a1e8(0, 256, 0)
 *   0x2002fb0 -> Func_0808a130(0, 2)
 *   0x2002fb6 -> Func_0808a0e8(8)
 *   0x2002fc8 -> Func_0808a260(0x56, 99)               established, arm scene (id, mode)
 *   0x2002fd0 -> Func_0808a250(53, 3)                  established, run cutscene (group, variant)
 * Three sites share one bit-identical encoding for Func_0808a090 and two
 * for Func_0808a0b8; the listing gives each a different address.
 *
 * The loader write is a BYTE, not an element. `r3 = 0x02000240 + 0x22b`
 * then `strb #3` -- 0x22b is 555, an ODD byte offset into the s16 array
 * the tree spells as Data_02000240, so it is the high byte of element 277
 * and cannot be written through the array form without misstating it. It
 * is expressed as a byte store through the array's address. Which half of
 * which field that is, this row does not establish.
 *
 * Every constant is transcribed as the instruction pair builds it: 164<<17,
 * 174<<15, 128<<9, 164<<1 and 200<<4 are written in that form rather than
 * flattened, because the shifted-literal shape is what the encoding shows.
 */

extern s16 Data_02000240[];

extern void Func_0808a018(void);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_080f9010(s32 sound_id);
extern void Func_080000d0(s32 callback, s32 rate);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a010(s32 frames);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a260(s32 group, s32 arg1);
extern void Func_0808a250(s32 group, s32 arg1);

void Func_02002f10(void)
{
    Func_0808a018();
    Func_0808a208(0x9999, 0x1333);
    Func_0808a210(164 << 17, -1, 174 << 15, 1);
    Func_0808a090(0, 0x9999, 0x4ccc);
    Func_0808a0d0(0, 164 << 1, 116);
    Func_080f9010(148);
    Func_080000d0(0x0200aeed, 200 << 4);
    Func_080091f0(128 << 9, 128 << 9, 128 << 9);
    Func_0808a090(8, 0x1999, 0xccc);
    Func_0808a090(9, 0x1999, 0xccc);
    Func_0808a100(8, 2);
    Func_0808a0b8(8, 164 << 1, 104);
    Func_0808a0b8(9, 164 << 1, 108);
    Func_0808a010(60);
    Func_0808a1e8(0, 128 << 1, 0);
    Func_0808a130(0, 2);
    Func_0808a0e8(8);

    ((u8 *)Data_02000240)[0x22b] = 3;

    Func_0808a260(0x56, 99);
    Func_0808a250(53, 3);
}
