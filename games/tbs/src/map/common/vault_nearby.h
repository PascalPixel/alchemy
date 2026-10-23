/// @file
/// Vault, before and after the thieves are caught: the two villagers who walk
/// about together (LOCALID_BOY_ROUND_THE_WELL and LOCALID_GIRL_ROUND_THE_WELL,
/// as both maps name them).
#include "types.h"
#include "object_event.h"

// The update hook of the two children round the well. Their timer holds two
// bits: bit 0 picks the partner (set for LOCALID_BOY_ROUND_THE_WELL, whose
// partner is LOCALID_GIRL_ROUND_THE_WELL; clear for
// LOCALID_GIRL_ROUND_THE_WELL) and bit 1 is set while the party talks to them.
// A villager stops when close to and facing its partner, and otherwise when the
// leader is within 18 pixels in front of it. While the window work's word at
// 376 or the render mode is set the range is 26 pixels, and a villager being
// talked to stops whichever way it faces.
//
// UB: update hooks return void. In tbs-en vault_revisit, plain
// void differs in 13 bytes of the 152-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void Vault_UpdateWalkingVillager(union FieldObject *fieldObject) {
#else
static s32 Vault_UpdateWalkingVillager(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *self = (struct ObjectEvent *)fieldObject;
  u8 **globals = gWindowWork;
  u8 *windowWork = globals[0];
  u8 *workspace = globals[12];
  s16 *flags = &self->timer;
  s32 force = 0;
  s32 range = 18;
  struct ObjectEvent *partner;
  struct ObjectEvent *player;

  if ((*flags & 1) != 0) {
    partner = ObjectEvent_GetById(LOCALID_GIRL_ROUND_THE_WELL);
  } else {
    partner = ObjectEvent_GetById(LOCALID_BOY_ROUND_THE_WELL);
  }
  if (Vault_StopIfFacingNear(self, partner, 32, 0) != 0) {
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  }

  player = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if (*(s16 *)(workspace + 376) != 0 ||
      windowWork[WINDOW_WORK_MIND_READ_OFFSET] != 0) {
    range = 26;
    if ((*flags & 2) != 0) {
      force = 1;
    }
  }

  Vault_StopIfFacingNear(self, player, range, force);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}
