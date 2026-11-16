/// @file
/// Izumo, the village in the far east of the Eastern Sea, plagued by the
/// Serpent until the party slays it. Move sets the misaligned stone of its
/// stone circle back in place (Izumo_FinishMovingStone), and Reveal then shows
/// the way down to the Izumo Ruins (Izumo_RevealTheStoneCircle,
/// Izumo_DescendIntoTheRuins). Cyclone clears its bushes
/// (Izumo_CycloneFirstPatch to Izumo_CycloneNinthPatch). Once the Serpent is
/// slain the village holds a festival, its dancers spinning on the spot
/// (gIzumo_FirstSpinnerScript), and as the party leaves it is stopped to be
/// thanked and told the legend of the Cloud Brand
/// (Izumo_StopThePartyToThankThem).

#include "shared/tables.h"
#include "scene/event_layer.h"
#include "field/emote.h"
#include "sounds.h"
#include "field/field_sequence_event.h"
#include "types.h"
#include "flags.h"
#include "system/task.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"
#include "field/call.h"
#include "field/flag_call.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/io_reg.h"
#include "version.h"
#include "system/frame.h"
#include "field/actor_script.h"
#include "field/sprite.h"
#include "sprite_ids.h"

/// Set as the stone circle's guard finds the misaligned stone back in place.
#define FLAG_GUARD_SAW_STONE_MOVED 0x924
/// Set while Move has set the stone in its place, for this visit.
#define FLAG_STONE_IN_PLACE 0x250
/// Set while Reveal is shown.
#define FLAG_REVEALING 0x301

/// The village's actors the events move: the stone Move sets back in the
/// circle ("The misaligned stone! It's back in its original position!"), the
/// circle's guard, the seven who dance once the Serpent is slain, and Susa.
enum {
  ACTOR_MISALIGNED_STONE = 12,
  ACTOR_STONE_CIRCLE_GUARD = 22,
  ACTOR_DANCER_A = 25,
  ACTOR_DANCER_B,
  ACTOR_DANCER_C,
  ACTOR_DANCER_D,
  ACTOR_DANCER_E,
  ACTOR_DANCER_F,
  ACTOR_DANCER_G,
  ACTOR_SUSA
};
/// Set while the one who stopped the party waits by the gate.
#define FLAG_THANKFUL_ONE_WAITS 0x302
/// Set as the party is stopped on its way out to be thanked.
#define FLAG_STOPPED_TO_THANK_THE_PARTY 0x929
/// Set once the token of gratitude has been offered.
#define FLAG_TOKEN_OFFERED 0x92a

extern u32 gIzumo_FirstSpinnerScript[];
extern u32 gIzumo_SecondSpinnerScript[];
extern u32 gIzumo_ThirdSpinnerScript[];
extern u32 gIzumo_FourthSpinnerScript[];
extern u32 gIzumo_FifthSpinnerScript[];
extern u32 gIzumo_SixthSpinnerScript[];
void Izumo_RevealTheStoneCircle(void);

extern s16 gIzumo_MovableStones[];
extern struct ActorPlacement gIzumo_ActorPlacementsOnceTheSerpentIsSlain[];
extern struct ActorPlacement gIzumo_ActorPlacements[];
extern struct Trigger gIzumo_TriggersOnceTheSerpentIsSlain[];
extern struct Trigger gIzumo_Triggers[];
extern u8 MsgCanYouImagine[];
extern u8 MsgSeeThisRing[];
extern u8 MsgButItS[];
extern u8 MsgSeeThisRingOf[];
extern u8 MsgLadyUzumeAnd[];
extern u8 MsgWouldYouBe[];
extern u8 MsgTheLegendSays[];
extern u8 MsgJennaWait[];
extern u8 MsgThisTokenIsn[];
extern u8 MsgTheMisalignedStone[];

struct Actor *Actor_GetById(u32 objectId);
void FieldEvent_Wait(s32 frames);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Actor_SetDestinationOffsetById(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_WalkToById(s32 id, s32 x, s32 z);
void Actor_FaceActorById(s32 id, s32 target, s32 frames);
void Actor_FaceDirectionById(s32 id, s32 angle, s32 flags);
void Actor_SetSpeedParameters(s32 id, FIXED speed, FIXED acceleration);
void MapGrid_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Actor_SetHorizontalPositionWithTerrain(u32 actorId, FIXED x, FIXED z);
s32 Actor_ShowEmoteById(s32 actorId, s32 emote, s32 frames);
void FieldEffect_SetAttached(s32 id, s32 a);
void FieldCamera_WaitForArrival(void);
void FieldCamera_MoveToActorById(s32 id, s32 value);
void Izumo_SetExitRequest(s32 value);
void Actor_PlaceBlocks(const s16 *data);
void Actor_LiftTargetBlock(const s16 *data);
void Actor_PlaceMovedBlock(void);
void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);
void Sound_Play(s32 cue);
void Izumo_RotateWaterColors(void);
void Izumo_StopThePartyToThankThem(void);
void Izumo_TellTheCloudBrandLegend(void);

/// Move on the stone circle: lifts the misaligned stone.
void Izumo_StartMovingStone(void) {
  Actor_LiftTargetBlock(gIzumo_MovableStones);
}

/// Move sets the misaligned stone down: back in its place in the circle, the
/// guard notices the first time and Reveal shows the way down; moved off it,
/// the way closes again.
void Izumo_FinishMovingStone(void) {
  struct Actor *actor = Actor_GetById(ACTOR_MISALIGNED_STONE);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);

  Actor_PlaceMovedBlock();
  Sound_Play(SE_WATER);
  if (x == 20 && z == 9) {
    if (Flag_IsSet(FLAG_REVEALING)) {
      MapGrid_CopyCellPictureIndicesRect(36, 12, 22, 8, 3, 3);
      Call6((void (*)())Map_CopyCellAttributeRect, 36, 12, 3, 3, 22, 8);
    }
    if (!Flag_IsSet(FLAG_GUARD_SAW_STONE_MOVED)) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldCamera_FollowActor(ACTOR_STONE_CIRCLE_GUARD, 1);
      FieldCamera_WaitForArrival();
      Actor_ShowEmoteById(ACTOR_STONE_CIRCLE_GUARD,
                          EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      FieldEvent_Wait(60);
      Actor_FaceDirectionById(ACTOR_STONE_CIRCLE_GUARD, FACING_WEST, 0);
      FieldEvent_Wait(60);
      Actor_ShowEmoteById(ACTOR_STONE_CIRCLE_GUARD,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      FieldEvent_Wait(60);
      Event_SetMessageId(MsgTheMisalignedStone);
      // "The misaligned stone! It's back in its original position! How did..."
      Event_ShowMessageAndWait(ACTOR_STONE_CIRCLE_GUARD, 0);
      FieldEvent_Wait(30);
      Actor_ShowEmoteById(ACTOR_STONE_CIRCLE_GUARD,
                          EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
      FieldEvent_Wait(30);
      // "Hmmm... Either Susa or Kushinada must have moved it when I wasn't..."
      Event_ShowMessageAndWait(ACTOR_STONE_CIRCLE_GUARD, 0);
      Actor_FaceDirectionById(ACTOR_STONE_CIRCLE_GUARD, FACING_SOUTH, 0);
      Event_End();
      Flag_Set(FLAG_GUARD_SAW_STONE_MOVED);
    }
    Flag_Set(FLAG_STONE_IN_PLACE);
  } else if (x == 19 && z == 9) {
    Flag_Clear(FLAG_STONE_IN_PLACE);
    if (Flag_IsSet(FLAG_REVEALING)) {
      MapGrid_CopyCellPictureIndicesRect(36, 8, 22, 8, 3, 3);
      Call6((void (*)())Map_CopyCellAttributeRect, 36, 8, 3, 3, 22, 8);
    }
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (GameFlag_Get(FLAG_SERPENT_SLAIN))
    return gIzumo_ActorPlacementsOnceTheSerpentIsSlain;
  return gIzumo_ActorPlacements;
}

/// Cyclone clears a patch of the village's bushes, the first of nine.
void Izumo_CycloneFirstPatch(void) {
  FieldTileCopies_CopyTouchedCells(53, 5, 8, 9, 23, 30, 0);
}

/// Cyclone clears the second patch and, unless its flag is set, uncovers a
/// treasure spot there, pushing off a leader standing on it.
void Izumo_CycloneSecondPatch(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.leaderActorId);

  FieldTileCopies_CopyTouchedCells(53, 5, 8, 9, 23, 30, 0);
  if (!GameFlag_Get(0xf9f)) {
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain,
          FIELD_TREASURE_ACTOR, toFIXED(456), toFIXED(584));
    if (FIELD_CELL(object->x.fixed) == 28 &&
        FIELD_CELL(object->z.fixed) == 36) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, -16, 0);
      object->velocityY = toFIXED(6);
      Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Event_End();
    }
  }
}

/// Cyclone clears the third patch, as the second does with its own cell.
void Izumo_CycloneThirdPatch(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.leaderActorId);

  FieldTileCopies_CopyTouchedCells(53, 5, 8, 9, 23, 30, 0);
  if (!GameFlag_Get(0xfa0)) {
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain,
          FIELD_TREASURE_ACTOR + 1, toFIXED(472), toFIXED(552));
    if (FIELD_CELL(object->x.fixed) == 29 &&
        FIELD_CELL(object->z.fixed) == 34) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Call3((void (*)())Actor_SetDestinationOffsetById,
            gMainRoutineWork.leaderActorId, 0, -16);
      object->velocityY = toFIXED(6);
      Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Event_End();
    }
  }
}

/// Cyclone clears the fourth patch of bushes.
void Izumo_CycloneFourthPatch(void) {
  FieldTileCopies_CopyTouchedCells(44, 5, 8, 5, 7, 32, 0);
}

/// Cyclone clears the fifth patch of bushes.
void Izumo_CycloneFifthPatch(void) {
  FieldTileCopies_CopyTouchedCells(55, 0, 4, 2, 16, 23, 0);
}

/// Cyclone clears the sixth patch of bushes.
void Izumo_CycloneSixthPatch(void) {
  FieldTileCopies_CopyTouchedCells(42, 0, 6, 2, 9, 18, 0);
}

/// Cyclone clears the seventh patch, as the second does with its own cell.
void Izumo_CycloneSeventhPatch(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.leaderActorId);

  FieldTileCopies_CopyTouchedCells(42, 0, 6, 2, 9, 18, 0);
  if (!GameFlag_Get(0xfa1)) {
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain,
          FIELD_TREASURE_ACTOR + 2, toFIXED(168), toFIXED(328));
    if (FIELD_CELL(object->x.fixed) == 10 &&
        FIELD_CELL(object->z.fixed) == 20) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, 16, 0);
      object->velocityY = toFIXED(6);
      Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Event_End();
    }
  }
}

/// Cyclone clears the eighth patch of bushes.
void Izumo_CycloneEighthPatch(void) {
  FieldTileCopies_CopyTouchedCells(49, 0, 5, 4, 24, 16, 0);
}

/// Cyclone clears the ninth patch of bushes.
void Izumo_CycloneNinthPatch(void) {
  FieldTileCopies_CopyTouchedCells(36, 0, 5, 3, 16, 9, 0);
}

/// The leader walks through a door, leaving by the step trigger's exit.
void Izumo_EnterDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeedParameters(gMainRoutineWork.leaderActorId, toFIXED(1) / 2,
                           toFIXED(1) / 4);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Sound_Play(SE_MAP_EXIT);
  Izumo_SetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  if (GameFlag_Get(FLAG_SERPENT_SLAIN))
    return gIzumo_TriggersOnceTheSerpentIsSlain;
  return gIzumo_Triggers;
}

/// The ring of stones: one line after the flag is set, otherwise the full talk
/// and its answer.
void Izumo_TalkToStoneCircleGuard(s32 actor) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (Flag_IsSet(FLAG_GUARD_SAW_STONE_MOVED)) {
    Event_SetMessageId(MsgCanYouImagine);
    // "Can you imagine what it must be like to have the power to move..."
    Event_ShowMessageAndWait(actor, 0);
  } else {
    const u8 *message = MsgSeeThisRing;

    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      Event_SetMessageId(message + 1);
    } else {
      FieldEvent_Wait(20);
      Event_SetMessageId(message + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  }
  Event_End();
}

/// The stone circle's guard during the festival: he shows the ring of stones
/// and asks a question, or once the stone has moved complains he must still
/// stand guard.
void Izumo_TalkToFestivalStoneCircleGuard(s32 actor) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (Flag_IsSet(FLAG_GUARD_SAW_STONE_MOVED)) {
    Event_SetMessageId(MsgButItS);
    // "But it's a festival! Why do I have to keep guarding the stone circle?"
    Event_ShowMessageAndWait(actor, 0);
  } else {
    const u8 *message = MsgSeeThisRingOf;

    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      Event_SetMessageId(message + 1);
    } else {
      FieldEvent_Wait(20);
      Event_SetMessageId(message + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  }
  Event_End();
}

/// A villager asks about Lady Uzume and her family, and answers by the reply.
void Izumo_TellOfUzumesFamily(s32 actor) {
  const u8 *message;

  Event_Start();
  Event_ResolvePendingActions(0);
  message = MsgLadyUzumeAnd;
  Event_SetMessageId(message);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    Event_SetMessageId(message + 1);
  } else {
    FieldEvent_Wait(20);
    Event_SetMessageId(message + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
  Event_End();
}

/// Every fourth frame, colours 1 to 14 of palettes 3, 4 and 5 rotate up by one.
void Izumo_RotateWaterColors(void) {
  volatile u16 *palette;
  s16 saved;
  s32 index;

  if ((gFrameCount & 3) == 0) {
    saved = *(volatile u16 *)&BG_PLTT_COLOR(3, 14);
    index = 14;
  first:
    palette = (u16 *)&BG_PLTT_COLOR(3, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto first;
    *(volatile u16 *)&BG_PLTT_COLOR(3, 1) = saved;

    saved = *(volatile u16 *)&BG_PLTT_COLOR(4, 14);
    index = 14;
  second:
    palette = (u16 *)&BG_PLTT_COLOR(4, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto second;
    *(volatile u16 *)&BG_PLTT_COLOR(4, 1) = saved;

    saved = *(volatile u16 *)&BG_PLTT_COLOR(5, 14);
    index = 14;
  third:
    palette = (u16 *)&BG_PLTT_COLOR(5, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto third;
    *(volatile u16 *)&BG_PLTT_COLOR(5, 1) = saved;
  }
}

/// Susa stops the party on its way out to offer a weapon; refused, he waits by
/// the gate; accepted, he tells the Cloud Brand's legend.
void Izumo_StopThePartyToThankThem(void) {
  GameFlag_Set(FLAG_STOPPED_TO_THANK_THE_PARTY);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgJennaWait);
  Actor_WalkToById(gMainRoutineWork.leaderActorId, 200, 264);
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  // "Felix, wait!"
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  FieldEvent_Wait(15);
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, ACTOR_SUSA,
        toFIXED(200), toFIXED(216));
  Actor_WalkToById(ACTOR_SUSA, 200, 248);
  Actor_WaitForMoveById(ACTOR_SUSA);
  FieldEvent_Wait(30);
  // "It troubled me to let you leave like this..."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_FaceActorById(gMainRoutineWork.leaderActorId, ACTOR_SUSA, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationById(ACTOR_SUSA, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(30);
  // "I owe you much for your help, but I didn't have any way to thank you..."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "And then I thought of something."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_SetAnimationById(ACTOR_SUSA, ANIM_NOD);
  FieldEvent_Wait(15);
  // "You're a warrior, and warriors need weapons. Wouldn't you agree?"
  FieldMessage_ShowAtActor(ACTOR_SUSA, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      1) {
    FieldEvent_Wait(90);
    Call3((void (*)())Actor_SetSpeedParameters, ACTOR_SUSA, toFIXED(2) / 5,
          toFIXED(1) / 5);
    Actor_WalkToById(ACTOR_SUSA, 200, 216);
    Actor_WaitForMoveById(ACTOR_SUSA);
    FieldCamera_MoveToActorById(ACTOR_SUSA, 1);
    FieldEvent_Wait(60);
    Call3((void (*)())Actor_FaceDirectionById, ACTOR_SUSA, FACING_SOUTH, 0);
    Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "Are you sure about that? I was only hoping to show you my appreciation."
    Event_ShowMessageAndWait(ACTOR_SUSA, 0);
    Actor_SetSpritePriority(ACTOR_SUSA, 3);
    Actor_GetById(ACTOR_SUSA)->motionFlags |= 2;
    GameFlag_Set(FLAG_THANKFUL_ONE_WAITS);
  } else {
    Izumo_TellTheCloudBrandLegend();
  }
  Event_End();
}

/// Susa tells the legend of the Cloud Brand, hidden where the Serpent's tail
/// fell, and leaves for Kushinada.
void Izumo_TellTheCloudBrandLegend(void) {
  Event_SetMessageId(MsgThisTokenIsn);
  Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "This token isn't something I possess. It's a legend, one that might..."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_FaceDirectionById(ACTOR_SUSA, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  // "Long ago, the Great Serpent of Mikage would sleep with its body..."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_FaceActorById(ACTOR_SUSA, gMainRoutineWork.leaderActorId, 0);
  Actor_SetAnimationById(ACTOR_SUSA, ANIM_NOD);
  FieldEvent_Wait(30);
  // "Where its tail fell, the Cloud Brand lay hidden. That's the legend..."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_StartRepeatedMotion(ACTOR_SUSA, 1);
  FieldEvent_Wait(30);
  // "The serpent was once the sword's protector, but it lost the sword..."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_SetAnimationById(ACTOR_SUSA, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(30);
  // "Anyway, maybe you want to go check out the dragon again... I left..."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  FieldEvent_Wait(15);
  Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Well, I've said what I came to say. I'm going back to Kushinada."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_FaceActorById(ACTOR_SUSA, gMainRoutineWork.leaderActorId, 0);
  FieldEvent_Wait(15);
  // "I hope you are successful in your quest."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_SetAnimationById(ACTOR_SUSA, ANIM_NOD);
  FieldEvent_Wait(15);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetSpeedParameters(ACTOR_SUSA, toFIXED(1) / 5, toFIXED(1) / 10);
  Actor_GetById(ACTOR_SUSA)->motionFlags = 0;
  Actor_SetSpritePriority(ACTOR_SUSA, 3);
  Actor_WalkToById(ACTOR_SUSA, 200, 208);
  Actor_WaitForMoveById(ACTOR_SUSA);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_SUSA, 0, 0);
  GameFlag_Set(FLAG_TOKEN_OFFERED);
  GameFlag_Clear(FLAG_THANKFUL_ONE_WAITS);
}

/// Susa, waiting by the gate, offers his token of gratitude again; once told,
/// he repeats that it is a legend.
void Izumo_OfferATokenOfGratitude(void) {
  if (!GameFlag_Get(FLAG_TOKEN_OFFERED)) {
    const u8 *message;

    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    message = MsgWouldYouBe;
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(ACTOR_SUSA, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
        0) {
      Izumo_TellTheCloudBrandLegend();
    } else {
      Event_SetMessageId(message - 1);
      Event_ShowMessageAndWait(ACTOR_SUSA, 0);
    }
    Event_End();
  } else {
    Event_ShowMessageAndWait((s32)MsgTheLegendSays, 0);
  }
}

/// Sets up Izumo as the party enters: the fade in, the water's colours, the
/// stone circle, and once the Serpent is slain the dancers and Susa.
s32 Scene_Initialize(void) {
  GameFlag_Clear(FLAG_THANKFUL_ONE_WAITS);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Task_Add(Izumo_RotateWaterColors, TASK_PASS_FRAME);
  Actor_PlaceBlocks(gIzumo_MovableStones);
  if (GameFlag_Get(FLAG_SERPENT_SLAIN)) {
    Actor_GetById(15)->steeringFlags = 2;
    Actor_SetAnimationById(ACTOR_DANCER_A, 5);
    Actor_SetAnimationById(ACTOR_DANCER_B, 5);
    Actor_SetAnimationById(ACTOR_DANCER_C, 5);
    Actor_SetAnimationById(ACTOR_DANCER_D, 5);
    Actor_SetAnimationById(ACTOR_DANCER_E, 6);
    Actor_SetAnimationById(ACTOR_DANCER_F, 6);
    Actor_SetAnimationById(ACTOR_DANCER_G, 5);
    if (gMainRoutineWork.entrance == 8 &&
        GameFlag_Get(FLAG_IZUMO_RECOGNIZED_THE_HEROES) &&
        !GameFlag_Get(FLAG_STOPPED_TO_THANK_THE_PARTY) &&
        !GameFlag_Get(FLAG_TOKEN_OFFERED)) {
      Event_StartScreenTransition();
      Izumo_StopThePartyToThankThem();
    }
    if (GameFlag_Get(FLAG_STOPPED_TO_THANK_THE_PARTY) &&
        !GameFlag_Get(FLAG_TOKEN_OFFERED)) {
      Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, ACTOR_SUSA,
            toFIXED(200), toFIXED(216));
      Actor_SetSpritePriority(ACTOR_SUSA, 3);
      Actor_GetById(ACTOR_SUSA)->motionFlags |= 2;
      GameFlag_Set(FLAG_THANKFUL_ONE_WAITS);
    }
  }
  Actor_SetSpritePriority(24, 3);
  Actor_GetById(24)->motionFlags |= 2;
  return 0;
}

/// Nothing to prepare before the map is built. Returns 0.
s32 Scene_PrepareMap(void) { return 0; }

/// Reveal on the stone circle: shows the way down if the stone is in place.
void Izumo_RevealTheStoneCircle(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);

  GameFlag_Set(FLAG_REVEALING);
  if (GameFlag_Get(FLAG_STONE_IN_PLACE)) {
    MapGrid_CopyCellPictureIndicesRect(36, 12, 22, 8, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 36, 12, 3, 3, 22, 8);
    if (x == 23 && z == 9)
      Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.leaderActorId,
                                             toFIXED(376), toFIXED(136));
  } else {
    MapGrid_CopyCellPictureIndicesRect(36, 8, 22, 8, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 36, 8, 3, 3, 22, 8);
  }
}

/// Reveal ends: the stone circle's way down is hidden.
void Izumo_HideTheStoneCircle(void) {
  GameFlag_Clear(FLAG_REVEALING);
  MapGrid_CopyCellPictureIndicesRect(40, 8, 22, 8, 3, 3);
  Call6((void (*)())Map_CopyCellAttributeRect, 40, 8, 3, 3, 22, 8);
}

/// Felix walks down into the stone circle's opening and the scene leaves for
/// the Izumo Ruins.
void Izumo_DescendIntoTheRuins(void) {
  s32 value = gFieldWork->stepTrigger;

  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestinationOffsetById(ACTOR_FELIX, 0, 4);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FELIX), 0);
#if TLA_PATCH_1 /* From patch 1. */
  Sound_Play(SE_STAIRS_DOWN);
#endif
  Actor_SetDestinationOffsetById(ACTOR_FELIX, 0, 16);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetAnimationById(ACTOR_FELIX, 13);
  FieldEvent_Wait(14);
  Izumo_SetExitRequest(value);
}

/// Felix steps in from the circle's left and goes down into the ruins.
void Izumo_EnterTheRuinsFromTheLeft(void) {
  Call3((void (*)())Actor_WalkByAndWaitById, ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWaitById(ACTOR_FELIX, 16, 0);
  FieldEvent_Wait(10);
  Izumo_DescendIntoTheRuins();
}

/// Felix steps in from the circle's right and goes down into the ruins.
void Izumo_EnterTheRuinsFromTheRight(void) {
  Call3((void (*)())Actor_WalkByAndWaitById, ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWaitById(ACTOR_FELIX, -16, 0);
  FieldEvent_Wait(10);
  Izumo_DescendIntoTheRuins();
}

/// Felix faces the circle's opening and goes down into the ruins.
void Izumo_EnterTheRuins(void) {
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Izumo_DescendIntoTheRuins();
}

#include "text/msg_ids.h"

/// The stones Move can lift, ended by -1: the misaligned stone.
s16 gIzumo_MovableStones[] = {
    ACTOR_MISALIGNED_STONE,
    -1,
};

// clang-format off
u32 gIzumo_FirstSpinnerScript[] = {                                                               
      SC_SET(SW_COLLISION, 129),                                               
      SC_ACCELERATION(0.2),                                                
      SC_SPEED(0.6),                                                
      SC_SET(SW_STEERING, 2),                                                  
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_LABEL(1),                                                             
      SC_WAIT(30),                                                             
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(60),                                                             
      SC_WAIT(30),                                                             
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(60),                                                             
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
      SC_END,                                                                  
  };

u32 gIzumo_SecondSpinnerScript[] = {                                                               
      SC_SET(SW_COLLISION, 129),                                               
      SC_ACCELERATION(0.2),                                                
      SC_SPEED(0.6),                                                
      SC_SET(SW_STEERING, 2),                                                  
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_LABEL(1),                                                             
      SC_WAIT(30),                                                             
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(60),                                                             
      SC_WAIT(30),                                                             
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(60),                                                             
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
      SC_END,                                                                  
  };

u32 gIzumo_ThirdSpinnerScript[] = {                                                               
      SC_SET(SW_COLLISION, 129),                                               
      SC_ACCELERATION(0.2),                                                    
      SC_SPEED(0.6),                                                
      SC_SET(SW_STEERING, 2),                                                  
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_LABEL(1),                                                             
      SC_WAIT(30),                                                             
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(60),                                                             
      SC_WAIT(30),                                                             
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(60),                                                             
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
      SC_END,                                                                  
  };

u32 gIzumo_FourthSpinnerScript[] = {                                                               
      SC_SET(SW_COLLISION, 129),                                               
      SC_ACCELERATION(0.2),                                                
      SC_SPEED(0.6),                                                
      SC_SET(SW_STEERING, 2),                                                  
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_LABEL(1),                                                             
      SC_WAIT(30),                                                             
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(60),                                                             
      SC_WAIT(30),                                                             
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(60),                                                             
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
      SC_END,                                                                  
  };

u32 gIzumo_FifthSpinnerScript[] = {                                                               
      SC_SET(SW_COLLISION, 129),                                               
      SC_ACCELERATION(0.2),                                                
      SC_SPEED(0.6),                                                
      SC_SET(SW_STEERING, 2),                                                  
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_LABEL(1),                                                             
      SC_WAIT(30),                                                             
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(60),                                                             
      SC_WAIT(30),                                                             
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(60),                                                             
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
      SC_END,                                                                  
  };

u32 gIzumo_SixthSpinnerScript[] = {                                                               
      SC_SET(SW_COLLISION, 129),                                               
      SC_ACCELERATION(0.3),                                                    
      SC_SPEED(0.6),                                                
      SC_SET(SW_STEERING, 2),                                                  
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_LABEL(1),                                                             
      SC_WAIT(30),                                                             
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(60),                                                             
      SC_WAIT(30),                                                             
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(5),                                                              
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(60),                                                             
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
      SC_END,                                                                  
  };

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(Izumo),
    SCENE_EXIT_UNLESS(1, WorldMap, 38, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, IzumoInteriors, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, IzumoInteriors, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, IzumoInteriors, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, IzumoInteriors, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, IzumoInteriors, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, IzumoInteriors, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, IzumoInteriors, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, IzumoRuinsFirstRoom, 1, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gIzumo_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 120, 0, 488, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 488, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 568, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 248, 0, 464, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 344, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 424, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 488, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 360, 0, 392, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 117, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 155, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 155, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gIzumo_ActorPlacementsOnceTheSerpentIsSlain[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 120, 0, 488, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 568, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 472, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 344, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 424, 0, 568, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 488, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 360, 0, 392, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 117, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 155, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 155, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (s32)gIzumo_FirstSpinnerScript, 256, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (s32)gIzumo_SecondSpinnerScript, 264, 0, 504, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (s32)gIzumo_ThirdSpinnerScript, 360, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 472, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, (s32)gIzumo_FourthSpinnerScript, 232, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, (s32)gIzumo_FifthSpinnerScript, 328, 0, 504, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_WOMAN, CONDITION_ALWAYS, (s32)gIzumo_SixthSpinnerScript, 336, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUSA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gIzumo_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS,
     EVENT(Izumo_EnterTheRuins)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS,
     EVENT(Izumo_EnterTheRuinsFromTheRight)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS,
     EVENT(Izumo_EnterTheRuinsFromTheLeft)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgWeVeHad)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgMyNameS)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLadyUzumeDoesn)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgSusaSGoing)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAsAChild)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgChoosingTheNext)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTheDaughtersOf)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgABraveWarrior)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Izumo_TalkToStoneCircleGuard)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgYeahSusaCame)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgEvenIfSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgTheGreatSerpent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgIMNotGoing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgIMSureThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgWhatIfLosing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgLadyUzumeLoves)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgSacrificesAsGreat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgKushinadaSAlways)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgWhyWonT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgWhyDoesLady)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgYouKnowI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgLadyUzumeSays)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Izumo_StartMovingStone)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Izumo_FinishMovingStone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     CONDITION_ALWAYS, EVENT(Izumo_StartMovingStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS,
     EVENT(Izumo_FinishMovingStone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 21,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneFirstPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 22,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneSecondPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 23,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneThirdPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 24,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneFourthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 25,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneFifthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 26,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneSixthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 27,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneSeventhPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 28,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneEighthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 29,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneNinthPatch)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Izumo_RevealTheStoneCircle)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Izumo_HideTheStoneCircle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIzumo_TriggersOnceTheSerpentIsSlain[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, FLAG_THANKFUL_ONE_WAITS,
     EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS,
     EVENT(Izumo_EnterTheRuins)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS,
     EVENT(Izumo_EnterTheRuinsFromTheRight)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS,
     EVENT(Izumo_EnterTheRuinsFromTheLeft)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgThoseScreamsFrom)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgWeRePlanning)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgSusaHasChanged)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKushinadaHasBeen)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgIVeHeardPeople)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgSusaHasGrown)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgSusaHasPut)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgLadyUzumePredicted)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS,
     EVENT(Izumo_TalkToFestivalStoneCircleGuard)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgIfYouReLooking)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgDidYouHear)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgIWonderIf)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgDancingLikeThis)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgEveryoneSaysThat)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgWeMayNever)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgOurIslandIs)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, MSGNO(MsgTheSerpentTaught)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, EVENT(Izumo_TellOfUzumesFamily)},
    {TRIGGER_TALK, 32, CONDITION_ALWAYS, EVENT(Izumo_OfferATokenOfGratitude)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgIHadAlways)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgThisCelebrationWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgItSOdd)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgKushinadaIsSuch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgWhatWouldMake)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgAllTheGirls)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgEveryoneSBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgSoSusaReally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgSusaDidA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgLadyUzumeAndKushinada)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgKushinadaMustBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgAllAnyoneTalks)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS,
     MSGNO(MsgADragonA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS,
     MSGNO(MsgSusaSaidThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS,
     MSGNO(MsgWeNeedTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS,
     MSGNO(MsgDancingIsGood)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS,
     MSGNO(MsgThatSusaUsed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS,
     MSGNO(MsgAsLongAsWe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 32 | TRIGGER_SUBJECT_OWN_EFFECT,
     CONDITION_ALWAYS, EVENT(Izumo_OfferATokenOfGratitude)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     CONDITION_ALWAYS, EVENT(Izumo_StartMovingStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS,
     EVENT(Izumo_FinishMovingStone)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Izumo_StartMovingStone)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Izumo_FinishMovingStone)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Izumo_RevealTheStoneCircle)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Izumo_HideTheStoneCircle)},
#if TLA_PATCH_1 /* From patch 1. */
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 21,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneFirstPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 22,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneSecondPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 23,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneThirdPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 24,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneFourthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 25,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneFifthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 26,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneSixthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 27,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneSeventhPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 28,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneEighthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 29,
     CONDITION_ALWAYS, EVENT(Izumo_CycloneNinthPatch)},
#endif
    {TRIGGER_TABLE_END},
};

/// Unused: a halfword nothing reads.
u16 gIzumo_UnusedHalfword[] = {
    1,
};
