#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 offset);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 flags);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void UiText_OpenMessageAtObject(s32 id, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 id, s32 flags);
void Func_02002ae8(const void *data, s32 a, s32 b);
void Func_02002af0(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Func_02002af8(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Func_02002b28(void);
void Func_02002b30(void);
void Func_02002b88(s32 id, s32 a, s32 b);
void Func_02002b90(s32 id, s32 x, s32 y, s32 z);
void Func_02002bd0(s32 message);
void Func_02002be0(s32 id, s32 a, s32 b);
void Func_02002be8(s32 a, s32 b);
void Func_02002bf8(s32 id, s32 a);
void Func_02002c08(s32 id, s32 a, s32 b);
void Func_02002c30(s32 a);
void Func_02002c58(s32 a);
s32 Func_02002c80(void);
void Func_02002c88(s32 a, s32 b);
void Func_02002c90(s32 a);
void Func_02000f80(void);
void Func_020019d8(void);

extern const u8 Data_02002d38[];
extern const u8 Data_02002ff0[];
extern const u8 Data_02003358[];
extern const u8 Data_0200367c[];
extern const u8 Data_0200394c[];

extern u8 MsgItLooksLikeThe;
extern u8 MsgTheArmorerAnd;
extern u8 MsgAfterTheChampa;
extern u8 MsgPeopleAreSaying;
extern u8 MsgIKnewThere;
extern u8 MsgOwOwOw;
extern u8 MsgMyBoyfriendShin;
extern u8 MsgAllMyInjuries;
extern u8 MsgMySisterWas;
extern u8 MsgIfYouDon;
extern u8 MsgHaveYouBy;
extern u8 MsgUsingPsynergyOn;
extern u8 MsgWhereCouldMenardi;
extern u8 MsgEveryoneHereIn;
extern u8 MsgYeahWellWe;
extern u8 MsgYouCominThrough;
extern u8 MsgYeahUhWe;
extern u8 MsgOhUhYou;
extern u8 MsgWasIJust;
extern u8 MsgHeyMisterAre;
extern u8 MsgDidYouCome;
extern u8 MsgIsThereBy;

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

static __inline__ void CommitPos(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CommitPositionAndActivate(id, x, z);
}

static __inline__ void ArmCallback(s32 id, s32 angle, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_ArmCallback(id, angle, flags);
}

static __inline__ void Walk(s32 id, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002c08(id, a, b);
}

static __inline__ void OpenAt(s32 id, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    UiText_OpenMessageAtObject(id, flags);
}

static __inline__ void CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002af0(a, b, c, d, e, f);
}

static __inline__ void CopyCells2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002af8(a, b, c, d, e, f);
}

static __inline__ void Emote(s32 id, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002be0(id, a, b);
}

static __inline__ void Face(s32 id, s32 target, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetAngleToward(id, target, flags);
}

static __inline__ void Anim(s32 id, s32 mode)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Motion_SetModeAndWaitAnimation(id, mode);
}

static __inline__ void Show(s32 id, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002b88(id, a, b);
}

static __inline__ void Place(s32 id, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002b90(id, x, y, z);
}

static __inline__ void Speed(s32 id, s32 s, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(id, s, a);
}

static __inline__ void SetPos(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetPositionAndReset(id, x, z);
}

static __inline__ void Turn(s32 id, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002bf8(id, a);
}

void Func_020029ac(void)
{
    GameFlag_SetBit(0x910);
}

void Func_020029b8(void)
{
    GameFlag_SetBit(0x914);
}

void Func_020029c8(void)
{
    s32 next;

    next = 16;
    if (*(s16 *)((u8 *)Ram_HeapSlots->event_work + 0x154) > 34)
        next = 15;
    ObjectMotion_ArmCallback(4, 0xc000, 0);
    Battle_WaitMode0(10);
    Object_GetById(4)->action_flags &= 0xfe;
    ObjectMotion_OffsetPositionAndResetMotion(4, 0, 4);
    ObjectMotion_CommitCurrentPositionAndActivate(4);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(4), 0);
    Object_SetModeById(4, 13);
    ObjectMotion_OffsetPositionAndResetMotion(4, 0, 16);
    ObjectMotion_CommitCurrentPositionAndActivate(4);
    Object_SetModeById(4, 10);
    Battle_WaitMode0(10);
    Func_02002c90(123);
    Func_02002c30(next);
    Func_02002b30();
}

void Func_02002a58(void)
{
    s32 id;

    Func_02002b28();
    Func_02002c58(0);
    CommitPos(4, 0, -14);
    id = 4;
    ObjectMotion_CommitPositionAndActivate(id, 16, 0);
    Func_020029c8();
}

void Func_02002a80(void)
{
    s32 id, x, z;

    Func_02002b28();
    Func_02002c58(0);
    CommitPos(4, -2, -6);
    x = 14;
    z = 8;
    id = 4;
    /* FAKEMATCH: the actor id comes between the distances and their negation. */
    __asm__ volatile("" : "+r"(id));
    ObjectMotion_CommitPositionAndActivate(id, -x, -z);
    Func_020029c8();
}

void Func_02002ab0(void)
{
    s32 id, a;

    Func_02002b28();
    Func_02002c58(0);
    a = 192;
    id = 4;
    /* FAKEMATCH: the actor id comes between the angle byte and its shift. */
    __asm__ volatile("" : "+r"(id));
    ObjectMotion_ArmCallback(id, a << 8, 0);
    Func_020029c8();
}
