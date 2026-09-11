#include "types.h"
#include "scene.h"
#include "abi/battle/presentation/actor/set_other_modes.h"

void BattlePres_SetOtherActorRecordModes(s32 excluded)
{
    s16 values[14];
    s32 count = Actor_Apply(3, values);
    s32 index;

    for (index = 0; index < count; index++) {
        if (values[index] != excluded)
            Actor_Apply2(values[index], 1);
    }
}
