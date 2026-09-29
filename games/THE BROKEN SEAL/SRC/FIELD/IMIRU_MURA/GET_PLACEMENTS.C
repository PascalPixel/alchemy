#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 gImiruMuraPlacements2[];
extern const struct ScenePlacement gImiruMuraPlacements[];
extern const struct ScenePlacement gImiruMuraPlacementsFlag881[];

void FieldScene_PrepareActors(u8 *placements);

/*
 * The actors placed in Imil. The second area's table is prepared, then
 * patched in place once flag 0x881 is set: the overlay image is writable.
 * The coordinates are written as shifts, which is how a 16.16 whole number
 * is built here. The word at +0x4c is set only on this path and never read
 * back, so its meaning is unverified.
 */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    u8 *script;

    if (gGameState.scene == ((s32)&SceneId_ImiruMura2)) {
        script = gImiruMuraPlacements2;
        FieldScene_PrepareActors(script);
        if (GameFlag_IsSet(0x881) != 0) {
            script[262] = 0;
            *(s32 *)(script + 0x50) = 182 << 16;
            *(s32 *)(script + 0x58) = 564 << 16;
            *(s32 *)(script + 0x4c) = 2;
        }
        return (const struct ScenePlacement *)script;
    }

    if (GameFlag_IsSet(0x881) != 0) {
        return gImiruMuraPlacementsFlag881;
    }
    return gImiruMuraPlacements;
}
