#include "types.h"
#include "edition.h"
#include "window_tile.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"

extern u8 MsgAbilityNothing[];
extern u8 MsgAbilityDescription[];
extern u8 MsgKey0d48[];

struct PsynergyMenuWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct PsynergyMenuWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04[2];
  u8 holdTilemapUpload;
  u8 textPalette;
};

struct PsynergyMenuNavigation {
  u8 unk00[0x30];
  s32 row;
  s32 page;
  s32 preferredRow;
  u8 unk3c[0x10];
  s32 timer;
};

struct PsynergyMenuUnit {
  u8 unk000[0x3a];
  s16 pp;
  u8 unk03c[0x101];
  u8 psySeal;
};

struct PsynergyAbility {
  u8 unk00;
  u8 flags; ///< bit 7: usable in battle
  u8 element;
  u8 unk03[5];
  u8 range;
  u8 ppCost;
};

struct PsynergyMenuSprite {
  struct PsynergyMenuSprite *next;
  union {
    struct {
      unsigned long attr01;
      unsigned long attr23;
    } word;
    struct {
      u16 y : 8;
      u16 affine : 2;
      u16 mode : 2;
      u16 mosaic : 1;
      u16 colors : 1;
      u16 shape : 2;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unused;
    } f;
  } oam;
};

struct PsynergyMenuOam {
  struct PsynergyMenuOam *next;
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 colors : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unused;
};

#define NAVIGATION ((struct PsynergyMenuNavigation *)Ram_HeapSlots->unke4[0])

// How much of the description under the cursor is copied: more outside
// Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define CURSOR_TEXT_LENGTH 32
#else
#define CURSOR_TEXT_LENGTH 52
#endif

// Japanese decodes the text under the cursor raw where the other languages
// copy it expanded.
#if LANGUAGE != LANGUAGE_JAPANESE
#define CURSOR_TEXT_COPY Message_CopyExpanded
#else
#define CURSOR_TEXT_COPY Message_DecodeRaw
#endif

// The list is a tile wider in Spanish and French, and its columns move with it.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
#define LIST_WINDOW_X 8
#define LIST_WINDOW_WIDTH 22
#define COST_COLUMN 12
#define COST_X 112
#define RANGE_COLUMN 16
#else
#define LIST_WINDOW_X 9
#define LIST_WINDOW_WIDTH 21
#define COST_COLUMN 11
#define COST_X 104
#define RANGE_COLUMN 15
#endif

// Sets a menu sprite's attribute words, as The Broken Seal's
// MenuSprite_Initialize.
#define MENU_SPRITE_INITIALIZE(sprite, shape)                                  \
  do {                                                                         \
    (sprite).oam.word.attr01 = (shape);                                        \
    (sprite).oam.word.attr23 = 0;                                              \
  } while (0)

// The menu's page, row and preferred row, as The Broken Seal's
// BATTLE_MENU_GET_CURSOR and BATTLE_MENU_SET_CURSOR.
#define MENU_GET_CURSOR(page_, row_, preferred)                                \
  do {                                                                         \
    (page_) = NAVIGATION->page;                                                \
    (row_) = NAVIGATION->row;                                                  \
    (preferred) = NAVIGATION->preferredRow;                                    \
  } while (0)

#define MENU_SET_CURSOR(page_, row_, preferred)                                \
  do {                                                                         \
    NAVIGATION->page = (page_);                                                \
    NAVIGATION->row = (row_);                                                  \
    NAVIGATION->preferredRow = (preferred);                                    \
  } while (0)

// Marks a window's tilemap row for upload, as The Broken Seal's
// Window_MarkTilemapRow.
#define WINDOW_MARK_TILEMAP_ROW(work, row)                                     \
  do {                                                                         \
    (work)->dirty |= 2 << ((u32)(row) >> 2);                                   \
  } while (0)

extern const u8 gResource_FixedBlockBTiles[];

s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 entry, const void *tiles);
struct PsynergyMenuUnit *Unit_Get(s32 member);
struct PsynergyMenuWindow *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                         s32 flags);
void Vram_CopyTile(u32 sourceTile, u32 destinationTile);
void Window_HighlightRect(s32 x, s32 y, s32 width, s32 height, s32 palette);
void Vram_FillWindowBlockPattern(void);
void Message_CopyExpanded(s32 message, u16 *text, s32 capacity);
void Message_DecodeRaw(s32 message, u16 *text, s32 capacity);
void Text_DrawWideStringAtOffset(u16 *text, struct PsynergyMenuWindow *window,
                                 s32 x, s32 y);
void WindowSprite_RedrawSavedRect(struct PsynergyMenuWindow *window);
struct PsynergyAbility *BattleAction_Get(s32 id);
void Window_SetTilemapEntry(struct PsynergyMenuWindow *window, s32 entry, s32 x,
                            s32 y, s32 flags);
void Ability_LoadGlyph(s32 ability, s32 mode, s32 *slot, s32 *glyph, s32 count);
void Func_08041f70(s32 color);
void Text_DrawCharacterAtOffset(s32 message, struct PsynergyMenuWindow *window,
                                s32 x, s32 y);
void Text_DrawNumberAtOffset(s32 value, s32 digits,
                             struct PsynergyMenuWindow *window, s32 x, s32 y);
void Window_DrawThreeTileColumn(struct PsynergyMenuWindow *window, s32 x, s32 y,
                                s32 pattern, s32 flags);
void Oam_AddSpriteAtDepth(void *sprite, s32 depth);
void Sound_PlayFar(s32 cue);
void Input_ResetKeyRepeat(void);
void wait(s32 frames);
void Window_Close(void *window, s32 mode);
void Resource_ResetEntry(s32 slot);

s32 Func_0804a134(s32 unitId, u16 *actions, s32 count) {
  struct PsynergyMenuWork *work;
  s32 drawnFirstEntry;
  s32 firstEntry;
  s32 drawnRow;
  s32 cursorHandle;
  struct PsynergyMenuUnit *unit;
  struct PsynergyMenuWindow *messageWindow;
  s32 iconCount;
  s32 preferredRow;
  struct {
    s32 x, y;
  } cursorPosition;
  struct PsynergyMenuNavigation *navigation;
  struct PsynergyMenuWindow *window;
  s32 selectedRow;
  s32 encodedAction;
  s32 iconTile;
  s32 pageTile;
  s32 rangePattern;
  s32 result;
  s32 i;
  struct PsynergyMenuSprite cursor;
  struct PsynergyMenuSprite *iconSprite;
  u16 messageText[64];
  s32 iconSlots[5];
  struct PsynergyMenuSprite sprites[5];
  s32 glyph;

  work = (struct PsynergyMenuWork *)Ram_HeapSlots->windowTiles;
  drawnRow = -1;
  drawnFirstEntry = -1;
  cursorHandle = VramCache_AllocEntry(128);
  unit = Unit_Get(unitId);
  messageWindow = Window_Create(0, 5, 30, 4, 42);
  iconCount = 5;
  MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
  window = Window_Create(LIST_WINDOW_X, 9, LIST_WINDOW_WIDTH, 11, 6);

  for (i = 0; i < 5; i++) {
    s32 y;
    // t holds the attribute zero, then is reused for the window's y; as plain
    // stores, 2 of 817 instructions differ in tla-en.
    u32 t = 0;

    y = i * 2;
    /* FAKEMATCH(dead): an empty once-only block after the row, which keeps it
     * and the zero ahead of the stores; without it, 10 of 817 instructions
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
    /* FAKEMATCH(temporary): iconSprite holds &cursor; as cursor used directly,
     * 304 of 817 instructions differ in tla-en. */
    iconSprite = &cursor;
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      work->holdTilemapUpload = 1;
      Window_HighlightRect(window->x + 1, window->y + drawnRow * 2 + 1,
                           window->width - 2, 1, 15);
      Vram_FillWindowBlockPattern();
      if (count != 0)
        CURSOR_TEXT_COPY((actions[firstEntry + selectedRow] & 0x3fff) +
                             (s32)MsgAbilityDescription,
                         messageText, CURSOR_TEXT_LENGTH);
      else
        CURSOR_TEXT_COPY((s32)MsgKey0d48, messageText, CURSOR_TEXT_LENGTH);
      Text_DrawWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;
      if (firstEntry != drawnFirstEntry) {
        WindowSprite_RedrawSavedRect(window);
        for (i = 0; i < 5; i++) {
          struct PsynergyAbility *action;

          encodedAction = actions[firstEntry + i];
          if (encodedAction == 0)
            break;
          action = BattleAction_Get(encodedAction);
          Window_SetTilemapEntry(window, WINDOW_TILE_PP_LABEL, COST_COLUMN,
                                 i * 2, 0);
          Window_SetTilemapEntry(window, WINDOW_TILE_PP_LABEL_END,
                                 COST_COLUMN + 1, i * 2, 0);
          Ability_LoadGlyph(encodedAction & 0x3fff, 0, &iconSlots[i], &glyph,
                            1);
          sprites[i].oam.f.tile = glyph;
          if ((action->flags & 0x80) == 0)
            Func_08041f70(4);
          else if (action->ppCost > unit->pp)
            Func_08041f70(2);
          else if (unit->psySeal != 0)
            Func_08041f70(9);
          work->textPalette = 5;
          Text_DrawCharacterAtOffset(encodedAction + (s32)MsgAbilityNothing,
                                     window, 16, i * 16);
          Text_DrawNumberAtOffset(action->ppCost, 2, window, COST_X, i * 16);
          Func_08041f70(15);
          work->textPalette = 15;
          if (action->element != 4)
            Window_SetTilemapEntry(window, action->element + 0x5001,
                                   RANGE_COLUMN, i * 2, 0);
          rangePattern = action->range;
          if (rangePattern == 255)
            rangePattern = 11;
          else
            rangePattern--;
          Window_DrawThreeTileColumn(window, RANGE_COLUMN + 1, i * 2,
                                     rangePattern, 0);
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
      WINDOW_MARK_TILEMAP_ROW(work, window->y - 1);
    }

    for (i = 0; i < iconCount; i++)
      Oam_AddSpriteAtDepth(&sprites[i], 240);

    cursorPosition.x = window->x * 8 - 4;
    cursorPosition.y = (selectedRow * 2 + window->y) * 8 + 20;
    ((u32 *)iconSprite)[1] = 0x40000000;
    ((u32 *)iconSprite)[2] = 0;
    iconSprite->oam.f.tile =
        VramCache_Reload(cursorHandle, gResource_FixedBlockBTiles);
    iconSprite->oam.f.x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
    iconSprite->oam.f.y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0)
      Oam_AddSpriteAtDepth(iconSprite, 242);

    MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
    navigation = NAVIGATION;
    if (gInput.pressed & 1) {
      if (count != 0) {
        result = firstEntry + selectedRow;
        if ((BattleAction_Get(actions[result])->flags & 0x80) == 0) {
          // Not usable in battle: the menu stays open.
        } else {
          break;
        }
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
  for (i = 0; i < 5; i++)
    Resource_ResetEntry(iconSlots[i]);
  Resource_ResetEntry(cursorHandle);
  wait(1);
  return result;
}
