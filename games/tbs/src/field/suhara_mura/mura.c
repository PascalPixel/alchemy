#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "text/msg_ids.h"




u8 *SceneData_GetScriptTable(void)
{
    return SuharaMura_Scripts;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return SuharaMura_Messages;
}

s32 SceneData_SelectActorTableByFlag96f(void)
{
    if (Engine_GameFlagIsSet(0x96f) != 0) {
        return (s32)SuharaMura_ActorsFlag96f;
    }
    return (s32)SuharaMura_Actors;
}

void SuharaMura_TalkLalivero(s32 obj)
{
    s32 cue = (s32)MsgSuharaTryingGetLalivero;
    Engine_EventSetMessage(cue);
    Engine_EventOpenMessage(obj, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(cue + 1);
    } else {
        Engine_EventSetMessage(cue + 2);
    }
    Engine_EventShowMessage(obj, 0);
}

void SuharaMura_TalkSandstorm(s32 obj)
{
    s32 cue = (s32)MsgSuharaBroughtSuhallaSandstorm;
    Engine_EventSetMessage(cue);
    Engine_EventOpenMessage(obj, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(cue + 1);
    } else {
        Engine_EventSetMessage(cue + 2);
    }
    Engine_EventShowMessage(obj, 0);
}

void FieldScene_RunLayoutStepThenSet201(void)
{
    s32 width = 4;
    s32 height = 9;

    Engine_MapCopyCellAttributes(25, 9, 1, 1, width, height);
    Engine_GameFlagSet(0x201);
}

void SuharaMura_AnimateCells0(void)
{
    struct EventWork *work = gEventWork;
    s32 no = work->touched_trigger;
    u16 x = SuharaMura_CellAnimationOrigins[no * 2];
    u16 y = SuharaMura_CellAnimationOrigins[no * 2 + 1];

    Audio_PlayCue(158);
    Map_AnimateCells(SuharaMura_CellSteps0, x, y);
    Actor_WalkBy(0, 0, -16);
    gEventWork->transition_frames = 16;
    Engine_EventRequestExit(no);
}

void SuharaMura_AnimateCells1(void)
{
    struct EventWork *work = gEventWork;
    s32 no = work->touched_trigger;
    u16 x = SuharaMura_CellAnimationOrigins[no * 2];
    u16 y = SuharaMura_CellAnimationOrigins[no * 2 + 1];

    Audio_PlayCue(158);
    Map_AnimateCells(SuharaMura_CellSteps1, x, y);
    Actor_WalkBy(0, 0, -16);
    gEventWork->transition_frames = 16;
    Engine_EventRequestExit(no);
}

s32 SceneData_SelectExtraTableByFlag96f(void)
{
    if (Engine_GameFlagIsSet(0x96F) != 0) {
        return (s32)SuharaMura_ExtrasFlag96f;
    }
    return (s32)SuharaMura_Extras;
}

s32 SceneState_InitEntryWorkspaceAndFlag96f(void)
{
    struct EventWork *work;

    if (gGameState.entrance == 90) {
        Engine_GameFlagSet(0x96f);
    }

    work = gEventWork;
    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transition_frames = 24;

    if (Engine_GameFlagIsSet(0x201) != 0) {
        FieldScene_RunLayoutStepThenSet201();
        Engine_ActorSetAnimation(16, 4);
    }
    return 0;
}
