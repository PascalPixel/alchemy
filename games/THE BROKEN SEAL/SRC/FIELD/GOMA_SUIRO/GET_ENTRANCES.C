#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gGomaSuiroEntrances2[];
extern const struct SceneEntrance gGomaSuiroEntrancesOther[];

/* Where the party appears; the second area has its own entrances. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_GomaSuiro2) {
        return gGomaSuiroEntrances2;
    }
    return gGomaSuiroEntrancesOther;
}
