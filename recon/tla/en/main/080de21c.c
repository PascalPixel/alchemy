// Near miss, differ 32 of 364 (2026-10-15): ⚓️'s form of ☀️'s
// RunBattleEffect04. Every instruction and register is the native's; the
// 32 differ only in order: in the five position copies the next field's
// load and the offset constant's build are swapped, the native builds the
// growth loop's two limits after it loads the scale, and the copy loop's
// start (sp + 24) and the last loop's r7 are set at other places.
// effect04_entry.c's Func_080de214 declares it taking the effect argument.
// Belongs between Func_080de214 and update_orbiting_particle_main.c's
// BattleFx_UpdateOrbitingParticleMain.
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "effect_slot.h"

struct PendingEvent;

struct ResourceEntry {
  u8 unk00[0x10];
  u8 vramBlock;
};

struct Effect04Scene {
  u16 angle;
  u8 unk02[2];
  s32 x;
  s32 y;
  s32 z;
  struct ObjectRuntime *mainObject;
  struct ObjectRuntime *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u8 unk1c[4];
  s8 enabled;
  u8 unk21[0x13];
  s8 childOption;
  s8 childMode;
  u8 unk36[0x1a];
  struct EffectSlot slots[24];
};

struct Effect04Vector {
  s32 x;
  s32 y;
  s32 z;
};

void BattleEffect_InitializeSharedScene(void);
void BattleFx_PrepareBufferInterpolation(void);
void Camera_WorldToScreen(struct Effect04Vector *position);
void EffectSlot_Initialize(struct EffectSlot *slot, s32 kind, s32 x, s32 z);
void Animation_ApplyChildValuesToRecordFar(void *record, u32 value);
void Animation_ApplyChildValuesFar(void *object, u32 value);
void Vector_AddPolarOffset(s32 magnitude, s32 angle,
                           struct Effect04Vector *position);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
struct ResourceEntry *Object_ReplaceResourceEntry(void *sprite,
                                                  struct ResourceEntry *last);
void ObjectDispatch_InitializeFar(void *object, const void *script);
s32 Func_08020210(struct ObjectRuntime *object,
                  struct Effect04Vector *position);
s32 ScriptObject_CheckOverlapFar(struct ObjectRuntime *object,
                                 struct Effect04Vector *position);
s32 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector);
s32 EventRuntime_RunPendingEvent(struct PendingEvent *event, s32 mode,
                                 s32 target);
s32 Resource_ResetEntry(u32 entry);
void Func_080de060(struct EffectSlot *slot);

extern const u8 gData_080f0ef8[];
extern const u8 gData_080f0e58[];

// Twelve sparks burst over the user, two frames apart; a ball grows in
// front of it and flies off with three trailing copies, and the target is
// pushed 16 pixels away unless something blocks it.
void Func_080de21c(void) {
  struct ObjectRuntime *copies[4];
  struct Effect04Vector pos;
  struct Effect04Scene *scene;
  struct ObjectRuntime *target;
  struct ObjectRuntime *source;
  struct ObjectRuntime *object;
  struct ObjectRuntime *copy;
  struct EffectSlot *slot;
  struct ResourceEntry *resource;
  struct PendingEvent *event;
  s32 scale;
  s32 i;
  u8 marker;

  scene = Ram_HeapSlots->effectWork;
  target = scene->child;
  BattleEffect_InitializeSharedScene();
  Audio_PlayCue(130);
  slot = scene->slots;

  for (i = 0; i <= 11; i++) {
    source = scene->mainObject;
    pos.x = source->x;
    pos.y = source->y + 0x100000;
    pos.z = source->z;
    Camera_WorldToScreen(&pos);
    EffectSlot_Initialize(slot, 0x2a0, pos.x, pos.z);
    EffectSlot_SetCallback(slot, Func_080de060);
    EffectSlot_SetObjectMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(slot->object, 9);
    slot->scaleY = 0xb333;
    slot->scaleX = 0xb333;
    WaitFrames(2);
    slot++;
  }

  source = scene->mainObject;
  pos.x = source->x;
  pos.y = source->y + 0x100000;
  pos.z = source->z;
  Vector_AddPolarOffset(0x80000, scene->angle, &pos);
  object = Object_Spawn(278, pos.x, pos.y, pos.z);
  if (object == NULL) {
    BattleFx_PrepareBufferInterpolation();
    return;
  }
  object->scaleY = 0x4000;
  object->scaleX = 0x4000;
  object->angle = scene->angle;
  object->speedLimit = 0x40000;
  object->acceleration = 0x40000;
  object->motionFlags = 0;
  Object_SetMode(object, 5);
  Animation_ApplyChildValuesFar(object, 3);
  scale = object->scaleX;
  if (scale < 0x10000) {
    do {
      scale += 0x500;
      object->scaleY = scale;
      object->scaleX = scale;
      WaitFrames(1);
      scale = object->scaleX;
    } while (scale <= 0xffff);
  }
  WaitFrames(3);

  resource = NULL;
  for (i = 2; i >= 0; i--) {
    copy = copies[i] = Object_Spawn(278, object->x, object->y, object->z);
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->angle = scene->angle;
      copy->speedLimit = 0x40000;
      copy->acceleration = 0x40000;
      copy->motionFlags = 0;
      Object_SetMode(copy, 5);
      Animation_ApplyChildValuesFar(copy, 2);
      resource = Object_ReplaceResourceEntry(copy->animation, resource);
    }
  }
  marker = resource->vramBlock;

  if (scene->enabled != 0) {
    source = scene->mainObject;
    pos.x = source->x;
    pos.y = source->y + 0x100000;
    pos.z = source->z;
    Vector_AddPolarOffset(0x380000, scene->angle, &pos);
  } else {
    pos.x = scene->x;
    pos.y = scene->y + 0x100000;
    pos.z = scene->z;
  }
  Object_SetPosition(object, pos.x, pos.y, pos.z);
  ObjectDispatch_InitializeFar(object, gData_080f0ef8);
  for (i = 0; i < 3; i++) {
    copy = copies[i];
    if (copy != NULL) {
      WaitFrames(3);
      Object_SetPosition(copy, pos.x, pos.y, pos.z);
      ObjectDispatch_InitializeFar(copy, gData_080f0e58);
    }
  }

  for (i = 0; i <= 59 && object->script != NULL; i++)
    WaitFrames(1);

  if (target != NULL && scene->childMode == 0) {
    if (scene->childOption != 0)
      target->velocityY = 0x80000;
    pos.x = target->x;
    pos.y = target->y;
    pos.z = target->z;
    Vector_AddPolarOffset(0x100000, scene->angle, &pos);
    if (Func_08020210(target, &pos) == 0 &&
        ScriptObject_CheckOverlapFar(target, &pos) == 0) {
      target->acceleration = 0x10000;
      target->speedLimit = 0x10000;
      Object_SetPosition(target, pos.x, pos.y, pos.z);
    }
  }

  event = Func_080ce458(0x50000005, 4, Func_080ce31c(4));
  if (event != NULL)
    EventRuntime_RunPendingEvent(event, scene->firstObjectId,
                                 scene->secondObjectId);
  WaitFrames(10);
  BattleFx_PrepareBufferInterpolation();
  WaitFrames(20);
  if (marker != 96)
    Resource_ResetEntry(marker);
}
