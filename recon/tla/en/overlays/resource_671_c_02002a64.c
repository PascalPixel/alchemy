/* Draft, differ 38 of 521: written from the listing with the scene-layer inline wrappers of
 * gabomba.c (where it adopts, before Func_02002f70). Left: every Func_0200578c call sets the id
 * last, where the draft sets it before the third argument (12 rows); the counter at event work
 * 0x1c4 swaps r2 and r3 in the else branch; the speed 0xc000 finishes one row early; the second
 * party-state address takes r2 where the native has r3. UiText_OpenMessageAtObject declared
 * without a prototype sets r1 between the halves of 0x6005 as the native does (40 to 38). Tried:
 * Func_0200578c through call.h Call3, unprototyped, with u8, s16 or u16 ids (38), an argument
 * barrier in the wrapper (124) or in a macro (67). */
#include "types.h"
#include "text/msg_ids.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

extern const u8 Data_02005944[];
extern const u8 Data_02005d7c[];
extern u32 Data_02005ccc[];
extern u32 Data_02005d24[];
extern u32 Data_02005d8c[];
struct ObjectRuntime *Object_GetById(s32 id);
u8 *UnitGet(s32 owner);
void Func_0200533c(void);
void Func_02005364(void);
void Func_02005724(s32 id, s32 target);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void WaitFrames(s32 frames);
void Func_02005754(const u8 *message);
void Func_02005764(s32 id, s32 value);
void Func_02005714(s32 id, s32 a, s32 b);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02001b38(void);
void Func_02001b68(void);
void Func_02002394(void);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *callback);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *callback);
void Func_02005674(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020053bc(const u8 *data);
void Func_02005824(void);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);
void Func_0200582c(s32 index, s32 a, s32 b);
s32 Func_02003604(void);
void Func_020056b4(void);
void Func_0200580c(s32 value);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
s32 Func_020048a4(s32 x, s32 z, s32 value, s32 id);
void Func_0200571c(s32 id, s32 x, s32 z, s32 speed);
void Func_0200564c(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020057a4(s32 speed, s32 acceleration);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void Func_02005774(s32 id, s32 value);
void Func_020057b4(void);
void Func_02004f40(s32 *x_out, s32 *z_out);
struct ObjectRuntime *Func_0200563c(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_02005624(struct ObjectRuntime *object, s32 mode);
s32 ArcTan2(s32 y, s32 x);
void Func_02005644(struct ObjectRuntime *object);
void Func_02005794(s32 id, s32 value);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_Launch(s32 id, s32 a, s32 b);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Func_0200578c(s32 id, s32 a, s32 b);
void Func_020057e4(s32 a, s32 b);
void Func_020057dc(s32 a, s32 b);
void Func_020057ec(s32 a);
void Func_020057c4(s32 exit);

static __inline__ void Actor_SetSpeed(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_Slide(s32 id, s32 x, s32 z, s32 speed)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200571c(id, x, z, speed);
}

static __inline__ void Actor_Turn(s32 id, s32 angle, s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_ArmCallback(id, angle, frames);
}

static __inline__ void Actor_Walk(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetPositionAndReset(id, x, z);
}

static __inline__ void Func_02005774_(s32 id, s32 value)
{
    /* FAKEMATCH: forwarding through the inline sets the id between the halves of the split constant, as the native does; called directly the constant is built first (one reordered pair, differ 2). */
    Func_02005774(id, value);
}

static __inline__ void Camera_SetBounds(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Motion_CamBounds(x, y, z, mode);
}

extern u32 Data_02005c40[];

static __inline__ void Func_0200578c_(s32 id, s32 a, s32 b)
{
    Func_0200578c(id, a, b);
}

static __inline__ void Func_02005764_(s32 id, s32 value)
{
    Func_02005764(id, value);
}

struct ObjectRuntime *Func_020057bc(void);
void Event_FinishScreenTransition(void);
void Func_0200581c(void);
s32 UiText_OpenMessageAtObject();
s32 Inventory_PromptAndSetObjectMode(s32 id, s32 flags);

void Func_02002a64(void)
{
    s32 speed;

    Func_020056b4();
    Func_0200580c(0);
    Camera_SetBounds(-1, -1, -1, 0);
    speed = 0xc000;
    Actor_Slide(gPartyState.current_owner, 174 << 17, 156 << 17, speed);
    Actor_Slide(7, 165 << 17, 156 << 17, speed);
    {
        s32 slow = 0xb000;

        Actor_Slide(23, 216 << 17, 156 << 17, slow);
    }
    Actor_Slide(6, 224 << 17, 156 << 17, speed);
    Actor_Slide(5, 234 << 17, 156 << 17, speed);
    WaitFrames(1);
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 0x201;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Battle_WaitMode0(40);
    Func_0200578c_(23, 256, 20);
    Func_02005754(MsgGabombaUnderstandsChannel);
    Func_02005764_(23, 0);
    Func_020057bc()->motion_flags = 0;
    Func_020057a4(0xcccc, 0x1999);
    Camera_SetBounds(196 << 17, -1, 146 << 17, 1);
    Func_020057b4();
    Actor_Turn(gPartyState.current_owner, 0, 0);
    Actor_Turn(5, 0x8000, 0);
    Actor_Turn(7, 0, 0);
    Func_02005774_(6, 0x8000);
    Object_SetModeById(6, 3);
    Func_02005764_(0x4006, 0);
    UiText_OpenMessageAtObject(0x6005, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
        Func_0200578c(7, 256, 20);
        Actor_SetSpeed(7, 0x20000, 0x10000);
        Actor_Walk(7, 340, 296);
        Actor_Walk(7, 354, 296);
        Actor_Walk(7, 364, 308);
        Actor_Turn(gPartyState.current_owner, 0xe000, 0);
        Func_02005774_(7, 0x6000);
        Func_02005764(0x6007, 0);
        (*(u16 *)((u8 *)Ram_HeapSlots->event_work + 0x1c4))++;
    } else {
        (*(u16 *)((u8 *)Ram_HeapSlots->event_work + 0x1c4))++;
        Actor_SetSpeed(7, 0x20000, 0x10000);
        Actor_Walk(7, 340, 296);
        Actor_Walk(7, 354, 296);
        Actor_Walk(7, 364, 308);
        Actor_Turn(gPartyState.current_owner, 0xe000, 0);
        Func_02005774_(7, 0x6000);
        Object_SetModeById(7, 3);
        Func_02005764(0x6007, 0);
    }
    Func_02005774_(23, 0x8000);
    Motion_SetModeAndWaitAnimation(23, 4);
    Func_02005764_(0x6017, 0);
    Actor_Turn(7, 0, 0);
    Func_0200578c_(7, 257, 40);
    Func_02005764_(0x6017, 0);
    Motion_SetModeAndWaitAnimation(7, 3);
    Func_0200578c_(23, 265, 20);
    Func_02005764_(0x6017, 0);
    Func_02005764_(0x4007, 0);
    Func_0200578c_(7, 256, 40);
    Func_02005764_(0x4007, 0);
    Object_SetModeById(23, 3);
    Func_02005764_(0x4017, 0);
    Func_0200578c_(7, 265, 10);
    Func_02005764_(0x4007, 0);
    Actor_SetSpeed(6, 0xcccc, 0x6666);
    Actor_Walk(6, 440, 296);
    Func_02005774_(6, 0x8000);
    Motion_SetModeAndWaitAnimation(6, 3);
    Func_02005764_(0x4006, 0);
    Actor_Turn(7, 0xe000, 0);
    Func_0200578c_(7, 261, 0);
    Actor_Turn(6, 0x2000, 0);
    Actor_Turn(5, 0xa000, 0);
    Actor_Turn(gPartyState.current_owner, 0, 80);
    Func_0200578c_(7, 261, 40);
    Func_02005774_(7, 0);
    Func_02005764_(0x4007, 0);
    Actor_Turn(5, 0x8000, 0);
    Func_02005774_(6, 0x8000);
    Object_SetModeById(gPartyState.current_owner, 3);
    Object_SetModeById(23, 3);
    Object_SetModeById(5, 3);
    Motion_SetModeAndWaitAnimation(6, 3);
    Motion_SetModeAndWaitAnimation(7, 3);
    Func_02005774_(23, 0xb000);
    Func_02005764_(23, 0);
    Func_0200578c_(23, 257, 0);
    Actor_Turn(23, 0xd000, 20);
    Func_02005774_(23, 0xb000);
    Func_02005764_(23, 0);
    Func_020057a4(0x4cccc, 0x9999);
    Camera_SetBounds(196 << 17, -1, 176 << 16, 1);
    Func_020057b4();
    Func_02005764_(23, 0);
    Func_02005764_(6, 0);
    Func_02005764_(23, 0);
    Func_02005764_(7, 0);
    Func_02005764_(0x2005, 0);
    Camera_SetBounds(196 << 17, -1, 146 << 17, 1);
    Func_020057b4();
    Object_SetModeById(23, 3);
    Func_02005764_(23, 0);
    Func_02005774_(23, 0x8000);
    Func_02005764_(23, 0);
    Actor_SetSpeed(5, 0x19999, 0xcccc);
    Actor_SetSpeed(6, 0x19999, 0xcccc);
    Actor_SetSpeed(23, 0x19999, 0xcccc);
    Actor_SetSpeed(7, 0xcccc, 0x6666);
    ObjectMotion_EnableActionAndSetCallback(7, Data_02005c40);
    ObjectMotion_EnableActionAndSetCallback(23, Data_02005c40);
    ObjectMotion_EnableActionAndSetCallback(6, Data_02005c40);
    Object_SetActionCallbackAndRefreshById(5, Data_02005c40);
    GameFlag_SetBit(0x8f6);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_0200581c();
    Func_020057c4(13);
}
