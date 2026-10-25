// Draft, differ 1062 of 984: the battle summon menu, The Broken Seal's
// BattleSummonMenu_Run (games/tbs/src/window/screens/summon_menu.c) on this
// game's work and messages. The list window stands at column 13, 17 tiles
// wide; the standby counts above it are tiles (counts of ten and more from
// tile 0xf196), and each summon's cost is drawn from column 11. Structure
// follows the listing; left: the native reloads the list window from its stack
// slot inside the sprite setup loop where the draft hoists it, and keeps
// drawnFirstEntry in r9, so the allocation differs throughout.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"

extern u8 MsgAbilityNothing[];
extern u8 Msg_0885[];

struct SummonMenuWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct SummonMenuWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04[2];
  u8 holdTilemapUpload;
};

struct SummonMenuNavigation {
  u8 unk00[0x30];
  s32 row;
  s32 page;
  s32 preferredRow;
  u8 unk3c[0x10];
  s32 timer;
  u8 unk50[0x88];
  s32 tutorialStep;
  s32 tutorialDelay;
};

struct SummonDefinition {
  u16 abilityId;
  u8 unk02[2];
  u8 djinnRequired[4];
};

struct SummonMenuSprite {
  u8 unk00[4];
  u32 attr01;
  u32 attr23;
};

struct SummonMenuOam {
  u8 unk00[4];
  u8 y;
  u8 attr0high;
  u16 x : 9;
  u16 attr1high : 7;
  u16 tile : 10;
  u16 attr2high : 6;
};

#define NAVIGATION ((struct SummonMenuNavigation *)Ram_HeapSlots->unke4[0])
#define SUMMON_END 32

extern const u8 gResource_FixedBlockBTiles[];

s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 entry, const void *tiles);
struct SummonMenuWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                         s32 flags);
s32 Summon_ListLearnedFar(u8 *summons);
const struct SummonDefinition *SummonDefinition_Get(s32 summon);
void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 palette);
void Ui_FillVramBlockPattern(void);
void UiText_CopyMessageString(s32 message, u16 *text, s32 capacity);
void UiText_RenderWideStringAtOffset(u16 *text, struct SummonMenuWindow *window,
                                     s32 x, s32 y);
void RenderOutput_RedrawSavedRect(struct SummonMenuWindow *window);
void UiWindow_SetTilemapEntry(struct SummonMenuWindow *window, s32 entry, s32 x,
                              s32 y, s32 flags);
void Ability_LoadGlyph(s32 ability, s32 mode, s32 *slot, s32 *glyph, s32 count);
void Func_08041f70(s32 color);
void UiText_DrawCharacterAtOffset(s32 message, struct SummonMenuWindow *window,
                                  s32 x, s32 y);
void Func_0803c274(struct SummonMenuWindow *window, s32 character, s32 x, s32 y,
                   s32 mode);
void Runtime_PushSlotEntry(void *sprite, s32 depth);
void Audio_PlayCue(s32 cue);
void Input_ResetKeyRepeat(void);
void WaitFrames(s32 frames);
void Resource_ResetEntry(s32 slot);
void UiWork_Finalize(void *window, s32 mode);

s32 Func_08049154(s32 unused0, s32 unused1, const u8 *standby) {
  struct SummonMenuOam cursorSprite;
  struct SummonMenuOam sprites[4];
  u8 learnedSummons[33];
  u8 orderedSummons[33];
  u16 text[64];
  s32 iconSlots[4];
  u8 visibleSummons[4];
  s32 glyph;
  s32 count;
  struct SummonMenuWindow *window;
  struct SummonMenuWork *work;
  s32 drawnRow;
  s32 drawnFirstEntry;
  s32 selectedRow;
  s32 firstEntry;
  s32 cursorHandle;
  struct SummonMenuWindow *description;
  struct SummonMenuWindow *counts;
  s32 mask;
  s32 preferredRow;
  s32 cursorY;
  s32 cursorX;
  const struct SummonDefinition *summon;
  struct SummonMenuNavigation *navigation;
  s32 orderedCount;
  s32 id;
  s32 tile;
  s32 affordable;
  s32 pressed;
  s32 repeated;
  s32 result;
  s32 index;

  work = (struct SummonMenuWork *)Ram_HeapSlots->windowTiles;
  drawnRow = -1;
  drawnFirstEntry = -1;
  cursorHandle = (s16)VramCache_AllocEntry(128);
  description = UiWindow_Create(0, 4, 30, 4, 42);
  counts = UiWindow_Create(20, 8, 10, 3, 6);
  mask = 0;
  navigation = NAVIGATION;
  firstEntry = navigation->page;
  selectedRow = navigation->row;
  preferredRow = navigation->preferredRow;
  window = UiWindow_Create(13, 11, 17, 9, 6);

  for (index = 0; index < 4; index++) {
    ((struct SummonMenuSprite *)&sprites[index])->attr01 = 0x40000000;
    ((struct SummonMenuSprite *)&sprites[index])->attr23 = 0;
    sprites[index].x = window->x * 8 + 8;
    sprites[index].y = (index * 2 + window->y) * 8 + 4;
  }
  for (index = 0; index < 4; index++) {
    s32 slot = VramCache_AllocEntry(128);

    iconSlots[index] = slot;
    sprites[index].tile = VramCache_Reload(slot, (const void *)-1);
  }

  count = Summon_ListLearnedFar(learnedSummons);
  /* FAKEMATCH(one-pass): the first scan in a block that runs once, as in The
   * Broken Seal. */
  do {
    s32 i;

    orderedCount = 0;
    for (i = count - 1; i >= 0; i--) {
      const struct SummonDefinition *definition;
      s32 n;
      s32 k;

      k = learnedSummons[i];
      definition = SummonDefinition_Get(k);
      for (n = 0; n <= 3; n++) {
        if (definition->djinnRequired[n] > standby[n])
          break;
      }
      if (n == 4) {
        orderedSummons[orderedCount++] = k;
        learnedSummons[i] = SUMMON_END;
      }
    }
  } while (0);
  {
    s32 i;
    s32 k;

    for (i = 0; i < count; i++) {
      k = learnedSummons[i];
      if (k != SUMMON_END)
        orderedSummons[orderedCount++] = k;
    }
  }
  orderedSummons[orderedCount] = SUMMON_END;

  for (;;) {
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      work->holdTilemapUpload = 1;
      Ui_SetRectHighlight(window->x + 1, window->y + drawnRow * 2 + 1,
                          window->width - 2, 1, 15);
      Ui_FillVramBlockPattern();
      {
        const struct SummonDefinition *definition;
        s32 element;

        definition =
            SummonDefinition_Get(orderedSummons[firstEntry + selectedRow]);
        UiText_CopyMessageString(definition->abilityId + (s32)Msg_0885, text,
                                 52);
        UiText_RenderWideStringAtOffset(text, description, 0, 4);
        mask = 0;
        drawnRow = selectedRow;
        for (element = 0; element <= 3; element++) {
          if (definition->djinnRequired[element] != 0)
            mask |= 1 << element;
        }
      }

      if (firstEntry != drawnFirstEntry) {
        RenderOutput_RedrawSavedRect(window);
        {
          s32 element;

          for (element = 0; element <= 3; element++) {
            UiWindow_SetTilemapEntry(counts, element + 0x5001, element * 2, 0,
                                     0);
            if (standby[element] > 9)
              UiWindow_SetTilemapEntry(counts, standby[element] + 0xf196,
                                       element * 2 + 1, 0, 0);
            else
              UiWindow_SetTilemapEntry(counts,
                                       (standby[element] + '0') | 0xf000,
                                       element * 2 + 1, 0, 0);
          }
        }
        {
          s32 element;
          s32 col;
          s32 y;

          index = 0;
          if ((id = orderedSummons[firstEntry + index]) != SUMMON_END) {
          next:
            summon = SummonDefinition_Get(id);
            for (element = 0; element <= 3; element++) {
              if (summon->djinnRequired[element] > standby[element])
                break;
            }
            affordable = element == 4;
            Ability_LoadGlyph(summon->abilityId & 0x3fff, 0, &iconSlots[index],
                              &glyph, 1);
            y = index * 2;
            sprites[index].tile = glyph;
            if (!affordable)
              Func_08041f70(2);
            UiText_DrawCharacterAtOffset(SummonDefinition_Get(id)->abilityId +
                                             (s32)MsgAbilityNothing,
                                         window, 16, index * 16);
            col = 0;
            for (element = 0; element <= 3; element++) {
              if (summon->djinnRequired[element] != 0) {
                UiWindow_SetTilemapEntry(window, element + 0x5001, col * 2 + 11,
                                         y, 0);
                Func_0803c274(window, summon->djinnRequired[element] + '0',
                              col * 2 + 12, y, 0);
                col++;
              }
            }
            Func_08041f70(15);
            visibleSummons[index++] = 1;
            if (index <= 3 &&
                (id = orderedSummons[firstEntry + index]) != SUMMON_END)
              goto next;
          }
          while (index <= 3)
            visibleSummons[index++] = 0;
        }
        drawnFirstEntry = firstEntry;
      }

      if (count > 4) {
        for (index = 0; index < (count + 3) / 4; index++) {
          tile = index + 0xf301;
          if (index == firstEntry / 4)
            tile = index + 0xf30b;
          UiWindow_SetTilemapEntry(
              window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
        }
      }
      Ui_SetRectHighlight(window->x + 1, window->y + selectedRow * 2 + 1,
                          window->width - 2, 1, 14);
      work->dirty = 1;
      work->holdTilemapUpload = 0;
    }

    for (index = 0; index <= 3; index++) {
      if (visibleSummons[index] != 0)
        Runtime_PushSlotEntry(&sprites[index], 240);
    }

    cursorX = window->x * 8 - 2;
    cursorY = (selectedRow * 2 + window->y) * 8 + 20;
    ((struct SummonMenuSprite *)&cursorSprite)->attr01 = 0x40000000;
    ((struct SummonMenuSprite *)&cursorSprite)->attr23 = 0;
    cursorSprite.tile =
        VramCache_Reload((u16)cursorHandle, gResource_FixedBlockBTiles);
    cursorSprite.x = cursorX + ((gFrameCount & 4) >> 1) - 4;
    cursorSprite.y = cursorY - ((gFrameCount & 4) >> 2) - 8;
    Runtime_PushSlotEntry(&cursorSprite, 242);

    {
      s32 element;
      s32 blink;

      blink = gFrameCount & 8;
      for (element = 0; element <= 3; element++) {
        tile = 15 - (blink != 0);
        if ((1 << element & mask) == 0)
          tile = 15;
        Ui_SetRectHighlight(counts->x + element * 2 + 1, counts->y + 1, 2, 1,
                            tile);
      }
    }

    if (count > 4) {
      for (index = 0; index < (count + 3) / 4; index++) {
        tile = index + 0xf301;
        if ((gFrameCount & 15) <= 11 && index == firstEntry / 4)
          tile = index + 0xf30b;
        UiWindow_SetTilemapEntry(
            window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
      }
      UiWindow_SetTilemapEntry(window, 0xf334,
                               window->width - (count + 3) / 4 - 3, -1, 0);
      UiWindow_SetTilemapEntry(window, 0xf335, window->width - 2, -1, 0);
      work->dirty |= 2 << ((u32)(window->y - 1) >> 2);
    }

    navigation = NAVIGATION;
    navigation->page = firstEntry;
    navigation->row = selectedRow;
    navigation->preferredRow = preferredRow;
    pressed = gInput.pressed;
    repeated = gInput.repeat;
    if (navigation->tutorialStep != 0) {
      repeated = 0;
      pressed = 0;
      if (navigation->tutorialDelay == 0) {
        navigation->tutorialDelay = 120;
        repeated = 1;
        pressed = 1;
      } else {
        navigation->tutorialDelay--;
      }
    }
    if (pressed & 1) {
      result = orderedSummons[firstEntry + selectedRow];
      break;
    }
    if (NAVIGATION->timer == 0 || (pressed & 2)) {
      Audio_PlayCue(113);
      result = -1;
      break;
    }
    if (repeated & 0x80) {
      Audio_PlayCue(111);
      selectedRow++;
      if (selectedRow == 4 || firstEntry + selectedRow == count)
        selectedRow = 0;
      preferredRow = selectedRow;
    } else if (repeated & 0x40) {
      Audio_PlayCue(111);
      selectedRow--;
      if (selectedRow < 0) {
        if (firstEntry == (count - 1) / 4 * 4)
          selectedRow = count - firstEntry - 1;
        else
          selectedRow = 3;
      }
      preferredRow = selectedRow;
    } else if (repeated & 0x10) {
      Audio_PlayCue(111);
      Input_ResetKeyRepeat();
      if (firstEntry + 4 >= count) {
        if (firstEntry != 0) {
          firstEntry = 0;
          selectedRow = preferredRow;
        }
      } else {
        firstEntry += 4;
        selectedRow = preferredRow;
        if (firstEntry == (count - 1) / 4 * 4) {
          selectedRow = count - firstEntry - 1;
          if (selectedRow > preferredRow)
            selectedRow = preferredRow;
        }
      }
    } else if (repeated & 0x20) {
      Audio_PlayCue(111);
      Input_ResetKeyRepeat();
      if (firstEntry != 0) {
        firstEntry -= 4;
        selectedRow = preferredRow;
      } else {
        firstEntry = (count - 1) / 4 * 4;
        selectedRow = preferredRow;
        if (firstEntry != 0) {
          selectedRow = count - firstEntry - 1;
          if (selectedRow > preferredRow)
            selectedRow = preferredRow;
        }
      }
    }
    WaitFrames(1);
  }

  WaitFrames(1);
  for (index = 0; index < 4; index++)
    Resource_ResetEntry(iconSlots[index]);
  Resource_ResetEntry((u16)cursorHandle);
  UiWork_Finalize(counts, 1);
  UiWork_Finalize(description, 1);
  UiWork_Finalize(window, 1);
  WaitFrames(1);
  return result;
}
