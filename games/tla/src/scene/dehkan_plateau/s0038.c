/// @file
/// Scene code of overlay 0x655, in ROM order, from s0038's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "event_inline.h"
#include "ram_buffer.h"

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02000128(s32 id, u32 mode, s32 flag, s32 layer);

void Func_02000038(const u16 *list) {
  const u16 *spec;
  u16 id;
  struct Actor *object;
  s16 mode;
  s16 flag;
  s32 limit;
  s32 first;
  limit = 0xff;
  first = list[0];
  if (first == ((limit << 8) + 0xff)) {
    return;
  }
  spec = list;
next:
  id = spec[0];
  object = Actor_GetById(id);
  mode = spec[1];
  flag = spec[2];
  object->collisionFlags &= 0xfe;
  Actor_SetSingleChildFlags(object, 0);
  Func_02000128(id, (u16)mode, (u16)flag, object->terrainId);
  limit = 0xff;
  spec += 3;
  first = spec[0];
  if (first != ((limit << 8) + 0xff)) {
    goto next;
  }
}

struct EffectWork1a {
  u8 unk00[0x1a];
  s16 active;
};

void Func_02000128(s32 id, u32 mode, s32 flag, s32 layer);

void Func_020000a4(const void *data, s32 mode, s32 timer) {
  const u16 *spec = data;
  struct EffectWork1a *work = Ram_HeapSlots->effectWork;
  while (*spec != 0xffff) {
    u32 value = *spec;
    s32 active = work->active;

    if (active == value) {
      struct Actor *object = Actor_GetById(active);
      s32 kind;
      s32 flag;

      spec++;
      kind = spec[0];
      flag = spec[1];
      if (timer <= 7)
        object->scaleY += -0x1f00;
      if (mode == 1) {
        GameFlag_SetBit(flag);
        Func_02000128(active, kind, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
    spec += 3;
  }
}

struct MapLayer {
  u8 *cells;
  u8 unk04[52];
};

struct MapLayers {
  u8 unk000[0x138];
  struct MapLayer layers[4];
};

s32 Func_02003db0(struct Actor *object);
void Func_02003ba0(struct Actor *object, s32 mode);

void Func_02000128(s32 id, u32 mode, s32 flag, s32 layer) {
  struct Actor *object = Actor_GetById(id);
  u8 *cell = ((struct MapLayers *)Ram_HeapSlots->mapWork)->layers[layer].cells;

  cell += Func_02003db0(object) * 4;
  if (GameFlag_Test(flag)) {
    /* FAKEMATCH(temporary): the flags read into a local before the cell store
     * and ORed into the register that holds 2, as the native does; as one |=,
     * which builds the 0 for the cell before the read, 8 of 75 instructions
     * differ in tla-en. */
    u8 flags = object->priorityFlags;
    s32 bits;

    cell[2] = 0;
    bits = 2;
    bits |= flags;
    object->priorityFlags = bits;
    switch (mode) {
    case 0:
      Func_02003ba0(object, 2);
      break;
    case 1:
      Func_02003ba0(object, 4);
      break;
    case 2:
      Func_02003ba0(object, 6);
      break;
    }
  } else {
    cell[2] = 255;
    switch (mode) {
    case 0:
      Func_02003ba0(object, 1);
      break;
    case 1:
      Func_02003ba0(object, 3);
      break;
    case 2:
      Func_02003ba0(object, 5);
      break;
    }
  }
}
