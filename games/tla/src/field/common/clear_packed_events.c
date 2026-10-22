#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "party_state.h"
#include "owner_state.h"
#include "text/msg_ids.h"

struct OwnerVitals {
  u8 unk000[20];
  s16 hpRatio;
  s16 ppRatio;
  u8 unk018[28];
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
  u8 unk03c[0x131 - 60];
  u8 status;
};

void Event_ClearValidPackedIds(void) {
  if (Event_ValidatePackedId(gPartyState.packedEvents[0]))
    gPartyState.packedEvents[0] = 0;
  if (Event_ValidatePackedId(gPartyState.packedEvents[1]))
    gPartyState.packedEvents[1] = 0;
}

static __inline__ s32 Vitals_Ratio(s32 value, s32 max) {
  s32 ratio = (value << 14) / max;
  s32 clamped = 0x4000;

  if (ratio <= 0x4000) {
    clamped = 0;
    if (ratio >= 0)
      clamped = ratio;
  }
  return clamped;
}

void Party_RemoveOwnerRestored(s32 owner) {
  struct OwnerVitals *vitals;
  s32 i;
  s32 count;
  s32 standing;

  Party_RemoveActiveOwnerFar(owner);
  Event_ClearValidPackedIds();
  vitals = Unit_Get(owner);
  /* FAKEMATCH: the do-while keeps both vital copies ahead of the ratio */
  do {
    vitals->hp = vitals->maxHp;
    vitals->pp = vitals->maxPp;
  } while (0);
  vitals->hpRatio = Vitals_Ratio(vitals->hp, vitals->maxHp);
  if (vitals->hpRatio == 0 && vitals->hp != 0)
    vitals->hpRatio = 1;
  vitals->ppRatio = Vitals_Ratio(vitals->pp, vitals->maxPp);
  if (vitals->ppRatio == 0 && vitals->pp != 0)
    vitals->ppRatio = 1;
  vitals->status = 0;
  standing = 0;
  count = Party_CountActiveOwnersFar();
  for (i = 0; i < count; i++) {
    vitals = Unit_Get(gPartyState.activeOwners[i]);
    if (vitals->hp != 0)
      standing++;
  }
  if (standing == 0) {
    vitals = Unit_Get(gPartyState.currentOwner);
    vitals->hp = 1;
    vitals->hpRatio = Vitals_Ratio(vitals->hp, vitals->maxHp);
    if (vitals->hpRatio == 0 && vitals->hp != 0)
      vitals->hpRatio = 1;
    vitals->ppRatio = Vitals_Ratio(vitals->pp, vitals->maxPp);
    if (vitals->ppRatio == 0 && vitals->pp != 0)
      vitals->ppRatio = 1;
  }
}

s32 PartyInventory_FindRoomFar(s32 item);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);

s32 Func_080d25c8(s32 item) {
  Party_CountActiveOwnersFar();
  if (PartyInventory_FindRoomFar(item) < 0) {
    UiText_DrawQuantity(item, 2);
    UiText_ShowPositionedMessageAndWaitFar((s32)MsgPartyCannotCarry, 1);
    UiText_DrawQuantity(item, 2);
    UiText_ShowPositionedMessageAndWaitFar((s32)MsgPartyCannotCarry + 1, 1);
    return -1;
  }
  return 0;
}
