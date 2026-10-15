#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "owner_state.h"

struct TradeOfferList {
  struct TradeOffer offers[72];
  s32 count;
};

struct TradeOfferTable {
  u8 unk00[8];
  struct TradeOfferList list;
};

// Whether the djinn can be set: it is owned, fewer than ten are set, it is
// not set already, and no trade offer holds it unless the offer has run out.
s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit) {
  struct OwnerDjinnState *state = UnitGet(owner);
  struct TradeOfferList *table;
  s32 i;
  s32 status;
  u32 owned;

  if (state->counts.banks.owned[index] == 0)
    return 0;
  if (state->counts.banks.active[index] > 9) {
    state->counts.banks.active[index] = 10;
    return 0;
  }
  owned = state->flags.banks.owned[index];
  if ((owned & (1 << bit)) == 0)
    return 0;
  if ((state->flags.banks.active[index] & (1 << bit)) != 0)
    return 0;

  table =
      &((struct TradeOfferTable *)Trade_GetOfferState((owner < 0 || owner > 7)))
           ->list;
  for (i = 0; i < table->count; i++) {
    if (index == table->offers[i].element && bit == table->offers[i].djinn)
      break;
  }
  if (i == table->count ||
      ((status = table->offers[i].status) <= 0 && status != -2))
    return 1;
  return 0;
}

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit) {
  s32 value =
      ((struct OwnerDjinnState *)UnitGet(owner))->flags.banks.active[index] &
      (1 << bit);

  return (u32)(-value | value) >> 31;
}

s32 Djinn_Activate(s32 owner, s32 index, s32 bit) {
  struct OwnerDjinnState *state = UnitGet(owner);
  s32 result = Trade_CanOfferDjinn(owner, index, bit);

  if (result != 0) {
    u32 owned = state->flags.banks.owned[index];
    if (owned & (1 << bit)) {
      state->flags.banks.active[index] |= 1 << bit;
    } else {
      return 0;
    }
    state->counts.banks.active[index]++;
    Owner_RefreshDerivedData(owner);
  }
  return result;
}

u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit) {
  struct OwnerDjinnState *state = UnitGet(owner);
  u32 present = Djinn_IsActive(owner, index, bit);

  if (present != 0) {
    state->counts.banks.active[index]--;
    state->flags.banks.active[index] &= ~(1 << bit);
    Owner_RefreshDerivedData(owner);
  }
  return present;
}
