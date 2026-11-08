// Draft, differ 38 of 3111 (tla-en, 2026-11-08)
// Eighth round, 8 November: 40 to 38, three tagged barriers (the cursor sprite,
// the two fill loops). The list icons' loop now sets sprite in its for, after
// iconEntry, so add r5, sp, #480 follows mov r8, r0 as in the native. Left:
// note 1 (about 28), the range column's movs r1, #16 (2), page 2's help-window
// loads (6) and, on page 0, movs r2, #128 before the status kind's reload (2).
// Note 1 is two separate facts, both about pseudo numbers:
// - statusSprites' hoist (sfp - 212) is a gcse PRE register, and PRE numbers
//   its registers by hash bucket, (13926 + offset) mod buckets, with buckets
//   = (insns at gcse) / 2 | 1, 1383 here, so -212 wraps and sorts last. With
//   1397 to 1523 buckets (or 1269 to 1371) it sorts below the other hoists as
//   in the native: padding the function's end with 30 or 40 empty asms (a
//   probe only) moved it and the four other hoists to slots 20 to 40, the
//   native's order one slot lower because next still sits at 52. So the
//   original has at least 25 more, or 21 fewer, RTL insns at gcse than this
//   draft; 20 or 26 asms (1393, 1395 buckets) change nothing.
// - next (slot 20) must be numbered after every PRE register and before the
//   Psynergy list's 64 + j * 16 giv (slot 16, reduced in loop.c's first pass,
//   which takes loops last to first): a giv reduced in that first pass.
//   djinnList[j] is reduced there only when the loop counts 68 insns or fewer
//   (benefit 4 times threshold 17; 79 now): every constant set (0xf00 and
//   0x3e0000 twice each, 0x10000, 0xff, the 0 argument, MsgDjinnName, 0x5001,
//   0x80000000) out of the loop and column's copy gone. Mask locals bring
//   only the second pass to 68: the giv then outnumbers the Psynergy giv
//   (slots 16 and 20 swap) and PRE shares the masked values across the
//   calls, 211.
// Tried, nothing better: a block-local pointer for the status sprites (40);
// the range column's call with tileFlags (40), a ternary (70) or range =
// ability->range - 1 (95); page 2's help window through locals in all 64
// orders (best 38: y and width first give the native's order, but reload's
// round robin gives y's index r3 and the window's ldr r0 moves one earlier);
// rowBase or released as its fifth argument (38); the status help message's
// operand orders (38); djinnList + j and byte-offset spellings (no giv, 850).
// Other editions, each at its own address: tla-ja 93 (08046d04: help texts of
// 32 not 52 characters, one more branch on the Djinn page, a few layout
// constants), tla-de 223, tla-es 282, tla-fr 238, tla-it 282 (58 to 72 more
// instructions of language layout); all stay on their scaffolds.
// Seventh round, 8 November: unchanged. At the cursor sprite, without its asm
// the draft loads the call's second argument ahead of the word stores (56);
// a constant local for the first word or the tile read into a block local 56,
// the words swapped 58, through the name sprite's pointer 119 to 132.
// Sixth round, 8 November: without the first fill loop's asm 42, without both 44;
// the list loop's test as j < 4, the fill loop with j++ as its own statement, 42;
// the fill as if-do-while 66, j != 4 94, a goto to the fill loop 870.
// Fifth round, 8 November: the name sprite's and both place_sprites loops' empty
// asms are now empty once-only blocks (do { } while (0)), 40 each, so plain C; at
// the cursor sprite a once-only block reads 98 to 115 in any position (the asm
// stays, 56 without), at the two fill loops 42 (the same as no asm: the list
// loop's exit is threaded past the fill test before reload; while, goto-free
// do-while and j < 4 forms 44 or 94). Page 2's help-window call loads x before
// y here (sched2 tie by luid, equal dependents); y first through a local reads
// 44 (index registers swap). djinnList[j] or &djinnList[j] read 850 (no giv,
// one slot fewer). 20 minutes of permuter (seed 7741; most
// candidates did not compile): nothing below 40.
// Fourth round, 7 November: next set in the Djinn loop's for after j = 0 (46 to
// 40, its copy now through r2 as the native); 8 minutes of permuter, nothing
// better. Slots run by descending pseudo number, so next must outnumber the
// hoist: only loop.c's giv (note 1) gives it that.
// Third round, 7 November: an empty asm after the first sprite word at the
// name, cursor, status and list icon sprites (tagged barriers) closes note 2
// (88 to 46); the two fill-loop barriers now pay 2 each. Left: note 1's slots
// (next and statusSprites' hoist; the hoist is a PRE register numbered by
// hash bucket, (13926 + offset) mod 1381, so -212 sorts last) and note 3.
// Second round, 6 November: 40 minutes of permuter (seed 7105), nothing better.
// Left: the two stack slots of note 1 and the scheduling pairs of notes 2
// and 3 (sprite words, the help window's loads on page 2, movs r1, #16
// before the range column's window).
// The items' first entry is read as entries + rowBase + row (2 better than
// &entries[rowBase + row]); count = 0 and j = -1 as statements before the
// first item loop (8 better than the comma in its for). The two empty asms
// before the fill loops are tagged barriers (92 without both) that stop the
// list loops' exits threading past the fill loops' tests; a second counter
// for the fill loop (i, row or its own k) reads 218 to 522. next's declaration
// in any position of the function's locals reads 124. Older notes follow.
// Draft, differ 124 of 3111, skeleton gap 27 (tla-en, 2026-11-01): the battle
// summary screen, The Broken Seal's BattleStatusScreen_Run
// (games/tbs/src/window/screens/battle_status.c) grown into four pages that A
// cycles through: the stats (page 0, as The Broken Seal's), the member's battle
// Psynergy (1, four rows a page, with PP cost, element and range, coloured by
// what can be cast), its Djinn (2, three columns of three, set, on standby or
// recovering with the turns left) and its items (3, four rows, usable ones
// first). Each list page has its own cursor group (3 to 5), and the help line
// follows the cursor. The battle figure is spawned for the member shown and
// released on the way out; a reserve member (past the fourth) is labelled.
//
// Control flow, loops and instruction count agree with the listing. The devices
// are the barriers. Three things are left, none closed by reshaping:
//
// 1. Two stack slots, about 20 instructions. next is [sp, #20] in the native and
// [sp, #52] here, which moves helpText and statusKind by 4; statusSprites'
// hoisted address is [sp, #44] in the native and [sp, #20] here. Slots follow
// pseudo number, so the native's pointer is one loop.c made: djinnList[j],
// reduced as an address giv with the increment beside the load (ldmia). Here
// that giv is refused: "68 vs 76", benefit 4 times threshold 17 against 76 real
// insns (the mask constants' sets are most of them). It reduces, and the slot
// drops below the hoists, once one-constant locals bring the loop to 68 insns,
// but those locals break the masks' allocation and CSE across the calls. A
// benefit of 5 or a threshold of 19 would do it without them; a threshold is a
// compiler question, out of scope here. Tried: a pointer set in the loop, one
// shared with the list build, one set before the test (reduces, no auto-inc), a
// struct's array (benefit 3), a two-step index. S2 has no device for this, so
// the scheduling devices below cannot reach differ 0 and none was added.
//
// 2. Sprite words: after ldr of 0x80000400 and friends the draft fills the load's
// stall with an argument set-up (adds r0, r5 and ldr r1 at the name, cursor and
// list icon sprites), the native stores straight away. That reorders the pool,
// so those sites differ by a word in every pc offset. No source form moves it;
// a store's use of a load looks costed differently (K2's scheduling model).
//
// 3. Scheduling pairs: cursor.group's store before reading groupPositions[0]
// (three sites, the array read through [cursor.group] changes nothing),
// add r1 around the count's store, the help window's four loads on page 2,
// and movs r7, #0 (helpValue set after
// agilityDelta costs 90: see below). Without the barriers the two list loops'
// exits thread past the fill loops' tests (bgt +114, not +26); break changes
// nothing.
//
// Forms the match depends on: helpValue is set before agilityDelta's read, which
// shortens &unit->agi's life by one insn so it outranks the size and takes r5, and
// IwramCopyWords falls out of its register and is reloaded at each call. The
// Djinn list is built through djinnList[djinnCount], a giv of the count, not a
// pointer: that gives the t + base start and both increments. The recovery list
// is read through recovery->entries[k] (base + t). j is also the "not yet shown"
// -1 of pages 1 and 3 and the scroll marks' counter; statusKinds[i++] rather than
// a pointer, so the stages are not reread after a store; the positions read
// directly from the cursor after clamping, not through a local; the sprites'
// words and fields share a union.
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

struct SummaryPsynergy {
  u16 id;
  u16 unk02;
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
  struct SummaryPsynergy psynergy[32];
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

struct SummaryOam {
  u8 unk00[4];
  union {
    struct {
      u32 attr01;
      u32 attr23;
    } word;
    struct {
      u8 y;
      u8 attr0high;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unk0a;
    } f;
  } oam;
};

struct SummaryPoint {
  s32 x;
  s32 y;
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

struct SummaryRecoveryList {
  struct SummaryRecoveryEntry entries[72];
  s32 count;
};

struct SummaryRecovery {
  u8 unk000[8];
  struct SummaryRecoveryList list;
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
    for (j = 0; j < ((count) + 3) / 4; j++) {                         \
      s32 tile = 0xf301 + j;                                                \
                                                                               \
      if (j == (rowBase) / 4)                                               \
        tile = 0xf30b + j;                                                  \
      Window_SetTilemapEntry(                                                \
          window, tile, (window)->width - ((count) + 3) / 4 + j - 10, 7,    \
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
    iconSprites[i].oam.f.tile = VramCache_Reload(iconSlots[i], (const void *)-1);
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
  Func_08041b68();

  for (;;) {
    keys = gInput.repeat;
    sprite = &nameSprite;
    Func_0804537c(0x060066c0);
    if (redraw != 0)
      Func_08045528(member, portraitSlot);
    sprite->oam.word.attr01 = 0x80000400;
    // A once-only block: the sprite word is stored straight after its load.
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
    cursorPosition.x = cursor.column * 8;
    cursorPosition.y = cursor.row * 8;

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
     * their load, before the call's arguments; as no asm, 54 of 3111
     * instructions differ in tla-en. */
    asm("");
    cursorSprite.oam.f.tile =
        VramCache_Reload(cursorSlot, gResource_FixedBlockBTiles);
    {
      s32 bob = gFrameCount & 4;
      s32 x = statusWindow->x * 8 + cursorPosition.x + 16;

      cursorSprite.oam.f.x = page == 0 ? x - bob / 4 : x + bob / 4;
    }
    cursorSprite.oam.f.y =
        cursorPosition.y + statusWindow->y * 8 - (s32)(gFrameCount & 4) / 4 + 16;
    if (page != 0)
      cursorSprite.oam.f.affineIndex = 0;
    else
      cursorSprite.oam.f.affineIndex = 8;
    Oam_AddSpriteAtDepth(&cursorSprite, 241);

    if (redraw == 0)
      goto place_sprites;

    unit = Unit_Get(member);
    if (page == 0)
      Vram_FillWindowBlockPattern();

    if (page == 3) {
      // Items: those the member can use in battle first.
      u16 *entries = NAVIGATION->list;
      s32 count;
      s32 index;
      s32 item;
      s32 row;

      count = 0;
      j = -1;
      for (index = 0;
           index != 15 && (item = unit->items[index]) != 0; index++) {
        Item_Get(item);
        if (Item_ClassifyUseAbility(member, item) == 0)
          entries[count++] = item;
      }
      for (index = 0; index != 15 && (item = unit->items[index]) != 0; index++) {
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
      if (rowBase != j) {
        Window_DrawDividerLine(statusWindow, 0, 16, 29, 16);
        for (j = 0; j <= 3 && (item = entries[rowBase + j]) != 0; j++) {
          Item_Get(item);
          Func_08041f70(15);
          if (Item_ClassifyUseAbility(member, item) != 0)
            Func_08041f70(4);
          else if (item & 0x400)
            Func_08041f70(2);
          Text_DrawCharacterAtOffset((item & 0x1ff) + (s32)MsgItemName,
                                       statusWindow, 32, 64 + j * 16);
          Func_08041f70(15);
          iconSprites[j].oam.f.tile = ItemIcon_LoadIntoSlot(item, iconSlots[j]);
        }
        /* FAKEMATCH(barrier): an empty asm keeps the list loop's exit on the fill
         * loop's test; as no asm, 40 of 3111 instructions differ in tla-en. */
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
        if (BattleAction_Get(id)->flags & 0x80)
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
        if (rowBase != j) {
          Window_DrawDividerLine(statusWindow, 0, 16, 29, 16);
          for (j = 0; j <= 3 && (id = entries[rowBase + j]) != 0; j++) {
            struct SummaryAbility *ability = BattleAction_Get(id);
            s32 range;
            s32 tileFlags;

            Window_SetTilemapEntry(statusWindow, 0xf01f, 11, j * 2 + 8, 0);
            Window_SetTilemapEntry(statusWindow, 0xf01e, 12, j * 2 + 8, 0);
            iconSprites[j].oam.f.tile =
                Resource_LoadIndexedEntryToBuffer(id, iconSlots[j]);
            if (!(ability->flags & 0x80))
              Func_08041f70(4);
            else if (ability->ppCost > unit->pp)
              Func_08041f70(2);
            else if (unit->psySeal != 0)
              Func_08041f70(9);
            tileFlags = 0;
            work->textColor = 5;
            Text_DrawCharacterAtOffset(id + (s32)MsgAbilityNothing,
                                         statusWindow, 16, 64 + j * 16);
            Text_DrawNumberAtOffset(ability->ppCost, 2, statusWindow, 104,
                                      64 + j * 16);
            Func_08041f70(15);
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
          /* FAKEMATCH(barrier): an empty asm keeps the list loop's exit on the fill
           * loop's test; as no asm, 40 of 3111 instructions differ in tla-en. */
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
      Func_0803a2b0(helpWindow->x, helpWindow->y, helpWindow->width,
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
                &Djinn_GetRecoveryTableFar((u32)member > 7)->list;
            s32 k;
            s32 value;

            for (k = 0;
                 k < recovery->count &&
                 !(recovery->entries[k].owner == member &&
                   recovery->entries[k].element == element &&
                   recovery->entries[k].index == bit);
                 k++)
              ;
            value = (element << 8) | bit | 0x10000;
            djinnList[djinnCount] = value;
            if (recovery->entries[k].turns > 0)
              djinnList[djinnCount] = value | (recovery->entries[k].turns << 17);
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
            Message_CopyExpanded(((entry & 0xf00) >> 8) * 20 + (entry & 0xff) +
                                     (s32)Msg_09b1,
                                 psynergyText, 32);
          } else {
            Message_CopyExpanded((s32)Msg_0cf9, psynergyText, 32);
          }
        } else {
          Message_CopyExpanded(((entry & 0xf00) >> 8) * 20 + (entry & 0xff) +
                                   (s32)Msg_09b1,
                               psynergyText, 32);
        }
      } else {
        Message_CopyExpanded((s32)Msg_0d4e, psynergyText, 32);
      }
      work->holdTilemapUpload = released;
      Vram_FillWindowBlockPattern();
      Text_DrawWideStringAtOffset(psynergyText, helpWindow, 0, 20);
      if (rowBase != shownBase) {
        u32 *next;
        u32 shown;
        s32 column;
        s32 tileRow;

        for (j = 0, next = djinnList; j <= 8; j++) {
          shown = *next++;
          column = j / 3 * 7;
          tileRow = j % 3 * 2 + 9;
          if (shown == 0x80000000)
            break;
          Window_SetTilemapEntry(statusWindow,
                                   ((shown & 0xf00) >> 8) + 0x5001, column,
                                   tileRow, 0);
          if (shown & 0x3e0000)
            Func_08041f70(4);
          else if (shown & 0x10000)
            Func_08041f70(2);
          Text_DrawCharacterAtOffset(
              ((shown & 0xf00) >> 8) * 20 + (shown & 0xff) + (s32)MsgDjinnName,
              statusWindow, column * 8 + 8, tileRow * 8);
          if (shown & 0x3e0000)
            Text_DrawNumberAtOffset((shown & 0x3e0000) >> 17, 2, statusWindow,
                                      column * 8 + 40, tileRow * 8);
          Func_08041f70(15);
        }
      }
      work->dirty = 1;
      work->holdTilemapUpload = 0;
    }

    if (redraw != 0) {
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
      Text_DrawCharacterAtOffset((s32)Msg_0d1a - 10, statusWindow, 136, 16);
      Text_DrawNumberInWindow(unit->atk, 3, statusWindow, 184, 16);
      Text_DrawCharacterAtOffset((s32)Msg_0d1a - 9, statusWindow, 136, 24);
      Text_DrawNumberInWindow(unit->def, 3, statusWindow, 184, 24);
      Text_DrawCharacterAtOffset((s32)Msg_0d1a - 8, statusWindow, 136, 32);
      Text_DrawNumberInWindow(unit->agi, 3, statusWindow, 184, 32);
      Text_DrawCharacterAtOffset((s32)Msg_0d1a - 7, statusWindow, 136, 40);
      Text_DrawNumberInWindow(unit->luck, 3, statusWindow, 184, 40);
      Text_DrawCharacterAtOffset((s32)MsgClassName + unit->classId,
                                   statusWindow, 0, 48);
      if (page == 0) {
        if (hasDjinn != 0)
          Text_DrawCharacterAtOffset((s32)Msg_0d1a - 1, statusWindow, 0, 72);
        Text_DrawCharacterAtOffset((s32)Msg_0d1a - 5, statusWindow, 0, 80);
        Text_DrawCharacterAtOffset((s32)Msg_0d1a - 4, statusWindow, 0, 88);
        Text_DrawCharacterAtOffset((s32)Msg_0d1a - 3, statusWindow, 0, 96);
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
        helpValue = 0;
        agilityDelta = unit->agi;
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
    {
      struct SummaryResourceEntry *entry;
      s32 *slot = statusSlots;
      s32 iconX;

      sprite = statusSprites;
      for (i = 0, entry = gResourceTableEntries, iconX = 112; i <= 10; i++) {
        sprite->oam.word.attr01 = 0x40000400;
        // A once-only block: the sprite word is stored straight after its load.
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
          // A once-only block: the sprite word is stored straight after its
          // load.
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
      Resource_ResetEntry(*slot++);
    slot = statusSlots;
    for (i = 10; i >= 0; i--)
      Resource_ResetEntry(*slot++);
  }
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
