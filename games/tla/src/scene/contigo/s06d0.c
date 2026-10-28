#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_02000620(s32 *a, s32 *b);
s32 Func_02000658(struct ObjectRuntime *object, struct ObjectRuntime *other,
                  s32 range);
void Func_02003fdc(struct ObjectRuntime *object, s32 mode);
void Func_0200402c(struct ObjectRuntime *object, s32 value);
s32 Func_02004254(struct ObjectRuntime *object);

s32 Func_020006d0(struct ObjectRuntime *self) {
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *state;
  u32 mode;
  s8 *effect;
  struct ObjectRuntime *other;

  Func_0200402c(self, Func_02004254(Object_GetById(gAdventure.currentOwner)));
  mode = 0x3ff & *(u16 *)(event + 0x166);
  state = &self->movementState;
  if (mode == 141) {
    effect = Ram_HeapSlots->effectWork;
    if (effect[0x713] != 0)
      goto near;
  }
  if (BYTE_AT(self, 99) == 1)
    goto near;
  other = Object_GetById(gAdventure.currentOwner);
  if (Func_02000620(&self->x, &other->x) <= 11)
    goto far;
  if (BYTE_AT(self, 98) == 1)
    other = Object_GetById(15);
  else
    other = Object_GetById(14);
  if (Func_02000658(self, other, 18) != 0)
    goto near;
  if (Func_02000658(self, Object_GetById(gAdventure.currentOwner), 26) != 0)
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
