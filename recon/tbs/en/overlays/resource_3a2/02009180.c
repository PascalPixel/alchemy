/* Draft of resource_3a2 0x02009180 (Scene_GetEvents), from
 * games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/EVENT_SETUP_SCENE.C.
 * Remaining difference: the ROM loads scene 0x4a from the literal pool, as a
 * link-time scene symbol would; C builds the constant with movs. The listing
 * keeps these rows. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent YamaRama_TempleEvents[];
extern const struct SceneEvent YamaRama_Events[];

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == 0x4a) {
        return YamaRama_TempleEvents;
    }
    return YamaRama_Events;
}
