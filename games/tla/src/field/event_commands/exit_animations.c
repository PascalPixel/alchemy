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
  *position++ = x / Q16_ONE;
  *position = z / Q16_ONE;
  if ((object->spriteStorageKind & 15) == 1)
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

void *Sprite_AddAnimationFar(void *, s32);

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

  object = Actor_Lookup(id);
  context = object->sprite;
  mode = (side != 1);
  mode = 26 - mode;
  effect = Sprite_AddAnimationFar(context, 267);
  zero = 0;
  // Through a byte pointer: as context->flags the kind is set first.
  *&context->flags = zero;
  kind = 15;
  effect->param = kind;
  object->x.fixed = (object->x.fixed & 0xfff00000) + px(8);
  object->z.fixed = (object->z.fixed & 0xfff00000) + px(16);
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
  object = Actor_Lookup(party->leaderActorId);
  context = object->sprite;
  effect = Sprite_AddAnimationFar(context, 267);
  zero = 0;
  context->flags = zero;
  /* FAKEMATCH(dead): an empty once-only loop. */
  do {
  } while (0);
  param = 15;
  effect->param = param;
  object->x.fixed = (object->x.fixed & 0xFFF00000) + 0x80000;
  object->z.fixed = (object->z.fixed & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = TARGET_UNSET;
  object->targetZ = TARGET_UNSET;
  Actor_SelectAnimationFar(object, mode);
  wait(18);
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
void Entrance_EndArrivalAnimation(s32 mode) {
  s32 zero;
  struct Actor *obj = Actor_Lookup(gMainRoutineWork.leaderActorId);
  struct ContextSprite *ctx = obj->sprite;
  struct ContextEffectEntry *eff = Sprite_AddAnimationFar(ctx, 0x10b);

  zero = 0;
  ctx->flags = zero;
  eff->param = 15;
  obj->x.fixed = (obj->x.fixed & 0xfff00000) + px(8);
  obj->z.fixed &= 0xfff00000;
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
  Actor_SetMoveTargetFar(obj, obj->x.fixed, obj->y.fixed, obj->z.fixed + px(8));
  Actor_WaitForMoveFar(obj);
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
        object->y.fixed += px(160);
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
void Func_080d5fd4(void) {
  struct Actor *object = Actor_Lookup(gMainRoutineWork.leaderActorId);
  u8 *metadata =
      Sprite_AddAnimationFar(*(void **)((u8 *)object + 0x50), 268);

  metadata[5] = 15;
}
