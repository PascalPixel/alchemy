#include "kibombo.h"

extern u8 gData_0200635c[];
extern s16 gData_02007948[];
extern struct KibomboObject *gData_02007950;

void Func_02001318(void);
s32 Func_02002b08(s32 value);
void Func_02002b18(s32 index, s32 entry, s32 delta);
void Func_02002ba4(s32 index, s32 entry, s32 delta);

struct KibomboWork90 {
  u8 unk00[0x18];
  struct KibomboObject *leader;
};

s32 Func_02000054(struct KibomboObject *object) {
  if (gFrameCount & 1) {
    s32 dx;
    s32 dy;

    dx = -0x10000;
    dy = dx;
    dx += ((Random16Far() * 3) >> 16) << 16;
    dy += ((Random16Far() * 3) >> 16) << 16;

    Object_SetPositionAndResetMotion(object, object->field44 + dx,
                                     object->field48 + dy, object->z);
  }
  return 1;
}
