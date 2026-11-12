/// @file
/// Scene code of overlay 0x670, in ROM order, from s0038's offset up to the next object the overlay links.

#include "kibombo.h"

s32 Func_02000038(struct KibomboObject *object) {
  object->sprite->rotation += 0x600;
  return 0;
}

s32 Func_02000048(struct KibomboObject *object) {
  object->field44 = object->x;
  object->field48 = object->y;
  return 0;
}

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

    Actor_SetPositionAndResetMotion(object, object->field44 + dx,
                                     object->field48 + dy, object->z);
  }
  return 1;
}

void Func_020000a0(struct KibomboObject *object) {
  if (gFrameCount & 2)
    Actor_SetPartAttribute(object, 10);
  else
    Actor_SetPartAttribute(object, 0);
}

s32 Func_020000c4(struct KibomboObject *object) {
  if (object->timer != 0) {
    object->timer--;
  } else {
    object->timer = ((Random16Far() * 10) >> 16) + 10;
    object->facing = Random16Far();
  }
  return 1;
}

