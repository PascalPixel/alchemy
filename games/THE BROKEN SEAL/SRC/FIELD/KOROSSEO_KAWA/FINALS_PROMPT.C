#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 MsgKorosseoStageFirstFinalsMatch[];
extern u8 MsgKorosseoStageSecondFinalsMatch[];
extern u8 MsgKorosseoStageThirdFinalsMatch[];
extern u8 MsgKorosseoWouldYouLikeHearDescription[];

void Battle_ResetEffectCounter(void);
void UiWork_PushValueSlot(s32 value, s32 digits);
s32 PartyTalkMenu_Choose(s32 menu);

/* A finals competitor's greeting. The river, the wall and the log-rolling
   stage each announce their own finals match; a competitor whose flag
   (0x200 plus its number) is set has nothing more to say, one already
   met (0x208 plus its number) offers the talk menu, and the first meeting
   offers to describe the match. */
s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 speaker, s32 competitor)
{
    s32 scene;
    s32 message;
    s32 choice;

    Battle_ResetEffectCounter();
    UiWork_PushValueSlot(competitor, 5);
    scene = gGameState.scene;
    if (scene == (s32)&SceneId_KorosseoKawa) {
        message = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (scene == (s32)&SceneId_KorosseoKabe) {
        message = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        message = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Event_SetMessage(message);
    Event_ShowMessage(speaker, 0);
    if (GameFlag_IsSet(competitor + 512) != 0) {
        return 2;
    }
    if (GameFlag_IsSet(competitor + 520) != 0) {
        choice = PartyTalkMenu_Choose(0);
        if (choice == 1) {
            return 2;
        }
        if (choice == 2 || choice == -1) {
            return 3;
        }
        return choice;
    }
    GameFlag_Set(competitor + 520);
    Event_SetMessage((s32)MsgKorosseoWouldYouLikeHearDescription);
    Event_OpenMessage(speaker, 0);
    return Event_ChooseYesNo(0, 0);
}

/* The line after the stage's finals announcement. */
void SceneState_SendIdBySceneId(s32 speaker, s32 competitor)
{
    s32 scene;
    s32 message;

    UiWork_PushValueSlot(competitor, 5);
    scene = gGameState.scene;
    if (scene == (s32)&SceneId_KorosseoKawa) {
        message = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (scene == (s32)&SceneId_KorosseoKabe) {
        message = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        message = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Event_SetMessage(message + 1);
    Event_ShowMessage(speaker, 0);
}
