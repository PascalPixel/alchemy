#include "TYPES.H"

void Engine_EventBegin();
s32 Engine_GameFlagIsSet();
void Engine_ActorFaceActor();
s32 Engine_EventWait();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_ActorFaceDirection();
void Engine_EventEnd();
s32 Engine_ActorRunRepeatedMotion();
s32 Engine_ActorGet();
void Main_08000128();
void Engine_ActorSetAnimation();
void Engine_ActorSetDestinationOffset();
void Engine_ActorWaitForMove();
void Effect_Spawn();
void Engine_AudioPlayCue();
s32 Engine_TaskAddCallback();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination();
s32 Engine_ActorEnableActionCallback();
void Engine_TaskRemoveCallback();
void Engine_ActorStartAction();
void Engine_EventShowMessageAndWait();
void Engine_GameFlagSet();
void Main_0808a5e8();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

struct SpawnParams {
    s32 count;
    u8 unknown_04[20];
    u16 sprite;
    u16 pad_1a;
    s32 callback;
    u16 facing;
    u8 unknown_22[6];
};

extern u8 Value_02008371;

void ShianMura_RunNpcMeetScene(void)
{
    s32 flag;
    s32 record;
    s32 cb;
    s32 back;
    s32 pos[3];
    struct SpawnParams params;

    Engine_EventBegin();
    flag = Value1(Engine_GameFlagIsSet, 0x202);
    if (flag != 0) {
        Engine_ActorFaceActor(14, 0, 0);
        Engine_EventWait(10);
        Call1(Engine_EventSetMessage, 0x17f4);
        Engine_EventShowMessage(14, 0);
        Engine_ActorFaceDirection(14, 0, 10);
        Engine_EventEnd();
    } else {
        Call1(Engine_EventSetMessage, 0x17f2);
        Engine_EventShowMessage(14, 0);
        Engine_ActorRunRepeatedMotion(0, 2);
        *(u8 *)(Engine_ActorGet(0) + 90) &= 254;
        pos[0] = flag;
        pos[1] = flag;
        pos[2] = flag;
        record = Engine_ActorGet(0);
        Call3(Main_08000128, -0x80000, *(u16 *)(record + 6), (s32)pos);
        Engine_ActorSetAnimation(0, 2);
        Engine_ActorSetDestinationOffset(0, pos[0] / 0x10000, pos[2] / 0x10000);
        Engine_ActorWaitForMove(0);
        *(u8 *)(Engine_ActorGet(0) + 90) |= 1;
        Engine_EventWait(30);
        Call2((void (*)())Engine_ActorRunRepeatedMotion, 14, 2);
        params.count = 1;
        Effect_Spawn(0xc00000, 0, 0x1380000, 0x1999, 0x3333, 0, 0x20001, 0);
        Effect_Spawn(0xc00000, 0, 0x1380000, 0x3333, 0x1999, 0, 0x20001, 0);
        Engine_AudioPlayCue(132);
        cb = (s32)&Value_02008371;
        Value2(Engine_TaskAddCallback, cb, 0xc80);
        *(s32 *)(Engine_ActorGet(14) + 40) = 0x60000;
        *(s32 *)(Engine_ActorGet(14) + 72) = 0x10000;
        *(s32 *)(Engine_ActorGet(14) + 68) = 0;
        Call3(Engine_ActorSetSpeed, 14, 0x30000, 0x18000);
        Call3(Engine_ActorSetDestination, 14, 168, 0x138);
        Engine_ActorWaitForMove(14);
        Engine_AudioPlayCue(134);
        Value2(Engine_ActorEnableActionCallback, 19, 0x20096a0);
        Value2(Engine_TaskAddCallback, 0x2008359, 0xc80);
        {
            s32 sprite = 0x11b;

            params.sprite = sprite;
        }
        params.callback = 0x2009714;
        params.facing = 0x4000;
        Effect_Spawn(0xa80000, 0, 0x14c0000, 0, 0, 0, 0x720000, &params);
        Call3(Engine_ActorSetDestination, 14, 146, 0x138);
        Engine_ActorWaitForMove(14);
        Engine_TaskRemoveCallback(cb);
        Effect_Spawn(0x900000, 0, 0x1380000, 0, 0, 0, 0x20001, 0);
        Effect_Spawn(0x900000, 0, 0x1380000, -0x3333, 0x1999, 0, 0x20001, 0);
        back = -0x8000;
        Effect_Spawn(0x900000, 0, 0x1380000, back, 0, 0, 0x20001, 0);
        Engine_ActorStartAction(19);
        Engine_AudioPlayCue(124);
        Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0, 0, 0, 0x20001, 0);
        Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0x3333, 0, 0, 0x20001, 0);
        Effect_Spawn(0xa80000, 0x80000, 0x1380000, -0x3333, 0, 0, 0x20001, 0);
        Call1(Engine_TaskRemoveCallback, 0x2008359);
        *(u16 *)(*(s32 *)(Engine_ActorGet(19) + 80) + 30) = back;
        *(s32 *)(Engine_ActorGet(14) + 68) = 0x4000;
        *(s32 *)(Engine_ActorGet(14) + 72) = 0x10000;
        Call1((void (*)())Engine_EventWait, 30);
        Engine_ActorFaceDirection(14, 0, 20);
        Engine_ActorRunRepeatedMotion(14, 2);
        Engine_EventWait(20);
        Engine_EventShowMessageAndWait(14, 0, 20);
        Call1(Engine_GameFlagSet, 0x202);
        Main_0808a5e8();
        Engine_EventEnd();
    }
}
