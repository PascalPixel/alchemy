/* Not-yet-C: complete [08093fa0,08094154), 436 bytes including pools.
 * The adjacent exact grid-placement routine supplies signed /16 indexing:
 * 432 bytes / 163 differing halfwords / 66 aligned edits, frame 24 bytes.
 * Remaining: opening global loads, tile-plane operand registers and the
 * pool before failure cleanup. Retain the full-width activation symbol.
 * A u16 literal active gives 428/68 with movs, not the required pool load;
 * a u8 link value splits the pool too early (444/74); a halfword aggregate
 * adds extension (436/79). These pool-width axes are stopped.
 * The raw failure tail uses .2byte directives, so topology's uncovered
 * target is not evidence of a control-flow gap. No adoption or credit. */
#include "OBJECT_RUNTIME.H"
#include "BATTLE_EFFECT_RUNTIME.H"

struct GridTileCell_08093fa0 {
    u8 unknown_00[2];
    u8 kind;
    u8 unknown_03;
};

extern struct GridTileCell_08093fa0 Data_0200fe00[];
extern struct GridTileCell_08093fa0 gMapCellBuffer[];
extern struct BattleWork gGameState;
extern u8 Value_00000001;

struct ObjectRuntime *Object_GetById(u32 object_id);
void Battle_Reset(void);
s32 CheckMapPositionCellOccupiedFar(const s32 *position);
void ObjectMotion_SetPositionAndCommit(s32 object_id, s32 x, s32 z);
void ObjectDispatch_SetSingleChildField26Far(struct ObjectRuntime *object, s32 value);
void Object_SetMode(struct ObjectRuntime *object, s32 command);
void WaitFrames(s32 frames);
void Object_SetPosition(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 object_id);
void Battle_WaitMode0(s32 should_wait);
void BattleFx_FinishAction(void);

s32 battle_owner_69(void)
{
    struct BattleWork *work = &gGameState;
    struct ObjectRuntime *object = Object_GetById(work->object_id);
    s32 variant = 1;
    s32 tile_x = *(s16 *)((u8 *)object + 10);
    s32 tile_z = *(s16 *)((u8 *)object + 18);
    s32 tile_mask = 0xfff0;
    s32 grid_x;
    s32 grid_z;
    s32 result;

    tile_x &= tile_mask;
    tile_z &= tile_mask;
    grid_x = 8 + tile_x;
    grid_z = 8 + tile_z;

    Battle_Reset();

    if (object->animation_kind == 1) {
        variant = *((u8 *)object->animation + 0x26);
    }

    if (work->mode_1f2 == 0) {
        s32 index = grid_x / 16 + (grid_z / 16) * 128;

        if (gMapCellBuffer[index].kind == Data_0200fe00[index].kind) {
            s32 position[6];

            position[0] = object->x;
            position[1] = object->y;
            position[2] = object->z;
            result = CheckMapPositionCellOccupiedFar(position);
            if (result != 0) {
                goto fail;
            }

            object->action_flags = 0;
            ObjectMotion_SetPositionAndCommit(work->object_id, grid_x, grid_z);
            Object_SetMode(object, 6);
            WaitFrames(4);
            Object_SetMode(object, 7);
            object->velocity_y = 0x40000;
            WaitFrames(4);
            object->flags = 0;
            variant &= 0xfe;
            ObjectDispatch_SetSingleChildField26Far(object, variant);
            object->speed_limit = 0x10000;
            object->velocity_y = 0;
            Object_SetMode(object, 12);
            WaitFrames(4);
            work->mode_1f2 = 1;
            object->action_flags = 1;
            WaitFrames(8);
        } else {
            goto fail;
        }
    } else {
        object->flags = 0;
        Object_SetMode(object, 11);
        Object_SetPosition(object, grid_x << 16, object->y + 0x80000,
            (grid_z << 16) + (s32)0xfff00000);
        ObjectMotion_CommitCurrentPositionAndActivate(work->object_id);
        object->flags = 3;
        {
            s32 active = (s32)&Value_00000001;

            variant |= active;
            object->terrain_height = object->y;
            ObjectDispatch_SetSingleChildField26Far(object, variant);
            Battle_WaitMode0(4);
            work->mode_1f2 = 0;
            object->action_flags = active;
        }
    }

    BattleFx_FinishAction();
    return 0;

fail:
    BattleFx_FinishAction();
    return -1;
}
