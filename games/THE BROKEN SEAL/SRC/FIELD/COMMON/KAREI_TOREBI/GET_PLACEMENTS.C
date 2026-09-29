#include "KAREI.H"

extern const struct ScenePlacement gKareiTorebiPlacements1[];
extern const struct ScenePlacement gKareiTorebiPlacements1Flag93e[];
extern const struct ScenePlacement gKareiTorebiPlacements2[];
extern const struct ScenePlacement gKareiTorebiPlacements2Flag93e[];
extern const struct ScenePlacement gKareiTorebiPlacements2Flag950[];
extern const struct ScenePlacement gKareiTorebiPlacements3[];
extern const struct ScenePlacement gKareiTorebiPlacements3Flag950[];
extern const struct ScenePlacement gKareiTorebiPlacementsOther[];

/* The actors placed in each scene, which change as flags 0x93e and 0x950
   are set. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 room = gGameState.scene;

    if (room == (s32)&SceneId_KareiTorebi1) {
        if (GameFlag_IsSet(0x93e) != 0) {
            return gKareiTorebiPlacements1Flag93e;
        }
        return gKareiTorebiPlacements1;
    }

    if (room == (s32)&SceneId_KareiTorebi3) {
        if (GameFlag_IsSet(0x950) != 0) {
            return gKareiTorebiPlacements3Flag950;
        }
        return gKareiTorebiPlacements3;
    }

    if (room == (s32)&SceneId_KareiTorebi2) {
        if (GameFlag_IsSet(0x950) != 0) {
            return gKareiTorebiPlacements2Flag950;
        }
        if (GameFlag_IsSet(0x93e) != 0) {
            return gKareiTorebiPlacements2Flag93e;
        }
        return gKareiTorebiPlacements2;
    }

    return gKareiTorebiPlacementsOther;
}
