// Near miss, differ 6 of 60: The Broken Seal's
// Menu_UpdateFirstObjectRowPositions over this game's menu work (objects at
// 248, x at 280, y at 296, scale at 312). Three rows sit one step off: at
// entry the native loads the menu work before it copies the cleared index
// (ldr r3, [r3]; mov r8, r2) and clears the index after the heap base's
// shift; in the loop it saves r4 around the call before the second read of
// the scale (str r4, [sp, #0]; ldr r3, [r5, #64]). Measured: the work
// loaded in a one-pass block (differ 10: the frame and the index move ahead
// of the heap base).
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
