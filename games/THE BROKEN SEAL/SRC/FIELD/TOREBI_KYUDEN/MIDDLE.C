#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KYUDEN.H"
extern u8 MsgTorebiCantFightBecauseLittleIndigestion[];
extern u8 MsgTorebiEvenIfEscapedBabiPalace[];

void RunMiddleAuxiliarySequence(s32 a)
{
    u8 *obj;
    u8 *q;

    obj = (u8 *)Value0((s32 (*)())Engine_ActorGet);
    Event_Begin();
    q = TorebiKyuden_MiddleActionScript;
    Actor_EnableActionCallback(a, q);
    Event_SetMessage((s32)MsgTorebiCantFightBecauseLittleIndigestion);
    Event_ShowMessage(a, 0);
    Actor_Stop(a);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(60);
    Event_ShowMessage(a, 0);
    Event_Wait(20);
    Actor_ShowEmote(a, 258, 60);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_EnableActionCallback(a, q);
    Event_ShowMessage(a, 0);
    Call3_scene_effect_sequence_head(Engine_ActorFaceDirection, a, 0xe000, 0);
    Event_Wait(10);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Actor_EnableActionCallback(a, q);
    Event_End();
}

void FieldScene_RunScene3b8_0200049c(s32 unused0, s32 a1)
{
    Event_Begin();
    Event_SetMessage((s32)MsgTorebiEvenIfEscapedBabiPalace);
    Event_ShowMessage(a1, 0);
    if (GameFlag_IsSet(0x968) == 0) {
        GameFlag_Set(0x968);
        Psynergy_Cancel();
        Event_Wait(50);
        Actor_ShowEmote(a1, 0x100, 70);
        Actor_FaceActor(a1, ACTOR_PARTY_LEADER, 40);
        Event_ShowMessage(a1, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(a1, 4);
        Event_Wait(20);
        Event_ShowMessage(a1, 0);
        Actor_FaceDirection(a1, 0x8000, 0);
    }
    Event_End();
}
