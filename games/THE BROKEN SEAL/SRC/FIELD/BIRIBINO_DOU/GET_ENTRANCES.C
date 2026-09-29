#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gBiribinoDouEntrances3[];
extern const struct SceneEntrance gBiribinoDouEntrances2[];
extern const struct SceneEntrance gBiribinoDouEntrances1[];
extern const struct SceneEntrance gBiribinoDouEntrancesOther[];

/* Where the party appears in each of the cave's three areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_BiribinoDou3) {
        return gBiribinoDouEntrances3;
    }
    if (selector == (s32)&SceneId_BiribinoDou2) {
        return gBiribinoDouEntrances2;
    }
    if (selector == (s32)&SceneId_BiribinoDou1) {
        return gBiribinoDouEntrances1;
    }
    return gBiribinoDouEntrancesOther;
}
