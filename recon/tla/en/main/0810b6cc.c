// Draft, differ 18 of 78: the forge's offer for one inventory item: refuses
// an item that is no material, otherwise asks (a rare material takes its own
// lines), and on yes takes the item, draws the count and stores the rolled
// result. No ☀️ twin: ☀️ has no forge. Left: register choice: the native
// keeps slot in r10, the menu work in r9 and the result in r8 (pushing
// r8 to r10); here slot takes r8, the work r10 and the result r9, so only r8
// and r10 are saved, plus one movs of the 0x1ff mask a slot later. The
// message choices as ternaries give the native's shared calls (differ 70 as
// separate if/else calls). Tried: the result declared first (18).
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

// Unnamed in the catalog yet: 4864 to 4870 take placeholders here.
extern u8 Msg04864[], Msg04865[], Msg04866[], Msg04867[], Msg04868[];
extern u8 Msg04869[], Msg04870[];

struct ShopUnit *UnitGet(s32 unitId);
s32 Forge_GetMaterialChance(s32 item);
s32 Forge_RollResult(s32 item);
void Forge_SetItem(s32 item);
void Func_0810857c(s32 message);
s32 Func_08108630(s32 value);
void UiText_DrawQuantity(s32 value, s32 slot);
void Inventory_RemoveFar(s32 unitId, s32 slot);
void Shop_DrawItemCount(void *window, s32 unitId);

s32 Forge_OfferItem(s32 unitId, s32 slot) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  struct ShopUnit *unit = UnitGet(unitId);
  s32 item = unit->inventory[slot] & 0x1ff;
  s32 chance = Forge_GetMaterialChance(item);
  s32 result = -1;

  if (Forge_GetMaterialChance(item) == 0) {
    Func_0810857c((s32)Msg04864);
  } else {
    UiText_DrawQuantity(item, 2);
    Func_0810857c(chance == 1 ? (s32)Msg04865 : (s32)Msg04866);
    if (Func_08108630(0) != 0) {
      Func_0810857c(chance == 1 ? (s32)Msg04867 : (s32)Msg04868);
    } else {
      Func_0810857c(chance == 1 ? (s32)Msg04869 : (s32)Msg04870);
      Inventory_RemoveFar(unitId, slot);
      Shop_DrawItemCount(shop->itemWindow, unitId);
      item = Forge_RollResult(item);
      Forge_SetItem(item);
      result = 0;
    }
  }
  return result;
}
