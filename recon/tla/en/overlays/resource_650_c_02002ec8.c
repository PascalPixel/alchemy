/* Near miss, differ 8: gcse's PRE merges case 4's load of gPartyState with
 * the switch's (inserting it on case 2's two edges), so one pseudo spans
 * both and takes r1, where the listing loads the switch's base into r3 and
 * case 4's afresh into r1; the second callback's pool load and case 4's
 * movs r0, #242 are then scheduled one instruction away. A one-pass block
 * around the switch only reorders (permute, 10,233 candidates, differ 8). */
#include "types.h"
#include "callback_scheduler.h"
#include "party_state.h"
#include "scene_ids.h"

s32 GameFlag_Test(s32 flag);
void Func_0200153c(void);
void Func_02001558(void);
void Func_020015a0(void);
void Func_020016d8(void);
void Func_02001ee0(void);
void Func_02002304(void);
void Func_02002fd4(void);
void Func_020031a8(s32 a);

s32 Func_02002ec8(void)
{
    Func_0200153c();
    Scheduler_AddOrUpdateCallback(Func_02001558, 0xc80);
    Scheduler_AddOrUpdateCallback(Func_020015a0, 0x480);
    switch (gPartyState.entrance) {
    case 2:
        if (GameFlag_Test(0x109) == 0)
            Func_020031a8(1);
    case 4:
        gPartyState.saved_scene = (s32)&SceneId_DeriHeya2;
        gPartyState.saved_entrance = 1;
        Func_02002fd4();
        break;
    case 11:
        Func_020016d8();
        break;
    case 12:
        Func_02001ee0();
        break;
    case 13:
        Func_02002304();
        break;
    }
    return 0;
}
