/* differ 22: argument and constant scheduling around the turn service calls and the event delay store */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "field_event.h"
#include "callback_scheduler.h"
#include "text/msg_ids.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 Data_020007e4;

struct FieldActor *Object_GetById(s32 id);
void Object_SetPartAttribute(struct FieldActor *object, s32 value);
void WaitFrames(s32 frames);
void ObjectMotion_SetActionVariant(u32, s32);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_ArmCallback(s32 id, s32 value, s32 flags);
void ObjectMotion_Launch(s32 id, s32 height, s32 frames);
void Object_SetModeById(u32, s32);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
u32 Random16Far(void);
void GameFlag_SetBit(s32 flag);
void Func_02000568(void);
void Func_020005f8(s32 skip);
void Func_02000600(s32 speaker, s32 listener);
void Func_02000598(s32 message);
void Func_020005a0(s32 speaker, s32 flags);
void Func_02000608(void);
void Func_020005c0(s32 id, s32 a, s32 b);
void Func_020005b0(s32 id, s32 value);
void Func_020005c8(s32 countdown);
void Func_020000f4(void);
void Func_02000060(void);


#define LAYER /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each call's constants */
static inline void Talk(s32 a, s32 b) { LAYER Func_02000600(a, b); }
static inline void Say(s32 a, s32 b) { LAYER Func_020005a0(a, b); }
static inline void Turn(s32 a, s32 b, s32 c) { LAYER Func_020005c0(a, b, c); }
static inline void Arm(s32 a, s32 b, s32 c) { LAYER ObjectMotion_ArmCallback(a, b, c); }
static inline void Hop(s32 a, s32 b, s32 c) { LAYER ObjectMotion_Launch(a, b, c); }
static inline void Mode(s32 a, s32 b) { LAYER Object_SetModeById(a, b); }
static inline void Variant(s32 a, s32 b) { LAYER ObjectMotion_SetActionVariant(a, b); }
static inline void Pose(s32 a, s32 b) { LAYER Func_020005b0(a, b); }
static inline void ModeWait(s32 a, s32 b) { LAYER Motion_SetModeAndWaitAnimation(a, b); }

void Func_020001b0(void)
{
    struct FieldActor *object;

    Func_02000568();
    Func_020005f8(0);
    Object_SetPartAttribute(Object_GetById(gPartyState.currentOwner), 15);
    WaitFrames(1);
    Data_020007e4 = 0;
    Scheduler_AddOrUpdateCallback(Func_020000f4, 1152);
    Variant(8, 2);
    Variant(9, 2);
    WORD_AT(Ram_HeapSlots->eventWork, 436) = 40;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Battle_WaitMode0(40);
    Talk(13, 14);
    Func_02000598((s32)MsgImilVoicesDisbeliefReply);
    Say(3, 0);
    Func_02000608();
    Turn(8, 256, 0);
    Turn(8, 256, 20);
    Arm(8, 0x3000, 0);
    Hop(8, 4, 10);
    Say(0x8008, 0);
    Turn(11, 257, 0);
    Turn(12, 257, 40);
    Arm(11, 0xd000, 0);
    Arm(12, 0xb000, 0);
    Talk(13, 14);
    Say(3, 0);
    Func_02000608();
    Mode(9, 3);
    Say(0x8009, 0);
    Pose(12, 0);
    Say(0x800c, 0);
    Pose(11, 0x8000);
    Mode(11, 4);
    Say(0x400b, 0);
    object = Object_GetById(11);
    BYTE_AT(object, 98) = ((Random16Far() * 80) >> 16) + 40;
    BYTE_AT(object, 99) = 0;
    WORD_AT(object, 108) = (s32)Func_02000060;
    object = Object_GetById(12);
    BYTE_AT(object, 98) = ((Random16Far() * 80) >> 16) + 40;
    BYTE_AT(object, 99) = 2;
    WORD_AT(object, 108) = (s32)Func_02000060;
    Talk(13, 14);
    Say(3, 0);
    Func_02000608();
    Mode(8, 4);
    Say(0x8008, 0);
    Talk(13, 14);
    Say(3, 0);
    Func_02000608();
    Say(0x8009, 0);
    Talk(13, 14);
    Say(3, 0);
    Func_02000608();
    Say(0x8008, 0);
    Talk(13, 14);
    Say(3, 0);
    Func_02000608();
    Say(0x8009, 0);
    Talk(13, 14);
    Say(3, 0);
    Func_02000608();
    ModeWait(8, 3);
    Say(0x8008, 0);
    Talk(13, 14);
    Say(1, 0);
    Func_02000608();
    Hop(9, 2, 0);
    Turn(9, 263, 20);
    Say(0x8009, 0);
    Talk(13, 14);
    Say(3, 0);
    Func_02000608();
    ModeWait(8, 3);
    Say(0x8008, 0);
    Talk(13, 14);
    Say(6, 0);
    Func_02000608();
    Battle_WaitMode0(20);
    *(u16 *)0x05000000 = 0;
    WaitFrames(1);
    WORD_AT(Ram_HeapSlots->eventWork, 436) = 16;
    WORD_AT(Ram_HeapSlots->eventWork, 428) = 513;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    GameFlag_SetBit(0xa3c);
    Func_020005c8(3);
}
