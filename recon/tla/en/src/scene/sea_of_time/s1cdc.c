/// @file
/// The overlay's start: sets the screen effect, restores the clock and palette in the first room, draws the unlit cells in the second, and schedules the frame callback.
#include "../../../../../../games/tla/src/scene/sea_of_time/sea_of_time.h"
#include "main_routine_work.h"
#include "scene_ids.h"

extern u16 gData_020026c0[];

void Task_Add(void (*func)(void), s32 priority);
void Func_02000c50(void);
void Func_02001650(void);
void Func_02001a14(void);
void Func_0200201c(s32 a, s32 b);

s32 Func_02001cdc(void) {
  ((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 256;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov687Room1) {
    gData_020022c8 = gFrameCount;
    gData_020022cc = 1;
    Dma_Set((const void *)SPLIT_CONSTANT(0x5000000, 98), gData_020026c0, SPLIT_CONSTANT(0x80000000, 14),
            (volatile u32 *)SPLIT_CONSTANT(0x4000000, 212));
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
        Dma_Set((const void *)SPLIT_CONSTANT(0x5000000, 98), gData_020026c0, SPLIT_CONSTANT(0x80000000, 14),
                (volatile u32 *)SPLIT_CONSTANT(0x4000000, 212));
      } else {
        if (!GameFlag_Test(0xa90))
          Func_020018a8(21, 23, 9);
        if (!GameFlag_Test(SPLIT_CONSTANT(0xa00, 145)))
          Func_020018a8(37, 17, 10);
        if (!GameFlag_Test(SPLIT_CONSTANT(0xa00, 147)))
          Func_020018a8(47, 27, 12);
        if (!GameFlag_Test(SPLIT_CONSTANT(0xa00, 148)))
          Func_020018a8(59, 19, 13);
        {
          void *block = Ram_HeapSlots->block32;

          *(volatile u16 *)0x0500001e &= SPLIT_CONSTANT(0xff00, 224);
          Dma_Set((const void *)0x05000000, block, SPLIT_CONSTANT(0x84000000, 112),
                  (volatile u32 *)SPLIT_CONSTANT(0x4000000, 212));
        }
        Func_0200201c(0x10000, 0);
      }
      if (gMainRoutineWork.entrance == 4 && !GameFlag_Test(265))
        Func_02001a14();
    } else {
      if (!GameFlag_Test(SPLIT_CONSTANT(0xa00, 149)))
        Func_020018a8(89, 85, 9);
    }
  }
  {
    s32 priority = 0xc80;

    Task_Add(Func_02001650, priority);
  }
  return 0;
}
