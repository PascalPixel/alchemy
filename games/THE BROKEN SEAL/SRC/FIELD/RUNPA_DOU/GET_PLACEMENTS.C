#include "CAVE.H"

/* The cave's actors stand only in its own scene. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_RunpaDou) {
        return gCavePlacements;
    }
    return gCaveNoPlacements;
}
