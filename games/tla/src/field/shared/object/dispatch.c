#include "object_dispatch.h"
#include "animspr.h"

/// Points an object at its script and clears its script state.
void Actor_StartScript(struct DispatchObject *object,
                               const s32 *script) {
  if (object != 0) {
    /* FAKEMATCH(pin): the byte stores' halfword zero held in r3,
     * which the native loads from the pool before the first store's zero; as
     * plain zero stores, 2 of 16 instructions differ in tla-en. */
    register u16 zero asm("r3") = 0;

    /* FAKEMATCH(barrier): the empty asm that keeps the pinned zero's pool
     * load ahead of the first store; as the pin alone, 2 of 16 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(zero));
    object->value04 = 0;
    object->value00 = (u32)script;
    object->value5b = zero;
    object->value5d = zero;
    object->value57 = zero;
  }
}

void Actor_SelectAnimation(struct DispatchObject *object,
                                            s32 argument) {
  struct DispatchChild **items;
  struct DispatchChild *item;
  s32 count;

  if (object != 0) {
    switch (object->kind & 0xf) {
    case 1:
      Animation_ApplyChildArgument(object->target.child, argument);
      break;
    case 2:
      items = object->target.children;
      count = 3;
      do {
        item = *items++;
        if (item != 0)
          Animation_ApplyChildArgument(item, argument);
        count--;
      } while (count >= 0);
      break;
    }
  }
}

void Actor_SetAnimationSpeed(struct DispatchObject *object,
                                         s32 value) {
  s32 count;
  struct DispatchChild *child;
  struct DispatchChild **children;

  if (object != 0) {
    switch (object->kind & 0xf) {
    case 1:
      Animation_ApplyChildValue(object->target.child, value);
      return;
    case 2:
      children = object->target.children;
      count = 3;
      do {
        child = *children++;
        if (child != 0)
          Animation_ApplyChildValue(child, value);
        count--;
      } while (count >= 0);
      break;
    }
  }
}

void Actor_SetAnimationAndSpeed(struct DispatchObject *object,
                                        s32 argument, s32 value) {
  struct DispatchChild **items;
  struct DispatchChild *item;
  s32 count;

  if (object != 0) {
    switch (object->kind & 0xf) {
    case 1:
      Animation_ApplyChildArgument(object->target.child, argument);
      Animation_ApplyChildValue(object->target.child, value);
      break;
    case 2:
      items = object->target.children;
      for (count = 3; count >= 0; count--) {
        item = *items++;
        if (item != 0) {
          Animation_ApplyChildArgument(item, argument);
          Animation_ApplyChildValue(item, value);
        }
      }
      break;
    }
  }
}

void Actor_SetSpriteRotation(struct DispatchObject *object,
                                        u32 value) {
  if (object != 0 && (object->kind & 0xf) == 1)
    object->target.child->value12 = value;
}

void Animation_SetIndexAndInitializeObjects(struct DispatchObject *object,
                                      s32 index) {
  struct DispatchChild *child;

  if (object != NULL && (object->kind & 0xf) == 1) {
    child = object->target.child;
    if (index >= 0) {
      child->entries[0]->animId = index;
      InitializeAnimationObjects(child);
    }
  }
}

void Actor_RegisterSpriteMetadata(struct DispatchObject *object,
                                          s32 value) {
  if (object != 0 && (object->kind & 0xf) == 1) {
    struct DispatchChild *child = object->target.child;
    if (value >= 0)
      ResourceMetadata_Register(child, value);
  }
}
