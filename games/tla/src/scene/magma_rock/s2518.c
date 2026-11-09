#include "types.h"
#include "actor.h"
#include "field/sprite.h"
#include "event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

struct LayeredSprite {
  u8 unk00[4];
  u32 attributes;
  u8 unk08[4];
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 unk11 : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  struct AnimationSpritePart shadow;
  struct AnimationEntry *entries[4];
};

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Func_02003a54(struct Actor *object, s32 a);
void Func_020015e4(void);
void Func_02001528(void);

static inline void AttachPart(struct Actor *part, struct Actor *anchor,
                              u8 order) {
  BYTE_AT(part, 98) = order;
  part->watchedObject = anchor;
  part->update = Func_02001528;
  Actor_SetPartAttribute(part, 3);
}

static inline void BuildParts(struct Actor *anchor) {
  struct Actor *parts[4];
  struct Actor *part;
  struct LayeredSprite *first;
  struct LayeredSprite *sprite;
  u32 i;

  parts[0] = ((struct Actor *)Import_ActorCreate(
      0x15f, anchor->x + 0x10000, anchor->y + 0x200000, anchor->z));
  i = 1;
next:
  first = parts[0]->animation;
  part = ((struct Actor *)Import_ActorCreate(0x15f, 0, 0, 0));
  parts[i] = part;
  sprite = part->animation;
  sprite->slot = first->slot;
  sprite->retainCacheEntry = 1;
  part->x = parts[0]->x;
  part->y = parts[0]->y;
  part->z = parts[0]->z;
  part->motionFlags = 0;
  Actor_SetSingleChildFlags(part, 0);
  Func_02003a54(parts[i], 3);
  AttachPart(parts[i], anchor, i - 1);
  if (++i != 4)
    goto next;
  part = parts[0];
  part->motionFlags = 0;
  Actor_SetSingleChildFlags(part, 0);
  Func_02003a54(parts[0], 3);
  Scene_SetActorSpritePriority(parts[0], 3);
  AttachPart(parts[0], anchor, 3);
}

void Func_02002518(u16 id) {
  if (Actor_GetById(id)->update != Func_020015e4)
    return;
  BuildParts(Actor_GetById(id)->watchedObject);
}
