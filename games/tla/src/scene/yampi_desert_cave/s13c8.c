#include "yampi_desert_cave.h"

void Func_020013c8(s32 a) { YampiDesertCave_Call1920(a, 64, 0x302); }

const u8 *Func_020013d8(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov662Room2)
    return gData_02001ddc;
  if (gAdventure.scene == (s32)&SceneId_Ov662Room3)
    return gData_02001e78;
  return gData_02001d28;
}
