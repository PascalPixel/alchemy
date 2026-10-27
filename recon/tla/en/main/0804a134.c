// Draft, differ 456 of 817: the battle Psynergy menu, The Broken Seal's
// BattlePsynergyMenu_Run (games/tbs/src/window/screens/psynergy_menu.c) on
// this game's work, messages and tiles; the frame tees are copied with
// Func_08049a04 (The Broken Seal's Vram_CopyTile), and an ability not usable
// in battle is refused without the dead store. Structure follows the
// listing; left, as in Func_08049a30: the sprite setup loop's window reload,
// the frame and the stack slots.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"

extern u8 MsgAbilityNothing[];
extern u8 Msg_0885[];
extern u8 Msg_0d48[];

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
  u8 unk00[4];
  u32 attr01;
  u32 attr23;
};

struct PsynergyMenuOam {
  u8 unk00[4];
  u8 y;
  u8 attr0high;
  u16 x : 9;
  u16 attr1high : 7;
  u16 tile : 10;
  u16 attr2high : 6;
};

#define NAVIGATION ((struct PsynergyMenuNavigation *)Ram_HeapSlots->unke4[0])

extern const u8 gResource_FixedBlockBTiles[];

s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 entry, const void *tiles);
struct PsynergyMenuUnit *Unit_Get(s32 member);
struct PsynergyMenuWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                           s32 flags);
void Func_08049a04(u32 sourceTile, u32 destinationTile);
void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 palette);
void Ui_FillVramBlockPattern(void);
void UiText_CopyMessageString(s32 message, u16 *text, s32 capacity);
void UiText_RenderWideStringAtOffset(u16 *text,
                                     struct PsynergyMenuWindow *window, s32 x,
                                     s32 y);
void RenderOutput_RedrawSavedRect(struct PsynergyMenuWindow *window);
struct PsynergyAbility *BattleAction_Get(s32 id);
void UiWindow_SetTilemapEntry(struct PsynergyMenuWindow *window, s32 entry,
                              s32 x, s32 y, s32 flags);
void Ability_LoadGlyph(s32 ability, s32 mode, s32 *slot, s32 *glyph, s32 count);
void Func_08041f70(s32 color);
void UiText_DrawCharacterAtOffset(s32 message,
                                  struct PsynergyMenuWindow *window, s32 x,
                                  s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 digits,
                               struct PsynergyMenuWindow *window, s32 x, s32 y);
void UiWindow_DrawThreeTileColumn(struct PsynergyMenuWindow *window, s32 x,
                                  s32 y, s32 pattern, s32 flags);
void Runtime_PushSlotEntry(void *sprite, s32 depth);
void Audio_PlayCue(s32 cue);
void Input_ResetKeyRepeat(void);
void WaitTicks(s32 ticks);
void UiWork_Finalize(void *window, s32 mode);
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
  s32 cursorX;
  s32 cursorY;
  struct PsynergyMenuNavigation *navigation;
  struct PsynergyMenuWindow *window;
  s32 selectedRow;
  s32 encodedAction;
  s32 iconTile;
  s32 rangePattern;
  s32 result;
  s32 i;
  struct PsynergyMenuOam cursor;
  u16 messageText[64];
  s32 iconSlots[5];
  struct PsynergyMenuOam sprites[5];
  s32 glyph;

  work = (struct PsynergyMenuWork *)Ram_HeapSlots->windowTiles;
  drawnRow = -1;
  drawnFirstEntry = -1;
  cursorHandle = VramCache_AllocEntry(128);
  unit = Unit_Get(unitId);
  messageWindow = UiWindow_Create(0, 5, 30, 4, 42);
  iconCount = 5;
  navigation = NAVIGATION;
  firstEntry = navigation->page;
  selectedRow = navigation->row;
  preferredRow = navigation->preferredRow;
  window = UiWindow_Create(9, 9, 21, 11, 6);

  for (i = 0; i < 5; i++) {
    s32 y = i * 2;
    struct PsynergyMenuOam *iconSprite = &sprites[i];

    ((struct PsynergyMenuSprite *)&sprites[i])->attr01 = 0x40000000;
    ((struct PsynergyMenuSprite *)&sprites[i])->attr23 = 0;
    iconSprite->x = window->x * 8 + 8;
    iconSprite->y = (y + window->y) * 8 + 4;
  }
  for (i = 0; i < 5; i++) {
    iconSlots[i] = VramCache_AllocEntry(128);
    sprites[i].tile = VramCache_Reload(iconSlots[i], (const void *)-1);
  }
  Func_08049a04(0xf018, 0x200);
  Func_08049a04(0xf018, 0x201);
  Func_08049a04(0xf019, 0x210);
  Func_08049a04(0xf019, 0x211);

  for (;;) {
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      work->holdTilemapUpload = 1;
      Ui_SetRectHighlight(window->x + 1, window->y + drawnRow * 2 + 1,
                          window->width - 2, 1, 15);
      Ui_FillVramBlockPattern();
      if (count != 0)
        UiText_CopyMessageString((actions[firstEntry + selectedRow] & 0x3fff) +
                                     (s32)Msg_0885,
                                 messageText, 52);
      else
        UiText_CopyMessageString((s32)Msg_0d48, messageText, 52);
      UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;
      if (firstEntry != drawnFirstEntry) {
        RenderOutput_RedrawSavedRect(window);
        for (i = 0; i < 5; i++) {
          struct PsynergyAbility *action;

          encodedAction = actions[firstEntry + i];
          if (encodedAction == 0)
            break;
          action = BattleAction_Get(encodedAction);
          UiWindow_SetTilemapEntry(window, 0xf01f, 11, i * 2, 0);
          UiWindow_SetTilemapEntry(window, 0xf01e, 12, i * 2, 0);
          Ability_LoadGlyph(encodedAction & 0x3fff, 0, &iconSlots[i], &glyph,
                            1);
          sprites[i].tile = glyph;
          if ((action->flags & 0x80) == 0)
            Func_08041f70(4);
          else if (action->ppCost > unit->pp)
            Func_08041f70(2);
          else if (unit->psySeal != 0)
            Func_08041f70(9);
          work->textPalette = 5;
          UiText_DrawCharacterAtOffset(encodedAction + (s32)MsgAbilityNothing,
                                       window, 16, i * 16);
          UiText_DrawNumberAtOffset(action->ppCost, 2, window, 104, i * 16);
          Func_08041f70(15);
          work->textPalette = 15;
          if (action->element != 4)
            UiWindow_SetTilemapEntry(window, action->element + 0x5001, 15,
                                     i * 2, 0);
          rangePattern = action->range;
          if (rangePattern == 255)
            rangePattern = 11;
          else
            rangePattern--;
          UiWindow_DrawThreeTileColumn(window, 16, i * 2, rangePattern, 0);
        }
        iconCount = i;
        drawnFirstEntry = firstEntry;
      }
      if (count > 5) {
        for (i = 0; i < (count + 4) / 5; i++) {
          iconTile = i + 0xf301;
          if (i == firstEntry / 5)
            iconTile = i + 0xf30b;
          UiWindow_SetTilemapEntry(
              window, iconTile, window->width - (count + 4) / 5 + i - 2, -1, 0);
        }
      }
      Ui_SetRectHighlight(window->x + 1, window->y + selectedRow * 2 + 1,
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
        UiWindow_SetTilemapEntry(
            window, iconTile, window->width - (count + 4) / 5 + i - 2, -1, 0);
      }
      UiWindow_SetTilemapEntry(window, 0xf334,
                               window->width - (count + 4) / 5 - 3, -1, 0);
      UiWindow_SetTilemapEntry(window, 0xf335, window->width - 2, -1, 0);
      work->dirty |= 2 << ((u32)(window->y - 1) >> 2);
    }

    for (i = 0; i < iconCount; i++)
      Runtime_PushSlotEntry(&sprites[i], 240);

    cursorX = window->x * 8 - 4;
    cursorY = (selectedRow * 2 + window->y) * 8 + 20;
    ((struct PsynergyMenuSprite *)&cursor)->attr01 = 0x40000000;
    ((struct PsynergyMenuSprite *)&cursor)->attr23 = 0;
    cursor.tile = VramCache_Reload(cursorHandle, gResource_FixedBlockBTiles);
    cursor.x = cursorX + ((gFrameCount & 4) >> 1) - 4;
    cursor.y = cursorY - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0)
      Runtime_PushSlotEntry(&cursor, 242);

    navigation = NAVIGATION;
    navigation->page = firstEntry;
    navigation->row = selectedRow;
    navigation->preferredRow = preferredRow;
    if (gInput.pressed & 1) {
      if (count != 0) {
        result = firstEntry + selectedRow;
        if (BattleAction_Get(actions[result])->flags & 0x80)
          break;
      } else {
        result = -1;
        break;
      }
    } else if (navigation->timer == 0 || (gInput.pressed & 2)) {
      Audio_PlayCue(113);
      result = -1;
      break;
    }

    if (count != 0) {
      if (gInput.repeat & 0x80) {
        Audio_PlayCue(111);
        selectedRow++;
        if (selectedRow == 5 || firstEntry + selectedRow == count)
          selectedRow = 0;
        preferredRow = selectedRow;
      } else if (gInput.repeat & 0x40) {
        Audio_PlayCue(111);
        selectedRow--;
        if (selectedRow < 0) {
          if (firstEntry == (count - 1) / 5 * 5)
            selectedRow = count - firstEntry - 1;
          else
            selectedRow = 4;
        }
        preferredRow = selectedRow;
      } else if (gInput.repeat & 0x10) {
        Audio_PlayCue(111);
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
        Audio_PlayCue(111);
        Input_ResetKeyRepeat();
        if (firstEntry != 0) {
          selectedRow = preferredRow;
          firstEntry -= 5;
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
    WaitTicks(1);
  }

  UiWork_Finalize(messageWindow, 1);
  UiWork_Finalize(window, 1);
  WaitTicks(1);
  for (i = 0; i < 5; i++)
    Resource_ResetEntry(iconSlots[i]);
  Resource_ResetEntry(cursorHandle);
  WaitTicks(1);
  return result;
}
