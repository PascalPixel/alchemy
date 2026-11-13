/// @file
/// A Mercury Djinni joining the party, and the eight chains of nodes drawn each
/// frame.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "gamepak/far_call/shop.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/field_sequence.h"
#include "module/field/object.h"
#include "module/system/system.h"
#include "scene_call.h"

/* VIEW(f0d8740): routine FieldEffect_UpdateRandomTargetParticle as void
 * FieldEffect_UpdateRandomTargetParticle(void); the definition has void
 * FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect). */
extern void FieldEffect_UpdateRandomTargetParticle__f0d8740(void) __asm__(
    "FieldEffect_UpdateRandomTargetParticle");

/* VIEW(f0d8740): tag struct CaptureObject as struct CaptureObject__f0d8740;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:4832: structure has no member
 * named `sprite'. */
struct CaptureObject__f0d8740 {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[3];
  u8 priorityFlags;
  u8 unk24[44];
  struct CaptureSprite *sprite;
  u8 unk54;
  u8 mode;
  u8 unk56[4];
  u8 collisionFlags;
  u8 visible;
  u8 unk5c[12];
  struct CaptureObject__f0d8740 *linked;
  void *callback;
};

/* VIEW(f0d8740): routine Actor_GetById as struct CaptureObject__f0d8740
 * *Actor_GetById(s32 id); mars_join has struct Actor *Actor_GetById(u32
 * objectId). */
extern struct CaptureObject__f0d8740 *
Actor_GetById__f0d8740(s32 id) __asm__("Actor_GetById");
void Actor_Jump(s32 id, s32 height, s32 frames);
/* VIEW(f0d8740): routine Actor_CreateFar as struct CaptureObject__f0d8740
 * *Actor_CreateFar(s32 kind, s32 x, s32 y, s32 z); visual_attributes has struct
 * BattleEffectWatchedObject *Actor_CreateFar(s32 kind, s32 x, s32 y, s32 z). */
extern struct CaptureObject__f0d8740 *
Func_080200c0__f0d8740(s32 kind, s32 x, s32 y,
                       s32 z) __asm__("Actor_CreateFar");
/* VIEW(f0d8740): routine Actor_Destroy as void Actor_Destroy(struct
 * CaptureObject__f0d8740 *object); battle.h has void Actor_Destroy(struct Actor
 * *object). */
extern void Actor_Destroy__f0d8740(
    struct CaptureObject__f0d8740 *object) __asm__("Actor_Destroy");
/* VIEW(f0d8740): routine Camera_WorldToScreen as void Camera_WorldToScreen(s32
 * *position); mars_join has void Camera_WorldToScreen(struct EffectVector
 * *position). */
extern void
Camera_WorldToScreen__f0d8740(s32 *position) __asm__("Camera_WorldToScreen");
/* VIEW(f0d8740): routine FieldParticle_Initialize as void
 * FieldParticle_Initialize(void *slot, s32 kind, s32 x, s32 y); motes has void
 * FieldParticle_Initialize(struct FieldParticle *slot, s32 kind, s32 x, s32 z).
 */
extern void
FieldParticle_Initialize__f0d8740(void *slot, s32 kind, s32 x,
                                  s32 y) __asm__("FieldParticle_Initialize");
/* VIEW(f0d8740): routine FieldParticle_SetCallback as void
 * FieldParticle_SetCallback(void *slot, void *callback); field_particle.h has
 * void FieldParticle_SetCallback(struct FieldParticle *, EffectCallback). */
extern void FieldParticle_SetCallback__f0d8740(
    void *slot, void *callback) __asm__("FieldParticle_SetCallback");
/* VIEW(f0d8740): routine FieldParticle_SetActorMode as void
 * FieldParticle_SetActorMode(void *slot, s32 mode); field_particle.h has void
 * FieldParticle_SetActorMode(struct FieldParticle *, s32). */
extern void FieldParticle_SetActorMode__f0d8740(void *slot, s32 mode) __asm__(
    "FieldParticle_SetActorMode");

void Djinn_RunMercuryJoinEffect(s32 arg) {
  struct CaptureObject__f0d8740 *djinni;
  struct CaptureObject__f0d8740 *copy;
  struct CaptureObject__f0d8740 *previous;
  struct CaptureResource *resource;
  void *effectSlots;
  struct CaptureObject__f0d8740 **cursor;
  struct CaptureObject__f0d8740 **list;
  u8 *slot;
  s8 *slotState;
  s32 kind;
  s32 remaining;
  s32 resourceId;
  struct CaptureObject__f0d8740 *copies[8];
  struct {
    s32 x;
    s32 y;
    s32 z;
  } position;

  djinni = Actor_GetById__f0d8740(arg);
  if (djinni == NULL)
    return;

  FieldEffect_InitializeSlots();
  effectSlots = Ram_HeapSlots->effectWork;
  Shop_SaveAndTintFieldPaletteFar(0x204084);
  wait(30);
  djinni->visible = 0;
  Sound_PlayFar(173);
  Motion_SetVarCbAndRefresh(arg, 1);
  Sound_PlayFar(175);
  Motion_SetVarCbAndRefresh(arg, 1);
  wait(20);
  Sound_PlayFar(152);
  Actor_Jump(arg, 3, 14);
  Sound_PlayFar(152);
  Actor_Jump(arg, 5, 16);
  Sound_PlayFar(152);
  Actor_Jump(arg, 7, 18);
  wait(20);

  kind = *djinni->sprite->resource;
  resource = NULL;
  previous = djinni;
  cursor = copies;
  for (remaining = 7; remaining >= 0; remaining--) {
    copy = Func_080200c0__f0d8740(kind, djinni->x, djinni->y, djinni->z);
    *cursor++ = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->mode = 0;
      copy->priorityFlags = 2;
      copy->collisionFlags |= 1;
      copy->callback = (void *)Actor_MoveHalfwayTowardTargetCopy;
      copy->facing = djinni->facing;
      Actor_SetSpriteColorModeFar((struct Actor *)copy, 9);
      Actor_SetMode(copy, 0);
      Actor_SetSingleChildFlagsFar((struct Actor *)copy, 0);
      resource = Sprite_ShareVramCacheEntry(copy->sprite, resource);
      copy->linked = previous;
      previous = copy;
    }
  }

  resourceId = resource->id;
  Sound_PlayFar(153);
  djinni->mode = 0;
  for (remaining = 14; remaining >= 0; remaining--) {
    djinni->y += 0x80000;
    wait(1);
  }
  Actor_Destroy__f0d8740(djinni);
  list = copies;
  for (remaining = 7; remaining >= 0; remaining--)
    Actor_Destroy__f0d8740(*list++);
  if (resourceId != 96)
    VramCache_FreeEntry(resourceId);
  wait(10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
  wait(20);
  Actor_SetMode(Actor_GetById__f0d8740(gMainRoutineWork.currentOwner), 28);
  wait(20);

  position.x = djinni->x;
  position.y = djinni->y;
  position.z = djinni->z;
  Camera_WorldToScreen__f0d8740(&position.x);

  slot = (u8 *)effectSlots + 80;
  remaining = 23;
  do {
    FieldParticle_Initialize__f0d8740(slot, 301, position.x, position.z);
    FieldParticle_SetCallback__f0d8740(
        slot, (void *)FieldEffect_UpdateRandomTargetParticle__f0d8740);
    FieldParticle_SetActorMode__f0d8740(slot, 7);
    Animation_ApplyChildValuesToRecordFar(*(struct ActorSpriteView **)slot, 9);
    remaining--;
    wait(1);
    slot += 72;
  } while (remaining >= 0);

  wait(120);
  {
    s32 nextState;

    nextState = 2;
    slotState = effectSlots;
    slotState += 144;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += 72;
    } while (remaining >= 0);
  }

  wait(50);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ReleaseSlots();
}

// XXX: what the work block in heap slot 39 holds, and what this family does;
// its callers and the block's readers would settle it.

void Func_080d897c(void) {
  struct Work9c *work = HeapSlot_AllocEwram(156, sizeof(struct Work9c));
  void *buffer = Heap_Alloc(0x100);

  Dma_Fill32(0, work, sizeof(struct Work9c));
  work->vramEntry = VramCache_FindFreeEntry();
  Decompress_TaggedLz(gData_080f0c04, buffer);
  VramCache_Load(work->vramEntry, 0x100, buffer);
  Task_Add(Func_080d8a00, TASK_PASS_VBLANK);
  Task_Add(Func_080d8c10, TASK_PASS_VBLANK + 15);
  Heap_Release(buffer);
}

struct ChainWork {
  u8 unk000[8];
  s32 vramEntry;
  struct ChainSlot slots[8];
};

/* VIEW(f0d8740): routine OamMatrix_AllocFromTransform as s32
 * OamMatrix_AllocFromTransform(struct ChainTransform *source); affine.h has s32
 * OamMatrix_AllocFromTransform(struct AffineTransform *source). */
extern s32 AffineMatrix_BuildForEffect__f0d8740(
    struct ChainTransform *source) __asm__("OamMatrix_AllocFromTransform");
/* VIEW(f0d8740): routine Oam_AddSpriteAtDepthClamped as void
 * Oam_AddSpriteAtDepthClamped(void *entry, s32 slot); f0d6674 has void
 * Oam_AddSpriteAtDepthClamped(struct GroundParticle *sprite, s32 depth). */
extern void
Func_080140d8__f0d8740(void *entry,
                       s32 slot) __asm__("Oam_AddSpriteAtDepthClamped");

/// The heap slot 0x9c work's per-frame callback (Func_080d897c schedules it).
/// For each of the eight slots it draws a sprite at the midpoint of each pair
/// of neighbouring nodes in the slot's chain, 4 pixels up, turned along the
/// pair and given the slot's priority, culled to the screen and sorted into the
/// OAM buckets by depth; the chain's first node takes the last angle.
void Func_080d8a00(void) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  struct ChainSlot *slot = work->slots;
  s32 i;
  s32 prevX = 0;
  s32 prevY = 0;
  u32 tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  s32 prevZ = 0;
  s32 started = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 originY = map->origin[1];
  struct ChainTransform transform;
  struct ChainNode *node;
  u32 angle;

  for (i = 7; i >= 0; i--, slot++) {
    for (node = slot->head; node != NULL; node = node->next) {
      s32 priority = slot->priority;
      s32 y = node->y;
      s32 x = node->x;
      s32 z = node->z;

      y += 0xfffc0000;

      if (started) {
        s32 screenX;
        s32 depth;
        s32 screenY;

        angle = ((u16)ArcTan2(prevX - x, (z - prevZ) + (prevY - y)) + 0x4000) &
                0xffff;
        screenX = (prevX + x) / 2 - viewX;
        // prevY is reused for the midpoint's height, then its depth key; a
        // local of its own differs.
        prevY = (prevY + y) / 2 - originY;
        depth = (prevZ + z) / 2 - viewZ - originY;
        screenY = depth - prevY;
        if ((u32)(screenX + 0x1fffff) <= 0x12ffffe && screenY > -0x200000 &&
            screenY <= 0xdfffff) {
          s32 spriteX = ((screenX >> 16) - 4) & 0x1ff;
          s32 spriteY = ((screenY >> 16) - 4) & 0xff;
          struct AnimationSpritePart *sprite;

          prevY = ((prevY + depth) >> 16) + 58;
          sprite = &node->sprite;

          ((u32 *)sprite)[0] = 0;
          ((u32 *)&node->sprite)[1] = spriteY | (spriteX << 16) | 0x2000;
          ((u32 *)&node->sprite)[2] = 0x800 | tile;
          if (angle != 0) {
            transform.angle = angle;
            transform.scaleX = 0x100;
            transform.scaleY = 0x100;
            node->sprite.affine = 1;
            node->sprite.affineIndex =
                AffineMatrix_BuildForEffect__f0d8740(&transform);
          }
          node->sprite.priority = priority;
          node->angle = angle;
          Func_080140d8__f0d8740(sprite, prevY);
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

struct SwingWork {
  u8 unk000[12];
  struct SwingSlot slots[8];
};

void Func_080d8c10(void) {
  struct SwingEvent *event = Ram_HeapSlots->eventWork;
  struct SwingWork *work = Ram_HeapSlots->unk84[6];
  struct SwingSlot *slot;
  s32 i = 0;

  if (!GameFlag_Test(260) && event->timer > 12) {
    if (gMainRoutineWork.renderMode == 0) {
      s32 target = Field_FindNearbyTalkTarget(gMainRoutineWork.currentOwner);

      if (target != -1) {
        for (i = 0; i < 8; i++)
          if (work->slots[i].id == target || work->slots[i].value == target)
            break;
        if (i < 8 && Func_080d99b8(work->slots[i].id, work->slots[i].value))
          event->prompt = 0xfc85;
      }
    } else if (gMainRoutineWork.renderMode == 3) {
      event->prompt = 0xfc86;
    }
  }
  slot = work->slots;
  for (i = 0; i < 8; i++, slot++) {
    if (slot->active != 0) {
      s32 swing = Iwram_MulQ16(Trig_Sin(slot->phase), slot->amplitude);

      slot->phase += 0x1000;
      slot->amplitude = Iwram_MulQ16(slot->amplitude, 0xf333);
      if (slot->amplitude <= 0x1998) {
        slot->amplitude = 0;
        slot->active = 0;
      }
      Func_080d8fa8(i, slot->id, slot->value, swing);
    }
  }
}

struct Work9cItem *Func_080d8d40(void) {
  struct Work9c *work = WORK_9C;
  struct Work9cItem *item = work->items;
  s32 index;

  for (index = 0; index <= 127; index++) {
    if (item->used == 0)
      return item;
    item++;
  }
  return NULL;
}

void Func_080d8d68(s32 index, s32 id, s32 other) {
  struct Work9c *work = WORK_9C;
  s32 priority = 2;
  struct Actor *first = ObjectTable_Get(id);
  struct Actor *second = ObjectTable_Get(other);

  if (first != NULL && second != NULL) {
    struct Work9cSlot *slot;
    s32 lift;

    if (first->animationKind == 1)
      priority = ((struct FieldSprite *)first->sprite)->priority;
    slot = &work->slots[index];
    slot->id = id;
    slot->value = other;
    *(s16 *)slot->unk06 = priority;
    lift = 128;
    /* FAKEMATCH(barrier): an empty asm on lift keeps its 128 built after the
     * priority's store and ahead of the coordinates' loads; without it, 10
     * of 75 instructions differ in tla-en. */
    asm volatile("" : "+l"(lift));
    lift <<= 12;
    Func_080d8e08(index, first->x, first->y + lift, first->z, second->x,
                  second->y + lift, second->z);
  }
  Actor_SetMode(first, 2);
  Actor_SetMode(second, 2);
}

s32 Func_080d8e08(s32 index, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2) {
  struct RopeWork *work = Ram_HeapSlots->unk84[6];
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
  s32 sin;
  s32 cos;
  s32 i;

  count = Iwram_Sqrt(Iwram_MulQ16(sx, sx) + Iwram_MulQ16(sy, sy) +
                     Iwram_MulQ16(sz, sz)) /
              6 +
          1;
  slot = &work->slots[index];
  link = &slot->head;
  angle = ArcTan2(sx, sy);
  sin = Trig_Sin(angle);
  cos = Trig_Cos(angle);
  slot->count = count;
  for (i = 0; i <= count; i++) {
    s32 bow;

    node = Func_080d8d40();
    if (node == NULL) {
      result = -1;
      break;
    }
    bow = Iwram_MulQ16(Trig_Sin((i << 16) / count), 0x10000);
    node->x = x1 + i * dx / count + Iwram_MulQ16(bow, cos);
    node->y = y1 + i * dy / count + Iwram_MulQ16(bow, sin);
    node->z = z1 + i * dz / count;
    node->slot = index + 1;
    if (i == count)
      node->kind = 2;
    else
      node->kind = 1;
    *link = node;
    link = &node->next;
  }
  *link = NULL;
  slot->state = 1;
  slot->active = 1;
  slot->amplitude = 0x10000;
  slot->phase = 0x4000;
  if (slot->flag != 0)
    GameFlag_SetBit(slot->flag);
  return result;
}

void Func_080d8fa8(s32 index, s32 from, s32 to, s32 mode) {
  struct Actor *a = ObjectTable_Get(from);
  struct Actor *b = ObjectTable_Get(to);

  if (a != NULL && b != NULL)
    Func_080d8ff8(index, a->x, a->y + 0x80000, a->z, b->x, b->y + 0x80000, b->z,
                  mode);
}
