// Draft, differ 66 of 174, skeleton gap 46 (tla-en, 2026-11-01)
// The update Func_02003e54 hooks: the object's frame count (byte 0x63) cycles
// its animation on odd frames; in state 0 (byte 0x62) every 16th frame starts
// one of the two sparkles with sound 598, alternating; in state 1, once both
// have run out, the object stops and frees its VRAM entry. Each running sparkle
// rises a pixel a frame for 32 frames above the object, its two sprites pulsing
// with the sine of its age, the second one a pixel lower and mirrored, drawn
// just in front of and behind the object.
// Left is allocation alone: the listing keeps work in r8 and the count of the
// loop in r11, with x and sine in stack slots; here sine and x hold r8 and r11
// and work and i spill (pseudo ranks: sine .51, x .33, work .24, i .115).
// The rest of the shape is settled: `i` counting up is reversed by loop.c into
// the listing's 1, 0 with its two givs, which is what reloads timers[i] for
// position[1]; `/ 2` and `% 16` keep the count's reloads; one pointer for the
// position and the sprite stops loop.c hoisting sp + 12; `j = 0` set this early
// is what gives j r10. Tried without gain: sine and x as an array, scaleY read
// from scaleX, reading position[0] in the inner loop, goto-built loops. A
// do { } while (0) around the block lowers differ to 51 by leaving x, z and j
// uninitialized on a path, which changes behaviour: rejected, but it says the
// listing's x, sine and j are ranked as if they lived longer than they do here.
#include "types.h"
#include "actor.h"

struct SparkleSprite {
  u8 unk00[0x0c];
  s32 x;
  s32 z;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  u16 depth;
  u8 unk20[8];
};

struct SparkleWork {
  struct SparkleSprite sprites[4];
  s16 timers[2];
  s16 vramEntry;
  u16 next;
  struct Actor *object;
};

void Animation_ApplyChildValues(struct Actor *object, u32 frame);
void Func_020042b4(s32 sound);
void Resource_ResetEntry(s32 entry);
s32 Math_Sine(s32 angle);
void Func_0200421c(s32 *position);
s32 Func_0200425c(struct Actor *object);
void Func_0200424c(struct SparkleSprite *sprite);

void Func_02003cd8(struct Actor *object) {
  struct SparkleWork *work = (struct SparkleWork *)object->watchedObject;
  u8 *count = (u8 *)object + 0x63;
  u8 state;
  struct SparkleSprite *sprite;

  s32 position[3];

  s32 sine;
  s32 x;
  s32 z;
  s32 i;
  s32 j;

  if (*count & 1)
    Animation_ApplyChildValues(object, (u8)((*count / 2) % 6));
  state = ((u8 *)object)[0x62];
  if (state == 0) {
    if (*count % 16 == 0) {
      Func_020042b4(598);
      work->timers[(s16)work->next] = 0;
      work->next ^= 1;
    }
  } else if (state == 1) {
    if (work->timers[0] > 31 && work->timers[1] > 31) {
      Animation_ApplyChildValues(object, 0);
      Resource_ResetEntry(work->vramEntry);
      object->update = 0;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    sine = Math_Sine(work->timers[i] << 10);
    if (work->timers[i] < 0)
      continue;
    if (work->timers[i] > 31)
      continue;
    j = 0;
    sprite = (struct SparkleSprite *)position;
    ((s32 *)sprite)[0] = object->x;
    ((s32 *)sprite)[1] = object->y + (work->timers[i] << 16);
    ((s32 *)sprite)[2] = object->z;
    Func_0200421c((s32 *)sprite);
    x = ((s32 *)sprite)[0];
    z = ((s32 *)sprite)[2];
    sprite = &work->sprites[i * 2];
    for (; j <= 1; j++, sprite++) {
      sprite->z = z;
      sprite->x = x;
      sprite->scaleX = sine;
      sprite->scaleY = sine;
      if (j == 0) {
        sprite->depth = Func_0200425c(object) - 1;
      } else {
        sprite->depth = Func_0200425c(object) + 1;
        sprite->z -= 0x10000;
        sprite->scaleY = -sprite->scaleY;
      }
      Func_0200424c(sprite);
    }
    work->timers[i]++;
  }
}
