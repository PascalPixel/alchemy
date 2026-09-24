#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneState_RunFlagGatedSetupCascade(void);
void RunSceneSelectionChain(void);
void Local_02003364(void);
void FieldScene_RunScene3b1_02003dec(void);
void FieldScene_RunScene3b1_02003d10(void);
void FieldScene_RunScene3b1_02003eec(void);
void FieldScene_RunScene3b1_02003e34(void);
void FieldScene_RunFlagBranchedSetupCascade(void);
void FieldScene_RunScene3b1_020040e8(void);
void FieldScene_RunScene3b1_0200413c(void);
void FieldScene_RunScene3b1_02004198(void);
void FieldScene_RunActors24And25Setup(void);
void FieldScene_RunScene3b1_02005068(void);
void FieldScene_RunActors24And25SetupWithValue929(void);
void FieldScene_RunScene3b1_020056dc(void);
void FieldScene_RunActors24And25SetupWithValue92a(void);
void FieldScene_RunExtendedFormationPresentation(void);
void RunActorsEightAndNineMapEvent(void);
void Scene_RunFourActorProgressPresentation(void);
void FieldScene_RunScene3b1_02006110(void);
void FieldScene_RunSceneStep(s32 step, s32 a1, s32 a2);
void Scene_UpdateCueTimer(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
};

extern union GameStateRows Data_02000240_t;
extern const u8 Data_0200e8e4[];
extern s32 Data_0200ff84;

/* Ship cabins entry: record the arrival, set the companions' child values by the story flags, then run the entrance's scene. */
s32 FuneHeya_ApplyEntryState(void)
{
    Engine_TaskWait(1);
    Engine_GameFlagSet(0x144);
    *(s32 *)(*(s32 *)0x03001ebc + 0x1c0) = 0x209;
    switch (Data_02000240_t.halves[225][0]) {
    case 1:
    case 2:
    case 11:
        if (Engine_GameFlagIsSet(0x93e) == 0 && Engine_GameFlagIsSet(0x928) != 0) {
            Engine_ActorSetChildValue(9, 2);
        } else if (Engine_GameFlagIsSet(0x911) != 0) {
            Engine_ActorSetChildValue(12, 2);
        }
        break;
    case 4:
    case 12:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 23:
    case 24:
        Engine_ActorSetChildValue(19, 2);
        break;
    case 5:
        if (Engine_GameFlagIsSet(0x93e) == 0 && Engine_GameFlagIsSet(0x911) != 0) {
            Engine_ActorSetChildValue(13, 2);
        }
        break;
    }
    switch (Data_02000240_t.halves[225][0]) {
    case 1:
    case 2:
        SceneState_RunFlagGatedSetupCascade();
        break;
    case 4:
        RunSceneSelectionChain();
        break;
    case 5:
        Local_02003364();
        break;
    case 10:
        if (Engine_GameFlagIsSet(0x928) != 0) {
            FieldScene_RunScene3b1_02003dec();
        } else {
            FieldScene_RunScene3b1_02003d10();
        }
        break;
    case 11:
        if (Engine_GameFlagIsSet(0x928) != 0) {
            FieldScene_RunScene3b1_02003eec();
        } else {
            FieldScene_RunScene3b1_02003e34();
        }
        break;
    case 12:
        FieldScene_RunFlagBranchedSetupCascade();
        break;
    case 13:
        FieldScene_RunScene3b1_020040e8();
        break;
    case 14:
        FieldScene_RunScene3b1_0200413c();
        break;
    case 15:
        if (Engine_GameFlagIsSet(0x109) != 0) {
            Engine_EventBegin();
            FieldScene_RunSceneStep(25, 1, 0);
            FieldScene_RunSceneStep(22, 0, 0);
            Engine_ActorEnableActionCallback(36, Data_0200e8e4);
            Engine_ActorEnableActionCallback(37, Data_0200e8e4);
            Engine_ActorEnableActionCallback(38, Data_0200e8e4);
            Engine_ActorSetChildValue(36, 3);
            Engine_ActorSetChildValue(37, 3);
            Engine_ActorSetChildValue(38, 3);
            Engine_EventEnd();
        } else {
            FieldScene_RunScene3b1_02004198();
        }
        break;
    case 16:
        FieldScene_RunActors24And25Setup();
        break;
    case 17:
        if (Engine_GameFlagIsSet(0x109) != 0) {
            Engine_EventBegin();
            FieldScene_RunSceneStep(25, 2, 0);
            FieldScene_RunSceneStep(22, 0, 0);
            Engine_ActorEnableActionCallback(36, Data_0200e8e4);
            Engine_ActorEnableActionCallback(37, Data_0200e8e4);
            Engine_EventEnd();
        } else {
            FieldScene_RunScene3b1_02005068();
        }
        break;
    case 18:
        FieldScene_RunActors24And25SetupWithValue929();
        break;
    case 19:
        if (Engine_GameFlagIsSet(0x109) != 0) {
            Engine_EventBegin();
            FieldScene_RunSceneStep(25, 3, 0);
            FieldScene_RunSceneStep(22, 0, 0);
            Engine_ActorEnableActionCallback(36, Data_0200e8e4);
            Engine_ActorEnableActionCallback(37, Data_0200e8e4);
            Engine_ActorSetChildValue(36, 3);
            Engine_ActorSetChildValue(37, 3);
            Engine_EventEnd();
        } else {
            FieldScene_RunScene3b1_020056dc();
        }
        break;
    case 20:
        FieldScene_RunActors24And25SetupWithValue92a();
        break;
    case 21:
        if (Engine_GameFlagIsSet(0x109) != 0) {
            if (Engine_GameFlagIsSet(0x302) != 0) {
                /* FAKEMATCH: the held zero loads the timer address first. */
                {
                    s32 zero = 0;

                    Data_0200ff84 = zero;
                }
                Engine_TaskAddCallback(Scene_UpdateCueTimer, 0xc80);
                Engine_ActorSetAnimation(9, 5);
            }
        } else {
            FieldScene_RunExtendedFormationPresentation();
        }
        break;
    case 22:
        RunActorsEightAndNineMapEvent();
        break;
    case 23:
        if (Engine_GameFlagIsSet(0x109) == 0) {
            Scene_RunFourActorProgressPresentation();
        }
        break;
    case 24:
        FieldScene_RunScene3b1_02006110();
        break;
    case 30:
        FieldScene_RunSceneStep(20, 0x926, 0x92b);
        FieldScene_RunSceneStep(21, 0, 0);
        Engine_GameFlagSet(0x902);
        Engine_EventRequestExit(1);
        break;
    }
    return 0;
}
