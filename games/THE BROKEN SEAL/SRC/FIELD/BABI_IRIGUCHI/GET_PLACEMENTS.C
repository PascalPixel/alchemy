#include "IRIGUCHI.H"

extern const struct ScenePlacement gBabiIriguchiPlacements3[];
extern const struct ScenePlacement gBabiIriguchiPlacements2[];
extern const struct ScenePlacement gBabiIriguchiPlacements1[];
extern const struct ScenePlacement gBabiIriguchiPlacementsOther[];

/* The actors placed in each of the entrance's scenes. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_BabiIriguchi3) {
        return gBabiIriguchiPlacements3;
    }
    if (scene == (s32)&SceneId_BabiIriguchi2) {
        return gBabiIriguchiPlacements2;
    }
    if (scene == (s32)&SceneId_BabiIriguchi1) {
        return gBabiIriguchiPlacements1;
    }
    return gBabiIriguchiPlacementsOther;
}
