// Draft, differ 82 of 262: the native spills shadowHalfWidth (sp+16) and
// keeps shadowHalfHeight in r11, here both take r9 and r10, which moves the
// spill slots and the rotated path's registers. Moving the rotated path's
// locals to function scope changes nothing.
// The sibling of Render_ApplyProjectedPlacement (0x080220f0) that
// takes the draw slot: the shadow is 16 by 8 pixels (32 by 16 at double
// size) and both sprites go into Oam_AddSpriteAtDepth's slot depth.
#include "types.h"
#include <affine.h>
#include "animspr.h"
#include "iwram_call.h"

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

#define OBJECT_SHADOW 1

s32 Func_08021a84(struct PlacedObject *object, s32 mode);
void Oam_AddSpriteAtDepth(void *entry, s32 slot);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Func_08022318(struct PlacedObject *object, s32 *pos, s32 *scale, s32 mode,
                   s32 depth) {
  s32 matrix;
  u32 halfWidth;
  u32 halfHeight;
  s32 shadowHalfWidth;
  s32 affine;
  s32 px;
  s32 py;
  s32 pz;
  s32 pw;
  s32 scaleX;
  s32 scaleY;
  s32 shadowHalfHeight;
  s32 ground[3];
  s32 screen[3];
  s32 flip;
  struct AffineTransform effect;
  s32 anchorX;
  s32 anchorY;
  s32 x;
  s32 y;

  halfWidth = object->width >> 1;
  shadowHalfHeight = 4;
  halfHeight = object->height >> 1;
  shadowHalfWidth = 8;
  px = *pos++;
  scaleX = *scale++;
  scaleY = *scale;
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
    shadowHalfWidth = 16;
    shadowHalfHeight = 8;
  }

  y = ((pz - pw) >> 16) - shadowHalfHeight;
  if ((object->flags & OBJECT_SHADOW) && y < 160) {
    struct AnimationSpritePart *shadow = &object->shadow;

    shadow->affine = 0;
    shadow->x = (px >> 16) - shadowHalfWidth;
    shadow->y = y;
    Oam_AddSpriteAtDepth(shadow, depth);
  }

  if (scaleX == 0 || scaleY == 0)
    return;

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
  y = ((pz - py) >> 16) - halfHeight - ((anchorY + 0xffff) >> 16);
  if (x < 240 && y < 160) {
    /* FAKEMATCH: the part is pinned to the r0 the native addresses its
     * attributes through, as in Render_ApplyProjectedPlacement. */
    register struct AnimationSpritePart *part asm("r0") = &object->part;

    part->x = x;
    part->y = y;
    part->affine = affine;
    part->affineIndex = matrix;
    Oam_AddSpriteAtDepth(part, depth);
  }
}
