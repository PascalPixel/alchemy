// Near miss, differ 2 (one reordered row): the listing loads object->x into r6
// before it copies the 0x40 mask to r3; this source copies first. Splitting
// the expression, forced temporaries and one or two barriers or one-pass
// blocks at every statement boundary (216 forms) leave the same row.
// Overlay 69C Func_02001d48 is the same function.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct CellStep {
  u8 unk00[6];
  s16 x;
  s16 z;
};

void FieldEvent_WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02001550(void);
void Func_02001ca0(s32 cue);
void Func_02001bb0(s32 x, s32 z, s32 width, s32 height, s32 from_x, s32 from_z);

void Func_020015d8(struct CellStep *step) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s32 x = (object->x >> 20) | (s16)(step->x & 0x40);
  s32 z = (object->z >> 20) | (s16)(step->z & 0x40);

  Func_02001550();
  Func_02001ca0(161);
  Func_02001bb0(step->x, step->z + 1, 1, 1, x, z);
  FieldEvent_WaitFrames(12);
}
