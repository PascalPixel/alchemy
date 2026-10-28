#include "types.h"
#include "object_runtime.h"
#include "object_dispatch.h"

struct ObjectRuntime *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
extern const s32 gData_0802ec88[];

struct Child {
  u8 unk00[9];
  u8 low9 : 2;
  u8 layer : 2;
  u8 high9 : 4;
  u8 unk0a[0x10];
  u8 value1a;
};

void Func_08026fc8(struct ObjectRuntime *object, s32 mode) {
  struct ObjectRuntime *created;
  struct Child *sprite;

  created = Func_08023220(14, object->x, object->y + 0x40000, object->z);
  sprite = object->animation;
  if (!(object->unk66 & 1))
    Audio_PlayCue(623);
  if (created != NULL) {
    struct Child *child = created->animation;

    created->terrainHeight = object->terrainHeight;
    ObjectDispatch_Initialize((struct DispatchObject *)created, gData_0802ec88);
    created->motionFlags = 0;
    if (child != NULL) {
      Animation_ApplyChildArgument((struct DispatchChild *)child, 1);
      child->value1a = 0;
      child->layer = sprite->layer;
      if (mode == 2) {
        Animation_ApplyChildValuesToRecord((struct DispatchChild *)child, 13);
        Animation_ApplyChildValue((struct DispatchChild *)child, 8);
      }
    }
  }
}
