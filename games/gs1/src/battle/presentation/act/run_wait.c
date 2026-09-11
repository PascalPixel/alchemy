#include "types.h"
#include "scene.h"
#include "abi/battle/presentation/act/run_wait.h"

extern volatile u32 gIw;

void BattleEv_RunWait(s32 action)
{
    u8 *runtime = *(u8 **)0x03001ebc;
    s32 wait_token = Battle_Run();
    s32 resolved_action;
    u32 frames = 0;

    Battle_Run2(1);
    resolved_action = Battle_Run3(action);
    if (action <= 7) {
        s32 masked_action = action & 0x0fff;

        if (Battle_Run4(masked_action) == 0) {
            resolved_action = masked_action;
        }
    }
    Battle_Run5(resolved_action);

    if (*(s32 *)(runtime + 0x1cc) == 0) {
        while (Battle_Run6(wait_token) == 0) {
            Battle_Run2(1);
            frames++;
            if (frames > 600 ||
                ((gIw & 4) && (gIw & 0x100) &&
                 (gIw & 0x200) && (gIw & 1))) {
                Battle_Run7();
            }
        }
    }

    Battle_Run2(1);
}
