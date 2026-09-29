#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gTakaraShimaEvents1[];
extern const struct SceneEvent gTakaraShimaEvents2[];
extern const struct SceneEvent gTakaraShimaEvents3[];
extern const struct SceneEvent gTakaraShimaEvents4[];
extern const struct SceneEvent gTakaraShimaEvents5[];
extern const struct SceneEvent gTakaraShimaEventsOther[];

/* What the island answers, chosen as its entrances are. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_TakaraShima1) {
        return gTakaraShimaEvents1;
    }
    if (scene == (s32)&SceneId_TakaraShima2) {
        return gTakaraShimaEvents2;
    }
    if (scene == (s32)&SceneId_TakaraShima3) {
        return gTakaraShimaEvents3;
    }
    if (scene == (s32)&SceneId_TakaraShima4) {
        return gTakaraShimaEvents4;
    }
    if (scene == (s32)&SceneId_TakaraShima5) {
        return gTakaraShimaEvents5;
    }
    return gTakaraShimaEventsOther;
}
