/// @file
/// An actor's screen position, the encounter transition, and the context
/// effects of leaving through an exit and arriving.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "sounds.h"
#include "sprite_ids.h"
#include "field/field.h"
#include "fixed_math.h"
#include "flags.h"

/// Writes the actor's position on screen, in pixels, to position[0] and
/// position[1]; an actor with a sprite is raised by its frame box. Returns -1
/// when the actor does not exist.
s32 Actor_GetScreenPosition(s32 actorId, s32 *position) {
  struct Actor *actor = Actor_Lookup(actorId);
  s32 cameraX;
  s32 cameraZ;
  FIXED x;
  FIXED z;

  if (actor == 0)
    return -1;
  cameraX = MapState_View(Ram_HeapSlots->mapGrid)[0] & FIELD_PIXEL_MASK;
  cameraZ = MapState_View(Ram_HeapSlots->mapGrid)[1] & FIELD_PIXEL_MASK;
  x = actor->x.fixed - cameraX;
  z = actor->z.fixed - cameraZ - actor->y.fixed;
  *position++ = x / FIXED_ONE;
  *position = z / FIXED_ONE;
  if ((actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
    *position -= (s8)Resource_GetMetadataRecordFarFar(
                     ((struct Sprite *)actor->sprite)->entries[0]->animationId)
                     ->spriteTop;
  return 0;
}

/// The encounter's screen closes over ENCOUNTER_CLOSE_FRAMES, and its white
/// flash then fades to black over ENCOUNTER_FADE_FRAMES, ENCOUNTER_FADE_STEP
/// levels a frame from just below RGB_LEVEL_MAX.
#define ENCOUNTER_CLOSE_FRAMES 16
#define ENCOUNTER_FADE_FRAMES 16
#define ENCOUNTER_FADE_STEP 2

/// Starts the transition into a random battle: plays the scene's battle music
/// and the encounter sounds, closes the screen and flashes the backdrop white,
/// fading it to black.
void Field_StartEncounterTransition(s32 unused) {
  struct FieldState *state;
  s32 i;
  s32 level;

  state = Ram_HeapSlots->fieldState;
  Sound_PlayFar(gMainRoutineState.battleMusic);
  Sound_PlayFar(SE_STOP_EXIT_SE);
  Sound_PlayFar(SE_BEAST_CRY);
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
    // On the World Map the flash colours the window palette's black instead,
    // and the screen closes to the centre line in flickers; a field scene
    // closes with WINDOW_TRANSITION_LEADER_INVERSE.
    Io_Write16(RGB_WHITE,
               (u16 *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_BLACK));
    ScreenTransition_Finish(
        SCENE_TRANSITION(TRANSITION_CENTRE_LINE, CENTRE_LINE_FLICKERING),
        ENCOUNTER_CLOSE_FRAMES);
    state->transitionActive = 0;
    wait(ENCOUNTER_CLOSE_FRAMES);
    for (i = 0; i < ENCOUNTER_FADE_FRAMES; i++) {
      level = RGB_LEVEL_MAX - 1 - i * ENCOUNTER_FADE_STEP;
      Io_Write16(RGB(level, level, level),
                 (u16 *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_BLACK));
      wait(1);
    }
  } else {
    Io_Write16(RGB_WHITE, &PLTT_BACKDROP);
    ScreenTransition_Finish(
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER_INVERSE),
        ENCOUNTER_CLOSE_FRAMES);
    state->transitionActive = 0;
    wait(ENCOUNTER_CLOSE_FRAMES);
    for (i = 0; i < ENCOUNTER_FADE_FRAMES; i++) {
      level = RGB_LEVEL_MAX - 1 - i * ENCOUNTER_FADE_STEP;
      Io_Write16(RGB(level, level, level), &PLTT_BACKDROP);
      wait(1);
    }
  }
}

void *Sprite_AddAnimationFar(void *, s32);

/// Exit_PrepareAnimation for any actor: centres the actor on its cell at the
/// cell's south edge, stops it, hides its shadow and plays ANIM_LEADER_LEAVE
/// (side 1) or ANIM_LEADER_LEAVE_ALTERNATE over 18 frames,
/// its sprite gaining the doorway (SPRITE_DOORWAY) as a second part.
void Exit_PrepareActorAnimation(s32 actorId, s32 side) {
  struct Actor *actor;
  struct Sprite *sprite;
  struct SpriteAnimation *doorway;
  s32 animation;

  actor = Actor_Lookup(actorId);
  sprite = actor->sprite;
  animation = (side != 1);
  animation = ANIM_LEADER_LEAVE - animation;
  doorway =
      (struct SpriteAnimation *)Sprite_AddAnimationFar(sprite, SPRITE_DOORWAY);
  {
    u32 cleared = 0;
    u8 hidden = SPRITE_COLOR_HIDDEN;

    sprite->flags = cleared;
    doorway->colorMode = hidden;
    actor->x.fixed = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
    actor->z.fixed = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(16);
    actor->velocityX = cleared;
    actor->velocityZ = cleared;
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    Actor_SelectAnimationFar(actor, animation);
    wait(18);
  }
}

/// Starts the leader's animation for leaving through an exit: centres it on its
/// cell, at the cell's south edge, stops it and plays animation over 18 frames,
/// its sprite gaining the doorway (SPRITE_DOORWAY) as a second part.
void Exit_PrepareAnimation(s32 animation) {
  struct Actor *actor;
  struct Sprite *sprite;
  struct SpriteAnimation *doorway;

  actor = Actor_Lookup(gMainRoutineState.leaderActorId);
  sprite = actor->sprite;
  doorway =
      (struct SpriteAnimation *)Sprite_AddAnimationFar(sprite, SPRITE_DOORWAY);
  {
    u32 cleared = 0;
    u8 hidden = SPRITE_COLOR_HIDDEN;

    // No shadow as the leader leaves.
    sprite->flags = cleared;
    doorway->colorMode = hidden;
    // Keep the cell and put the leader at its middle in x and its south edge in
    // z.
    actor->x.fixed = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
    actor->z.fixed = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(16);
    actor->velocityX = cleared;
    actor->velocityZ = cleared;
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    Actor_SelectAnimationFar(actor, animation);
    wait(18);
  }
}

/// Leaves through a stairs-up exit (ANIM_LEADER_LEAVE); FLAG_ARRIVAL_STAIRS_UP
/// makes the next scene play the matching arrival (Entrance_PlayArrival).
void Exit_StartStairsUp(void) {
  Exit_PrepareAnimation(ANIM_LEADER_LEAVE);
  GameFlag_Set(FLAG_ARRIVAL_STAIRS_UP);
}

/// Leaves through a stairs-down exit (ANIM_LEADER_LEAVE_ALTERNATE), recorded in
/// FLAG_ARRIVAL_STAIRS_DOWN.
void Exit_StartStairsDown(void) {
  Exit_PrepareAnimation(ANIM_LEADER_LEAVE_ALTERNATE);
  GameFlag_Set(FLAG_ARRIVAL_STAIRS_DOWN);
}

/// Plays the leader's arrival animation through an exit: centred on its cell at
/// the cell's north edge, the leader plays animation for 30 frames, loses the
/// doorway (SPRITE_DOORWAY) and walks 8 pixels south.
void Entrance_EndArrivalAnimation(s32 animation) {
  struct Actor *leader = Actor_Lookup(gMainRoutineState.leaderActorId);
  struct Sprite *sprite = leader->sprite;
  struct SpriteAnimation *doorway =
      (struct SpriteAnimation *)Sprite_AddAnimationFar(sprite, SPRITE_DOORWAY);
  sprite->flags = 0;
  doorway->colorMode = SPRITE_COLOR_HIDDEN;
  leader->x.fixed = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  leader->z.fixed &= FIELD_CELL_MASK;
  Actor_SelectAnimationFar(leader, animation);
  wait(30);
  // Back to one sprite part, with its shadow, walking 8 pixels south at 1
  // pixel a frame.
  sprite->count = 1;
  SpriteAnimation_ClearFar(sprite->entries[1]);
  sprite->entries[1] = NULL;
  sprite->flags = SPRITE_FLAG_SHADOW;
  leader->acceleration = FIXED_ONE;
  leader->speedLimit = FIXED_ONE;
  Actor_SetMoveTargetFar(leader, leader->x.fixed, leader->y.fixed,
                         leader->z.fixed + toFIXED(8));
  Actor_WaitForMoveFar(leader);
}

/// Plays the leader's arrival for the way the last scene was left: through a
/// stairs-up exit (FLAG_ARRIVAL_STAIRS_UP) or a stairs-down one
/// (FLAG_ARRIVAL_STAIRS_DOWN), or falling in from 160 pixels above
/// (FLAG_ARRIVE_FALLING). Returns the enum EntranceArrival it played.
s32 Entrance_PlayArrival(void) {
  s32 result = ARRIVAL_NONE;
  s32 flag = FLAG_ARRIVAL_STAIRS_UP;

  if (GameFlag_Get(flag) != 0) {
    Entrance_EndArrivalAnimation(ANIM_LEADER_ARRIVE);
    GameFlag_Clear(flag);
    result = ARRIVAL_STAIRS_UP;
  } else {
    flag = FLAG_ARRIVAL_STAIRS_DOWN;
    if (GameFlag_Get(flag) != 0) {
      Entrance_EndArrivalAnimation(ANIM_LEADER_ARRIVE_ALTERNATE);
      GameFlag_Clear(flag);
      result = ARRIVAL_STAIRS_DOWN;
    } else {
      flag = FLAG_ARRIVE_FALLING;
      if (GameFlag_Get(flag) != 0) {
        s32 id;
        struct Actor *actor;

        GameFlag_Clear(flag);
        id = gMainRoutineState.leaderActorId;
        actor = Actor_Lookup(id);
        actor->y.fixed += toFIXED(160);
        FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
        while (actor->y.fixed + actor->velocityY > actor->groundHeight) {
          wait(1);
        }
        Sound_PlayFar(SE_DOOR_CLOSE);
        actor->y.fixed = actor->groundHeight;
        Actor_SelectAnimationFar(actor, ANIM_LEADER_LEAN_BACK);
        Event_Wait(15);
        FieldCamera_FollowActor(id, 1);
        result = ARRIVAL_FALLING;
      }
    }
  }
  return result;
}

/// An EXIT_STYLE_HIDDEN_LAYER exit: gives the leader's sprite a second part,
/// SPRITE_HIDDEN_LAYER, a band over the top 8 rows of its 32 by 32 picture,
/// drawn in SPRITE_COLOR_HIDDEN.
void Exit_AddHiddenLeaderLayer(void) {
  struct Actor *actor = Actor_Lookup(gMainRoutineState.leaderActorId);
  struct SpriteAnimation *layer =
      Sprite_AddAnimationFar(actor->sprite, SPRITE_HIDDEN_LAYER);

  layer->colorMode = SPRITE_COLOR_HIDDEN;
}

#undef ENCOUNTER_CLOSE_FRAMES
#undef ENCOUNTER_FADE_FRAMES
#undef ENCOUNTER_FADE_STEP
