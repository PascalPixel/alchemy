#include "types.h"
#include "staged_actor.h"
#include "staged_actor_probe_state.h"

extern u8 *Data_03001ebc;
extern s32 Data_02008ec8[];
extern s32 Data_02008f08[];
extern s32 Data_02008f20[];
extern struct StagedActor *Func_0200119e(s32 actor);

#define StagedActorDirectionSteps Data_02008ec8
#define StagedActorProbeValues Data_02008f08
#define StagedActorProbeBounds Data_02008f20
#define GetStagedActor Func_0200119e
#define FindStagedActorAtDirectionalProbe Func_0200034c

struct StagedActorProbeBounds {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
};

struct StagedActor *FindStagedActorAtDirectionalProbe(
    s32 *direction_out, s32 *slot_out, s32 *probe_out)
{
    u8 *runtime;
    struct StagedActor *query;
    struct StagedActor **slot_cursor;
    u32 direction;
    u32 slot;
    struct StagedActor *actor;
    s32 probe_value;
    s32 *wanted_probe;
    u32 probe;
    struct StagedActorProbeBounds *bounds;
    s32 step;
    s32 query_x_value;
    s32 query_z_value;
    s32 query_x;
    s32 query_z;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;

    runtime = Data_03001ebc;
    query = GetStagedActor(0);
    direction = query->direction_and_kind >> 12;
    *direction_out = direction;
    slot = 8;
    slot_cursor = (struct StagedActor **)(runtime + 0x34);
    do {
        actor = *slot_cursor;
        probe_value = *(STAGED_ACTOR_PROBE_DETAILS(actor)->unknown_28);
        wanted_probe = StagedActorProbeValues;
        probe = 0;
        bounds = (struct StagedActorProbeBounds *)StagedActorProbeBounds;

        for (; probe <= 5; probe++, bounds++) {
            if (probe_value == *wanted_probe++) {
                *probe_out = probe;

                step = StagedActorDirectionSteps[*direction_out];
                query_x_value = query->x.value;
                query_x = ((query_x_value >> 16) + (step >> 16)) >> 4;
                query_z_value = query->z.value;
                query_z = ((query_z_value >> 16) + (s16)step) >> 4;
                left = (actor->x.parts.cell + bounds->left) >> 4;
                top = (actor->z.parts.cell + bounds->top) >> 4;
                right = (actor->x.parts.cell + bounds->right) >> 4;
                bottom = (actor->z.parts.cell + bounds->bottom) >> 4;

                if (left > query_x || query_x >= right ||
                    top > query_z || query_z >= bottom) {
                    continue;
                }
                if ((probe & 1) != 0) {
                    if (left == query_x_value >> 20) continue;
                } else if (top == query_z_value >> 20) {
                    continue;
                }
                *slot_out = slot;
                return actor;
            }
        }

        slot++;
        slot_cursor++;
    } while (slot <= 65);

    return 0;
}
