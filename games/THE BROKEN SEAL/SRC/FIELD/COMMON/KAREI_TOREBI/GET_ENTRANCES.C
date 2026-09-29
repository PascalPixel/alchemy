#include "KAREI.H"

extern const struct SceneEntrance gKareiTorebiEntrances1[];
extern const struct SceneEntrance gKareiTorebiEntrances2[];
extern const struct SceneEntrance gKareiTorebiEntrances3[];
extern const struct SceneEntrance gKareiTorebiEntrancesOther[];

/* Where the party appears in each of the road's three scenes. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiTorebi1) {
        return gKareiTorebiEntrances1;
    }
    if (scene == (s32)&SceneId_KareiTorebi3) {
        return gKareiTorebiEntrances3;
    }
    if (scene == (s32)&SceneId_KareiTorebi2) {
        return gKareiTorebiEntrances2;
    }
    return gKareiTorebiEntrancesOther;
}
