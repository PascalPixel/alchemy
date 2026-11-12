#include "types.h"

/// One halfword read as a struct's field.
struct HalfwordField {
  s16 value;
};

extern s16 gData_02006868;
extern s16 gData_020068a0;

void Func_0200246c(s32 a, s32 b);

void Func_02001254(void) {
  s16 *state = &gData_02006868;

  if (gData_020068a0 < 60) {
    if (*state == 0) {
      Func_0200246c(0, 3);
      *state = 1;
    }
  } else {
    // Read through a struct, so PRE does not merge it with the first arm's
    // read.
    if (((struct HalfwordField *)state)->value == 1) {
      Func_0200246c(0, 67);
      *state = 0;
    }
  }
  if (++gData_020068a0 > 300)
    gData_020068a0 = 0;
}

#include "ride.h"
#include "event_inline.h"

void Func_020012bc(struct Actor *object) {
  switch (gFrameCount & 7) {
  case 0:
    Animation_ApplyChildValues(object, 0);
    break;
  case 6:
    Animation_ApplyChildValues(object, 10);
    break;
  }
}

#include "actor.h"

struct Sprite020012e4 {
  u8 unk00[18];
  u16 unk12;
};

void Func_020012e4(struct Actor *actor) {
  struct Sprite020012e4 *sprite = actor->sprite;

  sprite->unk12 += 0x800;
  if (++actor->timer == 8)
    actor->velocityY = 0x40000;
  if (actor->timer > 9) {
    actor->x += 0x40000;
    actor->scaleX -= 0x800;
    actor->scaleY -= 0x800;
  }
  if (actor->timer == 32) {
    actor->x = 0;
    actor->z = 0;
    actor->update = NULL;
  }
}
