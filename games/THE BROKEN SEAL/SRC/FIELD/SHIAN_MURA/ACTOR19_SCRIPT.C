#include "TYPES.H"
#include "FIELD_EVENT.H"

void Effect_Spawn();
void Main_0808a5e8();

extern u8 Value_02008359;

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void Scene_RunActorNineteenScript(void)
{
    s32 cb;

    Engine_EventBegin();
    Value2((s32 (*)())Engine_ActorEnableActionCallback, 19, 0x20096a0);
    cb = (s32)&Value_02008359;
    Value2((s32 (*)())Engine_TaskAddCallback, cb, 0xc80);
    Engine_ActorStartAction(19);
    Engine_AudioPlayCue(124);
    Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0, 0, 0, 0x20001, 0);
    Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0x3333, 0, 0, 0x20001, 0);
    Effect_Spawn(0xa80000, 0x80000, 0x1380000, -0x3333, 0, 0, 0x20001, 0);
    Engine_TaskRemoveCallback(cb);
    Engine_ActorGet(19)->sprite->rotation = 0x8000;
    Engine_ActorSetPosition(21, 0xa80000, 0x1380000);
    Engine_EventWait(20);
    Engine_ActorFaceActor(14, 19, 0);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(10);
    Engine_EventSetMessage(0x17fd);
    if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x203)) {
        gEventWork->message++;
    }
    Engine_EventShowMessage(14, 0);
    Call1((void (*)())Engine_GameFlagSet, 0x203);
    Main_0808a5e8();
    Engine_EventEnd();
}
