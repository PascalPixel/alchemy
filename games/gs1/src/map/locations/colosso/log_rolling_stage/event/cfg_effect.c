#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/event/cfg_effect.h"
#include "colosso_log_rolling_stage.h"

typedef struct StageEffect {
    u8 filler00[0x30];
    s32 move_rate_x;
    s32 move_rate_z;
    u8 filler38[0x1d];
    u8 state;
} StageEffect;

extern StageEffect *Map_Run(s32);

void Colosso_ConfigureSceneEventEffect(void)
{
    StageEffect *effect;
    s32 move_rate;

    effect = Map_Run(30);
    effect->state = 0;
    move_rate = 0x19999;
    effect->move_rate_z = move_rate;
    effect->move_rate_x = move_rate;
    Map_Apply(effect, 2);
    Map_Apply2(effect, 0x0200CC48);
    Map_Do(0x363);
}
