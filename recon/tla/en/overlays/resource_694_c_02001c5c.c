/* Draft, differ 12 of 70: the native keeps the constants 1 and 2 of both Func_02004004 calls in r8
 * and r10 and stores them from there (movs r3, #1; mov r8, r3; movs r3, #2; mov sl, r3; mov r3,
 * r8; str r3, [sp]; mov r3, sl; str r3, [sp, #4]); this form stores each as it is built and keeps
 * them in r10 and r8. */
#include "types.h"
#include "party_state.h"

extern const u8 gData_0200466c[];

void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *script);
void Func_02004004(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width, s32 height);
void Func_0200406c(void);
void Func_02004204(s32 skip);
void Func_020042b4(s32 cue);

void Func_02001c5c(s32 a, s32 b, s32 c, s32 d)
{
    Func_0200406c();
    Func_02004204(0);
    Func_020042b4(188);
    Func_02004004(a, b, c, d, 1, 2);
    Func_02004004(a, b + 2, c + 1, d, 1, 2);
    Battle_WaitMode0(20);
    ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
    ObjectMotion_EnableActionAndSetCallback(gPartyState.currentOwner, gData_0200466c);
    WaitFrames(12);
    Func_020042b4(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
}
