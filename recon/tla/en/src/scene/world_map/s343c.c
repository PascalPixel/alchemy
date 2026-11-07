#include "types.h"
#include "frame.h"
#include "actor.h"

extern const u8 gData_02004f70[];

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02004b18(struct Actor *object, s32 a);
void Func_02004b20(struct Actor *object, const u8 *data);
struct Actor *Func_02004b30(s32 kind, s32 x, s32 y, s32 z);

void Func_0200343c(struct Actor *parent) {
  if ((gFrameCount & 7) == 0) {
    s32 x = parent->x;
    s32 y = parent->y;
    s32 z = parent->z;
    struct Actor *object = Func_02004b30(673, x, y, z);

    if (object != NULL) {
      Func_02004b20(object, gData_02004f70);
      Func_02004b18(object, 1);
      Actor_SetSingleChildFlags(object, 0);
      object->motionFlags = 0;
      object->scaleX = 0x20000;
      object->scaleY = 0x20000;
    }
  }
}
