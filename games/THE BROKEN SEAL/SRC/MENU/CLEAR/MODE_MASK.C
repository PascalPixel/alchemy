/* The clear screen's mode mask: once flag 324 is set, every scene but the
 * world map answers all bits, unless the game state's word at 0x23e is 2.
 * The mask is the sign of the scene's difference from the world map. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

s32 Scene_GetModeMask(void)
{
    s32 mode;

    if (Engine_GameFlagIsSet(324) == 0) {
        return 0;
    }
    if (gGameState.unknown_23e == 2) {
        return 0;
    }
    mode = gGameState.scene ^ (s32)&SceneId_WorldMap;
    return -(s32)((u32)(-mode | mode) >> 31);
}
