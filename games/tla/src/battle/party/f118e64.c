#include "types.h"

s32 BattleParty_PrepareActiveOwners(u16 *ids);
s32 BattleParty_PrepareReserveOwners(u16 *ids);
s32 BattleParty_ListActorIds(s32 side, u16 *ids);
u8 *UnitGet(s32 id);

void Func_08118e64(void) {
  u16 ids[16];
  u16 *id = ids;
  s32 count;

  count = BattleParty_PrepareActiveOwners(ids);
  count += BattleParty_PrepareReserveOwners(ids + count);
  count += BattleParty_ListActorIds(2, ids + count);
  if (count > 0) {
    do {
      u8 *unit = UnitGet(*id++);

      unit[299] = 0;
      unit[325] = 0;
      count--;
    } while (count != 0);
  }
}

struct TradeOffer {
  u8 element;
  u8 djinn;
  u8 unk02[2];
};

struct TradeOfferList {
  struct TradeOffer offers[72];
  s32 count;
};

struct TradeOfferState {
  u8 unk00[8];
  struct TradeOfferList list;
};

s32 Djinn_IsReadyFar(s32 owner, s32 element, s32 djinn);
struct TradeOfferState *Djinn_GetRecoveryTableFar(s32 side);
void DjinnRecovery_AddStandbyFar(s32 owner, s32 element, s32 djinn);

/// Adds to the trade offers every djinn an active owner can offer that is
/// not offered yet; owners above 7 offer on the second side.
void Func_08118eb0(void) {
  u16 owners[10];
  s32 count;
  s32 n;
  u32 owner;
  s32 element;
  s32 djinn;
  s32 i;
  struct TradeOfferList *list;

  count = BattleParty_PrepareActiveOwners(owners);
  for (n = 0; n < count; n++) {
    owner = owners[n];
    for (element = 0; element <= 3; element++) {
      for (djinn = 0; djinn <= 19; djinn++) {
        if (Djinn_IsReadyFar(owner, element, djinn) != 0) {
          list = &Djinn_GetRecoveryTableFar(owner > 7)->list;
          for (i = 0; i < list->count; i++) {
            if (element == list->offers[i].element &&
                djinn == list->offers[i].djinn)
              break;
          }
          if (i == list->count)
            DjinnRecovery_AddStandbyFar(owner, element, djinn);
        }
      }
    }
  }
}
