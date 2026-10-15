#include "types.h"
#include "field_scene.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct TileSprite {
  u8 unk00[18];
  u16 tile;
  u8 unk14[6];
  u8 frame;
};

s32 Scene_PrepareMap(void) { return 0; }

s32 Func_02000c9c(struct ObjectRuntime *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);
  struct TileSprite *sprite = object->animation;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}
