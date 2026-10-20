/// @file
/// An actor's screen position, the encounter transition, context effects.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

/// Finds where an actor is on screen: position[0] and position[1] receive its
/// x and y in pixels, y at the top of its sprite. Returns 0, or -1 if there is
/// no such actor.
s32 Object_GetScreenPosition(s32 objectId, s32 *position) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);
  s32 *camera;
  s32 cameraX;
  s32 cameraZ;
  s32 x;
  s32 z;

  if (object == 0)
    return -1;
  /* FAKEMATCH(temporary): the view position read as two adjacent words
   * through one pointer, so both loads share one base; as viewX and viewY
   * through gMapWork[0], 28 of 59 instructions differ in tbs-en. */
  camera = &((struct MapScrollWork *)gMapWork[0])->viewX;
  cameraX = camera[0] & 0xffff0000;
  cameraZ = camera[1] & 0xffff0000;
  x = object->x - cameraX;
  z = object->z - cameraZ - object->y;
  *position++ = x / 0x10000;
  *position = z / 0x10000;
  if ((object->animationKind & 15) == 1)
    *position -=
        (s8)Resource_GetMetadataRecordFar(
            ((struct AnimationObject *)object->animation)->entries[0]->animId)
            ->boxX;
  return 0;
}

UNUSED void ObjectEffect_ReservedNoOp941DC(void) {}

// UB: no value is returned, and run_scene.c ignores it; as void, the epilogue
// changes.
/// Starts the transition into a random battle: plays the scene's battle cue
/// and the encounter sounds, closes the screen and flashes the backdrop white,
/// fading it to black over 16 frames.
#ifdef UBFIX
void Field_StartEncounterTransition(s32 unused) {
#else
s32 Field_StartEncounterTransition(s32 unused) {
#endif
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  s32 i;
  s32 level;

  AudioCommand_PlayFar(gGameState.sceneCue);
  AudioCommand_PlayFar(SE_288);
  AudioCommand_PlayFar(SE_147);
  if (work->mode == MODE_WORLD_MAP) {
    // On the world map the flash colours BG palette colour 243 instead.
    Io_Write16(RGB(31, 31, 31), (u16 *)&BG_PLTT_COLOR(15, 3));
    DisplayTransition_Finish(0x401, 16);
    work->transitionActive = 0;
    WaitFrames(16);
    for (i = 0; i < 16; i++) {
      level = 30 - i * 2;
      Io_Write16(RGB(level, level, level), (u16 *)&BG_PLTT_COLOR(15, 3));
      WaitFrames(1);
    }
  } else {
    Io_Write16(RGB(31, 31, 31), &PLTT_BACKDROP);
    DisplayTransition_Finish(0x207, 16);
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
  param = 15;

  context->flags = zero;
  effect->param = param;
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = ACTOR_NO_TARGET;
  object->targetZ = ACTOR_NO_TARGET;
  ObjectDispatch_SelectAnimationFar(object, animation);
  WaitFrames(18);
}

/// Leaves through an exit of kind 0x20 (animation 26); flag 0x120 makes the
/// next scene play the matching arrival (ObjectEffect_RunPendingFlagEvent).
void ObjectEffect_BeginContextEffect26(void) {
  ObjectEffect_PrepareContextEffect(0x1A);
  GameFlag_SetBitFar(0x120);
}

/// Leaves through an exit of kind 0x30 (animation 25), recorded in flag 0x121.
void ObjectEffect_BeginContextEffect25(void) {
  ObjectEffect_PrepareContextEffect(0x19);
  GameFlag_SetBitFar(0x121);
}

/// Plays the leader's arrival animation through an exit: centred on its tile
/// at the tile's north edge, the leader plays animation for 30 frames, loses
/// the extra sprite part and walks 8 pixels south.
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
  eff->param = 15;
  leader->x = (leader->x & mask) + 0x80000;
  leader->z &= mask;
  ObjectDispatch_SelectAnimationFar(leader, animation);
  WaitFrames(30);
  spriteAnimation->count = 1;
  ResourceMetadata_ClearRecordFar(spriteAnimation->entries[1]);
  spriteAnimation->entries[1] = (void *)zero;
  spriteAnimation->flags = 1;
  leader->acceleration = 0x10000;
  leader->speedLimit = 0x10000;
  Object_SetMoveTargetFar(leader, leader->x, leader->y, leader->z + 0x80000);
  FieldObject_WaitForMoveFar(leader);
}

/// Plays the leader's arrival for the way the last scene was left: through an
/// exit of kind 0x20 (flag 0x120, animation 24, returns 1) or 0x30 (flag
/// 0x121, animation 23, returns 2), or falling in from 160 pixels above
/// (flag 0x122, returns 3). Returns 0 when there is nothing to play.
s32 ObjectEffect_RunPendingFlagEvent(void) {
  s32 result = 0;
  s32 flag = 0x120;

  if (GameFlag_TestFar(flag) != 0) {
    ObjectEffect_EndContextEffect(24);
    GameFlag_ClearBitFar(flag);
    result = 1;
  } else {
    flag = 0x121;
    if (GameFlag_TestFar(flag) != 0) {
      ObjectEffect_EndContextEffect(23);
      GameFlag_ClearBitFar(flag);
      result = 2;
    } else {
      flag = 0x122;
      if (GameFlag_TestFar(flag) != 0) {
        s32 leaderId;
        struct ObjectRuntime *leader;

        GameFlag_ClearBitFar(flag);
        leaderId = gGameState.selectedActor;
        leader = ObjectTable_Get(leaderId);
        leader->y += 0x00a00000;
        Camera_MoveWithinMapBounds(-1, -1, -1, 0);
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
