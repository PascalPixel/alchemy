#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventWorkRoute {
    u8 unknown_000[0x1ac];
    s32 route;
    u8 unknown_1b0[4];
    s32 countdown;
};

struct EventWork1c4 {
    u8 unknown_000[0x1c4];
    s16 message;
};

struct AnimationRecord26 {
    u8 unknown_00[0x1a];
    u8 frame;
};

extern u8 MsgFieldAddressCompanion;
extern const u8 Data_020005e4[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void Object_RefreshSelectorById(u32 id);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 BattleFx_GetResourceId(s32 id);
void UiText_GetResourceDimensions(s32 id, s32 *a, s32 *b, s32 *c, s32 *d);
void UiText_OpenMessageWindow(s32 id, s32 x, s32 y, u32 flags);
s32 UiWork_IsComplete(void);
void UiWork_Finalize(s32 work, s32 mode);
void UiWork_FinalizePendingCore(void);
s32 Func_0200053c(s32 a, s32 b, s32 c, s32 d);
void Func_0200055c(void);
void Func_02000564(void);
void Func_0200058c(s32 id, s32 x, s32 z);
void Func_0200059c(s32 message);
void Func_020005ac(s32 countdown);
void Func_020005cc(s32 skip);
void Func_020005dc(s32 cue);
void Func_02000310(s32 kind, s32 frames);

static __inline__ s32 Flag_Test(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_SetBit(flag);
}

static __inline__ void Flag_Clear(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_ClearBit(flag);
}

static __inline__ void Show(s32 kind, s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02000310(kind, frames);
}

void Func_02000120(void)
{
    if (Flag_Test(0x800) != 0) {
        Func_0200055c();
        Func_020005cc(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        gPartyState.render_mode = 6;
        gPartyState.owner_override = 1;
        Object_SetModeById(8, 7);
        Func_02000564();
    } else {
        gPartyState.current_owner = 4;
        gPartyState.render_mode = 0;
        gPartyState.owner_override = 0;
    }
}
