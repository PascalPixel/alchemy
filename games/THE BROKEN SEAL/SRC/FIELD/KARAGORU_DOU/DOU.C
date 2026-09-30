#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gKaragoruDouEntrances1[];
extern const struct SceneEntrance gKaragoruDouEntrances2[];
extern const struct SceneEntrance gKaragoruDouEntrances3[];
extern const struct SceneEntrance gKaragoruDouEntrancesOther[];

extern const u32 gKaragoruDouExits[];

extern const struct ScenePlacement gKaragoruDouPlacements1[];
extern const struct ScenePlacement gKaragoruDouPlacements1Flag96f[];
extern const struct ScenePlacement gKaragoruDouPlacements2[];
extern const struct ScenePlacement gKaragoruDouPlacements3[];
extern const struct ScenePlacement gKaragoruDouPlacementsOther[];
extern const struct SceneEvent gKaragoruDouEvents1[];
extern const struct SceneEvent gKaragoruDouEvents1Flag96f[];
extern const struct SceneEvent gKaragoruDouEvents2[];
extern const struct SceneEvent gKaragoruDouEvents3[];
extern const struct SceneEvent gKaragoruDouEventsOther[];

/* Where the party appears in each of the cave's three scenes. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KaragoruDou1) {
        return gKaragoruDouEntrances1;
    }
    if (scene == (s32)&SceneId_KaragoruDou2) {
        return gKaragoruDouEntrances2;
    }
    if (scene == (s32)&SceneId_KaragoruDou3) {
        return gKaragoruDouEntrances3;
    }
    return gKaragoruDouEntrancesOther;
}

/* The cave has no regions. */
const struct SceneRegion *Scene_GetRegions(void) { return 0; }

/* Every cave scene leaves through one exit table. */
const u32 *Scene_GetExits(void)
{
    return gKaragoruDouExits;
}

/* The actors placed in the cave; the first scene changes once flag 0x96f
   is set. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KaragoruDou1) {
        if (GameFlag_IsSet(0x96f) != 0) {
            return gKaragoruDouPlacements1Flag96f;
        }
        return gKaragoruDouPlacements1;
    }
    if (scene == (s32)&SceneId_KaragoruDou2) {
        return gKaragoruDouPlacements2;
    }
    if (scene == (s32)&SceneId_KaragoruDou3) {
        return gKaragoruDouPlacements3;
    }
    return gKaragoruDouPlacementsOther;
}

/* What the cave answers, chosen as its placements are. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KaragoruDou1) {
        if (GameFlag_IsSet(0x96f) != 0) {
            return gKaragoruDouEvents1Flag96f;
        }
        return gKaragoruDouEvents1;
    }
    if (scene == (s32)&SceneId_KaragoruDou2) {
        return gKaragoruDouEvents2;
    }
    if (scene == (s32)&SceneId_KaragoruDou3) {
        return gKaragoruDouEvents3;
    }
    return gKaragoruDouEventsOther;
}
