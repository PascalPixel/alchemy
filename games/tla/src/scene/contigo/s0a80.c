#include "types.h"
#include "call.h"
#include "frame.h"
#include "actor.h"

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
u32 Random16Far(void);
void Func_020009d0(void);

void Func_02000a80(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 0 of 47 instructions
   * differ in tla-en. */
  if ((gFrameCount & 7) == 0) {
    struct Actor *object =
        Actor_Spawn(308, 0x02c20000, SPLIT_CONSTANT(0xcccc), 0x740000);

    if (object != NULL) {
      Actor_SetSingleChildFlags(object, 0);
      object->update = Func_020009d0;
      object->motionFlags = 0;
      object->action = -(s32)((u32)(Random16Far() * 16) >> 16) * 2 + 20;
      object->unk66 = 0;
      object->gravity = SPLIT_CONSTANT(0x1999);
      object->velocityY = 0x30000;
      object->angle = Random16Far();
    }
  }
}
