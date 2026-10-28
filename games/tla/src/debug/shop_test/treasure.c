#include "types.h"
#include "adventure.h"
#include "inventory.h"
#include "text/msg_ids.h"

void FieldMessage_Show(s32 message, s32 position);
void Party_RemoveActiveOwner(s32 owner);

static __inline__ void Inventory_AddItemAgain(s32 owner, s32 item) {
  /* FAKEMATCH: a direct call keeps the repeated item in r5 for the next call;
   * ⚓️ builds it again before each. */
  Inventory_AddItem(owner, item);
}

/// Coins and equipment for owners 4 to 7, then owners 0 to 3 leave the party.
void Func_020003b8(void) {
  s32 i;

  FieldMessage_Show((s32)MsgYouGotTreasure, 1);
  gAdventure.coins = 777777;
  for (i = 9; i >= 0; i--) {
    Inventory_AddItem(4, 228);
    Inventory_AddItem(4, 229);
  }
  Inventory_AddItem(4, 184);
  Inventory_AddItem(4, 204);
  Inventory_AddItem(4, 224);
  Inventory_AddItem(4, 11);
  Inventory_AddItem(4, 267);
  Inventory_AddItem(4, 223);
  Inventory_AddItem(5, 226);
  Inventory_AddItem(5, 227);
  Inventory_AddItem(5, 230);
  Inventory_AddItem(5, 232);
  Inventory_AddItem(5, 231);
  Inventory_AddItem(5, 237);
  Inventory_AddItem(5, 265);
  Inventory_AddItem(6, 242);
  Inventory_AddItem(6, 258);
  Inventory_AddItem(6, 252);
  Inventory_AddItemAgain(6, 429);
  Inventory_AddItemAgain(6, 429);
  Inventory_AddItemAgain(6, 429);
  Inventory_AddItem(6, 433);
  Inventory_AddItem(6, 435);
  Inventory_AddItemAgain(6, 417);
  Inventory_AddItemAgain(6, 417);
  Inventory_AddItem(6, 418);
  Inventory_AddItem(6, 419);
  Inventory_AddItem(7, 189);
  Inventory_AddItem(7, 200);
  Inventory_AddItem(7, 201);
  Inventory_AddItem(7, 202);
  Inventory_AddItem(7, 203);
  Inventory_AddItem(7, 204);
  Inventory_AddItem(7, 207);
  Party_RemoveActiveOwner(0);
  Party_RemoveActiveOwner(1);
  Party_RemoveActiveOwner(2);
  Party_RemoveActiveOwner(3);
}
