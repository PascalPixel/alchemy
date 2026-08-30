#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

struct Creature_080bb8e8 {
    u8 padding_000[0x12a];
    u8 field_12a;
};

struct Runtime_080bb8e8 {
    s32 field_00;
    u8 padding_004[0x24];
    s16 field_28;
};

s32 Object_Destroy(s32);
struct Creature_080bb8e8 *Runtime_GetObject();
s32 Func_08077130(void *, s32);
struct Runtime_080bb8e8 *GetBattleObjectSlot(s32 arg0);
s32 ActivateBattleObjectSlot(s32 arg0);
s32 Func_080bac6c(s32);

s32 BattleActor_DestroyTemporaryObject(s32 arg0) {
    s32 result;
    struct Creature_080bb8e8 *creature;
    struct Runtime_080bb8e8 *runtime;

    creature = Runtime_GetObject();
    if (creature->field_12a == 1) {
        Func_08077130(creature, 0);
        Func_080bac6c(arg0);
        ActivateBattleObjectSlot(arg0);
        runtime = GetBattleObjectSlot(arg0);
        result = Object_Destroy(runtime->field_00);
        runtime->field_00 = 0;
        runtime->field_28 = 0;
        return result;
    }
    return (s32) creature;
}
