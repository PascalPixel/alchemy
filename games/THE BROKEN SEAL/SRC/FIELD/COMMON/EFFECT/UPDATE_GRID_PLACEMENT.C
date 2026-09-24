#include "TYPES.H"

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

extern s32 gGameState[];
/* The two tile-kind planes are addressed as fixed EWRAM tables. */
#define TILE_CELLS ((struct GridTileCell_08093e28 *)0x02010000)
#define TILE_CELLS_TARGET ((struct GridTileCell_08093e28 *)0x02010200)

#define ACTIVE_FLAG (((u8 *)gGameState)[498])

struct GridEffectObject_08093e28 *Object_GetById(s32);
void Battle_Reset(void);
s32 CheckMapPositionCellOccupiedFar(const s32 *position);
void ObjectMotion_SetPositionAndCommit(s32, s32, s32);
void ObjectMotion_ArmCallback(s32, s32, s32);
void Object_RefreshSelectorById(s32);
void ObjectDispatch_SetSingleChildField26Far(struct GridEffectObject_08093e28 *, s32);
void Object_SetMode(struct GridEffectObject_08093e28 *, s32);
void Object_SetPosition(struct GridEffectObject_08093e28 *, s32, s32, s32);
void ObjectMotion_CommitCurrentPositionAndActivate(s32);
void Battle_WaitMode0(s32);
void BattleFx_FinishAction(void);


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
