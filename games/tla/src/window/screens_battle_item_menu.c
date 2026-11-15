/// @file
/// The battle item menu.
// Part of window.c.

#include "window_internal.h"
#include "sounds.h"
#include "game/item.h"
#include "field/actor.h"

/// @file
/// Items: summon-selection menu and battle item-use eligibility.

#include "battle/battle_system.h"
#include "game/item.h"
#include "types.h"
#include "system/scene.h"
#include "edition.h"
#include "graphics/graphics.h"
#include "graphics/uiwindow.h"
#include "battle_effect/battle_effect.h"
#include "field/object.h"
#include "shop/shop.h"
#include "window/screens.h"

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
  def = Item_GetDirectFar(item);
  ret = 1;
  if (def->useType == 3) // Equipment that grants Psynergy.
  {
    return ret;
  }
  if (def->abilityId == 0) {
    return ret;
  }
  if ((def->type != 0) && (Item_CanUnitEquip(unit, item) == 0)) {
    return ret;
  }
  if ((BATTLE_OK & Ability_Get(def->abilityId)->flags) == 0) {
    return 2;
  }
  return 0;
}

/// Copies one 4bpp tile of BG character block 0 to another, each given as a
/// tilemap entry whose palette and flip bits are ignored. The battle lists copy
/// tiles 24 and 25 (as entries `0xf018` and `0xf019`) to tiles 512, 513, 528
/// and 529 as they open.
void Vram_CopyTile(u32 sourceTile, u32 destinationTile) {
  Dma_Set(BG_CHAR_BLOCK(0) + (sourceTile & 0x3ff) * TILE_SIZE_4BPP,
          BG_CHAR_BLOCK(0) + (destinationTile & 0x3ff) * TILE_SIZE_4BPP,
          DMA_COPY_WORDS(8), REG_DMA3);
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

#define NAVIGATION ((struct BattleMenuWork *)Ram_HeapSlots->battleMenuWork)
#define ITEM_BROKEN 0x400

// Sets a menu sprite's attribute words.
#define MENU_SPRITE_INITIALIZE(sprite, shape)                                  \
  do {                                                                         \
    (sprite).oam.word.attr01 = (shape);                                        \
    (sprite).oam.word.attr23 = 0;                                              \
  } while (0)

/// The battle item list for unit `member`: `count` inventory entries (ended by
/// 0), five to a page, each with its icon and name: plain when usable,
/// TEXT_COLOR_YELLOW when Item_ClassifyUseAbility refuses it, TEXT_COLOR_RED
/// when broken; the window above names the one under the cursor
/// (MsgItemPlainName), or says it cannot be used in battle.
s32 BattleItemMenu_Run(s32 member, u16 *entries, s32 count) {
  struct WindowWork *work;
  s32 drawnFirstEntry;
  s32 drawnRow;
  s32 cursorHandle;
  struct RenderInput *messageWindow;
  s32 iconCount;
  s32 preferredRow;
  struct {
    s32 x, y;
  } cursorPosition;
  struct BattleMenuWork *navigation;
  struct RenderInput *window;
  s32 firstEntry;
  s32 selectedRow;
  s32 item;
  s32 iconTile;
  s32 pageTile;
  s32 unusableReason;
  s32 result;
  s32 i;
  struct QueuedSprite cursorStorage;
  struct MenuSprite *iconSprite;
  struct QueuedSprite *sprite;
  u16 messageText[64];
  s32 iconSlots[5];
  struct MenuSprite sprites[5];

  work = Ram_HeapSlots->windowWork;
  drawnFirstEntry = -1;
  drawnRow = -1;
  cursorHandle = VramCache_AllocEntry(128);
  messageWindow = Window_Create(0, 5, 30, 4,
                                WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                                    WINDOW_FLAG_BORDERED_CANVAS);
  iconCount = 0;
  do {
    firstEntry = NAVIGATION->page;
    selectedRow = NAVIGATION->row;
    preferredRow = NAVIGATION->preferredRow;
  } while (0);
  window = Window_Create(15, 9, 15, 11, 6);

  for (i = 0; i < 5; i++) {
    s32 y;
    u32 t = 0;

    y = i * 2;
    /* FAKEMATCH(dead): an empty once-only block after the row. */
    do {
    } while (0);
    iconSprite = &sprites[i];
    sprites[i].oam.word.attr01 = OBJ_SIZE_16X16;
    sprites[i].oam.word.attr23 = t;
    iconSprite->oam.f.x = (s16)window->x * 8 + 8;
    t = (s16)window->y;
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
    /* FAKEMATCH(temporary): sprite holds &cursorStorage. */
    sprite = &cursorStorage;

    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      work->holdTilemapUpload = 1;
      Window_HighlightRect((s16)window->x + 1,
                           (s16)window->y + drawnRow * 2 + 1, window->width - 2,
                           1, WINDOW_PALETTE);
      Text_FillCanvasBackground();
      if (count != 0) {
        if (Item_ClassifyUseAbility(member,
                                    entries[firstEntry + selectedRow]) == 2)
          MENU_TEXT_COPY((s32)MsgKey0d4f, messageText, CURSOR_TEXT_LENGTH);
        else
          MENU_TEXT_COPY(
              (entries[firstEntry + selectedRow] & INVENTORY_ITEM_ID_MASK) +
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
          Item_GetDirectFar(item);
          Text_SetColor(TEXT_COLOR_WHITE);
          if (Item_ClassifyUseAbility(member, item) != 0)
            Text_SetColor(TEXT_COLOR_YELLOW);
          else if (item & ITEM_BROKEN)
            Text_SetColor(TEXT_COLOR_RED);
          Text_DrawMessageAsTiles((item & INVENTORY_ITEM_ID_MASK) +
                                      (s32)MsgItemName,
                                  window, 16, i * 16);
          Text_SetColor(TEXT_COLOR_WHITE);
          sprites[i].oam.f.tile = ItemIcon_LoadIntoSlot(item, iconSlots[i]);
        }
        iconCount = i;
        drawnFirstEntry = firstEntry;
      }
      if (count > 5) {
        for (i = 0; i < (count + 4) / 5; i++) {
          iconTile = i + LIST_TILE_PAGE_NUMBER;
          if (i == firstEntry / 5)
            iconTile = i + LIST_TILE_PAGE_NUMBER_LIT;
          Window_SetTilemapEntry(
              window, iconTile, window->width - (count + 4) / 5 + i - 2, -1, 0);
        }
      }
      Window_HighlightRect((s16)window->x + 1,
                           (s16)window->y + selectedRow * 2 + 1,
                           window->width - 2, 1, HIGHLIGHT_PALETTE);
      work->dirty = 1;
      work->holdTilemapUpload = 0;
    }

    if (count > 5) {
      for (i = 0; i < (count + 4) / 5; i++) {
        iconTile = i + LIST_TILE_PAGE_NUMBER;
        if ((gFrameCount & 15) <= 11) {
          if (i == firstEntry / 5)
            iconTile = i + LIST_TILE_PAGE_NUMBER_LIT;
        }
        Window_SetTilemapEntry(window, iconTile,
                               window->width - (count + 4) / 5 + i - 2, -1, 0);
      }
      pageTile = -1;
      Window_SetTilemapEntry(window, LIST_TILE_PAGES_LEFT,
                             window->width - (count + 4) / 5 - 3, pageTile, 0);
      Window_SetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2,
                             pageTile, 0);
      do {
        work->dirty |= 2 << ((u32)((s16)window->y - 1) >> 2);
      } while (0);
    }

    for (i = 0; i < iconCount; i++) {
      iconSprite = &sprites[i];
      Oam_AddSpriteAtDepth(iconSprite, 240);
    }

    cursorPosition.x = (s16)window->x * 8 - 2;
    cursorPosition.y = (selectedRow * 2 + (s16)window->y) * 8 + 20;
    /* FAKEMATCH(dead): sprite set again to the &cursorStorage it holds. */
    sprite = &cursorStorage;
    ((u32 *)sprite)[1] = 0x40000000;
    ((u32 *)sprite)[2] = 0;
    sprite->oam.index =
        VramCache_Reload(cursorHandle, gResource_FixedBlockBTiles);
    sprite->oam.x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
    sprite->oam.y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0)
      Oam_AddSpriteAtDepth(sprite, 242);

    do {
      NAVIGATION->page = firstEntry;
      NAVIGATION->row = selectedRow;
      NAVIGATION->preferredRow = preferredRow;
    } while (0);
    navigation = NAVIGATION;
    if (gInput.pressed & KEY_A) {
      if (count != 0) {
        result = firstEntry + selectedRow;
        Item_GetDirectFar(entries[result]);
        unusableReason = 0;
        if ((entries[result] & ITEM_BROKEN) == 0) {
          unusableReason = Item_ClassifyUseAbility(member, entries[result]);
          if (unusableReason == 0)
            break;
        }
        Sound_PlayFar(SE_MENU_ERROR);
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
    } else if (navigation->timer == 0 || (gInput.pressed & KEY_B)) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }

    if (count != 0) {
      if (gInput.repeat & KEY_DOWN) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        selectedRow++;
        if (selectedRow == 5 || firstEntry + selectedRow == count)
          selectedRow = 0;
        preferredRow = selectedRow;
      } else if (gInput.repeat & KEY_UP) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        selectedRow--;
        if (selectedRow < 0) {
          if (firstEntry == (count - 1) / 5 * 5)
            selectedRow = count - firstEntry - 1;
          else
            selectedRow = 4;
        }
        preferredRow = selectedRow;
      } else if (gInput.repeat & KEY_RIGHT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
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
      } else if (gInput.repeat & KEY_LEFT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
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
  VramCache_FreeEntry(cursorHandle);
  for (i = 0; i < 5; i++)
    VramCache_FreeEntry(iconSlots[i]);
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
