// Draft, differ 64 of 308 (2026-10-18): Func_080e09c0, the effect that
// lifts an object (363) over the caster, drops it, and, unless the scene is
// enabled, runs Func_080e0618's five phases as a frame callback until they
// finish (999). Left: the native copies the work block to r10 only after it
// has read the event and scene slots, so the event takes r2; this copies it
// first and the event takes r0, which moves the registers of the opening and
// the object copies after it. The sprite flags in the order priority,
// palette, palette256 measure best (66-96 otherwise). The two resources are
// 0x1ea and 0x1eb in directory.s (FieldFx_SparkleFade,
// FieldFx_StarSparkleFade), still unnamed there.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "fx_scene.h"
#include "effect_sprite.h"

struct ScatterParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct ScatterWork {
  u8 tiles[0x1000];
  struct EffectSprite sprites[64];
  struct ScatterParticle particles[64];
  struct ObjectRuntime *object;
  s16 phase;
  s16 timer;
  s16 spawnCount;
  s16 next;
  u16 tileA;
  u16 tileB;
  s16 flash;
  s16 hit;
};

struct ScatterEvent {
  u8 unk000[0xd31];
  s8 target;
  s8 targetY;
  s8 targetZ;
};

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleEffect_InitializeSharedScene(void);
void BattleFx_PrepareBufferInterpolation(void);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Object_Destroy(struct ObjectRuntime *object);
void *Resource_GetTableEntry(s32 id);
void Resource_DecodeType01(void *resource, void *destination);
s32 Resource_FindFreeEntry(void);
void Resource_ResetEntry(s32 slot);
s32 VramCache_Load(s32 slot, s32 size, void *data);
u32 Random16(void);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, void *position);
void WaitFrames(s32 frames);
void Func_080dbed0(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
struct ObjectRuntime *BattleFx_SpawnScatterParticle(s32 x, s32 y, s32 z);
void Func_080d92d4(s32 target, s32 y, s32 z);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallback(void (*callback)(void));
void Func_080e0618(void);

extern char ResourceId_SparkleFade;
extern char ResourceId_StarSparkleFade;

void Func_080e09c0(void) {
  struct ScatterWork *work = Runtime_AllocateHeapBlock(92, 0x2114);
  struct ScatterEvent *event = Ram_HeapSlots->eventWork;
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct ObjectRuntime *caster = scene->mainObject;
  struct ObjectRuntime *object;
  struct EffectSprite *sprite;
  struct ScatterParticle *particle;
  s32 slotA;
  s32 slotB;
  s32 tileA;
  s32 i;

  if (scene->child != 0) {
    scene->y += 0x40000;
    scene->z += 0x40000;
  }
  BattleEffect_InitializeSharedScene();
  object = Object_Spawn(363, 0, 0, 0);
  if (object != 0) {
    object->angle = caster->angle;
    object->motionFlags = 0;
    if (caster->priorityFlags & 0x20)
      object->priorityFlags |= 0x20;
    Object_SetMode(object, 0);
    Audio_PlayCue(138);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_SparkleFade),
                          work);
    slotA = Resource_FindFreeEntry();
    tileA = VramCache_Load(slotA, 0x100, work);
    work->tileA = tileA;
    Resource_DecodeType01(
        Resource_GetTableEntry((s32)&ResourceId_StarSparkleFade), work);
    slotB = Resource_FindFreeEntry();
    work->tileB = VramCache_Load(slotB, 0x100, work);

    particle = work->particles;
    sprite = work->sprites;
    for (i = 63; i >= 0; i--) {
      Func_080eaf98(sprite, 4, 4, 0, tileA);
      sprite->depth = 250;
      sprite->oam.priority = 0;
      sprite->oam.palette = 0;
      sprite->oam.palette256 = 1;
      particle->x = object->x;
      particle->y = object->y;
      particle->z = object->z;
      Vector_AddPolarOffset(0xc0000, Random16(), particle);
      particle->timer = -1;
      sprite++;
      particle++;
    }

    Func_080dbed0(object, scene->x, scene->y + 0x200000, scene->z);
    Object_SetMode(object, 1);
    WaitFrames(10);
    for (i = 15; i >= 0; i--) {
      object->y += -0x20000;
      WaitFrames(1);
    }
    Audio_PlayCue(132);
    if (scene->enabled != 0) {
      for (i = 15; i >= 0; i--)
        BattleFx_SpawnScatterParticle(object->x, object->y, object->z);
      WaitFrames(1);
    } else {
      WaitFrames(6);
      work->object = object;
      work->phase = 0;
      work->timer = 0;
      work->next = 0;
      work->spawnCount = 1;
      work->hit = 0;
      Scheduler_AddOrUpdateCallback(Func_080e0618, 0x480);
      while (work->phase != 999) {
        if (work->flash == 1) {
          Audio_PlayCue(0x200);
          Func_080d92d4(event->target, event->targetY, event->targetZ);
          work->hit = 1;
          Audio_PlayCue(395);
        }
        WaitFrames(1);
      }
      Scheduler_RemoveCallback(Func_080e0618);
    }
    Resource_ResetEntry(slotA);
    Resource_ResetEntry(slotB);
    Object_Destroy(object);
  }
  BattleFx_PrepareBufferInterpolation();
  Runtime_ReleaseHeapBlock(92);
}
