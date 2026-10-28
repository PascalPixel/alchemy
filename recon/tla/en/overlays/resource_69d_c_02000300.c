// Draft of Func_02000300 (overlay 69D, 57 instructions), differ 20: seven
// objects of kind 672 with random speeds and phases 0, 5, 10 ..., each updated
// by Func_02000244. Written from the listing. With i < 7 the loop counts down
// (24); with i != 7 it counts up as the native, but the counter and the phase
// swap r6 and r7, and the priority bitfield loads the sprite byte into r2 where
// the native uses r3. Measured: the declarations swapped, u32 i, a do-while,
// the priority as explicit byte masks (20 to 23); pinning i to r7, or i and the
// phase, 37 to 43.
#include "types.h"
#include "actor.h"
#include "field_sprite.h"

struct Actor *Func_02001dc0(s32 kind, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_02000244(struct Actor *object);

void Func_02000300(void) {
  struct Actor *object;
  s32 i;
  s32 phase;

  phase = 0;
  for (i = 0; i != 7; i++) {
    object = Func_02001dc0(672, 0, 0, 0);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (Random16Far() << 2) + 0x60000;
    object->acceleration = ((Random16Far() << 11) >> 16) + 0x800;
    object->update = (void (*)(void))Func_02000244;
    object->action = Random16Far();
    object->unk66 = phase;
    Actor_SetSingleChildFlags(object, 0);
    Actor_SetPartAttribute(object, 2);
    ((struct FieldSprite *)object->animation)->priority = 1;
    phase += 5;
  }
}
