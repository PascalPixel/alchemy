#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gSuharaSabakuEntrances1[];
extern const struct SceneEntrance gSuharaSabakuEntrances2[];
extern const struct SceneEntrance gSuharaSabakuEntrances3[];
extern const struct SceneEntrance gSuharaSabakuEntrancesOther[];

/* Where the party appears in each of the desert's three areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_SuharaSabaku1) {
        return gSuharaSabakuEntrances1;
    }
    if (selector == (s32)&SceneId_SuharaSabaku2) {
        return gSuharaSabakuEntrances2;
    }
    if (selector == (s32)&SceneId_SuharaSabaku3) {
        return gSuharaSabakuEntrances3;
    }
    return gSuharaSabakuEntrancesOther;
}
