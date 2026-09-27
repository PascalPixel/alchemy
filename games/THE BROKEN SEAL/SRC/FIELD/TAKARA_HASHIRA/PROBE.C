/* Query the sprite footprint and translate its probe into map cells.
 * The extent temporary is reused for the translated z coordinate; keep
 * the fixed-to-cell conversion in that same local before publication.
 * Exact full extent: 236 bytes, including all three literal-pool words.
 * 2026-09-27: a separate shifted result left four halfwords different;
 * shifting the reused coordinate in place closes the complete owner. */
#include "STAGED_ACTOR.H"

struct StagedActor *Engine_ActorGet(s32 actor);

extern u8 *Data_03001e70;
extern s32 Data_0200ace0[];
extern struct StagedActorFootprint Data_0200acf8[];

s32 FieldScene_QueryActorFootprint(s32 id, s32 *width, s32 *depth, struct StagedActorProbe *probe, s32 *left, s32 *top)
{
    u8 *map = Data_03001e70;
    struct StagedActor *actor = Engine_ActorGet(id);
    u32 i;
    s32 a;
    s32 b;

    i = 0;
    if (*STAGED_ACTOR_PROBE_DETAILS(actor)->unknown_28 != Data_0200ace0[i]) {
    miss:
        probe->footprint_index = 7;
        if (++i > 5) {
            goto check;
        }
        if (*STAGED_ACTOR_PROBE_DETAILS(actor)->unknown_28 != Data_0200ace0[i]) {
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
    a = Data_0200acf8[probe->footprint_index].z0;
    if (a < 0) {
        a = -a;
    }
    b = Data_0200acf8[probe->footprint_index].z1;
    if (b < 0) {
        b = -b;
    }
    *depth = (a + b) >> 4;
    a = Data_0200acf8[probe->footprint_index].x0;
    if (a < 0) {
        a = -a;
    }
    b = Data_0200acf8[probe->footprint_index].x1;
    if (b < 0) {
        b = -b;
    }
    *width = (a + b) >> 4;
    probe->position_x += Data_0200acf8[probe->footprint_index].x0 << 16;
    {
        s32 step = Data_0200acf8[probe->footprint_index].z0 << 16;

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
