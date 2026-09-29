#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gKaragoruDouEntrances1[];
extern const struct SceneEntrance gKaragoruDouEntrances2[];
extern const struct SceneEntrance gKaragoruDouEntrances3[];
extern const struct SceneEntrance gKaragoruDouEntrancesOther[];

/* Where the party appears in each of the cave's three scenes. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KaragoruDou1) {
        return gKaragoruDouEntrances1;
    }
    if (scene == (s32)&SceneId_KaragoruDou2) {
        return gKaragoruDouEntrances2;
    }
    if (scene == (s32)&SceneId_KaragoruDou3) {
        return gKaragoruDouEntrances3;
    }
    return gKaragoruDouEntrancesOther;
}
