#include "types.h"

// The fixed camera's object placement: The Lost Age places each object in a
// function of its own, the body of ☀️'s ObjectSystem_UpdateCameraFixed loop.

struct PlacedObject {
  u8 unk00[6];
  u16 angle;
  s32 position[4];
  s32 scale[2];
  u8 unk20[2];
  u8 layer;
  u8 flags;
  u8 unk24[0x2c];
  void *target;
  u8 kind;
};

s32 GameFlag_Test(s32 flag);
void Func_0802254c(void *sprite, s32 *point, const s32 *scale, s32 facing,
                   s32 depth);
extern const s32 gData_0802eb90[2];

// Places an object's sprite (kind 1, unless flag 0x10 hides it) or each of its
// group's four sprites (kind 2), turned by angle. While battle commands are
// chosen (flag 363) the sprites are drawn at 1.0 scale.
void Func_08023d80(struct PlacedObject *object, s32 angle) {
  const s32 *scale = object->scale;
  s32 *point;
  void *sprite;
  void **sprites;
  s32 remaining;

  if (GameFlag_Test(363))
    scale = gData_0802eb90;
  switch (object->kind & 15) {
  case 1:
    point = object->position;
    sprite = object->target;
    if (!(object->flags & 0x10))
      Func_0802254c(sprite, point, scale, object->angle + angle, object->layer);
    break;
  case 2:
    sprites = object->target;
    for (remaining = 3; remaining >= 0; remaining--) {
      sprite = *sprites++;
      if (sprite != NULL)
        Func_0802254c(sprite, object->position, scale, object->angle + angle,
                      object->layer);
    }
    break;
  case 0:
    break;
  }
}
