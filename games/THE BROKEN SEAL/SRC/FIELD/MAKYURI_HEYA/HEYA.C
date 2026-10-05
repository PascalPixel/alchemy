#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "PROBE.H"

extern u8 MsgMakyuriHeyaStrangeForcesBlockWay[];

extern const struct SceneEntrance gMakyuriHeyaEntrances4[];
extern const struct SceneEntrance gMakyuriHeyaEntrances3[];
extern const struct SceneEntrance gMakyuriHeyaEntrances2[];
extern const struct SceneEntrance gMakyuriHeyaEntrancesOther[];

extern u8 MakyuriHeya_SceneTable[];

extern const struct ScenePlacement gMakyuriHeyaPlacements1[];
extern const struct ScenePlacement gMakyuriHeyaPlacements2[];
extern const struct ScenePlacement gMakyuriHeyaPlacements3[];
extern const struct ScenePlacement gMakyuriHeyaPlacements4[];
extern const struct ScenePlacement gMakyuriHeyaPlacementsOther[];

extern const struct SceneEvent gMakyuriHeyaEvents1[];
extern const struct SceneEvent gMakyuriHeyaEvents2[];
extern const struct SceneEvent gMakyuriHeyaEvents3[];
extern const struct SceneEvent gMakyuriHeyaEventsOther[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MakyuriHeya4) {
        return gMakyuriHeyaEntrances4;
    }
    if (selector == (s32)&SceneId_MakyuriHeya3) {
        return gMakyuriHeyaEntrances3;
    }
    if (selector == (s32)&SceneId_MakyuriHeya2) {
        return gMakyuriHeyaEntrances2;
    }
    return gMakyuriHeyaEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *MakyuriHeya_GetExits(void)
{
    return MakyuriHeya_SceneTable;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MakyuriHeya1) {
        return gMakyuriHeyaPlacements1;
    }
    if (selector == (s32)&SceneId_MakyuriHeya2) {
        return gMakyuriHeyaPlacements2;
    }
    if (selector == (s32)&SceneId_MakyuriHeya3) {
        return gMakyuriHeyaPlacements3;
    }
    if (selector == (s32)&SceneId_MakyuriHeya4) {
        return gMakyuriHeyaPlacements4;
    }
    return gMakyuriHeyaPlacementsOther;
}

void MakyuriHeya_TickSpawnTimer(void)
{
    Makyuri_TickSpawnTimer();
}

void MakyuriHeya_ClearSceneRecordAndTarget(void)
{
    SceneState_ClearCurrentRecordAndReleaseTarget();
}

void FieldScene_RunSingleStep(void)
{
    Makyuri_RunActorMove();
}

void MakyuriHeya_TriggerPillarRise(void)
{
    MakyuriIriguchi_RaisePillar();
}

void SceneDialogue_RunLine1637(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgMakyuriHeyaStrangeForcesBlockWay, 1);
    Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MakyuriHeya1) {
        return gMakyuriHeyaEvents1;
    }
    if (selector == (s32)&SceneId_MakyuriHeya2) {
        return gMakyuriHeyaEvents2;
    }
    if (selector == (s32)&SceneId_MakyuriHeya3) {
        return gMakyuriHeyaEvents3;
    }
    return gMakyuriHeyaEventsOther;
}
