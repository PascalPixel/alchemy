#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "RESOURCE_IDS.H"

s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 GameFlag_GetByteFar(s32 flag);

void Map_UpdateCellRect(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
s32 Korosseo_ShowItemIcon(s32 slot, s32 item);
s32 FieldScene_BuildSceneDescriptorAndInstallTask(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
void Korosseo_SelectSoloCompetitor(s32 index);
void SceneActor_PlaceSlots1To3FromWork(void);
void SceneActor_MarkObjectAtTiles94To95(s32 actor);
void FieldScene_RunTwoArmSequence(s32 arm);
void FieldScene_RunCommandSequence(s32 actor);
void FieldScene_RunSixSteps380To3A8(void);
void FieldScene_RunScene3ba_02000974(s32 direction);
void SceneState_InitControlWhenFlag109Clear(s32 value);
void Object_LinkObjectAndSetCallback(s32 actor, s32 leader);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void FieldScene_RunOpeningAuxiliarySequence(void);
void SceneState_SetStateHalfword386To99WhenMatched(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
};



static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* River arena entry: record the arrival, set the logs, ledges and item icons by the story flags, then start the entrance's opening scene. */
s32 KorosseoKawa_ApplyEntryState(void)
{
    struct FieldActor *actor;
    s32 hit;
    s32 x;
    s32 z;
    s32 zero;
    s32 col;

    gEventWork->start_transition = 0;
    GameFlag_Set(0x144);
    actor = Engine_ActorGet(9);
    hit = Map_GetTerrainHeightFar(0, actor->x.fixed, actor->z.fixed);
    if (actor->y.fixed == 0 && hit == 0) {
        actor->priority_flags = 2;
        actor->motion_flags = hit;
        Map_CopyCellAttributes(14, 13, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
    }
    x = GameFlag_GetByteFar(0x310);
    if (x == 0) {
        x = 25;
    }
    actor = Engine_ActorGet(10);
    actor->x.fixed = (x << 20) + 0x80000;
    actor->motion_flags = 0;
    actor->priority_flags = 2;
    Map_CopyCellAttributes(14, 13, 1, 1, x, 12);
    Scheduler_AddOrUpdateCallback(FieldScene_RunOpeningAuxiliarySequence, 0xc80);
    actor = Engine_ActorGet(15);
    actor->unknown_22 = 1;
    zero = 0;
    if (Engine_GameFlagIsSet(0x303)) {
        Engine_ObjectSetAnimation(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
        actor->collision_flags = zero;
        actor->priority_flags = 3;
        Map_CopyCellAttributes(47, 24, 1, 1, 47, 12);
    }
    actor = Engine_ActorGet(17);
    col = actor->z.fixed >> 20;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Map_CopyCellAttributes(64, 24, 3, 1, 64, col);
    actor = Engine_ActorGet(18);
    col = actor->x.fixed >> 20;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Map_CopyCellAttributes(63, 25, 1, 3, col, 9);
    if (Engine_GameFlagIsSet(0x302)) {
        Map_CopyCellAttributes(37, 7, 1, 4, 34, 7);
        Map_CopyCellAttributes(36, 7, 1, 4, 37, 7);
        Map_CopyCells(100, 29, 1, 3, 34, 38);
    }
    actor = Engine_ActorGet(13);
    if (Engine_GameFlagIsSet(0x301)) {
        Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
        actor->motion_flags = zero;
        actor->acceleration = 0x6666;
        actor->speed = 0xcccc;
        actor->y.fixed = 0x80000;
        Engine_ObjectSetAnimation(actor, 3);
    } else {
        Engine_ObjectSetAnimation(actor, 2);
    }
    Engine_ActorGet(14)->priority_flags = 2;
    Korosseo_ShowItemIcon(24, 120);
    Korosseo_ShowItemIcon(25, 127);
    switch (((union GameStateRows *)&gGameState)->halves[225][0]) {
    case 1:
        FieldScene_BuildSceneDescriptorAndInstallTask(0, 8, 4, 0x5180000, 0xc00000, 24, 25);
        Call6(Map_UpdateCellRect, 127, 0, 1, 2, 19, 2);
        Engine_ActorDestroy(19);
        Engine_ActorDestroy(20);
        Engine_ActorDestroy(21);
        Engine_ActorDestroy(22);
        Engine_ActorDestroy(23);
        if (!Engine_GameFlagIsSet(0x109)) {
            Engine_AudioPlayCue(17);
            Korosseo_SelectSoloCompetitor(0);
            SceneActor_PlaceSlots1To3FromWork();
            SceneActor_MarkObjectAtTiles94To95(1);
            SceneActor_MarkObjectAtTiles94To95(2);
            SceneActor_MarkObjectAtTiles94To95(3);
            FieldScene_RunTwoArmSequence(1);
        }
        Object_LinkObjectAndSetCallback(1, 0);
        Object_LinkObjectAndSetCallback(2, 0);
        Object_LinkObjectAndSetCallback(3, 0);
        SceneState_InitControlWhenFlag109Clear((s32)&ResourceId_RivalPathA);
        break;
    case 2:
        Scheduler_AddOrUpdateCallback(SceneState_SetStateHalfword386To99WhenMatched, 0xc80);
        Engine_ActorDestroy(24);
        Engine_ActorDestroy(25);
        if (!Engine_GameFlagIsSet(0x109)) {
            SceneActor_PlaceSlots1To3FromWork();
            Korosseo_SelectSoloCompetitor(1);
            FieldScene_RunTwoArmSequence(0);
        }
        break;
    case 3:
        if (!Engine_GameFlagIsSet(0x109)) {
            FieldScene_RunCommandSequence(19);
            FieldScene_RunSixSteps380To3A8();
        }
        break;
    case 4:
        FieldScene_RunScene3ba_02000974(1);
        Engine_EventRequestExit(4);
        break;
    case 5:
        FieldScene_RunScene3ba_02000974(-1);
        Engine_EventRequestExit(5);
        break;
    }
    return 0;
}
