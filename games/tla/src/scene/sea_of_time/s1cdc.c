/// @file
/// The overlay's start: sets the screen effect, restores the clock and palette in the first room, draws the unlit cells in the second, and schedules the frame callback.
#include "sea_of_time.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"

extern u16 gData_020026c0[];

void Func_02000c50(void);
void Func_02001650(void);
void Func_02001a14(void);

s32 Func_02001cdc(void) {
  ((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 256;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov687Room1) {
    gData_020022c8 = gFrameCount;
    gData_020022cc = 1;
    Dma_Set((const void *)0x05000062, gData_020026c0, 0x8000000e,
            (volatile u32 *)0x040000d4);
    if (GameFlag_Test(0x200)) {
      u32 *state = gSceneState;
      u32 offset, i;

      gData_020022c8 = *state++;
      gData_020022cc = *state++;
      gFrameCount = *state;
      i = 1;
      offset = 98;
      do {
        *(volatile u16 *)(offset + 0x05000000) = 0x5ccb;
        i++;
        offset += 2;
      } while (i != 14);
      Task_Add(Func_02000c50, 0x480);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov687Room2) {
    gData_020022c8 = gFrameCount;
    gData_020022cc = 0;
    if (gMainRoutineWork.entrance <= 4) {
      if (!GameFlag_Test(0x940)) {
        Dma_Set((const void *)0x05000062, gData_020026c0, 0x8000000e,
                (volatile u32 *)0x040000d4);
      } else {
        if (!GameFlag_Test(0xa90))
          Func_020018a8(21, 23, 9);
        if (!GameFlag_Test(0xa91))
          Func_020018a8(37, 17, 10);
        if (!GameFlag_Test(0xa93))
          Func_020018a8(47, 27, 12);
        if (!GameFlag_Test(0xa94))
          Func_020018a8(59, 19, 13);
        {
          void *block = Ram_HeapSlots->block32;

          *(volatile u16 *)0x0500001e &= 0xffe0;
          Dma_Set((const void *)0x05000000, block, 0x84000070,
                  (volatile u32 *)0x040000d4);
        }
        BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
      }
      if (gMainRoutineWork.entrance == 4 && !GameFlag_Test(265))
        Func_02001a14();
    } else {
      if (!GameFlag_Test(0xa95))
        Func_020018a8(89, 85, 9);
    }
  }
  {
    s32 priority = 0xc80;

    Task_Add(Func_02001650, priority);
  }
  return 0;
}
