#include "colosso_log_rolling_stage.h"

typedef struct StageEffect {
    s32 reserved_00;
    s32 reserved_04;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[65];
    u8 state;
    u8 reserved_56[14];
    s16 linked_effect_slot;
} StageEffect;

extern u8 Data_0200ce74[];

extern StageEffect *Func_020087b4(s16);
extern void Func_020086be(StageEffect *, s32, s32, s32);
extern void Func_020086a6(StageEffect *, s32);
extern void Func_02008964(s32);

s32 ColossoLogRollingStage_RaiseLinkedSceneEffect(StageEffect *source)
{
    StageEffect *effect = Func_020087b4(source->linked_effect_slot);

    Func_020086be(effect, source->x, source->y + 0x240000, source->z);
    effect->state = 0;
    Func_020086a6(effect, (s32)Data_0200ce74);
    Func_02008964(83);
    source->linked_effect_slot = 0;
    return 0;
}
