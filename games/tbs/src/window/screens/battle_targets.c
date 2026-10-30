/// @file
/// Choosing a battle target.
// Part of screens.c.

#include "../screens_internal.h"

#if LANGUAGE == LANGUAGE_JAPANESE
#define TARGET_MYSTERY_NAME_COPY Message_DecodeRaw
#define TARGET_MYSTERY_NAME_CAPACITY 10
#else
#define TARGET_MYSTERY_NAME_COPY Message_CopyExpanded
#define TARGET_MYSTERY_NAME_CAPACITY 14
#endif

/// A target marker's track: where it is, its state flags and the target it
/// follows. Track 0 follows the aimed unit, track i the unit i places after
/// it and track 6 - i the unit i places before.
struct CursorSlot {
  u8 x; ///< screen pixels
  u8 y; ///< screen pixels
  /// bits: 7..2 frames to stay put (the byte drops by 4 a frame) | 1 in the
  /// current spread | 0 easing halfway to its unit each frame
  u8 flags;
  s8 index; ///< the place it follows from the aimed one; 6 when unused
};

/// Lets the player choose the target of an action of unit `selection`, with
/// a cursor over the aimed unit and smaller markers on the others its range
/// covers. `mode` is the action's targetMode: ABILITY_TARGET_ALLIES the party,
/// ABILITY_TARGET_SELF only `selection` itself, anything else the enemies
/// (battle_internal.h has the names, which this folder cannot see yet).
/// `spread` is its range: 1 one unit, n the aimed one and n - 1 either side,
/// ABILITY_RANGE_ALL every one (the cursor does not move); 0 counts as 1.
/// `kind` (enum BattleTargetHint) is Battle_ClassifyEntryKind's class, or
/// TARGET_HINT_SET_DJINNI: on a party member it opens a window of what the
/// action would do (its HP or PP, the statuses it cures, "Set Djinni"), and a
/// reviving or curing action not aimed at everyone starts on the first party
/// member who needs it. On an enemy a window across the screen names it.
///
/// Returns 0x100 | the party place or 0x180 | the enemy place of the unit
/// chosen with A, or -1 when cancelled or when the link battle's time runs
/// out. While it runs, the battle view slides (targetViewOffset of
/// BattleMenuWork: -2 for the party, 16 for the enemies) and the covered
/// units are marked (BattleHighlight_SetModesFar). The Djinn tutorial
/// presses A every 60 frames.
s32 BattleTarget_RunSelection(s32 selection, s32 mode, u32 spread, u32 kind) {
  u16 candidateIds[8];
  struct SelectionSprite cursorSprites[6];
  struct CursorSlot cursorTracks[6];
  struct ScreenPoint actorPosition;
  u16 spreadActorIds[14];
  u8 spreadTrackIndices[8];
  struct ScreenPoint selectedPosition;
  u16 targetName[15];
  struct ScreenPoint namePosition;
  struct ScreenPoint markerPosition;
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
  struct CursorSlot *track;
  struct SelectionSprite *marker;
  struct SelectionSprite *firstSprite;
  struct Unit *target;

  battle = gBattleWork;
  targetCount = 0;
  redraw = 0xFFFF;
  cursorSlot = VramCache_AllocEntry(256);
  /* FAKEMATCH(dead): a null derived from selection, with no constant
   * equivalence, keeps conditionWindow in a register; as conditionWindow = 0,
   * 492 of 1585 instructions differ in tbs-en. */
  conditionWindow = (struct Window *)((u32)(u16)selection >> 16);
  if (spread == 0)
    spread = 1;
  // UB: slot 57 is outside the scalar gBattleWork pointer. Named accesses
  // add 12 bytes here in EN; using gHeapSlots adds 16 bytes.
#ifdef UBFIX
  if (mode == ABILITY_TARGET_ALLIES || mode == ABILITY_TARGET_SELF)
    gBattleMenuWork->targetViewOffset = -2;
  else
    gBattleMenuWork->targetViewOffset = 16;
#else
  if (mode == ABILITY_TARGET_ALLIES || mode == ABILITY_TARGET_SELF)
    ((struct BattleMenuWork **)&gBattleWork)[48]->targetViewOffset = -2;
  else
    ((struct BattleMenuWork **)&gBattleWork)[48]->targetViewOffset = 16;
#endif
  for (i = 0; i < 6; i++)
    cursorTracks[i].flags = 0;
  // The candidates: the party, the acting unit alone, or the enemies.
  targetIndex = -1;
  if (mode == ABILITY_TARGET_ALLIES) {
    for (i = 0; i <= 5 && battle->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
      candidateIds[targetCount] = battle->partyUnits[i];
      targetCount++;
    }
  } else if (mode == ABILITY_TARGET_SELF) {
    candidateIds[0] = (u16)selection;
    targetCount = 1;
  } else {
    for (i = 0; i <= 5 && battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
      candidateIds[targetCount] = battle->enemyIds[i];
      targetCount++;
    }
  }
  // Keep the candidate list separate from the markers drawn for a spread.
  candidateIds[targetCount] = BATTLE_UNIT_LIST_END;
  candidateCount = targetCount;
  // Restorative actions prefer an ally who benefits from their effect.
  // Otherwise the cursor starts on the acting unit when it is a
  // candidate.
  if (mode == ABILITY_TARGET_ALLIES) {
    if (spread != ABILITY_RANGE_ALL && kind != TARGET_HINT_NONE) {
      found = 0;
      for (j = 0; j < targetCount; j++) {
        struct Unit *candidate;

        if (candidateIds[j] == BATTLE_UNIT_REMOVED)
          continue;
        candidate = Unit_GetFar(candidateIds[j]);
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
  // Else the middle one; then step back past empty places and, in a link
  // battle, past downed enemies.
  if (targetIndex < 0)
    targetIndex = (targetCount - 1) / 2;
  while (candidateIds[targetIndex] == BATTLE_UNIT_REMOVED ||
         (GameFlag_GetFar(FLAG_LINK_BATTLE) && mode == ABILITY_TARGET_FOES &&
          Unit_GetFar(candidateIds[targetIndex])->hp == 0)) {
    targetIndex = targetIndex + targetCount - 1;
    targetIndex %= targetCount;
  }
  // Aimed at enemies or itself, the cursor starts at the acting unit (at y
  // 128), stays two frames, then eases to the target.
  if (mode != ABILITY_TARGET_ALLIES) {
    BattleMotion_ProjectConditionalPositionFar(
        selection, ScreenPoint_Coordinates(&actorPosition));
    cursorTracks[0].flags = 8;
    cursorTracks[0].x = (u8)actorPosition.x;
    cursorTracks[0].y = 128;
  }
#if !TBS_PATCH_4
  /* FAKEMATCH(barrier): an empty asm clobbering r8 and r10 once selection and
   * mode are dead, so conditionWindow takes r9, selection r10 and mode r8; as
   * no asm, 96 of 1585 instructions differ in tbs-en. */
  __asm__("" : : : "r8", "r10");
#endif
  window = Window_Create(0, 12, 30, 4,
                         WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                             WINDOW_FLAG_OUTLINED_TEXT);
  while (1) {
    firstSprite = cursorSprites;
    drawCursors = 0;
    {
      s32 t = targetIndex * 2;

      /* FAKEMATCH(dead): the switch arms, which emit no branch, end CSE before
       * the call and keep the offset reload ahead of the ids load; as
       * candidateIds[targetIndex], 1 of 1585 instructions differ in tbs-en. */
      switch (t) {
      case 0:
        t = targetIndex * 2;
        break;
      default:
        t = targetIndex * 2;
        break;
      }
      BattleMotion_ProjectConditionalPositionFar(
          *(u16 *)((u8 *)candidateIds + t),
          ScreenPoint_Coordinates(&selectedPosition));
    }
    // The cursor: a 16x16, 256-colour sprite of 32 frames (256 bytes each),
    // a frame every four frames, bobbing 2 pixels each way every 16 frames.
    ((s32 *)firstSprite)[1] = 0x40002000;
    ((s32 *)firstSprite)[2] = 0;
    firstSprite->tile =
        VramCache_Reload(cursorSlot, (((gFrameCount >> 2) & 31) << 8) +
                                         (s32)gMenu_AnimatedCursorTiles);
    selectedPosition.y += Trig_Sin(gFrameCount << 12) / 32768;
    y = selectedPosition.y;
    // Ease halfway each frame; the other markers show once the cursor is
    // within 7 pixels of its target.
    if (cursorTracks[0].flags & 1) {
      cursorX = (selectedPosition.x + cursorTracks[0].x) / 2;
      cursorY = (y + cursorTracks[0].y) / 2;
      if ((selectedPosition.x - cursorX < 0
               ? -(selectedPosition.x - cursorX)
               : selectedPosition.x - cursorX) <= 7)
        drawCursors = 1;
      selectedPosition.x = cursorX;
      cursorTracks[0].x = (u8)cursorX;
      cursorTracks[0].y = (u8)cursorY;
      selectedPosition.y = cursorY;
    } else if ((u8)cursorTracks[0].flags <= 3) {
      cursorTracks[0].x = (u8)selectedPosition.x;
      selectedPosition.y = y;
      cursorTracks[0].y = (u8)y;
      cursorTracks[0].flags = 1;
    } else {
      selectedPosition.x = cursorTracks[0].x;
      selectedPosition.y = cursorTracks[0].y;
      cursorTracks[0].flags = (u8)(cursorTracks[0].flags - 4);
      if (cursorTracks[0].flags <= 3)
        cursorTracks[0].flags = 1;
    }
    firstSprite->x = selectedPosition.x - 8;
    firstSprite->y = (u8)(selectedPosition.y - 16);
    Oam_AddSpriteAtDepth((s32 *)firstSprite, 240);
    // The other markers' scale in 8.8: 176 (0.69 times) beside an aimed one,
    // full size when the action takes every unit.
    if (spread == ABILITY_RANGE_ALL) {
      effect.scaleX = 256;
      effect.scaleY = 256;
    } else {
      effect.scaleX = 176;
      effect.scaleY = 176;
    }
    effect.angle = 0;
#if !TBS_PATCH_4
    /* FAKEMATCH(dead): seventeen dead stores restore the native stack frame's
     * spill slots at sp+36 to sp+24; without them, 65 of 1585 instructions
     * differ in tbs-en, each a spill offset. */
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
#endif
    matrix = OamMatrix_AllocFromTransform(&effect);
    // The aim moved: list the units the range covers, the aimed one first,
    // mark them, and redraw the window for the aimed one.
    if (redraw & 1) {
      targetCount = 0;
      for (i = 0; i < 6; i++)
        cursorTracks[i].flags &= (u8)~2;
      for (i = 0; (u32)i < spread; i++) {
        {
          n = targetIndex + i;
          if (n < candidateCount && candidateIds[n] != BATTLE_UNIT_REMOVED) {
            spreadActorIds[targetCount] = candidateIds[n];
            cursorTracks[i].flags |= 2;
            if (cursorTracks[i].index != i) {
              cursorTracks[i].flags &= (u8)~1;
              cursorTracks[i].index = (s8)i;
            }
            spreadTrackIndices[targetCount] = (u8)i;
            targetCount++;
          }
          if (i != 0 && (n = targetIndex - i) >= 0 &&
              candidateIds[n] != BATTLE_UNIT_REMOVED) {
            spreadActorIds[targetCount] = candidateIds[n];
            cursorTracks[6 - i].flags |= 2;
            if (cursorTracks[6 - i].index != -i) {
              cursorTracks[6 - i].flags &= (u8)~1;
              cursorTracks[6 - i].index = (s8)-i;
            }
            spreadTrackIndices[targetCount] = (u8)(6 - i);
            targetCount++;
          }
        }
      }
      for (i = 0; i < 6; i++) {
        if ((cursorTracks[i].flags & 2) == 0)
          cursorTracks[i].index = 6;
      }
      spreadActorIds[targetCount] = BATTLE_UNIT_LIST_END;
      BattleHighlight_SetModesFar(spreadActorIds, 1);
      // A party member (units 0 to 7): the window of what the action does,
      // placed by the party member's tile column (anchor) and kept on screen.
      if (candidateIds[targetIndex] <= 7) {
        if (spread != ABILITY_RANGE_ALL && kind != TARGET_HINT_NONE) {
          unit = Unit_GetFar(candidateIds[targetIndex]);
          BattleMotion_ProjectConditionalPositionFar(
              candidateIds[targetIndex],
              ScreenPoint_Coordinates(&selectedPosition));
          if (conditionWindow != 0)
            Window_Close(conditionWindow, 1);
          switch (kind) {
          case TARGET_HINT_SET_DJINNI: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 4;
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
            if (System_GetLanguage() == 4) {
              if (anchor + 6 > 29)
                helpColumn = 20;
              conditionWindow = Window_Create(helpColumn, 8, 10, 3, 6);
            } else if (System_GetLanguage() == 3) {
              if (anchor + 7 > 29)
                helpColumn = 19;
              conditionWindow = Window_Create(helpColumn, 8, 11, 3, 6);
            } else {
              if (anchor + 5 > 29)
                helpColumn = 21;
              conditionWindow = Window_Create(helpColumn, 8, 9, 3, 6);
            }
#elif (LANGUAGE == LANGUAGE_FRENCH)
            if (anchor + 5 > 29)
              helpColumn = 21;
            conditionWindow = Window_Create(helpColumn, 8, 9, 3, 6);
#elif (LANGUAGE == LANGUAGE_JAPANESE)
            if (anchor + 4 > 29)
              helpColumn = 22;
            conditionWindow = Window_Create(helpColumn, 8, 8, 3, 6);
#else
            if (anchor + 4 > 29)
              helpColumn = 22;
            conditionWindow = Window_Create(helpColumn, 8, 9, 3, 6);
#endif
          }
            Text_SetColor(TEXT_COLOR_RED);
            Text_DrawMessageAsTiles(MsgTargetSetDjinni, conditionWindow, 0, 0);
            Text_SetColor(TEXT_COLOR_WHITE);
            break;
          case TARGET_HINT_HP: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 7;
            if (anchor + 6 > 29)
              helpColumn = 17;
            conditionWindow = Window_Create(helpColumn, 8, 13, 3, 6);
          }
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
            switch (System_GetLanguage()) {
            case CARTRIDGE_LANGUAGE_GERMAN:
              Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_KpString,
                                      conditionWindow, 0, 0);
              break;
            case CARTRIDGE_LANGUAGE_FRENCH:
              Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_PvString,
                                      conditionWindow, 0, 0);
              break;
            case CARTRIDGE_LANGUAGE_SPANISH:
              Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_PvString,
                                      conditionWindow, 0, 0);
              break;
            case CARTRIDGE_LANGUAGE_ITALIAN:
              Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_HpString,
                                      conditionWindow, 0, 0);
              break;
            }
#else
            Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_HpString,
                                    conditionWindow, 0, 0);
#endif
            Text_DrawNumberInWindow(unit->hp, 4, conditionWindow, 16, 0);
            Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_SlashString,
                                    conditionWindow, 48, 0);
            Text_DrawNumberInWindow(unit->hpMax, 4, conditionWindow, 56, 0);
            break;
          case TARGET_HINT_PP: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 7;
            if (anchor + 6 > 29)
              helpColumn = 17;
            conditionWindow = Window_Create(helpColumn, 8, 13, 3, 6);
          }
            Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_PpString,
                                    conditionWindow, 0, 0);
            Text_DrawNumberInWindow(unit->pp, 4, conditionWindow, 16, 0);
            Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_SlashString,
                                    conditionWindow, 48, 0);
            Text_DrawNumberInWindow(unit->ppMax, 4, conditionWindow, 56, 0);
            break;
          case TARGET_HINT_REVIVE: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 7;
            if (anchor + 5 > 29)
              helpColumn = 18;
            conditionWindow = Window_Create(helpColumn, 8, 12, 3, 6);
          }
            if (unit->hp != 0)
              goto no_condition;
            Text_DrawMessageAsTiles(MsgTargetDowned, conditionWindow, 0, 0);
            break;
          case TARGET_HINT_CURE_POISON: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 7;
            if (anchor + 5 > 29)
              helpColumn = 18;
            conditionWindow = Window_Create(helpColumn, 8, 12, 3, 6);
          }
            if (unit->poison == 0)
              goto no_condition;
            Text_DrawMessageAsTiles(MsgTargetCurePoison, conditionWindow, 0, 0);
            break;
          case TARGET_HINT_RESTORE:
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
            {
              s32 anchor;

              anchor = selectedPosition.x / 8;
              helpColumn = anchor - 7;
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
              if (System_GetLanguage() == 3) {
                if (anchor + 10 > 29)
                  helpColumn = 13;
                conditionWindow =
                    Window_Create(helpColumn, helpRow, 17, count + 2, 6);
              } else {
                if (anchor + 9 > 29)
                  helpColumn = 14;
                conditionWindow =
                    Window_Create(helpColumn, helpRow, 16, count + 2, 6);
              }
#elif (LANGUAGE == LANGUAGE_JAPANESE)
              if (anchor + 7 > 29)
                helpColumn = 16;
              conditionWindow =
                  Window_Create(helpColumn, helpRow, 14, count + 2, 6);
#else
              if (anchor + 9 > 29)
                helpColumn = 14;
              conditionWindow =
                  Window_Create(helpColumn, helpRow, 16, count + 2, 6);
#endif
            }
            count = 0;
            if (unit->delusion != 0) {
              Text_DrawMessageAsTiles(MsgTargetCureDelusion, conditionWindow, 0,
                                      0);
              count = 1;
            }
            if (unit->stun != 0) {
              Text_DrawMessageAsTiles(MsgTargetCureStun, conditionWindow, 0,
                                      count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              Text_DrawMessageAsTiles(MsgTargetCureSleep, conditionWindow, 0,
                                      count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              Text_DrawMessageAsTiles(MsgTargetCureSeal, conditionWindow, 0,
                                      count * 8);
              ++count;
            }
            if (unit->grimReaper != 0) {
              Text_DrawMessageAsTiles(MsgTargetCureCurse, conditionWindow, 0,
                                      count * 8);
              count++;
            }
            if (count == 0)
              goto no_condition;
            break;
          default:
            break;
          case TARGET_HINT_CURE_ALL:
            count = 0;
            if (unit->poison != 0)
              count = 1;
            if (unit->delusion != 0)
              count++;
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
            if (count == 0)
              count = 1;
            helpRow = 9 - count;
            if (helpRow <= 3)
              helpRow = 4;
            {
              s32 anchor;

              anchor = selectedPosition.x / 8;
              helpColumn = anchor - 7;
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
              if (System_GetLanguage() == 3) {
                if (anchor + 10 > 29)
                  helpColumn = 13;
                conditionWindow =
                    Window_Create(helpColumn, helpRow, 17, count + 2, 6);
              } else {
                if (anchor + 9 > 29)
                  helpColumn = 14;
                conditionWindow =
                    Window_Create(helpColumn, helpRow, 16, count + 2, 6);
              }
#elif (LANGUAGE == LANGUAGE_JAPANESE)
              if (anchor + 7 > 29)
                helpColumn = 16;
              conditionWindow =
                  Window_Create(helpColumn, helpRow, 14, count + 2, 6);
#else
              if (anchor + 9 > 29)
                helpColumn = 14;
              conditionWindow =
                  Window_Create(helpColumn, helpRow, 16, count + 2, 6);
#endif
            }
            count = 0;
            if (unit->poison != 0) {
              Text_DrawMessageAsTiles(MsgTargetCurePoison, conditionWindow, 0,
                                      0);
              count = 1;
            }
            if (unit->delusion != 0) {
              Text_DrawMessageAsTiles(MsgTargetCureDelusion, conditionWindow, 0,
                                      count * 8);
              count++;
            }
            if (unit->stun != 0) {
              Text_DrawMessageAsTiles(MsgTargetCureStun, conditionWindow, 0,
                                      count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              Text_DrawMessageAsTiles(MsgTargetCureSleep, conditionWindow, 0,
                                      count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              Text_DrawMessageAsTiles(MsgTargetCureSeal, conditionWindow, 0,
                                      count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              Text_DrawMessageAsTiles(MsgTargetCureCurse, conditionWindow, 0,
                                      count * 8);
              count++;
            }
            if (unit->haunt != 0) {
              Text_DrawMessageAsTiles(MsgTargetCureHaunt, conditionWindow, 0,
                                      8 * count);
              count++;
            }
            if (count != 0)
              break;
          no_condition:
            Text_SetColor(TEXT_COLOR_RED);
            Text_DrawMessageAsTiles(MsgTargetNoEffect, conditionWindow, 0, 0);
            Text_SetColor(TEXT_COLOR_WHITE);
            break;
          }
        }
      } else if (spread != ABILITY_RANGE_ALL) {
        // An enemy: its name, centred under it and kept on the 224 pixels
        // of the window; ENEMY_MYSTERY_MAN and ENEMY_MYSTERY_WOMAN are named
        // "Mystery Man" and "Mystery Woman".
        target = Unit_GetFar(candidateIds[targetIndex]);
        BattleMotion_ProjectConditionalPositionFar(
            candidateIds[targetIndex], ScreenPoint_Coordinates(&namePosition));
        namePosition.y += Trig_Sin(gFrameCount << 12) / 32768;
        if (target->id == ENEMY_MYSTERY_WOMAN ||
            target->id == ENEMY_MYSTERY_MAN) {
          width = MsgMysteryPersonName;
          if (target->id == ENEMY_MYSTERY_WOMAN)
            width++;
          TARGET_MYSTERY_NAME_COPY(width, targetName,
                                   TARGET_MYSTERY_NAME_CAPACITY);
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
        Text_ClearCanvas();
        Text_DrawWideStringAtOffset(targetName, window, namePosition.x, 4);
      }
      redraw &= ~1;
    }
    if (drawCursors != 0) {
      for (i = 1; i < targetCount; i++) {
        marker = &firstSprite[i];
        track = &cursorTracks[spreadTrackIndices[i]];
        BattleMotion_ProjectConditionalPositionFar(
            spreadActorIds[i], ScreenPoint_Coordinates(&markerPosition));
        markerPosition.y =
            markerPosition.y + Trig_Sin(gFrameCount << 12) / 32768;
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
        Oam_AddSpriteAtDepth((s32 *)marker, 240);
      }
    }
    pressed = gKeysPressed;
    repeated = gKeysRepeat;
    if (gBattleMenuWork->tutorialStep) {
      repeated = 0;
      pressed = 0;
      if (gBattleMenuWork->tutorialDelay == 0) {
        gBattleMenuWork->tutorialDelay = 60;
        repeated = 1;
        pressed = 1;
      } else {
        gBattleMenuWork->tutorialDelay--;
      }
    }
    // A: the aimed unit's side and place; the loop ends after this frame.
    if (pressed & KEY_A) {
      s32 id;
      s32 k;

      id = candidateIds[targetIndex];
      redraw = 0;
      result = -1;
      for (k = 0; k <= 5 && battle->partyUnits[k] != BATTLE_UNIT_LIST_END;
           k++) {
        if (battle->partyUnits[k] == id) {
          result = 0x100 | k;
          break;
        }
      }
      if (result < 0) {
        for (k = 0; k <= 5 && battle->enemyIds[k] != BATTLE_UNIT_LIST_END;
             k++) {
          if (battle->enemyIds[k] == id) {
            result = 0x180 | k;
            break;
          }
        }
      }
      targetIndex = result;
    } else if (spread != ABILITY_RANGE_ALL) {
      if (repeated & (KEY_DOWN | KEY_RIGHT)) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        do {
          targetIndex++;
          targetIndex %= candidateCount;
        } while (candidateIds[targetIndex] == BATTLE_UNIT_REMOVED);
        redraw |= 1;
      }
      if (repeated & (KEY_UP | KEY_LEFT)) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        do {
          targetIndex = targetIndex + candidateCount - 1;
          targetIndex %= candidateCount;
        } while (candidateIds[targetIndex] == BATTLE_UNIT_REMOVED);
        redraw |= 1;
      }
    }
    if (gBattleMenuWork->timer == 0 || (pressed & KEY_B)) {
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
  if (conditionWindow != 0)
    Window_Close(conditionWindow, 1);
  Window_Close(window, 1);
  BattleHighlight_SetModesFar(candidateIds, 0);
  gBattleMenuWork->targetViewOffset = 0;
  wait(1);
  return targetIndex;
}
