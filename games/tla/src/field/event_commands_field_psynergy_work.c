/// @file
/// The effect scene a field Psynergy runs in: fading to and from its cast
/// colour, its effect slots and resources, the actor pair it works on, and
/// Mind Read's wave and palettes.
// Part of event_commands.c.

#include "event_commands_internal.h"
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
#include "field/flag_call.h"
#include "gamepak/far_call/window.h"
#include "system/heap_release.h"
#include "field/event_commands.h"
#include "field/object.h"
#include "text/msg_ids.h"
#include "system/system.h"
#include "field/map_grid.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "system/iwram_call.h"
#include "battle/battle_unit.h"
#include "field/sprite.h"
#include "field/actor.h"
#include "system/frame.h"
#include "field/actor_view.h"
#include "field/field_sprite.h"
#include "system/dma.h"

/// How long FieldPsynergy_End waits at most for the World Map's palette fade.
#define END_FADE_WAIT_FRAMES 90

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
void Camera_WorldToScreen(struct Vector3 *position) {
  struct HeapSlots *slots = Ram_HeapSlots;

  if (((struct FieldWork *)slots->fieldWork)->mode == ACTOR_SYSTEM_WORLD_MAP) {
    s32 result[3];

    Matrix_ProjectPoint(position, result);
    position->x = INTtoFIXED(result[0]);
    position->z = INTtoFIXED(result[1]);
    position->y = 0;
  } else {
    struct MapWork *map = slots->mapGrid;
    s32 x = map->viewX & FIELD_PIXEL_MASK;
    s32 z = map->viewZ & FIELD_PIXEL_MASK;

    // Relative to the view's whole-pixel corner; height lifts a point up
    // the screen.
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
  remainingSlots = ARRAY_COUNT(scene->particles) - 1;
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
void FieldPsynergy_LoadResources(s32 abilityId, s32 mode) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct FieldEffectWork *work;
  s32 running;

  if (event->psynergyActing == 0) {
    work = HeapSlot_Alloc(HEAP_SLOT_FIELD_EFFECT, sizeof(*work));
    IWRAM_FILL_WORDS(work, sizeof(*work), 0);
  } else {
    work = Ram_HeapSlots->fieldEffectWork;
  }
  work->action = abilityId;
  work->fieldPsynergy = Ability_Get(abilityId)->fieldPsynergy;
  running = event->psynergyActing;
  if (running != 0)
    return;
  work->freeBlocks = VRAM_BLOCK_COUNT - ResourceTable_CountFreeBlocks();
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
  if (gMainRoutineWork.mapId == SCENE_GABOMBA_CATACOMBS_STREAM)
    work->lit = 1;
  if (gMainRoutineWork.mapId == SCENE_TUNDARIA_TOWER_ENTRANCE)
    work->lit = 1;
  if (gMainRoutineWork.mapId == SCENE_TUNDARIA_TOWER_CENTER_PRONG)
    work->lit = 1;
  if (gMainRoutineWork.mapId == SCENE_TUNDARIA_TOWER_BURST_BROOCH)
    work->lit = 1;
  if (gMainRoutineWork.mapId == SCENE_JUPITER_LIGHTHOUSE_AERIE)
    work->lit = 1;
  if (gMainRoutineWork.mapId == (s32)&SceneId_JupiterLighthouseAerieBeacon)
    work->lit = 1;
  if (gMainRoutineWork.mapId == SCENE_LOHO)
    work->lit = 1;
  FieldPsynergy_SetupActorPair(gMainRoutineWork.leaderActorId, -1);
  if (work->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
    event->effectTimerRunning = 0;
  if (work->fieldPsynergy != FIELD_PSYNERGY_RING)
    event->ringTimerRunning = 0;
  event->targetX = 0;
  event->targetY = 0;
  event->targetZ = 0;
  event->blazeLongSpray = 0;
  event->psynergyHitObject = -1;
  BattleEffect_StartArcEffect();
  Task_Add(BattleEffect_UpdateAllEffectSlots, TASK_PASS_FRAME);
}

/// Records the caster and the target of a field Psynergy.
void FieldPsynergy_SetupActorPair(s32 userId, s32 targetId) {
  struct Actor *user;
  struct Actor *target;
  struct FieldEffectWork *scene;

  scene = Ram_HeapSlots->fieldEffectWork;
  scene->firstActorId = userId;
  user = Actor_Lookup((s16)userId);
  scene->secondActorId = targetId;
  scene->mainActor = user;
  target = Actor_Lookup((s16)targetId);
  // The facing rounded to four directions, and to eight.
  scene->angle = (user->facing + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
  scene->angle8 = (user->facing + FACING_STEP) & TRIG_TURN * 7 / 8;
  scene->child = target;
  if (user != NULL) {
    u8 terrain;

    terrain = user->priorityFlags;

    scene->savedPriorityFlags = terrain;
    scene->savedSpritePriority =
        ((struct Sprite *)user->sprite)->image.priority;
  }
  if (target != NULL) {
    // Save target motion and colors for later restoration.
    scene->savedCallback = target->update;
    scene->savedScript = target->script;
    scene->savedPalette =
        ((struct Sprite *)target->sprite)->entries[0]->colorMode;
    scene->x = target->x.fixed;
    scene->z = target->z.fixed;
    scene->y = target->y.fixed;
  } else {
    scene->x = user->x.fixed;
    scene->z = user->z.fixed;
    scene->y = user->y.fixed;
    Vector_AddPolarOffset(toFIXED(16), scene->angle, &scene->x);
  }
}

/// Starts casting: the caster raises its hand (ANIM_CAST_PSYNERGY_START) and
/// stops, flashes with a sound, and the ability's name is announced; with
/// FLAG_PLAY_PSYNERGY_CAST set the caster holds out its hand
/// (ANIM_CAST_PSYNERGY_END) and keeps its glow.
void FieldPsynergy_StartCast(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *actor = work->mainActor;
  s32 ability = work->action;

  Actor_SelectAnimationFar(actor, ANIM_CAST_PSYNERGY_START);
  actor->targetX = actor->x.fixed;
  actor->velocityX = 0;
  actor->targetY = actor->y.fixed;
  actor->velocityY = 0;
  actor->targetZ = actor->z.fixed;
  actor->velocityZ = 0;
  if (work->visible != 0) {
    Sound_PlayFar(SE_POWER_UP);
    BattleEffect_SetObjectUpdateMode(actor, 1);
  }
  if (work->active != 0) {
    work->messageShown = 1;
    Actor_SetOthersPaused(actor, 1, 0);
    MessageArgument_PushFar(ability, MESSAGE_ARGUMENT_ABILITY);
    FieldMessage_ShowFar((s32)&MsgCastAbilityName, work->messageMode);
    Actor_SetOthersPaused(actor, 0, ANIM_STEP_NORMAL);
    work->messageShown = 0;
  }
  if (GameFlag_Get(FLAG_PLAY_PSYNERGY_CAST)) {
    if (work->visible != 0)
      BattleEffect_SetObjectUpdateMode(actor, 2);
    Actor_SelectAnimationFar(actor, ANIM_CAST_PSYNERGY_END);
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
void FieldPsynergy_End(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct PaletteFade *fade = Ram_HeapSlots->paletteFade;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *owner = work->mainActor;
  s32 i;
  s16 first;

  for (i = 0; i < (s32)ARRAY_COUNT(work->particles); i++) {
    struct FieldParticle *slot = &work->particles[i];
    if (slot->active)
      FieldEffect_ClearOwnedSlot(slot);
  }
  first = FIELD_PSYNERGY_LASH;
  if (work->fieldPsynergy >= first &&
      (work->fieldPsynergy <= FIELD_PSYNERGY_GRIND ||
       work->fieldPsynergy == FIELD_PSYNERGY_TELEPORT))
    FieldPsynergy_MarkWorldMapTilesUsed();
  if (event->psynergyActing != 0)
    return;
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP && fade != NULL) {
    i = 0;
    if (fade->duration != 0) {
      do {
        wait(1);
        if (++i >= END_FADE_WAIT_FRAMES)
          break;
      } while (fade->duration != 0);
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
    ((struct Sprite *)owner->sprite)->image.priority =
        work->savedSpritePriority;
  }
  if (work->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
    event->effectTimerRunning = 1;
  if (work->fieldPsynergy != FIELD_PSYNERGY_RING)
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

/// Starts the wave effect's work in heap slot HEAP_SLOT_MIND_READ: a phase
/// taken from the frame count, id and value, then the shared scene, the first
/// hue step and its colour fade, with FieldPsynergy_UpdateMindReadWave run each
/// frame.
void FieldPsynergy_StartMindReadWave(s32 id, s32 value) {
  struct MindReadWork *work;

  work = HeapSlot_AllocEwram(HEAP_SLOT_MIND_READ, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  work->hue = (gFrameCount * 2) % HUE_DEGREES;
  work->sourceId = id;
  work->targetId = value;
  work->delay = 8;
  work->counter = 0;
  FieldPsynergy_FadeToCastColor();
  FieldEffect_AdvanceHueCycle();
  PaletteFade_SetTarget(
      RGB(work->red, work->green, work->blue) | PALETTE_TINT_CUSTOM, 1);
  PaletteFade_StartInterpolation(8);
  FieldPsynergy_GreyMindReadPalette();
  Task_Add((TaskFunc)FieldPsynergy_UpdateMindReadWave, TASK_PASS_FRAME);
}

/// Stops the wave effect: restores the caster's colours and the palettes and
/// releases the Psynergy's work.
void FieldPsynergy_StopMindReadWave(void) {
  struct MindReadWork *work = Ram_HeapSlots->mindRead;

  FieldPsynergy_RestoreMindReadPalette();
  Task_Remove(FieldPsynergy_UpdateMindReadWave);
  Actor_SetSpritePalette(Actor_GetById(work->sourceId), 1);
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_MIND_READ);
}

#define MIND_READ_GREY RGB(28, 28, 28)

/// Turns WINDOW_COLOR_WHITE and WINDOW_PALETTE's colours from
/// WINDOW_COLOR_BLACK to WINDOW_COLOR_LIGHT_GREY grey (all but colour 2), so
/// messages show as thoughts, and starts the BG0 line scroll
/// (FieldEffect_ArmBg0HBlankDma) at each VBlank.
void FieldPsynergy_GreyMindReadPalette(void) {
  volatile u16 *p;

  ((struct WindowWork *)Ram_HeapSlots->windowWork)->mindRead = 1;
  p = &BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_WHITE);
  *p = MIND_READ_GREY;
  p += 2;
  *p = MIND_READ_GREY;
  p += 8;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p -= 8;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  Task_Add((TaskFunc)FieldEffect_ArmBg0HBlankDma, TASK_PASS_VBLANK - 20);
}

/// Stops the BG0 line scroll and restores WINDOW_PALETTE.
void FieldPsynergy_RestoreMindReadPalette(void) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;

  Task_Remove((TaskFunc)FieldEffect_ArmBg0HBlankDma);
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_WHITE) = RGB_WHITE;
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_BLACK) = RGB(0, 0, 0);
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_DARK_GREY) = RGB(10, 10, 10);
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_LIGHT_GREY) = RGB(20, 20, 20);
  WindowColor_ApplyFar(gMainRoutineWork.windowHue,
                       gMainRoutineWork.windowBrightness);
  work->mindRead = 0;
}
