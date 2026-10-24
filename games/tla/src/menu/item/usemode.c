#include "types.h"
#include "ram_buffer.h"
#include "edition.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

/// The item command grid, by row * 3 + column: Use, Equip and Details on top,
/// Give, Remove and Drop below.
enum ItemMenuCommand {
  ITEM_COMMAND_USE,
  ITEM_COMMAND_EQUIP,
  ITEM_COMMAND_INSPECT, ///< Details
  ITEM_COMMAND_GIVE,
  ITEM_COMMAND_REMOVE,
  ITEM_COMMAND_DROP
};

/// An item command's state when the selected item refuses it.
#define ITEM_COMMAND_STATE_DISABLED (-1)

#define TEXT_COLOR_NORMAL 15   ///< the window text's default colour
#define TEXT_COLOR_DISABLED 14 ///< the grey of a command that refuses A

// The x of the command grid's middle column: German, Spanish and French
// draw Equip and Remove 8 pixels further left.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH)
#define ITEM_COMMAND_TEXT_X 32
#else
#define ITEM_COMMAND_TEXT_X 40
#endif

s32 UiWork_SetParamNibbleFar(s32 color);
extern u8 MsgItemCommandUse[];
void UiText_DrawCharacterAtOffsetFar(u32 message, s32 window, s32 x, s32 y);

/// Writes the six command names into window, the disabled ones in grey.
void ItemMenu_DrawCommands(const s8 *commandStates, s32 window) {
  s32 disabled;
  s32 useState;
  u32 firstCommandMessage;

  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  useState = commandStates[ITEM_COMMAND_USE];
  /* FAKEMATCH(barrier): an empty asm on the states after the first is read,
   * so -1 is built around that load; without it, 2 of 92 instructions differ
   * in tla-en. */
  asm("" : "+l"(commandStates));
  disabled = ITEM_COMMAND_STATE_DISABLED;
  if (useState == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);

  firstCommandMessage = (u32)MsgItemCommandUse;
  // "Use"
  UiText_DrawCharacterAtOffsetFar(firstCommandMessage, window, 0, 24);
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_EQUIP] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  // "Equip"
  UiText_DrawCharacterAtOffsetFar(firstCommandMessage + 1, window,
                                  ITEM_COMMAND_TEXT_X, 24);
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_GIVE] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  // "Give"
  UiText_DrawCharacterAtOffsetFar(firstCommandMessage + 2, window, 0, 32);
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_DROP] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  // "Drop"
  UiText_DrawCharacterAtOffsetFar(firstCommandMessage + 3, window, 80, 32);
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_INSPECT] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  // "Details"
  UiText_DrawCharacterAtOffsetFar(firstCommandMessage + 4, window, 80, 24);
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_REMOVE] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  // "Remove"
  UiText_DrawCharacterAtOffsetFar(firstCommandMessage + 5, window,
                                  ITEM_COMMAND_TEXT_X, 32);
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
}

struct ItemDefinition *Item_Get(s32);
s32 BattleEffect_HasTriggerFar(s32);
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
// Spanish and Italian builds use a different name for this far call,
// as in shop.c.
s32 Func_080c8500(s32);
#endif
void *BattleAction_Get(s32);
s32 Item_CanOwnerEquip(s32, s32);

s32 Item_ClassifyUseMode(s32 owner, s32 itemId) {
  s32 masked = itemId;
  void *itemData;
  s32 result = -1;

  masked &= 0x1ff;
  itemData = Item_Get(masked);

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
  if (Func_080c8500(masked) != 0) {
#else
  if (BattleEffect_HasTriggerFar(masked) != 0) {
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
    abilityData = BattleAction_Get(id);

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
void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y);
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
    InventoryMenu_ShowModalMessage((s32)MsgItBroke, 0, -1);
    RenderOutput_RedrawSavedRectFar(*(void **)(work + 0x30));
  }
}
