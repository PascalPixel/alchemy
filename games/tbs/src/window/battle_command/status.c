/// @file
/// The status screen.
/// Part of battle_command.c, the menu object, compiled only through it.

#include "../battle_command_internal.h"

#if TBS_PATCH_1
#define OWNER_STATUS_TEXT_MAX 0x80
#define OWNER_STATUS_VALUE_X 184
#else
#define OWNER_STATUS_TEXT_MAX 0x40
#define OWNER_STATUS_VALUE_X 176
#endif

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define OWNER_STATUS_LABEL(row) (MsgOwnerStatusAttack + (row))
#else
#define OWNER_STATUS_LABEL(row) (MsgOwnerStatusExp - 10 + (row))
#endif

#define STATUS_CURSOR_GENERAL 0
#define STATUS_CURSOR_COMBAT_STATS 1
#define STATUS_CURSOR_CONDITIONS 2

/// The status screen's cursor: its place in each group, the help entry it
/// shows and where it points.
struct StatusCursor {
  /// XOR2 navigation can transiently use index3; keep all five positions.
  s8 groupPositions[5];
  s32 helpEntry;
  s32 drawnHelpEntry;
  s32 group;
  s32 groupSize;
  s32 column; ///< tiles
  s32 row;    ///< tiles
};

/// Shows a party member's stats and condition help until B or the link session
/// ends. The supplied list enables shoulder-button member switching. Measure
/// stage deltas temporarily, then restore the unit from its snapshot.
s32 Ui_RunOwnerStatusScreen(u16 *list, s32 listCount, s32 owner) {
  struct MenuSprite portraitSprite;
  struct MenuSprite cursorSprite;
  s32 statusSlots[11];
  s8 statusKinds[11];
  struct MenuSprite statusSprites[11];
  struct MenuSprite nameSprite;
  struct StatusCursor cursor;
  struct UiRenderWork *render;
  struct UiWindow *statusWindow;
  struct UiWindow *helpWindow;
  s32 ownerIndex;
  s32 cursorSlot;
  struct MenuPoint cursorPosition;
  s32 portraitSlot;
  s32 statusCount;
  s32 hasDjinn;
  s32 keys;
  u16 *helpText;
  s32 statusKind;
  s32 *statusSlot;
  s32 defenseDelta;
  s32 i;
  s32 j;
  s32 attackDelta;
  s32 redraw;
  struct BattleUnit *unit;
  struct BattleUnit *savedUnit;
  const struct OwnerStatusStatCursor *cursorStop;
  struct MenuSprite *statusSprite;
  s32 agilityDelta;
  s32 helpValue;
  s32 groupPosition;
  struct MenuCell *navigation;
  s32 iconX;

  render = UI_WORK;
  redraw = 1;
  ownerIndex = -1;
  portraitSlot = VramCache_AllocEntry(0x200);
  statusCount = 0;
  hasDjinn = Party_SumDjinnCountsFar(-1);

  navigation = ((struct MenuCell *)gWindowWork[42]);
  navigation->busy = 1;
  if (navigation->work != NULL) {
    UiWork_Finalize(navigation->work, 1);
    navigation->work = NULL;
  }
  AudioCommand_PlayFar(SE_MENU_CONFIRM);

  for (i = 4; i >= 0; i--) {
    cursor.groupPositions[i] = 0;
  }
  cursor.helpEntry = 0;
  cursor.drawnHelpEntry = 0;
  cursor.group = STATUS_CURSOR_GENERAL;

  cursorSlot = VramCache_AllocEntry(0x80);
  for (i = 0; i <= 10; i++) {
    statusSlots[i] = VramCache_AllocEntry(0x80);
    statusKinds[i] = -1;
  }

  if (listCount != 0) {
    for (i = 0; i <= 5; i++) {
      s32 kind = list[i];

      if (kind == 0xFF) {
        break;
      }
      if (kind != 0xFE) {
        if (kind == owner) {
          ownerIndex = i;
          break;
        }
      }
    }
  }

  cursorPosition.x = 0;
  cursorPosition.y = 0;
  // The stat grid and its explanation have separate lifetimes and redraws.
  statusWindow = UiWindow_Create(0, 0, 30, 20, 6);
  helpWindow = UiWindow_Create(0, 14, 30, 6, 10);
  UiWindow_MarkVisibleTileAttributes();

  for (;;) {
    keys = gKeysRepeat;

    nameSprite.oam.word.attr01 = 0x80000400;
    nameSprite.oam.word.attr23 = 0;
    nameSprite.oam.f.tile = UiGlyph_LoadPortrait(owner, portraitSlot);
    nameSprite.oam.f.x = 8;
    nameSprite.oam.f.y = 24;
    nameSprite.oam.f.palette = 14;
    Runtime_PushSlotEntry((s32 *)&nameSprite, 240);

    switch (cursor.helpEntry) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    default:
      cursor.group = STATUS_CURSOR_GENERAL;
      if (hasDjinn != 0) {
        cursor.groupSize = 9;
      } else {
        cursor.groupSize = 7;
      }
      break;
    case 10:
    case 11:
    case 12:
    case 13:
      cursor.group = STATUS_CURSOR_COMBAT_STATS;
      cursor.groupSize = 4;
      break;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
      cursor.group = STATUS_CURSOR_CONDITIONS;
      cursor.groupSize = statusCount;
      break;
    }

    Ui_SetRectHighlight(statusWindow->x, statusWindow->y, statusWindow->width,
                        statusWindow->height, 15);

    if (statusCount != 0) {
      if ((u32)cursor.group <= STATUS_CURSOR_COMBAT_STATS) {
        groupPosition = cursor.groupPositions[cursor.group];
        if ((keys & KEY_DOWN) != 0) {
          keys = 0;
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
          groupPosition++;
          if (groupPosition >= cursor.groupSize) {
            groupPosition = 0;
            if (cursor.group == STATUS_CURSOR_COMBAT_STATS) {
              cursor.group = STATUS_CURSOR_CONDITIONS;
              groupPosition = cursor.groupPositions[cursor.group];
            }
          }
        } else if ((keys & KEY_UP) != 0) {
          keys = 0;
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
          groupPosition--;
          if (groupPosition < 0) {
            groupPosition = cursor.groupSize - 1;
            if (cursor.group == STATUS_CURSOR_COMBAT_STATS) {
              cursor.group = STATUS_CURSOR_CONDITIONS;
              groupPosition = cursor.groupPositions[cursor.group];
            }
          }
        } else if ((keys & 0x31) != 0) {
          keys = 0;
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
          // Toggle toward conditions; group3 is transient until reclassified.
          cursor.group ^= STATUS_CURSOR_CONDITIONS;
          if ((gKeysRepeat & KEY_A) != 0) {
            groupPosition = cursor.groupPositions[cursor.group];
          } else if ((gKeysRepeat & KEY_LEFT) != 0) {
            groupPosition = statusCount - 1;
          } else {
            groupPosition = 0;
          }
        }
        cursor.groupPositions[cursor.group] = (s8)groupPosition;
      } else if (cursor.group == STATUS_CURSOR_CONDITIONS) {
        groupPosition = cursor.groupPositions[cursor.group];
        if (groupPosition >= cursor.groupSize) {
          groupPosition = cursor.groupSize - 1;
        }
        if (groupPosition < 0) {
          cursor.group = STATUS_CURSOR_GENERAL;
          groupPosition = cursor.groupPositions[0];
        } else if ((keys & KEY_RIGHT) != 0) {
          keys = 0;
          groupPosition++;
          if (groupPosition >= cursor.groupSize) {
            cursor.group = STATUS_CURSOR_GENERAL;
            groupPosition = cursor.groupPositions[0];
          }
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        } else if ((keys & KEY_LEFT) != 0) {
          keys = 0;
          groupPosition--;
          if (groupPosition < 0) {
            cursor.group = STATUS_CURSOR_GENERAL;
            groupPosition = cursor.groupPositions[0];
          }
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        } else if ((keys & 0xC1) != 0) {
          keys = 0;
          cursor.group = STATUS_CURSOR_GENERAL;
          groupPosition = cursor.groupPositions[0];
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        }
        cursor.groupPositions[cursor.group] = (s8)groupPosition;
      }
    }

    if (cursor.group == STATUS_CURSOR_GENERAL) {
      s32 k = cursor.groupPositions[cursor.group];

      if (hasDjinn == 0) {
        k += 9;
      }
      cursorStop = &gOwnerStatus_StatCursors[k];
      cursor.helpEntry = cursorStop->entry;
      cursor.column = cursorStop->column;
      cursor.row = cursorStop->row;
      Ui_SetRectHighlight(statusWindow->x + cursorStop->highlightColumn + 1,
                          statusWindow->y + cursorStop->highlightRow + 1,
                          cursorStop->highlightWidth, 1, 14);
    } else if (cursor.group == STATUS_CURSOR_COMBAT_STATS) {
      cursor.helpEntry =
          gOwnerStatus_EquipmentCursors[cursor.groupPositions[cursor.group]]
              .entry;
      cursor.column =
          gOwnerStatus_EquipmentCursors[cursor.groupPositions[cursor.group]]
              .column;
      cursor.row =
          gOwnerStatus_EquipmentCursors[cursor.groupPositions[cursor.group]]
              .row;
    } else if (cursor.group == STATUS_CURSOR_CONDITIONS) {
      cursor.helpEntry =
          gOwnerStatus_ConditionCursors[cursor.groupPositions[cursor.group]]
              .entry;
      cursor.column =
          gOwnerStatus_ConditionCursors[cursor.groupPositions[cursor.group]]
              .column;
      cursor.row =
          gOwnerStatus_ConditionCursors[cursor.groupPositions[cursor.group]]
              .row;
    }

    if (cursor.drawnHelpEntry != cursor.helpEntry) {
      cursor.drawnHelpEntry = cursor.helpEntry;
      redraw = 2;
    }
    cursorPosition.x = cursor.column * 8;
    cursorPosition.y = cursor.row * 8;

    {
      struct MenuSprite *face = GetBattleObjectSlotFar(owner)->object->records;

      portraitSprite.oam.word.attr01 = 0xC0002400;
      portraitSprite.oam.word.attr23 = 0;
#if TBS_PATCH_4
      /* FAKEMATCH(barrier): an empty asm, so the move into sl comes between
       * the two stores and the face tile loads before 0xFC00, as native;
       * without it, 13 of 1458 instructions differ in tbs-es. */
      asm("");
#endif
      portraitSprite.oam.f.tile = face->oam.f.tile;
      portraitSprite.oam.f.x = 0xAC;
      portraitSprite.oam.f.y = 56;
      Runtime_PushSlotEntry((s32 *)&portraitSprite, 240);
    }

    cursorSprite.oam.word.attr01 = 0x40000400;
    cursorSprite.oam.word.attr23 = 0;
    /* FAKEMATCH(barrier): counted as 4 bytes, the empty asm puts gFrameCount's
     * literal in the next pool, as native; without it, 13 of 1392
     * instructions differ in tbs-en. */
    asm("");
    cursorSprite.oam.f.tile =
        VramCache_Reload(cursorSlot, (s32)gResource_FixedBlockBTiles);
    cursorSprite.oam.f.x =
        (cursorPosition.x + statusWindow->x * 8 - ((gFrameCount & 4) >> 2)) +
        16;
    cursorSprite.oam.f.y = (u8)((cursorPosition.y + statusWindow->y * 8 -
                                 ((gFrameCount & 4) >> 2)) +
                                16);
    cursorSprite.oam.f.affineIndex = 8;
    Runtime_PushSlotEntry((s32 *)&cursorSprite, 241);

    if (redraw != 0) {
      unit = UnitGetFar(owner);
#if TBS_PATCH_1
      UiWindow_MarkVisibleTileAttributes();
#endif
      Ui_FillVramBlockPattern();

      if ((redraw & 1) != 0) {
        RenderOutput_RedrawSavedRect(statusWindow);
        UiWindow_MapTextCanvasTiles(helpWindow->x, helpWindow->y,
                                    helpWindow->width, helpWindow->height, 0);
        UiWindow_DrawDividerLine(statusWindow, 0, 14, 29, 14);

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
        switch (System_GetLanguage()) {
        case 2:
          UiText_DrawStringAtOffset((u8 *)unit->name, statusWindow, 0, 0);
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_NivString, statusWindow,
                                    51, 0);
          UiText_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
          break;
        case 3:
          UiText_DrawStringAtOffset((u8 *)unit->name, statusWindow, 0, 0);
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_NvString, statusWindow,
                                    56, 0);
          UiText_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
          break;
        default:
          UiText_DrawStringAtOffset((u8 *)unit->name, statusWindow, 0, 0);
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_LevelString,
                                    statusWindow, 56, 0);
          UiText_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
          break;
        }
#else
        UiText_DrawStringAtOffset((u8 *)unit->name, statusWindow, 0, 0);
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_LevelString, statusWindow,
                                  OWNER_STATUS_LEVEL_X, 0);
        UiText_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
#endif
        UiText_DrawCharacterAtOffset(MsgOwnerStatusExp, statusWindow, 0, 8);
        UiText_DrawNumberInWindow(unit->experience, 8, statusWindow, 40, 8);

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
        switch (System_GetLanguage()) {
        case 1:
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_KpString, statusWindow,
                                    40, 24);
          break;
        case 2:
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PvString, statusWindow,
                                    40, 24);
          break;
        case 3:
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PvString, statusWindow,
                                    40, 24);
          break;
        case 4:
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_HpString, statusWindow,
                                    40, 24);
          break;
        }
#else
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_HpString, statusWindow, 40,
                                  24);
#endif
        UiText_DrawNumberInWindow(unit->hp, 4, statusWindow, 56, 24);
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString, statusWindow,
                                  88, 24);
        UiText_DrawNumberInWindow(unit->maxHp, 4, statusWindow, 96, 24);
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PpString, statusWindow, 40,
                                  32);
        UiText_DrawNumberInWindow(unit->pp, 4, statusWindow, 56, 32);
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString, statusWindow,
                                  88, 32);
        UiText_DrawNumberInWindow(unit->maxPp, 4, statusWindow, 96, 32);

        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(0), statusWindow, 136,
                                     16);
        UiText_DrawNumberInWindow(unit->atk, 3, statusWindow,
                                  OWNER_STATUS_VALUE_X, 16);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(1), statusWindow, 136,
                                     24);
        UiText_DrawNumberInWindow(unit->def, 3, statusWindow,
                                  OWNER_STATUS_VALUE_X, 24);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(2), statusWindow, 136,
                                     32);
        UiText_DrawNumberInWindow(unit->agi, 3, statusWindow,
                                  OWNER_STATUS_VALUE_X, 32);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(3), statusWindow, 136,
                                     40);
        UiText_DrawNumberInWindow(unit->luck, 3, statusWindow,
                                  OWNER_STATUS_VALUE_X, 40);

        UiText_DrawCharacterAtOffset(MsgClassName + unit->classIndex,
                                     statusWindow, 0, 48);
        if (hasDjinn != 0) {
          UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(9), statusWindow, 0,
                                       72);
        }
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(5), statusWindow, 0,
                                     80);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(6), statusWindow, 0,
                                     88);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(7), statusWindow, 0,
                                     96);

        for (j = 0; j <= 3; j++) {
          s32 pal;

          pal = 1;
          if (hasDjinn != 0) {
            pal = 0;
          }
          UiWindow_SetTilemapEntry(statusWindow, 0x5001 + j, 7 + j * 4, pal + 8,
                                   0);
          if (hasDjinn != 0) {
            UiText_DrawNumberInWindow(unit->djinnActiveCounts[j], 1,
                                      statusWindow, 40 + j * 32, 72);
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString,
                                      statusWindow, 48 + j * 32, 72);
            UiText_DrawNumberInWindow(unit->djinnOwnedCounts[j], 1,
                                      statusWindow, 48 + j * 32 + 8, 72);
          }
          UiText_DrawNumberInWindow(Unit_GetElementLevelFar(owner, j), 2,
                                    statusWindow, 48 + j * 32, 80);
          UiText_DrawNumberInWindow(unit->element[j].power, 3, statusWindow,
                                    40 + j * 32, 88);
          UiText_DrawNumberInWindow(unit->element[j].resistance, 3,
                                    statusWindow, 40 + j * 32, 96);
        }

        i = 0;
        if (unit->hp == 0) {
          statusKinds[0] = 16;
          i = 1;
        }
        do {
          if (unit->restraint != 0) {
            statusKinds[i++] = 15;
          }
          if (i > 7) {
            break;
          }
          if (unit->grimReaper != 0) {
            statusKinds[i++] = 8;
          }
          if (i > 7) {
            break;
          }
          if (unit->poison == 1) {
            statusKinds[i++] = 1;
          }
          if (i > 7) {
            break;
          }
          if (unit->poison == 2) {
            statusKinds[i++] = 2;
          }
          if (i > 7) {
            break;
          }
          if (unit->psySeal != 0) {
            statusKinds[i++] = 4;
          }
          if (i > 7) {
            break;
          }
          if (unit->stun != 0) {
            statusKinds[i++] = 3;
          }
          if (i > 7) {
            break;
          }
          if (unit->sleep != 0) {
            statusKinds[i++] = 5;
          }
          if (i > 7) {
            break;
          }
          if (unit->haunt != 0) {
            statusKinds[i++] = 7;
          }
          if (i > 7) {
            break;
          }
          if (unit->delusion != 0) {
            statusKinds[i++] = 6;
          }
          if (i > 7) {
            break;
          }
          if (unit->attackTurns != 0) {
            if (unit->attackStage > 0) {
              statusKinds[i++] = 9;
            }
            if (unit->attackStage < 0) {
              statusKinds[i++] = 10;
            }
          }
          if (i > 7) {
            break;
          }
          if (unit->defenseTurns != 0) {
            if (unit->defenseStage > 0) {
              statusKinds[i++] = 11;
            }
            if (unit->defenseStage < 0) {
              statusKinds[i++] = 12;
            }
          }
          if (i > 7) {
            break;
          }
          if (unit->resistanceTurns != 0) {
            if (unit->resistanceStage > 0) {
              statusKinds[i++] = 13;
            }
            if (unit->resistanceStage < 0) {
              statusKinds[i++] = 14;
            }
          }
          if (i > 7) {
            break;
          }
          if (unit->agilityStage > 0) {
            statusKinds[i++] = 17;
          }
          if (unit->agilityStage < 0) {
            statusKinds[i++] = 18;
          }
        } while (0);

        for (j = 0; j < i; j++) {
          Resource_LoadTableEntryToBuffer(statusKinds[j], statusSlots[j]);
        }
        if (i == 0) {
          statusKinds[0] = 0;
          i = 1;
        }
        if (i <= 10) {
          for (j = i; j < 11; j++) {
            statusKinds[j] = -1;
          }
        }
        statusCount = i;
        if (statusKinds[0] == 0) {
          UiText_DrawCharacterAtOffset(unit->hp != 0 ? MsgOwnerStatusNormal
                                                     : MsgOwnerStatusDowned,
                                       statusWindow, 112, 0);
        }
      }

      helpText = (u16 *)MemAlloc(OWNER_STATUS_TEXT_MAX * sizeof(*helpText));
      if ((u32)cursor.helpEntry > 13) {
        statusKind = statusKinds[cursor.helpEntry - 14];
        if (statusKind == 0 && unit->hp == 0) {
          statusKind = 16;
        }

        // Remove stages on a snapshot to measure their displayed contribution.
        savedUnit = (struct BattleUnit *)MemAlloc(sizeof(*unit));
        helpValue = 0;
        Iwram_CopyWords(savedUnit, unit, sizeof(*unit));

        attackDelta = unit->atk;
        defenseDelta = unit->def;
        agilityDelta = unit->agi;
        unit->attackStage = 0;
        unit->defenseStage = 0;
        unit->agilityStage = 0;
        UnitCalcFar(owner);
        attackDelta -= unit->atk;
        defenseDelta -= unit->def;
        agilityDelta -= unit->agi;
        Iwram_CopyWords(unit, savedUnit, sizeof(*unit));
        MemFree(savedUnit);

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
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
          if (helpValue < 0) {
            helpValue = -helpValue;
          }
#endif
          break;
        default:
          break;
        }
        UiWork_PushValueSlot(helpValue, 5);
        UiText_CopyMessageString(statusKind + MsgOwnerStatusConditionHelp,
                                 helpText, OWNER_STATUS_TEXT_MAX);
      } else if (cursor.helpEntry == 2 && unit->level <= 98) {
        UiWork_PushValueSlot(
            Unit_GetExperienceForLevelFar(owner, unit->level + 1) -
                unit->experience,
            5);
        UiText_CopyMessageString(MsgOwnerStatusNextLevel, helpText,
                                 OWNER_STATUS_TEXT_MAX);
      } else {
        UiText_CopyMessageString(cursor.helpEntry + MsgOwnerStatusEntryHelp,
                                 helpText, OWNER_STATUS_TEXT_MAX);
      }

      UiText_RenderWideStringAtOffset(helpText, (struct UiWindow *)helpWindow,
                                      0, 4);
      MemFree(helpText);
      redraw = 0;
      render->dirty = 1;
    }

    statusSprite = statusSprites;
    statusSlot = statusSlots;
    {
      struct VramBlockCacheEntry *cache;

      for (i = 0, cache = gVramBlockCache, iconX = 112; i <= 10; i++) {
        statusSprite->oam.word.attr01 = 0x40000400;
        statusSprite->oam.word.attr23 = 0;
        statusSprite->oam.f.tile = cache[*statusSlot++].offset >> 5;
        statusSprite->oam.f.x = iconX;
        statusSprite->oam.f.y = (u8)(statusWindow->y * 8 + 8);
        if (statusKinds[i] > 0) {
          Runtime_PushSlotEntry((s32 *)statusSprite, 240);
        }
        iconX += 15;
        statusSprite++;
      }
    }

    if (gLinkCountdownWork->active == 0) {
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      break;
    }

    if (listCount != 0) {
      if ((keys & KEY_R) != 0) {
        ownerIndex++;
        if (ownerIndex >= listCount) {
          ownerIndex = 0;
        }
        owner = list[ownerIndex];
        redraw = 1;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      } else if ((keys & KEY_L) != 0) {
        ownerIndex--;
        if (ownerIndex < 0) {
          ownerIndex = listCount - 1;
        }
        owner = list[ownerIndex];
        redraw = 1;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      }
    }
    WaitFrames(1);
  }

  {
    s32 *statusSlotCursor = statusSlots;

    for (i = 10; i >= 0; i--) {
      Resource_ResetEntry(*statusSlotCursor++);
    }
  }
  Resource_ResetEntry(portraitSlot);
  Resource_ResetEntry(cursorSlot);
  WaitFrames(1);
#if TBS_PATCH_1
  UiWindow_MarkVisibleTileAttributes();
#endif
  UiWork_Finalize(statusWindow, 1);
  UiWork_Finalize(helpWindow, 1);
  PartyStatusDraw(gBattleWork->partyStatus);
  // UB: the indexed scalar pointer steps outside gBattleWork to slot 57.
  // The named slot changes native instructions at the same 0xcf8-byte EN
  // extent; using gWorkSlot instead adds eight bytes.
#ifdef UBFIX
  gLinkCountdownWork->busy = 0;
#else
  ((struct MenuCell **)&gBattleWork)[HEAP_SLOT_WINDOW + 42 - HEAP_SLOT_BATTLE]
      ->busy = 0;
#endif
  WaitFrames(1);
  return 0;
}
