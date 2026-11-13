#include "types.h"
#include "task.h"
#include "ram_buffer.h"
#include "field_particle.h"
#include "battle.h"
#include "battle_system.h"
#include "event.h"
#include "field.h"
#include "flag_call.h"
#include "gamepak/far_call/window.h"
#include "heap_release.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "text/msg_ids.h"

/// Fades the palettes back to the saved ones over the given frames.
void BattleEffect_FadeToSavedPalettes(s32 frames) {
  Field_StartPaletteTransition(frames);
}

void FieldPsynergy_FadeFromCastColor(void) { Field_StartPaletteTransition(8); }

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

/* VIEW(scene): tag struct BattleFxScene as struct BattleFxScene__scene;
 * field_psynergy_work.h has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/scene.c:57: dereferencing pointer to
 * incomplete type. */
struct BattleFxScene__scene {
  u8 unk00[0x50];
  struct FieldParticle slots[24];
  u8 unk710[0xe4];
};

/// A task: updates the scene's 24 effect slots.
void BattleEffect_UpdateAllEffectSlots(void) {
  struct BattleFxScene__scene *scene;
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
    Iwram_FillWordsEntry(work, sizeof(*work), 0);
  } else {
    work = Ram_HeapSlots->effectWork;
  }
  work->action = abilityId;
  work->animation = Ability_Get(abilityId)->unk04[2];
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
  FieldPsynergy_SetupActorPair(gMainRoutineWork.currentOwner, -1);
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

void FieldPsynergy_SetupActorPair(s32 userId, s32 targetId) {
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

#include "frame.h"
#include "actor_view.h"

void BattleEffect_StopMainActor(void) {
  struct EffectSceneWork *work = Ram_HeapSlots->effectWork;

  BattleEffect_SetObjectUpdateMode(work->mainObject, 0);
  wait(1);
}

#include "field_sprite.h"

void BattleEffect_CleanupSceneObjects(void) {
  struct CleanupWork *work = Ram_HeapSlots->effectWork;
  struct CleanupEventWork *event = Ram_HeapSlots->eventWork;
  s8 *block = Ram_HeapSlots->block32;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *owner = work->owner;
  s32 i;
  s16 first;

  for (i = 0; i < 24; i++) {
    struct FieldParticle *slot = &work->slots[i];
    if (slot->active)
      BattleEffect_ClearOwnedSlot(slot);
  }
  // A local that holds one constant: it makes the first test cmp #17; blt as
  // the native (with the literal, 4 of 171 differ in tla-en).
  first = 17;
  if (work->animation >= first &&
      (work->animation <= 27 || work->animation == 30))
    Func_080dc954();
  if (event->unkcb6 != 0)
    return;
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  if (event->mode == 3 && block != NULL) {
    i = 0;
    if (block[0x2a01] != 0) {
      do {
        wait(1);
        if (++i > 89)
          break;
      } while (block[0x2a01] != 0);
    }
  }
  event->unkcb7 = 0;
  Func_080dbb40();
  Task_Remove(BattleEffect_UpdateAllEffectSlots);
  map->shakeX = work->savedShakeX;
  map->shakeY = work->savedShakeY;
  map->shakeDecay = work->savedShakeDecay;
  if (owner != NULL) {
    owner->priorityFlags = work->ownerByte;
    ((struct FieldSprite *)owner->sprite)->priority = work->layer;
  }
  if (work->animation != 8)
    event->unkca5 = 1;
  if (work->animation != 28)
    event->unkcad = 1;
  Event_NoOp();
  HeapSlot_Release(224);
}

/// When the event work's mode byte at 0x18a is 3, copies it to the map
/// work's byte at 0x970.
void Func_080dc954(void) {
  struct HeapSlots *slots = Ram_HeapSlots;
  s32 mode = *(s8 *)((u8 *)slots->eventWork + 0x18a);

  if (mode == 3)
    *((u8 *)slots->mapWork + 0x970) = mode;
}

#include "heap_release.h"
#include "dma.h"

#define WAVE_FX_SLOT 88

/* VIEW(scene): tag struct WaveFxWork as struct WaveFxWork__scene;
 * module/field/event_commands.h has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/scene.c:343: structure has no member named
 * `id'. */
struct WaveFxWork__scene {
  u8 unk000[0x28b];
  s8 red;
  s8 green;
  s8 blue;
  u16 phase;
  u16 id;
  u16 value;
  u8 steps;
  u8 unk295;
  u8 unk296[2];
};

/// Starts the wave effect's work in heap slot WAVE_FX_SLOT: a phase taken
/// from the frame count, id and value, then the shared scene, the first hue
/// step and its colour fade, with FieldPsynergy_UpdateMindReadWave run each
/// frame.
void FieldPsynergy_StartMindReadWave(s32 id, s32 value) {
  struct WaveFxWork__scene *work;

  work = HeapSlot_AllocEwram(WAVE_FX_SLOT, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  work->phase = (gFrameCount * 2) % 360;
  work->id = id;
  work->value = value;
  work->steps = 8;
  work->unk295 = 0;
  FieldPsynergy_FadeToCastColor();
  FieldEffect_AdvanceHueCycle();
  BattleEffect_ApplyColorToTargetBuffer(
      (work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
  PaletteFade_StartInterpolation(8);
  FieldPsynergy_GreyMindReadPalette();
  Task_Add((TaskFunc)FieldPsynergy_UpdateMindReadWave, TASK_PASS_VBLANK);
}

void FieldPsynergy_StopMindReadWave(void) {
  u8 *work = Ram_HeapSlots->unk4c[3];

  FieldPsynergy_RestoreMindReadPalette();
  Task_Remove((TaskFunc)FieldPsynergy_UpdateMindReadWave);
  Actor_SetSpritePalette(Actor_GetById(*(u16 *)(work + 0x290)), 1);
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(WAVE_FX_SLOT);
}

#define BG_PLTT_COLOR(bank, color)                                             \
  (*(volatile u16 *)(0x05000000 + ((bank) * 16 + (color)) * 2))
#define GREY28 0x739c

/// Gray BG palette 15 colors 1 and 3-12, then start line scrolling.
void FieldPsynergy_GreyMindReadPalette(void) {
  volatile u16 *p;

  Ram_HeapSlots->windowTiles[4] = 1;
  p = &BG_PLTT_COLOR(15, 1);
  *p = GREY28;
  p += 2;
  *p = GREY28;
  p += 8;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p -= 8;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  Task_Add((TaskFunc)FieldEffect_ArmBg0HBlankDma, TASK_PASS_FRAME - 20);
}

void Func_08038348(s32 first, s32 second);

/// Stops line scrolling and restores background palette 15.
void FieldPsynergy_RestoreMindReadPalette(void) {
  u8 *work = Ram_HeapSlots->windowTiles;

  Task_Remove((TaskFunc)FieldEffect_ArmBg0HBlankDma);
  BG_PLTT_COLOR(15, 1) = 0x7fff;  // White.
  BG_PLTT_COLOR(15, 3) = 0;       // Black.
  BG_PLTT_COLOR(15, 11) = 0x294a; // Gray (10).
  BG_PLTT_COLOR(15, 12) = 0x5294; // Gray (20).
  Func_08038348(gMainRoutineWork.unk220[5], gMainRoutineWork.unk220[6]);
  work[4] = 0;
}
