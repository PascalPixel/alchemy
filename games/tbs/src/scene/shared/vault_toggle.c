// Vault's rope bridge, before and after the thieves are caught. A path runs north and south under
// the bridge, and the 4-by-2 metatiles at (23, 26) are both the path below and the bridge's deck
// above: METATILE_BEHAVIOR_VAULT_BRIDGE_END (vault.h) gives them the deck's ground, 32 pixels up,
// so the leader crosses on the bridge, and METATILE_BEHAVIOR_VAULT_WEST_OF_BRIDGE gives them back
// the path's ground, so he walks under it. FLAG_VAULT_ON_BRIDGE remembers the deck, which each
// scene restores on a kept-position return, such as after battle.

// The metatiles under Vault's rope bridge have the deck's ground, so the leader is on the bridge;
// kept through a battle.
#define FLAG_VAULT_ON_BRIDGE FLAG_SCENE_TEMP_00

// Isaac steps onto either end of the rope bridge (metatiles (22, 26) and (27, 26)): the metatiles
// under it take the deck's ground, copied from a spare at metatiles (55, 26) to (58, 27), so that
// he walks across the bridge.
static void scVaultRaisePathToBridge(void)
{
    gmFlagSet(FLAG_VAULT_ON_BRIDGE);
    fdMapGridCopyMetatileAttributes(55, 26, 4, 2, 23, 26);
}

// Isaac steps off the west end of the rope bridge (metatiles (21, 25) to (21, 27)): the metatiles
// under it take the path's ground again, copied from the path north of the bridge, so that the path
// south runs under it.
static void scVaultLowerPathUnderBridge(void)
{
    gmFlagClear(FLAG_VAULT_ON_BRIDGE);
    fdMapGridCopyMetatileAttributes(23, 23, 4, 2, 23, 26);
}
