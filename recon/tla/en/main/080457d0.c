// Draft, differ 906 of 1119: the battle command row, The Broken Seal's
// BattleCommandRow_Run (games/tbs/src/window/screens/command_row.c) on this
// game's work and command numbers. The party's row (mode 0) is Fight (14),
// command 49 (when the battle allows it, no reserve member is chosen and
// more than four are active), Flee (4, when flee is passed above 0 and the
// battle rule is not 2) and Status (7); a member's row Attack (0),
// Psynergy (1), Djinn (15) and Summon (16) as in The Broken Seal, Item (2)
// and Defend (3). B cancels in both rows. The tutorial's actors are 4 to 6.
// The strip loops carry The Broken Seal's devices. Left: the native keeps
// the work block in r9 and reads the navigation work early into r5; the
// draft takes sl and reads it late (reading it early, differ 1027), and
// the allocation shifts from there.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "system/io_write_queue.h"

extern u8 MsgCommandName[];

struct CommandRowIcon {
  u8 unk00[4];
  u8 y;
  u8 objMode : 2; ///< 3: double-size affine
  u8 high05 : 6;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u8 unk08[4];
  u16 resource;
  u16 unused;
  s32 positionX;
  s32 positionY;
  s32 kind;
};

struct CommandRowAffine {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
};

struct CommandRowWork {
  u8 paletteSlots[256];
  s32 paletteCount;
  struct CommandRowIcon entries[6];
  void *window;
  s32 iconCount;
  struct CommandRowAffine affine;
  s32 affineIndex;
  s32 selectedIndex;
  s32 drawnIndex;
  s32 commandKinds[6];
};

/// The battle menu's navigation work (heap slot 57).
struct CommandRowNavigation {
  u8 unk00[0x3c];
  s32 commandSelection[2];
  u8 unk44[8];
  s32 timer; ///< the link battle's time left; 0 takes the first command
  u8 unk50[0x88];
  s32 tutorialStep;
  s32 tutorialDelay;
  s32 commandActor;
  u8 unke4[0x20];
  s32 reserveChoice; ///< -1 while no reserve member is chosen
};

struct CommandRowWindowWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04;
  u8 flags05;
  u8 holdTilemapUpload;
  u8 unk07;
  u16 tilemap[0x400];
};

struct BgScrollPair {
  s16 x;
  s16 y;
};

#define NAVIGATION ((struct CommandRowNavigation *)Ram_HeapSlots->unke4[0])
#define WINDOW_WORK ((struct CommandRowWindowWork *)Ram_HeapSlots->windowTiles)

extern const u16 gUi_BounceScales[32];
extern struct BgScrollPair gBgScroll[];
extern u8 gAdventure[];

s32 VramCache_AllocEntry(s32 size);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
s32 Party_CountActiveOwnersFar(void);
s32 *Djinn_GetRecoveryTableFar(s32 kind);
void Func_08118130(s32 kind, u8 *counts);
void Func_080456f8(struct CommandRowWork *work, s32 index, s32 kind);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_SetIrqHandler(s32 irq, s32 line, void (*handler)(void));
void Graphics_SetBg1Priority3(void);
void Func_08045780(void);
void RenderOutput_PrepareForRedraw(void *window);
void UiText_DrawCharacterAtOffset(s32 message, void *window, s32 x, s32 y);
s32 AffineMatrix_BuildForEffect(struct CommandRowAffine *affine);
void Runtime_PushSlotEntry(void *sprite, s32 depth);
s32 Func_0804519c(void);
void *UiText_ShowMessageAndWaitComplete(s32 message, s32 x, s32 y);
void Func_08118128(s32 x, s32 y);
void UiWork_Finalize(void *window, s32 mode);
void Audio_PlayCue(s32 cue);
void QueueIoWriteDelay2(u32 address, u32 value);
void WaitFrames(s32 frames);
void Resource_ResetEntry(s32 resource);
void MemFree(void *block);

s32 Func_080457d0(s32 mode, s32 fleeAllowed) {
  u8 party[4];
  struct CommandRowWindowWork *work;
  struct CommandRowWork *selectionUi;
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
  void *message;

  work = WINDOW_WORK;
  iconSlot = VramCache_AllocEntry(0x400);
  tradeCount = 0;
  stripDirty = 1;
  selectionUi = Runtime_BumpAllocateAlternatePool(sizeof(*selectionUi));
  for (i = 255; i >= 0; i--)
    selectionUi->paletteSlots[i] = 255;
  selectionUi->paletteCount = 1;
  if (mode == 0)
    selectionUi->window = UiWindow_Create(20, 17, 10, 3, 6);
  else
    selectionUi->window = UiWindow_Create(22, 17, 8, 3, 6);
  selectionUi->drawnIndex = -1;
  if (mode == 0) {
    selectionUi->commandKinds[0] = 14;
    kindCount = 1;
    if (((u8 *)Ram_HeapSlots->battleWork)[0x44] == 0 &&
        NAVIGATION->reserveChoice == -1 && Party_CountActiveOwnersFar() > 4) {
      selectionUi->commandKinds[1] = 49;
      kindCount = 2;
    }
    if (fleeAllowed > 0 && gAdventure[0x24b] != 2)
      selectionUi->commandKinds[kindCount++] = 4;
    selectionUi->commandKinds[kindCount++] = 7;
    selectionUi->commandKinds[kindCount] = -1;
    selectionUi->selectedIndex = NAVIGATION->commandSelection[0];
  } else {
    selectionUi->selectedIndex = NAVIGATION->commandSelection[1];
    selectionUi->commandKinds[0] = 0;
    selectionUi->commandKinds[1] = 1;
    kindCount = 2;
    if (*Djinn_GetRecoveryTableFar(0) != 0) {
      Func_08118130(0, party);
      for (i = 0; i <= 3; i++)
        tradeCount += party[i];
      selectionUi->commandKinds[2] = 15;
      kindCount = 3;
      if (tradeCount != 0) {
        selectionUi->commandKinds[3] = 16;
        kindCount = 4;
      }
    }
    selectionUi->commandKinds[kindCount++] = 2;
    selectionUi->commandKinds[kindCount++] = 3;
    selectionUi->commandKinds[kindCount] = -1;
  }
  for (entryIndex = 0; entryIndex <= 5; entryIndex++) {
    selectionUi->entries[entryIndex].resource = iconSlot;
    if (selectionUi->commandKinds[entryIndex] == -1)
      break;
    Func_080456f8(selectionUi, entryIndex,
                  selectionUi->commandKinds[entryIndex]);
  }
  selectionUi->iconCount = entryIndex;
  selectionUi->affine.scaleX = 320;
  selectionUi->affine.scaleY = 304;
  selectionUi->affine.angle = 0;
  for (entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++) {
    s32 x = (entryIndex - selectionUi->iconCount) * 24;

    selectionUi->entries[entryIndex].positionX = x + 155;
    if (mode != 0)
      selectionUi->entries[entryIndex].positionX = x + 171;
    selectionUi->entries[entryIndex].positionY = 136;
  }
  Scheduler_AddOrUpdateCallback(Graphics_SetBg1Priority3, 0xc80);
  Runtime_SetIrqHandler(2, 136, Func_08045780);

  for (;;) {
    pulseScale = gUi_BounceScales[(gFrameCount * 2) & 31];
    pulseScale = (pulseScale - 256) / 4 + 304;
    selectionUi->affine.scaleY = selectionUi->affine.scaleX = pulseScale;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 32;
    if (stripDirty != 0) {
      stripDirty = 0;
      work->holdTilemapUpload = 1;
      if (mode != 0) {
        s32 panel;
        s32 count;

        for (panel = 0; panel < 6 - selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;

            for (col = 0; col <= 2; col++)
              work->tilemap[(entryIndex + 17) * 32 + panel * 3 + 4 +
                            (col & 3)] = 0xf07f;
          }
        }
        for (panel = 0; panel < selectionUi->iconCount; panel++) {
          count = selectionUi->iconCount;
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;

            for (col = 0; col <= 2; col++) {
              s32 tmp;
              /* FAKEMATCH(temporary): vram, offset, mask and tmp as in The
               * Broken Seal. */
              volatile u16 *vram = (volatile u16 *)0x0600f800 + 21 * 32 + 22;
              s32 offset = 0x100;
              s32 mask = 3;

              vram[panel * 3 + entryIndex * 32 + (col & mask) - count * 3] =
                  tmp = panel * 16 + entryIndex * 4 + col + offset;
              tmp = col & mask;
              work->tilemap[(entryIndex + 17) * 32 + panel * 3 + tmp -
                            count * 3 + 22] = 0;
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
              work->tilemap[(entryIndex + 17) * 32 + panel * 3 + 2 +
                            (col & 3)] = 0xf07f;
          }
        }
        for (panel = 0; panel < selectionUi->iconCount; panel++) {
          /* FAKEMATCH(one-pass): as in The Broken Seal. */
          do {
          } while (0);
          count = selectionUi->iconCount;
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;

            for (col = 0; col <= 2; col++) {
              s32 tmp;
              /* FAKEMATCH(temporary): vram, offset, mask and tmp as in The
               * Broken Seal. */
              volatile u16 *vram = (volatile u16 *)0x0600f800 + 21 * 32 + 20;
              s32 offset = 0x100;
              s32 mask = 3;

              vram[panel * 3 + entryIndex * 32 + (col & mask) - count * 3] =
                  tmp = panel * 16 + entryIndex * 4 + col + offset;
              tmp = col & mask;
              work->tilemap[(entryIndex + 17) * 32 + panel * 3 + tmp -
                            count * 3 + 20] = 0;
            }
          }
        }
      }
    }
    if (selectionUi->drawnIndex != selectionUi->selectedIndex) {
      RenderOutput_PrepareForRedraw(selectionUi->window);
      UiText_DrawCharacterAtOffset(
          selectionUi->entries[selectionUi->selectedIndex].kind +
              (s32)MsgCommandName,
          selectionUi->window, 0, 0);
      selectionUi->drawnIndex = selectionUi->selectedIndex;
      Func_080456f8(selectionUi, selectionUi->selectedIndex,
                    selectionUi->commandKinds[selectionUi->selectedIndex]);
    }
    selectionUi->affineIndex =
        AffineMatrix_BuildForEffect(&selectionUi->affine);
    for (entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++) {
      struct CommandRowIcon *entry = &selectionUi->entries[entryIndex];

      if (entryIndex == selectionUi->selectedIndex) {
        entry->affineIndex = selectionUi->affineIndex;
        entry->objMode = 3;
        entry->x = entry->positionX + pulseScale * 7 / 512 - 14;
        entry->y = entry->positionY + pulseScale * 3 / 256 - 20;
        Runtime_PushSlotEntry(entry, 241);
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
              work->flags05 |= 4;
              if (selectionUi->commandKinds[selectionUi->selectedIndex] == 15)
                message = UiText_ShowMessageAndWaitComplete(
                    Func_0804519c() + 10, 15, 8);
              else if (selectionUi->commandKinds[selectionUi->selectedIndex] ==
                       16)
                message = UiText_ShowMessageAndWaitComplete(Func_0804519c() + 9,
                                                            15, 8);
              WINDOW_WORK->flags05 &= ~4;
              Func_08118128(102, 155);
              UiWork_Finalize(message, 1);
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
      Audio_PlayCue(113);
      command = -1;
      break;
    }
    if (repeat & 0x90) {
      Audio_PlayCue(111);
      selectionUi->selectedIndex =
          (selectionUi->selectedIndex + 1) % selectionUi->iconCount;
    } else if (repeat & 0x60) {
      Audio_PlayCue(111);
      selectionUi->selectedIndex =
          (selectionUi->selectedIndex + selectionUi->iconCount - 1) %
          selectionUi->iconCount;
    } else if (NAVIGATION->timer == 0) {
      command = selectionUi->commandKinds[0];
      break;
    }
    QueueIoWriteDelay2(0x04000000, 0x1741);
    work->holdTilemapUpload = 0;
    WaitFrames(1);
  }
  if (mode != 0)
    NAVIGATION->commandSelection[1] = selectionUi->selectedIndex;
  else
    NAVIGATION->commandSelection[0] = selectionUi->selectedIndex;
  for (entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++)
    Resource_ResetEntry(selectionUi->entries[entryIndex].resource);
  work->holdTilemapUpload = 1;
  UiWork_Finalize(selectionUi->window, 1);
  {
    s32 panel;

    for (panel = 0; panel <= 6; panel++) {
      for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
        s32 col;

        for (col = 0; col <= 2; col++)
          work->tilemap[(entryIndex + 17) * 32 + panel * 3 + 5 + (col & 3)] =
              0xf07f;
      }
    }
  }
  work->dirty = 1;
  WaitFrames(1);
  Scheduler_RemoveCallback(Graphics_SetBg1Priority3);
  Runtime_SetIrqHandler(2, 0, 0);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x1541, 0x20000);
  MemFree(selectionUi);
  work->holdTilemapUpload = 0;
  WaitFrames(1);
  return command;
}
