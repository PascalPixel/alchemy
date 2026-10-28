#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

struct Actor *Actor_GetById(s32 id);
s32 Func_02000620(s32 *a, s32 *b);
s32 Func_02000658(struct Actor *object, struct Actor *other,
                  s32 range);
void Func_02003fdc(struct Actor *object, s32 mode);
void Func_0200402c(struct Actor *object, s32 value);
s32 Func_02004254(struct Actor *object);

s32 Func_020006d0(struct Actor *self) {
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *state;
  u32 mode;
  s8 *effect;
  struct Actor *other;

  Func_0200402c(self, Func_02004254(Actor_GetById(gMainRoutineWork.currentOwner)));
  mode = 0x3ff & *(u16 *)(event + 0x166);
  state = &self->movementState;
  if (mode == 141) {
    effect = Ram_HeapSlots->effectWork;
    if (effect[0x713] != 0)
      goto near;
  }
  if (BYTE_AT(self, 99) == 1)
    goto near;
  other = Actor_GetById(gMainRoutineWork.currentOwner);
  if (Func_02000620(&self->x, &other->x) <= 11)
    goto far;
  if (BYTE_AT(self, 98) == 1)
    other = Actor_GetById(15);
  else
    other = Actor_GetById(14);
  if (Func_02000658(self, other, 18) != 0)
    goto near;
  if (Func_02000658(self, Actor_GetById(gMainRoutineWork.currentOwner), 26) != 0)
    goto near;
far:
  *state = 0;
  Func_02003fdc(self, 2);
  return 0;
near:
  *state = 1;
  Func_02003fdc(self, 1);
  return 0;
}
