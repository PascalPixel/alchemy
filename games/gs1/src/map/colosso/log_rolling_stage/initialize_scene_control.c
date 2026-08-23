#include "colosso_log_rolling_stage.h"

typedef struct SceneControl {
    s16 enabled;
    s16 active;
    s16 scene_variant;
    s16 phase;
    s16 timer;
} SceneControl;

extern SceneControl Data_02001000;
extern u8 *Data_03001f3c;
extern u8 Data_0200bef1[];

extern s32 Func_02008e56(void);
extern void Func_02008e26(s32, s32);
extern s32 Func_02008f44(s32);
extern void Func_02008dfa(s32, s32);

void ColossoLogRollingStage_InitializeSceneControl(void)
{
    u8 *scene_state = Data_03001f3c;
    SceneControl *control = &Data_02001000;

    Func_02008e26(Func_02008e56(), (s32)(scene_state + 240));
    if (Func_02008f44(0x109) == 0) {
        control->enabled = 1;
        control->active = 1;
        control->scene_variant = *(u16 *)(scene_state + 224);
        control->timer = 0;
        control->phase = 0;
    }
    {
        s32 event_id = 0xc85;

        Func_02008dfa((s32)Data_0200bef1, event_id);
    }
}
