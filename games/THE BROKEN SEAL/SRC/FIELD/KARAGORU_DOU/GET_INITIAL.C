#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const u32 gKaragoruDouExits[];

/* The cave has no regions. */
const struct SceneRegion *Scene_GetRegions(void) { return 0; }

/* Every cave scene leaves through one exit table. */
const u32 *Scene_GetExits(void)
{
    return gKaragoruDouExits;
}
