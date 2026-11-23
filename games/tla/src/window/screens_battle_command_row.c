/// @file
/// The row of command icons the player picks a command from.
// Part of screens.c.

#include "window_internal.h"
#include "sounds.h"
#include "window/screens.h"
#include "menu/djinn_menu.h"

/// Opens a message at (x, y), waits for its text to finish, and returns its
/// window so the caller can hold it open for a separate prompt.
struct Window *Message_ShowAndWait(s32 message, s32 x, s32 y) {
  struct Window *result;

  result = Message_OpenWindow(message, x, y, MESSAGE_WINDOW_ZOOM);
  while (Message_AllTyped() == 0)
    wait(1);
  return result;
}

/// The tile column where a party member's command row ends and its name window
/// begins.
#if LANGUAGE != LANGUAGE_JAPANESE
#define WINDOW_X 22
#else
#define WINDOW_X 23
#endif

/// BG1's tile number of the command row's first icon tile.
#define COMMAND_ROW_ICON_TILE (BG_CHAR_SIZE / (8 * 8))

/// BG1's map row shown on the command row's top row, at the battle
/// background's scroll.
#define COMMAND_ROW_BG1_ROW (COMMAND_ROW_TOP + BATTLE_BACKGROUND_SCROLL_Y / 8)

/// The selected icon's smallest scale, 8.8 fixed point, as it pulses.
#define PULSE_SCALE_MIN Q_8_8(1.1875)

#include "system/bg_scroll.h"
#include "system/main_routine_work.h"

/// Runs a battle command row: the commands as icons along the bottom of the
/// screen, the selected one pulsing as a sprite and named in a window, until
/// a command is chosen. Returns the command chosen (enum CommandIcon), -1 when
/// B cancels, or -2 when L or R asks for the Status screen; the last two in a
/// party member's row only. When the link battle's time runs out the row takes
/// its first command. Battle_CollectPartyCommands runs both rows.
///
/// COMMAND_ROW_PARTY is the party's row at the start of a turn: Fight, Flee
/// and Status. COMMAND_ROW_MEMBER, as any other mode, is a party member's row:
/// Attack, Psynergy, Djinn once the party's Djinn are enough for a summon
/// (DjinnRecoveryTable.summonMask), Summon while any Djinni is on standby,
/// Item and Defend. Each row remembers its choice in
/// BattleMenuWork.commandSelection. While the Djinn tutorial runs, the row
/// presses the keys for the player.
s32 BattleCommandRow_Run(s32 mode, s32 fleeAllowed) {
  u8 party[4];
  struct WindowWork *work;
  struct BattleCommandRowWork *selectionUi;
  struct BattleMenuWork *battleMenu;
  struct BattleMenuWork *navigation;
  s32 stripDirty;
  s32 tradeCount;
  s32 pulseScale;
  s32 command;
  s32 entryIndex;
  s32 keys;
  s32 repeat;
  s32 iconSlot;
  s32 i;
  s32 kindCount;

  work = WINDOW_WORK;
  // One 1 KB entry for the selected icon's 32x32, 256-colour sprite.
  iconSlot = VramCache_AllocEntry(32 * 32);
  stripDirty = 1;
  tradeCount = 0;
  battleMenu = BATTLE_MENU_WORK;
  selectionUi = Heap_AllocEwram(sizeof(*selectionUi));
  // No icon colour placed yet; the shared palette starts at BG entry 1.
  for (i = ARRAY_COUNT(selectionUi->palette.slot) - 1; i >= 0; i--)
    selectionUi->palette.slot[i] = GLYPH_PALETTE_SLOT_FREE;
  selectionUi->palette.count = 1;
  if (mode == COMMAND_ROW_PARTY)
    selectionUi->window = Window_Create(20, COMMAND_ROW_TOP, 10, 3, 6);
  else
    selectionUi->window = Window_Create(WINDOW_X, COMMAND_ROW_TOP,
                                        DISPLAY_TILE_WIDTH - WINDOW_X, 3, 6);
  selectionUi->drawnIndex = -1;
  if (mode == COMMAND_ROW_PARTY) {
    kindCount = 0;
    selectionUi->commandKinds[kindCount++] = COMMAND_ICON_FIGHT;
    if (Ram_HeapSlots->battleWork->isLink == 0 &&
        battleMenu->swappedUnit == -1 && Party_CountMembersFar() > 4)
      selectionUi->commandKinds[kindCount++] = COMMAND_ICON_SWITCH;
    if (fleeAllowed > 0 && gMainRoutineWork.battleType != BATTLE_TYPE_NO_ESCAPE)
      selectionUi->commandKinds[kindCount++] = COMMAND_ICON_FLEE;
    selectionUi->commandKinds[kindCount++] = COMMAND_ICON_STATUS;
    selectionUi->commandKinds[kindCount] = -1;
    selectionUi->selectedIndex = BATTLE_MENU_WORK->commandSelection[0];
  } else {
    s32 memberCount;

    selectionUi->selectedIndex = BATTLE_MENU_WORK->commandSelection[1];
    memberCount = 0;
    selectionUi->commandKinds[memberCount++] = COMMAND_ICON_ATTACK;
    selectionUi->commandKinds[memberCount++] = COMMAND_ICON_PSYNERGY;
    if (*(s32 *)Djinn_GetRecoveryTableFar(0) != 0) {
      BattleDjinn_CountStandbyFar(0, party);
      for (i = 0; i <= 3; i++)
        tradeCount += party[i];
      selectionUi->commandKinds[memberCount++] = COMMAND_ICON_DJINN;
      if (tradeCount != 0)
        selectionUi->commandKinds[memberCount++] = COMMAND_ICON_SUMMON;
    }
    selectionUi->commandKinds[memberCount++] = COMMAND_ICON_ITEM;
    selectionUi->commandKinds[memberCount++] = COMMAND_ICON_DEFEND;
    selectionUi->commandKinds[memberCount] = -1;
  }
  for (entryIndex = 0; entryIndex <= 5; entryIndex++) {
    selectionUi->entries[entryIndex].resource = iconSlot;
    if (selectionUi->commandKinds[entryIndex] == -1)
      break;
    BattleCommandRow_LoadIcon(selectionUi, entryIndex,
                              selectionUi->commandKinds[entryIndex]);
  }
  selectionUi->iconCount = entryIndex;
  selectionUi->affine.scaleX = Q_8_8(1.25);
  selectionUi->affine.scaleY = PULSE_SCALE_MIN;
  selectionUi->affine.angle = 0;
  entryIndex = 0;
  if (entryIndex < selectionUi->iconCount) {
    do {
      s32 x = (entryIndex - selectionUi->iconCount) * 24;

      selectionUi->entries[entryIndex].positionX = x + 155;
      if (mode != COMMAND_ROW_PARTY)
        selectionUi->entries[entryIndex].positionX = x + WINDOW_X * 8 - 5;
      selectionUi->entries[entryIndex].positionY = COMMAND_ROW_TOP * 8;
      entryIndex++;
    } while (entryIndex < selectionUi->iconCount);
  }
  // BG1 shows the icons: behind the other layers above the row, in front from
  // its top line down (CommandRow_PutBg1Behind).
  Task_Add(CommandRow_PutBg1Behind, TASK_PASS_VBLANK);
  Irq_SetHandler(IRQ_VCOUNT, COMMAND_ROW_TOP * 8, CommandRow_PutBg1InFront);

  for (;;) {
    // PULSE_SCALE_MIN to 1.3125 times, over 16 frames.
    pulseScale = gWindowSprite_BounceScales[(gFrameCount * 2) & 31];
    pulseScale = (pulseScale - Q_8_8(1)) / 4 + PULSE_SCALE_MIN;
    selectionUi->affine.scaleY = selectionUi->affine.scaleX = pulseScale;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
    // Draw the strip once, six panels of 3x3 tiles ending at the name window:
    // the panels left empty as WINDOW_TILE_COMMAND_PANEL in the UI tilemap,
    // then each icon's tiles in BG1's map, the UI tilemap cleared over them.
    if (stripDirty != 0) {
      stripDirty = 0;
      work->holdTilemapUpload = 1;
      if (mode != COMMAND_ROW_PARTY) {
        s32 panel;

        for (panel = 0; panel < 6 - selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;

            for (col = 0; col <= 2; col++)
              *(work->tilemap[0] +
                (entryIndex + COMMAND_ROW_TOP) * BG_SCREEN_WIDTH + panel * 3 +
                WINDOW_X - 18 + (col & 3)) = WINDOW_TILE_COMMAND_PANEL;
          }
        }
        for (panel = 0; panel < selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;

            for (col = 0; col <= 2; col++) {
              volatile u16 *vram =
                  (volatile u16 *)BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK) +
                  COMMAND_ROW_BG1_ROW * BG_SCREEN_WIDTH + WINDOW_X;

              vram[panel * 3 + entryIndex * BG_SCREEN_WIDTH + (col & 3) -
                   selectionUi->iconCount * 3] =
                  panel * 16 + entryIndex * 4 + col + COMMAND_ROW_ICON_TILE;
              *(work->tilemap[0] +
                (entryIndex + COMMAND_ROW_TOP) * BG_SCREEN_WIDTH + panel * 3 +
                (col & 3) - selectionUi->iconCount * 3 + WINDOW_X) = 0;
            }
          }
        }
      } else {
        s32 panel;

        for (panel = 0; panel < 6 - selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;

            for (col = 0; col <= 2; col++)
              *(work->tilemap[0] +
                (entryIndex + COMMAND_ROW_TOP) * BG_SCREEN_WIDTH + panel * 3 +
                2 + (col & 3)) = WINDOW_TILE_COMMAND_PANEL;
          }
        }
        for (panel = 0; panel < selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;

            for (col = 0; col <= 2; col++) {
              volatile u16 *vram =
                  (volatile u16 *)BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK) +
                  COMMAND_ROW_BG1_ROW * BG_SCREEN_WIDTH + 20;

              vram[panel * 3 + entryIndex * BG_SCREEN_WIDTH + (col & 3) -
                   selectionUi->iconCount * 3] =
                  panel * 16 + entryIndex * 4 + col + COMMAND_ROW_ICON_TILE;
              *(work->tilemap[0] +
                (entryIndex + COMMAND_ROW_TOP) * BG_SCREEN_WIDTH + panel * 3 +
                (col & 3) - selectionUi->iconCount * 3 + 20) = 0;
            }
          }
        }
      }
    }
    if (selectionUi->drawnIndex != selectionUi->selectedIndex) {
      WindowSprite_PrepareForRedraw(selectionUi->window);
      Text_DrawMessageAsTiles(
          selectionUi->entries[selectionUi->selectedIndex].kind +
              (s32)MsgCommandName,
          selectionUi->window, 0, 0);
      selectionUi->drawnIndex = selectionUi->selectedIndex;
      BattleCommandRow_LoadIcon(
          selectionUi, selectionUi->selectedIndex,
          selectionUi->commandKinds[selectionUi->selectedIndex]);
    }
    selectionUi->affineIndex =
        OamMatrix_AllocFromTransform(&selectionUi->affine);
    for (entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++) {
      struct BattleCommandRowIcon *entry = &selectionUi->entries[entryIndex];

      if (entryIndex == selectionUi->selectedIndex) {
        entry->sprite.oam.f.affineIndex = selectionUi->affineIndex;
        entry->sprite.oam.f.affine = 3;
        entry->sprite.oam.f.x = entry->positionX + pulseScale * 7 / 512 - 14;
        entry->sprite.oam.f.y = entry->positionY + pulseScale * 3 / 256 - 20;
        Oam_AddSpriteAtDepth(entry, OAM_DEPTH_WINDOW + 1);
      } else {
        entry->sprite.oam.f.x = entry->positionX;
        entry->sprite.oam.f.y = entry->positionY;
        entry->sprite.oam.f.affineIndex = 0;
        entry->sprite.oam.f.affine = 0;
      }
    }

    // The Djinn tutorial presses keys for the player: Felix is led to the Djinn
    // and Summon commands, Jenna and Sheba defend, and the party's row takes
    // Fight.
    keys = gInput.pressed;
    repeat = gInput.repeat;
    navigation = BATTLE_MENU_WORK;
    if (navigation->tutorialStep != 0) {
      keys = repeat = 0;
      if (navigation->tutorialDelay == 0) {
        if ((u32)(navigation->commandActor - JENNA) <= SHEBA - JENNA) {
          if (selectionUi->commandKinds[selectionUi->selectedIndex] ==
              COMMAND_ICON_DEFEND)
            keys = repeat = KEY_A;
          else
            keys = repeat = KEY_LEFT;
          navigation->tutorialDelay = 30;
        } else if (navigation->commandActor == FELIX) {
          s32 kind = selectionUi->commandKinds[selectionUi->selectedIndex];

          if (kind == COMMAND_ICON_SUMMON ||
              (tradeCount == 0 && kind == COMMAND_ICON_DJINN)) {
            if (navigation->tutorialStep == 1) {
              void *message;

              ((struct WindowWork *)Ram_HeapSlots->windowWork)->battleMode |=
                  WINDOW_GLYPHS_BLENDED;
              if (selectionUi->commandKinds[selectionUi->selectedIndex] ==
                  COMMAND_ICON_DJINN)
                message =
                    Message_ShowAndWait(Djinn_GetGiveTutorialMessage() +
                                            DJINN_TUTORIAL_LINE_UNLEASH_COMMAND,
                                        15, 8);
              else if (selectionUi->commandKinds[selectionUi->selectedIndex] ==
                       COMMAND_ICON_SUMMON)
                message = Message_ShowAndWait(Djinn_GetGiveTutorialMessage() +
                                                  DJINN_TUTORIAL_LINE_SUMMON,
                                              15, 8);
              ((struct WindowWork *)Ram_HeapSlots->windowWork)->battleMode &=
                  ~WINDOW_GLYPHS_BLENDED;
              BattleMessage_WaitForAdvanceAtFar(102, 155);
              Window_Close(message, 1);
              navigation->tutorialStep++;
              navigation->tutorialDelay = 45;
            } else {
              keys = repeat = KEY_A;
              navigation->tutorialDelay = 200;
            }
          } else {
            keys = repeat = KEY_RIGHT;
            navigation->tutorialDelay = 40;
          }
        } else {
          navigation->tutorialDelay = 60;
          keys = repeat = KEY_A;
        }
      } else {
        navigation->tutorialDelay--;
      }
    }
    if ((keys & KEYS_SHOULDERS) && mode != COMMAND_ROW_PARTY) {
      command = -2;
      break;
    }
    if (keys & KEY_A) {
      command = selectionUi->commandKinds[selectionUi->selectedIndex];
      break;
    }
    if (keys & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      command = -1;
      break;
    }
    if (repeat & (KEY_RIGHT | KEY_DOWN)) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      selectionUi->selectedIndex =
          (selectionUi->selectedIndex + 1) % selectionUi->iconCount;
    } else if (repeat & (KEY_LEFT | KEY_UP)) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      selectionUi->selectedIndex =
          (selectionUi->selectedIndex + selectionUi->iconCount - 1) %
          selectionUi->iconCount;
    } else if (BATTLE_MENU_WORK->timer == 0) {
      // The link battle's time is up.
      command = selectionUi->commandKinds[0];
      break;
    }
    IoWriteQueue_Store16((u32)&REG_DISPCNT,
                         DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                             DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON);
    work->holdTilemapUpload = 0;
    wait(1);
  }
  if (mode != COMMAND_ROW_PARTY)
    BATTLE_MENU_WORK->commandSelection[1] = selectionUi->selectedIndex;
  else
    BATTLE_MENU_WORK->commandSelection[0] = selectionUi->selectedIndex;
  for (entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++)
    VramCache_FreeEntry(selectionUi->entries[entryIndex].resource);
  work->holdTilemapUpload = 1;
  Window_Close(selectionUi->window, 1);
  // Cover the strip again.
  {
    s32 panel;

    for (panel = 0; panel <= 6; panel++) {
      for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
        s32 col;

        for (col = 0; col <= 2; col++)
          *(volatile u16 *)(work->tilemap[0] +
                            (entryIndex + COMMAND_ROW_TOP) * BG_SCREEN_WIDTH +
                            panel * 3 + 5 + (col & 3)) =
              WINDOW_TILE_COMMAND_PANEL;
      }
    }
  }
  work->dirty = 1;
  wait(1);
  Task_Remove(CommandRow_PutBg1Behind);
  Irq_SetHandler(IRQ_VCOUNT, 0, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
                      IO_WRITE_HALFWORD);
  Heap_Release(selectionUi);
  work->holdTilemapUpload = 0;
  wait(1);
  return command;
}

/// Switches the UI tilemap entries of the `width` x `height` tile rectangle at
/// screen tile (x, y) to WINDOW_PALETTE or HIGHLIGHT_PALETTE by bit 12, keeping
/// the rest of each entry: the menus' row highlight. `paletteBit` is the
/// palette, of which only the low bit counts.
void Window_HighlightRect(s32 x, s32 y, s32 width, s32 height, s32 paletteBit) {
  struct WindowWork *work = Ram_HeapSlots->windowWork;

  paletteBit &= 1;
  paletteBit = BG_TILE_PALETTE(paletteBit);

  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > DISPLAY_TILE_WIDTH - 1) {
    width = DISPLAY_TILE_WIDTH - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  // BUG: tests the bottom against the screen's width, so a window reaching
  // below the screen draws into the map's unseen rows.
#ifdef BUGFIX
  if (y + height > DISPLAY_TILE_HEIGHT - 1) {
#else
  if (y + height > DISPLAY_TILE_WIDTH - 1) {
#endif
    height = DISPLAY_TILE_HEIGHT - y;
  }

  if (width > 0 && height > 0) {
    do {
      u16 *cell = &work->tilemap[y][x];
      s32 remaining = width;

      while (remaining != 0) {
        u32 tile = *cell;
        remaining--;
        tile = (tile & ~BG_TILE_PALETTE(1)) | paletteBit;
        *cell = tile;
        cell++;
      }
      UiRender_MarkRow(work, y);
      height--;
      y++;
    } while (height != 0);
  }
}

/// Clears the highlight from the party status window's party member columns,
/// then highlights the columns of the units in `ids` (up to four, ended by
/// BATTLE_UNIT_LIST_END): the party member whose commands are being chosen. A
/// unit not in the party is skipped. The window is right-aligned on the 30-tile
/// row, so its party member columns start at column 29 - 6 * members (the Djinn
/// column, when shown, lies left of them). Returns 0.
/// Battle_CollectPartyCommands calls it.
s32 PartyStatus_HighlightMembers(u16 *ids) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 count;
  s32 i;
  s32 j;

  count = BattleUnit_ListIdsFar(1, 0);
  Window_HighlightRect(29 - count * 6, 0, 25, 5, WINDOW_PALETTE);
  i = 0;
  if (ids[0] != BATTLE_UNIT_LIST_END) {
  next_id:
    for (j = 0; j < 4 && battle->partyUnits[j] != ids[i]; j++) {
      if (battle->partyUnits[j] == BATTLE_UNIT_LIST_END) {
        j = 4;
        break;
      }
    }
    if (j != 4)
      Window_HighlightRect(29 - (count - j) * 6, 0, 7, 5, HIGHLIGHT_PALETTE);
    if (++i < 4 && ids[i] != BATTLE_UNIT_LIST_END)
      goto next_id;
  }
  return 0;
}

#undef WINDOW_X
