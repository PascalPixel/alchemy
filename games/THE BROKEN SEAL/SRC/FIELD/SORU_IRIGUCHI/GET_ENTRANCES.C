#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gSoruIriguchiEntrances2[];
extern const struct SceneEntrance gSoruIriguchiEntrances1[];
extern const struct SceneEntrance gSoruIriguchiEntrancesOther[];

/* Where the party appears in the scene it enters. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_SoruIriguchi2) {
        return gSoruIriguchiEntrances2;
    }
    if (scene == (s32)&SceneId_SoruIriguchi1) {
        return gSoruIriguchiEntrances1;
    }
    return gSoruIriguchiEntrancesOther;
}
