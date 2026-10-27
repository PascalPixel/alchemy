/// @file
/// The Status screen.
// Part of screens.c.

#include "../screens_internal.h"

#if LANGUAGE != LANGUAGE_JAPANESE
#define OWNER_STATUS_TEXT_MAX 0x80
#define OWNER_STATUS_VALUE_X 184
#else
#define OWNER_STATUS_TEXT_MAX 0x40
#define OWNER_STATUS_VALUE_X 176
#endif

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define OWNER_STATUS_LABEL(row) (MsgOwnerStatusAttack + (row))
#else
#define OWNER_STATUS_LABEL(row)                                                \
  (MsgOwnerStatusExp - OWNER_STATUS_LABEL_EXP + (row))
#endif

/// The Status screen's labels, in their message order from
/// MsgOwnerStatusAttack (OWNER_STATUS_LABEL).
enum BattleSummaryLabel {
  OWNER_STATUS_LABEL_ATTACK = 0,
  OWNER_STATUS_LABEL_DEFENSE = 1,
  OWNER_STATUS_LABEL_AGILITY = 2,
  OWNER_STATUS_LABEL_LUCK = 3,
  OWNER_STATUS_LABEL_ATTRIBUTES = 4,    ///< "Attributes", which nothing draws
  OWNER_STATUS_LABEL_ELEMENT_LEVEL = 5, ///< "Lv"
  OWNER_STATUS_LABEL_POWER = 6,
  OWNER_STATUS_LABEL_RESIST = 7,
  OWNER_STATUS_LABEL_CLASS = 8, ///< "Class", which nothing draws
  OWNER_STATUS_LABEL_DJINN = 9,
  OWNER_STATUS_LABEL_EXP = 10 ///< MsgOwnerStatusExp
};

/// The Status screen's help lines, from MsgOwnerStatusEntryHelp, which the
/// cursor stops in windata.s name; the status icons' places follow.
enum BattleSummaryHelp {
  /// The name, whose help says how to switch members with L and R.
  OWNER_STATUS_HELP_NAME = 0,
  OWNER_STATUS_HELP_LEVEL = 1, ///< no cursor stop shows it
  OWNER_STATUS_HELP_EXPERIENCE = 2,
  OWNER_STATUS_HELP_HP = 3,
  OWNER_STATUS_HELP_PP = 4,
  OWNER_STATUS_HELP_CLASS = 5,
  OWNER_STATUS_HELP_DJINN = 6,
  OWNER_STATUS_HELP_ELEMENT_LEVEL = 7,
  OWNER_STATUS_HELP_POWER = 8,
  OWNER_STATUS_HELP_RESIST = 9,
  OWNER_STATUS_HELP_ATTACK = 10,
  OWNER_STATUS_HELP_DEFENSE = 11,
  OWNER_STATUS_HELP_AGILITY = 12,
  OWNER_STATUS_HELP_LUCK = 13,
  /// The first status icon's place: its help is its status's
  /// (MsgOwnerStatusConditionHelp).
  OWNER_STATUS_HELP_CONDITION = 14,
  OWNER_STATUS_HELP_LAST_CONDITION = 21 ///< the eighth icon's place
};

/// A status the Status screen shows as an icon, which picks both the icon
/// and its help (MsgOwnerStatusConditionHelp + condition).
enum BattleSummaryStatus {
  OWNER_CONDITION_EMPTY = -1, ///< a place with no icon
  OWNER_CONDITION_NORMAL = 0, ///< no status: "Normal status."
  OWNER_CONDITION_POISON = 1,
  OWNER_CONDITION_DEADLY_POISON = 2,
  OWNER_CONDITION_STUN = 3,
  OWNER_CONDITION_PSYNERGY_SEALED = 4,
  OWNER_CONDITION_SLEEP = 5,
  OWNER_CONDITION_DELUSION = 6,
  OWNER_CONDITION_HAUNT = 7,
  OWNER_CONDITION_GRIM_REAPER = 8, ///< downed when its turns run out
  OWNER_CONDITION_ATTACK_UP = 9,
  OWNER_CONDITION_ATTACK_DOWN = 10,
  OWNER_CONDITION_DEFENSE_UP = 11,
  OWNER_CONDITION_DEFENSE_DOWN = 12,
  OWNER_CONDITION_RESISTANCE_UP = 13,
  OWNER_CONDITION_RESISTANCE_DOWN = 14,
  OWNER_CONDITION_CURSED_ITEM = 15,
  OWNER_CONDITION_DOWNED = 16,
  OWNER_CONDITION_AGILITY_UP = 17,
  OWNER_CONDITION_AGILITY_DOWN = 18
};

/// What the Status screen redraws on its next frame.
#define OWNER_STATUS_REDRAW_ALL 1  ///< on opening, and on L or R
#define OWNER_STATUS_REDRAW_HELP 2 ///< only the help line

// The Status screen's cursor groups, each with its table of cursor stops.
/// The name, experience, HP, PP, class, Djinn and element rows
/// (gBattleStatusScreen_StatCursors).
#define STATUS_CURSOR_GENERAL 0
/// Attack, Defense, Agility and Luck (gBattleStatusScreen_EquipmentCursors).
#define STATUS_CURSOR_COMBAT_STATS 1
#define STATUS_CURSOR_CONDITIONS 2 ///< the status icons

/// The Status screen's cursor: its place in each group, the help entry it
/// shows and where it points.
struct StatusCursor {
  /// The cursor's place in each group. [3] is written for the one frame
  /// Left, Right or A in STATUS_CURSOR_COMBAT_STATS leaves the group at a
  /// value no branch reads; [4] is cleared at the start and never read.
  s8 groupPositions[5];
  /// The help shown (enum BattleSummaryHelp). It decides the group.
  s32 helpEntry;
  s32 drawnHelpEntry; ///< the help drawn last; a change redraws the help
  s32 group;          ///< STATUS_CURSOR_ group
  s32 groupSize;      ///< how many stops the group has
  s32 column;         ///< tiles
  s32 row;            ///< tiles
};

/// Runs the Status screen for party member `member` until B is pressed or
/// the link battle's time runs out: the member's level, experience, HP and
/// PP, class, stats, Djinn, elements and statuses, with a help line for
/// the item under the cursor. With `listCount` nonzero, L and R step through
/// the members of `list` (BATTLE_UNIT_LIST_END ends it, BATTLE_UNIT_REMOVED is
/// an empty place). The link timer's window is closed while it is open
/// (BattleMenuWork.timerHidden), and the party status window is redrawn on the
/// way out. Returns 0. Battle_CollectPartyCommands opens it from the Status
/// command and from L or R in a member's command row.
s32 BattleStatusScreen_Run(u16 *list, s32 listCount, s32 member) {
  struct MenuSprite portraitSprite;
  struct MenuSprite cursorSprite;
  s32 statusSlots[11];
  s8 statusKinds[11];
  struct MenuSprite statusSprites[11];
  struct MenuSprite nameSprite;
  struct StatusCursor cursor;
  struct WindowWork *work;
  struct Window *statusWindow;
  struct Window *helpWindow;
  s32 memberIndex;
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
  struct Unit *unit;
  struct Unit *savedUnit;
  const struct BattleSummaryStatCursor *cursorStop;
  struct MenuSprite *statusSprite;
  s32 agilityDelta;
  s32 helpValue;
  s32 groupPosition;
  struct BattleMenuWork *navigation;
  s32 iconX;

  work = WINDOW_WORK;
  redraw = OWNER_STATUS_REDRAW_ALL;
  memberIndex = -1;
  portraitSlot = VramCache_AllocEntry(0x200);
  statusCount = 0;
  hasDjinn = Party_SumDjinnCountsFar(-1);

  navigation = ((struct BattleMenuWork *)
                    gWindowWork[HEAP_SLOT_BATTLE_MENU - HEAP_SLOT_WINDOW]);
  navigation->timerHidden = 1;
  if (navigation->timerWindow != 0) {
    Window_Close((struct Window *)navigation->timerWindow, 1);
    navigation->timerWindow = 0;
  }
  Sound_PlaySongFar(SE_MENU_CONFIRM);

  for (i = 4; i >= 0; i--) {
    cursor.groupPositions[i] = 0;
  }
  cursor.helpEntry = 0;
  cursor.drawnHelpEntry = 0;
  cursor.group = STATUS_CURSOR_GENERAL;

  cursorSlot = VramCache_AllocEntry(0x80);
  for (i = 0; i <= 10; i++) {
    statusSlots[i] = VramCache_AllocEntry(0x80);
    statusKinds[i] = OWNER_CONDITION_EMPTY;
  }

  // Find the member's place in the list, for L and R.
  if (listCount != 0) {
    for (i = 0; i <= 5; i++) {
      s32 kind = list[i];

      if (kind == BATTLE_UNIT_LIST_END) {
        break;
      }
      if (kind != BATTLE_UNIT_REMOVED) {
        if (kind == member) {
          memberIndex = i;
          break;
        }
      }
    }
  }

  cursorPosition.x = 0;
  cursorPosition.y = 0;
  // The stat grid and its explanation have separate lifetimes and redraws.
  statusWindow = Window_Create(0, 0, 30, 20, 6);
  helpWindow = Window_Create(0, 14, 30, 6,
                             WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS);
  Window_MarkVisibleTextTiles();

  for (;;) {
    keys = gKeysRepeat;

    // The member's 32x32 portrait at the top left.
    nameSprite.oam.word.attr01 = 0x80000400;
    nameSprite.oam.word.attr23 = 0;
    nameSprite.oam.f.tile = Portrait_Load(member, portraitSlot);
    nameSprite.oam.f.x = 8;
    nameSprite.oam.f.y = 24;
    nameSprite.oam.f.palette = 14;
    Oam_AddSpriteAtDepth((s32 *)&nameSprite, 240);

    // The group follows the help shown: the general rows have 9 stops with
    // Djinn, 7 without.
    switch (cursor.helpEntry) {
    case OWNER_STATUS_HELP_NAME:
    case OWNER_STATUS_HELP_EXPERIENCE ... OWNER_STATUS_HELP_RESIST:
    default:
      cursor.group = STATUS_CURSOR_GENERAL;
      if (hasDjinn != 0) {
        cursor.groupSize = 9;
      } else {
        cursor.groupSize = 7;
      }
      break;
    case OWNER_STATUS_HELP_ATTACK ... OWNER_STATUS_HELP_LUCK:
      cursor.group = STATUS_CURSOR_COMBAT_STATS;
      cursor.groupSize = 4;
      break;
    case OWNER_STATUS_HELP_CONDITION ... OWNER_STATUS_HELP_LAST_CONDITION:
      cursor.group = STATUS_CURSOR_CONDITIONS;
      cursor.groupSize = statusCount;
      break;
    }

    Window_HighlightRect(statusWindow->x, statusWindow->y, statusWindow->width,
                         statusWindow->height, WINDOW_PALETTE);

    if (statusCount != 0) {
      if ((u32)cursor.group <= STATUS_CURSOR_COMBAT_STATS) {
        groupPosition = cursor.groupPositions[cursor.group];
        if ((keys & KEY_DOWN) != 0) {
          keys = 0;
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
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
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
          groupPosition--;
          if (groupPosition < 0) {
            groupPosition = cursor.groupSize - 1;
            if (cursor.group == STATUS_CURSOR_COMBAT_STATS) {
              cursor.group = STATUS_CURSOR_CONDITIONS;
              groupPosition = cursor.groupPositions[cursor.group];
            }
          }
        } else if ((keys & (KEY_A | KEY_RIGHT | KEY_LEFT)) != 0) {
          keys = 0;
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
          // A, Right or Left toggles between the general group and the
          // statuses. From STATUS_CURSOR_COMBAT_STATS this makes a group
          // no branch below reads; the next frame's switch restores it.
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
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        } else if ((keys & KEY_LEFT) != 0) {
          keys = 0;
          groupPosition--;
          if (groupPosition < 0) {
            cursor.group = STATUS_CURSOR_GENERAL;
            groupPosition = cursor.groupPositions[0];
          }
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        } else if ((keys & (KEY_A | KEY_UP | KEY_DOWN)) != 0) {
          // A, Up or Down go back to the general group.
          keys = 0;
          cursor.group = STATUS_CURSOR_GENERAL;
          groupPosition = cursor.groupPositions[0];
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        }
        cursor.groupPositions[cursor.group] = (s8)groupPosition;
      }
    }

    if (cursor.group == STATUS_CURSOR_GENERAL) {
      s32 k = cursor.groupPositions[cursor.group];

      // Without Djinn in the party the stops come from the table's second
      // part, which leaves out the class and Djinn stops: 7 stops instead of 9.
      if (hasDjinn == 0) {
        k += 9;
      }
      cursorStop = &gBattleStatusScreen_StatCursors[k];
      cursor.helpEntry = cursorStop->entry;
      cursor.column = cursorStop->column;
      cursor.row = cursorStop->row;
      Window_HighlightRect(statusWindow->x + cursorStop->highlightColumn + 1,
                           statusWindow->y + cursorStop->highlightRow + 1,
                           cursorStop->highlightWidth, 1, HIGHLIGHT_PALETTE);
    } else if (cursor.group == STATUS_CURSOR_COMBAT_STATS) {
      cursor.helpEntry = gBattleStatusScreen_EquipmentCursors
                             [cursor.groupPositions[cursor.group]]
                                 .entry;
      cursor.column = gBattleStatusScreen_EquipmentCursors
                          [cursor.groupPositions[cursor.group]]
                              .column;
      cursor.row = gBattleStatusScreen_EquipmentCursors
                       [cursor.groupPositions[cursor.group]]
                           .row;
    } else if (cursor.group == STATUS_CURSOR_CONDITIONS) {
      cursor.helpEntry = gBattleStatusScreen_ConditionCursors
                             [cursor.groupPositions[cursor.group]]
                                 .entry;
      cursor.column = gBattleStatusScreen_ConditionCursors
                          [cursor.groupPositions[cursor.group]]
                              .column;
      cursor.row = gBattleStatusScreen_ConditionCursors
                       [cursor.groupPositions[cursor.group]]
                           .row;
    }

    if (cursor.drawnHelpEntry != cursor.helpEntry) {
      cursor.drawnHelpEntry = cursor.helpEntry;
      redraw = OWNER_STATUS_REDRAW_HELP;
    }
    cursorPosition.x = cursor.column * 8;
    cursorPosition.y = cursor.row * 8;

    // The member's battle figure: its battle object's first sprite record,
    // as a 64x64, 256-colour sprite at (172, 56).
    {
      struct MenuSprite *face =
          (struct MenuSprite *)GetBattleObjectSlotFar(member)->actor->sprite;

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
      Oam_AddSpriteAtDepth((s32 *)&portraitSprite, 240);
    }

    cursorSprite.oam.word.attr01 = 0x40000400;
    cursorSprite.oam.word.attr23 = 0;
    /* FAKEMATCH(barrier): counted as 4 bytes, the empty asm puts gFrameCount's
     * literal in the next pool, as native; without it, 13 of 1392
     * instructions differ in tbs-en. */
    asm("");
    cursorSprite.oam.f.tile =
        VramCache_Reload(cursorSlot, (s32)gPointingHandTiles);
    cursorSprite.oam.f.x =
        (cursorPosition.x + statusWindow->x * 8 - ((gFrameCount & 4) >> 2)) +
        16;
    cursorSprite.oam.f.y = (u8)((cursorPosition.y + statusWindow->y * 8 -
                                 ((gFrameCount & 4) >> 2)) +
                                16);
    // 8 sets attribute 1's bit 12, a sprite's horizontal flip when it is not
    // affine: the hand is mirrored.
    cursorSprite.oam.f.affineIndex = 8;
    Oam_AddSpriteAtDepth((s32 *)&cursorSprite, 241);

    if (redraw != 0) {
      unit = Unit_GetFar(member);
#if TBS_PATCH_1
      Window_MarkVisibleTextTiles();
#endif
      Text_FillCanvasBackground();

      if ((redraw & OWNER_STATUS_REDRAW_ALL) != 0) {
        WindowSprite_RedrawSavedRect(statusWindow);
        Window_MapTextCanvasTiles(helpWindow->x, helpWindow->y,
                                  helpWindow->width, helpWindow->height, 0);
        Window_DrawDividerLine(statusWindow, 0, 14, 29, 14);

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
        switch (System_GetLanguage()) {
        case CARTRIDGE_LANGUAGE_FRENCH:
          Text_DrawStringAtOffset((u8 *)unit->name, statusWindow, 0, 0);
          Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_NivString,
                                  statusWindow, 51, 0);
          Text_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
          break;
        case CARTRIDGE_LANGUAGE_SPANISH:
          Text_DrawStringAtOffset((u8 *)unit->name, statusWindow, 0, 0);
          Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_NvString,
                                  statusWindow, 56, 0);
          Text_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
          break;
        default:
          Text_DrawStringAtOffset((u8 *)unit->name, statusWindow, 0, 0);
          Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_LevelString,
                                  statusWindow, 56, 0);
          Text_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
          break;
        }
#else
        Text_DrawStringAtOffset((u8 *)unit->name, statusWindow, 0, 0);
        Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_LevelString,
                                statusWindow, OWNER_STATUS_LEVEL_X, 0);
        Text_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
#endif
        Text_DrawMessageAsTiles(MsgOwnerStatusExp, statusWindow, 0, 8);
        Text_DrawNumberInWindow(unit->experience, 8, statusWindow, 40, 8);

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
        switch (System_GetLanguage()) {
        case CARTRIDGE_LANGUAGE_GERMAN:
          Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_KpString,
                                  statusWindow, 40, 24);
          break;
        case CARTRIDGE_LANGUAGE_FRENCH:
          Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_PvString,
                                  statusWindow, 40, 24);
          break;
        case CARTRIDGE_LANGUAGE_SPANISH:
          Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_PvString,
                                  statusWindow, 40, 24);
          break;
        case CARTRIDGE_LANGUAGE_ITALIAN:
          Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_HpString,
                                  statusWindow, 40, 24);
          break;
        }
#else
        Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_HpString,
                                statusWindow, 40, 24);
#endif
        Text_DrawNumberInWindow(unit->hp, 4, statusWindow, 56, 24);
        Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_SlashString,
                                statusWindow, 88, 24);
        Text_DrawNumberInWindow(unit->hpMax, 4, statusWindow, 96, 24);
        Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_PpString,
                                statusWindow, 40, 32);
        Text_DrawNumberInWindow(unit->pp, 4, statusWindow, 56, 32);
        Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_SlashString,
                                statusWindow, 88, 32);
        Text_DrawNumberInWindow(unit->ppMax, 4, statusWindow, 96, 32);

        Text_DrawMessageAsTiles(OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_ATTACK),
                                statusWindow, 136, 16);
        Text_DrawNumberInWindow(unit->atk, 3, statusWindow,
                                OWNER_STATUS_VALUE_X, 16);
        Text_DrawMessageAsTiles(OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_DEFENSE),
                                statusWindow, 136, 24);
        Text_DrawNumberInWindow(unit->def, 3, statusWindow,
                                OWNER_STATUS_VALUE_X, 24);
        Text_DrawMessageAsTiles(OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_AGILITY),
                                statusWindow, 136, 32);
        Text_DrawNumberInWindow(unit->agi, 3, statusWindow,
                                OWNER_STATUS_VALUE_X, 32);
        Text_DrawMessageAsTiles(OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_LUCK),
                                statusWindow, 136, 40);
        Text_DrawNumberInWindow(unit->luck, 3, statusWindow,
                                OWNER_STATUS_VALUE_X, 40);

        Text_DrawMessageAsTiles(MsgClassName + unit->classId, statusWindow, 0,
                                48);
        if (hasDjinn != 0) {
          Text_DrawMessageAsTiles(OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_DJINN),
                                  statusWindow, 0, 72);
        }
        Text_DrawMessageAsTiles(
            OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_ELEMENT_LEVEL), statusWindow,
            0, 80);
        Text_DrawMessageAsTiles(OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_POWER),
                                statusWindow, 0, 88);
        Text_DrawMessageAsTiles(OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_RESIST),
                                statusWindow, 0, 96);

        for (j = 0; j <= 3; j++) {
          s32 pal;

          pal = 1;
          if (hasDjinn != 0) {
            pal = 0;
          }
          Window_SetTilemapEntry(statusWindow, 0x5001 + j, 7 + j * 4, pal + 8,
                                 0);
          if (hasDjinn != 0) {
            Text_DrawNumberInWindow(unit->djinnSetCounts[j], 1, statusWindow,
                                    40 + j * 32, 72);
            Text_DrawStringAtOffset((u8 *)gBattleStatusScreen_SlashString,
                                    statusWindow, 48 + j * 32, 72);
            Text_DrawNumberInWindow(unit->djinnHeldCounts[j], 1, statusWindow,
                                    48 + j * 32 + 8, 72);
          }
          Text_DrawNumberInWindow(Unit_GetElementLevelFar(member, j), 2,
                                  statusWindow, 48 + j * 32, 80);
          Text_DrawNumberInWindow(unit->element[j].power, 3, statusWindow,
                                  40 + j * 32, 88);
          Text_DrawNumberInWindow(unit->element[j].resistance, 3, statusWindow,
                                  40 + j * 32, 96);
        }

        // The status icons, at most eight, in this order: downed, cursed
        // item, Grim Reaper, poison, deadly poison, Psynergy sealed, stun,
        // sleep, haunt, delusion, then Attack, Defense and Resistance raised
        // or lowered, and Agility. OWNER_CONDITION_NORMAL alone means none.
        i = 0;
        if (unit->hp == 0) {
          statusKinds[0] = OWNER_CONDITION_DOWNED;
          i = 1;
        }
        do {
          if (unit->curse != 0) {
            statusKinds[i++] = OWNER_CONDITION_CURSED_ITEM;
          }
          if (i > 7) {
            break;
          }
          if (unit->grimReaper != 0) {
            statusKinds[i++] = OWNER_CONDITION_GRIM_REAPER;
          }
          if (i > 7) {
            break;
          }
          if (unit->poison == 1) {
            statusKinds[i++] = OWNER_CONDITION_POISON;
          }
          if (i > 7) {
            break;
          }
          if (unit->poison == 2) {
            statusKinds[i++] = OWNER_CONDITION_DEADLY_POISON;
          }
          if (i > 7) {
            break;
          }
          if (unit->psySeal != 0) {
            statusKinds[i++] = OWNER_CONDITION_PSYNERGY_SEALED;
          }
          if (i > 7) {
            break;
          }
          if (unit->stun != 0) {
            statusKinds[i++] = OWNER_CONDITION_STUN;
          }
          if (i > 7) {
            break;
          }
          if (unit->sleep != 0) {
            statusKinds[i++] = OWNER_CONDITION_SLEEP;
          }
          if (i > 7) {
            break;
          }
          if (unit->haunt != 0) {
            statusKinds[i++] = OWNER_CONDITION_HAUNT;
          }
          if (i > 7) {
            break;
          }
          if (unit->delusion != 0) {
            statusKinds[i++] = OWNER_CONDITION_DELUSION;
          }
          if (i > 7) {
            break;
          }
          if (unit->attackTurns != 0) {
            if (unit->attackStage > 0) {
              statusKinds[i++] = OWNER_CONDITION_ATTACK_UP;
            }
            if (unit->attackStage < 0) {
              statusKinds[i++] = OWNER_CONDITION_ATTACK_DOWN;
            }
          }
          if (i > 7) {
            break;
          }
          if (unit->defenseTurns != 0) {
            if (unit->defenseStage > 0) {
              statusKinds[i++] = OWNER_CONDITION_DEFENSE_UP;
            }
            if (unit->defenseStage < 0) {
              statusKinds[i++] = OWNER_CONDITION_DEFENSE_DOWN;
            }
          }
          if (i > 7) {
            break;
          }
          if (unit->resistanceTurns != 0) {
            if (unit->resistanceStage > 0) {
              statusKinds[i++] = OWNER_CONDITION_RESISTANCE_UP;
            }
            if (unit->resistanceStage < 0) {
              statusKinds[i++] = OWNER_CONDITION_RESISTANCE_DOWN;
            }
          }
          if (i > 7) {
            break;
          }
          if (unit->agilityStage > 0) {
            statusKinds[i++] = OWNER_CONDITION_AGILITY_UP;
          }
          if (unit->agilityStage < 0) {
            statusKinds[i++] = OWNER_CONDITION_AGILITY_DOWN;
          }
        } while (0);

        for (j = 0; j < i; j++) {
          Resource_LoadTableEntryToBuffer(statusKinds[j], statusSlots[j]);
        }
        if (i == 0) {
          statusKinds[0] = OWNER_CONDITION_NORMAL;
          i = 1;
        }
        if (i <= 10) {
          for (j = i; j < 11; j++) {
            statusKinds[j] = OWNER_CONDITION_EMPTY;
          }
        }
        statusCount = i;
        if (statusKinds[0] == OWNER_CONDITION_NORMAL) {
          Text_DrawMessageAsTiles(unit->hp != 0 ? MsgOwnerStatusNormal
                                                : MsgOwnerStatusDowned,
                                  statusWindow, 112, 0);
        }
      }

      helpText = (u16 *)Alloc(OWNER_STATUS_TEXT_MAX * sizeof(*helpText));
      if ((u32)cursor.helpEntry >= OWNER_STATUS_HELP_CONDITION) {
        statusKind =
            statusKinds[cursor.helpEntry - OWNER_STATUS_HELP_CONDITION];
        if (statusKind == OWNER_CONDITION_NORMAL && unit->hp == 0) {
          statusKind = OWNER_CONDITION_DOWNED;
        }

        // A status's help carries a number: the Grim Reaper's turns left,
        // what the stat stages add (the stats less those recalculated with
        // the stages cleared, after which the unit is restored from a copy),
        // or 20 for each Resistance stage.
        savedUnit = (struct Unit *)Alloc(sizeof(*unit));
        helpValue = 0;
        Iwram_CopyWords(savedUnit, unit, sizeof(*unit));

        attackDelta = unit->atk;
        defenseDelta = unit->def;
        agilityDelta = unit->agi;
        unit->attackStage = 0;
        unit->defenseStage = 0;
        unit->agilityStage = 0;
        Unit_CalculateFar(member);
        attackDelta -= unit->atk;
        defenseDelta -= unit->def;
        agilityDelta -= unit->agi;
        Iwram_CopyWords(unit, savedUnit, sizeof(*unit));
        Free(savedUnit);

        switch (statusKind) {
        case OWNER_CONDITION_GRIM_REAPER:
          helpValue = unit->grimReaper;
          break;
        case OWNER_CONDITION_ATTACK_UP:
          helpValue = attackDelta;
          break;
        case OWNER_CONDITION_ATTACK_DOWN:
          helpValue = -attackDelta;
          break;
        case OWNER_CONDITION_DEFENSE_UP:
          helpValue = defenseDelta;
          break;
        case OWNER_CONDITION_DEFENSE_DOWN:
          helpValue = -defenseDelta;
          break;
        case OWNER_CONDITION_AGILITY_UP:
          helpValue = agilityDelta;
          break;
        case OWNER_CONDITION_AGILITY_DOWN:
          helpValue = -agilityDelta;
          break;
        case OWNER_CONDITION_RESISTANCE_UP:
        case OWNER_CONDITION_RESISTANCE_DOWN:
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
        MessageArgument_Push(helpValue, MESSAGE_ARGUMENT_NUMBER);
        Message_CopyExpanded(statusKind + MsgOwnerStatusConditionHelp, helpText,
                             OWNER_STATUS_TEXT_MAX);
      } else if (cursor.helpEntry == OWNER_STATUS_HELP_EXPERIENCE &&
                 unit->level <= 98) {
        // Experience below level 99: the points still needed for the next.
        MessageArgument_Push(
            Unit_GetExperienceForLevelFar(member, unit->level + 1) -
                unit->experience,
            MESSAGE_ARGUMENT_NUMBER);
        Message_CopyExpanded(MsgOwnerStatusNextLevel, helpText,
                             OWNER_STATUS_TEXT_MAX);
      } else {
        Message_CopyExpanded(cursor.helpEntry + MsgOwnerStatusEntryHelp,
                             helpText, OWNER_STATUS_TEXT_MAX);
      }

      Text_DrawWideStringAtOffset(helpText, (struct Window *)helpWindow, 0, 4);
      Free(helpText);
      redraw = 0;
      work->dirty = 1;
    }

    // The status icons, 15 pixels apart from x 112 on the first row;
    // OWNER_CONDITION_NORMAL and empty places are not drawn.
    statusSprite = statusSprites;
    statusSlot = statusSlots;
    {
      struct VramCacheEntry *entry;

      for (i = 0, entry = gVramCacheEntries, iconX = 112; i <= 10; i++) {
        statusSprite->oam.word.attr01 = 0x40000400;
        statusSprite->oam.word.attr23 = 0;
        statusSprite->oam.f.tile = entry[*statusSlot++].offset >> 5;
        statusSprite->oam.f.x = iconX;
        statusSprite->oam.f.y = (u8)(statusWindow->y * 8 + 8);
        if (statusKinds[i] > OWNER_CONDITION_NORMAL) {
          Oam_AddSpriteAtDepth((s32 *)statusSprite, 240);
        }
        iconX += 15;
        statusSprite++;
      }
    }

    if (gBattleMenuWork->timer == 0) {
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      break;
    }

    if (listCount != 0) {
      if ((keys & KEY_R) != 0) {
        memberIndex++;
        if (memberIndex >= listCount) {
          memberIndex = 0;
        }
        member = list[memberIndex];
        redraw = OWNER_STATUS_REDRAW_ALL;
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      } else if ((keys & KEY_L) != 0) {
        memberIndex--;
        if (memberIndex < 0) {
          memberIndex = listCount - 1;
        }
        member = list[memberIndex];
        redraw = OWNER_STATUS_REDRAW_ALL;
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      }
    }
    WaitFrames(1);
  }

  {
    s32 *statusSlotCursor = statusSlots;

    for (i = 10; i >= 0; i--) {
      VramCache_FreeEntry(*statusSlotCursor++);
    }
  }
  VramCache_FreeEntry(portraitSlot);
  VramCache_FreeEntry(cursorSlot);
  WaitFrames(1);
#if TBS_PATCH_1
  Window_MarkVisibleTextTiles();
#endif
  Window_Close(statusWindow, 1);
  Window_Close(helpWindow, 1);
  PartyStatus_Draw(gBattleWork->partyStatus);
  // UB: the indexed scalar pointer steps outside gBattleWork to
  // HEAP_SLOT_BATTLE_MENU.
  // The named slot changes native instructions at the same 0xcf8-byte EN
  // extent; using gHeapSlots instead adds eight bytes.
#ifdef UBFIX
  gBattleMenuWork->timerHidden = 0;
#else
  ((struct BattleMenuWork **)&gBattleWork)[HEAP_SLOT_BATTLE_MENU -
                                           HEAP_SLOT_BATTLE]
      ->timerHidden = 0;
#endif
  WaitFrames(1);
  return 0;
}
