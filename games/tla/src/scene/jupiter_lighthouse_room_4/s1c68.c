#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

struct JupiterLighthouseAerieWork {
  u8 unk00[2];
  s16 flag;
  u8 unk04[2];
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  u8 unk0e[18];
};

struct JupiterLighthouseAerieEntry {
  s16 x;
  s16 z;
  s16 flag;
  u8 unk06[2];
};

extern u8 gData_02002864[];

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02001a20(void);
void Func_02001dd0(void);

void Func_02001c68(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  struct JupiterLighthouseAerieWork *work =
      (struct JupiterLighthouseAerieWork *)gData_02002864;
  s32 x = ((s32)GameFlag_GetByte(work->flag));
  s32 z = ((s32)GameFlag_GetByte(work->flag + 8));

  if (x != 0 || z != 0) {
    struct Actor *shadow;

    GameFlag_SetByte(work->flag, 0);
    GameFlag_SetByte(work->flag + 8, 0);
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
