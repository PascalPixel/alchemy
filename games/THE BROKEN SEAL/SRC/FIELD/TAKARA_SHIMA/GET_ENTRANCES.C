#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gTakaraShimaEntrances1[];
extern const struct SceneEntrance gTakaraShimaEntrances2[];
extern const struct SceneEntrance gTakaraShimaEntrances3[];
extern const struct SceneEntrance gTakaraShimaEntrances4[];
extern const struct SceneEntrance gTakaraShimaEntrances5[];
extern const struct SceneEntrance gTakaraShimaEntrancesOther[];

/* Where the party appears on the island: its first five scenes have their
   own entrances, every other scene shares one table. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_TakaraShima1) {
        return gTakaraShimaEntrances1;
    }
    if (scene == (s32)&SceneId_TakaraShima2) {
        return gTakaraShimaEntrances2;
    }
    if (scene == (s32)&SceneId_TakaraShima3) {
        return gTakaraShimaEntrances3;
    }
    if (scene == (s32)&SceneId_TakaraShima4) {
        return gTakaraShimaEntrances4;
    }
    if (scene == (s32)&SceneId_TakaraShima5) {
        return gTakaraShimaEntrances5;
    }
    return gTakaraShimaEntrancesOther;
}
