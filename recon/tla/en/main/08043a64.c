// Draft, differ 8 of 99 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Near miss, differ 8 of 99, 2026-10-15: The Broken Seal's
// ObjectPlacement_CreateGroup on this game's menu runtime (party icons at
// 0xf8, positions at 0x118 and 0x128, scales at 0x138), without the party
// remap call, at priority 0x480. Two empty asm barriers in the loop (found by
// a device search, differ 14 without). Remaining: the native stores the
// second and third arguments to the stack before it copies the fourth and
// starts the heap slot's address (str r2; str r1; mov r8, r3; movs r3, #192;
// lsls r3); this copies and starts first. At the loop's end the native loads
// the next party byte before it builds -1; this builds first.

#include "types.h"
#include "ram_buffer.h"
#include "task.h"
#include "render_input.h"
#include "save_state.h"
#include "field_sprite.h"

struct ObjectRowWork {
  u8 unk000[0xf8];
  void *icons[4];
  u8 unk108[0x10];
  s16 x[8];
  s16 y[8];
  s32 scale[4];
};

void *ResourceObject_CreateFar(s32 resource);
void Animation_ApplyChildArgumentFar(void *, s32);
void Func_08043b70(void);

void Func_08043a64(struct RenderInput *origin, s32 x, s32 y,
                   struct SaveSummary *descriptor) {
  struct ObjectRowWork *state =
      (struct ObjectRowWork *)Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = 0; i < 4 && descriptor->party[i] != -1; i++) {
    void *object = ResourceObject_CreateFar(descriptor->party[i]);

    if (object != 0) {
      Animation_ApplyChildArgumentFar(object, 1);
      ((struct FieldSprite *)object)->nextFrame = 0;
      asm("");
      ((struct FieldSprite *)object)->priority = 0;
      asm("");
    }

    state->icons[i] = object;
    state->x[i] = (x + (s16)origin->x + i * 3) * 8 + 0x10;
    state->y[i] = (y + (s16)origin->y) * 8 + 0x10;
    state->scale[i] = 0x10000;
  }

  Task_Add(Func_08043b70, TASK_PASS_VBLANK);
}
