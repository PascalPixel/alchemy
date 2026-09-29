/* The finals' stage announcer: the line naming this match depends on which
 * of the three Colosso stages the party is on, and the first time a stage's
 * flag is met the announcer offers to describe it. */
#include "LOG_ROLLING.H"
extern u8 MsgKorosseoStageFirstFinalsMatch[];
extern u8 MsgKorosseoStageSecondFinalsMatch[];
extern u8 MsgKorosseoStageThirdFinalsMatch[];
extern u8 MsgKorosseoWouldYouLikeHearDescription[];

void Battle_ResetEffectCounter(void);
void UiText_DrawQuantity(s32 value, s32 digits);
s32 PartyTalkMenu_Choose(s32 menu);

s32 ColossoLogRollingStage_RunStateInteraction(s32 actor, s32 flags)
{
    s32 scene;
    s32 msg;
    s32 result;

    Battle_ResetEffectCounter();
    UiText_DrawQuantity(flags, 5);
    scene = gGameState.scene;
    if (scene == (s32)&SceneId_KorosseoKawa) {
        msg = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (scene == (s32)&SceneId_KorosseoKabe) {
        msg = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        msg = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Event_SetMessage(msg);
    Event_ShowMessage(actor, 0);
    if (GameFlag_IsSet(flags + 512) != 0) {
        return 2;
    }
    if (GameFlag_IsSet(flags + 520) != 0) {
        result = PartyTalkMenu_Choose(0);
        if (result == 1) {
            return 2;
        }
        if (result == 2 || result == -1) {
            return 3;
        }
        return result;
    }
    GameFlag_Set(flags + 520);
    Event_SetMessage((s32)MsgKorosseoWouldYouLikeHearDescription);
    Event_OpenMessage(actor, 0);
    return Event_ChooseYesNo(0, 0);
}

void ColossoLogRollingStage_InitializeStateInteraction(s32 actor, s32 flags)
{
    s32 scene;
    s32 msg;

    UiText_DrawQuantity(flags, 5);
    scene = gGameState.scene;
    if (scene == (s32)&SceneId_KorosseoKawa) {
        msg = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (scene == (s32)&SceneId_KorosseoKabe) {
        msg = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        msg = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Event_SetMessage(msg + 1);
    Event_ShowMessage(actor, 0);
}
