#include "colosso_log_rolling_stage.h"

typedef struct StageEffect {
    u8 filler00[0x30];
    s32 move_rate_x;
    s32 move_rate_z;
    u8 filler38[0x1d];
    u8 state;
} StageEffect;

extern StageEffect *Func_02005462(s32);
extern void Func_02005340(StageEffect *, s32);
extern void Func_02005350(StageEffect *, s32);
extern void Func_02005426(s32);

void ColossoLogRollingStage_ConfigureSceneEventEffect(void)
{
    StageEffect *effect;
    s32 move_rate;

    effect = Func_02005462(30);
    effect->state = 0;
    move_rate = 0x19999;
    effect->move_rate_z = move_rate;
    effect->move_rate_x = move_rate;
    Func_02005340(effect, 2);
    Func_02005350(effect, 0x0200CC48);
    Func_02005426(0x363);
}
