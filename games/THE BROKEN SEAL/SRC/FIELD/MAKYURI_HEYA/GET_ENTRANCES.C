#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gMakyuriHeyaEntrances4[];
extern const struct SceneEntrance gMakyuriHeyaEntrances3[];
extern const struct SceneEntrance gMakyuriHeyaEntrances2[];
extern const struct SceneEntrance gMakyuriHeyaEntrancesOther[];

/* Where the party appears in each of the lighthouse rooms; the first takes
   the table the other scenes take. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MakyuriHeya4) {
        return gMakyuriHeyaEntrances4;
    }
    if (selector == (s32)&SceneId_MakyuriHeya3) {
        return gMakyuriHeyaEntrances3;
    }
    if (selector == (s32)&SceneId_MakyuriHeya2) {
        return gMakyuriHeyaEntrances2;
    }
    return gMakyuriHeyaEntrancesOther;
}
