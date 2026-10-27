#include "types.h"
#include "object_runtime.h"

void WaitTicks(s32 ticks);

struct TileSprite {
  u8 unk00[18];
  u16 tile;
};

void Func_020003cc(struct ObjectRuntime *object) {
  struct TileSprite *sprite = object->animation;

  sprite->tile -= 0x800;
}

void Func_020003dc(struct ObjectRuntime *object, s32 height) {
  s32 count = 60;

  while (count != 0) {
    WaitTicks(1);
    count--;
    if (object->y <= height)
      break;
  }
}
