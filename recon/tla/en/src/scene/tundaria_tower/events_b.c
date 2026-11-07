#include "../../../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"

void Func_02000bfc(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;


  if (party->scene == (s32)&SceneId_Ov68fRoom6) {
    struct Actor *object;

    Func_02003170(gData_02003264);
    object = *(struct Actor **)((u8 *)work + 0xcbc);
    object->action = ((object->x >> 20) << 8) | ((object->z >> 20) & 0xff);
  } else if (party->scene == (s32)&SceneId_Ov68fRoom7) {
    Func_02003170(gData_0200326e);
  }
}
