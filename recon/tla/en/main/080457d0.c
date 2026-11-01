// Draft, differ 17 of 1119, skeleton gap 9 (tla-en, 2026-11-01)
// The battle command row, The Broken Seal's BattleCommandRow_Run
// (games/tbs/src/window/screens/battle_command_row.c) on this game's work and
// command numbers. The party's row (mode 0) is Fight (14), command 49 (when
// the battle allows it, no reserve member is chosen and more than four are
// active), Flee (4, when flee is passed above 0 and the battle rule is not 2)
// and Status (7); a member's row Attack (0), Psynergy (1), Djinn (15) and
// Summon (16) as in The Broken Seal, Item (2) and Defend (3). B cancels in
// both rows. The tutorial's actors are 4 to 6.
// Left, all plain C with no device:
// - The strip's closing loop: the native keeps the add of 0x452 apart from the
//   store; here it folds into strh r6, [r3, r7]. The Broken Seal's tagged
//   FAKEMATCH(volatile) on that store gives differ 8, gap 4 + 4 (the same
//   device as its twin; left out of this draft for now). Eight respellings of
//   the address gained nothing.
// - Two scheduling pairs: str r5, [sp, #4] falls after mov ip, r6 in both icon
//   loops, and the first entry's strh falls after movs r6, #228. Reordering the
//   value and index expressions and naming the kind changed neither.
// What moved it: battleMenu is its own variable (read early, in r5), the two
// rows keep separate counters, message is declared where it is used (its
// spill slot follows), the flag store is through a plain u8 pointer (a struct
// field keeps the commandKinds reload from happening), and Task_Add returns s32.
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

extern const u16 gWindowSprite_BounceScales[32];
extern struct BgScrollPair gBgScroll[];
extern u8 gMainRoutineWork[];

s32 VramCache_AllocEntry(s32 size);
void *Heap_AllocEwram(s32 size);
void *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
s32 Party_CountActiveOwnersFar(void);
s32 *Djinn_GetRecoveryTableFar(s32 kind);
void Func_08118130(s32 kind, u8 *counts);
void Func_080456f8(struct CommandRowWork *work, s32 index, s32 kind);
s32 Task_Add(void (*func)(void), s32 order);
s32 Task_Remove(void (*func)(void));
void Irq_SetHandler(u32 irq, s32 vcount, void (*handler)(void));
void Graphics_SetBg1Priority3(void);
void Func_08045780(void);
void WindowSprite_PrepareForRedraw(void *window);
void Text_DrawCharacterAtOffset(s32 message, void *window, s32 x, s32 y);
s32 AffineMatrix_BuildForEffect(struct CommandRowAffine *affine);
void Oam_AddSpriteAtDepth(void *sprite, s32 depth);
s32 Func_0804519c(void);
void *Message_ShowAndWait(s32 message, s32 x, s32 y);
void Func_08118128(s32 x, s32 y);
void Window_Close(void *window, s32 mode);
void Sound_PlayFar(s32 cue);
void QueueIoWriteDelay2(u32 address, u32 value);
void wait(s32 frames);
s32 Resource_ResetEntry(u32 entry);
void Heap_Release(void *block);

s32 Func_080457d0(s32 mode, s32 fleeAllowed) {
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
    selectionUi->window = Window_Create(22, 17, 8, 3, 6);
  selectionUi->drawnIndex = -1;
  if (mode == 0) {
    kindCount = 0;
    selectionUi->commandKinds[kindCount++] = 14;
    if (((u8 *)Ram_HeapSlots->battleWork)[0x44] == 0 &&
        battleMenu->reserveChoice == -1 && Party_CountActiveOwnersFar() > 4)
      selectionUi->commandKinds[kindCount++] = 49;
    if (fleeAllowed > 0 && gMainRoutineWork[0x24b] != 2)
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
    if (*Djinn_GetRecoveryTableFar(0) != 0) {
      Func_08118130(0, party);
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
    Func_080456f8(selectionUi, entryIndex,
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
        selectionUi->entries[entryIndex].positionX = x + 171;
      selectionUi->entries[entryIndex].positionY = 136;
      entryIndex++;
    } while (entryIndex < selectionUi->iconCount);
  }
  Task_Add(Graphics_SetBg1Priority3, 0xc80);
  Irq_SetHandler(2, 136, Func_08045780);

  for (;;) {
    pulseScale = gWindowSprite_BounceScales[(gFrameCount * 2) & 31];
    pulseScale = (pulseScale - 256) / 4 + 304;
    selectionUi->affine.scaleY = selectionUi->affine.scaleX = pulseScale;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 32;
    if (stripDirty != 0) {
      stripDirty = 0;
      work->holdTilemapUpload = 1;
      if (mode != 0) {
        s32 panel;

        for (panel = 0; panel < 6 - selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;

            for (col = 0; col <= 2; col++)
              *(work->tilemap + (entryIndex + 17) * 32 + panel * 3 + 4 +
                (col & 3)) = 0xf07f;
          }
        }
        for (panel = 0; panel < selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;

            for (col = 0; col <= 2; col++) {
              volatile u16 *vram = (volatile u16 *)0x0600f800 + 21 * 32 + 22;

              vram[panel * 3 + entryIndex * 32 + (col & 3) -
                   selectionUi->iconCount * 3] =
                  panel * 16 + entryIndex * 4 + col + 0x100;
              *(work->tilemap + (entryIndex + 17) * 32 + panel * 3 + (col & 3) -
                selectionUi->iconCount * 3 + 22) = 0;
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
      Text_DrawCharacterAtOffset(
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
                    Func_0804519c() + 10, 15, 8);
              else if (selectionUi->commandKinds[selectionUi->selectedIndex] ==
                       16)
                message = Message_ShowAndWait(Func_0804519c() + 9,
                                                            15, 8);
              ((u8 *)Ram_HeapSlots->windowTiles)[5] &= ~4;
              Func_08118128(102, 155);
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
    QueueIoWriteDelay2(0x04000000, 0x1741);
    work->holdTilemapUpload = 0;
    wait(1);
  }
  if (mode != 0)
    NAVIGATION->commandSelection[1] = selectionUi->selectedIndex;
  else
    NAVIGATION->commandSelection[0] = selectionUi->selectedIndex;
  for (entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++)
    Resource_ResetEntry(selectionUi->entries[entryIndex].resource);
  work->holdTilemapUpload = 1;
  Window_Close(selectionUi->window, 1);
  {
    s32 panel;

    for (panel = 0; panel <= 6; panel++) {
      for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
        s32 col;

        for (col = 0; col <= 2; col++)
          *(work->tilemap + (entryIndex + 17) * 32 + panel * 3 + 5 + (col & 3)) =
              0xf07f;
      }
    }
  }
  work->dirty = 1;
  wait(1);
  Task_Remove(Graphics_SetBg1Priority3);
  Irq_SetHandler(2, 0, 0);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x1541, 0x20000);
  Heap_Release(selectionUi);
  work->holdTilemapUpload = 0;
  wait(1);
  return command;
}
