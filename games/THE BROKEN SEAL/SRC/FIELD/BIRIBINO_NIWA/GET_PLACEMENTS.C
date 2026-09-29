#include "NIWA.H"

extern u8 gBiribinoNiwaPlacements[];
extern const struct ScenePlacement gBiribinoNiwaPlacementsOther[];

/* The actors placed in the garden, patched in place by flags 0x84f and
   0x845: the overlay image is writable. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_BiribinoNiwa) {
        if (GameFlag_IsSet(0x84f) != 0)
            gBiribinoNiwaPlacements[118] = 1;
        if (GameFlag_IsSet(0x845) != 0)
            gBiribinoNiwaPlacements[70] = 0;
        return (const struct ScenePlacement *)gBiribinoNiwaPlacements;
    }
    return gBiribinoNiwaPlacementsOther;
}
