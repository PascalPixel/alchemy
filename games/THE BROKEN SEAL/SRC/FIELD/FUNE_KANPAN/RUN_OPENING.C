#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"
extern u8 MsgFuneHaveMakeThemPromiseHelp[];
extern u8 MsgFuneIfWeDontLeaveSoon[];
extern u8 MsgFuneNowWeHaveProtectShip[];
extern u8 MsgFuneSomebodyStopThem[];
extern u8 MsgFuneTheyCantPlanningMutiny[];
extern u8 MsgFuneToldWereLeavingSoonSet[];
extern u8 MsgFuneHeadedColosso[];
extern u8 FuneKanpan_RandomActorActions[];

union Slot {
    s32 w;
    s16 h[2];
};

s32 BuildMotionCountdown(s32, s16);
s32 Object_GetById();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 rec7;
    s32 record;
    s32 shown;

    ((void (*)())Engine_EventBegin)();
    if (GameFlag_IsSet(0x925) != 0) {
        Event_SetMessage((s32)MsgFuneNowWeHaveProtectShip);
        Event_ShowMessage(21, 0);
    } else {
        if (GameFlag_IsSet(0x922) != 0) {
            Actor_RunRepeatedMotion(21, 2);
            Event_SetMessage((s32)MsgFuneSomebodyStopThem);
            Event_ShowMessage(21, 0);
            rec7 = Value1(Object_GetById, 21);
            record = Random_Next();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(u16 *)(rec7 + 100) = shown;
            Engine_ActorEnableActionCallback(21, (u32)FuneKanpan_RandomActorActions);
        } else {
            Actor_ShowEmote(21, 0x103, 0);
            Actor_StartRepeatedMotion(21, 3);
            Event_SetMessage((s32)MsgFuneToldWereLeavingSoonSet);
            Event_ShowMessage(21, 0);
        }
    }
    Event_End();
}

void FieldScene_RunScene3afSequenceA(void)
{
    s32 rec7;
    s32 record;
    s32 shown;

    ((void (*)())Engine_EventBegin)();
    if (GameFlag_IsSet(0x925) != 0) {
        Event_SetMessage((s32)MsgFuneHaveMakeThemPromiseHelp);
        Event_ShowMessage(24, 0);
    } else {
        if (GameFlag_IsSet(0x922) != 0) {
            Actor_RunRepeatedMotion(24, 2);
            Event_SetMessage((s32)MsgFuneTheyCantPlanningMutiny);
            Event_ShowMessage(24, 0);
            rec7 = Value1(Object_GetById, 24);
            record = Random_Next();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(u16 *)(rec7 + 100) = shown;
            Engine_ActorEnableActionCallback(24, (u32)FuneKanpan_RandomActorActions);
        } else {
            Actor_ShowEmote(24, 0x103, 0);
            Actor_StartRepeatedMotion(24, 3);
            Event_SetMessage((s32)MsgFuneIfWeDontLeaveSoon);
            Event_ShowMessage(24, 0);
        }
    }
    Event_End();
}

void SceneDialogue_RunActor21Line(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgFuneHeadedColosso);
    Event_AskYesNo(21, 0);
    Event_End();
}
