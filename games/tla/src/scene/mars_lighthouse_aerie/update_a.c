#include "mars_lighthouse_aerie.h"

void Func_02000038(struct Actor *object) {
  u8 *timer = &object->unk5c[6];

  if (*timer != 0) {
    (*timer)--;
  } else {
    *timer = (((u32)Random16Far() * 80) >> 16) + 60;
    object->angle = Random16Far();
  }
}

void Func_02000068(struct Actor *object) {
  if ((gFrameCount >> 1) & 1)
    Actor_SetPartAttribute(object, 15);
  else
    Actor_SetPartAttribute(object, 0);
}

/// Shrinks the object toward its normal size, and when its timer runs out
/// restarts it at 40 to 119 frames and swells it again.
void Func_0200008c(struct Actor *object) {
  u8 *timer;
  s32 scale = object->scaleX;

  /* FAKEMATCH: a one-pass block loads the scale before the bound is built (one
   * reordered row). */
  do {
  } while (0);
  if (scale > 0x10000) {
    object->scaleX = scale - 0x800;
    object->scaleY -= 0x400;
  }
  timer = &object->unk5c[6];
  if (*timer != 0) {
    (*timer)--;
  } else {
    u32 r = Random16Far();
    /* FAKEMATCH: the random number times 4 apart from times 80, and the timer
     * read in a one-pass block, read the timer between the multiply's two
     * halves; as one expression it is read after (differ 2). */
    u32 r4 = r << 2;
    u8 t;

    do {
      t = *timer;
    } while (0);
    *timer = t + (((r4 + r) * 16) >> 16) + 40;
    object->scaleX = 0x12000;
    object->scaleY = 0x11000;
  }
}

/// Circles the object around actor 8: each frame it steps its angle back by
/// 0x800 and places itself at the anchor's x and z plus the cosine and sine
/// times two radii read from its speed limit and its counter byte.
// The twin of ☀️'s SceneEffect_UpdateCounterDrivenOrbit.
void Func_020000e4(struct Actor *object) {
  struct Actor *anchor = Actor_GetById(8);
  u16 *pangle = (u16 *)&object->action;
  s32 angle = *pangle;
  s32 cosine;
  s32 sine;
  s32 along;
  s32 across;
  u8 *counter;

  cosine = Math_Cosine(angle);
  counter = &object->unk5c[6];
  {
    s32 radius = object->speedLimit + *counter;
    /* FAKEMATCH(pin): the anchor copied into r1 for its x; as anchor->x read
     * straight, 2 of 48 instructions differ in tla-en. */
    register struct Actor *a asm("r1");

    a = anchor;
    /* FAKEMATCH(barrier): an asm reading the copy, so the copy comes before
     * the radius's + 6; as anchor->x read straight, 2 of 48 instructions
     * differ in tla-en. */
    asm("" : : "l"(a));
    along = a->x + cosine * (radius + 6);
  }
  object->x = along;
  sine = Math_Sine(angle);
  across = anchor->z + sine * (*counter + 4);
  object->z = across;
  object->targetX = object->x;
  object->targetZ = across;
  *pangle -= 0x800;
}
