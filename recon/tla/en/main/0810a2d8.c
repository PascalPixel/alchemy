// Near miss, differ 2 of 170 (tla-en, 2026-10-31): sched2 order only, one
// pair. An empty asm before the price read (tagged below) settles the second
// pair the earlier notes describe (4 to 2); the first remains. The native
// spills the slot argument (str r1) before it builds the heap slots' address;
// in sched2 the address chain (movs, lsls, adds, then the load) has priority
// 151 against the spill's 150, so its movs goes first, the same swap as
// Shop_SellItem (0810a108). After Inventory_FindEquippedFar the
// native spills the equipped slot (str r0) before it copies the unit pointer
// for the price's halfword load (mov r3, r8); both have priority 35, and
// sched2 prefers the copy, which depends on the call only through r3's
// clobber, over the spill that waits on the call's result. Reading the item id
// as ☀️ does (inventory[slot] & 0x1ff in its initializer) orders the slot
// offset's copy and the 0x1ff mask with no barrier (6 to 4; the earlier draft
// kept an untagged empty asm before the mask). Prototype variants for
// Shop_GetItemPrice (u16, unprototyped) change nothing, and a 4-minute
// permuter run found nothing better (2026-10-31). With the barrier in place:
// an empty asm or one-pass block ahead of the shop read puts the slot spill
// first but moves the unitId copy (adds r7, r0) ahead of it too, since that
// copy is an entry insn (2 either way); a copy of unitId made after it is
// merged back to the entry; a one-pass block before the price read in place of
// the asm upsets allocation (134); a volatile shop read, u32 slot and a
// separate assignment of equipped change nothing; and a second 4-minute
// permuter run found nothing better (2026-10-31).
// The shop repairs a unit's damaged item at slot (refusing other kinds,
// undamaged items, cursed equipped items and a price over the party's coins),
// then offers to equip it. ☀️'s Shop_RepairItem in
// games/tbs/src/shop/shop/repair.c, rewritten with ⚓️'s calls (Unit_Get,
// Shop_GetItemPrice, MessageArgument_PushFar, the shop message and choice
// routines Func_0810857c and Func_08108630, Inventory_RepairFar, Func_08109188
// for the coins) and numbered messages, their ☀️ names beside them.
#include "main_routine_work.h"
#include "battle_system.h"
#include "battle_unit.h"
#include "item.h"
#include "ram_buffer.h"

// The shop work block, as this function reads it.
struct ShopRuntime {
  u8 unk000[0x24];
  s32 itemWindow;
};

extern u8 Msg04715[]; // MsgCannotRepairKind
extern u8 Msg04716[]; // MsgNotDamaged
extern u8 Msg04717[]; // MsgCursedWontComeOff
extern u8 Msg04718[]; // MsgNotEnoughMoney
extern u8 Msg04719[]; // MsgShopRepairConfirm, then its three replies

struct BattleUnit *Unit_Get(s32 unitId);
s32 Inventory_FindEquippedFar(s32 unitId, s32 type);
s32 Shop_GetItemPrice(s32 item);
void MessageArgument_PushFar(s32 value, s32 slot);
void Func_0810857c(s32 message);
s32 Func_08108630(s32 value);
void Message_CloseFinishedWindowsFar(void);
void wait(s32 frames);
void Sound_PlayFar(s32 cue);
void Inventory_RepairFar(s32 unitId, s32 slot);
s32 Party_AdjustCoinsFar(s32 amount);
void Func_08109188(void);
void Shop_DrawItemCount(s32 window, s32 unitId);
s32 Shop_ConfirmEquip(s32 unitId, s32 slot);
s32 Shop_SellOld(s32 unitId, s32 slot);

void Func_0810a2d8(s32 unitId, s32 slot) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  struct BattleUnit *unit = Unit_Get(unitId);
  s32 itemId = unit->inventory[slot] & 0x1ff;
  struct ItemDefinition *item = Item_Get(itemId);
  s32 equipped = Inventory_FindEquippedFar(unitId, item->type);
  u32 price;
  s32 message;
  u8 kind;
  u32 saved;

  /* FAKEMATCH(barrier): the empty asm ends the scheduling block after the
   * equipped slot's spill, so it is stored before the unit pointer is copied
   * for the price's load; as the function without it, 4 of 170 instructions
   * differ in tla-en. */
  asm("");
  price = Shop_GetItemPrice(unit->inventory[slot]);
  kind = item->useType;
  if (kind != 2) {
    MessageArgument_PushFar(itemId, 2);
    Func_0810857c((s32)Msg04715);
    return;
  }
  if (!(unit->inventory[slot] & 0x400)) {
    MessageArgument_PushFar(itemId, 2);
    Func_0810857c((s32)Msg04716);
    return;
  }
  if ((unit->inventory[slot] & 0x200) && (kind & item->flags)) {
    MessageArgument_PushFar(itemId, 2);
    Func_0810857c((s32)Msg04717);
    return;
  }
  if (price > gMainRoutineWork.coins) {
    Func_0810857c((s32)Msg04718);
    return;
  }
  MessageArgument_PushFar(itemId, 2);
  MessageArgument_PushFar(price, 5);
  message = (s32)Msg04719;
  Func_0810857c(message);
  if (Func_08108630(0) != 0) {
    Func_0810857c(message + 1);
    return;
  }
  saved = unit->inventory[slot];
  unit->inventory[slot] = 0;
  Shop_DrawItemCount(shop->itemWindow, unitId);
  MessageArgument_PushFar(itemId, 2);
  Func_0810857c(message + 2);
  Message_CloseFinishedWindowsFar();
  wait(10);
  Sound_PlayFar(100);
  wait(110);
  Sound_PlayFar(100);
  wait(110);
  Sound_PlayFar(100);
  wait(110);
  Sound_PlayFar(112);
  wait(20);
  unit->inventory[slot] = saved;
  Inventory_RepairFar(unitId, slot);
  Party_AdjustCoinsFar(-price);
  Func_08109188();
  Shop_DrawItemCount(shop->itemWindow, unitId);
  MessageArgument_PushFar(itemId, 2);
  Func_0810857c(message + 3);
  if (Shop_ConfirmEquip(unitId, slot))
    Shop_SellOld(unitId, equipped);
}
