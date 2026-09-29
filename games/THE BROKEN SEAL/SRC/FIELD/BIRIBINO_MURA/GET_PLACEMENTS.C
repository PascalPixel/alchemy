#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gBiribinoMuraPlacements1[];
extern const struct ScenePlacement gBiribinoMuraPlacements2[];
extern const struct ScenePlacement gBiribinoMuraPlacements3[];
extern const struct ScenePlacement gBiribinoMuraPlacementsOther[];

/* The actors placed in each of Bilibin's three scenes. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_BiribinoMura1) {
        return gBiribinoMuraPlacements1;
    }
    if (scene == (s32)&SceneId_BiribinoMura3) {
        return gBiribinoMuraPlacements3;
    }
    if (scene == (s32)&SceneId_BiribinoMura2) {
        return gBiribinoMuraPlacements2;
    }
    return gBiribinoMuraPlacementsOther;
}
