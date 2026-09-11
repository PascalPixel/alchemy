#include "types.h"
#include "scene.h"
#include "abi/battle/presentation/actor/clear_modes.h"

void BattlePres_ClearAllActorRecordModes(void)
{
    s16 values[14];
    s32 count = Actor_Apply(3, values);
    s32 index;

    for (index = 0; index < count; index++)
        Actor_Apply2(values[index], 0);
}
