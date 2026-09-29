#include "TASK.H"
extern u8 MsgKorosseoStageFirstFinalsMatch[];
extern u8 MsgKorosseoStageSecondFinalsMatch[];
extern u8 MsgKorosseoStageThirdFinalsMatch[];
extern u8 MsgKorosseoWouldYouLikeHearDescription[];

void Battle_ResetEffectCounter(void);
s32 PartyTalkMenu_Choose(s32 mode);

/* A finals competitor names the match by the stage the party stands on, the
 * river, the wall or the log-rolling stage; the first time, the competitor
 * offers to describe it. Returns the choice, or 2 and 3 for a party that has
 * heard it before. */
s32 KorosseoKabe_RunStateInteraction(s32 a, s32 b)
{
    s32 v;
    s32 id;
    s32 r;

    Battle_ResetEffectCounter();
    UiWork_PushValueSlot(b, 5);
    v = gGameState.scene;
    if (v == (s32)&SceneId_KorosseoKawa) {
        id = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (v == (s32)&SceneId_KorosseoKabe) {
        id = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        id = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Event_SetMessage(id);
    Event_ShowMessage(a, 0);
    if (GameFlag_IsSet(b + 512) != 0) {
        return 2;
    }
    if (GameFlag_IsSet(b + 520) != 0) {
        r = PartyTalkMenu_Choose(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    GameFlag_Set(b + 520);
    Event_SetMessage((s32)MsgKorosseoWouldYouLikeHearDescription);
    Event_OpenMessage(a, 0);
    return Event_ChooseYesNo(0, 0);
}

/* The competitor's follow-up line for the stage's match. */
void KorosseoKabe_ShowFollowUpPrompt(s32 a, s32 b)
{
    s32 v;
    s32 id;

    UiWork_PushValueSlot(b, 5);
    v = gGameState.scene;
    if (v == (s32)&SceneId_KorosseoKawa) {
        id = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (v == (s32)&SceneId_KorosseoKabe) {
        id = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        id = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Event_SetMessage(id + 1);
    Event_ShowMessage(a, 0);
}
