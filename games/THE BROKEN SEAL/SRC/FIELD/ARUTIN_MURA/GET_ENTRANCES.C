#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gArutinMuraEntrances1[];
extern const struct SceneEntrance gArutinMuraEntrances2[];
extern const struct SceneEntrance gArutinMuraEntrancesOther[];

/* Where the party appears in each of Altin's two areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_ArutinMura1) {
        return gArutinMuraEntrances1;
    }
    if (v == (s32)&SceneId_ArutinMura2) {
        return gArutinMuraEntrances2;
    }
    return gArutinMuraEntrancesOther;
}
