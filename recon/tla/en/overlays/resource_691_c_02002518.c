// Draft, differ 97 of 112: builds four parts around the watched object of an object
// whose update is Func_020015e4, each copying the first part's sprite and
// position and taking Func_02001528. Left: the native keeps the parts' array
// base in r7, the anchor in r8 and the count in r10, with 0 in r6 for the
// motion flags; a six-minute permute found nothing closer.
#include "types.h"
#include "object_runtime.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void Func_02003a54(struct ObjectRuntime *object, s32 a);
void Func_020015e4(void);
void Func_02001528(void);

void Func_02002518(u16 id) {
  struct ObjectRuntime *parts[4];
  struct ObjectRuntime *anchor;
  s32 i;

  if (Object_GetById(id)->update != Func_020015e4)
    return;
  anchor = Object_GetById(id)->watchedObject;
  parts[0] = Engine_ObjectCreate(0x15f, anchor->x + 0x10000,
                                 anchor->y + 0x200000, anchor->z);
  i = 1;
next:
  {
    u8 *first = parts[0]->animation;
    struct ObjectRuntime *part = Engine_ObjectCreate(0x15f, 0, 0, 0);
    u8 *sprite = part->animation;

    sprite[16] = first[16];
    parts[i] = part;
    sprite[17] |= 1;
    part->x = parts[0]->x;
    part->y = parts[0]->y;
    part->z = parts[0]->z;
    part->motionFlags = 0;
    ObjectDispatch_SetSingleChildFlags(part, 0);
    Func_02003a54(parts[i], 3);
    part = parts[i];
    BYTE_AT(part, 98) = i - 1;
    part->watchedObject = anchor;
    part->update = Func_02001528;
    Object_SetPartAttribute(part, 3);
  }
  if (++i != 4)
    goto next;
  parts[0]->motionFlags = 0;
  ObjectDispatch_SetSingleChildFlags(parts[0], 0);
  Func_02003a54(parts[0], 3);
  Object_SetSpritePriority(parts[0], 3);
  {
    struct ObjectRuntime *part = parts[0];

    BYTE_AT(part, 98) = 3;
    part->watchedObject = anchor;
    part->update = Func_02001528;
    Object_SetPartAttribute(part, 3);
  }
}
