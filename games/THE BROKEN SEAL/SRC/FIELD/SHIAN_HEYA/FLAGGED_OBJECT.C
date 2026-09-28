#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum FlaggedObjectMessage {
    MSG_XIAN_HAS_MARTIAL_ARTS_BUT = 0x1817,
    MSG_MARTIAL_ARTISTS_CANT_USE_HEAVY = 0x1819,
    MSG_XIANS_SPECIAL_ARMOR_NOT_SUITED = 0x181b,
    MSG_WARRIORS_SHOULD_LEARN_ABOUT_TOWNS = 0x181d,
    MSG_SOMETHING_WRONG_ON_SILK_ROAD = 0x1a3a,
    MSG_HSUS_INJURY_TAUGHT_GOOD_LESSON = 0x1a46,
    MSG_KUNG_FU_MAKES_QUICK_DOES = 0x1a48,
    MSG_DO_PLAN_CROSS_DESERT_WARRIOR = 0x1a4a,
    MSG_WE_HAVE_FEW_CUSTOMERS_THESE = 0x1a4e,
    MSG_WARRIOR_WELCOME = 0x1a64
};


#define NULL ((void *)0)
void Effect_Move(void *object);

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
            Event_SetMessage(MSG_XIAN_HAS_MARTIAL_ARTS_BUT);
        } else {
            Event_SetMessage(MSG_HSUS_INJURY_TAUGHT_GOOD_LESSON);
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
            Event_SetMessage(MSG_MARTIAL_ARTISTS_CANT_USE_HEAVY);
        } else {
            Event_SetMessage(MSG_KUNG_FU_MAKES_QUICK_DOES);
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
        Event_SetMessage(MSG_XIANS_SPECIAL_ARMOR_NOT_SUITED);
        Event_ShowMessage(16, 0);
    } else {
        Event_SetMessage(MSG_DO_PLAN_CROSS_DESERT_WARRIOR);
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
            Event_SetMessage(MSG_WARRIORS_SHOULD_LEARN_ABOUT_TOWNS);
        } else {
            Event_SetMessage(MSG_WE_HAVE_FEW_CUSTOMERS_THESE);
        }
        Event_ShowMessage(17, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor10Dialogue(void)
{
    s32 Event_AskYesNo(s32, s32);

    Event_Begin();
    Event_SetMessage(MSG_SOMETHING_WRONG_ON_SILK_ROAD);
    Event_AskYesNo(10, 0);
    Event_End();
}
