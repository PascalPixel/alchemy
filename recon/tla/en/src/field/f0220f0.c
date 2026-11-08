#include "types.h"
#include <affine.h>
#include "sprite.h"
#include "iwram_call.h"

// The Lost Age's animation object: The Broken Seal's fields with the
// shadow's sprite moved after them.
struct PlacedObject {
  struct AnimationSpritePart part;
  s32 scale;
  u8 slot;
  u8 displayFlags;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  struct AnimationSpritePart shadow;
  struct AnimationEntry *entries[4];
};

// The object casts a shadow.
#define OBJECT_SHADOW 1

s32 Func_08021a84(struct PlacedObject *object, s32 mode);
void Func_080140d8(void *entry, s32 slot);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

// Places an animation object's sprite (and its shadow) at the screen point
// of a projected position (x, y, z, ground), scaled by scale[0] and
// scale[1]; the sprites go into the depth list by their depth.
void Render_ApplyProjectedPlacement(struct PlacedObject *object, s32 *pos,
                                    s32 *scale, s32 mode) {
  s32 matrix;
  u32 halfWidth;
  u32 halfHeight;
  s32 affine;
  s32 px;
  s32 py;
  s32 pz;
  s32 pw;
  s32 scaleX;
  s32 scaleY;
  // Unused: gives the stack frame its 24 bytes.
  s32 ground[3];
  s32 screen[3];
  s32 flip;
  struct AffineTransform effect;
  s32 anchorX;
  s32 anchorY;
  s32 x;
  s32 y;
  struct AnimationSpritePart *part;

  halfWidth = object->width >> 1;
  halfHeight = object->height >> 1;
  scaleX = *scale++;
  scaleY = *scale;
  px = *pos++;
  py = *pos++;
  pz = *pos++;
  pw = *pos;
  flip = Func_08021a84(object, mode);
  if (flip == 0 && scaleX == 0x10000 && scaleY == scaleX &&
      object->rotation == 0) {
    affine = 0;
    matrix = 0;
  } else {
    affine = 1;
    effect.angle = object->rotation;
    effect.scaleX = scaleX >> 8;
    effect.scaleY = scaleY >> 8;
    if (flip != 0)
      effect.scaleX = -effect.scaleX;
    matrix = AffineMatrix_BuildForEffect(&effect);
  }
  if (scaleX > 0x10000 || scaleY > 0x10000) {
    affine = 3;
    halfWidth <<= 1;
    halfHeight <<= 1;
  }

  // The shadow: an 8 by 4 sprite on the ground below the object.
  y = ((pz - pw) >> 16) - 4;
  if ((object->flags & OBJECT_SHADOW) && y < 160) {
    part = &object->shadow;
    part->affine = 0;
    part->x = (px >> 16) - 8;
    part->y = y;
    Func_080140d8(part, ((pz + pw) >> 16) + 60);
  }

  if (scaleX == 0 || scaleY == 0)
    return;

  // The anchor, scaled, and turned with the object.
  if (object->rotation == 0) {
    anchorX = object->offsetX * scaleX;
    anchorY = ((object->height >> 1) - object->offsetY) * scaleY;
  } else {
    s32 sin = Trig_Sin(object->rotation);
    s32 cos = Trig_Cos(object->rotation);
    s32 offsetX = object->offsetX * scaleX;
    s32 offsetY = ((object->height >> 1) - object->offsetY) * scaleY;

    anchorX = Iwram_MulQ16(cos, offsetX) + Iwram_MulQ16(sin, offsetY);
    anchorY = Iwram_MulQ16(cos, offsetY) - Iwram_MulQ16(sin, offsetX);
  }
  x = (px >> 16) - halfWidth + ((anchorX + 0xffff) >> 16);
  y = ((pz - py) >> 16) - halfHeight - ((anchorY + SPLIT_CONSTANT(0xff00, 255)) >> 16);
  if (x < 240 && y < 160) {
    part = &object->part;
    part->x = x;
    part->y = y;
    part->affine = affine;
    part->affineIndex = matrix;
    Func_080140d8(part, ((py + pz) >> 16) + 62);
  }
}
