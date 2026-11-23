/// @file
/// The five small islets of the Eastern Sea, each a scene of its own in this
/// one overlay, and the animal trade that leads from one to the next, every
/// trade made by Mind Reading the animal.
///
/// It starts on E Tundaria Islet, in the snow south of Tundaria. Felix hops
/// across the drifting icebergs to an icy ledge, pushes the log there down into
/// the channel and pushes Pengulina, who hurt her leg on the far shore, back
/// over it to Pengus, who gives him a Pretty Stone
/// (Islets_PengusGivesPrettyStone). On SE Angara Islet he brings two crates
/// down off their ledges, freezes the puddle under the vine with Frost and
/// climbs to the bird's nest, and the bird trades its very fancy neckerchief,
/// the Red Cloth, for the stone (Islets_TradeWithBird). On N Osenia Islet he
/// uses Sand to reach the cow in the yard beside the house, ties the
/// neckerchief in the yard's tree and is given Milk (Islets_TradeWithCow). On
/// West Indra Islet the dog paddling among the piers trades the Li'l Turtle it
/// caught for the Milk (Islets_TradeWithDog). Last, on the Sea of Time Islet,
/// Lonesome George, the lonely turtle in the shallows below the cliffs, takes
/// the Li'l Turtle as a friend and swims the party to his secret spot, the
/// little islet that holds Islet Cave, and back whenever they ask
/// (Islets_GiveTurtle).
#include "system/task.h"
#include "scene/event_layer.h"
#include "flags.h"
#include "item_ids.h"

/// The trades along the islets, each set once done: the log pushed into the
/// water, the dog's, Pengus's Pretty Stone, the cow's, the bird's, and the Li'l
/// Turtle given to Lonesome George.
#define FLAG_LOG_PUSHED_INTO_WATER 0xa2c
#define FLAG_DOG_TRADED 0xaa1
#define FLAG_PENGUS_GAVE_PRETTY_STONE 0xaa2
#define FLAG_COW_TRADED 0xaa3
#define FLAG_BIRD_TRADED 0xaa4
#define FLAG_GEORGE_HAS_THE_TURTLE 0xaa5
/// For this visit, each islet's own: Pengus chasing Pengulina round the snow
/// since the Pretty Stone; the cow's trade or Lonesome George's turtle turned
/// down, so each asks again at once; each crate dropped from its ledge, and the
/// two stacked in one cell; the puddle frozen; the treasure hunters' slip; the
/// bird back on its nest; the dog's trade just made; the party at Lonesome
/// George's secret spot; and Reveal shown, holding the icebergs still.
#define FLAG_PENGUS_CHASING_PENGULINA FLAG_SCENE_TEMP_00
#define FLAG_RED_CLOTH_REFUSED FLAG_SCENE_TEMP_01
#define FLAG_TURTLE_REFUSED FLAG_SCENE_TEMP_01
#define FLAG_CRATE_A_DROPPED FLAG_SCENE_TEMP_10
#define FLAG_CRATE_B_DROPPED FLAG_SCENE_TEMP_11
#define FLAG_CRATES_IN_ONE_CELL FLAG_SCENE_TEMP_12
#define FLAG_SOUTHEAST_ANGARA_ISLET_PUDDLE_FROZEN FLAG_SCENE_TEMP_13
#define FLAG_TREASURE_HUNTERS_LET_SLIP FLAG_SCENE_TEMP_14
#define FLAG_BIRD_ON_NEST FLAG_SCENE_TEMP_15
#define FLAG_DOG_JUST_TRADED FLAG_SCENE_TEMP_16
#define FLAG_AT_SECRET_SPOT FLAG_SCENE_TEMP_17
#define FLAG_REVEAL_SHOWN FLAG_SCENE_TEMP_18
#include "battle/battle.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "macros.h"
#include "field/scene_tables.h"
#include "field/event_inline.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "field/call.h"
#include "version.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "system/heap_release.h"
#include "graphics/palette_snapshots.h"
#include "scene/scene.h"
#include "system/vector3.h"
#include "shared/tables.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "scene/mars_lighthouse_entrance.h"
#include "scene/shrine_of_the_sea_god.h"
#include "scene/ground_effects.h"

/// The cells Felix steps on or walks into that start this file's events, by
/// islet. On West Indra Islet, the east side of the sailor's boat, where he
/// jumps across to the pier.
enum { STEP_TRIGGER_JUMP_TO_PIER = 16 };
/// On E Tundaria Islet: the shore below the west ladder and the iceberg moored
/// beside it, the icy ledge's east end, the shore above the iceberg west of the
/// healer, the log on the ledge, and the spot behind Pengulina once she is home
/// with Pengus.
enum {
  STEP_TRIGGER_ICEBERG_BY_LADDER = 7,
  STEP_TRIGGER_SHORE_BY_LADDER = 8,
  STEP_TRIGGER_JUMP_OFF_LEDGE = 10,
  STEP_TRIGGER_JUMP_TO_ICEBERG = 11,
  STEP_TRIGGER_BY_PENGULINA = 20,
  STEP_TRIGGER_PUSH_LOG = 30
};
/// On SE Angara Islet: the cliff top just south of the bird's nest, and the
/// cell behind each ledge's edge, (12, 24) and (7, 27), from which walking
/// south pushes the crate standing at the edge off the ledge
/// (Islets_PushCrateOffLedge); a crate moved away from one of them leaves the
/// trigger there again.
enum { STEP_TRIGGER_NEAR_NEST = 20, STEP_TRIGGER_PUSH_CRATE_ON = 21 };
/// On N Osenia Islet, from patch 1, the six clumps of weeds Cyclone blows away
/// for good (Islets_ClearFirstPatch to Islets_ClearSixthPatch).
enum {
  WEEDS_IN_COW_YARD = 10,
  WEEDS_BY_HOUSE = 11,
  WEED_SOUTH_OF_YARD = 12,
  WEEDS_ON_TILLED_FIELD = 13,
  WEEDS_EAST_OF_FIELD = 14,
  WEED_BY_FIELD_FENCE = 15
};

/// E Tundaria Islet's raised trigger: Felix has fallen into the sea among the
/// icebergs (Islets_SinkInSea), and the exit that starts the islet again where
/// the party lands from the World Map.
enum { TRIGGER_SINK_IN_SEA = 200, EXIT_AFTER_SINKING = 3 };

/// The Sea of Time Islet's ride on Lonesome George: the exits to and from his
/// secret spot, and the entrances they arrive by.
enum {
  EXIT_TO_SECRET_SPOT = 3,
  EXIT_BACK_FROM_SECRET_SPOT = 4,
  ENTRANCE_BACK_FROM_SECRET_SPOT = 3,
  ENTRANCE_AT_SECRET_SPOT = 4
};

struct Actor;

void Islets_PushLogIntoWater(void);

void Islets_TalkToPengus(void);
void Islets_MindReadPengus(void);
void Islets_MindReadPengulina(void);
void Islets_TalkToTreasureHunters(void);

void Islets_TradeWithBird(void);

void Islets_GiveTurtle(void);
void Islets_RideBackToIsland(void);

void Islets_TalkToSailorsSon(void);

void Islets_OpenFieldMenu(void);
void Islets_JumpDownLedge(void);
void Islets_SinkIntoSand(void);
void Islets_RiseFromSand(void);
void Islets_StartPushingCrate(void);
void Islets_FreezePuddle(void);
void Islets_StepUpOntoShore(void);

void Islets_StepDownOntoIceberg(void);

void Islets_StartReveal(void);
void Islets_EndReveal(void);

void Islets_ClearFirstPatch(void);
void Islets_ClearSecondPatch(void);
void Islets_ClearFourthPatch(void);
void Islets_ClearSixthPatch(void);

void Islets_OpenDoorAndEnter(void);
void Islets_LeaveByStep(void);

void Islets_SinkInSea(void);
void Islets_JumpForward(void);

extern u32 gIslets_EastTundariaWalkScript[];
extern u32 gIslets_BirdCirclesScript[];
extern u32 gIslets_PacingSonScript[];
extern u16 gIslets_DoorFrames[];
extern u16 gIslets_NoDoorFrames[];

void Field_TryJumpForward();

void PushedActor_FillGridAttributeRectangle(s32 layer, s32 x, s32 z, u32 width,
                                            u32 height, s32 value);
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *number, s32 *kind);

void Islets_SetDeckBlocksByHeight(void);

struct Actor;

/// An actor script copied whole into the event that runs it
/// (gIslets_SpinScript).
struct ActionScript {
  u32 words[9];
};

void Actor_Release(struct Actor *actor);
void FieldPsynergy_GreyMindReadPalette(void);
void FieldPsynergy_RestoreMindReadPalette(void);
void FieldCamera_WaitForArrival(void);
void TileCopies_CopyTouchedCells();

s32 PushTarget_FillFootprint(s32 id);

void Actor_DisableById(s32 actorId);

/// The islets' people and animals, islet by islet, numbered by the rows of
/// each islet's placements.
/// West Indra Islet: the sailor on his boat, his two sons, one pacing the
/// pier, and the dog that trades.
enum {
  ACTOR_SAILOR = 8,
  ACTOR_SAILORS_SON = 9,
  ACTOR_SAILORS_OTHER_SON = 10,
  ACTOR_DOG = 11
};
/// E Tundaria Islet: the man in the green vest who walks by the hut, Pengus
/// and Pengulina, the log pushed into the channel, the drifting icebergs and
/// the healer.
enum {
  ACTOR_MAN_IN_GREEN_VEST = 8,
  ACTOR_PENGUS = 9,
  ACTOR_LOG = 10,
  /// The eight icebergs that drift across the water (Islets_DriftIcebergs).
  ACTOR_FIRST_ICEBERG = 11,
  ACTOR_LAST_ICEBERG = 18,
  ACTOR_PENGULINA = 19,
  ACTOR_PRETTY_STONE_ICON = 20, ///< shows the Pretty Stone over the leader
  ACTOR_HEALER = 21
};
/// N Osenia Islet: the young man who dreams of crossing the Sea of Time and
/// his father, the cow, and a blue orb drawn hidden.
enum {
  ACTOR_SON_ON_THE_SEA_OF_FOG = 8,
  ACTOR_FOG_DREAMER = 9,
  ACTOR_COW = 10,
  ACTOR_BLUE_ORB = 11
};
/// SE Angara Islet: the two treasure hunters, the bird that trades, the two
/// crates pushed off their ledges, and the puddle.
enum {
  ACTOR_TREASURE_HUNTER = 8,
  ACTOR_TREASURE_HUNTERS_PARTNER = 9,
  ACTOR_BIRD = 10,
  /// An unseen marker the setup parks at the map's corner (0, 0).
  ACTOR_UNSEEN_MARKER = 11,
  ACTOR_CRATE_A = 12,
  ACTOR_CRATE_B = 13,
  ACTOR_PUDDLE ///< frozen into an ice pillar by Frost (Islets_FreezePuddle)
};
/// The Sea of Time Islet: the old man in the palm forest, Lonesome George, who
/// carries the party to his secret spot, the Li'l Turtle the party gives him,
/// and two unseen blocks on the hut's deck.
enum {
  ACTOR_OLD_MAN = 8,
  ACTOR_LONESOME_GEORGE = 9,
  ACTOR_LIL_TURTLE,
  ACTOR_DECK_BLOCK_A,
  ACTOR_DECK_BLOCK_B
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "text/msg_ids.h"
#include "types.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/map_grid.h"
#include "field/scene_tables.h"
#include "system/input.h"
#include "text/msg_ids.h"
#include "system/frame.h"
#include "system/vector3.h"

void Actor_StartScript(struct Actor *actor, const void *script);
void PaletteCycle_Stop(void);

void Islets_StartWaveDma(void);
void Islets_FillWaveLines(void);

extern u32 gIslets_WadingSplashScript[];
extern struct Trigger gIslets_Triggers[];
extern struct Trigger gIslets_WestIndraTriggers[];
extern struct Trigger gIslets_EastTundariaTriggers[];
extern struct Trigger gIslets_NorthOseniaTriggers[];
extern struct Trigger gIslets_SoutheastAngaraTriggers[];
extern struct Trigger gIslets_SeaOfTimeTriggers[];
extern u16 gSceneState[];

void Islets_InitializeWestIndra(void);
void Islets_InitializeEastTundaria(void);
void Islets_InitializeNorthOsenia(void);
void Islets_InitializeSoutheastAngara(void);
void Islets_InitializeSeaOfTime(void);

/// A door that swings open as Felix walks into it (Islets_OpenDoorAndEnter):
/// its cell animation and the cell it plays at.
struct IsletsDoor {
  const u16 *frames;
  u16 x;
  u16 z;
};

/// Where a pushed object comes to rest, read by the push that follows.
struct PushTarget {
  FIXED x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void *callback;
};

extern struct ActorPlacement gIslets_Placements[];
extern struct ActorPlacement gIslets_WestIndraActorPlacements[];
extern struct ActorPlacement gIslets_EastTundariaActorPlacements[];
extern struct ActorPlacement gIslets_NorthOseniaActorPlacements[];
extern struct ActorPlacement gIslets_SoutheastAngaraActorPlacements[];
extern struct ActorPlacement gIslets_SeaOfTimeActorPlacements[];
extern s32 gIslets_EnterDoorScript[];
extern struct IsletsDoor gIslets_Doors[];
extern u16 gPushKinds[];

void Islets_SetCellCollision(s32 x, s32 z, s32 layers, s32 value);
void Islets_FinishPushingCrate(void);
void Islets_SetSonsTargetedByHeight(void);
void Leader_PushObjectAhead(void);
s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *position);
struct Actor *Actor_FindAtCell(struct Vector3 *position, s32 mode);
s32 PushTarget_FindSlide(struct PushTarget *target);
void PushTarget_Push(struct PushTarget target);

void Islets_PlayFootsteps(void);
void Islets_RideOnLonesomeGeorge(struct Actor *actor);
void Actor_EmitWadingSplashes(struct Actor *actor);
void Actor_PlaceOnActor(s32 actorId, s32 target);
void FieldPsynergy_StopMindReadWave(void);

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_WEST_INDRA_ISLET)
    return gIslets_WestIndraActorPlacements;
  if (scene == SCENE_EAST_TUNDARIA_ISLET)
    return gIslets_EastTundariaActorPlacements;
  if (scene == SCENE_NORTH_OSENIA_ISLET)
    return gIslets_NorthOseniaActorPlacements;
  if (scene == SCENE_SOUTHEAST_ANGARA_ISLET)
    return gIslets_SoutheastAngaraActorPlacements;
  if (scene == SCENE_SEA_OF_TIME_ISLET) {
    if (Event_GetFlag(FLAG_GEORGE_HAS_THE_TURTLE))
      gIslets_SeaOfTimeActorPlacements[1].talkFacing =
          TALK_FACING_PARTY_AND_BACK;
    return gIslets_SeaOfTimeActorPlacements;
  }
  return gIslets_Placements;
}

/// Walking south against the log on E Tundaria Islet's icy ledge, from cell
/// (11, 7) or (12, 7), Felix pushes it over the edge, and it falls with a thud
/// into the channel between Pengulina's shore and Pengus's, where it floats as
/// a bridge across. The cells along it are made walkable, copied from a row the
/// map keeps for the purpose, and FLAG_LOG_PUSHED_INTO_WATER keeps it there.
void Islets_PushLogIntoWater(void) {
  struct PushTarget target;
  struct Actor *actor = Actor_Get(ACTOR_LOG);

  if (PushTarget_FindSlide(&target)) {
    PushTarget_Push(target);
    System_Wait(5);
    actor->gravity = toFIXED(1) / 2;
    Actor_SetAnimation(ACTOR_LOG, 3);
    Actor_SetDestinationOffset(ACTOR_LOG, 0, 8);
    FieldEvent_Wait(22);
    Event_PlaySound(SE_LOG_FALL);
    Actor_SetAnimation(ACTOR_LOG, 8);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->motionFlags = 0;
    actor->velocityY = 0;
    actor->y.fixed = toFIXED(-8);
    actor->groundHeight = toFIXED(-8);
    MapGrid_CopyCellAttributes(9, 1, 4, 1, 9, 10);
    MapGrid_CopyCellAttributes(9, 8, 4, 1, 9, 9);
    MapGrid_CopyCellAttributes(9, 64, 4, 1, 9, 74);
    Event_SetFlag(FLAG_LOG_PUSHED_INTO_WATER);
  }
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_WEST_INDRA_ISLET)
    return gIslets_WestIndraTriggers;
  if (scene == SCENE_EAST_TUNDARIA_ISLET)
    return gIslets_EastTundariaTriggers;
  if (scene == SCENE_NORTH_OSENIA_ISLET)
    return gIslets_NorthOseniaTriggers;
  if (scene == SCENE_SOUTHEAST_ANGARA_ISLET)
    return gIslets_SoutheastAngaraTriggers;
  if (scene == SCENE_SEA_OF_TIME_ISLET)
    return gIslets_SeaOfTimeTriggers;
  return gIslets_Triggers;
}

#include "text/msg_ids.h"

/// Talking to Pengus, the penguin on E Tundaria Islet's east shore: until
/// Pengulina is home he stops, cries "Pen pen!" and goes back to gazing across
/// the water at her; after, he answers "Pen pen.", or "Pen pen pen..." while
/// he chases her round the snow.
void Islets_TalkToPengus(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
    if (Event_GetFlag(FLAG_PENGUS_CHASING_PENGULINA)) {
      Actor_FaceActor(ACTOR_PENGUS, gMainRoutineWork.leaderActorId, 0);
      FieldEvent_SetMessageId((s32)MsgPenPenPen);
    } else {
      FieldEvent_SetMessageId((s32)MsgPenPen);
    }
    Event_ShowMessage(ACTOR_PENGUS, 0);
  } else {
    FieldActor_Stop(ACTOR_PENGUS);
    System_Wait(1);
    Actor_FaceActor(ACTOR_PENGUS, gMainRoutineWork.leaderActorId, 0);
    FieldEvent_SetMessageId((s32)MsgPenPenExclaimed);
    // "Pen pen!"
    Event_ShowMessage(ACTOR_PENGUS, 0);
    FieldActor_WatchAndSetScript(ACTOR_PENGUS, ACTOR_PENGULINA);
  }
  Event_End();
  return;
}

/// Mind Read on Pengus: "My darling Pengulina hurt her leg on the far shore and
/// can't swim back." Once she is home he is happy she is all right, or, while
/// he chases her round the snow, says nothing beats playing there with her.
void Islets_MindReadPengus(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
    if (Event_GetFlag(FLAG_PENGUS_CHASING_PENGULINA)) {
      FieldEvent_SetMessageId((s32)MsgThereIsNothing);
    } else {
      FieldEvent_SetMessageId((s32)MsgIMSoHappy);
    }
    Event_ShowMessage(ACTOR_PENGUS, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgMyDarlingPengulina);
    // "My darling Pengulina hurt her leg on the far shore and can't swim..."
    Event_ShowMessage(ACTOR_PENGUS, 0);
  }
  Event_End();
  return;
}

/// Talking to Pengulina, the penguin with the bow: on the far shore she cries
/// "Pen pen pehnnn!"; once home, "Pen pen.", or "Pen pen pehnnnn." while
/// Pengus chases her.
void Islets_TalkToPengulina(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
    if (Event_GetFlag(FLAG_PENGUS_CHASING_PENGULINA)) {
      Actor_FaceActor(ACTOR_PENGULINA, gMainRoutineWork.leaderActorId, 0);
      FieldEvent_SetMessageId((s32)MsgPenPenPehnnnn);
    } else {
      FieldEvent_SetMessageId((s32)MsgPenPen);
    }
    Event_ShowMessage(ACTOR_PENGULINA, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgPenPenPehnnn);
    // "Pen pen pehnnn!"
    Event_ShowMessage(ACTOR_PENGULINA, 0);
  }
  Event_End();
  return;
}

/// Mind Read on Pengulina: "Ohh... My leg... I can't get back to my dearest
/// Pengus..." Once home she is glad to be back with him, or, while he chases
/// her, grumbles that he smothers her.
void Islets_MindReadPengulina(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
    if (Event_GetFlag(FLAG_PENGUS_CHASING_PENGULINA)) {
      FieldEvent_SetMessageId((s32)MsgPengusIsPretty);
    } else {
      FieldEvent_SetMessageId((s32)MsgIMSoGlad);
    }
    Event_ShowMessage(ACTOR_PENGULINA, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgOhhMyLeg);
    // "Ohh... My leg... I can't get back to my dearest Pengus..."
    Event_ShowMessage(ACTOR_PENGULINA, 0);
  }
  return;
}

#include "shared/leader_is_facing_north.h"

/// The healer on E Tundaria Islet: spoken to from in front, with Felix facing
/// north, she offers a sanctum's healing; from any other side she says she is
/// there to tend to travelers.
void Islets_TalkToHealer(void) {

  if (Leader_IsFacingNorth()) {
    Sanctum_Run(ACTOR_HEALER);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgIAmHere);
    // "I am here to tend to any travelers who come to this place."
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_End();
  }
  return;
}

/// The treasure hunter by the tent on SE Angara Islet blurts out that they are
/// not looking for Treasure Isle. The first time, his partner turns on him,
/// "What are you saying!? Idiot!", then hops and swears he said nothing about
/// it; after that he is too busy to talk.
void Islets_TalkToTreasureHunters(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (!Event_GetFlag(FLAG_TREASURE_HUNTERS_LET_SLIP)) {
    FieldEvent_SetMessageId((s32)MsgWeLikeThis);
    // "We like this island. A lot... That's why we're here... Because we like
    // it." "We sure aren't looking for Treasure Isle! Nope!"
    Event_ShowMessage(ACTOR_TREASURE_HUNTER, 0);
    Actor_SetAttachedEffect(ACTOR_TREASURE_HUNTERS_PARTNER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_FaceDirection(ACTOR_TREASURE_HUNTERS_PARTNER, FACING_WEST, 0);
    // "What are you saying!? Idiot!"
    Event_ShowMessage(ACTOR_TREASURE_HUNTERS_PARTNER, 0);
    Actor_FaceDirectionAndWait(ACTOR_TREASURE_HUNTER, FACING_EAST);
    FieldActor_Jump(ACTOR_TREASURE_HUNTERS_PARTNER, 4, 10);
    // "Don't listen to that guy. He didn't say anything about Treasure Isle.
    // Nothing at all."
    Event_ShowMessage(ACTOR_TREASURE_HUNTERS_PARTNER, 0);
    Actor_SetAttachedEffect(ACTOR_TREASURE_HUNTERS_PARTNER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldEvent_Wait(20);
    Event_SetFlag(FLAG_TREASURE_HUNTERS_LET_SLIP);
  }
  Actor_FaceActor(ACTOR_TREASURE_HUNTER, gMainRoutineWork.leaderActorId, 0);
  FieldEvent_SetMessageId((s32)MsgLookWeRe);
  // "Look, we're busy... We can't talk now... Catch us some other time..."
  Event_ShowMessage(ACTOR_TREASURE_HUNTER, 0);
  Actor_FaceDirection(ACTOR_TREASURE_HUNTERS_PARTNER, FACING_SOUTH_SOUTHWEST,
                      0);
  Event_End();
  return;
}

/// The treasure hunter's partner by the tent says how huge the Eastern Sea is,
/// and warns Felix there is no treasure in these waters.
void Islets_TalkToTreasureHuntersPartner(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgTheEasternSea);
  // "The Eastern Sea is huge, even if you don't add the Sea of Time..."
  Event_ShowMessage(ACTOR_TREASURE_HUNTERS_PARTNER, 0);
  // "You don't look like much of a sailor. You're not a treasure hunter..."
  Event_ShowMessage(ACTOR_TREASURE_HUNTERS_PARTNER, 0);
  Event_End();
  return;
}

/// Talking to the orange bird on SE Angara Islet: "Chirp chirp!" Once it has
/// traded, it turns to watch Felix.
void Islets_TalkToBird(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_BIRD_TRADED)) {
    FieldActor_WatchAndSetScript(ACTOR_BIRD, gMainRoutineWork.leaderActorId);
  }
  FieldEvent_SetMessageId((s32)MsgChirpChirp);
  // "Chirp chirp!"
  Event_ShowMessage(ACTOR_BIRD, 0);
  Event_End();
  return;
}

/// Mind Read on the bird on SE Angara Islet with the Pretty Stone in the party:
/// it hops, cries that it must have the stone, flutters down to Felix and back,
/// and trades its very fancy neckerchief, the Red Cloth, for it. Without the
/// stone it longs for something fancy; after the trade it asks whether it does
/// not look fancy.
void Islets_TradeWithBird(void) {
  struct Actor *leader;
  struct Actor *actor;
  struct Actor *spawned;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (FieldPartyInventory_FindMember(ITEM_PRETTY_STONE) == -1) {
    if (Event_GetFlag(FLAG_BIRD_TRADED)) {
      FieldActor_WatchAndSetScript(ACTOR_BIRD, gMainRoutineWork.leaderActorId);
      FieldEvent_SetMessageId((s32)MsgWhatDoYou);
    } else {
      FieldEvent_SetMessageId((s32)MsgILoveFeeling);
    }
    Event_ShowMessage(ACTOR_BIRD, 0);
  } else {
    leader = Actor_Get(gMainRoutineWork.leaderActorId);
    actor = Actor_Get(ACTOR_BIRD);
    Actor_FaceActor(ACTOR_BIRD, gMainRoutineWork.leaderActorId, 0);
    FieldActor_Jump(ACTOR_BIRD, 4, 10);
    FieldEvent_SetMessageId((s32)MsgOhMyThat);
    // "Oh my! That stone you have! It's divine! Simply divine! I must have..."
    Event_ShowMessage(ACTOR_BIRD, 0);
    Actor_SetAnimation(ACTOR_BIRD, 4);
    FieldActor_SetPositionAndResetMotion(
        actor, (leader->x.fixed + actor->x.fixed) / 2, leader->y.fixed,
        (leader->z.fixed + actor->z.fixed) / 2);
    System_Wait(20);
    FieldActor_SetPositionAndResetMotion(actor, toFIXED(120), toFIXED(128),
                                         toFIXED(312));
    Actor_SetAnimation(ACTOR_BIRD, 5);
    System_Wait(10);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
    spawned = FieldActor_Create(SPRITE_UNSEEN_MARKER, leader->x.fixed,
                                toFIXED(152), leader->z.fixed);
    if (spawned != 0) {
      Actor_SetAnimation(gMainRoutineWork.leaderActorId,
                         ANIM_LEADER_ARMS_RAISED);
      FieldActor_SetItemIcon(spawned, ITEM_RED_CLOTH);
      Actor_SetSpriteFlags(spawned, 0);
      spawned->motionFlags = 0;
    }
    FieldEvent_Wait(10);
    FieldPsynergy_RestoreMindReadPalette();
    // "Felix traded his stone for the very fancy neckerchief."
    Message_ShowCentered((s32)MsgOhMyThat + 1, 1);
    PartyInventory_Remove(ITEM_PRETTY_STONE);
    Party_GiveItem(ITEM_RED_CLOTH, 0);
    FieldPsynergy_GreyMindReadPalette();
    if (spawned != 0) {
      Actor_Release(spawned);
      Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
    }
    Actor_FaceDirectionAndWait(ACTOR_BIRD, FACING_EAST);
    Event_SetFlag(FLAG_BIRD_TRADED);
  }
  Event_End();
}

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->steeringFlags &= ~ACTOR_FACES_MOTION,                 \
   Actor_WalkToAndWait(actor, x, z), Event_Wait(1),                            \
   Actor_GetById(actor)->steeringFlags |= ACTOR_FACES_MOTION)

/// The cow walks round to Felix, he holds the Milk it gives him over his head,
/// and the neckerchief is traded for it.
#define HAND_BACK(leader, given, off)                                          \
  {                                                                            \
    Actor_SetSpeed(ACTOR_COW, 0.8);                                            \
    Actor_WalkToAndWait(ACTOR_COW, 128, 164);                                  \
    Actor_FaceDirectionAndRefreshById(ACTOR_COW, FACING_WEST);                 \
    Event_Wait(20);                                                            \
    Actor_SetSpeed(ACTOR_COW, 1.6);                                            \
    STEP(ACTOR_COW, 108, 168);                                                 \
    given = FieldActor_Create(SPRITE_UNSEEN_MARKER, leader->x.fixed,           \
                              leader->y.fixed + toFIXED(24), leader->z.fixed); \
    if (given != 0) {                                                          \
      Actor_SetAnimationById(gMainRoutineWork.leaderActorId,                   \
                             ANIM_LEADER_ARMS_RAISED);                         \
      given->motionFlags = off;                                                \
      Actor_SetItemIcon(given, ITEM_MILK);                                     \
      Event_Wait(20);                                                          \
    }                                                                          \
    STEP(ACTOR_COW, 116, 164);                                                 \
    FieldPsynergy_RestoreMindReadPalette();                                    \
    PartyInventory_Remove(ITEM_RED_CLOTH);                                     \
    PartyInventory_GiveItem(ITEM_MILK, 0);                                     \
    if (given != 0) {                                                          \
      Actor_Release(given);                                                    \
      Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_STAND);      \
    }                                                                          \
    FieldPsynergy_GreyMindReadPalette();                                       \
    GameFlag_Set(FLAG_COW_TRADED);                                             \
  }

/// Mind Read on the cow in the yard beside N Osenia Islet's house, with the Red
/// Cloth in the party: "Oh! That red neckerchief is quite nice indeed." Felix
/// ties it in the yard's tree, and when the cow asks "Are you giving that
/// lovely thing to moooooi?", a yes trades it for Milk; a no takes it down
/// again, and the next Mind Read asks at once. Without the cloth the cow feels
/// low and does not know why, and after the trade it praises its neckerchief.
void Islets_TradeWithCow(void) {
  struct Actor *item, *leader, *given;
  const u8 *asked, *noticed;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (FieldPartyInventory_FindMember(ITEM_RED_CLOTH) != -1) {
    if (Event_GetFlag(FLAG_RED_CLOTH_REFUSED)) {
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      FieldCamera_SetSpeed(0.8);
      FieldCamera_MoveTo(140, CAMERA_KEEP, 190, 1);
      Actor_FaceActorById(ACTOR_COW, gMainRoutineWork.leaderActorId, 0);
      asked = MsgAreYouGiving;
      Event_SetMessageId(asked);
      Event_ShowMessageAndWait(ACTOR_COW, 0);
      FieldCamera_WaitForArrival();
      FieldPsynergy_RestoreMindReadPalette();
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
          0) {
        leader = Actor_GetById(gMainRoutineWork.leaderActorId);
        Actor_WatchAndSetScript(ACTOR_COW, gMainRoutineWork.leaderActorId);
        Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
        Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 98, 168);
        Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                          FACING_NORTH);
        Actor_Jump(gMainRoutineWork.leaderActorId, 4, 10);
        item = FieldActor_Create(SPRITE_UNSEEN_MARKER, toFIXED(104),
                                 toFIXED(24), toFIXED(160));
        // Default: the object handed back is set for real in HAND_BACK.
        given = 0;
        if (item != 0) {
          Actor_SetItemIcon(item, ITEM_RED_CLOTH);
          Actor_SetSpriteFlags(item, 0);
          item->motionFlags = 0;
          item->retainSpriteCache = 1;
        }
        Event_Wait(10);
        FieldPsynergy_GreyMindReadPalette();
        Actor_Stop(ACTOR_COW);
        Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
        Actor_ShowEmoteById(ACTOR_COW, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
        Event_SetMessageId(asked + 1);
        Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_COW, 0);
        HAND_BACK(leader, given, 0)
      } else {
        FieldPsynergy_GreyMindReadPalette();
        Actor_ShowEmoteById(ACTOR_COW, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
        Event_SetMessageId(asked + 4);
        Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_COW, 0);
      }
    } else {
      noticed = MsgOhThatRed;
      Event_SetMessageId(noticed);
      Event_ShowMessageAndWait(ACTOR_COW, 0);
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      FieldCamera_SetSpeed(0.8);
      FieldCamera_MoveTo(140, CAMERA_KEEP, 190, 1);
      Actor_FaceDirection(ACTOR_COW, FACING_SOUTH_SOUTHWEST, 0);
      Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 120, 190);
      Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                        FACING_EAST);
      STEP(gMainRoutineWork.leaderActorId, 132, 190);
      item = FieldActor_Create(SPRITE_UNSEEN_MARKER, toFIXED(140), 0,
                               toFIXED(190));
      if (item != 0) {
        Actor_SetItemIcon(item, ITEM_RED_CLOTH);
        Actor_SetSpriteFlags(item, 0);
        item->motionFlags = 0;
        item->retainSpriteCache = 1;
      }
      STEP(gMainRoutineWork.leaderActorId, 120, 190);
      Actor_FaceDirectionById(ACTOR_COW, FACING_SOUTH_SOUTHEAST, 0);
      Actor_Jump(ACTOR_COW, 4, 10);
      Event_ShowMessageAndWait(ACTOR_COW, 0);
      Actor_WatchAndSetScript(ACTOR_COW, gMainRoutineWork.leaderActorId);
      STEP(gMainRoutineWork.leaderActorId, 132, 190);
      FieldActor_SetPositionAndResetMotion(item, toFIXED(195), 0, toFIXED(115));
      STEP(gMainRoutineWork.leaderActorId, 120, 190);
      Event_Wait(20);
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 98, 168);
      Actor_Jump(gMainRoutineWork.leaderActorId, 4, 10);
      Actor_SetPositionAndResetMotion(item, toFIXED(104), toFIXED(24),
                                      toFIXED(160));
      Event_Wait(10);
      FieldPsynergy_RestoreMindReadPalette();
      Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
      FieldMessage_Show(noticed + 2, 1);
      (gFieldWork->message)++;
      FieldPsynergy_GreyMindReadPalette();
      Actor_Jump(ACTOR_COW, 6, 10);
      Event_SetMessageId(noticed + 3);
      Event_ShowMessageAndWait(ACTOR_COW, 0);
      Actor_Stop(ACTOR_COW);
      Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                        FACING_EAST);
      FieldPsynergy_RestoreMindReadPalette();
      if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                                0) == 0) {
        item = Actor_GetById(gMainRoutineWork.leaderActorId);
        FieldPsynergy_GreyMindReadPalette();
        Actor_ShowEmoteById(ACTOR_COW, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
        Event_SetMessageId(noticed + 4);
        Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_COW, 0);
        HAND_BACK(item, item, 0)
      } else {
        FieldPsynergy_GreyMindReadPalette();
        Actor_ShowEmoteById(ACTOR_COW, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
        Event_SetMessageId(noticed + 7);
        Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_COW, 0);
        Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                          FACING_NORTH);
        Actor_Jump(gMainRoutineWork.leaderActorId, 4, 10);
        if (item != 0)
          Actor_Release(item);
        Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId,
                                   FACING_SOUTH);
        Event_SetFlag(FLAG_RED_CLOTH_REFUSED);
      }
    }
  } else {
    if (Event_GetFlag(FLAG_COW_TRADED))
      Event_SetMessageId(MsgYupANice);
    else
      Event_SetMessageId(MsgIVeBeenFeeling);
    // "I've been feeling so low lately... I long to feel the utter joy..."
    Event_ShowMessageAndWait(ACTOR_COW, 0);
  }
  Actor_SetAnimationById(ACTOR_COW, 4);
  Event_End();
}
#undef STEP

/// Felix holds up his arms with an item over his head. Returns the item's
/// actor, or null.
struct Actor *Islets_ShowItemAbove(struct Actor *actor, s32 value) {
  struct Actor *spawned;

  spawned = FieldActor_Create(SPRITE_UNSEEN_MARKER, actor->x.fixed,
                              actor->y.fixed + toFIXED(24), actor->z.fixed);
  if (spawned != 0) {
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
    spawned->motionFlags = 0;
    FieldActor_SetItemIcon(spawned, value);
    Actor_SetSpriteFlags(spawned, 0);
    spawned->retainSpriteCache = 1;
  }
  return spawned;
}

/// An actor's update: every fourth frame a splash where it is
/// (gIslets_WadingSplashScript), as it wades or swims through the shallows.
void Actor_EmitWadingSplashes(struct Actor *actor) {
  struct Actor *spawned;

  if ((gFrameCount & 3) == 0) {
    spawned = FieldActor_Create(SPRITE_WADING_SPLASH, actor->x.fixed,
                                actor->y.fixed, actor->z.fixed);
    if (spawned != 0) {
      Actor_SetSpriteFlags(spawned, 0);
      Actor_SelectAnimation(spawned, 1);
      Actor_StartScript(spawned, gIslets_WadingSplashScript);
    }
  }
}

extern s32 gIslets_DogHopsFacingEastScript[];
extern s32 gIslets_DogHopsFacingWestScript[];

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->steeringFlags &= ~ACTOR_FACES_MOTION,                 \
   Actor_WalkToAndWait(actor, x, z), FieldEvent_Wait(1),                       \
   Actor_GetById(actor)->steeringFlags |= ACTOR_FACES_MOTION)

/// Mind Read on the dog paddling among West Indra Islet's piers, with Milk in
/// the party: "Sniff! Sniff! I smell fresh milk!" It splashes round Felix, he
/// sets the Milk down in front of it, and it trades him the Li'l Turtle it
/// caught. Without Milk it complains the water is so salty; after the trade it
/// says it is taking little laps so as not to waste it.
void Islets_TradeWithDog(void) {
  struct Actor *leader, *trader, *item, *gift;
  s32 right;
  s32 giftItem;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (FieldPartyInventory_FindMember(ITEM_MILK) != -1) {
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    trader = Actor_GetById(ACTOR_DOG);
    // Default: the trader's zero update stores read it.
    gift = 0;
    right = 0;
    Actor_FaceActor(ACTOR_DOG, gMainRoutineWork.leaderActorId, 0);
    trader->update = Actor_EmitWadingSplashes;
    FieldActor_Jump(ACTOR_DOG, 4, 10);
    trader->update = 0;
    Event_SetMessageId(MsgSniffSniffI);
    // "Sniff! Sniff! I smell fresh milk!"
    Event_ShowMessage(ACTOR_DOG, 0);
    if (leader->x.fixed > trader->x.fixed)
      right = 1;
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
    Actor_SetSpeed(ACTOR_DOG, 1.6);
    if (right) {
      Actor_FaceDirection(ACTOR_DOG, FACING_EAST, 0);
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 296, 856);
      Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_WEST);
      item = Islets_ShowItemAbove(leader, ITEM_MILK);
      trader->update = Actor_EmitWadingSplashes;
      Actor_RunScriptAndWait(ACTOR_DOG, gIslets_DogHopsFacingEastScript);
      trader->update = 0;
    } else {
      Actor_FaceDirection(ACTOR_DOG, FACING_WEST, 0);
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 264, 856);
      Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
      item = Islets_ShowItemAbove(leader, ITEM_MILK);
      trader->update = Actor_EmitWadingSplashes;
      Actor_RunScriptAndWait(ACTOR_DOG, gIslets_DogHopsFacingWestScript);
      trader->update = 0;
    }
    // "Hah hah hah! Are you going to give me that milk? Are you? Huh? Huh!?..."
    Event_ShowMessage(ACTOR_DOG, 0);
    FieldActor_WatchAndSetScript(ACTOR_DOG, gMainRoutineWork.leaderActorId);
    if (item != 0) {
      void (*emitSmoke)(void);
      item->collisionFlags = ACTOR_NOT_TARGETED | ACTOR_SOLID;
      FieldActor_SetPositionAndResetMotion(item, toFIXED(104), 0, toFIXED(840));
      emitSmoke = Actor_EmitWadingSplashes;
      if (right) {
        leader->update = emitSmoke;
        Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 296, 840);
        leader->update = 0;
        Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_WEST);
        leader->update = emitSmoke;
        STEP(gMainRoutineWork.leaderActorId, 288, 840);
        FieldActor_SetPositionAndResetMotion(item, toFIXED(280), 0,
                                             toFIXED(840));
        STEP(gMainRoutineWork.leaderActorId, 296, 840);
        leader->update = 0;
        FieldActor_Stop(gMainRoutineWork.leaderActorId);
        System_Wait(1);
        trader->update = emitSmoke;
        STEP(ACTOR_DOG, 292, 844);
      } else {
        leader->update = emitSmoke;
        Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 264, 840);
        leader->update = 0;
        Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
        leader->update = emitSmoke;
        STEP(gMainRoutineWork.leaderActorId, 272, 840);
        FieldActor_SetPositionAndResetMotion(item, toFIXED(280), 0,
                                             toFIXED(840));
        STEP(gMainRoutineWork.leaderActorId, 264, 840);
        leader->update = 0;
        FieldActor_Stop(gMainRoutineWork.leaderActorId);
        System_Wait(1);
        trader->update = emitSmoke;
        STEP(ACTOR_DOG, 268, 844);
      }
    }
    giftItem = ITEM_LIL_TURTLE;
    gift = Islets_ShowItemAbove(leader, giftItem);
    STEP(ACTOR_DOG, 280, 856);
    trader->update = 0;
    FieldPsynergy_RestoreMindReadPalette();
    // "Felix traded his milk for the dog's turtle!"
    Message_ShowCentered((s32)MsgTradedHisMilk, 1);
    PartyInventory_Remove(ITEM_MILK);
    Party_GiveItem(giftItem, 0);
    FieldPsynergy_GreyMindReadPalette();
    Actor_GetById(gMainRoutineWork.leaderActorId)->facing = FACING_SOUTH;
    if (gift != 0) {
      Actor_Release(gift);
      Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
    }
    Actor_FaceDirectionAndWait(ACTOR_DOG, FACING_NORTH);
    Event_SetFlag(FLAG_DOG_TRADED);
    Event_SetFlag(FLAG_DOG_JUST_TRADED);
  } else {
    if (Event_GetFlag(FLAG_DOG_TRADED)) {
      FieldActor_WatchAndSetScript(ACTOR_DOG, gMainRoutineWork.leaderActorId);
      if (Event_GetFlag(FLAG_DOG_JUST_TRADED))
        Event_SetMessageId(MsgIDonTLike);
      else
        Event_SetMessageId(MsgIDonTWantTo);
    } else {
      Event_SetMessageId(MsgTheWaterHere);
    }
    Event_ShowMessage(ACTOR_DOG, 0);
  }
  Event_End();
}

/// Felix's update while he rides Lonesome George: kept 16 pixels above him,
/// facing his way.
void Islets_RideOnLonesomeGeorge(struct Actor *actor) {
  struct Actor *source = Actor_Get(ACTOR_LONESOME_GEORGE);

  FieldActor_SetPositionAndResetMotion(
      actor, source->x.fixed, source->y.fixed + toFIXED(16), source->z.fixed);
  actor->facing = source->facing;
}

/// How the screen closes as Lonesome George swims off with the party: round
/// Felix in the Japanese release, in a shape from patch 1 on.
#if !TLA_PATCH_1
#define ISLETS_RIDE_TRANSITION                                                 \
  SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER)
#else
#define ISLETS_RIDE_TRANSITION                                                 \
  SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE)
#endif

extern struct ActionScript gIslets_SpinScript;

/// Lonesome George takes the Li'l Turtle: he smiles, spins round for joy and
/// cries "At long last, I have a friend!", offers to show the party his secret
/// spot, and swims off south with Felix on his back and the little turtle
/// behind, to the secret spot.
void Islets_LonesomeGeorgeThanks(struct Actor *gift) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *guide = Actor_Get(ACTOR_LONESOME_GEORGE);
  struct ActionScript script = gIslets_SpinScript;

  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldPsynergy_GreyMindReadPalette();
  Actor_ShowEmote(ACTOR_LONESOME_GEORGE, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  guide->update = Actor_EmitWadingSplashes;
  Actor_RunScriptAndWait(ACTOR_LONESOME_GEORGE, &script);
  guide->update = 0;
  FieldEvent_SetMessageId(MsgAtLongLast);
  // "At long last, I have a friend! Lonesome George I am no more! I'm so..."
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  // "I don't have much to offer in return, but I can show you... my..."
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST);
  Actor_SetSpeed(ACTOR_LONESOME_GEORGE, 1.6);
  guide->update = Actor_EmitWadingSplashes;
  Actor_WalkToAndWait(ACTOR_LONESOME_GEORGE, 296, 452);
  guide->update = 0;
  Actor_FaceDirection(ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST, 0);
  gift->speedLimit = toFIXED(8) / 5;
  gift->acceleration = toFIXED(4) / 5;
  gift->update = Actor_EmitWadingSplashes;
  Actor_SelectAnimation(gift, 2);
  FieldActor_SetMoveTarget(gift, 278, 0, 452);
  FieldActor_WaitForMove(gift);
  Actor_SelectAnimation(gift, 1);
  gift->update = 0;
  PartyInventory_Remove(ITEM_LIL_TURTLE);
  // "Hop on my back!"
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  ((struct MindReadWork *)HeapSlot_AllocEwram(HEAP_SLOT_MIND_READ,
                                              sizeof(struct MindReadWork)))
      ->counter = MIND_READ_QUIET;
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  leader->update = Actor_EmitWadingSplashes;
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 312, 446);
  leader->update = 0;
  FieldActor_Jump(gMainRoutineWork.leaderActorId, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 296, 452);
  leader->update = Islets_RideOnLonesomeGeorge;
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId),
                            SPRITE_COLOR_HIDDEN);
  Actor_SetAnimation(ACTOR_LONESOME_GEORGE, ANIM_NOD);
  FieldEvent_Wait(20);
  // "And we're off!"
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  Actor_SetSpeed(ACTOR_LONESOME_GEORGE, 0.8);
  gift->speedLimit = toFIXED(4) / 5;
  gift->acceleration = toFIXED(2) / 5;
  guide->update = Actor_EmitWadingSplashes;
  Actor_SetDestination(ACTOR_LONESOME_GEORGE, 288, 538);
  FieldEvent_Wait(20);
  FieldPsynergy_RestoreMindReadPalette();
  PaletteCycle_Stop();
  gift->update = Actor_EmitWadingSplashes;
  Actor_SelectAnimation(gift, 2);
  FieldActor_SetMoveTarget(gift, 288, 0, 538);
  FieldEvent_Wait(20);
  gFieldWork->transition = ISLETS_RIDE_TRANSITION;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(EXIT_TO_SECRET_SPOT);
  Event_SetFlag(FLAG_GEORGE_HAS_THE_TURTLE);
}

#define SPAWN(a, b, answer)                                                    \
  {                                                                            \
    a = FieldActor_Create(SPRITE_TURTLE_SMALL, toFIXED(296), 0, toFIXED(424)); \
    b = FieldActor_Create(SPRITE_WADING_SPLASH, toFIXED(296), 0,               \
                          toFIXED(424));                                       \
    if (a != 0) {                                                              \
      a->motionFlags = answer;                                                 \
      Actor_SetSpriteFlags(a, 0);                                              \
    }                                                                          \
    if (b != 0) {                                                              \
      b->scaleY = FIXED_ONE * 2;                                               \
      Actor_SetSpriteFlags(b, 0);                                              \
      Actor_SelectAnimation(b, 1);                                             \
      Actor_StartScript(b, gIslets_WadingSplashScript);                        \
    }                                                                          \
  }

/// Mind Read on Lonesome George, the big turtle in the shallows below the Sea
/// of Time Islet's cliffs, with the Li'l Turtle in the party: he is sad and
/// lonely, Felix wades over and lets the little turtle into the water, and
/// when George asks him to set it free with him, a yes makes them friends
/// (Islets_LonesomeGeorgeThanks); a no takes it back, and the next Mind Read
/// asks at once. Once he has the turtle he offers to swim the party to his
/// secret spot again; before, without it, he is just very lonely.
void Islets_GiveTurtle(void) {
  struct Actor *leader, *a, *b, *other;
  const u8 *message;
  s32 answer;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (PartyInventory_FindMember(ITEM_LIL_TURTLE) != -1) {
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (GameFlag_Get(FLAG_TURTLE_REFUSED) != 0) {
      Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
      /* FAKEMATCH(dead): a set to the splash update, never read: SPAWN sets
       * it before its use. */
      a = (struct Actor *)Actor_EmitWadingSplashes;
      leader->update = Actor_EmitWadingSplashes;
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 318, 424);
      leader->update = 0;
      Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
      Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_NORTH_NORTHEAST);
      message = MsgOhYouRe;
      Event_SetMessageId(message);
      Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
      FieldPsynergy_RestoreMindReadPalette();
      answer =
          Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0);
      if (answer == 0) {
        Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_WEST);
        Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
        leader->update = Actor_EmitWadingSplashes;
        STEP(gMainRoutineWork.leaderActorId, 302, 424);
        SPAWN(a, b, 0)
        STEP(gMainRoutineWork.leaderActorId, 318, 424);
        leader->update = 0;
        Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHWEST,
                            0);
        Islets_LonesomeGeorgeThanks(a);
      } else {
        FieldPsynergy_GreyMindReadPalette();
        Event_SetMessageId(message - 1);
        Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
        Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE,
                                   FACING_SOUTH_SOUTHWEST);
      }
    } else {
      Event_SetMessageId(MsgIMSoVery);
      // "I'm so very sad and lonely. I wish I had a friend to call my own..."
      Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
      Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                          EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
      Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
      leader->update = Actor_EmitWadingSplashes;
      // Default: SPAWN sets a to the small turtle it creates.
      a = 0;
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 318, 424);
      leader->update = 0;
      Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_WEST);
      Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
      leader->update = Actor_EmitWadingSplashes;
      STEP(gMainRoutineWork.leaderActorId, 302, 424);
      SPAWN(a, b, 0)
      STEP(gMainRoutineWork.leaderActorId, 318, 424);
      leader->update = 0;
      Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
      Actor_ShowEmoteById(ACTOR_LONESOME_GEORGE,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
      Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_NORTH_NORTHWEST);
      other = Actor_GetById(ACTOR_LONESOME_GEORGE);
      other->update = Actor_EmitWadingSplashes;
      FieldActor_Jump(ACTOR_LONESOME_GEORGE, 4, 20);
      other->update = 0;
      // "Hey! You've got an itty- bitty turtle tot, don't you?"
      Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
      Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_NORTH_NORTHEAST);
      // "Why don't you set that li'l guy free with me?"
      Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
      FieldPsynergy_RestoreMindReadPalette();
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
          0) {
        Islets_LonesomeGeorgeThanks(a);
      } else {
        FieldPsynergy_GreyMindReadPalette();
        Actor_ShowEmoteById(ACTOR_LONESOME_GEORGE,
                            EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
        // "No? Oh... Then lonely I shall stay. But if you change your mind..."
        Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
        Actor_FaceDirection(ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST, 0);
        Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_WEST);
        leader->update = Actor_EmitWadingSplashes;
        Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 302, 424);
        leader->update = 0;
        if (a != 0)
          Actor_Release(a);
        Event_SetFlag(FLAG_TURTLE_REFUSED);
      }
    }
  } else if (GameFlag_Get(FLAG_GEORGE_HAS_THE_TURTLE)) {
    Actor_FaceActor(ACTOR_LONESOME_GEORGE, gMainRoutineWork.leaderActorId, 0);
    Event_SetMessageId(MsgWantToGo);
    // "Want to go to my secret spot again?"
    Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
    FieldPsynergy_RestoreMindReadPalette();
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
        0) {
      struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
      FieldPsynergy_GreyMindReadPalette();
      (gFieldWork->message)++;
      // "Then hop on my back!"
      Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
      ((struct MindReadWork *)HeapSlot_AllocEwram(HEAP_SLOT_MIND_READ,
                                                  sizeof(struct MindReadWork)))
          ->counter = MIND_READ_QUIET;
      FieldActor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
      Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST);
      Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
      FieldActor_Jump(gMainRoutineWork.leaderActorId, 6, 0);
      Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 296, 452);
      leader->update = Islets_RideOnLonesomeGeorge;
      FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                                SPRITE_COLOR_HIDDEN);
      Actor_SetAnimation(ACTOR_LONESOME_GEORGE, ANIM_NOD);
      FieldEvent_Wait(20);
      // "And we're off!"
      Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
      FieldPsynergy_RestoreMindReadPalette();
      Actor_SetSpeed(ACTOR_LONESOME_GEORGE, 0.8);
      Actor_SetSpeed(ACTOR_LIL_TURTLE, 0.8);
      Actor_GetById(ACTOR_LONESOME_GEORGE)->update = Actor_EmitWadingSplashes;
      Actor_SetDestination(ACTOR_LONESOME_GEORGE, 288, 538);
      FieldEvent_Wait(40);
      FieldPsynergy_RestoreMindReadPalette();
      PaletteCycle_Stop();
      Actor_GetById(ACTOR_LIL_TURTLE)->update = Actor_EmitWadingSplashes;
      Actor_WalkTo(ACTOR_LIL_TURTLE, 288, 538);
      FieldEvent_Wait(20);
      gFieldWork->transition = ISLETS_RIDE_TRANSITION;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      FieldPsynergy_GreyMindReadPalette();
      FieldEvent_SetExitRequest(EXIT_TO_SECRET_SPOT);
    } else {
      FieldPsynergy_GreyMindReadPalette();
      Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
      Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST);
    }
  } else {
    Event_SetMessageId(MsgIMSoVeryVery);
    // "I'm so very...very lonely. I could sure use a friend."
    Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  }
  Event_End();
}

extern u32 gIslets_GeorgeSwimsBackScript[];
extern u32 gIslets_LilTurtleSwimsBackScript[];

/// Mind Read on Lonesome George at his secret spot: "You all set to go back to
/// the island?" With a yes Felix hops on and George swims north with the Li'l
/// Turtle, back to the Sea of Time Islet; with a no he says to take your time.
void Islets_RideBackToIsland(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *actor = Actor_Get(ACTOR_LONESOME_GEORGE);
  void (*update)(void);
  s32 result;
  u8 *done;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(ACTOR_LONESOME_GEORGE, gMainRoutineWork.leaderActorId, 0);
  FieldEvent_Wait(10);
  FieldEvent_SetMessageId((s32)MsgYouAllSet);
  // "You all set to go back to the island?"
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  FieldPsynergy_RestoreMindReadPalette();
  result =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0);
  if (result == 0) {
    FieldPsynergy_GreyMindReadPalette();
    Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
    ((struct MindReadWork *)HeapSlot_AllocEwram(HEAP_SLOT_MIND_READ,
                                                sizeof(struct MindReadWork)))
        ->counter = MIND_READ_QUIET;
    FieldActor_Jump(gMainRoutineWork.leaderActorId, 6, 0);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
    Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 152, 768);
    leader->update = (ObjectUpdate)Islets_RideOnLonesomeGeorge;
    FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId),
                              SPRITE_COLOR_HIDDEN);
    Actor_SetAnimation(ACTOR_LONESOME_GEORGE, ANIM_NOD);
    FieldEvent_Wait(20);
    FieldPsynergy_RestoreMindReadPalette();
    Actor_SetSpeed(ACTOR_LONESOME_GEORGE, 1.6);
    Actor_SetSpeed(ACTOR_LIL_TURTLE, 1.6);
    update = (void (*)(void))Actor_EmitWadingSplashes;
    done = &actor->riseEnabled;
    *done = 0;
    actor->update = update;
    Actor_SetScript(ACTOR_LONESOME_GEORGE, gIslets_GeorgeSwimsBackScript);
    FieldEvent_Wait(30);
    Actor_Get(ACTOR_LIL_TURTLE)->update = update;
    Actor_SetScript(ACTOR_LIL_TURTLE, gIslets_LilTurtleSwimsBackScript);
    do {
      System_Wait(1);
    } while (*done == 0);
    PaletteCycle_Stop();
    System_Wait(1);
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    FieldPsynergy_GreyMindReadPalette();
    FieldEvent_SetExitRequest(EXIT_BACK_FROM_SECRET_SPOT);
  } else {
    FieldPsynergy_GreyMindReadPalette();
    FieldEvent_SetMessageId((s32)MsgYouAllSet + 4);
    // "Take your time! I don't like to rush..."
    Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
    Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH);
  }
  Event_End();
}

/// The sailor's son on West Indra Islet's pier boasts that his pop is the best
/// sailor in all the Eastern Sea.
void Islets_TalkToSailorsSon(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(ACTOR_SAILORS_SON, gMainRoutineWork.leaderActorId, 0);
  FieldEvent_SetMessageId((s32)MsgMyPopS);
  // "My pop's the best sailor in all the Eastern Sea! That's why he can..."
  Event_ShowMessage(ACTOR_SAILORS_SON, 0);
  Actor_FaceDirectionAndWait(ACTOR_SAILORS_SON, FACING_NORTH_NORTHWEST);
  Event_End();
}

/// Nothing visible happens when Felix talks to one of the two unseen blocks on
/// the edge of the hut's deck on the Sea of Time Islet: the menu opens, just as
/// when he talks to nothing.
void Islets_OpenFieldMenu(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->fieldMenu = 1;
}

/// A task: while Felix is below the hut's deck (less than 40 pixels up), its
/// two unseen blocks are passed over by talk and Psynergy.
void Islets_SetDeckBlocksByHeight(void) {
  if (Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed < toFIXED(40)) {
    Actor_Get(ACTOR_DECK_BLOCK_A)->collisionFlags |= ACTOR_NOT_TARGETED;
    Actor_Get(ACTOR_DECK_BLOCK_B)->collisionFlags |= ACTOR_NOT_TARGETED;
  } else {
    Actor_Get(ACTOR_DECK_BLOCK_A)->collisionFlags &= ~ACTOR_NOT_TARGETED;
    Actor_Get(ACTOR_DECK_BLOCK_B)->collisionFlags &= ~ACTOR_NOT_TARGETED;
  }
}

/// Unused: the bytes between gIslets_WaveTableIndex and the wave lines, which
/// nothing reads; the editions after the Japanese one leave four more.
#if TLA_PATCH_1
#define WAVE_LINES_PADDING 11
#else
#define WAVE_LINES_PADDING 15
#endif

u8 gIslets_WaveTableIndex BSS_DATA;
u8 gIslets_UnusedAfterWaveTableIndex[WAVE_LINES_PADDING] BSS_DATA;
s16 gIslets_WaveLines[2][DISPLAY_HEIGHT] BSS_DATA;
s32 gIslets_SecondCrateZ BSS_DATA;
s32 gIslets_FirstCrateZ BSS_DATA;
s32 gIslets_SecondCrateX BSS_DATA;
s32 gIslets_FirstCrateX BSS_DATA;

/// Unused with Islets_StartWave: points the HBlank DMA at the wave table filled
/// last frame, so that each line of the water's background sways.
void Islets_StartWaveDma(void) {
  s16 *lines = gIslets_WaveLines[gIslets_WaveTableIndex];
  volatile u16 *dma;

  Io_Write16(lines[0], &REG_BG1HOFS);
  dma = REG_DMA0;
  dma[5] &= ~(DMA_START_SPECIAL | DMA_DREQ_ON | DMA_REPEAT);
  dma[5] &= ~DMA_ENABLE;
  (void)dma[5];
  Dma_Set(lines + 1, (void *)&REG_BG1HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)dma);
}

/// Unused with Islets_StartWave: fills the next wave table with a sine wave
/// that moves each frame, swaying each line of the water up to 2 pixels either
/// way.
void Islets_FillWaveLines(void) {
  struct MapLayer *layer =
      &((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[2];
  s16 *lines = gIslets_WaveLines[gIslets_WaveTableIndex ^ 1];
  FIXED v[2];
  s32 *count = &v[1];
  FIXED sine;

  for (*count = 0; *count != DISPLAY_HEIGHT; (*count)++) {
    sine = Math_Sin((*count + (u8)gFrameCount + FIXEDtoINT(layer->y)) << 9);
    v[0] = sine;
    *lines++ = FIXEDtoINT(layer->x) + FIXEDtoINT_TWICE(sine) + *count % 3 - 1;
  }
  gIslets_WaveTableIndex ^= 1;
}

/// Unused: starts the water's ripple, each line of its background swaying as
/// Lemuria's water does.
UNUSED void Islets_StartWave(void) {
  Task_AddFrame(Islets_FillWaveLines, TASK_PASS_FRAME);
  Task_AddFrame(Islets_StartWaveDma, TASK_PASS_VBLANK);
}

/// Once Felix has pushed Pengulina over the log to Pengus's shore and steps up
/// behind her (cell (13, 10)), Pengus hops for joy, thanks him for saving his
/// sweet Pengulina and waddles over, and Felix holds up the Pretty Stone he is
/// given.
void Islets_PengusGivesPrettyStone(void) {
  struct Actor *actor = Actor_GetById(ACTOR_PENGULINA);
  struct Actor *leader;
  struct Actor *item;
  FIXED z;

  if (actor->x.fixed < toFIXED(228) || actor->x.fixed > toFIXED(236))
    return;
  z = actor->z.fixed;
  if (z < toFIXED(164))
    return;
  if (z > toFIXED(172))
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(ACTOR_PENGULINA, ANIM_STAND);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 216, 184);
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.leaderActorId, 9);
  FieldEvent_SetMessageId((s32)MsgThankYouSo);
  FieldActor_Jump(ACTOR_PENGUS, 2, 10);
  FieldActor_Jump(ACTOR_PENGUS, 4, 10);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PENGUS, FACING_WEST, 10);
  // "Thank you so much for saving my sweet Pengulina. Allow me to give..."
  Event_ShowMessage(ACTOR_PENGUS, 0);
  Actor_GetById(ACTOR_PENGUS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_PENGUS, 222, 186);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_PENGUS)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_PENGUS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_PENGUS, 232, 186);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_PENGUS)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  item = Actor_GetById(ACTOR_PRETTY_STONE_ICON);
  item->motionFlags = 0;
  FieldActor_SetItemIcon(item, ITEM_PRETTY_STONE);
  FieldActor_SetPositionAndResetMotion(
      item, leader->x.fixed, leader->y.fixed + toFIXED(24), leader->z.fixed);
  FieldEvent_Wait(20);
  FieldPsynergy_RestoreMindReadPalette();
  Party_GiveItem(ITEM_PRETTY_STONE, 0);
  FieldPsynergy_GreyMindReadPalette();
  Actor_SetPosition(ACTOR_PRETTY_STONE_ICON, 0, 0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  FieldPsynergy_StopMindReadWave();
  Actor_FaceDirectionAndWait(ACTOR_PENGUS, FACING_NORTH_NORTHWEST);
  Event_SetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE);
  Event_End();
}

/// A task: the footstep sound every 16 frames, while the bird flies back to its
/// nest.
void Islets_PlayFootsteps(void) {
  if ((gFrameCount & 15) == 0)
    Event_PlaySound(SE_FOOTSTEP);
}

/// When Felix climbs up near the bird's nest on SE Angara Islet's cliff top
/// (cell (7, 21)), the bird stops circling over the east of the islet, flies
/// across, hops onto the nest and turns to watch him.
void Islets_BirdReturnsToNest(void) {
  struct Actor *actor = Actor_Get(ACTOR_BIRD);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_Stop(ACTOR_BIRD);
  System_Wait(1);
  actor->motionFlags = 0;
  Actor_SetSpeed(ACTOR_BIRD, 1.6);
  FieldActor_SetPositionAndResetMotion(actor, toFIXED(268), toFIXED(64),
                                       toFIXED(390));
  System_Wait(1);
  actor->groundHeight = toFIXED(128);
  System_Wait(1);
  actor->y.fixed = toFIXED(160);
  System_Wait(1);
  FieldTask_AddFrame(Islets_PlayFootsteps, TASK_PASS_FRAME);
  FieldActor_SetMoveTarget(actor, 132, 128, 320);
  FieldActor_WaitForMove(actor);
  FieldTask_Remove(Islets_PlayFootsteps);
  Actor_SetAnimation(ACTOR_BIRD, 5);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldActor_SetSpritePriority(ACTOR_BIRD, 2);
  Actor_FaceActor(ACTOR_BIRD, gMainRoutineWork.leaderActorId, 0);
  FieldEvent_Wait(10);
  FieldActor_Jump(ACTOR_BIRD, 4, 0);
  Actor_MoveToAndWait(ACTOR_BIRD, 120, 312);
  Actor_SetAnimation(ACTOR_BIRD, 5);
  actor->motionFlags = 0;
  Actor_FaceDirectionAndWait(ACTOR_BIRD, FACING_EAST);
  Event_SetFlag(FLAG_BIRD_ON_NEST);
  System_Wait(1);
  Event_End();
}

/// Walking south off the east end of E Tundaria Islet's icy ledge (cell
/// (15, 8)), Felix jumps down to the snow below.
void Islets_JumpDownLedge(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_JumpDownLedge(gMainRoutineWork.leaderActorId, 6, 1);
  Event_End();
  return;
}

/// Sand on N Osenia Islet or the Sea of Time Islet: Felix sinks into the
/// ground and moves through it as a mound of sand. On N Osenia Islet, cast on
/// the strip of sand west of the cow's fence, the mound slides north along the
/// strip past the barrels and he rises out of the sand at its north end, onto
/// the grass. On the Sea of Time Islet, cast at the foot of the cliff by the
/// palms, it travels east along the cliff's foot and down the beach to the
/// water's edge, where he rises. The event only puts him on the map's upper
/// layer, which holds the ways open to Sand.
void Islets_SinkIntoSand(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  leader->layer = 2;
}

/// When Sand ends on N Osenia Islet or the Sea of Time Islet, Felix rises out
/// of the ground where he is and walks on the ground layer again.
void Islets_RiseFromSand(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  leader->layer = 0;
}

/// Sets the behaviour of the map cell under a 16.16 position, on the ground
/// layer (bit 0 of layers), the upper one (bit 1) or both.
void Islets_SetCellCollision(s32 x, s32 z, s32 layers, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *lowerCells = map->layers[0].cells;
  struct MapCell *upperCells = map->layers[2].cells;
  s32 index = x / toFIXED(16) + (z / toFIXED(16)) * MAPGRID_SIZE;

  if (layers & 1) {
    lowerCells += index;
    lowerCells->collisionCode = value;
  }
  if (layers & 2) {
    upperCells += index;
    upperCells->collisionCode = value;
  }
}

/// Nothing visible happens as Felix starts to push or Move one of SE Angara
/// Islet's two crates: the game notes where both stand. When one stands at a
/// ledge's edge right above the other below, (7, 28) over (7, 29) or (12, 25)
/// over (12, 26), the lower one's cell is opened on the upper layer, so that
/// the upper crate can come down on top of it.
void Islets_StartPushingCrate(void) {
  struct Actor *actor;
  s32 *x0;
  s32 *z0;
  FIXED x1;
  FIXED z1;

  actor = Actor_Get(ACTOR_CRATE_A);
  x0 = &gIslets_FirstCrateX;
  z0 = &gIslets_FirstCrateZ;
  *x0 = actor->x.fixed;
  *z0 = actor->z.fixed;
  actor = Actor_Get(ACTOR_CRATE_B);
  gIslets_SecondCrateX = x1 = actor->x.fixed;
  gIslets_SecondCrateZ = z1 = actor->z.fixed;
  if (FIELD_CELL(*x0) == 7 && FIELD_CELL(*z0) == 29 && FIELD_CELL(x1) == 7 &&
      FIELD_CELL(z1) == 28)
    Islets_SetCellCollision(*x0, *z0, 2, 0);
  if (FIELD_CELL(gIslets_FirstCrateX) == 12 &&
      FIELD_CELL(gIslets_FirstCrateZ) == 25 &&
      FIELD_CELL(gIslets_SecondCrateX) == 12 &&
      FIELD_CELL(gIslets_SecondCrateZ) == 26)
    Islets_SetCellCollision(gIslets_SecondCrateX, gIslets_SecondCrateZ, 2, 0);
}

/// A crate Felix has pushed or Moved on SE Angara Islet settles, and one pushed
/// off its ledge drops to the ground below with a rattle, the upper ledge's to
/// cell (12, 26) and the middle ledge's to (7, 29). If the other crate already
/// stands there, the two end up stacked in that cell for good, a step Felix
/// can hop onto. The cells each crate leaves are opened and those it rests on
/// walled.
void Islets_FinishPushingCrate(void) {
  struct Actor *first = Actor_Get(ACTOR_CRATE_A);
  struct Actor *second = Actor_Get(ACTOR_CRATE_B);
  s32 firstX = FIELD_CELL(first->x.fixed);
  s32 firstZ = FIELD_CELL(first->z.fixed);
  s32 secondX = FIELD_CELL(second->x.fixed);
  s32 secondZ = FIELD_CELL(second->z.fixed);
  s32 both;

  if (FIELD_CELL(gIslets_FirstCrateX) == 12 &&
      FIELD_CELL(gIslets_FirstCrateZ) == 24) {
    Islets_SetCellCollision(gIslets_FirstCrateX, gIslets_FirstCrateZ, 1,
                            STEP_TRIGGER_PUSH_CRATE_ON);
    Islets_SetCellCollision(gIslets_FirstCrateX, gIslets_FirstCrateZ, 2, 0);
  } else {
    s32 *z = &gIslets_FirstCrateZ;

    Islets_SetCellCollision(gIslets_FirstCrateX, *z, 3, 0);
  }
  if (FIELD_CELL(gIslets_SecondCrateX) == 7 &&
      FIELD_CELL(gIslets_SecondCrateZ) == 27) {
    Islets_SetCellCollision(gIslets_SecondCrateX, gIslets_SecondCrateZ, 1,
                            STEP_TRIGGER_PUSH_CRATE_ON);
    Islets_SetCellCollision(gIslets_SecondCrateX, gIslets_SecondCrateZ, 2, 0);
  } else {
    s32 *z = &gIslets_SecondCrateZ;

    Islets_SetCellCollision(gIslets_SecondCrateX, *z, 3, 0);
  }
  Islets_SetCellCollision(first->x.fixed, first->z.fixed, 3,
                          CELL_BEHAVIOR_WALL);
  Islets_SetCellCollision(second->x.fixed, second->z.fixed, 3,
                          CELL_BEHAVIOR_WALL);
  if (!Event_GetFlag(FLAG_CRATE_A_DROPPED) && firstX == 12 && firstZ == 26) {
    both = 0;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (secondX == 12 && secondZ == 26) {
      MapGrid_CopyCellAttributes(7, 2, 1, 1, firstX, firstZ);
      both = 1;
    }
    first->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    System_Wait(10);
    Event_PlaySound(SE_RATTLE);
    System_Wait(10);
    first->motionFlags = 0;
    Event_SetFlag(FLAG_CRATE_A_DROPPED);
    if (both) {
      Islets_SetCellCollision(first->x.fixed, first->z.fixed, 3, 0);
      Islets_SetCellCollision(second->x.fixed, second->z.fixed, 3, 0);
      MapGrid_CopyCellAttributes(7, 3, 1, 1, 12, 26);
      Event_SetFlag(FLAG_CRATES_IN_ONE_CELL);
      first->riseCounter = 1;
      second->riseCounter = 1;
    }
    Event_End();
  }
  if (!Event_GetFlag(FLAG_CRATE_B_DROPPED) && secondX == 7 && secondZ == 29) {
    both = 0;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (firstX == 7 && firstZ == 29) {
      MapGrid_CopyCellAttributes(7, 2, 1, 1, firstX, firstZ);
      both = 1;
    }
    second->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    System_Wait(10);
    Event_PlaySound(SE_RATTLE);
    System_Wait(10);
    second->motionFlags = 0;
    Event_SetFlag(FLAG_CRATE_B_DROPPED);
    if (both) {
      Islets_SetCellCollision(first->x.fixed, first->z.fixed, 3, 0);
      Islets_SetCellCollision(second->x.fixed, second->z.fixed, 3, 0);
      MapGrid_CopyCellAttributes(7, 3, 1, 1, 7, 29);
      Event_SetFlag(FLAG_CRATES_IN_ONE_CELL);
      first->riseCounter = 1;
      second->riseCounter = 1;
    }
    Event_End();
  }
}

/// Walking south from cell (12, 24) on SE Angara Islet's upper ledge, or
/// (7, 27) on its middle one, Felix pushes the crate standing at the ledge's
/// edge over it, and it drops below (Islets_FinishPushingCrate). When the
/// other crate already stands below, at (7, 29), this one lands on top of it.
void Islets_PushCrateOffLedge(void) {
  struct Actor *first = Actor_Get(ACTOR_CRATE_A);
  struct Actor *second = Actor_Get(ACTOR_CRATE_B);

  gIslets_FirstCrateX = first->x.fixed;
  gIslets_FirstCrateZ = first->z.fixed;
  gIslets_SecondCrateX = second->x.fixed;
  gIslets_SecondCrateZ = second->z.fixed;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (FIELD_CELL(gIslets_FirstCrateX) == 7 &&
      FIELD_CELL(gIslets_FirstCrateZ) == 29 &&
      FIELD_CELL(gIslets_SecondCrateX) == 7 &&
      FIELD_CELL(gIslets_SecondCrateZ) == 28) {
    first->riseCounter = 0;
    ((void (*)())Islets_SetCellCollision)(gIslets_FirstCrateX,
                                          gIslets_FirstCrateZ, 2);
  }
  Leader_PushObjectAhead();
  Islets_FinishPushingCrate();
  Event_End();
}

/// Frost on the puddle at the foot of SE Angara Islet's west cliff (cell
/// (5, 29)) freezes it into an ice pillar under the vine, a step Felix can hop
/// onto from the stacked crates and climb on from.
void Islets_FreezePuddle(void) {
  MapGrid_CopyCellAttributes(5, 2, 1, 2, 5, 28);
  Event_SetFlag(FLAG_SOUTHEAST_ANGARA_ISLET_PUDDLE_FROZEN);
}

/// Holding Up on the iceberg moored at the foot of E Tundaria Islet's west
/// shore, below the ladder (cell (4, 18)), Felix steps up off it onto the snow.
void Islets_StepUpOntoShore(void) {
  struct Actor *actor;

  if (gInput.held & KEY_UP) {
    actor = Actor_Get(gMainRoutineWork.leaderActorId);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    actor->motionFlags = 0;
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 72, 280);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    Event_End();
  }
}

/// Holding Down at the south edge of E Tundaria Islet's west shore, below the
/// ladder (cell (4, 17)), Felix steps down onto the iceberg there, but only
/// while it lies moored between its drifts (before frame 10 or after frame 210
/// of the icebergs' round).
void Islets_StepDownOntoIceberg(void) {
  struct Actor *actor;
  u16 *frame;

  if (gInput.held & KEY_DOWN) {
    frame = gSceneState;
    actor = Actor_Get(gMainRoutineWork.leaderActorId);
    if ((u16)(*frame - 10) > 200) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      actor->motionFlags = 0;
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 72, 296);
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Event_End();
    }
  }
}

/// Nothing visible happens as Reveal takes effect on E Tundaria Islet, but the
/// icebergs hold still until it fades (Islets_EndReveal).
void Islets_StartReveal(void) {
  Event_SetFlag(FLAG_REVEAL_SHOWN);
  return;
}

/// Nothing visible happens as Reveal fades on E Tundaria Islet, but the
/// icebergs drift again.
void Islets_EndReveal(void) {
  Event_ClearFlag(FLAG_REVEAL_SHOWN);
  return;
}

// The following six are from patch 1.
#if TLA_PATCH_1
/// Cyclone on the weeds round the cow in its yard on N Osenia Islet (cells
/// (5, 10) to (8, 13)) blows them away, and they stay gone while the party is
/// on the islet. Added after the Japanese release: each clump of weeds is one
/// of six events, which draws the cleared grass from the map's spare columns
/// over the cells Cyclone touched and records them
/// (FieldTileCopies_CopyTouchedCells) so that they stay cleared after a battle
/// or the menu.
void Islets_ClearFirstPatch(void) {
  FieldTileCopies_CopyTouchedCells(65, 0, 4, 4, 65, 10, 1);
  return;
}

/// Cyclone on the two weeds just east of the cow's fence, in front of N Osenia
/// Islet's house (cells (12, 10) and (11, 11)), blows them away for good, as
/// Islets_ClearFirstPatch does.
void Islets_ClearSecondPatch(void) {
  FieldTileCopies_CopyTouchedCells(65, 0, 2, 2, 71, 10, 1);
  return;
}

/// Cyclone on the weed south of the cow's yard, by its fence (cell (8, 15)),
/// blows it away for good, as Islets_ClearFirstPatch does.
void Islets_ClearThirdPatch(void) {
  FieldTileCopies_CopyTouchedCells(65, 0, 1, 1, 68, 15, 1);
  return;
}

/// Cyclone on the weeds on N Osenia Islet's tilled field, south of the house
/// (cells (15, 20) to (16, 22)), blows them away for good, as
/// Islets_ClearFirstPatch does.
void Islets_ClearFourthPatch(void) {
  FieldTileCopies_CopyTouchedCells(65, 0, 2, 3, 75, 20, 1);
  return;
}

/// Cyclone on the weeds just east of N Osenia Islet's tilled field (cells
/// (18, 22) to (21, 22)) blows them away for good, as Islets_ClearFirstPatch
/// does.
void Islets_ClearFifthPatch(void) {
  FieldTileCopies_CopyTouchedCells(65, 0, 4, 1, 78, 22, 1);
  return;
}

/// Cyclone on the weed by the fence south-east of N Osenia Islet's tilled field
/// (cell (19, 23)) blows it away for good, as Islets_ClearFirstPatch does.
void Islets_ClearSixthPatch(void) {
  FieldTileCopies_CopyTouchedCells(65, 0, 1, 1, 79, 23, 1);
  return;
}
#endif

/// Walking north into the wooden door of the round snow hut on E Tundaria Islet
/// (cell (18, 12)), Felix lines up with it, it swings open and he walks inside.
/// The door's cell behaviour is the exit he leaves by, and picks its opening
/// from gIslets_Doors.
void Islets_OpenDoorAndEnter(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *actor;
  s16 *exit;
  s32 index;

  actor = Actor_Get(party->leaderActorId);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetAnimation(party->leaderActorId, ANIM_WALK);
  Actor_SetSpeed(party->leaderActorId, 0.5);
  FieldActor_SetMoveTargetFX(
      actor,
      toFIXED(MAPGRID_CELL_PIXELS / 2 -
              FIXEDtoINT(actor->x.fixed) % MAPGRID_CELL_PIXELS) +
          actor->x.fixed,
      actor->y.fixed, actor->z.fixed);
  FieldActor_WaitForMove(actor);
  actor->facing = FACING_NORTH;
  exit = &work->stepTrigger;
  index = *exit;
  Event_PlaySound(SE_DOOR_OPEN);
  index--;
  MapGrid_AnimateCells(gIslets_Doors[index].frames, gIslets_Doors[index].x,
                       gIslets_Doors[index].z);
  Actor_SetScript(party->leaderActorId, gIslets_EnterDoorScript);
  System_Wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(*exit);
}

/// On the Sea of Time Islet, stepping onto the path out through the palms at
/// the islet's north-west (cells (5, 5) and (6, 5)) takes the party out to the
/// World Map, and onto the door of the hut on stilts (cell (13, 10)) takes them
/// inside. The cell's behaviour is the exit, and the surf stops moving as the
/// screen goes.
void Islets_LeaveByStep(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  PaletteCycle_Stop();
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->stepTrigger);
}

/// Walking north into the door of the thatched hut on stilts by West Indra
/// Islet's piers (cell (13, 48)), Felix lines up with it and walks inside.
void Islets_EnterDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *actor;

  actor = Actor_Get(party->leaderActorId);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetAnimation(party->leaderActorId, ANIM_WALK);
  Actor_SetSpeed(party->leaderActorId, 0.5);
  FieldActor_SetMoveTargetFX(
      actor,
      toFIXED(MAPGRID_CELL_PIXELS / 2 -
              FIXEDtoINT(actor->x.fixed) % MAPGRID_CELL_PIXELS) +
          actor->x.fixed,
      actor->y.fixed, actor->z.fixed);
  FieldActor_WaitForMove(actor);
  actor->facing = FACING_NORTH;
  Actor_SetScript(party->leaderActorId, gIslets_EnterDoorScript);
  System_Wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->stepTrigger);
}

/// Lonesome George swims the party back from his secret spot into the shallows
/// below the Sea of Time Islet's cliffs, the Li'l Turtle following: "Here we
/// are!" Felix hops off, and George says to let him know whenever they want to
/// go again.
void Islets_ArriveBackAtIsland(void) {
  struct FieldWork *work;
  struct Actor *owner = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *actor = Actor_Get(ACTOR_LONESOME_GEORGE);

  Actor_PlaceOnActor(ACTOR_LONESOME_GEORGE, gMainRoutineWork.leaderActorId);
  Actor_SetPosition(ACTOR_LIL_TURTLE, 273, 520);
  System_Wait(1);
  actor->facing = FACING_NORTH;
  owner->update = (ObjectUpdate)Islets_RideOnLonesomeGeorge;
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId),
                            SPRITE_COLOR_HIDDEN);
  Actor_SetAnimation(ACTOR_LONESOME_GEORGE, ANIM_NOD);
  System_Wait(1);
  work = Ram_HeapSlots->fieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(ACTOR_LONESOME_GEORGE, 0.6);
  Actor_SetSpeed(ACTOR_LIL_TURTLE, 0.6);
  Actor_Get(ACTOR_LIL_TURTLE)->update = (ObjectUpdate)Actor_EmitWadingSplashes;
  actor->update = (ObjectUpdate)Actor_EmitWadingSplashes;
  Actor_WalkTo(ACTOR_LIL_TURTLE, 280, 468);
  Actor_MoveToAndWait(ACTOR_LONESOME_GEORGE, 296, 452);
  Actor_SetAnimation(ACTOR_LIL_TURTLE, 1);
  owner->update = 0;
  actor->update = 0;
  Actor_Get(ACTOR_LIL_TURTLE)->update = 0;
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.leaderActorId, 9);
  FieldEvent_SetMessageId((s32)MsgHereWeAre);
  // "Here we are!"
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  Actor_SetAnimation(ACTOR_LONESOME_GEORGE, ANIM_STAND);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId),
                            SPRITE_COLOR_NORMAL);
  FieldActor_Jump(gMainRoutineWork.leaderActorId, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 312, 440);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTHWEST);
  // "If you want to go to my secret spot again, just let me know."
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH);
  work = Ram_HeapSlots->fieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteCycle_Start();
}

extern u32 gIslets_GeorgeSwimsToSecretSpotScript[];
extern u32 gIslets_LilTurtleSwimsToSecretSpotScript[];

/// Lonesome George swims the party round to his secret spot, the little islet
/// with the cave mouth, the Li'l Turtle following: "Well, here we are." Felix
/// hops ashore below the cave, and George says to let him know when they want
/// to go back. The Japanese edition starts no screen transition here.
void Islets_ArriveAtSecretSpot(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *guide = Actor_Get(ACTOR_LONESOME_GEORGE);
  u8 *done;

  Actor_PlaceOnActor(ACTOR_LONESOME_GEORGE, gMainRoutineWork.leaderActorId);
  Actor_SetPosition(ACTOR_LIL_TURTLE, 240, 540);
  System_Wait(1);
  guide->facing = FACING_SOUTH;
  leader->update = Islets_RideOnLonesomeGeorge;
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId),
                            SPRITE_COLOR_HIDDEN);
  Actor_SetAnimation(ACTOR_LONESOME_GEORGE, ANIM_NOD);
  System_Wait(1);
#if TLA_PATCH_1 /* The Japanese release leaves the transition word. */
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
#endif
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(ACTOR_LONESOME_GEORGE, 0.8);
  Actor_SetSpeed(ACTOR_LIL_TURTLE, 0.8);
  Actor_Get(ACTOR_LIL_TURTLE)->update = Actor_EmitWadingSplashes;
  guide->update = Actor_EmitWadingSplashes;
  done = &guide->riseEnabled;
  *done = 0;
  Actor_StartScript(guide, gIslets_GeorgeSwimsToSecretSpotScript);
  Actor_SetScript(ACTOR_LIL_TURTLE, gIslets_LilTurtleSwimsToSecretSpotScript);
  do
    System_Wait(1);
  while (*done == 0);
  leader->update = 0;
  guide->update = 0;
  Actor_Get(ACTOR_LIL_TURTLE)->update = 0;
  Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_NORTH);
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.leaderActorId, 9);
  FieldEvent_SetMessageId(MsgWellHereWe);
  // "Well, here we are."
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  Actor_SetAnimation(ACTOR_LONESOME_GEORGE, ANIM_STAND);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId),
                            SPRITE_COLOR_NORMAL);
  FieldActor_Jump(gMainRoutineWork.leaderActorId, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 152, 742);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
  // "When you want to go back to the island, just let me know."
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH);
  PaletteCycle_Start();
#if TLA_PATCH_1
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
#endif
}

/// Sets up the islet being entered: the fade in, and the islet's own setup.
/// Returns 0.
s32 Scene_Initialize(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s16 scene;

  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  scene = gMainRoutineWork.next;
  if (scene == SCENE_WEST_INDRA_ISLET)
    Islets_InitializeWestIndra();
  else if (scene == SCENE_EAST_TUNDARIA_ISLET)
    Islets_InitializeEastTundaria();
  else if (scene == SCENE_NORTH_OSENIA_ISLET)
    Islets_InitializeNorthOsenia();
  else if (scene == SCENE_SOUTHEAST_ANGARA_ISLET)
    Islets_InitializeSoutheastAngara();
  else if (scene == SCENE_SEA_OF_TIME_ISLET)
    Islets_InitializeSeaOfTime();
  return 0;
}

/// A task: while Felix is down in the water among West Indra Islet's piers, the
/// sailor's two sons up on the pier are passed over by talk and Psynergy.
void Islets_SetSonsTargetedByHeight(void) {
  if (Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed > toFIXED(8)) {
    Actor_Get(ACTOR_SAILORS_SON)->collisionFlags &= ~ACTOR_NOT_TARGETED;
    Actor_Get(ACTOR_SAILORS_OTHER_SON)->collisionFlags &= ~ACTOR_NOT_TARGETED;
  } else {
    Actor_Get(ACTOR_SAILORS_SON)->collisionFlags |= ACTOR_NOT_TARGETED;
    Actor_Get(ACTOR_SAILORS_OTHER_SON)->collisionFlags |= ACTOR_NOT_TARGETED;
  }
}

/// Sets up West Indra Islet: wading in its shallows, the dog, the sailor's
/// other son pacing the pier, and, once the dog has traded, the Milk left in
/// front of it.
void Islets_InitializeWestIndra(void) {
  struct MapWorkGroundEffects *map =
      (struct MapWorkGroundEffects *)Ram_HeapSlots->mapGrid;
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == WEST_INDRA_ISLET_FROM_CHAMPA_ROOMS)
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  map->wading = MAP_WADING_UPPER_LEVEL;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_DOG), 0);
  Actor_SetSpeed(ACTOR_SAILORS_OTHER_SON, 0.8);
  Actor_Get(ACTOR_SAILORS_OTHER_SON)->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
  if (Event_GetFlag(FLAG_DOG_TRADED)) {
    spawned =
        FieldActor_Create(SPRITE_UNSEEN_MARKER, toFIXED(280), 0, toFIXED(840));
    if (spawned != 0) {
      spawned->motionFlags = 0;
      FieldActor_SetItemIcon(spawned, ITEM_MILK);
      Actor_SetSpriteFlags(spawned, 0);
      spawned->collisionFlags = ACTOR_NOT_TARGETED | ACTOR_SOLID;
      spawned->retainSpriteCache = 1;
    }
    Actor_Get(ACTOR_DOG)->facing = FACING_NORTH_NORTHEAST;
  } else {
    Actor_SetAnimation(ACTOR_DOG, 5);
  }
  Task_AddFrame(Islets_SetSonsTargetedByHeight, TASK_PASS_FRAME);
}

/// Readies an iceberg to drift: a platform that moves at 0.3 pixels a frame.
void Islets_StartIceberg(s32 id) {
  struct Actor *actor = Actor_Get(id);

  Actor_SetSpriteFlags(actor, 0);
  actor->retainSpriteCache = 3;
  actor->motionFlags = 0;
  actor->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  actor->acceleration = toFIXED(3) / 20;
  actor->speedLimit = toFIXED(3) / 10;
}

/// Sends an iceberg two cells (32 pixels) a column and row from where it was
/// placed.
void Islets_MoveIceberg(s32 id, s32 column, s32 row) {
  struct Actor *actor = Actor_Get(id);
  FIXED x = toFIXED(column * 32 + actor->timer);
  FIXED z = toFIXED(row * 32 + (s16)actor->userValue);

  FieldActor_SetMoveTargetFX(actor, x, actor->y.fixed, z);
}

/// A task: E Tundaria Islet's eight icebergs drift out and back. In each
/// 260-frame round they set off at frame 0, each two cells one way
/// (Islets_MoveIceberg), stop at 100, head home at 130 and stop at 220. They
/// hold still while Psynergy is cast or Reveal shows (FLAG_REVEAL_SHOWN), and
/// once Felix has fallen into the sea they hold still and raise
/// TRIGGER_SINK_IN_SEA.
void Islets_DriftIcebergs(void) {
  u16 *frame = &gSceneState[0];
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u16 *owed = &gSceneState[1];
  s32 i;
  s32 count;

  if (work->psynergy != 0 || GameFlag_Get(FLAG_REVEAL_SHOWN)) {
    for (i = ACTOR_FIRST_ICEBERG; i <= ACTOR_LAST_ICEBERG; i++)
      Actor_GetById(i)->paused = 1;
    return;
  }
  for (i = ACTOR_FIRST_ICEBERG; i <= ACTOR_LAST_ICEBERG; i++)
    Actor_GetById(i)->paused = 0;
  if (Actor_GetById(gMainRoutineWork.leaderActorId)->y.fixed < -toFIXED(12)) {
    work = Ram_HeapSlots->fieldWork;
    for (i = ACTOR_FIRST_ICEBERG; i <= ACTOR_LAST_ICEBERG; i++)
      Actor_GetById(i)->paused = 1;
    {
      s16 *raised = &work->raisedTrigger;
      s32 trigger = TRIGGER_SINK_IN_SEA;

      *raised = trigger;
    }
    return;
  }
  count = *owed + 1;
  *owed = count;
  if (work->pushCell != 0 || (u16)count == 0)
    return;
  do {
    switch (*frame) {
    case 0:
      for (i = ACTOR_FIRST_ICEBERG; i <= ACTOR_LAST_ICEBERG; i++)
        Actor_SetAnimation(i, 2);
      Islets_MoveIceberg(ACTOR_FIRST_ICEBERG, 0, 1);
      Islets_MoveIceberg(ACTOR_FIRST_ICEBERG + 1, 0, -1);
      Islets_MoveIceberg(ACTOR_FIRST_ICEBERG + 2, 0, 1);
      Islets_MoveIceberg(ACTOR_FIRST_ICEBERG + 3, 0, 1);
      Islets_MoveIceberg(ACTOR_FIRST_ICEBERG + 4, -1, 0);
      Islets_MoveIceberg(ACTOR_FIRST_ICEBERG + 5, 0, 1);
      Islets_MoveIceberg(ACTOR_FIRST_ICEBERG + 6, 1, 0);
      Islets_MoveIceberg(ACTOR_LAST_ICEBERG, 0, -1);
      break;
    case 100:
    case 220:
      for (i = ACTOR_FIRST_ICEBERG; i <= ACTOR_LAST_ICEBERG; i++)
        Actor_SetAnimation(i, 1);
      break;
    case 130:
      for (i = ACTOR_FIRST_ICEBERG; i <= ACTOR_LAST_ICEBERG; i++) {
        Actor_SetAnimation(i, 2);
        Islets_MoveIceberg(i, 0, 0);
      }
      break;
    }
    if (++*frame == 260)
      *frame = 0;
  } while (--*owed != 0);
}

extern u32 gIslets_PengusChasesPengulinaScript[];

/// Sets up E Tundaria Islet: the icebergs' drift, the log floating in the
/// channel once it is pushed, and Pengus chasing Pengulina round the snow once
/// the Pretty Stone is given.
void Islets_InitializeEastTundaria(void) {
  struct Actor *actor;
  struct Actor *other;
  u16 *state;
  u16 *next;
  s32 i;

  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  if (gMainRoutineWork.entrance ==
      EAST_TUNDARIA_ISLET_FROM_MIKASALLA_ROOMS_HOUSES)
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  i = ACTOR_FIRST_ICEBERG;
  do {
    Islets_StartIceberg(i++);
  } while (i <= ACTOR_LAST_ICEBERG);
  state = gSceneState;
  next = &gSceneState[1];
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    *state = 0;
    *next = 0;
  }
  FieldTask_AddFrame(Islets_DriftIcebergs, TASK_PASS_FRAME);
  if (Event_GetFlag(FLAG_LOG_PUSHED_INTO_WATER)) {
    actor = Actor_Get(ACTOR_LOG);
    Actor_SetPosition(ACTOR_LOG, 176, 168);
    Actor_SetAnimation(ACTOR_LOG, 4);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->motionFlags = 0;
    actor->y.fixed = toFIXED(-8);
    actor->groundHeight = toFIXED(-8);
    MapGrid_CopyCellAttributes(9, 1, 4, 1, 9, 10);
    MapGrid_CopyCellAttributes(9, 64, 4, 1, 9, 74);
  } else {
    PushTarget_FillFootprint(ACTOR_LOG);
  }
  if (!Event_GetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
    FieldActor_WatchAndSetScript(ACTOR_PENGUS, ACTOR_PENGULINA);
    Actor_SetAnimation(ACTOR_PENGULINA, ANIM_STILL);
    Actor_Get(ACTOR_PENGULINA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  } else if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    other = Actor_Get(ACTOR_PENGUS);
    other->facing = FACING_SOUTH_SOUTHWEST;
    other->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
    {
      Actor_SetPositionAndFacing(ACTOR_PENGULINA, toFIXED(234), toFIXED(204),
                                 FACING_SOUTH_SOUTHEAST);
    }
    Actor_Get(ACTOR_PENGULINA)->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
    Actor_SetSpeed(ACTOR_PENGUS, 2);
    Actor_SetSpeed(ACTOR_PENGULINA, 1.6);
    Actor_SetScript(ACTOR_PENGUS, gIslets_PengusChasesPengulinaScript);
    Actor_SetScript(ACTOR_PENGULINA, gIslets_PengusChasesPengulinaScript);
    Event_SetFlag(FLAG_PENGUS_CHASING_PENGULINA);
  }
}

/// Sets up N Osenia Islet: the cow, and once it has traded, the neckerchief
/// tied in the yard's tree and the cow standing below it; and the hidden blue
/// orb.
void Islets_InitializeNorthOsenia(void) {
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == NORTH_OSENIA_ISLET_FROM_DAILA_ROOMS_HOUSES)
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_COW), 0);
  Actor_SetAnimation(ACTOR_COW, 4);
  if (Event_GetFlag(FLAG_COW_TRADED)) {
    spawned = FieldActor_Create(SPRITE_UNSEEN_MARKER, toFIXED(104), toFIXED(24),
                                toFIXED(160));
    if (spawned != 0) {
      spawned->motionFlags = 0;
      FieldActor_SetItemIcon(spawned, ITEM_RED_CLOTH);
      Actor_SetSpriteFlags(spawned, 0);
      spawned->retainSpriteCache = 1;
    }
    Actor_SetPosition(ACTOR_COW, 104, 204);
  }
  FieldActor_SetSpritePriority(ACTOR_BLUE_ORB, 1);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BLUE_ORB), SPRITE_COLOR_HIDDEN);
  Actor_Get(ACTOR_BLUE_ORB)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
}

/// Sets up SE Angara Islet: the bird, the crates (stacked in one cell once both
/// have been brought down there) and the puddle (an ice pillar once frozen).
void Islets_InitializeSoutheastAngara(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_Get(ACTOR_BIRD);
  struct Actor *last;
  s32 set;

  map->layers[2].speedX = 0;
  map->layers[2].speedY = 0;
  if (gMainRoutineWork.entrance == SOUTHEAST_ANGARA_ISLET_FROM_CHAMPA_ROOMS)
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    actor->motionFlags = 0;
    FieldActor_SetSpritePriority(ACTOR_BIRD, 1);
  }
  Actor_SetPosition(ACTOR_UNSEEN_MARKER, 0, 0);
  Actor_DisableById(ACTOR_CRATE_A);
  Actor_DisableById(ACTOR_CRATE_B);
  set = Event_GetFlag(FLAG_CRATES_IN_ONE_CELL);
  if (set) {
    struct Actor *first = Actor_Get(ACTOR_CRATE_A);
    struct Actor *second = Actor_Get(ACTOR_CRATE_B);

    first->motionFlags = 0;
    second->motionFlags = 0;
    first->riseCounter = 1;
    second->riseCounter = 1;
    MapGrid_CopyCellAttributes(7, 3, 1, 1, 7, 29);
  } else {
    struct Actor *first = Actor_Get(ACTOR_CRATE_A);
    struct Actor *second = Actor_Get(ACTOR_CRATE_B);

    first->motionFlags = 0;
    second->motionFlags = 0;
    Islets_SetCellCollision(first->x.fixed, first->z.fixed, 3,
                            CELL_BEHAVIOR_WALL);
    Islets_SetCellCollision(second->x.fixed, second->z.fixed, 3,
                            CELL_BEHAVIOR_WALL);
  }
  Actor_DisableById(ACTOR_PUDDLE);
  last = Actor_Get(ACTOR_PUDDLE);
  last->riseCounter = 1;
  last->motionFlags = 0;
  Islets_SetCellCollision(last->x.fixed, last->z.fixed, 1, CELL_BEHAVIOR_WALL);
  if (Event_GetFlag(FLAG_SOUTHEAST_ANGARA_ISLET_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(5, 2, 1, 2, 5, 28);
}

/// The Sea of Time Islet's surf, two palette cycles of the same four colours
/// from SURF_FIRST_COLOR: the foam along the shore in the shallows, and the
/// surf round the islets and their rocks out at sea, each turned one place
/// every sixth frame.
enum {
  SHALLOWS_SURF_PALETTE = 3,
  SEA_SURF_PALETTE = 4,
  SURF_COLORS = 4,
  SURF_CYCLE_DELAY = 5
};

/// Sets up the Sea of Time Islet: the surf's palette cycles, Lonesome George
/// and the Li'l Turtle once given, and the ride's arrival or return by
/// entrance.
void Islets_InitializeSeaOfTime(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 done;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Map_SetUpperLevelWading(map);
  if (gMainRoutineWork.entrance == SEA_OF_TIME_ISLET_FROM_APOJII_ISLANDS_SHOPS)
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  PaletteCycle_Initialize();
  PaletteCycle_Add(SHALLOWS_SURF_PALETTE, SURF_FIRST_COLOR, SURF_CYCLE_DELAY,
                   SURF_COLORS);
  PaletteCycle_Add(SEA_SURF_PALETTE, SURF_FIRST_COLOR, SURF_CYCLE_DELAY,
                   SURF_COLORS);
  if (Event_GetFlag(FLAG_GEORGE_HAS_THE_TURTLE) &&
      (done = Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) == 0) {
    Actor_SetPosition(ACTOR_LIL_TURTLE, 280, 468);
    Actor_Get(ACTOR_LIL_TURTLE)->motionFlags = done;
    {
      FIXED x = toFIXED(296);

      Actor_SetPositionFX(ACTOR_LONESOME_GEORGE, x, toFIXED(452));
    }
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_LONESOME_GEORGE), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_DECK_BLOCK_A), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_DECK_BLOCK_B), 0);
  FieldTask_AddFrame(Islets_SetDeckBlocksByHeight, TASK_PASS_FRAME);
  // At the secret spot, by the ride or from the Islet Cave.
  if ((u16)(gMainRoutineWork.entrance - ENTRANCE_AT_SECRET_SPOT) <= 1) {
    Event_SetFlag(FLAG_AT_SECRET_SPOT);
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
      {
        Actor_SetPositionAndFacing(ACTOR_LONESOME_GEORGE, toFIXED(152),
                                   toFIXED(768), FACING_SOUTH);
      }
      Actor_SetPosition(ACTOR_LIL_TURTLE, 176, 777);
      System_Wait(1);
    }
  }
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == ENTRANCE_BACK_FROM_SECRET_SPOT) {
      Islets_ArriveBackAtIsland();
    } else if (entrance == ENTRANCE_AT_SECRET_SPOT) {
      Islets_ArriveAtSecretSpot();
#if TLA_PATCH_1
    } else if (entrance != SEA_OF_TIME_ISLET_FROM_ISLET_CAVE_HALLS) {
#else
    } else {
#endif
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      PaletteCycle_Start();
    }
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    PaletteCycle_Start();
  }
  Event_End();
}

/// Nothing to prepare before the map is built. Returns 0.
s32 Scene_PrepareMap(void) { return 0; }

/// Allocates the palette cycling queue (HEAP_SLOT_PALETTE_CYCLES) with its four
/// entries cleared.
void PaletteCycle_Initialize(void) {
  struct PaletteCycleWork *paletteCycles;
  struct PaletteCycle *cycle;
  u16 cycleIndex;
  u16 colorIndex;

  paletteCycles = HeapSlot_AllocEwram(HEAP_SLOT_PALETTE_CYCLES,
                                      sizeof(struct PaletteCycleWork));
  cycle = paletteCycles->entries;
  for (cycleIndex = 0; cycleIndex != ARRAY_COUNT(paletteCycles->entries);
       cycleIndex++) {
    cycle->source = 0;
    cycle->position = 0;
    cycle->timer = 0;
    cycle->delay = 0;
    cycle->count = 0;
    for (colorIndex = 0; colorIndex != ARRAY_COUNT(cycle->colors);
         colorIndex++) {
      cycle->colors[colorIndex] = 0;
    }
    cycle++;
  }
  paletteCycles->count = 0;
}

#include "../field/palqueue_add.c"

#include "text/msg_ids.h"

/// Starts the palette cycles' task, as the surf moves while the party is on the
/// Sea of Time Islet.
void PaletteCycle_Start(void) {
  Task_AddFrame(&PaletteCycle_Step, TASK_PASS_FRAME);
  return;
}

/// Stops the palette cycles' task; their queue stays for the next start.
void PaletteCycle_Stop(void) {
  FieldTask_Remove(PaletteCycle_Step);
  return;
}

/// The palette cycles' task: each queued cycle whose timer has run out rotates
/// its run of colours one place further, unless Psynergy or a menu is open.
void PaletteCycle_Step(void) {
  if (gFieldWork->psynergy == 0 && gFieldWork->fieldMenu == 0 &&
      gFieldWork->pauseMenu == 0) {
    struct PaletteCycleWork *work = gPaletteWork;
    u8 i;
    u16 count = work->count & 3;
    u16 buf[16];

    for (i = 0; i < count; i++) {
      struct PaletteCycle *cycle = &work->entries[i];
      if (cycle->timer == 0) {
        u16 start = cycle->position;
        u16 len = cycle->count;
        void *dest = cycle->source;
        u16 *src = cycle->colors;
        u8 j;
        for (j = len - start; j < len; j++)
          buf[j] = *src++;
        for (j = 0; j < len - start; j++)
          buf[j] = *src++;
        Dma_Set(buf, dest, DMA_COPY_HALFWORDS(len), REG_DMA3);
        start++;
        if (start >= len)
          start = 0;
        cycle->position = start;
        cycle->timer = cycle->delay;
      } else {
        cycle->timer--;
      }
    }
  }
}

/// When Felix falls into the sea among E Tundaria Islet's drifting icebergs, he
/// sinks out of sight, and the islet starts again on its south shore, where the
/// party lands from the World Map. The icebergs' drift (Islets_DriftIcebergs)
/// raises this the moment his height falls below -12 pixels, that is, once he
/// is down at the sea, whose cells lie at -16, off the icebergs and the shore;
/// the icebergs then hold still.
void Islets_SinkInSea(void) {
  struct Actor *leader;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_DROP_THROUGH_HOLE);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  Actor_SetSpriteFlags(leader, 0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 18);
  FieldEvent_Wait(40);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(EXIT_AFTER_SINKING);
  return;
}

/// Walking east off the sailor's boat at West Indra Islet's piers (cell
/// (5, 45)), Felix jumps the gap onto the pier. On E Tundaria Islet, walking
/// south off the shore west of the healer (cell (16, 16)), he jumps out onto
/// the iceberg moored below, where there is one to land on.
void Islets_JumpForward(void) {
  // Pushing south off the shore at cell (16, 16) for some nine seconds, a
  // whole round of the icebergs' drift, Felix sees them drift out and back
  // below but none comes to rest under the shore, and he does not jump.
  // XXX: the jump onto an iceberg from E Tundaria Islet's shore is still read
  // from the map, not seen. Cell (16, 16) is the only jump cell (behaviour 11)
  // on the south shore's cells (2, 12) to (22, 24), and pushing south on it
  // again (down:600) ended at once with nothing run. A recording of the push at
  // the moment an iceberg lies two cells south, or with the icebergs held still
  // by Reveal where one lies below, would settle it.
  Field_TryJumpForward();
  return;
}

extern s8 gPushFootprints[][4];

#include "shared/push_target_fill_footprint.h"

#include "../field/map_cell_fill.c"

extern s32 gPushLeaderSteps[];

/// Whether an actor at a position would collide with the map
/// (MapGrid_CheckCollision).
static __inline__ s32 PushTarget_CheckCollision(struct Actor *actor,
                                                struct Vector3 *cell) {
  return MapGrid_CheckCollision(actor, cell);
}

#include "shared/push_target_find_slide.h"

#include "shared/push_target_find_ahead.h"

void Actor_StopAtPushTarget(struct Actor *actor);

#include "shared/push_target_push.h"

#include "shared/actor_stop_at_push_target.h"

#undef STEP
#undef HAND_BACK
#undef ISLETS_RIDE_TRANSITION
#undef SPAWN
#include "text/msg_ids.h"

void Islets_TalkToPengulina(void);
void Islets_TalkToHealer(void);
void Islets_TalkToTreasureHuntersPartner(void);
void Islets_TalkToBird(void);
void Islets_TradeWithCow(void);
void Islets_PengusGivesPrettyStone(void);
void Islets_BirdReturnsToNest(void);
void Islets_FinishPushingCrate(void);
void Islets_EnterDoor(void);

struct ActionScript gIslets_SpinScript = {{
    SC_ADD(SW_FACING, FACING_STEP),
    SC_WAIT(1),
    SC_LOOP(16, SC_TOP),
    SC_END,
}};

s32 gIslets_EnterDoorScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Z, toFIXED(-0.5)),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_END,
};

#include "shared/push_kinds.c"

#include "shared/push_leader_steps.c"

u32 gIslets_EastTundariaWalkScript[] = {
#if !TLA_PATCH_1
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
    SC_ACCELERATION(0.4),
#else
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.4),
#endif
    SC_SPEED(0.4),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(376, 0, 216),
    SC_SYNC,
    SC_MOVE(392, 0, 224),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(80),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(376, 0, 216),
    SC_SYNC,
    SC_MOVE(376, 0, 184),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gIslets_BirdCirclesScript[] = {
    SC_ANIMATION(4),
    SC_SPEED(0.8),
    SC_ACCELERATION(0.2),
    SC_SET(SW_Y, toFIXED(64)),
    SC_LABEL(1),
    SC_MOVE(352, 64, 336),
    SC_SYNC,
    SC_MOVE(368, 64, 336),
    SC_SYNC,
    SC_MOVE(376, 64, 328),
    SC_SYNC,
    SC_MOVE(376, 64, 312),
    SC_SYNC,
    SC_MOVE(368, 64, 304),
    SC_SYNC,
    SC_MOVE(352, 64, 304),
    SC_SYNC,
    SC_MOVE(344, 64, 312),
    SC_SYNC,
    SC_MOVE(344, 64, 328),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

/// A loop round the snow that Pengus and Pengulina both waddle, Pengus the
/// faster, so that he chases her.
u32 gIslets_PengusChasesPengulinaScript[] = {
    SC_ANIMATION(2), SC_MOVE(234, 0, 240),        SC_SYNC, SC_MOVE(276, 0, 222),
    SC_SYNC,         SC_MOVE(308, 0, 256),        SC_SYNC, SC_MOVE(252, 0, 256),
    SC_SYNC,         SC_MOVE(264, 0, 208),        SC_SYNC, SC_MOVE(232, 0, 184),
    SC_SYNC,         SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gIslets_DogHopsFacingWestScript[] = {
    SC_ANIMATION(3),
    SC_SE(SE_JUMP),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(0, 0, -16),
    SC_SYNC,
    SC_SE(SE_JUMP),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(-32, 0, 0),
    SC_SYNC,
    SC_SE(SE_JUMP),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(0, 0, 32),
    SC_SYNC,
    SC_SE(SE_JUMP),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(32, 0, 0),
    SC_SYNC,
    SC_SE(SE_JUMP),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE(280, 0, 856),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

s32 gIslets_DogHopsFacingEastScript[] = {
    SC_ANIMATION(3),
    SC_SE(SE_JUMP),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(0, 0, 16),
    SC_SYNC,
    SC_SE(SE_JUMP),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(32, 0, 0),
    SC_SYNC,
    SC_SE(SE_JUMP),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(0, 0, -32),
    SC_SYNC,
    SC_SE(SE_JUMP),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(-32, 0, 0),
    SC_SYNC,
    SC_SE(SE_JUMP),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE(280, 0, 856),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gIslets_WadingSplashScript[] = {
    SC_WAIT(15),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

u32 gIslets_GeorgeSwimsToSecretSpotScript[] = {
    SC_MOVE(248, 0, 686), SC_SYNC, SC_MOVE(248, 0, 768),       SC_SYNC,
    SC_MOVE(152, 0, 768), SC_SYNC, SC_SET(SW_RISE_ENABLED, 1), SC_END,
};

u32 gIslets_LilTurtleSwimsToSecretSpotScript[] = {
    SC_ANIMATION(2), SC_MOVE(248, 0, 686), SC_SYNC, SC_MOVE(248, 0, 768),
    SC_SYNC,         SC_MOVE(176, 0, 777), SC_SYNC, SC_ANIMATION(1),
    SC_END,
};

u32 gIslets_GeorgeSwimsBackScript[] = {
    SC_MOVE(248, 0, 768), SC_SYNC, SC_MOVE(248, 0, 686),       SC_SYNC,
    SC_MOVE(248, 0, 584), SC_SYNC, SC_SET(SW_RISE_ENABLED, 1), SC_END,
};

u32 gIslets_LilTurtleSwimsBackScript[] = {
    SC_ANIMATION(2), SC_MOVE(248, 0, 768), SC_SYNC, SC_MOVE(248, 0, 686),
    SC_SYNC,         SC_MOVE(240, 0, 600), SC_SYNC, SC_ANIMATION(1),
    SC_END,
};

/// The sailor's other son pacing West Indra Islet's pier, east and back.
u32 gIslets_PacingSonScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(184, 0, 936),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
#ifdef BUGFIX
    SC_WAIT(80),
#else
    // BUG: SC_SYNC where the west end's SC_WAIT(80) was meant; the walker
    // skips the bare 80, a word above 63 (field_actor_motion.c), so the boy
    // turns back at once here.
    SC_SYNC,
    80,
#endif
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(88, 0, 936),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(80),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u16 gIslets_DoorFrames[] = {
    92, 10, 1, 2, 6, 92, 8, 1, 2, 6, -1,
};

u16 gIslets_NoDoorFrames[] = {
    -1,
};

struct IsletsDoor gIslets_Doors[] = {
    {gIslets_NoDoorFrames, 64, 64},
    {gIslets_DoorFrames, 82, 10},
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 208, 0, 800, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
u32 gSceneExits[] = {
    SCENE_EXITS(WestIndraIslet),
    SCENE_EXIT(WEST_INDRA_ISLET_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_WEST_INDRA_ISLET),
    SCENE_EXIT(WEST_INDRA_ISLET_TO_CHAMPA_ROOMS, ChampaRooms, CHAMPA_ROOMS_FROM_WEST_INDRA_ISLET),
    SCENE_EXITS(EastTundariaIslet),
    SCENE_EXIT(EAST_TUNDARIA_ISLET_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_EAST_TUNDARIA_ISLET),
    SCENE_EXIT(EAST_TUNDARIA_ISLET_TO_MIKASALLA_ROOMS_HOUSES, MikasallaRoomsHouses, MIKASALLA_ROOMS_HOUSES_FROM_EAST_TUNDARIA_ISLET),
    SCENE_EXIT(EXIT_AFTER_SINKING, EastTundariaIslet, EAST_TUNDARIA_ISLET_FROM_WORLD_MAP),
    SCENE_EXITS(NorthOseniaIslet),
    SCENE_EXIT(NORTH_OSENIA_ISLET_TO_WORLD_MAP_A, WorldMap, WORLD_MAP_FROM_NORTH_OSENIA_ISLET_A),
    SCENE_EXIT(NORTH_OSENIA_ISLET_TO_WORLD_MAP_B, WorldMap, WORLD_MAP_FROM_NORTH_OSENIA_ISLET_B),
    SCENE_EXIT(NORTH_OSENIA_ISLET_TO_DAILA_ROOMS_HOUSES, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_NORTH_OSENIA_ISLET),
    SCENE_EXIT(NORTH_OSENIA_ISLET_TO_WORLD_MAP_C, WorldMap, WORLD_MAP_FROM_NORTH_OSENIA_ISLET_C),
    SCENE_EXITS(SoutheastAngaraIslet),
    SCENE_EXIT(SOUTHEAST_ANGARA_ISLET_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SOUTHEAST_ANGARA_ISLET),
    SCENE_EXIT(SOUTHEAST_ANGARA_ISLET_TO_CHAMPA_ROOMS, ChampaRooms, CHAMPA_ROOMS_FROM_SOUTHEAST_ANGARA_ISLET),
    SCENE_EXITS(SeaOfTimeIslet),
    SCENE_EXIT(SEA_OF_TIME_ISLET_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SEA_OF_TIME_ISLET),
    SCENE_EXIT(SEA_OF_TIME_ISLET_TO_APOJII_ISLANDS_SHOPS, ApojiiIslandsShops, APOJII_ISLANDS_SHOPS_FROM_SEA_OF_TIME_ISLET),
    SCENE_EXIT(EXIT_TO_SECRET_SPOT, SeaOfTimeIslet, ENTRANCE_AT_SECRET_SPOT),
    SCENE_EXIT(EXIT_BACK_FROM_SECRET_SPOT, SeaOfTimeIslet, ENTRANCE_BACK_FROM_SECRET_SPOT),
    SCENE_EXIT(SEA_OF_TIME_ISLET_TO_ISLET_CAVE_HALLS, IsletCaveHalls, ISLET_CAVE_HALLS_FROM_SEA_OF_TIME_ISLET),
    SCENE_EXITS_END,
};

struct ActorPlacement gIslets_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_WestIndraActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 744, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_IN_BLUE_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 920, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_IN_BLUE_SHIRT, CONDITION_ALWAYS, (s32)gIslets_PacingSonScript, 88, 0, 936, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 856, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_EastTundariaActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, (s32)gIslets_EastTundariaWalkScript, 376, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PENGUIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 176, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PENGUIN_WITH_BOW, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 280, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_NorthOseniaActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 248, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 184, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COW, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 132, 0, 176, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 326, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_SoutheastAngaraActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 296, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 280, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ORANGE_BIRD, CONDITION_ALWAYS, (s32)gIslets_BirdCirclesScript, 360, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_SeaOfTimeActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 168, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_TURTLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TURTLE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 202, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 266, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gIslets_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_WestIndraTriggers[] = {
    {TRIGGER_EXIT, WEST_INDRA_ISLET_TO_WORLD_MAP, CONDITION_ALWAYS,
     WEST_INDRA_ISLET_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), WEST_INDRA_ISLET_TO_CHAMPA_ROOMS,
     CONDITION_ALWAYS, EVENT(Islets_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), STEP_TRIGGER_JUMP_TO_PIER,
     CONDITION_ALWAYS, EVENT(Islets_JumpForward)},
    {TRIGGER_TALK, ACTOR_SAILOR, CONDITION_ALWAYS, MSGNO(MsgEverSinceThe)},
    {TRIGGER_TALK, ACTOR_SAILORS_SON, CONDITION_ALWAYS,
     EVENT(Islets_TalkToSailorsSon)},
    {TRIGGER_TALK, ACTOR_SAILORS_OTHER_SON, CONDITION_ALWAYS,
     MSGNO(MsgYouReLucky)},
    {TRIGGER_TALK, ACTOR_DOG, CONDITION_ALWAYS, MSGNO(MsgArffArff)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR, CONDITION_ALWAYS,
     MSGNO(MsgTheWaterwaysNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_SON, CONDITION_ALWAYS,
     MSGNO(MsgICanTWaitUntil)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_OTHER_SON,
     CONDITION_ALWAYS, MSGNO(MsgICanTWaitUntilThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOG, CONDITION_ALWAYS,
     EVENT(Islets_TradeWithDog)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_EastTundariaTriggers[] = {
    {TRIGGER_EXIT, EAST_TUNDARIA_ISLET_TO_WORLD_MAP, CONDITION_ALWAYS,
     EAST_TUNDARIA_ISLET_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     EAST_TUNDARIA_ISLET_TO_MIKASALLA_ROOMS_HOUSES, CONDITION_ALWAYS,
     EVENT(Islets_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), STEP_TRIGGER_JUMP_OFF_LEDGE,
     CONDITION_ALWAYS, EVENT(Islets_JumpDownLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), STEP_TRIGGER_JUMP_TO_ICEBERG,
     CONDITION_ALWAYS, EVENT(Islets_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), STEP_TRIGGER_PUSH_LOG,
     FLAG_LOG_PUSHED_INTO_WATER, EVENT(Islets_PushLogIntoWater)},
    {TRIGGER_STEP, STEP_TRIGGER_BY_PENGULINA, FLAG_PENGUS_GAVE_PRETTY_STONE,
     EVENT(Islets_PengusGivesPrettyStone)},
    {TRIGGER_STEP, STEP_TRIGGER_ICEBERG_BY_LADDER, CONDITION_ALWAYS,
     EVENT(Islets_StepUpOntoShore)},
    {TRIGGER_STEP, STEP_TRIGGER_SHORE_BY_LADDER, CONDITION_ALWAYS,
     EVENT(Islets_StepDownOntoIceberg)},
    {TRIGGER_TALK, ACTOR_MAN_IN_GREEN_VEST, CONDITION_ALWAYS,
     MSGNO(MsgMyMomSays)},
    {TRIGGER_TALK, ACTOR_PENGUS, CONDITION_ALWAYS, EVENT(Islets_TalkToPengus)},
    {TRIGGER_TALK, ACTOR_PENGULINA, CONDITION_ALWAYS,
     EVENT(Islets_TalkToPengulina)},
    {TRIGGER_RAISED, TRIGGER_SINK_IN_SEA, CONDITION_ALWAYS,
     EVENT(Islets_SinkInSea)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_IN_GREEN_VEST,
     CONDITION_ALWAYS, MSGNO(MsgWhoInWeyard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PENGUS, CONDITION_ALWAYS,
     EVENT(Islets_MindReadPengus)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PENGULINA, CONDITION_ALWAYS,
     EVENT(Islets_MindReadPengulina)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Islets_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
     MSGNO(MsgMayTheseTravelers)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Islets_StartReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Islets_EndReveal)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_NorthOseniaTriggers[] = {
    {TRIGGER_EXIT, NORTH_OSENIA_ISLET_TO_WORLD_MAP_A, CONDITION_ALWAYS,
     NORTH_OSENIA_ISLET_TO_WORLD_MAP_A},
    {TRIGGER_EXIT, NORTH_OSENIA_ISLET_TO_WORLD_MAP_B, CONDITION_ALWAYS,
     NORTH_OSENIA_ISLET_TO_WORLD_MAP_B},
    {TRIGGER_EXIT, NORTH_OSENIA_ISLET_TO_DAILA_ROOMS_HOUSES, CONDITION_ALWAYS,
     NORTH_OSENIA_ISLET_TO_DAILA_ROOMS_HOUSES},
    {TRIGGER_EXIT, NORTH_OSENIA_ISLET_TO_WORLD_MAP_C, CONDITION_ALWAYS,
     NORTH_OSENIA_ISLET_TO_WORLD_MAP_C},
    {TRIGGER_TALK, ACTOR_SON_ON_THE_SEA_OF_FOG, CONDITION_ALWAYS,
     MSGNO(MsgIJustWant)},
    {TRIGGER_TALK, ACTOR_FOG_DREAMER, CONDITION_ALWAYS, MSGNO(MsgTheSeaOf)},
    {TRIGGER_TALK, ACTOR_COW, CONDITION_ALWAYS, MSGNO(MsgMooMoo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SON_ON_THE_SEA_OF_FOG,
     CONDITION_ALWAYS, MSGNO(MsgDadAndGrandpa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FOG_DREAMER, CONDITION_ALWAYS,
     MSGNO(MsgIRememberOnce)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COW, CONDITION_ALWAYS,
     EVENT(Islets_TradeWithCow)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(Islets_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(Islets_RiseFromSand)},
#if TLA_PATCH_1 /* From patch 1. */
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     WEEDS_IN_COW_YARD, CONDITION_ALWAYS, EVENT(Islets_ClearFirstPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     WEEDS_BY_HOUSE, CONDITION_ALWAYS, EVENT(Islets_ClearSecondPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     WEED_SOUTH_OF_YARD, CONDITION_ALWAYS, EVENT(Islets_ClearThirdPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     WEEDS_ON_TILLED_FIELD, CONDITION_ALWAYS, EVENT(Islets_ClearFourthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     WEEDS_EAST_OF_FIELD, CONDITION_ALWAYS, EVENT(Islets_ClearFifthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     WEED_BY_FIELD_FENCE, CONDITION_ALWAYS, EVENT(Islets_ClearSixthPatch)},
#endif
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_SoutheastAngaraTriggers[] = {
    {TRIGGER_EXIT, SOUTHEAST_ANGARA_ISLET_TO_WORLD_MAP, CONDITION_ALWAYS,
     SOUTHEAST_ANGARA_ISLET_TO_WORLD_MAP},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK |
         TRIGGER_FLAG_FACING | FACING_NORTH,
     SOUTHEAST_ANGARA_ISLET_TO_CHAMPA_ROOMS, CONDITION_ALWAYS,
     SOUTHEAST_ANGARA_ISLET_TO_CHAMPA_ROOMS},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), STEP_TRIGGER_PUSH_CRATE_ON,
     FLAG_CRATES_IN_ONE_CELL, EVENT(Islets_PushCrateOffLedge)},
    {TRIGGER_PUSH, 0, FLAG_CRATES_IN_ONE_CELL, EVENT(Islets_StartPushingCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_CRATE_A, FLAG_CRATES_IN_ONE_CELL, EVENT(Islets_StartPushingCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_CRATE_B, FLAG_CRATES_IN_ONE_CELL, EVENT(Islets_StartPushingCrate)},
    {TRIGGER_PUSHED, 0, FLAG_CRATES_IN_ONE_CELL,
     EVENT(Islets_FinishPushingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRATE_A, FLAG_CRATES_IN_ONE_CELL,
     EVENT(Islets_FinishPushingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRATE_B, FLAG_CRATES_IN_ONE_CELL,
     EVENT(Islets_FinishPushingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE,
     FLAG_SOUTHEAST_ANGARA_ISLET_PUDDLE_FROZEN, EVENT(Islets_FreezePuddle)},
    {TRIGGER_TALK, ACTOR_TREASURE_HUNTER, CONDITION_ALWAYS,
     EVENT(Islets_TalkToTreasureHunters)},
    {TRIGGER_TALK, ACTOR_TREASURE_HUNTERS_PARTNER, CONDITION_ALWAYS,
     EVENT(Islets_TalkToTreasureHuntersPartner)},
    {TRIGGER_TALK, ACTOR_BIRD, CONDITION_ALWAYS, EVENT(Islets_TalkToBird)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREASURE_HUNTER,
     CONDITION_ALWAYS, MSGNO(MsgIfWeRe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREASURE_HUNTERS_PARTNER,
     CONDITION_ALWAYS, MSGNO(MsgSureThereAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BIRD, CONDITION_ALWAYS,
     EVENT(Islets_TradeWithBird)},
    {TRIGGER_STEP, STEP_TRIGGER_NEAR_NEST, FLAG_BIRD_ON_NEST,
     EVENT(Islets_BirdReturnsToNest)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_SeaOfTimeTriggers[] = {
    {TRIGGER_STEP, SEA_OF_TIME_ISLET_TO_WORLD_MAP, CONDITION_ALWAYS,
     EVENT(Islets_LeaveByStep)},
    {TRIGGER_STEP, SEA_OF_TIME_ISLET_TO_APOJII_ISLANDS_SHOPS, CONDITION_ALWAYS,
     EVENT(Islets_LeaveByStep)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
     SEA_OF_TIME_ISLET_TO_ISLET_CAVE_HALLS, CONDITION_ALWAYS,
     SEA_OF_TIME_ISLET_TO_ISLET_CAVE_HALLS},
    {TRIGGER_TALK, ACTOR_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgYouUsedTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, CONDITION_ALWAYS,
     MSGNO(MsgIRememberHow)},
    {TRIGGER_TALK, ACTOR_LONESOME_GEORGE, FLAG_GEORGE_HAS_THE_TURTLE,
     MSGNO(MsgGeorgeSilence)},
    {TRIGGER_TALK, ACTOR_LONESOME_GEORGE, CONDITION_ALWAYS, MSGNO(MsgYup)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LONESOME_GEORGE,
     FLAG_AT_SECRET_SPOT, EVENT(Islets_GiveTurtle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LONESOME_GEORGE,
     CONDITION_ALWAYS, EVENT(Islets_RideBackToIsland)},
    {TRIGGER_TALK, ACTOR_DECK_BLOCK_A, CONDITION_ALWAYS,
     EVENT(Islets_OpenFieldMenu)},
    {TRIGGER_TALK, ACTOR_DECK_BLOCK_B, CONDITION_ALWAYS,
     EVENT(Islets_OpenFieldMenu)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(Islets_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(Islets_RiseFromSand)},
    {TRIGGER_TABLE_END},
};
