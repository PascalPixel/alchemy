#include "STATUS.H"

extern const struct SceneEntrance gKorashiamuIriguchiEntrances1[];
extern const struct SceneEntrance gKorashiamuIriguchiEntrances3[];
extern const struct SceneEntrance gKorashiamuIriguchiEntrancesOther[];

/* Where the party appears in the scene it enters. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KorashiamuIriguchi1) {
        return gKorashiamuIriguchiEntrances1;
    }
    if (scene == (s32)&SceneId_KorashiamuIriguchi3) {
        return gKorashiamuIriguchiEntrances3;
    }
    return gKorashiamuIriguchiEntrancesOther;
}
