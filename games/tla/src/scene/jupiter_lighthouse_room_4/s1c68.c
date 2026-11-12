#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"
#include "cell_target.h"

extern struct CellTarget gData_02002864;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02001a20(void);
void Func_02001dd0(void);

void Func_02001c68(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  struct CellTarget *work =
      &gData_02002864;
  s32 x = ((s32)FieldGameFlag_GetByte(work->flagByte));
  s32 z = ((s32)FieldGameFlag_GetByte(work->flagByte + 8));

  if (x != 0 || z != 0) {
    struct Actor *shadow;

    GameFlag_SetByte(work->flagByte, 0);
    GameFlag_SetByte(work->flagByte + 8, 0);
    object->x = ((x + dx) << 20) + 0x80000;
    object->z = ((z + dz) << 20) + 0x80000;
    shadow = *(struct Actor **)(event + 460);
    shadow->x = object->x;
    shadow->z = object->z;
    Func_02001dd0();
    Func_02001a20();
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = value;
  }
}
