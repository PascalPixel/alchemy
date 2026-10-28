// Near miss (87 of 197 instructions differ): the native keeps the
// palette fill's colour and base inside its loop, reloading both each pass,
// and builds the entrance offset in r0; the draft hoists them. Destined
// for the head of scene/sea_of_time/fade.c as the overlay's start.
#include "../../../../games/tla/src/scene/sea_of_time/sea_of_time.h"
#include "adventure.h"
#include "scene_ids.h"

extern u16 gData_020026c0[];

void Task_Add(void (*func)(void), s32 priority);
void Func_02000c50(void);
void Func_02001650(void);
void Func_02001a14(void);
void Func_0200201c(s32 a, s32 b);

s32 Func_02001cdc(void) {
  ((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 256;
  if (gAdventure.scene == (s32)&SceneId_Ov687Room1) {
    gData_020022c8 = gFrameCount;
    gData_020022cc = 1;
    Dma_Set((const void *)0x05000062, gData_020026c0, 0x8000000e,
            (volatile u32 *)0x040000d4);
    if (GameFlag_Test(0x200)) {
      u32 *state = gSceneState;
      u32 i;

      gData_020022c8 = *state++;
      gData_020022cc = *state++;
      gFrameCount = *state;
      for (i = 1; i != 14; i++)
        ((volatile u16 *)0x05000060)[i] = 0x5ccb;
      Task_Add(Func_02000c50, 0x480);
    }
  } else if (gAdventure.scene == (s32)&SceneId_Ov687Room2) {
    gData_020022c8 = gFrameCount;
    gData_020022cc = 0;
    if (gAdventure.entrance <= 4) {
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
        *(volatile u16 *)0x0500001e &= 0xffe0;
        Dma_Set((const void *)0x05000000, Ram_HeapSlots->block32, 0x84000070,
                (volatile u32 *)0x040000d4);
        Func_0200201c(0x10000, 0);
      }
      if (gAdventure.entrance == 4 && !GameFlag_Test(265))
        Func_02001a14();
    } else {
      if (!GameFlag_Test(0xa95))
        Func_020018a8(89, 85, 9);
    }
  }
  Task_Add(Func_02001650, 0xc80);
  return 0;
}
