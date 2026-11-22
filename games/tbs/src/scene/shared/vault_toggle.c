/// @file
/// Vault's rope bridge, before and after the thieves are caught. A path runs
/// north and south under the bridge, and the 4-by-2 cells at (23, 26) are both
/// the path below and the bridge's deck above: CELL_BEHAVIOR_VAULT_BRIDGE_END
/// (vault.h) gives them the deck's ground, 32 pixels up, so the leader crosses
/// on the bridge, and CELL_BEHAVIOR_VAULT_WEST_OF_BRIDGE gives them back the
/// path's ground, so he walks under it. FLAG_VAULT_ON_BRIDGE remembers the
/// deck, which each scene restores on a kept-position return, such as after
/// battle.

/// The cells under Vault's rope bridge have the deck's ground, so the leader is
/// on the bridge; kept through a battle.
#define FLAG_VAULT_ON_BRIDGE FLAG_SCENE_TEMP_00

/// Nothing visible happens as Isaac steps onto either end of the rope bridge in
/// Vault's south-east (cells (22, 26) and (27, 26)). The game gives the cells
/// under the bridge the deck's ground, so that he walks across the bridge
/// rather than along the path below it.
///
/// The deck's ground is copied from a spare copy at cells (55, 26) to
/// (58, 27) of the map.
static void Vault_RaisePathToBridge(void) {
  GameFlag_Set(FLAG_VAULT_ON_BRIDGE);
  MapGrid_CopyCellAttributes(55, 26, 4, 2, 23, 26);
}

/// Nothing visible happens as Isaac steps off the west end of Vault's rope
/// bridge (cells (21, 25) to (21, 27)). The game gives the cells under the
/// bridge the path's ground again, so that the path south runs under it.
///
/// The ground is copied from the path's cells north of the bridge, and
/// FLAG_VAULT_ON_BRIDGE is cleared.
static void Vault_LowerPathUnderBridge(void) {
  GameFlag_Clear(FLAG_VAULT_ON_BRIDGE);
  MapGrid_CopyCellAttributes(23, 23, 4, 2, 23, 26);
}
