#include "colosso_log_rolling_stage.h"

extern s16 Data_02000240[];
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;

extern void Func_020074d2(s32, s32);
extern void Func_02007632(s32);
extern void Func_0200764a(s32, s32);

void ColossoLogRollingStage_InitializeStateInteraction(s32 actor_handle, s32 interaction_base)
{
    s32 stage_variant;
    s32 script_id;

    Func_020074d2(interaction_base, 5);
    stage_variant = Data_02000240[224];
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)&Value_00002076;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)&Value_00002078;
    } else {
        script_id = (s32)&Value_0000207a;
    }
    Func_02007632(script_id + 1);
    Func_0200764a(actor_handle, 0);
}
