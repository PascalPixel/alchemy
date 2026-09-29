#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gImiruFuchinEvents1[];
extern const struct SceneEvent gImiruFuchinEvents2[];
extern const struct SceneEvent gImiruFuchinEvents3[];
extern const struct SceneEvent gImiruFuchinEvents4[];
extern const struct SceneEvent gImiruFuchinEvents5[];
extern const struct SceneEvent gImiruFuchinEvents6[];
extern const struct SceneEvent gImiruFuchinEvents7[];
extern const struct SceneEvent gImiruFuchinEventsOther[];

/* What each area answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_ImiruFuchin1) {
        return gImiruFuchinEvents1;
    }
    if (v == (s32)&SceneId_ImiruFuchin2) {
        return gImiruFuchinEvents2;
    }
    if (v == (s32)&SceneId_ImiruFuchin3) {
        return gImiruFuchinEvents3;
    }
    if (v == (s32)&SceneId_ImiruFuchin4) {
        return gImiruFuchinEvents4;
    }
    if (v == (s32)&SceneId_ImiruFuchin5) {
        return gImiruFuchinEvents5;
    }
    if (v == (s32)&SceneId_ImiruFuchin6) {
        return gImiruFuchinEvents6;
    }
    if (v == (s32)&SceneId_ImiruFuchin7) {
        return gImiruFuchinEvents7;
    }
    return gImiruFuchinEventsOther;
}
