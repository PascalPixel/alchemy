// Draft, differ 62 of 174, skeleton gap 21 (tla-en, 2026-11-01)
// The update Func_02003e54 hooks: the object's frame count (byte 0x63) cycles
// its animation on odd frames; in state 0 (byte 0x62) every 16th frame starts
// one of the two sparkles with sound 598, alternating; in state 1, once both
// have run out, the object stops and frees its VRAM entry. Each running sparkle
// rises a pixel a frame for 32 frames above the object, its two sprites pulsing
// with the sine of its age, the second one a pixel lower and mirrored, drawn
// just in front of and behind the object.
// Left is allocation alone, and every skeleton line follows from it: the listing
// keeps work in r8 and the loop's count in r11, with x and sine at sp+8 and
// sp+4; here sine and x hold r8 and r11 and work and the count spill. Slots go
// by pseudo number, so x is declared before sine. Ranks here (refs, live):
// sprite 6.4, timers .75, object .64, sine .51 (8, 47), j .48 (14, 88),
// z .34, x .33 (5, 30), work .24 (9, 113), count .115 (7, 122), giv .113. The
// listing needs work, j, z and the count all to beat x and sine, which no
// ordinary rank of x and sine does: sine would need to live past 200 insns.
// Compiler changed, so all of this was remeasured on 2026-11-01:
// - Memory x and sine (s32[2], s32[3], position[5], a five-word struct) hoist the
//   frame address, frame 32 and differ 160.
// - struct { s32 sine, x; } is DImode, low registers only, so it would explain
//   the stack slots with the count in r11, and it does live the whole loop. But
//   it has 17 refs (partial sets count twice), ranks 1.2 above timers, takes
//   r6/r7 and pushes object to sl: differ 163. It needs under about 14 refs to
//   fall behind timers; copying sine to a scale local, scaleY read from scaleX
//   and chained stores all leave 17. Best lead if a way to fewer refs turns up.
// - `j = 0` at the `for`, or between x and z where the listing shows it: j
//   overtakes object (.72), differ 137. Set this early it stays below object.
// - The one pointer for position and sprite is a reused local (S2 `temporary`):
//   a separate pointer or plain position[] both hoist sp + 12, differ 158. It
//   stays until something else keeps that address in the loop.
// - if-block for the range test 73; `& 15` for `% 16` 66; y term order 70;
//   sine as a loop-scope initializer, all locals in the loop, any declaration
//   order: unchanged, 62.
// A do { } while (0) around the block gave 51 before the compiler change by
// leaving x, z and j uninitialized on a path, which changes behaviour: rejected.
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

  s32 x;
  s32 sine;
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
      sprite->scaleX = sine;
      sprite->x = x;
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
