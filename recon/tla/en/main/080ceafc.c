#include "TYPES.H"
#include "SCENE.H"
s32 __umodsi3(s32, s32) __attribute__((const));
void Event_SetValue1d8(s32);
void BattleEv_RunWait(s32, s32);
void BattleFx_FinishAction();
void _call_via_r3(s32, s32);

/* battle/effects/run/run_event_action.c */
/*
 * _call_via_r3 names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it. The callee is whatever
 * BattleFx_FinishAction returned, not a fixed address.
 */

void UiText_ShowPositionedMessageAndWaitFar(s32, s32);
s32 GameFlag_TestFar(s32);
void Battle_Reset();

extern char MsgNothingHappens;

s32 BattleFx_RunEventAction(void *arg0, s32 arg1, s32 arg2)
{
    s32 resource;

    if (arg0 != NULL) {
        resource = FIELD_AT_OFFSET(arg0, s32 *, 8);
        if (resource != 0) {
            if (resource < 0x10000) {
                Battle_Reset();
                Event_SetValue1d8(FIELD_AT_OFFSET(arg0, s32 *, 8));
                BattleEv_RunWait(arg2, 0);
                BattleFx_FinishAction();
            } else {
                _call_via_r3(arg1, arg2);
            }
        }
        if (GameFlag_TestFar(0x142) != 0) {
            Battle_Reset();
            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgNothingHappens, 1);
            BattleFx_FinishAction();
        }
    }
    return 0;
}
