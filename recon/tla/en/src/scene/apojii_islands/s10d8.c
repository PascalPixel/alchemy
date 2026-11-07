#include "types.h"
#include "actor.h"
#include "field_sprite.h"

extern const u8 gData_02001ee8[];

struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Animation_ApplyChildArgument(struct FieldSprite *sprite, s32 value);
void Func_02001c74(struct Actor *object, const u8 *script);

void Func_020010d8(struct Actor *object) {
  struct Actor *child;
  struct FieldSprite *parent;
  struct FieldSprite *sprite;

  child = Import_ActorCreate(14, object->x, object->y + 0x40000,
                              object->z + 0x40000);
  parent = object->animation;
  if (child != NULL) {
    sprite = child->animation;
    child->terrainHeight = object->terrainHeight;
    Func_02001c74(child, gData_02001ee8);
    child->motionFlags = 0;
    if (sprite != NULL) {
      Animation_ApplyChildArgument(sprite, 1);
      sprite->nextFrame = 0;
      sprite->priority = parent->priority;
    }
  }
}
