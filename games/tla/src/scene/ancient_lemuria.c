/// @file
/// Ancient Lemuria, the shore before the hidden city, where the Lemurians meet
/// the party that defeated Poseidon (AncientLemuria_MeetTheLemurians). Cyclone
/// on the first bush brings out Rime, the Mercury Djinni, which hides in the
/// blue statue until Tremor shakes it out (AncientLemuria_CycloneRevealsRime,
/// AncientLemuria_TremorShakesOutRime).

#include "types.h"
#include "system/io_reg.h"
#include "flags.h"
#include "map.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "field_event.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "event_work.h"
#include "module/scene/scene.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

void AncientLemuria_StillTheStatue(s32 id);
void AncientLemuria_MeetTheLemurians(void);
void Field_TryJumpForward(void);
void TileCopies_CopyTouchedCells(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 destX, s32 destY, s32 layer);

static inline void MapCopy(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                           s32 destY, s32 layer) {
  TileCopies_CopyTouchedCells(srcX, srcY, width, height, destX, destY, layer);
}
#include "text/msg_ids.h"
/// The twelve colours, a frame in four, the water's shimmer takes in turn.
extern s16 gAncientLemuria_ShimmerColors[];

void AncientLemuria_RimeJoins(void) { Djinn_JoinParty(12, 1, 13); }

void AncientLemuria_ShakeTheStatue(struct Actor *object) {
  struct FieldSprite *sprite = object->sprite;
  s16 time = object->timer++;

  sprite->rotation = Iwram_MulQ16(Trig_Sin(time << 12), 0x100);
  if ((time & 15) == 0)
    Sound_Play(SE_SWITCH);
}

void AncientLemuria_StillTheStatue(s32 id) {
  struct Actor *object = Actor_GetById(id);
  struct FieldSprite *sprite = object->sprite;

  WORD_AT(object, 108) = 0;
  object->timer = 0;
  sprite->rotation = 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void AncientLemuria_KeepCameraSouth(void) {
  Ram_HeapSlots->mapWork->minZ = px(750);
}

void AncientLemuria_LetCameraNorth(void) {
  Ram_HeapSlots->mapWork->minZ = px(352);
}

void AncientLemuria_StartMovingStatue(void) {}

void AncientLemuria_FinishMovingStatue(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (x == 8 && z == 36)
    GameFlag_Set(0x961);
  if (GameFlag_Get(0x961))
    AncientLemuria_MeetTheLemurians();
}

/// One hop of object 12 by (dx, dz).
static inline void Hop(s32 dx, s32 dz) {
  Actor_Jump(12, 5, 0);
  Actor_WalkByAndWaitById(12, dx, dz);
  Event_Wait(3);
}

/// Once (flag 0x96c): object 12 spins for 30 frames while it grows from half
/// scale, then hops thirteen times while the leader follows, and object 13
/// takes AncientLemuria_ShakeTheStatue as its update.
void AncientLemuria_CycloneRevealsRime(void) {
  struct Actor *object;
  s32 i;

  /* FAKEMATCH(wrapper): scene.h's MapCopy. */
  MapCopy(7, 13, 1, 1, 5, 13, 0);
  if (Event_GetFlag(0x96c))
    return;
  object = Actor_GetById(12);
  Event_SetFlag(0x96c);
  Event_SetFlag(0x96e);
  Actor_SetSpriteFlags(Actor_GetById(12), 0);
  Actor_SetPosition(12, 88, 216);
  Actor_Jump(12, 9, 0);
  object->scaleY = Q16_ONE / 2;
  object->scaleX = Q16_ONE / 2;
  for (i = 29; i >= 0; i--) {
    object->facing += 0x1000;
    {
      /* FAKEMATCH(pin): the limit's first half stays in r2. */
      register s32 limit asm("r2") = 255;

      /* FAKEMATCH(barrier): the barrier starts the 0xffff limit before the
       * scale's load and finishes it after. */
      asm("" : "+r"(limit) : "r"(object->scaleX));
      if (object->scaleX <= (limit << 8) + 255)
        object->scaleX += 0xa00;
    }
    object->scaleY = object->scaleX;
    wait(1);
  }
  object->facing = 0x3000;
  object->scaleY = Q16_ONE;
  object->scaleX = Q16_ONE;
  Actor_SetSpriteFlags(Actor_GetById(12), 1);
  Actor_SetSpritePriority(12, 1);
  Event_Wait(8);
  Actor_Jump(12, 9, 0);
  Actor_WalkToAndWaitById(12, 120, 200);
  Actor_GetById(12)->priorityFlags |= 1;
  Actor_WalkToAndWaitById(12, 136, 200);
  Event_Wait(3);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Hop(0, -16);
  Hop(16, 0);
  Hop(16, 0);
  Hop(0, -16);
  Hop(16, 0);
  Hop(0, -16);
  Hop(16, 0);
  Hop(16, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Hop(0, 16);
  Hop(16, 0);
  Hop(16, 0);
  Hop(16, 0);
  Hop(16, 0);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Actor_GetById(13)->update = AncientLemuria_ShakeTheStatue;
  Actor_GetById(13)->timer = 0;
}

void AncientLemuria_CycloneSecondBush(void) { MapCopy(9, 9, 2, 3, 8, 9, 0); }

void AncientLemuria_CycloneThirdBush(void) { MapCopy(38, 0, 5, 2, 12, 7, 0); }

void AncientLemuria_CycloneFourthBush(void) { MapCopy(39, 2, 4, 3, 14, 11, 0); }

void AncientLemuria_CycloneFifthBush(void) { MapCopy(39, 2, 3, 3, 21, 9, 0); }

void AncientLemuria_CycloneSixthBush(void) { MapCopy(39, 0, 4, 3, 25, 8, 0); }

void AncientLemuria_StartTremorOnTheStatue(void) {
  AncientLemuria_StillTheStatue(13);
}

/// Once flag 0x96c is set and 0x96d is not: sets 0x96d, clears 0x96e, launches
/// object 12 from 22 pixels up at half size and turns it an eighth of a turn a
/// frame for 24 frames while it grows back to full size, then leaves it facing
/// 0x3000.
void AncientLemuria_TremorShakesOutRime(void) {
  struct Actor *object;
  s32 i;
  struct Actor *p;

  if (Event_GetFlag(0x96d))
    return;
  if (!GameFlag_Get(0x96c))
    return;
  object = Actor_GetById(12);
  GameFlag_Set(0x96d);
  GameFlag_Clear(0x96e);
  AncientLemuria_StillTheStatue(13);
  Actor_SetPosition(12, 270, 184);
  object->y = 22 << 16;
  Actor_Jump(12, 11, 0);
  object->scaleY = Q16_ONE / 2;
  object->scaleX = Q16_ONE / 2;
  Actor_SetSpeed(12, 2.2);
  Actor_WalkTo(12, 312, 184);
  for (i = 23; i >= 0; i--) {
    {
      s32 turn = 0x2000;

      object->facing += turn;
    }
    /* FAKEMATCH(temporary): the object read again through p after the turn. */
    p = object;
    if (p->scaleX <= 0xffff)
      p->scaleX += 0xa00;
    p->scaleY = p->scaleX;
    wait(1);
  }
  p->facing = 0x3000;
  p->scaleY = Q16_ONE;
  p->scaleX = Q16_ONE;
  Actor_WaitForMoveById(12);
}

void AncientLemuria_HeadForTheDock(void) {
  struct Actor *object;

  if (!GameFlag_Get(0x961) || GameFlag_Get(0x96a)) {
    Sound_Play(SE_MAP_EXIT);
    Import_EventSetExitRequest(2);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldActor_LaunchFromLeader(11, 0, -16, 0x5000);
    Actor_ArmCallback(4, FACING_NORTH, 0);
    Actor_WaitForMoveById(11);
    Event_Wait(20);
    if (GameFlag_Get(7)) {
      Event_SetMessageId(MsgLemuriaStayExploring);
      Event_ShowMessageThenWait(11, 0, 10);
    } else {
      Event_SetMessageId(MsgLemuriaStayExploringAlternate);
      Event_ShowMessageThenWait(11, 0, 10);
    }
    Actor_SetSpeed(11, 1.2);
    Actor_SetModeById(11, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != NULL)
      Actor_SetDestinationById(11, *(s16 *)((u8 *)object + 10),
                               *(s16 *)((u8 *)object + 18));
    Actor_WaitForMoveById(11);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_WalkToAndWait(4, 168, 832);
    Event_End();
  }
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// Rotates colours 2-14 of palettes 7 and 8 by one entry every four frames,
/// and selects colour 0x7f from the table.
s32 AncientLemuria_CycleWaterColors(void) {
  if ((*Ram_FrameCount & 3) == 0) {
    volatile u16 *palette;
    s16 saved;
    s32 index;

    saved = BG_PLTT_COLOR(7, 14);
    index = 14;
  first:
    palette = (u16 *)&BG_PLTT_COLOR(7, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto first;
    BG_PLTT_COLOR(7, 1) = saved;
    saved = BG_PLTT_COLOR(8, 14);
    index = 14;
  second:
    palette = (u16 *)&BG_PLTT_COLOR(8, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto second;
    BG_PLTT_COLOR(8, 1) = saved;
  }
  {
    s16 color = gAncientLemuria_ShimmerColors[(*Ram_FrameCount >> 2) % 12];

    BG_PLTT_COLOR(7, 15) = color;
    return color;
  }
}

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 513;
  Task_Add(AncientLemuria_CycleWaterColors, 0xc80);
  if (gMainRoutineWork.entrance != 1)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  else
    GameFlag_Set(FLAG_SHOW_AREA_NAME);
  if (GameFlag_Get(0x961)) {
    Actor_SetPosition(8, 136, 584);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  }
  if (Event_GetFlag(0x96d))
    Actor_SetPosition(12, 312, 184);
  if (gMainRoutineWork.entrance == 1 && !GameFlag_Get(0x96d) &&
      GameFlag_Get(0x96c)) {
    *(void (**)(void))(((u8 *)Actor_GetById(13)) + 108) =
        AncientLemuria_ShakeTheStatue;
    *(u16 *)(((u8 *)Actor_GetById(13)) + 100) = 0;
  }
  Actor_SetSpriteFlags(Actor_GetById(13), 0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  s32 entrance = gMainRoutineWork.entrance;

  if (entrance == 1)
    GameFlag_Set(0x960);
  else if (entrance == 2)
    GameFlag_Clear(0x960);
  else if (entrance == 3)
    GameFlag_Clear(0x960);
  else if (entrance == 4)
    GameFlag_Set(0x960);
  else if (entrance == 5)
    GameFlag_Set(0x960);
  if (GameFlag_Get(0x960)) {
    MapGrid_CopyCellPictureCellsRect(38, 47, 26, 12, 8, 47);
    MapGrid_CopyCellPictureCellsRect(102, 47, 26, 12, 72, 47);
    Map_CopyCellAttributeRect(38, 47, 26, 12, 8, 47);
  }
  return 0;
}

void AncientLemuria_JumpForward(void) { Field_TryJumpForward(); }

void Actor_MoveToObject();

void AncientLemuria_MeetTheLemurians(void) {
  struct Actor *leader;
  s32 result;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaRecognizePsynergy);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessageThenWait(10, 0, 10);
  Field_TryJumpForward();
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_Wait(20);
  Actor_SetModeById(4, 1);
  Event_Wait(20);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_ArmCallback(4, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_ArmCallback(4, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetSpeed(4, 1);
  Actor_WalkByAndWait(4, 0, -16);
  Event_Wait(10);
  FieldCamera_MoveWithinMapBounds(px(168), -1, px(536), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_SetSpeed(9, 0.8);
  Actor_WalkByAndWaitById(9, 24, 0);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_SetSpeed(10, 0.8);
  Actor_WalkByAndWait(10, -24, 0);
  Actor_ArmCallback(10, 0x5000, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(10, 2);
  Event_Wait(10);
  Event_OpenMessage(10, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result == 0) {
    Event_Wait(30);
    Actor_StartRepeatedMotion(10, 3);
    FieldEffect_SetAttached(10,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Event_ShowMessageThenWait(10, 0, 10);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
  } else {
    Event_Wait(40);
    Actor_StartRepeatedMotion(10, 3);
    FieldEffect_SetAttached(10,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
    Event_ShowMessageThenWait(10, 0, 0);
    Actor_Jump(10, 6, 15);
    Actor_Jump(10, 6, 23);
    Event_Wait(10);
  }
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  FieldActor_LaunchFromLeader(11, 16, -16, 0xb000);
  Actor_WaitForMoveById(11);
  Event_Wait(20);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 11, 0, 10);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirection(11, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(11, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetModeById(4, 4);
  Motion_SetModeAndWaitAnimation(11, 4);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(10, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_MoveToObject(7, 4);
  Actor_GetById(7)->scaleX = 0;
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ArmCallback(9, FACING_EAST, 0);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Event_Wait(45);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(25);
  Actor_GetById(7)->scaleX = Q16_ONE;
  FieldActor_LaunchFromLeader(7, 0, -16, 0xc000);
  Actor_WaitForMoveById(7);
  Event_Wait(20);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHWEST, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldMessage_ShowAtActor(9, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | 10, 0, 0);
  Message_CloseFinishedWindows();
  Event_Wait(10);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_WalkByAndWait(9, -8, 8);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(10, -8, 8);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 4);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(7, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(35);
  Actor_FaceDirection(11, FACING_SOUTH_SOUTHWEST, 0);
  Actor_ArmCallback(4, FACING_NORTHEAST, 0);
  Event_Wait(50);
  Actor_FaceDirection(11, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetModeById(11, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(25);
  Actor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_ArmCallback(9, FACING_EAST, 0);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Event_Wait(40);
  Actor_FaceDirection(7, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_SetSpeed(7, 1);
  Actor_WalkByAndWait(7, 0, -16);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_StartRepeatedMotion(9, 2);
  Motion_SetVarCbAndRefresh(10, 2);
  Event_Wait(10);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 11, 0, 10);
  Actor_Jump(9, 6, 15);
  Actor_Jump(9, 6, 23);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_SetSpritePriority(11, 2);
  Actor_Jump(11, 6, 15);
  Actor_Jump(11, 6, 23);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 11, 0, 10);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 11, 0, 10);
  Actor_ArmCallback(7, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_StartRepeatedMotion(11, 3);
  Actor_SetAttachedEffect(11, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 11, 0, 10);
  Actor_WalkByAndWait(11, 0, -16);
  Actor_FaceDirection(11, FACING_NORTH_NORTHWEST, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 11, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ArmCallback(9, FACING_EAST, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(20);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_SMILE, 55);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(10, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  Actor_WalkBy(9, 0, -16);
  Actor_WalkByAndWait(10, 0, -16);
  Actor_FaceDirection(11, FACING_WEST, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(11, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 11, 0, 10);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  FieldCamera_MoveWithinMapBounds(px(168), -1, px(512), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(11, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_WINCE, 55);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ArmCallback(9, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_ArmCallback(11, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetSpritePriority(4, 2);
  Actor_WalkByAndWait(4, 8, -20);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(20);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_ArmCallback(11, 0xb000, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(20);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  FieldMessage_ShowAtActor(9, 0);
  if ((s8)gMainRoutineWork.counter274 <= 4) {
    result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
    if (result == 0) {
      Event_Wait(25);
      Actor_ArmCallback(11, 0x5000, 0);
      Event_Wait(30);
      Motion_SetModeAndWaitAnimation(11, 3);
      Event_Wait(10);
      Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 11, 0, 10);
      ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
    } else {
      Event_Wait(35);
      Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
      Actor_ArmCallback(11, 0x5000, 0);
      Event_Wait(30);
      Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
      ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
      Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 11, 0, 10);
    }
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count += 2;
  } else {
    /* FAKEMATCH(pin): the count pinned to r2. */
    register u32 n asm("r2");

    n = ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count;
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count = n + 2;
    result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
    if (result == 0) {
      Event_Wait(25);
      Actor_FaceDirection(11, FACING_SOUTH_SOUTHWEST, 0);
      Actor_FaceDirectionById(7, FACING_SOUTH_SOUTHEAST);
      Event_Wait(10);
      Actor_ShowEmoteById(7, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
      Motion_SetVarCbAndRefresh(11, 2);
      Event_Wait(10);
      Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 11, 0, 10);
      ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
    } else {
      Event_Wait(35);
      Actor_FaceDirection(11, FACING_SOUTH_SOUTHWEST, 0);
      Actor_FaceDirectionById(7, FACING_SOUTH_SOUTHEAST);
      Event_Wait(10);
      Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
      Actor_Jump(11, 6, 15);
      Actor_Jump(11, 6, 23);
      ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
      Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 11, 0, 10);
    }
    Actor_FaceDirectionById(7, FACING_NORTH);
    Event_Wait(10);
  }
  Actor_WalkByAndWait(7, 0, -10);
  Event_Wait(15);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(11, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(10, 4);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_SetModeById(9, 3);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(25);
  FieldCamera_FollowActor(10, 1);
  Actor_WalkBy(10, 0, -32);
  Event_Wait(10);
  Actor_WalkByAndWaitById(9, 16, 0);
  Actor_WalkBy(9, 0, -32);
  Actor_WaitForMoveById(10);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  Actor_WaitForMoveById(9);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Event_Wait(30);
  FieldCamera_MoveWithinMapBounds(px(168), -1, px(536), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_WalkByAndWait(11, 0, -10);
  Actor_ArmCallback(11, FACING_WEST, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 11, 0, 10);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessageThenWait(7, 0, 10);
  Motion_SetVarCbAndRefresh(7, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 11, 0, 10);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ArmCallback(7, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(7, 0, -32);
  Actor_FaceDirection(11, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  Actor_ArmCallback(7, 0x3000, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_ArmCallback(11, 0x5000, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Actor_SetSpeed(4, 0.8);
  Actor_SetSpeed(11, 0.8);
  Actor_WalkByAndWait(4, -8, -16);
  Actor_WalkBy(11, 0, -10);
  Actor_WalkByAndWait(4, 0, -16);
  Actor_WaitForMoveById(11);
  Actor_ArmCallback(11, 0xb000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(20);
  Actor_SetModeById(4, 3);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(20);
  Actor_SetSpeed(7, 1.2);
  Actor_SetModeById(7, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestinationById(7, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  Actor_SetSpeed(11, 1.2);
  Actor_SetModeById(11, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestinationById(11, ((s16 *)&leader->x)[1],
                             ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(11);
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  Event_End();
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 100, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(AncientLemuria),
    SCENE_EXIT_UNLESS(1, AncientLemuria, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, LemuriaDock, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Lemuria, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AncientLemuria, 1, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(289, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 584,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(157, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 504,
                    0x3000, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(157, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 504,
                    0x5000, TALK_FACING_PARTY, 0),
    {56, CONDITION_ALWAYS, 1, -px(56), 0, -px(56), FACING_NORTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, -px(56), 0, -px(56), FACING_NORTH, 2, 0},
    {244, 0x51, 1, -px(56), 0, -px(56), FACING_SOUTH, 0, 0},
    ACTOR_PLACEMENT(289, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 184,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(AncientLemuria_HeadForTheDock)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(AncientLemuria_KeepCameraSouth)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(AncientLemuria_LetCameraNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 22, CONDITION_ALWAYS,
     EVENT(AncientLemuria_JumpForward)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(AncientLemuria_RimeJoins)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     0x961, (u32)(AncientLemuria_StartMovingStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x961,
     EVENT(AncientLemuria_FinishMovingStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(AncientLemuria_CycloneRevealsRime)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(AncientLemuria_CycloneSecondBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32,
     CONDITION_ALWAYS, EVENT(AncientLemuria_CycloneThirdBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33,
     CONDITION_ALWAYS, EVENT(AncientLemuria_CycloneFourthBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 34,
     CONDITION_ALWAYS, EVENT(AncientLemuria_CycloneFifthBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 35,
     CONDITION_ALWAYS, EVENT(AncientLemuria_CycloneSixthBush)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_TREMOR, PSYNERGY_PHASE_BEFORE_CAST), 13,
     IF_FLAG_SET(0x96e), EVENT(AncientLemuria_StartTremorOnTheStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 13, IF_FLAG_SET(0x96e),
     EVENT(AncientLemuria_TremorShakesOutRime)},
    {TRIGGER_TABLE_END},
};

// clang-format off
s16 gAncientLemuria_ShimmerColors[] = {
    0x5f0c, 0x62c8, 0x6684, 0x6a40, 0x6a00, 0x69c0,
    0x6980, 0x69c0, 0x6a00, 0x6a40, 0x6684, 0x62c8,
};
// clang-format on
