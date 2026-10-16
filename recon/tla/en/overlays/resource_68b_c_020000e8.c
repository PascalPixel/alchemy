// Near miss, differ 2 of 81: the update of an actor that faces the leader
// and turns to mode 1 when near (in range of its linked object or of the
// leader) and mode 2 when far. An empty volatile asm keeps the effect work's
// load ahead of the 0x713 offset, as the native orders them (differ 4
// without it). Left: the native loads gPartyState's address before it
// starts the 0x214 offset (movs r2, #133); here the constant goes first.
// alchemy permute for 3 minutes found nothing closer.
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_02001d54(struct ObjectRuntime *object);
void Func_02001ba4(struct ObjectRuntime *object, s32 priority);
void Func_02001b7c(struct ObjectRuntime *object, s32 mode);
s32 Func_02000038(s32 *a, s32 *b);
s32 Func_02000070(struct ObjectRuntime *self, struct ObjectRuntime *other,
                  s32 range);

s32 Func_020000e8(struct ObjectRuntime *self) {
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *state;
  u32 mode;
  s8 *effect;
  struct ObjectRuntime *other;

  Func_02001ba4(self, Func_02001d54(Object_GetById(gPartyState.currentOwner)));
  mode = 0x3ff & *(u16 *)(event + 0x166);
  state = &self->movementState;
  if (mode == 141) {
    effect = Ram_HeapSlots->effectWork;
    /* FAKEMATCH: the barrier keeps the effect work load ahead of the 0x713
     * offset (differ 2 more without it). */
    asm volatile("");
    if (effect[0x713] != 0)
      goto near;
  }
  if (self->action == 1)
    goto near;
  other = Object_GetById(gPartyState.currentOwner);
  if (Func_02000038(&self->x, &other->x) <= 11)
    goto far;
  if (Func_02000070(self, self->linkedObject, 18) != 0)
    goto near;
  if (Func_02000070(self, Object_GetById(gPartyState.currentOwner), 26) != 0)
    goto near;
far:
  *state = 0;
  Func_02001b7c(self, 2);
  return 0;
near:
  *state = 1;
  Func_02001b7c(self, 1);
  return 0;
}
