/* Near miss, differ 6: three calls set their argument registers in another
 * order (the listing builds r0 last for Func_02001304 and loads the callback
 * after the shift for Scheduler_AddOrUpdateCallback); direct calls and the
 * call.h layer give the same code. */
#include "types.h"
#include "iwram_call.h"
#include "call.h"
#include "party_state.h"
#include "scene_ids.h"

extern u16 Data_02001ae0[];

void Func_020012b4(s32, s32);
void Func_020012fc(void);
void Func_02001304();
void Scheduler_AddOrUpdateCallback();
void Func_02000738(void);

void Func_02000768(void)
{
    if (gPartyState.entrance == 99) {
        gPartyState.render_mode = 7;
        gPartyState.owner_override = 1;
        Func_020012b4((s32)&SceneId_Ov66bRoom2, 1);
    }
    Func_020012fc();
    Call3(Func_02001304, 0, 9, 10);
    Iwram_CopyWords(Data_02001ae0, (void *)0x05000018, 8);
    Call2(Scheduler_AddOrUpdateCallback, (s32)Func_02000738, 0x480);
}
