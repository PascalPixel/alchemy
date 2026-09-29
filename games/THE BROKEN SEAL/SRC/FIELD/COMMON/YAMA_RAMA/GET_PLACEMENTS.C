#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement YamaRama_TemplePlacements[];
extern const struct ScenePlacement YamaRama_Placements[];

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_YamaRama1) {
        return YamaRama_TemplePlacements;
    }
    return YamaRama_Placements;
}
