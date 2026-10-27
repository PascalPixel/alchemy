/// @file
/// An actor's screen position, the encounter transition, context
/// effects.
// Part of field_services.c.

#include "../field_services_internal.h"

/// Finds where an actor is on screen: position[0] and position[1]
/// receive its x and y in pixels, y at the top of its sprite. Returns 0, or -1
/// if there is no such actor. Called by the message window code in
/// messages.c to place a speaker's bubble, and through
/// Actor_GetScreenPositionFar by the window frames to open a message
/// window above or below the leader.
s32 Actor_GetScreenPosition(s32 objectId, s32 *position) {
  struct Object *object = Actor_GetByActorId(objectId);
#ifdef UBFIX
  struct MapGridScrollWork *grid;
#else
  s32 *camera;
#endif
  s32 cameraX;
  s32 cameraZ;
  s32 x;
  s32 z;

  if (object == 0)
    return -1;
  // UB: camera[1] crosses the viewX scalar; UBFIX reads the named pair.
#ifdef UBFIX
  grid = gMapGridWork[0];
  cameraX = grid->viewX & 0xffff0000;
  cameraZ = grid->viewY & 0xffff0000;
#else
  /* FAKEMATCH(temporary): the view position read as two adjacent words
   * through one pointer, so both loads share one base; as viewX and viewY
   * through gMapGridWork[0], 28 of 59 instructions differ in tbs-en. */
  camera = &((struct MapGridScrollWork *)gMapGridWork[0])->viewX;
  cameraX = camera[0] & 0xffff0000;
  cameraZ = camera[1] & 0xffff0000;
#endif
  x = object->x - cameraX;
  z = object->z - cameraZ - object->y;
  *position++ = x / 0x10000;
  *position = z / 0x10000;
  if ((object->spriteStorageKind & 15) == OBJECT_SPRITE_ONE)
    *position -= (s8)Resource_GetMetadataRecordFar(
                     object->sprite->entries[0]->animationId)
                     ->boxX;
  return 0;
}

UNUSED void Exit_NoOp(void) {}

// UB: no value is returned, and run_scene.c ignores it; as void, the epilogue
// changes.
/// Starts the transition into a random battle: plays the map's battle music
/// and the encounter sounds, closes the screen and flashes the backdrop white,
/// fading it to black over 16 frames. Called by SceneManager_Run when it
/// handles work->encounter; clears work->transitionActive once the screen is
/// closed.
#ifdef UBFIX
void Field_StartEncounterTransition(s32 unused) {
#else
s32 Field_StartEncounterTransition(s32 unused) {
#endif
  struct FieldWork *work = gFieldWork;
  s32 i;
  s32 level;

  Sound_PlaySongFar(gSaveBlock.battleMusic);
  Sound_PlaySongFar(SE_STOP_ACTION_SE);
  Sound_PlaySongFar(SE_BEAST_CRY);
  if (work->mode == OBJECT_SYSTEM_WORLD_MAP) {
    // On the world map the flash colours BG palette colour 243 instead, and
    // the screen closes on the flickering centre line (style 4, variant 1);
    // a field map closes with WINDOW_TRANSITION_LEADER_INVERSE. Both take
    // 16 frames.
    Io_Write16(RGB(31, 31, 31), (u16 *)&BG_PLTT_COLOR(15, 3));
    ScreenTransition_Finish(SCENE_TRANSITION(4, 1), 16);
    work->transitionActive = 0;
    WaitFrames(16);
    // Step the white down by 2 a frame: 30 to 0 in 16 frames.
    for (i = 0; i < 16; i++) {
      level = 30 - i * 2;
      Io_Write16(RGB(level, level, level), (u16 *)&BG_PLTT_COLOR(15, 3));
      WaitFrames(1);
    }
  } else {
    Io_Write16(RGB(31, 31, 31), &PLTT_BACKDROP);
    ScreenTransition_Finish(
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER_INVERSE),
        16);
    work->transitionActive = 0;
    WaitFrames(16);
    for (i = 0; i < 16; i++) {
      level = 30 - i * 2;
      Io_Write16(RGB(level, level, level), &PLTT_BACKDROP);
      WaitFrames(1);
    }
  }
}

/// Starts the leader's animation for leaving through an exit: centres it on
/// its cell, at the cell's south edge, stops it and plays animation over 18
/// frames. The leader's sprite gains a second part (metadata 27) for it.
/// Called by the two ObjectEffect_StartContextEffect functions below.
void Exit_PrepareAnimation(s32 animation) {
  u32 zero;
  u8 param;
  struct Object *object;
  struct Sprite *context;
  struct SpriteAnim *effect;

  object = Actor_GetByActorId(gSaveBlock.leaderActorId);
  context = object->sprite;
  effect = (struct SpriteAnim *)Sprite_AddAnimFar(context, 27);
  zero = 0;
  param = SPRITE_COLOR_HIDDEN;

  // No shadow as the leader leaves.
  context->flags = zero;
  effect->colorMode = param;
  // Positions are 16.16 fixed point and cells 16 pixels: keep the cell and
  // put the leader at its middle in x (8 pixels) and its south edge in z.
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = ACTOR_NO_TARGET;
  object->targetZ = ACTOR_NO_TARGET;
  Object_SelectAnimationFar(object, animation);
  WaitFrames(18);
}

/// Leaves through a EXIT_STYLE_STAIRS_UP exit (ANIM_LEADER_LEAVE);
/// FLAG_ARRIVAL_STAIRS_UP makes the next map play the matching arrival
/// (Entrance_PlayArrival). Called by the exit handling in
/// overworld/triggers.c, after it plays SE_STAIRS_UP.
void Exit_StartStairsUp(void) {
  Exit_PrepareAnimation(ANIM_LEADER_LEAVE);
  GameFlag_SetFar(FLAG_ARRIVAL_STAIRS_UP);
}

/// Leaves through a EXIT_STYLE_STAIRS_DOWN exit
/// (ANIM_LEADER_LEAVE_ALTERNATE), recorded in
/// FLAG_ARRIVAL_STAIRS_DOWN. Called by the exit handling in
/// overworld/triggers.c, after it plays SE_STAIRS_DOWN.
void Exit_StartStairsDown(void) {
  Exit_PrepareAnimation(ANIM_LEADER_LEAVE_ALTERNATE);
  GameFlag_SetFar(FLAG_ARRIVAL_STAIRS_DOWN);
}

/// Plays the leader's arrival animation through an exit: centred on its cell
/// at the cell's north edge, the leader plays animation for 30 frames, loses
/// the extra sprite part and walks 8 pixels south. Called by
/// Entrance_PlayArrival; frees the sprite part
/// Exit_PrepareAnimation registered.
void Entrance_EndArrivalAnimation(s32 animation) {
  s32 zero;
  s32 mask;
  struct Object *leader = Actor_GetByActorId(gSaveBlock.leaderActorId);
  struct Sprite *spriteAnimation = leader->sprite;
  struct SpriteAnim *eff =
      (struct SpriteAnim *)Sprite_AddAnimFar(spriteAnimation, 27);

  zero = 0;
  mask = 0xfff00000;
  spriteAnimation->flags = zero;
  eff->colorMode = SPRITE_COLOR_HIDDEN;
  leader->x = (leader->x & mask) + 0x80000;
  leader->z &= mask;
  Object_SelectAnimationFar(leader, animation);
  WaitFrames(30);
  // Back to one sprite part, with its shadow, walking 8 pixels south at 1
  // pixel a frame.
  spriteAnimation->count = 1;
  SpriteAnim_ClearFar(spriteAnimation->entries[1]);
  spriteAnimation->entries[1] = (void *)zero;
  spriteAnimation->flags = SPRITE_FLAG_SHADOW;
  leader->acceleration = Q16_ONE;
  leader->speedLimit = Q16_ONE;
  Object_SetMoveTargetFar(leader, leader->x, leader->y, leader->z + 0x80000);
  Object_WaitForMoveFar(leader);
}

/// Plays the leader's arrival for the way the last map was left: through a
/// EXIT_STYLE_STAIRS_UP exit (FLAG_ARRIVAL_STAIRS_UP,
/// ANIM_LEADER_ARRIVE, returns 1) or a EXIT_STYLE_STAIRS_DOWN one
/// (FLAG_ARRIVAL_STAIRS_DOWN, ANIM_LEADER_ARRIVE_ALTERNATE, returns
/// 2), or falling in from 160 pixels above (FLAG_ARRIVE_FALLING, returns 3).
/// Returns 0 when there is nothing to play. Called by SceneManager_Run as the
/// map fades in, which waits out the fade itself only when this returns 0.
/// Clears the flag it acts on.
s32 Entrance_PlayArrival(void) {
  s32 result = 0;
  s32 flag = FLAG_ARRIVAL_STAIRS_UP;

  if (GameFlag_GetFar(flag) != 0) {
    Entrance_EndArrivalAnimation(ANIM_LEADER_ARRIVE);
    GameFlag_ClearFar(flag);
    result = 1;
  } else {
    flag = FLAG_ARRIVAL_STAIRS_DOWN;
    if (GameFlag_GetFar(flag) != 0) {
      Entrance_EndArrivalAnimation(ANIM_LEADER_ARRIVE_ALTERNATE);
      GameFlag_ClearFar(flag);
      result = 2;
    } else {
      flag = FLAG_ARRIVE_FALLING;
      if (GameFlag_GetFar(flag) != 0) {
        s32 leaderId;
        struct Object *leader;

        GameFlag_ClearFar(flag);
        leaderId = gSaveBlock.leaderActorId;
        leader = Actor_GetByActorId(leaderId);
        // FLAG_ARRIVE_FALLING is set by maps the party drops out of (the
        // Suhalla storm, a floor giving way in the Tret tree): start 160 pixels
        // up and let gravity bring the leader down to the ground.
        leader->y += 0x00a00000;
        FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
        // Until the next frame's fall would reach the ground.
        while (leader->y + leader->velocityY > leader->groundHeight) {
          WaitFrames(1);
        }
        Sound_PlaySongFar(SE_DOOR_CLOSE);
        leader->y = leader->groundHeight;
        Object_SelectAnimationFar(leader, ANIM_LEADER_LEAN_BACK);
        FieldScript_WaitFrames(15);
        FieldCamera_FollowActor(leaderId, 1);
        result = 3;
      }
    }
  }
  return result;
}
