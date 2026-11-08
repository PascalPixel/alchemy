// Differ 6 of 1647 (tla-en, 2026-11-08, -mtune=gba); the same rows as on 2026-11-07.
// 2026-11-08: unchanged by `*(candidateIds + targetIndex)` (128), the sprite
// frame's sum reversed or in a local, drawCursors' store as 0, the return types
// of Resource_ResetEntry, VramCache_AllocEntry, Sound_PlayFar (all 6), or
// once-only blocks at the loop head (108 to 112, the GCSE hash-table effect);
// the signature device re-measured at 8 without it. Also unchanged: drawCursors
// cleared before firstSprite is set (6), the selected id read into a
// block-scoped local (6); worse: the clear or firstSprite moved past the call
// (16 to 28), the sprite's word stores swapped (10), a once-only block before
// the tile reload or around the word store (108).
// Differ 8 of 1748 in tla-de, tla-fr, tla-es and tla-it at their own addresses
// (0804a984, 0804a9a0, 0804a994, 0804a96c; 2026-11-07): the same loop-head
// pairs as English. The four European editions share one image that asks the
// cartridge its language at run time (System_GetLanguage): the "!" help window
// (kind 7) is 10 columns wide in Italian and 11 in Spanish, the condition lists
// (kinds 4 and 6) 17 in Spanish, and the HP window draws KP, PV or PS by
// language. Their condition window is pinned to r10 rather than r9.
// 2026-11-07: Oam_AddSpriteAtDepth declared returning s32 (permuter), 8 to 6; a
// further 15 minutes of permuting and loop-head reorderings found nothing.
// Battle target selection, The Broken Seal's BattleTarget_RunSelection
// (games/tbs/src/window/screens/battle_targets.c) on this game's work and
// messages. The modes are numbered differently (2 the party, 4 the acting unit
// alone, 1 the enemies, whose downed ones a link battle skips); a new hint (8)
// starts on the unit the last command aimed at, which the navigation work
// remembers (0x10c); the enemies "Mystery Man" and "Mystery Woman" are enemy
// kinds 355 and 352.
//
// Left: every instruction is present and the registers and frame agree; the 6
// are 3 pairs swapped by sched2 at the loop head: the candidateIds base and
// index slot loads, and cursorSlot's load against `movs r3, #31`. In the block
// after a __modsi3 call the slot load outranks the store on load cost (prio 7
// against 5), so the tie the listing shows is not reachable from the source:
// that is -mthumb-slow-loads' approximation (K2), not a shape question.
// Reordering the sprite's two word stores gives differ 10 now; byte-offset
// forms of the first candidateIds read go backwards.
//
// Two forms here are held by measurement. The spill slots of the addresses GCSE
// hoists follow its hash table, whose size comes from the function's insn count:
// the listing's order needs 719 buckets or fewer (or 733 or more) at this frame.
// The pin's copies add seven insns, so TARGET_BOB spells the rounding out, which
// drops one copy at each of its three uses (differ 130 to 36). If the pin goes,
// TARGET_BOB can go back to `/ 32768`. The pin itself stays because the plain
// `conditionWindow = NULL` is spilled; The Broken Seal gets r9 from a null with
// no constant note instead, a tagged dead device this round does not add.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "battle/battle_unit.h"
#include "version.h"
#include "module/system/system.h"

// The register the condition window's pin takes (see the tag).
#if TLA_PATCH_2
#define CONDITION_WINDOW_REGISTER "r10"
#else
#define CONDITION_WINDOW_REGISTER "r9"
#endif

extern u8 Msg_0c5a[];
extern u8 Msg_0d03[];
extern u8 Msg_0d04[];
extern u8 Msg_0d05[];
extern u8 Msg_0d06[];
extern u8 Msg_0d07[];
extern u8 Msg_0d08[];
extern u8 Msg_0d09[];
extern u8 Msg_0d0a[];
extern u8 Msg_0d0b[];
extern u8 Msg_0d0c[];

struct TargetWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct TargetNavigation {
  u8 unk00[0x28];
  s32 targetViewOffset;
  u8 unk2c[0x20];
  s32 timer;
  u8 unk50[0x88];
  s32 tutorialStep;
  s32 tutorialDelay;
  u8 unke0[0x2c];
  s32 lastTarget;
};

struct TargetBattle {
  u8 unk00[0x58];
  s16 partyUnits[7];
  s16 enemyIds[7];
};

struct TargetPoint {
  s32 x;
  s32 y;
  s32 z;
};

struct TargetTrack {
  u8 x;
  u8 y;
  u8 flags;
  s8 index;
};

struct TargetSprite {
  u8 unk00[4];
  u8 y;
  u8 affine : 2;
  u8 mode : 2;
  u8 high05 : 4;
  u16 x : 9;
  u16 matrix : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 attr2high : 6;
  u16 unk0a;
};

struct TargetAffine {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
  u16 unk06;
};

#define NAVIGATION ((struct TargetNavigation *)Ram_HeapSlots->unke4[0])

extern const u8 gData_0805c9c4[];
extern const u8 gOwnerStatus_HpString[];
extern const u8 gOwnerStatus_SlashString[];
extern const u8 gOwnerStatus_PpString[];
#if TLA_PATCH_2
extern const u8 gOwnerStatus_KpString[];
extern const u8 gOwnerStatus_PvString[];
s32 System_GetLanguage(void);
#endif

s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 entry, const void *tiles);
struct BattleUnit *Unit_Get(s32 unit);
s32 GameFlag_Test(s32 flag);
void BattleMotion_ProjectConditionalPositionFar(s32 unit,
                                                struct TargetPoint *point);
struct TargetWindow *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                     s32 flags);
s32 Trig_Sin(s32 angle);
s32 Oam_AddSpriteAtDepth(void *sprite, s32 depth); // Returns void; see the tag.
s32 AffineMatrix_BuildForEffect(struct TargetAffine *affine);
void BattleHighlight_SetModesFar(u16 *units, s32 mode);
void Window_Close(void *window, s32 mode);
void Text_DrawStringAtOffset(const u8 *text, struct TargetWindow *window,
                               s32 x, s32 y);
void Text_DrawNumberInWindow(s32 value, s32 digits,
                               struct TargetWindow *window, s32 x, s32 y);
void Text_DrawCharacterAtOffset(s32 message, struct TargetWindow *window,
                                  s32 x, s32 y);
void Func_08041f70(s32 color);
void Message_CopyExpanded(s32 message, u16 *text, s32 capacity);
s32 Text_GetWideStringWidth(u16 *text);
void Vram_ClearWindowBlock(void);
void Text_DrawWideStringAtOffset(u16 *text, struct TargetWindow *window,
                                     s32 x, s32 y);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
void Resource_ResetEntry(s32 slot);

// The help window's column beside the aimed member, kept on screen.
#define TARGET_HELP_COLUMN(back, width, last)                                  \
  {                                                                            \
    s32 anchor = selectedPosition.x / 8;                                       \
    helpColumn = anchor - (back);                                              \
    if (anchor + (width) > 29)                                                 \
      helpColumn = (last);                                                     \
  }

// The window listing the aimed member's conditions, count lines tall. The
// European editions share one image and widen it at run time for Spanish.
#if TLA_PATCH_2
#define TARGET_CONDITION_LIST_WINDOW()                                         \
  {                                                                            \
    s32 anchor = selectedPosition.x / 8;                                       \
    helpColumn = anchor - 7;                                                   \
    if (System_GetLanguage() == CARTRIDGE_LANGUAGE_SPANISH) {                  \
      if (anchor + 10 > 29)                                                    \
        helpColumn = 13;                                                       \
      conditionWindow = Window_Create(helpColumn, helpRow, 17, count + 2, 6);  \
    } else {                                                                   \
      if (anchor + 9 > 29)                                                     \
        helpColumn = 14;                                                       \
      conditionWindow = Window_Create(helpColumn, helpRow, 16, count + 2, 6);  \
    }                                                                          \
  }
#else
#define TARGET_CONDITION_LIST_WINDOW()                                         \
  {                                                                            \
    TARGET_HELP_COLUMN(7, 9, 14);                                              \
    conditionWindow = Window_Create(helpColumn, helpRow, 16, count + 2, 6);    \
  }
#endif

// The cursors' bob, a sine turn every 16 frames scaled down to 2 pixels.
#define TARGET_BOB(y)                                                          \
  {                                                                            \
    s32 bob = Trig_Sin(gFrameCount << 12);                                     \
                                                                               \
    if (bob < 0)                                                               \
      bob += 32767;                                                            \
    (y) += bob >> 15;                                                          \
  }

s32 Func_0804a85c(s32 selection, s32 mode, u32 spread, u32 kind) {
  /* FAKEMATCH(signature): Oam_AddSpriteAtDepth declared returning s32; as
   * void, 8 of 1647 instructions differ in tla-en. */
  u16 candidateIds[8];
  struct TargetSprite cursorSprites[6];
  struct TargetTrack cursorTracks[6];
  struct TargetPoint actorPosition;
  u16 spreadActorIds[14];
  u8 spreadTrackIndices[8];
  struct TargetPoint selectedPosition;
  u16 targetName[15];
  struct TargetPoint namePosition;
  struct TargetPoint markerPosition;
  struct TargetAffine effect;
  struct BattleUnit *unit;
  struct TargetBattle *battle;
  struct TargetWindow *window;
  /* FAKEMATCH(pin): the condition window in r9 (r10 in the European editions),
   * where the native keeps it; as an ordinary local its NULL default's constant note halves its priority,
   * it is spilled and 494 of 1647 instructions differ in tla-en (6 with the
   * pin; 2026-11-08). */
  register struct TargetWindow *conditionWindow asm(CONDITION_WINDOW_REGISTER);
  s32 targetIndex;
  s32 targetCount;
  s32 candidateCount;
  s32 redraw;
  s32 cursorSlot;
  s32 drawCursors;
  s32 matrix;
  s32 found;
  s32 helpColumn;
  s32 helpRow;
  s32 count;
#if TLA_PATCH_2
  s32 *countAddress;
#endif
  s32 i;
  s32 j;
  s32 n;
  s32 cursorX;
  s32 cursorY;
  s32 pressed;
  s32 y;
  s32 width;
  s32 repeated;
  s32 result;
  struct TargetTrack *track;
  struct TargetSprite *marker;
  struct TargetSprite *firstSprite;
  struct BattleUnit *target;

  battle = Ram_HeapSlots->battleWork;
  targetCount = 0;
  redraw = 0xffff;
  cursorSlot = VramCache_AllocEntry(256);
  conditionWindow = NULL;
  if (spread == 0)
    spread = 1;
  if (mode == 2 || mode == 4)
    NAVIGATION->targetViewOffset = -2;
  else
    NAVIGATION->targetViewOffset = 16;
  for (i = 0; i < 6; i++)
    cursorTracks[i].flags = 0;
  targetIndex = -1;
  if (mode == 2) {
    for (i = 0; i <= 5 && battle->partyUnits[i] != 255; i++) {
      candidateIds[targetCount] = battle->partyUnits[i];
      targetCount++;
    }
  } else if (mode == 4) {
    candidateIds[0] = selection;
    targetCount = 1;
  } else {
    for (i = 0; i <= 5 && battle->enemyIds[i] != 255; i++) {
      candidateIds[targetCount] = battle->enemyIds[i];
      targetCount++;
    }
  }
  candidateIds[targetCount] = 255;
  candidateCount = targetCount;
  if (mode == 2) {
    if (spread != 255 && kind != 0) {
      found = 0;
      for (j = 0; j < targetCount; j++) {
        struct BattleUnit *candidate;

        if (candidateIds[j] == 254)
          continue;
        candidate = Unit_Get(candidateIds[j]);
        switch (kind) {
        case 5:
          if (candidate->hp == 0) {
            selection = candidateIds[j];
            found = 1;
          }
          break;
        case 3:
          if (candidate->poison != 0)
            found = 1;
          break;
        case 4:
          if (candidate->delusion != 0 || candidate->stun != 0 ||
              candidate->sleep != 0 || candidate->psySeal != 0 ||
              candidate->grimReaper != 0)
            found = 1;
          break;
        case 6:
          if (candidate->delusion != 0 || candidate->stun != 0 ||
              candidate->sleep != 0 || candidate->psySeal != 0 ||
              candidate->grimReaper != 0 || candidate->poison != 0 ||
              candidate->haunt != 0)
            found = 1;
          break;
        case 8:
          if (NAVIGATION->lastTarget == candidateIds[j])
            found = 1;
          break;
        }
        if (found != 0) {
          selection = candidateIds[j];
          break;
        }
      }
    }
    for (j = 0; j < targetCount; j++) {
      if (candidateIds[j] == selection)
        break;
    }
    if (j != targetCount)
      targetIndex = j;
  }
  if (targetIndex < 0)
    targetIndex = (targetCount - 1) / 2;
  while (candidateIds[targetIndex] == 254 ||
         (GameFlag_Test(0x16c) && mode == 1 &&
          Unit_Get(candidateIds[targetIndex])->hp == 0)) {
    targetIndex = targetIndex + targetCount - 1;
    targetIndex %= targetCount;
  }
  if (mode != 2) {
    BattleMotion_ProjectConditionalPositionFar(selection, &actorPosition);
    cursorTracks[0].flags = 8;
    cursorTracks[0].x = actorPosition.x;
    cursorTracks[0].y = 0x80;
  }
  window = Window_Create(0, 12, 30, 4, 74);

  for (;;) {
    firstSprite = cursorSprites;
    drawCursors = 0;
    BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                               &selectedPosition);
    ((u32 *)firstSprite)[1] = 0x40002000;
    ((u32 *)firstSprite)[2] = drawCursors;
    firstSprite->tile = VramCache_Reload(
        cursorSlot, ((gFrameCount >> 2) & 31) * 256 + gData_0805c9c4);
    TARGET_BOB(selectedPosition.y);
    y = selectedPosition.y;
    if (cursorTracks[0].flags & 1) {
      cursorX = (selectedPosition.x + cursorTracks[0].x) / 2;
      cursorY = (y + cursorTracks[0].y) / 2;
      if ((selectedPosition.x - cursorX < 0
               ? -(selectedPosition.x - cursorX)
               : selectedPosition.x - cursorX) <= 7)
        drawCursors = 1;
      selectedPosition.x = cursorX;
      cursorTracks[0].x = cursorX;
      selectedPosition.y = cursorY;
      cursorTracks[0].y = cursorY;
    } else if (cursorTracks[0].flags <= 3) {
      cursorTracks[0].x = selectedPosition.x;
      selectedPosition.y = y;
      cursorTracks[0].y = y;
      cursorTracks[0].flags = 1;
    } else {
      selectedPosition.x = cursorTracks[0].x;
      selectedPosition.y = cursorTracks[0].y;
      cursorTracks[0].flags -= 4;
      if (cursorTracks[0].flags <= 3)
        cursorTracks[0].flags = 1;
    }
    firstSprite->x = selectedPosition.x - 8;
    firstSprite->y = selectedPosition.y - 16;
    Oam_AddSpriteAtDepth(firstSprite, 240);
    if (spread == 255) {
      effect.scaleX = 256;
      effect.scaleY = 256;
    } else {
      effect.scaleX = 176;
      effect.scaleY = 176;
    }
    effect.angle = 0;
    matrix = AffineMatrix_BuildForEffect(&effect);

    if (redraw & 1) {
      targetCount = 0;
      for (i = 0; i < 6; i++)
        cursorTracks[i].flags &= ~2;
      for (i = 0; (u32)i < spread; i++) {
        n = targetIndex + i;
        if (n < candidateCount && candidateIds[n] != 254) {
          spreadActorIds[targetCount] = candidateIds[n];
          cursorTracks[i].flags |= 2;
          if (cursorTracks[i].index != i) {
            cursorTracks[i].flags &= ~1;
            cursorTracks[i].index = i;
          }
          spreadTrackIndices[targetCount] = i;
          targetCount++;
        }
        if (i != 0 && (n = targetIndex - i) >= 0 && candidateIds[n] != 254) {
          spreadActorIds[targetCount] = candidateIds[n];
          cursorTracks[6 - i].flags |= 2;
          if (cursorTracks[6 - i].index != -i) {
            cursorTracks[6 - i].flags &= ~1;
            cursorTracks[6 - i].index = -i;
          }
          spreadTrackIndices[targetCount] = 6 - i;
          targetCount++;
        }
      }
      for (i = 0; i < 6; i++) {
        if ((cursorTracks[i].flags & 2) == 0)
          cursorTracks[i].index = 6;
      }
      spreadActorIds[targetCount] = 255;
      BattleHighlight_SetModesFar(spreadActorIds, 1);
      if ((u32)(NAVIGATION->lastTarget = candidateIds[targetIndex]) <= 7) {
        if (spread != 255 && kind != 0) {
          unit = Unit_Get(candidateIds[targetIndex]);
          BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                                     &selectedPosition);
          if (conditionWindow != NULL)
            Window_Close(conditionWindow, 1);
          switch (kind) {
          case 7:
#if TLA_PATCH_2
          {
            s32 anchor = selectedPosition.x / 8;

            helpColumn = anchor - 4;
            if (System_GetLanguage() == CARTRIDGE_LANGUAGE_ITALIAN) {
              if (anchor + 6 > 29)
                helpColumn = 20;
              conditionWindow = Window_Create(helpColumn, 8, 10, 3, 6);
            } else if (System_GetLanguage() == CARTRIDGE_LANGUAGE_SPANISH) {
              if (anchor + 7 > 29)
                helpColumn = 19;
              conditionWindow = Window_Create(helpColumn, 8, 11, 3, 6);
            } else {
              if (anchor + 5 > 29)
                helpColumn = 21;
              conditionWindow = Window_Create(helpColumn, 8, 9, 3, 6);
            }
          }
#else
            TARGET_HELP_COLUMN(4, 4, 22);
            conditionWindow = Window_Create(helpColumn, 8, 9, 3, 6);
#endif
            Func_08041f70(2);
            Text_DrawCharacterAtOffset((s32)Msg_0d0c, conditionWindow, 0, 0);
            Func_08041f70(15);
            break;
          case 1:
            TARGET_HELP_COLUMN(7, 6, 17);
            conditionWindow = Window_Create(helpColumn, 8, 13, 3, 6);
#if TLA_PATCH_2
            switch (System_GetLanguage()) {
            case CARTRIDGE_LANGUAGE_GERMAN:
              Text_DrawStringAtOffset(gOwnerStatus_KpString, conditionWindow,
                                        0, 0);
              break;
            case CARTRIDGE_LANGUAGE_FRENCH:
            case CARTRIDGE_LANGUAGE_SPANISH:
              Text_DrawStringAtOffset(gOwnerStatus_PvString, conditionWindow,
                                        0, 0);
              break;
            case CARTRIDGE_LANGUAGE_ITALIAN:
              Text_DrawStringAtOffset(gOwnerStatus_HpString, conditionWindow,
                                        0, 0);
              break;
            }
#else
            Text_DrawStringAtOffset(gOwnerStatus_HpString, conditionWindow, 0,
                                      0);
#endif
            Text_DrawNumberInWindow(unit->hp, 4, conditionWindow, 16, 0);
            Text_DrawStringAtOffset(gOwnerStatus_SlashString, conditionWindow,
                                      48, 0);
            Text_DrawNumberInWindow(unit->maxHp, 4, conditionWindow, 56, 0);
            break;
          case 2:
            TARGET_HELP_COLUMN(7, 6, 17);
            conditionWindow = Window_Create(helpColumn, 8, 13, 3, 6);
            Text_DrawStringAtOffset(gOwnerStatus_PpString, conditionWindow, 0,
                                      0);
            Text_DrawNumberInWindow(unit->pp, 4, conditionWindow, 16, 0);
            Text_DrawStringAtOffset(gOwnerStatus_SlashString, conditionWindow,
                                      48, 0);
            Text_DrawNumberInWindow(unit->maxPp, 4, conditionWindow, 56, 0);
            break;
          case 5:
            TARGET_HELP_COLUMN(7, 5, 18);
            conditionWindow = Window_Create(helpColumn, 8, 12, 3, 6);
            if (unit->hp != 0)
              goto no_condition;
            Text_DrawCharacterAtOffset((s32)Msg_0d0b, conditionWindow, 0, 0);
            break;
          case 3:
            TARGET_HELP_COLUMN(7, 5, 18);
            conditionWindow = Window_Create(helpColumn, 8, 12, 3, 6);
            if (unit->poison == 0)
              goto no_condition;
            Text_DrawCharacterAtOffset((s32)Msg_0d04, conditionWindow, 0, 0);
            break;
          case 4: {
            count = 0;
            if (unit->delusion != 0)
              count = 1;
            if (unit->stun != 0)
              count++;
            if (unit->sleep != 0)
              count++;
            if (unit->psySeal != 0)
              count++;
            if (unit->grimReaper != 0)
              count++;
            if (count == 0)
              count = 1;
            helpRow = 9 - count;
            if (helpRow <= 3)
              helpRow = 4;
            TARGET_CONDITION_LIST_WINDOW();
            count = 0;
            if (unit->delusion != 0) {
              Text_DrawCharacterAtOffset((s32)Msg_0d05, conditionWindow, 0,
                                           0);
              count = 1;
            }
            if (unit->stun != 0) {
              Text_DrawCharacterAtOffset((s32)Msg_0d06, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              Text_DrawCharacterAtOffset((s32)Msg_0d07, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              Text_DrawCharacterAtOffset((s32)Msg_0d08, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              Text_DrawCharacterAtOffset((s32)Msg_0d09, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (count == 0)
              goto no_condition;
            break;
          }
          default:
            break;
          case 6: {
            count = 0;
            if (unit->poison != 0)
              count = 1;
            if (unit->delusion != 0)
              count++;
#if TLA_PATCH_2
            /* FAKEMATCH(temporary): count read back through a pointer to it;
             * as `count == 0`, 148 of 1748 instructions differ in tla-de
             * (spill slots). */
            countAddress = &count;
#endif
            if (unit->stun != 0)
              count++;
            if (unit->sleep != 0)
              count++;
            if (unit->psySeal != 0)
              count++;
            if (unit->grimReaper != 0)
              count++;
            if (unit->haunt != 0)
              count++;
#if TLA_PATCH_2
            if (*countAddress == 0)
#else
            if (count == 0)
#endif
              count = 1;
            helpRow = 9 - count;
            if (helpRow <= 3)
              helpRow = 4;
            TARGET_CONDITION_LIST_WINDOW();
            count = 0;
            if (unit->poison != 0) {
              Text_DrawCharacterAtOffset((s32)Msg_0d04, conditionWindow, 0,
                                           0);
              count = 1;
            }
            if (unit->delusion != 0) {
              Text_DrawCharacterAtOffset((s32)Msg_0d05, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->stun != 0) {
              Text_DrawCharacterAtOffset((s32)Msg_0d06, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              Text_DrawCharacterAtOffset((s32)Msg_0d07, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              Text_DrawCharacterAtOffset((s32)Msg_0d08, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              Text_DrawCharacterAtOffset((s32)Msg_0d09, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->haunt != 0) {
              Text_DrawCharacterAtOffset((s32)Msg_0d0a, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (count != 0)
              break;
          }
          no_condition:
            Func_08041f70(2);
            Text_DrawCharacterAtOffset((s32)Msg_0d03, conditionWindow, 0, 0);
            Func_08041f70(15);
            break;
          }
        }
      } else if (spread != 255) {
        target = Unit_Get(candidateIds[targetIndex]);
        BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                                   &namePosition);
        TARGET_BOB(namePosition.y);
        if (target->enemyId == 355 || target->enemyId == 352) {
          width = (s32)Msg_0c5a;
          if (target->enemyId == 355)
            width++;
          Message_CopyExpanded(width, targetName, 14);
        } else {
          for (i = 0; i <= 13; i++) {
            targetName[i] = n = target->name[i];
            if (!n)
              break;
          }
          targetName[i] = 0;
        }
        width = Text_GetWideStringWidth(targetName);
        namePosition.x -= width / 2;
        namePosition.x -= 8;
        if (namePosition.x + width > 224)
          namePosition.x = 224 - width;
        if (namePosition.x < 0)
          namePosition.x = 0;
        Vram_ClearWindowBlock();
        Text_DrawWideStringAtOffset(targetName, window, namePosition.x, 4);
      }
      redraw &= ~1;
    }

    if (drawCursors != 0) {
      for (i = 1; i < targetCount; i++) {
        marker = &firstSprite[i];
        track = &cursorTracks[spreadTrackIndices[i]];
        BattleMotion_ProjectConditionalPositionFar(spreadActorIds[i],
                                                   &markerPosition);
        TARGET_BOB(markerPosition.y);
        *marker = *firstSprite;
        if (track->flags & 1) {
          s32 x = (markerPosition.x + track->x) / 2;
          s32 y = (markerPosition.y + track->y) / 2;

          markerPosition.x = x;
          track->x = x;
          markerPosition.y = y;
          track->y = y;
        } else {
          s32 x = marker->x;
          s32 y = marker->y + 8;

          track->flags = 1;
          markerPosition.x = x;
          track->x = x;
          markerPosition.y = y;
          track->y = y;
        }
        marker->mode = 1;
        marker->x = markerPosition.x - 8;
        marker->y = markerPosition.y - 12;
        if (spread == 255)
          marker->affine = 0;
        else
          marker->affine = 1;
        marker->matrix = matrix;
        Oam_AddSpriteAtDepth(marker, 240);
      }
    }

    pressed = gInput.pressed;
    repeated = gInput.repeat;
    if (NAVIGATION->tutorialStep != 0) {
      repeated = 0;
      pressed = 0;
      if (NAVIGATION->tutorialDelay == 0) {
        NAVIGATION->tutorialDelay = 60;
        repeated = 1;
        pressed = 1;
      } else {
        NAVIGATION->tutorialDelay--;
      }
    }
    if (pressed & 1) {
      s32 id = candidateIds[targetIndex];
      s32 k;

      redraw = 0;
      result = -1;
      for (k = 0; k <= 5 && battle->partyUnits[k] != 255; k++) {
        if (battle->partyUnits[k] == id) {
          result = 0x100 | k;
          break;
        }
      }
      if (result < 0) {
        for (k = 0; k <= 5 && battle->enemyIds[k] != 255; k++) {
          if (battle->enemyIds[k] == id) {
            result = 0x180 | k;
            break;
          }
        }
      }
      targetIndex = result;
    } else if (spread != 255) {
      if (repeated & 0x90) {
        Sound_PlayFar(111);
        do {
          targetIndex++;
          targetIndex %= candidateCount;
        } while (candidateIds[targetIndex] == 254);
        redraw |= 1;
      }
      if (repeated & 0x60) {
        Sound_PlayFar(111);
        do {
          targetIndex = targetIndex + candidateCount - 1;
          targetIndex %= candidateCount;
        } while (candidateIds[targetIndex] == 254);
        redraw |= 1;
      }
    }
    if (NAVIGATION->timer == 0 || (pressed & 2)) {
      Sound_PlayFar(113);
      targetIndex = -1;
      break;
    }
    wait(1);
    if (redraw == 0)
      break;
  }
  wait(1);
  Resource_ResetEntry(cursorSlot);
  if (conditionWindow != NULL)
    Window_Close(conditionWindow, 1);
  Window_Close(window, 1);
  BattleHighlight_SetModesFar(candidateIds, 0);
  NAVIGATION->targetViewOffset = 0;
  wait(1);
  return targetIndex;
}
