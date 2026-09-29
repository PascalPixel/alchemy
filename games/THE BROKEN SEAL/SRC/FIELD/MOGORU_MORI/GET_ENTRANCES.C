#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gMogoruMoriEntrances1[];
extern const struct SceneEntrance gMogoruMoriEntrances2[];
extern const struct SceneEntrance gMogoruMoriEntrances3[];
extern const struct SceneEntrance gMogoruMoriEntrancesOther[];

/* Where the party appears in each of the forest's three areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MogoruMori1) {
        return gMogoruMoriEntrances1;
    }
    if (selector == (s32)&SceneId_MogoruMori2) {
        return gMogoruMoriEntrances2;
    }
    if (selector == (s32)&SceneId_MogoruMori3) {
        return gMogoruMoriEntrances3;
    }
    return gMogoruMoriEntrancesOther;
}
