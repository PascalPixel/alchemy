/// @file
/// Vault's rope bridge, before and after the thieves are caught. A path runs
/// north and south under the bridge, and the 4-by-2 cells at (23, 26) are both
/// the path below and the bridge's deck above: CELL_BEHAVIOR_VAULT_BRIDGE_END
/// (vault.h) gives them the deck's ground, 32 pixels up, so the leader crosses
/// on the bridge, and CELL_BEHAVIOR_VAULT_WEST_OF_BRIDGE gives them back the
/// path's ground, so he walks under it (render probe vault-bridge: crossing
/// either way he stays 32 pixels up, and walking the path he stays on the
/// ground). FLAG_VAULT_ON_BRIDGE remembers the deck, which each scene restores
/// on a kept-position return, such as after battle.
#include "types.h"
#include "actor.h"
#include "event.h"
#include "flags.h"

/// The cells under Vault's rope bridge have the deck's ground, so the leader is
/// on the bridge; kept through a battle.
#define FLAG_VAULT_ON_BRIDGE FLAG_SCENE_TEMP_00

/// Gives the cells under the bridge the deck's ground, from a copy kept
/// elsewhere in the map. CELL_BEHAVIOR_VAULT_BRIDGE_END reaches it; it sets
/// FLAG_VAULT_ON_BRIDGE, keeping the deck across a kept-position return.
static void Vault_RaisePathToBridge(void) {
  GameFlag_Set(FLAG_VAULT_ON_BRIDGE);
  MapGrid_CopyCellAttributes(55, 26, 4, 2, 23, 26);
}

/// Gives the cells under the bridge the path's ground again, from the path's
/// cells north of the bridge, and clears FLAG_VAULT_ON_BRIDGE.
/// CELL_BEHAVIOR_VAULT_WEST_OF_BRIDGE reaches it.
static void Vault_LowerPathUnderBridge(void) {
  GameFlag_Clear(FLAG_VAULT_ON_BRIDGE);
  MapGrid_CopyCellAttributes(23, 23, 4, 2, 23, 26);
}
