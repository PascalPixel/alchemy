#include "types.h"
#include "ram_buffer.h"

struct ShopRuntime {
  u8 unk00[0x24];
  void *itemWindow;
};

struct ShopUnit {
  u8 unk00[216];
  u16 inventory[15];
};

extern const u8 MsgNoThatSNoGood[];
extern const u8 MsgItSOkIfIWork[];
extern const u8 MsgYouWantItToShine[];
extern const u8 MsgHmThatSNotRight[];
extern const u8 MsgWellHmIWonder[];
extern const u8 MsgWellAllRightIM[];
extern const u8 MsgALittleElbowGrease[];

struct ShopUnit *Unit_Get(s32 unitId);
s32 Forge_GetMaterialChance(s32 item);
s32 Forge_RollResult(s32 item);
void Forge_SetItem(s32 item);
void Func_0810857c(s32 message);
s32 Func_08108630(s32 value);
void MessageArgument_PushFar(s32 value, s32 slot);
s32 Inventory_RemoveFar(s32 unitId, s32 slot);
void Shop_DrawItemCount(void *window, s32 unitId);

/// The forge's offer for one inventory item: refuses an item that is no
/// material, otherwise asks (a rare material, chance 1, takes its own lines),
/// and on yes takes the item, redraws the count and stores the rolled result.
/// Returns 0 when the item was taken, else -1.
s32 Func_0810b6cc(s32 unitId, s32 slot) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  struct ShopUnit *unit = Unit_Get(unitId);
  u32 raw = unit->inventory[slot];
  s32 item;
  s32 chance;
  /* FAKEMATCH(pin): the result in r8, as the native keeps it; as a plain
   * local, 16 of 78 instructions differ in tla-en. */
  register s32 result asm("r8");

  item = 0x1ff;
  item &= raw;
  chance = Forge_GetMaterialChance(item);
  result = -1;

  if (Forge_GetMaterialChance(item) == 0) {
    Func_0810857c((s32)MsgNoThatSNoGood);
  } else {
    MessageArgument_PushFar(item, 2);
    Func_0810857c(chance == 1 ? (s32)MsgItSOkIfIWork
                              : (s32)MsgYouWantItToShine);
    if (Func_08108630(0) != 0) {
      Func_0810857c(chance == 1 ? (s32)MsgHmThatSNotRight
                                : (s32)MsgWellHmIWonder);
    } else {
      Func_0810857c(chance == 1 ? (s32)MsgWellAllRightIM
                                : (s32)MsgALittleElbowGrease);
      Inventory_RemoveFar(unitId, slot);
      Shop_DrawItemCount(shop->itemWindow, unitId);
      item = Forge_RollResult(item);
      Forge_SetItem(item);
      result = 0;
    }
  }
  return result;
}
