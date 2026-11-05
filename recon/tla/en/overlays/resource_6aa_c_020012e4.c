// Draft, rewritten as C from the listing's control flow (tla-en, 5 November 2026).
#include "types.h"
#include "actor.h"

struct Sprite020012e4 {
  u8 unk00[18];
  u16 unk12;
};

void Func_020012e4(struct Actor *actor) {
  struct Sprite020012e4 *sprite = actor->animation;
  s16 *action;

  sprite->unk12 += 0x800;
  action = &actor->action;
  if (++*action == 8)
    actor->velocityY = 0x40000;
  if (*action > 9) {
    actor->x += 0x40000;
    actor->scaleX -= 0x800;
    actor->scaleY -= 0x800;
  }
  if (*action == 32) {
    actor->x = 0;
    actor->z = 0;
    actor->update = NULL;
  }
}
