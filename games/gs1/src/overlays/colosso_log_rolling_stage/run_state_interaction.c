#include "colosso_log_rolling_stage.h"

extern s16 Data_02000240[];
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern u8 Value_0000207c;

extern void Func_0200760c(void);
extern void Func_0200741c(s32, s32);
extern void Func_0200757a(s32);
extern void Func_02007592(s32, s32);
extern s32 Func_0200748c(s32);
extern s32 Func_0200749c(s32);
extern s32 Func_0200747e(s32);
extern void Func_020074cc(s32);
extern void Func_020075ca(s32);
extern s32 Func_020075da(s32, s32);
extern s32 Func_0200753a(s32, s32);

s32 ColossoLogRollingStage_RunStateInteraction(s32 actor_handle, s32 interaction_base)
{
    s32 stage_variant;
    s32 script_id;
    s32 result;

    Func_0200760c();
    Func_0200741c(interaction_base, 5);
    stage_variant = Data_02000240[224];
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)&Value_00002076;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)&Value_00002078;
    } else {
        script_id = (s32)&Value_0000207a;
    }
    Func_0200757a(script_id);
    Func_02007592(actor_handle, 0);
    if (Func_0200748c(interaction_base + 512) != 0) {
        return 2;
    }
    if (Func_0200749c(interaction_base + 520) != 0) {
        result = Func_0200747e(0);
        if (result == 1) {
            return 2;
        }
        if (result == 2 || result == -1) {
            return 3;
        }
        return result;
    }
    Func_020074cc(interaction_base + 520);
    Func_020075ca((s32)&Value_0000207c);
    Func_020075da(actor_handle, 0);
    return Func_0200753a(0, 0);
}
