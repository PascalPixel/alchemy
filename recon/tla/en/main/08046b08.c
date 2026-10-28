// Draft, differ 2310 of 3111: the battle summary screen, The Broken Seal's
// BattleSummaryScreen_Run (games/tbs/src/window/screens/battle_status.c) grown into
// four pages that A cycles through: the stats (page 0, as The Broken Seal's),
// the member's battle Psynergy (1, four rows a page, with PP cost, element and
// range, coloured by what can be cast), its Djinn (2, three columns of three,
// set, on standby or recovering with the turns left) and its items (3, four
// rows, usable ones first). Each list page has its own cursor group (3 to 5),
// and the help line follows the cursor. The battle figure is spawned for the
// member shown and released on the way out; a reserve member (past the
// fourth) is labelled. The structure follows the listing, and the locals are
// declared in the native's stack frame order (the first declared takes the highest
// slot; in code order, differ 3122); left: cursorX and cursorY, which the
// native spills and the draft keeps in registers, shift the slots below them,
// and the allocation differs from there.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "iwram_call.h"

extern u8 MsgAbilityNothing[];
extern u8 MsgClassName[];
extern u8 MsgDjinnName[];
extern u8 MsgItemName[];
extern u8 MsgItemPlainName[];
extern u8 Msg_0885[];
extern u8 Msg_09b1[];
extern u8 Msg_0cf9[];
extern u8 Msg_0d10[];
extern u8 Msg_0d11[];
extern u8 Msg_0d12[];
extern u8 Msg_0d13[];
extern u8 Msg_0d15[];
extern u8 Msg_0d16[];
extern u8 Msg_0d17[];
extern u8 Msg_0d19[];
extern u8 Msg_0d1a[];
extern u8 Msg_0d1d[];
extern u8 Msg_0d1e[];
extern u8 Msg_0d1f[];
extern u8 Msg_0d20[];
extern u8 Msg_0d21[];
extern u8 Msg_0d33[];
extern u8 Msg_0d46[];
extern u8 Msg_0d48[];
extern u8 Msg_0d4e[];
extern u8 Msg_0d4f[];

struct SummaryElement {
  s16 power;
  s16 resist;
};

/// A unit as the summary screen reads it.
struct SummaryUnit {
  u8 name[15];
  u8 level;
  u8 unk10[0x24];
  s16 hpMax;
  s16 ppMax;
  s16 hp;
  s16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 unk43[5];
  struct SummaryElement element[4];
  u32 psynergy[32];
  u16 items[15];
  u8 unkf6[2];
  u32 djinnOwned[4];
  u32 djinnSet[4];
  u8 djinnHeld[4];
  u8 djinnSetCount[4];
  u8 unk120[4];
  u32 experience;
  u8 unk128;
  u8 classId;
  u8 unk12a[6];
  s8 curse;
  s8 poison;
  u8 attackTurns;
  s8 attackStage;
  u8 defenseTurns;
  s8 defenseStage;
  u8 resistanceTurns;
  s8 resistanceStage;
  u8 delusion;
  u8 unk139[2];
  u8 stun;
  u8 sleep;
  u8 psySeal;
  u8 unk13e[2];
  u8 haunt;
  u8 grimReaper;
  u8 unk142[5];
  s8 agilityStage;
  u8 unk148[4];
};

struct SummarySprite {
  u8 unk00[4];
  u32 attr01;
  u32 attr23;
};

struct SummaryOam {
  u8 unk00[4];
  u8 y;
  u8 attr0high;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
};

struct SummaryCursor {
  s8 groupPositions[8];
  s32 helpEntry;
  s32 drawnHelpEntry;
  s32 group;
  s32 groupSize;
  s32 column;
  s32 row;
};

struct SummaryWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct SummaryWindowWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04[2];
  u8 holdTilemapUpload;
  u8 textColor;
};

struct SummaryNavigation {
  u8 unk00[0x44];
  void *timerWindow;
  s32 timerHidden;
  s32 timer;
  u8 unk50[0x46];
  u16 list[64];
};

struct SummaryAbility {
  u8 unk00;
  u8 flags; ///< bit 7: usable in battle
  u8 element;
  u8 unk03[5];
  u8 range;
  u8 ppCost;
};

struct SummaryStatCursor {
  u8 entry;
  u8 column;
  u8 row;
  u8 highlightColumn;
  u8 highlightRow;
  u8 highlightWidth;
  u8 unk06[2];
};

struct SummaryCursorStop {
  u8 entry;
  u8 column;
  u8 row;
  u8 unk03;
};

struct SummaryRecoveryEntry {
  u8 element;
  u8 index;
  u8 owner;
  s8 turns;
};

struct SummaryRecovery {
  u8 unk000[8];
  struct SummaryRecoveryEntry entries[72];
  s32 count;
};

struct SummaryResourceEntry {
  u16 unk00;
  u16 offset;
};

#define NAVIGATION ((struct SummaryNavigation *)Ram_HeapSlots->unke4[0])

extern const struct SummaryStatCursor gOwnerStatus_StatCursors[];
extern const struct SummaryCursorStop gOwnerStatus_EquipmentCursors[];
extern const struct SummaryCursorStop gOwnerStatus_ConditionCursors[];
extern const u8 gOwnerStatus_LevelString[];
extern const u8 gOwnerStatus_HpString[];
extern const u8 gOwnerStatus_SlashString[];
extern const u8 gOwnerStatus_PpString[];
extern struct SummaryResourceEntry gResourceTableEntries[];
extern const u8 gResource_FixedBlockBTiles[];

s32 Party_SumDjinnCountsFar(s32 member);
void *Heap_AllocEwram(s32 size);
void BattleActor_SpawnObjectsForListFar(u16 *list, s32 count);
void BattleActor_CommitPlacementFar(void);
s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 entry, const void *tiles);
void Window_Close(void *window, s32 mode);
void Sound_PlayFar(s32 cue);
struct SummaryWindow *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                      s32 flags);
void Func_08041b68(void);
void Func_0804537c(u32 address);
void Func_08045528(s32 member, s32 slot);
void Oam_AddSpriteAtDepth(void *sprite, s32 depth);
void Window_HighlightRect(s32 x, s32 y, s32 width, s32 height, s32 palette);
void ReleaseBattleObjectRecordsFar(s32 member);
void Func_08118148(u16 *list, s32 count, s32 mode);
void **GetBattleObjectSlotFar(s32 member);
struct SummaryUnit *Unit_Get(s32 member);
void Vram_FillWindowBlockPattern(void);
void *Item_Get(s32 item);
s32 Item_ClassifyUseAbility(s32 member, s32 item);
void Message_CopyExpanded(s32 message, u16 *text, s32 capacity);
void WindowSprite_RedrawSavedRect(struct SummaryWindow *window);
void Func_0803a2b0(s32 x, s32 y, s32 width, s32 height, s32 flags);
void Text_DrawWideStringAtOffset(u16 *text, struct SummaryWindow *window,
                                     s32 x, s32 y);
void Window_DrawDividerLine(struct SummaryWindow *window, s32 x, s32 y,
                              s32 width, s32 height);
void Func_08041f70(s32 color);
void Text_DrawCharacterAtOffset(s32 message, struct SummaryWindow *window,
                                  s32 x, s32 y);
s32 ItemIcon_LoadIntoSlot(s32 item, s32 slot);
void Window_SetTilemapEntry(struct SummaryWindow *window, s32 entry, s32 x,
                              s32 y, s32 flags);
struct SummaryAbility *BattleAction_Get(s32 id);
s32 Resource_LoadIndexedEntryToBuffer(s32 id, s32 slot);
void Text_DrawNumberAtOffset(s32 value, s32 digits,
                               struct SummaryWindow *window, s32 x, s32 y);
void Window_DrawThreeTileColumn(struct SummaryWindow *window, s32 x, s32 y,
                                  s32 pattern, s32 flags);
struct SummaryRecovery *Djinn_GetRecoveryTableFar(s32 kind);
void MessageArgument_ClearAll(void);
void MessageArgument_Push(s32 value, s32 kind);
void Text_DrawStringAtOffset(const u8 *text, struct SummaryWindow *window,
                               s32 x, s32 y);
void Text_DrawNumberInWindow(s32 value, s32 digits,
                               struct SummaryWindow *window, s32 x, s32 y);
s32 Unit_GetElementLevelFar(s32 member, s32 element);
void Resource_LoadTableEntryToBuffer(s32 entry, s32 slot);
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void Unit_CalculateFar(s32 member);
s32 Unit_GetExperienceForLevelFar(s32 member, s32 level);
void wait(s32 frames);
void Resource_ResetEntry(s32 slot);
void PartyStatus_Draw(s32 flags);

// The four list-page scroll marks, one tile per page of four rows (Psynergy
// and items).
#define SUMMARY_DRAW_PAGE_MARKS(window, count, rowBase)                        \
  {                                                                            \
    s32 mark;                                                                  \
                                                                               \
    for (mark = 0; mark < ((count) + 3) / 4; mark++) {                         \
      s32 tile = 0xf301 + mark;                                                \
                                                                               \
      if (mark == (rowBase) / 4)                                               \
        tile = 0xf30b + mark;                                                  \
      Window_SetTilemapEntry(                                                \
          window, tile, (window)->width - ((count) + 3) / 4 + mark - 10, 7,    \
          0);                                                                  \
    }                                                                          \
    Window_SetTilemapEntry(window, 0xf336,                                   \
                             (window)->width - ((count) + 3) / 4 - 11, 7, 0);  \
    Window_SetTilemapEntry(window, 0xf337, (window)->width - 10, 7, 0);      \
  }

s32 Func_08046b08(u16 *list, s32 listCount, s32 member) {
  struct SummaryOam portraitSprite;
  struct SummaryOam cursorSprite;
  s32 statusSlots[11];
  s8 statusKinds[11];
  struct SummaryOam statusSprites[11];
  struct SummaryOam iconSprites[4];
  struct SummaryOam nameSprite;
  struct SummaryCursor cursor;
  s32 iconSlots[4];
  u16 figureList[2];
  u16 spawnList[2];
  u16 itemText[64];
  u16 psynergyText[64];
  struct SummaryWindowWork *work;
  struct SummaryWindow *statusWindow;
  s32 redraw;
  struct SummaryWindow *helpWindow;
  s32 memberIndex;
  s32 cursorSlot;
  s32 cursorY;
  s32 cursorX;
  s32 portraitSlot;
  s32 statusCount;
  s32 hasDjinn;
  s32 shownMember;
  s32 page;
  s32 figureShown;
  s32 *djinnList;
  s32 djinnCount;
  s32 psynergyCount;
  s32 itemCount;
  s32 keys;
  struct SummaryUnit *unit;
  s32 i;
  s32 j;
  struct SummaryNavigation *navigation;
  u16 *entries;
  s32 rowBase;
  s32 row;
  s32 groupPosition;

  work = (struct SummaryWindowWork *)Ram_HeapSlots->windowTiles;
  redraw = 1;
  memberIndex = -1;
  statusCount = 0;
  hasDjinn = Party_SumDjinnCountsFar(-1);
  page = 0;
  figureShown = 0;
  djinnList = Heap_AllocEwram(84 * sizeof(*djinnList));
  itemCount = psynergyCount = djinnCount = 0;
  figureList[0] = shownMember = list[0];
  figureList[1] = 255;
  BattleActor_SpawnObjectsForListFar(figureList, 1);
  portraitSlot = VramCache_AllocEntry(0x200);
  navigation = NAVIGATION;
  navigation->timerHidden = redraw;
  if (navigation->timerWindow != NULL) {
    Window_Close(navigation->timerWindow, 1);
    navigation->timerWindow = (void *)page;
  }
  Sound_PlayFar(112);

  for (i = 4; i >= 0; i--)
    cursor.groupPositions[i] = 0;
  cursor.helpEntry = 0;
  cursor.drawnHelpEntry = 0;
  cursor.group = 0;
  cursorSlot = VramCache_AllocEntry(0x80);
  for (i = 0; i <= 10; i++) {
    statusSlots[i] = VramCache_AllocEntry(0x80);
    statusKinds[i] = -1;
  }
  for (i = 0; i <= 3; i++) {
    iconSlots[i] = VramCache_AllocEntry(0x80);
    iconSprites[i].tile = VramCache_Reload(iconSlots[i], (const void *)-1);
  }
  if (listCount != 0) {
    for (i = 0; i <= 5; i++) {
      s32 kind = list[i];

      if (kind == 255)
        break;
      if (kind != 254 && kind == member) {
        memberIndex = i;
        break;
      }
    }
  }
  cursorX = 0;
  cursorY = 0;
  statusWindow = Window_Create(0, 0, 30, 20, 6);
  helpWindow = Window_Create(0, 14, 30, 6, 10);
  Func_08041b68();

  for (;;) {
    keys = gInput.repeat;
    Func_0804537c(0x060066c0);
    if (redraw != 0)
      Func_08045528(member, portraitSlot);
    ((struct SummarySprite *)&nameSprite)->attr01 = 0x80000400;
    ((struct SummarySprite *)&nameSprite)->attr23 = 0;
    nameSprite.tile = gResourceTableEntries[portraitSlot].offset >> 5;
    nameSprite.x = 8;
    nameSprite.y = 24;
    nameSprite.palette = 14;
    Oam_AddSpriteAtDepth(&nameSprite, 240);

    switch (cursor.helpEntry) {
    case 0 ... 9:
    default:
      cursor.group = 0;
      if (hasDjinn != 0)
        cursor.groupSize = 9;
      else
        cursor.groupSize = 7;
      break;
    case 10 ... 13:
      cursor.group = 1;
      cursor.groupSize = 4;
      break;
    case 14 ... 21:
      cursor.group = 2;
      cursor.groupSize = statusCount;
      break;
    case 22:
      cursor.group = 3;
      cursor.groupSize = psynergyCount;
      break;
    case 23:
      cursor.group = 4;
      cursor.groupSize = djinnCount;
      break;
    case 24:
      cursor.group = 5;
      cursor.groupSize = itemCount;
      break;
    }
    Window_HighlightRect(statusWindow->x, statusWindow->y, statusWindow->width,
                        statusWindow->height, 15);

    // A turns the page; each list page starts its own group at the top.
    if (gInput.pressed & 1) {
      page = (page + 1) % 4;
      cursor.group = page + 3;
      cursor.groupPositions[page + 3] = 0;
      cursor.groupSize = 1;
      if (page != 0)
        cursor.helpEntry = page + 21;
      else
        cursor.helpEntry = page;
      redraw = 1;
      Sound_PlayFar(112);
    }

    if (statusCount != 0) {
      if ((u32)cursor.group <= 1) {
        groupPosition = cursor.groupPositions[cursor.group];
        if (keys & 0x80) {
          keys = 0;
          Sound_PlayFar(111);
          groupPosition++;
          if (groupPosition >= cursor.groupSize) {
            groupPosition = 0;
            if (cursor.group == 1) {
              cursor.group = 2;
              groupPosition = cursor.groupPositions[2];
            }
          }
        } else if (keys & 0x40) {
          keys = 0;
          Sound_PlayFar(111);
          groupPosition--;
          if (groupPosition < 0) {
            groupPosition = cursor.groupSize - 1;
            if (cursor.group == 1) {
              cursor.group = 2;
              groupPosition = cursor.groupPositions[2];
            }
          }
        } else if (keys & 0x30) {
          keys = 0;
          Sound_PlayFar(111);
          cursor.group ^= 2;
          groupPosition = statusCount - 1;
          if (!(gInput.repeat & 0x20))
            groupPosition = 0;
        }
        cursor.groupPositions[cursor.group] = groupPosition;
      } else if (cursor.group == 2) {
        groupPosition = cursor.groupPositions[cursor.group];
        if (groupPosition >= cursor.groupSize)
          groupPosition = cursor.groupSize - 1;
        if (groupPosition < 0) {
          cursor.group = 0;
          groupPosition = cursor.groupPositions[0];
        } else if (keys & 0x10) {
          keys = 0;
          groupPosition++;
          if (groupPosition >= cursor.groupSize) {
            cursor.group = 0;
            groupPosition = cursor.groupPositions[0];
          }
          Sound_PlayFar(111);
        } else if (keys & 0x20) {
          keys = 0;
          groupPosition--;
          if (groupPosition < 0) {
            cursor.group = 0;
            groupPosition = cursor.groupPositions[0];
          }
          Sound_PlayFar(111);
        } else if (keys & 0xc0) {
          keys = 0;
          cursor.group = 0;
          groupPosition = cursor.groupPositions[0];
          Sound_PlayFar(111);
        }
        cursor.groupPositions[cursor.group] = groupPosition;
      } else if (cursor.group == 3 || cursor.group == 5) {
        // Four rows a page: up and down step, left and right turn the page.
        groupPosition = cursor.groupPositions[cursor.group];
        if (keys & 0x80) {
          keys = 0;
          Sound_PlayFar(111);
          redraw |= 2;
          groupPosition++;
          if (groupPosition >= cursor.groupSize)
            groupPosition = 0;
        } else if (keys & 0x40) {
          keys = 0;
          Sound_PlayFar(111);
          redraw |= 2;
          groupPosition--;
          if (groupPosition < 0)
            groupPosition = cursor.groupSize - 1;
        } else if (keys & 0x10) {
          keys = 0;
          Sound_PlayFar(111);
          redraw |= 2;
          groupPosition += 4;
          if (groupPosition >= cursor.groupSize) {
            if (groupPosition / 4 - 1 == (cursor.groupSize - 1) / 4)
              groupPosition -= groupPosition / 4 * 4;
            else
              groupPosition = cursor.groupSize - 1;
          }
        } else if (keys & 0x20) {
          keys = 0;
          Sound_PlayFar(111);
          redraw |= 2;
          groupPosition -= 4;
          if (groupPosition < 0)
            groupPosition = cursor.groupSize / 4 * 4 + (groupPosition + 4) % 4;
        }
        if (groupPosition >= cursor.groupSize)
          groupPosition = cursor.groupSize - 1;
        if (groupPosition < 0)
          groupPosition = 0;
        cursor.groupPositions[cursor.group] = groupPosition;
      } else if (cursor.group == 4) {
        // Three Djinn a column.
        groupPosition = cursor.groupPositions[cursor.group];
        if (keys & 0x80) {
          keys = 0;
          Sound_PlayFar(111);
          redraw |= 2;
          groupPosition++;
          if (groupPosition >= cursor.groupSize)
            groupPosition = 0;
        } else if (keys & 0x40) {
          keys = 0;
          Sound_PlayFar(111);
          redraw |= 2;
          groupPosition--;
          if (groupPosition < 0)
            groupPosition = cursor.groupSize - 1;
        } else if (keys & 0x10) {
          keys = 0;
          Sound_PlayFar(111);
          redraw |= 2;
          groupPosition += 3;
          if (groupPosition >= cursor.groupSize)
            groupPosition = groupPosition % 3;
        } else if (keys & 0x20) {
          keys = 0;
          Sound_PlayFar(111);
          redraw |= 2;
          groupPosition -= 3;
          if (groupPosition < 0)
            groupPosition = cursor.groupSize / 3 * 3 + (groupPosition + 3) % 3;
        }
        if (groupPosition >= cursor.groupSize)
          groupPosition = cursor.groupSize - 1;
        if (groupPosition < 0)
          groupPosition = 0;
        cursor.groupPositions[cursor.group] = groupPosition;
      }
    }

    if (cursor.group == 0) {
      const struct SummaryStatCursor *stop;
      s32 k = cursor.groupPositions[cursor.group];

      if (hasDjinn == 0)
        k += 9;
      stop = &gOwnerStatus_StatCursors[k];
      cursor.helpEntry = stop->entry;
      cursor.column = stop->column;
      cursor.row = stop->row;
      Window_HighlightRect(statusWindow->x + stop->highlightColumn + 1,
                          statusWindow->y + stop->highlightRow + 1,
                          stop->highlightWidth, 1, 14);
    } else if (cursor.group == 1) {
      const struct SummaryCursorStop *stop =
          &gOwnerStatus_EquipmentCursors[cursor.groupPositions[cursor.group]];

      cursor.helpEntry = stop->entry;
      cursor.column = stop->column;
      cursor.row = stop->row;
    } else if (cursor.group == 2) {
      const struct SummaryCursorStop *stop =
          &gOwnerStatus_ConditionCursors[cursor.groupPositions[cursor.group]];

      cursor.helpEntry = stop->entry;
      cursor.column = stop->column;
      cursor.row = stop->row;
      if (statusKinds[0] == 0)
        Window_HighlightRect(statusWindow->x + 15, statusWindow->y + 1, 10, 1,
                            14);
      else
        Window_HighlightRect(statusWindow->x + stop->column + 1,
                            statusWindow->y + stop->row + 1, 2, 2, 14);
    } else if (cursor.group == 3 || cursor.group == 5) {
      s8 position = cursor.groupPositions[cursor.group];

      cursor.row = (s8)(position % 4) * 2 + 8;
      if (cursor.group == 3)
        cursor.column = -2;
      else
        cursor.column = -1;
      Window_HighlightRect(statusWindow->x + 1, statusWindow->y + cursor.row + 1,
                          20, 1, 14);
    } else {
      s32 position = cursor.groupPositions[4];

      cursor.row = (s8)(position % 3) * 2 + 9;
      cursor.column = (s8)(position / 3) * 7 - 2;
      Window_HighlightRect(statusWindow->x + cursor.column + 3,
                          statusWindow->y + cursor.row + 1, 7, 1, 14);
    }
    if (cursor.drawnHelpEntry != cursor.helpEntry) {
      cursor.drawnHelpEntry = cursor.helpEntry;
      redraw |= 2;
    }
    cursorX = cursor.column * 8;
    cursorY = cursor.row * 8;

    // The member's battle figure, respawned when L or R changes the member.
    if (shownMember != member) {
      spawnList[0] = member;
      spawnList[1] = 255;
      ReleaseBattleObjectRecordsFar(shownMember);
      shownMember = member;
      Func_08118148(spawnList, 1, 0);
      figureShown = 0;
    }
    {
      struct SummaryOam *face =
          ((struct SummaryOam **)*GetBattleObjectSlotFar(member))[20];

      ((struct SummarySprite *)&portraitSprite)->attr01 = 0xc0002400;
      ((struct SummarySprite *)&portraitSprite)->attr23 = 0;
      if (face != NULL) {
        portraitSprite.tile = face->tile;
        portraitSprite.x = 0xac;
        portraitSprite.y = 56;
        if (figureShown != 0)
          Oam_AddSpriteAtDepth(&portraitSprite, 240);
        figureShown = 1;
      }
    }

    ((struct SummarySprite *)&cursorSprite)->attr01 = 0x40000400;
    ((struct SummarySprite *)&cursorSprite)->attr23 = 0;
    cursorSprite.tile =
        VramCache_Reload(cursorSlot, gResource_FixedBlockBTiles);
    if (page == 0)
      cursorSprite.x =
          statusWindow->x * 8 + cursorX + 16 - (s32)(gFrameCount & 4) / 4;
    else
      cursorSprite.x =
          statusWindow->x * 8 + cursorX + 16 + (s32)(gFrameCount & 4) / 4;
    cursorSprite.y =
        cursorY + statusWindow->y * 8 - (s32)(gFrameCount & 4) / 4 + 16;
    if (page != 0)
      cursorSprite.affineIndex = 0;
    else
      cursorSprite.affineIndex = 8;
    Oam_AddSpriteAtDepth(&cursorSprite, 241);

    if (redraw == 0)
      goto place_sprites;

    unit = Unit_Get(member);
    if (page == 0)
      Vram_FillWindowBlockPattern();

    if (page == 3) {
      // Items: those the member can use in battle first.
      s32 count = 0;
      s32 position;

      entries = NAVIGATION->list;
      rowBase = -1;
      for (i = 0; i < 15 && unit->items[i] != 0; i++) {
        Item_Get(unit->items[i]);
        if (Item_ClassifyUseAbility(member, unit->items[i]) == 0)
          entries[count++] = unit->items[i];
      }
      for (i = 0; i < 15 && unit->items[i] != 0; i++) {
        Item_Get(unit->items[i]);
        if (Item_ClassifyUseAbility(member, unit->items[i]) != 0)
          entries[count++] = unit->items[i];
      }
      entries[count] = 0;
      itemCount = count;
      position = cursor.groupPositions[5];
      if (count <= position)
        position = cursor.groupPositions[5] = count - 1;
      rowBase = position / 4 * 4;
      row = (s8)(position - rowBase);
      if (count != 0) {
        u16 *entry = &entries[rowBase + row];

        if (Item_ClassifyUseAbility(member, *entry) == 2)
          Message_CopyExpanded((s32)Msg_0d4f, itemText, 32);
        else
          Message_CopyExpanded((*entry & 0x1ff) + (s32)MsgItemPlainName,
                                   itemText, 32);
      } else {
        Message_CopyExpanded((s32)Msg_0d46, itemText, 32);
      }
      WindowSprite_RedrawSavedRect(statusWindow);
      Func_0803a2b0(helpWindow->x, helpWindow->y, helpWindow->width,
                    helpWindow->height, 0);
      Vram_FillWindowBlockPattern();
      Text_DrawWideStringAtOffset(itemText, helpWindow, 0, 20);
      if (rowBase != -1) {
        Window_DrawDividerLine(statusWindow, 0, 16, 29, 16);
        for (j = 0; j <= 3 && entries[rowBase + j] != 0; j++) {
          s32 item = entries[rowBase + j];

          Item_Get(item);
          Func_08041f70(15);
          if (Item_ClassifyUseAbility(member, item) != 0)
            Func_08041f70(4);
          else if (item & 0x400)
            Func_08041f70(2);
          Text_DrawCharacterAtOffset((item & 0x1ff) + (s32)MsgItemName,
                                       statusWindow, 32, 64 + j * 16);
          Func_08041f70(15);
          iconSprites[j].tile = ItemIcon_LoadIntoSlot(item, iconSlots[j]);
        }
        for (; j <= 3; j++)
          iconSprites[j].tile =
              VramCache_Reload(iconSlots[j], (const void *)-1);
      }
      if (count > 4)
        SUMMARY_DRAW_PAGE_MARKS(statusWindow, count, rowBase);
    } else if (page == 1) {
      // Battle Psynergy: grey when it cannot be used in battle, red when the
      // PP are short, another colour while Psynergy is sealed.
      s32 count = 0;
      s32 position;

      entries = NAVIGATION->list;
      rowBase = -1;
      WindowSprite_RedrawSavedRect(statusWindow);
      for (i = 0; i < 32; i++) {
        s32 id = unit->psynergy[i] & 0x3fff;

        if (id == 0)
          break;
        if (BattleAction_Get(id)->flags & 0x80)
          entries[count++] = id;
      }
      entries[count] = 0;
      psynergyCount = count;
      position = cursor.groupPositions[3];
      if (count <= position)
        position = cursor.groupPositions[3] = count - 1;
      rowBase = position / 4 * 4;
      row = (s8)(position - rowBase);
      if (rowBase != -1 || row != -1) {
        work->holdTilemapUpload = 1;
        Vram_FillWindowBlockPattern();
        if (count != 0)
          Message_CopyExpanded((entries[rowBase + row] & 0x3fff) +
                                       (s32)Msg_0885,
                                   psynergyText, 52);
        else
          Message_CopyExpanded((s32)Msg_0d48, psynergyText, 52);
        WindowSprite_RedrawSavedRect(statusWindow);
        Func_0803a2b0(helpWindow->x, helpWindow->y, helpWindow->width,
                      helpWindow->height, 0);
        Text_DrawWideStringAtOffset(psynergyText, helpWindow, 0, 20);
        if (rowBase != -1) {
          s32 y = 64;
          s32 tileRow = 8;

          Window_DrawDividerLine(statusWindow, 0, 16, 29, 16);
          for (j = 0; j <= 3 && entries[rowBase + j] != 0;
               j++, y += 16, tileRow += 2) {
            s32 id = entries[rowBase + j];
            struct SummaryAbility *ability = BattleAction_Get(id);
            s32 range;

            Window_SetTilemapEntry(statusWindow, 0xf01f, 11, tileRow, 0);
            Window_SetTilemapEntry(statusWindow, 0xf01e, 12, tileRow, 0);
            iconSprites[j].tile =
                Resource_LoadIndexedEntryToBuffer(id, iconSlots[j]);
            if (!(ability->flags & 0x80))
              Func_08041f70(4);
            else if (ability->ppCost > unit->pp)
              Func_08041f70(2);
            else if (unit->psySeal != 0)
              Func_08041f70(9);
            work->textColor = 5;
            Text_DrawCharacterAtOffset(id + (s32)MsgAbilityNothing,
                                         statusWindow, 16, y);
            Text_DrawNumberAtOffset(ability->ppCost, 2, statusWindow, 104, y);
            Func_08041f70(15);
            work->textColor = 15;
            if (ability->element != 4)
              Window_SetTilemapEntry(statusWindow, 0x5001 + ability->element,
                                       15, tileRow, 0);
            range = ability->range;
            if (range == 255)
              range = 11;
            else
              range--;
            Window_DrawThreeTileColumn(statusWindow, 16, tileRow, range, 0);
          }
          for (; j <= 3; j++)
            iconSprites[j].tile =
                VramCache_Reload(iconSlots[j], (const void *)-1);
        }
        if (count > 4)
          SUMMARY_DRAW_PAGE_MARKS(statusWindow, count, rowBase);
        work->dirty = 1;
        work->holdTilemapUpload = 0;
      }
    } else if (page == 2) {
      // Djinn: each element's set ones, then those on standby or recovering
      // (bit 16, the turns left above bit 17).
      s32 element;
      s32 entry;

      WindowSprite_RedrawSavedRect(statusWindow);
      Func_0803a2b0(helpWindow->x, helpWindow->y, helpWindow->width,
                    helpWindow->height, 0);
      Window_DrawDividerLine(statusWindow, 0, 16, 29, 16);
      djinnCount = 0;
      for (element = 0; element <= 3; element++) {
        s32 *out = &djinnList[djinnCount];

        for (j = 0; j <= 19; j++) {
          if (unit->djinnSet[element] & (1 << j)) {
            *out++ = (element << 8) | j;
            djinnCount++;
          } else if (unit->djinnOwned[element] & (1 << j)) {
            struct SummaryRecovery *recovery =
                Djinn_GetRecoveryTableFar((u32)member > 7);
            s32 k;
            s32 value;

            for (k = 0; k < recovery->count; k++) {
              if (recovery->entries[k].owner == member &&
                  recovery->entries[k].element == element &&
                  recovery->entries[k].index == j)
                break;
            }
            value = (element << 8) | j | 0x10000;
            *out = value;
            if (recovery->entries[k].turns > 0)
              *out = value | (recovery->entries[k].turns << 17);
            out++;
            djinnCount++;
          }
        }
      }
      djinnList[djinnCount] = 0x80000000;
      if (cursor.groupPositions[4] >= djinnCount)
        cursor.groupPositions[4] = djinnCount - 1;
      entry = djinnList[cursor.groupPositions[4]];
      work->holdTilemapUpload = 1;
      if (djinnCount != 0) {
        MessageArgument_ClearAll();
        if ((entry & 0x10000) && !(entry & 0x3e0000)) {
          Message_CopyExpanded((s32)Msg_0cf9, psynergyText, 32);
        } else {
          if (entry & 0x10000)
            MessageArgument_Push((entry & 0x3e0000) >> 17, 5);
          Message_CopyExpanded(((entry & 0xf00) >> 8) * 20 +
                                       (entry & 0xff) + (s32)Msg_09b1,
                                   psynergyText, 32);
        }
      } else {
        Message_CopyExpanded((s32)Msg_0d4e, psynergyText, 32);
      }
      work->holdTilemapUpload = 0;
      Vram_FillWindowBlockPattern();
      Text_DrawWideStringAtOffset(psynergyText, helpWindow, 0, 20);
      {
        s32 *next = djinnList;

        for (j = 0; j <= 8; j++) {
          s32 column = j / 3 * 7;
          s32 tileRow = j % 3 * 2 + 9;

          entry = *next++;
          if (entry == (s32)0x80000000)
            break;
          Window_SetTilemapEntry(statusWindow,
                                   ((entry & 0xf00) >> 8) + 0x5001, column,
                                   tileRow, 0);
          if (entry & 0x3e0000)
            Func_08041f70(4);
          else if (entry & 0x10000)
            Func_08041f70(2);
          Text_DrawCharacterAtOffset(
              ((entry & 0xf00) >> 8) * 20 + (entry & 0xff) + (s32)MsgDjinnName,
              statusWindow, column * 8 + 8, tileRow * 8);
          if (entry & 0x3e0000)
            Text_DrawNumberAtOffset((entry & 0x3e0000) >> 17, 2, statusWindow,
                                      column * 8 + 40, tileRow * 8);
          Func_08041f70(15);
        }
      }
      work->dirty = 1;
      work->holdTilemapUpload = 0;
    }

    if (redraw != 0) {
      s8 *kind;

      if (page == 0) {
        WindowSprite_RedrawSavedRect(statusWindow);
        Func_0803a2b0(helpWindow->x, helpWindow->y, helpWindow->width,
                      helpWindow->height, page);
        Window_DrawDividerLine(statusWindow, 0, 14, 29, 14);
      }
      Text_DrawStringAtOffset(unit->name, statusWindow, 0, 0);
      if (memberIndex > 3)
        Text_DrawCharacterAtOffset((s32)Msg_0d1f, statusWindow, 40, 16);
      Text_DrawStringAtOffset(gOwnerStatus_LevelString, statusWindow, 56, 0);
      Text_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
      Text_DrawCharacterAtOffset((s32)Msg_0d1a, statusWindow, 0, 8);
      Text_DrawNumberInWindow(unit->experience, 8, statusWindow, 40, 8);
      Text_DrawStringAtOffset(gOwnerStatus_HpString, statusWindow, 40, 24);
      Text_DrawNumberInWindow(unit->hp, 4, statusWindow, 56, 24);
      Text_DrawStringAtOffset(gOwnerStatus_SlashString, statusWindow, 88, 24);
      Text_DrawNumberInWindow(unit->hpMax, 4, statusWindow, 96, 24);
      Text_DrawStringAtOffset(gOwnerStatus_PpString, statusWindow, 40, 32);
      Text_DrawNumberInWindow(unit->pp, 4, statusWindow, 56, 32);
      Text_DrawStringAtOffset(gOwnerStatus_SlashString, statusWindow, 88, 32);
      Text_DrawNumberInWindow(unit->ppMax, 4, statusWindow, 96, 32);
      Text_DrawCharacterAtOffset((s32)Msg_0d10, statusWindow, 136, 16);
      Text_DrawNumberInWindow(unit->atk, 3, statusWindow, 184, 16);
      Text_DrawCharacterAtOffset((s32)Msg_0d11, statusWindow, 136, 24);
      Text_DrawNumberInWindow(unit->def, 3, statusWindow, 184, 24);
      Text_DrawCharacterAtOffset((s32)Msg_0d12, statusWindow, 136, 32);
      Text_DrawNumberInWindow(unit->agi, 3, statusWindow, 184, 32);
      Text_DrawCharacterAtOffset((s32)Msg_0d13, statusWindow, 136, 40);
      Text_DrawNumberInWindow(unit->luck, 3, statusWindow, 184, 40);
      Text_DrawCharacterAtOffset((s32)MsgClassName + unit->classId,
                                   statusWindow, 0, 48);
      if (page == 0) {
        if (hasDjinn != 0)
          Text_DrawCharacterAtOffset((s32)Msg_0d19, statusWindow, 0, 72);
        Text_DrawCharacterAtOffset((s32)Msg_0d15, statusWindow, 0, 80);
        Text_DrawCharacterAtOffset((s32)Msg_0d16, statusWindow, 0, 88);
        Text_DrawCharacterAtOffset((s32)Msg_0d17, statusWindow, 0, 96);
        for (j = 0; j <= 3; j++) {
          s32 pal = 1;

          if (hasDjinn != 0)
            pal = 0;
          Window_SetTilemapEntry(statusWindow, 0x5001 + j, 7 + j * 4, pal + 8,
                                   0);
          if (hasDjinn != 0) {
            Text_DrawNumberInWindow(unit->djinnSetCount[j], 1, statusWindow,
                                      40 + j * 32, 72);
            Text_DrawStringAtOffset(gOwnerStatus_SlashString, statusWindow,
                                      48 + j * 32, 72);
            Text_DrawNumberInWindow(unit->djinnHeld[j], 1, statusWindow,
                                      48 + j * 32 + 8, 72);
          }
          Text_DrawNumberInWindow(Unit_GetElementLevelFar(member, j), 2,
                                    statusWindow, 48 + j * 32, 80);
          Text_DrawNumberInWindow(unit->element[j].power, 3, statusWindow,
                                    40 + j * 32, 88);
          Text_DrawNumberInWindow(unit->element[j].resist, 3, statusWindow,
                                    40 + j * 32, 96);
        }
      }

      // The status icons, as The Broken Seal orders them.
      i = 0;
      if (unit->hp == 0) {
        statusKinds[0] = 16;
        i = 1;
      }
      kind = &statusKinds[i];
      do {
        if (unit->curse != 0) {
          *kind++ = 15;
          i++;
        }
        if (i > 7)
          break;
        if (unit->grimReaper != 0) {
          *kind++ = 8;
          i++;
        }
        if (i > 7)
          break;
        if (unit->poison == 1) {
          *kind++ = 1;
          i++;
        }
        if (i > 7)
          break;
        if (unit->poison == 2) {
          *kind++ = 2;
          i++;
        }
        if (i > 7)
          break;
        if (unit->psySeal != 0) {
          *kind++ = 4;
          i++;
        }
        if (i > 7)
          break;
        if (unit->stun != 0) {
          *kind++ = 3;
          i++;
        }
        if (i > 7)
          break;
        if (unit->sleep != 0) {
          *kind++ = 5;
          i++;
        }
        if (i > 7)
          break;
        if (unit->haunt != 0) {
          *kind++ = 7;
          i++;
        }
        if (i > 7)
          break;
        if (unit->delusion != 0) {
          *kind++ = 6;
          i++;
        }
        if (i > 7)
          break;
        if (unit->attackTurns != 0) {
          if (unit->attackStage > 0) {
            *kind++ = 9;
            i++;
          }
          if (unit->attackStage < 0) {
            *kind++ = 10;
            i++;
          }
        }
        if (i > 7)
          break;
        if (unit->defenseTurns != 0) {
          if (unit->defenseStage > 0) {
            *kind++ = 11;
            i++;
          }
          if (unit->defenseStage < 0) {
            *kind++ = 12;
            i++;
          }
        }
        if (i > 7)
          break;
        if (unit->resistanceTurns != 0) {
          if (unit->resistanceStage > 0) {
            *kind++ = 13;
            i++;
          }
          if (unit->resistanceStage < 0) {
            *kind++ = 14;
            i++;
          }
        }
        if (i > 7)
          break;
        if (unit->agilityStage > 0) {
          *kind++ = 17;
          i++;
        }
        if (unit->agilityStage < 0) {
          *kind++ = 18;
          i++;
        }
      } while (0);
      for (j = 0; j < i; j++)
        Resource_LoadTableEntryToBuffer(statusKinds[j], statusSlots[j]);
      if (i == 0) {
        statusKinds[0] = 0;
        i = 1;
      }
      if (i <= 10) {
        for (j = i; j < 11; j++)
          statusKinds[j] = -1;
      }
      statusCount = i;
      if (statusKinds[0] == 0)
        Text_DrawCharacterAtOffset(unit->hp != 0 ? (s32)Msg_0d1d
                                                   : (s32)Msg_0d1e,
                                     statusWindow, 112, 0);
    }

    if (page == 0) {
      u16 *helpText = Heap_Alloc(128 * sizeof(*helpText));

      if ((u32)cursor.helpEntry >= 14) {
        s32 statusKind = statusKinds[cursor.helpEntry - 14];
        struct SummaryUnit *savedUnit;
        s32 attackDelta;
        s32 defenseDelta;
        s32 agilityDelta;
        s32 helpValue;

        if (statusKind == 0 && unit->hp == 0)
          statusKind = 16;
        savedUnit = Heap_Alloc(sizeof(*savedUnit));
        Iwram_CopyWords(savedUnit, unit, sizeof(*savedUnit));
        attackDelta = unit->atk;
        defenseDelta = unit->def;
        agilityDelta = unit->agi;
        helpValue = 0;
        unit->attackStage = 0;
        unit->defenseStage = 0;
        unit->agilityStage = 0;
        Unit_CalculateFar(member);
        attackDelta -= unit->atk;
        defenseDelta -= unit->def;
        agilityDelta -= unit->agi;
        Iwram_CopyWords(unit, savedUnit, sizeof(*savedUnit));
        Heap_Release(savedUnit);
        switch (statusKind) {
        case 8:
          helpValue = unit->grimReaper;
          break;
        case 9:
          helpValue = attackDelta;
          break;
        case 10:
          helpValue = -attackDelta;
          break;
        case 11:
          helpValue = defenseDelta;
          break;
        case 12:
          helpValue = -defenseDelta;
          break;
        case 17:
          helpValue = agilityDelta;
          break;
        case 18:
          helpValue = -agilityDelta;
          break;
        case 13:
        case 14:
          helpValue = unit->resistanceStage * 20;
          if (helpValue < 0)
            helpValue = -helpValue;
          break;
        }
        MessageArgument_Push(helpValue, 5);
        Message_CopyExpanded(statusKind + (s32)Msg_0d33, helpText, 128);
      } else if (cursor.helpEntry == 2 && unit->level <= 98) {
        MessageArgument_Push(
            Unit_GetExperienceForLevelFar(member, unit->level + 1) -
                unit->experience,
            5);
        Message_CopyExpanded((s32)Msg_0d20, helpText, 128);
      } else {
        Message_CopyExpanded(cursor.helpEntry + (s32)Msg_0d21, helpText,
                                 128);
      }
      Text_DrawWideStringAtOffset(helpText, helpWindow, 0, 4);
      Heap_Release(helpText);
    }
    redraw = 0;
    work->dirty = 1;

  place_sprites:
    for (i = 0; i <= 10; i++) {
      ((struct SummarySprite *)&statusSprites[i])->attr01 = 0x40000400;
      ((struct SummarySprite *)&statusSprites[i])->attr23 = 0;
      statusSprites[i].tile = gResourceTableEntries[statusSlots[i]].offset >> 5;
      statusSprites[i].x = 112 + i * 15;
      statusSprites[i].y = statusWindow->y * 8 + 8;
      if (statusKinds[i] > 0)
        Oam_AddSpriteAtDepth(&statusSprites[i], 240);
    }
    if (page == 1 || page == 3) {
      for (i = 0; i <= 3; i++) {
        ((struct SummarySprite *)&iconSprites[i])->attr01 = 0x40000400;
        ((struct SummarySprite *)&iconSprites[i])->attr23 = 0;
        iconSprites[i].tile = gResourceTableEntries[iconSlots[i]].offset >> 5;
        if (page == 3)
          iconSprites[i].x = 24;
        else
          iconSprites[i].x = 8;
        iconSprites[i].y = (i * 2 + statusWindow->y) * 8 + 68;
        Oam_AddSpriteAtDepth(&iconSprites[i], 240);
      }
    }
    if (NAVIGATION->timer == 0 || (gInput.pressed & 2))
      break;
    if (listCount != 0) {
      if (keys & 0x100) {
        memberIndex++;
        if (memberIndex >= listCount)
          memberIndex = 0;
        member = list[memberIndex];
        redraw = 1;
        Sound_PlayFar(111);
      } else if (keys & 0x200) {
        memberIndex--;
        if (memberIndex < 0)
          memberIndex = listCount - 1;
        member = list[memberIndex];
        redraw = 1;
        Sound_PlayFar(111);
      }
    }
    wait(1);
  }

  for (i = 3; i >= 0; i--)
    Resource_ResetEntry(iconSlots[3 - i]);
  for (i = 10; i >= 0; i--)
    Resource_ResetEntry(statusSlots[10 - i]);
  Resource_ResetEntry(portraitSlot);
  Resource_ResetEntry(cursorSlot);
  wait(1);
  Func_08041b68();
  Window_Close(statusWindow, 1);
  Window_Close(helpWindow, 1);
  PartyStatus_Draw(((u8 *)Ram_HeapSlots->battleWork)[0x41]);
  NAVIGATION->timerHidden = 0;
  ReleaseBattleObjectRecordsFar(shownMember);
  BattleActor_CommitPlacementFar();
  wait(1);
  Heap_Release(djinnList);
  return 0;
}
