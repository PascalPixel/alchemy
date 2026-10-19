/// @file
/// Part of battle_command.c, the menu object, compiled only through it.
/// Choosing a battle target.

#include "../battle_command_internal.h"

#if LANGUAGE == LANGUAGE_JAPANESE
#define TARGET_MYSTERY_NAME_COPY UiText_DecodeMessage
#define TARGET_MYSTERY_NAME_CAPACITY 10
#else
#define TARGET_MYSTERY_NAME_COPY UiText_CopyMessageString
#define TARGET_MYSTERY_NAME_CAPACITY 14
#endif

struct CursorSlot {
  u8 x;
  u8 y;
  u8 flags;
  s8 index;
};

/// Choose a party/enemy target and animate its selection markers. Return the
/// encoded side and party position, or -1 on cancel. Healing and status-cure
/// categories choose an appropriate initial ally and supply condition help.
s32 BattleTarget_RunSelection(s32 selection, s32 mode, u32 spread, u32 kind) {
  u16 candidateIds[8];
  struct SelectionSprite cursorSprites[6];
  struct CursorSlot cursorTracks[6];
  struct EffectPosition actorPosition;
  u16 spreadActorIds[14];
  u8 spreadTrackIndices[8];
  struct EffectPosition selectedPosition;
  u16 targetName[15];
  struct EffectPosition namePosition;
  struct EffectPosition markerPosition;
  struct AffineTransform effect;
  struct BattleUnit *unit;
  struct BattleWork *battle;
  struct UiWindow *window;
#if TBS_PATCH_4
  struct UiWindow *conditionWindow;
#else
  /* FAKEMATCH(pin): the condition window in r9, where the native keeps it; as
   * an ordinary local it is spilled, the frame shrinks to 320 bytes and 969
   * of 1585 instructions differ in tbs-en. */
  register struct UiWindow *conditionWindow asm("r9");
#endif
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
  struct BattleUnit *target;

  battle = gBattleWork;
  targetCount = 0;
  redraw = 0xFFFF;
  cursorSlot = VramCache_AllocEntry(256);
#if TBS_PATCH_4
  /* FAKEMATCH: register choice only: this value-preserving null
   * initializer is needed to match infoWin in r10; a direct zero swaps
   * high registers and spill slots (2026-10-13: infoWin = 0 differs 94
   * in ES and IT). */
  conditionWindow = (struct UiWindow *)((u32)(u16)selection >> 16);
#else
  conditionWindow = 0;
#endif
  if (spread == 0)
    spread = 1;
  // UB: slot 57 is outside the scalar gBattleWork pointer. Named accesses
  // add 12 bytes here in EN; using gWorkSlot adds 16 bytes.
#ifdef UBFIX
  if (mode == 2 || mode == 4)
    gLinkCountdownWork->slideOffset = -2;
  else
    gLinkCountdownWork->slideOffset = 16;
#else
  if (mode == 2 || mode == 4)
    ((struct MenuCell **)&gBattleWork)[48]->slideOffset = -2;
  else
    ((struct MenuCell **)&gBattleWork)[48]->slideOffset = 16;
#endif
  for (i = 0; i < 6; i++)
    cursorTracks[i].flags = 0;
  targetIndex = -1;
  if (mode == 2) {
    for (i = 0; i <= 5 && battle->partyUnits[i] != 0xFF; i++) {
      candidateIds[targetCount] = battle->partyUnits[i];
      targetCount++;
    }
  } else if (mode == 4) {
    candidateIds[0] = (u16)selection;
    targetCount = 1;
  } else {
    for (i = 0; i <= 5 && battle->enemyIds[i] != 0xFF; i++) {
      candidateIds[targetCount] = battle->enemyIds[i];
      targetCount++;
    }
  }
  // Keep the candidate list separate from the markers drawn for a spread.
  candidateIds[targetCount] = 0xFF;
  candidateCount = targetCount;
  // Restorative actions prefer an ally who benefits from their effect.
  if (mode == 2) {
    if (spread != ABILITY_RANGE_ALL && kind != 0) {
      found = 0;
      for (j = 0; j < targetCount; j++) {
        struct BattleUnit *candidate;

        if (candidateIds[j] == 0xFE)
          continue;
        candidate = UnitGetFar(candidateIds[j]);
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
  while (candidateIds[targetIndex] == 0xFE ||
         (GameFlag_TestFar(FLAG_LINK_BATTLE) && mode == 1 &&
          UnitGetFar(candidateIds[targetIndex])->hp == 0)) {
    targetIndex = targetIndex + targetCount - 1;
    targetIndex %= targetCount;
  }
  if (mode != 2) {
    BattleMotion_ProjectConditionalPositionFar(
        selection, EffectPosition_Coordinates(&actorPosition));
    cursorTracks[0].flags = 8;
    cursorTracks[0].x = (u8)actorPosition.x;
    cursorTracks[0].y = 0x80;
  }
  window = UiWindow_Create(0, 12, 30, 4, 74);
  while (1) {
    firstSprite = cursorSprites;
    drawCursors = 0;
    {
      s32 t = targetIndex * 2;

      /* FAKEMATCH: the separate switch arms end CSE before the
       * projection call, keeping the scaled offset reload ahead of
       * the ids load. Sharing their block reverses two rows; no
       * switch branch remains in any edition. Plain forms measured
       * (2026-10-13, EN and ES alike): ids[cursor] or a block-local
       * t = cursor * 2 reload ids before cursor * 2 (differ 2);
       * cursor[ids], *(cursor + ids) and (u8 *)ids + cursor * 2
       * differ 24; the call ahead of head and pending 12 to 24. */
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
          EffectPosition_Coordinates(&selectedPosition));
    }
    ((s32 *)firstSprite)[1] = 0x40002000;
    ((s32 *)firstSprite)[2] = 0;
    firstSprite->tile =
        VramCache_Reload(cursorSlot, (((gFrameCount >> 2) & 31) << 8) +
                                         (s32)gMenu_AnimatedCursorTiles);
    selectedPosition.y += Trig_Sin(gFrameCount << 12) / 32768;
    y = selectedPosition.y;
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
    Runtime_PushSlotEntry((s32 *)firstSprite, 240);
    if (spread == ABILITY_RANGE_ALL) {
      effect.scaleX = 256;
      effect.scaleY = 256;
    } else {
      effect.scaleX = 176;
      effect.scaleY = 176;
    }
    effect.angle = 0;
#if !(LANGUAGE == LANGUAGE_SPANISH) && !(LANGUAGE == LANGUAGE_ITALIAN)
    /* FAKEMATCH(dead): seventeen dead stores restore the native frame's spill
     * slots (table, entries, ids and cursor * 2 at sp+36 to sp+24, above
     * &effect and selectionSlot); without them, 65 of 1585 instructions
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
    matrix = AffineMatrix_BuildForEffect(&effect);
    if (redraw & 1) {
      targetCount = 0;
      for (i = 0; i < 6; i++)
        cursorTracks[i].flags &= (u8)~2;
      for (i = 0; (u32)i < spread; i++) {
        {
          n = targetIndex + i;
          if (n < candidateCount && candidateIds[n] != 0xFE) {
            spreadActorIds[targetCount] = candidateIds[n];
            cursorTracks[i].flags |= 2;
            if (cursorTracks[i].index != i) {
              cursorTracks[i].flags &= (u8)~1;
              cursorTracks[i].index = (s8)i;
            }
            spreadTrackIndices[targetCount] = (u8)i;
            targetCount++;
          }
          if (i != 0 && (n = targetIndex - i) >= 0 && candidateIds[n] != 0xFE) {
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
      spreadActorIds[targetCount] = 0xFF;
      BattlePres_SetActorModesFar(spreadActorIds, 1);
      if (candidateIds[targetIndex] <= 7) {
        if (spread != ABILITY_RANGE_ALL && kind != 0) {
          unit = UnitGetFar(candidateIds[targetIndex]);
          BattleMotion_ProjectConditionalPositionFar(
              candidateIds[targetIndex],
              EffectPosition_Coordinates(&selectedPosition));
          if (conditionWindow != 0)
            UiWork_Finalize(conditionWindow, 1);
          switch (kind) {
          case 7: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 4;
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
            if (System_GetLanguage() == 4) {
              if (anchor + 6 > 29)
                helpColumn = 20;
              conditionWindow = UiWindow_Create(helpColumn, 8, 10, 3, 6);
            } else if (System_GetLanguage() == 3) {
              if (anchor + 7 > 29)
                helpColumn = 19;
              conditionWindow = UiWindow_Create(helpColumn, 8, 11, 3, 6);
            } else {
              if (anchor + 5 > 29)
                helpColumn = 21;
              conditionWindow = UiWindow_Create(helpColumn, 8, 9, 3, 6);
            }
#elif (LANGUAGE == LANGUAGE_FRENCH)
            if (anchor + 5 > 29)
              helpColumn = 21;
            conditionWindow = UiWindow_Create(helpColumn, 8, 9, 3, 6);
#elif (LANGUAGE == LANGUAGE_JAPANESE)
            if (anchor + 4 > 29)
              helpColumn = 22;
            conditionWindow = UiWindow_Create(helpColumn, 8, 8, 3, 6);
#else
            if (anchor + 4 > 29)
              helpColumn = 22;
            conditionWindow = UiWindow_Create(helpColumn, 8, 9, 3, 6);
#endif
          }
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset(MsgTargetSetDjinni, conditionWindow, 0,
                                         0);
            UiWork_SetParamNibble(15);
            break;
          case 1: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 7;
            if (anchor + 6 > 29)
              helpColumn = 17;
            conditionWindow = UiWindow_Create(helpColumn, 8, 13, 3, 6);
          }
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
            switch (System_GetLanguage()) {
            case 1:
              UiText_DrawStringAtOffset((u8 *)gOwnerStatus_KpString,
                                        conditionWindow, 0, 0);
              break;
            case 2:
              UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PvString,
                                        conditionWindow, 0, 0);
              break;
            case 3:
              UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PvString,
                                        conditionWindow, 0, 0);
              break;
            case 4:
              UiText_DrawStringAtOffset((u8 *)gOwnerStatus_HpString,
                                        conditionWindow, 0, 0);
              break;
            }
#else
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_HpString,
                                      conditionWindow, 0, 0);
#endif
            UiText_DrawNumberInWindow(unit->hp, 4, conditionWindow, 16, 0);
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString,
                                      conditionWindow, 48, 0);
            UiText_DrawNumberInWindow(unit->maxHp, 4, conditionWindow, 56, 0);
            break;
          case 2: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 7;
            if (anchor + 6 > 29)
              helpColumn = 17;
            conditionWindow = UiWindow_Create(helpColumn, 8, 13, 3, 6);
          }
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PpString,
                                      conditionWindow, 0, 0);
            UiText_DrawNumberInWindow(unit->pp, 4, conditionWindow, 16, 0);
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString,
                                      conditionWindow, 48, 0);
            UiText_DrawNumberInWindow(unit->maxPp, 4, conditionWindow, 56, 0);
            break;
          case 5: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 7;
            if (anchor + 5 > 29)
              helpColumn = 18;
            conditionWindow = UiWindow_Create(helpColumn, 8, 12, 3, 6);
          }
            if (unit->hp != 0)
              goto no_condition;
            UiText_DrawCharacterAtOffset(MsgTargetDowned, conditionWindow, 0,
                                         0);
            break;
          case 3: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 7;
            if (anchor + 5 > 29)
              helpColumn = 18;
            conditionWindow = UiWindow_Create(helpColumn, 8, 12, 3, 6);
          }
            if (unit->poison == 0)
              goto no_condition;
            UiText_DrawCharacterAtOffset(MsgTargetCurePoison, conditionWindow,
                                         0, 0);
            break;
          case 4:
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
                    UiWindow_Create(helpColumn, helpRow, 17, count + 2, 6);
              } else {
                if (anchor + 9 > 29)
                  helpColumn = 14;
                conditionWindow =
                    UiWindow_Create(helpColumn, helpRow, 16, count + 2, 6);
              }
#elif (LANGUAGE == LANGUAGE_JAPANESE)
              if (anchor + 7 > 29)
                helpColumn = 16;
              conditionWindow =
                  UiWindow_Create(helpColumn, helpRow, 14, count + 2, 6);
#else
              if (anchor + 9 > 29)
                helpColumn = 14;
              conditionWindow =
                  UiWindow_Create(helpColumn, helpRow, 16, count + 2, 6);
#endif
            }
            count = 0;
            if (unit->delusion != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureDelusion,
                                           conditionWindow, 0, 0);
              count = 1;
            }
            if (unit->stun != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureStun, conditionWindow,
                                           0, count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureSleep, conditionWindow,
                                           0, count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureSeal, conditionWindow,
                                           0, count * 8);
              ++count;
            }
            if (unit->grimReaper != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureCurse, conditionWindow,
                                           0, count * 8);
              count++;
            }
            if (count == 0)
              goto no_condition;
            break;
          default:
            break;
          case 6:
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
                    UiWindow_Create(helpColumn, helpRow, 17, count + 2, 6);
              } else {
                if (anchor + 9 > 29)
                  helpColumn = 14;
                conditionWindow =
                    UiWindow_Create(helpColumn, helpRow, 16, count + 2, 6);
              }
#elif (LANGUAGE == LANGUAGE_JAPANESE)
              if (anchor + 7 > 29)
                helpColumn = 16;
              conditionWindow =
                  UiWindow_Create(helpColumn, helpRow, 14, count + 2, 6);
#else
              if (anchor + 9 > 29)
                helpColumn = 14;
              conditionWindow =
                  UiWindow_Create(helpColumn, helpRow, 16, count + 2, 6);
#endif
            }
            count = 0;
            if (unit->poison != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCurePoison, conditionWindow,
                                           0, 0);
              count = 1;
            }
            if (unit->delusion != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureDelusion,
                                           conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->stun != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureStun, conditionWindow,
                                           0, count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureSleep, conditionWindow,
                                           0, count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureSeal, conditionWindow,
                                           0, count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureCurse, conditionWindow,
                                           0, count * 8);
              count++;
            }
            if (unit->haunt != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureHaunt, conditionWindow,
                                           0, 8 * count);
              count++;
            }
            if (count != 0)
              break;
          no_condition:
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset(MsgTargetNoEffect, conditionWindow, 0,
                                         0);
            UiWork_SetParamNibble(15);
            break;
          }
        }
      } else if (spread != ABILITY_RANGE_ALL) {
        target = UnitGetFar(candidateIds[targetIndex]);
        BattleMotion_ProjectConditionalPositionFar(
            candidateIds[targetIndex],
            EffectPosition_Coordinates(&namePosition));
        namePosition.y += Trig_Sin(gFrameCount << 12) / 32768;
        if (target->classId == 125 || target->classId == 122) {
          width = MsgMysteryPersonName;
          if (target->classId == 125)
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
        width = UiText_GetWideStringWidth(targetName);
        namePosition.x -= width / 2;
        namePosition.x -= 8;
        if (namePosition.x + width > 224)
          namePosition.x = 224 - width;
        if (namePosition.x < 0)
          namePosition.x = 0;
        Ui_ClearVramBlock();
        UiText_RenderWideStringAtOffset(targetName, window, namePosition.x, 4);
      }
      redraw &= ~1;
    }
    if (drawCursors != 0) {
      for (i = 1; i < targetCount; i++) {
        marker = &firstSprite[i];
        track = &cursorTracks[spreadTrackIndices[i]];
        BattleMotion_ProjectConditionalPositionFar(
            spreadActorIds[i], EffectPosition_Coordinates(&markerPosition));
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
        marker->param = matrix;
        Runtime_PushSlotEntry((s32 *)marker, 240);
      }
    }
    pressed = gKeysPressed;
    repeated = gKeysRepeat;
    if (gLinkCountdownWork->autoEnabled) {
      repeated = 0;
      pressed = 0;
      if (gLinkCountdownWork->autoDelay == 0) {
        gLinkCountdownWork->autoDelay = 60;
        repeated = 1;
        pressed = 1;
      } else {
        gLinkCountdownWork->autoDelay--;
      }
    }
    if (pressed & KEY_A) {
      s32 id;
      s32 k;

      id = candidateIds[targetIndex];
      redraw = 0;
      result = -1;
      for (k = 0; k <= 5 && battle->partyUnits[k] != 0xFF; k++) {
        if (battle->partyUnits[k] == id) {
          result = 0x100 | k;
          break;
        }
      }
      if (result < 0) {
        for (k = 0; k <= 5 && battle->enemyIds[k] != 0xFF; k++) {
          if (battle->enemyIds[k] == id) {
            result = 0x180 | k;
            break;
          }
        }
      }
      targetIndex = result;
    } else if (spread != ABILITY_RANGE_ALL) {
      if (repeated & (KEY_DOWN | KEY_RIGHT)) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        do {
          targetIndex++;
          targetIndex %= candidateCount;
        } while (candidateIds[targetIndex] == 0xFE);
        redraw |= 1;
      }
      if (repeated & (KEY_UP | KEY_LEFT)) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        do {
          targetIndex = targetIndex + candidateCount - 1;
          targetIndex %= candidateCount;
        } while (candidateIds[targetIndex] == 0xFE);
        redraw |= 1;
      }
    }
    if (gLinkCountdownWork->active == 0 || (pressed & KEY_B)) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      targetIndex = -1;
      break;
    }
    WaitFrames(1);
    if (redraw == 0)
      break;
  }
  WaitFrames(1);
  Resource_ResetEntry(cursorSlot);
  if (conditionWindow != 0)
    UiWork_Finalize(conditionWindow, 1);
  UiWork_Finalize(window, 1);
  BattlePres_SetActorModesFar(candidateIds, 0);
  gLinkCountdownWork->slideOffset = 0;
  WaitFrames(1);
  return targetIndex;
}
