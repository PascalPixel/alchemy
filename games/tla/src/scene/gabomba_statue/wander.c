#include "types.h"
#include "event_inline.h"

struct Wanderer {
  u8 unk00[6];
  u16 facing;
  u8 unk08[90];
  u8 timer;
  u8 turned;
  u16 facingA;
  u16 facingB;
};

u32 Random16Far(void);

void Func_02000280(void) { Func_080d2b4c(33, 9); }
s32 Func_0200028c(struct Wanderer *actor) {
  u8 *timer = &actor->timer;

  if (*timer != 0) {
    (*timer)--;
  } else {
    u8 *turned;
    u16 *facing;

    *timer = (Random16Far() * 40 >> 16) + 10;
    /* FAKEMATCH: ⚓️ stores the new time before it addresses the turn flag */
    do {
    } while (0);
    turned = &actor->turned;
    if (*turned != 0) {
      *turned = 0;
      /* FAKEMATCH: and clears the flag before it addresses the facing */
      do {
      } while (0);
      facing = &actor->facingA;
    } else {
      *turned = 1;
      facing = &actor->facingB;
    }
    actor->facing = *facing;
  }
  return 1;
}
