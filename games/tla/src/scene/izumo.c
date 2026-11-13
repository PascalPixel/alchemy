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
#include "field/emote.h"
#include "sounds.h"
#include "event_work.h"
#include "types.h"
#include "flags.h"
#include "system/task.h"
#include "system/ram_buffer.h"
#include "field_event.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "call.h"
#include "flag_call.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/io_reg.h"
#include "version.h"
#include "system/frame.h"
#include "objcmd.h"

// Set as the stone circle's guard finds the misaligned stone back in place.
#define FLAG_GUARD_SAW_STONE_MOVED 0x924
// Set while Move has set the stone in its place, for this visit.
#define FLAG_STONE_IN_PLACE 0x250
// Set while Reveal is shown.
#define FLAG_REVEALING 0x301
// Set while the one who stopped the party waits by the gate.
#define FLAG_THANKFUL_ONE_WAITS 0x302
// Set as the party is stopped on its way out to be thanked.
#define FLAG_STOPPED_TO_THANK_THE_PARTY 0x929
// Set once the token of gratitude has been offered.
#define FLAG_TOKEN_OFFERED 0x92a

extern u32 gIzumo_FirstSpinnerScript[];
extern u32 gIzumo_SecondSpinnerScript[];
extern u32 gIzumo_ThirdSpinnerScript[];
extern u32 gIzumo_FourthSpinnerScript[];
extern u32 gIzumo_FifthSpinnerScript[];
extern u32 gIzumo_SixthSpinnerScript[];
void Izumo_RevealTheStoneCircle(void);

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

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
extern u8 MsgWait2[];
extern u8 MsgThisTokenIsn[];
extern u8 MsgTheMisalignedStone[];

struct Actor *Actor_GetById(u32 objectId);
void FieldEvent_Wait(s32 frames);
void FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Actor_SetDestinationOffsetById(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_WalkToById(s32 id, s32 x, s32 z);
void Actor_FaceActorById(s32 id, s32 target, s32 frames);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void MapGrid_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Actor_ShowEmoteById(s32 id, s32 a, s32 b);
void FieldEffect_SetAttached(s32 id, s32 a);
void FieldCamera_WaitForArrival(void);
void FieldCamera_MoveToActorById(s32 id, s32 value);
void Func_02000db0(s32 value);
void Actor_PlaceBlocks(const s16 *data);
void Actor_LiftTargetBlock(const s16 *data);
void Actor_PlaceMovedBlock(void);
void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);
void Sound_Play(s32 cue);
void Izumo_RotateWaterColors(void);
void Izumo_StopThePartyToThankThem(void);
void Izumo_TellTheCloudBrandLegend(void);

void Izumo_StartMovingStone(void) {
  Actor_LiftTargetBlock(gIzumo_MovableStones);
}

void Izumo_FinishMovingStone(void) {
  struct Actor *actor = Actor_GetById(12);
  s32 x = actor->x >> 20;
  s32 z = actor->z >> 20;

  /* FAKEMATCH(wrapper): the flag commands go through flag_call.h's inlines. */
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
      FieldCamera_FollowActor(22, 1);
      FieldCamera_WaitForArrival();
      Actor_ShowEmoteById(22, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      FieldEvent_Wait(60);
      Actor_ArmCallback(22, FACING_WEST, 0);
      FieldEvent_Wait(60);
      Actor_ShowEmoteById(22, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      FieldEvent_Wait(60);
      Event_SetMessageId(MsgTheMisalignedStone);
      Event_ShowMessageAndWait(22, 0);
      FieldEvent_Wait(30);
      Actor_ShowEmoteById(22, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
      FieldEvent_Wait(30);
      Event_ShowMessageAndWait(22, 0);
      Actor_ArmCallback(22, FACING_SOUTH, 0);
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

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (GameFlag_Get(FLAG_SERPENT_SLAIN))
    return gIzumo_ActorPlacementsOnceTheSerpentIsSlain;
  return gIzumo_ActorPlacements;
}

void Izumo_CycloneFirstPatch(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 53, 5, 8, 9, 23, 30, 0);
}

void Izumo_CycloneSecondPatch(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);

  Call7((void (*)())TileCopies_CopyTouchedCells, 53, 5, 8, 9, 23, 30, 0);
  if (!GameFlag_Get(3999)) {
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 64, PIXELS(456),
          PIXELS(584));
    if (object->x >> 20 == 28 && object->z >> 20 == 36) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffsetById(gMainRoutineWork.currentOwner, -16, 0);
      object->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Izumo_CycloneThirdPatch(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);

  Call7((void (*)())TileCopies_CopyTouchedCells, 53, 5, 8, 9, 23, 30, 0);
  if (!GameFlag_Get(4000)) {
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 65, PIXELS(472),
          PIXELS(552));
    if (object->x >> 20 == 29 && object->z >> 20 == 34) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Call3((void (*)())Actor_SetDestinationOffsetById,
            gMainRoutineWork.currentOwner, 0, -16);
      object->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Izumo_CycloneFourthPatch(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 44, 5, 8, 5, 7, 32, 0);
}

void Izumo_CycloneFifthPatch(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 55, 0, 4, 2, 16, 23, 0);
}

void Izumo_CycloneSixthPatch(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 42, 0, 6, 2, 9, 18, 0);
}

void Izumo_CycloneSeventhPatch(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);

  Call7((void (*)())TileCopies_CopyTouchedCells, 42, 0, 6, 2, 9, 18, 0);
  if (!GameFlag_Get(4001)) {
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 66, PIXELS(168),
          PIXELS(328));
    if (object->x >> 20 == 10 && object->z >> 20 == 20) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffsetById(gMainRoutineWork.currentOwner, 16, 0);
      object->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Izumo_CycloneEighthPatch(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 49, 0, 5, 4, 24, 16, 0);
}

void Izumo_CycloneNinthPatch(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 36, 0, 5, 3, 16, 9, 0);
}

void Izumo_EnterDoor(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Sound_Play(SE_MAP_EXIT);
  Func_02000db0(*(s16 *)(work + 340));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

const struct Trigger *Scene_GetEvents(void) {
  if (GameFlag_Get(FLAG_SERPENT_SLAIN))
    return gIzumo_TriggersOnceTheSerpentIsSlain;
  return gIzumo_Triggers;
}

static inline s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH(wrapper): the flag test forwarded through the inline. */
  return GameFlag_Get(flag);
}

/// The ring of stones: one line after the flag is set, otherwise the full talk
/// and its answer.
void Izumo_TalkToStoneCircleGuard(s32 actor) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test_(FLAG_GUARD_SAW_STONE_MOVED)) {
    Event_SetMessageId(MsgCanYouImagine);
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

void Izumo_TalkToFestivalStoneCircleGuard(s32 actor) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test_(FLAG_GUARD_SAW_STONE_MOVED)) {
    Event_SetMessageId(MsgButItS);
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

void Izumo_StopThePartyToThankThem(void) {
  /* FAKEMATCH(signature): six routines called through call.h's Call3 and Value
   * with function types they do not have. */
  GameFlag_Set(FLAG_STOPPED_TO_THANK_THE_PARTY);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgWait2);
  Actor_WalkToById(gMainRoutineWork.currentOwner, 200, 264);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Event_ShowMessageAndWait(32, 0);
  FieldEvent_Wait(15);
  Value3((s32 (*)())Actor_ShowEmoteById, gMainRoutineWork.currentOwner, 256,
         30);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 32, PIXELS(200),
        PIXELS(216));
  Actor_WalkToById(32, 200, 248);
  Actor_WaitForMoveById(32);
  FieldEvent_Wait(30);
  Event_ShowMessageAndWait(32, 0);
  Actor_FaceActorById(gMainRoutineWork.currentOwner, 32, 0);
  FieldEvent_Wait(30);
  Actor_SetModeById(32, 4);
  FieldEvent_Wait(30);
  Event_ShowMessageAndWait(32, 0);
  Value3((s32 (*)())Actor_ShowEmoteById, gMainRoutineWork.currentOwner, 261,
         60);
  Actor_ShowEmoteById(32, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetModeById(32, 3);
  FieldEvent_Wait(15);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 32, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    FieldEvent_Wait(90);
    Call3((void (*)())Actor_SetSpeedParameters, 32, 0x6666, 0x3333);
    Actor_WalkToById(32, 200, 216);
    Actor_WaitForMoveById(32);
    FieldCamera_MoveToActorById(32, 1);
    FieldEvent_Wait(60);
    Call3((void (*)())Actor_ArmCallback, 32, FACING_SOUTH, 0);
    Actor_ShowEmoteById(32, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    Event_ShowMessageAndWait(32, 0);
    Actor_SetSpritePriority(32, 3);
    Actor_GetById(32)->motionFlags |= 2;
    GameFlag_Set(FLAG_THANKFUL_ONE_WAITS);
  } else {
    Izumo_TellTheCloudBrandLegend();
  }
  Event_End();
}

void Izumo_TellTheCloudBrandLegend(void) {
  Event_SetMessageId(MsgThisTokenIsn);
  Actor_ShowEmoteById(32, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageAndWait(32, 0);
  Actor_ArmCallback(32, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Event_ShowMessageAndWait(32, 0);
  Actor_FaceActorById(32, gMainRoutineWork.currentOwner, 0);
  Actor_SetModeById(32, 3);
  FieldEvent_Wait(30);
  Event_ShowMessageAndWait(32, 0);
  Actor_StartRepeatedMotion(32, 1);
  FieldEvent_Wait(30);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetModeById(32, 4);
  FieldEvent_Wait(30);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(15);
  Actor_ShowEmoteById(32, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageAndWait(32, 0);
  Actor_FaceActorById(32, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(15);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetModeById(32, 3);
  FieldEvent_Wait(15);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(30);
  Actor_SetSpeedParameters(32, 0x3333, 0x1999);
  Actor_GetById(32)->motionFlags = 0;
  Actor_SetSpritePriority(32, 3);
  Actor_WalkToById(32, 200, 208);
  Actor_WaitForMoveById(32);
  Actor_SetHorizontalPositionWithTerrain(32, 0, 0);
  GameFlag_Set(FLAG_TOKEN_OFFERED);
  GameFlag_Clear(FLAG_THANKFUL_ONE_WAITS);
}

void Izumo_OfferATokenOfGratitude(void) {
  if (!GameFlag_Get(FLAG_TOKEN_OFFERED)) {
    const u8 *message;

    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_ShowEmoteById(32, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    message = MsgWouldYouBe;
    Event_SetMessageId(message);
    /* FAKEMATCH(signature): FieldMessage_ShowAtActor called as returning a
     * value. */
    ((s32 (*)(s32, s32))FieldMessage_ShowAtActor)(32, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Izumo_TellTheCloudBrandLegend();
    } else {
      Event_SetMessageId(message - 1);
      Event_ShowMessageAndWait(32, 0);
    }
    Event_End();
  } else {
    Event_ShowMessageAndWait((s32)MsgTheLegendSays, 0);
  }
}

s32 Scene_Initialize(void) {
  GameFlag_Clear(FLAG_THANKFUL_ONE_WAITS);
  WORD_AT(Ram_HeapSlots->eventWork, 428) = 0x100;
  Task_Add(Izumo_RotateWaterColors, TASK_PASS_FRAME);
  Actor_PlaceBlocks(gIzumo_MovableStones);
  if (GameFlag_Get(FLAG_SERPENT_SLAIN)) {
    BYTE_AT(Actor_GetById(15), 90) = 2;
    Actor_SetModeById(25, 5);
    Actor_SetModeById(26, 5);
    Actor_SetModeById(27, 5);
    Actor_SetModeById(28, 5);
    Actor_SetModeById(29, 6);
    Actor_SetModeById(30, 6);
    Actor_SetModeById(31, 5);
    if (gMainRoutineWork.entrance == 8 &&
        GameFlag_Get(FLAG_IZUMO_RECOGNIZED_THE_HEROES) &&
        !GameFlag_Get(FLAG_STOPPED_TO_THANK_THE_PARTY) &&
        !GameFlag_Get(FLAG_TOKEN_OFFERED)) {
      Event_StartScreenTransition();
      Izumo_StopThePartyToThankThem();
    }
    if (GameFlag_Get(FLAG_STOPPED_TO_THANK_THE_PARTY) &&
        !GameFlag_Get(FLAG_TOKEN_OFFERED)) {
      Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 32, PIXELS(200),
            PIXELS(216));
      Actor_SetSpritePriority(32, 3);
      Actor_GetById(32)->motionFlags |= 2;
      GameFlag_Set(FLAG_THANKFUL_ONE_WAITS);
    }
  }
  Actor_SetSpritePriority(24, 3);
  Actor_GetById(24)->motionFlags |= 2;
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Izumo_RevealTheStoneCircle(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  GameFlag_Set(FLAG_REVEALING);
  if (GameFlag_Get(FLAG_STONE_IN_PLACE)) {
    MapGrid_CopyCellPictureIndicesRect(36, 12, 22, 8, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 36, 12, 3, 3, 22, 8);
    if (x == 23 && z == 9)
      Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                             PIXELS(376), PIXELS(136));
  } else {
    MapGrid_CopyCellPictureIndicesRect(36, 8, 22, 8, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 36, 8, 3, 3, 22, 8);
  }
}

void Izumo_HideTheStoneCircle(void) {
  GameFlag_Clear(FLAG_REVEALING);
  MapGrid_CopyCellPictureIndicesRect(40, 8, 22, 8, 3, 3);
  Call6((void (*)())Map_CopyCellAttributeRect, 40, 8, 3, 3, 22, 8);
}

void Izumo_DescendIntoTheRuins(void) {
  s32 value = ((struct EventWork *)Ram_HeapSlots->eventWork)->exit;

  Actor_ArmCallback(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  BYTE_AT(Actor_GetById(4), 90) &= ~1;
  Actor_SetDestinationOffsetById(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSpriteFlags(Actor_GetById(4), 0);
#if TLA_PATCH_1 /* From patch 1. */
  Sound_Play(SE_STAIRS_DOWN);
#endif
  Actor_SetDestinationOffsetById(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 13);
  FieldEvent_Wait(14);
  Func_02000db0(value);
}

void Izumo_EnterTheRuinsFromTheLeft(void) {
  Call3((void (*)())Actor_WalkByAndWaitById, 4, 0, -14);
  Actor_WalkByAndWaitById(4, 16, 0);
  FieldEvent_Wait(10);
  Izumo_DescendIntoTheRuins();
}

void Izumo_EnterTheRuinsFromTheRight(void) {
  Call3((void (*)())Actor_WalkByAndWaitById, 4, 0, -14);
  Actor_WalkByAndWaitById(4, -16, 0);
  FieldEvent_Wait(10);
  Izumo_DescendIntoTheRuins();
}

void Izumo_EnterTheRuins(void) {
  Actor_ArmCallback(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Izumo_DescendIntoTheRuins();
}

#include "text/msg_ids.h"

s16 gIzumo_MovableStones[] = {
    12,
    -1,
};

// clang-format off
DEFINE_SPIN_FROM_SOUTH_SCRIPT(gIzumo_FirstSpinnerScript)

DEFINE_SPIN_FROM_EAST_SCRIPT(gIzumo_SecondSpinnerScript)

DEFINE_SPIN_FROM_NORTH_SCRIPT(gIzumo_ThirdSpinnerScript, 0x3333)

DEFINE_SPIN_FROM_SOUTH_SCRIPT(gIzumo_FourthSpinnerScript)

DEFINE_SPIN_FROM_EAST_SCRIPT(gIzumo_FifthSpinnerScript)

DEFINE_SPIN_FROM_NORTH_SCRIPT(gIzumo_SixthSpinnerScript, 0x4ccc)

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
    {272, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(120), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(120), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(184), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(184), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(152), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(152), FACING_NORTH, 2, 0},
    {117, CONDITION_ALWAYS, 3, PIXELS(120), 0, PIXELS(488), FACING_WEST, 1, 0},
    {120, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(488), FACING_SOUTH, 0, 0},
    {118, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(568), FACING_SOUTH, 1, 0},
    {122, CONDITION_ALWAYS, 3, PIXELS(248), 0, PIXELS(464), FACING_NORTH, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(344), FACING_SOUTH, 0, 0},
    {125, CONDITION_ALWAYS, 3, PIXELS(424), 0, PIXELS(600), FACING_SOUTH, 1, 0},
    {126, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(488), FACING_NORTHEAST, 1, 0},
    {127, CONDITION_ALWAYS, 3, PIXELS(360), 0, PIXELS(392), FACING_WEST, 1, 0},
    {117, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(117), FACING_SOUTH, 1, 0},
    {117, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(155), FACING_SOUTH, 1, 0},
    {117, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(155), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gIzumo_ActorPlacementsOnceTheSerpentIsSlain[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(120), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(120), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(184), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(184), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(152), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(152), FACING_NORTH, 2, 0},
    {117, CONDITION_ALWAYS, 3, PIXELS(120), 0, PIXELS(488), FACING_WEST, 1, 0},
    {120, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(552), FACING_SOUTH, 2, 0},
    {118, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(568), FACING_SOUTH, 1, 0},
    {122, CONDITION_ALWAYS, 3, PIXELS(472), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(344), FACING_SOUTH, 0, 0},
    {125, CONDITION_ALWAYS, 3, PIXELS(424), 0, PIXELS(568), FACING_SOUTH, 1, 0},
    {126, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(488), FACING_NORTHEAST, 1, 0},
    {127, CONDITION_ALWAYS, 3, PIXELS(360), 0, PIXELS(392), FACING_WEST, 1, 0},
    {117, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(117), FACING_SOUTH, 1, 0},
    {117, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(155), FACING_SOUTH, 1, 0},
    {117, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(155), FACING_SOUTH, 1, 0},
    {120, CONDITION_ALWAYS, (s32)gIzumo_FirstSpinnerScript, PIXELS(256), 0, PIXELS(424), FACING_SOUTH, 1, 0},
    {120, CONDITION_ALWAYS, (s32)gIzumo_SecondSpinnerScript, PIXELS(264), 0, PIXELS(504), FACING_SOUTH, 1, 0},
    {120, CONDITION_ALWAYS, (s32)gIzumo_ThirdSpinnerScript, PIXELS(360), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(472), FACING_NORTHEAST, 1, 0},
    {124, CONDITION_ALWAYS, (s32)gIzumo_FourthSpinnerScript, PIXELS(232), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {124, CONDITION_ALWAYS, (s32)gIzumo_FifthSpinnerScript, PIXELS(328), 0, PIXELS(504), FACING_SOUTH, 1, 0},
    {125, CONDITION_ALWAYS, (s32)gIzumo_SixthSpinnerScript, PIXELS(336), 0, PIXELS(424), FACING_SOUTH, 1, 0},
    {29, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 1, 0},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1056, CONDITION_ALWAYS,
     EVENT(Izumo_OfferATokenOfGratitude)},
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
