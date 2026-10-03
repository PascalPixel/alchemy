/* Field: jump the leader two tiles forward when the tile ahead is blocked but
   the one beyond is free: the leader hops over, lands, and a ledge target
   in front shakes as it goes; landing on a hole tile repeats the jump.
   Returns 0 after a jump, -1 when the way is blocked. */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "GAME_STATE.H"
#include "OBJECT_RUNTIME.H"
#include "MAP.H"
#include "FIELD_SPRITE.H"
#include "FIELDRUN.H"

extern struct FieldStepWork *gEventWork;
struct ObjectRuntime *Object_GetById(s32 id);
void Vector_AddPolarOffset(s32 distance, s32 angle, struct FieldPosition *position);
s32 Object_CheckMovementCollision(struct ObjectRuntime *actor, struct FieldPosition *position);
void Battle_Reset(void);
void Object_SetMode(struct ObjectRuntime *actor, s32 mode);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void ObjectDispatch_SetSingleChildField26Far(struct ObjectRuntime *actor, s32 value);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *actor, s32 kind);
void BattleFx_FinishAction(void);
s32 Func_080091b0(s32 layer, s32 x, s32 z);


/* The two tile-kind planes are addressed as fixed EWRAM tables. */
#define TILE_CELLS ((struct MapCell *)Ram_MapCellBuffer)
#define TILE_CELLS_TARGET ((struct MapCell *)(Ram_MapCellBuffer + 0x200))
#define TILE_CELLS_ABOVE ((struct MapCell *)(Ram_MapCellBuffer - 0x200))
s32 CheckMapPositionCellOccupiedFar(const s32 *position);
void ObjectMotion_ArmCallback(s32, s32, s32);
void Object_RefreshSelectorById(s32);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void ObjectMotion_CommitCurrentPositionAndActivate(s32);
void Battle_WaitMode0(s32);

s32 Field_TryJumpForward(void)
{
    struct ObjectRuntime *leader = Object_GetById(gGameState.selected_actor);
    s32 result = -1;
    s32 angle = (leader->angle + 0x2000) & 0xc000;
    u8 *flags = &leader->flags;
    u8 saved = *flags;
    struct FieldStepWork *work = gEventWork;
    s32 child_mode = 1;
    struct FieldPosition position;
    struct FieldPosition *pos;
    struct ObjectRuntime *target;
    /* FAKEMATCH: one local holds the tile mask and then the descent step,
       a second the lift; that split gives the reference its registers. */
    s32 step;
    s32 lift;

    pos = &position;
again:
    step = 0xfff00000;
    pos->x = (leader->x & step) + 0x80000;
    pos->y = leader->y;
    pos->z = (leader->z & step) + 0x80000;
    Vector_AddPolarOffset(0x100000, angle, pos);
    if (Object_CheckMovementCollision(leader, pos) == 1)
        return -1;
    pos->x = (leader->x & step) + 0x80000;
    pos->y = leader->y;
    pos->z = (leader->z & step) + 0x80000;
    Vector_AddPolarOffset(0x200000, angle, pos);
    if (Object_CheckMovementCollision(leader, pos) != 0)
        goto done;
    if (leader->animation_kind == 1)
        child_mode = ((struct FieldSprite *)leader->animation)->flags;
    Battle_Reset();
    Object_SetMode(leader, 6);
    WaitFrames(6);
    Audio_PlayCue(152);
    Object_SetMode(leader, 7);
    leader->speed_limit = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocity_y = 0x40000;
    *flags &= 0x7e;
    ObjectDispatch_SetSingleChildField26Far(leader, child_mode & 0xfe);
    ObjectMotion_SetPositionAndCommit(gGameState.selected_actor, *(s16 *)((u8 *)&pos->x + 2), *(s16 *)((u8 *)&pos->z + 2));
    Object_SetMode(leader, 6);
    ObjectDispatch_SetSingleChildField26Far(leader, child_mode);
    if ((target = Object_FindNearestFacingTarget(leader, 207)) != NULL
        || (target = Object_FindNearestFacingTarget(leader, 205)) != NULL) {
        Object_SetMode(target, 7);
        lift = 0xffff0000;
        leader->y += lift;
        leader->terrain_height += lift;
        WaitFrames(2);
        leader->y += lift;
        leader->terrain_height += lift;
        WaitFrames(10);
        step = 0x10000;
        leader->y += step;
        leader->terrain_height += step;
        WaitFrames(4);
        leader->y += step;
        leader->terrain_height += step;
    } else {
        WaitFrames(6);
    }
    *flags = saved;
    BattleFx_FinishAction();
    if (work != NULL)
        work->encounter_steps += Iwram_MulQ16(work->encounter_rate, 0x200000);
    if (Func_080091b0(leader->terrain_id, pos->x, pos->z) == 0xf9) {
        Object_SetMode(leader, 1);
        WaitFrames(6);
        goto again;
    }
    result = 0;
done:
    return result;
}

s32 FieldEffect_UpdateGridPlacement(void)
{
    struct ObjectRuntime *object = Object_GetById(gGameState.selected_actor);
    s32 grid_x = 8 + ((*(s16 *)((u8 *)&object->x + 2)) & 0xfff0);
    s32 grid_z = 8 + ((*(s16 *)((u8 *)&object->z + 2)) & 0xfff0);
    s32 tile_x = grid_x - 8;
    s32 tile_z = grid_z - 8;
    s32 result;

    Battle_Reset();

    if (gGameState.movement_mode == 0) {
        s32 index = grid_x / 16 + (grid_z / 16) * 128;

        if (TILE_CELLS[index].collision_code == TILE_CELLS_TARGET[index].collision_code) {
            struct FieldPosition position;

            position.x = object->x;
            position.y = object->y + (s32)0xfff00000;
            position.z = object->z;
            result = CheckMapPositionCellOccupiedFar((const s32 *)&position);
            if (result != 0)
                goto failure;

            ObjectMotion_SetPositionAndCommit(gGameState.selected_actor, grid_x, grid_z);
            object->speed_limit = 0x10000;
            ObjectMotion_ArmCallback(gGameState.selected_actor, 0xc000, 0);
            Object_RefreshSelectorById(gGameState.selected_actor);
            object->action_flags = 1;
            object->flags = 0;
            ObjectDispatch_SetSingleChildField26Far(object, 0);
            Object_SetMode(object, 13);
            Object_SetPosition(object, grid_x << 16,
                object->y + (s32)0xfff00000, (grid_z << 16) + 0x100000);
            ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selected_actor);
            gGameState.movement_mode = 1;
        } else {
            goto failure;
        }
    } else {
        Object_SetMode(object, 10);
        object->flags = 3;
        object->velocity_y = 0x40000;
        object->terrain_height = object->y;
        ObjectDispatch_SetSingleChildField26Far(object, 1);
        Battle_WaitMode0(6);
        gGameState.movement_mode = 0;
        object->action_flags = 1;
        object->angle = 0xc000;
    }

    BattleFx_FinishAction();
    return 0;

failure:
    BattleFx_FinishAction();
    return -1;
}

/* Field: the leader takes a ladder from its foot, or steps off there.
   Off the ladder, where the cell one row north is of the leader's kind and
   nothing stands on the leader's tile, the leader hops onto the rungs; on
   the ladder, the leader drops to the tile one row north. Returns 0 after
   either, -1 when there is no ladder to take. */
s32 battle_owner_69(void)
{
    struct ObjectRuntime *object = Object_GetById(gGameState.selected_actor);
    s16 child_mode = 1;
    s32 grid_x = 8 + ((*(s16 *)((u8 *)&object->x + 2)) & 0xfff0);
    s32 grid_z = 8 + ((*(s16 *)((u8 *)&object->z + 2)) & 0xfff0);
    s32 tile_x = grid_x - 8;
    s32 tile_z = grid_z - 8;
    s32 result;

    Battle_Reset();

    if (object->animation_kind == 1)
        child_mode = ((struct FieldSprite *)object->animation)->flags;

    if (gGameState.movement_mode == 0) {
        s32 index = grid_x / 16 + (grid_z / 16) * 128;

        if (TILE_CELLS[index].collision_code == TILE_CELLS_ABOVE[index].collision_code) {
            struct FieldPosition position;

            position.x = object->x;
            position.y = object->y;
            position.z = object->z;
            result = CheckMapPositionCellOccupiedFar((const s32 *)&position);
            if (result != 0)
                goto failure;

            object->action_flags = 0;
            ObjectMotion_SetPositionAndCommit(gGameState.selected_actor, grid_x, grid_z);
            Object_SetMode(object, 6);
            WaitFrames(4);
            Object_SetMode(object, 7);
            object->velocity_y = 0x40000;
            WaitFrames(4);
            object->flags = 0;
            child_mode &= 0xfe;
            ObjectDispatch_SetSingleChildField26Far(object, child_mode);
            object->speed_limit = 0x10000;
            object->velocity_y = 0;
            Object_SetMode(object, 12);
            WaitFrames(4);
            gGameState.movement_mode = 1;
            object->action_flags = 1;
            WaitFrames(8);
        } else {
            goto failure;
        }
    } else {
        object->flags = 0;
        Object_SetMode(object, 11);
        Object_SetPosition(object, grid_x << 16, object->y + 0x80000,
            (grid_z << 16) + (s32)0xfff00000);
        ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selected_actor);
        object->flags = 3;
        child_mode |= 1;
        object->terrain_height = object->y;
        ObjectDispatch_SetSingleChildField26Far(object, child_mode);
        Battle_WaitMode0(4);
        gGameState.movement_mode = 0;
        object->action_flags = 1;
    }

    BattleFx_FinishAction();
    return 0;

failure:
    BattleFx_FinishAction();
    return -1;
}
