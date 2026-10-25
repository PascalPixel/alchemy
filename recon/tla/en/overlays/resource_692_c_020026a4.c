// Draft, differ 26 of 122: a glow at cell (x, z): creates a blended object (type
// 288) there with sound 106, fades its blend in over 8 frames and out over 16
// inside the object window, then copies the 1 by 2 cells from 64 columns across
// (Func_02004cd8), clears the cell's high byte (Func_02000694) and removes the
// object.
#include "types.h"
#include "io_reg.h"
#include "object_runtime.h"
#include "field_sprite.h"

struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004e90(s32 value);
void Func_02004f30(s32 sound);
void WaitFrames(s32 frames);
void Func_02004cd8(s32 a, s32 b, s32 x, s32 z, s32 width, s32 height);
void Func_02000694(s32 layer, s32 x, s32 z, s32 mask);
void Func_02004cb0(struct ObjectRuntime *object);

void Func_020026a4(s32 x, s32 z) {
  struct ObjectRuntime *object;
  s32 i;
  s32 cellZ;

  object = Engine_ObjectCreate(288, x << 16, 0, z << 16);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f44;
  Object_SetSpritePriority(object, 0);
  ObjectDispatch_SetSingleChildFlags(object, 0);
  ((struct FieldSprite *)object->animation)->blendMode = 2;
  Func_02004d40();
  Func_02004e90(0);
  REG_WINOUT = 0x3f1f;
  REG_DISPCNT |= 0x8000;
  Func_02004f30(106);
  i = 0;
fadeIn:
  REG_BLDALPHA = (i << 9) | 16;
  WaitFrames(1);
  if (++i != 8)
    goto fadeIn;
  i = 0;
fadeOut:
  REG_BLDALPHA = (16 - i) | 0x1000;
  WaitFrames(1);
  if (++i != 16)
    goto fadeOut;
  x >>= 4;
  cellZ = (z >> 4) - 1;
  Func_02004cd8(65, 1, x + 64, cellZ, 1, 2);
  Func_02000694(0, x, cellZ, 0xff00);
  Func_02004cb0(object);
  Func_02004d48();
  REG_BLDCNT = 0;
  REG_DISPCNT &= 0x7fff;
}
