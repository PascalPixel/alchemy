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
    }

    state->icons[i] = object;
    state->x[i] = (x + (s16)origin->x + i * 3) * 8 + 0x10;
    state->y[i] = (y + (s16)origin->y) * 8 + 0x10;
    state->scale[i] = 0x10000;
  }

  Task_Add(Func_08043b70, TASK_PASS_VBLANK);
}
