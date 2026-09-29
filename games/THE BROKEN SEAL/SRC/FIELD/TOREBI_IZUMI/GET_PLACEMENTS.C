#include "TOPIC.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gTorebiIzumiPlacements2[];
extern const struct ScenePlacement gTorebiIzumiPlacementsOther[];

/* The actors placed at the spring; its second row places its own. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_TorebiIzumi2) {
        return gTorebiIzumiPlacements2;
    }
    return gTorebiIzumiPlacementsOther;
}
