#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gVinasuHeyaEntrances1[];
extern const struct SceneEntrance gVinasuHeyaEntrances3[];
extern const struct SceneEntrance gVinasuHeyaEntrances4[];
extern const struct SceneEntrance gVinasuHeyaEntrances5[];
extern const struct SceneEntrance gVinasuHeyaEntrances6[];
extern const struct SceneEntrance gVinasuHeyaEntrancesOther[];

/* Where the party appears in each of the rooms; the second takes the table the other scenes take. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_VinasuHeya1) {
        return gVinasuHeyaEntrances1;
    }
    if (v == (s32)&SceneId_VinasuHeya3) {
        return gVinasuHeyaEntrances3;
    }
    if (v == (s32)&SceneId_VinasuHeya4) {
        return gVinasuHeyaEntrances4;
    }
    if (v == (s32)&SceneId_VinasuHeya5) {
        return gVinasuHeyaEntrances5;
    }
    if (v == (s32)&SceneId_VinasuHeya6) {
        return gVinasuHeyaEntrances6;
    }
    return gVinasuHeyaEntrancesOther;
}
