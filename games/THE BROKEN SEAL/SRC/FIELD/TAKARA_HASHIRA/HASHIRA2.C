#include "GLOBAL_CELLS.H"
#include "HASHIRA.H"
/* Query the sprite footprint and translate its probe into map cells.
 * The extent temporary is reused for the translated z coordinate; keep
 * the fixed-to-cell conversion in that same local before publication.
 * Exact full extent: 236 bytes, including all three literal-pool words.
 * 2026-09-27: a separate shifted result left four halfwords different;
 * shifting the reused coordinate in place closes the complete owner. */
#include "STAGED_ACTOR.H"
#include "TYPES.H"
#include "DMA.H"

extern s32 StagedActor_FootprintKinds[];
extern struct StagedActorFootprint StagedActor_FootprintBounds[];


struct MapLayer {
    u32 *cells;
    u8 pad04[44];
};

struct MapState {
    u8 pad000[0x130];
    struct MapLayer layers[1];
};

/* The size test is unsigned, so out20 and out16 are u32. The two rec words
 * that travel on the stack are read into their own locals, which puts both
 * loads before both stores. The +0x23 flag set takes its mask from a local
 * assigned first, which fixes the register the mask lands in. */

/*
 * Apply a placement query and tag the object. FieldScene_QueryActorFootprint fills out20 and
 * out16, a 24-byte record of which only rec[2] and rec[4] are read back, and
 * two further out-params whose written values are never read -- only the
 * pointers matter. On success the object is tagged at +0x23 and one of two
 * near-identical finishing calls runs; on failure the object is untouched.
 */

/* The callees are declared at their in-overlay entry points. Declaring them at
 * their veneer addresses in ROM space would route every call through a
 * veneer. */

/*
 * Apply a placement query to an actor. FieldScene_QueryActorFootprint is an out-param helper:
 * it fills out20 and out16, a 24-byte record of which only rec[2] and rec[4]
 * are read back, and out12 and out8 passed on the stack. Its field semantics
 * are not established. On success the values thread unchanged into the two
 * finishing calls in that shape; on failure the object is left untouched.
 */

/*
 * Wait at most sixty polls for the object's +12 to reach its +20, then clear
 * +0x28, set +0x3c, and mirror +20 back into +12. Engine_TaskWait(1) is taken
 * to be a one-frame wait, and the field offsets are named by position and
 * not verified.
 */
void OverlayObject_WaitUntilSettledAndReset(u8 *obj)
{
    s32 cnt = 60;

    for (;;) {
        if (cnt == 0) {
            break;
        }
        WaitFrames(1);
        if (*(u32 *)(obj + 12) == *(u32 *)(obj + 20)) {
            break;
        }
        cnt--;
    }

    *(u32 *)(obj + 0x28) = 0;
    *(u32 *)(obj + 0x3c) = 0x80000000;
    *(u32 *)(obj + 12) = *(u32 *)(obj + 20);
}

/*
 * Polls for up to sixty ticks until the height at +12 falls to the target
 * at +20 or to limit, then clears +0x28 and parks +0x3c. The height is not
 * mirrored back from +20 afterwards. The owner at 0x02000da8 is 52 bytes
 * and carries no pool.
 */
void SceneActor_WaitHeightBelowLimit(u8 *obj, s32 limit)
{
    s32 cnt = 60;

    for (;;) {
        if (cnt == 0) {
            break;
        }
        WaitFrames(1);
        if (*(s32 *)(obj + 12) <= *(s32 *)(obj + 20)) {
            break;
        }
        if (*(s32 *)(obj + 12) <= limit) {
            break;
        }
        cnt--;
    }

    *(u32 *)(obj + 0x28) = 0;
    *(u32 *)(obj + 0x3c) = 0x80000000;
}

s32 FieldScene_QueryActorFootprint(s32 id, s32 *width, s32 *depth, struct StagedActorProbe *probe, s32 *left, s32 *top)
{
    u8 *map = gMapWork[0];
    struct StagedActor *actor = (struct StagedActor *)Object_GetById(id);
    u32 i;
    s32 a;
    s32 b;

    i = 0;
    if (actor->animation->entries[0]->anim_id != StagedActor_FootprintKinds[i]) {
    miss:
        probe->footprint_index = 7;
        if (++i > 5) {
            goto check;
        }
        if (actor->animation->entries[0]->anim_id != StagedActor_FootprintKinds[i]) {
            goto miss;
        }
    }
    probe->footprint_index = i;
check:
    if ((u32)probe->footprint_index > 6) {
        return 0;
    }
    probe->position_x = actor->x.value;
    probe->position_y = actor->y;
    probe->position_z = actor->z.value;
    a = StagedActor_FootprintBounds[probe->footprint_index].z0;
    if (a < 0) {
        a = -a;
    }
    b = StagedActor_FootprintBounds[probe->footprint_index].z1;
    if (b < 0) {
        b = -b;
    }
    *depth = (a + b) >> 4;
    a = StagedActor_FootprintBounds[probe->footprint_index].x0;
    if (a < 0) {
        a = -a;
    }
    b = StagedActor_FootprintBounds[probe->footprint_index].x1;
    if (b < 0) {
        b = -b;
    }
    *width = (a + b) >> 4;
    probe->position_x += StagedActor_FootprintBounds[probe->footprint_index].x0 << 16;
    {
        s32 step = StagedActor_FootprintBounds[probe->footprint_index].z0 << 16;

        a = probe->position_z;
        a += step;
        a >>= 20;
        probe->position_z = a;
    }
    probe->position_x >>= 20;
    *left = *(s32 *)(map + 0x13c) >> 20;
    *top = *(s32 *)(map + 0x140) >> 20;
    return 1;
}

s32 SceneActor_ApplyPlacementQueryAndTag(u8 *no)
{
    u8 *obj = Actor_Get(no);
    u32 out20, out16;
    s32 out12, out8;
    s32 rec[6];
    s32 r2, r4;
    u8 mask;

    if (FieldScene_QueryActorFootprint(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    r2 = rec[2];
    r4 = rec[4];
    Map_CopyCellAttributes(2, 2, out20, out16, r2, r4);

    Object_SetMode(obj, 4);
    mask = 2;
    obj[0x23] = obj[0x23] | mask;

    if (out20 > out16) {
        Map_CopyCellsTo(70, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    } else {
        Map_CopyCellsTo(68, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    }

    return 1;
}

s32 SceneActor_ApplyPlacementQuery(u8 *no)
{
    u8 *obj = Actor_Get(no);
    s32 out20, out16, out12, out8;
    s32 rec[6];

    if (FieldScene_QueryActorFootprint(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    {
        s32 x = out12 + rec[2];
        s32 z = out8 + rec[4];

        Map_CopyCellAttributes(x, z, out20, out16, rec[2], rec[4]);
        StagedActor_FillGridAttributeRectangle(0, rec[2], rec[4], out20, out16, 255);
    }

    Object_SetMode(obj, 1);
    obj[0x23] &= 0xfd;

    return 1;
}

/* Copy the map cell at x, y of a layer to dst with DMA 3 and wait for it. */
void TakaraHashira_ReadMapCell(s32 layer, s32 x, s32 y, u32 *dst)
{
    struct MapState *map = gMapWork[0];

    if (map != 0) {
        u32 *cell = map->layers[layer].cells;

        cell += x + (y << 7);
        Dma_Set(cell, dst, 0x84000001, (volatile u32 *)0x040000d4);
        {
            volatile u32 *dma = (volatile u32 *)0x040000d4;

            while (dma[2] & 0x80000000)
                ;
        }
    }
}
