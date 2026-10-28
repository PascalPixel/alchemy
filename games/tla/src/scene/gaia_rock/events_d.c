#include "types.h"
#include "actor.h"
#include "frame.h"

struct Actor *Actor_GetById(s32 id);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Func_02000e24(struct Actor *object, s32 a);
void Func_02000e2c(struct Actor *object, const u8 *script);
void Func_02000e3c(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02000e5c(struct Actor *object, s32 a);
void Func_02000f1c(struct Actor *object, s32 a);

extern const u8 gData_020015c4[];

void Func_02000670(void) {
  struct Actor *object = Actor_GetById(17);
  u8 phase = gFrameCount;

  if (phase == 0) {
    Func_02000f1c(object, 202);
    object = Actor_Spawn(0x118, object->x, object->y, object->z);
    if (object != NULL) {
      object->acceleration = 0x20000;
      object->speedLimit = 0x20000;
      object->scaleX = 0x8000;
      object->scaleY = 0x1999;
      object->priorityFlags = phase;
      Func_02000e24(object, 2);
      Func_02000e3c(object, object->x - 0x200000, object->y, object->z);
      Func_02000e2c(object, gData_020015c4);
      Func_02000e5c(object, 2);
    }
  }
}
