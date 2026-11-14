/// @file
/// An actor's screen position, the encounter transition, and the context
/// effects of leaving through an exit and arriving.
// Part of event_commands.c.

#include "../event_commands_internal.h"
#include "sounds.h"
#include "module/field/field.h"
#include "fixed_math.h"
#include "flags.h"

/// Writes the object's position on screen, in pixels, to position[0] and
/// position[1]; an animated object is raised by its frame box. Returns -1 when
/// the object does not exist.
s32 Actor_GetScreenPosition(s32 actorId, s32 *position) {
  struct Actor *object = ObjectTable_Get(actorId);
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
  map = Ram_HeapSlots->mapWork;
  cameraX = map->viewX & 0xffff0000;
  cameraZ = map->viewZ & 0xffff0000;
#else
  camera = &Ram_HeapSlots->mapWork->viewX;
  cameraX = camera[0] & 0xffff0000;
  cameraZ = camera[1] & 0xffff0000;
#endif
  x = object->x - cameraX;
  z = object->z - cameraZ - object->y;
  *position++ = x / Q16_ONE;
  *position = z / Q16_ONE;
  if ((object->animationKind & 15) == 1)
    *position -= (s8)Resource_GetMetadataRecordFarFar(
                     *((struct ObjectAnimationState *)object->sprite)->value28)
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
  Sound_PlayFar(SE_STOP_MUSIC_PLAYER_5);
  Sound_PlayFar(SE_BEAST_CRY);
  if (work->mode == 3) {
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

void *ResourceMetadata_RegisterFar(void *, s32);

/// The Broken Seal's Exit_PrepareAnimation for any actor: centres actor id on
/// its cell at the cell's south edge, stops it, hides its shadow and plays
/// animation 25 (side 1) or 26 over 18 frames, its sprite gaining the doorway
/// part (metadata 267).
void Exit_PrepareActorAnimation(s32 id, s32 side) {
  u32 zero;
  u8 kind;
  struct Actor *object;
  struct ContextAnimation *context;
  struct ContextEffect *effect;
  s32 mode;

  object = ObjectTable_Get(id);
  context = object->sprite;
  mode = (side != 1);
  mode = 26 - mode;
  effect = ResourceMetadata_RegisterFar(context, 267);
  zero = 0;
  // Through a byte pointer: as context->flags the kind is set first.
  *&context->flags = zero;
  kind = 15;
  effect->param = kind;
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = TARGET_UNSET;
  object->targetZ = TARGET_UNSET;
  Actor_SelectAnimationFar(object, mode);
  wait(18);
}

/// Starts the leader's animation for leaving through an exit: centres it on
/// its tile, at the tile's south edge, stops it and gives it the animation's
/// mode, then waits 18 frames. The leader's sprite gains a second part
/// (metadata 267) for it.
void Exit_PrepareAnimation(s32 animation) {
  u32 zero;
  u8 param;
  struct Actor *object;
  struct ContextAnimation *context;
  struct ContextEffect *effect;
  struct MainRoutineWork *party;
  s32 mode;

  party = &gMainRoutineWork;
  mode = animation;
  object = ObjectTable_Get(party->leaderActorId);
  context = object->sprite;
  effect = ResourceMetadata_RegisterFar(context, 267);
  zero = 0;
  context->flags = zero;
  /* FAKEMATCH(dead): an empty once-only loop. */
  do {
  } while (0);
  param = 15;
  effect->param = param;
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = TARGET_UNSET;
  object->targetZ = TARGET_UNSET;
  Actor_SelectAnimationFar(object, mode);
  wait(18);
}

void Exit_StartStairsUp(void) {
  Exit_PrepareAnimation(0x1A);
  GameFlag_Set(FLAG_ARRIVAL_STAIRS_UP);
}

void Exit_StartStairsDown(void) {
  Exit_PrepareAnimation(0x19);
  GameFlag_Set(FLAG_ARRIVAL_STAIRS_DOWN);
}

void Entrance_EndArrivalAnimation(s32 mode) {
  s32 zero;
  struct Actor *obj = ObjectTable_Get(gMainRoutineWork.leaderActorId);
  struct ContextSprite *ctx = obj->sprite;
  struct ContextEffectEntry *eff = ResourceMetadata_RegisterFar(ctx, 0x10b);

  zero = 0;
  ctx->flags = zero;
  eff->param = 15;
  obj->x = (obj->x & 0xfff00000) + 0x80000;
  obj->z &= 0xfff00000;
  Actor_SelectAnimationFar(obj, mode);
  wait(30);
  ctx->count = 1;
  SpriteAnimation_ClearFar(ctx->entries[1]);
  {
    /* FAKEMATCH(pin): the sprite copied into r1 for its two stores. */
    register struct ContextSprite *pinnedCtx asm("r1") = ctx;

    pinnedCtx->flags = 1;
    pinnedCtx->entries[1] = (void *)zero;
  }
  obj->acceleration = px(1);
  obj->speedLimit = px(1);
  Actor_SetMoveTargetFar(obj, obj->x, obj->y, obj->z + 0x80000);
  Actor_WaitForMoveFar(obj);
}

s32 Entrance_PlayArrival(void) {
  s32 result = 0;
  s32 flag = 0x120;

  if (GameFlag_Get(flag) != 0) {
    Entrance_EndArrivalAnimation(24);
    GameFlag_Clear(flag);
    result = 1;
  } else {
    flag = 0x121;
    if (GameFlag_Get(flag) != 0) {
      Entrance_EndArrivalAnimation(23);
      GameFlag_Clear(flag);
      result = 2;
    } else {
      flag = 0x122;
      if (GameFlag_Get(flag) != 0) {
        s32 id;
        struct Actor *object;

        GameFlag_Clear(flag);
        id = gMainRoutineWork.leaderActorId;
        object = ObjectTable_Get(id);
        object->y += px(160);
        FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
        while (object->y + object->velocityY > object->terrainHeight) {
          wait(1);
        }
        Sound_PlayFar(SE_DOOR_CLOSE);
        object->y = object->terrainHeight;
        Actor_SelectAnimationFar(object, 22);
        Event_Wait(15);
        FieldCamera_FollowActor(id, 1);
        result = 3;
      }
    }
  }
  return result;
}

void Func_080d5fd4(void) {
  struct Actor *object = ObjectTable_Get(gMainRoutineWork.leaderActorId);
  u8 *metadata =
      ResourceMetadata_RegisterFar(*(void **)((u8 *)object + 0x50), 268);

  metadata[5] = 15;
}
