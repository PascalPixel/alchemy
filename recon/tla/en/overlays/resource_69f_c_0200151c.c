// Near miss: differ 2 of 49 (one reordered row). The native reads the owner's x
// (ldr r6, [r0, #8]) before it copies the layer mask (adds r3, r2, #0); the
// draft copies first. A one-pass block on the x read puts it ahead of the step
// read and the mask as well; with the step read and the mask inside the block
// the mask is and-ed in place and its copy is lost (differ 7). The same
// function is in overlays 698, 69A, 69C, 69D, 6A4, 6AD and 6B0 after their
// step-off.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct MapStep {
  u8 unk00[6];
  s16 x;
  s16 z;
};

struct ObjectRuntime *Object_GetById(s32 id);
void FieldEvent_WaitFrames(s32 frames);
void Func_02001494(void);
void Func_02001bec(s32 cue);
void Func_02001afc(s32 x, s32 z, s32 width, s32 height, s32 to_x, s32 to_z);

void Func_0200151c(struct MapStep *step) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s16 layerX = step->x & 0x40;
  s32 x = (object->x >> 20) | layerX;
  s16 layerZ = step->z & 0x40;
  s32 z = (object->z >> 20) | layerZ;

  Func_02001494();
  Func_02001bec(161);
  Func_02001afc(step->x, step->z + 1, 1, 1, x, z);
  FieldEvent_WaitFrames(12);
}
