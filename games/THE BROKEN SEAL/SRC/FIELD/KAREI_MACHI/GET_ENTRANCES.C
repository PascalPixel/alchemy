#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gKareiMachiEntrances1[];
extern const struct SceneEntrance gKareiMachiEntrances2[];
extern const struct SceneEntrance gKareiMachiEntrances3[];
extern const struct SceneEntrance gKareiMachiEntrances4[];
extern const struct SceneEntrance gKareiMachiEntrances5[];
extern const struct SceneEntrance gKareiMachiEntrances6[];
extern const struct SceneEntrance gKareiMachiEntrancesOther[];

/* Where the party appears in the scene it enters. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiMachi1) {
        return gKareiMachiEntrances1;
    }
    if (scene == (s32)&SceneId_KareiMachi2) {
        return gKareiMachiEntrances2;
    }
    if (scene == (s32)&SceneId_KareiMachi3) {
        return gKareiMachiEntrances3;
    }
    if (scene == (s32)&SceneId_KareiMachi4) {
        return gKareiMachiEntrances4;
    }
    if (scene == (s32)&SceneId_KareiMachi5) {
        return gKareiMachiEntrances5;
    }
    if (scene == (s32)&SceneId_KareiMachi6) {
        return gKareiMachiEntrances6;
    }
    return gKareiMachiEntrancesOther;
}
