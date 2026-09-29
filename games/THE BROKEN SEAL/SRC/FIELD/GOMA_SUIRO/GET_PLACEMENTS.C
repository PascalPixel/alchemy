#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gGomaSuiroPlacements2[];
extern const struct ScenePlacement gGomaSuiroPlacementsOther[];

/* The actors placed; the second area has its own. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_GomaSuiro2) {
        return gGomaSuiroPlacements2;
    }
    return gGomaSuiroPlacementsOther;
}
