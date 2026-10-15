#include "types.h"
#include "party_state.h"
#include "object_runtime.h"
#include "call.h"
#include "text/msg_ids.h"

extern s32 Data_020024b4;
extern s32 Data_020024b0;

struct ObjectRuntime *Object_GetById(s32 id);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void Object_LinkPair(s32 id, s32 partner, s32 flags);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000188(void);
void Func_020001cc(void);
void Func_02000258(void);
void Func_02000288(void);
void Func_020002cc(void);
void Func_020006e4(void);
void Func_02001f1c(void);
void Func_02001f2c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001f5c(void);
void Func_02001fc4(s32 id, s32 x, s32 z);
void Func_02001fcc(s32 id, s32 x, s32 z, s32 angle);
void Func_0200201c(const void *message);
void Func_0200202c(s32 id, s32 a, s32 b);
void Func_02002044(s32 id, s32 angle);
s32 Func_0200204c(s32 id, s32 emote, s32 frames);
struct ObjectRuntime *Func_02002074(void);
void Func_0200207c(s32 countdown);
void Func_020020ac(s32 skip);
void Func_020020b4(s32 id, s32 dx, s32 dz, s32 angle);

static __inline__ void AddCallback(void (*callback)(void), s32 priority)
{
    /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the arguments up in their own order and keeps each call's constants; a direct call builds the costly constant first or shares it (differ 2 or more). */
    Scheduler_AddOrUpdateCallback(callback, priority);
}

// Sends actor id to actor 4's spot at a run.
static inline void RunToActor4(s32 id)
{
    struct ObjectRuntime *target;

    Call3((void (*)())ObjectMotion_SetSpeedParameters, id, 0x13333, 0x9999);
    Call2((void (*)())Object_SetModeById, id, 2);
    target = Object_GetById(4);
    if (target != NULL)
        ObjectMotion_ResetAndSetPosition(id, ((s16 *)&target->x)[1], ((s16 *)&target->z)[1]);
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, id);
    Call3((void (*)())Func_02001fc4, id, 0, 0);
}

// The scene run from entrance 20: the camera frames actors 4 to 9 around the
// spot, two callbacks start the bobbing, and after the exchange of looks and
// emotes the four actors run to actor 4.
void Func_020006e4(void)
{
    Func_02001f5c();
    Call1((void (*)())Func_020020ac, 0);
    Call4((void (*)())Motion_CamBounds, -1, -1, -1, 0);
    Func_02002074()->motion_flags = 0;
    Call4((void (*)())Motion_CamBounds, 0x1ca0000, 0x300000, 0x1980000, 0);
    Call1((void (*)())Func_0200201c, (s32)MsgHoldOnTo);
    Call4((void (*)())Func_02001fcc, 4, 0x1d60000, 0x1880000, 0);
    Call4((void (*)())Func_020020b4, 6, 0, -24, 0x4000);
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN)
    Call4((void (*)())Func_020020b4, 5, -16, -16, 0x2000);
#else
    {
        /* FAKEMATCH: the European editions build -16 once in r1 and copy it to r2; as two constants it is built twice, and as one variable (copied, chained or in an array) it takes r2 and is copied to r1 (differ 6). */
        register s32 offset asm("r1") = -16;

        Call4((void (*)())Func_020020b4, 5, offset, offset, 0x2000);
    }
#endif
    Call4((void (*)())Func_020020b4, 9, -24, 8, 0xf000);
    Call4((void (*)())Func_020020b4, 7, -4, 24, 0xe000);
    Call6((void (*)())Func_02001f2c, 60, 53, 67, 16, 7, 3);
    Func_02001f1c();
    Call1((void (*)())WaitFrames, 1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Data_020024b4 = 0;
    Value2(Scheduler_AddOrUpdateCallback, (s32)Func_02000258, 0x480);
    Data_020024b0 = 0;
    Value2(Scheduler_AddOrUpdateCallback, (s32)Func_02000288, 0x480);
    Value3(Func_0200204c, 6, 0x102, 30);
    Call2((void (*)())Func_02002044, 6, 0x4000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())Func_0200202c, 6, 0, 5);
    Call2((void (*)())Func_02002044, 5, 0xe000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 4);
    Call3((void (*)())Func_0200202c, 5, 0, 5);
    Value3(Func_0200204c, 4, 0x102, 30);
    Call2((void (*)())Func_02002044, 4, 0x4000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call2((void (*)())Func_02002044, 7, 0xc000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())Func_0200202c, 7, 0, 5);
    Call3((void (*)())ObjectMotion_ArmCallback, 6, 0x4000, 0);
    Call2((void (*)())Func_02002044, 5, 0x2000);
    Call1((void (*)())Battle_WaitMode0, 30);
    Call2((void (*)())Func_02002044, 5, 0xe000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())Func_0200202c, 5, 0, 5);
    Call2((void (*)())Func_02002044, 6, 0x6000);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 6, 3);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call2((void (*)())Func_02002044, 6, 0x4000);
    Call3((void (*)())Func_0200202c, 6, 0, 5);
    Value3(Func_0200204c, 7, 0x109, 30);
    Call2((void (*)())Func_02002044, 7, 0xc000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())Func_0200202c, 7, 0, 5);
    Call2((void (*)())Func_02002044, 9, 0x2000);
    Value3(Func_0200204c, 9, 0x101, 30);
    Call3((void (*)())Func_0200202c, 9, 0, 5);
    Call2((void (*)())Func_02002044, 7, 0xa000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Value3(Func_0200204c, 7, 0x102, 30);
    Call3((void (*)())Func_0200202c, 7, 0, 5);
    Call3((void (*)())Object_LinkPair, 6, 5, 0);
    Call1((void (*)())Battle_WaitMode0, 30);
    Call3((void (*)())ObjectMotion_ArmCallback, 6, 0x4000, 0);
    Call2((void (*)())Func_02002044, 5, 0x2000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Value3(Func_0200204c, 6, 0x101, 30);
    Call3((void (*)())Func_0200202c, 6, 0, 5);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 5, 2);
    Call1((void (*)())Battle_WaitMode0, 5);
    Call3((void (*)())Func_0200202c, 5, 0, 5);
    Call2((void (*)())Func_02002044, 7, 0xa000);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())Func_0200202c, 7, 0, 5);
    Call3((void (*)())Func_0200202c, 9, 0, 5);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())Func_0200202c, 7, 0, 5);
    Value3(Func_0200204c, 9, 0x102, 30);
    Call3((void (*)())Func_0200202c, 9, 0, 5);
    Call2((void (*)())Func_02002044, 9, 0xc000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())Func_0200202c, 9, 0, 5);
    Call3((void (*)())ObjectMotion_ArmCallback, 4, 0x6000, 0);
    Call2((void (*)())Func_02002044, 9, 0xe000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())Func_0200202c, 9, 0, 5);
    Call2((void (*)())Func_02002044, 9, 0xc000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 9, 4);
    Call3((void (*)())Func_0200202c, 9, 0, 5);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())Object_LinkPair, 9, 7, 0);
    Call3((void (*)())Object_LinkPair, 6, 5, 0);
    Call2((void (*)())Object_SetModeById, 6, 3);
    Call2((void (*)())Object_SetModeById, 5, 3);
    Call2((void (*)())Object_SetModeById, 9, 3);
    Call2((void (*)())Object_SetModeById, 7, 3);
    Call1((void (*)())Battle_WaitMode0, 40);
    RunToActor4(7);
    RunToActor4(5);
    RunToActor4(6);
    RunToActor4(9);
    Call1((void (*)())Func_0200207c, 1);
}

void Func_02000b84(void)
{
    Object_GetById(gPartyState.current_owner)->priority_flags |= 0x20;
    AddCallback(Func_02000188, 0x480);
    AddCallback(Func_020001cc, 0x480);
    if (gPartyState.entrance == 10)
        Func_020002cc();
    do { } while (0); /* FAKEMATCH: the one-pass block keeps the second entrance test a load of its own (ldrsh, cmp #20); without it the two tests share a halfword load and compare shifted (differ 33). */
    if (gPartyState.entrance == 20)
        Func_020006e4();
}
