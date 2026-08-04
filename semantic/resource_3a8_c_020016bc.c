#include "types.h"

/*
 * resource_3a8 owner at 0x020016bc, 300 bytes: the scene's entry fix-up -
 * settle the story flags for this room, place its fixed props, and either skip
 * entity 26 or bring it up in its idle presentation and start its task.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7, r8 / push {r7}` at
 * 0x020016bc through the single interworking epilogue at 0x020017bc
 * (`pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} / bx r0`), followed by
 * an alignment halfword and its eight-word literal pool at
 * 0x020017c8-0x020017e7.  The pool is data, not code, and is excluded.  There
 * is no stack frame; nothing is live past the return.
 *
 * Signature.  The epilogue is `pop {r0} ; bx r0`, so r0 holds the popped
 * return address and the owner returns nothing: `void`.  r0 is written from a
 * pool word before any read and r1-r3 are never read before being written, so
 * it takes no arguments.
 *
 * Call accounting: 17 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3a8 16bc` - 10 distinct import
 * veneers, no intra-overlay call.  The disassembler's own annotations are
 * wrong in the usual overlay way (an overlay `bl` stores `target_offset - 2`)
 * and were not used.  In body order:
 *   0x020016c4, 0x020016e8, 0x0200171e -> 0x02003bd4 -> Func_080770c0 (3)
 *   0x020016ce, 0x020016d4, 0x020016da, 0x020016e0, 0x020016f2
 *                                       -> 0x02003bdc -> Func_080770c8 (5)
 *   0x02001710, 0x0200172e              -> 0x02003c74 -> Func_0808a0f0 (2)
 *   0x02001718 -> 0x02003cac -> Func_0808a158
 *   0x02001736 -> 0x02003c1c -> Func_0808a080
 *   0x02001780 -> 0x02003b44 -> Func_08000140
 *   0x02001788 -> 0x02003bcc -> Func_08015250
 *   0x02001798 -> 0x02003b54 -> Func_080001c8
 *   0x0200179e -> 0x02003b4c -> Func_08000150
 *   0x020017b8 -> 0x02003b1c -> Func_080000d0
 * The per-target counts match the tool's exactly.
 *
 * LINK BASE 0x02008000, confirmed again by the cheapest witness there is: the
 * pool word 0x0200b90d handed to Func_080000d0 is 0x390c + the Thumb bit, and
 * file offset 0x390c is Func_0200390c, which has a BYTE-EXACT source in
 * assets/code/resource_3a8_c_0200390c.c.  So it is the per-frame task being
 * installed, not data.  The other pool words are story-flag ids and 0x02000240,
 * which the byte-exact sibling assets/code/resource_3a8_c_0200164c.c already
 * spells as `extern s16 Data_02000240[]`; that name and type are reused here.
 *
 * The tail from Func_08000140 onwards is the same palette-ramp idiom the
 * byte-exact-derived semantic sibling for 0x02003a3c documents: reserve 0x608
 * bytes on channel 17, run Func_08015250(0xb5) for its shared-workspace side
 * effect (its result is discarded), upload a 0x80-entry ramp starting 0x400
 * into the reservation, then commit the channel.
 *
 * UNCERTAINTIES.
 *  - `handle->field27` and `entity->field55` are stored from r8, which on that
 *    path is the Func_080770c0(0x916) result and is provably 0 there - the
 *    branch is taken only when it compares equal to zero.  They are written as
 *    0 below, matching the same preamble in the 0x02003a3c sibling.
 *  - `entity->field30` is likewise stored from r8, i.e. 0.
 *  - Func_0808a158(21, 2) and Func_0808a0f0's third argument are used with the
 *    same shapes elsewhere in the tree (a selector plus a value, and a 16.16
 *    coordinate pair) but neither import's full interface is established here.
 */

struct SceneHandle {
    u8 unknown_00[5];
    u8 flags05;                     /* 0x05 */
    u8 unknown_06[3];
    u8 flags09;                     /* 0x09 */
    u8 unknown_0a[0x12];
    u8 paletteIndex;                /* 0x1c */
    u8 unknown_1d[0xa];
    u8 field27;                     /* 0x27 */
};

struct SceneEntity {
    u8 unknown_00[8];
    s32 x;                          /* 0x08, 16.16 */
    s32 y;                          /* 0x0c, 16.16 */
    s32 z;                          /* 0x10, 16.16 */
    u8 unknown_14[0x1c];
    s32 field30;                    /* 0x30 */
    u8 unknown_34[4];
    s32 field38;                    /* 0x38, copy of x */
    s32 field3c;                    /* 0x3c, copy of y */
    s32 field40;                    /* 0x40, copy of z */
    u8 unknown_44[0xc];
    struct SceneHandle *handle;     /* 0x50 */
    u8 unknown_54[1];
    u8 field55;                     /* 0x55 */
    u8 unknown_56[6];
    u8 field5c;                     /* 0x5c */
    u8 unknown_5d[4];
    u8 field61;                     /* 0x61 */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
s32 Func_080770c0();                    /* read a story flag */
void Func_080770c8();                   /* set a story flag */
void Func_0808a0f0();                   /* place an entity at (x, z) */
void Func_0808a158();                   /* select an entity presentation */
struct SceneEntity *Func_0808a080();    /* scene entity by selector */
s32 Func_08000140();                    /* reserve workspace on a channel */
void Func_08000150();                   /* commit the channel reservation */
void Func_080001c8();                   /* upload a palette ramp */
s32 Func_08015250();                    /* shared-workspace side effect */
void Func_080000d0();                   /* install a per-frame task */

/* Room state table; the byte-exact sibling for 0x0200164c reads it the same
 * way, as signed halfwords from 0x02000240. */
extern s16 Data_02000240[];

void Func_020016bc(void)
{
    struct SceneEntity *entity;
    struct SceneHandle *handle;
    s32 skip;
    s32 gradient;

    if (Func_080770c0(0x941) != 0) {
        Func_080770c8(0x321);
        Func_080770c8(0x913);
        Func_080770c8(0x912);
        Func_080770c8(0x915);
    }

    if (Func_080770c0(0x940) != 0) {
        Func_080770c8(0x321);
    }

    if (Data_02000240[225] == 14) {
        Func_0808a0f0(25, 0x01a80000, 0x00580000);
    }

    Func_0808a158(21, 2);

    skip = Func_080770c0(0x916);
    if (skip != 0) {
        Func_0808a0f0(26, 0, 0);
        return;
    }

    entity = Func_0808a080(26);
    handle = entity->handle;

    handle->flags05 = (u8)(handle->flags05 & ~0x20);
    handle->flags09 = (u8)(((handle->flags09 & ~0x0c) | 0x04) & 0x0f);
    handle->field27 = 0;            /* stored from r8, which is 0 here */

    entity->field5c = 1;
    entity->field55 = 0;            /* stored from r8, which is 0 here */
    entity->y = 0x000a0000;
    entity->field61 = 1;

    gradient = Func_08000140(17, 0x608);
    Func_08015250(0xb5);
    gradient += 0x400;
    Func_080001c8(handle->paletteIndex, 0x80, gradient);
    Func_08000150(17);

    entity->field30 = 0;            /* stored from r8, which is 0 here */
    entity->field38 = entity->x;
    entity->field3c = entity->y;
    entity->field40 = entity->z;

    /* 0x0200b90d = file offset 0x390c + the Thumb bit = Func_0200390c, the
     * byte-exact task in assets/code/resource_3a8_c_0200390c.c. */
    Func_080000d0((void (*)(void))0x0200b90d, 3200);
}
