#include "scene.h"
#include "abi/battle/effects/calc/select_living_targets.h"
#include "battle_runtime.h"
#include "battle_types.h"

struct BattleEffectTargetArgument {
    u8 reserved_00[36];
    s16 unit_id;
};

void BattleFx_SelectLivingTargets(struct BattleEffectTargetArgument *argument)
{
    s16 targets[14];
    s32 count;
    s32 i;

    count = 0;
    if (argument->unit_id > 127) {
        for (i = 0; i != 6; i++) {
            s32 unit = i + 128;

            if (BattleUnit_Get(unit)->hp > 0)
                targets[count++] = unit;
        }
    } else {
        for (i = 0; i != 8; i++) {
            if (BattleUnit_Get(i)->hp > 0)
                targets[count++] = i;
        }
    }
    targets[count] = 0xff;
    Battle_Apply(targets, 0);
}
