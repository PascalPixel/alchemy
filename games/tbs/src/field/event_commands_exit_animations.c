/// @file
/// An actor's screen position, the encounter transition, and the leader
/// leaving through an exit and arriving.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Finds where an actor is on screen: position[0] and position[1] receive its x
/// and y in pixels, y at the top of its sprite.
s32 Actor_GetScreenPosition(s32 actorId, s32 *position) {
  struct Actor *actor = Actor_Lookup(actorId);
  struct MapGridScrollWork *grid;
  s32 *view;
  s32 cameraX;
  s32 cameraZ;
  FIXED x;
  FIXED z;

  if (actor == 0)
    return -1;
  grid = gMapGridWork[0];
  view = grid->view;
  cameraX = view[0] & FIELD_PIXEL_MASK;
  cameraZ = view[1] & FIELD_PIXEL_MASK;
  x = actor->x.fixed - cameraX;
  z = actor->z.fixed - cameraZ - actor->y.fixed;
  *position++ = x / FIXED_ONE;
  *position = z / FIXED_ONE;
  if ((actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
    *position -=
        Resource_GetMetadataRecordFar(actor->sprite->entries[0]->animationId)
            ->boxX;
  return 0;
}

UNUSED void Exit_NoOp(void) {}

/// The encounter's screen closes over ENCOUNTER_CLOSE_FRAMES, and its white
/// flash then fades to black over ENCOUNTER_FADE_FRAMES, ENCOUNTER_FADE_STEP
/// levels a frame from just below RGB_LEVEL_MAX.
#define ENCOUNTER_CLOSE_FRAMES 16
#define ENCOUNTER_FADE_FRAMES 16
#define ENCOUNTER_FADE_STEP 2

/// Starts the transition into a random battle: plays the scene's battle music
/// and the encounter sounds, closes the screen and flashes the backdrop white,
/// fading it to black.
UNREAD_RESULT Field_StartEncounterTransition(s32 unused) {
  struct FieldWork *work = gFieldWork;
  s32 i;
  s32 level;

  Sound_PlayFar(gMainRoutineWork.battleMusic);
  Sound_PlayFar(SE_STOP_ACTION_SE);
  Sound_PlayFar(SE_BEAST_CRY);
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    // On the World Map the flash colours the window palette's black instead,
    // and the screen closes toward the centre line from its middle
    // (ScreenTransition_UpdateFromCenter); a field scene closes with
    // WINDOW_TRANSITION_LEADER_INVERSE.
    Io_Write16(RGB_WHITE,
               (u16 *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BLACK_COLOR));
    ScreenTransition_Finish(SCENE_TRANSITION(TRANSITION_CENTER_LINE, 1),
                            ENCOUNTER_CLOSE_FRAMES);
    work->transitionActive = 0;
    wait(ENCOUNTER_CLOSE_FRAMES);
    for (i = 0; i < ENCOUNTER_FADE_FRAMES; i++) {
      level = RGB_LEVEL_MAX - 1 - i * ENCOUNTER_FADE_STEP;
      Io_Write16(RGB(level, level, level),
                 (u16 *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BLACK_COLOR));
      wait(1);
    }
  } else {
    Io_Write16(RGB_WHITE, &PLTT_BACKDROP);
    ScreenTransition_Finish(
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER_INVERSE),
        ENCOUNTER_CLOSE_FRAMES);
    work->transitionActive = 0;
    wait(ENCOUNTER_CLOSE_FRAMES);
    for (i = 0; i < ENCOUNTER_FADE_FRAMES; i++) {
      level = RGB_LEVEL_MAX - 1 - i * ENCOUNTER_FADE_STEP;
      Io_Write16(RGB(level, level, level), &PLTT_BACKDROP);
      wait(1);
    }
  }
}

/// Starts the leader's animation for leaving through an exit: centres it on its
/// cell, at the cell's south edge, stops it and plays animation over 18 frames.
void Exit_PrepareAnimation(s32 animation) {
  struct Actor *actor;
  struct Sprite *sprite;
  struct SpriteAnimation *doorway;

  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
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

/// Leaves through an EXIT_STYLE_STAIRS_UP exit (ANIM_LEADER_LEAVE);
/// FLAG_ARRIVAL_STAIRS_UP makes the next scene play the matching arrival
/// (Entrance_PlayArrival).
void Exit_StartStairsUp(void) {
  Exit_PrepareAnimation(ANIM_LEADER_LEAVE);
  GameFlag_SetFar(FLAG_ARRIVAL_STAIRS_UP);
}

/// Leaves through an EXIT_STYLE_STAIRS_DOWN exit (ANIM_LEADER_LEAVE_ALTERNATE),
/// recorded in FLAG_ARRIVAL_STAIRS_DOWN.
void Exit_StartStairsDown(void) {
  Exit_PrepareAnimation(ANIM_LEADER_LEAVE_ALTERNATE);
  GameFlag_SetFar(FLAG_ARRIVAL_STAIRS_DOWN);
}

/// Plays the leader's arrival animation through an exit: centred on its cell at
/// the cell's north edge, the leader plays animation for 30 frames, loses the
/// hidden doorway part and walks 8 pixels south.
void Entrance_PlayArrivalAnimation(s32 animation) {
  struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
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
/// EXIT_STYLE_STAIRS_UP exit (FLAG_ARRIVAL_STAIRS_UP, ANIM_LEADER_ARRIVE,
/// returns 1) or a EXIT_STYLE_STAIRS_DOWN one (FLAG_ARRIVAL_STAIRS_DOWN,
/// ANIM_LEADER_ARRIVE_ALTERNATE, returns 2), or falling in from 160 pixels
/// above (FLAG_ARRIVE_FALLING, returns 3).
s32 Entrance_PlayArrival(void) {
  s32 result = 0;
  s32 flag = FLAG_ARRIVAL_STAIRS_UP;

  if (GameFlag_GetFar(flag) != 0) {
    Entrance_PlayArrivalAnimation(ANIM_LEADER_ARRIVE);
    GameFlag_ClearFar(flag);
    result = 1;
  } else {
    flag = FLAG_ARRIVAL_STAIRS_DOWN;
    if (GameFlag_GetFar(flag) != 0) {
      Entrance_PlayArrivalAnimation(ANIM_LEADER_ARRIVE_ALTERNATE);
      GameFlag_ClearFar(flag);
      result = 2;
    } else {
      flag = FLAG_ARRIVE_FALLING;
      if (GameFlag_GetFar(flag) != 0) {
        s32 leaderId;
        struct Actor *leader;

        GameFlag_ClearFar(flag);
        leaderId = gMainRoutineWork.leaderActorId;
        leader = Actor_Lookup(leaderId);
        // FLAG_ARRIVE_FALLING is set by scenes the party drops out of (the
        // Suhalla storm, a floor giving way in the Tret tree): start 160 pixels
        // up and let gravity bring the leader down to the ground.
        leader->y.fixed += toFIXED(160);
        FieldCamera_MoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP,
                                        0);
        // Until the next frame's fall would reach the ground.
        while (leader->y.fixed + leader->velocityY > leader->groundHeight) {
          wait(1);
        }
        Sound_PlayFar(SE_DOOR_CLOSE);
        leader->y.fixed = leader->groundHeight;
        Actor_SelectAnimationFar(leader, ANIM_LEADER_LEAN_BACK);
        Event_Wait(15);
        FieldCamera_FollowActor(leaderId, 1);
        result = 3;
      }
    }
  }
  return result;
}

#undef ENCOUNTER_CLOSE_FRAMES
#undef ENCOUNTER_FADE_FRAMES
#undef ENCOUNTER_FADE_STEP
