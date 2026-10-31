// Near miss, differ 2 of 258 (tla-en, 2026-10-30): one reordered pair, the
// native's `movs r3, #63` before `ldrb r2, [r0, #7]` (the affine index's
// read-modify-write right after matrix's masked value is stored back to
// sp+28). sched2 orders the pair as the native does not: the ROM waits before
// that byte load after the store, while -mthumb-slow-loads exempts an
// extended load and one that opens a read-modify-write, so it ranks first.
// No source form moves it (store order, matrix or affine unsigned, x and y
// swapped; 5,792 permuter candidates); the rest is exact. The pin is gone:
// one `part` for the shadow and the image, as in The Broken Seal, takes r0
// by itself (differ 2 with or without the pin).
// Earlier: differ 4 of 258 under -mthumb-slow-loads (2026-10-26); no single
// empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brought it closer (device search).
// Near miss of Render_ApplyProjectedPlacement: differ 8 of 258 (four
// reordered pairs; 214 before this pass, 332 as The Broken Seal's C). The
// Broken Seal's function of the same name
// (games/tbs/src/field/field/sprite_draw.c) as The
// Lost Age reworked it: the object keeps its shadow's sprite after its own
// fields (so the shared AnimationObject does not fit), the shadow is an 8 by 4
// sprite at a fixed offset, the sprite's anchor turns with the object (sine and
// cosine of its rotation), a zero scale draws nothing, and both sprites go into
// the depth list by their depth (Func_080140d8). The twin's two unused arrays
// stay: they make the native 64-byte stack frame.

// The allocation came from the locals declared in the order of the native's
// spill slots (matrix at sp+28 down to pz at sp+4) and the anchor products
// written value times scale (differ 192 to 30).

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
  /* FAKEMATCH(dead): two unused arrays, as in The Broken Seal, for 24 bytes
   * of the native 64-byte stack frame; without them, 6 of 258 instructions
   * differ in tla-en. */
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
  y = ((pz - py) >> 16) - halfHeight - ((anchorY + 0xffff) >> 16);
  if (x < 240 && y < 160) {
    part = &object->part;
    part->x = x;
    part->y = y;
    part->affine = affine;
    part->affineIndex = matrix;
    Func_080140d8(part, ((py + pz) >> 16) + 62);
  }
}
