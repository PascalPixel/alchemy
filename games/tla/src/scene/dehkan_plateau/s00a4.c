#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct EffectWork1a {
  u8 unk00[0x1a];
  s16 active;
};

void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Func_02000128(s32 id, u32 mode, s32 flag, s32 layer);

void Func_020000a4(const void *data, s32 mode, s32 timer) {
  const u16 *spec = data;
  struct EffectWork1a *work = Ram_HeapSlots->effectWork;
  while (*spec != 0xffff) {
    u32 value = *spec;
    s32 active = work->active;

    if (active == value) {
      struct Actor *object = Actor_GetById(active);
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
  }
}
