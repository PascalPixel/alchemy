/// @file
/// The chain works: a slot's chain of nodes laid along a line between two
/// points or built around a point, the states of its slots, a second chain
/// work with its per-frame callback, finding a slot, and a field effect that
/// animates sixteen tiles of its own.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "module/field/field.h"
#include "module/field/object.h"

/* VIEW(f0d8ff8): routine ArcTan2 as s16 ArcTan2(s32 y, s32 x); f0d8740 has s32
 * ArcTan2(s32 y, s32 x); tried as the definition's s32 ArcTan2(s32 x, s32 y),
 * which the object already declares, Rope_LayBowed moves: 1 of 120 instructions
 * in tla-ja. */
extern s16 ArcTan2__f0d8ff8(s32 y, s32 x) __asm__("ArcTan2");

/* VIEW(f0d8ff8): tag struct ChainNode as struct ChainNode__f0d8ff8;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:5333: structure has no member
 * named `visible'. */
struct ChainNode__f0d8ff8 {
  struct ChainNode__f0d8ff8 *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
};

/* VIEW(f0d8ff8): tag struct ChainSlot as struct ChainSlot__f0d8ff8;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:5325: structure has no member
 * named `segments'. */
struct ChainSlot__f0d8ff8 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[22];
  struct ChainNode__f0d8ff8 *head;
};

/* VIEW(f0d8ff8): tag struct ChainWork as struct ChainWork__f0d8ff8;
 * visual_attributes.c has another definition. */
struct ChainWork__f0d8ff8 {
  u8 unk00[12];
  struct ChainSlot__f0d8ff8 slots[8];
};

/// Lays a slot's chain of nodes along the line between two points, bowed
/// sideways by a half sine of the given height.
s32 Rope_LayBowed(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                  s32 height) {
  struct ChainWork__f0d8ff8 *work = Ram_HeapSlots->unk84[6];
  s32 dx = toX - x;
  s32 ax = dx >> 8;
  s32 dy = toY - y;
  struct ChainSlot__f0d8ff8 *slot = &work->slots[index];
  struct ChainNode__f0d8ff8 *node = slot->head;
  u16 angle = ArcTan2__f0d8ff8(ax, dy >> 8);
  s32 sin = Trig_Sin(angle);
  s32 cos = Trig_Cos(angle);
  s32 segments = slot->segments;
  s32 i;
  s32 visible = 1; // One constant for its whole life.
  {
    for (i = 0; i <= segments; i++) {
      s32 bow = Iwram_MulQ16(Trig_Sin((i << 15) / segments), height);

      node->x = x + i * dx / segments + Iwram_MulQ16(bow, cos);
      node->y = y + i * dy / segments + Iwram_MulQ16(bow, sin);
      node->z = z + i * (toZ - z) / segments;
      node->visible = visible;
      node = node->next;
    }
  }
  return 0;
}

/// Coils rope index at post firstId, its length reaching post secondId: the
/// rope as it waits to be lashed.
void Rope_Coil(s32 slot, s32 firstId, s32 secondId) {
  struct LinkEntry *entries = Ram_HeapSlots->unk84[6];
  s32 layer = 2;
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != 0 && second != 0) {
    struct LinkEnds *ends;
    s32 lift;

    if (first->animationKind == 1)
      layer = ((struct LinkAnimation *)first->sprite)->layer;
    ends = &entries[slot].ends;
    ends->first = firstId;
    ends->second = secondId;
    ends->layer = layer;
    lift = 128;
    /* FAKEMATCH(barrier): an empty asm on lift keeps its 128 built after the
     * layer's store and ahead of the coordinates' loads; without it, 10 of
     * 69 instructions differ in tla-en. */
    asm volatile("" : "+l"(lift));
    lift <<= 12;
    Rope_LayCoil(slot, first->x, first->y + lift, first->z, second->x,
                 second->y + lift, second->z);
  }
}

/* VIEW(f0d9194): tag struct ChainNode as struct ChainNode__f0d9194;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:5445: structure has no member
 * named `visible'. */
struct ChainNode__f0d9194 {
  struct ChainNode__f0d9194 *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
};

/* VIEW(f0d9194): tag struct ChainSlot as struct ChainSlot__f0d9194;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:5435: structure has no member
 * named `segments'. */
struct ChainSlot__f0d9194 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[22];
  struct ChainNode__f0d9194 *head;
};

/* VIEW(f0d9194): tag struct ChainWork as struct ChainWork__f0d9194;
 * visual_attributes.c has another definition. */
struct ChainWork__f0d9194 {
  u8 unk00[12];
  struct ChainSlot__f0d9194 slots[8];
};

/* VIEW(f0d9194): routine Rope_AllocNode as struct ChainNode__f0d9194
 * *Rope_AllocNode(void); the definition has struct Work9cItem
 * *Rope_AllocNode(void). */
extern struct ChainNode__f0d9194 *
Rope_AllocNode__f0d9194(void) __asm__("Rope_AllocNode");

/// Builds a slot's chain of nodes, one for each 6 units of the distance between
/// two points, laid out as a flat spiral around the first point.
s32 Rope_LayCoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct ChainWork__f0d9194 *work = Ram_HeapSlots->unk84[6];
  s32 result = 0;
  s32 dx = (toX - x) >> 8;
  s32 dy = (toY - y) >> 8;
  s32 dz = (toZ - z) >> 8;
  s32 length = Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                               Iwram_MulQ16(dz, dz));
  s32 segments = length / 6 + 1;
  struct ChainSlot__f0d9194 *slot = &work->slots[index];
  struct ChainNode__f0d9194 **link = &slot->head;
  s32 angle = 0;
  s32 radius = 0x40000;
  s32 i;
  s32 visible = 2; // One constant for its whole life.

  slot->segments = segments;
  for (i = 0; i <= segments; i++) {
    struct ChainNode__f0d9194 *node = Rope_AllocNode__f0d9194();
    u16 step;

    if (node == NULL) {
      result = -1;
      break;
    }
    node->x = x + Iwram_MulQ16(Trig_Cos(angle), radius);
    node->y = y;
    node->z = z + Iwram_MulQ16(Trig_Sin(angle), radius);
    node->visible = visible;
    *link = node;
    step = ArcTan2(0x60000, radius);
    angle += step;
    link = &node->next;
    radius += step * 4;
  }
  *link = NULL;
  return result;
}

/// Writes the position of the node at rope index's depth into position.
void Rope_GetEndNodePosition(s32 index, s32 *position) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot = &work->slots[index];
  struct Work9cNode *node = slot->head;
  s32 depth;

  depth = slot->depth;
  if (depth > 0) {
    do {
      depth--;
      node = node->next;
    } while (depth != 0);
  }
  SET_VECTOR(position, node->x, node->y, node->z);
}

#include "module/field/field.h"

/* VIEW(f0d92d4): tag struct ChainSlot as struct ChainSlot__f0d92d4;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:5515: structure has no member
 * named `segments'. */
struct ChainSlot__f0d92d4 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[18];
  s32 unk18;
  void *head;
};

/* VIEW(f0d92d4): tag struct ChainWork as struct ChainWork__f0d92d4;
 * visual_attributes.c has another definition. */
struct ChainWork__f0d92d4 {
  u8 unk00[12];
  struct ChainSlot__f0d92d4 slots[8];
};

extern s32 gRope_LashPhase;

/* VIEW(f0d92d4): routine Rope_Throw as void Rope_Throw(s32 index, s32 x,
 * s32 y, s32 z, s32 toX, s32 toY, s32 toZ, s32 wobble); the definition has s32
 * Rope_Throw(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ). */
extern void Rope_Throw__f0d92d4(s32 index, s32 x, s32 y, s32 z, s32 toX,
                                s32 toY, s32 toZ,
                                s32 wobble) __asm__("Rope_Throw");
/* VIEW(f0d92d4): routine Rope_Recoil as void Rope_Recoil(s32 index, s32 x,
 * s32 y, s32 z, s32 toX, s32 toY, s32 toZ, s32 wobble); the definition has s32
 * Rope_Recoil(s32 index, s32 x, s32 y, s32 z). */
extern void Rope_Recoil__f0d92d4(s32 index, s32 x, s32 y, s32 z, s32 toX,
                                 s32 toY, s32 toZ,
                                 s32 wobble) __asm__("Rope_Recoil");

/// Lash: throws rope index from post fromId to post toId and ties it (both
/// posts take animation 2). A rope coiled too short to reach falls back to its
/// post.
void Rope_Lash(s32 index, s32 fromId, s32 toId) {
  struct ChainWork__f0d92d4 *work = Ram_HeapSlots->unk84[6];
  struct Actor *from = ObjectTable_Get(fromId);
  struct Actor *to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    s32 toX = to->x;
    s32 toY = to->y;
    s32 toZ = to->z;
    struct ChainSlot__f0d92d4 *slot = &work->slots[index];
    s32 x, y = 0, z; // Default: y.
    s32 xx, yy, length, segments, count;
    s32 (*mul)(s32, s32);

    if (((u8 *)to)[99] == 99)
      (*(TargetHook *)((u8 *)to + 104))(to, &toX, &toZ);
    x = (toX - from->x) >> 8;
    y = (toY - from->y) >> 8;
    z = (toZ - from->z) >> 8;
    mul = Iwram_MulQ16;
    xx = mul(x, x);
    yy = mul(y, y);
    length = xx + yy + mul(z, z);
    segments = Iwram_SqrtEntry(length) / 6 + 1;
    count = slot->segments;
    if (segments > count) {
      count++;
      x = from->x + (toX - from->x) * count / segments;
      y = toY;
      z = from->z + (toZ - from->z) * count / segments;

      Rope_Throw__f0d92d4(index, from->x, from->y + 0x80000, from->z, x,
                          y + 0x80000, z, Trig_Sin(gRope_LashPhase) << 2);
      slot->unk18 = 0;
      wait(1);
      Rope_Recoil__f0d92d4(index, from->x, from->y + 0x80000, from->z, x,
                           y + 0x80000, z, Trig_Sin(gRope_LashPhase) << 2);
    } else {
      Rope_Throw__f0d92d4(index, from->x, from->y + 0x80000, from->z, to->x,
                          to->y + 0x80000, to->z,
                          Trig_Sin(gRope_LashPhase) << 2);
      gRope_LashPhase += 0x1000;
      wait(6);
      Actor_SelectAnimationFar(from, 2);
      Actor_SelectAnimationFar(to, 2);
    }
  }
}

#include "module/system/system.h"

/* VIEW(f0d9498): tag struct ChainNode as struct ChainNode__f0d9498;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:5629: structure has no member
 * named `visible'. */
struct ChainNode__f0d9498 {
  struct ChainNode__f0d9498 *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
  u8 unk13;
};

/* VIEW(f0d9498): tag struct ChainSlot as struct ChainSlot__f0d9498;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:5586: structure has no member
 * named `active'. */
struct ChainSlot__f0d9498 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[2];
  s16 flag;
  s16 active;
  s32 unk0c;
  u8 unk10[4];
  s32 unk14;
  s32 unk18;
  struct ChainNode__f0d9498 *head;
};

/* VIEW(f0d9498): tag struct ChainWork as struct ChainWork__f0d9498;
 * visual_attributes.c has another definition. */
struct ChainWork__f0d9498 {
  u8 unk00[12];
  struct ChainSlot__f0d9498 slots[8];
};

/// Throws rope index from one point to another over twice its node count in
/// frames: the end flies on an arc and each node follows its neighbour at 6
/// pixels; then the rope swings and its flag is set. Does nothing to a rope
/// already thrown.
s32 Rope_Throw(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct ChainWork__f0d9498 *work = Ram_HeapSlots->unk84[6];
  s32 dx = toX - x;
  s32 ex = dx >> 8;
  s32 dy = toY - y;
  s32 ey = dy >> 8;
  s32 dz = toZ - z;
  s32 segments;
  s32 frame;
  s32 sine;
  s32 cosine;
  s32 lastX;
  s32 lastY;
  s32 lastZ;
  struct ChainSlot__f0d9498 *slot = &work->slots[index];
  struct ChainNode__f0d9498 *node = slot->head;
  void **cursor;
  s32 i;
  u16 angle;
  struct ChainNode__f0d9498 **nodes;

  if (slot->active == 1)
    return;
  slot->active = 1;
  segments = slot->segments;
  cursor = nodes = Heap_Alloc(0x200);
  for (i = 0; i <= segments; i++) {
    *cursor++ = node;
    node = node->next;
  }
  angle = ArcTan2(ex, ey);
  sine = Trig_Sin(angle);
  cosine = Trig_Cos(angle);
  for (frame = 1; frame <= segments * 2; frame++) {
    for (i = segments; i >= 0; i--) {
      node = nodes[i];
      node->unk13 = index + 1;
      if (i == segments) {
        ex = Iwram_MulQ16(Trig_Sin((frame << 14) / i), 0x80000);

        node->x = x + dx * frame / 2 / i + Iwram_MulQ16(ex, cosine);
        lastX = node->x;
        node->y = y + dy * frame / 2 / i + Iwram_MulQ16(ex, sine);
        lastY = node->y;
        node->z = z + dz * frame / 2 / i;
        lastZ = node->z;
      } else {
        s32 ez;

        ex = (lastX - node->x) >> 8;
        ey = (lastY - node->y) >> 8;
        ez = (lastZ - node->z) >> 8;
        ex = Iwram_SqrtEntry(Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) +
                             Iwram_MulQ16(ez, ez));

        if (ex > 6) {
          node->x = lastX + (node->x - lastX) * 6 / ex;
          lastX = node->x;
          node->y = lastY + (node->y - lastY) * 6 / ex;
          lastY = node->y;
          node->z = lastZ + (node->z - lastZ) * 6 / ex;
          node->visible = 1;
          lastZ = node->z;
        }
      }
    }
    wait(1);
  }
  slot = &work->slots[index];
  slot->unk18 = 1;
  slot->unk0c = 0x40000;
  slot->unk14 = 0x4000;
  if (slot->flag != 0)
    GameFlag_Set(slot->flag);
  Heap_Release(nodes);
  return 0;
}

/* VIEW(f0d9710): tag struct ChainWork as struct ChainWork__f0d9710;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:5671: structure has no member
 * named `from'. */
struct ChainWork__f0d9710 {
  struct Actor *from;
  struct Actor *to;
};

extern s32 gRope_UntiePhase;

/* VIEW(f0d9710): routine Rope_Recoil as void Rope_Recoil(s32 index, s32 x,
 * s32 y, s32 z, s32 toX, s32 toY, s32 toZ, s32 wobble); the definition has s32
 * Rope_Recoil(s32 index, s32 x, s32 y, s32 z). */
extern void Rope_Recoil__f0d9710(s32 index, s32 x, s32 y, s32 z, s32 toX,
                                 s32 toY, s32 toZ,
                                 s32 wobble) __asm__("Rope_Recoil");

/// Unties rope index between posts fromId and toId (Rope_Recoil).
void Rope_Untie(s32 index, s32 fromId, s32 toId) {
  struct ChainWork__f0d9710 *work;
  struct Actor *from;
  struct Actor *to;

  work = Ram_HeapSlots->unk84[6];
  from = ObjectTable_Get(fromId);
  to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    work->from = from;
    work->to = to;
    Rope_Recoil__f0d9710(index, from->x, from->y + 0x80000, from->z, to->x,
                         to->y + 0x80000, to->z,
                         Trig_Sin(gRope_UntiePhase) * 4);
    gRope_UntiePhase += 0x1000;
  }
}

#include "module/field/field.h"
#include "module/field/object.h"
#include "module/system/system.h"

/* VIEW(f0d9790): tag struct ChainNode as struct ChainNode__f0d9790;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:5764: structure has no member
 * named `visible'. */
struct ChainNode__f0d9790 {
  struct ChainNode__f0d9790 *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
  u8 unk13;
};

/* VIEW(f0d9790): tag struct ChainSlot as struct ChainSlot__f0d9790;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:5725: structure has no member
 * named `active'. */
struct ChainSlot__f0d9790 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[2];
  s16 flag;
  s16 active;
  u8 unk0c[12];
  s32 unk18;
  struct ChainNode__f0d9790 *head;
};

/* VIEW(f0d9790): tag struct ChainWork as struct ChainWork__f0d9790;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:5749: structure has no member
 * named `from'. */
struct ChainWork__f0d9790 {
  struct Actor *from;
  struct Actor *to;
  u8 unk08[4];
  struct ChainSlot__f0d9790 slots[8];
};

/// Pulls a thrown rope back into its coil at the first point, one node a frame,
/// clearing its flag; both posts take animation 3 first.
s32 Rope_Recoil(s32 index, s32 x, s32 y, s32 z) {
  struct ChainWork__f0d9790 *work = Ram_HeapSlots->unk84[6];
  s32 zero = 0;
  struct ChainSlot__f0d9790 *slot = &work->slots[index];
  struct ChainNode__f0d9790 *node = slot->head;
  struct ChainNode__f0d9790 **cursor;
  s32 i;
  s32 angle;
  s32 radius;
  s32 lastZ;
  s32 segments;
  s32 frame;
  s32 lastX;
  s32 lastY;
  struct ChainNode__f0d9790 **nodes;

  if (slot->active == 0)
    return;
  slot->active = zero;
  segments = slot->segments;
  cursor = nodes = Heap_Alloc(0x200);
  slot->unk18 = zero;
  if (slot->flag != 0)
    GameFlag_Clear(slot->flag);
  if (segments >= 0) {
    i = segments + 1;
    do {
      i += -1;
      /* FAKEMATCH(barrier): an empty asm reading the counter in a low register,
       * which copies it for the compare ahead of the store; without it, 2 of
       * 225 instructions differ in tla-en. */
      asm("" : : "l"(i));
      *cursor++ = node;
      node = node->next;
    } while (i != 0);
  }
  Actor_SelectAnimationFar(work->from, 3);
  Actor_SelectAnimationFar(work->to, 3);
  wait(4);
  for (frame = 0; frame <= segments; frame++) {
    angle = frame << 12;
    radius = 0x40000;
    for (i = 0; i <= segments; i++) {
      node = nodes[i];
      node->unk13 = 0;
      if (i <= frame) {
        u16 step;

        node->x = x + Iwram_MulQ16(Trig_Cos(angle), radius);
        lastX = node->x;
        node->y = y;
        lastY = node->y;
        node->z = z + Iwram_MulQ16(Trig_Sin(angle), radius);
        lastZ = node->z;
        node->visible = 2;
        step = ArcTan2(0x60000, radius);
        angle += step;
        radius += step * 4;
      } else {
        s32 dx = (lastX - node->x) >> 8;
        s32 dy = (lastY - node->y) >> 8;
        s32 dz = (lastZ - node->z) >> 8;

        radius = Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                                 Iwram_MulQ16(dz, dz));

        if (radius <= 6)
          break;
        node->x = lastX + (node->x - lastX) * 6 / radius;
        lastX = node->x;
        node->y = lastY + (node->y - lastY) * 6 / radius;
        lastY = node->y;
        node->z = lastZ + (node->z - lastZ) * 6 / radius;
        node->visible = 2;
        lastZ = node->z;
      }
    }
    wait(1);
  }
  Heap_Release(nodes);
  return 0;
}

/// Does nothing; nothing calls it.
void Rope_ReservedNoOp(void) {}

extern s32 gRope_DebugSwinging;

/// Unused: nothing refers to it. An actor hook that, once R is pressed, swings
/// rope 0 between actors 12 and 13 every frame.
void Rope_DebugSwing(struct Actor *object) {
  u8 *parts = object->sprite;

  parts[22] = 253;
  parts[23] = 12;
  if (gInput.repeat & 0x200)
    gRope_DebugSwinging = 1;
  if (gRope_DebugSwinging != 0)
    Rope_SwingBetween(0, 12, 13, 0);
}

struct Actor *Actor_GetById(u32 objectId);

/// Returns 1 when the leader stands on the cell of post firstId or secondId, or
/// a cell beside it along an axis, at the same height.
s32 Rope_IsLeaderAtEnd(s32 firstId, s32 secondId) {
  struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *first = Actor_GetById(firstId);
  struct Actor *second = Actor_GetById(secondId);
  s32 x = owner->x >> 20;
  s32 y = owner->y >> 20;
  s32 z = owner->z >> 20;
  s32 firstX = first->x >> 20;
  s32 firstY = first->y >> 20;
  s32 firstZ = first->z >> 20;
  s32 secondX = second->x >> 20;
  s32 secondY = second->y >> 20;
  s32 secondZ = second->z >> 20;

  if (y == firstY) {
    if (x == firstX) {
      if (z == firstZ - 1 || z == firstZ + 1 || z == firstZ)
        return 1;
    } else if (z == firstZ) {
      if (x == firstX - 1 || x == firstX + 1 || x == firstX)
        return 1;
    }
  }
  if (y == secondY) {
    if (x == secondX) {
      if (z == secondZ - 1 || z == secondZ + 1 || z == secondZ)
        return 1;
    } else if (z == secondZ) {
      if (x == secondX - 1 || x == secondX + 1 || x == secondX)
        return 1;
    }
  }
  return 0;
}

/// Finds the rope whose first post is id: writes its other post and its index
/// and returns 0, or returns -1.
s32 Rope_FindById(s32 id, s32 *value, s32 *index) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot = work->slots;
  s32 i;

  for (i = 0; i <= 7; i++) {
    if (slot->head != NULL && slot->id == id) {
      *value = slot->value;
      *index = i;
      return 0;
    }
    slot++;
  }
  return -1;
}

/// Returns rope index's state (1 once tied), or -1 while the ropes have no
/// work.
s32 Rope_GetState(s32 index) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot;

  if (work == NULL)
    return -1;
  slot = &work->slots[index];
  return slot->state;
}

/// Gives rope index its flag and places it between posts a2 and a3: tied when
/// the flag is set, else coiled. A scene places each of its ropes with it.
void Rope_Place(s32 index, s32 flag, s32 a2, s32 a3) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot;

  slot = &work->slots[index];
  slot->flag = flag;
  if (!GameFlag_Get(flag))
    Rope_Coil(index, a2, a3);
  else
    Rope_Tie(index, a2, a3);
}

// The tightropes (heap slot 160): ropes laid straight between two posts that
// the party walks (render mode 4) and that wobble underfoot. The family copies
// the ropes' routines. XXX: Telago's walkthrough calls them tightropes (Trial
// Road, Gondowan Cliffs); a render would confirm the name.

/// Allocates the tightropes' work, loads their node picture and starts
/// Tightrope_Draw and Tightrope_Update.
void Tightrope_Initialize(void) {
  struct WorkA0 *work = HeapSlot_AllocEwram(160, sizeof(struct WorkA0));
  void *buffer = Heap_Alloc(0x900);

  Dma_Fill32(0, work, sizeof(struct WorkA0));
  work->vramEntry = VramCache_FindFreeEntry();
  Decompress_TaggedLz(gRope_Tiles, buffer);
  VramCache_Load(work->vramEntry, 64, buffer);
  Task_Add(Tightrope_Draw, TASK_PASS_VBLANK);
  Task_Add(Tightrope_Update, TASK_PASS_VBLANK + 15);
  Heap_Release(buffer);
}

#include "module/field/field.h"
#include "module/field/field_sequence.h"
#include "module/field/object.h"
#include "module/system/system.h"

/* VIEW(f0d9b80): tag struct ChainSlot as struct ChainSlot__f0d9b80;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:5953: structure has no member
 * named `angle'. */
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

/* VIEW(f0d9b80): tag struct ChainWork as struct ChainWork__f0d9b80;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:5958: structure has no member
 * named `angle'. */
struct ChainWork__f0d9b80 {
  u8 unk000[8];
  s32 vramEntry;
  struct ChainSlot__f0d9b80 slots[8];
};

/* VIEW(f0d9b80): routine OamMatrix_AllocFromTransform as s32
 * OamMatrix_AllocFromTransform(struct ChainTransform *source); affine.h has s32
 * OamMatrix_AllocFromTransform(struct AffineTransform *source). */
extern s32 AffineMatrix_BuildForEffect__f0d9b80(
    struct ChainTransform *source) __asm__("OamMatrix_AllocFromTransform");
/* VIEW(f0d9b80): routine Oam_AddSpriteAtDepthClamped as void
 * Oam_AddSpriteAtDepthClamped(void *entry, s32 slot); f0d6674 has void
 * Oam_AddSpriteAtDepthClamped(struct GroundParticle *sprite, s32 depth). */
extern void
Func_080140d8__f0d9b80(void *entry,
                       s32 slot) __asm__("Oam_AddSpriteAtDepthClamped");

void Tightrope_Draw(void) {
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
      matrix = AffineMatrix_BuildForEffect__f0d9b80(&transform);
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
          Func_080140d8__f0d9b80(&node->sprite, key);
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

/// The tightropes' per-frame task (Tightrope_Initialize adds it).
/// Unless flag 260 is set, once the event work's counter passes 12 it picks
/// the event work's cue: with render mode 0 the slot linking the leader's
/// nearby talk target sets 0xfc83 when Rope_IsLeaderAtEnd accepts its two ends,
/// and render mode 4 sets 0xfc84. Then every active slot wobbles: the amplitude
/// decays by 0.95 a frame until it drops to 0x1998, and Tightrope_WobbleBetween
/// redraws the slot with the sine of its angle times it.
void Tightrope_Update(void) {
  struct WobbleEvent *event = Ram_HeapSlots->eventWork;
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Slot *slot;
  s32 i = 0;

  if (!GameFlag_Get(260) && event->counter > 12) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->renderMode == 0) {
      s32 target = Field_FindNearbyTalkTarget(party->currentOwner);

      if (target != -1) {
        for (i = 0; i < 8; i++) {
          if (work->slots[i].first == target || work->slots[i].second == target)
            break;
        }
        if (i < 8 &&
            Rope_IsLeaderAtEnd(work->slots[i].first, work->slots[i].second))
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
      Tightrope_WobbleBetween(i, slot->first, slot->second, wobble);
    }
  }
}

/// Returns the first unused of the tightropes' 128 nodes, or NULL.
struct WorkA0Node *Tightrope_AllocNode(void) {
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

/// Lays tightrope slotNo straight between posts firstId and secondId, and
/// shows both posts with it (animation 4).
void Tightrope_Place(s32 slotNo, s32 firstId, s32 secondId) {
  struct WorkA0 *work = WORK_A0;
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    struct WorkA0Slot *slot = &work->slots[slotNo];

    slot->first = firstId;
    slot->second = secondId;
    Tightrope_Lay(slotNo, first->x, first->y + 0x80000, first->z, second->x,
                  second->y + 0x80000, second->z);
    Actor_SelectAnimationFar(first, 4);
    Actor_SelectAnimationFar(second, 4);
  }
}

/// Builds tightrope index's nodes on the straight line between two points,
/// one for each 6 pixels.
s32 Tightrope_Lay(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
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
    struct LineNode *node = Tightrope_AllocNode();

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

/// Redraws tightrope index between posts from and to, sagging by mode.
void Tightrope_WobbleBetween(s32 index, s32 from, s32 to, s32 mode) {
  struct Actor *a = ObjectTable_Get(from);
  struct Actor *b = ObjectTable_Get(to);

  if (a != NULL && b != NULL)
    Tightrope_LayBowed(index, a->x, a->y + 0x80000, a->z, b->x, b->y + 0x80000,
                       b->z, mode);
}

s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

s32 Tightrope_LayBowed(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY,
                       s32 toZ, s32 height) {
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

/// Unused: nothing refers to it. Rope_Coil for a tightrope.
void Tightrope_Coil(s32 index, s32 from, s32 to) {
  struct WorkA0 *work = WORK_A0;
  struct Actor *a = ObjectTable_Get(from);
  struct Actor *b = ObjectTable_Get(to);

  if (a != NULL && b != NULL) {
    struct WorkA0Slot *slot = &work->slots[index];

    slot->first = from;
    slot->second = to;
    Tightrope_LayCoil(index, a->x, a->y + 0x80000, a->z, b->x, b->y + 0x80000,
                      b->z);
  }
}

// Builds a chain of nodes around a point.

/* VIEW(f0d9b80): routine Rope_AllocNode as struct LineNode
 * *Rope_AllocNode(void); f0d9194 has struct ChainNode__f0d9194
 * *Rope_AllocNode(void). */
extern struct LineNode *Rope_AllocNode__f0d9b80(void) __asm__("Rope_AllocNode");

/// Builds a slot's chain of nodes, one for each 6 units of the distance between
/// two points, laid out as a flat spiral around the first point, every node of
/// kind 2.
s32 Tightrope_LayCoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY,
                      s32 toZ) {
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
    struct LineNode *node = Rope_AllocNode__f0d9b80();
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

extern s32 gTightrope_LashPhase;
/* VIEW(f0d9b80): routine Tightrope_Throw as void Tightrope_Throw(s32 arg, s32
 * x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2, s32 wave); the definition has s32
 * Tightrope_Throw(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ).
 */
extern void Tightrope_Throw__f0d9b80(s32 arg, s32 x1, s32 y1, s32 z1, s32 x2,
                                     s32 y2, s32 z2,
                                     s32 wave) __asm__("Tightrope_Throw");

/// Unused: nothing refers to it. Rope_Lash for a tightrope, without the
/// short rope's fall.
void Tightrope_Lash(s32 arg, s32 firstId, s32 secondId) {
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    Tightrope_Throw__f0d9b80(arg, first->x, first->y + 0x80000, first->z,
                             second->x, second->y + 0x80000, second->z,
                             Trig_Sin(gTightrope_LashPhase) << 2);
    gTightrope_LashPhase += 0x1000;
    wait(6);
    Actor_SelectAnimationFar(first, 2);
    Actor_SelectAnimationFar(second, 2);
  }
}

#include "item_icon.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "module/system/system.h"

/// Rope_Throw for a tightrope.
s32 Tightrope_Throw(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct WorkA0 *work = WORK_A0;
  s32 dx = toX - x;
  s32 ex = dx >> 8;
  s32 dy = toY - y;
  s32 ey = dy >> 8;
  s32 dz = toZ - z;
  s32 ez = dz >> 8;
  s32 segments = Iwram_SqrtEntry(Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) +
                                 Iwram_MulQ16(ez, ez)) /
                     6 +
                 1;
  s32 frame;
  struct WorkA0Node *node = work->slots[index].head;
  s32 sine;
  s32 height;
  s32 cosine;
  s32 lastX, lastY, lastZ;
  struct WorkA0Node **nodes;
  struct WorkA0Node **cursor;
  struct WorkA0Slot *slot;
  s32 i;
  u16 angle;

  cursor = nodes = Heap_Alloc(0x200);
  if (segments >= 0) {
    i = segments + 1;
    do {
      i += -1;
      /* FAKEMATCH(barrier): an empty asm reading the counter in a low register,
       * which copies it for the compare ahead of the store; without it, 2 of
       * 319 instructions differ in tla-en. */
      asm("" : : "l"(i));
      *cursor++ = node;
      node = node->next;
    } while (i != 0);
  }
  angle = ArcTan2(ex, ey);
  sine = Trig_Sin(angle);
  cosine = Trig_Cos(angle);
  for (frame = 1; frame <= segments * 2; frame++) {
    for (i = segments; i >= 0; i--) {
      node = nodes[i];
      // XXX: byte 17 of a node is the slot it belongs to, plus one; likely
      // what the per-frame callbacks read to draw it.
      node->unk11[0] = index + 1;
      if (i == segments) {
        s32 offsetX;
        s32 offsetY;

        height = Iwram_MulQ16(Trig_Sin((frame << 14) / i), 0x80000);
        offsetX = Iwram_MulQ16(height, cosine);
        lastX = node->x = x + dx * frame / 2 / i + offsetX;
        offsetY = Iwram_MulQ16(height, sine);
        lastY = node->y = y + dy * frame / 2 / i + offsetY;
        lastZ = node->z = z + dz * frame / 2 / i;
      } else {
        ex = (lastX - node->x) >> 8;
        ey = (lastY - node->y) >> 8;
        ez = (lastZ - node->z) >> 8;
        height = Iwram_SqrtEntry(Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) +
                                 Iwram_MulQ16(ez, ez));

        if (height <= 6)
          break;
        lastX = node->x = lastX + (node->x - lastX) * 6 / height;
        lastY = node->y = lastY + (node->y - lastY) * 6 / height;
        lastZ = node->z = lastZ + (node->z - lastZ) * 6 / height;
        node->used = 1;
      }
    }
    wait(1);
  }
  slot = &work->slots[index];
  // XXX: the field worka0.h calls tail is tested as a flag by Tightrope_Update;
  // a pointer here only because the adopted header types it so.
  slot->tail = (struct WorkA0Node *)1;
  slot->amplitude = 0x40000;
  slot->angle = 0x4000;
  Heap_Release(nodes);
  return 0;
}

extern s32 gTightrope_UntiePhase;

/// Unused: nothing refers to it. Rope_Untie for a tightrope.
void Tightrope_Untie(s32 arg, s32 firstId, s32 secondId) {
  struct Actor **work;
  struct Actor *first;
  struct Actor *second;

  work = (struct Actor **)WORK_A0;
  first = ObjectTable_Get(firstId);
  second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    work[0] = first;
    work[1] = second;
    Tightrope_Recoil(arg, first->x, first->y + 0x80000, first->z, second->x,
                     second->y + 0x80000, second->z,
                     Trig_Sin(gTightrope_UntiePhase) << 2);
    gTightrope_UntiePhase += 0x1000;
  }
}

/// Rope_Recoil for a tightrope.
s32 Tightrope_Recoil(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                     s32 wave) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *tail = NULL;
  struct WorkA0Node *node;
  s32 dx, dy, dz;
  s32 dist;
  s32 count;
  s32 frame;
  s32 i;
  s32 angle;
  s32 x, y, z;
  struct WorkA0Node **nodes;
  struct WorkA0Node **entry;

  dx = (x2 - x1) >> 8;
  dy = (y2 - y1) >> 8;
  dz = (z2 - z1) >> 8;
  dist = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);
  count = Iwram_Sqrt(dist) / 6 + 1;
  node = work->slots[slot].head;
  entry = nodes = Heap_Alloc(0x200);
  work->slots[slot].tail = tail;
  if (count >= 0) {
    i = count + 1;
    do {
      i += -1;
      /* FAKEMATCH(barrier): an empty asm reading the counter in a low register,
       * which copies it for the compare ahead of the store; without it, 2 of
       * 259 instructions differ in tla-en. */
      asm("" : : "l"(i));
      *entry++ = node;
      node = node->next;
    } while (i != 0);
  }
  Actor_SelectAnimationFar(((void **)work)[0], 3);
  Actor_SelectAnimationFar(((void **)work)[1], 3);
  wait(4);
  for (frame = 0; frame <= count; frame++) {
    angle = frame << 12;
    dist = 0x40000;
    for (i = 0; i <= count; i++) {
      node = nodes[i];
      node->unk11[0] = 0;
      if (i <= frame) {
        x = node->x = x1 + Iwram_MulQ16(Trig_Cos(angle), dist);
        y = node->y = y1;
        z = node->z = z1 + Iwram_MulQ16(Trig_Sin(angle), dist);
        node->used = 2;
        {
          u16 step = ArcTan2(0x60000, dist);

          angle += step;
          dist += step * 4;
        }
      } else {
        dx = (x - node->x) >> 8;
        dy = (y - node->y) >> 8;
        dz = (z - node->z) >> 8;
        dist =
            Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);
        dist = Iwram_Sqrt(dist);
        if (dist <= 6)
          break;
        x = node->x = x + (node->x - x) * 6 / dist;
        y = node->y = y + (node->y - y) * 6 / dist;
        node->z = z + (node->z - z) * 6 / dist;
        node->used = 2;
        z = node->z;
      }
    }
    wait(1);
  }
  Heap_Release(nodes);
  return 0;
}

extern s32 gTightrope_DebugSwinging;

/// Frees tightrope index's nodes; it is no longer drawn.
void Tightrope_Release(s32 index) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *node;

  for (node = work->slots[index].head; node != NULL; node = node->next)
    node->used = 0;
  work->slots[index].head = NULL;
}

/// Stops tightrope index's wobble.
void Tightrope_Settle(s32 index) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *node;

  for (node = work->slots[index].head; node != NULL; node = node->next)
    node->used = 2;
  work->slots[index].tail = NULL;
}

/// Unused: nothing refers to it. Rope_DebugSwing for a tightrope.
void Tightrope_DebugSwing(struct Actor *object) {
  u8 *parts = object->sprite;

  parts[22] = 253;
  parts[23] = 12;
  if (gInput.repeat & 0x200)
    gTightrope_DebugSwinging = 1;
  if (gTightrope_DebugSwinging != 0)
    Tightrope_WobbleBetween(0, 12, 13, 0);
}

// The four 256-byte pictures of each set (type-01 compressed).
extern const u8 Data_080f0c1c[], Data_080f0c63[], Data_080f0cb8[];
extern const u8 Data_080f0d07[], Data_080f0d5e[], Data_080f0dbb[];

#define TILE_EFFECT_SLOT 164
#define TILE_EFFECT_KIND_ALT 372 // the kind that takes the first picture set
#define PICTURE_SIZE 0x100

// The hanging vines (heap slot 164): eight slots, each seven links hung below
// an anchor object, which sway, swing less while the leader stands under
// them, and carry the leader across a gap. XXX: Telago's walkthrough has
// Felix swing on a vine hanging over a gap once Whirlwind moves it (Gaia
// Rock); a render would confirm the name and the swing.

/// Starts the hanging vines, a field effect that animates sixteen tiles of its
/// own: takes a
/// 0x10e4-byte work block at heap offset 164 (worka0.h's WorkA4 lies there),
/// clears it, decodes four 256-byte tile pictures into a scratch buffer (one
/// set for kind 372, another for the rest; the second set repeats one
/// picture), loads them into a VRAM slot and schedules the two per-frame
/// routines HangingVine_Draw and HangingVine_Update.
struct TileEffectWork *HangingVine_Initialize(s32 kind) {
  struct TileEffectWork *work;
  u8 *pictures;
  s32 size;

  work = HeapSlot_AllocEwram(TILE_EFFECT_SLOT, sizeof(struct TileEffectWork));
  size = 4 * PICTURE_SIZE;
  pictures = Heap_Alloc(size);
  Dma_Fill32(0, work, sizeof(struct TileEffectWork));
  work->vramEntry = VramCache_FindFreeEntry();
  if (kind == TILE_EFFECT_KIND_ALT) {
    Decompress_TaggedLz(Data_080f0c1c, pictures);
    Decompress_TaggedLz(Data_080f0c63, pictures + PICTURE_SIZE);
    Decompress_TaggedLz(Data_080f0cb8, pictures + 2 * PICTURE_SIZE);
    Decompress_TaggedLz(Data_080f0d07, pictures + 3 * PICTURE_SIZE);
    VramCache_Load(work->vramEntry, size, pictures);
  } else {
    Decompress_TaggedLz(Data_080f0d5e, pictures);
    Decompress_TaggedLz(Data_080f0dbb, pictures + PICTURE_SIZE);
    Decompress_TaggedLz(Data_080f0dbb, pictures + 2 * PICTURE_SIZE);
    Decompress_TaggedLz(Data_080f0e00, pictures + 3 * PICTURE_SIZE);
    VramCache_Load(work->vramEntry, size, pictures);
    /* FAKEMATCH(barrier): an empty asm closing the arm keeps each decode's
     * destination set before its source loads; without it, 4 of 95
     * instructions differ in tla-en. */
    asm("");
  }
  {
    s32 priority = 0x480;

    Task_Add(HangingVine_Draw, priority);
  }
  Task_Add(HangingVine_Update, 0x48f);
  Heap_Release(pictures);
  return work;
}

/* VIEW(f0da3bc): routine Oam_AddSpriteAtDepthClamped as void
 * Oam_AddSpriteAtDepthClamped(void *entry, s32 slot); f0d6674 has void
 * Oam_AddSpriteAtDepthClamped(struct GroundParticle *sprite, s32 depth). */
extern void
Func_080140d8__f0da3bc(void *entry,
                       s32 slot) __asm__("Oam_AddSpriteAtDepthClamped");

// The transform as this function builds it, a word at a time.

/* VIEW(f0da3bc): tag struct ChainSlot as struct ChainSlot__f0da3bc;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:6634: structure has no member
 * named `root'. */
struct ChainSlot__f0da3bc {
  s16 id;
  u8 unk02[22];
  struct ChainLink *root;
};

/* VIEW(f0da3bc): tag struct ChainWork as struct ChainWork__f0da3bc;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:6624: structure has no member
 * named `sheet'. */
struct ChainWork__f0da3bc {
  s32 sheet; ///< the resource entry of the links' tiles
  struct ChainSlot__f0da3bc slots[8];
};

/// The vines' first per-frame task: draws each link turned along its vine.
void HangingVine_Draw(void) {
  struct ChainWork__f0da3bc *work = Ram_HeapSlots->unk84[8];
  s32 i;
  u32 angle;
  s32 prevX = 0;
  s32 prevY = 0;
  u32 tiles = gResourceTableEntries[work->sheet].offset >> 5;
  s32 prevZ = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 height = map->origin[1];
  struct ChainTransform transform;
  struct ChainLink *link;

  for (i = 0; i < 8; i++) {
    for (link = work->slots[i].root; link != NULL; link = link->next) {
      s32 x = link->x;
      s32 y = link->y;
      s32 z = link->z;

      if (link->kind > 1) {
        s32 dz = prevZ - z;
        s32 absZ = dz >= 0 ? dz : z - prevZ;
        s32 dy = prevY - y;
        s32 screenX, screenY;
        u32 tile;

        if (dy >= 0 ? absZ < dy : absZ < y - prevY)
          angle = (u16)ArcTan2(prevX - x, dy);
        else
          angle = (u16)ArcTan2(prevX - x, dz);
        tile = tiles + link->kind * 8 - 16;
        screenX = (prevX + x) / 2 - viewX;
        prevY = (prevY + y) / 2 - height;
        prevZ = (prevZ + z) / 2 - viewZ - height;
        screenY = prevZ - prevY;
        if ((u32)(screenX + 0x1fffff) <= 0x12ffffe && screenY > -0x200000 &&
            screenY <= 0xdfffff) {
          s32 sx = ((screenX >> 16) - 8) & 0x1ff;
          s32 sy = ((screenY >> 16) - 8) & 0xff;
          s32 priority = ((prevY + prevZ) >> 16) + 34;
          struct AnimationSpritePart *sprite = &link->sprite;

          ((u32 *)sprite)[0] = 0;
          ((u32 *)&link->sprite)[1] = sy | (sx << 16) | 0x40002000;
          ((u32 *)&link->sprite)[2] = tile | 0x800;
          if (angle != 0) {
            transform.angle = angle;
            transform.scaleX = 0x100;
            transform.scaleY = 0x100;
            link->sprite.affine = 1;
            link->sprite.affineIndex = OamMatrix_AllocFromTransform(
                (struct AffineTransform *)&transform);
          }
          link->angle = angle;
          Func_080140d8__f0da3bc(sprite, priority);
        }
      }
      prevX = x;
      prevY = y;
      prevZ = z;
    }
    prevX = 0;
    prevY = 0;
    link = work->slots[i].root;
    prevZ = 0;
    if (link != NULL)
      link->angle = angle;
  }
}
#undef TILE_EFFECT_SLOT
#undef TILE_EFFECT_KIND_ALT
#undef PICTURE_SIZE

/* VIEW(f0dacb8): tag struct ChainSlot as struct ChainSlot__f0dacb8;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:6731: structure has no member
 * named `root'. */
struct ChainSlot__f0dacb8 {
  s16 objectId;
  u8 unk02[2];
  s32 swing;   // the arc's width, eased each frame
  s32 spacing; // between links
  u8 unk08[4];
  s32 phase;
  s32 active;
  struct ChainLink *root;
};

/* VIEW(f0dacb8): tag struct ChainWork as struct ChainWork__f0dacb8;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, HangingVine_Update moves: 1 of 173 instructions in
 * tla-ja.
 */
struct ChainWork__f0dacb8 {
  u8 unk00[4];
  struct ChainSlot__f0dacb8 slots[8];
};

struct Actor *Actor_GetById(u32 objectId);

#define CHAIN_WORK ((struct ChainWork__f0dacb8 *)Ram_HeapSlots->unk84[8])

/// The vines' second per-frame task: sways each active vine's links below
/// its anchor.
void HangingVine_Update(void) {
  struct ChainWork__f0dacb8 *work = CHAIN_WORK;
  struct ChainSlot__f0dacb8 *slot;
  struct Actor *leader;
  s32 x, y, z;
  struct ChainLink *link;
  s32 i;
  s32 target;
  s32 phase;
  s32 swing;
  s32 sway;
  s32 radius;
  s32 angle;

  i = 0;
  leader = Actor_GetById(Party_GetLeaderActor());
  target = 0x100;
  for (slot = work->slots; i <= 7; i++, slot++) {
    if (slot->root == 0 || slot->active == 0)
      continue;

    slot->phase += 0x200;
    link = slot->root->next;
    x = link->x;
    y = link->y;
    z = link->z;
    swing = slot->swing;

    // Swing less while the leader stands within 8 pixels across, a pixel
    // deep, and between 8 above and 80 below the anchor.
    if ((x - leader->x >= 0 ? x - leader->x <= 0x7ffff
                            : leader->x - x <= 0x7ffff) &&
        (u32)(z - leader->z + 0xfffff) <= 0xffffe &&
        y + (8 << 16) > leader->y && y - (80 << 16) < leader->y)
      target = 32;

    // Hang the five links below the anchor.
    // BUG: the link loop reuses the chain counter, which it leaves at 6, so
    // after the first active chain only the chain after it is updated.
    link = link->next;
    for (i = 1, sway = swing; i <= 5; i++) {
      radius = i * slot->spacing;
      angle = Iwram_MulQ16(Trig_Sin(slot->phase - i * 800), swing + sway / 2);
      link->x = x - Iwram_MulQ16(Trig_Sin(angle), radius);
      sway += swing;
      link->y = y - Iwram_MulQ16(Trig_Cos(angle), radius);
      link->z = z;
      link = link->next;
    }

    // Ease the swing towards the target.
    if (slot->swing > target)
      slot->swing = Iwram_MulQ16(slot->swing, 0xfd70);
    else
      slot->swing = Iwram_MulQ16(slot->swing, 0x11c28);
  }
}

/// Returns the first of WORK_A4's eight slots with no chain, or NULL.
struct WorkA4Slot *HangingVine_FindFreeSlot(void) {
  struct WorkA4 *work = WORK_A4;
  struct WorkA4Slot *found = NULL;
  s32 index;
  /* FAKEMATCH(dead): index set to 0 before the loop sets it again, so its
   * zero is copied from found's register (adds r1, r0, #0); without it, 2 of
   * 26 instructions differ in tla-en. */
  index = 0;

  for (index = 0; index <= 7; index++) {
    if (work->slots[index].root == NULL) {
      found = &work->slots[index];
      break;
    }
  }
  return found;
}

/// Returns the vine slot among WORK_A4's eight that holds actor id, or NULL.
struct WorkA4Slot *HangingVine_FindSlot(s32 id) {
  struct WorkA4 *work = WORK_A4;
  struct WorkA4Slot *found = NULL;
  s32 index;
  /* FAKEMATCH(dead): index set to 0 before the loop sets it again, so its
   * zero is copied from found's register (adds r1, r5, #0); without it, 2 of
   * 27 instructions differ in tla-en. */
  index = 0;

  for (index = 0; index <= 7; index++) {
    if (work->slots[index].id == id) {
      found = &work->slots[index];
      break;
    }
  }
  return found;
}

struct WorkA4Item *HangingVine_AllocLink(void) {
  struct WorkA4 *work = WORK_A4;
  struct WorkA4Item *item = work->items;
  s32 index;

  for (index = 0; index <= 127; index++) {
    if (item->used == 0)
      return item;
    item++;
  }
  return NULL;
}
#undef CHAIN_WORK
