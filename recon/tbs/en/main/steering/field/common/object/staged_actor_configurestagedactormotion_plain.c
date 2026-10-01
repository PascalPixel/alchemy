/* NONMATCHING: 2026-10-01 brief Wave2 ConfigureStagedActorMotion plain-source attempt.
 * Removing this one source device changes SceneActor_MoveAndRedraw.
 * First remaining difference: SceneActor_MoveAndRedraw: mov	r0, #0 => ldr	r1, .L0+20 (325/325 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * This reduced draft preserves the affected function and its declarations.
 * Production retains the measured device with its FAKEMATCH reason.
 */
#include "STAGED_ACTOR.H"
#include "FIXED_POINT_POSITION.H"
#include "IWRAM_CALL.H"
#include "CALL.H"
extern u8 gMapCellBuffer[];

/* Linked into several field overlays; each overlay has its own copy of the
 * footprint and direction tables. */

extern u8 *gWork;
/* A word, not a pointer: an integer read shares the alias set of the probe
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

/* Called through this helper, not directly: the copied arguments keep the
 * argument order the reference schedules. */


s32 FixedPoint_Distance(s32 *first_position, s32 *second_position)
;

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
;

s32 StagedActor_FillGridAttributeRectangle(u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value)
;

s32 StagedActor_StopBlockedMotion(struct StagedActor *actor)
;

u8 *FieldScene_FindActorRegion(s32 *direction, s32 *slot, s32 *footprint)
;

s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe)
;

void SceneActor_MoveAndRedraw(struct StagedActorProbe probe)
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
    actor = Object_GetById(probe.actor_slot);
    footprint_table = (u8 *)StagedActor_FootprintBounds;
    {
        s32 footprint_offset = probe.footprint_index << 4;
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
        s32 table_offset = probe.footprint_index << 4;
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
        s32 horizontal_delta = probe.position_x - original_position.x;
        if (horizontal_delta < 0)
            horizontal_delta += 0x1ffff;
        horizontal_delta >>= 17;
        {
            s32 vertical_delta =
                probe.position_z - original_position.y;
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
    Object_SetPosition(actor, probe.position_x, probe.position_y,
                       probe.position_z);
    ObjectMotion_CommitCurrentPositionAndActivate(0);
    Object_SetModeById(0, 2);
    ObjectMotion_SetSpeedParameters(0, 0x4ccc, 0x1999);

    {
        s32 packed_orientation = StagedActor_DirectionSteps[direction];
        ObjectMotion_OffsetPositionAndResetMotion(0, ((s16)(packed_orientation >> 16)) / 2,
                                                  ((s16)packed_orientation) / 2);
    }
    if (probe.callback != 0)
        probe.callback();

    ObjectMotion_CommitCurrentPositionAndActivate(0);
    Object_SetModeById(0, 1);
    ((StagedActorRecord *)Object_GetById(0))->callback = 0;
    Object_CommitPosition(actor);
    Audio_PlayCue(288);
    Audio_PlayCue(213);

    actor->x = probe.position_x;
    actor->y = probe.position_z;
    actor->horizontal_velocity = 0;
    actor->vertical_velocity = 0;
    Object_SetMode(actor, 1);

    {
        u8 *table = (u8 *)StagedActor_FootprintBounds;
        s32 horizontal_origin;
        s32 vertical_origin;

        {
            s32 table_offset = probe.footprint_index << 4;
            probe.position_x =
                (probe.position_x +
                 (*(s32 *)(table + table_offset) << 16));
            table_offset += 4;
            probe.position_z =
                (probe.position_z +
                 (*(s32 *)(table + table_offset) << 16));
            probe.position_x >>= 20;
            probe.position_z >>= 20;
        }
        horizontal_origin = *(s32 *)(workspace + 316) >> 20;
        vertical_origin = *(s32 *)(workspace + 320) >> 20;

        Call6(Map_CopyCellAttributeRect, probe.position_x, probe.position_z, horizontal_extent, vertical_extent, horizontal_origin + probe.position_x, vertical_origin + probe.position_z);
        StagedActor_FillGridAttributeRectangle(0, probe.position_x, probe.position_z,
                                               horizontal_extent, vertical_extent, 255);
        StagedActor_FillGridAttributeRectangle(2, probe.position_x, probe.position_z,
                                               horizontal_extent, vertical_extent, 255);

        {
            s32 table_offset = probe.footprint_index << 4;
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

s32 FieldScene_RedrawActorFootprint(s32 id)
;
