#include "TYPES.H"
#include "CALL.H"
extern struct EventWork *gEventWork;

void Engine_TaskWait();
s32 Engine_GameFlagIsSet();
s32 Engine_GameFlagSet();
void Engine_ColorBufferApplySource();
void Engine_ColorBufferApplyTarget();
void Engine_ColorBufferInterpolate();
void Engine_ActorSetPosition();
void Scene_SpringStatueTrap();
void FieldScene_RunClosingSequence();
void Engine_GameFlagClear();
void FieldScene_RunFlaggedSequence();
s32 CheckAllStatueLights();
s32 Engine_MapCopyCellsTo();
void Map_ClearLayerEntryFlag();
void Engine_MapCopyCellAttributes();
void SoruNichigetsu_RunLightScene();
void BattleFx_SetQueuedSoundAndPlay();
void Engine_WorkSetValuesIfNonNegative();
void InitializeSceneRecordBuffer();

extern u8 Data_02000240[];
extern s16 gCell[][1];

s32 SoruNichigetsu_RestoreEntryState(void)
{
    u32 i;
    s32 record;
    s32 done;
    s32 lit;

    Engine_TaskWait(1);
    *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x204;
    done = 0;
    if (Engine_GameFlagIsSet(0x809) != 0 && Engine_GameFlagIsSet(0x814) == 0
        && Engine_GameFlagIsSet(0x819) == 0) {
        Engine_GameFlagSet(0x144);
    }
    Engine_ColorBufferApplySource(0x10000, 0);
    if (Engine_GameFlagIsSet(0x109) != 0) {
        if (Engine_GameFlagIsSet(0x201) != 0) {
            Engine_ColorBufferApplyTarget(0x2051cc, 1);
            Engine_ColorBufferInterpolate(1);
            Engine_TaskWait(1);
        } else if (Engine_GameFlagIsSet(0x202) != 0) {
            Engine_ColorBufferApplyTarget(0x202db1, 1);
            Engine_ColorBufferInterpolate(1);
            Engine_TaskWait(1);
        }
    } else {
        Engine_GameFlagSet(0x200);
        if (Engine_GameFlagIsSet(0x80a) != 0) {
            Call3(Engine_ActorSetPosition, 16, 0x2400000, 0x780000);
        }
    }
    if (gCell[225][0] == 4) {
        if (Value1(Engine_GameFlagIsSet, 0x813) == 0) {
            Scene_SpringStatueTrap();
            Engine_GameFlagSet(0x813);
            done = 1;
        }
    } else if (gCell[225][0] == 5) {
        if (Value1(Engine_GameFlagIsSet, 0x812) == 0) {
            FieldScene_RunClosingSequence();
            Engine_GameFlagSet(0x812);
            Engine_GameFlagClear(0x80b);
            Engine_GameFlagClear(0x80c);
            Engine_GameFlagClear(0x80d);
            Engine_GameFlagClear(0x80e);
            done = 1;
        }
    } else if (gCell[225][0] == 6) {
        if (Engine_GameFlagIsSet(0x812) != 0) {
            FieldScene_RunFlaggedSequence();
            Engine_GameFlagSet(0x822);
        }
        done = 1;
    }
    if (Value1(Engine_GameFlagIsSet, 0x80b) != 0) {
        Engine_GameFlagSet(0x826);
    }
    if (Value1(Engine_GameFlagIsSet, 0x80c) != 0) {
        Engine_GameFlagSet(0x827);
    }
    if (Value1(Engine_GameFlagIsSet, 0x80d) != 0) {
        Engine_GameFlagSet(0x828);
    }
    if (Value1(Engine_GameFlagIsSet, 0x80e) != 0) {
        Engine_GameFlagSet(0x829);
    }
    Engine_TaskWait(4);
    if (done != 0) {
    } else {
        if (CheckAllStatueLights() == 0) {
        } else {
            Call6(Engine_MapCopyCellsTo, 30, 44, 30, 38, 12, 5);
            Call6(Engine_MapCopyCellsTo, 30, 44, 34, 37, 4, 1);
            Call6(Engine_MapCopyCellsTo, 14, 41, 32, 41, 8, 4);
            Call6(Engine_MapCopyCellsTo, 45, 28, 34, 10, 4, 2);
            Call6(Engine_MapCopyCellsTo, 45, 30, 16, 10, 4, 2);
            Engine_MapCopyCellsTo(14, 45, 14, 41, 8, 4);
            if (gCell[225][0] != 8) {
                if (Engine_GameFlagIsSet(0x814) == 0) {
                    if (Engine_GameFlagIsSet(0x819) != 0) {
                        Call6(Engine_MapCopyCellsTo, 0, 32, 17, 39, 2, 1);
                        Call6(Engine_MapCopyCellsTo, 42, 62, 17, 36, 2, 3);
                        Call6(Engine_MapCopyCellsTo, 0, 32, 17, 40, 2, 1);
                        Call6(Engine_MapCopyCellsTo, 0, 32, 17, 41, 2, 1);
                        Engine_MapCopyCellsTo(0, 32, 17, 42, 2, 3);
                    } else {
                        Engine_MapCopyCellsTo(44, 59, 17, 37, 2, 6);
                    }
                    Map_ClearLayerEntryFlag(9);
                    Map_ClearLayerEntryFlag(10);
                }
            }
            Engine_ActorSetPosition(16, 0, 0);
            goto L_02002510;
        }
        lit = 0;
        if (Engine_GameFlagIsSet(0x80b) != 0) {
            Call6(Engine_MapCopyCellsTo, 45, 28, 34, 10, 2, 1);
            Engine_MapCopyCellsTo(45, 30, 16, 10, 2, 1);
            lit = 1;
        }
        if (Engine_GameFlagIsSet(0x80c) != 0) {
            Call6(Engine_MapCopyCellsTo, 47, 28, 36, 10, 2, 1);
            Engine_MapCopyCellsTo(47, 30, 18, 10, 2, 1);
            lit = 1;
        }
        if (Engine_GameFlagIsSet(0x80d) != 0) {
            Call6(Engine_MapCopyCellsTo, 45, 29, 34, 11, 2, 1);
            Engine_MapCopyCellsTo(45, 31, 16, 11, 2, 1);
            lit = 1;
        }
        if (Engine_GameFlagIsSet(0x80e) != 0) {
            Call6(Engine_MapCopyCellsTo, 47, 29, 36, 11, 2, 1);
            Engine_MapCopyCellsTo(47, 31, 18, 11, 2, 1);
            lit = 1;
        }
        if (Engine_GameFlagIsSet(0x812) == 0) {
            if (lit == 0) {
                goto L_020024fc;
            }
        }
        Call6(Engine_MapCopyCellsTo, 30, 43, 32, 40, 8, 3);
        Call6(Engine_MapCopyCellsTo, 30, 43, 33, 39, 8, 1);
        Call6(Engine_MapCopyCellsTo, 30, 43, 36, 38, 3, 3);
        Engine_MapCopyCellsTo(36, 58, 32, 41, 8, 4);
        L_020024fc:;
        Call6(Engine_MapCopyCellAttributes, 15, 6, 2, 1, 17, 6);
    }
    L_02002510:;
    if (Value1(Engine_GameFlagIsSet, 0x309) == 0) {
        if (gCell[225][0] != 8) {
            goto L_0200254a;
        }
        SoruNichigetsu_RunLightScene();
        Engine_GameFlagSet(0x309);
        Call6(Engine_MapCopyCellAttributes, 15, 6, 2, 1, 17, 6);
    } else {
        L_0200254a:;
        if (Engine_GameFlagIsSet(0x814) != 0) {
            BattleFx_SetQueuedSoundAndPlay(141);
            Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
            InitializeSceneRecordBuffer();
            Call6(Engine_MapCopyCellAttributes, 15, 6, 2, 1, 17, 6);
        }
    }
    return 0;
}
