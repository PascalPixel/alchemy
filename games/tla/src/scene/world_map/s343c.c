#include "types.h"
#include "frame.h"
#include "object_runtime.h"

extern const u8 gData_02004f70[];

void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02004b18(struct ObjectRuntime *object, s32 a);
void Func_02004b20(struct ObjectRuntime *object, const u8 *data);
struct ObjectRuntime *Func_02004b30(s32 kind, s32 x, s32 y, s32 z);

void Func_0200343c(struct ObjectRuntime *parent) {
  if ((gFrameCount & 7) == 0) {
    s32 x = parent->x;
    s32 y = parent->y;
    s32 z = parent->z;
    struct ObjectRuntime *object = Func_02004b30(673, x, y, z);

    if (object != NULL) {
      Func_02004b20(object, gData_02004f70);
      Func_02004b18(object, 1);
      ObjectDispatch_SetSingleChildFlags(object, 0);
      object->motionFlags = 0;
      object->scaleX = 0x20000;
      object->scaleY = 0x20000;
    }
  }
}
