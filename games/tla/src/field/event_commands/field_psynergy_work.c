/// @file
/// The effect scene a field Psynergy runs in: fading to and from its cast
/// colour, its effect slots and resources, the actor pair it works on, and
/// Mind Read's wave and palettes.
// Part of event_commands.c.

#include "../event_commands_internal.h"
#include "fixed_math.h"
#include "sounds.h"

#include "types.h"
#include "system/task.h"
#include "system/ram_buffer.h"
#include "field/field_particle.h"
#include "battle/battle.h"
#include "battle/battle_system.h"
#include "field/event.h"
#include "field/field.h"
#include "field/perspective.h"
#include "flag_call.h"
#include "gamepak/far_call/window.h"
#include "system/heap_release.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "text/msg_ids.h"
#include "module/system/system.h"
#include "map.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "system/iwram_call.h"
#include "battle/battle_unit.h"
#include "field/sprite.h"
#include "actor.h"
#include "system/frame.h"
#include "actor_view.h"
#include "field_sprite.h"
#include "system/dma.h"

/// Fades the palettes back to the saved ones over the given frames.
void BattleEffect_FadeToSavedPalettes(s32 frames) {
  Field_StartPaletteTransition(frames);
}

/// Fades the palettes back from a cast's colours to the saved ones over 8
/// frames; while Reveal is active the BG half fades to Reveal's grey
/// instead (PALETTE_TINT_GRAYSCALE).
void FieldPsynergy_FadeFromCastColor(void) { Field_StartPaletteTransition(8); }

/// Converts a world position (16.16 pixels) to the screen: x and z receive the
/// screen's x and y and y becomes 0.
void Camera_WorldToScreen(struct EffectVector *position) {
  struct HeapSlots *slots = Ram_HeapSlots;

  if (((struct FieldWork *)slots->fieldWork)->mode == 3) {
    s32 result[3];

    Render_ProjectPoint(position, result);
    position->x = result[0] << 16;
    position->z = result[1] << 16;
    position->y = 0;
  } else {
    u8 *state = (u8 *)slots->mapGrid;
    s32 x = *(s32 *)(state + 228) & 0xffff0000;
    s32 z = *(s32 *)(state + 232) & 0xffff0000;

    position->x -= x;
    position->z -= position->y;
    position->z -= z;
    position->y = 0;
  }
}

/// A task: updates the scene's 24 effect slots.
void BattleEffect_UpdateAllEffectSlots(void) {
  struct FieldEffectWork *scene;
  struct FieldParticle *slot;
  s32 remainingSlots;

  scene = Ram_HeapSlots->fieldEffectWork;
  remainingSlots = 0x17;
  slot = scene->particles;
  do {
    remainingSlots -= 1;
    FieldParticle_Update(slot);
    slot++;
  } while (remainingSlots >= 0);
}

/// Prepares the work of the field Psynergy for ability (heap slot
/// fieldEffectWork): its kind, the free VRAM it starts with, the map's shake
/// to restore, and whether the place is lit. While another field Psynergy is
/// resolving, only the ability and kind are set.
void BattleEffect_LoadActionEffectResources(s32 abilityId, s32 mode) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct FieldEffectWork *work;
  s32 running;

  if (event->psynergyActing == 0) {
    work = HeapSlot_Alloc(HEAP_SLOT_FIELD_EFFECT, sizeof(*work));
    /* FAKEMATCH(dead): an empty once-only loop. */
    do {
    } while (0);
    Iwram_FillWordsEntry(work, sizeof(*work), 0);
  } else {
    work = Ram_HeapSlots->fieldEffectWork;
  }
  work->action = abilityId;
  work->fieldPsynergy = Ability_Get(abilityId)->fieldEffect;
  running = event->psynergyActing;
  if (running != 0)
    return;
  work->freeBlocks = 512 - ResourceTable_CountFreeBlocks();
  work->mode = mode;
  work->visible = 1;
  work->enabled = 1;
  work->active = 1;
  work->messageMode = 1;
  work->messageShown = 0;
  work->stage = running;
  work->frame = -1;
  work->spriteActive = running;
  work->savedShakeX = map->shakeX;
  work->savedShakeY = map->shakeY;
  work->savedShakeDecay = map->shakeDecay;
  if (gMainRoutineWork.mapId == SCENE_GABOMBA_CATACOMBS_5)
    work->lit = 1;
  if (gMainRoutineWork.mapId == SCENE_TUNDARIA_TOWER_1)
    work->lit = 1;
  if (gMainRoutineWork.mapId == SCENE_TUNDARIA_TOWER_2)
    work->lit = 1;
  if (gMainRoutineWork.mapId == SCENE_TUNDARIA_TOWER_7)
    work->lit = 1;
  if (gMainRoutineWork.mapId == SCENE_JUPITER_LIGHTHOUSE_AERIE)
    work->lit = 1;
  if (gMainRoutineWork.mapId == (s32)&SceneId_JupiterLighthouseAerieBeacon)
    work->lit = 1;
  if (gMainRoutineWork.mapId == SCENE_LOHO_1)
    work->lit = 1;
  FieldPsynergy_SetupActorPair(gMainRoutineWork.leaderActorId, -1);
  if (work->fieldPsynergy != 8)
    event->effectTimerRunning = 0;
  if (work->fieldPsynergy != 28)
    event->ringTimerRunning = 0;
  event->targetX = 0;
  event->targetY = 0;
  event->targetZ = 0;
  event->unkd38 = 0;
  event->unkd30 = 255;
  BattleEffect_StartArcEffect();
  Task_Add(BattleEffect_UpdateAllEffectSlots, 0x480);
}

/// Records the caster and the target of a field Psynergy.
void FieldPsynergy_SetupActorPair(s32 userId, s32 targetId) {
  struct Actor *user;
  struct Actor *target;
  struct FieldEffectWork *scene;

  scene = Ram_HeapSlots->fieldEffectWork;
  scene->firstActorId = userId;
  user = ObjectTable_Get((s16)userId);
  scene->secondActorId = targetId;
  scene->mainActor = user;
  target = ObjectTable_Get((s16)targetId);
  scene->angle = (user->facing + TRIG_TURN / 8) &
                 TRIG_TURN * 3 / 4; // Round to four directions.
  scene->angle8 = (user->facing + TRIG_TURN / 16) & 0xe000;
  scene->child = target;
  if (user != NULL) {
    u8 terrain;

    terrain = user->priorityFlags;

    scene->savedPriorityFlags = terrain;
    scene->savedSpritePriority = (u32)(((u8 *)user->sprite)[9] << 28) >> 30;
  }
  if (target != NULL) {
    // Save target motion and colors for later restoration.
    scene->savedCallback = target->update;
    scene->savedScript = target->script;
    scene->savedPalette =
        ((struct AnimationObject *)target->sprite)->entries[0]->param;
    scene->x = target->x.fixed;
    scene->z = target->z.fixed;
    scene->y = target->y.fixed;
  } else {
    scene->x = user->x.fixed;
    scene->z = user->z.fixed;
    scene->y = user->y.fixed;
    Vector_AddPolarOffset(px(16), scene->angle, &scene->x);
  }
}

/// Starts casting: the caster takes its casting pose (animation 20) and stops,
/// flashes with a sound, and the ability's name is announced; with flag 0x140
/// set the caster takes animation 21 and keeps its glow.
void EventObject_Initialize(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *object = work->mainActor;
  s32 ability = work->action;
  s32 zero;
  s8 sound;

  Actor_SelectAnimationFar(object, 20);
  /* FAKEMATCH(dead): the stores in a once-only block keep them all ahead of the
   * sound's read. */
  do {
    object->targetX = object->x.fixed;
    object->velocityX = 0;
    object->targetY = object->y.fixed;
    object->velocityY = 0;
    object->targetZ = object->z.fixed;
    object->velocityZ = 0;
  } while (0);
  sound = work->visible;
  /* FAKEMATCH(barrier): the zero set through an empty asm after the sound's
   * read. */
  asm("" : "=r"(zero) : "0"(0));
  if (sound != 0) {
    Sound_PlayFar(SE_POWER_UP);
    BattleEffect_SetObjectUpdateMode(object, 1);
  }
  if (work->active != 0) {
    work->messageShown = 1;
    Actor_SetOthersPaused(object, 1, 0);
    MessageArgument_PushFar(ability, MESSAGE_ARGUMENT_ABILITY);
    FieldMessage_ShowFar((s32)&MsgBlank2, work->messageMode);
    Actor_SetOthersPaused(object, 0, 16);
    work->messageShown = zero;
  }
  if (GameFlag_Get(0x140)) {
    if (work->visible != 0)
      BattleEffect_SetObjectUpdateMode(object, 2);
    Actor_SelectAnimationFar(object, 21);
  } else {
    BattleEffect_StopMainActor();
  }
  event->castStarted = 1;
}

/// Takes the caster out of its casting update and waits a frame.
void BattleEffect_StopMainActor(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  BattleEffect_SetObjectUpdateMode(work->mainActor, 0);
  wait(1);
}

/// Ends a field Psynergy: frees the field effect sprites still in use and,
/// unless another is being resolved, waits for the swelling arcs to end,
/// restores the map's shake and releases the work.
void BattleEffect_CleanupSceneObjects(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s8 *block = Ram_HeapSlots->paletteFade;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *owner = work->mainActor;
  s32 i;
  s16 first;

  for (i = 0; i < 24; i++) {
    struct FieldParticle *slot = &work->particles[i];
    if (slot->active)
      FieldEffect_ClearOwnedSlot(slot);
  }
  first = 17;
  if (work->fieldPsynergy >= first &&
      (work->fieldPsynergy <= 27 || work->fieldPsynergy == 30))
    FieldPsynergy_MarkWorldMapTilesUsed();
  if (event->psynergyActing != 0)
    return;
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP && block != NULL) {
    i = 0;
    if (block[0x2a01] != 0) {
      do {
        wait(1);
        if (++i > 89)
          break;
      } while (block[0x2a01] != 0);
    }
  }
  event->castStarted = 0;
  FieldPsynergy_ReleaseSceneSprite();
  Task_Remove(BattleEffect_UpdateAllEffectSlots);
  map->shakeX = work->savedShakeX;
  map->shakeY = work->savedShakeY;
  map->shakeDecay = work->savedShakeDecay;
  if (owner != NULL) {
    owner->priorityFlags = work->savedPriorityFlags;
    ((struct FieldSprite *)owner->sprite)->priority = work->savedSpritePriority;
  }
  if (work->fieldPsynergy != 8)
    event->effectTimerRunning = 1;
  if (work->fieldPsynergy != 28)
    event->ringTimerRunning = 1;
  Event_NoOp();
  HeapSlot_Release(HEAP_SLOT_FIELD_EFFECT);
}

/// On the World Map, notes that the field Psynergy has used the second half of
/// the BG tile buffer, so the next still picture decodes it again.
void FieldPsynergy_MarkWorldMapTilesUsed(void) {
  struct HeapSlots *slots = Ram_HeapSlots;
  s32 mode = ((struct FieldWork *)slots->fieldWork)->mode;

  if (mode == ACTOR_SYSTEM_WORLD_MAP)
    ((struct WorldMapView *)slots->mapGrid)->tileBufferUsed = mode;
}

/* VIEW(scene): tag struct WaveFxWork as struct WaveFxWork__scene;
 * module/field/event_commands.h has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/field_psynergy_work.c: structure has no
 * member named `id'. */
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

/// Starts the wave effect's work in heap slot HEAP_SLOT_MIND_READ: a phase
/// taken from the frame count, id and value, then the shared scene, the first
/// hue step and its colour fade, with FieldPsynergy_UpdateMindReadWave run each
/// frame.
void FieldPsynergy_StartMindReadWave(s32 id, s32 value) {
  struct WaveFxWork__scene *work;

  work = HeapSlot_AllocEwram(HEAP_SLOT_MIND_READ, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  work->phase = (gFrameCount * 2) % 360;
  work->id = id;
  work->value = value;
  work->steps = 8;
  work->unk295 = 0;
  FieldPsynergy_FadeToCastColor();
  FieldEffect_AdvanceHueCycle();
  PaletteFade_SetTarget(
      (work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
  PaletteFade_StartInterpolation(8);
  FieldPsynergy_GreyMindReadPalette();
  Task_Add((TaskFunc)FieldPsynergy_UpdateMindReadWave, TASK_PASS_VBLANK);
}

/// Stops the wave effect: restores the caster's colours and the palettes and
/// releases the Psynergy's work.
void FieldPsynergy_StopMindReadWave(void) {
  u8 *work = Ram_HeapSlots->mindRead;

  FieldPsynergy_RestoreMindReadPalette();
  Task_Remove((TaskFunc)FieldPsynergy_UpdateMindReadWave);
  Actor_SetSpritePalette(Actor_GetById(*(u16 *)(work + 0x290)), 1);
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_MIND_READ);
}

#define BG_PLTT_COLOR(bank, color)                                             \
  (*(volatile u16 *)(0x05000000 + ((bank) * 16 + (color)) * 2))
#define GREY28 0x739c

/// Gray BG palette 15 colors 1 and 3-12, then start line scrolling.
void FieldPsynergy_GreyMindReadPalette(void) {
  volatile u16 *p;

  Ram_HeapSlots->windowWork[4] = 1;
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

/// Stops line scrolling and restores background palette 15.
void FieldPsynergy_RestoreMindReadPalette(void) {
  u8 *work = Ram_HeapSlots->windowWork;

  Task_Remove((TaskFunc)FieldEffect_ArmBg0HBlankDma);
  BG_PLTT_COLOR(15, 1) = 0x7fff;  // White.
  BG_PLTT_COLOR(15, 3) = 0;       // Black.
  BG_PLTT_COLOR(15, 11) = 0x294a; // Gray (10).
  BG_PLTT_COLOR(15, 12) = 0x5294; // Gray (20).
  WindowColor_ApplyFar(gMainRoutineWork.unusedAfterParty[5],
                       gMainRoutineWork.unusedAfterParty[6]);
  work[4] = 0;
}
