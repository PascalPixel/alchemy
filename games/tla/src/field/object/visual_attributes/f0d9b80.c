/// @file
/// Routines first written as f0d9b80.c; none has a twin in The Broken Seal.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

/* VIEW(f0d9b80): tag struct ChainSlot as struct ChainSlot__f0d9b80; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5953: structure has no member named `angle'. */
struct ChainSlot__f0d9b80 {
  s16 first;
  s16 second;
  u32 angle;
  s32 amplitude;
  u8 unk0c[4];
  s32 wobble;
  struct ChainNode *tail;
  struct ChainNode *head;
};

/* VIEW(f0d9b80): tag struct ChainWork as struct ChainWork__f0d9b80; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5958: structure has no member named `angle'. */
struct ChainWork__f0d9b80 {
  u8 unk000[8];
  s32 vramEntry;
  struct ChainSlot__f0d9b80 slots[8];
};


void Func_080d9b80(void) {
  struct ChainWork__f0d9b80 *work = Ram_HeapSlots->unk84[7];
  s32 i;
  s32 prevX = 0;
  s32 prevY = 0;
  s32 prevZ = 0;
  s32 started = 0;
  u32 tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 originY = map->origin[1];
  struct ChainTransform transform;

  for (i = 0; i < 8; i++) {
    u32 angle = work->slots[i].angle;
    struct ChainNode *node = work->slots[i].head;
    s32 affine;
    s32 matrix;

    if (angle != 0) {
      transform.angle = angle;
      transform.scaleX = 0x100;
      transform.scaleY = 0x100;
      affine = 1;
      matrix = AffineMatrix_BuildForEffect(&transform);
    } else {
      affine = 0;
      matrix = 0;
    }
    for (; node != NULL; node = node->next) {
      s32 y = node->y;
      s32 x = node->x;
      s32 z = node->z;

      y += 0xfffc0000;

      if (started) {
        s32 screenX = (prevX + x) / 2 - viewX;
        s32 screenY;

        prevY = (prevY + y) / 2 - originY;
        prevZ = (prevZ + z) / 2 - viewZ - originY;
        screenY = prevZ - prevY;

        if ((u32)(screenX + 0x1fffff) <= 0x12ffffe && screenY > -0x200000 &&
            screenY <= 0xdfffff) {
          s32 left = ((screenX >> 16) - 4) & 0x1ff;
          s32 top = ((screenY >> 16) - 4) & 0xff;
          s32 key = ((prevY + prevZ) >> 16) + 58;

          ((u32 *)&node->sprite)[0] = 0;
          ((u32 *)&node->sprite)[1] = top | (left << 16) | 0x2000;
          ((u32 *)&node->sprite)[2] = tile | 0x800;
          node->sprite.affine = affine;
          node->sprite.affineIndex = matrix;
          Func_080140d8(&node->sprite, key);
        }
      }
      prevX = x;
      prevY = y;
      prevZ = z;
      started = 1;
    }
    prevX = 0;
    prevY = 0;
    prevZ = 0;
    started = 0;
  }
}


struct WobbleEvent {
  u8 unk000[362];
  u16 cue;
  u8 unk16c[24];
  s16 counter;
};

s32 GameFlag_Test(s32 flag);
s32 Field_FindNearbyTalkTarget(s32 id);
s32 Func_080d99b8(s32 first, s32 second);
void Func_080da060(s32 index, s32 first, s32 second, s32 wobble);

/// The heap slot 0xa0 work's per-frame callback (Func_080d9b08 adds it).
/// Unless flag 260 is set, once the event work's counter passes 12 it picks
/// the event work's cue: with render mode 0 the slot linking the leader's
/// nearby talk target sets 0xfc83 when Func_080d99b8 accepts its two ends, and
/// render mode 4 sets 0xfc84. Then every active slot wobbles: the amplitude
/// decays by 0.95 a frame until it drops to 0x1998, and Func_080da060 redraws
/// the slot with the sine of its angle times it.
void Func_080d9d40(void) {
  struct WobbleEvent *event = Ram_HeapSlots->eventWork;
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Slot *slot;
  s32 i = 0;

  if (!GameFlag_Test(260) && event->counter > 12) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->renderMode == 0) {
      s32 target = Field_FindNearbyTalkTarget(party->currentOwner);

      if (target != -1) {
        for (i = 0; i < 8; i++) {
          if (work->slots[i].first == target || work->slots[i].second == target)
            break;
        }
        if (i < 8 && Func_080d99b8(work->slots[i].first, work->slots[i].second))
          event->cue = 0xfc83;
      }
    } else if (party->renderMode == 4) {
      event->cue = 0xfc84;
    }
  }

  slot = work->slots;
  for (i = 0; i < 8; i++, slot++) {
    if (slot->tail != NULL) {
      s32 wobble = Iwram_MulQ16(Trig_Sin(slot->angle), slot->amplitude);

      slot->angle += 0x1000;
      slot->amplitude = Iwram_MulQ16(slot->amplitude, 0xf333);
      if (slot->amplitude <= 0x1998) {
        slot->amplitude = 0;
        slot->tail = NULL;
      }
      Func_080da060(i, slot->first, slot->second, wobble);
    }
  }
}

struct WorkA0Node *Func_080d9e74(void) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *item = work->items;
  s32 index;

  for (index = 0; index <= 127; index++) {
    if (item->used == 0)
      return item;
    item++;
  }
  return NULL;
}


s32 Func_080d9f1c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);

void Func_080d9e98(s32 slotNo, s32 firstId, s32 secondId) {
  struct WorkA0 *work = WORK_A0;
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    struct WorkA0Slot *slot = &work->slots[slotNo];

    slot->first = firstId;
    slot->second = secondId;
    Func_080d9f1c(slotNo, first->x, first->y + 0x80000, first->z, second->x,
                  second->y + 0x80000, second->z);
    Actor_SetMode(first, 4);
    Actor_SetMode(second, 4);
  }
}

s32 ArcTan2(s32 x, s32 y);

struct LineNode {
  struct LineNode *next;
  s32 x;
  s32 y;
  s32 z;
  u8 kind;
  u8 slot;
};

struct LineSlot {
  s16 first;
  s16 second;
  s32 direction;
  s32 amplitude;
  u8 unk0c[4];
  s32 angle;
  s32 active;
  struct LineNode *head;
};

struct LineWork {
  u8 unk00[12];
  struct LineSlot slots[8];
};

s32 Func_080d9f1c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct LineWork *work = Ram_HeapSlots->unk84[7];
  s32 result = 0;
  s32 dx = toX - x;
  s32 sx = dx >> 8;
  s32 dy = toY - y;
  s32 sy = dy >> 8;
  s32 dz = toZ - z;
  s32 sz = dz >> 8;
  s32 segments = Iwram_SqrtEntry(Iwram_MulQ16(sx, sx) + Iwram_MulQ16(sy, sy) +
                                 Iwram_MulQ16(sz, sz)) /
                     6 +
                 1;
  struct LineNode **link = &work->slots[index].head;
  s32 i;

  work->slots[index].direction = (u16)ArcTan2(sz, sx);
  for (i = 0; i <= segments; i++) {
    struct LineNode *node = Func_080d9e74();

    if (node == NULL) {
      result = -1;
      break;
    }
    node->slot = index + 1;
    node->x = x + i * dx / segments;
    node->y = y + i * dy / segments;
    node->z = z + i * dz / segments;
    if (i == segments)
      node->kind = 2;
    else
      node->kind = 1;
    *link = node;
    link = &node->next;
  }
  *link = NULL;
  work->slots[index].active = 1;
  return result;
}

s32 Func_080da0b0(s32 index, s32 x, s32 y, s32 z, s32 to_x, s32 to_y, s32 to_z,
                  s32 mode);

void Func_080da060(s32 index, s32 from, s32 to, s32 mode) {
  struct Actor *a = ObjectTable_Get(from);
  struct Actor *b = ObjectTable_Get(to);

  if (a != NULL && b != NULL)
    Func_080da0b0(index, a->x, a->y + 0x80000, a->z, b->x, b->y + 0x80000, b->z,
                  mode);
}

s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

s32 Func_080da0b0(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                  s32 height) {
  struct LineWork *work = Ram_HeapSlots->unk84[7];
  s32 dx = toX - x;
  s32 sx = dx >> 8;
  s32 dy = toY - y;
  s32 sy = dy >> 8;
  s32 dz = toZ - z;
  s32 sz = dz >> 8;
  s32 segments = Iwram_SqrtEntry(IwramMulQ16(sx, sx) + IwramMulQ16(sy, sy) +
                                 IwramMulQ16(sz, sz)) /
                     6 +
                 1;
  struct LineNode *node = work->slots[index].head;
  s32 i;
  s32 one = 1;

  for (i = 0; i <= segments; i += one) {
    s32 bow = IwramMulQ16(Trig_Sin((i << 15) / segments), height);

    node->x = x + i * dx / segments;
    node->y = y + i * dy / segments + bow;
    node->z = z + i * dz / segments;
    node->kind = one;
    node = node->next;
  }
  return 0;
}

s32 Func_080da22c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);

void Func_080da1b8(s32 index, s32 from, s32 to) {
  struct WorkA0 *work = WORK_A0;
  struct Actor *a = ObjectTable_Get(from);
  struct Actor *b = ObjectTable_Get(to);

  if (a != NULL && b != NULL) {
    struct WorkA0Slot *slot = &work->slots[index];

    slot->first = from;
    slot->second = to;
    Func_080da22c(index, a->x, a->y + 0x80000, a->z, b->x, b->y + 0x80000,
                  b->z);
  }
}

// Builds a chain of nodes around a point.


/// Builds a slot's chain of nodes, one for each 6 units of the distance between
/// two points, laid out as a flat spiral around the first point, every node of
/// kind 2.
s32 Func_080da22c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct LineWork *work = Ram_HeapSlots->unk84[7];
  s32 result = 0;
  s32 dx = (toX - x) >> 8;
  s32 dy = (toY - y) >> 8;
  s32 dz = (toZ - z) >> 8;
  s32 length = Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                               Iwram_MulQ16(dz, dz));
  s32 segments = length / 6 + 1;
  struct LineNode **link = &work->slots[index].head;
  s32 angle = 0;
  s32 radius = 0x40000;
  s32 i;
  s32 kind = 2; // One constant for its whole life.

  for (i = 0; i <= segments; i++) {
    struct LineNode *node = Func_080d8d40();
    u16 step;

    if (node == NULL) {
      result = -1;
      break;
    }
    node->x = x + Iwram_MulQ16(Trig_Cos(angle), radius);
    node->y = y;
    node->z = z + Iwram_MulQ16(Trig_Sin(angle), radius);
    node->kind = kind;
    *link = node;
    step = ArcTan2(0x60000, radius);
    angle += step;
    link = &node->next;
    radius += step * 4;
  }
  *link = NULL;
  return result;
}


extern s32 gData_080f3948;

void Func_080da334(s32 arg, s32 firstId, s32 secondId) {
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    Func_080da3bc(arg, first->x, first->y + 0x80000, first->z, second->x,
                  second->y + 0x80000, second->z,
                  Trig_Sin(gData_080f3948) << 2);
    gData_080f3948 += 0x1000;
    wait(6);
    Actor_SetMode(first, 2);
    Actor_SetMode(second, 2);
  }
}
