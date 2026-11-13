/// @file
/// Vault, before and after the thieves are caught: the two villagers who walk
/// about together (ACTOR_BOY_ROUND_THE_WELL and ACTOR_GIRL_ROUND_THE_WELL,
/// as both scenes name them).

/// The update routine of the two children round the well. Their timer holds two
/// bits: bit 0 picks the partner (set for ACTOR_BOY_ROUND_THE_WELL, whose
/// partner is ACTOR_GIRL_ROUND_THE_WELL; clear for
/// ACTOR_GIRL_ROUND_THE_WELL) and bit 1 is set while the party talks to them.
/// A villager stops when close to and facing its partner, and otherwise when
/// the leader is within 18 pixels in front of it. While a talk is pending
/// (FieldWork's talk) or Mind Read is on the range is 26 pixels, and a villager
/// being talked to stops whichever way it faces.
static UNREAD_RESULT Vault_UpdateWalkingVillager(struct Actor *actor) {
  // The heap slots from the window renderer's on.
  u8 **slots = gWindowWork;
  u8 *windowWork = slots[0];
  struct FieldWork *work = FIELD_WORK_FROM(slots, HEAP_SLOT_WINDOW);
  s16 *flags = &actor->timer;
  s32 force = 0;
  s32 range = 18;
  struct Actor *partner;
  struct Actor *player;

  if ((*flags & 1) != 0) {
    partner = Actor_GetById(ACTOR_GIRL_ROUND_THE_WELL);
  } else {
    partner = Actor_GetById(ACTOR_BOY_ROUND_THE_WELL);
  }
  if (Vault_StopIfFacingNear(actor, partner, 32, 0) != 0) {
    RETURN_UNREAD(0);
  }

  player = Actor_GetById(ACTOR_PARTY_LEADER);

  if ((s16)work->talk != 0 || windowWork[WINDOW_WORK_MIND_READ_OFFSET] != 0) {
    range = 26;
    if ((*flags & 2) != 0) {
      force = 1;
    }
  }

  Vault_StopIfFacingNear(actor, player, range, force);
  RETURN_UNREAD(0);
}
