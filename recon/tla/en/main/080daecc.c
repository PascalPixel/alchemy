// Matches: differ 0 (tla-en, 2026-11-01)
// Turns an object into a chain of links hanging from its cell: starts the
// chain work (Func_080da9a8) if none is running, takes a chain slot for the
// object (none if it already has one, or none is free), clears the collision
// codes of the five cells above, sinks the object a cell down and 64 pixels
// forward, then hangs seven links (kinds 1, 2, 3/4 alternating, 5 for the
// last) 14 pixels apart from 15 pixels above the cell. When grow is set the
// links unroll over 28 frames.
// Forms that the match depends on, for review before adoption:
// - `cell = cells; cell += ...` in two statements: as one statement the store
//   becomes an address giv and is reduced (differ 44). This is a shaping
//   choice with no plain one-statement equivalent found; it carries no tag
//   because it is none of S2's kinds, so the reviewer should judge it.
// - The grow loop is an `if` around a `do`, so drop is set after the entry
//   tests (a `for` with drop set before gives differ 124 on its own, and
//   `offset` counted from 0 is left unreduced because its giv has no benefit).
// - link->y is computed from i each pass, not stepped: loop.c's reduced giv
//   gives the native's order and allocation (differ 12 against 116).
// - The cell column and row reuse x and z, and last is a one-constant local
//   (the native keeps it in r4, spilled around each call).
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sprite.h"
#include "map.h"

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
  struct {
    u8 unk00[0x28];
    s16 *entries[4];
  } *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
};

struct ChainObject *Actor_GetById(s32 id);
void *Func_080da9a8(s32 kind);
struct ChainSlot *Func_080dae70(s32 id);
struct ChainSlot *Func_080dae3c(void);
struct ChainLink *Func_080daea8(void);
void Actor_SetSingleChildFlagsFar(void *object, s32 flags);

#define CELL_MASK 0xfff00000 // 16-pixel cells

void Func_080daecc(s32 objectId, s32 grow) {
  struct ChainObject *object;
  struct MapWork *map;
  struct ChainSlot *slot;
  struct ChainLink *link;
  struct ChainLink **tail;
  struct MapCell *cells;
  struct MapCell *cell;
  s32 i;
  s32 x;
  s32 top;
  s32 z;
  s32 y;
  s32 frame;
  s32 last;
  s32 kind;
  s32 running;

  object = Actor_GetById(objectId);
  running = Ram_HeapSlots->unk84[8];
  kind = 372;
  map = Ram_HeapSlots->mapWork;
  if (running == 0) {
    if (object->sprite != 0 && object->sprite->entries[0] != 0)
      kind = *object->sprite->entries[0];
    Func_080da9a8(kind);
  }
  if (Func_080dae70(objectId) != 0)
    return;
  slot = Func_080dae3c();
  if (slot == 0)
    return;

  // Clear the collision codes of the five cells above the object.
  x = object->x >> 20;
  z = (object->z - object->y) >> 20;
  cells = map->layers[2].cells;
  for (i = 0; i < 5; i++) {
    z++;
    cell = cells;
    cell += x + z * 128;
    cell->collisionCode = 0;
  }

  x = (object->x & CELL_MASK) + (8 << 16);
  y = object->y & CELL_MASK;
  top = y + (8 << 16);
  z = (object->z & CELL_MASK) + (16 << 16);
  Actor_SetMode((struct Actor *)object, 0);
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->z += 16 << 16;
  object->y -= 64 << 16;
  Actor_SetSingleChildFlagsFar(object, 0);

  // Hang the links.
  last = 6;
  tail = &slot->root;
  for (i = 0; i <= last && (link = Func_080daea8()) != 0; i++) {
    link->x = x;
    link->y = y + (15 << 16) - i * (14 << 16);
    link->z = z;
    if (i == 0)
      link->kind = 1;
    else if (i == 1)
      link->kind = 2;
    else if (i == last)
      link->kind = 5;
    else
      link->kind = i % 2 + 3;
    *tail = link;
    tail = &link->next;
  }

  if (grow == 0) {
    for (i = 0, link = slot->root; i <= last && link != 0; i++)
      link = link->next;
  } else {
    for (frame = 1; frame <= 28; frame++) {
      i = 0;
      link = slot->root;
      if (i <= last && link != 0) {
        s32 drop = -(frame << 15);

        do {
          link->x = x;
          link->y = top - drop / 2;
          link->z = z;
          drop += frame << 16;
          i++;
          link = link->next;
        } while (i <= last && link != 0);
      }
      wait(1);
    }
  }
  slot->active = 1;
  slot->phase = 0;
  slot->swing = 0x100;
  slot->objectId = objectId;
  slot->spacing = 14 << 16;
}
