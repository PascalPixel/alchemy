// Draft, differ 27 of 61 (was 52): finds the listed object that is the
// effect work's active one, shrinks it in the last frames and, in mode 1,
// sets its flag and places it through Func_02000128. The list walk is a
// goto loop (the native rebuilds 0xffff in each pass and enters at the
// test). Left: the native keeps active in r7 and the object in r6 (here
// the other way round), and sets r1 to 255 before the entry jump,
// skipping the test's first instruction.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct EffectWork1a {
  u8 unk00[0x1a];
  s16 active;
};

void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02000128(s32 id, u32 mode, s32 flag, s32 layer);

void Func_020000a4(const void *data, s32 mode, s32 timer) {
  const u16 *spec = data;
  struct EffectWork1a *work = Ram_HeapSlots->effectWork;
  s32 active;

  goto test;
loop:
  active = work->active;
  if (active == *spec) {
    struct ObjectRuntime *object = Object_GetById(active);
    s32 kind;
    s32 flag;

    spec++;
    kind = spec[0];
    flag = spec[1];
    if (timer <= 7)
      object->scaleY += -0x1f00;
    if (mode == 1) {
      GameFlag_SetBit(flag);
      Func_02000128(active, kind, flag, object->terrainId);
      object->scaleY = 0x10000;
    }
    return;
  }
  spec += 3;
test:
  if (*spec != 0xffff)
    goto loop;
}
