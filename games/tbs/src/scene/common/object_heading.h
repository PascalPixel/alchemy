#include "types.h"
#include "field_obj.h"

static s32 OverlayObject_UpdateHeadingTimer(struct FieldActor *obj) {
  s32 cnt = obj->work;
  s32 loaded = (s16)obj->work;
  s32 amount;

  if (loaded == 0) {
    obj->facing += (u32)(Engine_RandomNext() << 15) >> 16;
    amount = (u32)(Engine_RandomNext() * 80) >> 16;
    obj->work = amount;
    if (amount == 0) {
      goto done;
    }
    cnt = amount;
  }
  obj->work = cnt - 1;
done:
  return 1;
}
