// Matches: differ 0 of 174 (tla-en, 2026-10-30), with one tagged one-pass block
// after the sprite mode store (the remaining site the 26 October notes left).
// Pushing a block through sand: on even frames a dust effect rises at the
// block, thrown faster while a key is held, then the block steps on and is
// pulled toward its cell centre across the push. gInput is read as
// input.h's InputState (held at offset 0).
// Earlier (26 October 2026, under -mthumb-slow-loads): differ 4 of 174, the
// native issuing gInput's address load one slot later after a store, where
// alchemy-gcc issues it at once (two sites); an empty asm after the sprite
// mode store also matches, but the one-pass block is the milder device;
// volatile on the sprite pointer or on the gInput read leaves 2; alchemy
// permute (5 to 6 minutes) found nothing.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"
#include "input.h"

#define FIELD32(object, offset) (*(s32 *)((u8 *)(object) + (offset)))

extern u32 gFrameCount;
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
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  if (gMainRoutineWork.renderMode != 5) {
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
        /* FAKEMATCH(one-pass): the loop note after the sprite store keeps
         * gInput's address load after it; as the store alone, 2 of 174
         * instructions differ in tla-en. */
        do {
        } while (0);
        if (gInput.held != 0) {
          FIELD32(dust, 68) =
              Math_Cosine(((Random16Far() << 3) >> 16) << 14) + dx;
          FIELD32(dust, 76) =
              Math_Sine(((Random16Far() << 3) >> 16) << 14) + dz;
          if (gInput.held & 2) {
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
    if (!(gInput.held & 240)) {
      s32 z = object->z >> 16;

      object->z += ((8 - z % 16) << 16) >> 3;
    }
  } else {
    object->z += dz;
    if (!(gInput.held & 240)) {
      s32 x = object->x >> 16;

      object->x += ((8 - x % 16) << 16) >> 3;
    }
  }
}
