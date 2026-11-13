/// @file
/// The row of command icons the player picks a command from.
// Part of window.c.

#include "../screens_internal.h"
#include "module/window/screens.h"

/* VIEW(screens): variable gWindowSprite_BounceScales as const u16
 * gWindowSprite_BounceScales[32]; window/window has u16
 * gWindowSprite_BounceScales[]; not tried: gWindowSprite_BounceScales's
 * declaration holds a FAKEMATCH tag. */
extern const u16 gWindowSprite_BounceScales__screens[32] __asm__(
    "gWindowSprite_BounceScales");
#define gWindowSprite_BounceScales gWindowSprite_BounceScales__screens

struct WindowTiles {
  u8 unk00[3];
  u8 dirty; ///< bits: one per four tile rows, from bit 1
  u8 unk04[4];
  u16 cells[32][32];
};

s32 Message_ShowAndWait(s32 message, s32 x, s32 y) {
  s32 result;

  result = Message_OpenWindow(message, x, y, 1);
  while (Message_AllTyped() == 0)
    wait(1);
  return result;
}

// The tile column where a party member's command row ends and its name window
// begins.
#if LANGUAGE != LANGUAGE_JAPANESE
#define WINDOW_X 22
#else
#define WINDOW_X 23
#endif

#define NAVIGATION ((struct CommandRowNavigation *)Ram_HeapSlots->unke4[0])
#define WINDOW_WORK ((struct CommandRowWindowWork *)Ram_HeapSlots->windowTiles)

/* VIEW(screens): variable gBgScroll as struct BgScrollPair gBgScroll[];
 * bg_scroll.h has struct BgScroll gBgScroll[]; not tried: gBgScroll's
 * declaration holds a FAKEMATCH tag. */
extern struct BgScrollPair gBgScroll__screens[] __asm__("gBgScroll");
#include "main_routine_work.h"

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
/// (BattleSummonState.availableMask), Summon while any Djinni is on standby,
/// Item and Defend. Each row remembers its choice in
/// BattleMenuWork.commandSelection. While the Djinn tutorial runs, the row
/// presses the keys for the player.
s32 BattleCommandRow_Run(s32 mode, s32 fleeAllowed) {
  u8 party[4];
  struct CommandRowWindowWork *work;
  struct CommandRowWork *selectionUi;
  struct CommandRowNavigation *battleMenu;
  struct CommandRowNavigation *navigation;
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
  iconSlot = VramCache_AllocEntry(0x400);
  stripDirty = 1;
  tradeCount = 0;
  battleMenu = NAVIGATION;
  selectionUi = Heap_AllocEwram(sizeof(*selectionUi));
  for (i = 255; i >= 0; i--)
    selectionUi->paletteSlots[i] = 255;
  selectionUi->paletteCount = 1;
  if (mode == 0)
    selectionUi->window = Window_Create(20, 17, 10, 3, 6);
  else
    selectionUi->window = Window_Create(WINDOW_X, 17, 30 - WINDOW_X, 3, 6);
  selectionUi->drawnIndex = -1;
  if (mode == 0) {
    kindCount = 0;
    selectionUi->commandKinds[kindCount++] = 14;
    if (((u8 *)Ram_HeapSlots->battleWork)[0x44] == 0 &&
        battleMenu->reserveChoice == -1 && Party_CountMembersFar() > 4)
      selectionUi->commandKinds[kindCount++] = 49;
    if (fleeAllowed > 0 && gMainRoutineWork.battleRule24b != 2)
      selectionUi->commandKinds[kindCount++] = 4;
    selectionUi->commandKinds[kindCount++] = 7;
    selectionUi->commandKinds[kindCount] = -1;
    selectionUi->selectedIndex = NAVIGATION->commandSelection[0];
  } else {
    s32 memberCount;

    selectionUi->selectedIndex = NAVIGATION->commandSelection[1];
    memberCount = 0;
    selectionUi->commandKinds[memberCount++] = 0;
    selectionUi->commandKinds[memberCount++] = 1;
    if (*(s32 *)Djinn_GetRecoveryTableFar(0) != 0) {
      Func_08118130__screens(0, party);
      for (i = 0; i <= 3; i++)
        tradeCount += party[i];
      selectionUi->commandKinds[memberCount++] = 15;
      if (tradeCount != 0)
        selectionUi->commandKinds[memberCount++] = 16;
    }
    selectionUi->commandKinds[memberCount++] = 2;
    selectionUi->commandKinds[memberCount++] = 3;
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
  selectionUi->affine.scaleX = 320;
  selectionUi->affine.scaleY = 304;
  selectionUi->affine.angle = 0;
  entryIndex = 0;
  if (entryIndex < selectionUi->iconCount) {
    do {
      s32 x = (entryIndex - selectionUi->iconCount) * 24;

      selectionUi->entries[entryIndex].positionX = x + 155;
      if (mode != 0)
        selectionUi->entries[entryIndex].positionX = x + WINDOW_X * 8 - 5;
      selectionUi->entries[entryIndex].positionY = 136;
      entryIndex++;
    } while (entryIndex < selectionUi->iconCount);
  }
  Task_Add(CommandRow_PutBg1Behind, 0xc80);
  Irq_SetHandler(2, 136, CommandRow_PutBg1InFront);

  for (;;) {
    pulseScale = gWindowSprite_BounceScales[(gFrameCount * 2) & 31];
    pulseScale = (pulseScale - 256) / 4 + 304;
    selectionUi->affine.scaleY = selectionUi->affine.scaleX = pulseScale;
    gBgScroll__screens[1].x = 0;
    gBgScroll__screens[1].y = 32;
    if (stripDirty != 0) {
      stripDirty = 0;
      work->holdTilemapUpload = 1;
      if (mode != 0) {
        s32 panel;

        for (panel = 0; panel < 6 - selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;

            for (col = 0; col <= 2; col++)
              *(work->tilemap + (entryIndex + 17) * 32 + panel * 3 + WINDOW_X -
                18 + (col & 3)) = 0xf07f;
          }
        }
        for (panel = 0; panel < selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;

            for (col = 0; col <= 2; col++) {
              volatile u16 *vram =
                  (volatile u16 *)0x0600f800 + 21 * 32 + WINDOW_X;

              vram[panel * 3 + entryIndex * 32 + (col & 3) -
                   selectionUi->iconCount * 3] =
                  panel * 16 + entryIndex * 4 + col + 0x100;
              *(work->tilemap + (entryIndex + 17) * 32 + panel * 3 + (col & 3) -
                selectionUi->iconCount * 3 + WINDOW_X) = 0;
            }
          }
        }
      } else {
        s32 panel;

        for (panel = 0; panel < 6 - selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;

            for (col = 0; col <= 2; col++)
              *(work->tilemap + (entryIndex + 17) * 32 + panel * 3 + 2 +
                (col & 3)) = 0xf07f;
          }
        }
        for (panel = 0; panel < selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;

            for (col = 0; col <= 2; col++) {
              volatile u16 *vram = (volatile u16 *)0x0600f800 + 21 * 32 + 20;

              vram[panel * 3 + entryIndex * 32 + (col & 3) -
                   selectionUi->iconCount * 3] =
                  panel * 16 + entryIndex * 4 + col + 0x100;
              *(work->tilemap + (entryIndex + 17) * 32 + panel * 3 + (col & 3) -
                selectionUi->iconCount * 3 + 20) = 0;
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
      struct CommandRowIcon *entry = &selectionUi->entries[entryIndex];

      if (entryIndex == selectionUi->selectedIndex) {
        entry->affineIndex = selectionUi->affineIndex;
        entry->objMode = 3;
        entry->x = entry->positionX + pulseScale * 7 / 512 - 14;
        entry->y = entry->positionY + pulseScale * 3 / 256 - 20;
        Oam_AddSpriteAtDepth(entry, 241);
      } else {
        entry->x = entry->positionX;
        entry->y = entry->positionY;
        entry->affineIndex = 0;
        entry->objMode = 0;
      }
    }

    keys = gInput.pressed;
    repeat = gInput.repeat;
    navigation = NAVIGATION;
    if (navigation->tutorialStep != 0) {
      keys = repeat = 0;
      if (navigation->tutorialDelay == 0) {
        if ((u32)(navigation->commandActor - 5) <= 1) {
          if (selectionUi->commandKinds[selectionUi->selectedIndex] == 3)
            keys = repeat = 1;
          else
            keys = repeat = 32;
          navigation->tutorialDelay = 30;
        } else if (navigation->commandActor == 4) {
          s32 kind = selectionUi->commandKinds[selectionUi->selectedIndex];

          if (kind == 16 || (tradeCount == 0 && kind == 15)) {
            if (navigation->tutorialStep == 1) {
              void *message;

              ((u8 *)Ram_HeapSlots->windowTiles)[5] |= 4;
              if (selectionUi->commandKinds[selectionUi->selectedIndex] == 15)
                message = Message_ShowAndWait(
                    Djinn_GetGiveTutorialMessage() + 10, 15, 8);
              else if (selectionUi->commandKinds[selectionUi->selectedIndex] ==
                       16)
                message = Message_ShowAndWait(
                    Djinn_GetGiveTutorialMessage() + 9, 15, 8);
              ((u8 *)Ram_HeapSlots->windowTiles)[5] &= ~4;
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
    if ((keys & 0x300) && mode != 0) {
      command = -2;
      break;
    }
    if (keys & 1) {
      command = selectionUi->commandKinds[selectionUi->selectedIndex];
      break;
    }
    if (keys & 2) {
      Sound_PlayFar(113);
      command = -1;
      break;
    }
    if (repeat & 0x90) {
      Sound_PlayFar(111);
      selectionUi->selectedIndex =
          (selectionUi->selectedIndex + 1) % selectionUi->iconCount;
    } else if (repeat & 0x60) {
      Sound_PlayFar(111);
      selectionUi->selectedIndex =
          (selectionUi->selectedIndex + selectionUi->iconCount - 1) %
          selectionUi->iconCount;
    } else if (NAVIGATION->timer == 0) {
      command = selectionUi->commandKinds[0];
      break;
    }
    IoWriteQueue_Store16(0x04000000, 0x1741);
    work->holdTilemapUpload = 0;
    wait(1);
  }
  if (mode != 0)
    NAVIGATION->commandSelection[1] = selectionUi->selectedIndex;
  else
    NAVIGATION->commandSelection[0] = selectionUi->selectedIndex;
  for (entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++)
    VramCache_FreeEntry(selectionUi->entries[entryIndex].resource);
  work->holdTilemapUpload = 1;
  Window_Close(selectionUi->window, 1);
  {
    s32 panel;

    for (panel = 0; panel <= 6; panel++) {
      for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
        s32 col;

        // The add of 0x452 stays apart from the store it addresses; as a plain
        // store, 11 of 1119 instructions differ in tla-en.
        for (col = 0; col <= 2; col++)
          *(volatile u16 *)(work->tilemap + (entryIndex + 17) * 32 + panel * 3 +
                            5 + (col & 3)) = 0xf07f;
      }
    }
  }
  work->dirty = 1;
  wait(1);
  Task_Remove(CommandRow_PutBg1Behind);
  Irq_SetHandler(2, 0, 0);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x1541, 0x20000);
  Heap_Release(selectionUi);
  work->holdTilemapUpload = 0;
  wait(1);
  return command;
}

void Window_HighlightRect(s32 x, s32 y, s32 width, s32 height, s32 alt) {
  struct WindowTiles *tiles = (struct WindowTiles *)Ram_HeapSlots->windowTiles;

  alt &= 1;
  alt <<= 12;

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
      u16 *p = &tiles->cells[y][x];
      s32 col = width;

      if (col != 0) {
        do {
          u32 tile = *p;
          col--;
          tile = (tile & ~0x1000) | alt;
          *p = tile;
          p++;
        } while (col != 0);
      }
      // The dirty-row mark in a once-only block keeps it before the row count's
      // step; as the plain store, 2 of 72 instructions differ in tla-en.
      do {
        tiles->dirty |= 2 << ((u32)y >> 2);
      } while (0);
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

  count = BattleParty_ListActorIdsFar(1, 0);
  Window_HighlightRect(29 - count * 6, 0, 25, 5, 15);
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
      Window_HighlightRect(29 - (count - j) * 6, 0, 7, 5, 14);
    if (++i < 4 && ids[i] != 0xff)
      goto next_id;
  }
  return 0;
}

#undef WINDOW_X
#undef NAVIGATION
#undef WINDOW_WORK
