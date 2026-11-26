/// @file
/// The battle's Status screen, which the Status command opens.
// Part of screens.c.

#include "screens_internal.h"

/// The Status screen's help text buffer, in characters, and the x of the
/// Attack, Defense and Agility figures, in pixels.
#if LANGUAGE != LANGUAGE_JAPANESE
#define OWNER_STATUS_TEXT_MAX 128
#define OWNER_STATUS_VALUE_X 184
#else
#define OWNER_STATUS_TEXT_MAX 64
#define OWNER_STATUS_VALUE_X 176
#endif

/// The places of the status icons in a row beside the party member's name, 15
/// pixels apart.
#define OWNER_STATUS_ICON_PLACES 11
/// The status icons shown at most.
#define OWNER_STATUS_ICONS_MAX 8

/// The message of a stat's label on the Status screen, by its row.
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
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

/// Moves `cursor` to the stop its group remembers in `stops`: that stop's help
/// entry, column and row.
#define STATUS_CURSOR_TAKE_STOP(cursor, stops)                                 \
  ((cursor).helpEntry =                                                        \
       (stops)[(cursor).groupPositions[(cursor).group]].entry,                 \
   (cursor).column = (stops)[(cursor).groupPositions[(cursor).group]].column,  \
   (cursor).row = (stops)[(cursor).groupPositions[(cursor).group]].row)

/// Runs the Status screen for party member `member` until B is pressed or the
/// link battle's time runs out: the party member's level, experience, HP and
/// PP, class, stats, Djinn, elements and statuses, with a help line for the
/// item under the cursor.
s32 BattleStatusScreen_Run(u16 *list, s32 listCount, s32 member) {
  struct MenuSprite figureSprite;
  struct MenuSprite cursorSprite;
  s32 statusSlots[OWNER_STATUS_ICON_PLACES];
  s8 statusKinds[OWNER_STATUS_ICON_PLACES];
  struct MenuSprite statusSprites[OWNER_STATUS_ICON_PLACES];
  struct MenuSprite portraitSprite;
  struct StatusCursor cursor;
  struct WindowState *state;
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
  struct BattleMenuState *navigation;
  s32 iconX;

  state = WINDOW_STATE;
  redraw = OWNER_STATUS_REDRAW_ALL;
  memberIndex = -1;
  portraitSlot = VramCache_AllocEntry(512);
  statusCount = 0;
  hasDjinn = Party_SumDjinnCountsFar(-1);

  navigation = gBattleMenuState;
  navigation->timerHidden = 1;
  if (navigation->timerWindow != 0) {
    Window_Close(navigation->timerWindow, 1);
    navigation->timerWindow = 0;
  }
  Sound_PlayFar(SE_MENU_CONFIRM);

  for (i = 0; i < 5; i++) {
    cursor.groupPositions[i] = 0;
  }
  cursor.helpEntry = 0;
  cursor.drawnHelpEntry = 0;
  cursor.group = STATUS_CURSOR_GENERAL;

  cursorSlot = VramCache_AllocEntry(128);
  for (i = 0; i < OWNER_STATUS_ICON_PLACES; i++) {
    statusSlots[i] = VramCache_AllocEntry(128);
    statusKinds[i] = OWNER_CONDITION_EMPTY;
  }

  // Find the party member's place in the list, for L and R.
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
  statusWindow = Window_Create(0, 0, 30, 20, MENU_WINDOW_FLAGS);
  helpWindow = Window_Create(0, 14, 30, 6,
                             WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS);
  Window_MarkVisibleTextTiles();

  for (;;) {
    keys = gKeysRepeat;

    // The party member's 32x32 portrait at the top left.
    portraitSprite.oam.word.attr01 =
        OBJ_SIZE_32X32 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
    portraitSprite.oam.word.attr23 = 0;
    portraitSprite.oam.f.tile = Portrait_Load(member, portraitSlot);
    portraitSprite.oam.f.x = 8;
    portraitSprite.oam.f.y = 24;
    portraitSprite.oam.f.palette = PORTRAIT_PALETTE;
    Oam_AddSpriteAtDepth(&portraitSprite, OAM_DEPTH_WINDOW);

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
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
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
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
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
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          // A, Right or Left toggles between the general group and the
          // statuses.
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
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        } else if ((keys & KEY_LEFT) != 0) {
          keys = 0;
          groupPosition--;
          if (groupPosition < 0) {
            cursor.group = STATUS_CURSOR_GENERAL;
            groupPosition = cursor.groupPositions[0];
          }
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        } else if ((keys & (KEY_A | KEY_UP | KEY_DOWN)) != 0) {
          // A, Up or Down go back to the general group.
          keys = 0;
          cursor.group = STATUS_CURSOR_GENERAL;
          groupPosition = cursor.groupPositions[0];
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
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
      STATUS_CURSOR_TAKE_STOP(cursor, gBattleStatusScreen_EquipmentCursors);
    } else if (cursor.group == STATUS_CURSOR_CONDITIONS) {
      STATUS_CURSOR_TAKE_STOP(cursor, gBattleStatusScreen_ConditionCursors);
    }

    if (cursor.drawnHelpEntry != cursor.helpEntry) {
      cursor.drawnHelpEntry = cursor.helpEntry;
      redraw = OWNER_STATUS_REDRAW_HELP;
    }
    cursorPosition.x = cursor.column * 8;
    cursorPosition.y = cursor.row * 8;

    // The party member's battle figure: its battle object's sprite image's
    // tiles, as a 64x64, 256-colour sprite at (172, 56).
    {
      struct Sprite *figure = BattleObject_GetFar(member)->actor->sprite;

      figureSprite.oam.word.attr01 =
          OBJ_SIZE_64X64 | OBJ_256_COLORS | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
      figureSprite.oam.word.attr23 = 0;
      /* FAKEMATCH(barrier): an empty asm after the figure's two stores, which
       * only es and it need; as no asm. */
      asm("");
      figureSprite.oam.f.tile = figure->image.tile;
      figureSprite.oam.f.x = 172;
      figureSprite.oam.f.y = 56;
      Oam_AddSpriteAtDepth(&figureSprite, OAM_DEPTH_WINDOW);
    }

    cursorSprite.oam.word.attr01 =
        OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
    cursorSprite.oam.word.attr23 = 0;
    /* FAKEMATCH(barrier): an empty asm after the cursor's two stores; the
     * literal pool needs 4 bytes counted that no instruction emits; as no asm.
     */
    asm("");
    cursorSprite.oam.f.tile =
        VramCache_Reload(cursorSlot, (s32)gPointingHandTiles);
    cursorSprite.oam.f.x =
        (cursorPosition.x + statusWindow->x * 8 - ((gFrameCount & 4) >> 2)) +
        16;
    cursorSprite.oam.f.y =
        (cursorPosition.y + statusWindow->y * 8 - ((gFrameCount & 4) >> 2)) +
        16;
    // 8 sets attribute 1's bit 12, a sprite's horizontal flip when it is not
    // affine: the hand is mirrored.
    cursorSprite.oam.f.affineIndex = 8;
    Oam_AddSpriteAtDepth(&cursorSprite, OAM_DEPTH_WINDOW + 1);

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

#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
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

#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
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
        Text_DrawNumberInWindow(unit->attack, 3, statusWindow,
                                OWNER_STATUS_VALUE_X, 16);
        Text_DrawMessageAsTiles(OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_DEFENSE),
                                statusWindow, 136, 24);
        Text_DrawNumberInWindow(unit->defense, 3, statusWindow,
                                OWNER_STATUS_VALUE_X, 24);
        Text_DrawMessageAsTiles(OWNER_STATUS_LABEL(OWNER_STATUS_LABEL_AGILITY),
                                statusWindow, 136, 32);
        Text_DrawNumberInWindow(unit->agility, 3, statusWindow,
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

        for (j = 0; j < ELEMENT_COUNT; j++) {
          s32 pal;

          pal = 1;
          if (hasDjinn != 0) {
            pal = 0;
          }
          Window_SetTilemapEntry(statusWindow, LIST_TILE_SUMMON_ELEMENT + j,
                                 7 + j * 4, pal + 8, 0);
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

        // The status icons, at most OWNER_STATUS_ICONS_MAX, in this order:
        // downed, cursed item, Grim Reaper, poison, deadly poison, Psynergy
        // sealed, stun, sleep, haunt, delusion, then Attack, Defense and
        // Resistance raised or lowered, and Agility. OWNER_CONDITION_NORMAL
        // alone means none.
        i = 0;
        if (unit->hp == 0) {
          statusKinds[0] = OWNER_CONDITION_DOWNED;
          i = 1;
        }
        do {
          if (unit->curse != 0) {
            statusKinds[i++] = OWNER_CONDITION_CURSED_ITEM;
          }
          if (i >= OWNER_STATUS_ICONS_MAX) {
            break;
          }
          if (unit->grimReaper != 0) {
            statusKinds[i++] = OWNER_CONDITION_GRIM_REAPER;
          }
          if (i >= OWNER_STATUS_ICONS_MAX) {
            break;
          }
          if (unit->poison == 1) {
            statusKinds[i++] = OWNER_CONDITION_POISON;
          }
          if (i >= OWNER_STATUS_ICONS_MAX) {
            break;
          }
          if (unit->poison == 2) {
            statusKinds[i++] = OWNER_CONDITION_DEADLY_POISON;
          }
          if (i >= OWNER_STATUS_ICONS_MAX) {
            break;
          }
          if (unit->psySeal != 0) {
            statusKinds[i++] = OWNER_CONDITION_PSYNERGY_SEALED;
          }
          if (i >= OWNER_STATUS_ICONS_MAX) {
            break;
          }
          if (unit->stun != 0) {
            statusKinds[i++] = OWNER_CONDITION_STUN;
          }
          if (i >= OWNER_STATUS_ICONS_MAX) {
            break;
          }
          if (unit->sleep != 0) {
            statusKinds[i++] = OWNER_CONDITION_SLEEP;
          }
          if (i >= OWNER_STATUS_ICONS_MAX) {
            break;
          }
          if (unit->haunt != 0) {
            statusKinds[i++] = OWNER_CONDITION_HAUNT;
          }
          if (i >= OWNER_STATUS_ICONS_MAX) {
            break;
          }
          if (unit->delusion != 0) {
            statusKinds[i++] = OWNER_CONDITION_DELUSION;
          }
          if (i >= OWNER_STATUS_ICONS_MAX) {
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
          if (i >= OWNER_STATUS_ICONS_MAX) {
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
          if (i >= OWNER_STATUS_ICONS_MAX) {
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
          if (i >= OWNER_STATUS_ICONS_MAX) {
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
          StatusIcon_LoadIntoSlot(statusKinds[j], statusSlots[j]);
        }
        if (i == 0) {
          statusKinds[0] = OWNER_CONDITION_NORMAL;
          i = 1;
        }
        if (i < OWNER_STATUS_ICON_PLACES) {
          for (j = i; j < OWNER_STATUS_ICON_PLACES; j++) {
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

      helpText = (u16 *)Heap_Alloc(OWNER_STATUS_TEXT_MAX * sizeof(*helpText));
      if ((u32)cursor.helpEntry >= OWNER_STATUS_HELP_CONDITION) {
        statusKind =
            statusKinds[cursor.helpEntry - OWNER_STATUS_HELP_CONDITION];
        if (statusKind == OWNER_CONDITION_NORMAL && unit->hp == 0) {
          statusKind = OWNER_CONDITION_DOWNED;
        }

        // A status's help carries a number: the Grim Reaper's turns left,
        // what the stat stages add (the stats less those recalculated with
        // the stages cleared, after which the unit is restored from a copy),
        // or RESISTANCE_PER_STAGE for each Resistance stage.
        savedUnit = (struct Unit *)Heap_Alloc(sizeof(*unit));
        helpValue = 0;
        Iwram_CopyWords(savedUnit, unit, sizeof(*unit));

        attackDelta = unit->attack;
        defenseDelta = unit->defense;
        agilityDelta = unit->agility;
        unit->attackStage = 0;
        unit->defenseStage = 0;
        unit->agilityStage = 0;
        Unit_CalculateFar(member);
        attackDelta -= unit->attack;
        defenseDelta -= unit->defense;
        agilityDelta -= unit->agility;
        Iwram_CopyWords(unit, savedUnit, sizeof(*unit));
        Heap_Release(savedUnit);

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
          helpValue = unit->resistanceStage * RESISTANCE_PER_STAGE;
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
                 unit->level < UNIT_LEVEL_MAX) {
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
      Heap_Release(helpText);
      redraw = 0;
      state->dirty = 1;
    }

    // The status icons, 15 pixels apart from x 112 on the first row;
    // OWNER_CONDITION_NORMAL and empty places are not drawn.
    statusSprite = statusSprites;
    statusSlot = statusSlots;
    {
      struct VramCacheEntry *entry;

      for (i = 0, entry = gVramCacheEntries, iconX = 112;
           i < OWNER_STATUS_ICON_PLACES; i++) {
        MenuSprite_Initialize(*statusSprite,
                              OBJ_SIZE_16X16 |
                                  OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
        statusSprite->oam.f.tile = entry[*statusSlot++].offset >> 5;
        statusSprite->oam.f.x = iconX;
        statusSprite->oam.f.y = (u8)(statusWindow->y * 8 + 8);
        if (statusKinds[i] > OWNER_CONDITION_NORMAL) {
          Oam_AddSpriteAtDepth(statusSprite, OAM_DEPTH_WINDOW);
        }
        iconX += 15;
        statusSprite++;
      }
    }

    if (gBattleMenuState->timer == 0) {
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
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      } else if ((keys & KEY_L) != 0) {
        memberIndex--;
        if (memberIndex < 0) {
          memberIndex = listCount - 1;
        }
        member = list[memberIndex];
        redraw = OWNER_STATUS_REDRAW_ALL;
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      }
    }
    wait(1);
  }

  {
    s32 *statusSlotCursor = statusSlots;

    for (i = 0; i < OWNER_STATUS_ICON_PLACES; i++) {
      VramCache_FreeEntry(*statusSlotCursor++);
    }
  }
  VramCache_FreeEntry(portraitSlot);
  VramCache_FreeEntry(cursorSlot);
  wait(1);
#if TBS_PATCH_1
  Window_MarkVisibleTextTiles();
#endif
  Window_Close(statusWindow, 1);
  Window_Close(helpWindow, 1);
  PartyStatus_Draw(gBattleState->partyStatus);
  gBattleMenuState->timerHidden = 0;
  wait(1);
  return 0;
}
