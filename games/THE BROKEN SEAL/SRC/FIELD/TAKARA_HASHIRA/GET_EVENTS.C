#include "HASHIRA.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gTakaraHashiraEvents1[];
extern const struct SceneEvent gTakaraHashiraEvents2[];
extern const struct SceneEvent gTakaraHashiraEvents3[];
extern const struct SceneEvent gTakaraHashiraEvents4[];
extern const struct SceneEvent gTakaraHashiraEvents5[];
extern const struct SceneEvent gTakaraHashiraEventsOther[];

/* What each of the pillar rooms answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_TakaraHashira1) {
        return gTakaraHashiraEvents1;
    }
    if (scene == (s32)&SceneId_TakaraHashira2) {
        return gTakaraHashiraEvents2;
    }
    if (scene == (s32)&SceneId_TakaraHashira3) {
        return gTakaraHashiraEvents3;
    }
    if (scene == (s32)&SceneId_TakaraHashira4) {
        return gTakaraHashiraEvents4;
    }
    if (scene == (s32)&SceneId_TakaraHashira5) {
        return gTakaraHashiraEvents5;
    }
    return gTakaraHashiraEventsOther;
}
