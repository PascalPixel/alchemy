#include "actor.h"
#include "ram_buffer.h"
#include "system/main_routine_work.h"
#include "types.h"
#include "event_inline.h"

struct MagmaRockEventWork {
  u8 unk000[362];
  u16 unk16a;
};

s32 Math_Cosine(s32 angle);
void Func_02000c40(struct Actor *object);
void Func_02000d40(struct Actor *object);
s32 Func_020005a0(s32 *a, s32 *b);

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

void Func_02000dc0(struct Actor *object) {
  s32 step;
  struct Actor *partner;
  struct Actor *leader;

  step = Math_Cosine(object->angle);
  partner = Actor_Get(HALF_AT(object, 102) + BYTE_AT(object, 98));
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  object->x += step * 2;
  Func_02000d40(object);
  if (leader->x - 0x80000 < object->x + 0x60000 &&
      object->x - 0x60000 < leader->x + 0x80000 &&
      leader->z - 0x80000 < object->z + 0x60000 &&
      object->z - 0x60000 < leader->z + 0x80000 &&
      leader->y < object->y + 0x80000 &&
      object->y - 0x80000 < leader->y + 0xf0000) {
    ((struct MagmaRockEventWork *)Ram_HeapSlots->eventWork)->unk16a = 200;
    object->update = NULL;
    object->x = 0;
    object->y = 0;
    object->z = 0;
  }
  if (Func_020005a0(&object->x, &partner->x) <= 8 &&
      object->y >> 20 == partner->y >> 20) {
    object->x = 0;
    object->update = NULL;
    object->angle ^= 0x9000;
    object->y = 0;
    object->z = 0;
    BYTE_AT(object, 98) ^= 1;
    partner->action = 0;
    partner->update = (void *)Func_02000c40;
    partner->watchedObject = object;
  }
}
