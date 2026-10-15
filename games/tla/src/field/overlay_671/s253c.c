#include "types.h"
#include "text/msg_ids.h"
#include "object_runtime.h"
#include "party_state.h"

extern u32 Data_02005af4[];
extern u32 Data_02005b1c[];
extern u32 Data_02005b60[];
extern u32 Data_02005ba4[];
extern u32 Data_02005be8[];
struct ObjectRuntime *Object_GetById(s32 id);
void Func_020056b4(void);
void Func_0200580c(s32 value);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
struct ObjectRuntime *Func_020057bc(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Battle_WaitMode0(s32 frames);
void Func_02005754(const u8 *message);
void Func_02005764(s32 id, s32 value);
void Func_02005724(s32 id, s32 target);
void WaitFrames(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *callback);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *callback);
void Func_020057a4(s32 speed, s32 acceleration);
void Func_02005774(s32 id, s32 value);
void Func_020057b4(void);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Func_020057e4(s32 a, s32 b);
void Func_020057dc(s32 a, s32 b);
void Func_020057ec(s32 a);
void Func_020057c4(s32 exit);

static __inline__ void Camera_SetBounds(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Motion_CamBounds(x, y, z, mode);
}

static __inline__ void Camera_SetSpeed(s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020057a4(speed, acceleration);
}

static __inline__ void Actor_SetSpeed(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_Turn(s32 id, s32 angle, s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_ArmCallback(id, angle, frames);
}

static __inline__ void Actor_Place(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_ResetAndSetPositionInMode2(id, x, z);
}

static __inline__ void Func_02005764_(s32 id, s32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005764(id, value);
}

static __inline__ void Func_02005774_(s32 id, s32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005774(id, value);
}

void Func_0200253c(void)
{
    Func_020056b4();
    Func_0200580c(0);
    Camera_SetBounds(-1, -1, -1, 0);
    Func_020057bc()->motion_flags = 0;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Battle_WaitMode0(40);
    Func_02005754(MsgGabombaFocusEnergies);
    Func_02005764_(24, 0);
    Func_02005724(23, gPartyState.current_owner);
    Func_02005724(5, gPartyState.current_owner);
    Func_02005724(6, gPartyState.current_owner);
    Func_02005724(7, gPartyState.current_owner);
    WaitFrames(1);
    Actor_SetSpeed(gPartyState.current_owner, 0xcccc, 0x6666);
    Actor_SetSpeed(5, 0xcccc, 0x6666);
    Actor_SetSpeed(6, 0xcccc, 0x6666);
    Actor_SetSpeed(7, 0xcccc, 0x6666);
    Actor_SetSpeed(23, 0xcccc, 0x6666);
    ObjectMotion_EnableActionAndSetCallback(23, Data_02005ba4);
    WaitFrames(10);
    ObjectMotion_EnableActionAndSetCallback(gPartyState.current_owner, Data_02005af4);
    ObjectMotion_EnableActionAndSetCallback(5, Data_02005b1c);
    ObjectMotion_EnableActionAndSetCallback(7, Data_02005b60);
    Object_RefreshSelectorById(23);
    Battle_WaitMode0(20);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_SetBounds(217 << 17, -1, 143 << 17, 1);
    Actor_SetSpeed(23, 0x13333, 0x9999);
    Object_SetActionCallbackAndRefreshById(23, Data_02005be8);
    Battle_WaitMode0(40);
    Func_02005774_(23, 0xd000);
    Func_02005764_(23, 0);
    Camera_SetSpeed(0x4cccc, 0x9999);
    Camera_SetBounds(217 << 17, -1, 168 << 16, 1);
    Func_020057b4();
    Battle_WaitMode0(20);
    Actor_Turn(6, 0x2000, 0);
    Actor_Turn(5, 0xa000, 0);
    Actor_Turn(gPartyState.current_owner, 0xa000, 0);
    Actor_Turn(7, 0x2000, 40);
    Object_GetById(7)->angle = 0x4000;
    Object_GetById(gPartyState.current_owner)->angle = 0x4000;
    Object_GetById(6)->angle = 0x4000;
    Object_GetById(5)->angle = 0x4000;
    Object_GetById(7)->action_flags &= 0xfe;
    Object_GetById(gPartyState.current_owner)->action_flags &= 0xfe;
    Object_GetById(6)->action_flags &= 0xfe;
    Object_GetById(5)->action_flags &= 0xfe;
    Actor_Place(7, 400, 272);
    Actor_Place(gPartyState.current_owner, 420, 272);
    Actor_Place(6, 444, 272);
    Actor_Place(5, 468, 272);
    Battle_WaitMode0(20);
    Func_020057e4(0x10000, 0);
    Func_020057dc(0, 0);
    Func_020057ec(80);
    WaitFrames(80);
    Func_020057c4(11);
}
