#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gBiribinoDouPlacements3[];
extern const struct ScenePlacement gBiribinoDouPlacements2[];
extern const struct ScenePlacement gBiribinoDouPlacements1[];
extern const struct ScenePlacement gBiribinoDouPlacementsOther[];
extern const struct SceneEvent gBiribinoDouEvents3[];
extern const struct SceneEvent gBiribinoDouEvents2[];
extern const struct SceneEvent gBiribinoDouEvents1[];
extern const struct SceneEvent gBiribinoDouEventsOther[];

/* The actors placed in each area. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_BiribinoDou3) {
        return gBiribinoDouPlacements3;
    }
    if (selector == (s32)&SceneId_BiribinoDou2) {
        return gBiribinoDouPlacements2;
    }
    if (selector == (s32)&SceneId_BiribinoDou1) {
        return gBiribinoDouPlacements1;
    }
    return gBiribinoDouPlacementsOther;
}

/* What each area answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_BiribinoDou3) {
        return gBiribinoDouEvents3;
    }
    if (selector == (s32)&SceneId_BiribinoDou2) {
        return gBiribinoDouEvents2;
    }
    if (selector == (s32)&SceneId_BiribinoDou1) {
        return gBiribinoDouEvents1;
    }
    return gBiribinoDouEventsOther;
}
