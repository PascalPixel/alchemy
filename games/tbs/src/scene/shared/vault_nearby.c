/// @file
/// Vault, before and after the thieves are caught: the two villagers who walk
/// about together (ACTOR_BOY_ROUND_THE_WELL and ACTOR_GIRL_ROUND_THE_WELL,
/// as both scenes name them).

/// The bits of a child round the well's timer.
enum {
  /// Set for ACTOR_BOY_ROUND_THE_WELL, whose partner is
  /// ACTOR_GIRL_ROUND_THE_WELL; clear for the girl.
  VILLAGER_IS_BOY = 1,
  /// Set while the party talks to it, so that it stops whichever way it faces.
  VILLAGER_TALKED_TO = 2
};

/// The update routine of the two children round the well, by their timer's
/// VILLAGER_IS_BOY and VILLAGER_TALKED_TO. A villager stops when close to and
/// facing its partner, and otherwise when the leader is within STOP_RANGE in
/// front of it. While a talk is pending (FieldState's talk) or Mind Read is on
/// the range is TALK_STOP_RANGE, and a villager being talked to stops whichever
/// way it faces.
static UNREAD_RESULT Vault_UpdateWalkingVillager(struct Actor *actor) {
  u8 *windowState = gWindowState[0];
  struct FieldState *state = gFieldState;
  s32 force = 0;
  s32 range = STOP_RANGE;
  struct Actor *partner;
  struct Actor *player;

  if ((actor->timer & VILLAGER_IS_BOY) != 0) {
    partner = Actor_GetById(ACTOR_GIRL_ROUND_THE_WELL);
  } else {
    partner = Actor_GetById(ACTOR_BOY_ROUND_THE_WELL);
  }
  if (Vault_StopIfFacingNear(actor, partner, PARTNER_STOP_RANGE, 0) != 0) {
    RETURN_UNREAD(0);
  }

  player = Actor_GetById(ACTOR_PARTY_LEADER);

  if ((s16)state->talk != 0 ||
      windowState[WINDOW_STATE_MIND_READ_OFFSET] != 0) {
    range = TALK_STOP_RANGE;
    if ((actor->timer & VILLAGER_TALKED_TO) != 0) {
      force = 1;
    }
  }

  Vault_StopIfFacingNear(actor, player, range, force);
  RETURN_UNREAD(0);
}
