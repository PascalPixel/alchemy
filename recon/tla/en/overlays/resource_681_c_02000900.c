// Draft, differ 58 of 106: keeps the counters of an object and person 9 in
// step: at 30 both move on to 31; past 2 each stops (its movement state 1) and
// sets its flag 0x206 or 0x205, unless person 9 runs one of its two scripts; at
// 0 each clears it; both stay at most 35.
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

void Func_02000900(struct ObjectRuntime *object) {
  struct ObjectRuntime *other = Object_GetById(9);
  u8 *count = &FIELD_AT(object, u8, 96);
  u8 *otherCount;

  if (*count == 30 || FIELD_AT(other, u8, 96) == 30) {
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 362) = 42;
    if (*count == 30)
      *count = 31;
    if (FIELD_AT(other, u8, 96) == 30)
      FIELD_AT(other, u8, 96) = 31;
  }
  if (*count > 2) {
    other->movementState = 1;
    GameFlag_SetBit(0x206);
  }
  otherCount = &FIELD_AT(other, u8, 96);
  if (*otherCount > 2 && !GameFlag_Test(0x206) &&
      other->script != gData_020056c4 && other->script != gData_02005728) {
    object->movementState = 1;
    GameFlag_SetBit(0x205);
  }
  if (*count == 0) {
    other->movementState = 0;
    GameFlag_ClearBit(0x206);
  }
  if (*otherCount == 0) {
    object->movementState = 0;
    GameFlag_ClearBit(0x205);
  }
  if (*count > 35)
    *count = 35;
  if (*otherCount > 35)
    *otherCount = 35;
}
