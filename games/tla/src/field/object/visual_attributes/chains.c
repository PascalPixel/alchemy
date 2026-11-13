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

/// Lays a slot's chain of nodes along the line between two points, bowed
/// sideways by a half sine of the given height.
s32 Rope_LayBowed(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                  s32 height) {
  struct RopeWork *work = ROPE_WORK;
  s32 dx = toX - x;
  s32 ax = dx >> 8;
  s32 dy = toY - y;
  struct RopeSlot *slot = &work->slots[index];
  struct RopeNode *node = slot->head;
  u16 angle = ArcTan2__f0d8ff8(ax, dy >> 8);
  s32 sin = Trig_Sin(angle);
  s32 cos = Trig_Cos(angle);
  s32 segments = slot->segments;
  s32 i;
  s32 kind = 1; // One constant for its whole life.
  {
    for (i = 0; i <= segments; i++) {
      s32 bow = Iwram_MulQ16(Trig_Sin((i << 15) / segments), height);

      node->x = x + i * dx / segments + Iwram_MulQ16(bow, cos);
      node->y = y + i * dy / segments + Iwram_MulQ16(bow, sin);
      node->z = z + i * (toZ - z) / segments;
      node->kind = kind;
      node = node->next;
    }
  }
  return 0;
}

/// Coils rope index at post firstId, its length reaching post secondId: the
/// rope as it waits to be lashed.
void Rope_Coil(s32 index, s32 firstId, s32 secondId) {
  struct RopeWork *work = ROPE_WORK;
  s32 priority = 2;
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    struct RopeSlot *slot;
    s32 lift;

    if (first->animationKind == 1)
      priority = ((struct FieldSprite *)first->sprite)->priority;
    slot = &work->slots[index];
    slot->first = firstId;
    slot->second = secondId;
    slot->priority = priority;
    lift = 128;
    /* FAKEMATCH(barrier): an empty asm on lift keeps its 128 built after the
     * priority's store and ahead of the coordinates' loads; without it, 10 of
     * 69 instructions differ in tla-en. */
    asm volatile("" : "+l"(lift));
    lift <<= 12;
    Rope_LayCoil(index, first->x, first->y + lift, first->z, second->x,
                 second->y + lift, second->z);
  }
}

/// Builds a slot's chain of nodes, one for each 6 units of the distance between
/// two points, laid out as a flat spiral around the first point.
s32 Rope_LayCoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct RopeWork *work = ROPE_WORK;
  s32 result = 0;
  s32 dx = (toX - x) >> 8;
  s32 dy = (toY - y) >> 8;
  s32 dz = (toZ - z) >> 8;
  s32 length = Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                               Iwram_MulQ16(dz, dz));
  s32 segments = length / 6 + 1;
  struct RopeSlot *slot = &work->slots[index];
  struct RopeNode **link = &slot->head;
  s32 angle = 0;
  s32 radius = 0x40000;
  s32 i;
  s32 kind = 2; // One constant for its whole life.

  slot->segments = segments;
  for (i = 0; i <= segments; i++) {
    struct RopeNode *node = Rope_AllocNode();
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

/// Writes the position of the node at rope index's depth into position.
void Rope_GetEndNodePosition(s32 index, s32 *position) {
  struct RopeWork *work = ROPE_WORK;
  struct RopeSlot *slot = &work->slots[index];
  struct RopeNode *node = slot->head;
  s32 depth;

  depth = slot->segments;
  if (depth > 0) {
    do {
      depth--;
      node = node->next;
    } while (depth != 0);
  }
  SET_VECTOR(position, node->x, node->y, node->z);
}

#include "module/field/field.h"

extern s32 gRope_LashPhase;

/// Lash: throws rope index from post fromId to post toId and ties it (both
/// posts take animation 2). A rope coiled too short to reach falls back to its
/// post.
void Rope_Lash(s32 index, s32 fromId, s32 toId) {
  struct RopeWork *work = ROPE_WORK;
  struct Actor *from = ObjectTable_Get(fromId);
  struct Actor *to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    s32 toX = to->x;
    s32 toY = to->y;
    s32 toZ = to->z;
    struct RopeSlot *slot = &work->slots[index];
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

      Rope_Throw(index, from->x, from->y + 0x80000, from->z, x, y + 0x80000, z,
                 Trig_Sin(gRope_LashPhase) << 2);
      slot->swinging = 0;
      wait(1);
      Rope_Recoil(index, from->x, from->y + 0x80000, from->z, x, y + 0x80000, z,
                  Trig_Sin(gRope_LashPhase) << 2);
    } else {
      Rope_Throw(index, from->x, from->y + 0x80000, from->z, to->x,
                 to->y + 0x80000, to->z, Trig_Sin(gRope_LashPhase) << 2);
      gRope_LashPhase += 0x1000;
      wait(6);
      Actor_SelectAnimationFar(from, 2);
      Actor_SelectAnimationFar(to, 2);
    }
  }
}

#include "module/system/system.h"

/// Throws rope index from one point to another over twice its node count in
/// frames: the end flies on an arc and each node follows its neighbour at 6
/// pixels; then the rope swings and its flag is set. Does nothing to a rope
/// already thrown.
s32 Rope_Throw(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct RopeWork *work = ROPE_WORK;
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
  struct RopeSlot *slot = &work->slots[index];
  struct RopeNode *node = slot->head;
  void **cursor;
  s32 i;
  u16 angle;
  struct RopeNode **nodes;

  if (slot->state == 1)
    return;
  slot->state = 1;
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
      node->slot = index + 1;
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
          node->kind = 1;
          lastZ = node->z;
        }
      }
    }
    wait(1);
  }
  slot = &work->slots[index];
  slot->swinging = 1;
  slot->amplitude = 0x40000;
  slot->phase = 0x4000;
  if (slot->flag != 0)
    GameFlag_Set(slot->flag);
  Heap_Release(nodes);
  return 0;
}

extern s32 gRope_UntiePhase;

/// Unties rope index between posts fromId and toId (Rope_Recoil).
void Rope_Untie(s32 index, s32 fromId, s32 toId) {
  struct RopeWork *work;
  struct Actor *from;
  struct Actor *to;

  work = ROPE_WORK;
  from = ObjectTable_Get(fromId);
  to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    work->recoilFrom = from;
    work->recoilTo = to;
    Rope_Recoil(index, from->x, from->y + 0x80000, from->z, to->x,
                to->y + 0x80000, to->z, Trig_Sin(gRope_UntiePhase) * 4);
    gRope_UntiePhase += 0x1000;
  }
}

#include "module/field/field.h"
#include "module/field/object.h"
#include "module/system/system.h"

/// Pulls a thrown rope back into its coil at the first point, one node a frame,
/// clearing its flag; both posts take animation 3 first.
s32 Rope_Recoil(s32 index, s32 x, s32 y, s32 z) {
  struct RopeWork *work = ROPE_WORK;
  s32 zero = 0;
  struct RopeSlot *slot = &work->slots[index];
  struct RopeNode *node = slot->head;
  struct RopeNode **cursor;
  s32 i;
  s32 angle;
  s32 radius;
  s32 lastZ;
  s32 segments;
  s32 frame;
  s32 lastX;
  s32 lastY;
  struct RopeNode **nodes;

  if (slot->state == 0)
    return;
  slot->state = zero;
  segments = slot->segments;
  cursor = nodes = Heap_Alloc(0x200);
  slot->swinging = zero;
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
  Actor_SelectAnimationFar(work->recoilFrom, 3);
  Actor_SelectAnimationFar(work->recoilTo, 3);
  wait(4);
  for (frame = 0; frame <= segments; frame++) {
    angle = frame << 12;
    radius = 0x40000;
    for (i = 0; i <= segments; i++) {
      node = nodes[i];
      node->slot = 0;
      if (i <= frame) {
        u16 step;

        node->x = x + Iwram_MulQ16(Trig_Cos(angle), radius);
        lastX = node->x;
        node->y = y;
        lastY = node->y;
        node->z = z + Iwram_MulQ16(Trig_Sin(angle), radius);
        lastZ = node->z;
        node->kind = 2;
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
        node->kind = 2;
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
  struct RopeWork *work = ROPE_WORK;
  struct RopeSlot *slot = work->slots;
  s32 i;

  for (i = 0; i <= 7; i++) {
    if (slot->head != NULL && slot->first == id) {
      *value = slot->second;
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
  struct RopeWork *work = ROPE_WORK;
  struct RopeSlot *slot;

  if (work == NULL)
    return -1;
  slot = &work->slots[index];
  return slot->state;
}

/// Gives rope index its flag and places it between posts a2 and a3: tied when
/// the flag is set, else coiled. A scene places each of its ropes with it.
void Rope_Place(s32 index, s32 flag, s32 a2, s32 a3) {
  struct RopeWork *work = ROPE_WORK;
  struct RopeSlot *slot;

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
  struct TightropeWork *work =
      HeapSlot_AllocEwram(160, sizeof(struct TightropeWork));
  void *buffer = Heap_Alloc(0x900);

  Dma_Fill32(0, work, sizeof(struct TightropeWork));
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

void Tightrope_Draw(void) {
  struct TightropeWork *work = TIGHTROPE_WORK;
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
  struct AffineTransform transform;

  for (i = 0; i < 8; i++) {
    u32 angle = work->slots[i].direction;
    struct TightropeNode *node = work->slots[i].head;
    s32 affine;
    s32 matrix;

    if (angle != 0) {
      transform.angle = angle;
      transform.scaleX = 0x100;
      transform.scaleY = 0x100;
      affine = 1;
      matrix = OamMatrix_AllocFromTransform(&transform);
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
          Oam_AddSpriteAtDepthClamped(&node->sprite, key);
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
  struct TightropeWork *work = TIGHTROPE_WORK;
  struct TightropeSlot *slot;
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
    if (slot->wobbling != 0) {
      s32 wobble = Iwram_MulQ16(Trig_Sin(slot->phase), slot->amplitude);

      slot->phase += 0x1000;
      slot->amplitude = Iwram_MulQ16(slot->amplitude, 0xf333);
      if (slot->amplitude <= 0x1998) {
        slot->amplitude = 0;
        slot->wobbling = 0;
      }
      Tightrope_WobbleBetween(i, slot->first, slot->second, wobble);
    }
  }
}

/// Returns the first unused of the tightropes' 128 nodes, or NULL.
struct TightropeNode *Tightrope_AllocNode(void) {
  struct TightropeWork *work = TIGHTROPE_WORK;
  struct TightropeNode *node = work->nodes;
  s32 index;

  for (index = 0; index <= 127; index++) {
    if (node->kind == 0)
      return node;
    node++;
  }
  return NULL;
}

/// Lays tightrope slotNo straight between posts firstId and secondId, and
/// shows both posts with it (animation 4).
void Tightrope_Place(s32 slotNo, s32 firstId, s32 secondId) {
  struct TightropeWork *work = TIGHTROPE_WORK;
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    struct TightropeSlot *slot = &work->slots[slotNo];

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
  struct TightropeWork *work = TIGHTROPE_WORK;
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
  struct TightropeNode **link = &work->slots[index].head;
  s32 i;

  work->slots[index].direction = (u16)ArcTan2(sz, sx);
  for (i = 0; i <= segments; i++) {
    struct TightropeNode *node = Tightrope_AllocNode();

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
  work->slots[index].wobbling = 1;
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
  struct TightropeWork *work = TIGHTROPE_WORK;
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
  struct TightropeNode *node = work->slots[index].head;
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
  struct TightropeWork *work = TIGHTROPE_WORK;
  struct Actor *a = ObjectTable_Get(from);
  struct Actor *b = ObjectTable_Get(to);

  if (a != NULL && b != NULL) {
    struct TightropeSlot *slot = &work->slots[index];

    slot->first = from;
    slot->second = to;
    Tightrope_LayCoil(index, a->x, a->y + 0x80000, a->z, b->x, b->y + 0x80000,
                      b->z);
  }
}

/// Coils tightrope index around the first point, one node for each 6 pixels of
/// the distance to the second, every node coiled (kind 2). Only the unused
/// Tightrope_Coil calls it, and it takes its nodes from the ropes' pool.
s32 Tightrope_LayCoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY,
                      s32 toZ) {
  struct TightropeWork *work = TIGHTROPE_WORK;
  s32 result = 0;
  s32 dx = (toX - x) >> 8;
  s32 dy = (toY - y) >> 8;
  s32 dz = (toZ - z) >> 8;
  s32 length = Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                               Iwram_MulQ16(dz, dz));
  s32 segments = length / 6 + 1;
  struct TightropeNode **link = &work->slots[index].head;
  s32 angle = 0;
  s32 radius = 0x40000;
  s32 i;
  s32 kind = 2; // One constant for its whole life.

  for (i = 0; i <= segments; i++) {
    struct TightropeNode *node = (struct TightropeNode *)Rope_AllocNode();
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

/// Unused: nothing refers to it. Rope_Lash for a tightrope, without the
/// short rope's fall.
void Tightrope_Lash(s32 arg, s32 firstId, s32 secondId) {
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    Tightrope_Throw(arg, first->x, first->y + 0x80000, first->z, second->x,
                    second->y + 0x80000, second->z,
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
  struct TightropeWork *work = TIGHTROPE_WORK;
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
  struct TightropeNode *node = work->slots[index].head;
  s32 sine;
  s32 height;
  s32 cosine;
  s32 lastX, lastY, lastZ;
  struct TightropeNode **nodes;
  struct TightropeNode **cursor;
  struct TightropeSlot *slot;
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
      node->slot = index + 1;
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
        node->kind = 1;
      }
    }
    wait(1);
  }
  slot = &work->slots[index];
  slot->wobbling = 1;
  slot->amplitude = 0x40000;
  slot->phase = 0x4000;
  Heap_Release(nodes);
  return 0;
}

extern s32 gTightrope_UntiePhase;

/// Unused: nothing refers to it. Rope_Untie for a tightrope.
void Tightrope_Untie(s32 arg, s32 firstId, s32 secondId) {
  struct TightropeWork *work;
  struct Actor *first;
  struct Actor *second;

  work = TIGHTROPE_WORK;
  first = ObjectTable_Get(firstId);
  second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    work->recoilFrom = first;
    work->recoilTo = second;
    Tightrope_Recoil(arg, first->x, first->y + 0x80000, first->z, second->x,
                     second->y + 0x80000, second->z,
                     Trig_Sin(gTightrope_UntiePhase) << 2);
    gTightrope_UntiePhase += 0x1000;
  }
}

/// Rope_Recoil for a tightrope.
s32 Tightrope_Recoil(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                     s32 wave) {
  struct TightropeWork *work = TIGHTROPE_WORK;
  s32 wobbling = 0;
  struct TightropeNode *node;
  s32 dx, dy, dz;
  s32 dist;
  s32 count;
  s32 frame;
  s32 i;
  s32 angle;
  s32 x, y, z;
  struct TightropeNode **nodes;
  struct TightropeNode **entry;

  dx = (x2 - x1) >> 8;
  dy = (y2 - y1) >> 8;
  dz = (z2 - z1) >> 8;
  dist = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);
  count = Iwram_Sqrt(dist) / 6 + 1;
  node = work->slots[slot].head;
  entry = nodes = Heap_Alloc(0x200);
  work->slots[slot].wobbling = wobbling;
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
  Actor_SelectAnimationFar(work->recoilFrom, 3);
  Actor_SelectAnimationFar(work->recoilTo, 3);
  wait(4);
  for (frame = 0; frame <= count; frame++) {
    angle = frame << 12;
    dist = 0x40000;
    for (i = 0; i <= count; i++) {
      node = nodes[i];
      node->slot = 0;
      if (i <= frame) {
        x = node->x = x1 + Iwram_MulQ16(Trig_Cos(angle), dist);
        y = node->y = y1;
        z = node->z = z1 + Iwram_MulQ16(Trig_Sin(angle), dist);
        node->kind = 2;
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
        node->kind = 2;
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
  struct TightropeWork *work = TIGHTROPE_WORK;
  struct TightropeNode *node;

  for (node = work->slots[index].head; node != NULL; node = node->next)
    node->kind = 0;
  work->slots[index].head = NULL;
}

/// Stops tightrope index's wobble.
void Tightrope_Settle(s32 index) {
  struct TightropeWork *work = TIGHTROPE_WORK;
  struct TightropeNode *node;

  for (node = work->slots[index].head; node != NULL; node = node->next)
    node->kind = 2;
  work->slots[index].wobbling = 0;
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

/// Starts the hanging vines: takes and clears their work (heap slot 164),
/// decodes four 256-byte tile pictures into a scratch buffer (one set for kind
/// 372, another for the rest; the second set repeats one picture), loads them
/// into a VRAM slot and schedules the two per-frame routines HangingVine_Draw
/// and HangingVine_Update.
struct WorkA4 *HangingVine_Initialize(s32 kind) {
  struct WorkA4 *work;
  u8 *pictures;
  s32 size;

  work = HeapSlot_AllocEwram(TILE_EFFECT_SLOT, sizeof(struct WorkA4));
  size = 4 * PICTURE_SIZE;
  pictures = Heap_Alloc(size);
  Dma_Fill32(0, work, sizeof(struct WorkA4));
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

/// The vines' first per-frame task: draws each link turned along its vine.
void HangingVine_Draw(void) {
  struct WorkA4 *work = WORK_A4;
  s32 i;
  u32 angle;
  s32 prevX = 0;
  s32 prevY = 0;
  u32 tiles = gResourceTableEntries[work->vramEntry].offset >> 5;
  s32 prevZ = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 height = map->origin[1];
  struct AffineTransform transform;
  struct HangingVineLink *link;

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
            link->sprite.affineIndex = OamMatrix_AllocFromTransform(&transform);
          }
          link->angle = angle;
          Oam_AddSpriteAtDepthClamped(sprite, priority);
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

struct Actor *Actor_GetById(u32 objectId);

/// The vines' second per-frame task: sways each active vine's links below
/// its anchor.
void HangingVine_Update(void) {
  struct WorkA4 *work = WORK_A4;
  struct WorkA4Slot *slot;
  struct Actor *leader;
  s32 x, y, z;
  struct HangingVineLink *link;
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

/// Returns the first unused of the vines' 128 links, or NULL.
struct HangingVineLink *HangingVine_AllocLink(void) {
  struct WorkA4 *work = WORK_A4;
  struct HangingVineLink *link = work->links;
  s32 index;

  for (index = 0; index <= 127; index++) {
    if (link->kind == 0)
      return link;
    link++;
  }
  return NULL;
}
