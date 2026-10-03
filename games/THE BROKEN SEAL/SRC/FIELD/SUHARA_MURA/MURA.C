#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u8 SuharaMura_Scripts[];
extern u8 SuharaMura_Messages[];
extern u8 SuharaMura_Actors[];
extern u8 SuharaMura_ActorsFlag96f[];

extern u8 MsgSuharaTryingGetLalivero[];
extern u8 MsgSuharaBroughtSuhallaSandstorm[];

extern s16 SuharaMura_CellAnimationOrigins[];
extern const u16 SuharaMura_CellSteps0[];
extern const u16 SuharaMura_CellSteps1[];
extern u8 SuharaMura_Extras[];
extern u8 SuharaMura_ExtrasFlag96f[];

/*
 * Suhara village's scene tables: the script and message tables and the
 * actor table the event flag 0x96f selects.
 */
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
    if (GameFlag_IsSet(0x96f) != 0) {
        return (s32)SuharaMura_ActorsFlag96f;
    }
    return (s32)SuharaMura_Actors;
}

/*
 * Two villagers' yes/no talks: each opens its question at the speaker and
 * answers with one of the two lines after it in the catalogue.
 */
void SuharaMura_TalkLalivero(s32 obj)
{
    s32 cue = (s32)MsgSuharaTryingGetLalivero;
    Engine_EventSetMessage(cue);
    Event_OpenMessage(obj, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(cue + 1);
    } else {
        Engine_EventSetMessage(cue + 2);
    }
    Event_ShowMessage(obj, 0);
}

void SuharaMura_TalkSandstorm(s32 obj)
{
    s32 cue = (s32)MsgSuharaBroughtSuhallaSandstorm;
    Engine_EventSetMessage(cue);
    Event_OpenMessage(obj, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(cue + 1);
    } else {
        Engine_EventSetMessage(cue + 2);
    }
    Event_ShowMessage(obj, 0);
}

/*
 * Scene script for overlay resource_3c1: the layout step, the indexed effect
 * setups, the event table choice and the entry state.
 */
void FieldScene_RunLayoutStepThenSet201(void)
{
    s32 width = 4;
    s32 height = 9;

    Map_CopyCellAttributes(25, 9, 1, 1, width, height);
    GameFlag_Set(0x201);
}

void SuharaMura_AnimateCells0(void)
{
    u8 *work = *(u8 **)&gEventWork;
    s32 no = *(s16 *)(work + 364);
    u16 x = SuharaMura_CellAnimationOrigins[no * 2];
    u16 y = SuharaMura_CellAnimationOrigins[no * 2 + 1];

    Audio_PlayCue(158);
    Map_AnimateCells(SuharaMura_CellSteps0, x, y);
    Actor_WalkBy(0, 0, -16);
    *(s32 *)(*(u8 **)&gEventWork + 456) = 16;
    Engine_EventRequestExit(no);
}

void SuharaMura_AnimateCells1(void)
{
    u8 *work = *(u8 **)&gEventWork;
    s32 no = *(s16 *)(work + 364);
    u16 x = SuharaMura_CellAnimationOrigins[no * 2];
    u16 y = SuharaMura_CellAnimationOrigins[no * 2 + 1];

    Audio_PlayCue(158);
    Map_AnimateCells(SuharaMura_CellSteps1, x, y);
    Actor_WalkBy(0, 0, -16);
    *(s32 *)(*(u8 **)&gEventWork + 456) = 16;
    Engine_EventRequestExit(no);
}

s32 SceneData_SelectExtraTableByFlag96f(void)
{
    if (GameFlag_IsSet(0x96F) != 0) {
        return (s32)SuharaMura_ExtrasFlag96f;
    }
    return (s32)SuharaMura_Extras;
}

s32 SceneState_InitEntryWorkspaceAndFlag96f(void)
{
    u8 *work;

    /* Record arrival on map 90, then publish the initial scene phase/timer. */
    if (gGameState.entrance == 90) {
        Engine_GameFlagSet(0x96f);
    }

    work = *(u8 **)&gEventWork;
    *(s32 *)(work + 448) = 256;
    *(s32 *)(work + 456) = 24;

    /* The dressing sequence and cue are unlocked by the shared event flag. */
    if (Engine_GameFlagIsSet(0x201) != 0) {
        FieldScene_RunLayoutStepThenSet201();
        Engine_ActorSetAnimation(16, 4);
    }
    return 0;
}
