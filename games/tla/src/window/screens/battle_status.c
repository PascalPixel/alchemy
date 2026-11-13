/// @file
/// The Status screen.
// Part of screens.c.

#include "../screens_internal.h"
#include "module/system/system.h"
#include "version.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/battle.h"
#include "module/window/screens.h"
#include "module/window/window.h"

// Room for the help line of an item and of a Djinni, and the second letter
// tile of the PP cost's label, as measured: the item line grows from German
// on, the Djinni line and the tile from French on.
#if TLA_PATCH_2
#define SUMMARY_ITEM_HELP_CAPACITY 52
#else
#define SUMMARY_ITEM_HELP_CAPACITY 32
#endif
#if TLA_PATCH_3
#define SUMMARY_DJINN_HELP_CAPACITY 52
#define SUMMARY_PP_LABEL_SECOND_TILE 0xf005
#else
#define SUMMARY_DJINN_HELP_CAPACITY 32
#define SUMMARY_PP_LABEL_SECOND_TILE 0xf01e
#endif

// Japanese text is shorter: its help lines and Psynergy descriptions take
// smaller buffers, and its stat numbers sit one tile further left. It decodes
// the item, Psynergy and Djinn lines raw where the other languages copy them
// expanded; the lines that take a number are copied expanded in every language.
#if LANGUAGE == LANGUAGE_JAPANESE
#define SUMMARY_TEXT_COPY Message_DecodeRaw
#define SUMMARY_PSYNERGY_TEXT_LENGTH 32
#define SUMMARY_HELP_TEXT_LENGTH 64
#define SUMMARY_STAT_VALUE_X 176
#else
#define SUMMARY_TEXT_COPY Message_CopyExpanded
#define SUMMARY_PSYNERGY_TEXT_LENGTH 52
#define SUMMARY_HELP_TEXT_LENGTH 128
#define SUMMARY_STAT_VALUE_X 184
#endif

#define NAVIGATION ((struct SummaryNavigation *)Ram_HeapSlots->unke4[0])

#if TLA_PATCH_2
extern const u8 gOwnerStatus_NivString[];
extern const u8 gOwnerStatus_NvString[];
extern const u8 gOwnerStatus_KpString[];
extern const u8 gOwnerStatus_PvString[];
#endif

/* VIEW(screens): routine BattleActor_SpawnObjectsForListFar as void
 * BattleActor_SpawnObjectsForListFar(u16 *list, s32 count); module/tla.h has
 * void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode). */
extern void BattleActor_SpawnObjectsForListFar__screens(
    u16 *list, s32 count) __asm__("BattleActor_SpawnObjectsForListFar");
void BattleObject_SpawnObjectsForListFar(u16 *list, s32 count, s32 mode);
/* VIEW(screens): routine BattleObject_GetFar as void
 * **BattleObject_GetFar(s32 member); motobj.h has struct BattleObj
 * *BattleObject_GetFar(s32 objectId). */
extern void **
GetBattleObjectSlotFar__screens(s32 member) __asm__("BattleObject_GetFar");
#ifdef MODERN
// Called with no prototype in scope, as its bytes need; a current GCC gets
// it here.
void Func_0803a2b0(s32 x, s32 y, u32 width, u32 height, s32 whole);
#endif
void Text_DrawWideStringAtOffset(u16 *text, struct SummaryWindow *window, s32 x,
                                 s32 y);
s32 Unit_GetExperienceForLevelFar(s32 member, s32 level);

// The four list-page scroll marks, one tile per page of four rows (Psynergy
// and items).
#define SUMMARY_DRAW_PAGE_MARKS(window, count, rowBase)                        \
  {                                                                            \
    for (j = 0; j < ((count) + 3) / 4; j++) {                                  \
      s32 tile = 0xf301 + j;                                                   \
                                                                               \
      if (j == (rowBase) / 4)                                                  \
        tile = 0xf30b + j;                                                     \
      Window_SetTilemapEntry(                                                  \
          window, tile, (window)->width - ((count) + 3) / 4 + j - 10, 7, 0);   \
    }                                                                          \
    Window_SetTilemapEntry(window, 0xf336,                                     \
                           (window)->width - ((count) + 3) / 4 - 11, 7, 0);    \
    Window_SetTilemapEntry(window, 0xf337, (window)->width - 10, 7, 0);        \
  }

/// Runs the Status screen for party member `member` until B is pressed or
/// the link battle's time runs out: the party member's level, experience, HP
/// and PP, class, stats, Djinn, elements and statuses, with a help line for the
/// item under the cursor. With `listCount` nonzero, L and R step through the
/// party members of `list` (BATTLE_UNIT_LIST_END ends it, BATTLE_UNIT_REMOVED
/// is an empty place). The link timer's window is closed while it is open
/// (BattleMenuWork.timerHidden), and the party status window is redrawn on the
/// way out. Returns 0. Battle_CollectPartyCommands opens it from the Status
/// command and from L or R in a party member's command row.
s32 BattleStatusScreen_Run(u16 *list, s32 listCount, s32 member) {
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
  struct SummaryPoint cursorPosition;
  s32 portraitSlot;
  s32 statusCount;
  s32 hasDjinn;
  s32 shownMember;
  s32 page;
  s32 figureShown;
  u32 *djinnList;
  s32 djinnCount;
  s32 psynergyCount;
  s32 itemCount;
  s32 keys;
  struct SummaryUnit *unit;
  s32 i;
  s32 j;
  struct SummaryNavigation *navigation;
  s32 rowBase;
  s32 row;
  s32 groupPosition;
  struct SummaryOam *sprite;

  work = (struct SummaryWindowWork *)Ram_HeapSlots->windowTiles;
  redraw = 1;
  memberIndex = -1;
  statusCount = 0;
  hasDjinn = Party_SumDjinnCountsFar(-1);
  page = 0;
  figureShown = 0;
  djinnList = Heap_AllocEwram(84 * sizeof(*djinnList));
  itemCount = psynergyCount = djinnCount = 0;
  shownMember = figureList[0] = list[0];
  figureList[1] = 255;
  BattleActor_SpawnObjectsForListFar__screens(figureList, 1);
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
    iconSprites[i].oam.f.tile =
        VramCache_Reload(iconSlots[i], (const void *)-1);
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
  cursorPosition.x = 0;
  cursorPosition.y = 0;
  statusWindow = Window_Create(0, 0, 30, 20, 6);
  helpWindow = Window_Create(0, 14, 30, 6, 10);
  Window_MarkVisibleTextTiles();

  for (;;) {
    keys = gInput.repeat;
    sprite = &nameSprite;
    BattleStatus_DrawPageArrows(0x060066c0);
    if (redraw != 0)
      Portrait_Load(member, portraitSlot);
    sprite->oam.word.attr01 = 0x80000400;
    /* FAKEMATCH(dead): an empty once-only block, so the sprite word is stored
     * straight after its load; as no block, 10 of 3111 instructions differ in
     * tla-en. */
    do {
    } while (0);
    sprite->oam.word.attr23 = 0;
    sprite->oam.f.tile = gResourceTableEntries[portraitSlot].offset >> 5;
    sprite->oam.f.x = 8;
    sprite->oam.f.y = 24;
    sprite->oam.f.palette = 14;
    Oam_AddSpriteAtDepth(sprite, 240);

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
              groupPosition = cursor.groupPositions[cursor.group];
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
              groupPosition = cursor.groupPositions[cursor.group];
            }
          }
        } else if (keys & 0x30) {
          keys = 0;
          Sound_PlayFar(111);
          cursor.group ^= 2;
          if (gInput.repeat & 0x20)
            groupPosition = statusCount - 1;
          else
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
        s32 djinnPosition = cursor.groupPositions[cursor.group];

        if (keys & 0x80) {
          keys = 0;
          Sound_PlayFar(111);
          redraw |= 2;
          djinnPosition++;
          if (djinnPosition >= cursor.groupSize)
            djinnPosition = 0;
        } else if (keys & 0x40) {
          keys = 0;
          Sound_PlayFar(111);
          redraw |= 2;
          djinnPosition--;
          if (djinnPosition < 0)
            djinnPosition = cursor.groupSize - 1;
        } else if (keys & 0x10) {
          keys = 0;
          Sound_PlayFar(111);
          redraw |= 2;
          djinnPosition += 3;
          if (djinnPosition >= cursor.groupSize)
            djinnPosition = djinnPosition % 3;
        } else if (keys & 0x20) {
          keys = 0;
          Sound_PlayFar(111);
          redraw |= 2;
          djinnPosition -= 3;
          if (djinnPosition < 0)
            djinnPosition = cursor.groupSize / 3 * 3 + (djinnPosition + 3) % 3;
        }
        if (djinnPosition >= cursor.groupSize)
          djinnPosition = cursor.groupSize - 1;
        if (djinnPosition < 0)
          djinnPosition = 0;
        cursor.groupPositions[cursor.group] = djinnPosition;
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
      cursor.helpEntry =
          gOwnerStatus_EquipmentCursors[cursor.groupPositions[cursor.group]]
              .entry;
      cursor.column =
          gOwnerStatus_EquipmentCursors[cursor.groupPositions[cursor.group]]
              .column;
      cursor.row =
          gOwnerStatus_EquipmentCursors[cursor.groupPositions[cursor.group]]
              .row;
    } else if (cursor.group == 2) {
      cursor.helpEntry =
          gOwnerStatus_ConditionCursors[cursor.groupPositions[cursor.group]]
              .entry;
      cursor.column =
          gOwnerStatus_ConditionCursors[cursor.groupPositions[cursor.group]]
              .column;
      cursor.row =
          gOwnerStatus_ConditionCursors[cursor.groupPositions[cursor.group]]
              .row;
      if (statusKinds[0] == 0)
        Window_HighlightRect(statusWindow->x + 15, statusWindow->y + 1, 10, 1,
                             14);
      else
        Window_HighlightRect(statusWindow->x + cursor.column + 1,
                             statusWindow->y + cursor.row + 1, 2, 2, 14);
    } else if (cursor.group == 3 || cursor.group == 5) {
      s8 position = cursor.groupPositions[cursor.group];

      cursor.row = (s8)(position % 4) * 2 + 8;
      if (cursor.group == 3)
        cursor.column = -2;
      else
        cursor.column = -1;
      Window_HighlightRect(statusWindow->x + 1,
                           statusWindow->y + cursor.row + 1, 20, 1, 14);
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
    cursorPosition.x = cursor.column * 8;
    cursorPosition.y = cursor.row * 8;

    // The member's battle figure, respawned when L or R changes the member.
    if (shownMember != member) {
      spawnList[0] = member;
      spawnList[1] = 255;
      ReleaseBattleObjectRecordsFar(shownMember);
      shownMember = member;
      BattleObject_SpawnObjectsForListFar(spawnList, 1, 0);
      figureShown = 0;
    }
    {
      struct SummaryOam *face =
          ((struct SummaryOam **)*GetBattleObjectSlotFar__screens(member))[20];

      portraitSprite.oam.word.attr01 = 0xc0002400;
      portraitSprite.oam.word.attr23 = 0;
      if (face != NULL) {
        portraitSprite.oam.f.tile = face->oam.f.tile;
        portraitSprite.oam.f.x = 0xac;
        portraitSprite.oam.f.y = 56;
        if (figureShown != 0)
          Oam_AddSpriteAtDepth(&portraitSprite, 240);
        figureShown = 1;
      }
    }

    cursorSprite.oam.word.attr01 = 0x40000400;
    cursorSprite.oam.word.attr23 = 0;
    /* FAKEMATCH(barrier): an empty asm stores the sprite words straight after
     * their load, before the call's arguments; as no asm, 16 of 3111
     * instructions differ in tla-en. */
    asm("");
    cursorSprite.oam.f.tile =
        VramCache_Reload(cursorSlot, gResource_FixedBlockBTiles);
    {
      s32 bob = gFrameCount & 4;
      s32 x = statusWindow->x * 8 + cursorPosition.x + 16;

      cursorSprite.oam.f.x = page == 0 ? x - bob / 4 : x + bob / 4;
    }
    cursorSprite.oam.f.y = cursorPosition.y + statusWindow->y * 8 -
                           (s32)(gFrameCount & 4) / 4 + 16;
    if (page != 0)
      cursorSprite.oam.f.affineIndex = 0;
    else
      cursorSprite.oam.f.affineIndex = 8;
    Oam_AddSpriteAtDepth(&cursorSprite, 241);

    if (redraw == 0)
      goto place_sprites;

    unit = Unit_Get(member);
    if (page == 0)
      Text_FillCanvasBackground();

    if (page == 3) {
      // Items: those the member can use in battle first.
      u16 *entries = NAVIGATION->list;
      s32 count;
      s32 index;
      s32 item;
      s32 row;

      count = 0;
      j = -1;
      for (index = 0; index != 15 && (item = unit->items[index]) != 0;
           index++) {
        Item_Get(item);
        if (Item_ClassifyUseAbility(member, item) == 0)
          entries[count++] = item;
      }
      for (index = 0; index != 15 && (item = unit->items[index]) != 0;
           index++) {
        Item_Get(item);
        if (Item_ClassifyUseAbility(member, item) != 0)
          entries[count++] = item;
      }
      entries[count] = 0;
      itemCount = count;
      if (count <= cursor.groupPositions[5])
        cursor.groupPositions[5] = count - 1;
      rowBase = cursor.groupPositions[5] / 4 * 4;
      row = (s8)(cursor.groupPositions[5] % 4);
      if (count != 0) {
        u16 *entry = entries + rowBase + row;

        if (Item_ClassifyUseAbility(member, *entry) == 2)
          SUMMARY_TEXT_COPY((s32)MsgKey0d4f, itemText, 32);
        else
          SUMMARY_TEXT_COPY((*entry & 0x1ff) + (s32)MsgItemPlainName, itemText,
                            SUMMARY_ITEM_HELP_CAPACITY);
      } else {
        SUMMARY_TEXT_COPY((s32)MsgKey0d46, itemText, 32);
      }
      WindowSprite_RedrawSavedRect(statusWindow);
      Window_MapTextCanvasTiles(helpWindow->x, helpWindow->y, helpWindow->width,
                                helpWindow->height, 0);
      Text_FillCanvasBackground();
      Text_DrawWideStringAtOffset(itemText, helpWindow, 0, 20);
      if (rowBase != j) {
        Window_DrawDividerLine(statusWindow, 0, 16, 29, 16);
        for (j = 0; j <= 3 && (item = entries[rowBase + j]) != 0; j++) {
          Item_Get(item);
          Text_SetColor(15);
          if (Item_ClassifyUseAbility(member, item) != 0)
            Text_SetColor(4);
          else if (item & 0x400)
            Text_SetColor(2);
          Text_DrawMessageAsTiles((item & 0x1ff) + (s32)MsgItemName,
                                  statusWindow, 32, 64 + j * 16);
          Text_SetColor(15);
          iconSprites[j].oam.f.tile = ItemIcon_LoadIntoSlot(item, iconSlots[j]);
        }
        /* FAKEMATCH(barrier): an empty asm keeps the list loop's exit on the
         * fill loop's test; as no asm, 2 of 3111 instructions differ in tla-en.
         */
        asm("");
        for (; j <= 3; j++)
          iconSprites[j].oam.f.tile =
              VramCache_Reload(iconSlots[j], (const void *)-1);
      }
      if (count > 4)
        SUMMARY_DRAW_PAGE_MARKS(statusWindow, count, rowBase);
    } else if (page == 1) {
      // Battle Psynergy: grey when it cannot be used in battle, red when the
      // PP are short, another colour while Psynergy is sealed.
      u16 *entries = NAVIGATION->list;
      s32 count;
      s32 shownRow;
      s32 index;
      s32 id;

      count = 0;
      j = -1;
      WindowSprite_RedrawSavedRect(statusWindow);
      shownRow = j;
      for (index = 0;
           index != 32 && (id = unit->psynergy[index].id & 0x3fff) != 0;
           index++) {
        if (Ability_Get(id)->flags & 0x80)
          entries[count++] = id;
      }
      entries[count] = 0;
      psynergyCount = count;
      if (count <= cursor.groupPositions[3])
        cursor.groupPositions[3] = count - 1;
      rowBase = cursor.groupPositions[3] / 4 * 4;
      row = (s8)(cursor.groupPositions[3] % 4);
      if (rowBase != j || row != shownRow) {
        work->holdTilemapUpload = 1;
        Text_FillCanvasBackground();
        if (count != 0)
          SUMMARY_TEXT_COPY((entries[rowBase + row] & 0x3fff) +
                                (s32)MsgAbilityDescription,
                            psynergyText, SUMMARY_PSYNERGY_TEXT_LENGTH);
        else
          SUMMARY_TEXT_COPY((s32)MsgKey0d48, psynergyText,
                            SUMMARY_PSYNERGY_TEXT_LENGTH);
        WindowSprite_RedrawSavedRect(statusWindow);
        Window_MapTextCanvasTiles(helpWindow->x, helpWindow->y,
                                  helpWindow->width, helpWindow->height, 0);
        Text_DrawWideStringAtOffset(psynergyText, helpWindow, 0, 20);
        if (rowBase != j) {
          Window_DrawDividerLine(statusWindow, 0, 16, 29, 16);
          for (j = 0; j <= 3 && (id = entries[rowBase + j]) != 0; j++) {
            struct SummaryAbility *ability = Ability_Get(id);
            s32 range;
            s32 tileFlags;

            Window_SetTilemapEntry(statusWindow, 0xf01f, 11, j * 2 + 8, 0);
            Window_SetTilemapEntry(statusWindow, SUMMARY_PP_LABEL_SECOND_TILE,
                                   12, j * 2 + 8, 0);
            iconSprites[j].oam.f.tile =
                Resource_LoadIndexedEntryToBuffer(id, iconSlots[j]);
            if (!(ability->flags & 0x80))
              Text_SetColor(4);
            else if (ability->ppCost > unit->pp)
              Text_SetColor(2);
            else if (unit->psySeal != 0)
              Text_SetColor(9);
            tileFlags = 0;
            work->textColor = 5;
            Text_DrawMessageAsTiles(id + (s32)MsgAbilityNothing, statusWindow,
                                    16, 64 + j * 16);
            Text_DrawNumberAtOffset(ability->ppCost, 2, statusWindow, 104,
                                    64 + j * 16);
            Text_SetColor(15);
            work->textColor = 15;
            if (ability->element != 4)
              Window_SetTilemapEntry(statusWindow, 0x5001 + ability->element,
                                     15, j * 2 + 8, tileFlags);
            range = ability->range;
            if (range == 255)
              range = 11;
            else
              range--;
            Window_DrawThreeTileColumn(statusWindow, 16, j * 2 + 8, range, 0);
          }
          /* FAKEMATCH(barrier): an empty asm keeps the list loop's exit on the
           * fill loop's test; as no asm, 2 of 3111 instructions differ in
           * tla-en. */
          asm("");
          for (; j <= 3; j++)
            iconSprites[j].oam.f.tile =
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
      s32 rowBase = 0;
      s32 shownBase = -1;
      s32 released = 0;
      s32 element;
      s32 bit;
      u32 entry;

      WindowSprite_RedrawSavedRect(statusWindow);
      Window_MapTextCanvasTiles(helpWindow->x, helpWindow->y, helpWindow->width,
                                helpWindow->height, 0);
      Window_DrawDividerLine(statusWindow, 0, 16, 29, 16);
      djinnCount = 0;
      for (element = 0; element <= 3; element++) {
        for (bit = 0; bit <= 19; bit++) {
          if (unit->djinnSet[element] & (1 << bit)) {
            djinnList[djinnCount] = (element << 8) | bit;
            djinnCount++;
          } else if (unit->djinnOwned[element] & (1 << bit)) {
            struct SummaryRecoveryList *recovery =
                &((struct SummaryRecovery *)Djinn_GetRecoveryTableFar(
                      (u32)member > 7))
                     ->list;
            s32 k;
            s32 value;

            for (k = 0; k < recovery->count &&
                        !(recovery->entries[k].owner == member &&
                          recovery->entries[k].element == element &&
                          recovery->entries[k].index == bit);
                 k++)
              ;
            value = (element << 8) | bit | 0x10000;
            djinnList[djinnCount] = value;
            if (recovery->entries[k].turns > 0)
              djinnList[djinnCount] =
                  value | (recovery->entries[k].turns << 17);
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
        if (entry & 0x10000) {
          if (entry & 0x3e0000) {
            MessageArgument_Push((entry & 0x3e0000) >> 17, 5);
            SUMMARY_TEXT_COPY(((entry & 0xf00) >> 8) * 20 + (entry & 0xff) +
                                  (s32)MsgDjinnEffect,
                              psynergyText, SUMMARY_DJINN_HELP_CAPACITY);
          } else {
            Message_CopyExpanded((s32)MsgReadyToSummon, psynergyText, 32);
          }
        } else {
          SUMMARY_TEXT_COPY(((entry & 0xf00) >> 8) * 20 + (entry & 0xff) +
                                (s32)MsgDjinnEffect,
                            psynergyText, SUMMARY_DJINN_HELP_CAPACITY);
        }
      } else {
        SUMMARY_TEXT_COPY((s32)MsgNoDjinn, psynergyText, 32);
      }
      work->holdTilemapUpload = released;
      Text_FillCanvasBackground();
      Text_DrawWideStringAtOffset(psynergyText, helpWindow, 0, 20);
      if (rowBase != shownBase) {
        u32 shown;
        s32 column;
        s32 tileRow;

        for (j = 0; j <= 8; j++) {
          // rowBase is this page's 0, as pages 1 and 3 index their lists.
          shown = djinnList[rowBase + j];
          column = j / 3 * 7;
          tileRow = j % 3 * 2 + 9;
          if (shown == 0x80000000)
            break;
          Window_SetTilemapEntry(statusWindow, ((shown & 0xf00) >> 8) + 0x5001,
                                 column, tileRow, 0);
          if (shown & 0x3e0000)
            Text_SetColor(4);
          else if (shown & 0x10000)
            Text_SetColor(2);
          Text_DrawMessageAsTiles(((shown & 0xf00) >> 8) * 20 + (shown & 0xff) +
                                      (s32)MsgDjinnName,
                                  statusWindow, column * 8 + 8, tileRow * 8);
// Spanish and Italian leave a recovering Djinni's turns off the list.
#if !TLA_PATCH_4
          if (shown & 0x3e0000)
            Text_DrawNumberAtOffset((shown & 0x3e0000) >> 17, 2, statusWindow,
                                    column * 8 + 40, tileRow * 8);
#endif
          Text_SetColor(15);
        }
      }
      /* FAKEMATCH(dead): a never-true arm, pages 1 and 3's scroll marks
       * with this page's 0; gcse folds it away but sizes its table with the
       * arm's insns, which orders the hoisted addresses' stack slots; as no
       * arm, 62 of 3111 instructions differ in tla-en. */
      if (released > 4)
        SUMMARY_DRAW_PAGE_MARKS(statusWindow, released, rowBase);
      work->dirty = 1;
      work->holdTilemapUpload = 0;
    }

    if (redraw != 0) {
      if (page == 0) {
        WindowSprite_RedrawSavedRect(statusWindow);
        Window_MapTextCanvasTiles(helpWindow->x, helpWindow->y,
                                  helpWindow->width, helpWindow->height, page);
        Window_DrawDividerLine(statusWindow, 0, 14, 29, 14);
      }
      Text_DrawStringAtOffset(unit->name, statusWindow, 0, 0);
      if (memberIndex > 3)
        Text_DrawMessageAsTiles((s32)MsgInTheBack, statusWindow, 40, 16);
#if TLA_PATCH_2
      // The European editions share one image and choose the level's label,
      // drawing the name again under it, at run time.
      switch (System_GetLanguage()) {
      case CARTRIDGE_LANGUAGE_FRENCH:
        Text_DrawStringAtOffset(unit->name, statusWindow, 0, 0);
        Text_DrawStringAtOffset(gOwnerStatus_NivString, statusWindow, 51, 0);
        Text_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
        break;
      case CARTRIDGE_LANGUAGE_SPANISH:
        Text_DrawStringAtOffset(unit->name, statusWindow, 0, 0);
        Text_DrawStringAtOffset(gOwnerStatus_NvString, statusWindow, 56, 0);
        Text_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
        break;
      default:
        Text_DrawStringAtOffset(unit->name, statusWindow, 0, 0);
        Text_DrawStringAtOffset(gOwnerStatus_LevelString, statusWindow, 56, 0);
        Text_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
        break;
      }
#else
      Text_DrawStringAtOffset(gOwnerStatus_LevelString, statusWindow, 56, 0);
      Text_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
#endif
      Text_DrawMessageAsTiles((s32)MsgExp, statusWindow, 0, 8);
      Text_DrawNumberInWindow(unit->experience, 8, statusWindow, 40, 8);
#if TLA_PATCH_2
      switch (System_GetLanguage()) {
      case CARTRIDGE_LANGUAGE_GERMAN:
        Text_DrawStringAtOffset(gOwnerStatus_KpString, statusWindow, 40, 24);
        break;
      case CARTRIDGE_LANGUAGE_FRENCH:
      case CARTRIDGE_LANGUAGE_SPANISH:
        Text_DrawStringAtOffset(gOwnerStatus_PvString, statusWindow, 40, 24);
        break;
      case CARTRIDGE_LANGUAGE_ITALIAN:
        Text_DrawStringAtOffset(gOwnerStatus_HpString, statusWindow, 40, 24);
        break;
      }
#else
      Text_DrawStringAtOffset(gOwnerStatus_HpString, statusWindow, 40, 24);
#endif
      Text_DrawNumberInWindow(unit->hp, 4, statusWindow, 56, 24);
      Text_DrawStringAtOffset(gOwnerStatus_SlashString, statusWindow, 88, 24);
      Text_DrawNumberInWindow(unit->hpMax, 4, statusWindow, 96, 24);
      Text_DrawStringAtOffset(gOwnerStatus_PpString, statusWindow, 40, 32);
      Text_DrawNumberInWindow(unit->pp, 4, statusWindow, 56, 32);
      Text_DrawStringAtOffset(gOwnerStatus_SlashString, statusWindow, 88, 32);
      Text_DrawNumberInWindow(unit->ppMax, 4, statusWindow, 96, 32);
      Text_DrawMessageAsTiles((s32)MsgExp - 10, statusWindow, 136, 16);
      Text_DrawNumberInWindow(unit->atk, 3, statusWindow, SUMMARY_STAT_VALUE_X,
                              16);
      Text_DrawMessageAsTiles((s32)MsgExp - 9, statusWindow, 136, 24);
      Text_DrawNumberInWindow(unit->def, 3, statusWindow, SUMMARY_STAT_VALUE_X,
                              24);
      Text_DrawMessageAsTiles((s32)MsgExp - 8, statusWindow, 136, 32);
      Text_DrawNumberInWindow(unit->agi, 3, statusWindow, SUMMARY_STAT_VALUE_X,
                              32);
      Text_DrawMessageAsTiles((s32)MsgExp - 7, statusWindow, 136, 40);
      Text_DrawNumberInWindow(unit->luck, 3, statusWindow, SUMMARY_STAT_VALUE_X,
                              40);
      Text_DrawMessageAsTiles((s32)MsgClassName + unit->classId, statusWindow,
                              0, 48);
      if (page == 0) {
        if (hasDjinn != 0)
          Text_DrawMessageAsTiles((s32)MsgExp - 1, statusWindow, 0, 72);
        Text_DrawMessageAsTiles((s32)MsgExp - 5, statusWindow, 0, 80);
        Text_DrawMessageAsTiles((s32)MsgExp - 4, statusWindow, 0, 88);
        Text_DrawMessageAsTiles((s32)MsgExp - 3, statusWindow, 0, 96);
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
      do {
        if (unit->curse != 0) {
          statusKinds[i++] = 15;
        }
        if (i > 7)
          break;
        if (unit->grimReaper != 0) {
          statusKinds[i++] = 8;
        }
        if (i > 7)
          break;
        if (unit->poison == 1) {
          statusKinds[i++] = 1;
        }
        if (i > 7)
          break;
        if (unit->poison == 2) {
          statusKinds[i++] = 2;
        }
        if (i > 7)
          break;
        if (unit->psySeal != 0) {
          statusKinds[i++] = 4;
        }
        if (i > 7)
          break;
        if (unit->stun != 0) {
          statusKinds[i++] = 3;
        }
        if (i > 7)
          break;
        if (unit->sleep != 0) {
          statusKinds[i++] = 5;
        }
        if (i > 7)
          break;
        if (unit->haunt != 0) {
          statusKinds[i++] = 7;
        }
        if (i > 7)
          break;
        if (unit->delusion != 0) {
          statusKinds[i++] = 6;
        }
        if (i > 7)
          break;
        if (unit->attackTurns != 0) {
          if (unit->attackStage > 0) {
            statusKinds[i++] = 9;
          }
          if (unit->attackStage < 0) {
            statusKinds[i++] = 10;
          }
        }
        if (i > 7)
          break;
        if (unit->defenseTurns != 0) {
          if (unit->defenseStage > 0) {
            statusKinds[i++] = 11;
          }
          if (unit->defenseStage < 0) {
            statusKinds[i++] = 12;
          }
        }
        if (i > 7)
          break;
        if (unit->resistanceTurns != 0) {
          if (unit->resistanceStage > 0) {
            statusKinds[i++] = 13;
          }
          if (unit->resistanceStage < 0) {
            statusKinds[i++] = 14;
          }
        }
        if (i > 7)
          break;
        if (unit->agilityStage > 0) {
          statusKinds[i++] = 17;
        }
        if (unit->agilityStage < 0) {
          statusKinds[i++] = 18;
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
        Text_DrawMessageAsTiles(unit->hp != 0 ? (s32)MsgNormal
                                              : (s32)MsgCantFight,
                                statusWindow, 112, 0);
    }

    if (page == 0) {
      u16 *helpText = Heap_Alloc(SUMMARY_HELP_TEXT_LENGTH * sizeof(*helpText));

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
        Iwram_CopyWordsEntry(savedUnit, unit, sizeof(*savedUnit));
        attackDelta = unit->atk;
        defenseDelta = unit->def;
        helpValue = 0;
        agilityDelta = unit->agi;
        unit->attackStage = 0;
        unit->defenseStage = 0;
        unit->agilityStage = 0;
        Unit_CalculateFar(member);
        attackDelta -= unit->atk;
        defenseDelta -= unit->def;
        agilityDelta -= unit->agi;
        Iwram_CopyWordsEntry(unit, savedUnit, sizeof(*savedUnit));
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
        /* FAKEMATCH(dead): an empty once-only block between the two calls, so
         * movs r2, #128 leads statusKind's reload; as no block, 2 of 3111
         * instructions differ in tla-en. */
        do {
        } while (0);
        Message_CopyExpanded(statusKind + (s32)MsgNormalStatus, helpText,
                             SUMMARY_HELP_TEXT_LENGTH);
      } else if (cursor.helpEntry == 2 && unit->level <= 98) {
        MessageArgument_Push(
            Unit_GetExperienceForLevelFar(member, unit->level + 1) -
                unit->experience,
            5);
        Message_CopyExpanded((s32)MsgKey0d20, helpText,
                             SUMMARY_HELP_TEXT_LENGTH);
      } else {
        Message_CopyExpanded(cursor.helpEntry + (s32)MsgKey0d21, helpText,
                             SUMMARY_HELP_TEXT_LENGTH);
      }
      Text_DrawWideStringAtOffset(helpText, helpWindow, 0, 4);
      Heap_Release(helpText);
    }
    redraw = 0;
    work->dirty = 1;

  place_sprites:
    {
      struct SummaryResourceEntry *entry;
      s32 *slot;
      s32 iconX;

      sprite = statusSprites;
      slot = statusSlots;
      for (i = 0, entry = gResourceTableEntries, iconX = 112; i <= 10; i++) {
        sprite->oam.word.attr01 = 0x40000400;
        /* FAKEMATCH(dead): an empty once-only block, so the sprite word is
         * stored straight after its load; as no block, 14 of 3111 instructions
         * differ in tla-en. */
        do {
        } while (0);
        sprite->oam.word.attr23 = 0;
        sprite->oam.f.tile = entry[*slot++].offset >> 5;
        sprite->oam.f.x = iconX;
        sprite->oam.f.y = statusWindow->y * 8 + 8;
        if (statusKinds[i] > 0)
          Oam_AddSpriteAtDepth(sprite, 240);
        iconX += 15;
        sprite++;
      }
      if (page == 1 || page == 3) {
        struct SummaryResourceEntry *iconEntry;
        s32 iconRow;

        for (i = 0, iconEntry = gResourceTableEntries, sprite = iconSprites,
            iconRow = 0;
             i <= 3; i++) {
          sprite->oam.word.attr01 = 0x40000400;
          /* FAKEMATCH(dead): an empty once-only block, so the sprite word is
           * stored straight after its load; as no block, 2 of 3111
           * instructions differ in tla-en. */
          do {
          } while (0);
          sprite->oam.word.attr23 = 0;
          sprite->oam.f.tile = iconEntry[iconSlots[i]].offset >> 5;
          if (page == 3)
            sprite->oam.f.x = 24;
          else
            sprite->oam.f.x = 8;
          sprite->oam.f.y = (iconRow + statusWindow->y) * 8 + 68;
          Oam_AddSpriteAtDepth(sprite, 240);
          sprite++;
          iconRow += 2;
        }
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

  {
    s32 *slot = iconSlots;

    for (i = 3; i >= 0; i--)
      VramCache_FreeEntry(*slot++);
    slot = statusSlots;
    for (i = 10; i >= 0; i--)
      VramCache_FreeEntry(*slot++);
  }
  VramCache_FreeEntry(portraitSlot);
  VramCache_FreeEntry(cursorSlot);
  wait(1);
#if TLA_PATCH_1
  Window_MarkVisibleTextTiles();
#endif
  Window_Close(statusWindow, 1);
  Window_Close(helpWindow, 1);
  PartyStatus_Draw(((u8 *)Ram_HeapSlots->battleWork)[0x41]);
  NAVIGATION->timerHidden = 0;
  ReleaseBattleObjectRecordsFar(shownMember);
  BattleObject_PutFar();
  wait(1);
  Heap_Release(djinnList);
  return 0;
}

#undef SUMMARY_ITEM_HELP_CAPACITY
#undef SUMMARY_DJINN_HELP_CAPACITY
#undef SUMMARY_PP_LABEL_SECOND_TILE
#undef SUMMARY_TEXT_COPY
#undef SUMMARY_PSYNERGY_TEXT_LENGTH
#undef SUMMARY_HELP_TEXT_LENGTH
#undef SUMMARY_STAT_VALUE_X
#undef NAVIGATION
#undef SUMMARY_DRAW_PAGE_MARKS
