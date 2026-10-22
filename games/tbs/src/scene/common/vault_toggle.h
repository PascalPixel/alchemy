/// @file
/// Vault's rope bridge, before and after the thieves are caught. A path runs
/// north and south under the bridge, and the 4-by-2 cells at (23, 26) are both
/// the path below and the bridge's deck above: VAULT_CELL_BRIDGE_END (vault.h)
/// gives them the deck's ground, 32 pixels up, so the leader crosses on the
/// bridge, and VAULT_CELL_WEST_OF_BRIDGE gives them back the path's ground, so
/// he walks under it (render probe vault-bridge: crossing either way he stays
/// 32 pixels up, and walking the path he stays on the ground).
/// FLAG_SCENE_TEMP_00 remembers the deck, which each scene restores on a
/// kept-position return, such as after battle.
#include "types.h"
#include "field_obj.h"
#include "field_event.h"
#include "flags.h"

/// Gives the cells under the bridge the deck's ground, from a copy kept
/// elsewhere in the map. VAULT_CELL_BRIDGE_END reaches it; it sets
/// FLAG_SCENE_TEMP_00, keeping the deck across a kept-position return.
static void Vault_RaisePathToBridge(void) {
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
  Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
}

/// Gives the cells under the bridge the path's ground again, from the path's
/// cells north of the bridge, and clears FLAG_SCENE_TEMP_00.
/// VAULT_CELL_WEST_OF_BRIDGE reaches it.
static void Vault_LowerPathUnderBridge(void) {
  Engine_GameFlagClear(FLAG_SCENE_TEMP_00);
  Map_CopyCellAttributes(23, 23, 4, 2, 23, 26);
}
