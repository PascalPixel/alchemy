#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "eventwrk.h"

extern u8 MsgFieldAddressCompanion;
extern const u8 Data_020005e4[];

void Battle_WaitMode0(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void Object_RefreshSelectorById(u32 id);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200055c(void);
void Func_02000564(void);
void Func_0200059c(s32 message);
void Func_020005ac(s32 countdown);
void Func_020005cc(s32 skip);
void Func_02000310(s32 kind, s32 frames);

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline sets the id between the speed's last add and the call, as the native does (differ 2 called directly). */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

void Func_020003cc(void)
{
    struct EventWork *work;
    u8 saved;

    saved = gPartyState.event_mode;
    gPartyState.event_mode = 2;
    Func_0200055c();
    Func_020005cc(0);
    ObjectMotion_SetSpeedParameters_(8, 0x451e, 0x228f);
    ObjectMotion_EnableActionAndSetCallback(8, Data_020005e4);
    Battle_WaitMode0(80);
    Func_0200059c((s32)&MsgFieldAddressCompanion);
    Func_02000310(6, 90);
    Func_02000310(7, 90);
    Func_02000310(6, 180);
    Func_02000310(7, 90);
    Func_02000310(6, 250);
    Func_02000310(7, 180);
    Func_02000310(5, 250);
    Func_02000310(8, 90);
    Func_02000310(7, 180);
    Func_02000310(8, 180);
    Func_02000310(7, 180);
    Func_02000310(6, 90);
    Func_02000310(7, 250);
    Func_02000310(5, 250);
    Func_02000310(7, 250);
    Func_02000310(6, 90);
    Func_02000310(5, 90);
    Func_02000310(8, 90);
    Func_02000310(7, 250);
    Func_02000310(4, 90);
    Func_02000310(7, 250);
    Object_RefreshSelectorById(8);
    Battle_WaitMode0(10);
    work = Ram_HeapSlots->event_work;
    work->delay = 30;
    work->screen_effect = 0x204;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020005ac(1);
    gPartyState.event_mode = saved;
    Func_02000564();
}
