#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gMakyuriHeyaEvents1[];
extern const struct SceneEvent gMakyuriHeyaEvents2[];
extern const struct SceneEvent gMakyuriHeyaEvents3[];
extern const struct SceneEvent gMakyuriHeyaEventsOther[];

/* What each lighthouse room answers; the fourth takes the table the other
   scenes take. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MakyuriHeya1) {
        return gMakyuriHeyaEvents1;
    }
    if (selector == (s32)&SceneId_MakyuriHeya2) {
        return gMakyuriHeyaEvents2;
    }
    if (selector == (s32)&SceneId_MakyuriHeya3) {
        return gMakyuriHeyaEvents3;
    }
    return gMakyuriHeyaEventsOther;
}
