#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gSuharaSabakuPlacements1[];
extern const struct ScenePlacement gSuharaSabakuPlacements2[];
extern const struct ScenePlacement gSuharaSabakuPlacements3[];
extern const struct ScenePlacement gSuharaSabakuPlacementsOther[];

/* The actors placed in each of the desert's three areas. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_SuharaSabaku1) {
        return gSuharaSabakuPlacements1;
    }
    if (selector == (s32)&SceneId_SuharaSabaku2) {
        return gSuharaSabakuPlacements2;
    }
    if (selector == (s32)&SceneId_SuharaSabaku3) {
        return gSuharaSabakuPlacements3;
    }
    return gSuharaSabakuPlacementsOther;
}
