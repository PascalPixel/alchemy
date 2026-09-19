#include "STAGED_ACTOR.H"
#include "FIXED_POINT_POSITION.H"

/* Linked into several field overlays; each overlay has its own copy of the
 * footprint and direction tables. */

extern u8 *gWork;
/* A word, not a pointer: an integer read shares the alias set of the request
 * fields SceneActor_MoveAndRedraw spills, which keeps those spills first. */
extern u32 gCam;
extern s32 StagedActor_DirectionSteps[];
extern s32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];

extern void WaitFrames(s32);
extern void Object_SetMode(void *, s32);
extern void Object_SetPosition(void *, s32, s32, s32);
extern void Object_CommitPosition(void *);
extern s32 Object_CheckMovementCollision(void *, s32 *);
extern void *Object_GetById(u32);
extern void Battle_WaitMode0(s32);
extern void ObjectMotion_SetSpeedParameters(s32, s32, s32);
extern void ObjectMotion_OffsetPositionAndResetMotion(s32, s32, s32);
extern void ObjectMotion_CommitCurrentPositionAndActivate(s32);
extern void Object_SetModeById(s32, s32);
extern void BattleFx_PlayQueuedSound(void);
extern void Map_CopyCellAttributeRect(s32, s32, s32, s32, s32, s32);
extern void Audio_PlayCue(s32);

static __inline__ void Scene_Call6(
    void (*func)(s32, s32, s32, s32, s32, s32),
    s32 x, s32 z, s32 width, s32 height, s32 dst_x, s32 dst_z)
{
    func(x, z, width, height, dst_x, dst_z);
}
/* Called through this helper, not directly: the copied arguments keep the
 * argument order the reference schedules. */
static __inline__ void ConfigureStagedActorMotion(
    s32 actor_id, s32 movement_rate, s32 movement_step)
{
    ObjectMotion_SetSpeedParameters(actor_id, movement_rate, movement_step);
}

s32 FixedPoint_Distance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

struct StagedActor *StagedActor_FindAtTile(s32 *position, struct StagedActor *origin)
{
    struct StagedActor **slots = (struct StagedActor **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        struct StagedActor *actor = slots[i];

        if ((position[0] >> 20) == (actor->x.value >> 20)
            && (position[1] / 0x10000) == (actor->y / 0x10000)
            && (position[2] >> 20) == (actor->z.value >> 20)) {
            return actor;
        }
    }
    return 0;
}

void StagedActor_AdvancePair(void)
{
    s32 dst[3];
    struct StagedActor *lead;
    struct StagedActor *next;
    struct StagedActor *blocker;
    s32 dir;
    u32 step;
    s32 rate;
    s32 zero;

    lead = Object_GetById(0);
    dir = lead->direction_and_kind >> 12;
    step = StagedActor_DirectionSteps[dir];
    dst[0] = lead->x.value + (step & 0xffff0000);
    dst[1] = lead->y;
    step <<= 16;
    dst[2] = lead->z.value + step;
    next = StagedActor_FindAtTile(dst, lead);
    if (next == 0) return;

    step = StagedActor_DirectionSteps[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    blocker = StagedActor_FindAtTile(dst, next);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) return;

    dst[0] = next->x.value;
    dst[1] = next->y + 0x100000;
    dst[2] = next->z.value;
    blocker = StagedActor_FindAtTile(dst, next);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) return;

    next->transition_mode = 2;
    step = StagedActor_DirectionSteps[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    if (Object_CheckMovementCollision(next, dst) > 0) return;

    zero = next->transition_busy;
    if (zero != 0) return;

    Object_SetMode(lead, 8);
    rate = 0x3333;
    WaitFrames(15);
    Audio_PlayCue(185);
    next->move_rate_x = rate;
    next->move_rate_z = rate;
    Object_SetPosition(next, dst[0], dst[1], dst[2]);
    lead->move_rate_x = rate;
    lead->move_rate_z = rate;
    Object_SetPosition(lead, dst[0], dst[1], dst[2]);
    Object_CommitPosition(next);
    BattleFx_PlayQueuedSound();
    next->x.value = dst[0];
    next->z.value = dst[2];
    next->unknown_24 = zero;
    next->unknown_2c = zero;
    lead->unknown_38 = 0x80000000;
    lead->unknown_40 = 0x80000000;
    lead->unknown_24 = zero;
    lead->unknown_2c = zero;
    lead->x.value = lead->x.parts.cell << 16;
    lead->z.value = lead->z.parts.cell << 16;
    Object_SetMode(lead, 1);
}

s32 StagedActor_FillGridAttributeRectangle(u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value)
{
    u8 *map = (u8 *)gCam;
    u8 *cells;
    u32 row;
    u32 column;

    if (map != 0) {
        if (layer <= 2) {
            u32 offset = layer * 48 + 304;

            cells = *(u8 **)(map + offset);
        } else {
            cells = (u8 *)0x02010000;
        }
        cells += (x + (z << 7)) * 4;
        for (row = 0; row < height; row++) {
            u8 *cell = cells + (row << 9);

            for (column = 0; column < width; column++) {
                cell[2] = (u8)value;
                cell += 4;
            }
        }
    }
    return 0;
}

s32 StagedActor_StopBlockedMotion(struct StagedActor *actor)
{
    struct StagedActorProbePoint probe;
    u32 dir;
    s32 step;
    struct StagedActor *target;

    dir = actor->direction_and_kind >> 12;
    step = StagedActor_DirectionSteps[dir];
    probe.x = actor->x.value + (step & 0xffff0000);
    probe.y = actor->y;
    step = step << 16;
    probe.z = actor->z.value + step;
    target = StagedActor_FindAtTile((s32 *)&probe, actor);
    if (target != 0) {
        u32 i = 0;
        s32 kind =
            *(STAGED_ACTOR_PROBE_DETAILS(target)->unknown_28);
        s32 *kinds = StagedActor_FootprintKinds;

        do {
            if (kind == *kinds++) goto done;
            i++;
        } while (i <= 5);
        actor->unknown_24 = 0;
        actor->unknown_2c = 0;
        actor->unknown_38 = 0x80000000;
        actor->unknown_40 = 0x80000000;
    }
    step = StagedActor_DirectionSteps[dir];
    probe.x = actor->x.value + (step & 0xffff0000);
    probe.y = actor->y;
    step = step << 16;
    probe.z = actor->z.value + step;
    if (Object_CheckMovementCollision(actor, (s32 *)&probe) > 0) {
        actor->unknown_24 = 0;
        actor->unknown_2c = 0;
        actor->unknown_38 = 0x80000000;
        actor->unknown_40 = 0x80000000;
    }
done:
    return 0;
}

u8 *FieldScene_FindActorRegion(s32 *direction, s32 *slot, s32 *footprint)
{
    u8 *work = gWork;
    u8 *player = Object_GetById(0);
    u8 **list;
    u32 i, j;

    *direction = *(u16 *)(player + 6) >> 12;
    list = (u8 **)(work + 52);
    for (i = 8; i <= 65; list++, i++) {
        u8 *actor = *list;
        s32 kind;
        j = 0;
        kind = (*(s16 **)(*(u8 **)(actor + 80) + 40))[j];
        for (j = 0; j < 6; j++) {
            if (kind == StagedActor_FootprintKinds[j]) {
                s32 x, z, left, top, right, bottom;
                *footprint = j;
                x = ((*(s32 *)(player + 8) >> 16) + (StagedActor_DirectionSteps[*direction] >> 16)) >> 4;
                z = ((*(s32 *)(player + 16) >> 16) + (s16)StagedActor_DirectionSteps[*direction]) >> 4;
                left = (*(s16 *)(actor + 10) + StagedActor_FootprintBounds[j * 4 + 0]) >> 4;
                top = (*(s16 *)(actor + 18) + StagedActor_FootprintBounds[j * 4 + 1]) >> 4;
                right = (*(s16 *)(actor + 10) + StagedActor_FootprintBounds[j * 4 + 2]) >> 4;
                bottom = (*(s16 *)(actor + 18) + StagedActor_FootprintBounds[j * 4 + 3]) >> 4;
                if (left <= x && x < right && top <= z && z < bottom) {
                    if (j & 1) {
                        if (left != (*(s32 *)(player + 8) >> 20)) {
                            *slot = i;
                            return actor;
                        }
                    } else if (top != (*(s32 *)(player + 16) >> 20)) {
                        *slot = i;
                        return actor;
                    }
                }
            }
        }
    }
    return NULL;
}

s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe)
{
    s32 direction;
    s32 position[3];
    struct StagedActor *actor;
    s32 steps;
    s32 width, height;
    u8 *mode;
    s32 column, row;
    s32 near_edge, far_edge;
    s32 offset;

    probe->unknown_14 = 0;
    actor = (struct StagedActor *)FieldScene_FindActorRegion(
        &direction, &probe->actor_slot, &probe->footprint_index);
    if (actor == 0) {
        return 0;
    }
    mode = &actor->transition_mode;
    *mode = 2;
    {
        s32 footprint = probe->footprint_index;

        steps = 0;
        offset = footprint * 4;
        near_edge = StagedActor_FootprintBounds[offset + 1];
        if (near_edge < 0) {
            near_edge = -near_edge;
        }
        far_edge = StagedActor_FootprintBounds[offset + 3];
        if (far_edge < 0) {
            far_edge = -far_edge;
        }
        height = (near_edge + far_edge) >> 4;
        near_edge = StagedActor_FootprintBounds[offset];
        if (near_edge < 0) {
            near_edge = -near_edge;
        }
        far_edge = StagedActor_FootprintBounds[offset + 2];
        if (far_edge < 0) {
            far_edge = -far_edge;
        }
        width = (near_edge + far_edge) >> 4;
    }
    position[0] = actor->x.value + (StagedActor_DirectionSteps[direction] & 0xffff0000);
    {
        s32 y = actor->y;

        position[1] = y;
        position[2] = actor->z.value + (StagedActor_DirectionSteps[direction] << 16);
        probe->position_y = y;
    }
    for (;;) {
        probe->position_z = position[2] + (StagedActor_FootprintBounds[probe->footprint_index * 4 + 1] << 16);
        for (row = 0; row < height; row++) {
            probe->position_x = position[0] + (StagedActor_FootprintBounds[probe->footprint_index * 4] << 16);
            for (column = 0; column < width; column++) {
                if (Object_CheckMovementCollision(actor, &probe->position_x) == 2) {
                    goto found;
                }
                probe->position_x += 0x100000;
            }
            probe->position_z += 0x100000;
        }
        steps++;
        position[0] += StagedActor_DirectionSteps[direction] & 0xffff0000;
        position[2] += StagedActor_DirectionSteps[direction] << 16;
    }
found:
    *mode = 0;
    if (steps == 0) {
        return 0;
    }
    probe->position_x = actor->x.value + (StagedActor_DirectionSteps[direction] & 0xffff0000) * steps;
    probe->position_y = actor->y;
    probe->position_z = actor->z.value + (StagedActor_DirectionSteps[direction] << 16) * steps;
    return 1;
}

void SceneActor_MoveAndRedraw(StagedActorMovementRequest request)
{
    u8 *workspace;
    StagedActorRecord *actor;
    StagedActorPosition original_position;
    StagedActorPosition tile_position;
    u8 *footprint_table;
    s32 direction;
    s32 horizontal_extent;
    s32 vertical_extent;

    workspace = (u8 *)gCam;
    direction = ((StagedActorRecord *)Object_GetById(0))->orientation >> 12;
    actor = Object_GetById(request.actor_id);
    footprint_table = (u8 *)StagedActor_FootprintBounds;
    {
        s32 footprint_offset = request.movement_index << 4;
        s32 table_offset = footprint_offset + 4;
        s32 extent_a = *(s32 *)(footprint_table + table_offset);
        s32 extent_b;
        if (extent_a < 0)
            extent_a = -extent_a;
        table_offset = footprint_offset;
        table_offset += 12;
        extent_b = *(s32 *)(footprint_table + table_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        vertical_extent = (extent_a + extent_b) >> 4;
        extent_a = *(s32 *)(footprint_table + footprint_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        table_offset = footprint_offset;
        table_offset += 8;
        extent_b = *(s32 *)(footprint_table + table_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        horizontal_extent = (extent_a + extent_b) >> 4;
    }

    actor->movement_rate = 0x8000;
    actor->movement_step = 0x1999;
    original_position.x = actor->x;
    original_position.y = actor->y;
    {
        s32 table_offset = request.movement_index << 4;
        tile_position.x =
            (actor->x +
             (*(s32 *)(footprint_table + table_offset) << 16));
        table_offset += 4;
        tile_position.y =
            (actor->y +
             (*(s32 *)(footprint_table + table_offset) << 16));
        tile_position.x >>= 20;
        tile_position.y >>= 20;
    }

    StagedActor_FillGridAttributeRectangle(0, tile_position.x, tile_position.y, horizontal_extent,
                                           vertical_extent, 0);
    ObjectMotion_SetSpeedParameters(0, 0x8000, 0x1999);
    Object_SetModeById(0, 8);
    Battle_WaitMode0(15);

    {
        s32 horizontal_delta = request.target_x - original_position.x;
        if (horizontal_delta < 0)
            horizontal_delta += 0x1ffff;
        horizontal_delta >>= 17;
        {
            s32 vertical_delta =
                request.tail.target_y - original_position.y;
            if (vertical_delta < 0)
                vertical_delta += 0x1ffff;
            vertical_delta >>= 17;
            ObjectMotion_OffsetPositionAndResetMotion(0, horizontal_delta, vertical_delta);
        }
    }

    ((StagedActorRecord *)Object_GetById(0))->callback = (u32)StagedActor_StopBlockedMotion;
    Battle_WaitMode0(4);
    if ((u32)(direction - 6) <= 7)
        Object_SetMode(actor, 3);
    else
        Object_SetMode(actor, 2);

    Audio_PlayCue(239);
    Object_SetPosition(actor, request.target_x, request.target_depth,
                       request.tail.target_y);
    ObjectMotion_CommitCurrentPositionAndActivate(0);
    Object_SetModeById(0, 2);
    ConfigureStagedActorMotion(0, 0x4ccc, 0x1999);

    {
        s32 packed_orientation = StagedActor_DirectionSteps[direction];
        ObjectMotion_OffsetPositionAndResetMotion(0, ((s16)(packed_orientation >> 16)) / 2,
                                                  ((s16)packed_orientation) / 2);
    }
    if (request.tail.callback != 0)
        request.tail.callback();

    ObjectMotion_CommitCurrentPositionAndActivate(0);
    Object_SetModeById(0, 1);
    ((StagedActorRecord *)Object_GetById(0))->callback = 0;
    Object_CommitPosition(actor);
    Audio_PlayCue(288);
    Audio_PlayCue(213);

    actor->x = request.target_x;
    actor->y = request.tail.target_y;
    actor->horizontal_velocity = 0;
    actor->vertical_velocity = 0;
    Object_SetMode(actor, 1);

    {
        u8 *table = (u8 *)StagedActor_FootprintBounds;
        s32 horizontal_origin;
        s32 vertical_origin;

        {
            s32 table_offset = request.movement_index << 4;
            request.target_x =
                (request.target_x +
                 (*(s32 *)(table + table_offset) << 16));
            table_offset += 4;
            request.tail.target_y =
                (request.tail.target_y +
                 (*(s32 *)(table + table_offset) << 16));
            request.target_x >>= 20;
            request.tail.target_y >>= 20;
        }
        horizontal_origin = *(s32 *)(workspace + 316) >> 20;
        vertical_origin = *(s32 *)(workspace + 320) >> 20;

        Scene_Call6(Map_CopyCellAttributeRect, request.target_x, request.tail.target_y,
                    horizontal_extent, vertical_extent,
                    horizontal_origin + request.target_x,
                    vertical_origin + request.tail.target_y);
        StagedActor_FillGridAttributeRectangle(0, request.target_x, request.tail.target_y,
                                               horizontal_extent, vertical_extent, 255);
        StagedActor_FillGridAttributeRectangle(2, request.target_x, request.tail.target_y,
                                               horizontal_extent, vertical_extent, 255);

        {
            s32 table_offset = request.movement_index << 4;
            original_position.x =
                (original_position.x +
                 (*(s32 *)(table + table_offset) << 16));
            table_offset += 4;
            original_position.y =
                (original_position.y +
                 (*(s32 *)(table + table_offset) << 16));
            original_position.x >>= 20;
            original_position.y >>= 20;
        }
        horizontal_origin += original_position.x;
        vertical_origin += original_position.y;

        Map_CopyCellAttributeRect(horizontal_origin, vertical_origin, horizontal_extent,
                                       vertical_extent, original_position.x,
                                       original_position.y);
        StagedActor_FillGridAttributeRectangle(2, original_position.x, original_position.y,
                                               horizontal_extent, vertical_extent, 0);
    }
    BattleFx_PlayQueuedSound();
}
