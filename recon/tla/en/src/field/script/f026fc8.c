#include "types.h"
#include "actor.h"
#include "actor_view.h"

struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
extern const s32 gData_0802ec88[];

struct Child {
  u8 unk00[9];
  u8 low9 : 2;
  u8 layer : 2;
  u8 high9 : 4;
  u8 unk0a[0x10];
  u8 value1a;
};

void Func_08026fc8(struct Actor *object, s32 mode) {
  struct Actor *created;
  struct Child *sprite;

  created = Func_08023220(14, object->x, object->y + 0x40000, object->z);
  sprite = object->animation;
  if (!(object->unk66 & 1))
    Sound_PlayFar(623);
  if (created != NULL) {
    struct Child *child = created->animation;

    created->terrainHeight = object->terrainHeight;
    Actor_StartScript((struct ActorView *)created, gData_0802ec88);
    created->motionFlags = 0;
    if (child != NULL) {
      Animation_ApplyChildArgument((struct ActorSpriteView *)child, 1);
      child->value1a = 0;
      child->layer = sprite->layer;
      if (mode == 2) {
        Animation_ApplyChildValuesToRecord((struct ActorSpriteView *)child, 13);
        Animation_ApplyChildValue((struct ActorSpriteView *)child, 8);
      }
    }
  }
}
