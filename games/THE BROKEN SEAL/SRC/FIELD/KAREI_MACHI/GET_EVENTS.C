#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gKareiMachiEvents1[];
extern const struct SceneEvent gKareiMachiEvents2[];
extern const struct SceneEvent gKareiMachiEvents3[];
extern const struct SceneEvent gKareiMachiEvents4[];
extern const struct SceneEvent gKareiMachiEvents5[];
extern const struct SceneEvent gKareiMachiEvents6[];
extern const struct SceneEvent gKareiMachiEventsOther[];

/* What the scene answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiMachi1) {
        return gKareiMachiEvents1;
    }
    if (scene == (s32)&SceneId_KareiMachi2) {
        return gKareiMachiEvents2;
    }
    if (scene == (s32)&SceneId_KareiMachi3) {
        return gKareiMachiEvents3;
    }
    if (scene == (s32)&SceneId_KareiMachi4) {
        return gKareiMachiEvents4;
    }
    if (scene == (s32)&SceneId_KareiMachi5) {
        return gKareiMachiEvents5;
    }
    if (scene == (s32)&SceneId_KareiMachi6) {
        return gKareiMachiEvents6;
    }
    return gKareiMachiEventsOther;
}
