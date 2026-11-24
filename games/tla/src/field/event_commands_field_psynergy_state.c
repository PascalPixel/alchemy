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
#include "system/main_routine_state.h"
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
  struct HeapSlots *heap = Ram_HeapSlots;

  if (((struct FieldState *)heap->fieldState)->mode == ACTOR_SYSTEM_WORLD_MAP) {
    s32 result[3];

    Matrix_ProjectPoint(position, result);
    position->x = INTtoFIXED(result[0]);
    position->z = INTtoFIXED(result[1]);
    position->y = 0;
  } else {
    struct MapState *map = heap->mapGrid;
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

// The same routine as FieldEffect_UpdateAllSlots (event_commands_djinni.c),
// under another name.
/// A task: updates the scene's 24 effect slots.
void BattleEffect_UpdateAllEffectSlots(void) {
  struct FieldEffectState *scene;
  struct FieldParticle *slot;
  s32 remainingSlots;

  scene = Ram_HeapSlots->fieldEffectState;
  remainingSlots = ARRAY_COUNT(scene->particles) - 1;
  slot = scene->particles;
  do {
    remainingSlots -= 1;
    FieldParticle_Update(slot);
    slot++;
  } while (remainingSlots >= 0);
}

/// Prepares the state of the field Psynergy for ability (heap slot
/// fieldEffectState): its kind, the free VRAM it starts with, the map's shake
/// to restore, and whether the place is lit. While another field Psynergy is
/// resolving, only the ability and kind are set.
void FieldPsynergy_LoadResources(s32 abilityId, s32 mode) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct FieldEffectState *state;
  s32 running;

  if (event->psynergyActing == 0) {
    state = HeapSlot_Alloc(HEAP_SLOT_FIELD_EFFECT, sizeof(*state));
    IWRAM_FILL_WORDS(state, sizeof(*state), 0);
  } else {
    state = Ram_HeapSlots->fieldEffectState;
  }
  state->action = abilityId;
  state->fieldPsynergy = Ability_Get(abilityId)->fieldPsynergy;
  running = event->psynergyActing;
  if (running != 0)
    return;
  state->freeBlocks = VRAM_BLOCK_COUNT - ResourceTable_CountFreeBlocks();
  state->mode = mode;
  state->visible = 1;
  state->enabled = 1;
  state->active = 1;
  state->messageMode = 1;
  state->messageShown = 0;
  state->stage = running;
  state->frame = -1;
  state->spriteActive = running;
  state->savedShakeX = map->shakeX;
  state->savedShakeY = map->shakeY;
  state->savedShakeDecay = map->shakeDecay;
  if (gMainRoutineState.mapId == SCENE_GABOMBA_CATACOMBS_STREAM)
    state->lit = 1;
  if (gMainRoutineState.mapId == SCENE_TUNDARIA_TOWER_ENTRANCE)
    state->lit = 1;
  if (gMainRoutineState.mapId == SCENE_TUNDARIA_TOWER_CENTER_PRONG)
    state->lit = 1;
  if (gMainRoutineState.mapId == SCENE_TUNDARIA_TOWER_BURST_BROOCH)
    state->lit = 1;
  if (gMainRoutineState.mapId == SCENE_JUPITER_LIGHTHOUSE_AERIE)
    state->lit = 1;
  if (gMainRoutineState.mapId == (s32)&SceneId_JupiterLighthouseAerieBeacon)
    state->lit = 1;
  if (gMainRoutineState.mapId == SCENE_LOHO)
    state->lit = 1;
  FieldPsynergy_SetupActorPair(gMainRoutineState.leaderActorId, -1);
  if (state->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
    event->effectTimerRunning = 0;
  if (state->fieldPsynergy != FIELD_PSYNERGY_RING)
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
  struct FieldEffectState *scene;

  scene = Ram_HeapSlots->fieldEffectState;
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
    u8 priorityFlags;

    priorityFlags = user->priorityFlags;

    scene->savedPriorityFlags = priorityFlags;
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
  struct FieldEffectState *state = Ram_HeapSlots->fieldEffectState;
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct Actor *actor = state->mainActor;
  s32 ability = state->action;

  Actor_SelectAnimationFar(actor, ANIM_CAST_PSYNERGY_START);
  actor->targetX = actor->x.fixed;
  actor->velocityX = 0;
  actor->targetY = actor->y.fixed;
  actor->velocityY = 0;
  actor->targetZ = actor->z.fixed;
  actor->velocityZ = 0;
  if (state->visible != 0) {
    Sound_PlayFar(SE_POWER_UP);
    BattleEffect_SetObjectUpdateMode(actor, 1);
  }
  if (state->active != 0) {
    state->messageShown = 1;
    Actor_SetOthersPaused(actor, 1, 0);
    MessageArgument_PushFar(ability, MESSAGE_ARGUMENT_ABILITY);
    FieldMessage_ShowFar((s32)&MsgCastAbilityName, state->messageMode);
    Actor_SetOthersPaused(actor, 0, ANIM_STEP_NORMAL);
    state->messageShown = 0;
  }
  if (GameFlag_Get(FLAG_PLAY_PSYNERGY_CAST)) {
    if (state->visible != 0)
      BattleEffect_SetObjectUpdateMode(actor, 2);
    Actor_SelectAnimationFar(actor, ANIM_CAST_PSYNERGY_END);
  } else {
    BattleEffect_StopMainActor();
  }
  event->castStarted = 1;
}

/// Takes the caster out of its casting update and waits a frame.
void BattleEffect_StopMainActor(void) {
  struct FieldEffectState *state = Ram_HeapSlots->fieldEffectState;

  BattleEffect_SetObjectUpdateMode(state->mainActor, 0);
  wait(1);
}

/// Ends a field Psynergy: frees the field effect sprites still in use and,
/// unless another is being resolved, waits for the swelling arcs to end,
/// restores the map's shake and releases the state.
void FieldPsynergy_End(void) {
  struct FieldEffectState *state = Ram_HeapSlots->fieldEffectState;
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct PaletteFade *fade = Ram_HeapSlots->paletteFade;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct Actor *owner = state->mainActor;
  s32 i;
  s16 first;

  for (i = 0; i < (s32)ARRAY_COUNT(state->particles); i++) {
    struct FieldParticle *slot = &state->particles[i];
    if (slot->active)
      FieldEffect_ClearOwnedSlot(slot);
  }
  first = FIELD_PSYNERGY_LASH;
  if (state->fieldPsynergy >= first &&
      (state->fieldPsynergy <= FIELD_PSYNERGY_GRIND ||
       state->fieldPsynergy == FIELD_PSYNERGY_TELEPORT))
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
  map->shakeX = state->savedShakeX;
  map->shakeY = state->savedShakeY;
  map->shakeDecay = state->savedShakeDecay;
  if (owner != NULL) {
    owner->priorityFlags = state->savedPriorityFlags;
    ((struct Sprite *)owner->sprite)->image.priority =
        state->savedSpritePriority;
  }
  if (state->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
    event->effectTimerRunning = 1;
  if (state->fieldPsynergy != FIELD_PSYNERGY_RING)
    event->ringTimerRunning = 1;
  Event_NoOp();
  HeapSlot_Release(HEAP_SLOT_FIELD_EFFECT);
}

/// On the World Map, notes that the field Psynergy has used the second half of
/// the BG tile buffer, so the next still image decodes it again.
void FieldPsynergy_MarkWorldMapTilesUsed(void) {
  struct HeapSlots *heap = Ram_HeapSlots;
  s32 mode = ((struct FieldState *)heap->fieldState)->mode;

  if (mode == ACTOR_SYSTEM_WORLD_MAP)
    ((struct WorldMapView *)heap->mapGrid)->tileBufferUsed = mode;
}

/// Starts Mind Read's wave between the caster and the one it reads
/// (HEAP_SLOT_MIND_READ): a hue taken from the frame count, the field faded
/// toward it, the message window greyed for thoughts, and
/// FieldPsynergy_UpdateMindReadWave run each frame.
void FieldPsynergy_StartMindReadWave(s32 casterId, s32 targetId) {
  struct MindReadState *state;

  state = HeapSlot_AllocEwram(HEAP_SLOT_MIND_READ, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  state->hue = (gFrameCount * 2) % HUE_DEGREES;
  state->sourceId = casterId;
  state->targetId = targetId;
  state->delay = 8;
  state->counter = 0;
  FieldPsynergy_FadeToCastColor();
  FieldEffect_AdvanceHueCycle();
  PaletteFade_SetTarget(RGB(state->red, state->green, state->blue) |
                            PALETTE_TINT_CUSTOM,
                        PALETTES_BG);
  PaletteFade_StartInterpolation(8);
  FieldPsynergy_GreyMindReadPalette();
  Task_Add((TaskFunc)FieldPsynergy_UpdateMindReadWave, TASK_PASS_FRAME);
}

/// Stops the wave effect: restores the caster's colours and the palettes and
/// releases the Psynergy's state.
void FieldPsynergy_StopMindReadWave(void) {
  struct MindReadState *state = Ram_HeapSlots->mindRead;

  FieldPsynergy_RestoreMindReadPalette();
  Task_Remove(FieldPsynergy_UpdateMindReadWave);
  Actor_SetSpritePalette(Actor_GetById(state->sourceId), 1);
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_MIND_READ);
}

#define MIND_READ_GREY RGB(28, 28, 28) ///< a thought window's light grey

/// Greys the message window's white and most of its other colours
/// (MIND_READ_GREY), so messages read as thoughts, and starts the BG0 line
/// scroll (FieldEffect_ArmBg0HBlankDma) at each VBlank.
void FieldPsynergy_GreyMindReadPalette(void) {
  volatile u16 *p;

  ((struct WindowState *)Ram_HeapSlots->windowState)->mindRead = 1;
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
  struct WindowState *state = (struct WindowState *)Ram_HeapSlots->windowState;

  Task_Remove((TaskFunc)FieldEffect_ArmBg0HBlankDma);
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_WHITE) = RGB_WHITE;
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_BLACK) = RGB(0, 0, 0);
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_DARK_GREY) = RGB(10, 10, 10);
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_LIGHT_GREY) = RGB(20, 20, 20);
  WindowColor_ApplyFar(gMainRoutineState.windowHue,
                       gMainRoutineState.windowBrightness);
  state->mindRead = 0;
}
