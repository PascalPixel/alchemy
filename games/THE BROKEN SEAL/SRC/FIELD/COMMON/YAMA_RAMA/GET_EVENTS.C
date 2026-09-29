#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent YamaRama_TempleEvents[];
extern const struct SceneEvent YamaRama_Events[];

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_YamaRama1) {
        return YamaRama_TempleEvents;
    }
    return YamaRama_Events;
}
