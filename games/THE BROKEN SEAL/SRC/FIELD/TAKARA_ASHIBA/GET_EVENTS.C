#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gTakaraAshibaEvents1[];
extern const struct SceneEvent gTakaraAshibaEvents2[];
extern const struct SceneEvent gTakaraAshibaEvents3[];
extern const struct SceneEvent gTakaraAshibaEventsOther[];

/* What each of the island's three platforms answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_TakaraAshiba1) {
        return gTakaraAshibaEvents1;
    }
    if (selector == (s32)&SceneId_TakaraAshiba2) {
        return gTakaraAshibaEvents2;
    }
    if (selector == (s32)&SceneId_TakaraAshiba3) {
        return gTakaraAshibaEvents3;
    }
    return gTakaraAshibaEventsOther;
}
