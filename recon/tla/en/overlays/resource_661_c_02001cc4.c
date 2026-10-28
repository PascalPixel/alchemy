// 26 October 2026, under -mthumb-slow-loads: differ 4 of 174: the native issues
// a load one slot later after a store, where alchemy-gcc issues it at once (two
// sites; an empty asm after the sprite mode store fixes one); alchemy permute
// (5 to 6 minutes) and a second asm or block does not move it.
// Draft, differ 4 of 174: pushing a block through sand: on even frames a
// dust effect rises at the block, thrown faster while a key is held, then the
// block steps on and is pulled toward its cell centre across the push. Left:
// the native loads gInput's address after the sprite store and before the
// second test's mask (a one-pass block around the sprite store leaves 2).
#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "call.h"

#define FIELD32(object, offset) (*(s32 *)((u8 *)(object) + (offset)))

extern u32 gFrameCount;
extern s32 gInput;
extern u32 gData_020042f0[];

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
struct Actor *Func_02003fc0(s32 kind, s32 x, s32 y, s32 z);
void Func_02003fa8(struct Actor *object, s32 mode);
void Func_02003fb8(struct Actor *object, const void *script);
void Effect_AdvanceMotionAtLeaderPriority(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);

struct DustSprite {
  u8 unk00[9];
  u8 unk09 : 2;
  u8 mode : 2;
  u8 unk09High : 4;
};

void Func_02001cc4(struct Actor *object, s32 dx, s32 dz) {
  Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                     0);
  if (gSequenceParentWork.renderMode != 5) {
    u32 odd = gFrameCount & 1;

    if (odd == 0) {
      struct Actor *dust = (struct Actor *)Value4(
          (s32 (*)())Func_02003fc0, 285, object->x, object->y, object->z);

      if (dust != NULL) {
        struct DustSprite *sprite;

        dust->motionFlags = odd;
        dust->collisionFlags = odd;
        Actor_SetSingleChildFlags(dust, 0);
        Func_02003fa8(dust, 1);
        Func_02003fb8(dust, gData_020042f0);
        sprite = dust->animation;
        sprite->mode = 1;
        if (gInput != 0) {
          FIELD32(dust, 68) =
              Math_Cosine(((Random16Far() << 3) >> 16) << 14) + dx;
          FIELD32(dust, 76) =
              Math_Sine(((Random16Far() << 3) >> 16) << 14) + dz;
          if (gInput & 2) {
            FIELD32(dust, 68) += dx;
            FIELD32(dust, 76) += dz;
          }
        } else {
          FIELD32(dust, 68) =
              (Math_Cosine(((Random16Far() << 3) >> 16) << 14) >> 1) + dx;
          FIELD32(dust, 76) =
              (Math_Sine(((Random16Far() << 3) >> 16) << 14) >> 1) + dz;
        }
        FIELD32(dust, 72) = 0;
        dust->action = ((Random16Far() << 9) >> 16) - 256;
        dust->speedLimit = 0;
        dust->acceleration = 0;
        dust->update = Effect_AdvanceMotionAtLeaderPriority;
      }
    }
  }
  if (dx != 0) {
    object->x += dx;
    if (!(gInput & 240)) {
      s32 z = object->z >> 16;

      object->z += ((8 - z % 16) << 16) >> 3;
    }
  } else {
    object->z += dz;
    if (!(gInput & 240)) {
      s32 x = object->x >> 16;

      object->x += ((8 - x % 16) << 16) >> 3;
    }
  }
}
