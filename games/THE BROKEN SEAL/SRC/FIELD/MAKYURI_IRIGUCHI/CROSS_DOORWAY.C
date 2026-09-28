#include "TYPES.H"

void Engine_EventBegin();
s32 Object_GetById();
void Engine_ActorSetPosition();
void ObjectMotion_SetSpeedParameters();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_ActorRunRepeatedMotion();
void Battle_WaitMode0();
void Engine_ActorWalkTo();
void Engine_ActorEnableActionCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Engine_ActorFaceDirection();
void Engine_ActorShowEmote();
void Engine_CameraMoveToActor();
void Engine_ActorWalkToAndWait();
void Engine_CameraWaitForMove();
void Map_CopyCellAttributeRect();
void Engine_EventEnd();
s32 SceneActor_FaceLeaderWhileGrounded(u8 *object);

/* Actor 8's action tables in the overlay's data. */
extern const u8 MakyuriIriguchi_Actor8Path1[];
extern const u8 MakyuriIriguchi_Actor8Path2[];
extern const u8 MakyuriIriguchi_Actor8Path3[];



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Makyuri entrance event: Ivan crosses the doorway twice while the guard's action callbacks alternate, then shows an emote and the cells are copied. */
/* Makyuri entrance event: an actor crosses the doorway twice while actor 8's action callbacks alternate, then shows an emote and the cells are copied. */
void MakyuriIriguchi_CrossDoorway(void)
{
    s32 i;
    s32 zero;
    s32 mask;
    u8 *p;
    s32 record;

    Engine_EventBegin();
    record = Object_GetById(12);
    *(s32 *)(record + 24) = -0x10000;
    record = Value1(Object_GetById, 13);
    *(s32 *)(record + 24) = -0x10000;
    record = Object_GetById(14);
    *(s32 *)(record + 24) = -0x10000;
    Call3(Engine_ActorSetPosition, 3, 0x880000, 0xb80000);
    Call3(Engine_ActorSetPosition, 0, 0x880000, 0x1280000);
    Call3(Engine_ActorSetPosition, 8, 0x880000, 0x980000);
    Call3(ObjectMotion_SetSpeedParameters, 3, 0x18000, 0xc000);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0x18000, 0xc000);
    Call3(ObjectMotion_SetSpeedParameters, 0, 0xcccc, 0x6666);
    Call2(Engine_CameraSetSpeed, 0xcccc, 0x1999);
    Call4(Engine_CameraMoveTo, 0x880000, -1, 0xb80000, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_ActorRunRepeatedMotion(3, 1);
    *(u8 *)(Object_GetById(8) + 90) &= 254;
    Battle_WaitMode0(20);
    mask = 254;
    zero = 0;
    for (i = 0; i < 2; i++) {
        Engine_ActorWalkTo(3, 152, 168);
        Battle_WaitMode0(10);
        Call2(Engine_ActorEnableActionCallback, 8, (s32)MakyuriIriguchi_Actor8Path3);
        ObjectMotion_CommitCurrentPositionAndActivate(3);
        Call3(Engine_ActorFaceDirection, 3, 0xc000, 30);
        Engine_ActorRunRepeatedMotion(3, 1);
        p = (u8 *)Object_GetById(3);
        /* FAKEMATCH: or-ing a zero kept from before the loop leaves the
         * reference's unread zero in sl. */
        p[90] = (p[90] & mask) | zero;
        Engine_ActorWalkTo(3, 136, 184);
        Battle_WaitMode0(10);
        Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path1);
        ObjectMotion_CommitCurrentPositionAndActivate(3);
        *(u8 *)(Object_GetById(3) + 90) |= 1;
        Battle_WaitMode0(30);
        Engine_ActorWalkTo(3, 120, 168);
        Battle_WaitMode0(5);
        Call2(Engine_ActorEnableActionCallback, 8, (s32)MakyuriIriguchi_Actor8Path2);
        ObjectMotion_CommitCurrentPositionAndActivate(3);
        Call3(Engine_ActorFaceDirection, 3, 0xc000, 30);
        Engine_ActorRunRepeatedMotion(3, 1);
        Battle_WaitMode0(15);
        *(u8 *)(Object_GetById(3) + 90) &= mask;
        Engine_ActorWalkTo(3, 136, 184);
        Battle_WaitMode0(15);
        Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path1);
        ObjectMotion_CommitCurrentPositionAndActivate(3);
        Engine_ActorRunRepeatedMotion(3, 1);
        *(u8 *)(Object_GetById(3) + 90) |= 1;
    }
    Battle_WaitMode0(20);
    Call3(Engine_ActorShowEmote, 3, 0x102, 60);
    record = Object_GetById(3);
    *(s32 *)(record + 108) = (s32)SceneActor_FaceLeaderWhileGrounded;
    Engine_CameraMoveToActor(0, 1);
    Battle_WaitMode0(30);
    Call3(Engine_ActorWalkToAndWait, 0, 136, 0x108);
    Engine_CameraWaitForMove();
    Call6(Map_CopyCellAttributeRect, 0, 0, 3, 3, 7, 9);
    Engine_EventEnd();
}
