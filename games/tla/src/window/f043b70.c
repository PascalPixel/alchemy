#include "types.h"
#include "ram_buffer.h"

struct ObjectRowState {
  u8 unk000[248];
  void *objects[4];
  u8 unk108[16];
  s16 x[8];
  s16 y[8];
  s32 scale[4];
};

void Render_ApplyProjectedPlacementFar(s32 handle, s32 *request, s32 *source,
                                       s32 flags);

void Func_08043b70(void) {
  struct ObjectRowState *state =
      (struct ObjectRowState *)Ram_HeapSlots->menuRuntime;
  s32 source[2];
  s32 request[4];
  s32 index = 0;
  s32 handle;

  while (1) {
    handle = (s32)state->objects[index];
    if (handle != 0) {
      source[0] = state->scale[index];
      source[1] = state->scale[index];
      request[0] = state->x[index] << 16;
      request[1] = 0x01f40000;
      request[2] = (state->y[index] << 16) + 0x01f40000;
      request[3] = 0;
      Render_ApplyProjectedPlacementFar(handle, request, source, 0x4000);
    }
    index += 1;
    if (index > 3) {
      break;
    }
  }
}
