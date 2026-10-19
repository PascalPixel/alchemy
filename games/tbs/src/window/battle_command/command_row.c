// Part of battle_command.c, the menu object, compiled only through it.
// The row of command icons the player picks a command from.

#include "../battle_command_internal.h"

// Open a message at (x, y), wait for its text to finish, and return its window
// so the caller can hold it open for a separate prompt.
struct UiWindow *UiText_ShowMessageAndWaitComplete(s32 message, s32 x, s32 y) {
  struct UiWindow *result;

  result = UiText_OpenMessageWindow(message, x, y, 1);
  while (UiWork_IsComplete() == 0)
    WaitFrames(1);
  return result;
}

#if TBS_PATCH_1
#define WINDOW_X 22 /* Window X position */
#else
#define WINDOW_X 23
#endif

// Select a command icon, returning its kind, -1 for cancel or -2 for a
// shoulder-button switch. Mode 0 is the top menu; the other mode selects
// a battle command and remembers its own cursor. Tutorial input is replayed
// through the same selection path.
s32 Menu_RunCommandRow(s32 mode) {
  u8 party[4];
  struct UiRenderWork *render;
  struct SelectionWork *selectionUi;
  struct MenuCell *navigation;
  s32 stripDirty, tradeCount, pulseScale, command;
  s32 entryIndex;
  s32 keys, repeat;
  s32 iconSlot;
  s32 i, kindCount;

  render = UI_WORK;
  iconSlot = VramCache_AllocEntry(0x400);
  stripDirty = 1;
  tradeCount = 0;
  selectionUi = Runtime_BumpAllocateAlternatePool(sizeof(*selectionUi));
  for (i = 255; i >= 0; i--)
    selectionUi->palette.slot[i] = 255;
  selectionUi->palette.count = 1;
  if (mode == 0)
    selectionUi->window = UiWindow_Create(20, 17, 10, 3, 6);
  else
    selectionUi->window = UiWindow_Create(WINDOW_X, 17, 30 - WINDOW_X, 3, 6);
  selectionUi->drawnIndex = -1;
  if (mode == 0) {
    kindCount = 0;
    selectionUi->commandKinds[kindCount++] = 14;
    selectionUi->commandKinds[kindCount++] = 4;
    selectionUi->commandKinds[kindCount++] = 7;
    selectionUi->commandKinds[kindCount] = -1;
    selectionUi->selectedIndex = gLinkCountdownWork->commandSelection[0];
  } else {
    selectionUi->selectedIndex = gLinkCountdownWork->commandSelection[1];
    kindCount = 0;
    selectionUi->commandKinds[kindCount++] = 0;
    selectionUi->commandKinds[kindCount++] = 1;
    if (*((s32 *)Djinn_GetRecoveryTableFar(0)) != 0) {
      BattlePlacement_CountValidEntriesFar(0, party);
      for (i = 0; i <= 3; i++)
        tradeCount += party[i];
      selectionUi->commandKinds[kindCount++] = 15;
      if (tradeCount != 0)
        selectionUi->commandKinds[kindCount++] = 16;
    }
    selectionUi->commandKinds[kindCount++] = 2;
    selectionUi->commandKinds[kindCount++] = 3;
    selectionUi->commandKinds[kindCount] = -1;
  }
  // Decode each visible command through one shared remapped palette.
  for (entryIndex = 0; entryIndex <= 5; entryIndex++) {
    selectionUi->entries[entryIndex].resource = iconSlot;
    if (selectionUi->commandKinds[entryIndex] == -1)
      break;
    UiText_DrawCharacter(selectionUi, entryIndex,
                         selectionUi->commandKinds[entryIndex]);
  }
  selectionUi->iconCount = entryIndex;
  selectionUi->affine.scaleX = 320;
  selectionUi->affine.scaleY = 304;
  selectionUi->affine.angle = 0;
  entryIndex = 0;
  if (entryIndex < selectionUi->iconCount) {
    do {
      s32 x = (entryIndex - selectionUi->iconCount) * 24;
      selectionUi->entries[entryIndex].posX = x + 155;
      if (mode != 0)
        selectionUi->entries[entryIndex].posX = x + WINDOW_X * 8 - 5;
      selectionUi->entries[entryIndex].posY = 136;
      entryIndex++;
    } while (entryIndex < selectionUi->iconCount);
  }
  Scheduler_AddOrUpdateCallback(Graphics_SetBg1Priority3, 0x480);
  Runtime_SetIrqHandler(2, 136, Graphics_SetBg1Priority0);

  for (;;) {
    pulseScale = gUi_BounceScales[(gFrameCount * 2) & 31];
    pulseScale = (pulseScale - 256) / 4 + 304;
    selectionUi->affine.scaleY = selectionUi->affine.scaleX = pulseScale;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 32;
    if (stripDirty != 0) {
      stripDirty = 0;
      render->menuBusy = 1;
      if (mode) {
        s32 panel;
        s32 count;
        for (panel = 0; panel < 6 - selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;
            for (col = 0; col <= 2; col++)
              *(render->tilemap + (entryIndex + 17) * 32 + panel * 3 +
                WINDOW_X - 18 + (col & 3)) = UI_TILE(0x7f);
          }
        }
        for (panel = 0; panel < selectionUi->iconCount; panel++) {
          count = selectionUi->iconCount;
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;
            for (col = 0; col <= 2; col++) {
              s32 tmp;
              /* FAKEMATCH: vram, offset and mask restore two forms the bytes
               * prove. Set ahead of the index, they are three more moves out of
               * the column loop in loop.c's first pass, which drops its
               * threshold so the tile pattern's row part stays in the column
               * loop until the second pass: the row loop reduces the index giv
               * first and copies it first (inits index r6, pattern r5; per row
               * mov ip, r6 and str r5, [sp, #4]). volatile keeps the VRAM base
               * add apart from the store it addresses. Without the three 24,
               * without offset 564, without mask 470, without volatile 112. */
              volatile u16 *vram =
                  (volatile u16 *)0x0600f800 + 21 * 32 + WINDOW_X;
              s32 offset = 0x100;
              s32 mask = 3;
              /* FAKEMATCH: needed to match: tmp, set twice (the tile, then the
               * column's index), is no giv, so the tile is recomputed in the
               * column loop as the bytes show (adds r3, r7, r0; adds r3, r3,
               * r7); 267 differ without it, 846 in both modes. */
              vram[panel * 3 + entryIndex * 32 + (col & mask) - count * 3] =
                  tmp = panel * 16 + entryIndex * 4 + col + offset;
              tmp = col & mask;
              *(render->tilemap + (entryIndex + 17) * 32 + panel * 3 + tmp -
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
              *(render->tilemap + (entryIndex + 17) * 32 + panel * 3 + 2 +
                (col & 3)) = UI_TILE(0x7f);
          }
        }
        for (panel = 0; panel < selectionUi->iconCount; panel++) {
          /* FAKEMATCH(one-pass): an empty block restores the CSE block
           * boundary at the panel body's head, so the exit test compares count
           * instead of reloading work->count; as no block, 99 of 1037
           * instructions differ in tbs-en. */
          do {
          } while (0);
          count = selectionUi->iconCount;
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;
            for (col = 0; col <= 2; col++) {
              s32 tmp;
              /* FAKEMATCH: as in mode 1: vram, offset and mask keep the tile
               * pattern in the column loop through loop.c's first pass,
               * volatile keeps the VRAM add apart from its store. */
              volatile u16 *vram = (volatile u16 *)0x0600f800 + 21 * 32 + 20;
              s32 offset = 0x100;
              s32 mask = 3;
              /* FAKEMATCH: needed to match: tmp, as in mode 1 (271 differ
               * without it, 846 in both modes). */
              vram[panel * 3 + entryIndex * 32 + (col & mask) - count * 3] =
                  tmp = panel * 16 + entryIndex * 4 + col + offset;
              tmp = col & mask;
              *(render->tilemap + (entryIndex + 17) * 32 + panel * 3 + tmp -
                count * 3 + 20) = 0;
            }
          }
        }
      }
    }
    if (selectionUi->drawnIndex != selectionUi->selectedIndex) {
      RenderOutput_PrepareForRedraw(selectionUi->window);
      UiText_DrawCharacterAtOffset(
          selectionUi->entries[selectionUi->selectedIndex].kind +
              MsgCommandName,
          selectionUi->window, 0, 0);
      selectionUi->drawnIndex = selectionUi->selectedIndex;
      UiText_DrawCharacter(
          selectionUi, selectionUi->selectedIndex,
          selectionUi->commandKinds[selectionUi->selectedIndex]);
    }
    selectionUi->affineIndex =
        AffineMatrix_BuildForEffect(&selectionUi->affine);
    for (entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++) {
      struct SelectionEntry *entry = &selectionUi->entries[entryIndex];
      if (entryIndex == selectionUi->selectedIndex) {
        entry->sprite.oam.f.affineIndex = selectionUi->affineIndex;
        entry->sprite.oam.f.affine = 3;
        entry->sprite.oam.f.x = entry->posX + pulseScale * 7 / 512 - 14;
        entry->sprite.oam.f.y = entry->posY + pulseScale * 3 / 256 - 20;
        Runtime_PushSlotEntry(entry, 0xf1);
      } else {
        entry->sprite.oam.f.x = entry->posX;
        entry->sprite.oam.f.y = entry->posY;
        entry->sprite.oam.f.affineIndex = 0;
        entry->sprite.oam.f.affine = 0;
      }
    }
    // The tutorial drives the same command selection using synthetic keys.
    keys = gKeysPressed;
    repeat = gKeysRepeat;
    navigation = gLinkCountdownWork;
    if (navigation->autoEnabled != 0) {
      keys = repeat = 0;
      if (navigation->autoDelay == 0) {
        if (navigation->autoMode == 1) {
          if (selectionUi->commandKinds[selectionUi->selectedIndex] == 3)
            keys = repeat = 1;
          else
            keys = repeat = 32;
          navigation->autoDelay = 30;
        } else if (navigation->autoMode == 0) {
          s32 kind = selectionUi->commandKinds[selectionUi->selectedIndex];
          if (kind == 16 || (tradeCount == 0 && kind == 15)) {
            if (navigation->autoEnabled == 1) {
              struct UiWindow *message;
              if (kind == 15)
                message = UiText_ShowMessageAndWaitComplete(
                    MsgDjinnTutorialUnleashCommand, 15, 8);
              else if (kind == 16)
                message = UiText_ShowMessageAndWaitComplete(
                    MsgDjinnTutorialSummon, 15, 8);
              BattlePresentation_WaitForPromptAtFar(102, 155);
              UiWork_Finalize(message, 1);
              navigation->autoEnabled++;
              navigation->autoDelay = 45;
            } else {
              keys = repeat = 1;
              navigation->autoDelay = 200;
            }
          } else {
            keys = repeat = 16;
            navigation->autoDelay = 40;
          }
        } else {
          navigation->autoDelay = 60;
          keys = repeat = 1;
        }
      } else {
        navigation->autoDelay--;
      }
    }
    if ((keys & KEYS_SHOULDERS) && mode != 0) {
      command = -2;
      break;
    }
    if (keys & KEY_A) {
      command = selectionUi->commandKinds[selectionUi->selectedIndex];
      break;
    }
    if (mode != 0 && (keys & KEY_B)) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      command = -1;
      break;
    }
    if (repeat & (KEY_DOWN | KEY_RIGHT)) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selectionUi->selectedIndex =
          (selectionUi->selectedIndex + 1) % selectionUi->iconCount;
    } else if (repeat & (KEY_UP | KEY_LEFT)) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selectionUi->selectedIndex =
          (selectionUi->selectedIndex + selectionUi->iconCount - 1) %
          selectionUi->iconCount;
    } else if (gLinkCountdownWork->active == 0) {
      command = selectionUi->commandKinds[0];
      break;
    }
    QueueIoWriteDelay2((u32)&REG_DISPCNT, DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP |
                                              DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                                              DISPCNT_BG2_ON | DISPCNT_OBJ_ON);
    render->menuBusy = 0;
    WaitFrames(1);
  }
  if (mode != 0)
    gLinkCountdownWork->commandSelection[1] = selectionUi->selectedIndex;
  else
    gLinkCountdownWork->commandSelection[0] = selectionUi->selectedIndex;
  for (entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++)
    Resource_ResetEntry(selectionUi->entries[entryIndex].resource);
  render->menuBusy = 1;
  UiWork_Finalize(selectionUi->window, 1);
  {
    s32 panel;
    for (panel = 0; panel <= 6; panel++) {
      for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
        s32 col;
        /* FAKEMATCH(volatile): the add of 0x44a stays apart from the store it
         * addresses (adds r3, r7; strh r6, [r3]); as a plain store, combine
         * folds it into strh r6, [r3, r7] and 2 of 1037 instructions differ
         * in tbs-en. */
        for (col = 0; col <= 2; col++)
          *(volatile u16 *)(render->tilemap + (entryIndex + 17) * 32 +
                            panel * 3 + 5 + (col & 3)) = UI_TILE(0x7f);
      }
    }
  }
  // Restore the normal BG1 handler after clearing the command strip.
  render->dirty = 1;
  WaitFrames(1);
  Scheduler_RemoveCallback(Graphics_SetBg1Priority3);
  Runtime_SetIrqHandler(2, 0, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
                      0x20000);
  MemFree(selectionUi);
  render->menuBusy = 0;
  WaitFrames(1);
  return command;
}

// Set bit 12 of each tile in a screen-space rectangle, preserving the other
// tile attributes. Keep the original bottom-edge test (29) and height (20).
void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 paletteBit) {
  struct UiRenderWork *render = UI_WORK;

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
      u16 *cell = &((u16 (*)[32])render->tilemap)[y][x];
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

      UiRender_MarkRow(render, y);
      height--;
      y++;
    } while (height != 0);
  }
}

// Dim the party strip, then highlight the actor IDs up to the 0xff terminator.
// Missing IDs are ignored; the search stops at the party's own terminator.
s32 BattleLayout_HighlightPartyPanels(u16 *ids) {
  struct BattleWork *battle = gBattleWork;
  s32 count;
  s32 i;
  s32 j;

  count = BattleParty_ListActorIdsFar(1, 0);
  Ui_SetRectHighlight(29 - count * 6, 0, 25, 5, 15);
  i = 0;
  if (ids[0] != 0xff) {
  next_id:
    for (j = 0; j < 4 && battle->partyUnits[j] != ids[i]; j++) {
      if (battle->partyUnits[j] == 0xff) {
        j = 4;
        break;
      }
    }
    if (j != 4)
      Ui_SetRectHighlight(29 - (count - j) * 6, 0, 7, 5, 14);
    if (++i < 4 && ids[i] != 0xff)
      goto next_id;
  }
  return 0;
}
