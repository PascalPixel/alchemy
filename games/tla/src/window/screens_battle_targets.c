/// @file
/// Choosing a battle target.
// Part of screens.c.

#include "window_internal.h"
#include "flags.h"
#include "sounds.h"
#include "field/flag_call.h"
#include "gamepak/far_call/battle.h"
#include "window/item_icon.h"
#include "window/window.h"

// Widths in tiles that fit each language's text: the "Set Djinni" window
// (MsgSetDjinni), the condition list, and the Mystery names' copy, which
// Japanese decodes raw where the other languages copy it expanded.
#if LANGUAGE == LANGUAGE_JAPANESE
#define TARGET_SET_DJINNI_WIDTH 8
#define TARGET_CONDITION_LIST_WIDTH 14
#define TARGET_MYSTERY_NAME_COPY Message_DecodeRaw
#define TARGET_MYSTERY_NAME_CAPACITY 10
#else
#define TARGET_SET_DJINNI_WIDTH 9
#define TARGET_CONDITION_LIST_WIDTH 16
#define TARGET_MYSTERY_NAME_COPY Message_CopyExpanded
#define TARGET_MYSTERY_NAME_CAPACITY 14
#endif

#if TLA_PATCH_2
// These editions size the "Set Djinni" window by the cartridge's language as
// it runs: wider for Italian and Spanish.
#define TARGET_SET_DJINNI_WIDTH_ITALIAN 10
#define TARGET_SET_DJINNI_WIDTH_SPANISH 11
extern const u8 gOwnerStatus_KpString[];
extern const u8 gOwnerStatus_PvString[];
s32 System_GetLanguage(void);
#endif

// The help window's column, `back` tiles left of the aimed member; a window
// `windowWidth` tiles wide that would reach the screen's last column stands
// against the right edge instead.
#define TARGET_HELP_COLUMN(back, windowWidth)                                  \
  {                                                                            \
    s32 anchor = selectedPosition.x / 8;                                       \
    helpColumn = anchor - (back);                                              \
    if (anchor + (windowWidth) - (back) > DISPLAY_TILE_WIDTH - 1)              \
      helpColumn = DISPLAY_TILE_WIDTH - (windowWidth);                         \
  }

#if TLA_PATCH_2
// These editions widen the condition list by a tile for Spanish as it runs.
#define TARGET_CONDITION_LIST_WIDTH_SPANISH 17
#define TARGET_CONDITION_LIST_WINDOW()                                         \
  {                                                                            \
    s32 anchor = selectedPosition.x / 8;                                       \
    helpColumn = anchor - 7;                                                   \
    if (System_GetLanguage() == CARTRIDGE_LANGUAGE_SPANISH) {                  \
      if (anchor + TARGET_CONDITION_LIST_WIDTH_SPANISH - 7 >                   \
          DISPLAY_TILE_WIDTH - 1)                                              \
        helpColumn = DISPLAY_TILE_WIDTH - TARGET_CONDITION_LIST_WIDTH_SPANISH; \
      conditionWindow =                                                        \
          Window_Create(helpColumn, helpRow,                                   \
                        TARGET_CONDITION_LIST_WIDTH_SPANISH, count + 2, 6);    \
    } else {                                                                   \
      if (anchor + TARGET_CONDITION_LIST_WIDTH - 7 > DISPLAY_TILE_WIDTH - 1)   \
        helpColumn = DISPLAY_TILE_WIDTH - TARGET_CONDITION_LIST_WIDTH;         \
      conditionWindow = Window_Create(                                         \
          helpColumn, helpRow, TARGET_CONDITION_LIST_WIDTH, count + 2, 6);     \
    }                                                                          \
  }
#else
#define TARGET_CONDITION_LIST_WINDOW()                                         \
  {                                                                            \
    TARGET_HELP_COLUMN(7, TARGET_CONDITION_LIST_WIDTH);                        \
    conditionWindow = Window_Create(                                           \
        helpColumn, helpRow, TARGET_CONDITION_LIST_WIDTH, count + 2, 6);       \
  }
#endif

// What the selection redraws: the spread's targets and their markers, and at
// the start every part.
#define TARGET_REDRAW_SPREAD 1
#define TARGET_REDRAW_ALL 0xFFFF

// The cursors' bob, a sine turn every 16 frames scaled down to 2 pixels: the
// sine divided by FIXED_ONE / 2, rounding toward zero.
#define TARGET_BOB(y)                                                          \
  {                                                                            \
    s32 bob = Trig_Sin(gFrameCount * (TRIG_TURN / 16));                        \
                                                                               \
    if (bob < 0)                                                               \
      bob += FIXED_ONE / 2 - 1;                                                \
    (y) += bob >> 15;                                                          \
  }

/// Lets the player choose the target of an action of unit `selection`, with a
/// cursor over the aimed unit and smaller markers on the others its range
/// covers. `mode` is the action's targetMode: ABILITY_TARGET_ALLIES the party,
/// ABILITY_TARGET_SELF only `selection` itself, anything else the enemies.
/// `spread` is its range: 1 one unit, n the aimed one and n - 1 either side,
/// ABILITY_RANGE_ALL every one (the cursor does not move); 0 counts as 1.
/// `kind` is Battle_ClassifyEntryKind's class, or TARGET_HINT_SET_DJINNI: on a
/// party member it opens a window of what the action would do (its HP or PP,
/// the statuses it cures, "Set Djinni"). Returns the target chosen,
/// PLACE_PARTY | its party slot or PLACE_ENEMY | its enemy slot, or -1 when
/// cancelled.
s32 BattleTarget_RunSelection(s32 selection, s32 mode, u32 spread, u32 kind) {
  u16 candidateIds[8];
  struct SelectionSprite cursorSprites[6];
  struct TargetTrack cursorTracks[6];
  struct TargetPoint actorPosition;
  u16 spreadActorIds[UNIT_MAX];
  u8 spreadTrackIndices[8];
  struct TargetPoint selectedPosition;
  u16 targetName[UNIT_NAME_SIZE];
  struct TargetPoint namePosition;
  struct TargetPoint markerPosition;
  struct AffineTransform effect;
  struct Unit *unit;
  struct BattleWork *battle;
  struct Window *window;
  struct Window *conditionWindow;
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
  struct SelectionSprite *marker;
  struct SelectionSprite *firstSprite;
  struct Unit *target;

  battle = Ram_HeapSlots->battleWork;
  targetCount = 0;
  redraw = TARGET_REDRAW_ALL;
  cursorSlot = VramCache_AllocEntry(256);
  /* FAKEMATCH(dead): a null computed from selection. */
  conditionWindow = (struct Window *)((u32)(u16)selection >> 16);
  if (spread == 0)
    spread = 1;
  if (mode == ABILITY_TARGET_ALLIES || mode == ABILITY_TARGET_SELF)
    BATTLE_MENU_WORK->targetViewOffset = -2;
  else
    BATTLE_MENU_WORK->targetViewOffset = 16;
  for (i = 0; i < 6; i++)
    cursorTracks[i].flags = 0;
  targetIndex = -1;
  if (mode == ABILITY_TARGET_ALLIES) {
    for (i = 0; i <= 5 && battle->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
      candidateIds[targetCount] = battle->partyUnits[i];
      targetCount++;
    }
  } else if (mode == ABILITY_TARGET_SELF) {
    candidateIds[0] = selection;
    targetCount = 1;
  } else {
    for (i = 0; i <= 5 && battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
      candidateIds[targetCount] = battle->enemyIds[i];
      targetCount++;
    }
  }
  candidateIds[targetCount] = BATTLE_UNIT_LIST_END;
  candidateCount = targetCount;
  if (mode == ABILITY_TARGET_ALLIES) {
    if (spread != ABILITY_RANGE_ALL && kind != TARGET_HINT_NONE) {
      found = 0;
      for (j = 0; j < targetCount; j++) {
        struct Unit *candidate;

        if (candidateIds[j] == BATTLE_UNIT_REMOVED)
          continue;
        candidate = Unit_Get(candidateIds[j]);
        switch (kind) {
        case TARGET_HINT_REVIVE:
          if (candidate->hp == 0) {
            selection = candidateIds[j];
            found = 1;
          }
          break;
        case TARGET_HINT_CURE_POISON:
          if (candidate->poison != 0)
            found = 1;
          break;
        case TARGET_HINT_RESTORE:
          if (candidate->delusion != 0 || candidate->stun != 0 ||
              candidate->sleep != 0 || candidate->psySeal != 0 ||
              candidate->grimReaper != 0)
            found = 1;
          break;
        case TARGET_HINT_CURE_ALL:
          if (candidate->delusion != 0 || candidate->stun != 0 ||
              candidate->sleep != 0 || candidate->psySeal != 0 ||
              candidate->grimReaper != 0 || candidate->poison != 0 ||
              candidate->haunt != 0)
            found = 1;
          break;
        case TARGET_HINT_LAST_TARGET:
          if (BATTLE_MENU_WORK->lastTarget == candidateIds[j])
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
  while (candidateIds[targetIndex] == BATTLE_UNIT_REMOVED ||
         (GameFlag_Get(FLAG_LINK_BATTLE) && mode == ABILITY_TARGET_FOES &&
          Unit_Get(candidateIds[targetIndex])->hp == 0)) {
    targetIndex = targetIndex + targetCount - 1;
    targetIndex %= targetCount;
  }
  if (mode != ABILITY_TARGET_ALLIES) {
    BattleMotion_ProjectConditionalPositionFar(selection, &actorPosition);
    cursorTracks[0].flags = 8;
    cursorTracks[0].x = actorPosition.x;
    cursorTracks[0].y = 128;
  }
#if !TLA_PATCH_2
  /* FAKEMATCH(barrier): an empty asm once selection and mode are dead. */
  __asm__("" : : : "r8", "r10");
#endif
  window = Window_Create(0, 12, 30, 4,
                         WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                             WINDOW_FLAG_OUTLINED_TEXT);

  for (;;) {
    /* FAKEMATCH(dead): a once-only block; as its body alone. */
    do {
      /* FAKEMATCH(temporary): firstSprite holds &cursorSprites[0]. */
      firstSprite = cursorSprites;
      drawCursors = 0;
      BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                                 &selectedPosition);
      ((u32 *)firstSprite)[1] = OBJ_SIZE_16X16 | OBJ_256_COLORS;
      ((u32 *)firstSprite)[2] = 0;
    } while (0);
    firstSprite->tile =
        VramCache_Reload(cursorSlot, ((gFrameCount >> 2) & 31) * 256 +
                                         gMenu_AnimatedCursorTiles);
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
    Oam_AddSpriteAtDepth(firstSprite, OAM_DEPTH_WINDOW);
    if (spread == ABILITY_RANGE_ALL) {
      effect.scaleX = Q_8_8(1);
      effect.scaleY = Q_8_8(1);
    } else {
      effect.scaleX = Q_8_8(11.0 / 16);
      effect.scaleY = Q_8_8(11.0 / 16);
    }
    effect.angle = 0;
    matrix = OamMatrix_AllocFromTransform(&effect);

    if (redraw & TARGET_REDRAW_SPREAD) {
      targetCount = 0;
      for (i = 0; i < 6; i++)
        cursorTracks[i].flags &= ~2;
      for (i = 0; (u32)i < spread; i++) {
        n = targetIndex + i;
        if (n < candidateCount && candidateIds[n] != BATTLE_UNIT_REMOVED) {
          spreadActorIds[targetCount] = candidateIds[n];
          cursorTracks[i].flags |= 2;
          if (cursorTracks[i].index != i) {
            cursorTracks[i].flags &= ~1;
            cursorTracks[i].index = i;
          }
          spreadTrackIndices[targetCount] = i;
          targetCount++;
        }
        if (i != 0 && (n = targetIndex - i) >= 0 &&
            candidateIds[n] != BATTLE_UNIT_REMOVED) {
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
      spreadActorIds[targetCount] = BATTLE_UNIT_LIST_END;
      BattleHighlight_SetModesFar(spreadActorIds, HIGHLIGHT_SELECTED);
      // The (u32) compares the stored id unsigned, as the bytes do.
      if ((u32)(BATTLE_MENU_WORK->lastTarget = candidateIds[targetIndex]) <=
          7) {
        if (spread != ABILITY_RANGE_ALL && kind != TARGET_HINT_NONE) {
          unit = Unit_Get(candidateIds[targetIndex]);
          BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                                     &selectedPosition);
          if (conditionWindow != NULL)
            Window_Close(conditionWindow, 1);
          switch (kind) {
          case TARGET_HINT_SET_DJINNI:
#if TLA_PATCH_2
          {
            s32 anchor = selectedPosition.x / 8;

            helpColumn = anchor - 4;
            if (System_GetLanguage() == CARTRIDGE_LANGUAGE_ITALIAN) {
              if (anchor + TARGET_SET_DJINNI_WIDTH_ITALIAN - 4 >
                  DISPLAY_TILE_WIDTH - 1)
                helpColumn =
                    DISPLAY_TILE_WIDTH - TARGET_SET_DJINNI_WIDTH_ITALIAN;
              conditionWindow = Window_Create(
                  helpColumn, 8, TARGET_SET_DJINNI_WIDTH_ITALIAN, 3, 6);
            } else if (System_GetLanguage() == CARTRIDGE_LANGUAGE_SPANISH) {
              if (anchor + TARGET_SET_DJINNI_WIDTH_SPANISH - 4 >
                  DISPLAY_TILE_WIDTH - 1)
                helpColumn =
                    DISPLAY_TILE_WIDTH - TARGET_SET_DJINNI_WIDTH_SPANISH;
              conditionWindow = Window_Create(
                  helpColumn, 8, TARGET_SET_DJINNI_WIDTH_SPANISH, 3, 6);
            } else {
              if (anchor + TARGET_SET_DJINNI_WIDTH - 4 > DISPLAY_TILE_WIDTH - 1)
                helpColumn = DISPLAY_TILE_WIDTH - TARGET_SET_DJINNI_WIDTH;
              conditionWindow =
                  Window_Create(helpColumn, 8, TARGET_SET_DJINNI_WIDTH, 3, 6);
            }
          }
#else
#ifdef BUGFIX
            TARGET_HELP_COLUMN(4, TARGET_SET_DJINNI_WIDTH);
#else
            // BUG: placed as the Japanese window, 8 tiles wide; the English one
            // is 9, so against the right edge its last column is past the
            // screen's.
            TARGET_HELP_COLUMN(4, 8);
#endif
            conditionWindow =
                Window_Create(helpColumn, 8, TARGET_SET_DJINNI_WIDTH, 3, 6);
#endif
            Text_SetColor(TEXT_COLOR_RED);
            Text_DrawMessageAsTiles((s32)MsgSetDjinni, conditionWindow, 0, 0);
            Text_SetColor(TEXT_COLOR_WHITE);
            break;
          case TARGET_HINT_HP:
            TARGET_HELP_COLUMN(7, 13);
            conditionWindow = Window_Create(helpColumn, 8, 13, 3, 6);
#if TLA_PATCH_2
            switch (System_GetLanguage()) {
            case CARTRIDGE_LANGUAGE_GERMAN:
              Text_DrawStringAtOffset(gOwnerStatus_KpString, conditionWindow, 0,
                                      0);
              break;
            case CARTRIDGE_LANGUAGE_FRENCH:
            case CARTRIDGE_LANGUAGE_SPANISH:
              Text_DrawStringAtOffset(gOwnerStatus_PvString, conditionWindow, 0,
                                      0);
              break;
            case CARTRIDGE_LANGUAGE_ITALIAN:
              Text_DrawStringAtOffset(gOwnerStatus_HpString, conditionWindow, 0,
                                      0);
              break;
            }
#else
            Text_DrawStringAtOffset(gOwnerStatus_HpString, conditionWindow, 0,
                                    0);
#endif
            Text_DrawNumberInWindow(unit->hp, 4, conditionWindow, 16, 0);
            Text_DrawStringAtOffset(gOwnerStatus_SlashString, conditionWindow,
                                    48, 0);
            Text_DrawNumberInWindow(unit->hpMax, 4, conditionWindow, 56, 0);
            break;
          case TARGET_HINT_PP:
            TARGET_HELP_COLUMN(7, 13);
            conditionWindow = Window_Create(helpColumn, 8, 13, 3, 6);
            Text_DrawStringAtOffset(gOwnerStatus_PpString, conditionWindow, 0,
                                    0);
            Text_DrawNumberInWindow(unit->pp, 4, conditionWindow, 16, 0);
            Text_DrawStringAtOffset(gOwnerStatus_SlashString, conditionWindow,
                                    48, 0);
            Text_DrawNumberInWindow(unit->ppMax, 4, conditionWindow, 56, 0);
            break;
          case TARGET_HINT_REVIVE:
            TARGET_HELP_COLUMN(7, 12);
            conditionWindow = Window_Create(helpColumn, 8, 12, 3, 6);
            if (unit->hp != 0)
              goto no_condition;
            Text_DrawMessageAsTiles((s32)MsgEffectDowned, conditionWindow, 0,
                                    0);
            break;
          case TARGET_HINT_CURE_POISON:
            TARGET_HELP_COLUMN(7, 12);
            conditionWindow = Window_Create(helpColumn, 8, 12, 3, 6);
            if (unit->poison == 0)
              goto no_condition;
            Text_DrawMessageAsTiles((s32)MsgEffectCurePoison, conditionWindow,
                                    0, 0);
            break;
          case TARGET_HINT_RESTORE: {
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
              Text_DrawMessageAsTiles((s32)MsgRemoveDelusion, conditionWindow,
                                      0, 0);
              count = 1;
            }
            if (unit->stun != 0) {
              Text_DrawMessageAsTiles((s32)MsgRemoveStun, conditionWindow, 0,
                                      count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              Text_DrawMessageAsTiles((s32)MsgWakeFromSleep, conditionWindow, 0,
                                      count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              Text_DrawMessageAsTiles((s32)MsgBreakPsynergySeal,
                                      conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              Text_DrawMessageAsTiles((s32)MsgDispelGrimReaper, conditionWindow,
                                      0, count * 8);
              count++;
            }
            if (count == 0)
              goto no_condition;
            break;
          }
          default:
            break;
          case TARGET_HINT_CURE_ALL: {
            count = 0;
            if (unit->poison != 0)
              count = 1;
            if (unit->delusion != 0)
              count++;
#if TLA_PATCH_2
            /* FAKEMATCH(temporary): count read back through a pointer to it. */
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
              Text_DrawMessageAsTiles((s32)MsgEffectCurePoison, conditionWindow,
                                      0, 0);
              count = 1;
            }
            if (unit->delusion != 0) {
              Text_DrawMessageAsTiles((s32)MsgRemoveDelusion, conditionWindow,
                                      0, count * 8);
              count++;
            }
            if (unit->stun != 0) {
              Text_DrawMessageAsTiles((s32)MsgRemoveStun, conditionWindow, 0,
                                      count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              Text_DrawMessageAsTiles((s32)MsgWakeFromSleep, conditionWindow, 0,
                                      count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              Text_DrawMessageAsTiles((s32)MsgBreakPsynergySeal,
                                      conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              Text_DrawMessageAsTiles((s32)MsgDispelGrimReaper, conditionWindow,
                                      0, count * 8);
              count++;
            }
            if (unit->haunt != 0) {
              Text_DrawMessageAsTiles((s32)MsgDispelEvilSpirit, conditionWindow,
                                      0, count * 8);
              count++;
            }
            if (count != 0)
              break;
          }
          no_condition:
            Text_SetColor(TEXT_COLOR_RED);
            Text_DrawMessageAsTiles((s32)MsgEffectNone, conditionWindow, 0, 0);
            Text_SetColor(TEXT_COLOR_WHITE);
            break;
          }
        }
      } else if (spread != ABILITY_RANGE_ALL) {
        target = Unit_Get(candidateIds[targetIndex]);
        BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                                   &namePosition);
        TARGET_BOB(namePosition.y);
        if (target->enemyId == ENEMY_MYSTERY_WOMAN ||
            target->enemyId == ENEMY_MYSTERY_MAN) {
          width = (s32)MsgMysteryMan;
          if (target->enemyId == ENEMY_MYSTERY_WOMAN)
            width++;
          TARGET_MYSTERY_NAME_COPY(width, targetName,
                                   TARGET_MYSTERY_NAME_CAPACITY);
        } else {
          for (i = 0; i < UNIT_NAME_SIZE - 1; i++) {
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
        Text_ClearCanvas();
        Text_DrawWideStringAtOffset(targetName, window, namePosition.x, 4);
      }
      redraw &= ~TARGET_REDRAW_SPREAD;
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
        if (spread == ABILITY_RANGE_ALL)
          marker->affine = 0;
        else
          marker->affine = 1;
        marker->matrix = matrix;
        Oam_AddSpriteAtDepth(marker, OAM_DEPTH_WINDOW);
      }
    }

    pressed = gInput.pressed;
    repeated = gInput.repeat;
    if (BATTLE_MENU_WORK->tutorialStep != 0) {
      repeated = 0;
      pressed = 0;
      if (BATTLE_MENU_WORK->tutorialDelay == 0) {
        BATTLE_MENU_WORK->tutorialDelay = 60;
        repeated = KEY_A;
        pressed = KEY_A;
      } else {
        BATTLE_MENU_WORK->tutorialDelay--;
      }
    }
    if (pressed & KEY_A) {
      s32 id = candidateIds[targetIndex];
      s32 k;

      redraw = 0;
      result = -1;
      for (k = 0; k <= 5 && battle->partyUnits[k] != BATTLE_UNIT_LIST_END;
           k++) {
        if (battle->partyUnits[k] == id) {
          result = PLACE_PARTY | k;
          break;
        }
      }
      if (result < 0) {
        for (k = 0; k <= 5 && battle->enemyIds[k] != BATTLE_UNIT_LIST_END;
             k++) {
          if (battle->enemyIds[k] == id) {
            result = PLACE_ENEMY | k;
            break;
          }
        }
      }
      targetIndex = result;
    } else if (spread != ABILITY_RANGE_ALL) {
      if (repeated & (KEY_RIGHT | KEY_DOWN)) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        do {
          targetIndex++;
          targetIndex %= candidateCount;
        } while (candidateIds[targetIndex] == BATTLE_UNIT_REMOVED);
        redraw |= TARGET_REDRAW_SPREAD;
      }
      if (repeated & (KEY_LEFT | KEY_UP)) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        do {
          targetIndex = targetIndex + candidateCount - 1;
          targetIndex %= candidateCount;
        } while (candidateIds[targetIndex] == BATTLE_UNIT_REMOVED);
        redraw |= TARGET_REDRAW_SPREAD;
      }
    }
    if (BATTLE_MENU_WORK->timer == 0 || (pressed & KEY_B)) {
      Sound_PlayFar(SE_MENU_CANCEL);
      targetIndex = -1;
      break;
    }
    wait(1);
    if (redraw == 0)
      break;
  }
  wait(1);
  VramCache_FreeEntry(cursorSlot);
  if (conditionWindow != NULL)
    Window_Close(conditionWindow, 1);
  Window_Close(window, 1);
  BattleHighlight_SetModesFar(candidateIds, HIGHLIGHT_OFF);
  BATTLE_MENU_WORK->targetViewOffset = 0;
  wait(1);
  return targetIndex;
}
#undef TARGET_SET_DJINNI_WIDTH
#if TLA_PATCH_2
#undef TARGET_SET_DJINNI_WIDTH_ITALIAN
#undef TARGET_SET_DJINNI_WIDTH_SPANISH
#undef TARGET_CONDITION_LIST_WIDTH_SPANISH
#endif
#undef TARGET_CONDITION_LIST_WIDTH
#undef TARGET_MYSTERY_NAME_COPY
#undef TARGET_MYSTERY_NAME_CAPACITY
#undef TARGET_HELP_COLUMN
#undef TARGET_CONDITION_LIST_WINDOW
#undef TARGET_BOB
#undef TARGET_REDRAW_SPREAD
#undef TARGET_REDRAW_ALL
