#include "HASHIRA.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gTakaraHashiraEntrances1[];
extern const struct SceneEntrance gTakaraHashiraEntrances2[];
extern const struct SceneEntrance gTakaraHashiraEntrances3[];
extern const struct SceneEntrance gTakaraHashiraEntrances4[];
extern const struct SceneEntrance gTakaraHashiraEntrances5[];
extern const struct SceneEntrance gTakaraHashiraEntrancesOther[];

/* Where the party appears in each of the pillar rooms. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_TakaraHashira1) {
        return gTakaraHashiraEntrances1;
    }
    if (scene == (s32)&SceneId_TakaraHashira2) {
        return gTakaraHashiraEntrances2;
    }
    if (scene == (s32)&SceneId_TakaraHashira3) {
        return gTakaraHashiraEntrances3;
    }
    if (scene == (s32)&SceneId_TakaraHashira4) {
        return gTakaraHashiraEntrances4;
    }
    if (scene == (s32)&SceneId_TakaraHashira5) {
        return gTakaraHashiraEntrances5;
    }
    return gTakaraHashiraEntrancesOther;
}
