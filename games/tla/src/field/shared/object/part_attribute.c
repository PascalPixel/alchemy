#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "actor.h"

struct ObjectPart {
  u8 unk00[5];
  u8 attribute;
  u8 unk06[0xa];
  void *graphics;
};

struct ObjectPartList {
  u8 unk00[0x19];
  u8 dirty;
  u8 unk1a;
  u8 count;
  u8 unk1c[0xc];
  struct ObjectPart *parts[1];
};

extern u32 gFrameCount;

void Actor_ApplyIndexedChildValue(struct Actor *object) {
  struct ObjectPartList *list;
  struct ObjectPart *part;
  s32 attribute;
  s32 i;

  if ((object->animationKind & 0xf) == 1) {
    list = object->animation;
    attribute = gObjectGroup_BlinkChildValues[(gFrameCount >> 1) & 3];
    for (i = 0; i < list->count; i++) {
      part = list->parts[i];
      if (part != 0 && part->graphics != 0) {
        part->attribute = attribute;
        /* FAKEMATCH: the ARM9 scheduling would reload the part count before
         * this store. */
        __asm__("");
      }
    }
    list->dirty = 1;
  }
}

void Actor_SetPartAttribute(struct Actor *object, s32 attribute) {
  struct ObjectPartList *list;
  struct ObjectPart *part;
  s32 i;

  if ((object->animationKind & 0xf) == 1) {
    list = object->animation;
    for (i = 0; i < list->count; i++) {
      part = list->parts[i];
      if (part != 0 && part->graphics != 0) {
        part->attribute = attribute;
        /* FAKEMATCH: the ARM9 scheduling would reload the part count before
         * this store. */
        __asm__("");
      }
    }
    list->dirty = 1;
  }
}

void Actor_StartScriptFar(struct Actor *object,
                                  const void *callback);

/// Links an object to the object in the low 8 bits of targetId and starts
/// callback on it. Unless bit 16 of targetId is set it takes a follow
/// distance of 40 pixels (action 40), the target's top speed with twice its
/// acceleration, to keep up with it, and no collision flags, so it cannot
/// block the target. The Broken Seal's ObjectEvent_SetTargetAndCallback.
void Actor_SetTargetAndCallback(u32 objectId, s32 targetId,
                                 const void *callback) {
  struct Actor *follower = ObjectTable_Get(objectId);
  struct Actor *target = ObjectTable_Get(targetId & 0xff);

  if (follower != NULL && target != NULL) {
    follower->watchedObject = target;
    if (!(targetId & 0x10000)) {
      /* FAKEMATCH(pin): the collision flags' zero held in r1, which the
       * native loads from the pool between the action's store and the
       * target's acceleration; as follower->collisionFlags = 0, 2 of 36
       * instructions differ in tla-en. */
      register u16 none asm("r1") = 0;

      follower->action = 40;
      /* FAKEMATCH(barrier): an empty asm holding the pinned zero and the
       * target's acceleration puts the zero's pool load between the action's
       * store and that load; as the pin alone, 2 of 36 differ in tla-en. */
      asm("" : "+l"(none), "+m"(target->acceleration));
      follower->acceleration = target->acceleration * 2;
      follower->speedLimit = target->speedLimit;
      follower->collisionFlags = none;
    }
    Actor_StartScriptFar(follower, callback);
  }
}
