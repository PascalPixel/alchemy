#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneState_ApplyFiveRectsAtColumn78(void);
void DialogueLayout_ConfigureTwoRegions(void);
void SceneEffect_InitSlotsEightToNineteen(void);
void SceneState_InitActorSlots8To19(void);
void SceneState_ConfigureEntries8Through19(void);
void Local_020031a8(void);
void FieldScene_RunScene3af_02001b58(void);
void FieldScene_RunScene3af_02001a98(void);
void FieldScene_RunActorTwentyDialogueSequence(void);
void FieldScene_RunShipDeckEventScript(void);
void FuneKanpan_RunDeckCrewScene(void);
void FieldScene_RunActorSequence(void);
void FieldScene_ConfigureActorGroup(void);
void FieldScene_ConfigureLeadActors(void);
void FieldScene_ConfigureThreeActors(void);
void FieldScene_RunPartyRosterScene(void);
void FieldScene_ConfigureFourActorPresentation(void);
void FuneKanpan_PlaceDeckActors(void);
void FieldScene_RunScene3af_02001920(void);
void FieldScene_RunScene3af_0200185c(void);
void SceneActor_PlaceActors20To27(void);
void FuneKanpan_PlaceRandomDeckActors(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
};

extern union GameStateRows Data_02000240_t;
extern s32 Data_0200db50[];
extern s32 Data_0200db60[];
extern u8 *Data_03001e70;

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Ship deck entry: record the arrival, set the deck by the voyage flags, then run the entrance's scene or place the deck crew. */
void FuneKanpan_ApplyEntryState(void)
{
    s32 flag;

    Engine_GameFlagSet(0x144);
    if (Engine_GameFlagIsSet(0x109) != 0) {
        Engine_GameFlagClear(0x271);
        Engine_GameFlagClear(0x272);
    }
    if (Engine_GameFlagIsSet(0x93e) != 0) {
        SceneState_ApplyFiveRectsAtColumn78();
        DialogueLayout_ConfigureTwoRegions();
        Engine_ActorSetChildValue(24, 2);
    } else if (Engine_GameFlagIsSet(0x8a0) != 0) {
        SceneState_ApplyFiveRectsAtColumn78();
        DialogueLayout_ConfigureTwoRegions();
    } else if ((flag = Engine_GameFlagIsSet(0x927)) != 0) {
        SceneState_ApplyFiveRectsAtColumn78();
        SceneEffect_InitSlotsEightToNineteen();
        Value2(Engine_TaskAddCallback, (s32)SceneState_ConfigureEntries8Through19, 0xc80);
        Data_0200db50[1] = 0x200000;
        Data_0200db60[1] = 0x13333;
        Value2(Engine_TaskAddCallback, (s32)Local_020031a8, 0xc80);
    } else if (Engine_GameFlagIsSet(0x928) != 0) {
        SceneState_ApplyFiveRectsAtColumn78();
        Data_0200db50[1] = flag;
        Data_0200db60[1] = flag;
        Value2(Engine_TaskAddCallback, (s32)Local_020031a8, 0xc80);
    }
    if (Engine_GameFlagIsSet(0x927) == 0) {
        SceneState_InitActorSlots8To19();
    }
    switch (Data_02000240_t.halves[225][0]) {
    case 4:
        Engine_ActorGet(0)->sprite->priority = 1;
        break;
    case 10:
        if (Engine_GameFlagIsSet(0x928) != 0) {
            FieldScene_RunScene3af_02001b58();
        } else {
            FieldScene_RunScene3af_02001a98();
        }
        return;
    case 11:
        FieldScene_RunActorTwentyDialogueSequence();
        return;
    case 12:
        FieldScene_RunShipDeckEventScript();
        return;
    case 13:
        FuneKanpan_RunDeckCrewScene();
        return;
    case 14:
        FieldScene_RunActorSequence();
        return;
    case 15:
        FieldScene_ConfigureActorGroup();
        return;
    case 16:
        if (Engine_GameFlagIsSet(0x109) != 0) {
            FieldScene_RunScene3af_02001920();
        } else {
            FieldScene_ConfigureLeadActors();
        }
        return;
    case 17:
        FieldScene_ConfigureThreeActors();
        return;
    case 18:
        if (Engine_GameFlagIsSet(0x109) == 0) {
            FieldScene_RunPartyRosterScene();
        }
        return;
    case 19:
        FieldScene_ConfigureFourActorPresentation();
        return;
    }
    if (Engine_GameFlagIsSet(0x93e) != 0) {
        *(s32 *)(Data_03001e70 + 236) = 0x410000;
    } else if (Engine_GameFlagIsSet(0x8a0) != 0) {
        FuneKanpan_PlaceDeckActors();
    } else if (Engine_GameFlagIsSet(0x92b) != 0) {
        FieldScene_RunScene3af_02001920();
    } else if (Engine_GameFlagIsSet(0x928) != 0) {
        FieldScene_RunScene3af_0200185c();
    } else if (Engine_GameFlagIsSet(0x925) != 0) {
        SceneActor_PlaceActors20To27();
    } else if (Engine_GameFlagIsSet(0x911) != 0) {
        FuneKanpan_PlaceRandomDeckActors();
    }
}
