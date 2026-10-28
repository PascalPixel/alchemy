#include "types.h"
#include "actor.h"

void wait(s32 frames);

struct TileSprite {
  u8 unk00[18];
  u16 tile;
};

void Func_020003cc(struct Actor *object) {
  struct TileSprite *sprite = object->animation;

  sprite->tile -= 0x800;
}

void Func_020003dc(struct Actor *object, s32 height) {
  s32 count = 60;

  while (count != 0) {
    wait(1);
    count--;
    if (object->y <= height)
      break;
  }
}
