#include "types.h"

extern volatile u32 Data_03001ae8;

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
            Func_080030f8(1);
            frames++;
            if (frames > 600 ||
                ((Data_03001ae8 & 4) && (Data_03001ae8 & 0x100) &&
                 (Data_03001ae8 & 0x200) && (Data_03001ae8 & 1))) {
                Func_08015140();
            }
        }
    }

    Func_080030f8(1);
}
