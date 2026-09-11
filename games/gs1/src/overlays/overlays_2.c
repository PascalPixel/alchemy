#include "types.h"
#include "scene.h"
#include "staged_actor.h"

/* overlays/shared/measure_fixed_point_position_distance.c */
/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

typedef s32 (*IwramIntegerSquareRoot)(s32);

s32 CalculateFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

/* overlays/shared/find_actor_at_fixed_point_position.c */
extern u8 *gWork;

s32 *Sys_Run(s32 *arg0)
{
    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((arg0[0] >> 20) == (p[2] >> 20)
            && (arg0[1] / 0x10000) == (p[3] / 0x10000)
            && (arg0[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/* overlays/scene/actor/staged_motion/advance_staged_actor_pair.c */
extern u32 StagedActorStepTable[];

extern struct StagedActor *GetStagedActor(s32 arg0);
extern struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

void MapStagedActor_AdvancePair(void)
{
    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 direction_index;
    u32 direction_step;
    s32 move_speed;
    s32 transition_state;

    lead_actor = GetStagedActor(0);
    direction_index = lead_actor->direction_and_kind >> 12;
    direction_step = StagedActorStepTable[direction_index];
    destination[0] = lead_actor->x.value + (direction_step & 0xffff0000);
    destination[1] = lead_actor->y;
    direction_step <<= 16;
    destination[2] = lead_actor->z.value + direction_step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    direction_step = StagedActorStepTable[direction_index];
    destination[0] = next_actor->x.value + (direction_step & 0xffff0000);
    destination[1] = next_actor->y;
    direction_step <<= 16;
    destination[2] = next_actor->z.value + direction_step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    direction_step = StagedActorStepTable[direction_index];
    destination[0] = next_actor->x.value + (direction_step & 0xffff0000);
    destination[1] = next_actor->y;
    direction_step <<= 16;
    destination[2] = next_actor->z.value + direction_step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    transition_state = next_actor->transition_busy;
    if (transition_state != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_speed = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_speed;
    next_actor->move_rate_z = move_speed;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_speed;
    lead_actor->move_rate_z = move_speed;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = transition_state;
    next_actor->unknown_2c = transition_state;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = transition_state;
    lead_actor->unknown_2c = transition_state;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

/* overlays/shared/fill_grid_attribute_rectangle.c */
extern u8 *gCam;
extern u8 gUnk[];

s32 Sys_Run(u32 arg0, s32 arg1, s32 arg2, u32 arg3, u32 arg4, s32 arg5)
{
    u8 *g = gCam;
    u8 *base;
    u32 i;
    u32 j;

    if (g != 0) {
        if (arg0 <= 2) {
            u32 off = arg0 * 48 + 304;

            base = *(u8 **)(g + off);
        } else {
            base = gUnk;
        }
        base += (arg1 + (arg2 << 7)) * 4;
        for (i = 0; i < arg4; i++) {
            u8 *p = base + (i << 9);

            for (j = 0; j < arg3; j++) {
                p[2] = (u8)arg5;
                p += 4;
            }
        }
    }
    return 0;
}

/* overlays/scene/actor/staged_motion/stop_blocked_actor_motion.c */
typedef struct StagedMoveTarget { s32 unk0; s32 unk4; s32 unk8; } StagedMoveTarget;
typedef struct StagedActorRecord { u8 filler0[0x28]; s16 *unk28; } StagedActorRecord;
typedef struct StagedActor {
    u8 filler0[6];
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[0x10];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
    u8 filler44[0xC];
    StagedActorRecord *unk50;
} StagedActor;

extern s32 gOv[];
extern s32 gOv2[];
extern StagedActor *Actor_Run(StagedMoveTarget *, StagedActor *);

s32 MapStagedActor_StopBlockedMotion(StagedActor *actor)
{
    StagedMoveTarget target;
    u32 direction_index;
    s32 direction_step;
    StagedActor *blocking_actor;

    direction_index = actor->unk6 >> 12;
    direction_step = gOv[direction_index];
    target.unk0 = actor->unk8 + (direction_step & 0xffff0000);
    target.unk4 = actor->unkC;
    direction_step = direction_step << 16;
    target.unk8 = actor->unk10 + direction_step;
    blocking_actor = Actor_Run(&target, actor);
    if (blocking_actor != 0) {
        u32 kind_index = 0;
        s32 blocking_kind = *blocking_actor->unk50->unk28;
        s32 *p = gOv2;

        do {
            if (blocking_kind == *p++) goto done;
            kind_index++;
        } while (kind_index <= 5);
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
    direction_step = gOv[direction_index];
    target.unk0 = actor->unk8 + (direction_step & 0xffff0000);
    target.unk4 = actor->unkC;
    direction_step = direction_step << 16;
    target.unk8 = actor->unk10 + direction_step;
    if (Actor_Apply(actor, &target) > 0) {
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
done:
    return 0;
}

/* overlays/scene/actor/staged_motion/find_clear_actor_position.c */
extern s32 gOv[];
extern s32 gOv2[];

s32 MapStagedActor_FindClearPosition(s32 *request)
{
    s32 direction_index;
    s32 origin[3];
    u8 *actor;
    s32 step_count;
    s32 tiles_y, tiles_x;
    u8 *active_flag;
    s32 footprint_offset;
    s32 extent_a, extent_b;
    u8 *footprint_table;
    s32 *candidate;
    s32 found;
    request[5] = 0;
    actor = (u8 *)Actor_Place(&direction_index, request + 1, request);
    if (actor == 0)
        return 0;
    active_flag = actor + 0x22;
    *active_flag = 2;
    step_count = 0;
    footprint_table = (u8 *)gOv;
    footprint_offset = request[0] << 4;
    {
        s32 table_offset = footprint_offset + 4;
        extent_a = *(s32 *)(footprint_table + table_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        table_offset = footprint_offset;
        table_offset += 12;
        extent_b = *(s32 *)(footprint_table + table_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        tiles_y = (extent_a + extent_b) >> 4;
        extent_a = *(s32 *)(footprint_table + footprint_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        table_offset = footprint_offset;
        table_offset += 8;
        extent_b = *(s32 *)(footprint_table + table_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        tiles_x = (extent_a + extent_b) >> 4;
    }
    {
        u8 *actor_bytes;
        s32 actor_y;
        s32 direction_x;
        candidate = origin;
        direction_x = gOv2[direction_index] & 0xffff0000;
        actor_bytes = actor;
        candidate[0] = *(s32 *)(actor_bytes + 8) + direction_x;
        actor_y = *(s32 *)(actor_bytes + 12);
        candidate[1] = actor_y;
        candidate[2] = *(s32 *)(actor_bytes + 16) + (gOv2[direction_index] << 16);
        request[3] = actor_y;
    }
    for (;;) {
        s32 row, column;
        {
            u8 *table = (u8 *)gOv;
            s32 table_offset = request[0] << 4;
            table_offset += 4;
            request[4] = origin[2] + (*(s32 *)(table + table_offset) << 16);
        }
        row = 0;
        while (row < tiles_y) {
            {
                u8 *table = (u8 *)gOv;
                request[2] = origin[0] + (*(s32 *)(table + (request[0] << 4)) << 16);
            }
            column = 0;
            while (column < tiles_x) {
                s32 *probe = request + 2;
                if (Actor_Apply(actor, probe) == 2)
                    goto found;
                probe[0] = probe[0] + 0x100000;
                column++;
            }
            request[4] = request[4] + 0x100000;
            row++;
        }
        step_count++;
        origin[0] = origin[0] + (gOv2[direction_index] & 0xffff0000);
        origin[2] = origin[2] + (gOv2[direction_index] << 16);
    }
found:
    *active_flag = 0;
    found = 0;
    if (step_count != 0) {
        s32 direction = gOv2[direction_index];
        s32 direction_x = direction & 0xffff0000;
        s32 offset_x = direction_x *step_count;
        s32 offset_z = (direction << 16) * step_count;
        u8 *actor_bytes = actor;
        request[2] = *(s32 *)(actor_bytes + 8) + offset_x;
        request[3] = *(s32 *)(actor_bytes + 12);
        request[4] = *(s32 *)(actor_bytes + 16) + offset_z;
        found = 1;
    }
    return found;
}
