#include "types.h"

void *ObjectTable_Get(u32 object);

struct LinkedEffectVisual {
  u8 unk00[9];
  u8 flags;
  u8 unk0a[16];
  u8 value26;
};

struct LinkedEffectObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[60];
  struct LinkedEffectVisual *visual;
  u8 value54;
  u8 value55;
  u8 unk56[14];
  u16 counter;
  u16 resourceId;
  struct LinkedEffectObject *resource;
  void (*callback)(void);
};

struct LinkedEffectObject *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_InitializeFar(struct LinkedEffectObject *object,
                                  const void *script);
void Object_SetMode(struct LinkedEffectObject *object, s32 mode);
void Object_Destroy(struct LinkedEffectObject *object);
s32 Object_IsOutsideAnimation108(struct LinkedEffectObject *object);
void BattleEffect_CopyWatchedObjectPosition(void);
extern const u8 gData_080f341c[];

/// The Broken Seal's BattleEffect_ConfigureWatchedObject with this game's
/// calls: with flags & 3 zero it destroys the object's linked effect;
/// otherwise (mode 2, or when the linked effect is outside animation 0x108)
/// it creates effect object 264 at the object and links it, mode 1 keeping it
/// as the object's resource and mode 2 running gData_080f341c on it.
void Func_080d489c(s32 id, s32 flags) {
  struct LinkedEffectObject *object = ObjectTable_Get(id);
  struct LinkedEffectObject *child;
  struct LinkedEffectVisual *visual;
  s32 mode;

  child = 0;
  visual = 0;

  if (object == 0)
    return;

  if ((flags & 3) != 0) {
    if ((flags & 3) == 2 || Object_IsOutsideAnimation108(object->resource)) {
      child = Func_080200c0(264, object->x, object->y, object->z);
    }
  } else {
    child = object->resource;
    if (child == 0)
      return;
    Object_Destroy(child);
    object->resource = NULL;
    return;
  }

  if (child == 0)
    return;

  mode = flags & 3;
  switch (mode) {
  case 1:
    Object_SetMode(child, 1);
    object->resource = child;
    child->counter = 1;
    break;
  case 2:
    Object_SetMode(child, 2);
    ObjectDispatch_InitializeFar(child, gData_080f341c);
    child->counter = 1;
    break;
  }

  child->resourceId = id;
  child->value55 = 0;
  child->callback = BattleEffect_CopyWatchedObjectPosition;
  visual = child->visual;
  visual->value26 = 0;
  child->resource = object;

  if (flags & 0x100) {
    s32 mask = 13;
    u8 visualFlags = visual->flags;

    mask = -mask;
    mask &= visualFlags;
    mask |= 4;
    visual->flags = mask;
  } else {
    s32 copiedFlags = 12;
    u8 sourceFlags = object->visual->flags;
    u8 destinationFlags;
    s32 clearMask = 13;

    copiedFlags &= sourceFlags;
    destinationFlags = visual->flags;
    clearMask = -clearMask;
    clearMask &= destinationFlags;
    clearMask |= copiedFlags;
    visual->flags = clearMask;
  }
}
