#include "battle_runtime.h"
#include "battle_types.h"

void Func_080b5010(s16 *targets, s32 mode);

struct BattleEffectTargetArgument {
    u8 reserved_00[36];
    s16 unit_id;
};

void BattleEffect_SelectLivingTargets(struct BattleEffectTargetArgument *argument)
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
    Func_080b5010(targets, 0);
}
