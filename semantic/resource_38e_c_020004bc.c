#include "types.h"

/*
 * resource_38e owner at 0x020004bc, 80 bytes: the overlay's ENTRY-0 root -
 * seat scene entity 8 in its idle presentation and, when the shared scene
 * workspace says this is scene 0x22, run the scene body.
 *
 * The image's offset 0 is an exported-entry veneer table of the documented
 * `ldr r4,[pc,#0] / bx r4 / .word <target>` shape.  Its six words are
 * 0x020084bd, 0x02008089, 0x02008095, 0x0200809d, 0x02008155 and 0x02008091;
 * under the proven 0x02008000 link base entry 0 is file offset 0x04bc plus the
 * Thumb bit, which is this owner.  That made it the head of the call graph and
 * it was taken first.
 *
 * Link base, proved independently: seven of this overlay's byte-exact siblings
 * (0x02000030, 0x020000f8, 0x0200012c, 0x02000154, 0x02000184, 0x020001a4,
 * 0x020001c4) each appear in `assets/code/resource_38e_overlay.s` as an odd
 * pool word exactly 0x8000 above their file offset - 0x02008031, 0x020080f9,
 * 0x0200812d, 0x02008155, 0x02008185, 0x020081a5, 0x020081c5.  Seven
 * witnesses, one base.
 *
 * Complete owner: `push {r5, lr}` at 0x020004bc through the single epilogue at
 * 0x020004f8.  It ends `pop {r1} / bx r1`, so the return address lands in r1
 * and r0 survives - the owner returns a value, and `movs r0, #0` immediately
 * before it makes that value 0 (HANDOVER section-0 epilogue rule).
 *
 * Pool map from a control-flow walk: 0x020004bc-0x020004fd is reached as
 * instructions; 0x020004fe-0x0200050b is not.  That is 14 bytes - one alignment
 * halfword plus three words (0x03001ebc, 0x02000240, 0x00000022) - not the
 * customary 8 or 12.  66 code + 14 pool = 80, exactly the inventory row's
 * code_bytes and span_bytes.
 *
 * Calls: 2 sites, matching the advertised 2:
 *   Func_0808a080  x1   0x4ce
 *   Func_0200050c  x1   0x4f2   (this overlay's own owner, prologue site)
 *
 * 0x02000240 is the shared cross-overlay scene workspace, and the halfword read
 * here is byte offset 448 = index 224 - the exact cell the documented
 * `Data_02000240[224]` idiom names, read signed and branched on.  It is a RAM
 * address, not a file offset: it lies below the 0x02008000 link band.
 *
 * The handle mask `(flags09 & ~0x0c) | 0x04` is byte-for-byte the same
 * read-modify-write that semantic/overlays/resource_38e_c_02000974.c performs
 * on the same field of the same handle, which cross-validates the layout at no
 * cost.  Note the register trick that produces the mask: r3 is set to 0, stored
 * as flags23, and only then decremented by 13 to become ~0x0c - one register
 * carrying a stored VALUE and then a mask, the same family as the documented
 * displacement/value trap.
 */

/* The shared cross-overlay scene workspace. */
extern s16 Data_02000240[];

/* Pointer CELL holding the per-overlay workspace base, not the workspace. */
extern u8 *Data_03001ebc;

struct SceneHandle {
    u8 unknown_00[9];
    u8 flags09;                     /* 0x09 */
};

struct SceneEntity {
    u8 unknown_00[0x23];
    u8 flags23;                     /* 0x23 */
    u8 unknown_24[0x2c];
    struct SceneHandle *handle;     /* 0x50 */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
struct SceneEntity *Func_02000fc4();
void Func_02000a00(void);
                                         /* scene entity by selector */

                                         /* this overlay, 0x0200050c */

s32 Func_020004bc(void)
{
    u8 *workspace = Data_03001ebc;
    struct SceneEntity *entity;
    struct SceneHandle *handle;

    *(s32 *)(workspace + 448) = 256;            /* 128 << 1 */

    entity = Func_02000fc4(8);
    entity->flags23 = 0;

    handle = entity->handle;
    handle->flags09 = (u8)((handle->flags09 & ~0x0c) | 0x04);

    if (Data_02000240[224] == 0x22) {
        Func_02000a00();
    }

    return 0;
}
