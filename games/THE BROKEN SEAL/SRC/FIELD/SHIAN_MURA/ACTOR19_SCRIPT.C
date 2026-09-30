#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"
extern u8 MsgShianWhaWhaHappened[];

void Effect_Spawn();
void BattleFx_PlayQueuedSound();

void SceneEffect_AdvanceRotatingSprite();
extern const s32 ShianMura_Actor19Motion[];

void Scene_RunActorNineteenScript(void)
{
    s32 cb;

    Engine_EventBegin();
    ((s32 (*)())Engine_ActorEnableActionCallback)(19, (s32)ShianMura_Actor19Motion);
    cb = (s32)SceneEffect_AdvanceRotatingSprite;
    ((s32 (*)())Engine_TaskAddCallback)(cb, 0xc80);
    Object_RefreshSelectorById(19);
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
    Engine_EventSetMessage((s32)MsgShianWhaWhaHappened);
    if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x203)) {
        gEventWork->message++;
    }
    Engine_EventShowMessage(14, 0);
    ((void (*)())Engine_GameFlagSet)(0x203);
    BattleFx_PlayQueuedSound();
    Engine_EventEnd();
}
