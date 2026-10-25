// Draft, differ 617 of 790: the battle item menu, The Broken Seal's
// BattleItemMenu_Run (games/tbs/src/window/screens/item_menu.c) on this
// game's work, messages and tiles; the frame tees are copied with
// Func_08049a04 (The Broken Seal's Vram_CopyTile). Structure follows the
// listing; left: the native reloads the list window inside the sprite setup
// loop (here its x is hoisted), its frame is 8 bytes larger, and the stack
// slots and allocation differ from there.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"

struct ItemMenuWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct ItemMenuWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04[2];
  u8 holdTilemapUpload;
};

struct ItemMenuNavigation {
  u8 unk00[0x30];
  s32 row;
  s32 page;
  s32 preferredRow;
  u8 unk3c[0x10];
  s32 timer;
};

struct ItemMenuSprite {
  u8 unk00[4];
  u32 attr01;
  u32 attr23;
};

struct ItemMenuOam {
  u8 unk00[4];
  u8 y;
  u8 attr0high;
  u16 x : 9;
  u16 attr1high : 7;
  u16 tile : 10;
  u16 attr2high : 6;
};

#define NAVIGATION ((struct ItemMenuNavigation *)Ram_HeapSlots->unke4[0])
#define ITEM_BROKEN 0x400

// Sets a menu sprite's attribute words, as The Broken Seal's
// MenuSprite_Initialize.
#define MENU_SPRITE_INITIALIZE(sprite, shape)                                  \
  do {                                                                         \
    ((struct ItemMenuSprite *)&(sprite))->attr01 = (shape);                    \
    ((struct ItemMenuSprite *)&(sprite))->attr23 = 0;                          \
  } while (0)

extern const u8 gResource_FixedBlockBTiles[];

s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 entry, const void *tiles);
struct ItemMenuWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                       s32 flags);
void Func_08049a04(u32 sourceTile, u32 destinationTile);
void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 palette);
void Ui_FillVramBlockPattern(void);
s32 Item_ClassifyUseAbility(s32 member, s32 item);
void *Item_Get(s32 item);
void UiText_CopyMessageString(s32 message, u16 *text, s32 capacity);
void UiText_RenderWideStringAtOffset(u16 *text, struct ItemMenuWindow *window,
                                     s32 x, s32 y);
void RenderOutput_RedrawSavedRect(struct ItemMenuWindow *window);
void Func_08041f70(s32 color);
void UiText_DrawCharacterAtOffset(s32 message, struct ItemMenuWindow *window,
                                  s32 x, s32 y);
s32 ItemIcon_LoadIntoSlot(s32 item, s32 slot);
void UiWindow_SetTilemapEntry(struct ItemMenuWindow *window, s32 entry, s32 x,
                              s32 y, s32 flags);
void Runtime_PushSlotEntry(void *sprite, s32 depth);
void Audio_PlayCue(s32 cue);
void Input_ResetKeyRepeat(void);
void WaitFrames(s32 frames);
void UiWork_Finalize(void *window, s32 mode);
void Resource_ResetEntry(s32 slot);

s32 Func_08049a30(s32 member, u16 *entries, s32 count) {
  struct ItemMenuWork *work;
  s32 drawnFirstEntry;
  s32 drawnRow;
  s32 cursorHandle;
  struct ItemMenuWindow *messageWindow;
  s32 iconCount;
  s32 preferredRow;
  s32 cursorX;
  s32 cursorY;
  struct ItemMenuNavigation *navigation;
  struct ItemMenuWindow *window;
  s32 firstEntry;
  s32 selectedRow;
  s32 item;
  s32 iconTile;
  s32 unusableReason;
  s32 result;
  s32 i;
  struct ItemMenuOam cursorSprite;
  u16 messageText[64];
  s32 iconSlots[5];
  struct ItemMenuOam sprites[5];

  work = (struct ItemMenuWork *)Ram_HeapSlots->windowTiles;
  drawnFirstEntry = -1;
  drawnRow = -1;
  cursorHandle = VramCache_AllocEntry(128);
  messageWindow = UiWindow_Create(0, 5, 30, 4, 42);
  iconCount = 0;
  navigation = NAVIGATION;
  firstEntry = navigation->page;
  selectedRow = navigation->row;
  preferredRow = navigation->preferredRow;
  window = UiWindow_Create(15, 9, 15, 11, 6);

  for (i = 0; i < 5; i++) {
    s32 y = i * 2;
    struct ItemMenuOam *iconSprite = &sprites[i];

    MENU_SPRITE_INITIALIZE(sprites[i], 0x40000000);
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
      if (count != 0) {
        if (Item_ClassifyUseAbility(member,
                                    entries[firstEntry + selectedRow]) == 2)
          UiText_CopyMessageString(0xd4f, messageText, 52);
        else
          UiText_CopyMessageString((entries[firstEntry + selectedRow] & 0x1ff) +
                                       0x92,
                                   messageText, 52);
      } else {
        UiText_CopyMessageString(0xd46, messageText, 52);
      }
      UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;
      if (firstEntry != drawnFirstEntry) {
        RenderOutput_RedrawSavedRect(window);
        for (i = 0; i < 5; i++) {
          item = entries[firstEntry + i];
          if (item == 0)
            break;
          Item_Get(item);
          Func_08041f70(15);
          if (Item_ClassifyUseAbility(member, item) != 0)
            Func_08041f70(4);
          else if (item & ITEM_BROKEN)
            Func_08041f70(2);
          UiText_DrawCharacterAtOffset((item & 0x1ff) + 0x25f, window, 16,
                                       i * 16);
          Func_08041f70(15);
          sprites[i].tile = ItemIcon_LoadIntoSlot(item, iconSlots[i]);
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

    cursorX = window->x * 8 - 2;
    cursorY = (selectedRow * 2 + window->y) * 8 + 20;
    ((struct ItemMenuSprite *)&cursorSprite)->attr01 = 0x40000000;
    ((struct ItemMenuSprite *)&cursorSprite)->attr23 = 0;
    cursorSprite.tile =
        VramCache_Reload(cursorHandle, gResource_FixedBlockBTiles);
    cursorSprite.x = cursorX + ((gFrameCount & 4) >> 1) - 4;
    cursorSprite.y = cursorY - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0)
      Runtime_PushSlotEntry(&cursorSprite, 242);

    navigation = NAVIGATION;
    navigation->page = firstEntry;
    navigation->row = selectedRow;
    navigation->preferredRow = preferredRow;
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
        Audio_PlayCue(114);
        if (unusableReason == 2)
          UiText_CopyMessageString(0xd4f, messageText, 32);
        else if (entries[result] & ITEM_BROKEN)
          UiText_CopyMessageString(0xd4d, messageText, 32);
        else
          UiText_CopyMessageString(0xd4c, messageText, 32);
        Ui_FillVramBlockPattern();
        UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
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
    WaitFrames(1);
  }

  UiWork_Finalize(messageWindow, 1);
  UiWork_Finalize(window, 1);
  WaitFrames(1);
  Resource_ResetEntry(cursorHandle);
  for (i = 0; i < 5; i++)
    Resource_ResetEntry(iconSlots[i]);
  WaitFrames(1);
  return result;
}
