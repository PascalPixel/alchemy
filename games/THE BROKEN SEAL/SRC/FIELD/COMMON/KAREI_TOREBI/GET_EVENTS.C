#include "KAREI.H"

extern const struct SceneEvent gKareiTorebiEvents1[];
extern const struct SceneEvent gKareiTorebiEvents1Flag93e[];
extern const struct SceneEvent gKareiTorebiEvents2[];
extern const struct SceneEvent gKareiTorebiEvents2Flag93e[];
extern const struct SceneEvent gKareiTorebiEvents2Flag950[];
extern const struct SceneEvent gKareiTorebiEvents3[];
extern const struct SceneEvent gKareiTorebiEvents3Flag950[];
extern const struct SceneEvent gKareiTorebiEventsOther[];

/* What each scene answers, which changes as flags 0x93e and 0x950 are set. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiTorebi1) {
        if (GameFlag_IsSet(0x93e) != 0) {
            return gKareiTorebiEvents1Flag93e;
        }
        return gKareiTorebiEvents1;
    }

    if (scene == (s32)&SceneId_KareiTorebi3) {
        if (GameFlag_IsSet(0x950) != 0) {
            return gKareiTorebiEvents3Flag950;
        }
        return gKareiTorebiEvents3;
    }

    if (scene == (s32)&SceneId_KareiTorebi2) {
        if (GameFlag_IsSet(0x950) != 0) {
            return gKareiTorebiEvents2Flag950;
        }
        if (GameFlag_IsSet(0x93e) != 0) {
            return gKareiTorebiEvents2Flag93e;
        }
        return gKareiTorebiEvents2;
    }

    return gKareiTorebiEventsOther;
}
