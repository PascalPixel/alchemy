#include "types.h"
#include "scene.h"

/* battle/effects/run/run_event_action.c */
/*
 * Battle_Apply names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it. The callee is whatever
 * Battle_Run returned, not a fixed address.
 */

void UiText_DrawMessage(s32, s32);
s32 GameFlag_IsSet(s32);
void Battle_Reset();

extern char Value_00000927;

s32 BattleFx_RunEventAction(void *arg0, s32 arg1, s32 arg2)
{
    s32 resource;

    if (arg0 != NULL) {
        resource = FIELD_AT_OFFSET(arg0, s32 *, 8);
        if (resource != 0) {
            if (resource < 0x10000) {
                Battle_Reset();
                Battle_Do(FIELD_AT_OFFSET(arg0, s32 *, 8));
                Battle_Apply2(arg2, 0);
                Battle_Run();
            } else {
                Battle_Apply(arg1, arg2);
            }
        }
        if (GameFlag_IsSet(0x142) != 0) {
            Battle_Reset();
            UiText_DrawMessage((s32)&Value_00000927, 1);
            Battle_Run();
        }
    }
    return 0;
}
