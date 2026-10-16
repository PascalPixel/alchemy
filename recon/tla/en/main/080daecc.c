/* Draft, differ 252 of 228, first pass (2026-10-15). Turns an object into a
 * chain of links hanging from its cell: starts the chain work
 * (Func_080da9a8) if none is running, takes a chain slot for the object
 * (none if it already has one, or none is free), clears the tile flags of
 * the five cells above, sinks the object a cell down and 64 pixels forward,
 * then hangs seven links (kinds 1, 2, 3/4 alternating, 5 for the last) 14
 * pixels apart from 15 pixels above the cell. When grow is set the links
 * unroll over 28 frames. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "animspr.h"

struct ChainLink {
  struct ChainLink *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 kind;
};

struct ChainSlot {
  s16 objectId;
  u8 unk02[2];
  s32 swing;
  s32 spacing;
  u8 unk0c[4];
  s32 phase;
  s32 active;
  struct ChainLink *root;
};

struct ChainObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x3c];
  struct { u8 unk00[0x28]; s16 *entries[4]; } *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
};

struct ChainObject *Object_GetById(s32 id);
void *Func_080da9a8(s32 kind);
struct ChainSlot *Func_080dae70(s32 id);
struct ChainSlot *Func_080dae3c(void);
struct ChainLink *Func_080daea8(void);
void ObjectDispatch_SetSingleChildFlagsFar(void *object, s32 flags);

#define CELL_MASK 0xfff00000 // 16-pixel cells
#define CHAIN_LAST_LINK 6

void Func_080daecc(s32 objectId, s32 grow) {
  struct ChainObject *object;
  u8 *map;
  struct ChainSlot *slot;
  struct ChainLink *link;
  struct ChainLink **tail;
  u8 *cells;
  s32 column;
  s32 row;
  s32 index;
  s32 i;
  s32 x;
  s32 top;
  s32 z;
  s32 y;
  s32 frame;

  object = Object_GetById(objectId);
  map = (u8 *)Ram_HeapSlots->mapWork;
  if (Ram_HeapSlots->unk84[8] == 0) {
    s32 kind = 372;
    if (object->sprite != 0 && object->sprite->entries[0] != 0)
      kind = *object->sprite->entries[0];
    Func_080da9a8(kind);
  }
  if (Func_080dae70(objectId) != 0)
    return;
  slot = Func_080dae3c();
  if (slot == 0)
    return;

  // Clear the tile flags of the five cells above the object.
  column = object->x >> 20;
  row = (object->z - object->y) >> 20;
  cells = *(u8 **)(map + 0x1a8);
  index = (row << 9) + (column << 2);
  for (i = 4; i >= 0; i--) {
    index += 0x200;
    cells[index + 2] = 0;
  }

  x = (object->x & CELL_MASK) + (8 << 16);
  y = object->y & CELL_MASK;
  top = y + (8 << 16);
  z = (object->z & CELL_MASK) + (16 << 16);
  Object_SetMode((struct ObjectRuntime *)object, 0);
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->z += 16 << 16;
  object->y -= 64 << 16;
  ObjectDispatch_SetSingleChildFlagsFar(object, 0);

  // Hang the links.
  tail = &slot->root;
  y += 15 << 16;
  for (i = 0; i <= CHAIN_LAST_LINK && (link = Func_080daea8()) != 0; i++) {
    link->x = x;
    link->y = y;
    link->z = z;
    if (i == 0)
      link->kind = 1;
    else if (i == 1)
      link->kind = 2;
    else if (i == CHAIN_LAST_LINK)
      link->kind = 5;
    else
      link->kind = i % 2 + 3;
    *tail = link;
    y -= 14 << 16;
    tail = &link->next;
  }

  if (grow == 0) {
    for (i = 0, link = slot->root; i <= CHAIN_LAST_LINK && link != 0; i++)
      link = link->next;
  } else {
    for (frame = 1; frame <= 28; frame++) {
      s32 drop = -(frame << 15);

      for (i = 0, link = slot->root; i <= CHAIN_LAST_LINK && link != 0; i++) {
        link->x = x;
        link->y = top - drop / 2;
        link->z = z;
        drop += (frame + 1) << 16;
        link = link->next;
      }
      WaitFrames(1);
    }
  }
  slot->active = 1;
  slot->phase = 0;
  slot->swing = 0x100;
  slot->objectId = objectId;
  slot->spacing = 14 << 16;
}
