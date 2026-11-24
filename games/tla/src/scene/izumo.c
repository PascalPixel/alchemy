/// @file
/// Izumo, the village of houses on stilts on the far eastern island of the
/// Eastern Sea. Kushinada is to be offered there to the Serpent of Mt. Mikage
/// (Gaia Rock). A river winds through it. Susa and Lady Uzume's house stands
/// in the north, Kushinada's in the north-west, the inn and the shop in the
/// west and the Sanctum on the raised ground in the east.
///
/// North of the houses a ring of giant stones stands on an island in the
/// river, guarded by a woman at its bridge, and one stone of the ring is out
/// of line. Move pushes it east back into place (Izumo_FinishMovingStone), and
/// with it there Reveal shows the way down into the Izumo Ruins in the ring's
/// centre (Izumo_RevealTheStoneCircle, Izumo_DescendIntoTheRuins). Cyclone
/// scatters the village's weeds: under them lie two Antidotes in the
/// south-east corner and a Lucky Medal on the raised ground north of the inn.
///
/// Once the Serpent is slain the village holds a festival, its dancers turning
/// on the spot in the square (gIzumo_FirstDanceScript). The first time the
/// party leaves Susa's house after being hailed there as heroes, Susa runs out
/// after it to thank it and tells it the legend of the Cloud Brand
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
#include "system/main_routine_state.h"
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

/// Set once the stone circle's guard has seen the stone back in its place.
#define FLAG_GUARD_SAW_STONE_MOVED 0x924
/// Set while the out-of-line stone stands back in the ring, for this visit.
#define FLAG_STONE_IN_PLACE FLAG_SCENE_TEMP_50
/// Set while Reveal is showing.
#define FLAG_REVEALING FLAG_AREA_TEMP_01

/// The village's map cells that answer Cyclone: the weeds over the two
/// Antidotes and the Lucky Medal, and the rest of each patch.
enum {
  CELL_SOUTHEAST_WEEDS = 21,
  CELL_WEEDS_OVER_FIRST_ANTIDOTE,
  CELL_WEEDS_OVER_SECOND_ANTIDOTE,
  CELL_WEEDS_BY_THE_SHOP,
  CELL_RIVERBANK_WEEDS,
  CELL_LEDGE_WEEDS,
  CELL_WEEDS_OVER_LUCKY_MEDAL,
  CELL_WEEDS_BY_THE_SANCTUM,
  CELL_STONE_CIRCLE_WEEDS
};

/// The village's actors the events move: the stone out of line in the ring,
/// Okuni the dancer, the woman who guards the ring, the woman on the porch west
/// of Kushinada's door, the seven who dance once the Serpent is slain, and
/// Susa.
enum {
  ACTOR_MISALIGNED_STONE = 12,
  ACTOR_OKUNI = 15,
  ACTOR_STONE_CIRCLE_GUARD = 22,
  ACTOR_PORCH_WOMAN = 24,
  ACTOR_DANCER_A = 25,
  ACTOR_DANCER_B,
  ACTOR_DANCER_C,
  ACTOR_DANCER_D,
  ACTOR_DANCER_E,
  ACTOR_DANCER_F,
  ACTOR_DANCER_G,
  ACTOR_SUSA
};
/// Set while Susa, his thanks refused, waits in the doorway of his house.
#define FLAG_SUSA_WAITS FLAG_AREA_TEMP_02
/// Set as Susa stops the party outside his house to thank it.
#define FLAG_STOPPED_TO_THANK_THE_PARTY 0x929
/// Set once Susa has told the party the legend of the Cloud Brand.
#define FLAG_TOKEN_OFFERED 0x92a

/// The out-of-line stone's cell, and the cell east of it where it belongs in
/// the ring.
#define STONE_MISALIGNED_CELL_X 19
#define STONE_IN_PLACE_CELL_X 20
#define STONE_CELL_Z 9
/// The ring's centre, where Reveal shows the way down: a leader standing there
/// as it opens is set on the cell north of it.
#define CIRCLE_CENTER_CELL_X 23
#define CIRCLE_CENTER_CELL_Z 9

/// Where the two Antidotes and the Lucky Medal lie under the weeds, in pixels:
/// cells (28, 36), (29, 34) and (10, 20).
#define FIRST_ANTIDOTE_SPOT_X 456
#define FIRST_ANTIDOTE_SPOT_Z 584
#define SECOND_ANTIDOTE_SPOT_X 472
#define SECOND_ANTIDOTE_SPOT_Z 552
#define LUCKY_MEDAL_SPOT_X 168
#define LUCKY_MEDAL_SPOT_Z 328

/// The river's three palettes, from the first, and the colours of each that
/// shift to make it shimmer.
#define FIRST_WATER_PALETTE 3
#define WATER_FIRST_COLOR 1
#define WATER_LAST_COLOR 14

extern u32 gIzumo_FirstDanceScript[];
extern u32 gIzumo_SecondDanceScript[];
extern u32 gIzumo_ThirdDanceScript[];
extern u32 gIzumo_FourthDanceScript[];
extern u32 gIzumo_FifthDanceScript[];
extern u32 gIzumo_SixthDanceScript[];
void Izumo_RevealTheStoneCircle(void);

extern s16 gIzumo_StonesToMove[];
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
extern u8 MsgFelixWait[];
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
/// Leaves Izumo by one of its exits, as Event_SetExitRequest does.
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

/// Move (or a push) takes hold of the stone out of line in the ring of giant
/// stones, on the west side of the ring at cell (19, 9).
void Izumo_StartMovingStone(void) {
  Actor_LiftTargetBlock(gIzumo_StonesToMove);
}

/// Move sets the out-of-line stone down. Pushed east into the gap in the
/// ring, cell (20, 9), it is back in place: the first time, the woman guarding
/// the ring turns to look, cries "The misaligned stone! It's back in its
/// original position!" and decides Susa or Kushinada must have moved it. Pushed
/// back west to cell (19, 9), it is out of line again.
///
/// While Reveal is showing, the way down in the ring's centre opens or closes
/// with the stone.
void Izumo_FinishMovingStone(void) {
  struct Actor *actor = Actor_GetById(ACTOR_MISALIGNED_STONE);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);

  Actor_PlaceMovedBlock();
  Sound_Play(SE_WATER);
  if (x == STONE_IN_PLACE_CELL_X && z == STONE_CELL_Z) {
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
  } else if (x == STONE_MISALIGNED_CELL_X && z == STONE_CELL_Z) {
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

/// Cyclone cast in the weeds of the village's south-east corner, between the
/// river and the trees along the east edge (cells (23, 30) to (30, 38)),
/// scatters those within two cells of Felix.
void Izumo_ScatterSoutheastWeeds(void) {
  FieldTileCopies_CopyTouchedCells(53, 5, 8, 9, 23, 30, 0);
}

/// Cyclone cast standing on cell (28, 36) in the south-east weeds scatters
/// them and, until it is found, uncovers an Antidote under Felix's feet: he
/// jumps a cell west off it, and it lies in the cleared grass.
void Izumo_ScatterWeedsOverFirstAntidote(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineState.leaderActorId);

  FieldTileCopies_CopyTouchedCells(53, 5, 8, 9, 23, 30, 0);
  if (!GameFlag_Get(FLAG_IZUMO_FIRST_ANTIDOTE)) {
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain,
          FIELD_TREASURE_ACTOR, toFIXED(FIRST_ANTIDOTE_SPOT_X),
          toFIXED(FIRST_ANTIDOTE_SPOT_Z));
    if (FIELD_CELL(actor->x.fixed) ==
            FIELD_CELL(toFIXED(FIRST_ANTIDOTE_SPOT_X)) &&
        FIELD_CELL(actor->z.fixed) ==
            FIELD_CELL(toFIXED(FIRST_ANTIDOTE_SPOT_Z))) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldEffect_SetAttached(gMainRoutineState.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffsetById(gMainRoutineState.leaderActorId, -16, 0);
      actor->velocityY = toFIXED(6);
      Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
      Event_End();
    }
  }
}

/// Cyclone cast standing on cell (29, 34) in the south-east weeds scatters
/// them and, until it is found, uncovers the second Antidote under Felix's
/// feet: he jumps a cell north off it, and it lies in the cleared grass.
void Izumo_ScatterWeedsOverSecondAntidote(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineState.leaderActorId);

  FieldTileCopies_CopyTouchedCells(53, 5, 8, 9, 23, 30, 0);
  if (!GameFlag_Get(FLAG_IZUMO_SECOND_ANTIDOTE)) {
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain,
          FIELD_TREASURE_ACTOR + 1, toFIXED(SECOND_ANTIDOTE_SPOT_X),
          toFIXED(SECOND_ANTIDOTE_SPOT_Z));
    if (FIELD_CELL(actor->x.fixed) ==
            FIELD_CELL(toFIXED(SECOND_ANTIDOTE_SPOT_X)) &&
        FIELD_CELL(actor->z.fixed) ==
            FIELD_CELL(toFIXED(SECOND_ANTIDOTE_SPOT_Z))) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldEffect_SetAttached(gMainRoutineState.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Call3((void (*)())Actor_SetDestinationOffsetById,
            gMainRoutineState.leaderActorId, 0, -16);
      actor->velocityY = toFIXED(6);
      Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
      Event_End();
    }
  }
}

/// Cyclone cast in the weeds around the shop south of the inn (on the riverbank
/// west of it at cells (7, 32) and (7, 33), and south and east of its ladder)
/// scatters those within two cells of Felix.
void Izumo_ScatterWeedsByTheShop(void) {
  FieldTileCopies_CopyTouchedCells(44, 5, 8, 5, 7, 32, 0);
}

/// Cyclone cast in the weeds on the bank where the river turns west, north-east
/// of the inn (cells (16, 24), (17, 24) and (19, 23)), scatters those within
/// two cells of Felix.
void Izumo_ScatterRiverbankWeeds(void) {
  FieldTileCopies_CopyTouchedCells(55, 0, 4, 2, 16, 23, 0);
}

/// Cyclone cast in the weeds along the edge of the raised ground north of the
/// inn scatters those within two cells of Felix. The ground stands 32 pixels
/// high, so the cells Felix casts from, (9, 20) to (14, 21), lie two rows south
/// of where the weeds show on screen.
void Izumo_ScatterLedgeWeeds(void) {
  FieldTileCopies_CopyTouchedCells(42, 0, 6, 2, 9, 18, 0);
}

/// Cyclone cast standing on cell (10, 20), in the weeds on the raised ground
/// north of the inn, scatters them and, until it is found, uncovers the Lucky
/// Medal under Felix's feet: he jumps a cell east off it, and it lies in the
/// cleared grass.
void Izumo_ScatterWeedsOverLuckyMedal(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineState.leaderActorId);

  FieldTileCopies_CopyTouchedCells(42, 0, 6, 2, 9, 18, 0);
  if (!GameFlag_Get(FLAG_IZUMO_LUCKY_MEDAL)) {
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain,
          FIELD_TREASURE_ACTOR + 2, toFIXED(LUCKY_MEDAL_SPOT_X),
          toFIXED(LUCKY_MEDAL_SPOT_Z));
    if (FIELD_CELL(actor->x.fixed) == FIELD_CELL(toFIXED(LUCKY_MEDAL_SPOT_X)) &&
        FIELD_CELL(actor->z.fixed) == FIELD_CELL(toFIXED(LUCKY_MEDAL_SPOT_Z))) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldEffect_SetAttached(gMainRoutineState.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffsetById(gMainRoutineState.leaderActorId, 16, 0);
      actor->velocityY = toFIXED(6);
      Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
      Event_End();
    }
  }
}

/// Cyclone cast in the weeds by the ladder of the Sanctum, on the raised ground
/// in the east (cells (24, 18) to (28, 21)), scatters those within two cells of
/// Felix.
void Izumo_ScatterWeedsByTheSanctum(void) {
  FieldTileCopies_CopyTouchedCells(49, 0, 5, 4, 24, 16, 0);
}

/// Cyclone cast in the weeds just west of the ring of giant stones (cells
/// (16, 11) and (20, 11)) scatters those within two cells of Felix.
void Izumo_ScatterStoneCircleWeeds(void) {
  FieldTileCopies_CopyTouchedCells(36, 0, 5, 3, 16, 9, 0);
}

/// Felix climbs the ladder of one of the village's houses on stilts to its
/// door and goes in: the inn, the shop, the Sanctum, Kushinada's house, Susa's
/// house or one of the two houses in the east. The door's cell names the
/// exit, and so the room.
void Izumo_EnterDoor(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeedParameters(gMainRoutineState.leaderActorId, toFIXED(1) / 2,
                           toFIXED(1) / 4);
  Actor_SetSpritePriority(gMainRoutineState.leaderActorId, 1);
  Actor_GetById(gMainRoutineState.leaderActorId)->motionFlags = 0;
  Sound_Play(SE_MAP_EXIT);
  Izumo_SetExitRequest(state->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  if (GameFlag_Get(FLAG_SERPENT_SLAIN))
    return gIzumo_TriggersOnceTheSerpentIsSlain;
  return gIzumo_Triggers;
}

/// The woman guarding the ring of giant stones, at the east end of its bridge,
/// asks whether the party sees that one stone is out of line: yes brings
/// "Susa moved it... He didn't even use his hands.", no "You're...not terribly
/// observant, are you?". Once the stone is back she only marvels at a power
/// that moves stones without touching them.
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

/// The ring's guard during the festival asks the same question about the
/// out-of-line stone, or once it is back complains "But it's a festival! Why do
/// I have to keep guarding the stone circle?"
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

/// An old woman dancing in the square at the festival asks whether it
/// surprises the party that Lady Uzume and the others aren't blood relatives:
/// yes brings that their power may make them siblings after all, no that she is
/// glad Lady Uzume used her powers for the good of all.
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

/// The river shimmers: every fourth frame the shifting colours of each of its
/// palettes move along by one.
void Izumo_RotateWaterColors(void) {
  s16 saved;
  s32 index;

  if ((gFrameCount & 3) == 0) {
    saved = BG_PLTT_COLOR(FIRST_WATER_PALETTE, WATER_LAST_COLOR);
    for (index = WATER_LAST_COLOR; index != WATER_FIRST_COLOR; index--)
      BG_PLTT_COLOR(FIRST_WATER_PALETTE, index) =
          BG_PLTT_COLOR(FIRST_WATER_PALETTE, index - 1);
    BG_PLTT_COLOR(FIRST_WATER_PALETTE, WATER_FIRST_COLOR) = saved;

    saved = BG_PLTT_COLOR(FIRST_WATER_PALETTE + 1, WATER_LAST_COLOR);
    for (index = WATER_LAST_COLOR; index != WATER_FIRST_COLOR; index--)
      BG_PLTT_COLOR(FIRST_WATER_PALETTE + 1, index) =
          BG_PLTT_COLOR(FIRST_WATER_PALETTE + 1, index - 1);
    BG_PLTT_COLOR(FIRST_WATER_PALETTE + 1, WATER_FIRST_COLOR) = saved;

    saved = BG_PLTT_COLOR(FIRST_WATER_PALETTE + 2, WATER_LAST_COLOR);
    for (index = WATER_LAST_COLOR; index != WATER_FIRST_COLOR; index--)
      BG_PLTT_COLOR(FIRST_WATER_PALETTE + 2, index) =
          BG_PLTT_COLOR(FIRST_WATER_PALETTE + 2, index - 1);
    BG_PLTT_COLOR(FIRST_WATER_PALETTE + 2, WATER_FIRST_COLOR) = saved;
  }
}

/// As the party comes down from Susa's house after the heroes' welcome, Susa
/// comes out of the door after it ("Felix, wait!"): he could not let it leave
/// without thanks, and asks whether warriors need weapons. Yes, and he tells
/// the legend of the Cloud Brand. No, and he goes back up to his doorway and
/// waits there, blocking it.
void Izumo_StopThePartyToThankThem(void) {
  GameFlag_Set(FLAG_STOPPED_TO_THANK_THE_PARTY);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgFelixWait);
  Actor_WalkToById(gMainRoutineState.leaderActorId, 200, 264);
  Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
  // "Felix, wait!"
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  FieldEvent_Wait(15);
  Actor_ShowEmoteById(gMainRoutineState.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, ACTOR_SUSA,
        toFIXED(200), toFIXED(216));
  Actor_WalkToById(ACTOR_SUSA, 200, 248);
  Actor_WaitForMoveById(ACTOR_SUSA);
  FieldEvent_Wait(30);
  // "It troubled me to let you leave like this..."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_FaceActorById(gMainRoutineState.leaderActorId, ACTOR_SUSA, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationById(ACTOR_SUSA, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(30);
  // "I owe you much for your help, but I didn't have any way to thank you..."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_ShowEmoteById(gMainRoutineState.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "And then I thought of something."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_SetAnimationById(ACTOR_SUSA, ANIM_NOD);
  FieldEvent_Wait(15);
  // "You're a warrior, and warriors need weapons. Wouldn't you agree?"
  FieldMessage_ShowAtActor(ACTOR_SUSA, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
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
    Actor_GetById(ACTOR_SUSA)->motionFlags |= ACTOR_FALLS;
    GameFlag_Set(FLAG_SUSA_WAITS);
  } else {
    Izumo_TellTheCloudBrandLegend();
  }
  Event_End();
}

/// Susa tells the legend of the Cloud Brand, hidden where the Serpent's tail
/// fell, says he left something for the party at the dragon, and goes back into
/// his house to Kushinada.
void Izumo_TellTheCloudBrandLegend(void) {
  Event_SetMessageId(MsgThisTokenIsn);
  Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "This token isn't something I possess. It's a legend, one that might..."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_FaceDirectionById(ACTOR_SUSA, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  // "Long ago, the Great Serpent of Mikage would sleep with its body..."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_FaceActorById(ACTOR_SUSA, gMainRoutineState.leaderActorId, 0);
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
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_NOD);
  FieldEvent_Wait(15);
  Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Well, I've said what I came to say. I'm going back to Kushinada."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_FaceActorById(ACTOR_SUSA, gMainRoutineState.leaderActorId, 0);
  FieldEvent_Wait(15);
  // "I hope you are successful in your quest."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_SetAnimationById(ACTOR_SUSA, ANIM_NOD);
  FieldEvent_Wait(15);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetSpeedParameters(ACTOR_SUSA, toFIXED(1) / 5, toFIXED(1) / 10);
  Actor_GetById(ACTOR_SUSA)->motionFlags = 0;
  Actor_SetSpritePriority(ACTOR_SUSA, 3);
  Actor_WalkToById(ACTOR_SUSA, 200, 208);
  Actor_WaitForMoveById(ACTOR_SUSA);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_SUSA, 0, 0);
  GameFlag_Set(FLAG_TOKEN_OFFERED);
  GameFlag_Clear(FLAG_SUSA_WAITS);
}

/// Talking to Susa in his doorway, or reading his mind: he offers his token of
/// gratitude again ("Would you be willing to accept a token of my
/// gratitude?"), and yes brings the legend of the Cloud Brand. Once it is told
/// he says the legend calls it a powerful weapon for fighting dragons.
void Izumo_OfferATokenOfGratitude(void) {
  if (!GameFlag_Get(FLAG_TOKEN_OFFERED)) {
    const u8 *message;

    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    message = MsgWouldYouBe;
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(ACTOR_SUSA, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
        0) {
      Izumo_TellTheCloudBrandLegend();
    } else {
      Event_SetMessageId(message - 1);
      Event_ShowMessageAndWait(ACTOR_SUSA, 0);
    }
    Event_End();
  } else {
    // XXX: the message is passed where the speaker goes; recording a talk with
    // Susa once FLAG_TOKEN_OFFERED is set would show what the player sees.
    Event_ShowMessageAndWait((s32)MsgTheLegendSays, 0);
  }
}

/// Sets up Izumo as the party enters: the fade in, the river's shimmer, the
/// ring's stones, and once the Serpent is slain the dancers, Susa's thanks the
/// first time the party leaves his house as heroes, and Susa waiting in his
/// doorway while his thanks are refused.
s32 Scene_Initialize(void) {
  GameFlag_Clear(FLAG_SUSA_WAITS);
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Task_Add(Izumo_RotateWaterColors, TASK_PASS_VBLANK);
  Actor_PlaceBlocks(gIzumo_StonesToMove);
  if (GameFlag_Get(FLAG_SERPENT_SLAIN)) {
    Actor_GetById(ACTOR_OKUNI)->steeringFlags = 2;
    Actor_SetAnimationById(ACTOR_DANCER_A, 5);
    Actor_SetAnimationById(ACTOR_DANCER_B, 5);
    Actor_SetAnimationById(ACTOR_DANCER_C, 5);
    Actor_SetAnimationById(ACTOR_DANCER_D, 5);
    Actor_SetAnimationById(ACTOR_DANCER_E, 6);
    Actor_SetAnimationById(ACTOR_DANCER_F, 6);
    Actor_SetAnimationById(ACTOR_DANCER_G, 5);
    if (gMainRoutineState.entrance == IZUMO_FROM_IZUMO_ROOMS_NORTH &&
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
      Actor_GetById(ACTOR_SUSA)->motionFlags |= ACTOR_FALLS;
      GameFlag_Set(FLAG_SUSA_WAITS);
    }
  }
  Actor_SetSpritePriority(ACTOR_PORCH_WOMAN, 3);
  Actor_GetById(ACTOR_PORCH_WOMAN)->motionFlags |= ACTOR_FALLS;
  return 0;
}

/// Nothing to prepare before the map is built. Returns 0.
s32 Scene_PrepareMap(void) { return 0; }

/// Reveal shows a way down in the centre of the ring of giant stones, cell
/// (23, 9). It can be climbed down only while the out-of-line stone is back in
/// place; a leader standing on the centre then is set on the cell north of it.
void Izumo_RevealTheStoneCircle(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);

  GameFlag_Set(FLAG_REVEALING);
  if (GameFlag_Get(FLAG_STONE_IN_PLACE)) {
    MapGrid_CopyCellPictureIndicesRect(36, 12, 22, 8, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 36, 12, 3, 3, 22, 8);
    if (x == CIRCLE_CENTER_CELL_X && z == CIRCLE_CENTER_CELL_Z)
      Actor_SetHorizontalPositionWithTerrain(gMainRoutineState.leaderActorId,
                                             toFIXED(376), toFIXED(136));
  } else {
    MapGrid_CopyCellPictureIndicesRect(36, 8, 22, 8, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 36, 8, 3, 3, 22, 8);
  }
}

/// As Reveal fades the way down in the ring's centre is hidden again.
void Izumo_HideTheStoneCircle(void) {
  GameFlag_Clear(FLAG_REVEALING);
  MapGrid_CopyCellPictureIndicesRect(40, 8, 22, 8, 3, 3);
  Call6((void (*)())Map_CopyCellAttributeRect, 40, 8, 3, 3, 22, 8);
}

/// Felix, facing north, backs into the opening in the ring's centre and climbs
/// down it as down a ladder, and the scene leaves for the Izumo Ruins.
void Izumo_DescendIntoTheRuins(void) {
  s32 value = gFieldState->stepTrigger;

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
  Actor_SetAnimationById(ACTOR_FELIX, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
  FieldEvent_Wait(14);
  Izumo_SetExitRequest(value);
}

/// Felix walks into the opening in the ring from its west side, cell (22, 9):
/// he steps round to its north side and climbs down into the ruins.
void Izumo_EnterTheRuinsFromTheLeft(void) {
  Call3((void (*)())Actor_WalkByAndWaitById, ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWaitById(ACTOR_FELIX, 16, 0);
  FieldEvent_Wait(10);
  Izumo_DescendIntoTheRuins();
}

/// Felix walks into the opening in the ring from its east side, cell (24, 9):
/// he steps round to its north side and climbs down into the ruins.
void Izumo_EnterTheRuinsFromTheRight(void) {
  Call3((void (*)())Actor_WalkByAndWaitById, ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWaitById(ACTOR_FELIX, -16, 0);
  FieldEvent_Wait(10);
  Izumo_DescendIntoTheRuins();
}

/// Felix walks south into the opening in the ring from its north side, cell
/// (23, 8), turns round and climbs down into the ruins.
void Izumo_EnterTheRuins(void) {
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Izumo_DescendIntoTheRuins();
}

#include "text/msg_ids.h"

/// The stones Move can push, ended by -1: the one out of line in the ring.
s16 gIzumo_StonesToMove[] = {
    ACTOR_MISALIGNED_STONE,
    -1,
};

/// The festival dancers' scripts: each turns once round on the spot, a quarter
/// turn every 5 frames, then stands a second and a half facing as it began,
/// over and over. They differ only in the way each starts out facing.
// clang-format off
u32 gIzumo_FirstDanceScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
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

u32 gIzumo_SecondDanceScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
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

u32 gIzumo_ThirdDanceScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
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

// The same script as gIzumo_FirstDanceScript, under another name.
u32 gIzumo_FourthDanceScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
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

// The same script as gIzumo_SecondDanceScript, under another name.
u32 gIzumo_FifthDanceScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
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

u32 gIzumo_SixthDanceScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
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

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(Izumo),
    SCENE_EXIT_UNLESS(IZUMO_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_IZUMO,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_SOUTHEAST_A, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_SOUTHEAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_SOUTHEAST_B, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_SOUTHEAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_SOUTHWEST, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_SOUTH, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_NORTHWEST, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_NORTHEAST, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_NORTH, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_RUINS_FIRST_ROOM, IzumoRuinsFirstRoom,
                      IZUMO_RUINS_FIRST_ROOM_FROM_IZUMO, EXIT_FLAG_NONE),
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
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (s32)gIzumo_FirstDanceScript, 256, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (s32)gIzumo_SecondDanceScript, 264, 0, 504, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (s32)gIzumo_ThirdDanceScript, 360, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 472, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, (s32)gIzumo_FourthDanceScript, 232, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, (s32)gIzumo_FifthDanceScript, 328, 0, 504, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_WOMAN, CONDITION_ALWAYS, (s32)gIzumo_SixthDanceScript, 336, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUSA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gIzumo_Triggers[] = {
    {TRIGGER_EXIT, IZUMO_TO_WORLD_MAP, CONDITION_ALWAYS, IZUMO_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTHEAST_A,
     CONDITION_ALWAYS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTHEAST_B,
     CONDITION_ALWAYS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTHWEST,
     CONDITION_ALWAYS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTH,
     CONDITION_ALWAYS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_NORTHWEST,
     CONDITION_ALWAYS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_NORTHEAST,
     CONDITION_ALWAYS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_NORTH,
     CONDITION_ALWAYS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), IZUMO_TO_IZUMO_RUINS_FIRST_ROOM,
     CONDITION_ALWAYS, EVENT(Izumo_EnterTheRuins)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), IZUMO_TO_IZUMO_RUINS_FIRST_ROOM,
     CONDITION_ALWAYS, EVENT(Izumo_EnterTheRuinsFromTheRight)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), IZUMO_TO_IZUMO_RUINS_FIRST_ROOM,
     CONDITION_ALWAYS, EVENT(Izumo_EnterTheRuinsFromTheLeft)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgWeVeHad)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgMyNameS)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLadyUzumeDoesn)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgSusaSGoing)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAsAChild)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgChoosingTheNext)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTheDaughtersOf)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgABraveWarrior)},
    {TRIGGER_TALK, ACTOR_STONE_CIRCLE_GUARD, CONDITION_ALWAYS,
     EVENT(Izumo_TalkToStoneCircleGuard)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STONE_CIRCLE_GUARD,
     CONDITION_ALWAYS, MSGNO(MsgWhyDoesLady)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgYouKnowI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgLadyUzumeSays)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Izumo_StartMovingStone)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Izumo_FinishMovingStone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_MISALIGNED_STONE, CONDITION_ALWAYS, EVENT(Izumo_StartMovingStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MISALIGNED_STONE, CONDITION_ALWAYS,
     EVENT(Izumo_FinishMovingStone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_SOUTHEAST_WEEDS, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterSoutheastWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_WEEDS_OVER_FIRST_ANTIDOTE, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterWeedsOverFirstAntidote)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_WEEDS_OVER_SECOND_ANTIDOTE, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterWeedsOverSecondAntidote)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_WEEDS_BY_THE_SHOP, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterWeedsByTheShop)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_RIVERBANK_WEEDS, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterRiverbankWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_LEDGE_WEEDS, CONDITION_ALWAYS, EVENT(Izumo_ScatterLedgeWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_WEEDS_OVER_LUCKY_MEDAL, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterWeedsOverLuckyMedal)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_WEEDS_BY_THE_SANCTUM, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterWeedsByTheSanctum)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_STONE_CIRCLE_WEEDS, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterStoneCircleWeeds)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Izumo_RevealTheStoneCircle)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Izumo_HideTheStoneCircle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIzumo_TriggersOnceTheSerpentIsSlain[] = {
    {TRIGGER_EXIT, IZUMO_TO_WORLD_MAP, CONDITION_ALWAYS, IZUMO_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTHEAST_A,
     CONDITION_ALWAYS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTHEAST_B,
     CONDITION_ALWAYS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTHWEST,
     CONDITION_ALWAYS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTH,
     CONDITION_ALWAYS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_NORTHWEST,
     CONDITION_ALWAYS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_NORTHEAST,
     CONDITION_ALWAYS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_NORTH,
     FLAG_SUSA_WAITS, EVENT(Izumo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), IZUMO_TO_IZUMO_RUINS_FIRST_ROOM,
     CONDITION_ALWAYS, EVENT(Izumo_EnterTheRuins)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), IZUMO_TO_IZUMO_RUINS_FIRST_ROOM,
     CONDITION_ALWAYS, EVENT(Izumo_EnterTheRuinsFromTheRight)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), IZUMO_TO_IZUMO_RUINS_FIRST_ROOM,
     CONDITION_ALWAYS, EVENT(Izumo_EnterTheRuinsFromTheLeft)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgThoseScreamsFrom)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgWeRePlanning)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgSusaHasChanged)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKushinadaHasBeen)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgIVeHeardPeople)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgSusaHasGrown)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgSusaHasPut)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgLadyUzumePredicted)},
    {TRIGGER_TALK, ACTOR_STONE_CIRCLE_GUARD, CONDITION_ALWAYS,
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
    {TRIGGER_TALK, ACTOR_SUSA, CONDITION_ALWAYS,
     EVENT(Izumo_OfferATokenOfGratitude)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STONE_CIRCLE_GUARD,
     CONDITION_ALWAYS, MSGNO(MsgSusaDidA)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_SUSA | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
     EVENT(Izumo_OfferATokenOfGratitude)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_MISALIGNED_STONE, CONDITION_ALWAYS, EVENT(Izumo_StartMovingStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MISALIGNED_STONE, CONDITION_ALWAYS,
     EVENT(Izumo_FinishMovingStone)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Izumo_StartMovingStone)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Izumo_FinishMovingStone)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Izumo_RevealTheStoneCircle)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Izumo_HideTheStoneCircle)},
#if TLA_PATCH_1 /* From patch 1. */
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_SOUTHEAST_WEEDS, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterSoutheastWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_WEEDS_OVER_FIRST_ANTIDOTE, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterWeedsOverFirstAntidote)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_WEEDS_OVER_SECOND_ANTIDOTE, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterWeedsOverSecondAntidote)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_WEEDS_BY_THE_SHOP, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterWeedsByTheShop)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_RIVERBANK_WEEDS, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterRiverbankWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_LEDGE_WEEDS, CONDITION_ALWAYS, EVENT(Izumo_ScatterLedgeWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_WEEDS_OVER_LUCKY_MEDAL, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterWeedsOverLuckyMedal)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_WEEDS_BY_THE_SANCTUM, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterWeedsByTheSanctum)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_STONE_CIRCLE_WEEDS, CONDITION_ALWAYS,
     EVENT(Izumo_ScatterStoneCircleWeeds)},
#endif
    {TRIGGER_TABLE_END},
};

/// Unused: a halfword nothing reads.
u16 gIzumo_Unused[] = {
    1,
};
