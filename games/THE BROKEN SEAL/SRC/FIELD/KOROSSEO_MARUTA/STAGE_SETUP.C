/* The log-rolling stage's setup: publish the scene phase, install its
 * per-frame task and lay out the stage's props, drifting obstacles, the
 * two banks of paired actors and the presentation extras, choosing among
 * alternative layouts by the story flags; then run the beat the scene's
 * sub-state names. The overlay's exported entry. */
#include "TYPES.H"
#include "CALL.H"

/* Cell holding the shared scene-work pointer; +448 is the scene phase word. */
extern u8 *gEventWork;

/* Shared cross-overlay scene-record block: +450 is the scene sub-state, read as
 * a signed halfword, and +498 is a byte this owner clears on the way in. */
extern u8 gGameState[];

/* The object script record 30 runs. */
extern u8 KorosseoMaruta_Record30Script[];

/* The stage picture the scene control decodes. */
extern char ResourceId_RivalPathC;

u8 *Engine_ActorGet();
s32 Engine_GameFlagIsSet();
void Engine_GameFlagSet();
s32 GameFlag_GetByte();
s32 Engine_TaskAddCallback();
void Engine_MapCopyCellAttributes();
void Engine_MapCopyCellsTo();
s32 Map_GetTerrainHeight();
void Engine_ActorSetSpriteFlags();
void Engine_ObjectSetAnimation();
void Engine_ObjectSetScript();
void Map_SetLayerEntryFlag();
void Engine_ActorSetAnimation();
void Engine_ActorDestroy();
void Engine_ActorSetChildValue();
void Object_LinkObjectAndSetCallback();
void BattleEffect_PauseObject();
void Engine_AudioPlayCue();
void Engine_EventRequestExit();
s32 Korosseo_ShowItemIcon();
s32 ColossoLogRollingStage_SetupSceneDescriptor();
void ColossoLogRollingStage_InitializeSceneControl();
void ColossoLogRollingStage_RestoreActorPositions();
void Korosseo_SelectSoloCompetitor();
void ColossoLogRollingStage_RunScriptedTransition();
void FieldScene_RunMultiPhaseActorSequence();
void Korosseo_RunGreetScene();
void ColossoLogRollingStage_ClearSavedActorPositions();
void ColossoLogRollingStage_ShowActorPositionMessage(void);
void ColossoLogRollingStage_MarkSceneProgress(void);
void ColossoLogRollingStage_SceneTask(void);

/* Builds the log-rolling stage, then runs the beat named by the scene
 * sub-state.  Returns 0 on every path. */
s32 StageSetup_BuildAndDispatch(void)
{
    u8 *rec;
    s32 i;
    s32 val;
    s32 hit;
    s32 cnt;

    *(s32 *)(gEventWork + 448) = 256;
    Engine_GameFlagSet(324);
    Engine_TaskAddCallback(ColossoLogRollingStage_ShowActorPositionMessage, 3200);
    Call6(Engine_MapCopyCellAttributes, 74, 60, 8, 6, 120, 60);

    Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);

    rec = Engine_ActorGet(10);
    Engine_ActorSetSpriteFlags(rec, 0);
    rec[85] = 0;
    *(s32 *)(rec + 12) = 0x200000;

    rec = Engine_ActorGet(11);
    Engine_ActorSetSpriteFlags(rec, 0);
    rec[85] = 0;
    *(s32 *)(rec + 12) = 0x40000;

    if (Engine_GameFlagIsSet(866) != 0) {
        Engine_ActorSetAnimation(9, 5);
        rec = Engine_ActorGet(10);
        *(s32 *)(rec + 12) = 0x40000;
        rec = Engine_ActorGet(11);
        *(s32 *)(rec + 12) = 0x200000;
        Call6(Engine_MapCopyCellAttributes, 15, 12, 1, 1, 13, 12);
    } else {
        rec = Engine_ActorGet(9);
        *(s32 *)(rec + 24) = 0x18000;
        *(s32 *)(rec + 28) = 0x18000;
        if (Engine_GameFlagIsSet(871) != 0) {
            Call6(Engine_MapCopyCellAttributes, 0, 24, 1, 1, 9, 12);
        } else {
            Call6(Engine_MapCopyCellAttributes, 0, 25, 1, 1, 9, 12);
        }
    }

    if (Engine_GameFlagIsSet(872) != 0) {
        Engine_MapCopyCellAttributes(15, 12, 1, 1, 13, 12);
        Engine_MapCopyCellAttributes(1, 25, 1, 1, 9, 12);
        rec = Engine_ActorGet(12);
        Engine_ActorSetSpriteFlags(rec, 0);
        rec[85] = 0;
        *(s32 *)(rec + 12) = 0x20000;
        rec[35] = 2;
        rec = Engine_ActorGet(10);
        *(s32 *)(rec + 12) = 0x40000;
        rec[35] = 2;
        rec = Engine_ActorGet(11);
        *(s32 *)(rec + 12) = 0x200000;
    }

    cnt = GameFlag_GetByte(880);
    if (cnt == 0) {
        cnt = 19;
    }
    rec = Engine_ActorGet(13);
    *(s32 *)(rec + 8) = (cnt << 20) + 0x80000;
    rec[85] = 0;
    rec[35] = 2;
    Call6(Engine_MapCopyCellAttributes, 18, 10, 3, 1, 18, 11);
    Engine_MapCopyCellAttributes(17, 11, 1, 1, cnt, 11);

    /* The three drifting obstacles: any one still at rest and clear of the
     * grid is planted and drawn twice, once on its own row and once 52 rows
     * further down. */
    for (i = 15; i <= 17; i++) {
        rec = Engine_ActorGet(i);
        hit = Map_GetTerrainHeight(0, *(s32 *)(rec + 8), *(s32 *)(rec + 16));
        if (*(s32 *)(rec + 12) == 0 && hit == 0) {
            rec[35] = 2;
            rec[85] = hit;
            Call6(Engine_MapCopyCellAttributes, 83, 13, 1, 1, *(s32 *)(rec + 8) >> 20, *(s32 *)(rec + 16) >> 20);
            Call6(Engine_MapCopyCellAttributes, 83, 13, 1, 1, *(s32 *)(rec + 8) >> 20, (*(s32 *)(rec + 16) >> 20) + 52);
        }
    }

    /* Two banks of five paired actors, 18..22 and 23..27.  The cleared arm
     * also puts each pair on the grid; the uncleared arm only prepares the
     * records and installs the second per-frame task. */
    if (Engine_GameFlagIsSet(865) != 0) {
        s32 state;
        s32 mode;
        s32 row;

        i = 18;
        state = 0;
        mode = 2;
        val = 33;
        row = 11;
        for (; i <= 22; i++) {
            rec = Engine_ActorGet(i);
            rec[35] = mode;
            Engine_ObjectSetAnimation(rec, 2);
            rec = Engine_ActorGet(i + 5);
            rec[35] = mode;
            rec[85] = state;
            *(s32 *)(rec + 12) = 0x200000;
            Engine_ObjectSetAnimation(rec, 10);
            Engine_MapCopyCellAttributes(74, 12, 1, 1, val, row);
            val += 2;
        }
        Engine_ActorSetAnimation(28, 10);
        BattleEffect_PauseObject(28);
    } else {
        s32 state;
        s32 mode;

        i = 18;
        state = 0;
        mode = 2;
        for (; i <= 22; i++) {
            rec = Engine_ActorGet(i);
            rec[35] = mode;
            rec = Engine_ActorGet(i + 5);
            rec[35] = mode;
            rec[85] = state;
            *(s32 *)(rec + 12) = 0x200000;
        }
        Engine_TaskAddCallback(ColossoLogRollingStage_SceneTask, 3200);
    }

    if (Engine_GameFlagIsSet(864) != 0) {
        Engine_ActorSetAnimation(29, 4);
        Call6(Engine_MapCopyCellAttributes, 47, 61, 1, 4, 49, 61);
    }

    if (Engine_GameFlagIsSet(867) != 0) {
        Map_SetLayerEntryFlag(1);
        rec = Engine_ActorGet(30);
        rec[85] = 0;
        *(s32 *)(rec + 8) = 0x046a0000;
        *(s32 *)(rec + 16) = 0xb80000;
        Engine_ActorSetSpriteFlags(rec, 0);
        Engine_ObjectSetAnimation(rec, 3);
        Engine_ObjectSetScript(rec, KorosseoMaruta_Record30Script);
    } else {
        Map_SetLayerEntryFlag(2);
    }

    if (Engine_GameFlagIsSet(873) != 0) {
        rec = Engine_ActorGet(31);
        Engine_ObjectSetAnimation(rec, 8);
        rec[35] = 2;
        Engine_MapCopyCellAttributes(86, 10, 1, 2, 84, 10);
        Engine_MapCopyCellAttributes(86, 9, 1, 1, 84, 12);
    } else {
        rec = Engine_ActorGet(31);
        Call6(Engine_MapCopyCellAttributes, 85, 9, 1, 4, *(s32 *)(rec + 8) >> 20, 9);
        Call6(Engine_MapCopyCellAttributes, 85, 9, 1, 4, *(s32 *)(rec + 8) >> 20, 61);
    }

    {
        s32 state;
        s32 mode;

        state = 0;
        mode = 2;
        rec = Engine_ActorGet(9);
        rec[85] = state;
        rec[35] = mode;
        rec = Engine_ActorGet(10);
        rec[85] = state;
        rec[35] = mode;
        rec = Engine_ActorGet(11);
        rec[85] = state;
        rec[35] = mode;
        Engine_ActorSetAnimation(8, 9);
        gGameState[498] = state;
    }
    Korosseo_ShowItemIcon(39, 3);
    Korosseo_ShowItemIcon(40, 17);
    Engine_ActorSetChildValue(8, 2);

    switch (*(s16 *)(gGameState + 450)) {
    case 1:
        ColossoLogRollingStage_SetupSceneDescriptor(0, 8, 6, 0x5e80000, 0xc00000, 39, 40);
        Call6(Engine_MapCopyCellsTo, 127, 0, 1, 2, 5, 2);
        Engine_ActorDestroy(32);
        Engine_ActorDestroy(33);
        Engine_ActorDestroy(34);
        Engine_ActorDestroy(35);
        Engine_ActorDestroy(36);
        Engine_ActorDestroy(37);
        Engine_ActorDestroy(38);
        if (Engine_GameFlagIsSet(265) == 0) {
            Engine_AudioPlayCue(17);
            Korosseo_SelectSoloCompetitor(0);
            ColossoLogRollingStage_RestoreActorPositions();
            Object_LinkObjectAndSetCallback(1, 0);
            ColossoLogRollingStage_RunScriptedTransition(3);
        }
        Object_LinkObjectAndSetCallback(1, 0);
        Object_LinkObjectAndSetCallback(2, 0);
        Object_LinkObjectAndSetCallback(3, 0);
        ColossoLogRollingStage_InitializeSceneControl((s32)&ResourceId_RivalPathC);
        break;

    case 2:
        Engine_TaskAddCallback(ColossoLogRollingStage_MarkSceneProgress, 3200);
        Engine_ActorDestroy(39);
        Engine_ActorDestroy(40);
        if (Engine_GameFlagIsSet(265) != 0) {
            break;
        }
        ColossoLogRollingStage_RestoreActorPositions();
        Korosseo_SelectSoloCompetitor(1);
        ColossoLogRollingStage_RunScriptedTransition(0);
        break;

    case 3:
        if (Engine_GameFlagIsSet(265) != 0) {
            break;
        }
        Korosseo_RunGreetScene(32);
        ColossoLogRollingStage_ClearSavedActorPositions();
        break;

    case 4:
        FieldScene_RunMultiPhaseActorSequence(1);
        Engine_EventRequestExit(4);
        Engine_GameFlagSet(2384);
        Engine_GameFlagSet(2385);
        break;

    case 5:
        FieldScene_RunMultiPhaseActorSequence(-1);
        Engine_EventRequestExit(5);
        Engine_GameFlagSet(2384);
        break;
    }
    return 0;
}
