// Draft, differ 31 of 174 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Same function as overlay 694's Func_02003cd8, and that form closed most of
// the distance here. Each frame the object's count (byte 0x63) cycles its
// animation on odd counts; in state 0 (byte 0x62) every 16th count restarts
// one of the two sparkles with sound 598, alternating; in state 1, once both
// have run out, the object frees its VRAM entry and stops. Each running
// sparkle rises 32 pixels above the object, its two sprites pulsing with the
// sine of its age, the second a pixel lower, mirrored and one depth step
// behind. Settled shape: `*count / 2` is shortened to byte width, so its read
// stays apart from the `& 1` test's (`% 16` also beats `& 15`); `for (i = 0;
// i < 2; i++)` is reversed by loop.c into the listing's 1, 0 with its slot
// and offset givs; reading work->timers[i] through the struct is what rereads
// it after the s32 store (a plain s16 * does not conflict, an s16 pointer and
// goto stay at gap 24); p's default before the loop gives it two sets, so
// loop.c keeps p = position in the block; j = 0 this early is what gives j
// r10 (moved after the camera call it takes r7 and object falls to r8: differ
// 137). Left is allocation, and the `% 16` load and mask swapping r2 and r3.
// The listing has work r8, j r10, y r9 and i r11, with x, sine and offset on
// the stack; here sine r8, x r11 and work, i, offset on the stack. Ranks from
// lreg: sine 8 refs over 47 (.51), x 5 over 30 (.33), work 9 over 113 (.24),
// i 7 over 122 (.115). The listing needs sine and x as if they lived the
// whole loop, and work above j, which refs alone do not give (j 14 over 88,
// .48). Not found: what lengthens them; each is set on every pass, so flow
// kills them at the top. Tried without gain: `& 15`, s32 state, defaults for
// x, y, sine, j and sprite, sine and x as a two-word struct or array (a
// DImode pair takes r6 and r7, differ 163), position[] with no pointer (158).
// Overlay 694's own form shares one pointer for position and sprite, which
// also reaches differ 62: a local reused for a second value, not needed here.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "effect_sprite.h"

struct SparkleWork {
  struct EffectSprite sprites[4];
  s16 timers[2];
  s16 vramEntry;
  u16 next;
  struct Actor *object;
};

void Animation_ApplyChildValues(struct Actor *object, u32 frame);
void Func_02000f3c(s32 sound);
void Resource_ResetEntry(s32 entry);
s32 Math_Sine(s32 angle);
void Func_02000ec4(s32 *position);
s32 Func_02000efc(struct Actor *object);
void Func_02000eec(struct EffectSprite *sprite);

void Func_02000ad0(struct Actor *object) {
  struct SparkleWork *work = (struct SparkleWork *)object->watchedObject;
  u8 *count = (u8 *)object + 0x63;
  u8 state;
  struct EffectSprite *sprite;

  s32 position[3];
  s32 *p;

  s32 sine;
  s32 x;
  s32 y;
  s32 i;
  s32 j;

  if (*count & 1)
    Animation_ApplyChildValues(object, *count / 2 % 6);
  state = ((u8 *)object)[0x62];
  if (state == 0) {
    if (*count % 16 == 0) {
      Func_02000f3c(598);
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
  // Default: a second set keeps loop.c from hoisting p = position.
  p = 0;
  for (i = 0; i < 2; i++) {
    sine = Math_Sine(work->timers[i] << 10);
    if (work->timers[i] < 0)
      continue;
    if (work->timers[i] > 31)
      continue;
    j = 0;
    p = position;
    p[0] = object->x;
    p[1] = object->y + (work->timers[i] << 16);
    p[2] = object->z;
    Func_02000ec4(p);
    x = p[0];
    y = p[2];
    sprite = &work->sprites[i * 2];
    /* FAKEMATCH(barrier): the empty asm on sprite changes its allocation rank; as no
     * asm, 42 of 174 instructions differ in tla-en. */
    asm("" : : "r"(sprite));
    for (; j <= 1; j++, sprite++) {
      /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
       * order around it; as the plain statement, 45 of 174 instructions differ
       * in tla-en. */
      {
        sprite->y = y;
      }
      sprite->x = x;
      sprite->scaleX = sine;
      sprite->scaleY = sine;
      if (j == 0) {
        sprite->depth = Func_02000efc(object) - 1;
      } else {
        sprite->depth = Func_02000efc(object) + 1;
        sprite->y -= 0x10000;
        sprite->scaleY = -sprite->scaleY;
      }
      Func_02000eec(sprite);
    }
    work->timers[i]++;
  }
}
