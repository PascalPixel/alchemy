// Vault, before and after the thieves are caught: the jail's door. The scene names the jail's exit,
// VAULT_JAIL_EXIT.

// Isaac opens the jail's grey iron door, in the cliff below the mayor's house (metatile (4, 11)),
// and walks into the jail, a stone room of metatiles behind bars.
//
// He vanishes into the doorway as he goes, as at the shops.
static void scVaultEnterJail(void)
{
    ACTOR *actor  = fdActorGet(ACTOR_PARTY_LEADER);
    SPR   *sprite = actor->sprite;

    sySoundPlay(SE_DOOR_OPEN);
    fdMapGridPlayMapEdit(VaultJailDoorMetatiles, 35, 9);
    fdMapGridCopyMetatileAttributes(33, 20, 1, 3, 4, 10);
    actor->priorityFlags   &= ~ACTOR_PRIORITY_AUTOMATIC;
    sprite->image.priority  = 3;
    scVaultWalkThroughExit(72, 160, VAULT_JAIL_EXIT);
}
