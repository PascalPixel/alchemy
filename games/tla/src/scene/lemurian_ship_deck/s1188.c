#include "types.h"
#include "actor.h"
#include "macros.h"

extern const u8 gData_0200247c[];

s32 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);
struct Actor *Func_02001f0c(s32 type, s32 x, s32 y, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02001efc(struct Actor *object, s32 value);
void Func_02001f04(struct Actor *object, const void *data);
void Func_02001f24(struct Actor *object, s32 x, s32 y, s32 z);

void Func_02001188(void) {
  s32 position[3];
  struct Actor *object;

  SET_VECTOR(position, 0x1680000, 0x400000, 0x1a80000);
  Vector_AddPolarOffsetFar(0x200000, Random16Far(), position);
  object = Func_02001f0c(0x2a2, position[0], position[1], position[2]);
  if (object != NULL) {
    object->motionFlags = 0;
    *(s32 *)((u8 *)object + 0x30) = 0x20000;
    *(s32 *)((u8 *)object + 0x34) = 0x8000;
    Actor_SetSingleChildFlags(object, 0);
    Func_02001efc(object, 0);
    Func_02001f24(object, 0x1680000, 0x400000, 0x1a80000);
    Func_02001f04(object, gData_0200247c);
  }
}
