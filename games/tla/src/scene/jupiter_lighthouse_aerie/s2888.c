#include "../../../../games/tla/src/scene/jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"

void Func_02002888(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_GetById(gAdventure.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  struct JupiterLighthouseAerieWork *work =
      (struct JupiterLighthouseAerieWork *)gData_02003488;
  s32 x = GameFlag_GetByte(work->flag);
  s32 z = GameFlag_GetByte(work->flag + 8);

  if (x != 0 || z != 0) {
    struct Actor *shadow;

    GameFlag_SetByte(work->flag, 0);
    GameFlag_SetByte(work->flag + 8, 0);
    object->x = ((x + dx) << 20) + 0x80000;
    object->z = ((z + dz) << 20) + 0x80000;
    shadow = *(struct Actor **)(event + 460);
    shadow->x = object->x;
    shadow->z = object->z;
    Func_02002a18();
    Func_02002640();
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = value;
  }
}
