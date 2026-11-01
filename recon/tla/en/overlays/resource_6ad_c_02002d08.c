// Draft, differ 71 of 168, skeleton gap 29 (tla-en, 2026-11-01)
// Func_02002d08 of overlay 6ad, shared by 4 overlays: rolls five randoms for a
// new object near the actor, gives it velocity, scale and gravity, and copies the
// sprite settings of the work area's source object. gData_020023c4 + 0x188 is
// this overlay's address; the copies carry their own.
// Left: the source sprite pointer. The listing holds it in r8 (no spill, frame 4);
// here local-alloc puts it in r4 by caller-saves (its refs beat 4 * 2 calls), which
// gives frame 8, two stack accesses and the mov r8 uses. Sharing one local with x
// made it no better (gap 32), and that is a temporary device anyway.
// Left: a real for, do or while loop gives the listing's pool (mask -1024 first, in
// HImode, range 32, with the b over the pool), because loop.c drops the mask's SImode
// copy; but it also hoists half, mul, the zero and 63 out of the loop (gap 61).
// loop.c keeps a set in the loop when a jump or label sits between it and its
// last use (reg_in_basic_block_p), so the listing's body may have held a branch
// that later passes removed; none found. The goto loop here avoids hoisting, so
// the pool stays after the epilogue. A goto into a for body did not help.
#include "types.h"
#include "iwram_call.h"
#include "actor.h"

/// The sprite as this function copies it: FieldSprite's bytes, with bits 14
/// and 15 of attribute 1 as the one field the copy treats them as.
struct CopiedSprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[6];
  u8 vramBlock;
  u8 unk11;
  u16 rotation;
  u8 unk14;
  u8 unk15;
  u8 unk16[5];
  u8 count;
  u8 unk1c[12];
  void *record;
};

struct OverlayWork {
  u8 unk00[12];
  u16 unk0c;
  u8 unk0e[98];
  struct Actor *source;
};

extern u8 gData_020023c4[];
extern u8 gData_02005dbc[];

u32 Random16Far(void);
struct Actor *Func_02005498(s32 type, s32 x, s32 y, s32 z);
void Func_02005490(struct Actor *actor, void *data);
void Actor_SetSingleChildFlags(struct Actor *actor, s32 flags);
s32 Math_Cosine(u32 angle);
s32 Math_Sine(u32 angle);
s32 Resource_ResetEntry(u32 slot);
void ResourceMetadata_ClearRecord(void *destination);

void Func_02002d08(struct Actor *self) {
  struct OverlayWork *work = (struct OverlayWork *)(gData_020023c4 + 0x188);
  s32 i;

  if (work->source != NULL) {
    i = 0;
  next:
    {
      struct Actor *object;
      u32 a, b, c, d, e;
      s32 x, y, z;
      s32 half;

      a = Random16Far();
      b = Random16Far();
      x = self->x + (a << 4) - (b << 4);
      c = Random16Far();
      d = Random16Far();
      e = Random16Far();
      z = self->z + (d << 4) - (e << 4);
      y = (c << 3) + self->y + 0x40000;
      object = Func_02005498(489, x, y, z);
      if (object != NULL) {
        u32 angle = Random16Far();
        u32 speed = Random16Far();
        u32 scale = Random16Far() + Random16Far();
        s32 size;
        struct CopiedSprite *sprite;
        struct CopiedSprite *from;
        s32 velocity;

        half = 0x8000;
        Func_02005490(object, gData_02005dbc);
        Actor_SetSingleChildFlags(object, 0);
        object->velocityY = (angle << 2) + 0x20000;
        {
          s32 (*mul)(s32, s32);
          s32 cosine = Math_Cosine(angle);

          mul = Iwram_MulQ16;
          object->velocityZ = mul(speed << 3, cosine);
          velocity = mul(speed << 3, Math_Sine(angle));
        }
        object->gravity = 0x547a;
        size = (scale >> 2) + half;
        object->scaleX = size;
        object->scaleY = size;
        object->bounce = half;
        object->velocityX = velocity;
        object->acceleration = 0;
        sprite = object->animation;
        from = work->source->animation;
        Resource_ResetEntry(sprite->vramBlock);
        sprite->unk11 |= 1;
        sprite->vramBlock = work->unk0c;
        ResourceMetadata_ClearRecord(sprite->record);
        sprite->record = NULL;
        sprite->count = 0;
        sprite->unk14 = from->unk14;
        sprite->unk15 = from->unk15;
        sprite->shape = from->shape;
        sprite->size = from->size;
        sprite->tile = from->tile;
      }
    }
    if (--i < 0)
      goto done;
    goto next;
  done:;
  }
}
