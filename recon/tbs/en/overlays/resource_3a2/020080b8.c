/* Draft of resource_3a2 0x020080b8 (Scene_GetPlacements), from
 * games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/EVENT_SETUP_SCENE.C.
 * Remaining difference: the ROM loads scene 0x4a from the literal pool, as a
 * link-time scene symbol would; C builds the constant with movs. The listing
 * keeps these rows. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement YamaRama_TemplePlacements[];
extern const struct ScenePlacement YamaRama_Placements[];

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == 0x4a) {
        return YamaRama_TemplePlacements;
    }
    return YamaRama_Placements;
}
