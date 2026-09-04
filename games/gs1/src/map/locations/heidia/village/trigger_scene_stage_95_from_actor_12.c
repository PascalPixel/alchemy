#include "types.h"

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

s32 Func_02003006(s32);
void Func_02006cd4(s32);

#define TriggerSceneStage95FromActor12 Func_02001718
void TriggerSceneStage95FromActor12(void)
{
    u8 *scene_state = Data_03001ebc;

    if (Func_02003006(12) != 0 && Data_02000240[294] == 0) {
        s16 *scene_stage;
        s32 next_stage;

        Func_02006cd4(0x02009719);
        scene_stage = (s16 *)(scene_state + 386);
        next_stage = 95;
        *scene_stage = next_stage;
    }
}
