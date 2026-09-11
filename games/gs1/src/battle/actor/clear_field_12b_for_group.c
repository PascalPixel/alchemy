#include "types.h"
#include "scene.h"
#include "abi/battle/actor/clear_field_12b_for_group.h"

struct ActorState_080b90ac {
    u8 padding_000[0x12b];
    u8 field_12b;
};

struct ActorState_080b90ac *Runtime_GetObject(u16 id);
void BattleUnit_Recalculate(u16 id);

void BattleUnit_ClearField12bForGroup(void)
{
    u16 ids[14];
    s32 count;
    s32 index;

    count = Actor_Apply(3, ids);
    for (index = 0; index < count; index++) {
        struct ActorState_080b90ac *actor;

        actor = Runtime_GetObject(ids[index]);
        actor->field_12b = 0;
        BattleUnit_Recalculate(ids[index]);
    }
}
