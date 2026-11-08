// Differ 8 of 112 (tla-en, 2026-11-08, -mtune=gba), under -Os. Registers agree;
// left is the order of the parts[i] store, i * 4, and the two zeros around the
// sprite copy (the store after part->animation, or chained with the create,
// gives the same 8; after the sprite bytes, 97). Tried 2026-11-08: 02001738's
// PartSprite form (bit before slot, its differ 4) gives 10 here, slot first 10,
// mixed struct and char spellings 8 to 14, a 4-minute permute 8. Round two (2026-11-08): the sprite
// as a struct (vramBlock, keepEntry) lets the parts[i] store float, 10;
// storing parts[i] after the vramBlock copy in any typing swaps i and anchor
// (97); the motionFlags zero moved or chained, 8 to 14; a 6-minute permute 8.
// The allocation notes below predate -Os.
// Draft, differ 26 of 112, skeleton gap 3 (tla-en, 2026-11-01)
// Builds four parts around the watched object of an object whose update is
// Func_020015e4, each copying the first part's sprite and position and taking
// Func_02001528. Func_02001738 holds the same 50 statements with the same
// native registers, so they are written as helpers here, not as a device: the
// inline alone took differ 97 to 28 (it clears reload's r4), and AttachPart,
// the four statements repeated at both ends, to 26. i is unsigned: a signed i
// narrows `i - 1` to subs, unsigned gives the native's adds #255.
// Left, about 16 lines: allocation. The native has anchor in r8 and i in r10;
// here i takes r8 and anchor r10. Global-alloc ranks i (11 refs, live 96) above
// anchor (7 refs, live 80); anchor needs 10 refs, or i 7 or fewer, and no plain
// form found gives either. Tried: a real for or do loop (loop.c reduces the
// array index, differ 116), anchor as a local passed in, parts passed in
// (differ 68), a zero or update local (no gain, or worse).
// Left, about 10 lines: the native stores parts[i] between the read and write of
// sprite[17], with i * 4 computed before the sprite copy. Storing after the
// copy brings the shift after `first` dies and cascades (differ 71). A comma
// in the store's right side, with the byte 16 store through u8 *, gives the
// exact order (differ 26 before AttachPart), so the order is real, but that
// spelling is a disguise and stays out. Reading the sprite through the layered
// sprite layout of 08021a84 (slot at 16, retainCacheEntry at bit 0 of 17) reads
// better but lets the store float, differ 28; byte 98 is not unk5c (that is 94).
#include "types.h"
#include "call.h"
#include "actor.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

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
  u8 *first;
  u8 *sprite;
  u32 i;

  parts[0] = Import_ActorCreate(0x15f, anchor->x + 0x10000,
                                anchor->y + 0x200000, anchor->z);
  i = 1;
next:
  first = parts[0]->animation;
  part = Import_ActorCreate(0x15f, 0, 0, 0);
  parts[i] = part;
  sprite = part->animation;
  sprite[16] = first[16];
  sprite[17] |= 1;
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
