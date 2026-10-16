// Draft of overlay 69b's Func_020007d0, differ 12 of 57: spawns 22 particles
// (kind 672) with random speed limits and accelerations, each its own phase
// five apart, at sprite priority 1. Left: the priority store's registers are
// swapped: the native reads the sprite byte into r3 and copies the -13 mask
// into r2 (ldrb r3; adds r2, r0, #0; ands r3, r2; movs r2, #4; orrs r3, r2);
// this draft reads the byte into r2. Tried: the sprite as a local, the cast
// store, i < 22 with i * 5 (24), the declarations in another order; permute 3
// minutes found nothing.
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

struct ObjectRuntime *Func_020021cc(s32 kind, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
void Func_02000720(void);

void Func_020007d0(void) {
  s32 i;
  s32 phase;
  struct ObjectRuntime *object;

  for (i = 0, phase = 0; i != 22; i++, phase += 5) {
    object = Func_020021cc(672, 0, 0, 0);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (Random16Far() << 2) + 0x60000;
    object->acceleration = ((Random16Far() << 11) >> 16) + 0x800;
    object->update = Func_02000720;
    object->action = Random16Far();
    object->unk66 = phase;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Object_SetPartAttribute(object, 2);
    ((struct FieldSprite *)object->animation)->priority = 1;
  }
}
