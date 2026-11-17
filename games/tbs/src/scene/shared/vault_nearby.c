/// @file
/// Vault, before and after the thieves are caught: the two villagers who walk
/// about together (ACTOR_BOY_ROUND_THE_WELL and ACTOR_GIRL_ROUND_THE_WELL,
/// as both scenes name them).

/// How near in front of it, in pixels, a villager stops for its partner, and
/// for the leader: STOP_RANGE, or TALK_STOP_RANGE while a talk is pending or
/// Mind Read is on.
#define PARTNER_STOP_RANGE 32
#define STOP_RANGE 18
#define TALK_STOP_RANGE 26

/// The update routine of the two children round the well. Their timer holds two
/// bits: bit 0 picks the partner (set for ACTOR_BOY_ROUND_THE_WELL, whose
/// partner is ACTOR_GIRL_ROUND_THE_WELL; clear for
/// ACTOR_GIRL_ROUND_THE_WELL) and bit 1 is set while the party talks to them.
/// A villager stops when close to and facing its partner, and otherwise when
/// the leader is within STOP_RANGE in front of it. While a talk is pending
/// (FieldWork's talk) or Mind Read is on the range is TALK_STOP_RANGE, and a
/// villager being talked to stops whichever way it faces.
static UNREAD_RESULT Vault_UpdateWalkingVillager(struct Actor *actor) {
  u8 *windowWork = gWindowWork[0];
  struct FieldWork *work = gFieldWork;
  s32 force = 0;
  s32 range = STOP_RANGE;
  struct Actor *partner;
  struct Actor *player;

  if ((actor->timer & 1) != 0) {
    partner = Actor_GetById(ACTOR_GIRL_ROUND_THE_WELL);
  } else {
    partner = Actor_GetById(ACTOR_BOY_ROUND_THE_WELL);
  }
  if (Vault_StopIfFacingNear(actor, partner, PARTNER_STOP_RANGE, 0) != 0) {
    RETURN_UNREAD(0);
  }

  player = Actor_GetById(ACTOR_PARTY_LEADER);

  if ((s16)work->talk != 0 || windowWork[WINDOW_WORK_MIND_READ_OFFSET] != 0) {
    range = TALK_STOP_RANGE;
    if ((actor->timer & 2) != 0) {
      force = 1;
    }
  }

  Vault_StopIfFacingNear(actor, player, range, force);
  RETURN_UNREAD(0);
}
