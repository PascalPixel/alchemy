#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern s32 gData_020056c4[];
extern s32 gData_02005728[];

struct Counted {
  s32 *script;
  u8 unk04[0x57];
  u8 movementState;
  u8 unk5c[4];
  u8 count;
};

struct EventWork900 {
  u8 unk000[362];
  u16 timer;
};

void Func_02000900(struct Counted *object) {
  struct Counted *other = (struct Counted *)Actor_GetById(9);

  if (object->count == 30 || other->count == 30) {
    ((struct EventWork900 *)Ram_HeapSlots->eventWork)->timer = 42;
    if (object->count == 30)
      object->count = 31;
    if (other->count == 30)
      other->count = 31;
  }
  if (object->count > 2) {
    other->movementState = 1;
    GameFlag_SetBit(0x206);
  }
  {
    u8 *otherCount = &other->count;

    if (*otherCount > 2 && !GameFlag_Test(0x206) &&
        other->script != gData_020056c4 && other->script != gData_02005728) {
      object->movementState = 1;
      GameFlag_SetBit(0x205);
    }
    if (object->count == 0) {
      other->movementState = 0;
      GameFlag_ClearBit(0x206);
    }
    if (*otherCount == 0) {
      object->movementState = 0;
      GameFlag_ClearBit(0x205);
    }
    if (object->count > 35)
      object->count = 35;
    if (*otherCount > 35)
      *otherCount = 35;
  }
}
