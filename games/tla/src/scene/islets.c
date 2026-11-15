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
#include "flags.h"
#include "item_ids.h"
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
#include "field/event_inline.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "field/call.h"
#include "version.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/heap_release.h"
#include "graphics/palette_snapshots.h"
#include "field/field_bank.h"
#include "scene/scene.h"
#include "system/vector3.h"
#include "shared/tables.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "scene/mars_lighthouse_entrance.h"
#include "scene/shrine_of_the_sea_god.h"

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
extern s16 gIslets_DoorFrames[];
extern s16 gIslets_NoDoorFrames[];

void Islets_ShimmerWater(void);

void Field_TryJumpForward();

void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, u32 width, u32 height,
                           s32 value);
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *number, s32 *kind);

void Islets_SetLedgeBlocksByHeight(void);

struct Actor;

struct ActionScript {
  u32 words[9];
};

struct IsletsMapWork {
  u8 unk000[0x190];
  s32 value190;
  s32 value194;
};

void Actor_Release(struct Actor *object);
void FieldPsynergy_GreyMindReadPalette(void);
void FieldPsynergy_RestoreMindReadPalette(void);
void FieldCamera_WaitForArrival(void);
void TileCopies_CopyTouchedCells();

s32 PushTarget_FillFootprint(s32 id);

void Actor_DisableById(s32 actor);

/// The islets' people and animals, room by room, numbered by the rows of
/// each room's placements.
/// The dog's islet: the sailor's son, and the dog that trades.
enum { ACTOR_SAILORS_SON = 9, ACTOR_DOG = 11 };
/// The icebergs' islet: Pengus and Pengulina, the log pushed into the water
/// and the healer.
enum {
  ACTOR_PENGUS = 9,
  ACTOR_LOG = 10,
  ACTOR_PENGULINA = 19,
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
  ACTOR_CRATE_B = 13
};
/// The turtle's islet: Lonesome George, who carries the party to the secret
/// spot.
enum { ACTOR_LONESOME_GEORGE = 9 };

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

void Actor_SelectAnimation(struct Actor *object, s32 value);
void Actor_StartScript(struct Actor *object, const void *script);
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

struct IsletsMapFlags {
  u8 unk00[0x17];
  u8 unused : 2;
  u8 mode : 2;
};

struct IsletsExit {
  s32 value;
  struct {
    u16 first;
    u16 second;
  } pair;
};

struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
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
extern s32 gIslets_SecondCrateZ;
extern s32 gIslets_FirstCrateZ;
extern s32 gIslets_SecondCrateX;
extern s32 gIslets_FirstCrateX;
extern s32 gIslets_EnterDoorScript[];
extern struct IsletsExit gIslets_Doors[];
extern u16 gPushKinds[];

void Islets_SetCellCollision(s32 x, s32 z, s32 layers, s32 value);
void Islets_FinishPushingCrate(void);
void Islets_SetDockBlocksByHeight(void);
void Leader_PushObjectAhead(void);
s32 MapGrid_CheckCollision(struct Actor *object, struct Vector3 *position);
struct Actor *Actor_FindAtCell(struct Vector3 *position, s32 mode);
s32 PushTarget_FindSlide(struct PushTarget *target);
void PushTarget_Push(struct PushTarget target);

void Islets_PlayFootsteps(void);
void Islets_RideOnLonesomeGeorge(struct Actor *object);
void Actor_EmitSmoke(struct Actor *object);
void Islets_StartWaterShimmer(void);
void Actor_MoveToObject(s32 actor, s32 target);
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
    if (Event_GetFlag(0xaa5))
      gIslets_TurtleActorPlacements[1].talkFacing = 1;
    return gIslets_TurtleActorPlacements;
  }
  return gIslets_Placements;
}

/// Pushing the log off the ice: it falls into the water and makes a way back
/// for Pengulina.
void Islets_PushLogIntoWater(void) {
  struct PushTarget target;
  struct Actor *object = Actor_Get(ACTOR_LOG);

  if (PushTarget_FindSlide(&target)) {
    PushTarget_Push(target);
    System_Wait(5);
    object->gravity = px(1) / 2;
    Actor_SetAnimation(ACTOR_LOG, 3);
    Actor_SetDestinationOffset(ACTOR_LOG, 0, 8);
    FieldEvent_Wait(22);
    Event_PlaySound(SE_LOG_FALL);
    Actor_SetAnimation(ACTOR_LOG, 8);
    object->priorityFlags |= 2;
    object->motionFlags = 0;
    object->velocityY = 0;
    object->y.fixed = px(-8);
    object->groundHeight = px(-8);
    MapGrid_CopyCellAttributes(9, 1, 4, 1, 9, 10);
    MapGrid_CopyCellAttributes(9, 8, 4, 1, 9, 9);
    MapGrid_CopyCellAttributes(9, 64, 4, 1, 9, 74);
    Event_SetFlag(0xa2c);
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

void Islets_TalkToPengus(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(0xaa2)) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_00)) {
      Actor_FaceActor(ACTOR_PENGUS, gMainRoutineWork.leaderActorId, 0);
      FieldEvent_SetMessageId((s32)MsgPenPenPen);
    } else {
      FieldEvent_SetMessageId((s32)MsgPenPen2);
    }
    Event_ShowMessage(ACTOR_PENGUS, 0);
  } else {
    FieldActor_Stop(ACTOR_PENGUS);
    System_Wait(1);
    Actor_FaceActor(ACTOR_PENGUS, gMainRoutineWork.leaderActorId, 0);
    FieldEvent_SetMessageId((s32)MsgPenPen3);
    // "Pen pen!"
    Event_ShowMessage(ACTOR_PENGUS, 0);
    FieldActor_WatchAndSetScript(ACTOR_PENGUS, ACTOR_PENGULINA);
  }
  Event_End();
  return;
}

void Islets_MindReadPengus(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(0xaa2)) {
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

void Islets_TalkToPengulina(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(0xaa2)) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_00)) {
      Actor_FaceActor(ACTOR_PENGULINA, gMainRoutineWork.leaderActorId, 0);
      FieldEvent_SetMessageId((s32)MsgPenPenPehnnnn);
    } else {
      FieldEvent_SetMessageId((s32)MsgPenPen2);
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

void Islets_MindReadPengulina(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(0xaa2)) {
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

void Islets_TalkToHealer(void) {

  if (Leader_IsFacingNorth()) {
    Sanctum_Run(21);
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

void Islets_TalkToBird(void) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(0xaa4)) {
    FieldActor_WatchAndSetScript(ACTOR_BIRD, gMainRoutineWork.leaderActorId);
  }
  FieldEvent_SetMessageId((s32)MsgChirpChirp2);
  // "Chirp chirp!"
  Event_ShowMessage(ACTOR_BIRD, 0);
  Event_End();
  return;
}

/// The bird of SE Angara Islet: it takes the Pretty Stone and gives the Red
/// Cloth in return.
void Islets_TradeWithBird(void) {
  struct Actor *leader;
  struct Actor *object;
  struct Actor *spawned;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (FieldPartyInventory_FindMember(ITEM_PRETTY_STONE) == -1) {
    if (Event_GetFlag(0xaa4)) {
      FieldActor_WatchAndSetScript(ACTOR_BIRD, gMainRoutineWork.leaderActorId);
      FieldEvent_SetMessageId((s32)MsgWhatDoYou);
    } else {
      FieldEvent_SetMessageId((s32)MsgILoveFeeling);
    }
    Event_ShowMessage(ACTOR_BIRD, 0);
  } else {
    leader = Actor_Get(gMainRoutineWork.leaderActorId);
    object = Actor_Get(ACTOR_BIRD);
    Actor_FaceActor(ACTOR_BIRD, gMainRoutineWork.leaderActorId, 0);
    FieldActor_Jump(ACTOR_BIRD, 4, 10);
    FieldEvent_SetMessageId((s32)MsgOhMyThat);
    // "Oh my! That stone you have! It's divine! Simply divine! I must have..."
    Event_ShowMessage(ACTOR_BIRD, 0);
    Actor_SetAnimation(ACTOR_BIRD, 4);
    FieldActor_SetPositionAndResetMotion(
        object, (leader->x.fixed + object->x.fixed) / 2, leader->y.fixed,
        (leader->z.fixed + object->z.fixed) / 2);
    System_Wait(20);
    FieldActor_SetPositionAndResetMotion(object, px(120), px(128), px(312));
    Actor_SetAnimation(ACTOR_BIRD, 5);
    System_Wait(10);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
    spawned =
        FieldActor_Create(0x1e9, leader->x.fixed, px(152), leader->z.fixed);
    if (spawned != 0) {
      Actor_SetAnimation(gMainRoutineWork.leaderActorId, 28);
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
    Event_SetFlag(0xaa4);
  }
  Event_End();
}

// The Islets' trade of item 453 with object 10. With the item in the party,
// if flag 513 is set (asked before) the camera moves and object 10 asks again
// (MsgAreYouGiving); otherwise it notices the item (MsgOhThatRed), the leader
// steps up and shows it (object type 489, its picture set to the item by
// Actor_SetItemIcon) and object 10 asks. A yes hands the item across step by
// step, takes item 453 and gives item 454 (object 10 hands it back over the
// leader's head), sets flag 0xaa3 and counts the answer in the event work's
// halfword 0x1c4 the first time; a no sets flag 513. Without the item, object
// 10 says MsgYupANice after the trade or MsgIVeBeenFeeling before it.

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->steeringFlags &= ~1,                                  \
   Actor_WalkToAndWait(actor, x, z), Event_Wait(1),                            \
   Actor_GetById(actor)->steeringFlags |= 1)

/// Object 10 walks round and hands item 454 back over the leader's head.
#define HAND_BACK(leader, given, off)                                          \
  {                                                                            \
    Actor_SetSpeed(10, 0.8);                                                   \
    Actor_WalkToAndWait(10, 128, 164);                                         \
    Actor_FaceDirectionAndRefreshById(10, FACING_WEST);                        \
    Event_Wait(20);                                                            \
    Actor_SetSpeed(10, 1.6);                                                   \
    STEP(10, 108, 168);                                                        \
    given = FieldActor_Create(489, leader->x.fixed, leader->y.fixed + px(24),  \
                              leader->z.fixed);                                \
    if (given != 0) {                                                          \
      Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 28);              \
      given->motionFlags = off;                                                \
      Actor_SetItemIcon(given, ITEM_MILK);                                     \
      Event_Wait(20);                                                          \
    }                                                                          \
    STEP(10, 116, 164);                                                        \
    FieldPsynergy_RestoreMindReadPalette();                                    \
    PartyInventory_Remove(ITEM_RED_CLOTH);                                     \
    PartyInventory_GiveItem(ITEM_MILK, 0);                                     \
    if (given != 0) {                                                          \
      Actor_Release(given);                                                    \
      Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_STAND);      \
    }                                                                          \
    FieldPsynergy_GreyMindReadPalette();                                       \
    GameFlag_Set(0xaa3);                                                       \
  }

/// The cow of N Osenia Islet: it notices the Red Cloth ("Oh! That red
/// neckerchief...") and asks for it; a yes trades it for Milk, handed back
/// over the leader's head, and a no leaves it to ask again. Without the cloth
/// it talks of how it feels, or after the trade of the neckerchief.
void Islets_TradeWithCow(void) {
  struct Actor *item, *leader, *given;
  const u8 *asked, *noticed;
  /* FAKEMATCH(temporary): zero wrapped in a struct of one u16. */
  struct {
    u16 v;
  } zero;

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
        item = FieldActor_Create(489, px(104), px(24), px(160));
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
      item = FieldActor_Create(489, px(140), 0, px(190));
      if (item != 0) {
        Actor_SetItemIcon(item, ITEM_RED_CLOTH);
        Actor_SetSpriteFlags(item, 0);
        item->motionFlags = 0;
        zero.v = 0;
        item->retainSpriteCache = 1;
      }
      STEP(gMainRoutineWork.leaderActorId, 120, 190);
      Actor_FaceDirectionById(ACTOR_COW, FACING_SOUTH_SOUTHEAST, 0);
      Actor_Jump(ACTOR_COW, 4, 10);
      Event_ShowMessageAndWait(ACTOR_COW, 0);
      Actor_WatchAndSetScript(ACTOR_COW, gMainRoutineWork.leaderActorId);
      STEP(gMainRoutineWork.leaderActorId, 132, 190);
      FieldActor_SetPositionAndResetMotion(item, px(195), 0, px(115));
      STEP(gMainRoutineWork.leaderActorId, 120, 190);
      Event_Wait(20);
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 98, 168);
      Actor_Jump(gMainRoutineWork.leaderActorId, 4, 10);
      Actor_SetPositionAndResetMotion(item, px(104), px(24), px(160));
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
        HAND_BACK(item, item, zero.v)
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
    if (Event_GetFlag(0xaa3))
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

struct Actor *Islets_ShowItemAbove(struct Actor *object, s32 value) {
  struct Actor *spawned;

  spawned = FieldActor_Create(489, object->x.fixed, object->y.fixed + px(24),
                              object->z.fixed);
  if (spawned != 0) {
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, 28);
    spawned->motionFlags = 0;
    FieldActor_SetItemIcon(spawned, value);
    Actor_SetSpriteFlags(spawned, 0);
    spawned->retainSpriteCache = 1;
  }
  return spawned;
}

void Actor_EmitSmoke(struct Actor *object) {
  struct Actor *spawned;

  if ((gFrameCount & 3) == 0) {
    spawned = FieldActor_Create(14, object->x.fixed, object->y.fixed,
                                object->z.fixed);
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
  (Actor_GetById(actor)->steeringFlags &= ~1,                                  \
   Actor_WalkToAndWait(actor, x, z), FieldEvent_Wait(1),                       \
   Actor_GetById(actor)->steeringFlags |= 1)

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
      item = (struct Actor *)Islets_ShowItemAbove((s32)leader, 454);
      trader->update = Actor_EmitSmoke;
      Actor_RunScriptAndWait(ACTOR_DOG, gIslets_BirdFliesOffScript);
      trader->update = 0;
    } else {
      Actor_FaceDirection(ACTOR_DOG, FACING_WEST, 0);
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 264, 856);
      Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
      item = (struct Actor *)Islets_ShowItemAbove((s32)leader, 454);
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
      FieldActor_SetPositionAndResetMotion(item, px(104), 0, px(840));
      emitSmoke = Actor_EmitSmoke;
      if (right) {
        leader->update = emitSmoke;
        Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 296, 840);
        leader->update = 0;
        Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_WEST);
        leader->update = emitSmoke;
        STEP(gMainRoutineWork.leaderActorId, 288, 840);
        FieldActor_SetPositionAndResetMotion(item, px(280), 0, px(840));
        STEP(gMainRoutineWork.leaderActorId, 296, 840);
        leader->update = 0;
        FieldActor_Stop(gMainRoutineWork.leaderActorId);
        System_Wait(1);
        trader->update = emitSmoke;
        STEP(11, 292, 844);
      } else {
        leader->update = emitSmoke;
        Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 264, 840);
        leader->update = 0;
        Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
        leader->update = emitSmoke;
        STEP(gMainRoutineWork.leaderActorId, 272, 840);
        FieldActor_SetPositionAndResetMotion(item, px(280), 0, px(840));
        STEP(gMainRoutineWork.leaderActorId, 264, 840);
        leader->update = 0;
        FieldActor_Stop(gMainRoutineWork.leaderActorId);
        System_Wait(1);
        trader->update = emitSmoke;
        STEP(11, 268, 844);
      }
    }
    giftItem = 455;
    gift = (struct Actor *)Islets_ShowItemAbove((s32)leader, giftItem);
    STEP(11, 280, 856);
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
    Event_SetFlag(0xaa1);
    Event_SetFlag(FLAG_SCENE_TEMP_16);
  } else {
    if (Event_GetFlag(0xaa1)) {
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

void Islets_RideOnLonesomeGeorge(struct Actor *object) {
  struct Actor *source = Actor_Get(ACTOR_LONESOME_GEORGE);

  FieldActor_SetPositionAndResetMotion(
      object, source->x.fixed, source->y.fixed + px(16), source->z.fixed);
  object->facing = source->facing;
}

#if !TLA_PATCH_1
#define ISLETS_GIFT_TRANSITION 521
#else
#define ISLETS_GIFT_TRANSITION 513
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
  gift->speedLimit = 0x19999;
  gift->acceleration = 0xcccc;
  gift->update = Actor_EmitSmoke;
  Actor_SelectAnimation(gift, 2);
  FieldActor_SetMoveTarget(gift, 278, 0, 452);
  FieldActor_WaitForMove(gift);
  Actor_SelectAnimation(gift, 1);
  gift->update = 0;
  PartyInventory_Remove(ITEM_LIL_TURTLE);
  // "Hop on my back!"
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  ((u8 *)HeapSlot_AllocEwram(HEAP_SLOT_MIND_READ, 664))[661] = 99;
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  leader->update = Actor_EmitSmoke;
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 312, 446);
  leader->update = 0;
  FieldActor_Jump(gMainRoutineWork.leaderActorId, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 296, 452);
  leader->update = Islets_RideOnLonesomeGeorge;
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId), 15);
  Actor_SetAnimation(ACTOR_LONESOME_GEORGE, 3);
  FieldEvent_Wait(20);
  // "And we're off!"
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  Actor_SetSpeed(ACTOR_LONESOME_GEORGE, 0.8);
  gift->speedLimit = 0xcccc;
  gift->acceleration = 0x6666;
  guide->update = Actor_EmitSmoke;
  Actor_SetDestination(ACTOR_LONESOME_GEORGE, 288, 538);
  FieldEvent_Wait(20);
  FieldPsynergy_RestoreMindReadPalette();
  Islets_StopWaterShimmer();
  gift->update = Actor_EmitSmoke;
  Actor_SelectAnimation(gift, 2);
  FieldActor_SetMoveTarget(gift, 288, 0, 538);
  FieldEvent_Wait(20);
  gFieldWork->transition = ISLETS_GIFT_TRANSITION;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(3);
  Event_SetFlag(0xaa5);
}

// The value stored at the event work's 0x1ac: 521 in the Japanese release,
// 513 from patch 1.
#if TLA_PATCH_1
#define STORED_AT_1AC 513
#else
#define STORED_AT_1AC 521
#endif

#define SPAWN(a, b, answer)                                                    \
  {                                                                            \
    a = FieldActor_Create(163, px(296), 0, px(424));                           \
    b = FieldActor_Create(14, px(296), 0, px(424));                            \
    if (a != 0) {                                                              \
      a->motionFlags = answer;                                                 \
      Actor_SetSpriteFlags(a, 0);                                              \
    }                                                                          \
    if (b != 0) {                                                              \
      b->scaleY = Q16_ONE * 2;                                                 \
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
        SPAWN(a, b, answer)
        STEP(gMainRoutineWork.leaderActorId, 318, 424);
        leader->update = 0;
        Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHWEST,
                            0);
        Islets_LonesomeGeorgeThanks(a);
      } else {
        FieldPsynergy_GreyMindReadPalette();
        Event_SetMessageId(message - 1);
        goto refused;
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
  } else if (GameFlag_Get(0xaa5)) {
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
      ((u8 *)HeapSlot_AllocEwram(HEAP_SLOT_MIND_READ, 664))[0x295] = 99;
      FieldActor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
      Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST);
      Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
      FieldActor_Jump(gMainRoutineWork.leaderActorId, 6, 0);
      Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 296, 452);
      leader->update = Islets_RideOnLonesomeGeorge;
      FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                                15);
      Actor_SetAnimation(ACTOR_LONESOME_GEORGE, 3);
      FieldEvent_Wait(20);
      // "And we're off!"
      Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
      FieldPsynergy_RestoreMindReadPalette();
      Actor_SetSpeed(ACTOR_LONESOME_GEORGE, 0.8);
      Actor_SetSpeed(10, 0.8);
      Actor_GetById(ACTOR_LONESOME_GEORGE)->update = Actor_EmitSmoke;
      Actor_SetDestination(ACTOR_LONESOME_GEORGE, 288, 538);
      FieldEvent_Wait(40);
      FieldPsynergy_RestoreMindReadPalette();
      Islets_StopWaterShimmer();
      Actor_GetById(10)->update = Actor_EmitSmoke;
      Actor_WalkTo(10, 288, 538);
      FieldEvent_Wait(20);
      gFieldWork->transition = STORED_AT_1AC;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      FieldPsynergy_GreyMindReadPalette();
      FieldEvent_SetExitRequest(3);
    } else {
      FieldPsynergy_GreyMindReadPalette();
    refused:
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
  struct Actor *object = Actor_Get(ACTOR_LONESOME_GEORGE);
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
    ((struct {
      u8 unk000[0x295];
      u8 unk295;
    } *)HeapSlot_AllocEwram(HEAP_SLOT_MIND_READ, 0x298))
        ->unk295 = 99;
    FieldActor_Jump(gMainRoutineWork.leaderActorId, 6, 0);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
    Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 152, 768);
    leader->update = (void (*)(void))Islets_RideOnLonesomeGeorge;
    FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId), 15);
    Actor_SetAnimation(ACTOR_LONESOME_GEORGE, 3);
    FieldEvent_Wait(20);
    FieldPsynergy_RestoreMindReadPalette();
    Actor_SetSpeed(ACTOR_LONESOME_GEORGE, 1.6);
    Actor_SetSpeed(10, 1.6);
    update = (void (*)(void))Actor_EmitSmoke;
    done = (u8 *)object + 99;
    *done = 0;
    object->update = update;
    Actor_SetScript(ACTOR_LONESOME_GEORGE, gIslets_GeorgeWalksScript);
    FieldEvent_Wait(30);
    Actor_Get(10)->update = update;
    Actor_SetScript(10, gIslets_TurtleWalksScript);
    do {
      System_Wait(1);
    } while (*done == 0);
    Islets_StopWaterShimmer();
    System_Wait(1);
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
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

void Islets_SetLedgeBlocksByHeight(void) {
  if (Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed < px(40)) {
    Actor_Get(11)->collisionFlags |= 8;
    Actor_Get(12)->collisionFlags |= 8;
  } else {
    Actor_Get(11)->collisionFlags &= ~8;
    Actor_Get(12)->collisionFlags &= ~8;
  }
}

extern u8 gIslets_WaveTableIndex;
extern s16 gIslets_WaveLines[2][160];

/// Applies BG1 horizontal line scrolling with HBlank DMA.
void Islets_StartWaveDma(void) {
  s16 *lines = gIslets_WaveLines[gIslets_WaveTableIndex];
  s32 first = lines[0];
  volatile u16 *scroll;
  volatile u16 *dma;

  /* FAKEMATCH(barrier): an asm reading the first line keeps its load before the
   * register address is built. */
  __asm__("" : : "r"(first));
  scroll = &REG_BG1HOFS;
  *scroll = first;
  dma = REG_DMA0;

  dma[5] &= 0xc5ff; // Clear start timing and repeat.
  dma[5] &= 0x7fff; // Stop.
  lines++;
  (void)dma[5];
  Dma_Set(lines, (void *)scroll, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)dma);
}

void Islets_FillWaveLines(void) {
  s16 *layer = (s16 *)&Ram_HeapSlots->mapGrid->layers[2];
  s16 *lines = gIslets_WaveLines[gIslets_WaveTableIndex ^ 1];
  s32 v[2];
  s32 *count = &v[1];
  s32 value;
  s32 sine;

  for (*count = 0; *count != 160; (*count)++) {
    /* FAKEMATCH(dead): the sine's call and store in a once-only block. */
    do {
      sine = Math_Sin((*count + *(u8 *)&gFrameCount + layer[3]) << 9);
      v[0] = sine;
    } while (0);
    value = layer[1] + (sine >> 15);
    value += *count % 3;
    *lines++ = value - 1;
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
  struct Actor *object = Actor_GetById(ACTOR_PENGULINA);
  struct Actor *leader;
  struct Actor *item;
  s32 z;

  if (object->x.fixed < px(228) || object->x.fixed > px(236))
    return;
  z = object->z.fixed;
  if (z < 0xa40000)
    return;
  if (z > 0xac0000)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(ACTOR_PENGULINA, 1);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 216, 184);
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.leaderActorId, 9);
  FieldEvent_SetMessageId((s32)MsgThankYouSo);
  FieldActor_Jump(ACTOR_PENGUS, 2, 10);
  FieldActor_Jump(ACTOR_PENGUS, 4, 10);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PENGUS, FACING_WEST, 10);
  // "Thank you so much for saving my sweet Pengulina. Allow me to give..."
  Event_ShowMessage(ACTOR_PENGUS, 0);
  Actor_GetById(ACTOR_PENGUS)->steeringFlags &= ~1;
  Actor_WalkToAndWait(ACTOR_PENGUS, 222, 186);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_PENGUS)->steeringFlags |= 1;
  Actor_GetById(ACTOR_PENGUS)->steeringFlags &= ~1;
  Actor_WalkToAndWait(ACTOR_PENGUS, 232, 186);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_PENGUS)->steeringFlags |= 1;
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 28);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  item = Actor_GetById(20);
  item->motionFlags = 0;
  FieldActor_SetItemIcon(item, ITEM_PRETTY_STONE);
  FieldActor_SetPositionAndResetMotion(
      item, leader->x.fixed, leader->y.fixed + px(24), leader->z.fixed);
  FieldEvent_Wait(20);
  FieldPsynergy_RestoreMindReadPalette();
  Party_GiveItem(ITEM_PRETTY_STONE, 0);
  FieldPsynergy_GreyMindReadPalette();
  Actor_SetPosition(20, 0, 0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  FieldPsynergy_StopMindReadWave();
  Actor_FaceDirectionAndWait(ACTOR_PENGUS, FACING_NORTH_NORTHWEST);
  Event_SetFlag(0xaa2);
  Event_End();
}

void Islets_PlayFootsteps(void) {
  if ((gFrameCount & 15) == 0)
    Event_PlaySound(SE_FOOTSTEP);
}

/// Coming near the nest, the bird flies back onto it.
void Islets_BirdReturnsToNest(void) {
  struct Actor *object = Actor_Get(ACTOR_BIRD);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_Stop(ACTOR_BIRD);
  System_Wait(1);
  object->motionFlags = 0;
  Actor_SetSpeed(ACTOR_BIRD, 1.6);
  FieldActor_SetPositionAndResetMotion(object, px(268), px(64), px(390));
  System_Wait(1);
  object->groundHeight = px(128);
  System_Wait(1);
  object->y.fixed = px(160);
  System_Wait(1);
  FieldTask_AddFrame(Islets_PlayFootsteps, TASK_PASS_VBLANK);
  FieldActor_SetMoveTarget(object, 132, 128, 320);
  FieldActor_WaitForMove(object);
  FieldTask_Remove(Islets_PlayFootsteps);
  Actor_SetAnimation(ACTOR_BIRD, 5);
  object->motionFlags = 3;
  FieldActor_SetSpritePriority(ACTOR_BIRD, 2);
  Actor_FaceActor(ACTOR_BIRD, gMainRoutineWork.leaderActorId, 0);
  FieldEvent_Wait(10);
  FieldActor_Jump(ACTOR_BIRD, 4, 0);
  Actor_MoveToAndWait(ACTOR_BIRD, 120, 312);
  Actor_SetAnimation(ACTOR_BIRD, 5);
  object->motionFlags = 0;
  Actor_FaceDirectionAndWait(ACTOR_BIRD, FACING_EAST);
  Event_SetFlag(FLAG_SCENE_TEMP_15);
  System_Wait(1);
  Event_End();
}

void Islets_JumpDownLedge(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_JumpDownLedge(gMainRoutineWork.leaderActorId, 6, 1);
  Event_End();
  return;
}

void Leader_SetLayer(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  leader->layer = 2;
}

void Islets_StepOntoGround(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  leader->layer = 0;
}

void Islets_SetCellCollision(s32 x, s32 z, s32 layers, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cells0 = map->layers[0].cells;
  struct MapCell *cells2 = map->layers[2].cells;
  s32 index = x / 0x100000 + (z / 0x100000) * 128;

  if (layers & 1) {
    cells0 += index;
    cells0->collisionCode = value;
  }
  if (layers & 2) {
    cells2 += index;
    cells2->collisionCode = value;
  }
}

/// Keeps the two crates' places as they are pushed; a crate pushed into the
/// water at (7, 28) or (12, 25) floats there, the way over it open.
void Islets_StartPushingCrate(void) {
  struct Actor *object;
  s32 *x0;
  s32 *z0;
  s32 x1;
  s32 z1;

  object = Actor_Get(ACTOR_CRATE_A);
  x0 = &gIslets_FirstCrateX;
  z0 = &gIslets_FirstCrateZ;
  *x0 = object->x.fixed;
  *z0 = object->z.fixed;
  object = Actor_Get(ACTOR_CRATE_B);
  gIslets_SecondCrateX = x1 = object->x.fixed;
  gIslets_SecondCrateZ = z1 = object->z.fixed;
  if (*x0 >> 20 == 7 && *z0 >> 20 == 29 && x1 >> 20 == 7 && z1 >> 20 == 28)
    Islets_SetCellCollision(*x0, *z0, 2, 0);
  if (gIslets_FirstCrateX >> 20 == 12 && gIslets_FirstCrateZ >> 20 == 25 &&
      gIslets_SecondCrateX >> 20 == 12 && gIslets_SecondCrateZ >> 20 == 26)
    Islets_SetCellCollision(gIslets_SecondCrateX, gIslets_SecondCrateZ, 2, 0);
}

void Islets_FinishPushingCrate(void) {
  struct Actor *first = Actor_Get(ACTOR_CRATE_A);
  struct Actor *second = Actor_Get(ACTOR_CRATE_B);
  s32 firstX = FIELD_CELL(first->x.fixed);
  s32 firstZ = FIELD_CELL(first->z.fixed);
  s32 secondX = FIELD_CELL(second->x.fixed);
  s32 secondZ = FIELD_CELL(second->z.fixed);
  s32 both;

  if (gIslets_FirstCrateX >> 20 == 12 && gIslets_FirstCrateZ >> 20 == 24) {
    Islets_SetCellCollision(gIslets_FirstCrateX, gIslets_FirstCrateZ, 1, 21);
    Islets_SetCellCollision(gIslets_FirstCrateX, gIslets_FirstCrateZ, 2, 0);
  } else {
    s32 *z = &gIslets_FirstCrateZ;

    Islets_SetCellCollision(gIslets_FirstCrateX, *z, 3, 0);
  }
  if (gIslets_SecondCrateX >> 20 == 7 && gIslets_SecondCrateZ >> 20 == 27) {
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
    first->motionFlags = 3;
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
      ((u8 *)first)[0x62] = 1;
      ((u8 *)second)[0x62] = 1;
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
    second->motionFlags = 3;
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
      ((u8 *)first)[0x62] = 1;
      ((u8 *)second)[0x62] = 1;
    }
    Event_End();
  }
}

void Islets_SaveCratePositions(void) {
  struct Actor *first = Actor_Get(ACTOR_CRATE_A);
  struct Actor *second = Actor_Get(ACTOR_CRATE_B);

  gIslets_FirstCrateX = first->x.fixed;
  gIslets_FirstCrateZ = first->z.fixed;
  gIslets_SecondCrateX = second->x.fixed;
  gIslets_SecondCrateZ = second->z.fixed;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (gIslets_FirstCrateX >> 20 == 7 && gIslets_FirstCrateZ >> 20 == 29 &&
      gIslets_SecondCrateX >> 20 == 7 && gIslets_SecondCrateZ >> 20 == 28) {
    ((u8 *)first)[0x62] = 0;
    ((void (*)())Islets_SetCellCollision)(gIslets_FirstCrateX,
                                          gIslets_FirstCrateZ, 2);
  }
  Leader_PushObjectAhead();
  Islets_FinishPushingCrate();
  Event_End();
}

void Islets_FreezePuddle(void) {
  MapGrid_CopyCellAttributes(5, 2, 1, 2, 5, 28);
  Event_SetFlag(FLAG_SCENE_TEMP_13);
}

void Islets_StepUpOntoIceberg(void) {
  struct Actor *object;

  if (gInput.held & KEY_UP) {
    object = Actor_Get(gMainRoutineWork.leaderActorId);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    object->motionFlags = 0;
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 72, 280);
    object->motionFlags = 3;
    Event_End();
  }
}

void Islets_StepDownFromIceberg(void) {
  struct Actor *object;
  u16 *state;

  if (gInput.held & KEY_DOWN) {
    state = gSceneState;
    object = Actor_Get(gMainRoutineWork.leaderActorId);
    if ((u16)(*state + 0xfff6) > 200) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      object->motionFlags = 0;
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 72, 296);
      object->motionFlags = 3;
      Event_End();
    }
  }
}

void Islets_StartReveal(void) {
  Event_SetFlag(FLAG_SCENE_TEMP_18);
  return;
}

void Islets_EndReveal(void) {
  Event_ClearFlag(FLAG_SCENE_TEMP_18);
  return;
}

// The following six are from patch 1.
#if TLA_PATCH_1
void Islets_ClearFirstPatch(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 65, 0, 4, 4, 65, 10, 1);
  return;
}

void Islets_ClearSecondPatch(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 65, 0, 2, 2, 71, 10, 1);
  return;
}

void Islets_ClearThirdPatch(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 65, 0, 1, 1, 68, 15, 1);
  return;
}

void Islets_ClearFourthPatch(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 65, 0, 2, 3, 75, 20, 1);
  return;
}

void Islets_ClearFifthPatch(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 65, 0, 4, 1, 78, 22, 1);
  return;
}

void Islets_ClearSixthPatch(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 65, 0, 1, 1, 79, 23, 1);
  return;
}
#endif

void Islets_OpenDoorAndEnter(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  s16 *exit;
  s32 index;

  object = Actor_Get(party->leaderActorId);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetAnimation(party->leaderActorId, ANIM_WALK);
  Actor_SetSpeed(party->leaderActorId, 0.5);
  FieldActor_SetMoveTargetFixed(
      object,
      ((8 - (FIELD_PIXEL(object->x.fixed)) % 16) << 16) + object->x.fixed,
      object->y.fixed, object->z.fixed);
  FieldActor_WaitForMove(object);
  object->facing = FACING_NORTH;
  exit = &work->stepTrigger;
  index = *exit;
  Event_PlaySound(SE_DOOR_OPEN);
  index--;
  MapGrid_AnimateCells(gIslets_Doors[index].value,
                       gIslets_Doors[index].pair.first,
                       gIslets_Doors[index].pair.second);
  Actor_SetScript(party->leaderActorId, gIslets_EnterDoorScript);
  System_Wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(*exit);
}

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

void Islets_EnterDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;

  object = Actor_Get(party->leaderActorId);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetAnimation(party->leaderActorId, ANIM_WALK);
  Actor_SetSpeed(party->leaderActorId, 0.5);
  FieldActor_SetMoveTargetFixed(
      object,
      ((8 - (FIELD_PIXEL(object->x.fixed)) % 16) << 16) + object->x.fixed,
      object->y.fixed, object->z.fixed);
  FieldActor_WaitForMove(object);
  object->facing = FACING_NORTH;
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
  struct Actor *object = Actor_Get(ACTOR_LONESOME_GEORGE);

  Actor_MoveToObject(ACTOR_LONESOME_GEORGE, gMainRoutineWork.leaderActorId);
  Actor_SetPosition(10, 273, 520);
  System_Wait(1);
  object->facing = FACING_NORTH;
  owner->update = (void (*)(void))Islets_RideOnLonesomeGeorge;
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId), 15);
  Actor_SetAnimation(ACTOR_LONESOME_GEORGE, 3);
  System_Wait(1);
  work = Ram_HeapSlots->fieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(ACTOR_LONESOME_GEORGE, 0.6);
  Actor_SetSpeed(10, 0.6);
  Actor_Get(10)->update = (void (*)(void))Actor_EmitSmoke;
  object->update = (void (*)(void))Actor_EmitSmoke;
  Actor_WalkTo(10, 280, 468);
  Actor_MoveToAndWait(ACTOR_LONESOME_GEORGE, 296, 452);
  Actor_SetAnimation(10, 1);
  owner->update = 0;
  object->update = 0;
  Actor_Get(10)->update = 0;
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.leaderActorId, 9);
  FieldEvent_SetMessageId((s32)MsgHereWeAre);
  // "Here we are!"
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  Actor_SetAnimation(ACTOR_LONESOME_GEORGE, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId), 0);
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

#define EVENT_WORK ((u8 *)Ram_HeapSlots->fieldWork)
#define OBJECT(id) ((struct Actor *)((s32)Actor_Get((id))))

extern u32 gIslets_TurtleSwimsScript[];
extern u32 gIslets_TurtleTurnsScript[];

/// Lonesome George brings the party back from his secret spot: he swims in
/// with the leader, says "Well, here we are." and lifts him ashore. The
/// Japanese edition starts no screen transition here.
void Islets_ArriveBackAtIsland(void) {
  struct Actor *leader = OBJECT(gMainRoutineWork.leaderActorId);
  struct Actor *guide = OBJECT(9);
  u8 *done;

  Actor_MoveToObject(ACTOR_LONESOME_GEORGE, gMainRoutineWork.leaderActorId);
  Actor_SetPosition(10, 240, 540);
  System_Wait(1);
  guide->facing = FACING_SOUTH;
  leader->update = Islets_RideOnLonesomeGeorge;
  FieldActor_SetSpriteColor(((s32)Actor_Get(gMainRoutineWork.leaderActorId)),
                            15);
  Actor_SetAnimation(ACTOR_LONESOME_GEORGE, 3);
  System_Wait(1);
#if TLA_PATCH_1 /* The Japanese release leaves the transition word. */
  ((struct FieldWork *)EVENT_WORK)->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, 1);
#endif
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(ACTOR_LONESOME_GEORGE, 0.8);
  Actor_SetSpeed(10, 0.8);
  OBJECT(10)->update = Actor_EmitSmoke;
  guide->update = Actor_EmitSmoke;
  done = (u8 *)guide + 0x63;
  *done = 0;
  Actor_StartScript((s32)guide, gIslets_TurtleSwimsScript);
  Actor_SetScript(10, gIslets_TurtleTurnsScript);
  do
    System_Wait(1);
  while (*done == 0);
  leader->update = 0;
  guide->update = 0;
  OBJECT(10)->update = 0;
  Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_NORTH);
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.leaderActorId, 9);
  FieldEvent_SetMessageId(MsgWellHereWe);
  // "Well, here we are."
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  Actor_SetAnimation(ACTOR_LONESOME_GEORGE, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  FieldActor_SetSpriteColor(((s32)Actor_Get(gMainRoutineWork.leaderActorId)),
                            0);
  FieldActor_Jump(gMainRoutineWork.leaderActorId, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 152, 742);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
  // "When you want to go back to the island, just let me know."
  Event_ShowMessage(ACTOR_LONESOME_GEORGE, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_FaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH);
  Islets_StartWaterShimmer();
#if TLA_PATCH_1
  ((struct FieldWork *)EVENT_WORK)->transition =
      SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
#endif
}

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

void Islets_SetDockBlocksByHeight(void) {
  if (Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed > px(8)) {
    Actor_Get(9)->collisionFlags &= ~8;
    Actor_Get(10)->collisionFlags &= ~8;
  } else {
    Actor_Get(9)->collisionFlags |= 8;
    Actor_Get(10)->collisionFlags |= 8;
  }
}

void Islets_InitializeDog(void) {
  struct IsletsMapFlags *map = (struct IsletsMapFlags *)Ram_HeapSlots->mapGrid;
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  map->mode = 1;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_DOG), 0);
  Actor_SetSpeed(10, 0.8);
  Actor_Get(10)->collisionFlags |= 0x80;
  if (Event_GetFlag(0xaa1)) {
    spawned = FieldActor_Create(489, px(280), 0, px(840));
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

void Islets_StartIceberg(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetSpriteFlags(object, 0);
  object->retainSpriteCache = 3;
  object->motionFlags = 0;
  object->priorityFlags |= 0x80;
  object->acceleration = 0x2666;
  object->speedLimit = 0x4ccc;
}

void Islets_MoveIceberg(s32 id, s32 column, s32 row) {
  struct Actor *object = Actor_Get(id);
  s32 x = (column * 32 + object->timer) << 16;
  s32 z = (row * 32 + (s16)object->userValue) << 16;

  FieldActor_SetMoveTargetFixed(object, x, object->y.fixed, z);
}

/// The Islets' marching guards (objects 11 to 18): while the event work is busy
/// or flag 0x218 is set they stand still; once the controlled character drops
/// below height -12 they stand and the event work's counter 0x16a becomes 200;
/// otherwise, for each frame owed (gSceneState's halfword 2) while no event
/// runs, a 260-frame routine sets them walking with their facing steps
/// (Islets_MoveIceberg) at 0, still at 100 and 220, and walking in place at
/// 130.
void Islets_DriftIcebergs(void) {
  u16 *step = (u16 *)((u8 *)gSceneState);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u16 *owed = (u16 *)(((u8 *)gSceneState) + 2);
  s32 i;
  s32 count;

  if (work->psynergy != 0 || GameFlag_Get(FLAG_SCENE_TEMP_18)) {
    for (i = 11; i <= 18; i++)
      Actor_GetById(i)->paused = 1;
    return;
  }
  for (i = 11; i <= 18; i++)
    Actor_GetById(i)->paused = 0;
  if (Actor_GetById(gMainRoutineWork.leaderActorId)->y.fixed < -px(12)) {
    work = Ram_HeapSlots->fieldWork;
    for (i = 11; i <= 18; i++)
      Actor_GetById(i)->paused = 1;
    {
      s16 *countdown = &work->raisedTrigger;
      s32 ticks = 200;

      *countdown = ticks;
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
      for (i = 11; i <= 18; i++)
        Actor_SetAnimation(i, 2);
      Islets_MoveIceberg(11, 0, 1);
      Islets_MoveIceberg(12, 0, -1);
      Islets_MoveIceberg(13, 0, 1);
      Islets_MoveIceberg(14, 0, 1);
      Islets_MoveIceberg(15, -1, 0);
      Islets_MoveIceberg(16, 0, 1);
      Islets_MoveIceberg(17, 1, 0);
      Islets_MoveIceberg(18, 0, -1);
      break;
    case 100:
    case 220:
      for (i = 11; i <= 18; i++)
        Actor_SetAnimation(i, 1);
      break;
    case 130:
      for (i = 11; i <= 18; i++) {
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
  struct Actor *object;
  struct Actor *other;
  u16 *state;
  u16 *next;
  s32 i;

  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  i = 11;
  do {
    Islets_StartIceberg(i++);
  } while (i <= 18);
  state = gSceneState;
  next = &gSceneState[1];
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    *state = 0;
    *next = 0;
  }
  FieldTask_AddFrame(Islets_DriftIcebergs, TASK_PASS_VBLANK);
  if (Event_GetFlag(0xa2c)) {
    object = Actor_Get(ACTOR_LOG);
    Actor_SetPosition(ACTOR_LOG, 176, 168);
    Actor_SetAnimation(ACTOR_LOG, 4);
    object->priorityFlags |= 2;
    object->motionFlags = 0;
    object->y.fixed = px(-8);
    object->groundHeight = px(-8);
    MapGrid_CopyCellAttributes(9, 1, 4, 1, 9, 10);
    MapGrid_CopyCellAttributes(9, 64, 4, 1, 9, 74);
  } else {
    PushTarget_FillFootprint(ACTOR_LOG);
  }
  if (!Event_GetFlag(0xaa2)) {
    FieldActor_WatchAndSetScript(ACTOR_PENGUS, ACTOR_PENGULINA);
    Actor_SetAnimation(ACTOR_PENGULINA, 0);
    Actor_Get(ACTOR_PENGULINA)->steeringFlags &= ~1;
  } else if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    other = Actor_Get(ACTOR_PENGUS);
    other->facing = FACING_SOUTH_SOUTHWEST;
    other->collisionFlags |= 0x80;
    {
      s32 facing = FACING_SOUTH_SOUTHEAST;

      Actor_SetPositionAndFacing(ACTOR_PENGULINA, px(234), px(204), facing);
    }
    Actor_Get(ACTOR_PENGULINA)->collisionFlags |= 0x80;
    Actor_SetSpeed(ACTOR_PENGUS, 2);
    Actor_SetSpeed(ACTOR_PENGULINA, 1.6);
    Actor_SetScript(ACTOR_PENGUS, gIslets_PenguinsHopScript);
    Actor_SetScript(ACTOR_PENGULINA, gIslets_PenguinsHopScript);
    Event_SetFlag(FLAG_SCENE_TEMP_00);
  }
}

void Islets_InitializeCow(void) {
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == 3)
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_COW), 0);
  Actor_SetAnimation(ACTOR_COW, 4);
  if (Event_GetFlag(0xaa3)) {
    spawned = FieldActor_Create(489, px(104), px(24), px(160));
    if (spawned != 0) {
      spawned->motionFlags = 0;
      FieldActor_SetItemIcon(spawned, ITEM_RED_CLOTH);
      Actor_SetSpriteFlags(spawned, 0);
      spawned->retainSpriteCache = 1;
    }
    Actor_SetPosition(ACTOR_COW, 104, 204);
  }
  FieldActor_SetSpritePriority(ACTOR_BLUE_ORB, 1);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BLUE_ORB), 15);
  Actor_Get(ACTOR_BLUE_ORB)->priorityFlags |= 2;
}

// A room's setup.

/// Sets up the room's actors and map work.
void Islets_InitializePier(void) {
  struct IsletsMapWork *map = (struct IsletsMapWork *)Ram_HeapSlots->mapGrid;
  struct Actor *object = Actor_Get(ACTOR_BIRD);
  struct Actor *last;
  s32 set;
  s32 zero = 0;

  map->value190 = zero;
  map->value194 = zero;
  if (gMainRoutineWork.entrance == 11)
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    object->motionFlags = zero;
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
    ((u8 *)first)[0x62] = 1;
    ((u8 *)second)[0x62] = 1;
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
  Actor_DisableById(14);
  last = Actor_Get(14);
  {
    s32 still = 0;

    ((u8 *)last)[0x62] = 1;
    last->motionFlags = still;
  }
  Islets_SetCellCollision(last->x.fixed, last->z.fixed, 1, 255);
  if (Event_GetFlag(FLAG_SCENE_TEMP_13))
    MapGrid_CopyCellAttributes(5, 2, 1, 2, 5, 28);
}

struct MapLayerBits {
  u8 unk00[23];
  u8 unk17Low : 2;
  u8 priority : 2;
  u8 unk17High : 4;
};

void Islets_StartPaletteCycle(void);

void Islets_InitializeTurtle(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapGrid;
  s32 done;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  ((struct MapLayerBits *)map)->priority = 1;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  Islets_StartPaletteCycle();
  PaletteCycle_Add(3, 12, 5, 4);
  PaletteCycle_Add(4, 12, 5, 4);
  if (Event_GetFlag(0xaa5) &&
      (done = Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) == 0) {
    Actor_SetPosition(10, 280, 468);
    Actor_Get(10)->motionFlags = done;
    {
      s32 a1 = 0x1280000;

      Actor_SetPositionFixed(ACTOR_LONESOME_GEORGE, a1, px(452));
    }
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_LONESOME_GEORGE), 0);
  Actor_SetSpriteFlags(Actor_Get(11), 0);
  Actor_SetSpriteFlags(Actor_Get(12), 0);
  FieldTask_AddFrame(Islets_SetLedgeBlocksByHeight, TASK_PASS_VBLANK);
  if ((u16)(gMainRoutineWork.entrance - 4) <= 1) {
    Event_SetFlag(FLAG_SCENE_TEMP_17);
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
      {
        s32 a3 = 0x4000;

        Actor_SetPositionAndFacing(ACTOR_LONESOME_GEORGE, px(152), px(768), a3);
      }
      Actor_SetPosition(10, 176, 777);
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

s32 Scene_PrepareMap(void) { return 0; }

void Islets_StartPaletteCycle(void) {
  struct PaletteCycleWork *queue;
  struct PaletteCycle *entry;
  s16 i, j;

  queue = HeapSlot_AllocEwram(HEAP_SLOT_PALETTE_CYCLES,
                              sizeof(struct PaletteCycleWork));
  entry = queue->entries;
  i = 0;
outer:
  entry->source = 0;
  entry->position = 0;
  entry->timer = 0;
  entry->delay = 0;
  entry->count = 0;
  j = 0;
inner:
  entry->colors[(u16)j] = 0;
  {
    s32 next = ((u16)j + 1) << 16;

    j = next >> 16;
    if (next != 16 << 16)
      goto inner;
  }
  entry++;
  {
    s32 next = (i << 16) + 0x10000;

    i = next >> 16;
    if (next != 4 << 16)
      goto outer;
  }
  queue->count = 0;
}

#define gPaletteWork ((struct PaletteCycleWork *)Ram_HeapSlots->paletteCycles)

#include "../field/palqueue_add.c"
#undef gPaletteWork

#include "text/msg_ids.h"

void Islets_StartWaterShimmer(void) {
  Task_AddFrame(&Islets_ShimmerWater, TASK_PASS_VBLANK);
  return;
}

void Islets_StopWaterShimmer(void) {
  FieldTask_Remove(Islets_ShimmerWater);
  return;
}

#define gPaletteWork ((struct PaletteCycleWork *)Ram_HeapSlots->paletteCycles)
#define EVENT_S16(offset) (*(s16 *)((u8 *)Ram_HeapSlots->fieldWork + (offset)))

void Islets_ShimmerWater(void) {
  if (EVENT_S16(0x166) == 0 && EVENT_S16(0x15a) == 0 && EVENT_S16(0x15e) == 0) {
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

void Islets_DropThroughHole(void) {
  s32 v5;
  struct Actor *object0;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_DROP_THROUGH_HOLE);
  v5 = &gMainRoutineWork;
  v5 = (v5 + 0x214);
  object0 = Actor_Get(*(s32 *)((u8 *)v5 + 0));
  Actor_SetSpriteFlags((s32)object0, 0);
  Actor_SetAnimation(*(s32 *)((u8 *)v5 + 0), 18);
  FieldEvent_Wait(40);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(3);
  return;
}

void Islets_JumpForward(void) {
  Field_TryJumpForward();
  return;
}

// Rewrites the map cells under a pushable actor. The actor's sprite
// shape picks one of the six footprints in gPushFootprints (four signed bytes
// each, in sixteenths of a cell); the cells under it are copied from the
// map's spare area (Map_CopyCellAttributeRect) and byte 2 of both collision
// layers there is filled with 255 (Map_FillCellRectByte2). Returns 1, or 0 for
// a shape with no footprint.

extern s8 gPushFootprints[][4];

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

/// Rewrites the map cells under a pushable actor. The actor's sprite shape
/// picks one of the six footprints in gPushFootprints (four signed bytes each,
/// in sixteenths of a cell); the cells under it are copied from the map's spare
/// area (Map_CopyCellAttributeRect) and byte 2 of both collision layers there
/// is filled with 255 (Map_FillCellRectByte2). Returns 1, or 0 for a shape with
/// no footprint.
s32 PushTarget_FillFootprint(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_GetById(id);
  s16 kind = *((struct ObjectAnimationState *)actor->sprite)->value28;
  s16 i = 0;
  s32 next;
  u32 index;
  s32 a, b;
  s32 fromX, fromZ;
  s32 width, height, x, z;

  if ((u16)kind != gPushKinds[(u16)i]) {
  again:
    next = (i + 1) * 65536;
    index = (u32)next >> 16;
    i = next >> 16;
    if (index <= 5 && (u16)kind != gPushKinds[index])
      goto again;
  }
  // fromX and b are reused for the next values they hold.
  fromX = (u16)i;
  if (fromX == 6)
    return 0;
  a = gPushFootprints[fromX][0];
  width = (Abs(a) + Abs(gPushFootprints[fromX][2])) >> 4;
  b = gPushFootprints[fromX][1];
  height = (Abs(b) + Abs(gPushFootprints[fromX][3])) >> 4;
  x = actor->x.fixed + (a << 16);
  z = actor->z.fixed + (b << 16);
  b = *(s32 *)(map + 0x148) >> 20;
  x >>= 20;
  z >>= 20;
  fromZ = *(s32 *)(map + 0x14c) >> 20;
  fromX = (u16)b;
  Map_CopyCellAttributeRect(x, z, width, height, x + fromX, z + (u16)fromZ);
  Map_FillCellRectByte2(0, x, z, width, height, 255);
  Map_FillCellRectByte2(2, x, z, width, height, 255);
  return 1;
}

void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, u32 width, u32 height,
                           s32 value) {
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

static __inline__ s32 RowByte(s8 *row, s32 index) { return row[index]; }

static __inline__ s32 BoxSpan(s32 kind, s32 start) {
  return (Abs(gPushFootprints[kind][start]) +
          Abs(gPushFootprints[kind][start + 2])) >>
         4;
}

static __inline__ s32 CallFunc_02003e2c(struct Actor *object,
                                        struct Vector3 *cell) {
  return MapGrid_CheckCollision(object, cell);
}

s32 PushTarget_FindSlide(struct PushTarget *push) {
  struct Actor *object;
  struct Vector3 cell;
  s32 direction, kind;
  s32 dx, dz, steps, i, j, offset, result;

  object = PushTarget_FindAhead(&direction, &push->id, &push->footprint);
  if (object != 0) {
    kind = push->footprint;
    /* FAKEMATCH(dead): the address of kind taken and dropped. */
    (void)&kind;
    {
      s32 width = BoxSpan(kind, 0);
      s32 height = BoxSpan(kind, 1);
      u8 *layer;
      s32 index;

      offset = gPushLeaderSteps[direction];
      dx = offset & 0xffff0000;
      dz = offset << 16;
      push->callback = 0;
      steps = 0;
      layer = &object->layer;
      *layer = 2;
      push->x = object->x.fixed + dx;
      push->z = object->z.fixed + dz;
      cell.y = object->y.fixed;
      for (;;) {
        s8 *table = gPushFootprints[0];

        index = kind * 4;
        /* FAKEMATCH(wrapper): the read through RowByte. */
        cell.z = push->z + (RowByte(table, index + 1) << 16);
        for (j = 0; j < height; j++) {
          cell.x = push->x + (gPushFootprints[0][index] << 16);
          for (i = 0; i < width; i++) {
            /* FAKEMATCH(wrapper): the call through CallFunc_02003e2c. */
            if (CallFunc_02003e2c(object, &cell) == 2)
              goto blocked;
            cell.x += px(16);
          }
          cell.z += px(16);
        }
        push->x += dx;
        steps++;
        push->z += dz;
      }
    blocked:
      *layer = 0;
    }
    result = 0;
    if (steps != 0) {
      push->x = object->x.fixed + dx * steps;
      result = 1;
      push->y = object->y.fixed;
      push->z = object->z.fixed + dz * steps;
    }
  } else
    result = 0;
  return result;
}

/// The whole-pixel half of a 16.16 coordinate.
#define HI(v) (((s16 *)&(v))[1])

struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor **entry;
  struct Actor *object;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->facing >> 12;
  i = 8;
  entry = &work->actors[8];
outer:
  object = *entry;
  k = 0;
inner:
  if (((struct ObjectAnimationState *)object->sprite)->value28[0] ==
      gPushKinds[k]) {
    x = (HI(leader->x.fixed) + (gPushLeaderSteps[*direction] >> 16)) >> 4;
    z = (HI(leader->z.fixed) + (s16)gPushLeaderSteps[*direction]) >> 4;
    left = (HI(object->x.fixed) + gPushFootprints[k][0]) >> 4;
    top = (HI(object->z.fixed) + gPushFootprints[k][1]) >> 4;
    right = (HI(object->x.fixed) + gPushFootprints[k][2]) >> 4;
    bottom = (HI(object->z.fixed) + gPushFootprints[k][3]) >> 4;
    if (left <= x && x < right && top <= z && z < bottom) {
      *kind = k;
      if (k & 1) {
        if (left != FIELD_CELL(leader->x.fixed)) {
          *slot = i;
          return object;
        }
      } else {
        if (top != FIELD_CELL(leader->z.fixed)) {
          *slot = i;
          return object;
        }
      }
    }
  }
  k++;
  if (k <= 5)
    goto inner;
  entry++;
  i++;
  if (i <= 63)
    goto outer;
  return NULL;
}

void Actor_StopAtPushTarget(struct Actor *object);

#define ABS(x) ((x) < 0 ? -(x) : (x))

/// Pushes an object to its target, the leader following, and moves its cells
/// with it.
void PushTarget_Push(struct PushTarget target) {
  u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *object = Actor_Get(target.id);
  s32 cell[3];
  s32 from[3];
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
  from[0] = object->x.fixed;
  from[2] = object->z.fixed;
  cell[0] = (object->x.fixed + (gPushFootprints[footprint][0] << 16)) >> 20;
  cell[2] = (object->z.fixed + (gPushFootprints[footprint][1] << 16)) >> 20;
  width = (ABS(gPushFootprints[footprint][0]) +
           ABS(gPushFootprints[footprint][2])) >>
          4;
  height = (ABS(gPushFootprints[footprint][1]) +
            ABS(gPushFootprints[footprint][3])) >>
           4;
  Map_FillCellRectByte2(0, cell[0], cell[2], width, height, 0);
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
    Actor_SelectAnimation(object, 3);
  else
    Actor_SelectAnimation(object, 2);
  Event_PlaySound(SE_HEAVY_MOVE);
  Actor_SetSpeedAndAcceleration(target.id, 0.5, 0.1);
  FieldActor_SetMoveTargetFixed(object, target.x, target.y, target.z);
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 2);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.3, 0.1);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId,
                             (gPushLeaderSteps[direction] << 16) >> 31,
                             (gPushLeaderSteps[direction] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(object);
  Event_PlaySound(SE_STOP_MUSIC_PLAYER_5);
  Event_PlaySound(SE_HEAVY_STOP);
  FieldActor_SetPositionAndResetMotion(object, target.x, object->y.fixed,
                                       target.z);
  Actor_SelectAnimation(object, 1);
  target.x = target.x + (gPushFootprints[footprint][0] << 16);
  target.z = target.z + (gPushFootprints[footprint][1] << 16);
  target.x >>= 20;
  target.z >>= 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  MapGrid_CopyCellAttributes(target.x, target.z, width, height,
                             originX + target.x, originZ + target.z);
  Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);
  Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (gPushFootprints[footprint][0] << 16)) >> 20;
  cell[2] = (from[2] + (gPushFootprints[footprint][1] << 16)) >> 20;
  originX += cell[0];
  originZ += cell[2];
  MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0], cell[2]);
  Map_FillCellRectByte2(2, cell[0], cell[2], width, height, 0);
  Event_NoOp();
}

void Actor_StopAtPushTarget(struct Actor *object) {
  struct Vector3 position;
  struct Actor *hit;
  u32 i;
  s32 angle = object->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position.x = object->x.fixed;
  position.y = object->y.fixed;
  position.z = object->z.fixed;
  FieldVector_AddPolarOffset(16, angle, &position);
  hit = Actor_FindAtCell(&position, 1);
  if (hit != 0) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)hit->sprite)->value28 == gPushKinds[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x.fixed,
                                         object->y.fixed, object->z.fixed);
  }
  position.x = object->x.fixed;
  position.y = object->y.fixed;
  position.z = object->z.fixed;
  if (MapGrid_CheckCollision(object, &position) > 0)
    FieldActor_SetPositionAndResetMotion(object, object->x.fixed,
                                         object->y.fixed, object->z.fixed);
}

#undef TLA_ISLETS_H
#undef STEP
#undef HAND_BACK
#undef ISLETS_GIFT_TRANSITION
#undef STORED_AT_1AC
#undef SPAWN
#undef EVENT_WORK
#undef OBJECT
#undef gPaletteWork
#undef EVENT_S16
#undef HI
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
      SC_ADD(SW_Z, -0x8000),                                                   
      SC_ADD(SW_SCALE_X, -Q16_ONE / 64),                                               
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),                                               
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

s32 gPushLeaderSteps[] = {                                                               
      PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),                  
      PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),                  
      PUSH_STEP(0, 16),  PUSH_STEP(-16, 0), PUSH_STEP(-16, 0),                 
      PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),                 
      PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16),                 
      PUSH_STEP(16, 0),                                                        
  };

u32 gIslets_DogWanderScript[] = {
#if !TLA_PATCH_1
    22, 30, 131, 22, 16, 0x6666,
#else
    22, 30, 129, 22, 16, 0x6666,
#endif
    22, 15, 0x6666, 39, 2, 4,
    0x1780000, 0, 0xd80000, 1, 4, 0x1880000,
    0, 0xe00000, 1, 39, 1, 0,
    1, 22, 3, 0x3000, 0, 80,
    39, 2, 4, 0x1780000, 0, 0xd80000,
    1, 4, 0x1780000, 0, 0xb80000, 1,
    39, 1, 0, 40, 13, 0xffff,
    0,
};

u32 gIslets_GullScript[] = {
    SC_ANIMATION(4),
    SC_SPEED(0.8),
    SC_ACCELERATION(0.2),
    SC_SET(SW_Y, px(64)),
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
    SC_SET(SW_VELOCITY_Y, px(4)),
    SC_MOVE_BY(0, 0, -16),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, px(4)),
    SC_MOVE_BY(-32, 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, px(4)),
    SC_MOVE_BY(0, 0, 32),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, px(4)),
    SC_MOVE_BY(32, 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, px(4)),
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
    SC_SET(SW_VELOCITY_Y, px(4)),
    SC_MOVE_BY(0, 0, 16),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, px(4)),
    SC_MOVE_BY(32, 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, px(4)),
    SC_MOVE_BY(0, 0, -32),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, px(4)),
    SC_MOVE_BY(-32, 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, px(4)),
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
    39, 2, 4, 0xb80000, 0, 0x3a80000,
    1, 39, 1, 1, 80, 39,
    2, 4, 0x580000, 0, 0x3a80000, 1,
    39, 1, 0, 80, 13, 0xffff,
    0,
};

s16 gIslets_DoorFrames[] = {
    92, 10, 1, 2, 6, 92, 8, 1,
    2, 6, -1,
};

s16 gIslets_NoDoorFrames[] = {
    -1,
};

struct IsletsExit gIslets_Doors[] = {
    {(s32)gIslets_NoDoorFrames, {64, 64}},
    {(s32)gIslets_DoorFrames, {82, 10}},
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 208, 0, 800, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(IsletsDog),
    SCENE_EXIT(1, WorldMap, 28),
    SCENE_EXIT(2, ChampaInteriors, 9),
    SCENE_EXITS(IsletsPrettyStone),
    SCENE_EXIT(1, WorldMap, 29),
    SCENE_EXIT(2, MikasallaInteriorsHouses, 5),
    SCENE_EXIT(3, IsletsPrettyStone, 1),
    SCENE_EXITS(IsletsCow),
    SCENE_EXIT(1, WorldMap, 30),
    SCENE_EXIT(2, WorldMap, 80),
    SCENE_EXIT(3, DailaInteriorsHouses, 7),
    SCENE_EXIT(4, WorldMap, 81),
    SCENE_EXITS(IsletsPier),
    SCENE_EXIT(10, WorldMap, 31),
    SCENE_EXIT(11, ChampaInteriors, 8),
    SCENE_EXITS(IsletsTurtle),
    SCENE_EXIT(1, WorldMap, 32),
    SCENE_EXIT(2, ApojiiIslandsShops, 6),
    SCENE_EXIT(3, IsletsTurtle, 4),
    SCENE_EXIT(4, IsletsTurtle, 3),
    SCENE_EXIT(5, IsletCaveMeld, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gIslets_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_DogActorPlacements[] = {
    ACTOR_PLACEMENT(73, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 744, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(77, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 920, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(77, CONDITION_ALWAYS, (s32)gIslets_PierPaceScript, 88, 0, 936, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 856, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_PrettyStoneActorPlacements[] = {
    ACTOR_PLACEMENT(71, CONDITION_ALWAYS, (s32)gIslets_DogWanderScript, 376, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(399, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7, 176, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(402, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(402, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(402, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(402, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(402, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(402, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(402, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(402, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(400, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 280, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_CowActorPlacements[] = {
    ACTOR_PLACEMENT(71, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 248, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(73, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 184, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COW, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 132, 0, 176, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, 7, 326, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_PierActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 296, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 280, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(405, CONDITION_ALWAYS, (s32)gIslets_GullScript, 360, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(288, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(288, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_TurtleActorPlacements[] = {
    ACTOR_PLACEMENT(75, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 168, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(162, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(163, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 202, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 266, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgArffArff2)},
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
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 30, 0xa2c,
     EVENT(Islets_PushLogIntoWater)},
    {TRIGGER_STEP, 20, 0xaa2, EVENT(Islets_PengusGivesPrettyStone)},
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
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgMooMoo2)},
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
     0x212, EVENT(Islets_StartPushingCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13,
     0x212, EVENT(Islets_StartPushingCrate)},
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
    {TRIGGER_TALK, 9, 0xaa5, MSGNO(MsgBlank3)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgYup2)},
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
