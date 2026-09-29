/* Draft (unverified): the Colosseum log-rolling stage setup, begun by an
 * agent adopting number-bound overlay drafts and interrupted before it was
 * linked or compared. */
/* The log-rolling stage's scene start, entry veneer 0: opens the screen,
 * places the logs, obstacles and paired actors from the story flags, then
 * runs the beat the entrance names. */
/* FAKEMATCH: the six-argument cell copies go through the call wrapper, which
 * passes their constants straight into the argument registers; the item
 * icon and scene descriptor calls are declared returning a value, which
 * sets r0 last of the arguments as the game does; and the loops hold their
 * constants in the per-arm variables the game allocates. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "RESOURCE_IDS.H"

/* Record 30's script, where the overlay's data lies. */
extern const s32 KorosseoMaruta_Object30Script[];

s32 GameFlag_GetByte(s32 flag);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Map_SetLayerEntryFlag(s32 value);
void Object_LinkObjectAndSetCallback(s32 object, s32 callback);
void BattleEffect_PauseObject(s32 object);
s32 Korosseo_ShowItemIcon();
s32 ColossoLogRollingStage_SetupSceneDescriptor();
void ColossoLogRollingStage_ShowActorPositionMessage(void);
void ColossoLogRollingStage_SceneTask(void);
void ColossoLogRollingStage_MarkSceneProgress(void);
void ColossoLogRollingStage_InitializeSceneControl(s32 resource);
void ColossoLogRollingStage_RestoreActorPositions(void);
void ColossoLogRollingStage_ClearSavedActorPositions(void);
void ColossoLogRollingStage_RunScriptedTransition(s32 mode);
void Korosseo_SelectSoloCompetitor(s32 id);
void Korosseo_RunGreetScene(s32 a0);
void FieldScene_RunMultiPhaseActorSequence(s32 a0);

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 StageSetup_BuildAndDispatch(void)
{
    u8 *rec;
    s32 i;
    s32 val;
    s32 hit;
    s32 cnt;

    gEventWork->start_transition = 0x100;
    Engine_GameFlagSet(324);
    Engine_TaskAddCallback(ColossoLogRollingStage_ShowActorPositionMessage, 3200);
    Call6((void (*)())Engine_MapCopyCellAttributes, 74, 60, 8, 6, 120, 60);

    Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);

    rec = (u8 *)Engine_ActorGet(10);
    Engine_ActorSetSpriteFlags((struct FieldActor *)rec, 0);
    rec[85] = 0;
    *(s32 *)(rec + 12) = 0x200000;

    rec = (u8 *)Engine_ActorGet(11);
    Engine_ActorSetSpriteFlags((struct FieldActor *)rec, 0);
    rec[85] = 0;
    *(s32 *)(rec + 12) = 0x40000;

    if (Engine_GameFlagIsSet(866) != 0) {
        Engine_ActorSetAnimation(9, 5);
        rec = (u8 *)Engine_ActorGet(10);
        *(s32 *)(rec + 12) = 0x40000;
        rec = (u8 *)Engine_ActorGet(11);
        *(s32 *)(rec + 12) = 0x200000;
        Call6((void (*)())Engine_MapCopyCellAttributes, 15, 12, 1, 1, 13, 12);
    } else {
        rec = (u8 *)Engine_ActorGet(9);
        *(s32 *)(rec + 24) = 0x18000;
        *(s32 *)(rec + 28) = 0x18000;
        if (Engine_GameFlagIsSet(871) != 0) {
            Call6((void (*)())Engine_MapCopyCellAttributes, 0, 24, 1, 1, 9, 12);
        } else {
            Call6((void (*)())Engine_MapCopyCellAttributes, 0, 25, 1, 1, 9, 12);
        }
    }

    if (Engine_GameFlagIsSet(872) != 0) {
        Call6((void (*)())Engine_MapCopyCellAttributes, 15, 12, 1, 1, 13, 12);
        Call6((void (*)())Engine_MapCopyCellAttributes, 1, 25, 1, 1, 9, 12);
        rec = (u8 *)Engine_ActorGet(12);
        Engine_ActorSetSpriteFlags((struct FieldActor *)rec, 0);
        rec[85] = 0;
        *(s32 *)(rec + 12) = 0x20000;
        rec[35] = 2;
        rec = (u8 *)Engine_ActorGet(10);
        *(s32 *)(rec + 12) = 0x40000;
        rec[35] = 2;
        rec = (u8 *)Engine_ActorGet(11);
        *(s32 *)(rec + 12) = 0x200000;
    }

    cnt = GameFlag_GetByte(880);
    if (cnt == 0) {
        cnt = 19;
    }
    rec = (u8 *)Engine_ActorGet(13);
    *(s32 *)(rec + 8) = (cnt << 20) + 0x80000;
    rec[85] = 0;
    rec[35] = 2;
    Call6((void (*)())Engine_MapCopyCellAttributes, 18, 10, 3, 1, 18, 11);
    Call6((void (*)())Engine_MapCopyCellAttributes, 17, 11, 1, 1, cnt, 11);

    /* The three drifting obstacles: any one still at rest and clear of the
     * grid is planted and drawn twice, once on its own row and once 52 rows
     * further down. */
    for (i = 15; i <= 17; i++) {
        rec = (u8 *)Engine_ActorGet(i);
        hit = Map_GetTerrainHeight(0, *(s32 *)(rec + 8), *(s32 *)(rec + 16));
        if (*(s32 *)(rec + 12) == 0 && hit == 0) {
            rec[35] = 2;
            rec[85] = hit;
            Call6((void (*)())Engine_MapCopyCellAttributes, 83, 13, 1, 1, *(s32 *)(rec + 8) >> 20,
                          *(s32 *)(rec + 16) >> 20);
            Call6((void (*)())Engine_MapCopyCellAttributes, 83, 13, 1, 1, *(s32 *)(rec + 8) >> 20,
                          (*(s32 *)(rec + 16) >> 20) + 52);
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
            rec = (u8 *)Engine_ActorGet(i);
            rec[35] = mode;
            Engine_ObjectSetAnimation((struct FieldActor *)rec, 2);
            rec = (u8 *)Engine_ActorGet(i + 5);
            rec[35] = mode;
            rec[85] = state;
            *(s32 *)(rec + 12) = 0x200000;
            Engine_ObjectSetAnimation((struct FieldActor *)rec, 10);
            Call6((void (*)())Engine_MapCopyCellAttributes, 74, 12, 1, 1, val, row);
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
            rec = (u8 *)Engine_ActorGet(i);
            rec[35] = mode;
            rec = (u8 *)Engine_ActorGet(i + 5);
            rec[35] = mode;
            rec[85] = state;
            *(s32 *)(rec + 12) = 0x200000;
        }
        Engine_TaskAddCallback(ColossoLogRollingStage_SceneTask, 3200);
    }

    if (Engine_GameFlagIsSet(864) != 0) {
        Engine_ActorSetAnimation(29, 4);
        Call6((void (*)())Engine_MapCopyCellAttributes, 47, 61, 1, 4, 49, 61);
    }

    if (Engine_GameFlagIsSet(867) != 0) {
        Map_SetLayerEntryFlag(1);
        rec = (u8 *)Engine_ActorGet(30);
        rec[85] = 0;
        *(s32 *)(rec + 8) = 0x046a0000;
        *(s32 *)(rec + 16) = 0xb80000;
        Engine_ActorSetSpriteFlags((struct FieldActor *)rec, 0);
        Engine_ObjectSetAnimation((struct FieldActor *)rec, 3);
        Engine_ObjectSetScript((struct FieldActor *)rec, KorosseoMaruta_Object30Script);
    } else {
        Map_SetLayerEntryFlag(2);
    }

    if (Engine_GameFlagIsSet(873) != 0) {
        rec = (u8 *)Engine_ActorGet(31);
        Engine_ObjectSetAnimation((struct FieldActor *)rec, 8);
        rec[35] = 2;
        Call6((void (*)())Engine_MapCopyCellAttributes, 86, 10, 1, 2, 84, 10);
        Call6((void (*)())Engine_MapCopyCellAttributes, 86, 9, 1, 1, 84, 12);
    } else {
        rec = (u8 *)Engine_ActorGet(31);
        Call6((void (*)())Engine_MapCopyCellAttributes, 85, 9, 1, 4, *(s32 *)(rec + 8) >> 20, 9);
        Call6((void (*)())Engine_MapCopyCellAttributes, 85, 9, 1, 4, *(s32 *)(rec + 8) >> 20, 61);
    }

    {
        s32 state;
        s32 mode;

        state = 0;
        mode = 2;
        rec = (u8 *)Engine_ActorGet(9);
        rec[85] = state;
        rec[35] = mode;
        rec = (u8 *)Engine_ActorGet(10);
        rec[85] = state;
        rec[35] = mode;
        rec = (u8 *)Engine_ActorGet(11);
        rec[85] = state;
        rec[35] = mode;
        Engine_ActorSetAnimation(8, 9);
        gGameState.movement_mode = state;
    }
    Korosseo_ShowItemIcon(39, 3);
    Korosseo_ShowItemIcon(40, 17);
    Engine_ActorSetChildValue(8, 2);

    switch (gGameState.entrance) {
    case 1:
        ColossoLogRollingStage_SetupSceneDescriptor(0, 8, 6, 0x5e80000, 0xc00000, 39, 40);
        Call6((void (*)())Engine_MapCopyCellsTo, 127, 0, 1, 2, 5, 2);
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
        ColossoLogRollingStage_InitializeSceneControl((s32)&ResourceId_PictureC);
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
