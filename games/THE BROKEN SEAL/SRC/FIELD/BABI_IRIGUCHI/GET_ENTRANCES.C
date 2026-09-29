#include "IRIGUCHI.H"

extern const struct SceneEntrance gBabiIriguchiEntrances3[];
extern const struct SceneEntrance gBabiIriguchiEntrances2[];
extern const struct SceneEntrance gBabiIriguchiEntrances1[];
extern const struct SceneEntrance gBabiIriguchiEntrancesOther[];
extern const struct SceneRegion gBabiIriguchiRegions3[];

/* Where the party appears in the scene it enters. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_BabiIriguchi3) {
        return gBabiIriguchiEntrances3;
    }
    if (scene == (s32)&SceneId_BabiIriguchi2) {
        return gBabiIriguchiEntrances2;
    }
    if (scene == (s32)&SceneId_BabiIriguchi1) {
        return gBabiIriguchiEntrances1;
    }
    return gBabiIriguchiEntrancesOther;
}

/* Only the third scene has regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    if (gGameState.scene == (s32)&SceneId_BabiIriguchi3) {
        return gBabiIriguchiRegions3;
    }
    return 0;
}
