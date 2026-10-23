/// @file
/// The row of command icons the player picks a command from.
// Part of screens.c.

#include "../screens_internal.h"

/// Opens a message at (x, y), waits for its text to finish, and returns its
/// window so the caller can hold it open for a separate prompt.
struct Window *Message_ShowAndWait(s32 message, s32 x, s32 y) {
  struct Window *result;

  result = Message_OpenWindow(message, x, y, 1);
  while (TextPrinter_AllFinished() == 0)
    WaitFrames(1);
  return result;
}

// The tile column where a member's command row ends and its name window
// begins.
#if LANGUAGE != LANGUAGE_JAPANESE
#define WINDOW_X 22
#else
#define WINDOW_X 23
#endif

/// Runs a battle command row: the commands as icons along the bottom of the
/// screen, the selected one pulsing as a sprite and named in a window, until
/// a command is chosen. Returns the command chosen (enum CommandIcon), -1 when
/// B cancels, or -2 when L or R asks for the summary screen; the last two in a
/// member's row only. When the link battle's time runs out the row takes its
/// first command. Battle_CollectPartyCommands runs both rows.
///
/// COMMAND_ROW_PARTY is the party's row at the start of a round: Fight, Flee
/// and Status. COMMAND_ROW_MEMBER, as any other mode, is a member's row:
/// Attack, Psynergy, Djinn once the party's Djinn are enough for a summon
/// (BattleSummonState.availableMask), Summon while any Djinni is on standby,
/// Item and Defend. Each row remembers its choice in
/// BattleMenuWork.commandSelection. While the Djinn tutorial runs, the row
/// presses the keys for the player.
s32 BattleCommandRow_Run(s32 mode) {
  u8 party[4];
  struct WindowWork *work;
  struct SelectionWork *selectionUi;
  struct BattleMenuWork *navigation;
  s32 stripDirty, tradeCount, pulseScale, command;
  s32 entryIndex;
  s32 keys, repeat;
  s32 iconSlot;
  s32 i, kindCount;

  work = WINDOW_WORK;
  // One 1 KB entry for the selected icon's 32x32, 256-colour sprite.
  iconSlot = VramCache_AllocEntry(0x400);
  stripDirty = 1;
  tradeCount = 0;
  selectionUi = AllocEwram(sizeof(*selectionUi));
  // No icon colour placed yet; the shared palette starts at BG entry 1.
  for (i = 255; i >= 0; i--)
    selectionUi->palette.slot[i] = 255;
  selectionUi->palette.count = 1;
  if (mode == COMMAND_ROW_PARTY)
    selectionUi->window = Window_Create(20, 17, 10, 3, 6);
  else
    selectionUi->window = Window_Create(WINDOW_X, 17, 30 - WINDOW_X, 3, 6);
  selectionUi->drawnIndex = -1;
  // The commands, as listed above. tradeCount counts the party's standby
  // Djinn.
  if (mode == COMMAND_ROW_PARTY) {
    kindCount = 0;
    selectionUi->commandKinds[kindCount++] = COMMAND_ICON_FIGHT;
    selectionUi->commandKinds[kindCount++] = COMMAND_ICON_FLEE;
    selectionUi->commandKinds[kindCount++] = COMMAND_ICON_STATUS;
    selectionUi->commandKinds[kindCount] = -1;
    selectionUi->selectedIndex = gBattleMenuWork->commandSelection[0];
  } else {
    selectionUi->selectedIndex = gBattleMenuWork->commandSelection[1];
    kindCount = 0;
    selectionUi->commandKinds[kindCount++] = COMMAND_ICON_ATTACK;
    selectionUi->commandKinds[kindCount++] = COMMAND_ICON_PSYNERGY;
    if (*((s32 *)Djinn_GetRecoveryTableFar(0)) != 0) {
      BattleDjinn_CountStandbyFar(0, party);
      for (i = 0; i <= 3; i++)
        tradeCount += party[i];
      selectionUi->commandKinds[kindCount++] = COMMAND_ICON_DJINN;
      if (tradeCount != 0)
        selectionUi->commandKinds[kindCount++] = COMMAND_ICON_SUMMON;
    }
    selectionUi->commandKinds[kindCount++] = COMMAND_ICON_ITEM;
    selectionUi->commandKinds[kindCount++] = COMMAND_ICON_DEFEND;
    // UB: with all six commands this -1 lands in commandKinds[6], one word
    // past the end of the block; the loops below stop at six and never read
    // it.
    selectionUi->commandKinds[kindCount] = -1;
  }
  // Decode each visible command through one shared remapped palette.
  for (entryIndex = 0; entryIndex <= 5; entryIndex++) {
    selectionUi->entries[entryIndex].resource = iconSlot;
    if (selectionUi->commandKinds[entryIndex] == -1)
      break;
    Text_DrawCharacter(selectionUi, entryIndex,
                       selectionUi->commandKinds[entryIndex]);
  }
  selectionUi->iconCount = entryIndex;
  selectionUi->affine.scaleX = 320;
  selectionUi->affine.scaleY = 304;
  selectionUi->affine.angle = 0;
  // Icons 24 pixels apart on line 136 (tile row 17), the last ending by the
  // name window.
  entryIndex = 0;
  if (entryIndex < selectionUi->iconCount) {
    do {
      s32 x = (entryIndex - selectionUi->iconCount) * 24;
      selectionUi->entries[entryIndex].positionX = x + 155;
      if (mode != COMMAND_ROW_PARTY)
        selectionUi->entries[entryIndex].positionX = x + WINDOW_X * 8 - 5;
      selectionUi->entries[entryIndex].positionY = 136;
      entryIndex++;
    } while (entryIndex < selectionUi->iconCount);
  }
  // BG1 shows the icons: behind the other layers above line 136, in front
  // from there down (Graphics_SetBg1Priority3).
  Task_Add(Graphics_SetBg1Priority3, 0x480);
  Runtime_SetIrqHandler(2, 136, Graphics_SetBg1Priority0);

  for (;;) {
    // 304 to 336 in 8.8 (1.19 to 1.31 times), over 16 frames.
    pulseScale = gWindowSprite_BounceScales[(gFrameCount * 2) & 31];
    pulseScale = (pulseScale - 256) / 4 + 304;
    selectionUi->affine.scaleY = selectionUi->affine.scaleX = pulseScale;
    // Scrolled up 32 pixels, BG1's map rows 21 to 23 show on rows 17 to 19.
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 32;
    // Draw the strip once, six panels of 3x3 tiles ending at the name
    // window: the panels left empty as WINDOW_TILE(0x7f) in the UI tilemap,
    // then each icon's top-left 3x3 tiles in BG1's map (screen block 31 at
    // 0x0600f800), the UI tilemap cleared to 0 over them.
    if (stripDirty != 0) {
      stripDirty = 0;
      work->holdTilemapUpload = 1;
      if (mode != COMMAND_ROW_PARTY) {
        s32 panel;
        s32 count;
        for (panel = 0; panel < 6 - selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;
            for (col = 0; col <= 2; col++)
              *(work->tilemap + (entryIndex + 17) * 32 + panel * 3 + WINDOW_X -
                18 + (col & 3)) = WINDOW_TILE(0x7f);
          }
        }
        for (panel = 0; panel < selectionUi->iconCount; panel++) {
          count = selectionUi->iconCount;
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;
            for (col = 0; col <= 2; col++) {
              s32 tmp;
              // VRAM through a volatile pointer: a plain one moves the address
              // adds (18 of 1037 instructions differ in tbs-en).
              /* FAKEMATCH(temporary): vram, offset and mask keep the row
               * loop's index and tile pattern; as constants in place, 6 of
               * 1037 instructions differ in tbs-en. */
              volatile u16 *vram =
                  (volatile u16 *)0x0600f800 + 21 * 32 + WINDOW_X;
              s32 offset = 0x100;
              s32 mask = 3;
              /* FAKEMATCH(temporary): tmp, set twice, is no induction variable,
               * so the tile is recomputed in the column loop; as the two values
               * in place, 128 of 1037 instructions differ in tbs-en. */
              vram[panel * 3 + entryIndex * 32 + (col & mask) - count * 3] =
                  tmp = panel * 16 + entryIndex * 4 + col + offset;
              tmp = col & mask;
              *(work->tilemap + (entryIndex + 17) * 32 + panel * 3 + tmp -
                count * 3 + WINDOW_X) = 0;
            }
          }
        }
      } else {
        s32 panel;
        s32 count;
        for (panel = 0; panel < 6 - selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;
            for (col = 0; col <= 2; col++)
              *(work->tilemap + (entryIndex + 17) * 32 + panel * 3 + 2 +
                (col & 3)) = WINDOW_TILE(0x7f);
          }
        }
        for (panel = 0; panel < selectionUi->iconCount; panel++) {
          /* FAKEMATCH(one-pass): the exit test compares count instead of
           * reloading work->count; as no block, 99 of 1037 instructions
           * differ in tbs-en. */
          do {
          } while (0);
          count = selectionUi->iconCount;
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;
            for (col = 0; col <= 2; col++) {
              s32 tmp;
              // VRAM through a volatile pointer, as in the member's row (19 of
              // 1037 instructions differ with a plain one).
              /* FAKEMATCH(temporary): vram, offset and mask, as in the member's
               * row; as constants in place, 6 of 1037 instructions differ in
               * tbs-en. */
              volatile u16 *vram = (volatile u16 *)0x0600f800 + 21 * 32 + 20;
              s32 offset = 0x100;
              s32 mask = 3;
              /* FAKEMATCH(temporary): tmp, as in the member's row; as the two
               * values in place, 129 of 1037 instructions differ in tbs-en. */
              vram[panel * 3 + entryIndex * 32 + (col & mask) - count * 3] =
                  tmp = panel * 16 + entryIndex * 4 + col + offset;
              tmp = col & mask;
              *(work->tilemap + (entryIndex + 17) * 32 + panel * 3 + tmp -
                count * 3 + 20) = 0;
            }
          }
        }
      }
    }
    // The selection moved: name the new command and load its icon into the
    // sprite's entry.
    if (selectionUi->drawnIndex != selectionUi->selectedIndex) {
      WindowSprite_PrepareForRedraw(selectionUi->window);
      Text_DrawMessageAsTiles(
          selectionUi->entries[selectionUi->selectedIndex].kind +
              MsgCommandName,
          selectionUi->window, 0, 0);
      selectionUi->drawnIndex = selectionUi->selectedIndex;
      Text_DrawCharacter(selectionUi, selectionUi->selectedIndex,
                         selectionUi->commandKinds[selectionUi->selectedIndex]);
    }
    selectionUi->affineIndex =
        OamMatrix_AllocFromTransform(&selectionUi->affine);
    // Only the selected icon is drawn as a sprite: double-size affine (mode
    // 3), on layer 0xf1, moved with the pulse. The others are BG1 tiles.
    for (entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++) {
      struct SelectionEntry *entry = &selectionUi->entries[entryIndex];
      if (entryIndex == selectionUi->selectedIndex) {
        entry->sprite.oam.f.affineIndex = selectionUi->affineIndex;
        entry->sprite.oam.f.affine = 3;
        entry->sprite.oam.f.x = entry->positionX + pulseScale * 7 / 512 - 14;
        entry->sprite.oam.f.y = entry->positionY + pulseScale * 3 / 256 - 20;
        Oam_AddSpriteAtDepth(entry, 0xf1);
      } else {
        entry->sprite.oam.f.x = entry->positionX;
        entry->sprite.oam.f.y = entry->positionY;
        entry->sprite.oam.f.affineIndex = 0;
        entry->sprite.oam.f.affine = 0;
      }
    }
    // The Djinn tutorial presses keys for the player, one each delay (1 is A,
    // 16 Right, 32 Left). In the party's row it takes Fight. Unit 1 moves left
    // to Defend and takes it; unit 0 moves right to Summon, or to Djinn while
    // no Djinni is on standby, shows that command's hint at tutorial step 1
    // and takes it on the next press. Other members take Attack.
    keys = gKeysPressed;
    repeat = gKeysRepeat;
    navigation = gBattleMenuWork;
    if (navigation->tutorialStep != 0) {
      keys = repeat = 0;
      if (navigation->tutorialDelay == 0) {
        if (navigation->commandActor == 1) {
          if (selectionUi->commandKinds[selectionUi->selectedIndex] ==
              COMMAND_ICON_DEFEND)
            keys = repeat = 1;
          else
            keys = repeat = 32;
          navigation->tutorialDelay = 30;
        } else if (navigation->commandActor == 0) {
          s32 kind = selectionUi->commandKinds[selectionUi->selectedIndex];
          if (kind == COMMAND_ICON_SUMMON ||
              (tradeCount == 0 && kind == COMMAND_ICON_DJINN)) {
            if (navigation->tutorialStep == 1) {
              struct Window *message;
              if (kind == COMMAND_ICON_DJINN)
                message =
                    Message_ShowAndWait(MsgDjinnTutorialUnleashCommand, 15, 8);
              else if (kind == COMMAND_ICON_SUMMON)
                message = Message_ShowAndWait(MsgDjinnTutorialSummon, 15, 8);
              BattleMessage_WaitForAdvanceAtFar(102, 155);
              Window_Close(message, 1);
              navigation->tutorialStep++;
              navigation->tutorialDelay = 45;
            } else {
              keys = repeat = 1;
              navigation->tutorialDelay = 200;
            }
          } else {
            keys = repeat = 16;
            navigation->tutorialDelay = 40;
          }
        } else {
          navigation->tutorialDelay = 60;
          keys = repeat = 1;
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
    if (mode != COMMAND_ROW_PARTY && (keys & KEY_B)) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      command = -1;
      break;
    }
    if (repeat & (KEY_DOWN | KEY_RIGHT)) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      selectionUi->selectedIndex =
          (selectionUi->selectedIndex + 1) % selectionUi->iconCount;
    } else if (repeat & (KEY_UP | KEY_LEFT)) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      selectionUi->selectedIndex =
          (selectionUi->selectedIndex + selectionUi->iconCount - 1) %
          selectionUi->iconCount;
    } else if (gBattleMenuWork->timer == 0) {
      // The link battle's time is up.
      command = selectionUi->commandKinds[0];
      break;
    }
    // BG1 on while the row is open; the exit below turns it off again.
    IoWriteQueue_Store16((u32)&REG_DISPCNT,
                         DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                             DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON);
    work->holdTilemapUpload = 0;
    WaitFrames(1);
  }
  if (mode != COMMAND_ROW_PARTY)
    gBattleMenuWork->commandSelection[1] = selectionUi->selectedIndex;
  else
    gBattleMenuWork->commandSelection[0] = selectionUi->selectedIndex;
  // Every icon names the same entry, so it is released once per icon.
  for (entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++)
    VramCache_FreeEntry(selectionUi->entries[entryIndex].resource);
  work->holdTilemapUpload = 1;
  Window_Close(selectionUi->window, 1);
  // Cover the strip again: seven panels of WINDOW_TILE(0x7f) from column 5.
  {
    s32 panel;
    for (panel = 0; panel <= 6; panel++) {
      for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
        s32 col;
        /* FAKEMATCH(volatile): the add of 0x44a stays apart from the store it
         * addresses; as a plain store, 2 of 1037 instructions differ in
         * tbs-en. */
        for (col = 0; col <= 2; col++)
          *(volatile u16 *)(work->tilemap + (entryIndex + 17) * 32 + panel * 3 +
                            5 + (col & 3)) = WINDOW_TILE(0x7f);
      }
    }
  }
  // Restore the normal BG1 handler after clearing the command strip.
  work->dirty = 1;
  WaitFrames(1);
  Task_Remove(Graphics_SetBg1Priority3);
  Runtime_SetIrqHandler(2, 0, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
                      0x20000);
  Free(selectionUi);
  work->holdTilemapUpload = 0;
  WaitFrames(1);
  return command;
}

/// Switches the UI tilemap entries of the `width` x `height` tile rectangle
/// at screen tile (x, y) to palette 15 or 14 by bit 12, keeping the rest of
/// each entry: the menus' row highlight. `paletteBit` is the palette number,
/// of which only the low bit counts (15 plain, 14 highlighted). Clipped as
/// Window_SetTextTileBitRect clips; each row changed is marked for
/// upload.
void Window_HighlightRect(s32 x, s32 y, s32 width, s32 height, s32 paletteBit) {
  struct WindowWork *work = WINDOW_WORK;

  paletteBit &= 1;
  paletteBit <<= 12;

  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }

  if (width > 0 && height > 0) {
    do {
      u16 *cell = &((u16 (*)[32])work->tilemap)[y][x];
      s32 remaining = width;

      if (remaining != 0) {
        do {
          u32 tile = *cell;
          tile = (tile & ~0x1000) | paletteBit;
          remaining--;
          *cell = tile;
          cell++;
        } while (remaining != 0);
      }

      Window_MarkTilemapRow(work, y);
      height--;
      y++;
    } while (height != 0);
  }
}

/// Clears the highlight from the party status window's member columns, then
/// highlights the columns of the units in `ids` (up to four, ended by 0xff):
/// the member whose commands are being chosen. A unit not in the party is
/// skipped. The window is right-aligned on the 30-tile row, so its member
/// columns start at column 29 - 6 * members (the Djinn column, when shown,
/// lies left of them). Returns 0. Battle_CollectPartyCommands calls it.
s32 PartyStatus_HighlightMembers(u16 *ids) {
  struct BattleWork *battle = gBattleWork;
  s32 count;
  s32 i;
  s32 j;

  count = BattleUnit_ListIdsFar(1, 0);
  Window_HighlightRect(29 - count * 6, 0, 25, 5, 15);
  i = 0;
  // The loop over ids is written with goto (MATCHING.md): as
  // for (i = 0; i < 4 && ids[i] != 0xff; i++), or as a do-while loop inside
  // this test, 49 of 74 instructions differ in tbs-en.
  if (ids[0] != 0xff) {
  next_id:
    for (j = 0; j < 4 && battle->partyUnits[j] != ids[i]; j++) {
      if (battle->partyUnits[j] == 0xff) {
        j = 4;
        break;
      }
    }
    if (j != 4)
      Window_HighlightRect(29 - (count - j) * 6, 0, 7, 5, 14);
    if (++i < 4 && ids[i] != 0xff)
      goto next_id;
  }
  return 0;
}
