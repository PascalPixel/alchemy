#include "types.h"

#define BattleUnit_ClearField12bForGroup Func_080b90ac

struct ActorState_080b90ac {
    u8 padding_000[0x12b];
    u8 field_12b;
};

s32 Func_080b6c08(s32 groups, u16 *ids);
struct ActorState_080b90ac *Runtime_GetObject(u16 id);
void BattleUnit_Recalculate(u16 id);

void BattleUnit_ClearField12bForGroup(void)
{
    u16 ids[14];
    s32 count;
    s32 index;

    count = Func_080b6c08(3, ids);
    for (index = 0; index < count; index++) {
        struct ActorState_080b90ac *actor;

        actor = Runtime_GetObject(ids[index]);
        actor->field_12b = 0;
        BattleUnit_Recalculate(ids[index]);
    }
}
