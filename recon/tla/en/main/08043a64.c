// Near miss, differ 4 of 99 (tla-en, 2026-10-31): only the entry block. The
// native stores the second and third arguments to the stack before it copies
// the fourth (str r2; str r1; mov r8, r3; movs r3, #192); this copies the
// fourth and starts the heap slot's address first. The instructions and
// registers are the native's; sched2 ranks the copy at priority 8 (its r3 is
// reused for the heap slot's address) and both stores at 7, a rank no
// reshaping moved (state assigned apart or in the loop's start, a party
// pointer, a break loop) and a 4-minute permuter run did not beat. Under
// -mthumb-slow-loads a stack store before the heap slot's load costs 2, so
// the stores rank 2 above that load, while the copy ranks 3 above it through
// its anti-dependence on r3 and the address's three instructions: no source
// that keeps these instructions ranks the stores first. A hand trace of
// sched2 gives the native's order if the address were one instruction there
// (copy at 6). Volatile x and y: differ 58.
// The Broken Seal's FileList_ShowPartySprites (games/tbs/src/window/screens/
// save.c) on this game's menu runtime (party icons at 0xf8, positions at
// 0x118 and 0x128, scales at 0x138), without the party remap call, at priority
// 0x480. The empty asm stands for a memory dependence: the native restores the
// party offset (ldr r4, [sp]) after the priority store, as if that store could
// alias the stack slot; a char-typed store, ((u8 *)object)[9] &= ~0xc, keeps
// that order but builds its mask differently (differ 56). An empty one-pass
// block after or around the stores, a volatile store or swapping the two
// stores does not.

#include "types.h"
#include "ram_buffer.h"
#include "task.h"
#include "render_input.h"
#include "save.h"
#include "field_sprite.h"
#include "resource.h"

struct ObjectRowWork {
  u8 unk000[0xf8];
  void *icons[4];
  u8 unk108[0x10];
  s16 x[8];
  s16 y[8];
  s32 scale[4];
};

void Animation_ApplyChildArgumentFar(void *, s32);
void Func_08043b70(void);

void Func_08043a64(struct RenderInput *origin, s32 x, s32 y,
                   struct SaveSummary *descriptor) {
  struct ObjectRowWork *state = Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = 0; i < 4 && descriptor->party[i] != -1; i++) {
    struct FieldSprite *object =
        (struct FieldSprite *)ResourceObject_CreateFar(descriptor->party[i]);

    if (object != 0) {
      Animation_ApplyChildArgumentFar(object, 1);
      object->nextFrame = 0;
      object->priority = 0;
      /* FAKEMATCH(barrier): an empty asm after the priority store; as plain
       * statements, 6 of 99 instructions differ in tla-en. */
      asm("");
    }

    state->icons[i] = object;
    state->x[i] = (x + (s16)origin->x + i * 3) * 8 + 0x10;
    state->y[i] = (y + (s16)origin->y) * 8 + 0x10;
    state->scale[i] = 0x10000;
  }

  Task_Add(Func_08043b70, TASK_PASS_VBLANK);
}
