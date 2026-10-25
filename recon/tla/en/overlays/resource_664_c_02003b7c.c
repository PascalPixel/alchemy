// Draft, differ 34 of 122: a marker of type 288 at (x, y) blends in over 8
// frames and out over 16 inside an event with cue 106, then its 1 by 2
// cells are copied from (65, 1) to the marked place, the cell's kind is
// set to 0xff00 and the marker removed. The same listing as overlay 692's
// Func_020026a4. Left: the native splits its literal pool across the fades
// (pc offsets), keeps the cell arguments' order and loads the first blend
// value later.
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "system/io_reg.h"

struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void Func_02005f94(s32 cue);
void WaitFrames(s32 frames);
void Func_02005d9c(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_0200062c(s32 layer, s32 x, s32 z, s32 cell);
void Func_02005d7c(struct ObjectRuntime *object);

void Func_02003b7c(s32 x, s32 y) {
  struct ObjectRuntime *marker;
  struct FieldSprite *sprite;
  s32 i, cellX, cellY;

  marker = Engine_ObjectCreate(288, x << 16, 0, y << 16);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f44;
  Object_SetSpritePriority(marker, 0);
  ObjectDispatch_SetSingleChildFlags(marker, 0);
  sprite = marker->animation;
  sprite->blendMode = 2;
  Func_02005e14();
  Func_02005f2c(0);
  REG_WINOUT = 0x3f1f;
  REG_DISPCNT |= 0x8000;
  Func_02005f94(106);
  i = 0;
fadeIn:
  REG_BLDALPHA = (i << 9) | 16;
  i++;
  WaitFrames(1);
  if (i != 8)
    goto fadeIn;
  i = 0;
fadeOut:
  REG_BLDALPHA = (16 - i) | 0x1000;
  i++;
  WaitFrames(1);
  if (i != 16)
    goto fadeOut;
  cellX = x >> 4;
  cellY = (y >> 4) - 1;
  Func_02005d9c(65, 1, cellX + 64, cellY, 1, 2);
  Func_0200062c(0, cellX, cellY, 0xff00);
  Func_02005d7c(marker);
  Func_02005e1c();
  REG_BLDCNT = 0;
  REG_DISPCNT &= 0x7fff;
}
