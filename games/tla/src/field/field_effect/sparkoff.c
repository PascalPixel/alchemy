#include "types.h"
#include "task.h"
#include "dma.h"
#include "edition.h"
#include "io_reg.h"
#include "io_write.h"
#include "map.h"
#include "ram_buffer.h"

s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

/// One of Func_080d6674's particles: its sprite's attribute words, its world
/// position and its frames left.
struct SparkParticle {
  u32 sprite[3];
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 unk18[4];
  u16 timer;
  u8 unk1e[2];
};

/// Heap slot 0x74 while the particles run.
struct SparkParticleWork {
  s32 vramEntry;
  s32 tile;
  struct SparkParticle particles[64];
};

void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Func_080d6674(void);
void Func_080d2c64(s32 cue);

extern const u8 Data_080f3901[];

/// Starts Func_080d6674's particles: allocates and clears heap slot 0x74,
/// decodes the particle tiles into VRAM, sets each of the 64 particles' sprite
/// words and places it on the ground at the map's focus with a timer of
/// (i & 11) + 1, turns on alpha blending of all layers at 16/16, schedules the
/// update and, when asked, plays cue 137 (Func_080d2c64).
void Func_080d6f34(s32 withCue) {
  struct SparkParticleWork *work = HeapSlot_AllocEwram(116, 0x814);
  struct SparkParticle *particle = work->particles;
  volatile u32 fill;
  /* FAKEMATCH(pin): the fill's zero held in r1, built after the copy of
   * work; as a plain local, 8 of 100 instructions differ in tla-en. */
  register u32 zero asm("r1");
  u8 *tiles;
  u32 i;
  struct HeapSlots *slots;
  /* FAKEMATCH(pin): clear held in r6, which keeps its zero set inside the
   * loop on every pass and ready for the last blend register; as a plain
   * local, 4 of 100 instructions differ in tla-en. */
  register s32 clear asm("r6");

  zero = 0;
  Dma_Fill32Cell(fill, zero, work, 0x814);
  tiles = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(Data_080f3901, tiles);
  work->vramEntry = Resource_FindFreeEntry();
  work->tile = VramCache_Load(work->vramEntry, 0x300, tiles);
  HeapSlot_Release(56);
  i = 0;
  slots = Ram_HeapSlots;
  for (; i < 64; i++, particle++) {
    struct MapWork *view = slots->mapWork;
    s32 *focus;
    /* FAKEMATCH(temporary): one integer cursor for the attribute stores and
     * the height's x keeps the stores in r1; as an s32 * cursor and an x of
     * its own, 18 of 100 instructions differ in tla-en. */
    u32 p;
    s32 z;

    p = (u32)particle;
    clear = 0;
    *(s32 *)p = clear;
    p += sizeof(s32);
    focus = view->origin;
    *(s32 *)p = 0x40000400;
    p += sizeof(s32);
    *(s32 *)p = 0xd400;
    p = focus[0];
    z = focus[2];
    particle->posX = (s32)p;
    particle->posZ = z;
    particle->posY = Map_GetTerrainHeightFar(0, (s32)p, z);
    particle->timer = (i & 11) + 1;
  }
  Io_Write16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
  Io_Write16(clear, &REG_BLDY);
  Task_Add(Func_080d6674, TASK_PASS_VBLANK);
  if (withCue != 0)
    Func_080d2c64(137);
}

void Func_080d7024(void) { Task_Remove(Func_080d6674); }

#include "heap_release.h"
#include "frame.h"
#include "system.h"

struct Sparkle {
  u8 padding00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 matrix : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 padding0a[2];
  s32 posX;
  s32 height;
  s32 posZ;
  s32 delay;
  u16 timer;
  u8 padding1e[2];
};

struct SparkleWork {
  u8 padding00[4];
  s32 tileBase;
  struct Sparkle sparkles[64];
  u8 padding808[8];
  s32 stopped;
};

struct SparklePicture {
  s16 dy;
  u16 tile;
};

extern const struct SparklePicture gData_080f0b7c[];

s32 GameFlag_Test(s32 flag);
void Func_080140d8(void *entry, s32 priority);

void FieldEffect_UpdateSparkles(void) {
  struct SparkleWork *work;
  u32 spawned;
  u32 i;
  struct MapWork *map;
  s32 *camera;
  s32 spawnX;
  s32 spawnZ;
  struct Sparkle *sparkle;
  const u16 *frame;
  s32 *leader;
  s32 cameraX;
  s32 cameraZ;
  s32 sx;
  s32 sy;
  s32 delay;
  s32 x;
  u32 a;
  u32 b;
  u32 frames;

  work = Ram_HeapSlots->unk70[1];
  spawned = 0;
  map = Ram_HeapSlots->mapWork;
  camera = &map->viewX;
  spawnX = 0;
  spawnZ = 0;
  i = 0;
  sparkle = work->sparkles;
  delay = 0;
  sx = 0;
  sy = 0;
  do {
    if (sparkle->timer != 0) {
      cameraX = camera[0];
      cameraZ = camera[1];
      if (GameFlag_Test(0x166) != 0)
        sparkle->timer++;
      frame = (const u16 *)&gData_080f0b7c[sparkle->timer >> 1];
      a = Random16();
      b = Random16();
      x = ((sparkle->posX - cameraX) >> 16) + (((a & 1) + (b & 1)) >> 1);
      sx = x - 1;
      sy = (sparkle->posZ - sparkle->height - cameraZ) / 0x10000 +
           *(const s16 *)frame;
      frame++;
      if ((u32)(x + 15) <= 255 && sy >= -32 && sy <= 159) {
        sparkle->tile = work->tileBase + *frame;
        sparkle->x = sx;
        sparkle->shape = 0;
        sparkle->size = 1;
#ifdef UBFIX
        frames = gFrameCount;
#else
        // UB: this reads a u32 as unsigned long. The unsigned long alias
        // combines the size and flip stores; as gFrameCount or a u32 alias,
        // 29 of 232 instructions differ in tla-en. UBFIX reads the declared
        // type.
        frames = *(unsigned long *)&gFrameCount;
#endif
        sparkle->y = sy;
        sparkle->matrix = ((frames >> 1) & 1) << 3;
        Func_080140d8(sparkle, 240);
      }
      sparkle->timer--;
    }
    if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0) {
      if (delay != 0) {
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->height = Map_GetTerrainHeightFar(0, sx, sy);
        sparkle->timer = 62 - delay;
        sparkle->delay = 0;
        spawned++;
        delay += 4;
      } else if ((Random16() & 255) == 0) {
        leader = map->origin;
        spawnX = leader[0] + (Random16() << 8) - 0x800000;
        spawnZ = leader[2] + (Random16() << 8) - 0x800000;
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->height = Map_GetTerrainHeightFar(0, sx, sy);
        sparkle->timer = 30;
        sparkle->delay = delay;
        spawned++;
        delay = 4;
      }
    }
    i++;
    sparkle++;
  } while (i <= 63);
}

#include "battle.h"
#include "graphics.h"
#include "palette_buffers.h"
#include "field_blend.h"
#include "resource.h"

void Func_080d67e8(void);

void BattleEffect_InterpolateBuffers(s16 *source, s16 *target, s16 *step,
                                     s32 frames);

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(const void *, void *))Resource_DecodeByteLzInRam)

union DustWord {
  u32 value;
  void *link;
};

struct DustParticle {
  union DustWord link;
  u32 attr01;
  u32 attr2;
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 pad18[4];
  u16 timer;
  u8 pad1e[2];
};

struct DustWork {
  s32 vramEntry;
  s32 tileBase;
  struct DustParticle particles[64];
  u8 pad808[12];
};

extern const u8 gData_080f38b2[];

void FieldEffect_InitializeSparkles(void) {
  u32 size = sizeof(struct DustWork);
  struct DustWork *work = HeapSlot_AllocEwram(116, size);
  struct DustParticle *p = work->particles;
  u8 *buf;
  u32 i;
  s32 clear;
  volatile u32 zero;
  struct HeapSlots *slots;

  // size is reused for the fill's zero; a zero of its own differs.
  size = 0;
  zero = size;
  Dma_Set(&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
              (sizeof(struct DustWork) / 4),
          REG_DMA3);
  buf = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f38b2, buf);
  work->vramEntry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->vramEntry, 0x200, buf);
  HeapSlot_Release(56);

  i = 0;
  slots = Ram_HeapSlots;
  clear = 0;
loop:
  {
    struct MapWork *view = slots->mapWork;
    s32 *leader;
    /* FAKEMATCH(temporary): one integer cursor for the stores and the x keeps
     * the leader in r2; as a u32 * cursor and an x of its own, 16 of 79
     * instructions differ in tla-en. */
    u32 attr;
    s32 z;

    attr = (u32)p;
    *(u32 *)attr = clear;
    attr += sizeof(union DustWord);
    leader = view->origin;
    *(u32 *)attr = 0x40000400;
    attr += sizeof(union DustWord);
    *(u32 *)attr = 0xd400;
    attr = leader[0];
    z = leader[2];
    p->posX = clear;
    p->posZ = clear;
    p->posY = Map_GetTerrainHeightFar(0, attr, z);
    p->timer = (i & 15) + 1;
  }
  i++;
  p++;
  if (i < 64)
    goto loop;
  Task_Add(FieldEffect_UpdateSparkles, TASK_PASS_VBLANK);
}

void BattleEffect_StartTwelveFrameBlend(void) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;
  volatile u32 zero;

  work = HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  buffers = Ram_HeapSlots->block32;
  zero = 0;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*work) / 4),
          REG_DMA3);
  BattleEffect_BuildBuffer(0x10003, (u16 *)buffers, (u16 *)work, 1);
  BattleEffect_BuildBuffer(0x10005, (u16 *)buffers, (u16 *)work->to, 1);
  BattleEffect_InterpolateBuffers(work->to, work->from, work->delta, 12);
  BattleEffect_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 600;
  work->enabled = 1;
  Task_Add(Func_080d67e8, TASK_PASS_VBLANK);
}

void BattleEffect_SetBlock30ValuesMaxZero(void) {
  struct FieldBlendWork *state =
      HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  state->timer = 0x7fff;
  state->enabled = 0;
}

void BattleEffect_SetBlock30Values12Zero(void) {
  struct FieldBlendWork *state =
      HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  state->timer = 12;
  state->enabled = 0;
}

void BattleEffect_SetBlock30Values128One(void) {
  struct FieldBlendWork *state =
      HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  state->timer = 128;
  state->enabled = 1;
}

void BattleEffect_StartBufferBlend(s32 from, s32 to) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;
  volatile u32 zero;
  s16 *target;

  work = HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  buffers = Ram_HeapSlots->block32;
  zero = 0;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*work) / 4),
          REG_DMA3);
  BattleEffect_BuildBuffer(from, (u16 *)buffers, (u16 *)work, 1);
  target = work->to;
  BattleEffect_BuildBuffer(to, (u16 *)buffers, (u16 *)target, 1);
  BattleEffect_InterpolateBuffers(target, work->from, work->delta, 12);
  BattleEffect_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 120;
  work->enabled = 0;
  Task_Add(Func_080d67e8, TASK_PASS_VBLANK);
}

#include "motobj.h"

/// Per-frame update: spins an object 1/8 turn a frame.
void FieldEffect_AdvanceSpinAngle(struct MotionObject *object) {
  object->angle += 0x2000;
}

// Objects: follow a watched object.

#include "actor.h"

/// Halves the distance to the watched object along x, y and z.
/// obj: object to move.
void Actor_MoveHalfwayTowardTarget(struct Actor *obj) {
  struct Actor *target = obj->watchedObject;

  obj->x += (target->x - obj->x) / 2;
  {
    s32 position = obj->y;

    position += (target->y - position) / 2;
    obj->y = position;
  }
  {
    s32 position = obj->z;

    position += (target->z - position) / 2;
    obj->z = position;
  }
}

#include "main_routine_work.h"
#include "macros.h"
#include "version.h"
#include "sprite.h"

void Actor_SetSingleChildFlagsFar(struct Actor *object, u32 flags);

#if TLA_PATCH_1
#define LEAP_SOUND 153
#else
#define LEAP_SOUND 136
#endif

struct Actor *Actor_GetById(u32 objectId);
void Event_Start(void);
void Event_End(void);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Actor_Jump(s32 id, s32 speed, s32 frames);
s32 Func_080d47b4(s32 actorId, s32 emote, s32 frames);
struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildValuesFar(struct ActorView *object, u32 value);
void *Sprite_ShareVramCacheEntry(void *sprite, void *last);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);

void Func_080d7524(s32 djinniId) {
  struct Actor *djinni;
  struct Actor *leader;
  struct Actor *copy;
  struct Actor *prev;
  void *sharedSprite;
  s32 dir;
  s32 back;
  s32 sprite;
  s32 tint;
  s32 i;
  struct Actor *copies[8];

  djinni = Actor_GetById(djinniId);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  dir = (leader->angle + 0x2000) & 0xc000;
  Event_Start();
  wait(10);
  Sound_PlayFar(173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  Sound_PlayFar(175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  wait(10);
  back = dir + 0x8000;
  Actor_ArmCallback(djinniId, back, 0);
  wait(10);
  djinni->angle = back;
  Func_080d47b4(djinniId, 0x103, 50);
  Actor_GetById(djinniId)->actionFlags &= ~1;
  djinni->motionFlags = 2;
  djinni->update = (void (*)(void))FieldEffect_AdvanceSpinAngle;
#if TLA_PATCH_1
  Sound_PlayFar(152);
#endif
  djinni->gravity = 0x4000;
  djinni->velocityY = 0x48000;
  wait(33);
  ((struct AnimationObject *)djinni->animation)->image.priority = 0;
#if TLA_PATCH_1
  Sound_PlayFar(152);
#endif
  djinni->velocityY = 0x4cccc;
  wait(35);
#if TLA_PATCH_1
  Sound_PlayFar(152);
#endif
  djinni->velocityY = 0x4e666;
  wait(38);

  djinni->update = NULL;
  djinni->angle = back;
  sprite = ((struct AnimationObject *)djinni->animation)->entries[0]->animId;
  tint = 9;
  if (sprite == 243)
    tint = 2;
  if (sprite == 245)
    tint = 10;
  // DEAD: tint is already 9 for sprite 244; the source keeps the test.
  if (sprite == 244)
    tint = 9;
  sharedSprite = NULL;
  prev = djinni;
  for (i = 0; i < 8; i++) {
    copy = Func_080200c0(sprite, djinni->x, djinni->y, djinni->z);
    // DEAD: nothing reads copies; the source keeps the store.
    copies[i] = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->motionFlags = 0;
      copy->priorityFlags = 2;
      copy->actionFlags |= 1;
      copy->update = (void (*)(void))Actor_MoveHalfwayTowardTarget;
      copy->angle = djinni->angle;
      ((struct AnimationObject *)copy->animation)->image.priority = 0;
      Animation_ApplyChildValuesFar(copy, tint);
      Actor_SetMode(copy, 0);
      Actor_SetSingleChildFlagsFar(copy, 0);
      sharedSprite = Sprite_ShareVramCacheEntry(copy->animation, sharedSprite);
      copy->watchedObject = prev;
      prev = copy;
    }
  }

  djinni->speedLimit = 0x4ccc;
  djinni->acceleration = 0x4ccc;
  Motion_SetTargetPositionFromMagnitudeAngle(djinni, 0x100000, dir + 0x8000);
  Sound_PlayFar(LEAP_SOUND);
  Actor_Jump(djinniId, 6, 5);
  wait(24);
  for (i = 0; i < 120; i++) {
    if (ABS(djinni->y - leader->y) < 0x140000)
      break;
    wait(1);
  }
  djinni->motionFlags = 0;
  djinni->velocityX = 0;
  djinni->velocityZ = 0;
  djinni->velocityY = 0;
  djinni->targetX = 0x80000000;
  djinni->targetZ = 0x80000000;
  djinni->targetY = 0x80000000;
  Actor_SetMode(djinni, 0);
  ((struct AnimationObject *)djinni->animation)->image.priority = 2;
#if TLA_PATCH_1
  djinni->gravity = 0x10000;
#endif
  Event_End();
}

#include "flag_call.h"

s32 Djinn_AddToLeastLoadedOwnerFar(s32 element, s32 djinni);
void Func_080d82e0(s32 actor);
void BattleEffect_RunMercuryDjinnCapture(s32 actor);
void Djinn_RunMarsJoinEffect(s32 actor);
void Djinn_RunJupiterJoinEffect(s32 actor);
void Func_080cb82c(void);
void Func_08038328(s32 owner, s32 element, s32 djinni);
void Func_080cb8a4(void);

extern s8 gData_02001000[];

struct EventActors {
  u8 unk00[20];
  void *actors[64];
};

/// A Djinni joins. Djinni 19 means the one recorded for the element in the
/// save (none: nothing happens). With flag 366 set it joins the current owner
/// by its flag (48 + element * 20 + djinni), otherwise the least loaded owner;
/// if it found one, the element's join effect plays at the actor (none for
/// actor -1), the Djinni is added, and the actor's event slot is cleared
/// (from patch 1; the Japanese release leaves it set).
void Func_080d7788(s32 actor, s32 element, s32 djinni) {
  s32 owner;

  if (djinni == 19) {
    s32 recorded = gData_02001000[element + 0x1950];

    if (recorded == 0)
      return;
    djinni = recorded - 1;
  }
  if (GameFlag_Test(366)) {
    owner = gMainRoutineWork.currentOwner;
    GameFlag_SetBit(element * 20 + djinni + 48);
  } else {
    owner = Djinn_AddToLeastLoadedOwnerFar(element, djinni);
  }
  if (owner < 0)
    return;
  Event_Start();
  if (actor != -1) {
    if (element == 0)
      Func_080d82e0(actor);
    else if (element == 1)
      BattleEffect_RunMercuryDjinnCapture(actor);
    else if (element == 2)
      Djinn_RunMarsJoinEffect(actor);
    else if (element == 3)
      Djinn_RunJupiterJoinEffect(actor);
  }
  Func_080cb82c();
  Func_08038328(owner, element, djinni);
  Func_080cb8a4();
#if TLA_PATCH_1
  ((struct EventActors *)Ram_HeapSlots->eventWork)->actors[actor] = NULL;
#endif
  Event_End();
}

struct ActorDescriptor {
  s16 action;
  s16 scene;
  u8 unk04[4];
  s32 x;
  s32 y;
  s32 z;
};

void Func_080d784c(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u32 raw = (u16)party->pendingDjinnEvent;
  s32 flags = party->pendingDjinnEvent & 0xf000;
  u32 scene = 0xfff;
  s32 id;

  scene &= raw;
  if (GameFlag_Test(0x109) != 0 && flags == 0) {
    flags = scene & 0x800;
    scene &= 0x7ff;
    if (scene - 300 <= 80 && party->sequenceResult > 0) {
      s32 *owner;

      for (id = 8, owner = &party->currentOwner; id <= 79; id++) {
        struct ActorDescriptor *desc = Scene_FindActorPlacement(id);
        struct Actor *obj;

        if (desc == NULL || desc->scene - 48 != scene - 300)
          continue;
        obj = ObjectTable_Get(id);
        if (obj == NULL)
          continue;
        if (flags == 0) {
          obj->terrainHeight = flags;
          obj->motionFlags = 3;
          Actor_SetPositionAndResetMotionFar(obj, desc->x, desc->y, desc->z);
        } else {
          struct Actor *selection = ObjectTable_Get(*owner);

          Actor_SetPositionAndResetMotionFar(obj, selection->x, selection->y,
                                             selection->z - (32 << 16));
        }
        Actor_SetMode(obj, 1);
      }
    }
  }
}

struct MapActor {
  s16 id;
  s16 event;
};

/// The Broken Seal's Djinn_ResolvePendingEvent, with this game's actors up to
/// 79, the plain flag calls, the event at gMainRoutineWork's 0x254 and its
/// 0x1000 kind tested like the first, falling into the common clear.
void Func_080d793c(s32 capture) {
  struct MainRoutineWork *st = &gMainRoutineWork;
  s16 *pending = &st->pendingDjinnEvent;
  s32 kind = *pending & 0xf000;
  s32 flag = *pending & 0xfff;

  if (capture == 0) {
    if (kind == 0) {
      flag &= 0x7ff;
      if (flag < 300 || flag > 380)
        return;
      if (st->sequenceResult > 0 && st->sequenceResult != 999)
        return;
      GameFlag_SetBit(flag - 172);
      *pending = kind;
      return;
    } else if (kind == 0x1000) {
      if (st->sequenceResult > 0 && st->sequenceResult != 999)
        GameFlag_SetBit(flag);
    } else
      return;
  } else if (kind == 0) {
    flag &= 0x7ff;
    if (flag >= 300 && flag <= 380) {
      flag &= 0x7ff;
      if (st->sequenceResult > 0) {
        s32 djinn = flag - 300;
        s32 element = djinn / 20;
        s32 index = djinn % 20;
        s32 actor;

        for (actor = 8; actor <= 79; actor++) {
          struct MapActor *found =
              (struct MapActor *)Scene_FindActorPlacement(actor);
          if (found != 0 && found->event - 48 == flag - 300) {
            wait(40);
            Func_080d7788(actor, element, index);
            break;
          }
        }
      }
    }
  }
  gMainRoutineWork.pendingDjinnEvent = 0;
}

#include "field_particle.h"

struct BattleFxScene {
  u8 unk00[0x50];
  struct FieldParticle slots[24];
  u8 unk710[0xe4];
};

#define BATTLE_EFFECT_SLOT 224

void BattleEffect_UpdateAllSlots(void) {
  struct BattleFxScene *scene;
  struct FieldParticle *slot;
  s32 remainingSlots;

  scene = Ram_HeapSlots->effectWork;
  remainingSlots = 0x17;
  slot = scene->slots;
  do {
    remainingSlots -= 1;
    FieldParticle_Update(slot);
    slot++;
  } while (remainingSlots >= 0);
}

void BattleEffect_InitializeSlots(void) {
  void *work;

  work = HeapSlot_Alloc(BATTLE_EFFECT_SLOT, sizeof(struct BattleFxScene));
  Dma_Fill32(0, work, sizeof(struct BattleFxScene));
  Task_Add((s32)BattleEffect_UpdateAllSlots, TASK_PASS_VBLANK);
}

void BattleEffect_ClearOwnedSlot(struct FieldParticle *);

void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct FieldParticle *slot;
  s32 i;

  Task_Remove((u32)BattleEffect_UpdateAllSlots);
  for (i = 0; i < 24; i++) {
    slot = &scene->slots[i];
    if (slot->active != 0)
      BattleEffect_ClearOwnedSlot(slot);
  }
  HeapSlot_Release(BATTLE_EFFECT_SLOT);
  wait(1);
}

void BattleEffect_AdvanceSpinAngle(struct MotionObject *object) {
  object->angle += 0x2000;
}

#include "fixed_math.h"

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

void Camera_WorldToScreen(struct EffectVector *position);
void Vector_AddPolarOffset(s32 magnitude, s32 angle,
                           struct EffectVector *position);

/// Field effect sprite callback of a Jupiter Djinni's light particles: each
/// circles its centre while it widens for 60 frames, circles 40, drifts toward
/// a point 20 pixels over the leader for 40, closes in for 60 and is freed. It
/// turns 1/32 turn a frame throughout, and its radius grows, then shrinks, by 1
/// pixel a frame. The slot's origin is the centre, in screen coordinates
/// (Camera_WorldToScreen).
void FieldEffect_RunAngledApproachPhases(struct FieldParticle *particle) {
  struct Actor *owner;
  struct EffectVector position;

  owner = Actor_GetById(gMainRoutineWork.currentOwner);
  // Each age test reads age signed (ldrsh); as particle->age == 60 and so on,
  // 54 of 122 instructions differ in tla-en.
  if (particle->state == 0) {
    particle->orbitRadius += 1;
    particle->orbitAngle += 1;
    if ((s16)particle->age == 60) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 1) {
    particle->orbitAngle += 1;
    if ((s16)particle->age == 40) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 2) {
    particle->orbitAngle += 1;
    position.x = owner->x;
    position.y = owner->y + 20 * Q16_ONE;
    position.z = owner->z;
    Camera_WorldToScreen(&position);
    particle->originX += (position.x - particle->originX) / 8;
    particle->originZ += (position.z - particle->originZ) / 8;
    if ((s16)particle->age == 40) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 3) {
    particle->orbitRadius -= 1;
    particle->orbitAngle += 1;
    if ((s16)particle->age == 60) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 4) {
    BattleEffect_ClearOwnedSlot(particle);
  }
  position.x = particle->originX;
  position.z = particle->originZ;
  Vector_AddPolarOffset(particle->orbitRadius << 16, particle->orbitAngle << 11,
                        &position);
  particle->x = position.x;
  particle->z = position.z;
}

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern u32 gVBlankCount;
extern u32 gData_080f0bfc[];

void Shop_RestoreSceneTilesFar(s32 value);
void FieldParticle_Initialize(struct FieldParticle *slot, s32 kind, s32 x,
                              s32 z);
void Animation_ApplyChildValuesToRecordFar(void *object, u32 value);
void Shop_InitializeEffectFar(void);

/// Ends a heavy impact: object id spins, leaps and vanishes in 24 sparks that
/// circle and then close on the current owner, which turns and takes mode 28.
void Djinn_RunJupiterJoinEffect(s32 id) {
  struct EffectVector position;
  struct MotionObject *object;
  struct BattleFxScene *scene;
  struct FieldParticle *slot;
  s32 scale;
  s32 count;

  object = Actor_GetById(id);
  if (object == NULL)
    return;
  BattleEffect_InitializeSlots();
  scene = Ram_HeapSlots->effectWork;
  Shop_RestoreSceneTilesFar(0x20118c);
  Sound_PlayFar(173);
  Motion_SetVarCbAndRefresh(id, 1);
  Sound_PlayFar(174);
  Motion_SetVarCbAndRefresh(id, 1);
  Sound_PlayFar(175);
  Motion_SetVarCbAndRefresh(id, 1);
  wait(20);

  // Spin, then leap high.
  Sound_PlayFar(140);
  object->update = BattleEffect_AdvanceSpinAngle;
  wait(40);
  Sound_PlayFar(153);
  Actor_Jump(id, 12, 22);

  // 24 sparks from the object's place on screen, one a frame.
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Camera_WorldToScreen(&position);
  Actor_Destroy(object);
  Sound_PlayFar(164);
  slot = scene->slots;
  count = 23;
  do {
    FieldParticle_Initialize(slot, 672, position.x, position.z);
    FieldParticle_SetCallback(slot, FieldEffect_RunAngledApproachPhases);
    FieldParticle_SetActorMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(slot->object, (Random16() * 7) >> 16);
    scale = Random16() / 3 + Q16_ONE;
    slot->scaleY = scale;
    slot->scaleX = scale;
    count--;
    wait(1);
    slot++;
  } while (count >= 0);

  // The current owner turns to 0x4000 and takes mode 28.
  wait(60);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
  wait(20);
  Actor_SetMode(Actor_GetById(gMainRoutineWork.currentOwner), 28);
  wait(40);
  Sound_PlayFar(164);
  wait(100);
  Shop_InitializeEffectFar();
  BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}

void BattleEffect_SetObjectAlternatingWords(u8 *object) {
  u32 *vblankCount = &gVBlankCount;
  u32 *table;
  u32 index;
  u32 value;

  table = gData_080f0bfc;
  index = (*vblankCount >> 2) & 1;
  value = index[table];
  ((struct MotionObject *)object)->scaleX = value;
  ((struct MotionObject *)object)->scaleY = value;
}

void BattleEffect_SpinUpAngle(void *obj) {
  u32 step;

  step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;
  FIELD_AT_OFFSET(obj, u16 *, 6) =
      (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + step + 0x1000);
  if (step < 0x1000U) {
    FIELD_AT_OFFSET(obj, s16 *, 0x64) =
        (s16)((u16)FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1);
  }
}

/// Spins an object 1/8 turn and shrinks it by 0x1c0 a frame, destroying it once
/// its scale falls below 0x3000.
void BattleEffect_ShrinkObjectAndDestroySlow(void *obj) {
  s32 step = 0xFFFFFE40;
  s32 scaleY;
  s32 scale;
  s32 angle;

  scaleY = ((struct MotionObject *)obj)->scaleY + step;
  scale = ((struct MotionObject *)obj)->scaleX;
  ((struct MotionObject *)obj)->scaleY = scaleY;
  angle = ((struct MotionObject *)obj)->angle;
  scale += step;
  ((struct MotionObject *)obj)->angle = angle + 0x2000;
  ((struct MotionObject *)obj)->scaleX = scale;
  if (scale < 0x3000)
    Actor_Destroy(obj);
}

#include "objdisp.h"

struct BattleFxScene;

#define Actor_SetChildValueUnlessFifteenFar                                    \
  ((void (*)(s32, s32))Animation_ApplyChildValuesToRecordFar)
#define Actor_ReleaseFar Actor_Destroy
#define gBattleEffect_PulseScales gData_080f0bfc
#define gEffectWork Ram_HeapSlots->effectWork

struct EffectOwner {
  u8 unk00[8];
  struct EffectVector position;
  u8 unk14[0x3c];
  struct AnimationObject *sprite;
};

void Map_SetScreenShakeFar(s32, s32, s32);

struct OrbitPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct DescendingOrbitObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[80];
  u16 timer;
  s16 angle;
};

struct SpreadPosition {
  s32 x;
  s32 y;
  s32 z;
};

extern const s32 gBattleEffect_SharedParticleScript[];

struct CaptureObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 baseY;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[8];
  s32 gravity;
  u8 unk2c[4];
  s32 speed;
  u8 unk34[33];
  u8 mode;
  u8 unk56[5];
  u8 visible;
  u8 unk5c[2];
  u16 lifetime;
  u8 unk60[4];
  u16 timer;
  u16 angle;
  u8 unk68[4];
  void *callback;
};

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);

struct LinkedCaptureObject {
  u8 padding000[8];
  s32 x;
  s32 y;
  s32 z;
  u8 padding014[84];
  struct LinkedCaptureObject *target;
};

void BattleEffect_UpdatePhasedRadialParticle(struct FieldParticle *effect) {
  struct EffectOwner *owner;
  struct EffectVector position;
  s32 state;

  owner = (struct EffectOwner *)Actor_GetById(gMainRoutineWork.currentOwner);
  state = effect->state;

  if (state == 0) {
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = effect->x;
    position.z = effect->z;
    Vector_AddPolarOffset(0x780000,
                          ((Random16() * 3 << 11) >> 16) -
                              ((Random16() * 3 << 11) >> 16) + 0xc000,
                          &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = 0x50000;
    effect->maxSpeed = 0x50000;
    effect->limitTurnStep = state;
    effect->state++;

    ((struct AnimationObject *)effect->object)->image.priority =
        owner->sprite->image.priority;
    effect->flags = 0;
    effect->age = 0;
    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(134);
  } else if (state == 1) {
    if ((s16)effect->age == 3) {
      ((struct AnimationObject *)effect->object)->image.priority = 0;
      effect->flags = 4;
    }
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 2) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      effect->originX = effect->x;
      effect->originZ = effect->z;
      ((struct AnimationObject *)effect->object)->image.priority = 0;
      effect->flags = 4;
      effect->render = 0;
      effect->state++;
      effect->callbackDelay = 40;
    }
  } else if (state == 3) {
    effect->render = 1;
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = owner->position.x;
    position.y = owner->position.y + 0x140000;
    position.z = owner->position.z;
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(0x40000, Random16(), &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->state++;
    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(145);
  } else if (state == 4) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 5) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      BattleEffect_ClearOwnedSlot(effect);
  }
}

void BattleEffect_ShrinkObjectAndDestroyFast(void *obj);

void BattleEffect_UpdateDescendingOrbitObject(
    struct DescendingOrbitObject *arg);

void BattleEffect_UpdateRadialSpread(struct FieldParticle *effect);

void BattleEffect_RunVenusDjinnCapture(s32 arg);

void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect);
