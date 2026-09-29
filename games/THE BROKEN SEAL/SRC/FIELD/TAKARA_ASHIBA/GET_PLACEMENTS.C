#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gTakaraAshibaPlacements1[];
extern const struct ScenePlacement gTakaraAshibaPlacements2[];
extern const struct ScenePlacement gTakaraAshibaPlacements3[];
extern const struct ScenePlacement gTakaraAshibaPlacementsOther[];

/* The actors placed on each of the island's three platforms. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_TakaraAshiba1) {
        return gTakaraAshibaPlacements1;
    }
    if (selector == (s32)&SceneId_TakaraAshiba2) {
        return gTakaraAshibaPlacements2;
    }
    if (selector == (s32)&SceneId_TakaraAshiba3) {
        return gTakaraAshibaPlacements3;
    }
    return gTakaraAshibaPlacementsOther;
}
