#include "types.h"

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

s32 Func_02006dd6(s32);
s32 Func_020030b4(s32);
void Func_02006e02(s32);
void Func_02006d88(s32);
void Func_02006d8e(s32);

#define TriggerSceneStage96FromActor21 Func_020017bc

void TriggerSceneStage96FromActor21(void)
{
    u8 *scene_state = Data_03001ebc;

    if (Func_02006dd6(549) == 0 && Func_020030b4(21) != 0
        && Data_02000240[294] == 0) {
        s16 *scene_stage;
        s32 next_stage;

        Func_02006e02(549);
        Func_02006d88(0x020097bd);
        Func_02006d8e(0x0200975d);
        scene_stage = (s16 *)(scene_state + 386);
        next_stage = 96;
        *scene_stage = next_stage;
    }
}
