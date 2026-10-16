// Near miss, differ 2 (one reordered row; 59 before, 2026-10-12): the same
// function as overlay 6A2 Func_02002d40 and 69C Func_02001d48, written as that
// draft is. The listing loads object->x into r6 before it copies the 0x40 mask
// to r3; this source copies first.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct CellStep {
  u8 unk00[6];
  s16 x;
  s16 z;
};

void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02002cb8(void);
void Func_020034b8(s32 cue);
void Func_02003350(s32 x, s32 z, s32 width, s32 height, s32 from_x, s32 from_z);

void Func_02002d40(struct CellStep *step) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s32 x = (object->x >> 20) | (s16)(step->x & 0x40);
  s32 z = (object->z >> 20) | (s16)(step->z & 0x40);

  Func_02002cb8();
  Func_020034b8(161);
  Func_02003350(step->x, step->z + 1, 1, 1, x, z);
  Battle_WaitMode0(12);
}
