#include "types.h"
#include "ram_buffer.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct ItemDefinition *Item_Get(s32);
s32 BattleFx_HasTriggerFar(s32);
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
// Spanish and Italian builds use a different name for this far call,
// as in shop.c.
s32 Func_080c8500(s32);
#endif
void *BattleActGet(s32);
s32 Item_CanOwnerEquip(s32, s32);

s32 Item_ClassifyUseMode(s32 owner, s32 itemId) {
  s32 masked = itemId;
  void *itemData;
  s32 result = -1;

  masked &= 0x1ff;
  itemData = Item_Get(masked);

#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
  if (Func_080c8500(masked) != 0) {
#else
  if (BattleFx_HasTriggerFar(masked) != 0) {
#endif
    return 0;
  }

  {
    u32 use = FIELD_AT_OFFSET(itemData, u16, 40);
    void *abilityData;
    u32 id;

    /* FAKEMATCH: ⚓️ loads the item's use word before it builds the 0x3fff mask;
     * the plain call starts the mask first (one pair of instructions swapped).
     */
    __asm__ volatile("");
    id = 0x3fff;
    id &= use;
    abilityData = BattleActGet(id);

    if (FIELD_AT_OFFSET(itemData, u16, 40) != 0) {
      if (FIELD_AT_OFFSET(itemData, u8, 2) != 0) {
        if (FIELD_AT_OFFSET(itemData, u8, 12) != 3) {
          if (Item_CanOwnerEquip(owner, masked) != 0) {
            result = 1;
          }
        }
      } else {
        result = 1;
      }

      if (result == 1) {
        s32 mask = 0x80;
        u8 field1 = FIELD_AT_OFFSET(abilityData, u8, 1);

        if ((field1 & 0x40) != 0) {
          u8 field8 = FIELD_AT_OFFSET(abilityData, u8, 8);
          result = (field8 == 0xff) ? 2 : 1;
        } else {
          result = (field1 & mask) ? -1 : 0;
        }
      }
    }
  }

  return result;
}

u32 Random16(void);
void Inventory_BreakFar(s32 owner, s32 slot);
void Audio_PlayCue(s32 cue);
void Func_080f8ce8(const u8 *message, s32 a, s32 b);
extern const u8 MsgItBroke[];
void RenderOutput_RedrawSavedRectFar(void *window);

void Func_080fb6d4(void) {
  u8 *work = Ram_HeapSlots->menuRuntime;
  u32 item = *(u16 *)(work + 0x16c);
  u32 mask;

  /* FAKEMATCH: the barrier keeps the item load ahead of the 0x1ff mask, and the
   * mask variable keeps the mask in r0 and the item in r3; without the barrier
   * the mask is started before the load (differ 2). */
  asm("");
  mask = 0x1ff;
  mask &= item;
  if (((u8 *)Item_Get(mask))[12] == 2 && Random16() < 0x2000) {
    Inventory_BreakFar(work[0x216], *(u16 *)(work + 0x168));
    Audio_PlayCue(138);
    Func_080f8ce8(MsgItBroke, 0, -1);
    RenderOutput_RedrawSavedRectFar(*(void **)(work + 0x30));
  }
}
