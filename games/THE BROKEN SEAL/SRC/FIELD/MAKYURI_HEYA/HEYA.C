#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "PROBE.H"

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

/* Where the party appears in each of the lighthouse rooms; the first takes
   the table the other scenes take. */
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

u8 *SceneData_GetTableE614(void)
{
    return MakyuriHeya_SceneTable;
}

/* The actors placed in each of the lighthouse rooms. */
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

void FieldScene_CallHelper67e8(void)
{
    Makyuri_TickSpawnTimer();
}

void FieldScene_Forward646c(void)
{
    SceneState_ClearCurrentRecordAndReleaseTarget();
}

void FieldScene_RunSingleStep(void)
{
    Makyuri_RunActorMove();
}

void FieldScene_CallHelper6364(void)
{
    MakyuriIriguchi_RaisePillar();
}

void SceneDialogue_RunLine1637(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered(MSG_STRANGE_FORCES_AT_WORK_SEEMS, 1);
    Engine_EventEnd();
}

/* What each lighthouse room answers; the fourth takes the table the other
   scenes take. */
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
