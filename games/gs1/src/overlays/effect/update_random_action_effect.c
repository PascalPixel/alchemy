#include "resource_3a8_effect.h"

#define SceneEffect_UpdateRandomAction Func_02000040

s32 Func_02003b7e();
s32 Func_02003bac();
s32 Func_02003bba();
u8 Func_02003bcc(void *, s32);
u8 Func_02003bd6(void *, s32);
#define SampleEffectAction Func_02003b7e
#define SampleEffectOffset Func_02003bac
#define SampleEffectTimer Func_02003bba
#define RunEffectAction3 Func_02003bcc
#define RunEffectAction4 Func_02003bd6

s32 SceneEffect_UpdateRandomAction(struct Resource3a8Effect *effect) {
    u32 action;
    u32 next_timer;

    if (effect->action_timer == 0) {
        action = (u32) (SampleEffectAction() * 8) >> 0x10;
        switch (action) {
        case 0:
            RunEffectAction3(effect, 3);
            break;
        case 1:
            RunEffectAction4(effect, 4);
            break;
        case 3:
        case 4:
            effect->unknown_06 += (u32) (SampleEffectOffset() << 0xF) >> 0x10;
            break;
        }
        next_timer = (u32) (SampleEffectTimer() * 0x50) >> 0x10;
        effect->action_timer = (s16) next_timer;
        if (next_timer != 0) {
            goto decrement_timer;
        }
    } else {
decrement_timer:
        --effect->action_timer;
    }
    return 1;
}
