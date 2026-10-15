#include "types.h"
#include "iwram_call.h"
#include "call.h"
#include "party_state.h"
#include "scene_ids.h"

extern u16 gData_02001ae0[];

void Func_020012b4(s32, s32);
void Func_020012fc(void);
void Func_02001304();
void Scheduler_AddOrUpdateCallback();
void Func_02000738(void);

void Func_02000768(void)
{
    if (gPartyState.entrance == 99) {
        gPartyState.renderMode = 7;
        gPartyState.ownerOverride = 1;
        Func_020012b4((s32)&SceneId_Ov66bRoom2, 1);
    }
    Func_020012fc();
    Value3((s32 (*)())Func_02001304, 0, 9, 10);
    Value3((s32 (*)())Iwram_CopyWordsEntry, (s32)gData_02001ae0, 0x05000018, 8);
    Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02000738, 0x480);
}
