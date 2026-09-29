#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gTakaraAshibaEntrances1[];
extern const struct SceneEntrance gTakaraAshibaEntrances2[];
extern const struct SceneEntrance gTakaraAshibaEntrances3[];
extern const struct SceneEntrance gTakaraAshibaEntrancesOther[];

/* Where the party appears on each of the island's three platforms. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_TakaraAshiba1) {
        return gTakaraAshibaEntrances1;
    }
    if (selector == (s32)&SceneId_TakaraAshiba2) {
        return gTakaraAshibaEntrances2;
    }
    if (selector == (s32)&SceneId_TakaraAshiba3) {
        return gTakaraAshibaEntrances3;
    }
    return gTakaraAshibaEntrancesOther;
}
