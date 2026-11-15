/// @file
/// Lemuria's palace, where King Hydros and his council receive the party and
/// show it two maps, the ancient world and the one Lunpa had.

#include "shared/tables.h"
#include "sprite_ids.h"
#include "field/field_work.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/iwram_call.h"
#include "system/vector3.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "field/event_commands.h"
#include "scene/scene.h"
#include "system/task.h"
#include "field/call.h"
#include "system/io_write.h"
#include "macros.h"
#include "text/msg_ids.h"

/// The palace, by the rows of gActorPlacements; the rows for Jenna, Sheba
/// and Piers go to their party slots.
enum LemuriaPalaceActorId {
  /// "He is still in chambers with Lord Conservato."
  ACTOR_GIRL_WAITING_ON_THE_KING = ACTOR_FIRST_PLACED,
  /// "If you seek the counsel of His Highness, the king, follow this
  /// passage..."
  ACTOR_WOMAN_SHOWING_THE_WAY,
  /// "His Highness never leaves a meeting with Conservato in a good mood."
  ACTOR_MAN_ON_THE_KINGS_MOOD,
  /// "Conservato and the king were yelling so loudly that I could hear
  /// them..."
  ACTOR_LEMURIAN_WHO_HEARD_THE_YELLING,
  /// "King Hydros is the wisest of all the Lemurian kings..."
  ACTOR_LEMURIAN_PRAISING_HYDROS,
  ACTOR_HYDROS,
  /// "So it would, Your Highness." "There is no need, Lunpa."
  ACTOR_LUNPA,
  ACTOR_CONSERVATO,
  /// "You are Lord Felix, correct?"
  ACTOR_PALACE_GUARD,
  ACTOR_KRADEN
};

extern s32 gLemuriaPalace_Doors[];
extern s32 gLemuriaPalace_InnerDoor[];

void LemuriaPalace_DrawMaps(void);

void Field_StartColorTransition();

extern s16 gLemuriaPalace_DoorOpening[];

/// The sprite each of the two maps is drawn as.
extern s32 gLemuriaPalace_MapTime;
extern u8 gLemuriaPalace_MapFadeLevels[];
extern s32 gLemuriaPalace_FirstMapShown;
extern s32 gLemuriaPalace_SecondMapShown;
extern struct EffectSprite gLemuriaPalace_FirstMapSprite;
extern struct EffectSprite gLemuriaPalace_SecondMapSprite;
extern const u8 gLemuriaPalace_FirstMapTiles[];
extern const u8 gLemuriaPalace_SecondMapTiles[];
extern s32 gLemuriaPalace_SecondMapVram;
extern s32 gLemuriaPalace_FirstMapTile;
extern s32 gLemuriaPalace_SecondMapTile;

void Event_Start();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// A palace door opens with its cell animation and the leader walks through it.
void LemuriaPalace_OpenDoorAndEnter(struct DoorStep *step, s32 countdown,
                                    s32 unused) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(step->script, step->x, step->z);
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = ACTOR_FALLS;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_CenterAndWalk(gMainRoutineWork.leaderActorId, 2, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void LemuriaPalace_EnterDoor(s32 kind) {
  const u8 *data = gLemuriaPalace_Doors;

  if (kind == 3)
    data += 8;
  if (kind == 16)
    data = gLemuriaPalace_InnerDoor;
  LemuriaPalace_OpenDoorAndEnter(data, kind, 0);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

extern s16 gLemuriaPalace_GlowColors[];

/// Every sixth frame rotates colours 1-14 of background palette 12 by one
/// place; every frame sets its colour 15 from a twelve-step glow.
void LemuriaPalace_CycleGlowColors(void) {
  /* FAKEMATCH(temporary): one colour local for the rotated colour and the
   * glow's. */
  s32 color;

  if (gFrameCount % 6 == 0) {
    s32 index;

    color = (s16)BG_PLTT_COLOR(12, 14);
    index = 14;
    do {
      BG_PLTT[12 * 16 + index] = BG_PLTT[12 * 16 + index - 1];
    } while (--index != 1);
    BG_PLTT_COLOR(12, 1) = color;
  }
  color = gLemuriaPalace_GlowColors[(gFrameCount >> 2) % 12];
  BG_PLTT_COLOR(12, 15) = color;
}

/// The room's setup: it schedules the palette cycle
/// LemuriaPalace_CycleGlowColors.
s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Task_AddFrame(LemuriaPalace_CycleGlowColors, TASK_PASS_FRAME);
  FieldActor_SetSpritePriority(ACTOR_GIRL_WAITING_ON_THE_KING, 3);
  if (gMainRoutineWork.entrance == 6)
    Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
  if (Event_GetFlag(0x96a)) {
    Actor_SetPosition(ACTOR_PIERS, 0, 0);
    Actor_SetPosition(ACTOR_CONSERVATO, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Actor 13 raises a hand and two sparks fly up, as each map is called up. The
/// caller passes the actor, but the sparks always come from actor 13.
void LemuriaPalace_CastMapSparks(s32 actorId) {
  struct Actor *object = Actor_Get(ACTOR_HYDROS);
  struct Actor *first;
  struct Actor *second;

  Actor_SetAnimation(ACTOR_HYDROS, 5);
  FieldEvent_Wait(28);
  Event_PlaySound(SE_MENU_CONFIRM);
  first = Actor_Spawn(674, object->x.fixed + px(10), object->y.fixed + px(16),
                      object->z.fixed);
  first->speedLimit = px(2);
  first->acceleration = px(2);
  first->motionFlags = 0;
  FieldActor_SetMoveTargetFixed(first, first->x.fixed + px(6),
                                first->y.fixed + px(8), first->z.fixed);
  second = Actor_Spawn(674, object->x.fixed + px(10), object->y.fixed + px(16),
                       object->z.fixed);
  second->speedLimit = px(2);
  second->acceleration = px(2);
  second->motionFlags = 0;
  FieldActor_SetMoveTargetFixed(second, first->x.fixed + px(8),
                                first->y.fixed - px(4), first->z.fixed);
  FieldActor_WaitForMove(first);
  Actor_Release(first);
  FieldActor_WaitForMove(second);
  Actor_Release(second);
  FieldEvent_Wait(30);
}

extern s8 gLemuriaPalace_MapMode;
extern u32 gLemuriaPalace_MapCrossfade[];

/// A task: draws the maps the hall shows, fading between them by the map mode
/// (1, the fade level by frame; 2, the cross-fade that flickers between them).
void LemuriaPalace_DrawMaps(void) {
  struct Vector3 position;
  s8 mode = gLemuriaPalace_MapMode;

  if (mode == 1) {
    s32 level =
        gLemuriaPalace_MapFadeLevels[(gLemuriaPalace_MapTime >> 3) & 15];

    Io_Write16(level << 8 | (16 - level), &REG_BLDALPHA);
    gLemuriaPalace_MapTime++;
  } else if (mode == 2) {
    s32 *time = &gLemuriaPalace_MapTime;
    s32 step = (*time >> 2) % 28;

    {
      u16 *table = (u16 *)gLemuriaPalace_MapCrossfade;
      /* FAKEMATCH(temporary): the offset in a local puts the table first in the
       * load. */
      s32 at = step * 2;
      s32 value = *(u16 *)((u8 *)table + at);

      REG_BLDALPHA = value;
    }
    if (step == 0) {
      gLemuriaPalace_FirstMapShown = 1;
      gLemuriaPalace_SecondMapShown = step;
      Event_PlaySound(SE_HIGH_JUMP);
    }
    if (step == 14) {
      gLemuriaPalace_FirstMapShown = 0;
      gLemuriaPalace_SecondMapShown = 1;
      Event_PlaySound(SE_HIGH_JUMP);
    }
    (*time)++;
  }
  position.x = px(832);
  position.y = 0;
  position.z = px(448);
  Camera_WorldToScreen(&position);
  if (gLemuriaPalace_FirstMapShown != 0) {
    gLemuriaPalace_FirstMapSprite.x = position.x;
    gLemuriaPalace_FirstMapSprite.y = position.z;
    EffectSprite_Draw(&gLemuriaPalace_FirstMapSprite);
  }
  if (gLemuriaPalace_SecondMapShown != 0) {
    gLemuriaPalace_SecondMapSprite.x = position.x;
    gLemuriaPalace_SecondMapSprite.y = position.z;
    EffectSprite_Draw(&gLemuriaPalace_SecondMapSprite);
  }
}

extern s32 gLemuriaPalace_FirstMapVram;

/// Loads the two maps the council shows the party, the ancient world and
/// Lunpa's, and starts drawing them.
void LemuriaPalace_LoadMaps(void) {
  /* FAKEMATCH(wrapper): forwarded. */
  void *buffer = (void *)Value1((s32 (*)())Heap_Alloc, 0x1000);
  s32 *firstSlot;
  s32 *firstTile;
  s32 *secondSlot;
  s32 *secondTile;

  Io_Write16(0x3f40, &REG_BLDCNT);
  Io_Write16(0x1000, &REG_BLDALPHA);
  Decompress_TaggedLz(gLemuriaPalace_FirstMapTiles, buffer);
  firstSlot = &gLemuriaPalace_FirstMapVram;
  *firstSlot = VramCache_FindFreeEntry();
  firstTile = &gLemuriaPalace_FirstMapTile;
  *firstTile = Vram_Load(gLemuriaPalace_FirstMapVram, 0x1000, buffer);
  EffectSprite_Init(&gLemuriaPalace_FirstMapSprite, 0, 0, 0xc0000000,
                    gLemuriaPalace_FirstMapTile);
  gLemuriaPalace_FirstMapSprite.depth = 1;
  gLemuriaPalace_FirstMapSprite.oam.priority = 2;
  gLemuriaPalace_FirstMapSprite.oam.objectMode = 1;
  gLemuriaPalace_FirstMapSprite.oam.fullColor = 1;
  gLemuriaPalace_FirstMapSprite.oam.palette = 0;
  Decompress_TaggedLz(gLemuriaPalace_SecondMapTiles, buffer);
  secondSlot = &gLemuriaPalace_SecondMapVram;
  *secondSlot = VramCache_FindFreeEntry();
  secondTile = &gLemuriaPalace_SecondMapTile;
  *secondTile = Vram_Load(gLemuriaPalace_SecondMapVram, 0x1000, buffer);
  EffectSprite_Init(&gLemuriaPalace_SecondMapSprite, 0, 0, 0xc0000000,
                    gLemuriaPalace_SecondMapTile);
  gLemuriaPalace_SecondMapSprite.depth = 2;
  gLemuriaPalace_SecondMapSprite.oam.priority = 2;
  gLemuriaPalace_SecondMapSprite.oam.objectMode = 1;
  gLemuriaPalace_SecondMapSprite.oam.fullColor = 1;
  gLemuriaPalace_SecondMapSprite.oam.palette = 0;
  gLemuriaPalace_FirstMapShown = 0;
  gLemuriaPalace_SecondMapShown = 0;
  gLemuriaPalace_MapTime = 0;
  gLemuriaPalace_MapMode = 0;
  Task_AddFrame(LemuriaPalace_DrawMaps, TASK_PASS_VBLANK);
  Heap_Release(buffer);
}

/// Stops drawing the maps and frees their tiles.
void LemuriaPalace_FreeMaps(void) {
  FieldTask_Remove(LemuriaPalace_DrawMaps);
  VramCache_FreeEntry(gLemuriaPalace_FirstMapVram);
  VramCache_FreeEntry(gLemuriaPalace_SecondMapVram);
}

static __inline__ void Blend_WriteAlpha(s32 value) { REG_BLDALPHA = value; }

#define BLEND_SET_ALPHA(value)                                                 \
  do {                                                                         \
    Blend_WriteAlpha(value);                                                   \
  } while (0)

/// Fades the shown map in over 16 steps of four frames.
void LemuriaPalace_FadeMapsIn(void) {
  s32 i;

  BLEND_SET_ALPHA(0x1000);
  gLemuriaPalace_MapMode = 0;
  i = 0;
loop:
  BLEND_SET_ALPHA(((16 - i) << 8) | i);
  System_Wait(4);
  i++;
  if (i < 16)
    goto loop;
  BLEND_SET_ALPHA(16);
  gLemuriaPalace_MapMode = 1;
}

/// The reverse fade, in steps of three frames.
void LemuriaPalace_FadeMapsOut(void) {
  s32 i;

  BLEND_SET_ALPHA(16);
  gLemuriaPalace_MapMode = 0;
  i = 0;
loop:
  BLEND_SET_ALPHA((i << 8) | (16 - i));
  System_Wait(3);
  i++;
  if (i < 16)
    goto loop;
  BLEND_SET_ALPHA(0x1000);
}

void Event_Start();
void Field_StartPaletteTransition();

/// In the palace the party is stopped by a guard, then brought before King
/// Hydros and his council, who have been waiting for the warriors Piers spoke
/// of.
/// "You are Lord Felix, correct?"
s32 LemuriaPalace_MeetKingHydros(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 answer, frame, i;
  s32 result;
  struct Actor *target;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaPalaceYouAreLordFelixCorrect);
  Actor_SetSpeed(ACTOR_PALACE_GUARD, 1.2);
  Actor_WalkByAndWait(ACTOR_PALACE_GUARD, -24, 0);
  Actor_FaceDirectionAndWait(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHWEST);
  Actor_FaceActor(ACTOR_PALACE_GUARD, gMainRoutineWork.leaderActorId, 0);
  // "You are Lord Felix, correct?"
  Event_OpenMessage(ACTOR_PALACE_GUARD, 0);
  answer = Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
  if (answer == 0)
    goto answeredNo;
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PALACE_GUARD, 4);
  Event_ShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 5);
  Actor_WalkByAndWait(gMainRoutineWork.leaderActorId, 0, 16);
  Actor_WalkByAndWait(ACTOR_PALACE_GUARD, 24, 0);
  Actor_FaceDirectionAndWait(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHWEST);
  result = Event_End();
  goto done;
answeredNo:
  Actor_SetAnimationAndWait(ACTOR_PALACE_GUARD, 3);
  Event_Wait(10);
  Actor_WalkByAndWait(ACTOR_PALACE_GUARD, 24, 0);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  Actor_WatchAndSetScript(ACTOR_PALACE_GUARD, gMainRoutineWork.leaderActorId);
  Event_SetFlag(0x96a);
  Event_SetFlag(0x96b);
  Party_AddMember(7);
  Actor_GetById(FIELD_TREASURE_ACTOR)->scaleY = 0;
  Actor_FaceDirection(ACTOR_LUNPA, FACING_EAST, 0);
  Actor_SetSpeed(ACTOR_HYDROS, 0.7);
  Actor_SetSpeed(ACTOR_LUNPA, 0.7);
  Actor_SetSpeed(ACTOR_PIERS, 0.7);
  Actor_WalkToAndWait(ACTOR_FELIX, 872, 528);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 408, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  Actor_ShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 45);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 3);
  Event_Wait(10);
  gFieldWork->message = (gFieldWork->message) + 1;
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_CONSERVATO, FACING_SOUTH_SOUTHWEST);
  Event_Wait(30);
  Actor_FaceDirectionAndWait(ACTOR_LUNPA, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_CONSERVATO, 2);
  Event_Wait(5);
  Actor_FaceDirectionAndWait(ACTOR_CONSERVATO, FACING_WEST);
  Actor_ShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_HYDROS, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_LUNPA, FACING_EAST);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(ACTOR_HYDROS, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_CONSERVATO, 2);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_HYDROS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  FieldCamera_FollowActor(13, 1);
  Actor_WalkTo(ACTOR_HYDROS, 864, 444);
  Event_Wait(20);
  Actor_WalkTo(ACTOR_LUNPA, 848, 444);
  Event_Wait(20);
  Actor_WalkTo(ACTOR_PIERS, 832, 444);
  Actor_WaitForMoveById(ACTOR_HYDROS);
  Actor_SetAnimation(ACTOR_HYDROS, 1);
  Actor_FaceDirection(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST, 0);
  Actor_WaitForMoveById(ACTOR_LUNPA);
  Actor_SetAnimation(ACTOR_LUNPA, 1);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_STAND);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 496, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(5);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_NORTH_NORTHWEST);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, -32, 0, FACING_NORTH);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHWEST);
  Actor_WalkByAndWait(ACTOR_SHEBA, 0, -8);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHWEST);
  Actor_WalkByAndWait(ACTOR_JENNA, 0, -8);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHEAST);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -8);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHEAST);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
  Actor_GetById(ACTOR_KRADEN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -8);
  Event_Wait(1);
  Actor_GetById(ACTOR_KRADEN)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_LUNPA, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_WEST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 3);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(37);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 0);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_NORTH_NORTHEAST);
  Event_Wait(15);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_ShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_HYDROS, 2);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 0);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_NORTH_NORTHEAST);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 3);
  Event_Wait(10);
  LemuriaPalace_CastMapSparks(ACTOR_HYDROS);
  Field_StartColorTransition(32, 0x2108);
  Event_Wait(32);
  Event_Wait(20);
  LemuriaPalace_LoadMaps();
  gLemuriaPalace_FirstMapShown = 1;
  gLemuriaPalace_SecondMapShown = 0;
  Event_PlaySound(SE_HIGH_JUMP);
  LemuriaPalace_FadeMapsIn();
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 55);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 10);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  LemuriaPalace_CastMapSparks(ACTOR_HYDROS);
  LemuriaPalace_FadeMapsOut();
  Event_Wait(30);
  gLemuriaPalace_FirstMapShown = 0;
  gLemuriaPalace_SecondMapShown = 1;
  Event_PlaySound(SE_HIGH_JUMP);
  LemuriaPalace_FadeMapsIn();
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 10);
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 5);
  Actor_GetById(ACTOR_KRADEN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -16);
  Event_Wait(1);
  Actor_GetById(ACTOR_KRADEN)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(25);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_ShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 5);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, 16);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_EAST);
  Event_Wait(5);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_LUNPA, 2);
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 5);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  LemuriaPalace_CastMapSparks(ACTOR_HYDROS);
  LemuriaPalace_FadeMapsOut();
  Event_Wait(10);
  gLemuriaPalace_MapTime = 0;
  gLemuriaPalace_MapMode = 2;
  Event_Wait(150);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_SetSpeed(ACTOR_CONSERVATO, 1);
  Actor_WalkToAndWait(ACTOR_CONSERVATO, 888, 444);
  Actor_FaceDirectionAndWait(ACTOR_CONSERVATO, FACING_SOUTH_SOUTHWEST);
  Event_Wait(5);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 472, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_CONSERVATO, FACING_WEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 4);
  Event_ShowMessageThenWait(ACTOR_HYDROS, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_CONSERVATO, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  FieldActor_ShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
  Event_Wait(10);
  LemuriaPalace_CastMapSparks(ACTOR_HYDROS);
  Event_PlaySound(SE_HIGH_JUMP);
  for (frame = 0; frame < 120; frame++) {
    wait(1);
    if ((gLemuriaPalace_MapTime >> 2) % 14 == 0)
      break;
  }
  LemuriaPalace_FreeMaps();
  Field_StartPaletteTransition(32);
  Event_Wait(40);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_HYDROS, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_HYDROS, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(ACTOR_HYDROS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_CONSERVATO, 3);
  Event_Wait(15);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, 0x7fff);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_ShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_ShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_ShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_EAST);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_CONSERVATO, 2);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_CONSERVATO, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
  Event_Wait(25);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  Event_Wait(15);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_LUNPA, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 5);
  Actor_ShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 4);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, 4);
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 5);
  FieldActor_ShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_FaceDirectionAndWait(ACTOR_CONSERVATO, FACING_WEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 4);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_HYDROS, 0, 5);
  FieldActor_ShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_SetAnimationAndWait(ACTOR_CONSERVATO, 4);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_HYDROS, 0, 5);
  FieldActor_ShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       45);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
  Event_Wait(5);
  Event_ShowMessageThenWait(ACTOR_HYDROS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_CONSERVATO, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  Event_OpenMessage(ACTOR_CONSERVATO, 0);
  if (!Event_AskYesNoWithActorAnimation(4, 0)) {
    Event_Wait(15);
    Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
    gFieldWork->message = (gFieldWork->message) + 1;
  } else {
    Event_Wait(35);
    gFieldWork->message = (gFieldWork->message) + 1;
    Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  }
  Actor_FaceDirectionAndWait(ACTOR_CONSERVATO, FACING_WEST);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_HYDROS, 0, 5);
  Actor_ShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(ACTOR_HYDROS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_CONSERVATO, 4);
  Actor_SetSpeed(ACTOR_CONSERVATO, 0.5);
  Actor_GetById(ACTOR_CONSERVATO)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_CONSERVATO, 32, 0);
  Event_Wait(1);
  Actor_GetById(ACTOR_CONSERVATO)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_HYDROS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTHEAST);
  Event_Wait(5);
  Actor_RunRepeatedMotion(ACTOR_CONSERVATO, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_CONSERVATO, 4);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_CONSERVATO, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_ShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_CONSERVATO, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_SetSpeed(ACTOR_CONSERVATO, 1.2);
  Actor_WalkByAndWait(ACTOR_CONSERVATO, -50, 24);
  Actor_WatchAndSetScript(ACTOR_HYDROS, 15);
  Actor_WatchAndSetScript(ACTOR_LUNPA, 15);
  Actor_WatchAndSetScript(ACTOR_PIERS, 15);
  Actor_WatchAndSetScript(ACTOR_FELIX, 15);
  Actor_WatchAndSetScript(ACTOR_JENNA, 15);
  Actor_WatchAndSetScript(ACTOR_SHEBA, 15);
  Actor_WatchAndSetScript(ACTOR_KRADEN, 15);
  FieldCamera_FollowActor(15, 1);
  Actor_WalkByAndWait(ACTOR_CONSERVATO, -8, 32);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(37);
  Actor_SetSpeed(ACTOR_FELIX, 2.2);
  Actor_SetSpeed(ACTOR_JENNA, 2.2);
  Actor_WalkBy(ACTOR_JENNA, -8, 16);
  Actor_WalkByAndWait(ACTOR_FELIX, 8, 16);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_EAST);
  Event_Wait(15);
  Actor_WatchAndSetScript(ACTOR_FELIX, 15);
  Actor_WatchAndSetScript(ACTOR_JENNA, 15);
  Actor_WalkByAndWait(ACTOR_CONSERVATO, 0, 64);
  Event_Wait(15);
  Actor_FaceDirectionAndWait(ACTOR_CONSERVATO, FACING_NORTH_NORTHEAST);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  Event_ShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_CONSERVATO, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_PALACE_GUARD, FACING_WEST);
  Event_Wait(15);
  Actor_FaceDirectionAndWait(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHEAST);
  Event_Wait(15);
  Actor_SetAnimation(ACTOR_PALACE_GUARD, 5);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_WalkByAndWait(ACTOR_CONSERVATO, 0, 96);
  Event_Wait(20);
  Actor_SetPosition(ACTOR_CONSERVATO, 0, 0);
  Actor_SetSpeed(ACTOR_FELIX, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_WalkBy(ACTOR_JENNA, 8, -16);
  Actor_WalkByAndWait(ACTOR_FELIX, -8, -16);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 480, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_Stop(ACTOR_PALACE_GUARD);
  Actor_SetAnimation(ACTOR_PALACE_GUARD, 1);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_HYDROS, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_HYDROS, 2);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 5);
  Actor_ShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_WEST);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_LUNPA, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, 3);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST);
  Event_Wait(5);
  Actor_WalkByAndWait(ACTOR_LUNPA, 24, 26);
  Actor_WalkByAndWait(ACTOR_LUNPA, 0, 8);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_LUNPA, 2);
  Event_Wait(10);
  Actor_Jump(ACTOR_LUNPA, 4, 20);
  i = 15;
  do {
    Actor_GetById(FIELD_TREASURE_ACTOR)->scaleY += 4096;
    i--;
    wait(1);
  } while (i >= 0);
  Actor_GetById(FIELD_TREASURE_ACTOR)->scaleY = Q16_ONE;
  Event_Wait(5);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_WalkByAndWait(ACTOR_LUNPA, -24, -26);
  Actor_WalkByAndWait(ACTOR_LUNPA, 0, -8);
  Actor_FaceDirectionAndWait(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_LUNPA, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 5);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
  Event_Wait(15);
  Actor_RunRepeatedMotion(ACTOR_HYDROS, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_WEST);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_LUNPA, 3);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 3);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 5);
  Actor_ShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_HYDROS, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_LUNPA, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 5);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_HYDROS, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_NORTH_NORTHEAST);
  Event_Wait(5);
  Actor_WalkToAndWait(ACTOR_HYDROS, 864, 392);
  Actor_FaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
  Actor_RunRepeatedMotion(ACTOR_LUNPA, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_LUNPA, FACING_WEST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_LUNPA, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_LUNPA, 0, 5);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, 2);
  target = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (target != 0) {
    Actor_SetDestination(ACTOR_KRADEN, target->x.part[0].pixel,
                         target->z.part[0].pixel);
  }
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  target = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (target != 0) {
    Actor_SetDestination(ACTOR_JENNA, target->x.part[0].pixel,
                         target->z.part[0].pixel);
  }
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  target = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (target != 0) {
    Actor_SetDestination(ACTOR_SHEBA, target->x.part[0].pixel,
                         target->z.part[0].pixel);
  }
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Event_Wait(10);
  Actor_WalkByAndWait(ACTOR_FELIX, -32, -48);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
  target = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (target != 0) {
    Actor_SetDestination(ACTOR_PIERS, target->x.part[0].pixel,
                         target->z.part[0].pixel);
  }
  Actor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_LUNPA, 3);
  Event_Wait(15);
  Actor_WalkToAndWait(ACTOR_LUNPA, 840, 408);
  Actor_FaceDirectionAndWait(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(10);
  result = Event_End();
done:
  return result;
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 288, 0, 454, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(LemuriaPalace),
    SCENE_EXIT_UNLESS(1, Lemuria, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, LemuriaPalace, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, LemuriaPalace, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, LemuriaPalace, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, LemuriaPalace, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, LemuriaPalace, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, LemuriaPalace, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, LemuriaPalace, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, LemuriaPalace, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, Lemuria, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, LemuriaPalace, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, LemuriaPalace, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, LemuriaPalace, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, LemuriaPalace, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, LemuriaPalace, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, LemuriaPalace, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(17, Lemuria, 17, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(157, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 912, 0, 624, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(156, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 520, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(160, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 456, FACING_EAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(165, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 360, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(164, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0, 136, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(31, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 864, 0, 392, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(43, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 408, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(34, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 392, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 560, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {56, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 424, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
/// The palace doors' cell animations: a narrow door's, then a wide door's.
s16 gLemuriaPalace_DoorOpening[] = {
    1, 52, 1, 2, 6, 2, 52, 1,
    2, 6, -1, 2, 54, 2, 2, 6,
    4, 54, 2, 2, 6, -1,
};

s32 gLemuriaPalace_Doors[] = {                                                               
      (s32)gLemuriaPalace_DoorOpening,                                                              
      0x170005,                                                                      
      (s32)gLemuriaPalace_DoorOpening,                                                              
      0x17001e,                                                                      
  };

s32 gLemuriaPalace_InnerDoor[] = {
    (s32)gLemuriaPalace_DoorOpening,
    0x40038,
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(LemuriaPalace_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(LemuriaPalace_EnterDoor)},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, ACTOR_GIRL_WAITING_ON_THE_KING, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, ACTOR_WOMAN_SHOWING_THE_WAY, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, ACTOR_MAN_ON_THE_KINGS_MOOD, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, ACTOR_LEMURIAN_WHO_HEARD_THE_YELLING, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, ACTOR_LEMURIAN_PRAISING_HYDROS, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, ACTOR_HYDROS, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, ACTOR_LUNPA, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, ACTOR_CONSERVATO, CONDITION_ALWAYS, 15},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), ACTOR_PALACE_GUARD, CONDITION_ALWAYS, EVENT(LemuriaPalace_EnterDoor)},
    {TRIGGER_EXIT, ACTOR_KRADEN, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, 20, 0x96a, EVENT(LemuriaPalace_MeetKingHydros)},
    {TRIGGER_TALK, ACTOR_GIRL_WAITING_ON_THE_KING, 0x196a, MSGNO(MsgLemuriaPalaceLordConservatoSeemedVeryAngry)},
    {TRIGGER_TALK, ACTOR_WOMAN_SHOWING_THE_WAY, 0x196a, MSGNO(MsgLemuriaPalaceBothLordConservatoAndKing)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_KINGS_MOOD, 0x196a, MSGNO(MsgLemuriaPalaceMaybeOnceYouHaveLeft)},
    {TRIGGER_TALK, ACTOR_LEMURIAN_WHO_HEARD_THE_YELLING, 0x196a, MSGNO(MsgLemuriaPalaceTodayHisHighnessAndLord)},
    {TRIGGER_TALK, ACTOR_LEMURIAN_PRAISING_HYDROS, 0x196a, MSGNO(MsgLemuriaPalaceIfHydrosPlansToUnleash)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_WAITING_ON_THE_KING, 0x196a, MSGNO(MsgLemuriaPalaceSenatesReportsUsuallyTend)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SHOWING_THE_WAY, 0x196a, MSGNO(MsgLemuriaPalaceKingsLeadershipIsTooProgressive)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_KINGS_MOOD, 0x196a, MSGNO(MsgLemuriaPalaceWheneverOutsidersArriveInLemuria)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIAN_WHO_HEARD_THE_YELLING, 0x196a, MSGNO(MsgLemuriaPalaceWhatWillHappenToUs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIAN_PRAISING_HYDROS, 0x196a, MSGNO(MsgLemuriaPalaceAsLongAsICan)},
    {TRIGGER_TALK, ACTOR_GIRL_WAITING_ON_THE_KING, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceHeIsStillInChambers)},
    {TRIGGER_TALK, ACTOR_WOMAN_SHOWING_THE_WAY, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceIfYouSeekCounsel)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_KINGS_MOOD, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceHisHighnessNeverLeavesMeeting)},
    {TRIGGER_TALK, ACTOR_LEMURIAN_WHO_HEARD_THE_YELLING, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceConservatoAndKingWereYelling)},
    {TRIGGER_TALK, ACTOR_LEMURIAN_PRAISING_HYDROS, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceKingHydrosIsWisest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_WAITING_ON_THE_KING, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceLordConservatoHoldsMostPower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SHOWING_THE_WAY, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceOhDearUsingPsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_KINGS_MOOD, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceHisHighnessKingIsMost)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIAN_WHO_HEARD_THE_YELLING, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceLordConservatoOnlyRaisesHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIAN_PRAISING_HYDROS, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceHisHighnessIsWorried)},
    {TRIGGER_TALK, ACTOR_HYDROS, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceDoNotListenToConservato)},
    {TRIGGER_TALK, ACTOR_LUNPA, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceYouMustSailToWestern)},
    {TRIGGER_TALK, ACTOR_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceIThinkIOverheardSomething)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HYDROS, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceMyPeopleMayNotDesire)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUNPA, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceHisMajestyHasCome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceActionsOfKingHydros)},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsTheresBookCalledGreatAdventures))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsIFoundLetterFromBabi))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsHeresBookCalledHistory))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsHeresBookCalledElementalLighthouses))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsHeresBookCalledGreatLemurian))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsHeresBookCalledAlchemy))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsHeresBookCalledAlchemyAndAncient))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsHeresBookCalledDiary))},
    {EXAMINE_TRIGGER(EXAMINE_WARDROBE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriorsWhatInWhatsWithAll))},
    {TRIGGER_TABLE_END},
};

s16 gLemuriaPalace_GlowColors[] = {
    0x7b00, 0x76c0, 0x7280, 0x6e40, 0x6a00, 0x65c0,
    0x6180, 0x65c0, 0x6a00, 0x6e40, 0x7280, 0x76c0,
};
// clang-format on
#undef BLEND_SET_ALPHA

#undef BLEND_SET_ALPHA

// clang-format off
LATE_DATA u32 gLemuriaPalace_MapCrossfade[] = {
    0xc040e02, 0x8080a06, 0x40c060a, 0x100010, 0x60a040c, 0xa060808,
    0xe020c04, 0xc040e02, 0x8080a06, 0x40c060a, 0x100010, 0x60a040c,
    0xa060808, 0xe020c04,
};
// clang-format on
