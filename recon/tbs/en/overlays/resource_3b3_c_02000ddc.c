/* NONMATCHING: 236 of 236 bytes, 5 differing halfwords (2026-09-27).
 * Hand-written: finds the actor's sprite kind in the six-entry table at
 * 0x0200ace0 (7 when absent), then fills the probe with the actor's position
 * moved by that entry's step, the step's extents and the map's scroll cell.
 * Binds Engine_ActorGet (0x0200ab1c) and the three data symbols. Remaining:
 * the z step's add; written z + step, local-alloc gives the step load r2 and
 * z r3 (the reference has them the other way round), written step + z, the
 * sum lands in the step's register.
 *
 * The exact STAGED_NAV callers at 02000ec8 and 02000f58 both call this
 * helper through runtime 02008ddc. Their six-word record is the existing
 * StagedActorProbe, and FieldScene_RedrawActorFootprint proves the shared
 * actor, sprite and footprint views. Using these types leaves the full
 * 236-byte baseline unchanged: 5 halfwords / 4 aligned edits. Copying the
 * sibling's separate z += step; z >>= 20 statements instead gives 8 / 7:
 * local allocation swaps the z and step loads, and the sum still uses r3.
 * Stop the type/subexpression-transfer axis here. The unresolved invariant
 * is the z sum/shift/store in r2 before x is shifted, not its field layout.
 */
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
    probe->position_z = ((Data_0200acf8[probe->footprint_index].z0 << 16) + probe->position_z) >> 20;
    probe->position_x >>= 20;
    *left = *(s32 *)(map + 0x13c) >> 20;
    *top = *(s32 *)(map + 0x140) >> 20;
    return 1;
}
