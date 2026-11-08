// Matches: differ 0 (tla-en, 2026-11-08), under -Os with -mtune=gba.
// The last 8 came from the sprite's type, not the statement order: read
// through the layered sprite layout matched in field/f02254c.c (scale, an s32,
// at 12; slot at 16; retainCacheEntry at bit 0 of 17), the bit's store
// conflicts in alias set with the s32 position loads that follow and so issues
// before the parts[0] reload, while the parts[i] store, an Actor pointer, stays
// free and fills the load slot after the bit's read, as natively. Through a
// u8 * every byte aliases the parts[] array and the store stays first (8); with
// only u8 and u16 fields before the slot (the earlier PartSprite and
// f021a84.c's layout) the reload rises above the bit's store (10). Any word
// field in the first 16 bytes gives 0; the layout is f02254c.c's whole.
// Func_02001738 holds the same 50 statements: in a scratch copy, its PartSprite
// with an s32 at 12 and the slot copied before the bit also measures 0.
//
// Builds four parts around the watched object of an object whose update is
// Func_020015e4, each copying the first part's sprite and position and taking
// Func_02001528. The statements shared with Func_02001738 are written as
// helpers: the inline BuildParts took differ 97 to 28 (it clears reload's r4),
// and AttachPart, the four statements repeated at both ends, to 26. i is
// unsigned: a signed i narrows `i - 1` to subs, unsigned gives the native's
// adds #255. A real for or do loop lets loop.c reduce the array index (116).
#include "types.h"
#include "call.h"
#include "actor.h"
#include "field/sprite.h"

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

struct Actor *Actor_GetById(s32 id);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
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

  parts[0] = Import_ActorCreate(0x15f, anchor->x + 0x10000,
                                anchor->y + 0x200000, anchor->z);
  i = 1;
next:
  first = parts[0]->animation;
  part = Import_ActorCreate(0x15f, 0, 0, 0);
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
