// Draft, differ 39 of 106: keeps the counters of an object and person 9 in
// step: at 30 both move on to 31; past 2 each stops (its movement state 1) and
// sets its flag 0x206 or 0x205, unless person 9 runs one of its two scripts; at
// 0 each clears it; both stay at most 35. Read through struct fields, the
// object's counter stays in r1 across the event work's store as in the native
// (through FIELD_AT it reloads, 58). Left: gcse makes person 9's counter
// address one pseudo from the first test on (inserting it on the edge into the
// 30 case), where the native rebuilds it in each test and sets r6 to it only
// after flag 0x206.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern s32 gData_020056c4[];
extern s32 gData_02005728[];

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);

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
  struct Counted *other = (struct Counted *)Object_GetById(9);

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
