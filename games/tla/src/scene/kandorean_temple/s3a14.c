#include "local.h"
#include "version.h"

struct MapWork17 {
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  u16 flags;
  u8 wideTiles;
  u8 bits0 : 2;
  u8 mode : 2;
};

void Func_02003a14(void) {
  struct MapWork17 *map = (struct MapWork17 *)Ram_HeapSlots->mapWork;

  gData_02006294 = 0;
  map->mode = 1;
  Func_02005338(0);
#if TLA_PATCH_1
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2)
    GameFlag_ClearBit(0x12f);
#endif
}
