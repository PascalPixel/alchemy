#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gKareiHeyaEntrances1[];
extern const struct SceneEntrance gKareiHeyaEntrances2[];
extern const struct SceneEntrance gKareiHeyaEntrancesOther[];

/* Where the party appears in the scene it enters. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiHeya1) {
        return gKareiHeyaEntrances1;
    }
    if (scene == (s32)&SceneId_KareiHeya2) {
        return gKareiHeyaEntrances2;
    }
    return gKareiHeyaEntrancesOther;
}
