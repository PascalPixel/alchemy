#include "types.h"
#include "task.h"
#include "ram_buffer.h"
#include "field_particle.h"

void Field_StartPaletteTransition(s32 frames);

/// Fades the palettes back to the saved ones over the given frames.
void BattleEffect_FadeToSavedPalettes(s32 frames) {
  Field_StartPaletteTransition(frames);
}

void BattleEffect_PrepareBufferInterpolation(void) {
  Field_StartPaletteTransition(8);
}

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

s32 Render_ProjectPoint(const void *position, s32 *result);

void Camera_WorldToScreen(struct EffectVector *position) {
  struct HeapSlots *slots = Ram_HeapSlots;

  if (*(s8 *)((u8 *)slots->eventWork + 0x18a) == 3) {
    s32 result[3];

    Render_ProjectPoint(position, result);
    position->x = result[0] << 16;
    position->z = result[1] << 16;
    position->y = 0;
  } else {
    u8 *state = (u8 *)slots->mapWork;
    s32 x = *(s32 *)(state + 228) & 0xffff0000;
    s32 z = *(s32 *)(state + 232) & 0xffff0000;

    position->x -= x;
    position->z -= position->y;
    position->z -= z;
    position->y = 0;
  }
}

struct BattleFxScene {
  u8 unk00[0x50];
  struct FieldParticle slots[24];
  u8 unk710[0xe4];
};

/// A task: updates the scene's 24 effect slots.
void BattleEffect_UpdateAllEffectSlots(void) {
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

#include "map.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "iwram_call.h"
#include "battle_unit.h"

struct EffectWork {
  u8 unk000[0x1c];
  u16 action;
  s16 animation;
  s8 enabled;
  u8 mode;
  s8 visible;
  s8 active;
  u8 unk024[0x1d];
  u8 lit;
  u16 freeBlocks;
  s32 savedShakeX;
  s32 savedShakeY;
  s32 savedShakeDecay;
  u8 unk050[0x6c2];
  s8 messageMode;
  u8 unk713;
  u8 unk714[0xb0];
  s16 unk7c4;
  s16 unk7c6;
  u8 unk7c8[0x2a];
  s16 unk7f2;
};

struct LoadEventWork {
  u8 unk000[0xca5];
  u8 unkca5;
  u8 unkca6[7];
  u8 unkcad;
  u8 unkcae[8];
  s8 unkcb6;
  u8 unkcb7[0x79];
  u8 unkd30;
  s8 target;
  s8 targetY;
  s8 targetZ;
  u8 unkd34[4];
  u8 unkd38;
};

void *HeapSlot_Alloc(s32 slot, s32 size);
struct BattleAct *BattleAction_Get(s32 actionId);
s32 ResourceTable_CountFreeBlocks(void);
void BattleEffect_SetupObjectPair(s32 userId, s32 targetId);
void BattleEffect_StartArcEffect(void);

#define HEAP_SLOT_FX 224

void BattleEffect_LoadActionEffectResources(s32 abilityId, s32 mode) {
  struct LoadEventWork *event = Ram_HeapSlots->eventWork;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct EffectWork *work;
  s32 running;

  if (event->unkcb6 == 0) {
    work = HeapSlot_Alloc(HEAP_SLOT_FX, sizeof(*work));
    // An empty once-only loop keeps the work's copy at once after the
    // allocation; without it, 4 of 239 instructions differ in tla-en.
    do {
    } while (0);
    Iwram_FillWords(work, sizeof(*work), 0);
  } else {
    work = Ram_HeapSlots->effectWork;
  }
  work->action = abilityId;
  work->animation = BattleAction_Get(abilityId)->unk04[2];
  running = event->unkcb6;
  if (running != 0)
    return;
  work->freeBlocks = 512 - ResourceTable_CountFreeBlocks();
  work->mode = mode;
  work->visible = 1;
  work->enabled = 1;
  work->active = 1;
  work->messageMode = 1;
  work->unk713 = 0;
  work->unk7c4 = running;
  work->unk7c6 = -1;
  work->unk7f2 = running;
  work->savedShakeX = map->shakeX;
  work->savedShakeY = map->shakeY;
  work->savedShakeDecay = map->shakeDecay;
  if (gMainRoutineWork.map == (s32)&SceneId_Ov673Room5)
    work->lit = 1;
  if (gMainRoutineWork.map == (s32)&SceneId_Ov68fRoom1)
    *((u8 *)work + 0x41) = 1;
  if (gMainRoutineWork.map == (s32)&SceneId_Ov68fRoom2)
    *((u8 *)work + 0x41) = 1;
  if (gMainRoutineWork.map == (s32)&SceneId_Ov68fRoom7)
    *((u8 *)work + 0x41) = 1;
  if (gMainRoutineWork.map == (s32)&SceneId_Ov6a4)
    *((u8 *)work + 0x41) = 1;
  if (gMainRoutineWork.map == (s32)&SceneId_Ov6a5)
    *((u8 *)work + 0x41) = 1;
  if (gMainRoutineWork.map == (s32)&SceneId_Ov6A6Room1)
    *((u8 *)work + 0x41) = 1;
  BattleEffect_SetupObjectPair(gMainRoutineWork.currentOwner, -1);
  if (work->animation != 8)
    event->unkca5 = 0;
  if (work->animation != 28)
    event->unkcad = 0;
  event->target = 0;
  event->targetY = 0;
  event->targetZ = 0;
  event->unkd38 = 0;
  event->unkd30 = 255;
  BattleEffect_StartArcEffect();
  Task_Add(BattleEffect_UpdateAllEffectSlots, 0x480);
}

#include "sprite.h"
#include "actor.h"

struct PairScene {
  u16 angle;  ///< Four directions.
  u16 angle8; ///< Eight directions.
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u8 unk1c[0x1c];
  void *savedCallback;
  const s32 *savedScript;
  u8 savedPalette;
  u8 unk41[0x6d7];
  u8 savedTerrain;
  u8 savedPriority;
};

void Vector_AddPolarOffset(s32 magnitude, s32 angle, void *position);

void BattleEffect_SetupObjectPair(s32 userId, s32 targetId) {
  struct Actor *user;
  struct Actor *target;
  struct PairScene *scene;

  scene = Ram_HeapSlots->effectWork;
  scene->firstObjectId = userId;
  user = ObjectTable_Get((s16)userId);
  scene->secondObjectId = targetId;
  scene->mainObject = user;
  target = ObjectTable_Get((s16)targetId);
  scene->angle = (user->facing + 0x2000) & 0xc000; // Round to four directions.
  scene->angle8 = (user->facing + 0x1000) & 0xe000;
  scene->child = target;
  if (user != NULL) {
    u8 terrain;

    terrain = user->priorityFlags;

    scene->savedTerrain = terrain;
    scene->savedPriority = (u32)(((u8 *)user->sprite)[9] << 28) >> 30;
  }
  if (target != NULL) {
    // Save target motion and colors for later restoration.
    scene->savedCallback = target->update;
    scene->savedScript = target->script;
    scene->savedPalette =
        ((struct AnimationObject *)target->sprite)->entries[0]->param;
    scene->x = target->x;
    scene->z = target->z;
    scene->y = target->y;
  } else {
    scene->x = user->x;
    scene->z = user->z;
    scene->y = user->y;
    Vector_AddPolarOffset(0x100000, scene->angle, &scene->x);
  }
}

struct FinishWork {
  u8 unk000[16];
  struct Actor *object;
  u8 unk014[8];
  s16 quantity;
  u8 unk01e[4];
  s8 sound;
  s8 showQuantity;
  u8 unk024[0x6ee];
  s8 messageRow;
  u8 messageShown;
};

extern char MsgBlank2;

s32 GameFlag_Test(s32 flag);
void BattleEffect_SetObjectUpdateMode(void *object, s32 mode);
void Actor_SetOthersPaused(struct Actor *excludedObject, s32 groupMode,
                           s32 action);
void MessageArgument_PushFar(s32, s32);
void FieldMessage_ShowFar(s32 message, s32 flags);
void BattleEffect_StopMainActor(void);

void Func_080dc6d8(void) {
  struct FinishWork *work = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object = work->object;
  s32 quantity = work->quantity;
  s32 zero;
  s8 sound;

  Actor_SetMode(object, 20);
  // The stores in a once-only block keep them all ahead of the sound's read;
  // as plain statements, 8 of 109 instructions differ in tla-en.
  do {
    object->targetX = object->x;
    object->velocityX = 0;
    object->targetY = object->y;
    object->velocityY = 0;
    object->targetZ = object->z;
    object->velocityZ = 0;
  } while (0);
  sound = work->sound;
  /* FAKEMATCH(barrier): the zero set through an empty asm after the sound's
   * read, so its copy to r8 comes after that read; as zero = 0, 4 of 109
   * instructions differ in tla-en. */
  asm("" : "=r"(zero) : "0"(0));
  if (sound != 0) {
    Sound_PlayFar(212);
    BattleEffect_SetObjectUpdateMode(object, 1);
  }
  if (work->showQuantity != 0) {
    work->messageShown = 1;
    Actor_SetOthersPaused(object, 1, 0);
    MessageArgument_PushFar(quantity, 4);
    FieldMessage_ShowFar((s32)&MsgBlank2, work->messageRow);
    Actor_SetOthersPaused(object, 0, 16);
    work->messageShown = zero;
  }
  if (GameFlag_Test(320)) {
    if (work->sound != 0)
      BattleEffect_SetObjectUpdateMode(object, 2);
    Actor_SetMode(object, 21);
  } else {
    BattleEffect_StopMainActor();
  }
  event[0xcb7] = 1;
}
