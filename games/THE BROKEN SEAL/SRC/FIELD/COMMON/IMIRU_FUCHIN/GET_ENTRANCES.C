#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gImiruFuchinEntrances1[];
extern const struct SceneEntrance gImiruFuchinEntrances2[];
extern const struct SceneEntrance gImiruFuchinEntrances3[];
extern const struct SceneEntrance gImiruFuchinEntrances4[];
extern const struct SceneEntrance gImiruFuchinEntrances5[];
extern const struct SceneEntrance gImiruFuchinEntrances6[];
extern const struct SceneEntrance gImiruFuchinEntrances7[];
extern const struct SceneEntrance gImiruFuchinEntrancesOther[];

/* Where the party appears in each of the seven areas around Imil. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_ImiruFuchin1) {
        return gImiruFuchinEntrances1;
    }
    if (v == (s32)&SceneId_ImiruFuchin2) {
        return gImiruFuchinEntrances2;
    }
    if (v == (s32)&SceneId_ImiruFuchin3) {
        return gImiruFuchinEntrances3;
    }
    if (v == (s32)&SceneId_ImiruFuchin4) {
        return gImiruFuchinEntrances4;
    }
    if (v == (s32)&SceneId_ImiruFuchin5) {
        return gImiruFuchinEntrances5;
    }
    if (v == (s32)&SceneId_ImiruFuchin6) {
        return gImiruFuchinEntrances6;
    }
    if (v == (s32)&SceneId_ImiruFuchin7) {
        return gImiruFuchinEntrances7;
    }
    return gImiruFuchinEntrancesOther;
}
