// Matches: differ 0 (tla-en, 2026-11-01)
// No devices: the old translation's locals are gone, and neither the side byte
// nor object + 102 needs a local (written with `side` read through a pointer set
// first, r1 and r2 swap in the two reloads, differ 12).
// Each frame the object moves along x by twice the cosine of its angle and
// throws its effect (Func_02000d40). If the leader is inside the box 6 pixels
// wider than its own 8 on x and z, and 8 below to 15 above on y, it writes 200
// to event work +362 and stops dead. Within 8 pixels of the object first + side,
// on the same height row, it stops, turns round, flips side and hands itself to
// that object, which takes Func_02000c40.
#include "actor.h"
#include "ram_buffer.h"
#include "system/main_routine_work.h"
#include "types.h"

struct MagmaRockEventWork {
  u8 unk000[362];
  u16 unk16a;
};

s32 Math_Cosine(s32 angle);
struct Actor *Actor_GetById(s32 id);
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
  partner = Actor_GetById(HALF_AT(object, 102) + BYTE_AT(object, 98));
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
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
