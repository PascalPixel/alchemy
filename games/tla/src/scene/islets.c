/// @file
/// The five islets of the Eastern Sea, one scene each in this overlay, and the
/// trade from one to the next: West Indra Islet (SceneId_IsletsDog), whose dog
/// trades the Li'l Turtle for Milk (Islets_TradeWithDog); E Tundaria Islet
/// (IsletsPrettyStone), where the log pushed into the water brings Pengulina
/// back to Pengus for a Pretty Stone (Islets_PengusGivesPrettyStone); N Osenia
/// Islet (IsletsCow), whose cow trades Milk for the Red Cloth
/// (Islets_TradeWithCow); SE Angara Islet (IsletsPier), whose bird trades the
/// Red Cloth for the Pretty Stone (Islets_TradeWithBird); and the Sea of Time
/// Islet (IsletsTurtle), where Lonesome George takes the Li'l Turtle and
/// carries the party to his secret spot (Islets_GiveTurtle,
/// Islets_ArriveAtSecretSpot).
#include "system/task.h"
#include "scene/event_layer.h"
#include "flags.h"
#include "item_ids.h"

/// The trades along the islets, each set once done: the log Pushed into the
/// water, the dog's, Pengus's Pretty Stone, the cow's, the bird's, and the Li'l
/// Turtle given to Lonesome George.
#define FLAG_LOG_PUSHED_INTO_WATER 0xa2c
#define FLAG_DOG_TRADED 0xaa1
#define FLAG_PENGUS_GAVE_PRETTY_STONE 0xaa2
#define FLAG_COW_TRADED 0xaa3
#define FLAG_BIRD_TRADED 0xaa4
#define FLAG_GEORGE_HAS_THE_TURTLE 0xaa5
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
void Leader_SetLayer(void);
void Islets_StepOntoGround(void);
void Islets_StartPushingCrate(void);
void Islets_FreezePuddle(void);
void Islets_StepUpOntoIceberg(void);

void Islets_StepDownFromIceberg(void);

void Islets_StartReveal(void);
void Islets_EndReveal(void);

void Islets_ClearFirstPatch(void);
void Islets_ClearSecondPatch(void);
void Islets_ClearFourthPatch(void);
void Islets_ClearSixthPatch(void);

void Islets_OpenDoorAndEnter(void);
void Islets_LeaveByStep(void);

void Islets_DropThroughHole(void);
void Islets_JumpForward(void);

extern u32 gIslets_DogWanderScript[];
extern u32 gIslets_GullScript[];
extern u32 gIslets_PierPaceScript[];
extern u16 gIslets_DoorFrames[];
extern u16 gIslets_NoDoorFrames[];

void Islets_ShimmerWater(void);

void Field_TryJumpForward();

void PushedActor_FillGridAttributeRectangle(s32 layer, s32 x, s32 z, u32 width,
                                            u32 height, s32 value);
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *number, s32 *kind);

void Islets_SetLedgeBlocksByHeight(void);

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

/// The islets' people and animals, room by room, numbered by the rows of
/// each room's placements.
/// The dog's islet: the sailor's son, and the dog that trades.
/// The pacing sailor walks the dock beside him.
enum { ACTOR_SAILORS_SON = 9, ACTOR_PACING_SAILOR = 10, ACTOR_DOG = 11 };
/// The icebergs' islet: Pengus and Pengulina, the log pushed into the water
/// and the healer.
enum {
  ACTOR_PENGUS = 9,
  ACTOR_LOG = 10,
  /// The eight icebergs that drift across the water (Islets_DriftIcebergs).
  ACTOR_FIRST_ICEBERG = 11,
  ACTOR_LAST_ICEBERG = 18,
  ACTOR_PENGULINA = 19,
  ACTOR_PRETTY_STONE_ICON = 20, ///< shows the Pretty Stone over the leader
  ACTOR_HEALER = 21
};
/// The cow's islet: the cow, and a blue orb lit in sprite colour 15.
enum { ACTOR_COW = 10, ACTOR_BLUE_ORB = 11 };
/// The pier: the treasure hunter and the sailor, the bird that trades, and the
/// two crates pushed along it.
enum {
  ACTOR_TREASURE_HUNTER = 8,
  ACTOR_PIER_SAILOR = 9,
  ACTOR_BIRD = 10,
  ACTOR_CRATE_A = 12,
  ACTOR_CRATE_B = 13,
  ACTOR_PIER_PUDDLE
};
/// The turtle's islet: Lonesome George, who carries the party to the secret
/// spot.
/// The Lil Turtle the party brought him swims beside him, and two unseen
/// blocks keep the leader off the ledge below.
enum {
  ACTOR_LONESOME_GEORGE = 9,
  ACTOR_LIL_TURTLE,
  ACTOR_LEDGE_BLOCK_A,
  ACTOR_LEDGE_BLOCK_B
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "text/msg_ids.h"
#ifndef TLA_ISLETS_H
#define TLA_ISLETS_H

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
void Islets_StopWaterShimmer(void);

void Islets_StartWaveDma(void);
void Islets_FillWaveLines(void);

extern u32 gSmokeScript[];
extern struct Trigger gIslets_Triggers[];
extern struct Trigger gIslets_DogTriggers[];
extern struct Trigger gIslets_PrettyStoneTriggers[];
extern struct Trigger gIslets_CowTriggers[];
extern struct Trigger gIslets_PierTriggers[];
extern struct Trigger gIslets_TurtleTriggers[];
extern u16 gSceneState[];

void Islets_InitializeDog(void);
void Islets_InitializePrettyStone(void);
void Islets_InitializeCow(void);
void Islets_InitializePier(void);
void Islets_InitializeTurtle(void);

/// A door the leader opens by stepping on its trigger
/// (Islets_OpenDoorAndEnter): its cell animation and the cell it plays at.
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
  void (*callback)(void);
};

extern struct ActorPlacement gIslets_Placements[];
extern struct ActorPlacement gIslets_DogActorPlacements[];
extern struct ActorPlacement gIslets_PrettyStoneActorPlacements[];
extern struct ActorPlacement gIslets_CowActorPlacements[];
extern struct ActorPlacement gIslets_PierActorPlacements[];
extern struct ActorPlacement gIslets_TurtleActorPlacements[];
extern s32 gIslets_EnterDoorScript[];
extern struct IsletsDoor gIslets_Doors[];
extern u16 gPushKinds[];

void Islets_SetCellCollision(s32 x, s32 z, s32 layers, s32 value);
void Islets_FinishPushingCrate(void);
void Islets_SetDockBlocksByHeight(void);
void Leader_PushObjectAhead(void);
s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *position);
struct Actor *Actor_FindAtCell(struct Vector3 *position, s32 mode);
s32 PushTarget_FindSlide(struct PushTarget *target);
void PushTarget_Push(struct PushTarget target);

void Islets_PlayFootsteps(void);
void Islets_RideOnLonesomeGeorge(struct Actor *actor);
void Actor_EmitSmoke(struct Actor *actor);
void Islets_StartWaterShimmer(void);
void Actor_MoveToObject(s32 actorId, s32 target);
void FieldPsynergy_StopMindReadWave(void);

#endif

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_ISLETS_DOG)
    return gIslets_DogActorPlacements;
  if (scene == SCENE_ISLETS_PRETTY_STONE)
    return gIslets_PrettyStoneActorPlacements;
  if (scene == SCENE_ISLETS_COW)
    return gIslets_CowActorPlacements;
  if (scene == SCENE_ISLETS_PIER)
    return gIslets_PierActorPlacements;
  if (scene == SCENE_ISLETS_TURTLE) {
    if (Event_GetFlag(FLAG_GEORGE_HAS_THE_TURTLE))
      gIslets_TurtleActorPlacements[1].talkFacing = 1;
    return gIslets_TurtleActorPlacements;
  }
  return gIslets_Placements;
}

/// Pushing the log off the ice: it falls into the water and makes a way back
/// for Pengulina.
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

  if (scene == SCENE_ISLETS_DOG)
    return gIslets_DogTriggers;
  if (scene == SCENE_ISLETS_PRETTY_STONE)
    return gIslets_PrettyStoneTriggers;
  if (scene == SCENE_ISLETS_COW)
    return gIslets_CowTriggers;
  if (scene == SCENE_ISLETS_PIER)
    return gIslets_PierTriggers;
  if (scene == SCENE_ISLETS_TURTLE)
    return gIslets_TurtleTriggers;
  return gIslets_Triggers;
}

#include "text/msg_ids.h"

/// Pengus: before the Pretty Stone he cries for Pengulina and watches her;
/// after, he chirps happily.
void Islets_TalkToPengus(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_00)) {
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

/// Mind Read on Pengus: Pengulina hurt her leg on the far shore and cannot swim
/// back; after the Pretty Stone, his thanks.
void Islets_MindReadPengus(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_00)) {
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

/// Pengulina: she cries from the far shore until the log takes her back to
/// Pengus.
void Islets_TalkToPengulina(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_00)) {
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

/// Mind Read on Pengulina: her leg keeps her from Pengus; after the Pretty
/// Stone, her thanks.
void Islets_MindReadPengulina(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_00)) {
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

/// The healer of E Tundaria Islet: across the counter, the sanctum's healing;
/// from elsewhere, why she is there.
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

/// The treasure hunter on the pier lets slip why they are there, the sailor
/// beside him covers it up the first time, then they are busy.
void Islets_TalkToTreasureHunters(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (!Event_GetFlag(FLAG_SCENE_TEMP_14)) {
    FieldEvent_SetMessageId((s32)MsgWeLikeThis);
    // "We like this island. A lot... That's why we're here... Because we..."
    Event_ShowMessage(ACTOR_TREASURE_HUNTER, 0);
    Actor_SetAttachedEffect(ACTOR_PIER_SAILOR,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_FaceDirection(ACTOR_PIER_SAILOR, FACING_WEST, 0);
    // "What are you saying!? Idiot!"
    Event_ShowMessage(ACTOR_PIER_SAILOR, 0);
    Actor_FaceDirectionAndWait(ACTOR_TREASURE_HUNTER, FACING_EAST);
    FieldActor_Jump(ACTOR_PIER_SAILOR, 4, 10);
    // "Don't listen to that guy. He didn't say anything about Treasure..."
    Event_ShowMessage(ACTOR_PIER_SAILOR, 0);
    Actor_SetAttachedEffect(ACTOR_PIER_SAILOR,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldEvent_Wait(20);
    Event_SetFlag(FLAG_SCENE_TEMP_14);
  }
  Actor_FaceActor(ACTOR_TREASURE_HUNTER, gMainRoutineWork.leaderActorId, 0);
  FieldEvent_SetMessageId((s32)MsgLookWeRe);
  // "Look, we're busy... We can't talk now... Catch us some other time..."
  Event_ShowMessage(ACTOR_TREASURE_HUNTER, 0);
  Actor_FaceDirection(ACTOR_PIER_SAILOR, FACING_SOUTH_SOUTHWEST, 0);
  Event_End();
  return;
}

/// The sailor on the pier talks of the Eastern Sea.
void Islets_TalkToSailor(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgTheEasternSea);
  // "The Eastern Sea is huge, even if you don't add the Sea of Time..."
  Event_ShowMessage(ACTOR_PIER_SAILOR, 0);
  // "You don't look like much of a sailor. You're not a treasure hunter..."
  Event_ShowMessage(ACTOR_PIER_SAILOR, 0);
  Event_End();
  return;
}

/// The bird chirps, watching the leader once it has traded.
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

/// The bird of SE Angara Islet: it takes the Pretty Stone and gives the Red
/// Cloth in return.
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

// The Islets' trade of the Red Cloth with the cow. With the cloth in the
// party, if FLAG_SCENE_TEMP_01 is set (asked before) the camera moves and the
// cow asks again (MsgAreYouGiving); otherwise it notices the cloth
// (MsgOhThatRed), the leader steps up and holds up its icon and the cow asks. A
// yes hands the cloth across step by step, takes it and gives Milk (the cow
// hands it back over the leader's head), sets FLAG_COW_TRADED and counts the
// answer in FieldWork.message the first time; a no sets FLAG_SCENE_TEMP_01.
// Without the cloth, the cow says MsgYupANice after the trade or
// MsgIVeBeenFeeling before it.

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->steeringFlags &= ~ACTOR_FACES_MOTION,                 \
   Actor_WalkToAndWait(actor, x, z), Event_Wait(1),                            \
   Actor_GetById(actor)->steeringFlags |= ACTOR_FACES_MOTION)

/// The cow walks round and hands the item back over the leader's head.
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
      Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 28);              \
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

/// The cow of N Osenia Islet: it notices the Red Cloth ("Oh! That red
/// neckerchief...") and asks for it; a yes trades it for Milk, handed back
/// over the leader's head, and a no leaves it to ask again. Without the cloth
/// it talks of how it feels, or after the trade of the neckerchief.
void Islets_TradeWithCow(void) {
  struct Actor *item, *leader, *given;
  const u8 *asked, *noticed;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (FieldPartyInventory_FindMember(ITEM_RED_CLOTH) != -1) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_01)) {
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
        Event_SetFlag(FLAG_SCENE_TEMP_01);
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

/// Shows an item's icon over an actor's head, the leader holding up its arms.
/// Returns the icon's actor, or null.
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

/// An actor's update: every fourth frame, a puff of the wake behind it
/// (gSmokeScript).
void Actor_EmitSmoke(struct Actor *actor) {
  struct Actor *spawned;

  if ((gFrameCount & 3) == 0) {
    spawned =
        FieldActor_Create(14, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    if (spawned != 0) {
      Actor_SetSpriteFlags(spawned, 0);
      Actor_SelectAnimation(spawned, 1);
      Actor_StartScript(spawned, gSmokeScript);
    }
  }
}

extern s32 gIslets_BirdFliesOffScript[];
extern s32 gIslets_BirdFliesBackScript[];

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->steeringFlags &= ~ACTOR_FACES_MOTION,                 \
   Actor_WalkToAndWait(actor, x, z), FieldEvent_Wait(1),                       \
   Actor_GetById(actor)->steeringFlags |= ACTOR_FACES_MOTION)

/// "Sniff! Sniff! I smell fresh milk!" The dog trades the Li'l Turtle for Milk.
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
    trader->update = Actor_EmitSmoke;
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
      trader->update = Actor_EmitSmoke;
      Actor_RunScriptAndWait(ACTOR_DOG, gIslets_BirdFliesOffScript);
      trader->update = 0;
    } else {
      Actor_FaceDirection(ACTOR_DOG, FACING_WEST, 0);
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 264, 856);
      Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
      item = Islets_ShowItemAbove(leader, ITEM_MILK);
      trader->update = Actor_EmitSmoke;
      Actor_RunScriptAndWait(ACTOR_DOG, gIslets_BirdFliesBackScript);
      trader->update = 0;
    }
    // "Hah hah hah! Are you going to give me that milk? Are you? Huh? Huh!?..."
    Event_ShowMessage(ACTOR_DOG, 0);
    FieldActor_WatchAndSetScript(ACTOR_DOG, gMainRoutineWork.leaderActorId);
    if (item != 0) {
      void (*emitSmoke)(void);
      item->collisionFlags = 9;
      FieldActor_SetPositionAndResetMotion(item, toFIXED(104), 0, toFIXED(840));
      emitSmoke = Actor_EmitSmoke;
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
    Event_SetFlag(FLAG_SCENE_TEMP_16);
  } else {
    if (Event_GetFlag(FLAG_DOG_TRADED)) {
      FieldActor_WatchAndSetScript(ACTOR_DOG, gMainRoutineWork.leaderActorId);
      if (Event_GetFlag(FLAG_SCENE_TEMP_16))
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

/// The leader's update while it rides Lonesome George: kept 16 pixels above
/// him, facing his way.
void Islets_RideOnLonesomeGeorge(struct Actor *actor) {
  struct Actor *source = Actor_Get(ACTOR_LONESOME_GEORGE);

  FieldActor_SetPositionAndResetMotion(
      actor, source->x.fixed, source->y.fixed + toFIXED(16), source->z.fixed);
  actor->facing = source->facing;
}

/// The window the scene closes through as Lonesome George swims off: around the
/// leader in the Japanese release, a shape from patch 1.
#if !TLA_PATCH_1
#define ISLETS_RIDE_TRANSITION                                                 \
  SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER)
#else
#define ISLETS_RIDE_TRANSITION                                                 \
  SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE)
#endif

extern struct ActionScript gIslets_SpinScript;

/// "At long last, I have a friend! Lonesome George I am no more!"
void Islets_LonesomeGeorgeThanks(struct Actor *gift) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *guide = Actor_Get(ACTOR_LONESOME_GEORGE);
  struct ActionScript script = gIslets_SpinScript;

  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldPsynergy_GreyMindReadPalette();
  Actor_ShowEmote(ACTOR_LONESOME_GEORGE, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  guide->update = Actor_EmitSmoke;
  Actor_RunScriptAndWait(ACTOR_LONESOME_GEORGE, &script);
  guide->update = 0;
  FieldEvent_SetMessageId(MsgAtLongLast);
  // "At long last, I have a friend! Lonesome George I am no more! I'm so..."
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  // "I don't have much to offer in return, but I can show you... my..."
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST);
  Actor_SetSpeed(ACTOR_LONESOME_GEORGE, 1.6);
  guide->update = Actor_EmitSmoke;
  Actor_WalkToAndWait(ACTOR_LONESOME_GEORGE, 296, 452);
  guide->update = 0;
  Actor_FaceDirection(ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST, 0);
  gift->speedLimit = toFIXED(8) / 5;
  gift->acceleration = toFIXED(4) / 5;
  gift->update = Actor_EmitSmoke;
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
      ->counter = 99;
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  leader->update = Actor_EmitSmoke;
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
  guide->update = Actor_EmitSmoke;
  Actor_SetDestination(ACTOR_LONESOME_GEORGE, 288, 538);
  FieldEvent_Wait(20);
  FieldPsynergy_RestoreMindReadPalette();
  Islets_StopWaterShimmer();
  gift->update = Actor_EmitSmoke;
  Actor_SelectAnimation(gift, 2);
  FieldActor_SetMoveTarget(gift, 288, 0, 538);
  FieldEvent_Wait(20);
  gFieldWork->transition = ISLETS_RIDE_TRANSITION;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(3);
  Event_SetFlag(FLAG_GEORGE_HAS_THE_TURTLE);
}

#define SPAWN(a, b, answer)                                                    \
  {                                                                            \
    a = FieldActor_Create(SPRITE_TURTLE_SMALL, toFIXED(296), 0, toFIXED(424)); \
    b = FieldActor_Create(14, toFIXED(296), 0, toFIXED(424));                  \
    if (a != 0) {                                                              \
      a->motionFlags = answer;                                                 \
      Actor_SetSpriteFlags(a, 0);                                              \
    }                                                                          \
    if (b != 0) {                                                              \
      b->scaleY = FIXED_ONE * 2;                                               \
      Actor_SetSpriteFlags(b, 0);                                              \
      Actor_SelectAnimation(b, 1);                                             \
      Actor_StartScript(b, gSmokeScript);                                      \
    }                                                                          \
  }

/// "Oh! You're really going to let me take care of your li'l turtle?"
void Islets_GiveTurtle(void) {
  struct Actor *leader, *a, *b, *other;
  const u8 *message;
  s32 answer;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (PartyInventory_FindMember(ITEM_LIL_TURTLE) != -1) {
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (GameFlag_Get(FLAG_SCENE_TEMP_01) != 0) {
      Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
      /* FAKEMATCH(temporary): the form kept. */
      a = (struct Actor *)Actor_EmitSmoke;
      leader->update = (void (*)(void))a;
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
        leader->update = (void (*)(void))a;
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
      leader->update = Actor_EmitSmoke;
      // Default: a is set for real when object 163 is made.
      a = 0;
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 318, 424);
      leader->update = 0;
      Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_WEST);
      Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
      leader->update = Actor_EmitSmoke;
      STEP(gMainRoutineWork.leaderActorId, 302, 424);
      SPAWN(a, b, 0)
      STEP(gMainRoutineWork.leaderActorId, 318, 424);
      leader->update = 0;
      Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
      Actor_ShowEmoteById(ACTOR_LONESOME_GEORGE,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
      Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_NORTH_NORTHWEST);
      other = Actor_GetById(ACTOR_LONESOME_GEORGE);
      other->update = Actor_EmitSmoke;
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
        leader->update = Actor_EmitSmoke;
        Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 302, 424);
        leader->update = 0;
        if (a != 0)
          Actor_Release(a);
        Event_SetFlag(FLAG_SCENE_TEMP_01);
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
          ->counter = 99;
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
      Actor_GetById(ACTOR_LONESOME_GEORGE)->update = Actor_EmitSmoke;
      Actor_SetDestination(ACTOR_LONESOME_GEORGE, 288, 538);
      FieldEvent_Wait(40);
      FieldPsynergy_RestoreMindReadPalette();
      Islets_StopWaterShimmer();
      Actor_GetById(ACTOR_LIL_TURTLE)->update = Actor_EmitSmoke;
      Actor_WalkTo(ACTOR_LIL_TURTLE, 288, 538);
      FieldEvent_Wait(20);
      gFieldWork->transition = ISLETS_RIDE_TRANSITION;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      FieldPsynergy_GreyMindReadPalette();
      FieldEvent_SetExitRequest(3);
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

extern u32 gIslets_GeorgeWalksScript[];
extern u32 gIslets_TurtleWalksScript[];

/// "You all set to go back to the island?" Lonesome George carries the party
/// back.
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
        ->counter = 99;
    FieldActor_Jump(gMainRoutineWork.leaderActorId, 6, 0);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
    Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 152, 768);
    leader->update = (void (*)(void))Islets_RideOnLonesomeGeorge;
    FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId),
                              SPRITE_COLOR_HIDDEN);
    Actor_SetAnimation(ACTOR_LONESOME_GEORGE, ANIM_NOD);
    FieldEvent_Wait(20);
    FieldPsynergy_RestoreMindReadPalette();
    Actor_SetSpeed(ACTOR_LONESOME_GEORGE, 1.6);
    Actor_SetSpeed(ACTOR_LIL_TURTLE, 1.6);
    update = (void (*)(void))Actor_EmitSmoke;
    done = (u8 *)actor + 99;
    *done = 0;
    actor->update = update;
    Actor_SetScript(ACTOR_LONESOME_GEORGE, gIslets_GeorgeWalksScript);
    FieldEvent_Wait(30);
    Actor_Get(ACTOR_LIL_TURTLE)->update = update;
    Actor_SetScript(ACTOR_LIL_TURTLE, gIslets_TurtleWalksScript);
    do {
      System_Wait(1);
    } while (*done == 0);
    Islets_StopWaterShimmer();
    System_Wait(1);
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    FieldPsynergy_GreyMindReadPalette();
    FieldEvent_SetExitRequest(4);
  } else {
    FieldPsynergy_GreyMindReadPalette();
    FieldEvent_SetMessageId((s32)MsgYouAllSet + 4);
    // "Take your time! I don't like to rush..."
    Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
    Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH);
  }
  Event_End();
}

/// The sailor's son on West Indra Islet boasts of his father.
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

/// Talking to actors 11 or 12 opens the field menu, as talking to nothing does.
void Islets_OpenFieldMenu(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->fieldMenu = 1;
}

/// A task: the two blocks by the turtle's ledge are skipped by talk and
/// Psynergy while the leader is below 40 pixels.
void Islets_SetLedgeBlocksByHeight(void) {
  if (Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed < toFIXED(40)) {
    Actor_Get(ACTOR_LEDGE_BLOCK_A)->collisionFlags |= ACTOR_NOT_TARGETED;
    Actor_Get(ACTOR_LEDGE_BLOCK_B)->collisionFlags |= ACTOR_NOT_TARGETED;
  } else {
    Actor_Get(ACTOR_LEDGE_BLOCK_A)->collisionFlags &= ~ACTOR_NOT_TARGETED;
    Actor_Get(ACTOR_LEDGE_BLOCK_B)->collisionFlags &= ~ACTOR_NOT_TARGETED;
  }
}

/// The bytes before the wave lines, which the ROM starts on 16 bytes; the
/// Japanese release's code ends 4 bytes short of where the others' does.
#if TLA_PATCH_1
#define WAVE_LINES_PADDING 11
#else
#define WAVE_LINES_PADDING 15
#endif

u8 gIslets_WaveTableIndex BSS_DATA;
u8 gIslets_UnusedAfterWaveTableIndex[WAVE_LINES_PADDING] BSS_DATA;
s16 gIslets_WaveLines[2][160] BSS_DATA;
s32 gIslets_SecondCrateZ BSS_DATA;
s32 gIslets_FirstCrateZ BSS_DATA;
s32 gIslets_SecondCrateX BSS_DATA;
s32 gIslets_FirstCrateX BSS_DATA;

/// Applies BG1 horizontal line scrolling with HBlank DMA.
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

/// Fills the next table of BG1 scroll lines with a sine wave that moves each
/// frame, for the water's HBlank ripple.
void Islets_FillWaveLines(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[2];
  s16 *lines = gIslets_WaveLines[gIslets_WaveTableIndex ^ 1];
  FIXED v[2];
  s32 *count = &v[1];
  FIXED sine;

  for (*count = 0; *count != 160; (*count)++) {
    sine = Math_Sin((*count + (u8)gFrameCount + (layer->y >> 16)) << 9);
    v[0] = sine;
    *lines++ = (layer->x >> 16) + (sine >> 15) + *count % 3 - 1;
  }
  gIslets_WaveTableIndex ^= 1;
}

/// The water ripples by an HBlank wave on BG1.
void Islets_StartWave(void) {
  Task_AddFrame(Islets_FillWaveLines, TASK_PASS_VBLANK);
  Task_AddFrame(Islets_StartWaveDma, TASK_PASS_FRAME);
}

/// Once the log carrying Pengulina reaches Pengus, the two and the leader react
/// and the party is given the Pretty Stone.
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

/// A task: a footstep every 16 frames.
void Islets_PlayFootsteps(void) {
  if ((gFrameCount & 15) == 0)
    Event_PlaySound(SE_FOOTSTEP);
}

/// Coming near the nest, the bird flies back onto it.
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
  FieldTask_AddFrame(Islets_PlayFootsteps, TASK_PASS_VBLANK);
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
  Event_SetFlag(FLAG_SCENE_TEMP_15);
  System_Wait(1);
  Event_End();
}

/// The leader jumps down the ledge.
void Islets_JumpDownLedge(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_JumpDownLedge(gMainRoutineWork.leaderActorId, 6, 1);
  Event_End();
  return;
}

/// The leader walks on the upper layer (2) of the map.
void Leader_SetLayer(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  leader->layer = 2;
}

/// The leader walks on the ground layer (0) of the map.
void Islets_StepOntoGround(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  leader->layer = 0;
}

/// Sets the collision code of the map cell at a 16.16 position, on the ground
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

/// Keeps the two crates' places as they are pushed; a crate pushed into the
/// water at (7, 28) or (12, 25) floats there, the way over it open.
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

/// After a crate is pushed, makes the cells it rests on solid, or floats both
/// crates where the pier's water is filled by them.
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
    Islets_SetCellCollision(gIslets_FirstCrateX, gIslets_FirstCrateZ, 1, 21);
    Islets_SetCellCollision(gIslets_FirstCrateX, gIslets_FirstCrateZ, 2, 0);
  } else {
    s32 *z = &gIslets_FirstCrateZ;

    Islets_SetCellCollision(gIslets_FirstCrateX, *z, 3, 0);
  }
  if (FIELD_CELL(gIslets_SecondCrateX) == 7 &&
      FIELD_CELL(gIslets_SecondCrateZ) == 27) {
    Islets_SetCellCollision(gIslets_SecondCrateX, gIslets_SecondCrateZ, 1, 21);
    Islets_SetCellCollision(gIslets_SecondCrateX, gIslets_SecondCrateZ, 2, 0);
  } else {
    s32 *z = &gIslets_SecondCrateZ;

    Islets_SetCellCollision(gIslets_SecondCrateX, *z, 3, 0);
  }
  Islets_SetCellCollision(first->x.fixed, first->z.fixed, 3, 255);
  Islets_SetCellCollision(second->x.fixed, second->z.fixed, 3, 255);
  if (!Event_GetFlag(FLAG_SCENE_TEMP_10) && firstX == 12 && firstZ == 26) {
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
    Event_SetFlag(FLAG_SCENE_TEMP_10);
    if (both) {
      Islets_SetCellCollision(first->x.fixed, first->z.fixed, 3, 0);
      Islets_SetCellCollision(second->x.fixed, second->z.fixed, 3, 0);
      MapGrid_CopyCellAttributes(7, 3, 1, 1, 12, 26);
      Event_SetFlag(FLAG_SCENE_TEMP_12);
      first->riseCounter = 1;
      second->riseCounter = 1;
    }
    Event_End();
  }
  if (!Event_GetFlag(FLAG_SCENE_TEMP_11) && secondX == 7 && secondZ == 29) {
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
    Event_SetFlag(FLAG_SCENE_TEMP_11);
    if (both) {
      Islets_SetCellCollision(first->x.fixed, first->z.fixed, 3, 0);
      Islets_SetCellCollision(second->x.fixed, second->z.fixed, 3, 0);
      MapGrid_CopyCellAttributes(7, 3, 1, 1, 7, 29);
      Event_SetFlag(FLAG_SCENE_TEMP_12);
      first->riseCounter = 1;
      second->riseCounter = 1;
    }
    Event_End();
  }
}

/// Remembers both crates' places before the leader pushes one.
void Islets_SaveCratePositions(void) {
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

/// Frost on the pier's puddle: the cells become an ice pillar.
void Islets_FreezePuddle(void) {
  MapGrid_CopyCellAttributes(5, 2, 1, 2, 5, 28);
  Event_SetFlag(FLAG_SCENE_TEMP_13);
}

/// Holding up at the iceberg's edge, the leader steps up onto it.
void Islets_StepUpOntoIceberg(void) {
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

/// Holding down, the leader steps off the iceberg back to the shore, unless the
/// icebergs are mid-drift (between steps 10 and 210 of their cycle).
void Islets_StepDownFromIceberg(void) {
  struct Actor *actor;
  u16 *state;

  if (gInput.held & KEY_DOWN) {
    state = gSceneState;
    actor = Actor_Get(gMainRoutineWork.leaderActorId);
    if ((u16)(*state - 10) > 200) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      actor->motionFlags = 0;
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 72, 296);
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Event_End();
    }
  }
}

/// Reveal begins: the icebergs stop drifting.
void Islets_StartReveal(void) {
  Event_SetFlag(FLAG_SCENE_TEMP_18);
  return;
}

/// Reveal ends: the icebergs drift again.
void Islets_EndReveal(void) {
  Event_ClearFlag(FLAG_SCENE_TEMP_18);
  return;
}

// The following six are from patch 1.
#if TLA_PATCH_1
/// From patch 1: one of six patches of touched cells cleared back to the map's
/// own (FieldTileCopies_CopyTouchedCells).
void Islets_ClearFirstPatch(void) {
  FieldTileCopies_CopyTouchedCells(65, 0, 4, 4, 65, 10, 1);
  return;
}

/// From patch 1: the second patch of touched cells cleared.
void Islets_ClearSecondPatch(void) {
  FieldTileCopies_CopyTouchedCells(65, 0, 2, 2, 71, 10, 1);
  return;
}

/// From patch 1: the third patch of touched cells cleared.
void Islets_ClearThirdPatch(void) {
  FieldTileCopies_CopyTouchedCells(65, 0, 1, 1, 68, 15, 1);
  return;
}

/// From patch 1: the fourth patch of touched cells cleared.
void Islets_ClearFourthPatch(void) {
  FieldTileCopies_CopyTouchedCells(65, 0, 2, 3, 75, 20, 1);
  return;
}

/// From patch 1: the fifth patch of touched cells cleared.
void Islets_ClearFifthPatch(void) {
  FieldTileCopies_CopyTouchedCells(65, 0, 4, 1, 78, 22, 1);
  return;
}

/// From patch 1: the sixth patch of touched cells cleared.
void Islets_ClearSixthPatch(void) {
  FieldTileCopies_CopyTouchedCells(65, 0, 1, 1, 79, 23, 1);
  return;
}
#endif

/// The leader centres on the door's column, opens it (gIslets_Doors) and walks
/// in, leaving by the step trigger's exit.
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
      actor, toFIXED(8 - FIXEDtoINT(actor->x.fixed) % 16) + actor->x.fixed,
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

/// Leaves by the step trigger's exit, stopping the water's shimmer.
void Islets_LeaveByStep(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Islets_StopWaterShimmer();
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->stepTrigger);
}

/// The leader centres on the doorway and walks in, leaving by the step
/// trigger's exit.
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
      actor, toFIXED(8 - FIXEDtoINT(actor->x.fixed) % 16) + actor->x.fixed,
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

/// "Here we are!" Lonesome George's secret spot.
void Islets_ArriveAtSecretSpot(void) {
  struct FieldWork *work;
  struct Actor *owner = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *actor = Actor_Get(ACTOR_LONESOME_GEORGE);

  Actor_MoveToObject(ACTOR_LONESOME_GEORGE, gMainRoutineWork.leaderActorId);
  Actor_SetPosition(ACTOR_LIL_TURTLE, 273, 520);
  System_Wait(1);
  actor->facing = FACING_NORTH;
  owner->update = (void (*)(void))Islets_RideOnLonesomeGeorge;
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
  Actor_Get(ACTOR_LIL_TURTLE)->update = (void (*)(void))Actor_EmitSmoke;
  actor->update = (void (*)(void))Actor_EmitSmoke;
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
  Islets_StartWaterShimmer();
}

extern u32 gIslets_TurtleSwimsScript[];
extern u32 gIslets_TurtleTurnsScript[];

/// Lonesome George brings the party back from his secret spot: he swims in
/// with the leader, says "Well, here we are." and lifts him ashore. The
/// Japanese edition starts no screen transition here.
void Islets_ArriveBackAtIsland(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *guide = Actor_Get(ACTOR_LONESOME_GEORGE);
  u8 *done;

  Actor_MoveToObject(ACTOR_LONESOME_GEORGE, gMainRoutineWork.leaderActorId);
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
  Actor_Get(ACTOR_LIL_TURTLE)->update = Actor_EmitSmoke;
  guide->update = Actor_EmitSmoke;
  done = &guide->riseEnabled;
  *done = 0;
  Actor_StartScript(guide, gIslets_TurtleSwimsScript);
  Actor_SetScript(ACTOR_LIL_TURTLE, gIslets_TurtleTurnsScript);
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
  Islets_StartWaterShimmer();
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
  if (scene == SCENE_ISLETS_DOG)
    Islets_InitializeDog();
  else if (scene == SCENE_ISLETS_PRETTY_STONE)
    Islets_InitializePrettyStone();
  else if (scene == SCENE_ISLETS_COW)
    Islets_InitializeCow();
  else if (scene == SCENE_ISLETS_PIER)
    Islets_InitializePier();
  else if (scene == SCENE_ISLETS_TURTLE)
    Islets_InitializeTurtle();
  return 0;
}

/// A task: the sailor's son and the pacing sailor are skipped by talk and
/// Psynergy while the leader is down on the dock.
void Islets_SetDockBlocksByHeight(void) {
  if (Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed > toFIXED(8)) {
    Actor_Get(ACTOR_SAILORS_SON)->collisionFlags &= ~ACTOR_NOT_TARGETED;
    Actor_Get(ACTOR_PACING_SAILOR)->collisionFlags &= ~ACTOR_NOT_TARGETED;
  } else {
    Actor_Get(ACTOR_SAILORS_SON)->collisionFlags |= ACTOR_NOT_TARGETED;
    Actor_Get(ACTOR_PACING_SAILOR)->collisionFlags |= ACTOR_NOT_TARGETED;
  }
}

/// Sets up West Indra Islet: the wading kind, the dog and the pacing sailor,
/// and the Milk's icon once the dog has traded.
void Islets_InitializeDog(void) {
  struct MapWorkGroundEffects *map =
      (struct MapWorkGroundEffects *)Ram_HeapSlots->mapGrid;
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  map->wading = MAP_WADING_UPPER_LEVEL;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_DOG), 0);
  Actor_SetSpeed(ACTOR_PACING_SAILOR, 0.8);
  Actor_Get(ACTOR_PACING_SAILOR)->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
  if (Event_GetFlag(FLAG_DOG_TRADED)) {
    spawned =
        FieldActor_Create(SPRITE_UNSEEN_MARKER, toFIXED(280), 0, toFIXED(840));
    if (spawned != 0) {
      spawned->motionFlags = 0;
      FieldActor_SetItemIcon(spawned, ITEM_MILK);
      Actor_SetSpriteFlags(spawned, 0);
      spawned->collisionFlags = 9;
      spawned->retainSpriteCache = 1;
    }
    Actor_Get(ACTOR_DOG)->facing = FACING_NORTH_NORTHEAST;
  } else {
    Actor_SetAnimation(ACTOR_DOG, 5);
  }
  Task_AddFrame(Islets_SetDockBlocksByHeight, TASK_PASS_VBLANK);
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

/// The Islets' marching guards (objects 11 to 18): while the event work is busy
/// or flag 0x218 is set they stand still; once the controlled character drops
/// below height -12 they stand and the event work's counter 0x16a becomes 200;
/// otherwise, for each frame owed (gSceneState's halfword 2) while no event
/// runs, a 260-frame routine sets them walking with their facing steps
/// (Islets_MoveIceberg) at 0, still at 100 and 220, and walking in place at
/// 130.
void Islets_DriftIcebergs(void) {
  u16 *step = &gSceneState[0];
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u16 *owed = &gSceneState[1];
  s32 i;
  s32 count;

  if (work->psynergy != 0 || GameFlag_Get(FLAG_SCENE_TEMP_18)) {
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
      s16 *countdown = &work->raisedTrigger;
      s32 frames = 200;

      *countdown = frames;
    }
    return;
  }
  count = *owed + 1;
  *owed = count;
  if (work->pushCell != 0 || (u16)count == 0)
    return;
  do {
    switch (*step) {
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
    if (++*step == 260)
      *step = 0;
  } while (--*owed != 0);
}

extern u32 gIslets_PenguinsHopScript[];

/// The room's setup: flags, the objects' places and the scene state.
void Islets_InitializePrettyStone(void) {
  struct Actor *actor;
  struct Actor *other;
  u16 *state;
  u16 *next;
  s32 i;

  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  if (gMainRoutineWork.entrance == 2)
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
  FieldTask_AddFrame(Islets_DriftIcebergs, TASK_PASS_VBLANK);
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
      s32 facing = FACING_SOUTH_SOUTHEAST;

      Actor_SetPositionAndFacing(ACTOR_PENGULINA, toFIXED(234), toFIXED(204),
                                 facing);
    }
    Actor_Get(ACTOR_PENGULINA)->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
    Actor_SetSpeed(ACTOR_PENGUS, 2);
    Actor_SetSpeed(ACTOR_PENGULINA, 1.6);
    Actor_SetScript(ACTOR_PENGUS, gIslets_PenguinsHopScript);
    Actor_SetScript(ACTOR_PENGULINA, gIslets_PenguinsHopScript);
    Event_SetFlag(FLAG_SCENE_TEMP_00);
  }
}

/// Sets up N Osenia Islet: the cow, wearing the Red Cloth once it has traded,
/// and the blue orb.
void Islets_InitializeCow(void) {
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == 3)
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

// A room's setup.

/// Sets up the room's actors and map work.
void Islets_InitializePier(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_Get(ACTOR_BIRD);
  struct Actor *last;
  s32 set;
  s32 zero = 0;

  map->layers[2].speedX = zero;
  map->layers[2].speedY = zero;
  if (gMainRoutineWork.entrance == 11)
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    actor->motionFlags = zero;
    FieldActor_SetSpritePriority(ACTOR_BIRD, 1);
  }
  Actor_SetPosition(11, 0, 0);
  Actor_DisableById(ACTOR_CRATE_A);
  Actor_DisableById(ACTOR_CRATE_B);
  set = Event_GetFlag(FLAG_SCENE_TEMP_12);
  if (set) {
    struct Actor *first = Actor_Get(ACTOR_CRATE_A);
    struct Actor *second = Actor_Get(ACTOR_CRATE_B);

    first->motionFlags = zero;
    second->motionFlags = zero;
    first->riseCounter = 1;
    second->riseCounter = 1;
    {
      s32 held = 29;

      MapGrid_CopyCellAttributes(7, 3, 1, 1, 7, held);
    }
  } else {
    struct Actor *first = Actor_Get(ACTOR_CRATE_A);
    struct Actor *second = Actor_Get(ACTOR_CRATE_B);

    first->motionFlags = 0;
    second->motionFlags = 0;
    Islets_SetCellCollision(first->x.fixed, first->z.fixed, 3, 255);
    Islets_SetCellCollision(second->x.fixed, second->z.fixed, 3, 255);
  }
  Actor_DisableById(ACTOR_PIER_PUDDLE);
  last = Actor_Get(ACTOR_PIER_PUDDLE);
  {
    s32 still = 0;

    last->riseCounter = 1;
    last->motionFlags = still;
  }
  Islets_SetCellCollision(last->x.fixed, last->z.fixed, 1, 255);
  if (Event_GetFlag(FLAG_SCENE_TEMP_13))
    MapGrid_CopyCellAttributes(5, 2, 1, 2, 5, 28);
}

void Islets_StartPaletteCycle(void);

/// Sets up the Sea of Time Islet: the water's palette cycle, Lonesome George
/// and the Li'l Turtle once given, and the ride's arrival or return by
/// entrance.
void Islets_InitializeTurtle(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 done;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Map_SetUpperLevelWading(map);
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  Islets_StartPaletteCycle();
  PaletteCycle_Add(3, 12, 5, 4);
  PaletteCycle_Add(4, 12, 5, 4);
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
  Actor_SetSpriteFlags(Actor_Get(ACTOR_LEDGE_BLOCK_A), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_LEDGE_BLOCK_B), 0);
  FieldTask_AddFrame(Islets_SetLedgeBlocksByHeight, TASK_PASS_VBLANK);
  if ((u16)(gMainRoutineWork.entrance - 4) <= 1) {
    Event_SetFlag(FLAG_SCENE_TEMP_17);
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
      {
        s32 south = FACING_SOUTH;

        Actor_SetPositionAndFacing(ACTOR_LONESOME_GEORGE, toFIXED(152),
                                   toFIXED(768), south);
      }
      Actor_SetPosition(ACTOR_LIL_TURTLE, 176, 777);
      System_Wait(1);
    }
  }
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 3) {
      Islets_ArriveAtSecretSpot();
    } else if (entrance == 4) {
      Islets_ArriveBackAtIsland();
#if TLA_PATCH_1
    } else if (entrance != 5) {
#else
    } else {
#endif
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Islets_StartWaterShimmer();
    }
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Islets_StartWaterShimmer();
  }
  Event_End();
}

/// Nothing to prepare before the map is built. Returns 0.
s32 Scene_PrepareMap(void) { return 0; }

/// Allocates the palette cycling queue (HEAP_SLOT_PALETTE_CYCLES) with its four
/// entries cleared.
void Islets_StartPaletteCycle(void) {
  struct PaletteCycleWork *paletteCycles;
  struct PaletteCycle *cycle;
  u16 cycleIndex;
  u16 colorIndex;

  paletteCycles = HeapSlot_AllocEwram(HEAP_SLOT_PALETTE_CYCLES,
                                      sizeof(struct PaletteCycleWork));
  cycle = paletteCycles->entries;
  // Four cycles of up to 16 saved colours each.
  for (cycleIndex = 0; cycleIndex != 4; cycleIndex++) {
    cycle->source = 0;
    cycle->position = 0;
    cycle->timer = 0;
    cycle->delay = 0;
    cycle->count = 0;
    for (colorIndex = 0; colorIndex != 16; colorIndex++) {
      cycle->colors[colorIndex] = 0;
    }
    cycle++;
  }
  paletteCycles->count = 0;
}

#define gPaletteWork ((struct PaletteCycleWork *)Ram_HeapSlots->paletteCycles)

#include "../field/palqueue_add.c"
#undef gPaletteWork

#include "text/msg_ids.h"

/// Starts the water's shimmer each frame (Islets_ShimmerWater).
void Islets_StartWaterShimmer(void) {
  Task_AddFrame(&Islets_ShimmerWater, TASK_PASS_VBLANK);
  return;
}

/// Stops the water's shimmer.
void Islets_StopWaterShimmer(void) {
  FieldTask_Remove(Islets_ShimmerWater);
  return;
}

#define gPaletteWork ((struct PaletteCycleWork *)Ram_HeapSlots->paletteCycles)

/// A task: turns each queued palette cycle one colour on when its delay runs
/// out, unless Psynergy or a menu is open.
void Islets_ShimmerWater(void) {
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

/// The leader drops through the hole and the scene leaves by exit 3.
void Islets_DropThroughHole(void) {
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
  FieldEvent_SetExitRequest(3);
  return;
}

/// The leader jumps forward over the gap, where it can.
void Islets_JumpForward(void) {
  Field_TryJumpForward();
  return;
}

extern s8 gPushFootprints[][4];

/// Rewrites the map cells under a pushable actor. The actor's sprite shape
/// picks one of the six footprints in gPushFootprints (four signed bytes each,
/// in sixteenths of a cell); the cells under it are copied from the map's spare
/// area (Map_CopyCellAttributeRect) and byte 2 of both collision layers there
/// is filled with 255 (PushedActor_FillGridAttributeRectangle). Returns 1, or 0
/// for a shape with no footprint.
s32 PushTarget_FillFootprint(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_GetById(id);
  s16 kind = ((struct Sprite *)actor->sprite)->entries[0]->animationId;
  s16 i = 0;
  s32 next;
  u32 index;
  s32 a, b;
  s32 fromX, fromZ;
  s32 width, height, x, z;

  if ((u16)kind != gPushKinds[(u16)i]) {
    do {
      next = (i + 1) * 65536;
      index = (u32)next >> 16;
      i = next >> 16;
    } while (index <= 5 && (u16)kind != gPushKinds[index]);
  }
  // fromX and b are reused for the next values they hold.
  fromX = (u16)i;
  if (fromX == 6)
    return 0;
  a = gPushFootprints[fromX][0];
  width = (ABS(a) + ABS(gPushFootprints[fromX][2])) >> 4;
  b = gPushFootprints[fromX][1];
  height = (ABS(b) + ABS(gPushFootprints[fromX][3])) >> 4;
  x = actor->x.fixed + toFIXED(a);
  z = actor->z.fixed + toFIXED(b);
  b = FIELD_CELL(map->layers[1].offsetX);
  x >>= 20;
  z >>= 20;
  fromZ = FIELD_CELL(map->layers[1].offsetY);
  fromX = (u16)b;
  Map_CopyCellAttributeRect(x, z, width, height, x + fromX, z + (u16)fromZ);
  PushedActor_FillGridAttributeRectangle(0, x, z, width, height, 255);
  PushedActor_FillGridAttributeRectangle(2, x, z, width, height, 255);
  return 1;
}

void PushedActor_FillGridAttributeRectangle(s32 layer, s32 x, s32 z, u32 width,
                                            u32 height, s32 value) {
  struct MapCell *cells = Ram_HeapSlots->mapGrid->layers[layer].cells;
  struct MapCell *cell;
  u32 i;
  u32 j;

  cells += x + z * 128;
  for (i = 0; i < height; i++) {
    cell = cells + i * 128;
    for (j = 0; j < width; j++) {
      cell->collisionCode = value;
      cell++;
    }
  }
}

extern s32 gPushLeaderSteps[];

/// Whether an actor at a position would collide with the map
/// (MapGrid_CheckCollision).
static __inline__ s32 PushTarget_CheckCollision(struct Actor *actor,
                                                struct Vector3 *cell) {
  return MapGrid_CheckCollision(actor, cell);
}

#include "shared/push_target_find_slide.h"

/// The whole-pixel half of a 16.16 coordinate.

/// Finds the pushable actor in front of the leader: one whose sprite is one of
/// the six push kinds and whose footprint covers the cell ahead. Returns it,
/// with the facing, its slot and its kind, or null.
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor **entry;
  struct Actor *actor;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->facing >> 12;
  i = 8;
  entry = &work->actors[8];
  do {
    actor = *entry;
    for (k = 0; k <= 5; k++) {
      if (((struct Sprite *)actor->sprite)->entries[0]->animationId ==
          gPushKinds[k]) {
        x = (leader->x.part[0].integer +
             (gPushLeaderSteps[*direction] >> 16)) >>
            4;
        z = (leader->z.part[0].integer + (s16)gPushLeaderSteps[*direction]) >>
            4;
        left = (actor->x.part[0].integer + gPushFootprints[k][0]) >> 4;
        top = (actor->z.part[0].integer + gPushFootprints[k][1]) >> 4;
        right = (actor->x.part[0].integer + gPushFootprints[k][2]) >> 4;
        bottom = (actor->z.part[0].integer + gPushFootprints[k][3]) >> 4;
        if (left <= x && x < right && top <= z && z < bottom) {
          *kind = k;
          if (k & 1) {
            if (left != FIELD_CELL(leader->x.fixed)) {
              *slot = i;
              return actor;
            }
          } else {
            if (top != FIELD_CELL(leader->z.fixed)) {
              *slot = i;
              return actor;
            }
          }
        }
      }
    }
    entry++;
    i++;
  } while (i <= 63);
  return NULL;
}

void Actor_StopAtPushTarget(struct Actor *actor);

#define ABS(x) ((x) < 0 ? -(x) : (x))

/// Pushes an actor to its target, the leader following, and moves its cells
/// with it.
void PushTarget_Push(struct PushTarget target) {
  struct MapWork *map =
      (struct MapWork *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *actor = Actor_Get(target.id);
  s32 cell[3];
  FIXED from[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 originX;
  s32 originZ;
  s32 direction;
  s32 speed;
  s32 acceleration;

  direction = leader->facing >> 16;
  footprint = target.footprint;
  speed = leader->speedLimit;
  acceleration = leader->acceleration;
  from[0] = actor->x.fixed;
  from[2] = actor->z.fixed;
  cell[0] = FIELD_CELL(actor->x.fixed + toFIXED(gPushFootprints[footprint][0]));
  cell[2] = FIELD_CELL(actor->z.fixed + toFIXED(gPushFootprints[footprint][1]));
  width = (ABS(gPushFootprints[footprint][0]) +
           ABS(gPushFootprints[footprint][2])) >>
          4;
  height = (ABS(gPushFootprints[footprint][1]) +
            ABS(gPushFootprints[footprint][3])) >>
           4;
  PushedActor_FillGridAttributeRectangle(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.5, 0.1);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 8);
  System_Wait(15);
  {
    s32 deltaX = (target.x - from[0]) >> 17;
    s32 deltaZ = (target.z - from[2]) >> 17;

    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, deltaX, deltaZ);
  }
  Actor_Get(gMainRoutineWork.leaderActorId)->update = Actor_StopAtPushTarget;
  System_Wait(4);
  if ((u32)(direction - 6) <= 7)
    Actor_SelectAnimation(actor, 3);
  else
    Actor_SelectAnimation(actor, 2);
  Event_PlaySound(SE_HEAVY_MOVE);
  Actor_SetSpeedAndAcceleration(target.id, 0.5, 0.1);
  FieldActor_SetMoveTargetFX(actor, target.x, target.y, target.z);
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.3, 0.1);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId,
                             (gPushLeaderSteps[direction] << 16) >> 31,
                             (gPushLeaderSteps[direction] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(actor);
  Event_PlaySound(SE_STOP_EXIT_SE);
  Event_PlaySound(SE_HEAVY_STOP);
  FieldActor_SetPositionAndResetMotion(actor, target.x, actor->y.fixed,
                                       target.z);
  Actor_SelectAnimation(actor, 1);
  target.x = target.x + toFIXED(gPushFootprints[footprint][0]);
  target.z = target.z + toFIXED(gPushFootprints[footprint][1]);
  target.x >>= 20;
  target.z >>= 20;
  originX = FIELD_CELL(map->layers[1].offsetX);
  originZ = FIELD_CELL(map->layers[1].offsetY);
  MapGrid_CopyCellAttributes(target.x, target.z, width, height,
                             originX + target.x, originZ + target.z);
  PushedActor_FillGridAttributeRectangle(0, target.x, target.z, width, height,
                                         255);
  PushedActor_FillGridAttributeRectangle(2, target.x, target.z, width, height,
                                         255);
  cell[0] = FIELD_CELL(from[0] + toFIXED(gPushFootprints[footprint][0]));
  cell[2] = FIELD_CELL(from[2] + toFIXED(gPushFootprints[footprint][1]));
  originX += cell[0];
  originZ += cell[2];
  MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0], cell[2]);
  PushedActor_FillGridAttributeRectangle(2, cell[0], cell[2], width, height, 0);
  Event_NoOp();
}

/// Stops an actor that walks into another actor that cannot be pushed, or into
/// a wall.
void Actor_StopAtPushTarget(struct Actor *actor) {
  struct Vector3 position;
  struct Actor *hit;
  u32 i;
  s32 angle = actor->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position.x = actor->x.fixed;
  position.y = actor->y.fixed;
  position.z = actor->z.fixed;
  FieldVector_AddPolarOffset(16, angle, &position);
  hit = Actor_FindAtCell(&position, 1);
  if (hit != 0) {
    for (i = 0; i <= 5; i++) {
      if (((struct Sprite *)hit->sprite)->entries[0]->animationId ==
          gPushKinds[i])
        return;
    }
    FieldActor_SetPositionAndResetMotion(actor, actor->x.fixed, actor->y.fixed,
                                         actor->z.fixed);
  }
  position.x = actor->x.fixed;
  position.y = actor->y.fixed;
  position.z = actor->z.fixed;
  if (MapGrid_CheckCollision(actor, &position) > 0)
    FieldActor_SetPositionAndResetMotion(actor, actor->x.fixed, actor->y.fixed,
                                         actor->z.fixed);
}

#undef TLA_ISLETS_H
#undef STEP
#undef HAND_BACK
#undef ISLETS_RIDE_TRANSITION
#undef SPAWN
#undef gPaletteWork
#undef ABS
#include "text/msg_ids.h"

void Islets_TalkToPengulina(void);
void Islets_TalkToHealer(void);
void Islets_TalkToSailor(void);
void Islets_TalkToBird(void);
void Islets_TradeWithCow(void);
void Islets_PengusGivesPrettyStone(void);
void Islets_BirdReturnsToNest(void);
void Islets_FinishPushingCrate(void);
void Islets_EnterDoor(void);

// clang-format off
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

u16 gPushKinds[] = {
    258, 259, 293, 294, 332, 331,
};

s8 gPushFootprints[][4] = {
    {-32, -8, 32, 8}, {-8, -32, 8, 32}, {-32, -16, 32, 0}, {-8, -32, 8, 32},
    {-32, -8, 32, 8}, {-8, -32, 8, 32},
};

#include "shared/push_leader_steps.c"

u32 gIslets_DogWanderScript[] = {
#if !TLA_PATCH_1
    SC_SET(SW_COLLISION, 131),
    SC_ACCELERATION(0.4),
#else
    SC_SET(SW_COLLISION, 129),
    SC_ACCELERATION(0.4),
#endif
    SC_SPEED(0.4),
    SC_ANIMATION(2),
    SC_MOVE(376, 0, 216),
    SC_SYNC,
    SC_MOVE(392, 0, 224),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(80),
    SC_ANIMATION(2),
    SC_MOVE(376, 0, 216),
    SC_SYNC,
    SC_MOVE(376, 0, 184),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gIslets_GullScript[] = {
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

u32 gIslets_PenguinsHopScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(234, 0, 240),
    SC_SYNC,
    SC_MOVE(276, 0, 222),
    SC_SYNC,
    SC_MOVE(308, 0, 256),
    SC_SYNC,
    SC_MOVE(252, 0, 256),
    SC_SYNC,
    SC_MOVE(264, 0, 208),
    SC_SYNC,
    SC_MOVE(232, 0, 184),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gIslets_BirdFliesBackScript[] = {
    SC_ANIMATION(3),
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(0, 0, -16),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(-32, 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(0, 0, 32),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(32, 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE(280, 0, 856),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

s32 gIslets_BirdFliesOffScript[] = {
    SC_ANIMATION(3),
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(0, 0, 16),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(32, 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(0, 0, -32),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE_BY(-32, 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_MOVE(280, 0, 856),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gSmokeScript[] = {
    SC_WAIT(15),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

u32 gIslets_TurtleSwimsScript[] = {                                                               
      SC_MOVE(248, 0, 686),                                     
      SC_SYNC,                                                                 
      SC_MOVE(248, 0, 768),                                     
      SC_SYNC,                                                                 
      SC_MOVE(152, 0, 768),                                      
      SC_SYNC,                                                                 
      SC_SET(SW_RISE_ENABLED, 1),                                                   
      SC_END,                                                                  
  };

u32 gIslets_TurtleTurnsScript[] = {                                                               
      SC_ANIMATION(2), SC_MOVE(248, 0, 686),                           
      SC_SYNC,   SC_MOVE(248, 0, 768),                           
      SC_SYNC,   SC_MOVE(176, 0, 777),                           
      SC_SYNC,   SC_ANIMATION(1),                                                    
      SC_END,                                                                  
  };

u32 gIslets_GeorgeWalksScript[] = {                                                               
      SC_MOVE(248, 0, 768),                                     
      SC_SYNC,                                                                 
      SC_MOVE(248, 0, 686),                                     
      SC_SYNC,                                                                 
      SC_MOVE(248, 0, 584),                                      
      SC_SYNC,                                                                 
      SC_SET(SW_RISE_ENABLED, 1),                                                   
      SC_END,                                                                  
  };

u32 gIslets_TurtleWalksScript[] = {                                                               
      SC_ANIMATION(2), SC_MOVE(248, 0, 768),                           
      SC_SYNC,   SC_MOVE(248, 0, 686),                           
      SC_SYNC,   SC_MOVE(240, 0, 600),                           
      SC_SYNC,   SC_ANIMATION(1),                                                    
      SC_END,                                                                  
  };

u32 gIslets_PierPaceScript[] = {
    39, 2, 4, toFIXED(184), 0, toFIXED(936),
    1, 39, 1, 1, 80, 39,
    2, 4, toFIXED(88), 0, toFIXED(936), 1,
    39, 1, 0, 80, 13, SC_FOREVER,
    0,
};

u16 gIslets_DoorFrames[] = {
    92, 10, 1, 2, 6, 92, 8, 1,
    2, 6, -1,
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

u32 gSceneExits[] = {
    SCENE_EXITS(IsletsDog),
    SCENE_EXIT(ISLETS_DOG_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ISLETS_DOG),
    SCENE_EXIT(ISLETS_DOG_TO_CHAMPA_ROOMS, ChampaRooms, CHAMPA_ROOMS_FROM_ISLETS_DOG),
    SCENE_EXITS(IsletsPrettyStone),
    SCENE_EXIT(ISLETS_PRETTY_STONE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ISLETS_PRETTY_STONE),
    SCENE_EXIT(ISLETS_PRETTY_STONE_TO_MIKASALLA_ROOMS_HOUSES, MikasallaRoomsHouses, MIKASALLA_ROOMS_HOUSES_FROM_ISLETS_PRETTY_STONE),
    SCENE_EXIT(3, IsletsPrettyStone, ISLETS_PRETTY_STONE_FROM_WORLD_MAP),
    SCENE_EXITS(IsletsCow),
    SCENE_EXIT(ISLETS_COW_TO_WORLD_MAP_A, WorldMap, WORLD_MAP_FROM_ISLETS_COW_A),
    SCENE_EXIT(ISLETS_COW_TO_WORLD_MAP_B, WorldMap, WORLD_MAP_FROM_ISLETS_COW_B),
    SCENE_EXIT(ISLETS_COW_TO_DAILA_ROOMS_HOUSES, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_ISLETS_COW),
    SCENE_EXIT(ISLETS_COW_TO_WORLD_MAP_C, WorldMap, WORLD_MAP_FROM_ISLETS_COW_C),
    SCENE_EXITS(IsletsPier),
    SCENE_EXIT(ISLETS_PIER_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ISLETS_PIER),
    SCENE_EXIT(ISLETS_PIER_TO_CHAMPA_ROOMS, ChampaRooms, CHAMPA_ROOMS_FROM_ISLETS_PIER),
    SCENE_EXITS(IsletsTurtle),
    SCENE_EXIT(ISLETS_TURTLE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ISLETS_TURTLE),
    SCENE_EXIT(ISLETS_TURTLE_TO_APOJII_ISLANDS_SHOPS, ApojiiIslandsShops, APOJII_ISLANDS_SHOPS_FROM_ISLETS_TURTLE),
    SCENE_EXIT(3, IsletsTurtle, 4),
    SCENE_EXIT(4, IsletsTurtle, 3),
    SCENE_EXIT(ISLETS_TURTLE_TO_ISLET_CAVE_MELD, IsletCaveMeld, ISLET_CAVE_MELD_FROM_ISLETS_TURTLE),
    SCENE_EXITS_END,
};

struct ActorPlacement gIslets_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_DogActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 744, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_IN_BLUE_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 920, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_IN_BLUE_SHIRT, CONDITION_ALWAYS, (s32)gIslets_PierPaceScript, 88, 0, 936, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 856, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_PrettyStoneActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, (s32)gIslets_DogWanderScript, 376, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PENGUIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7, 176, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
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

struct ActorPlacement gIslets_CowActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 248, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 184, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COW, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 132, 0, 176, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, 7, 326, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_PierActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 296, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 280, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ORANGE_BIRD, CONDITION_ALWAYS, (s32)gIslets_GullScript, 360, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_TurtleActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 168, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_TURTLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TURTLE_SMALL, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 202, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 266, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gIslets_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_DogTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
     EVENT(Islets_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 16, CONDITION_ALWAYS,
     EVENT(Islets_JumpForward)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgEverSinceThe)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Islets_TalkToSailorsSon)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgYouReLucky)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgArffArff)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgTheWaterwaysNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgICanTWaitUntil)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgICanTWaitUntilThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     EVENT(Islets_TradeWithDog)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_PrettyStoneTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
     EVENT(Islets_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS,
     EVENT(Islets_JumpDownLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS,
     EVENT(Islets_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 30, FLAG_LOG_PUSHED_INTO_WATER,
     EVENT(Islets_PushLogIntoWater)},
    {TRIGGER_STEP, 20, FLAG_PENGUS_GAVE_PRETTY_STONE,
     EVENT(Islets_PengusGivesPrettyStone)},
    {TRIGGER_STEP, 7, CONDITION_ALWAYS, EVENT(Islets_StepUpOntoIceberg)},
    {TRIGGER_STEP, 8, CONDITION_ALWAYS, EVENT(Islets_StepDownFromIceberg)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgMyMomSays)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Islets_TalkToPengus)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(Islets_TalkToPengulina)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Islets_DropThroughHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgWhoInWeyard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     EVENT(Islets_MindReadPengus)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     EVENT(Islets_MindReadPengulina)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(Islets_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgMayTheseTravelers)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Islets_StartReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Islets_EndReveal)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_CowTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgIJustWant)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTheSeaOf)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgMooMoo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgDadAndGrandpa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgIRememberOnce)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     EVENT(Islets_TradeWithCow)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(Leader_SetLayer)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(Islets_StepOntoGround)},
#if TLA_PATCH_1 /* From patch 1. */
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 10,
     CONDITION_ALWAYS, EVENT(Islets_ClearFirstPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 11,
     CONDITION_ALWAYS, EVENT(Islets_ClearSecondPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 12,
     CONDITION_ALWAYS, EVENT(Islets_ClearThirdPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 13,
     CONDITION_ALWAYS, EVENT(Islets_ClearFourthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 14,
     CONDITION_ALWAYS, EVENT(Islets_ClearFifthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 15,
     CONDITION_ALWAYS, EVENT(Islets_ClearSixthPatch)},
#endif
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_PierTriggers[] = {
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK |
         TRIGGER_FLAG_FACING | FACING_NORTH,
     11, CONDITION_ALWAYS, 11},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 21, FLAG_SCENE_TEMP_12,
     EVENT(Islets_SaveCratePositions)},
    {TRIGGER_PUSH, 0, FLAG_SCENE_TEMP_12, EVENT(Islets_StartPushingCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     FLAG_SCENE_TEMP_12, EVENT(Islets_StartPushingCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13,
     FLAG_SCENE_TEMP_12, EVENT(Islets_StartPushingCrate)},
    {TRIGGER_PUSHED, 0, FLAG_SCENE_TEMP_12, EVENT(Islets_FinishPushingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, FLAG_SCENE_TEMP_12,
     EVENT(Islets_FinishPushingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, FLAG_SCENE_TEMP_12,
     EVENT(Islets_FinishPushingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 14, FLAG_SCENE_TEMP_13,
     EVENT(Islets_FreezePuddle)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Islets_TalkToTreasureHunters)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Islets_TalkToSailor)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Islets_TalkToBird)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgIfWeRe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgSureThereAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     EVENT(Islets_TradeWithBird)},
    {TRIGGER_STEP, 20, FLAG_SCENE_TEMP_15, EVENT(Islets_BirdReturnsToNest)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_TurtleTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Islets_LeaveByStep)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Islets_LeaveByStep)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgYouUsedTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgIRememberHow)},
    {TRIGGER_TALK, 9, FLAG_GEORGE_HAS_THE_TURTLE, MSGNO(MsgGeorgeSilence)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgYup)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_SCENE_TEMP_17,
     EVENT(Islets_GiveTurtle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     EVENT(Islets_RideBackToIsland)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(Islets_OpenFieldMenu)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Islets_OpenFieldMenu)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(Leader_SetLayer)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(Islets_StepOntoGround)},
    {TRIGGER_TABLE_END},
};
