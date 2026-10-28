// Draft, differ 97 of 112: builds four parts around the watched object of an object
// whose update is Func_020015e4, each copying the first part's sprite and
// position and taking Func_02001528. Left: the native keeps the parts' array
// base in r7, the anchor in r8 and the count in r10, with 0 in r6 for the
// motion flags; a six-minute permute found nothing closer.
#include "types.h"
#include "actor.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

struct Actor *Actor_GetById(s32 id);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Func_02003a54(struct Actor *object, s32 a);
void Func_020015e4(void);
void Func_02001528(void);

void Func_02002518(u16 id) {
  struct Actor *parts[4];
  struct Actor *anchor;
  s32 i;

  if (Actor_GetById(id)->update != Func_020015e4)
    return;
  anchor = Actor_GetById(id)->watchedObject;
  parts[0] = Import_ActorCreate(0x15f, anchor->x + 0x10000,
                                 anchor->y + 0x200000, anchor->z);
  i = 1;
next:
  {
    u8 *first = parts[0]->animation;
    struct Actor *part = Import_ActorCreate(0x15f, 0, 0, 0);
    u8 *sprite = part->animation;

    sprite[16] = first[16];
    parts[i] = part;
    sprite[17] |= 1;
    part->x = parts[0]->x;
    part->y = parts[0]->y;
    part->z = parts[0]->z;
    part->motionFlags = 0;
    Actor_SetSingleChildFlags(part, 0);
    Func_02003a54(parts[i], 3);
    part = parts[i];
    BYTE_AT(part, 98) = i - 1;
    part->watchedObject = anchor;
    part->update = Func_02001528;
    Actor_SetPartAttribute(part, 3);
  }
  if (++i != 4)
    goto next;
  parts[0]->motionFlags = 0;
  Actor_SetSingleChildFlags(parts[0], 0);
  Func_02003a54(parts[0], 3);
  Scene_SetActorSpritePriority(parts[0], 3);
  {
    struct Actor *part = parts[0];

    BYTE_AT(part, 98) = 3;
    part->watchedObject = anchor;
    part->update = Func_02001528;
    Actor_SetPartAttribute(part, 3);
  }
}
