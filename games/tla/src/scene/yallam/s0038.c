#include "types.h"
#include "actor.h"

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);

struct TileSprite {
  u8 unk00[18];
  u16 tile;
};

s32 Func_02000038(struct Actor *object) {
  struct TileSprite *sprite = object->animation;

  sprite->tile = 0x8000;
  return 0;
}

s32 Func_02000044(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}
