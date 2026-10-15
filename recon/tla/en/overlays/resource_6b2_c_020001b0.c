/* Draft of Func_020001b0 (overlay 6B2, 303 instructions), differ 12: the scene of
 * Imiru's voices (actors 8, 9, 11 and 12), written from the listing. Calls with
 * constants go through call.h's Call2 and Call3, Func_020005c0 through Value3 (as
 * Call3: differ 22; all direct: 135). Three scheduling differences remain:
 *  - the event work's frames store: the native builds 0x3000000 (copied to r8)
 *    before it starts 436 (copied to r10); here 436 is started and copied first;
 *  - actor 11: the native loads Func_02000060 into r5 between the timer's address
 *    and its + 40; here after the timer's store;
 *  - actor 12: the timer's address (+ 98) comes before its + 40 here, after it in
 *    the native.
 * Measured: both actors through one inline helper (differ 67). alchemy permute,
 * 6 minutes: nothing closer (differ 18 at best). */
#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "call.h"

extern u32 Data_020007e4;
extern u8 MsgImiruVoicesDisbeliefReply[];

struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void GameFlag_SetBit(s32 flag);
u32 Random16Far(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_Launch(s32 actor, s32 height, s32 frames);
void Func_02000060(struct ObjectRuntime *object);
void Func_020000f4(void);
void Func_02000568(void);
void Func_02000598(const u8 *message);
void Func_020005a0(s32 a, s32 b);
void Func_020005b0(s32 actor, s32 value);
void Func_020005c0(s32 actor, s32 a, s32 b);
void Func_020005c8(s32 a);
void Func_020005f8(s32 skip);
void Func_02000600(s32 a, s32 b);
void Func_02000608(void);

void Func_020001b0(void)
{
    struct ObjectRuntime *actor;

    Func_02000568();
    Func_020005f8(0);
    Object_SetPartAttribute(Object_GetById(gPartyState.current_owner), 15);
    WaitFrames(1);
    Data_020007e4 = 0;
    Scheduler_AddOrUpdateCallback(Func_020000f4, 0x480);
    ObjectMotion_SetActionVariant(8, 2);
    ObjectMotion_SetActionVariant(9, 2);
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 436) = 40;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Battle_WaitMode0(40);
    Call2((void (*)())Func_02000600, 13, 14);
    Func_02000598(MsgImiruVoicesDisbeliefReply);
    Call2((void (*)())Func_020005a0, 3, 0);
    Func_02000608();
    Value3((s32 (*)())Func_020005c0, 8, 256, 0);
    Value3((s32 (*)())Func_020005c0, 8, 256, 20);
    Call3((void (*)())ObjectMotion_ArmCallback, 8, 0x3000, 0);
    Call3((void (*)())ObjectMotion_Launch, 8, 4, 10);
    Call2((void (*)())Func_020005a0, 0x8008, 0);
    Value3((s32 (*)())Func_020005c0, 11, 257, 0);
    Value3((s32 (*)())Func_020005c0, 12, 257, 40);
    Call3((void (*)())ObjectMotion_ArmCallback, 11, 0xd000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 12, 0xb000, 0);
    Call2((void (*)())Func_02000600, 13, 14);
    Call2((void (*)())Func_020005a0, 3, 0);
    Func_02000608();
    Object_SetModeById(9, 3);
    Call2((void (*)())Func_020005a0, 0x8009, 0);
    Call2((void (*)())Func_020005b0, 12, 0);
    Call2((void (*)())Func_020005a0, 0x800c, 0);
    Call2((void (*)())Func_020005b0, 11, 0x8000);
    Object_SetModeById(11, 4);
    Call2((void (*)())Func_020005a0, 0x400b, 0);
    actor = Object_GetById(11);
    *((u8 *)actor + 98) = ((Random16Far() * 80) >> 16) + 40;
    *((u8 *)actor + 99) = 0;
    actor->update = (void (*)(void))Func_02000060;
    actor = Object_GetById(12);
    *((u8 *)actor + 98) = ((Random16Far() * 80) >> 16) + 40;
    *((u8 *)actor + 99) = 2;
    actor->update = (void (*)(void))Func_02000060;
    Call2((void (*)())Func_02000600, 13, 14);
    Call2((void (*)())Func_020005a0, 3, 0);
    Func_02000608();
    Object_SetModeById(8, 4);
    Call2((void (*)())Func_020005a0, 0x8008, 0);
    Call2((void (*)())Func_02000600, 13, 14);
    Call2((void (*)())Func_020005a0, 3, 0);
    Func_02000608();
    Call2((void (*)())Func_020005a0, 0x8009, 0);
    Call2((void (*)())Func_02000600, 13, 14);
    Call2((void (*)())Func_020005a0, 3, 0);
    Func_02000608();
    Call2((void (*)())Func_020005a0, 0x8008, 0);
    Call2((void (*)())Func_02000600, 13, 14);
    Call2((void (*)())Func_020005a0, 3, 0);
    Func_02000608();
    Call2((void (*)())Func_020005a0, 0x8009, 0);
    Call2((void (*)())Func_02000600, 13, 14);
    Call2((void (*)())Func_020005a0, 3, 0);
    Func_02000608();
    Motion_SetModeAndWaitAnimation(8, 3);
    Call2((void (*)())Func_020005a0, 0x8008, 0);
    Call2((void (*)())Func_02000600, 13, 14);
    Call2((void (*)())Func_020005a0, 1, 0);
    Func_02000608();
    Call3((void (*)())ObjectMotion_Launch, 9, 2, 0);
    Value3((s32 (*)())Func_020005c0, 9, 263, 20);
    Call2((void (*)())Func_020005a0, 0x8009, 0);
    Call2((void (*)())Func_02000600, 13, 14);
    Call2((void (*)())Func_020005a0, 3, 0);
    Func_02000608();
    Motion_SetModeAndWaitAnimation(8, 3);
    Call2((void (*)())Func_020005a0, 0x8008, 0);
    Call2((void (*)())Func_02000600, 13, 14);
    Call2((void (*)())Func_020005a0, 6, 0);
    Func_02000608();
    Battle_WaitMode0(20);
    *(u16 *)0x05000000 = 0;
    WaitFrames(1);
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 436) = 16;
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 428) = 0x201;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    GameFlag_SetBit(0xa3c);
    Func_020005c8(3);
}
