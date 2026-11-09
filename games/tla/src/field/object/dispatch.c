#include "actor_view.h"
#include "sprite.h"

/// Points an object at its script and clears its script state.
void Actor_StartScript(struct ActorView *object,
                               const s32 *script) {
  if (object != 0) {
    object->value04 = 0;
    object->value00 = (u32)script;
    object->value5b = 0;
    object->value5d = 0;
    object->value57 = 0;
  }
}

void Actor_SelectAnimation(struct ActorView *object,
                                            s32 argument) {
  struct ActorSpriteView **items;
  struct ActorSpriteView *item;
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

void Actor_SetAnimationSpeed(struct ActorView *object,
                                         s32 value) {
  s32 count;
  struct ActorSpriteView *child;
  struct ActorSpriteView **children;

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

void Actor_SetAnimationAndSpeed(struct ActorView *object,
                                        s32 argument, s32 value) {
  struct ActorSpriteView **items;
  struct ActorSpriteView *item;
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

void Actor_SetSpriteRotation(struct ActorView *object,
                                        u32 value) {
  if (object != 0 && (object->kind & 0xf) == 1)
    object->target.child->value12 = value;
}

void Animation_SetIndexAndInitializeObjects(struct ActorView *object,
                                      s32 index) {
  struct ActorSpriteView *child;

  if (object != NULL && (object->kind & 0xf) == 1) {
    child = object->target.child;
    if (index >= 0) {
      child->entries[0]->animId = index;
      InitializeAnimationObjects(child);
    }
  }
}

void Actor_RegisterSpriteMetadata(struct ActorView *object,
                                          s32 value) {
  if (object != 0 && (object->kind & 0xf) == 1) {
    struct ActorSpriteView *child = object->target.child;
    if (value >= 0)
      ResourceMetadata_Register(child, value);
  }
}
