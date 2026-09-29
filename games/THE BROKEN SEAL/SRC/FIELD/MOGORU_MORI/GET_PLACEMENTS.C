#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gMogoruMoriPlacements1[];
extern const struct ScenePlacement gMogoruMoriPlacements2[];
extern const struct ScenePlacement gMogoruMoriPlacements3[];
extern const struct ScenePlacement gMogoruMoriPlacementsOther[];

/* The actors placed in each area. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MogoruMori1) {
        return gMogoruMoriPlacements1;
    }
    if (selector == (s32)&SceneId_MogoruMori2) {
        return gMogoruMoriPlacements2;
    }
    if (selector == (s32)&SceneId_MogoruMori3) {
        return gMogoruMoriPlacements3;
    }
    return gMogoruMoriPlacementsOther;
}
