#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gImiruMuraEntrances2[];
extern const struct SceneEntrance gImiruMuraEntrancesOther[];

/* Where the party appears; the second area has its own entrances. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_ImiruMura2) {
        return gImiruMuraEntrances2;
    }
    return gImiruMuraEntrancesOther;
}
