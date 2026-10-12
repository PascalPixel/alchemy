#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct CellTarget {
    s16 x;
    s16 z;
    s16 flag;
    s16 unknown_06;
};

struct CellTargetTable {
    s16 unknown_00;
    s16 position_flag;
    u8 unknown_04[6];
    s16 count;
    s16 current;
    u8 unknown_0e[18];
    struct CellTarget targets[1];
};

extern struct CellTargetTable Data_020024f8;

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_02001654(struct CellTargetTable *table, struct CellTarget *target);
void Func_020016c4(struct CellTargetTable *table, struct CellTarget *target);

// 先頭の仲間が立っているマスの仕掛けを動かす。動かしたら 1
s32 Func_020019f4(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    s32 x = leader->x >> 20;
    s32 z = leader->z >> 20;
    struct CellTargetTable *table = &Data_020024f8;
    struct CellTarget *target;
    s32 i;

    for (i = 0, target = table->targets; i < table->count; i++, target++) {
        if (target->x == x && target->z == z) {
            if (GameFlag_Test(target->flag) == 0) {
                Func_02001654(table, target);
                GameFlag_SetBit(target->flag);
                table->current = i;
                break;
            }
            if (i == table->current)
                break;
            table->current = i;
            Func_020016c4(table, target);
            GameFlag_SetByte(table->position_flag, x);
            GameFlag_SetByte(table->position_flag + 8, z);
            return 1;
        }
    }
    return 0;
}
