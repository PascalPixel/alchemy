#include "room.h"
#include "edition.h"

void Func_020002f8(void) {
  if ((gFrameCount & 7) == 0) {
    volatile u16 *palette;
    volatile u16 *slot;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)0x05000172;
    index = 9;
  first:
    palette = (u16 *)0x05000160;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 4)
      goto first;
    slot = (volatile u16 *)0x05000168;
    *slot = saved;
    slot += 11;
    saved = *slot;
    index = 15;
  second:
    palette = (volatile u16 *)0x05000160;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 12)
      goto second;
    *(volatile u16 *)0x05000178 = saved;
  }
}

s32 Func_02000360(void) {
  SCENE_WORK->unk1ac = 256;
  Actor_GetById(gAdventure.currentOwner)->priorityFlags |= 32;
  Task_Add(Func_020002f8, 0x480);
  Flag_Clear(0x12f);
#if (LANGUAGE != LANGUAGE_JAPANESE) /* Absent from the Japanese edition. */
  if (Flag_IsSet(0x90a))
    Actor_SetSpritePriority(9, 1);
  else
    Func_020004bc(9, 0, 0);
#endif
  return 0;
}

s32 Func_020003cc(void) {
  if (!Flag_IsSet(0x8df)) {
    Func_0200045c(57, 52, 64, 15, 1, 1);
    Func_0200045c(60, 53, 67, 16, 6, 3);
    Func_0200045c(50, 52, 16, 16, 6, 3);
    Func_0200045c(50, 61, 16, 28, 10, 3);
    Func_0200045c(61, 61, 16, 63, 10, 3);
  }
  return 0;
}
