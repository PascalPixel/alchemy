#include "types.h"

struct ActorState_080b90ac {
    u8 padding_000[0x12b];
    u8 field_12b;
};

s32 Func_080b6c08(s32 groups, u16 *ids);
struct ActorState_080b90ac *Func_08077008(u16 id);
void Func_08077010(u16 id);

void Func_080b90ac(void)
{
    u16 ids[14];
    s32 count;
    s32 index;

    count = Func_080b6c08(3, ids);
    for (index = 0; index < count; index++) {
        struct ActorState_080b90ac *actor;

        actor = Func_08077008(ids[index]);
        actor->field_12b = 0;
        Func_08077010(ids[index]);
    }
}
