#include "types.h"
#include "owner_state.h"

struct TradeOfferList {
  struct TradeOffer offers[72];
  s32 count;
};

struct TradeOfferTable {
  u8 unk00[8];
  struct TradeOfferList list;
};

s32 DjinnRecovery_Remove(s32 owner, s32 index, s32 bit) {
  struct TradeOfferList *table;
  s32 found = 0;
  s32 i;

  table = &((struct TradeOfferTable *)Djinn_GetRecoveryTable(
                (owner < 0 || owner > 7)))
               ->list;
  for (i = 0; i < table->count; i++) {
    if (index == table->offers[i].element && bit == table->offers[i].djinn) {
      table->count--;
      found = 1;
      break;
    }
  }
  for (; i < table->count; i++)
    table->offers[i] = table->offers[i + 1];
  return found;
}
