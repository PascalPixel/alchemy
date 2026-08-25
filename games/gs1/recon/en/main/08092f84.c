#include "types.h"

#define BattleEvent_RunActionAndWait Func_08092f84

void BattleEvent_RunActionAndWait(s32 action)
{
    u8 *runtime = *(u8 **)0x03001ebc;
    s32 wait_token = Func_08092c40();
    s32 resolved_action;
    u32 frames = 0;

    Func_080030f8(1);
    resolved_action = Func_08092ba8(action);
    if (action <= 7) {
        s32 masked_action = action & 0x0fff;

        if (Func_0808d394(masked_action) == 0) {
            resolved_action = masked_action;
        }
    }
    Func_08015100(resolved_action);

    if (*(s32 *)(runtime + 0x1cc) == 0) {
        while (Func_08015050(wait_token) == 0) {
            u32 input;

            Func_080030f8(1);
            frames++;
            input = *(u32 *)0x03001ae8;
            if (frames > 600 ||
                ((input & 4) && (input & 0x100) &&
                 (input & 0x200) && (input & 1))) {
                Func_08015140();
            }
        }
    }

    Func_080030f8(1);
}
