// Near miss, differ 2 of 75 (one reordered pair), 2026-10-18:
// GameFlag_RefreshLureCap, ported from The Broken Seal's function of the same
// name (games/tbs/src/game/game.c). Remaining: after Item_GetDirect the
// native restores the spilled slot and count (ldr r2, ldr r1) before it
// copies the item (adds r5, r0, #0); this copies first.
#include "types.h"
#include "party_state.h"
#include "owner_state.h"
#include "item.h"

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);

void GameFlag_RefreshLureCap(void) {
  s32 partySize;
  s32 partyIndex;

  GameFlag_ClearBit(0x167);
  partySize = Party_CountActiveOwners();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    struct OwnerInventoryState *unit;
    s32 inventorySlot;

    unit = Unit_Get(gPartyState.activeOwners[partyIndex]);
    for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++) {
      if (unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
        struct ItemEffect *effect;
        s32 effectIndex;

        effect = Item_GetDirect(unit->inventory[inventorySlot])->effects;
        for (effectIndex = 0; effectIndex < 4; effectIndex++) {
          u8 effectKind;

          effectKind = effect->kind;
          effect++;
          if (effectKind == 27) {
            GameFlag_SetBit(0x167);
          }
        }
      }
    }
  }
}
