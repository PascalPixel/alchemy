#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance YamaRama_TempleEntrances[];
extern const struct SceneEntrance YamaRama_Entrances[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_YamaRama1) {
        return YamaRama_TempleEntrances;
    }
    return YamaRama_Entrances;
}
