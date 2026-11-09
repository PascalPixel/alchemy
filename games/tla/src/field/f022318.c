#include "types.h"
#include <affine.h>
#include "sprite.h"
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

static inline u8 PlacedObject_Height(struct PlacedObject *object) {
  return object->height;
}

s32 Func_08021a84(struct PlacedObject *object, s32 mode);
void Oam_AddSpriteAtDepth(void *entry, s32 slot);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Func_08022318(struct PlacedObject *object, s32 *pos, s32 *scale, s32 mode,
                   s32 depth) {
  s32 matrix;
  u32 halfWidth;
  u32 halfHeight;
  s32 extentX;
  s32 affine;
  s32 px;
  s32 py;
  s32 pz;
  s32 pw;
  s32 scaleX;
  s32 scaleY;
  s32 extentY;
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
  /* FAKEMATCH(wrapper): the height read through an inline; as object->height,
   * 4 of 262 instructions differ in tla-en. */
  halfHeight = PlacedObject_Height(object) >> 1;
  extentX = 8;
  extentY = 4;
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
    extentX = 16;
    extentY = 8;
  }

  y = ((pz - pw) >> 16) - extentY;
  if ((object->flags & OBJECT_SHADOW) && y < 160) {
    part = &object->shadow;
    part->affine = 0;
    part->x = (px >> 16) - extentX;
    part->y = y;
    Oam_AddSpriteAtDepth(part, depth);
  }

  if (scaleX == 0 || scaleY == 0)
    return;

  if (object->rotation == 0) {
    anchorX = object->offsetX * scaleX;
    anchorY = ((object->height >> 1) - object->offsetY) * scaleY;
  } else {
    s32 sin = Trig_Sin(object->rotation);
    s32 cos = Trig_Cos(object->rotation);

    // extentX and extentY are reused for the anchor's offset; locals of their
    // own differ.
    extentX = object->offsetX * scaleX;
    /* FAKEMATCH(wrapper): the height read through an inline; as object->height,
     * 2 of 262 instructions differ in tla-en. */
    extentY = ((PlacedObject_Height(object) >> 1) - object->offsetY) * scaleY;

    anchorX = Iwram_MulQ16(cos, extentX) + Iwram_MulQ16(sin, extentY);
    anchorY = Iwram_MulQ16(cos, extentY) - Iwram_MulQ16(sin, extentX);
  }
  x = (px >> 16) - halfWidth + ((anchorX + 0xffff) >> 16);
  y = ((pz - py) >> 16) - halfHeight - ((anchorY + 0xffff) >> 16);
  if (x < 240 && y < 160) {
    part = &object->part;
    part->x = x;
    part->y = y;
    part->affine = affine;
    part->affineIndex = matrix;
    Oam_AddSpriteAtDepth(part, depth);
  }
}
