/// @file
/// Ropes, tightropes and hanging vines: chains of nodes laid between two
/// points or round one, drawn and updated each frame, and the leader crossing
/// them.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "gamepak/far_call/shop.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/object.h"
#include "system/system.h"
#include "field/event.h"
#include "macros.h"
#include "window/item_icon.h"

/// The animations of a rope's or tightrope's post (SPRITE_TIGHTROPE_POST).
enum PostAnimation {
  ANIM_POST_TIE = 2,   ///< the rope winding round the post, then ANIM_POST_TIED
  ANIM_POST_UNTIE = 3, ///< the rope unwinding, then the bare post
  ANIM_POST_TIED = 4   ///< the rope wound round the post
};

/// A sprite's depth slot is its point's y + z in pixels plus a bias: an actor's
/// sprite adds 62 and its shadow 60 (Render_ApplyProjectedPlacement), a rope's
/// and a tightrope's sprites this, a hanging vine's links HANGING_VINE_DEPTH.
#define ROPE_DEPTH 58

// The ropes Lash ties (HEAP_SLOT_ROPE), as in the Shrine of the Sea God: eight
// slots, each a chain of nodes between two posts, coiled round the foot of the
// first post until tied; tied, the rope runs from post to post and is wound
// round both, and can be crossed hand over hand (LEADER_MOVEMENT_ROPE). Pushing
// at a rope's post for more than ROPE_PUSH_STEPS frames raises
// FIELD_RAISED_TRIGGER_ROPE_ON, whose scene event puts the leader on the rope;
// on it, it raises FIELD_RAISED_TRIGGER_ROPE_OFF. Actor_CrossRope, which
// gActor_RopeScript runs once a frame, adds half a pixel to the rope's swing,
// up to two, on each frame it sends the leader on toward a node, so the swing
// is full within a few frames of setting off. Rope_Update and Rope_LayBowed bow
// the middle of the rope that far either way of its line, in a cycle of 16
// frames, the swing losing a twentieth a frame.

/// The leader steps onto a rope or tightrope once it has pushed against its
/// post (FieldState.blockedSteps) for more than this many frames.
#define ROPE_PUSH_STEPS 12

/// A post whose sceneByte byte holds this keeps in watchedActor a hook giving
/// the point Lash throws the rope to (Rope_Lash); no scene marks a post so.
#define ROPE_POST_HOOKED 99

/// The posts the unused debug hooks (Rope_DebugSwing, Tightrope_DebugSwing)
/// swing rope 0 between: the fifth and sixth actors a scene places.
#define DEBUG_FIRST_POST (ACTOR_FIRST_PLACED + 4)
#define DEBUG_SECOND_POST (ACTOR_FIRST_PLACED + 5)

/// Allocates the ropes' state, loads their node picture and starts Rope_Draw
/// and Rope_Update. A scene with ropes to lash calls it first.
void Rope_Initialize(void) {
  struct RopeState *state =
      HeapSlot_AllocEwram(HEAP_SLOT_ROPE, sizeof(struct RopeState));
  void *buffer = Heap_Alloc(256);

  Dma_Fill32(0, state, sizeof(struct RopeState));
  state->vramEntry = VramCache_FindFreeEntry();
  Decompress_TaggedLz(gRope_Tiles, buffer);
  VramCache_Load(state->vramEntry, 256, buffer);
  Task_Add(Rope_Draw, TASK_PASS_FRAME);
  Task_Add(Rope_Update, TASK_PASS_FRAME + 15);
  Heap_Release(buffer);
}

/// The ropes' drawing task: a sprite at the midpoint of each pair of
/// neighbouring nodes, 4 pixels up, turned along the pair and given its rope's
/// priority, for the pairs on screen; a rope's first node keeps the last turn.
void Rope_Draw(void) {
  struct RopeState *state = ROPE_STATE;
  struct RopeSlot *slot = state->slots;
  s32 i;
  s32 prevX = 0;
  s32 prevY = 0;
  u32 tile = gVramCacheEntries[state->vramEntry].offset / TILE_SIZE_4BPP;
  s32 prevZ = 0;
  s32 started = 0;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  s32 viewX = MapState_View(map)[0] & FIELD_PIXEL_MASK;
  s32 viewZ = MapState_View(map)[1] & FIELD_PIXEL_MASK;
  s32 originY = map->origin[1];
  struct AffineTransform transform;
  struct RopeNode *node;
  u32 angle;

  for (i = 0; i < ROPE_SLOT_COUNT; i++, slot++) {
    for (node = slot->head; node != NULL; node = node->next) {
      s32 priority = slot->priority;
      FIXED y = node->y;
      s32 x = node->x;
      s32 z = node->z;

      y -= toFIXED(4);

      if (started) {
        s32 screenX;
        s32 depth;
        s32 screenY;

        angle = ((u16)Trig_Atan(prevX - x, (z - prevZ) + (prevY - y)) +
                 DEGtoANG(90)) &
                TRIG_ANGLE_MASK;
        screenX = (prevX + x) / 2 - viewX;
        prevY = (prevY + y) / 2 - originY;
        depth = (prevZ + z) / 2 - viewZ - originY;
        screenY = depth - prevY;
        if (screenX > -toFIXED(32) && screenX < toFIXED(DISPLAY_WIDTH + 32) &&
            screenY > -toFIXED(32) && screenY < toFIXED(DISPLAY_HEIGHT + 64)) {
          s32 spriteX = (FIXEDtoINT(screenX) - 4) & OBJ_X_MASK;
          s32 spriteY = (FIXEDtoINT(screenY) - 4) & OBJ_Y_MASK;
          struct AnimationSpritePart *sprite;

          prevY = FIXEDtoINT(prevY + depth) + ROPE_DEPTH;
          sprite = &node->sprite;

          ((u32 *)sprite)[0] = 0;
          ((u32 *)&node->sprite)[1] = spriteY | OBJ_X(spriteX) | OBJ_256_COLORS;
          ((u32 *)&node->sprite)[2] = OBJ_PRIORITY(2) | tile;
          if (angle != 0) {
            transform.angle = angle;
            transform.scaleX = AFFINE_SCALE_ONE;
            transform.scaleY = AFFINE_SCALE_ONE;
            node->sprite.affine = 1;
            node->sprite.affineIndex = OamMatrix_AllocFromTransform(&transform);
          }
          node->sprite.priority = priority;
          node->angle = angle;
          Oam_AddSpriteAtDepthClamped(sprite, prevY);
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
    if (slot->head != NULL)
      slot->head->angle = angle;
  }
}

/// The ropes' task: unless the field is handling a request, raises
/// FIELD_RAISED_TRIGGER_ROPE_ON when the leader has pushed at one end of a tied
/// rope, or FIELD_RAISED_TRIGGER_ROPE_OFF while on one; then swings each
/// swinging rope, its swing losing a twentieth a frame until under a tenth of a
/// pixel.
void Rope_Update(void) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct RopeState *state = ROPE_STATE;
  struct RopeSlot *slot;
  s32 i = 0;

  if (!GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) &&
      event->blockedSteps > ROPE_PUSH_STEPS) {
    if (gMainRoutineState.movementMode == LEADER_MOVEMENT_WALK) {
      s32 target = Field_FindNearbyTalkTarget(gMainRoutineState.leaderActorId);

      if (target != -1) {
        for (i = 0; i < ROPE_SLOT_COUNT; i++)
          if (state->slots[i].first == target ||
              state->slots[i].second == target)
            break;
        if (i < ROPE_SLOT_COUNT &&
            Rope_IsLeaderAtEnd(state->slots[i].first, state->slots[i].second))
          event->raisedTrigger = FIELD_RAISED_TRIGGER_ROPE_ON;
      }
    } else if (gMainRoutineState.movementMode == LEADER_MOVEMENT_ROPE) {
      event->raisedTrigger = FIELD_RAISED_TRIGGER_ROPE_OFF;
    }
  }
  slot = state->slots;
  for (i = 0; i < ROPE_SLOT_COUNT; i++, slot++) {
    if (slot->swinging != 0) {
      FIXED swing = Iwram_MulFX(Trig_Sin(slot->phase), slot->amplitude);

      slot->phase += TRIG_TURN / 16;
      slot->amplitude = Iwram_MulFX(slot->amplitude, FIXED_ONE * 19 / 20);
      if (slot->amplitude < FIXED_ONE / 10) {
        slot->amplitude = 0;
        slot->swinging = 0;
      }
      Rope_SwingBetween(i, slot->first, slot->second, swing);
    }
  }
}

/// Returns the first unused of the ropes' 128 nodes, or NULL.
struct RopeNode *Rope_AllocNode(void) {
  struct RopeState *state = ROPE_STATE;
  struct RopeNode *node = state->nodes;
  s32 index;

  for (index = 0; index < ROPE_NODE_COUNT; index++) {
    if (node->kind == ROPE_NODE_FREE)
      return node;
    node++;
  }
  return NULL;
}

/// Lays rope index tied between posts id and other, and shows both posts with
/// the rope wound round them.
void Rope_Tie(s32 index, s32 id, s32 other) {
  struct RopeState *state = ROPE_STATE;
  s32 priority = 2;
  struct Actor *first = Actor_Lookup(id);
  struct Actor *second = Actor_Lookup(other);

  if (first != NULL && second != NULL) {
    struct RopeSlot *slot;

    if (first->spriteStorageKind == ACTOR_SPRITE_ONE)
      priority = ((struct Sprite *)first->sprite)->image.priority;
    slot = &state->slots[index];
    slot->first = id;
    slot->second = other;
    slot->priority = priority;
    Rope_LayTied(index, first->x.fixed, first->y.fixed + toFIXED(8),
                 first->z.fixed, second->x.fixed, second->y.fixed + toFIXED(8),
                 second->z.fixed);
  }
  Actor_SelectAnimationFar(first, ANIM_POST_TIE);
  Actor_SelectAnimationFar(second, ANIM_POST_TIE);
}

/// Builds rope index's nodes on the line between two points, one for each 6
/// pixels, waved a pixel either way by one whole sine; marks it tied and
/// swinging and sets its flag.
s32 Rope_LayTied(s32 index, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2) {
  struct RopeState *state = ROPE_STATE;
  s32 result = 0;
  s32 dx = x2 - x1;
  s32 sx = dx >> 8;
  s32 dy = y2 - y1;
  s32 sy = dy >> 8;
  s32 dz = z2 - z1;
  s32 sz = dz >> 8;
  s32 count;
  struct RopeSlot *slot;
  struct RopeNode **link;
  struct RopeNode *node;
  u16 angle;
  FIXED sin;
  FIXED cos;
  s32 i;

  count = Iwram_Sqrt(Iwram_MulFX(sx, sx) + Iwram_MulFX(sy, sy) +
                     Iwram_MulFX(sz, sz)) /
              6 +
          1;
  slot = &state->slots[index];
  link = &slot->head;
  angle = Trig_Atan(sx, sy);
  sin = Trig_Sin(angle);
  cos = Trig_Cos(angle);
  slot->segments = count;
  for (i = 0; i <= count; i++) {
    FIXED bow;

    node = Rope_AllocNode();
    if (node == NULL) {
      result = -1;
      break;
    }
    bow = Iwram_MulFX(Trig_Sin(i * TRIG_TURN / count), toFIXED(1));
    SET_POSITION(node, x1 + i * dx / count + Iwram_MulFX(bow, cos),
                 y1 + i * dy / count + Iwram_MulFX(bow, sin),
                 z1 + i * dz / count);
    node->slot = index + 1;
    if (i == count)
      node->kind = ROPE_NODE_UNCROSSABLE;
    else
      node->kind = ROPE_NODE_CROSSABLE;
    *link = node;
    link = &node->next;
  }
  *link = NULL;
  slot->state = ROPE_THROWN;
  slot->swinging = 1;
  slot->amplitude = FIXED_ONE;
  slot->phase = DEGtoANG(90);
  if (slot->flag != 0)
    GameFlag_Set(slot->flag);
  return result;
}

/// Lays rope index again between posts from and to, its middle bowed height
/// pixels (16.16) aside.
void Rope_SwingBetween(s32 index, s32 from, s32 to, FIXED height) {
  struct Actor *a = Actor_Lookup(from);
  struct Actor *b = Actor_Lookup(to);

  if (a != NULL && b != NULL)
    Rope_LayBowed(index, a->x.fixed, a->y.fixed + toFIXED(8), a->z.fixed,
                  b->x.fixed, b->y.fixed + toFIXED(8), b->z.fixed, height);
}

/* VIEW(ropes): routine Trig_Atan as returning s16; system/system.h has it
 * return s32. */
/* FAKEMATCH(asm): the second return type reaches Trig_Atan through an asm
 * label. */
extern s16 Trig_AtanHalfword(s32 x, s32 y) __asm__("Trig_Atan");

/// Lays a rope's nodes along the line between two points again, bowed by half a
/// sine height pixels (16.16) high.
s32 Rope_LayBowed(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                  FIXED height) {
  struct RopeState *state = ROPE_STATE;
  s32 dx = toX - x;
  s32 ax = dx >> 8;
  s32 dy = toY - y;
  struct RopeSlot *slot = &state->slots[index];
  struct RopeNode *node = slot->head;
  u16 angle = Trig_AtanHalfword(ax, dy >> 8);
  FIXED sin = Trig_Sin(angle);
  FIXED cos = Trig_Cos(angle);
  s32 segments = slot->segments;
  s32 i;
  s32 kind = ROPE_NODE_CROSSABLE;

  for (i = 0; i <= segments; i++) {
    FIXED bow = Iwram_MulFX(Trig_Sin(i * (TRIG_TURN / 2) / segments), height);

    SET_POSITION(node, x + i * dx / segments + Iwram_MulFX(bow, cos),
                 y + i * dy / segments + Iwram_MulFX(bow, sin),
                 z + i * (toZ - z) / segments);
    node->kind = kind;
    node = node->next;
  }
  return 0;
}

/// Coils rope index at post firstId, its length reaching post secondId: the
/// rope as it waits to be lashed.
void Rope_Coil(s32 index, s32 firstId, s32 secondId) {
  struct RopeState *state = ROPE_STATE;
  s32 priority = 2;
  struct Actor *first = Actor_Lookup(firstId);
  struct Actor *second = Actor_Lookup(secondId);

  if (first != NULL && second != NULL) {
    struct RopeSlot *slot;

    if (first->spriteStorageKind == ACTOR_SPRITE_ONE)
      priority = ((struct Sprite *)first->sprite)->image.priority;
    slot = &state->slots[index];
    slot->first = firstId;
    slot->second = secondId;
    slot->priority = priority;
    Rope_LayCoil(index, first->x.fixed, first->y.fixed + toFIXED(8),
                 first->z.fixed, second->x.fixed, second->y.fixed + toFIXED(8),
                 second->z.fixed);
  }
}

/// Builds rope index's nodes, one for each 6 pixels between two points, as a
/// flat spiral around the first point: the rope coiled at its post.
s32 Rope_LayCoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct RopeState *state = ROPE_STATE;
  s32 result = 0;
  s32 dx = (toX - x) >> 8;
  s32 dy = (toY - y) >> 8;
  s32 dz = (toZ - z) >> 8;
  s32 length = Iwram_SqrtEntry(Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy) +
                               Iwram_MulFX(dz, dz));
  s32 segments = length / 6 + 1;
  struct RopeSlot *slot = &state->slots[index];
  struct RopeNode **link = &slot->head;
  s32 angle = 0;
  FIXED radius = toFIXED(4);
  s32 i;
  s32 kind = ROPE_NODE_UNCROSSABLE;

  slot->segments = segments;
  for (i = 0; i <= segments; i++) {
    struct RopeNode *node = Rope_AllocNode();
    u16 step;

    if (node == NULL) {
      result = -1;
      break;
    }
    SET_POSITION(node, x + Iwram_MulFX(Trig_Cos(angle), radius), y,
                 z + Iwram_MulFX(Trig_Sin(angle), radius));
    node->kind = kind;
    *link = node;
    step = Trig_Atan(toFIXED(6), radius);
    angle += step;
    link = &node->next;
    radius += step * 4;
  }
  *link = NULL;
  return result;
}

/// Writes where rope index's last node is, its far end, into position.
void Rope_GetEndNodePosition(s32 index, s32 *position) {
  struct RopeState *state = ROPE_STATE;
  struct RopeSlot *slot = &state->slots[index];
  struct RopeNode *node = slot->head;
  s32 remaining;

  remaining = slot->segments;
  if (remaining > 0) {
    do {
      remaining--;
      node = node->next;
    } while (remaining != 0);
  }
  SET_VECTOR(position, node->x, node->y, node->z);
}

extern s32 gRope_LashPhase;

s32 Rope_Throw(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
               s32 wave);
s32 Rope_Recoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                s32 wave);

/// Lash: throws rope index from post fromId to post toId and winds it round
/// both. A rope coiled too short to reach falls back to its post.
void Rope_Lash(s32 index, s32 fromId, s32 toId) {
  struct RopeState *state = ROPE_STATE;
  struct Actor *from = Actor_Lookup(fromId);
  struct Actor *to = Actor_Lookup(toId);

  if (from != NULL && to != NULL) {
    FIXED toX = to->x.fixed;
    FIXED toY = to->y.fixed;
    FIXED toZ = to->z.fixed;
    struct RopeSlot *slot = &state->slots[index];
    s32 x, y = 0, z; // Default: y.
    s32 xx, yy, length, segments, count;
    s32 (*mul)(s32, s32);

    if (to->sceneByte == ROPE_POST_HOOKED)
      ((TargetHook)to->watchedActor)(to, &toX, &toZ);
    x = (toX - from->x.fixed) >> 8;
    y = (toY - from->y.fixed) >> 8;
    z = (toZ - from->z.fixed) >> 8;
    mul = Iwram_MulFX;
    xx = mul(x, x);
    yy = mul(y, y);
    length = xx + yy + mul(z, z);
    segments = Iwram_SqrtEntry(length) / 6 + 1;
    count = slot->segments;
    if (segments > count) {
      count++;
      x = from->x.fixed + (toX - from->x.fixed) * count / segments;
      y = toY;
      z = from->z.fixed + (toZ - from->z.fixed) * count / segments;

      Rope_Throw(index, from->x.fixed, from->y.fixed + toFIXED(8),
                 from->z.fixed, x, y + toFIXED(8), z,
                 Trig_Sin(gRope_LashPhase) << 2);
      slot->swinging = 0;
      wait(1);
      Rope_Recoil(index, from->x.fixed, from->y.fixed + toFIXED(8),
                  from->z.fixed, x, y + toFIXED(8), z,
                  Trig_Sin(gRope_LashPhase) << 2);
    } else {
      Rope_Throw(index, from->x.fixed, from->y.fixed + toFIXED(8),
                 from->z.fixed, to->x.fixed, to->y.fixed + toFIXED(8),
                 to->z.fixed, Trig_Sin(gRope_LashPhase) << 2);
      gRope_LashPhase += TRIG_TURN / 16;
      wait(6);
      Actor_SelectAnimationFar(from, ANIM_POST_TIE);
      Actor_SelectAnimationFar(to, ANIM_POST_TIE);
    }
  }
}

/// Throws rope index from one point to another over twice its node count in
/// frames: the end flies on an arc and each node follows the one ahead at 6
/// pixels; then the rope swings and its flag is set. Does nothing to a rope
/// already thrown. The callers pass the wobble Tightrope_Recoil takes, which a
/// rope leaves unused, and read no result: 0, or nothing for a rope already
/// thrown.
s32 Rope_Throw(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
               s32 wave) {
  struct RopeState *state = ROPE_STATE;
  s32 dx = toX - x;
  s32 ex = dx >> 8;
  s32 dy = toY - y;
  s32 ey = dy >> 8;
  s32 dz = toZ - z;
  s32 segments;
  s32 frame;
  FIXED sine;
  FIXED cosine;
  s32 lastX;
  s32 lastY;
  s32 lastZ;
  struct RopeSlot *slot = &state->slots[index];
  struct RopeNode *node = slot->head;
  void **cursor;
  s32 i;
  u16 angle;
  struct RopeNode **nodes;

  if (slot->state == ROPE_THROWN)
    return;
  slot->state = ROPE_THROWN;
  segments = slot->segments;
  cursor = nodes = Heap_Alloc(512);
  for (i = 0; i <= segments; i++) {
    *cursor++ = node;
    node = node->next;
  }
  angle = Trig_Atan(ex, ey);
  sine = Trig_Sin(angle);
  cosine = Trig_Cos(angle);
  for (frame = 1; frame <= segments * 2; frame++) {
    for (i = segments; i >= 0; i--) {
      node = nodes[i];
      node->slot = index + 1;
      if (i == segments) {
        ex = Iwram_MulFX(Trig_Sin(frame * (TRIG_TURN / 4) / i), toFIXED(8));

        node->x = x + dx * frame / 2 / i + Iwram_MulFX(ex, cosine);
        lastX = node->x;
        node->y = y + dy * frame / 2 / i + Iwram_MulFX(ex, sine);
        lastY = node->y;
        node->z = z + dz * frame / 2 / i;
        lastZ = node->z;
      } else {
        s32 ez;

        ex = (lastX - node->x) >> 8;
        ey = (lastY - node->y) >> 8;
        ez = (lastZ - node->z) >> 8;
        ex = Iwram_SqrtEntry(Iwram_MulFX(ex, ex) + Iwram_MulFX(ey, ey) +
                             Iwram_MulFX(ez, ez));

        if (ex > 6) {
          node->x = lastX + (node->x - lastX) * 6 / ex;
          lastX = node->x;
          node->y = lastY + (node->y - lastY) * 6 / ex;
          lastY = node->y;
          node->z = lastZ + (node->z - lastZ) * 6 / ex;
          node->kind = ROPE_NODE_CROSSABLE;
          lastZ = node->z;
        }
      }
    }
    wait(1);
  }
  slot = &state->slots[index];
  slot->swinging = 1;
  slot->amplitude = toFIXED(4);
  slot->phase = DEGtoANG(90);
  if (slot->flag != 0)
    GameFlag_Set(slot->flag);
  Heap_Release(nodes);
  return 0;
}

extern s32 gRope_UntiePhase;

/// Unties rope index between posts fromId and toId (Rope_Recoil).
void Rope_Untie(s32 index, s32 fromId, s32 toId) {
  struct RopeState *state;
  struct Actor *from;
  struct Actor *to;

  state = ROPE_STATE;
  from = Actor_Lookup(fromId);
  to = Actor_Lookup(toId);

  if (from != NULL && to != NULL) {
    state->recoilFrom = from;
    state->recoilTo = to;
    Rope_Recoil(index, from->x.fixed, from->y.fixed + toFIXED(8), from->z.fixed,
                to->x.fixed, to->y.fixed + toFIXED(8), to->z.fixed,
                Trig_Sin(gRope_UntiePhase) * 4);
    gRope_UntiePhase += TRIG_TURN / 16;
  }
}

/// Pulls a thrown rope back into its coil at the first point, one node a frame,
/// clearing its flag, starting four frames after both posts start to unwind.
/// The far end and the wobble are Tightrope_Recoil's arguments, which a rope
/// leaves unused; as with Rope_Throw, no caller reads the result.
s32 Rope_Recoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                s32 wave) {
  struct RopeState *state = ROPE_STATE;
  // Default: each count below sets it again.
  s32 i = 0;
  struct RopeSlot *slot = &state->slots[index];
  struct RopeNode *node = slot->head;
  void **cursor;
  s32 angle;
  s32 radius;
  s32 lastZ;
  s32 segments;
  s32 frame;
  s32 lastX;
  s32 lastY;
  void **nodes;

  if (slot->state == ROPE_COILED)
    return;
  slot->state = ROPE_COILED;
  segments = slot->segments;
  cursor = nodes = Heap_Alloc(512);
  slot->swinging = 0;
  if (slot->flag != 0)
    GameFlag_Clear(slot->flag);
  if (segments >= 0) {
    i = segments + 1;
    do {
      i += -1;
      *cursor++ = node;
      node = node->next;
    } while (i != 0);
  }
  Actor_SelectAnimationFar(state->recoilFrom, ANIM_POST_UNTIE);
  Actor_SelectAnimationFar(state->recoilTo, ANIM_POST_UNTIE);
  wait(4);
  for (frame = 0; frame <= segments; frame++) {
    angle = frame * (TRIG_TURN / 16);
    radius = toFIXED(4);
    for (i = 0; i <= segments; i++) {
      node = nodes[i];
      node->slot = 0;
      if (i <= frame) {
        u16 step;

        node->x = x + Iwram_MulFX(Trig_Cos(angle), radius);
        lastX = node->x;
        node->y = y;
        lastY = node->y;
        node->z = z + Iwram_MulFX(Trig_Sin(angle), radius);
        lastZ = node->z;
        node->kind = ROPE_NODE_UNCROSSABLE;
        step = Trig_Atan(toFIXED(6), radius);
        angle += step;
        radius += step * 4;
      } else {
        s32 dx = (lastX - node->x) >> 8;
        s32 dy = (lastY - node->y) >> 8;
        s32 dz = (lastZ - node->z) >> 8;

        radius = Iwram_SqrtEntry(Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy) +
                                 Iwram_MulFX(dz, dz));

        if (radius <= 6)
          break;
        node->x = lastX + (node->x - lastX) * 6 / radius;
        lastX = node->x;
        node->y = lastY + (node->y - lastY) * 6 / radius;
        lastY = node->y;
        node->z = lastZ + (node->z - lastZ) * 6 / radius;
        node->kind = ROPE_NODE_UNCROSSABLE;
        lastZ = node->z;
      }
    }
    wait(1);
  }
  Heap_Release(nodes);
  return 0;
}

/// Unused: nothing refers to it. Does nothing.
UNUSED void Rope_ReservedNoOp(void) {}

extern s32 gRope_DebugSwinging;

/// Unused: nothing refers to it. An actor hook that, once L is pressed, lays
/// rope 0 again between DEBUG_FIRST_POST and DEBUG_SECOND_POST every frame.
UNUSED void Rope_DebugSwing(struct Actor *actor) {
  struct Sprite *sprite = actor->sprite;

  sprite->offsetX = -3;
  sprite->offsetY = 12;
  if (gInput.repeat & KEY_L)
    gRope_DebugSwinging = 1;
  if (gRope_DebugSwinging != 0)
    Rope_SwingBetween(0, DEBUG_FIRST_POST, DEBUG_SECOND_POST, 0);
}

/// Returns 1 when the leader stands on the cell of post firstId or secondId, or
/// a cell beside it along an axis, at the same height.
s32 Rope_IsLeaderAtEnd(s32 firstId, s32 secondId) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct Actor *first = Actor_GetById(firstId);
  struct Actor *second = Actor_GetById(secondId);
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 y = FIELD_CELL(leader->y.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);
  s32 firstX = FIELD_CELL(first->x.fixed);
  s32 firstY = FIELD_CELL(first->y.fixed);
  s32 firstZ = FIELD_CELL(first->z.fixed);
  s32 secondX = FIELD_CELL(second->x.fixed);
  s32 secondY = FIELD_CELL(second->y.fixed);
  s32 secondZ = FIELD_CELL(second->z.fixed);

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
  struct RopeState *state = ROPE_STATE;
  struct RopeSlot *slot = state->slots;
  s32 i;

  for (i = 0; i < ROPE_SLOT_COUNT; i++) {
    if (slot->head != NULL && slot->first == id) {
      *value = slot->second;
      *index = i;
      return 0;
    }
    slot++;
  }
  return -1;
}

/// Returns rope index's enum RopeSlotState, or -1 while the ropes have no
/// state.
s32 Rope_GetState(s32 index) {
  struct RopeState *state = ROPE_STATE;
  struct RopeSlot *slot;

  if (state == NULL)
    return -1;
  slot = &state->slots[index];
  return slot->state;
}

/// Gives rope index its flag and places it between posts firstId and
/// secondId: tied when the flag is set, else coiled at the first. A scene
/// places each of its ropes with it.
void Rope_Place(s32 index, s32 flag, s32 firstId, s32 secondId) {
  struct RopeState *state = ROPE_STATE;
  struct RopeSlot *slot;

  slot = &state->slots[index];
  slot->flag = flag;
  if (!GameFlag_Get(flag))
    Rope_Coil(index, firstId, secondId);
  else
    Rope_Tie(index, firstId, secondId);
}

// The tightropes (HEAP_SLOT_TIGHTROPE), as in Champa: ropes stretched straight
// from post to post across a drop, that the party walks
// (LEADER_MOVEMENT_TIGHTROPE) and that wobble underfoot. The family copies the
// ropes' routines.

/// Allocates the tightropes' state, loads their node picture and starts
/// Tightrope_Draw and Tightrope_Update.
void Tightrope_Initialize(void) {
  struct TightropeState *state =
      HeapSlot_AllocEwram(HEAP_SLOT_TIGHTROPE, sizeof(struct TightropeState));
  void *buffer = Heap_Alloc(2304);

  Dma_Fill32(0, state, sizeof(struct TightropeState));
  state->vramEntry = VramCache_FindFreeEntry();
  Decompress_TaggedLz(gRope_Tiles, buffer);
  VramCache_Load(state->vramEntry, 64, buffer);
  Task_Add(Tightrope_Draw, TASK_PASS_FRAME);
  Task_Add(Tightrope_Update, TASK_PASS_FRAME + 15);
  Heap_Release(buffer);
}

/// The tightropes' drawing task: as Rope_Draw, a sprite at the midpoint of each
/// pair of neighbouring nodes, 4 pixels up, for the pairs on screen, all of a
/// tightrope turned along it.
void Tightrope_Draw(void) {
  struct TightropeState *state = TIGHTROPE_STATE;
  s32 i;
  s32 prevX = 0;
  FIXED prevY = 0;
  s32 prevZ = 0;
  s32 started = 0;
  u32 tile = gVramCacheEntries[state->vramEntry].offset / TILE_SIZE_4BPP;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  s32 viewX = MapState_View(map)[0] & FIELD_PIXEL_MASK;
  s32 viewZ = MapState_View(map)[1] & FIELD_PIXEL_MASK;
  s32 originY = map->origin[1];
  struct AffineTransform transform;

  for (i = 0; i < ROPE_SLOT_COUNT; i++) {
    u32 angle = state->slots[i].direction;
    struct TightropeNode *node = state->slots[i].head;
    s32 affine;
    s32 matrix;

    if (angle != 0) {
      transform.angle = angle;
      transform.scaleX = AFFINE_SCALE_ONE;
      transform.scaleY = AFFINE_SCALE_ONE;
      affine = 1;
      matrix = OamMatrix_AllocFromTransform(&transform);
    } else {
      affine = 0;
      matrix = 0;
    }
    for (; node != NULL; node = node->next) {
      FIXED y = node->y;
      s32 x = node->x;
      s32 z = node->z;

      y -= toFIXED(4);

      if (started) {
        s32 screenX = (prevX + x) / 2 - viewX;
        FIXED screenY;

        prevY = (prevY + y) / 2 - originY;
        prevZ = (prevZ + z) / 2 - viewZ - originY;
        screenY = prevZ - prevY;

        if (screenX > -toFIXED(32) && screenX < toFIXED(DISPLAY_WIDTH + 32) &&
            screenY > -toFIXED(32) && screenY < toFIXED(DISPLAY_HEIGHT + 64)) {
          s32 left = (FIXEDtoINT(screenX) - 4) & OBJ_X_MASK;
          s32 top = (FIXEDtoINT(screenY) - 4) & OBJ_Y_MASK;
          s32 key = FIXEDtoINT(prevY + prevZ) + ROPE_DEPTH;

          ((u32 *)&node->sprite)[0] = 0;
          ((u32 *)&node->sprite)[1] = top | OBJ_X(left) | OBJ_256_COLORS;
          ((u32 *)&node->sprite)[2] = tile | OBJ_PRIORITY(2);
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

/// The tightropes' task: Rope_Update for the tightropes, raising
/// FIELD_RAISED_TRIGGER_TIGHTROPE_ON and FIELD_RAISED_TRIGGER_TIGHTROPE_OFF;
/// the middle of a wobbling tightrope bobs up and down.
void Tightrope_Update(void) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct TightropeState *state = TIGHTROPE_STATE;
  struct TightropeSlot *slot;
  s32 i = 0;

  if (!GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) &&
      event->blockedSteps > ROPE_PUSH_STEPS) {
    struct MainRoutineState *party = &gMainRoutineState;

    if (party->movementMode == LEADER_MOVEMENT_WALK) {
      s32 target = Field_FindNearbyTalkTarget(party->leaderActorId);

      if (target != -1) {
        for (i = 0; i < ROPE_SLOT_COUNT; i++) {
          if (state->slots[i].first == target ||
              state->slots[i].second == target)
            break;
        }
        if (i < ROPE_SLOT_COUNT &&
            Rope_IsLeaderAtEnd(state->slots[i].first, state->slots[i].second))
          event->raisedTrigger = FIELD_RAISED_TRIGGER_TIGHTROPE_ON;
      }
    } else if (party->movementMode == LEADER_MOVEMENT_TIGHTROPE) {
      event->raisedTrigger = FIELD_RAISED_TRIGGER_TIGHTROPE_OFF;
    }
  }

  slot = state->slots;
  for (i = 0; i < ROPE_SLOT_COUNT; i++, slot++) {
    if (slot->wobbling != 0) {
      FIXED wobble = Iwram_MulFX(Trig_Sin(slot->phase), slot->amplitude);

      slot->phase += TRIG_TURN / 16;
      slot->amplitude = Iwram_MulFX(slot->amplitude, FIXED_ONE * 19 / 20);
      if (slot->amplitude < FIXED_ONE / 10) {
        slot->amplitude = 0;
        slot->wobbling = 0;
      }
      Tightrope_WobbleBetween(i, slot->first, slot->second, wobble);
    }
  }
}

/// Returns the first unused of the tightropes' 128 nodes, or NULL.
struct TightropeNode *Tightrope_AllocNode(void) {
  struct TightropeState *state = TIGHTROPE_STATE;
  struct TightropeNode *node = state->nodes;
  s32 index;

  for (index = 0; index < ROPE_NODE_COUNT; index++) {
    if (node->kind == ROPE_NODE_FREE)
      return node;
    node++;
  }
  return NULL;
}

/// Lays tightrope slotNo straight between posts firstId and secondId, and
/// shows the rope wound round both. Declared s32, as the scenes call it, though
/// it returns no value; no caller reads one.
s32 Tightrope_Place(s32 slotNo, s32 firstId, s32 secondId) {
  struct TightropeState *state = TIGHTROPE_STATE;
  struct Actor *first = Actor_Lookup(firstId);
  struct Actor *second = Actor_Lookup(secondId);

  if (first != NULL && second != NULL) {
    struct TightropeSlot *slot = &state->slots[slotNo];

    slot->first = firstId;
    slot->second = secondId;
    Tightrope_Lay(slotNo, first->x.fixed, first->y.fixed + toFIXED(8),
                  first->z.fixed, second->x.fixed, second->y.fixed + toFIXED(8),
                  second->z.fixed);
    Actor_SelectAnimationFar(first, ANIM_POST_TIED);
    Actor_SelectAnimationFar(second, ANIM_POST_TIED);
  }
}

/// Builds tightrope index's nodes on the straight line between two points,
/// one for each 6 pixels.
s32 Tightrope_Lay(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct TightropeState *state = TIGHTROPE_STATE;
  s32 result = 0;
  s32 dx = toX - x;
  s32 sx = dx >> 8;
  s32 dy = toY - y;
  s32 sy = dy >> 8;
  s32 dz = toZ - z;
  s32 sz = dz >> 8;
  s32 segments = Iwram_SqrtEntry(Iwram_MulFX(sx, sx) + Iwram_MulFX(sy, sy) +
                                 Iwram_MulFX(sz, sz)) /
                     6 +
                 1;
  struct TightropeNode **link = &state->slots[index].head;
  s32 i;

  state->slots[index].direction = (u16)Trig_Atan(sz, sx);
  for (i = 0; i <= segments; i++) {
    struct TightropeNode *node = Tightrope_AllocNode();

    if (node == NULL) {
      result = -1;
      break;
    }
    node->slot = index + 1;
    SET_POSITION(node, x + i * dx / segments, y + i * dy / segments,
                 z + i * dz / segments);
    if (i == segments)
      node->kind = ROPE_NODE_UNCROSSABLE;
    else
      node->kind = ROPE_NODE_CROSSABLE;
    *link = node;
    link = &node->next;
  }
  *link = NULL;
  state->slots[index].wobbling = 1;
  return result;
}

/// Lays tightrope index again between posts from and to, its middle raised
/// height pixels (16.16).
void Tightrope_WobbleBetween(s32 index, s32 from, s32 to, FIXED height) {
  struct Actor *a = Actor_Lookup(from);
  struct Actor *b = Actor_Lookup(to);

  if (a != NULL && b != NULL)
    Tightrope_LayBowed(index, a->x.fixed, a->y.fixed + toFIXED(8), a->z.fixed,
                       b->x.fixed, b->y.fixed + toFIXED(8), b->z.fixed, height);
}

FIXED IwramMulFX(FIXED a, FIXED b) __attribute__((long_call));

/// Rope_LayBowed for a tightrope: lays its nodes along the line between two
/// points again, raised by half a sine height pixels (16.16) high.
s32 Tightrope_LayBowed(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY,
                       s32 toZ, FIXED height) {
  struct TightropeState *state = TIGHTROPE_STATE;
  s32 dx = toX - x;
  s32 sx = dx >> 8;
  s32 dy = toY - y;
  s32 sy = dy >> 8;
  s32 dz = toZ - z;
  s32 sz = dz >> 8;
  s32 segments = Iwram_SqrtEntry(IwramMulFX(sx, sx) + IwramMulFX(sy, sy) +
                                 IwramMulFX(sz, sz)) /
                     6 +
                 1;
  struct TightropeNode *node = state->slots[index].head;
  s32 i;
  s32 kind = ROPE_NODE_CROSSABLE;

  for (i = 0; i <= segments; i++) {
    FIXED bow = IwramMulFX(Trig_Sin(i * (TRIG_TURN / 2) / segments), height);

    SET_POSITION(node, x + i * dx / segments, y + i * dy / segments + bow,
                 z + i * dz / segments);
    node->kind = kind;
    node = node->next;
  }
  return 0;
}

/// Unused: nothing refers to it. Rope_Coil for a tightrope.
UNUSED void Tightrope_Coil(s32 index, s32 from, s32 to) {
  struct TightropeState *state = TIGHTROPE_STATE;
  struct Actor *a = Actor_Lookup(from);
  struct Actor *b = Actor_Lookup(to);

  if (a != NULL && b != NULL) {
    struct TightropeSlot *slot = &state->slots[index];

    slot->first = from;
    slot->second = to;
    Tightrope_LayCoil(index, a->x.fixed, a->y.fixed + toFIXED(8), a->z.fixed,
                      b->x.fixed, b->y.fixed + toFIXED(8), b->z.fixed);
  }
}

/// Rope_LayCoil for a tightrope. Only the unused Tightrope_Coil calls it, and
/// it takes its nodes from the ropes' pool.
s32 Tightrope_LayCoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY,
                      s32 toZ) {
  struct TightropeState *state = TIGHTROPE_STATE;
  s32 result = 0;
  s32 dx = (toX - x) >> 8;
  s32 dy = (toY - y) >> 8;
  s32 dz = (toZ - z) >> 8;
  s32 length = Iwram_SqrtEntry(Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy) +
                               Iwram_MulFX(dz, dz));
  s32 segments = length / 6 + 1;
  struct TightropeNode **link = &state->slots[index].head;
  s32 angle = 0;
  FIXED radius = toFIXED(4);
  s32 i;
  s32 kind = ROPE_NODE_UNCROSSABLE;

  for (i = 0; i <= segments; i++) {
    struct TightropeNode *node = (struct TightropeNode *)Rope_AllocNode();
    u16 step;

    if (node == NULL) {
      result = -1;
      break;
    }
    SET_POSITION(node, x + Iwram_MulFX(Trig_Cos(angle), radius), y,
                 z + Iwram_MulFX(Trig_Sin(angle), radius));
    node->kind = kind;
    *link = node;
    step = Trig_Atan(toFIXED(6), radius);
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
UNUSED void Tightrope_Lash(s32 index, s32 firstId, s32 secondId) {
  struct Actor *first = Actor_Lookup(firstId);
  struct Actor *second = Actor_Lookup(secondId);

  if (first != NULL && second != NULL) {
    Tightrope_Throw(index, first->x.fixed, first->y.fixed + toFIXED(8),
                    first->z.fixed, second->x.fixed,
                    second->y.fixed + toFIXED(8), second->z.fixed,
                    Trig_Sin(gTightrope_LashPhase) << 2);
    gTightrope_LashPhase += TRIG_TURN / 16;
    wait(6);
    Actor_SelectAnimationFar(first, ANIM_POST_TIE);
    Actor_SelectAnimationFar(second, ANIM_POST_TIE);
  }
}

/// Rope_Throw for a tightrope.
s32 Tightrope_Throw(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct TightropeState *state = TIGHTROPE_STATE;
  s32 dx = toX - x;
  s32 ex = dx >> 8;
  s32 dy = toY - y;
  s32 ey = dy >> 8;
  s32 dz = toZ - z;
  s32 ez = dz >> 8;
  s32 segments = Iwram_SqrtEntry(Iwram_MulFX(ex, ex) + Iwram_MulFX(ey, ey) +
                                 Iwram_MulFX(ez, ez)) /
                     6 +
                 1;
  s32 frame;
  struct TightropeNode *node = state->slots[index].head;
  FIXED sine;
  s32 height;
  FIXED cosine;
  s32 lastX, lastY, lastZ;
  void **nodes;
  void **cursor;
  struct TightropeSlot *slot;
  s32 i;
  u16 angle;

  cursor = nodes = Heap_Alloc(512);
  if (segments >= 0) {
    i = segments + 1;
    do {
      i += -1;
      *cursor++ = node;
      node = node->next;
    } while (i != 0);
  }
  angle = Trig_Atan(ex, ey);
  sine = Trig_Sin(angle);
  cosine = Trig_Cos(angle);
  for (frame = 1; frame <= segments * 2; frame++) {
    for (i = segments; i >= 0; i--) {
      node = nodes[i];
      node->slot = index + 1;
      if (i == segments) {
        FIXED offsetX;
        FIXED offsetY;

        height = Iwram_MulFX(Trig_Sin(frame * (TRIG_TURN / 4) / i), toFIXED(8));
        offsetX = Iwram_MulFX(height, cosine);
        lastX = node->x = x + dx * frame / 2 / i + offsetX;
        offsetY = Iwram_MulFX(height, sine);
        lastY = node->y = y + dy * frame / 2 / i + offsetY;
        lastZ = node->z = z + dz * frame / 2 / i;
      } else {
        ex = (lastX - node->x) >> 8;
        ey = (lastY - node->y) >> 8;
        ez = (lastZ - node->z) >> 8;
        height = Iwram_SqrtEntry(Iwram_MulFX(ex, ex) + Iwram_MulFX(ey, ey) +
                                 Iwram_MulFX(ez, ez));

        if (height <= 6)
          break;
        lastX = node->x = lastX + (node->x - lastX) * 6 / height;
        lastY = node->y = lastY + (node->y - lastY) * 6 / height;
        lastZ = node->z = lastZ + (node->z - lastZ) * 6 / height;
        node->kind = ROPE_NODE_CROSSABLE;
      }
    }
    wait(1);
  }
  slot = &state->slots[index];
  slot->wobbling = 1;
  slot->amplitude = toFIXED(4);
  slot->phase = DEGtoANG(90);
  Heap_Release(nodes);
  return 0;
}

extern s32 gTightrope_UntiePhase;

/// Unused: nothing refers to it. Rope_Untie for a tightrope.
UNUSED void Tightrope_Untie(s32 index, s32 firstId, s32 secondId) {
  struct TightropeState *state;
  struct Actor *first;
  struct Actor *second;

  state = TIGHTROPE_STATE;
  first = Actor_Lookup(firstId);
  second = Actor_Lookup(secondId);

  if (first != NULL && second != NULL) {
    state->recoilFrom = first;
    state->recoilTo = second;
    Tightrope_Recoil(index, first->x.fixed, first->y.fixed + toFIXED(8),
                     first->z.fixed, second->x.fixed,
                     second->y.fixed + toFIXED(8), second->z.fixed,
                     Trig_Sin(gTightrope_UntiePhase) << 2);
    gTightrope_UntiePhase += TRIG_TURN / 16;
  }
}

/// Rope_Recoil for a tightrope.
s32 Tightrope_Recoil(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                     s32 wave) {
  struct TightropeState *state = TIGHTROPE_STATE;
  s32 wobbling = 0;
  struct TightropeNode *node;
  s32 dx, dy, dz;
  s32 dist;
  s32 count;
  s32 frame;
  s32 i;
  s32 angle;
  s32 x, y, z;
  void **nodes;
  void **entry;

  dx = (x2 - x1) >> 8;
  dy = (y2 - y1) >> 8;
  dz = (z2 - z1) >> 8;
  dist = Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy) + Iwram_MulFX(dz, dz);
  count = Iwram_Sqrt(dist) / 6 + 1;
  node = state->slots[slot].head;
  entry = nodes = Heap_Alloc(512);
  state->slots[slot].wobbling = wobbling;
  if (count >= 0) {
    i = count + 1;
    do {
      i += -1;
      *entry++ = node;
      node = node->next;
    } while (i != 0);
  }
  Actor_SelectAnimationFar(state->recoilFrom, ANIM_POST_UNTIE);
  Actor_SelectAnimationFar(state->recoilTo, ANIM_POST_UNTIE);
  wait(4);
  for (frame = 0; frame <= count; frame++) {
    angle = frame * (TRIG_TURN / 16);
    dist = toFIXED(4);
    for (i = 0; i <= count; i++) {
      node = nodes[i];
      node->slot = 0;
      if (i <= frame) {
        x = node->x = x1 + Iwram_MulFX(Trig_Cos(angle), dist);
        y = node->y = y1;
        z = node->z = z1 + Iwram_MulFX(Trig_Sin(angle), dist);
        node->kind = ROPE_NODE_UNCROSSABLE;
        {
          u16 step = Trig_Atan(toFIXED(6), dist);

          angle += step;
          dist += step * 4;
        }
      } else {
        dx = (x - node->x) >> 8;
        dy = (y - node->y) >> 8;
        dz = (z - node->z) >> 8;
        dist = Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy) + Iwram_MulFX(dz, dz);
        dist = Iwram_Sqrt(dist);
        if (dist <= 6)
          break;
        x = node->x = x + (node->x - x) * 6 / dist;
        y = node->y = y + (node->y - y) * 6 / dist;
        node->z = z + (node->z - z) * 6 / dist;
        node->kind = ROPE_NODE_UNCROSSABLE;
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
  struct TightropeState *state = TIGHTROPE_STATE;
  struct TightropeNode *node;

  for (node = state->slots[index].head; node != NULL; node = node->next)
    node->kind = ROPE_NODE_FREE;
  state->slots[index].head = NULL;
}

/// Stops tightrope index's wobble and marks its nodes not to be walked.
void Tightrope_Settle(s32 index) {
  struct TightropeState *state = TIGHTROPE_STATE;
  struct TightropeNode *node;

  for (node = state->slots[index].head; node != NULL; node = node->next)
    node->kind = ROPE_NODE_UNCROSSABLE;
  state->slots[index].wobbling = 0;
}

/// Unused: nothing refers to it. Rope_DebugSwing for a tightrope.
UNUSED void Tightrope_DebugSwing(struct Actor *actor) {
  struct Sprite *sprite = actor->sprite;

  sprite->offsetX = -3;
  sprite->offsetY = 12;
  if (gInput.repeat & KEY_L)
    gTightrope_DebugSwinging = 1;
  if (gTightrope_DebugSwinging != 0)
    Tightrope_WobbleBetween(0, DEBUG_FIRST_POST, DEBUG_SECOND_POST, 0);
}

/// The four pictures of each kind of hanging link, a vine's and a chain's:
/// top, upper, lower and bottom.
extern const u8 gHangingVine_VineTopPicture[], gHangingVine_VineUpperPicture[],
    gHangingVine_VineLowerPicture[];
extern const u8 gHangingVine_VineBottomPicture[],
    gHangingVine_ChainTopPicture[], gHangingVine_ChainMiddlePicture[];

/// The bytes of a link's picture, 16 by 16 pixels of 256 colours.
#define VINE_LINK_PICTURE_SIZE (16 * 16)

/// HangingVineLink.kind: a free link, the anchor (not drawn), then a link
/// showing each of the four pictures, in their order in VRAM.
enum HangingVineLinkKind {
  VINE_LINK_FREE,
  VINE_LINK_ANCHOR,
  VINE_LINK_TOP,
  VINE_LINK_UPPER,
  VINE_LINK_LOWER,
  VINE_LINK_BOTTOM
};

/// A vine's swing: its top link stays put, the first link below it sways one
/// and a half times the swing either way, and each link below that half a
/// swing more (HangingVine_Update). A vine left alone eases to the first,
/// and while the leader stands beside it to the second.
#define VINE_SWING_ALONE (TRIG_TURN / 256)
#define VINE_SWING_BESIDE (TRIG_TURN / 2048)
/// Each link sways this far behind the one above it, a turn being TRIG_TURN.
#define VINE_LINK_LAG 800

/// A vine link's depth slot bias, as ROPE_DEPTH a rope's.
#define HANGING_VINE_DEPTH 34

// The hanging vines (HEAP_SLOT_HANGING_VINE): eight slots, each seven links
// hung below an anchor actor, as the leafy vine that hangs down a wall in Gaia
// Rock's altar room, hanging down the wall to just over the leader's head. A
// vine sways, swings less while the leader stands beside it
// (HangingVine_Update), and once Whirlwind blows on it, swings the leader
// across (HangingVine_Swing, which FieldPsynergy_Whirlwind calls on its
// target).

/// Starts the hanging vines: takes and clears their state, loads the vine's
/// pictures for an anchor of SPRITE_SPROUTING_LEAVES, else the chain's, and
/// adds the tasks HangingVine_Draw and HangingVine_Update.
struct HangingVineState *HangingVine_Initialize(s32 spriteId) {
  struct HangingVineState *state;
  u8 *pictures;
  s32 size;

  state = HeapSlot_AllocEwram(HEAP_SLOT_HANGING_VINE,
                              sizeof(struct HangingVineState));
  size = 4 * VINE_LINK_PICTURE_SIZE;
  pictures = Heap_Alloc(size);
  Dma_Fill32(0, state, sizeof(struct HangingVineState));
  state->vramEntry = VramCache_FindFreeEntry();
  if (spriteId == SPRITE_SPROUTING_LEAVES) {
    Decompress_TaggedLz(gHangingVine_VineTopPicture, pictures);
    Decompress_TaggedLz(gHangingVine_VineUpperPicture,
                        pictures + VINE_LINK_PICTURE_SIZE);
    Decompress_TaggedLz(gHangingVine_VineLowerPicture,
                        pictures + 2 * VINE_LINK_PICTURE_SIZE);
    Decompress_TaggedLz(gHangingVine_VineBottomPicture,
                        pictures + 3 * VINE_LINK_PICTURE_SIZE);
    VramCache_Load(state->vramEntry, size, pictures);
  } else {
    Decompress_TaggedLz(gHangingVine_ChainTopPicture, pictures);
    Decompress_TaggedLz(gHangingVine_ChainMiddlePicture,
                        pictures + VINE_LINK_PICTURE_SIZE);
    Decompress_TaggedLz(gHangingVine_ChainMiddlePicture,
                        pictures + 2 * VINE_LINK_PICTURE_SIZE);
    Decompress_TaggedLz(gHangingVine_ChainBottomPicture,
                        pictures + 3 * VINE_LINK_PICTURE_SIZE);
    VramCache_Load(state->vramEntry, size, pictures);
  }
  Task_Add(HangingVine_Draw, TASK_PASS_FRAME);
  Task_Add(HangingVine_Update, TASK_PASS_FRAME + 15);
  Heap_Release(pictures);
  return state;
}

/// The vines' drawing task: each link's picture at the midpoint between it and
/// the link above, turned along the vine, for the links on screen.
void HangingVine_Draw(void) {
  struct HangingVineState *state = HANGING_VINE_STATE;
  s32 i;
  u32 angle;
  s32 prevX = 0;
  s32 prevY = 0;
  u32 tiles = gVramCacheEntries[state->vramEntry].offset / TILE_SIZE_4BPP;
  s32 prevZ = 0;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  s32 viewX = MapState_View(map)[0] & FIELD_PIXEL_MASK;
  s32 viewZ = MapState_View(map)[1] & FIELD_PIXEL_MASK;
  s32 height = map->origin[1];
  struct AffineTransform transform;
  struct HangingVineLink *link;

  for (i = 0; i < ROPE_SLOT_COUNT; i++) {
    for (link = state->slots[i].root; link != NULL; link = link->next) {
      s32 x = link->x;
      s32 y = link->y;
      s32 z = link->z;

      if (link->kind > VINE_LINK_ANCHOR) {
        s32 dz = prevZ - z;
        s32 absZ = dz >= 0 ? dz : z - prevZ;
        s32 dy = prevY - y;
        s32 screenX, screenY;
        u32 tile;

        if (dy >= 0 ? absZ < dy : absZ < y - prevY)
          angle = (u16)Trig_Atan(prevX - x, dy);
        else
          angle = (u16)Trig_Atan(prevX - x, dz);
        tile = tiles + (link->kind - VINE_LINK_TOP) *
                           (VINE_LINK_PICTURE_SIZE / TILE_SIZE_4BPP);
        screenX = (prevX + x) / 2 - viewX;
        prevY = (prevY + y) / 2 - height;
        prevZ = (prevZ + z) / 2 - viewZ - height;
        screenY = prevZ - prevY;
        if (screenX > -toFIXED(32) && screenX < toFIXED(DISPLAY_WIDTH + 32) &&
            screenY > -toFIXED(32) && screenY < toFIXED(DISPLAY_HEIGHT + 64)) {
          s32 sx = (FIXEDtoINT(screenX) - 8) & OBJ_X_MASK;
          s32 sy = (FIXEDtoINT(screenY) - 8) & OBJ_Y_MASK;
          s32 depth = FIXEDtoINT(prevY + prevZ) + HANGING_VINE_DEPTH;
          struct AnimationSpritePart *sprite = &link->sprite;

          ((u32 *)sprite)[0] = 0;
          ((u32 *)&link->sprite)[1] =
              sy | OBJ_X(sx) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
          ((u32 *)&link->sprite)[2] = tile | OBJ_PRIORITY(2);
          if (angle != 0) {
            transform.angle = angle;
            transform.scaleX = AFFINE_SCALE_ONE;
            transform.scaleY = AFFINE_SCALE_ONE;
            link->sprite.affine = 1;
            link->sprite.affineIndex = OamMatrix_AllocFromTransform(&transform);
          }
          link->angle = angle;
          Oam_AddSpriteAtDepthClamped(sprite, depth);
        }
      }
      prevX = x;
      prevY = y;
      prevZ = z;
    }
    prevX = 0;
    prevY = 0;
    link = state->slots[i].root;
    prevZ = 0;
    if (link != NULL)
      link->angle = angle;
  }
}
#undef VINE_LINK_PICTURE_SIZE

/// The vines' task: sways each active vine's links below its anchor, the swing
/// easing towards VINE_SWING_BESIDE while the leader stands beside the vine,
/// else towards VINE_SWING_ALONE.
void HangingVine_Update(void) {
  struct HangingVineState *state = HANGING_VINE_STATE;
  struct HangingVineSlot *slot;
  struct Actor *leader;
  s32 x, y, z;
  struct HangingVineLink *link;
  s32 i;
  s32 target;
  s32 swing;
  s32 sway;
  s32 radius;
  s32 angle;
#ifdef BUGFIX
  s32 slotIndex;
#endif

  i = 0;
  leader = Actor_GetById(Party_GetLeaderActor());
  target = VINE_SWING_ALONE;
  for (slot = state->slots; i < ROPE_SLOT_COUNT; i++, slot++) {
    if (slot->root == 0 || slot->active == 0)
      continue;

    slot->phase += TRIG_TURN / 128;
    link = ((struct HangingVineLink *)slot->root)->next;
    x = link->x;
    y = link->y;
    z = link->z;
    swing = slot->swing;

    // The leader stands beside the vine: within 8 pixels across and 16 deep of
    // its top link, and between 8 above and 80 below it.
    if ((x - leader->x.fixed >= 0 ? x - leader->x.fixed < toFIXED(8)
                                  : leader->x.fixed - x < toFIXED(8)) &&
        (u32)(z - leader->z.fixed + (toFIXED(16) - 1)) <= toFIXED(16) - 2 &&
        y + toFIXED(8) > leader->y.fixed && y - toFIXED(80) < leader->y.fixed)
      target = VINE_SWING_BESIDE;

    // Hang the five links below the top one.
    // BUG: the link loop reuses the slot counter and leaves it at 6, so past
    // the first active vine the loop goes on only while the next slot holds an
    // active vine too.
    link = link->next;
#ifdef BUGFIX
    slotIndex = i;
#endif
    for (i = 1, sway = swing; i <= 5; i++) {
      radius = i * slot->spacing;
      angle = Iwram_MulFX(Trig_Sin(slot->phase - i * VINE_LINK_LAG),
                          swing + sway / 2);
      link->x = x - Iwram_MulFX(Trig_Sin(angle), radius);
      sway += swing;
      link->y = y - Iwram_MulFX(Trig_Cos(angle), radius);
      link->z = z;
      link = link->next;
    }
#ifdef BUGFIX
    i = slotIndex;
#endif

    // Ease the swing towards the target.
    if (slot->swing > target)
      slot->swing = Iwram_MulFX(slot->swing, FIXED_ONE * 99 / 100);
    else
      slot->swing = Iwram_MulFX(slot->swing, FIXED_ONE * 111 / 100);
  }
}

/// Returns the first of HANGING_VINE_STATE's eight slots with no chain, or
/// NULL.
struct HangingVineSlot *HangingVine_FindFreeSlot(void) {
  struct HangingVineState *state = HANGING_VINE_STATE;
  struct HangingVineSlot *found = NULL;
  s32 index = 0;

  for (index = 0; index < ROPE_SLOT_COUNT; index++) {
    if (state->slots[index].root == NULL) {
      found = &state->slots[index];
      break;
    }
  }
  return found;
}

/// Returns the vine slot among HANGING_VINE_STATE's eight that holds actor id,
/// or NULL.
struct HangingVineSlot *HangingVine_FindSlot(s32 id) {
  struct HangingVineState *state = HANGING_VINE_STATE;
  struct HangingVineSlot *found = NULL;
  s32 index = 0;

  for (index = 0; index < ROPE_SLOT_COUNT; index++) {
    if (state->slots[index].id == id) {
      found = &state->slots[index];
      break;
    }
  }
  return found;
}

/// Returns the first unused of the vines' 128 links, or NULL.
struct HangingVineLink *HangingVine_AllocLink(void) {
  struct HangingVineState *state = HANGING_VINE_STATE;
  struct HangingVineLink *link = state->links;
  s32 index;

  for (index = 0; index < ROPE_NODE_COUNT; index++) {
    if (link->kind == VINE_LINK_FREE)
      return link;
    link++;
  }
  return NULL;
}

/// Hangs a vine of seven links from the anchor actor objectId, clearing the
/// five cells it hangs over, starting the vines first if they are not running;
/// with grow set, the vine drops into place over 28 frames.
void HangingVine_Hang(s32 objectId, s32 grow) {
  struct Actor *actor;
  struct MapState *map;
  struct HangingVineSlot *slot;
  struct HangingVineLink *link;
  struct HangingVineLink **tail;
  struct MapCell *cells;
  struct MapCell *cell;
  s32 i;
  s32 x;
  FIXED top;
  s32 z;
  s32 y;
  s32 frame;
  s32 last;
  s32 spriteId;
  s32 running;

  actor = Actor_GetById(objectId);
  running = Ram_HeapSlots->hangingVine;
  spriteId = SPRITE_SPROUTING_LEAVES;
  map = Ram_HeapSlots->mapGrid;
  if (running == 0) {
    if (actor->sprite != 0 && ((struct Sprite *)actor->sprite)->entries[0] != 0)
      spriteId = ((struct Sprite *)actor->sprite)->entries[0]->animationId;
    HangingVine_Initialize(spriteId);
  }
  if (HangingVine_FindSlot(objectId) != 0)
    return;
  slot = HangingVine_FindFreeSlot();
  if (slot == 0)
    return;

  // Clear the collision codes of the five cells the vine hangs over.
  x = FIELD_CELL(actor->x.fixed);
  z = FIELD_CELL(actor->z.fixed - actor->y.fixed);
  cells = map->layers[MAPGRID_LAYER_OVERHEAD].cells;
  for (i = 0; i < 5; i++) {
    z++;
    cell = cells;
    cell += x + z * MAPGRID_SIZE;
    cell->collisionCode = 0;
  }

  x = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  y = actor->y.fixed & FIELD_CELL_MASK;
  top = y + toFIXED(8);
  z = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(16);
  Actor_SelectAnimationFar((struct Actor *)actor, 0);
  actor->motionFlags = 0;
  actor->collisionFlags = 0;
  actor->z.fixed += toFIXED(16);
  actor->y.fixed -= toFIXED(64);
  Actor_SetSpriteFlagsFar(actor, 0);

  // Hang the links 14 pixels apart, the upper and lower pictures taking turns
  // between the top and the bottom.
  last = 6;
  tail = (struct HangingVineLink **)&slot->root;
  for (i = 0; i <= last && (link = HangingVine_AllocLink()) != 0; i++) {
    SET_POSITION(link, x, y + toFIXED(15) - i * toFIXED(14), z);
    if (i == 0)
      link->kind = VINE_LINK_ANCHOR;
    else if (i == 1)
      link->kind = VINE_LINK_TOP;
    else if (i == last)
      link->kind = VINE_LINK_BOTTOM;
    else
      link->kind = i % 2 + VINE_LINK_UPPER;
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
        s32 drop = -(frame * toFIXED(0.5));

        do {
          SET_POSITION(link, x, top - drop / 2, z);
          drop += toFIXED(frame);
          i++;
          link = link->next;
        } while (i <= last && link != 0);
      }
      wait(1);
    }
  }
  slot->active = 1;
  slot->phase = 0;
  slot->swing = VINE_SWING_ALONE;
  slot->id = objectId;
  slot->spacing = toFIXED(14);
}

/// The half turn on the leader's side of the anchor: 0 on its right, 1 on its
/// left.
#define SIDE_TURN(side) ((side) * (DEGtoANG(180)))

/// A vine's phase within one turn, its low byte dropped: a phase grows by
/// TRIG_TURN / 128 a frame and never wraps.
#define VINE_PHASE_MASK (TRIG_ANGLE_MASK & ~0xff)

/// A vine's swing, as VINE_SWING_ALONE: wound up a step a frame as Whirlwind
/// blows on it, while the leader hangs on, and while it carries the leader
/// across.
#define VINE_SWING_STEP (TRIG_TURN / 1024)
#define VINE_SWING_HELD (TRIG_TURN * 3 / 128)
#define VINE_SWING_CARRYING (TRIG_TURN / 32)

/// Whirlwind on the vine hanging from actor id: the vine swings out to the
/// leader's side and the leader takes hold. While the leader hangs on, B, Down
/// or the direction away from the vine lets go; any other button leaps, and the
/// vine carries the leader across to the far side.
void HangingVine_Swing(s32 id) {
  s32 rootX, rootY;
  s32 motionFlags;
  struct Sprite *heldSprite;
  s32 side = 0;
  struct Actor *leader = Actor_GetById(Party_GetLeaderActor());
  struct Actor *anchor = Actor_GetById(id);
  struct HangingVineSlot *slot;
  struct Sprite *sprite;
  s32 swing;
  s32 start;
  s32 dir;
  u16 letGo;
  FIXED radius;
  s32 steps;
  s32 angle;
  FIXED sine;
  s32 i;

  if (leader == NULL || anchor == NULL)
    return;
  motionFlags = leader->motionFlags;
  heldSprite = leader->sprite;
  if (leader->x.fixed < anchor->x.fixed)
    side = 1;
  slot = HangingVine_FindSlot(id);
  if (slot == NULL)
    return;

  start = slot->swing;
  if ((slot->phase ^ SIDE_TURN(side)) & (DEGtoANG(180))) {
    for (swing = start; swing >= 0; swing -= VINE_SWING_STEP) {
      slot->swing = swing;
      wait(1);
    }
    start = 0;
  }
  for (swing = start; swing <= VINE_SWING_HELD; swing += VINE_SWING_STEP) {
    slot->swing = swing;
    slot->phase = SIDE_TURN(side) + DEGtoANG(90);
    wait(1);
  }
  while ((slot->phase & VINE_PHASE_MASK) !=
         SIDE_TURN(side ^ 1) + DEGtoANG(90)) {
    slot->swing = VINE_SWING_HELD;
    wait(1);
  }

  sprite = leader->sprite;
  dir = 1;
  if (side) {
    letGo = KEY_DOWN | KEY_LEFT | KEY_B;
    dir -= 2;
  } else
    letGo = KEY_DOWN | KEY_RIGHT | KEY_B;
  slot->active = 1;
  Actor_SelectAnimationFar(leader, ANIM_LEADER_HANG);
  while (1) {
    // The vine bounces under the leader twice in 64 frames.
    slot->phase = SIDE_TURN(side ^ 1) + DEGtoANG(90);
    switch (gFrameCount & 63) {
    case 0:
    case 16:
      slot->swing = VINE_SWING_HELD + TRIG_TURN / 2048;
      slot->spacing = toFIXED(29) / 2;
      sprite->rotation = dir * (TRIG_TURN / 64);
      break;
    case 1:
    case 2:
    case 17:
    case 18:
      slot->swing += TRIG_TURN / 4096;
      slot->spacing += toFIXED(1) / 4;
      sprite->rotation += dir * (TRIG_TURN / 128);
      break;
    default:
      if (slot->swing > VINE_SWING_HELD)
        slot->swing -= TRIG_TURN / 2048;
      else
        slot->swing = VINE_SWING_HELD;
      if (slot->spacing > toFIXED(14))
        slot->spacing -= toFIXED(1) / 32;
      if (sprite->rotation != 0)
        sprite->rotation -= dir * (TRIG_TURN / 512);
      break;
    }
    if (gInput.pressed & letGo) {
      sprite->rotation = 0;
      slot->active = 1;
      return;
    }
    if (gInput.pressed != 0)
      break;
    wait(1);
  }

  sprite->rotation = 0;
  slot->active = 0;
  Actor_SelectAnimationFar(leader, ANIM_LEADER_SPRING);
  wait(6);
  Sound_PlayFar(SE_JUMP);
  Actor_SelectAnimationFar(leader, ANIM_LEADER_LEAP);
  leader->speedLimit = toFIXED(3);
  leader->acceleration = toFIXED(2);
  leader->velocityY = toFIXED(4);
  leader->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
  Actor_SetSpriteFlagsFar(leader, 0);
  wait(5);
  Actor_SelectAnimationFar(leader, ANIM_LEADER_SWING);
  leader->motionFlags = 0;
  rootX = ((struct HangingVineLink *)slot->root)->next->x;
  rootY = ((struct HangingVineLink *)slot->root)->next->y;
  radius = toFIXED(65);
  steps = 8;
  while ((slot->phase & VINE_PHASE_MASK) !=
         SIDE_TURN(side) + TRIG_TURN * 7 / 32) {
    if (steps != 0) {
      radius += toFIXED(5) / 16;
      slot->spacing += toFIXED(1) / 16;
      steps--;
    }
    slot->swing = VINE_SWING_CARRYING;
    slot->active = 1;
    // The leader hangs where the bottom link sways.
    sine = Trig_Sin(slot->phase - 5 * VINE_LINK_LAG);
    angle = IwramMulFX(sine, slot->swing + slot->swing * 5 / 2);
    leader->x.fixed = rootX - IwramMulFX(Trig_Sin(angle), radius);
    leader->y.fixed = rootY - IwramMulFX(Trig_Cos(angle), radius);
    leader->targetX = leader->x.fixed;
    leader->targetY = leader->y.fixed;
    heldSprite->rotation = angle;
    wait(1);
  }
  slot->active = 1;
  Actor_SelectAnimationFar(leader, ANIM_LEADER_LEAP);
  leader->motionFlags = motionFlags & ~ACTOR_FOLLOWS_TERRAIN;
  leader->velocityY = toFIXED(4);
  for (i = 0; i < 8; i++) {
    slot->spacing -= toFIXED(1) / 16;
    wait(1);
  }
  slot->spacing = toFIXED(14);
  heldSprite->rotation = 0;
  Actor_SelectAnimationFar(leader, ANIM_LEADER_SPRING);
  Actor_SetSpriteFlagsFar(leader, SPRITE_FLAG_SHADOW);
  leader->motionFlags = motionFlags;
  wait(6);
  slot->active = 1;
}

/// Hangs a vine at once from high above actor id down to its feet
/// (HangingVine_Hang): an actor that is not moving is lifted and moved 16
/// pixels north first, which HangingVine_Hang undoes; a moving one has its vine
/// only when force is set.
void HangingVine_Place(s32 id, s32 force) {
  struct Actor *actor = Actor_GetById(id);

  if (actor != NULL) {
    if (actor->motionFlags == 0) {
      actor->z.fixed -= toFIXED(16);
      actor->y.fixed += toFIXED(64);
      HangingVine_Hang(id, 0);
    } else if (force) {
      HangingVine_Hang(id, 0);
    }
  }
}

#undef SIDE_TURN
