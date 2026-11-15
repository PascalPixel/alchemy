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

/// Writes the object's position on screen, in pixels, to position[0] and
/// position[1]; an animated object is raised by its frame box. Returns -1 when
/// the object does not exist.
s32 Actor_GetScreenPosition(s32 actorId, s32 *position) {
  struct Actor *object = Actor_Lookup(actorId);
#ifdef UBFIX
  struct MapWork *map;
#else
  s32 *camera; // the view's top left, x then z
#endif
  s32 cameraX;
  s32 cameraZ;
  s32 x;
  s32 z;

  if (object == 0)
    return -1;
  // UB: camera[1] crosses the viewX scalar; UBFIX reads the named pair.
#ifdef UBFIX
  map = Ram_HeapSlots->mapGrid;
  cameraX = map->viewX & 0xffff0000;
  cameraZ = map->viewZ & 0xffff0000;
#else
  camera = &Ram_HeapSlots->mapGrid->viewX;
  cameraX = camera[0] & 0xffff0000;
  cameraZ = camera[1] & 0xffff0000;
#endif
  x = object->x.fixed - cameraX;
  z = object->z.fixed - cameraZ - object->y.fixed;
  *position++ = x / FIXED_ONE;
  *position = z / FIXED_ONE;
  if ((object->spriteStorageKind & 15) == 1)
    *position -= (s8)Resource_GetMetadataRecordFarFar(
                     ((struct Sprite *)object->sprite)->entries[0]->animationId)
                     ->boxX;
  return 0;
}

/// The Broken Seal's Scene_FadeColorFromWhite, playing cue 298 where that
/// plays 288: turns the backdrop (on the world map, a colour of palette 15)
/// white, finishes the screen transition and fades that colour to black
/// over 16 frames.
void Field_StartEncounterTransition(s32 unused) {
  struct FieldWork *work;
  s32 i;
  s32 c;

  work = Ram_HeapSlots->fieldWork;
  Sound_PlayFar(gMainRoutineWork.battleMusic);
  Sound_PlayFar(SE_STOP_EXIT_SE);
  Sound_PlayFar(SE_BEAST_CRY);
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    Io_Write16(RGB(31, 31, 31), (u16 *)&BG_PLTT_COLOR(15, 3));
    ScreenTransition_Finish(0x401, 16);
    work->transitionActive = 0;
    wait(16);
    for (i = 0; i < 16; i++) {
      c = 30 - i * 2;
      Io_Write16(RGB(c, c, c), (u16 *)&BG_PLTT_COLOR(15, 3));
      wait(1);
    }
  } else {
    Io_Write16(RGB(31, 31, 31), &PLTT_BACKDROP);
    ScreenTransition_Finish(0x207, 16);
    work->transitionActive = 0;
    wait(16);
    for (i = 0; i < 16; i++) {
      c = 30 - i * 2;
      Io_Write16(RGB(c, c, c), &PLTT_BACKDROP);
      wait(1);
    }
  }
}

void *Sprite_AddAnimationFar(void *, s32);

/// The Broken Seal's Exit_PrepareAnimation for any actor: centres actor id on
/// its cell at the cell's south edge, stops it, hides its shadow and plays
/// animation 25 (side 1) or 26 over 18 frames, its sprite gaining the doorway
/// part (metadata 267).
void Exit_PrepareActorAnimation(s32 id, s32 side) {
  struct Actor *actor;
  struct Sprite *sprite;
  struct SpriteAnimation *doorway;
  s32 animation;

  actor = Actor_Lookup(id);
  sprite = actor->sprite;
  animation = (side != 1);
  animation = 26 - animation;
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
    actor->targetX = TARGET_UNSET;
    actor->targetZ = TARGET_UNSET;
    Actor_SelectAnimationFar(actor, animation);
    wait(18);
  }
}

/// Starts the leader's animation for leaving through an exit: centres it on
/// its tile, at the tile's south edge, stops it and gives it the animation's
/// mode, then waits 18 frames. The leader's sprite gains a second part
/// (metadata 267) for it.
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
    // Each coordinate read and written as the word it is.
    actor->x.fixed = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
    actor->z.fixed = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(16);
    actor->velocityX = cleared;
    actor->velocityZ = cleared;
    actor->targetX = TARGET_UNSET;
    actor->targetZ = TARGET_UNSET;
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
/// extra sprite animation (sprite 267) and walks 8 pixels south.
void Entrance_EndArrivalAnimation(s32 animation) {
  struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  struct Sprite *spriteAnimation = leader->sprite;
  struct SpriteAnimation *eff =
      (struct SpriteAnimation *)Sprite_AddAnimationFar(spriteAnimation,
                                                       SPRITE_DOORWAY);
  spriteAnimation->flags = 0;
  eff->colorMode = SPRITE_COLOR_HIDDEN;
  leader->x.fixed = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  leader->z.fixed &= FIELD_CELL_MASK;
  Actor_SelectAnimationFar(leader, animation);
  wait(30);
  // Back to one sprite part, with its shadow, walking 8 pixels south at 1
  // pixel a frame.
  spriteAnimation->count = 1;
  SpriteAnimation_ClearFar(spriteAnimation->entries[1]);
  spriteAnimation->entries[1] = NULL;
  spriteAnimation->flags = SPRITE_FLAG_SHADOW;
  leader->acceleration = FIXED_ONE;
  leader->speedLimit = FIXED_ONE;
  Actor_SetMoveTargetFar(leader, leader->x.fixed, leader->y.fixed,
                         leader->z.fixed + toFIXED(8));
  Actor_WaitForMoveFar(leader);
}

/// Plays the leader's arrival for the way the last scene was left: through a
/// stairs-up exit (FLAG_ARRIVAL_STAIRS_UP, ANIM_LEADER_ARRIVE, returns 1) or a
/// stairs-down one (FLAG_ARRIVAL_STAIRS_DOWN, ANIM_LEADER_ARRIVE_ALTERNATE,
/// returns 2), or falling in from 160 pixels above (FLAG_ARRIVE_FALLING,
/// returns 3).
s32 Entrance_PlayArrival(void) {
  s32 result = 0;
  s32 flag = FLAG_ARRIVAL_STAIRS_UP;

  if (GameFlag_Get(flag) != 0) {
    Entrance_EndArrivalAnimation(ANIM_LEADER_ARRIVE);
    GameFlag_Clear(flag);
    result = 1;
  } else {
    flag = FLAG_ARRIVAL_STAIRS_DOWN;
    if (GameFlag_Get(flag) != 0) {
      Entrance_EndArrivalAnimation(ANIM_LEADER_ARRIVE_ALTERNATE);
      GameFlag_Clear(flag);
      result = 2;
    } else {
      flag = FLAG_ARRIVE_FALLING;
      if (GameFlag_Get(flag) != 0) {
        s32 id;
        struct Actor *object;

        GameFlag_Clear(flag);
        id = gMainRoutineWork.leaderActorId;
        object = Actor_Lookup(id);
        object->y.fixed += toFIXED(160);
        FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
        while (object->y.fixed + object->velocityY > object->groundHeight) {
          wait(1);
        }
        Sound_PlayFar(SE_DOOR_CLOSE);
        object->y.fixed = object->groundHeight;
        Actor_SelectAnimationFar(object, 22);
        Event_Wait(15);
        FieldCamera_FollowActor(id, 1);
        result = 3;
      }
    }
  }
  return result;
}

/// The exit of kind 0x50: gives the leader's sprite a layer of sprite 268,
/// drawn transparent (SPRITE_COLOR_HIDDEN).
void Exit_AddHiddenLeaderLayer(void) {
  struct Actor *object = Actor_Lookup(gMainRoutineWork.leaderActorId);
  struct SpriteAnimation *layer = Sprite_AddAnimationFar(object->sprite, 268);

  layer->colorMode = SPRITE_COLOR_HIDDEN;
}
