#include "types.h"
#include "scene.h"
#include "abi/battle/effects/obj/cycle_object_value_by_counter.h"

extern volatile s32 gIw;

void BattleFx_CycleObjectValueByCounter(s32 arg0)
{
    if ((gIw & 1) != 0) {
        s32 value = Battle_Apply((s32)((unsigned int)gIw >> 1), 6);

        Battle_Apply2(arg0, value);
    }
    if ((gIw & 15) == 0) {
        Battle_Do(arg0);
    }
}
