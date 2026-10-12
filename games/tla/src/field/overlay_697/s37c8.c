#include "ov697.h"
#include "text/msg_ids.h"

void Battle_WaitMode0(s32 frames);
void Task_Wait(s32 frames);
void Motion_CamBounds(s32 a, s32 b, s32 c, s32 d);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_SetVariantCallback(s32 actor, s32 variant);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005880(void);
void Func_020059e8(s32 value);
void Func_02002d78(s32 actor, s32 x, s32 z, s32 angle);
void Func_02005a48(s32 actor, s32 angle);
void Func_02005938(s32 message);
void Func_02005940(s32 actor, s32 flags);

static __inline__ void Motion_CamBounds_(s32 a, s32 b, s32 c, s32 d)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Motion_CamBounds(a, b, c, d);
}

void Func_020037c8(void)
{
    Func_02005880();
    Func_020059e8(0);
    Motion_CamBounds_(-1, -1, -1, 0);
    Func_02002d78(8, 264, 264, 0x8000);
    Func_02002d78(13, 280, 248, 0x8000);
    Func_02002d78(14, 280, 280, 0x8000);
    Func_02002d78(4, 240, 276, 0);
    Func_02002d78(7, 216, 254, 0);
    Func_02002d78(5, 232, 244, 0);
    Func_02002d78(6, 224, 296, 0);
    Object_SetModeById(4, 38);
    Object_SetModeById(5, 19);
    Object_SetModeById(6, 19);
    Object_SetModeById(7, 19);
    Func_02005a48(7, 0x4000);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(4), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(5), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(6), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(7), 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Battle_WaitMode0(30);
    Func_02005938((s32)MsgTrialRoadLeaveVillage);
    Motion_SetModeAndWaitAnimation(8, 3);
    Func_02005940(8, 0);
    ObjectMotion_SetVariantCallback(4, 2);
    Task_Wait(1);
    ObjectMotion_SetVariantCallback(6, 2);
    Task_Wait(2);
    ObjectMotion_SetVariantCallback(5, 2);
    ObjectMotion_SetVariantCallback(7, 2);
    Battle_WaitMode0(15);
}
