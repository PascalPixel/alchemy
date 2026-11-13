/// @file
/// The battle item menu.
// Part of screens.c.

#include "../screens_internal.h"
#include "item.h"
#include "actor.h"

/// @file
/// Items: summon-selection menu and battle item-use eligibility.

#include "battle_system.h"
#include "item.h"
#include "types.h"
#include "scene.h"
#include "edition.h"
#include "graphics.h"
#include "uiwindow.h"
#include "module/battle_effect/battle_effect.h"
#include "module/field/object.h"
#include "module/shop/shop.h"
#include "module/window/screens.h"

#define BATTLE_OK 0x80 ///< flags: usable in battle.

/// Determines whether an item can be used in battle.
/// Returns: 0 usable / 1 unusable / 2 unavailable in battle.
/// unit: character index.
/// item: carried item value.
s32 Item_ClassifyUseAbility(s32 unit, s32 item) {
  s32 ret;
  struct ItemDefinition *def;

  if (item == 0) {
    return 1;
  }
  def = Item_Get(item);
  ret = 1;
  if (def->useType == 3) // Equipment that grants Psynergy.
  {
    return ret;
  }
  if (def->actionId == 0) {
    return ret;
  }
  if ((def->type != 0) && (Item_CanUnitEquip(unit, item) == 0)) {
    return ret;
  }
  if ((BATTLE_OK & Ability_Get(def->actionId)->flags) == 0) {
    return 2;
  }
  return 0;
}

struct MenuFlagWork {
  u8 unk000[0x506];
  u8 busy;
};

struct FxGroup {
  u8 unk00[32];
  struct Actor *objects[8];
  u16 values[8];
  s32 count;
};

struct DescriptorEntry {
  s16 id;
  u8 unk02[6];
};

extern struct DescriptorEntry gData_080eff28[];

/* VIEW(screens): routine Func_0814ea58 as void Func_0814ea58(void *effect, s32
 * mode); module/battle_effect/battle_effect.h has void Func_0814ea58(struct
 * BattleEffectArgument *effect, s32 mode). */
extern void Func_0814ea58__screens(void *effect,
                                   s32 mode) __asm__("Func_0814ea58");

/// Copies one 4bpp tile of BG character block 0 to another, each given as a
/// tilemap entry whose palette and flip bits are ignored. The battle lists copy
/// tiles 24 and 25 (as entries `0xf018` and `0xf019`) to tiles 512, 513, 528
/// and 529 as they open.
void Vram_CopyTile(u32 sourceTile, u32 destinationTile) {
  Dma_Set(BG_CHAR_BLOCK(0) + (sourceTile & 0x3ff) * TILE_SIZE_4BPP,
          BG_CHAR_BLOCK(0) + (destinationTile & 0x3ff) * TILE_SIZE_4BPP,
          0x84000008, REG_DMA3);
}

// How much of the line under the cursor is copied: more outside Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define CURSOR_TEXT_LENGTH 32
#else
#define CURSOR_TEXT_LENGTH 52
#endif

// How much of a refusal is copied: more in German, Spanish, French and Italian.
#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_JAPANESE)
#define REFUSAL_TEXT_LENGTH 32
#else
#define REFUSAL_TEXT_LENGTH 52
#endif

// Japanese decodes both texts raw where the other languages copy them expanded.
#if LANGUAGE != LANGUAGE_JAPANESE
#define MENU_TEXT_COPY Message_CopyExpanded
#else
#define MENU_TEXT_COPY Message_DecodeRaw
#endif

struct ItemMenuWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04[2];
  u8 holdTilemapUpload;
};

#define NAVIGATION ((struct ItemMenuNavigation *)Ram_HeapSlots->unke4[0])
#define ITEM_BROKEN 0x400

// Sets a menu sprite's attribute words, as The Broken Seal's
// MenuSprite_Initialize.
#define MENU_SPRITE_INITIALIZE(sprite, shape)                                  \
  do {                                                                         \
    (sprite).oam.word.attr01 = (shape);                                        \
    (sprite).oam.word.attr23 = 0;                                              \
  } while (0)

s32 BattleItemMenu_Run(s32 member, u16 *entries, s32 count) {
  struct ItemMenuWork *work;
  s32 drawnFirstEntry;
  s32 drawnRow;
  s32 cursorHandle;
  struct ItemMenuWindow *messageWindow;
  s32 iconCount;
  s32 preferredRow;
  struct {
    s32 x, y;
  } cursorPosition;
  struct ItemMenuNavigation *navigation;
  struct ItemMenuWindow *window;
  s32 firstEntry;
  s32 selectedRow;
  s32 item;
  s32 iconTile;
  s32 pageTile;
  s32 unusableReason;
  s32 result;
  s32 i;
  struct ItemMenuOam cursorStorage;
  struct ItemMenuSprite *iconSprite;
  struct ItemMenuOam *sprite;
  u16 messageText[64];
  s32 iconSlots[5];
  struct ItemMenuSprite sprites[5];

  work = (struct ItemMenuWork *)Ram_HeapSlots->windowTiles;
  drawnFirstEntry = -1;
  drawnRow = -1;
  cursorHandle = VramCache_AllocEntry(128);
  messageWindow = Window_Create(0, 5, 30, 4, 42);
  iconCount = 0;
  do {
    firstEntry = NAVIGATION->page;
    selectedRow = NAVIGATION->row;
    preferredRow = NAVIGATION->preferredRow;
  } while (0);
  window = Window_Create(15, 9, 15, 11, 6);

  for (i = 0; i < 5; i++) {
    s32 y;
    // t holds the attribute zero, then is reused for the window's y; as plain
    // stores, 2 of 790 instructions differ in tla-en.
    u32 t = 0;

    y = i * 2;
    /* FAKEMATCH(dead): an empty once-only block after the row, which keeps it
     * and the zero ahead of the stores; without it, 56 of 790 instructions
     * differ in tla-en. */
    do {
    } while (0);
    iconSprite = &sprites[i];
    sprites[i].oam.word.attr01 = 0x40000000;
    sprites[i].oam.word.attr23 = t;
    iconSprite->oam.f.x = window->x * 8 + 8;
    t = window->y;
    iconSprite->oam.f.y = (y + t) * 8 + 4;
  }
  for (i = 0; i < 5; i++) {
    iconSlots[i] = VramCache_AllocEntry(128);
    sprites[i].oam.f.tile = VramCache_Reload(iconSlots[i], (const void *)-1);
  }
  Vram_CopyTile(0xf018, 0x200);
  Vram_CopyTile(0xf018, 0x201);
  Vram_CopyTile(0xf019, 0x210);
  Vram_CopyTile(0xf019, 0x211);

  for (;;) {
    /* FAKEMATCH(temporary): sprite holds &cursorStorage; as cursorStorage used
     * directly, 219 of 790 instructions differ in tla-en. */
    sprite = &cursorStorage;

    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      work->holdTilemapUpload = 1;
      Window_HighlightRect(window->x + 1, window->y + drawnRow * 2 + 1,
                           window->width - 2, 1, 15);
      Text_FillCanvasBackground();
      if (count != 0) {
        if (Item_ClassifyUseAbility(member,
                                    entries[firstEntry + selectedRow]) == 2)
          MENU_TEXT_COPY((s32)MsgKey0d4f, messageText, CURSOR_TEXT_LENGTH);
        else
          MENU_TEXT_COPY((entries[firstEntry + selectedRow] & 0x1ff) +
                             (s32)MsgItemPlainName,
                         messageText, CURSOR_TEXT_LENGTH);
      } else {
        MENU_TEXT_COPY((s32)MsgKey0d46, messageText, CURSOR_TEXT_LENGTH);
      }
      Text_DrawWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;
      if (firstEntry != drawnFirstEntry) {
        WindowSprite_RedrawSavedRect(window);
        for (i = 0; i < 5; i++) {
          item = entries[firstEntry + i];
          if (item == 0)
            break;
          Item_Get(item);
          Text_SetColor(15);
          if (Item_ClassifyUseAbility(member, item) != 0)
            Text_SetColor(4);
          else if (item & ITEM_BROKEN)
            Text_SetColor(2);
          Text_DrawMessageAsTiles((item & 0x1ff) + (s32)MsgItemName, window, 16,
                                  i * 16);
          Text_SetColor(15);
          sprites[i].oam.f.tile = ItemIcon_LoadIntoSlot(item, iconSlots[i]);
        }
        iconCount = i;
        drawnFirstEntry = firstEntry;
      }
      if (count > 5) {
        for (i = 0; i < (count + 4) / 5; i++) {
          iconTile = i + 0xf301;
          if (i == firstEntry / 5)
            iconTile = i + 0xf30b;
          Window_SetTilemapEntry(
              window, iconTile, window->width - (count + 4) / 5 + i - 2, -1, 0);
        }
      }
      Window_HighlightRect(window->x + 1, window->y + selectedRow * 2 + 1,
                           window->width - 2, 1, 14);
      work->dirty = 1;
      work->holdTilemapUpload = 0;
    }

    if (count > 5) {
      for (i = 0; i < (count + 4) / 5; i++) {
        iconTile = i + 0xf301;
        if ((gFrameCount & 15) <= 11) {
          if (i == firstEntry / 5)
            iconTile = i + 0xf30b;
        }
        Window_SetTilemapEntry(window, iconTile,
                               window->width - (count + 4) / 5 + i - 2, -1, 0);
      }
      pageTile = -1;
      Window_SetTilemapEntry(window, 0xf334,
                             window->width - (count + 4) / 5 - 3, pageTile, 0);
      Window_SetTilemapEntry(window, 0xf335, window->width - 2, pageTile, 0);
      do {
        work->dirty |= 2 << ((u32)(window->y - 1) >> 2);
      } while (0);
    }

    for (i = 0; i < iconCount; i++) {
      iconSprite = &sprites[i];
      Oam_AddSpriteAtDepth(iconSprite, 240);
    }

    cursorPosition.x = window->x * 8 - 2;
    cursorPosition.y = (selectedRow * 2 + window->y) * 8 + 20;
    /* FAKEMATCH(dead): sprite set again to the &cursorStorage it holds; without
     * it, 44 of 790 instructions differ in tla-en. */
    sprite = &cursorStorage;
    ((u32 *)sprite)[1] = 0x40000000;
    ((u32 *)sprite)[2] = 0;
    sprite->tile = VramCache_Reload(cursorHandle, gResource_FixedBlockBTiles);
    sprite->x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
    sprite->y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0)
      Oam_AddSpriteAtDepth(sprite, 242);

    do {
      NAVIGATION->page = firstEntry;
      NAVIGATION->row = selectedRow;
      NAVIGATION->preferredRow = preferredRow;
    } while (0);
    navigation = NAVIGATION;
    if (gInput.pressed & 1) {
      if (count != 0) {
        result = firstEntry + selectedRow;
        Item_Get(entries[result]);
        unusableReason = 0;
        if ((entries[result] & ITEM_BROKEN) == 0) {
          unusableReason = Item_ClassifyUseAbility(member, entries[result]);
          if (unusableReason == 0)
            break;
        }
        Sound_PlayFar(114);
        if (unusableReason == 2)
          MENU_TEXT_COPY((s32)MsgKey0d4f, messageText, REFUSAL_TEXT_LENGTH);
        else if (entries[result] & ITEM_BROKEN)
          MENU_TEXT_COPY((s32)MsgKey0d4d, messageText, REFUSAL_TEXT_LENGTH);
        else
          MENU_TEXT_COPY((s32)MsgKey0d4c, messageText, REFUSAL_TEXT_LENGTH);
        Text_FillCanvasBackground();
        Text_DrawWideStringAtOffset(messageText, messageWindow, 0, 4);
      } else {
        result = -1;
        break;
      }
    } else if (navigation->timer == 0 || (gInput.pressed & 2)) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }

    if (count != 0) {
      if (gInput.repeat & 0x80) {
        Sound_PlayFar(111);
        selectedRow++;
        if (selectedRow == 5 || firstEntry + selectedRow == count)
          selectedRow = 0;
        preferredRow = selectedRow;
      } else if (gInput.repeat & 0x40) {
        Sound_PlayFar(111);
        selectedRow--;
        if (selectedRow < 0) {
          if (firstEntry == (count - 1) / 5 * 5)
            selectedRow = count - firstEntry - 1;
          else
            selectedRow = 4;
        }
        preferredRow = selectedRow;
      } else if (gInput.repeat & 0x10) {
        Sound_PlayFar(111);
        Input_ResetKeyRepeat();
        if (firstEntry + 5 >= count) {
          if (firstEntry != 0) {
            firstEntry = 0;
            selectedRow = preferredRow;
          }
        } else {
          firstEntry += 5;
          selectedRow = preferredRow;
          if (firstEntry == (count - 1) / 5 * 5) {
            selectedRow = count - firstEntry - 1;
            if (selectedRow > preferredRow)
              selectedRow = preferredRow;
          }
        }
      } else if (gInput.repeat & 0x20) {
        Sound_PlayFar(111);
        Input_ResetKeyRepeat();
        if (firstEntry != 0) {
          firstEntry -= 5;
          selectedRow = preferredRow;
        } else {
          firstEntry = (count - 1) / 5 * 5;
          selectedRow = preferredRow;
          if (firstEntry != 0) {
            selectedRow = count - firstEntry - 1;
            if (selectedRow > preferredRow)
              selectedRow = preferredRow;
          }
        }
      }
    }
    wait(1);
  }

  Window_Close(messageWindow, 1);
  Window_Close(window, 1);
  wait(1);
  Resource_ResetEntry(cursorHandle);
  for (i = 0; i < 5; i++)
    Resource_ResetEntry(iconSlots[i]);
  wait(1);
  return result;
}

#undef BATTLE_OK
#undef CURSOR_TEXT_LENGTH
#undef REFUSAL_TEXT_LENGTH
#undef MENU_TEXT_COPY
#undef NAVIGATION
#undef ITEM_BROKEN
#undef MENU_SPRITE_INITIALIZE
