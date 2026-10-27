#include "types.h"
#include "system.h"
#include "ram_buffer.h"

struct Rec5 {
  u8 pad[5];
  unsigned int flag : 8;
};
struct Cur {
  unsigned short mark : 8;
};
extern u8 gData_03001f2c[];

u8 *BattleAction_Get(s32 action);
s32 BattleEffect_HasMatchingEvent5Far(u8 effect);
void Func_080f8ab4(s32 x, s32 y);
void *Unit_Get(s32 owner);
s32 Func_080fd6f0(void *unit, void *list, s32 mode);
s32 Func_080fd000(void *owner, void *list);

/// Selects a party tab in the Psynergy menu and builds that member's Psynergy
/// icons: The Broken Seal's PsynergyMenu_SelectPartySlot with this game's
/// work-block offsets (each 4 below The Broken Seal's).
s32 Func_080fcf5c(s32 partySlot) {
  void *menu = Ram_HeapSlots->menuRuntime;
  s32 offset = partySlot + 28;
  s32 cursorOffset = partySlot * 4 + 20;
  void *icon;
  u8 byteVal;
  s32 ownerIndex;
  s32 combinedOffset;
  s32 objOff;
  s32 objId;
  void *objPtr;
  void *p456;
  u8 *p2;
  s32 badge;
  s32 result;
  s32 cursorOffset2;

  result = 0;
  icon = *(void **)(menu + cursorOffset);
  *(u8 *)(icon + 5) = 1;
  *(u16 *)(icon + 12) = result;
  ((struct Rec5 *)(*(u8 **)(menu + 536)))->flag = 13;
  p2 = (u8 *)menu + 2;
  ownerIndex = *(s8 *)(menu + offset);
  byteVal = ((struct Cur *)(menu + 533))->mark;
  p2[offset] = byteVal;

  if (ownerIndex == -1) {
    *(u8 *)(menu + offset) = 0;
    combinedOffset = 0;
  } else {
    combinedOffset = ownerIndex * 2;
    Func_080f8ab4(ownerIndex * 24 - 10, 16);
  }

  objOff = combinedOffset + 516;
  objId = *(u16 *)(menu + objOff);
  objPtr = Unit_Get(objId);
  p456 = menu + 452;
  badge = Func_080fd6f0(objPtr, p456, 2);
  *(u8 *)(menu + 532) = (u8)badge;
  result = Func_080fd000(menu + 516, p456);

  cursorOffset2 = partySlot * 4 + 20;
  icon = *(void **)(menu + cursorOffset2);
  UiIcon_PrepareObject(icon);
  WaitFrames(1);
  return result;
}
