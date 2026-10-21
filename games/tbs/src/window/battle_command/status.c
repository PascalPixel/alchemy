/// @file
/// The status screen.
// Part of battle_command.c.

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

// The status screen's cursor groups, each with its table of cursor stops.
/// The name, experience, HP, PP, class, Djinn and element rows
/// (gOwnerStatus_StatCursors).
#define STATUS_CURSOR_GENERAL 0
/// Attack, Defense, Agility and Luck (gOwnerStatus_EquipmentCursors).
#define STATUS_CURSOR_COMBAT_STATS 1
#define STATUS_CURSOR_CONDITIONS 2 ///< the condition icons

/// The status screen's cursor: its place in each group, the help entry it
/// shows and where it points.
struct StatusCursor {
  /// The cursor's place in each group. [3] is written when Left, Right or A
  /// in group 1 makes it group 3 for a frame; [4] is cleared at the start
  /// and never read.
  s8 groupPositions[5];
  /// The help shown: MsgOwnerStatusEntryHelp + entry for 0 to 13, the help
  /// of condition icon entry - 14 from 14 on. It decides the group.
  s32 helpEntry;
  s32 drawnHelpEntry; ///< the help drawn last; a change redraws the help
  s32 group;          ///< STATUS_CURSOR_ group
  s32 groupSize;      ///< how many stops the group has
  s32 column;         ///< tiles
  s32 row;            ///< tiles
};

/// Runs the status screen for party member `owner` until B is pressed or
/// the link battle's time runs out: the member's level, experience, HP and
/// PP, class, stats, Djinn, elements and conditions, with a help line for
/// the item under the cursor. With `listCount` nonzero, L and R step through
/// the members of `list` (0xff ends it, 0xfe is an empty place). The link
/// timer's window is closed while it is open (BattleMenuWork.timerHidden),
/// and the party status window is redrawn on the way out. Returns 0.
/// Battle_CollectPartyCommands opens it from the Status command and from L
/// or R in a member's command row.
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
  struct BattleMenuWork *navigation;
  s32 iconX;

  render = UI_WORK;
  redraw = 1;
  ownerIndex = -1;
  portraitSlot = VramCache_AllocEntry(0x200);
  statusCount = 0;
  hasDjinn = Party_SumDjinnCountsFar(-1);

  navigation = ((struct BattleMenuWork *)gWindowWork[42]);
  navigation->timerHidden = 1;
  if (navigation->timerWindow != 0) {
    UiWork_Finalize((struct UiWindow *)navigation->timerWindow, 1);
    navigation->timerWindow = 0;
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

  // Find the member's place in the list, for L and R.
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

    // The member's 32x32 portrait at the top left.
    nameSprite.oam.word.attr01 = 0x80000400;
    nameSprite.oam.word.attr23 = 0;
    nameSprite.oam.f.tile = UiGlyph_LoadPortrait(owner, portraitSlot);
    nameSprite.oam.f.x = 8;
    nameSprite.oam.f.y = 24;
    nameSprite.oam.f.palette = 14;
    Runtime_PushSlotEntry((s32 *)&nameSprite, 240);

    // The group follows the help shown: entries 0 to 9 are the general rows
    // (9 stops with Djinn, 7 without), 10 to 13 the stats, 14 to 21 the
    // conditions.
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
          // A, Right or Left (0x31) toggles between the general group and
          // the conditions. From group 1 this makes group 3, which no branch
          // below reads; the next frame's switch makes it group 1 again.
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
          // A, Up or Down (0xc1) go back to the general group.
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

      // Without Djinn in the party the stops come from the table's second
      // part, which leaves out the class and Djinn stops (help entries 5 and
      // 6): 7 stops instead of 9.
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

    // The member's battle figure: its battle object's first sprite record,
    // as a 64x64, 256-colour sprite at (172, 56).
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
    // 8 sets attribute 1's bit 12, a sprite's horizontal flip when it is not
    // affine: the hand is mirrored.
    cursorSprite.oam.f.affineIndex = 8;
    Runtime_PushSlotEntry((s32 *)&cursorSprite, 241);

    // redraw: bit 0 draws the whole screen (on opening and on L or R), bit 1
    // only the help line.
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

        // The condition icons, at most eight, in this order: downed (16),
        // cursed item (15), Grim Reaper (8), poison (1), deadly poison (2),
        // Psynergy sealed (4), stun (3), sleep (5), haunt (7), delusion (6),
        // then Attack, Defense and Resistance raised or lowered (9 to 14)
        // and Agility (17, 18). The kind picks both the icon and its help
        // (MsgOwnerStatusConditionHelp + kind); 0 alone means normal.
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

        // A condition's help carries a number: the Grim Reaper's turns left,
        // what the stat stages add (the stats less those recalculated with
        // the stages cleared, after which the unit is restored from a copy),
        // or 20 for each Resistance stage.
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
#if TBS_PATCH_4
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
        // Experience below level 99: the points still needed for the next.
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

    // The condition icons, 15 pixels apart from x 112 on the first row;
    // normal (0) and unused places (-1) are not drawn.
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

    if (gLinkCountdownWork->timer == 0) {
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
  gLinkCountdownWork->timerHidden = 0;
#else
  ((struct BattleMenuWork **)&gBattleWork)[HEAP_SLOT_WINDOW + 42 -
                                           HEAP_SLOT_BATTLE]
      ->timerHidden = 0;
#endif
  WaitFrames(1);
  return 0;
}
