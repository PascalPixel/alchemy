// Draft, differ 205 of 174: the update Func_02003e54 hooks: the object's frame
// count (byte 0x63) cycles its animation on odd frames; in state 0 (byte 0x62)
// every 16th frame starts one of the two sparkles with sound 598, alternating;
// in state 1, once both have run out, the object stops and frees its VRAM
// entry. Each running sparkle rises a pixel a frame for 32 frames above the
// object, its two sprites pulsing with the sine of its age, the second one a
// pixel lower and mirrored, drawn just in front of and behind the object.
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
  s16 *timer;
  s32 position[3];
  s32 offset;
  s32 sine;
  s32 x;
  s32 z;
  s32 i;
  s32 j;

  if (*count & 1)
    Animation_ApplyChildValues(object, (u8)((*count >> 1) % 6));
  state = ((u8 *)object)[0x62];
  if (state == 0) {
    if ((*count & 15) == 0) {
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
  offset = 0;
  timer = work->timers;
  for (i = 1; i >= 0; i--, offset += 80, timer++) {
    sine = Math_Sine(*timer << 10);
    if (*timer < 0 || *timer > 31)
      continue;
    position[0] = object->x;
    position[1] = object->y + (*timer << 16);
    position[2] = object->z;
    Func_0200421c(position);
    x = position[0];
    z = position[2];
    sprite = (struct SparkleSprite *)((u8 *)work + offset);
    for (j = 0; j <= 1; j++, sprite++) {
      sprite->z = z;
      sprite->x = x;
      sprite->scaleX = sine;
      sprite->scaleY = sine;
      if (j == 0) {
        sprite->depth = Func_0200425c(object) - 1;
      } else {
        sprite->z -= 0x10000;
        sprite->depth = Func_0200425c(object) + 1;
        sprite->scaleY = -sprite->scaleY;
      }
      Func_0200424c(sprite);
    }
    (*timer)++;
  }
}
