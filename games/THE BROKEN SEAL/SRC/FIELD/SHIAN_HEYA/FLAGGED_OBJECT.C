#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgShianDoPlanCrossDesertWarrior[];
extern u8 MsgShianHsusInjuryTaughtGoodLesson[];
extern u8 MsgShianKungFuMakesQuickDoes[];
extern u8 MsgShianMartialArtistsCantUseHeavy[];
extern u8 MsgShianSomethingWrongOnSilkRoad[];
extern u8 MsgShianWarriorsShouldLearnAboutTowns[];
extern u8 MsgShianWeHaveFewCustomersThese[];
extern u8 MsgShianXianHasMartialArtsBut[];
extern u8 MsgShianXiansSpecialArmorNotSuited[];



#define NULL ((void *)0)
void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(void *object);

union Slot {
    s32 w;
    u16 h[2];
    void *p;
};

/* The scene's tables, in the overlay's read-only data. */
extern u8 ShianHeya_SceneTable0[];
extern u8 ShianHeya_SceneTable1[];
extern u8 ShianHeya_SceneTable2Entrance8[];
extern u8 ShianHeya_SceneTable2[];

void SceneTable_Prepare(void *);

s32 SceneData_GetTable8778(void)
{
    return (s32)ShianHeya_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTable8868(void)
{
    return (s32)ShianHeya_SceneTable1;
}

s32 SceneData_SelectTable89c8Or8890(void)
{
    if (gGameState.entrance == 8) {
        return (s32)ShianHeya_SceneTable2Entrance8;
    }
    SceneTable_Prepare(ShianHeya_SceneTable2);
    return (s32)ShianHeya_SceneTable2;
}

void SceneDialogue_RunActor14FlaggedDialogue(void)
{
    u16 *state = Actor_Get(ACTOR_PARTY_LEADER);
    u32 value = state[3];

    Event_Begin();
    if (value >= 0xa001 && value <= 0xdfff) {
        Shop_Open(16, 14);
    } else {
        if (GameFlag_IsSet(0x895) == 0) {
            Event_SetMessage((s32)MsgShianXianHasMartialArtsBut);
        } else {
            Event_SetMessage((s32)MsgShianHsusInjuryTaughtGoodLesson);
        }
        Event_ShowMessage(14, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor15FlaggedDialogue(void)
{
    u16 *state = Actor_Get(ACTOR_PARTY_LEADER);
    u32 value = state[3];

    Event_Begin();
    if (value >= 0xa001 && value <= 0xdfff) {
        Shop_Open(17, 15);
    } else {
        if (GameFlag_IsSet(0x895) == 0) {
            Event_SetMessage((s32)MsgShianMartialArtistsCantUseHeavy);
        } else {
            Event_SetMessage((s32)MsgShianKungFuMakesQuickDoes);
        }
        Event_ShowMessage(15, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor16FlaggedDialogue(void)
{
    void Event_ShowMessage();
    void Event_SetMessage(s32);

    u16 *state = Actor_Get(ACTOR_PARTY_LEADER);
    u32 value = state[3];

    Event_Begin();
    if (value >= 0xa001 && value <= 0xdfff) {
        Shop_Open(18, 16);
    } else if (GameFlag_IsSet(0x895) == 0) {
        Event_SetMessage((s32)MsgShianXiansSpecialArmorNotSuited);
        Event_ShowMessage(16, 0);
    } else {
        Event_SetMessage((s32)MsgShianDoPlanCrossDesertWarrior);
        Event_AskYesNo(16, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor17FlaggedDialogue(void)
{
    s32 GameFlag_IsSet(s32);

    u16 *state = Actor_Get(ACTOR_PARTY_LEADER);
    u32 value = state[3];

    Event_Begin();
    if (value < 0x2000 || value > 0xe000) {
        Inn_Open(5, 17);
    } else {
        if (GameFlag_IsSet(0x895) == 0) {
            Event_SetMessage((s32)MsgShianWarriorsShouldLearnAboutTowns);
        } else {
            Event_SetMessage((s32)MsgShianWeHaveFewCustomersThese);
        }
        Event_ShowMessage(17, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor10Dialogue(void)
{
    s32 Event_AskYesNo(s32, s32);

    Event_Begin();
    Event_SetMessage((s32)MsgShianSomethingWrongOnSilkRoad);
    Event_AskYesNo(10, 0);
    Event_End();
}
