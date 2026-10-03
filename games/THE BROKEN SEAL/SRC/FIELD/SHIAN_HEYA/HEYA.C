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

extern u8 MsgShianDidMonstersInAltinSpit[];

extern u8 MsgShianWarriorWelcome[];

/* The scene's tables, in the overlay's read-only data. */
extern u8 ShianHeya_SceneTable3[];
extern u8 ShianHeya_SceneTable3Entrance8[];
void *OverlayObject_CreateConfigured(s32 x, s32 y, s32 z, s32 type);

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
    u16 *state = Object_GetById(ACTOR_PARTY_LEADER);
    u32 value = state[3];

    Engine_EventBegin();
    if (value >= 0xa001 && value <= 0xdfff) {
        Engine_ShopOpen(16, 14);
    } else {
        if (Engine_GameFlagIsSet(0x895) == 0) {
            Engine_EventSetMessage((s32)MsgShianXianHasMartialArtsBut);
        } else {
            Engine_EventSetMessage((s32)MsgShianHsusInjuryTaughtGoodLesson);
        }
        Engine_EventShowMessage(14, 0);
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor15FlaggedDialogue(void)
{
    u16 *state = Object_GetById(ACTOR_PARTY_LEADER);
    u32 value = state[3];

    Engine_EventBegin();
    if (value >= 0xa001 && value <= 0xdfff) {
        Engine_ShopOpen(17, 15);
    } else {
        if (Engine_GameFlagIsSet(0x895) == 0) {
            Engine_EventSetMessage((s32)MsgShianMartialArtistsCantUseHeavy);
        } else {
            Engine_EventSetMessage((s32)MsgShianKungFuMakesQuickDoes);
        }
        Engine_EventShowMessage(15, 0);
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor16FlaggedDialogue(void)
{
    void Event_ShowMessage();
    void Engine_EventSetMessage(s32);

    u16 *state = Object_GetById(ACTOR_PARTY_LEADER);
    u32 value = state[3];

    Engine_EventBegin();
    if (value >= 0xa001 && value <= 0xdfff) {
        Engine_ShopOpen(18, 16);
    } else if (Engine_GameFlagIsSet(0x895) == 0) {
        Engine_EventSetMessage((s32)MsgShianXiansSpecialArmorNotSuited);
        Engine_EventShowMessage(16, 0);
    } else {
        Engine_EventSetMessage((s32)MsgShianDoPlanCrossDesertWarrior);
        Engine_EventAskYesNo(16, 0);
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor17FlaggedDialogue(void)
{
    s32 GameFlag_IsSet(s32);

    u16 *state = Object_GetById(ACTOR_PARTY_LEADER);
    u32 value = state[3];

    Engine_EventBegin();
    if (value < 0x2000 || value > 0xe000) {
        Engine_InnOpen(5, 17);
    } else {
        if (Engine_GameFlagIsSet(0x895) == 0) {
            Engine_EventSetMessage((s32)MsgShianWarriorsShouldLearnAboutTowns);
        } else {
            Engine_EventSetMessage((s32)MsgShianWeHaveFewCustomersThese);
        }
        Engine_EventShowMessage(17, 0);
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor10Dialogue(void)
{
    s32 Event_AskYesNo(s32, s32);

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShianSomethingWrongOnSilkRoad);
    Engine_EventAskYesNo(10, 0);
    Engine_EventEnd();
}

/* Actor 12 asks whether the monsters in Altin spat water. */
void SceneDialogue_RunActor12Dialogue(void)
{

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShianDidMonstersInAltinSpit);
    Engine_EventAskYesNo(12, 0);
    Engine_EventEnd();
}

/*
 * The Xian room's entry: the warrior's welcome, the scene table for the
 * entrance, and the objects the entrance sets up.
 */
void SceneDialogue_RunActor9MotionDialogue(void)
{
    void Engine_EventSetMessage(s32);

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShianWarriorWelcome);
    Engine_EventShowMessageAndWait(9, 0, 20);
    Engine_ActorFaceActor(9, 10, 0);
    Engine_EventWait(60);
    Engine_ActorFaceActor(9, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(9, 0);
    Engine_EventEnd();
}

s32 SceneData_SelectTable8d4cOr8a28(void)
{
    if (gGameState.entrance == 8) {
        return (s32)ShianHeya_SceneTable3Entrance8;
    }
    return (s32)ShianHeya_SceneTable3;
}

s32 SceneState_SetRuntimeWord448To521(void)
{

    s16 scene;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    scene = gGameState.entrance;
    if (scene == 4 || scene == 7) {
        OverlayObject_CreateConfigured(0x00f80000, 0, 0x01a10000, 20);
    } else if (scene == 6) {
        OverlayObject_CreateConfigured(0x01cc0000, 0, 0x02380000, 20);
        OverlayObject_CreateConfigured(0x01e40000, 0, 0x02380000, 20);
    } else if (scene == 8) {
        Engine_GameFlagClear(FLAG_ARRIVAL_EVENT_PENDING);
        Engine_ActorSetAnimation(10, 6);
    }
    return 0;
}
