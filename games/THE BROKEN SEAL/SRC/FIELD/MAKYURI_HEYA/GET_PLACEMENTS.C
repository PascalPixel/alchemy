#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gMakyuriHeyaPlacements1[];
extern const struct ScenePlacement gMakyuriHeyaPlacements2[];
extern const struct ScenePlacement gMakyuriHeyaPlacements3[];
extern const struct ScenePlacement gMakyuriHeyaPlacements4[];
extern const struct ScenePlacement gMakyuriHeyaPlacementsOther[];

/* The actors placed in each of the lighthouse rooms. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MakyuriHeya1) {
        return gMakyuriHeyaPlacements1;
    }
    if (selector == (s32)&SceneId_MakyuriHeya2) {
        return gMakyuriHeyaPlacements2;
    }
    if (selector == (s32)&SceneId_MakyuriHeya3) {
        return gMakyuriHeyaPlacements3;
    }
    if (selector == (s32)&SceneId_MakyuriHeya4) {
        return gMakyuriHeyaPlacements4;
    }
    return gMakyuriHeyaPlacementsOther;
}
