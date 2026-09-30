/* Field: jump the leader two tiles forward when the tile ahead is blocked but
   the one beyond is free: the leader hops over, lands, and a ledge target
   in front shakes as it goes; landing on a hole tile repeats the jump.
   Returns 0 after a jump, -1 when the way is blocked. */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"

struct JumpSprite {
    u8 unknown_00[38];
    u8 child_mode;
};

struct JumpActor {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    s32 shadow_y;
    u8 unknown_18[10];
    u8 layer;
    u8 unknown_23[5];
    s32 gravity;
    u8 unknown_2c[4];
    s32 speed;
    s32 acceleration;
    u8 unknown_38[24];
    struct JumpSprite *sprite;
    u8 kind;
    u8 motion_flags;
};

struct JumpWork {
    u8 unknown_000[432];
    s32 step_rate;
    s32 steps;
};

struct JumpPosition {
    s32 x;
    s32 y;
    s32 z;
};

extern s32 gGameState[];
extern struct JumpWork *gEventWork;
struct JumpActor *Object_GetById(s32 id);
void Vector_AddPolarOffset(s32 distance, s32 angle, struct JumpPosition *position);
s32 Object_CheckMovementCollision(struct JumpActor *actor, struct JumpPosition *position);
void Battle_Reset(void);
void Object_SetMode(struct JumpActor *actor, s32 mode);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void ObjectDispatch_SetSingleChildField26Far(struct JumpActor *actor, s32 value);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
struct JumpActor *Object_FindNearestFacingTarget(struct JumpActor *actor, s32 kind);
void BattleFx_FinishAction(void);
s32 Func_080091b0(s32 layer, s32 x, s32 z);

#define TILE_HI(ptr, offset) (*(s16 *)((u8 *)(ptr) + (offset) + 2))

struct GridEffectObject_08093e28 {
    u8 unknown_00[6];
    u16 field_06;
    s32 x;
    s32 y;
    s32 z;
    s32 field_20;
    u8 unknown_24[16];
    s32 field_28;
    u8 unknown_2c[4];
    s32 field_30;
    u8 unknown_34[33];
    u8 value_55;
    u8 unknown_56[4];
    u8 value_5a;
};

struct GridTileCell_08093e28 {
    u8 unknown_00[2];
    u8 kind;
    u8 unknown_03;
};

/* The two tile-kind planes are addressed as fixed EWRAM tables. */
#define TILE_CELLS ((struct GridTileCell_08093e28 *)Ram_MapCellBuffer)
#define TILE_CELLS_TARGET ((struct GridTileCell_08093e28 *)(Ram_MapCellBuffer + 0x200))
#define ACTIVE_FLAG (((u8 *)gGameState)[498])
s32 CheckMapPositionCellOccupiedFar(const s32 *position);
void ObjectMotion_SetPositionAndCommit(s32, s32, s32);
void ObjectMotion_ArmCallback(s32, s32, s32);
void Object_RefreshSelectorById(s32);
void Object_SetPosition(struct GridEffectObject_08093e28 *, s32, s32, s32);
void ObjectMotion_CommitCurrentPositionAndActivate(s32);
void Battle_WaitMode0(s32);

s32 Field_TryJumpForward(void)
{
    struct JumpActor *leader = Object_GetById(gGameState[125]);
    s32 result = -1;
    s32 angle = (leader->facing + 0x2000) & 0xc000;
    u8 *flags = &leader->motion_flags;
    u8 saved = *flags;
    struct JumpWork *work = gEventWork;
    s32 child_mode = 1;
    struct JumpPosition position;
    struct JumpPosition *pos;
    struct JumpActor *target;
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
    if (leader->kind == 1)
        child_mode = leader->sprite->child_mode;
    Battle_Reset();
    Object_SetMode(leader, 6);
    WaitFrames(6);
    Audio_PlayCue(152);
    Object_SetMode(leader, 7);
    leader->speed = 0x30000;
    leader->acceleration = 0x20000;
    leader->gravity = 0x40000;
    *flags &= 0x7e;
    ObjectDispatch_SetSingleChildField26Far(leader, child_mode & 0xfe);
    ObjectMotion_SetPositionAndCommit(gGameState[125], *(s16 *)((u8 *)pos + 2), *(s16 *)((u8 *)pos + 10));
    Object_SetMode(leader, 6);
    ObjectDispatch_SetSingleChildField26Far(leader, child_mode);
    if ((target = Object_FindNearestFacingTarget(leader, 207)) != NULL
        || (target = Object_FindNearestFacingTarget(leader, 205)) != NULL) {
        Object_SetMode(target, 7);
        lift = 0xffff0000;
        leader->y += lift;
        leader->shadow_y += lift;
        WaitFrames(2);
        leader->y += lift;
        leader->shadow_y += lift;
        WaitFrames(10);
        step = 0x10000;
        leader->y += step;
        leader->shadow_y += step;
        WaitFrames(4);
        leader->y += step;
        leader->shadow_y += step;
    } else {
        WaitFrames(6);
    }
    *flags = saved;
    BattleFx_FinishAction();
    if (work != NULL)
        work->steps += Iwram_MulQ16(work->step_rate, 0x200000);
    if (Func_080091b0(leader->layer, pos->x, pos->z) == 0xf9) {
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
    struct GridEffectObject_08093e28 *object = Object_GetById(gGameState[125]);
    s32 grid_x = 8 + (TILE_HI(object, 8) & 0xfff0);
    s32 grid_z = 8 + (TILE_HI(object, 16) & 0xfff0);
    s32 tile_x = grid_x - 8;
    s32 tile_z = grid_z - 8;
    s32 result;

    Battle_Reset();

    if (ACTIVE_FLAG == 0) {
        s32 index = grid_x / 16 + (grid_z / 16) * 128;

        if (TILE_CELLS[index].kind == TILE_CELLS_TARGET[index].kind) {
            s32 position[6];

            position[0] = object->x;
            position[1] = object->y + (s32)0xfff00000;
            position[2] = object->z;
            result = CheckMapPositionCellOccupiedFar(position);
            if (result != 0)
                goto failure;

            ObjectMotion_SetPositionAndCommit(gGameState[125], grid_x, grid_z);
            object->field_30 = 0x10000;
            ObjectMotion_ArmCallback(gGameState[125], 0xc000, 0);
            Object_RefreshSelectorById(gGameState[125]);
            object->value_5a = 1;
            object->value_55 = 0;
            ObjectDispatch_SetSingleChildField26Far(object, 0);
            Object_SetMode(object, 13);
            Object_SetPosition(object, grid_x << 16,
                object->y + (s32)0xfff00000, (grid_z << 16) + 0x100000);
            ObjectMotion_CommitCurrentPositionAndActivate(gGameState[125]);
            ACTIVE_FLAG = 1;
        } else {
            goto failure;
        }
    } else {
        Object_SetMode(object, 10);
        object->value_55 = 3;
        object->field_28 = 0x40000;
        object->field_20 = object->y;
        ObjectDispatch_SetSingleChildField26Far(object, 1);
        Battle_WaitMode0(6);
        ACTIVE_FLAG = 0;
        object->value_5a = 1;
        object->field_06 = 0xc000;
    }

    BattleFx_FinishAction();
    return 0;

failure:
    BattleFx_FinishAction();
    return -1;
}
