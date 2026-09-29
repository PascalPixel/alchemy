#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gRamakanSabakuEntrances1[];
extern const struct SceneEntrance gRamakanSabakuEntrances2[];
extern const struct SceneEntrance gRamakanSabakuEntrances3[];
extern const struct SceneEntrance gRamakanSabakuEntrances4[];
extern const struct SceneEntrance gRamakanSabakuEntrancesOther[];

/* Where the party appears in each of the desert's four areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_RamakanSabaku1) {
        return gRamakanSabakuEntrances1;
    }
    if (v == (s32)&SceneId_RamakanSabaku2) {
        return gRamakanSabakuEntrances2;
    }
    if (v == (s32)&SceneId_RamakanSabaku3) {
        return gRamakanSabakuEntrances3;
    }
    if (v == (s32)&SceneId_RamakanSabaku4) {
        return gRamakanSabakuEntrances4;
    }
    return gRamakanSabakuEntrancesOther;
}
