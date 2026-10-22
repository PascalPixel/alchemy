/// @file
/// An actor's screen position, the encounter transition, context effects.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// Finds where an actor is on screen: position[0] and position[1] receive its
/// x and y in pixels, y at the top of its sprite. Returns 0, or -1 if there is
/// no such actor. Called by the message window code in messages.c to place a
/// speaker's bubble, and through Object_GetScreenPositionFar by the window
/// frames to open a message window above or below the leader.
s32 Object_GetScreenPosition(s32 objectId, s32 *position) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);
#ifdef UBFIX
  struct MapScrollWork *map;
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
  map = gMapWork[0];
  cameraX = map->viewX & 0xffff0000;
  cameraZ = map->viewY & 0xffff0000;
#else
  /* FAKEMATCH(temporary): the view position read as two adjacent words
   * through one pointer, so both loads share one base; as viewX and viewY
   * through gMapWork[0], 28 of 59 instructions differ in tbs-en. */
  camera = &((struct MapScrollWork *)gMapWork[0])->viewX;
  cameraX = camera[0] & 0xffff0000;
  cameraZ = camera[1] & 0xffff0000;
#endif
  x = object->x - cameraX;
  z = object->z - cameraZ - object->y;
  *position++ = x / 0x10000;
  *position = z / 0x10000;
  if ((object->animationKind & 15) == OBJECT_SPRITE_ONE)
    *position -=
        (s8)Resource_GetMetadataRecordFar(
            ((struct AnimationObject *)object->animation)->entries[0]->animId)
            ->boxX;
  return 0;
}

/// Unused: an empty function nothing calls.
UNUSED void ObjectEffect_ReservedNoOp941DC(void) {}

// UB: no value is returned, and run_scene.c ignores it; as void, the epilogue
// changes.
/// Starts the transition into a random battle: plays the scene's battle cue
/// and the encounter sounds, closes the screen and flashes the backdrop white,
/// fading it to black over 16 frames. Called by Field_RunScene when it handles
/// work->battle; clears work->transitionActive once the screen is closed.
#ifdef UBFIX
void Field_StartEncounterTransition(s32 unused) {
#else
s32 Field_StartEncounterTransition(s32 unused) {
#endif
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  s32 i;
  s32 level;

  AudioCommand_PlayFar(gGameState.sceneCue);
  AudioCommand_PlayFar(SE_STOP_SLOT3);
  AudioCommand_PlayFar(SE_147);
  if (work->mode == MODE_WORLD_MAP) {
    // On the world map the flash colours BG palette colour 243 instead, and
    // the screen closes on the flickering centre line (style 4, variant 1);
    // a field scene closes with WINDOW_TRANSITION_LEADER_INVERSE. Both take
    // 16 frames.
    Io_Write16(RGB(31, 31, 31), (u16 *)&BG_PLTT_COLOR(15, 3));
    DisplayTransition_Finish(SCENE_TRANSITION(4, 1), 16);
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
    DisplayTransition_Finish(
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
/// its tile, at the tile's south edge, stops it and plays animation over 18
/// frames. The leader's sprite gains a second part (metadata 27) for it.
/// Called by the two ObjectEffect_BeginContextEffect functions below.
void ObjectEffect_PrepareContextEffect(s32 animation) {
  u32 zero;
  u8 param;
  struct ObjectRuntime *object;
  struct AnimationObject *context;
  struct AnimationEntry *effect;

  object = ObjectTable_Get(gGameState.selectedActor);
  context = object->animation;
  effect = (struct AnimationEntry *)ResourceMetadata_RegisterFar(context, 27);
  zero = 0;
  param = SPRITE_COLOR_HIDDEN;

  // No shadow as the leader leaves.
  context->flags = zero;
  effect->param = param;
  // Positions are 16.16 fixed point and tiles 16 pixels: keep the tile and
  // put the leader at its middle in x (8 pixels) and its south edge in z.
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = ACTOR_NO_TARGET;
  object->targetZ = ACTOR_NO_TARGET;
  ObjectDispatch_SelectAnimationFar(object, animation);
  WaitFrames(18);
}

/// Leaves through an EXIT_STYLE_ANIMATED exit (ANIM_LEADER_LEAVE);
/// FLAG_EXIT_ARRIVAL_PENDING makes the next scene play the matching arrival
/// (ObjectEffect_RunPendingFlagEvent). Called by the exit handling in events.c,
/// after it plays SE_STAIRS_UP.
void ObjectEffect_BeginContextEffect26(void) {
  ObjectEffect_PrepareContextEffect(ANIM_LEADER_LEAVE);
  GameFlag_SetBitFar(FLAG_EXIT_ARRIVAL_PENDING);
}

/// Leaves through an EXIT_STYLE_ANIMATED_ALTERNATE exit
/// (ANIM_LEADER_LEAVE_ALTERNATE), recorded in FLAG_EXIT_ARRIVAL_ALTERNATE.
/// Called by the exit handling in events.c, after it plays SE_STAIRS_DOWN.
void ObjectEffect_BeginContextEffect25(void) {
  ObjectEffect_PrepareContextEffect(ANIM_LEADER_LEAVE_ALTERNATE);
  GameFlag_SetBitFar(FLAG_EXIT_ARRIVAL_ALTERNATE);
}

/// Plays the leader's arrival animation through an exit: centred on its tile
/// at the tile's north edge, the leader plays animation for 30 frames, loses
/// the extra sprite part and walks 8 pixels south. Called by
/// ObjectEffect_RunPendingFlagEvent; frees the sprite part
/// ObjectEffect_PrepareContextEffect registered.
void ObjectEffect_EndContextEffect(s32 animation) {
  s32 zero;
  s32 mask;
  struct ObjectRuntime *leader = ObjectTable_Get(gGameState.selectedActor);
  struct AnimationObject *spriteAnimation = leader->animation;
  struct AnimationEntry *eff =
      (struct AnimationEntry *)ResourceMetadata_RegisterFar(spriteAnimation,
                                                            27);

  zero = 0;
  mask = 0xfff00000;
  spriteAnimation->flags = zero;
  eff->param = SPRITE_COLOR_HIDDEN;
  leader->x = (leader->x & mask) + 0x80000;
  leader->z &= mask;
  ObjectDispatch_SelectAnimationFar(leader, animation);
  WaitFrames(30);
  // Back to one sprite part, with its shadow, walking 8 pixels south at 1
  // pixel a frame.
  spriteAnimation->count = 1;
  ResourceMetadata_ClearRecordFar(spriteAnimation->entries[1]);
  spriteAnimation->entries[1] = (void *)zero;
  spriteAnimation->flags = SPRITE_FLAG_SHADOW;
  leader->acceleration = 0x10000;
  leader->speedLimit = 0x10000;
  Object_SetMoveTargetFar(leader, leader->x, leader->y, leader->z + 0x80000);
  FieldObject_WaitForMoveFar(leader);
}

/// Plays the leader's arrival for the way the last scene was left: through an
/// EXIT_STYLE_ANIMATED exit (FLAG_EXIT_ARRIVAL_PENDING, ANIM_LEADER_ARRIVE,
/// returns 1) or an EXIT_STYLE_ANIMATED_ALTERNATE one
/// (FLAG_EXIT_ARRIVAL_ALTERNATE, ANIM_LEADER_ARRIVE_ALTERNATE, returns 2), or
/// falling in from 160 pixels above (FLAG_ARRIVE_FALLING, returns 3). Returns 0
/// when there is nothing to play. Called by Field_RunScene as the scene fades
/// in, which waits out the fade itself only when this returns 0. Clears the
/// flag it acts on.
s32 ObjectEffect_RunPendingFlagEvent(void) {
  s32 result = 0;
  s32 flag = FLAG_EXIT_ARRIVAL_PENDING;

  if (GameFlag_TestFar(flag) != 0) {
    ObjectEffect_EndContextEffect(ANIM_LEADER_ARRIVE);
    GameFlag_ClearBitFar(flag);
    result = 1;
  } else {
    flag = FLAG_EXIT_ARRIVAL_ALTERNATE;
    if (GameFlag_TestFar(flag) != 0) {
      ObjectEffect_EndContextEffect(ANIM_LEADER_ARRIVE_ALTERNATE);
      GameFlag_ClearBitFar(flag);
      result = 2;
    } else {
      flag = FLAG_ARRIVE_FALLING;
      if (GameFlag_TestFar(flag) != 0) {
        s32 leaderId;
        struct ObjectRuntime *leader;

        GameFlag_ClearBitFar(flag);
        leaderId = gGameState.selectedActor;
        leader = ObjectTable_Get(leaderId);
        // FLAG_ARRIVE_FALLING is set by scenes the party drops out of (the
        // Suhalla storm, a floor giving way in the Tret tree): start 160 pixels
        // up and let gravity bring the leader down to the ground.
        leader->y += 0x00a00000;
        Camera_MoveWithinMapBounds(-1, -1, -1, 0);
        // Until the next frame's fall would reach the ground.
        while (leader->y + leader->velocityY > leader->terrainHeight) {
          WaitFrames(1);
        }
        AudioCommand_PlayFar(SE_159);
        leader->y = leader->terrainHeight;
        ObjectDispatch_SelectAnimationFar(leader, 22);
        FieldEvent_WaitFrames(15);
        Camera_FollowActor(leaderId, 1);
        result = 3;
      }
    }
  }
  return result;
}
