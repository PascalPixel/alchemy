typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_397 owner at 0x0200005c, 124 bytes: shift this scene's two tracked
 * objects by a whole-block offset and re-probe the terrain height under each.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7, r8 / push {r7}` at
 * 0x0200005c through `pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} /
 * bx r0` at 0x020000c4-0x020000cc.  The return address is popped into r0, so
 * the owner is `void` (HANDOVER section 0, epilogue rule).  r0 and r1 are both
 * read into r6/r7 before being written, so it takes two arguments.  The 8-byte
 * literal pool at 0x020000d0-0x020000d7 is inside the advertised span and
 * after the epilogue; it holds 0x03001ebc and 0x02000240 and is not code.
 *
 * Link base.  Image offset 0 of this overlay is the exported-entry veneer
 * table (`ldr r4,[pc,#0] / bx r4 / .word`), and all six of its words are odd
 * and land on function starts at `word - 0x8000`: 0x02008201 -> 0x0200 (this
 * overlay's root), 0x02008041 -> 0x0040, 0x0200804d -> 0x004c,
 * 0x02008055 -> 0x0054, 0x020081b5 -> 0x01b4, 0x02008049 -> 0x0048.  Six
 * parity witnesses from one read, so the 0x02008000 base is settled here too.
 * Neither pool word this owner loads is in that band: 0x03001ebc is the
 * well-known IWRAM workspace pointer and 0x02000240 is the shared
 * cross-overlay scene-record block (below the band on the two-sided pool-word
 * test, so a RAM global rather than in-image data).
 *
 * Three call sites, matching the inventory's calls=3.  Resolved with
 * `bun tools/overlay_call_targets.ts resource_397 005c`, which reports
 * sites=3 over TWO distinct targets:
 *   0x02000076 -> veneer 0x0334 -> Func_0808a080   (scene-record accessor)
 *   0x02000096 -> veneer 0x031c -> Func_080091a8   (terrain height probe)
 *   0x020000bc -> veneer 0x031c -> Func_080091a8
 * The printed `bl` annotations (0x020003ac, 0x020003b4, 0x020003da) are the
 * usual overlay decoding artefact -- the stored displacement is the target's
 * image offset minus 2 -- and the last two sites have bit-identical halfwords
 * (`f000 f98d`) yet print different callees, which is that bug's signature.
 * The multiset for this owner is therefore Func_0808a080 x1 and
 * Func_080091a8 x2; the two probe sites are separate arms over separate
 * records and are written out separately rather than folded.
 *
 * Argument registers at the two probe sites.  Only r0 is loaded immediately
 * before each `bl`, but r1 and r2 are still live from the two lines above --
 * they hold the freshly-stored x and z.  So each site is the established
 * three-argument form `Func_080091a8(layer, x, z)` (semantic/main/0808b674.c,
 * semantic/overlays/resource_3c5_c_02002660.c), not a one-argument call.  The
 * result is written to BOTH +12 and +20 of the record.
 *
 * The two records.  The first is `Func_0808a080(*(s32 *)(0x02000240 + 500))`,
 * exactly the accessor call resource_370's 0x02000054 owner makes with the
 * same argument.  The second is the pointer at +480 of the workspace reached
 * through 0x03001ebc.  Either may be null and each is guarded independently;
 * when the first is null control branches straight to the second test, which
 * is why the two bodies are duplicated in the original rather than shared.
 *
 * The shift.  Both arguments are shifted left by 20 before use.  Record
 * coordinates in this engine are 16.16 fixed point, so a shift of 20 rather
 * than 16 means the arguments are counted in units of sixteen tiles -- a
 * block/section step, not a tile step.  Nothing here depends on which unit the
 * caller intends; the shift is reproduced as written.
 *
 * Uncertainties.  The record fields at +12 and +20 both receive the probed
 * height; +12 is the y coordinate used by the sibling sources, and +20 is
 * assumed to be the matching target/settled height because it is written from
 * the same value and never read here.  The byte at +0x22 is the small
 * terrain/collision layer id the rest of the tree passes to Func_080091a8.
 */

/* Overlay imports (via the veneer table).  Old-style declarations: an overlay
 * import can be reached with different argument counts at different sites. */
u8 *Func_0808a080();     /* scene record for a scene-block id, or 0 */
s32 Func_080091a8();     /* terrain height at (layer, x, z) */

struct SceneObject_0200005c {
    u8 unknown_00[8];
    s32 x;                  /* +0x08 */
    s32 y;                  /* +0x0c, receives the probed height */
    s32 z;                  /* +0x10 */
    s32 settled_y;          /* +0x14, receives the same probed height */
    u8 unknown_18[0x0a];
    u8 layer22;             /* +0x22 */
};

/* Shared cross-overlay scene workspace; +500 holds this scene's block id. */
#define SCENE_BLOCK_ID_0200005c (*(s32 *)((u8 *)0x02000240 + 500))

void Func_0200005c(s32 block_x, s32 block_z)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    struct SceneObject_0200005c *object;
    s32 dx = block_x << 20;
    s32 dz = block_z << 20;
    s32 height;

    object = (struct SceneObject_0200005c *)Func_0808a080(SCENE_BLOCK_ID_0200005c);
    if (object != 0) {
        object->x += dx;
        object->z += dz;
        height = Func_080091a8((s32)object->layer22, object->x, object->z);
        object->y = height;
        object->settled_y = height;
    }

    object = *(struct SceneObject_0200005c **)(workspace + 480);
    if (object != 0) {
        object->x += dx;
        object->z += dz;
        height = Func_080091a8((s32)object->layer22, object->x, object->z);
        object->y = height;
        object->settled_y = height;
    }
}
