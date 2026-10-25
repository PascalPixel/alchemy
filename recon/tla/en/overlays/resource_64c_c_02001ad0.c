// Draft, differ 71 of 120: creates the two halves of a flash (sprite 0x10f)
// over parent and gives them Func_02001a80 and Func_02001a34. Left: register
// choice (the native keeps the array base in r8 and the zero in r10) and the
// order of the field stores after the create.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "vram_tab.h"

struct ObjectRuntime *Func_02001fcc(s32 sprite, s32 x, s32 y, s32 z);
void Animation_ApplyChildArgument(struct FieldSprite *sprite, s32 argument);
void Resource_ResetEntry(s32 entry);
void Func_02001a34(struct ObjectRuntime *object);
void Func_02001a80(struct ObjectRuntime *object);

void Func_02001ad0(struct ObjectRuntime *parent) {
  struct ObjectRuntime *objects[2];
  u8 *effect = Ram_HeapSlots->effectWork;
  struct FieldSprite *sprite;
  s32 i;

  for (i = 0; i <= 1; i++) {
    struct ObjectRuntime *object;

    object = Func_02001fcc(0x10f, parent->x, parent->y, parent->z);
    objects[i] = object;
    if (object == NULL)
      continue;
    sprite = object->animation;
    object->terrainHeight = parent->terrainHeight;
    object->motionFlags = 0;
    object->action = 0;
    object->linkedObject = parent;
    if (sprite == NULL)
      continue;
    Animation_ApplyChildArgument(sprite, 0);
    sprite->nextFrame = 0;
    Resource_ResetEntry(sprite->vramBlock);
    sprite->vramBlock = *(u16 *)(effect + 0x7c0);
    sprite->unk11 |= 1;
    sprite->tile = gResourceTableEntries[sprite->vramBlock].offset >> 5;
    sprite->fullColor = 0;
    sprite->shape = 1;
    sprite->flipX = 0;
    sprite->flipY = 1;
    ((u8 *)sprite->record)[0x16] = 0;
  }
  objects[0]->update = (void (*)(void))Func_02001a80;
  ((struct FieldSprite *)objects[0]->animation)->priority = 1;
  objects[1]->update = (void (*)(void))Func_02001a34;
  ((struct FieldSprite *)objects[1]->animation)->priority = 1;
  objects[1]->priorityFlags = 2;
}
