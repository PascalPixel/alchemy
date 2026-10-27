/// @file Babi Lighthouse and the road up to it: four maps in one overlay, in
/// three of the game's areas. SceneId_RoadToBabiLighthouse is the Road to Babi
/// Lighthouse, entered from the world map; SceneId_BabiLighthouseEntry is the
/// Babi Lighthouse Entry; SceneId_BabiLighthouseLuff and
/// SceneId_BabiLighthouseStoneDoor are both Babi Lighthouse. The four share one
/// exit table, the other getters choose by gSaveBlock.sceneId with the road as
/// the fallback, and BabiLighthouse_SetupMap restores the map being
/// entered. Retreat anywhere but on the road leads back to the Entry.
///
/// The Road to Babi Lighthouse has four workers and a carved stone pillar. Move
/// or Carry can set the pillar into its cell at (20, 12), where it rests 48
/// pixels up (BabiLighthouse_UpdateMovedPillar); Carry then drops it in a ring
/// of dust (BabiLighthouse_LandCarriedPillarInDust). A chest holds Lucky
/// Pepper.
///
/// At the Babi Lighthouse Entry two pillars stand on ledges and are pushed or
/// moved with Move; a leader left on a pillar's ledge cell jumps off
/// (BabiLighthouse_JumpFromLedge), and from TBS_PATCH_2 on the pillars also
/// sort against the leader (BabiLighthouse_MatchLeaderPriority). Growth grows a
/// sprout into a vine (FLAG_GROWTH_PASSAGE_OPEN). Two of Babi's soldiers, a
/// strongman and a bald, muscular man lie trembling on the ground
/// (gBabiLighthouse_TrembleScript), knocked down by the group the party is
/// following. Three exit regions drop the party to the floor below,
/// TunnelRuinsLadyStatues (tunnel_ruins.c), and the way back up comes out
/// beside the same holes.
///
/// BabiLighthouseLuff is three rooms joined by steps and a fourth behind a
/// locked door. Walking south into one of the pits at step triggers 25 to 28,
/// the leader jumps in and drops from above into a lower room, where he either
/// lands in dust (BabiLighthouse_LandWithParticles) or falls on through
/// another hole to the room below that (BabiLighthouse_FallToExit). Luff, a
/// Jupiter Djinni, starts its battle when spoken to.
///
/// BabiLighthouseStoneDoor is the cave before a stone door, and its two
/// events are ordered by flags: walking up to the door the first time plays
/// BabiLighthouse_FaranAsksPartyToOpenDoor
/// (FLAG_BABI_LIGHTHOUSE_ASKED_TO_FOLLOW), and the door's first opening plays
/// BabiLighthouse_ExplainTruthDoor (FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN);
/// BabiLighthouse_SetupMap stands Iodem, Faran and the onlookers where each
/// has left them. The door's switch shows only under Reveal
/// (BabiLighthouse_ShowTruthDoorSwitch, BabiLighthouse_HideTruthDoorSwitch);
/// searched under Reveal it is flipped and the door slides open
/// (FLAG_BABI_LIGHTHOUSE_DOOR_OPEN), and searched without it, it reads `The
/// truth of this door is open to those seeing with their true heart.` Walking
/// through the open door shuts it behind the party
/// (BabiLighthouse_CloseTruthDoor, FLAG_BEYOND_DOOR), and beyond it lies the
/// Tunnel Ruins (BABI_LIGHTHOUSE_STONE_DOOR_TO_TUNNEL_RUINS_GREAT_STATUE).

#include "save_block.h"
#include "version.h"
#include "heap_slots.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/babi_lighthouse.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/field/field.h"
#include "macros.h"

#define FIELD_STAGED_ACTOR_IMPORTS
#include "map_grid_copy.h"
#include "types.h"
#include "event_layer.h"
#include "scene_tables.h"
#include "djinn_ids.h"
#include "field_effect.h"
#include "pushed_actor.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fx_work.h"
#include "call.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "actor.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

/// BabiLighthouseStoneDoor, before the stone door: the door's two halves,
/// Iodem, the strongman who saw the kidnappers go through, a grey-haired man, a
/// young man and Faran.
enum {
  /// ★ Named from what the player sees: the left half of a dark stone door
  /// (its render)
  ACTOR_LEFT_DOOR = 8,
  /// ★ Named from what the player sees: the right half of the stone door (its
  /// render)
  ACTOR_RIGHT_DOOR = 9,
  /// ★ Named from the game: "I will trust Iodem once more..."
  /// (MsgBabiLighthouseIfYouCanOpenThisDoor)
  ACTOR_IODEM = 10,
  /// A strongman.
  /// ★ Named from the game: "When I saw them, they had already gone through to
  /// the other side." (MsgBabiLighthouseWhenISawThemTheyHad)
  ACTOR_WITNESS = 11,
  /// A burly grey-haired man with his arms folded.
  /// ★ Named from the game: "There is no way this door will open."
  /// (MsgBabiLighthouseIAmSorryButNotEven)
  ACTOR_DOUBTING_THE_DOOR = 12,
  /// A young man with brown hair in a purple shirt.
  /// ★ Named from the game: "her life may be in danger..."
  /// (MsgBabiLighthouseThisIsAllAHugeWaste)
  ACTOR_WORRIED_FOR_SHEBA = 13,
  /// ★ Named from the game: "There were strange folk going to the ruins...
  /// Faran followed them." (MsgBabiLighthouseThereWereStrangeFolkGoingTo)
  ACTOR_FARAN = 14
};

/// BabiLighthouseLuff's Djinni, Luff.
enum {
  /// ★ Named from the game: "Luff" (MsgDjinnName)
  ACTOR_LUFF = 8
};

/// The Entry (BabiLighthouseEntry): the two pillars on the ledges, the plant
/// for Growth and the four the kidnappers knocked down on their way in (two of
/// Babi's soldiers, a strongman and a bald, muscular man), lying on the ground,
/// each named by what he says. The pillars have the sprite of the pillars Move
/// or a push shifts in Venus Lighthouse.
enum {
  /// ★ Named from what the player sees: a stone pillar with a glowing yellow
  /// cap on the left ledge (its render)
  ACTOR_LEFT_PILLAR = 8,
  /// ★ Named from what the player sees: the same capped pillar on the right
  /// ledge (its render)
  ACTOR_RIGHT_PILLAR = 9,
  /// ★ Named from what the player sees: a small green plant, SPRITE_SPROUT (its
  /// render)
  ACTOR_PLANT = 10,
  /// A soldier lying on the ground.
  /// ★ Named from the game: "We were absolutely no match for them."
  /// (MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely)
  ACTOR_FALLEN_SOLDIER_NO_MATCH = 11,
  /// A strongman in a red top lying on the ground.
  /// ★ Named from the game: "Only two of those six ever attacked anyone..."
  /// (MsgBabiLighthouseOnlyTwoOfThoseSixEver)
  ACTOR_FALLEN_STRONGMAN_ONLY_TWO = 12,
  /// A soldier lying on the ground.
  /// ★ Named from the game: "They just kicked us out of their way"
  /// (MsgBabiLighthouseTheyJustKickedUsOutOf)
  ACTOR_FALLEN_SOLDIER_KICKED_ASIDE = 13,
  /// A bald, muscular man lying on the ground.
  /// ★ Named from the game: "That's why the ground is scorched."
  /// (MsgBabiLighthouseTheySetFireToEverythingThats)
  ACTOR_FALLEN_MAN_SCORCHED_GROUND = 14
};

/// The Road to Babi Lighthouse (RoadToBabiLighthouse): four workers (a bald,
/// muscular man, a strongman and two men with pickaxes), each named by what he
/// says, and the pillar for Move and Carry, with the sprite of the pillars
/// Carry takes across a gap in Venus Lighthouse.
enum {
  /// A bald, muscular man.
  /// ★ Named from the game: "Do we get today off?"
  /// (MsgBabiLighthouseWeHaventBeenOrderedToStart)
  ACTOR_WORKER_DAY_OFF = 8,
  /// A strongman in a red top.
  /// ★ Named from the game: "There were strange folk going to the ruins..."
  /// (MsgBabiLighthouseThereWereStrangeFolkGoingTo)
  ACTOR_WORKER_STRANGE_FOLK = 9,
  /// A man with a pickaxe.
  /// ★ Named from the game: "It's odd that Babi would build a lighthouse in a
  /// place like this." (MsgBabiLighthouseItsOddThatBabiWouldBuild)
  ACTOR_WORKER_ODD_PLACE = 10,
  /// A man with a pickaxe.
  /// ★ Named from the game: "Cutting these stones and stacking them on the
  /// ruins isn't easy." (MsgBabiLighthouseCuttingTheseStonesAndStackingThem)
  ACTOR_WORKER_CUTTING_STONES = 11,
  /// ★ Named from what the player sees: a short grey stone pillar carved with a
  /// spiral (its render)
  ACTOR_ROAD_PILLAR = 12
};

/// The lighthouse keeps two flags in the area flags from 0x300.
enum {
  /// Growth has grown the Entry's sprout into a vine.
  /// ★ Named from the game: "Growth" (MsgAbilityName)
  FLAG_GROWTH_PASSAGE_OPEN = 0x300,
  /// The party is on the far side of BabiLighthouseStoneDoor's stone door.
  /// ★ Named from the game: "Yes, they opened the stone door and entered!"
  /// (MsgBabiYoureSureTheyWentThrough)
  FLAG_BEYOND_DOOR = 0x301
};

/// What Actor_Prepare places at the far edge of a pit: a crate
/// drawn solid black (SPRITE_COLOR_BLACK) and translucent in front of the
/// actors, which shows the background through the leader where they
/// overlap, so that he vanishes below the edge as he drops over it. ★ Named
/// from what the player sees: the leader's body hidden as he drops into a pit,
/// and shown when the mask is left out (its render)
enum { PIT_MASK = SPRITE_CRATE };

/// A chest searched while facing the given way.
#define EXAMINE_CHEST_FACING(facing)                                           \
  (EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | (facing))

static void BabiLighthouse_CloseTruthDoor(void);
static void BabiLighthouse_RedrawLedgePillarCells(void);

/// Unused: a signed division nothing calls.
static __inline__ s32 UNUSED Entrance_Divide(s32 dividend, s32 divisor) {
  return dividend / divisor;
}

#include "common/staged_actor.h"

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Talking to the Djinni in BabiLighthouseLuff starts the battle with
/// Luff, a Jupiter Djinni.
static void BabiLighthouse_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(ACTOR_LUFF, DJINNI_LUFF);
}

#include "common/actor_waitidle.h"
#include "common/effect_advance.h"

/// Arriving by falling from the floor above: the leader drops in, lands with
/// a thud that shakes the screen and throws up a ring of dust, and is startled.
static void BabiLighthouse_LandWithParticles(void) {
  u32 particle;
  struct Actor *leader;
  struct EffectOptions options;
  s32 velocity[3];

  // Start above the room with movement disabled until the fall begins.
  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed = PIXELS(130);
  Actor_GetById(ACTOR_PARTY_LEADER)->gravity = 0x8000;
  Actor_GetById(ACTOR_PARTY_LEADER)->bounce = 0;
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(30);
  Sound_PlaySong(SE_DUST_PUFF);
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_WaitFrames(24);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  // Spread the landing particles around the leader before restoring bounce.
  options.palette = 7;
  options.update = (void (*)(struct Actor *))FieldEffect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (particle = 0; particle < 17; particle++) {
    SET_VECTOR(velocity, Trig_Cos(particle << 12), 0, Trig_Sin(particle << 12));
    velocity[0] += velocity[0] / 2;
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                      &options);
  }
  Sound_PlaySong(SE_THUMP);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Actor_GetById(ACTOR_PARTY_LEADER)->gravity = 0x10000;
  Actor_GetById(ACTOR_PARTY_LEADER)->bounce = 0x4000;
  Event_End();
}

/// Arriving at one of the drop entrances of BabiLighthouseLuff: the
/// leader drops in and falls on, out through the given exit.
static void BabiLighthouse_FallToExit(s32 exit) {
  struct Actor *leader;

  // Start above the room with movement disabled until the fall begins.
  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->y.fixed = PIXELS(130);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->gravity = 0x4000;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->bounce = 0;
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(10);
  Sound_PlaySong(SE_DUST_PUFF);
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->velocityY = -0x50000;
  Actor_WaitUntilIdle(Actor_Get(ACTOR_PARTY_LEADER));
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Import_EventSetExitRequest(exit);
  Event_End();
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_BabiLighthouseEntry) {
    return gBabiLighthouse_EntryEntrances;
  }
  if (sceneId == (s32)&SceneId_BabiLighthouseLuff) {
    return gBabiLighthouse_LuffEntrances;
  }
  if (sceneId == (s32)&SceneId_BabiLighthouseStoneDoor) {
    return gBabiLighthouse_StoneDoorEntrances;
  }
  return gBabiLighthouse_RoadEntrances;
}

/// Only the Entry has regions: the three spots that drop into the Tunnel
/// Ruins.
const struct SceneExitRegion *Scene_GetExitRegions(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_BabiLighthouseEntry) {
    return gBabiLighthouse_EntryRegions;
  }
  return 0;
}

/// The four maps share one exit table.
const u32 *BabiLighthouse_GetExits(void) { return gBabiLighthouse_Exits; }

const struct ActorTemplate *Scene_GetActorTemplates(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_BabiLighthouseEntry) {
    return gBabiLighthouse_EntryActorTemplates;
  }
  if (sceneId == (s32)&SceneId_BabiLighthouseLuff) {
    return gBabiLighthouse_LuffActorTemplates;
  }
  if (sceneId == (s32)&SceneId_BabiLighthouseStoneDoor) {
    return gBabiLighthouse_StoneDoorActorTemplates;
  }
  return gBabiLighthouse_RoadActorTemplates;
}

/// After Move on one of the pillars at the Babi Lighthouse Entry, or a push: a
/// leader left standing on a pillar's ledge cell jumps off it, and the pillars'
/// cells are redrawn.
static void BabiLighthouse_JumpFromLedge(void) {
  struct Actor *leader;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  Event_Start();
  if (((leader->x.fixed >> 20) == 6 || (leader->x.fixed >> 20) == 18) &&
      (leader->z.fixed >> 20) == 20) {
    leader->targetX = ACTOR_NO_TARGET;
    leader->targetZ = ACTOR_NO_TARGET;
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    {
      u16 facing = leader->facing;
      if ((u16)(facing + 0x4fff) <= 0x1fff ||
          (u16)(facing - 0x3001) <= 0x1fff) {
        Actor_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, 16, 0);
        Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
      } else {
        FieldActor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
        Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
      }
    }
  }
  BabiLighthouse_RedrawLedgePillarCells();
  Event_End();
}

/// Walking into one of the pillars at the Babi Lighthouse Entry pushes it.
static void BabiLighthouse_PushLedgePillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  BabiLighthouse_JumpFromLedge();
  Event_End();
}

/// Walking south off the ledge at the Babi Lighthouse Entry: pushes the pillar
/// below on the leader's side if it stands at the ledge's foot, and otherwise
/// climbs down.
static void BabiLighthouse_PushLedgePillarOrClimbDown(void) {
  struct Actor *pillar = Actor_GetById(ACTOR_RIGHT_PILLAR);

  if ((Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20) <= 12) {
    pillar = Actor_GetById(ACTOR_LEFT_PILLAR);
    if ((pillar->x.fixed >> 20) == 6) {
      if ((pillar->z.fixed >> 20) == 20) {
        BabiLighthouse_PushLedgePillar();
        return;
      }
    }
  } else {
    if ((pillar->x.fixed >> 20) == 18) {
      if ((pillar->z.fixed >> 20) == 20) {
        BabiLighthouse_PushLedgePillar();
        return;
      }
    }
  }
  Field_ClimbDown();
}

/// The leader jumps down into the pit ahead (FieldActor_JumpDownLedge),
/// and the screen closes behind him.
static void BabiLighthouse_JumpIntoPit(void) {
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

/// Walking south into the east pit of BabiLighthouseLuff's west room: the
/// leader jumps in and drops into the east room.
static void BabiLighthouse_DropFromWestRoomEastPit(void) {
  Event_Start();
  BabiLighthouse_JumpIntoPit();
  Import_EventSetExitRequest(
      BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_EAST);
  Event_End();
}

/// Walking south into the pit of BabiLighthouseLuff's east room: the pit mask
/// is placed by it, and the leader jumps in and drops into the south room.
static void BabiLighthouse_DropFromEastRoomPit(void) {
  Event_Start();
  Actor_Prepare(PIXELS(464), 0, PIXELS(290), PIT_MASK);
  BabiLighthouse_JumpIntoPit();
  Import_EventSetExitRequest(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_PIT);
  Event_End();
}

/// Walking south into the southwest pit of BabiLighthouseLuff's west room:
/// the pit mask and a capped pillar are placed by it, and the leader jumps in
/// and drops into the east room.
static void BabiLighthouse_DropFromWestRoomSouthwestPit(void) {
  Event_Start();
  Actor_Prepare(PIXELS(143), 0, PIXELS(290), PIT_MASK);
  Actor_Prepare(PIXELS(121), 0, PIXELS(286), SPRITE_CAPPED_PILLAR);
  BabiLighthouse_JumpIntoPit();
  Import_EventSetExitRequest(
      BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_SOUTHWEST);
  Event_End();
}

/// Walking south into the northwest pit of BabiLighthouseLuff's west room: the
/// leader jumps in and drops into the east room.
static void BabiLighthouse_DropFromWestRoomNorthwestPit(void) {
  Event_Start();
  BabiLighthouse_JumpIntoPit();
  Import_EventSetExitRequest(
      BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_NORTHWEST);
  Event_End();
}

/// Searching the locked door in BabiLighthouseLuff. "It seems to be locked."
static void BabiLighthouse_InspectLockedDoor(void) {
  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MsgBabiSeemsLocked, 1);
  Event_End();
}

/// A companion walks back to the leader and is moved out of sight to the map's
/// origin, so that the party walks as the leader alone again.
static inline void BabiLighthouse_WalkBackToLeader(s32 actorId) {
  struct Actor *leader;

  Import_ActorSetAnimation(actorId, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(actorId, leader->x.part.pixel, leader->z.part.pixel);
  }
  Actor_WaitForMoveById(actorId);
  Actor_SetPosition(actorId, 0, 0);
}

/// Walking up to the stone door in BabiLighthouseStoneDoor the first time:
/// Faran asks whether the kidnappers really went through it; the onlookers
/// argue, and Iodem, arriving with the party, tells who they are, and Faran,
/// doubting them, asks them to prove they can follow by opening the door.
static void BabiLighthouse_FaranAsksPartyToOpenDoor(void) {
  // Bring the party to the witnesses gathered around the sealed door.
  GameFlag_Set(FLAG_BABI_LIGHTHOUSE_ASKED_TO_FOLLOW);
  GameFlag_Set(FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED);
  Event_Start();
  Field_EndBusyAndReveal();
  Event_SetMessageId(MsgBabiYoureSureTheyWentThrough);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 296, 352);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_WaitFrames(10);
  FieldActor_LaunchFromLeader(ACTOR_IODEM, 16, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_GARET, -8, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_IVAN, 8, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_MIA, 24, 16, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_MIA);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  FieldCamera_SetSpeed(0x30000, 0x6000);
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(288), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_WITNESS, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_WITNESS, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  // The witnesses try the stone door and disagree about who passed through it.
  Actor_StartRepeatedMotion(ACTOR_WORRIED_FOR_SHEBA, 2);
  Actor_RunRepeatedMotion(ACTOR_DOUBTING_THE_DOOR, 2);
  Event_WaitFrames(40);
  Actor_StartRepeatedMotion(ACTOR_WORRIED_FOR_SHEBA, 2);
  Actor_RunRepeatedMotion(ACTOR_DOUBTING_THE_DOOR, 2);
  Event_WaitFrames(40);
  Actor_StartRepeatedMotion(ACTOR_WORRIED_FOR_SHEBA, 2);
  Actor_RunRepeatedMotion(ACTOR_DOUBTING_THE_DOOR, 2);
  Event_WaitFrames(40);
  Actor_ShowEmote(ACTOR_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Event_ShowMessage(ACTOR_DOUBTING_THE_DOOR, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_EAST, 0);
  Event_WaitFrames(25);
  Actor_RunRepeatedMotion(ACTOR_WORRIED_FOR_SHEBA, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_WORRIED_FOR_SHEBA, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_WEST, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_WORRIED_FOR_SHEBA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_WORRIED_FOR_SHEBA, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_WITNESS, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_WITNESS, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_WORRIED_FOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_SOUTH, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_WORRIED_FOR_SHEBA, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_WORRIED_FOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 75);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_SOUTH, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_SOUTH, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_FARAN, 0);
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(320), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  // The party learns where the kidnappers went and is asked to pursue them.
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_WaitFrames(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    gFieldWork->message++;
  } else {
    Event_WaitFrames(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    gFieldWork->message++;
    Event_ShowMessage(ACTOR_IODEM, 0);
  }
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetSpeed(ACTOR_FARAN, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_FARAN, 0, 16);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
  Event_WaitFrames(35);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_OpenMessage(ACTOR_FARAN, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_FARAN, 0);
    gFieldWork->message++;
  } else {
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    gFieldWork->message++;
    Event_ShowMessage(ACTOR_FARAN, 0);
  }
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_WORRIED_FOR_SHEBA, 2);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_WORRIED_FOR_SHEBA, 0x14ccc, 0xa666);
  Actor_WalkByAndWait(ACTOR_WORRIED_FOR_SHEBA, 0, 16);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_WORRIED_FOR_SHEBA, 0);
  Event_WaitFrames(10);
  Actor_SetSpeed(ACTOR_DOUBTING_THE_DOOR, 0x14ccc, 0xa666);
  Actor_WalkByAndWait(ACTOR_DOUBTING_THE_DOOR, 0, 16);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 50);
  Event_ShowMessage(ACTOR_DOUBTING_THE_DOOR, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Event_WaitFrames(25);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_OpenMessage(ACTOR_IODEM, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_WaitFrames(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    gFieldWork->message++;
  } else {
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    gFieldWork->message++;
    Event_ShowMessage(ACTOR_IODEM, 0);
  }
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
  Event_WaitFrames(35);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
  Event_WaitFrames(40);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_LinkPair(ACTOR_DOUBTING_THE_DOOR, ACTOR_WORRIED_FOR_SHEBA, 50);
  Import_ActorSetAnimation(ACTOR_DOUBTING_THE_DOOR, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_WORRIED_FOR_SHEBA, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_SOUTH, 0);
  Event_WaitFrames(20);
  Import_ActorSetAnimation(ACTOR_DOUBTING_THE_DOOR, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_WORRIED_FOR_SHEBA, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_SetSpeed(ACTOR_DOUBTING_THE_DOOR, 0x10000, 0x8000);
  Actor_SetSpeed(ACTOR_WORRIED_FOR_SHEBA, 0x10000, 0x8000);
  Actor_WalkBy(ACTOR_DOUBTING_THE_DOOR, 32, 0);
  Actor_WalkByAndWait(ACTOR_WORRIED_FOR_SHEBA, 32, 0);
  Actor_WalkBy(ACTOR_DOUBTING_THE_DOOR, 0, 16);
  Actor_WalkByAndWait(ACTOR_WORRIED_FOR_SHEBA, 16, 0);
  Actor_WalkTo(ACTOR_WORRIED_FOR_SHEBA, 344, 312);
  Actor_WalkToAndWait(ACTOR_DOUBTING_THE_DOOR, 344, 336);
  Import_ActorSetAnimation(ACTOR_WORRIED_FOR_SHEBA, ANIM_STAND);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_WEST, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_SetSpeed(ACTOR_FARAN, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_FARAN, 328, 312);
  Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
  Event_WaitFrames(30);
  Actor_SetSpeed(ACTOR_WITNESS, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_WITNESS, 328, 328);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
  Event_WaitFrames(20);
  // The companions walk back into the leader.
  Actor_SetSpeed(ACTOR_GARET, 0x13333, 0x9999);
  Actor_SetSpeed(ACTOR_IVAN, 0x13333, 0x9999);
  Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
  BabiLighthouse_WalkBackToLeader(ACTOR_GARET);
  BabiLighthouse_WalkBackToLeader(ACTOR_IVAN);
  BabiLighthouse_WalkBackToLeader(ACTOR_MIA);
  Event_WaitFrames(10);
  Event_End();
}

/// The stone door of BabiLighthouseStoneDoor opens before Iodem and Faran the
/// first time: everyone marvels, the door shuts again in front of Iodem, the
/// party reasons that it keeps out those without Psynergy, and Iodem and Faran
/// leave Sheba's rescue to the party.
static void BabiLighthouse_ExplainTruthDoor(void) {
  // Everyone reacts to the opened door before trying to follow the party.
  GameFlag_Set(FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN);
  Event_Start();
  Field_EndBusyAndReveal();
  Event_SetMessageId(MsgBabiTheDoor);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 296, 312);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Event_WaitFrames(10);
  Actor_LaunchFromLeader(ACTOR_GARET, 0, 16, FACING_EAST);
  FieldActor_LaunchFromLeader(ACTOR_IVAN, -16, -8, FACING_EAST);
  Actor_LaunchFromLeader(ACTOR_MIA, -16, 24, FACING_EAST);
  Actor_WaitForMoveById(ACTOR_MIA);
  Event_WaitFrames(20);
  FieldCamera_SetSpeed(0x30000, 0x6000);
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(328), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(10);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTHWEST + FACING_STEP, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Actor_Jump(ACTOR_IODEM, 4, 13);
  Actor_Jump(ACTOR_IODEM, 4, 30);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_WORRIED_FOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_NORTHWEST + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_NORTHWEST + FACING_STEP,
                      0);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_WORRIED_FOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_WORRIED_FOR_SHEBA, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_WEST, 0);
  Event_WaitFrames(25);
  Event_ShowMessage(ACTOR_DOUBTING_THE_DOOR, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_WEST, 0);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_WaitFrames(25);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_IODEM, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Import_ActorSetAnimation(ACTOR_DOUBTING_THE_DOOR, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_WORRIED_FOR_SHEBA, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_WEST, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  FieldCamera_FollowActor(ACTOR_IODEM, 1);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -32);
  // Demonstrate that the door closes again when someone without Psynergy
  // enters.
  BabiLighthouse_CloseTruthDoor();
  GameFlag_Clear(FLAG_BEYOND_DOOR);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Event_Start();
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_NORTHWEST + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_NORTHWEST + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_WORRIED_FOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(312), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_DOUBTING_THE_DOOR, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_WORRIED_FOR_SHEBA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_WORRIED_FOR_SHEBA, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
  Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
  Event_WaitFrames(20);
  // Explain the restriction and ask the onlookers to leave the pursuit to us.
  Event_OpenMessage(ACTOR_FARAN, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(30);
    Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_FARAN, 0);
    gFieldWork->message++;
  } else {
    Event_WaitFrames(30);
    Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
    Event_WaitFrames(20);
    gFieldWork->message++;
    Event_ShowMessage(ACTOR_FARAN, 0);
  }
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 16);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTHWEST, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  // Iodem and Faran agree to stay behind and leave the rescue to the party.
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_WaitFrames(20);
  Event_OpenMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_WaitFrames(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    gFieldWork->message++;
  } else {
    Event_WaitFrames(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    gFieldWork->message++;
    Event_ShowMessage(ACTOR_IODEM, 0);
  }
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(15);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(25);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Actor_WalkToAndWait(ACTOR_IODEM, 312, 312);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Event_WaitFrames(25);
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Actor_LinkPair(ACTOR_MIA, ACTOR_IVAN, 0);
  Event_WaitFrames(30);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  // The companions walk back into the leader.
  Actor_SetSpeed(ACTOR_GARET, 0x13333, 0x9999);
  Actor_SetSpeed(ACTOR_IVAN, 0x13333, 0x9999);
  Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
  BabiLighthouse_WalkBackToLeader(ACTOR_GARET);
  BabiLighthouse_WalkBackToLeader(ACTOR_IVAN);
  BabiLighthouse_WalkBackToLeader(ACTOR_MIA);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_NORTHWEST + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_NORTHWEST + FACING_STEP,
                      0);
  Event_WaitFrames(30);
  Event_End();
}

/// Reveal at BabiLighthouseStoneDoor shows the stone door's switch, flipped one
/// way or the other as the door stands.
static void BabiLighthouse_ShowTruthDoorSwitch(void) {
  if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) == 0) {
    MapGrid_CopyCellsLayered(36, 78, 1, 2, 17, 78);
  } else {
    MapGrid_CopyCellsLayered(34, 78, 1, 2, 17, 78);
  }
}

/// As Reveal's light ends at BabiLighthouseStoneDoor, the switch is hidden
/// again.
static void BabiLighthouse_HideTruthDoorSwitch(void) {
  MapGrid_CopyCellsLayered(32, 78, 1, 2, 17, 78);
}

/// The stone door's halves slide open (SE_GATE_MOVE); the first time, Iodem
/// and Faran see it.
static void BabiLighthouse_OpenTruthDoor(void) {
  if (Event_GetFlag(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) == 0) {
    GameFlag_Set(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN);
    Sound_PlaySong(SE_GATE_MOVE);
    Event_Start();
    Actor_SetDestination(ACTOR_LEFT_DOOR, 280, 240);
    Actor_SetDestination(ACTOR_RIGHT_DOOR, 328, 240);
    Actor_WaitForMoveById(ACTOR_LEFT_DOOR);
    Actor_WaitForMoveById(ACTOR_RIGHT_DOOR);
    MapGrid_CopyCellAttributes(81, 14, 4, 1, 17, 14);
    Event_End();
    if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN) == 0) {
      BabiLighthouse_ExplainTruthDoor();
    }
  }
}

/// Walking through the open door: it slides shut behind the party, now on its
/// other side.
static void BabiLighthouse_CloseTruthDoor(void) {
  if (Event_GetFlag(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) != 0) {
    GameFlag_Clear(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN);
    Sound_PlaySong(SE_GATE_MOVE);
    Event_Start();
    Actor_SetDestination(ACTOR_LEFT_DOOR, 296, 240);
    Actor_SetDestination(ACTOR_RIGHT_DOOR, 312, 240);
    Actor_WaitForMoveById(ACTOR_LEFT_DOOR);
    Actor_WaitForMoveById(ACTOR_RIGHT_DOOR);
    MapGrid_CopyCellAttributes(0, 14, 4, 1, 17, 14);
    Event_End();
    if (Event_GetFlag(FLAG_BEYOND_DOOR) != 0) {
      GameFlag_Clear(FLAG_BEYOND_DOOR);
    } else {
      GameFlag_Set(FLAG_BEYOND_DOOR);
    }
  }
}

/// Searching the door's switch: under Reveal's light it is flipped and the
/// door opens; otherwise it reads an inscription. "The truth of this door is
/// open to those seeing with their true heart."
static void BabiLighthouse_InspectTruthDoorSwitch(void) {
  struct FieldWork *work = gFieldWork;

  Event_Start();
  if (work->revealActive != 0) {
    if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) == 0) {

      FieldMessage_Show(MsgFieldFlippedSwitch, 1);
      Sound_PlaySong(SE_SWITCH);
      MapGrid_CopyCellsLayered(35, 78, 1, 2, 17, 78);
      Event_WaitFrames(10);
      MapGrid_CopyCellsLayered(34, 78, 1, 2, 17, 78);
      Event_WaitFrames(10);
      BabiLighthouse_OpenTruthDoor();
    }
  } else {
    Event_SetMessageId(MsgBabiTruthDoorOpenThoseSeeing);
    Event_ShowMessageAndWait(-1, 0);
  }
  Event_End();
}

/// Finds the pushed object one direction-table step ahead, including
/// ground height.
static struct Actor *BabiLighthouse_FindActorAhead(struct Actor *actor) {
  struct FieldPosition position;
  struct FieldPosition *target = &position;
  s32 directionStep = gPushedActor_DirectionSteps[actor->facing >> 12];

  {
    s32 x = actor->x.fixed;
    s32 z = actor->z.fixed;

    x += -0x10000 & directionStep;
    z += directionStep << 16;
    target->x = x;
    target->z = z;
  }
  target->y = MapGrid_GetTerrainHeight(actor->layer, target->x, target->z);
  return PushedActor_FindAtTile(target, actor);
}

/// Before Carry is cast on the Road to Babi Lighthouse's pillar: sets the
/// effect's childOption, so that once Carry lets go the effect waits for the
/// pillar to bounce up and land, and rings it with dust (carry.c).
static void BabiLighthouse_LandCarriedPillarInDust(void) {
  ((struct FieldEffectWork *)gFieldEffectWork)->childOption = 1;
}

/// After Move or Carry on the pillar on the Road to Babi Lighthouse: in its
/// cell at (20, 12) it settles at a height of 48 pixels.
static void BabiLighthouse_UpdateMovedPillar(void) {
  struct Actor *pillar = Actor_GetById(ACTOR_ROAD_PILLAR);
  s32 column = pillar->x.fixed >> 20;

  if (column == 20) {
    s32 row = pillar->z.fixed >> 20;

    if (row == 12) {
      pillar->motionFlags = ACTOR_FALLS;
      pillar->groundHeight = PIXELS(48);
      pillar->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      {
        s32 targetColumn = column, targetRow = row;

        MapGrid_CopyCellAttributes(38, 12, 1, 1, targetColumn, targetRow);
      }
    }
  }
}

/// Walking into the pillar on the Road to Babi Lighthouse: the leader pushes it
/// (PushedActor_AdvancePair) when it stands within 8 pixels of his
/// height.
static void BabiLighthouse_PushRoadPillar(void) {
  // Trial: inverting the empty success arm changes two branch instructions
  // (three text bytes in tbs-en); retain the existing branch order.
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  struct Actor *ahead = BabiLighthouse_FindActorAhead(leader);
  s32 heightDifference;

  if (ahead == 0) {
    return;
  }

  heightDifference = ahead->y.fixed - leader->y.fixed;

  if (heightDifference >= 0) {

    if (heightDifference < PIXELS(8)) {
    } else {
      return;
    }
  } else if (leader->y.fixed - ahead->y.fixed >= PIXELS(8)) {
    return;
  }

  PushedActor_AdvancePair();
}

/// Walking into the climbable cells by the Road to Babi Lighthouse's pillar:
/// the leader climbs up or down where he faces north or south, already
/// climbing or with no pillar ahead, and pushes the pillar
/// (BabiLighthouse_PushRoadPillar) when he does not climb.
static void BabiLighthouse_ClimbOrPushRoadPillar(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  struct Actor *ahead = BabiLighthouse_FindActorAhead(leader);
  s32 climbDirection = (leader->facing + 0x2000) & 0xc000;
  s32 climbResult = -1;

  if (gSaveBlock.movementMode == LEADER_MOVEMENT_CLIMB || ahead == 0) {
    if (climbDirection == FACING_NORTH) {
      climbResult = Field_ClimbUp();
    }
    if (climbDirection == FACING_SOUTH) {
      climbResult = Field_ClimbDown();
    }
  }
  if (climbResult != 0) {
    if (gSaveBlock.movementMode != 1) {
      BabiLighthouse_PushRoadPillar();
    }
  }
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_BabiLighthouseEntry) {
    return gBabiLighthouse_EntryTriggers;
  }
  if (sceneId == (s32)&SceneId_BabiLighthouseLuff) {
    return gBabiLighthouse_LuffTriggers;
  }
  if (sceneId == (s32)&SceneId_BabiLighthouseStoneDoor) {
    return gBabiLighthouse_StoneDoorTriggers;
  }
  return gBabiLighthouse_RoadTriggers;
}

/// Growth on the sprout at the Babi Lighthouse Entry: it grows into a vine the
/// party can climb.
static void BabiLighthouse_OpenGrowthPassage(void) {

  MapGrid_CopyCellAttributes(82, 7, 1, 2, 18, 7);
  WaitFrames(1);
  GameFlag_Set(FLAG_GROWTH_PASSAGE_OPEN);
}

/// Redraws the ledge cells of the Babi Lighthouse Entry, then blocks the cells
/// under its two pillars.
static void BabiLighthouse_RedrawLedgePillarCells(void) {
  struct Actor *leftPillar = Actor_GetById(ACTOR_LEFT_PILLAR);

  Actor_SetSpritePriority(ACTOR_LEFT_PILLAR, 1);
  Actor_SetSpritePriority(ACTOR_RIGHT_PILLAR, 1);
  MapGrid_CopyCellAttributes(69, 19, 3, 3, 5, 19);
  MapGrid_CopyCellAttributes(69, 19, 3, 3, 17, 19);
  {
    s32 column = leftPillar->x.fixed >> 20, row = leftPillar->z.fixed >> 20;

    MapGrid_CopyCellAttributes(3, 3, 1, 1, column, row);
  }
  {
    struct Actor *rightPillar = Actor_GetById(ACTOR_RIGHT_PILLAR);
    s32 column = rightPillar->x.fixed >> 20, row = rightPillar->z.fixed >> 20;

    MapGrid_CopyCellAttributes(3, 3, 1, 1, column, row);
  }
}

#if TBS_PATCH_2
/// Update callback of the pillars at the Babi Lighthouse Entry (TBS_PATCH_2 and
/// later): each is drawn in front of the leader when nearer the screen, and
/// on its layer otherwise.
static void BabiLighthouse_MatchLeaderPriority(struct Actor *updated) {
  struct Sprite *sprite;
  struct Actor *actor;
  s32 priority;

  sprite = Actor_GetById(ACTOR_PARTY_LEADER)->sprite;
  actor = Actor_GetById(ACTOR_LEFT_PILLAR);
  if (actor->z.fixed < Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed) {
    Actor_SetSpritePriority(ACTOR_LEFT_PILLAR, sprite->image.priority);
  } else {
    priority = sprite->image.priority;
    if (priority != 1)
      priority--;
    Actor_SetSpritePriority(ACTOR_LEFT_PILLAR, priority);
    Actor_GetById(ACTOR_LEFT_PILLAR)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  }
  actor = Actor_GetById(ACTOR_RIGHT_PILLAR);
  if (actor->z.fixed < Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed) {
    Actor_SetSpritePriority(ACTOR_RIGHT_PILLAR, sprite->image.priority);
  } else {
    priority = sprite->image.priority;
    if (priority != 1)
      priority--;
    Actor_SetSpritePriority(ACTOR_RIGHT_PILLAR, priority);
    Actor_GetById(ACTOR_RIGHT_PILLAR)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
  }
}
#endif

/// Sets up the map being entered: Retreat leads back to the Entry; the road's
/// pillar, the Entry's pillars, vine and the four lying on the ground,
/// BabiLighthouseLuff's arrivals by falling, and BabiLighthouseStoneDoor's door
/// and onlookers are restored. Returns 0.
s32 BabiLighthouse_SetupMap(void) {
  struct Actor *actor;
  struct Actor *placedActor;
  s32 sceneId;
  s32 mapCopy;
  struct SaveBlock *state;
  s32 x;
  s32 z;
  u8 zero;

  WaitFrames(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  state = &gSaveBlock;
  sceneId = state->sceneId;
  if (sceneId != (s32)&SceneId_RoadToBabiLighthouse) {
    state->retreatEntrance = 1;
    state->retreatScene = (s32)&SceneId_BabiLighthouseEntry;
    mapCopy = state->sceneId;
  }
  // The road: the moved pillar's metatile behavior where it stands.
  if (sceneId == (s32)&SceneId_RoadToBabiLighthouse) {
    placedActor = Actor_GetById(ACTOR_ROAD_PILLAR);
    x = placedActor->x.fixed >> 20;
    if (x == 20) {
      z = placedActor->z.fixed >> 20;
      if (z == 12) {
        MapGrid_CopyCellAttributes(38, 12, 1, 1, x, z);
      }
    }
  } else if (mapCopy == (s32)&SceneId_BabiLighthouseEntry) {
    // Set up the movable ledge pillars and the staggered actor
    // animations.
    Actor_SetSpriteColorById(ACTOR_LEFT_PILLAR, SPRITE_COLOR_GREY);
    Actor_SetSpriteColorById(ACTOR_RIGHT_PILLAR, SPRITE_COLOR_GREY);
#if TBS_PATCH_2
    // Through union ActorOrEffect: as struct Actor the stores move.
    ((union ActorOrEffect *)Actor_GetById(ACTOR_LEFT_PILLAR))->actor.update =
        BabiLighthouse_MatchLeaderPriority;
    ((union ActorOrEffect *)Actor_GetById(ACTOR_RIGHT_PILLAR))->actor.update =
        BabiLighthouse_MatchLeaderPriority;
#endif
    if (state->entrance ==
            BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTH &&
        GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Actor_SetPosition(ACTOR_RIGHT_PILLAR, PIXELS(312), PIXELS(328));
    }
    BabiLighthouse_RedrawLedgePillarCells();
    actor = Actor_GetById(ACTOR_RIGHT_PILLAR);
    actor->y.fixed = Actor_GetById(ACTOR_RIGHT_PILLAR)->groundHeight;
    if (GameFlag_Get(FLAG_GROWTH_PASSAGE_OPEN) != 0) {
      Import_ActorSetAnimation(ACTOR_PLANT, 4);
      Actor_GetById(ACTOR_PLANT)->collisionFlags = 0xff & ~ACTOR_SOLID;
      BabiLighthouse_OpenGrowthPassage();
    }
    // Their script waits stagger the first animations of the four lying on the
    // ground.
    placedActor = Actor_GetById(ACTOR_FALLEN_SOLDIER_NO_MATCH);
    placedActor->collisionFlags = 0;
    placedActor->priorityFlags = 0;
    placedActor->wait = 0;
    placedActor->sprite->image.priority = 3;
    zero = 0;
    placedActor->sprite->flags = zero;
    placedActor->sprite->rotation = 0xc000;
    Import_ActorSetAnimation(ACTOR_FALLEN_SOLDIER_NO_MATCH, 0);
    placedActor = Actor_GetById(ACTOR_FALLEN_STRONGMAN_ONLY_TWO);
    placedActor->collisionFlags = zero;
    placedActor->priorityFlags = zero;
    placedActor->wait = 30;
    placedActor->sprite->image.priority = 3;
    placedActor->sprite->flags = zero;
    placedActor->sprite->rotation = 0x4000;
    Import_ActorSetAnimation(ACTOR_FALLEN_STRONGMAN_ONLY_TWO, 0);
    placedActor = Actor_GetById(ACTOR_FALLEN_SOLDIER_KICKED_ASIDE);
    placedActor->collisionFlags = zero;
    placedActor->priorityFlags = zero;
    placedActor->wait = 60;
    placedActor->sprite->image.priority = 3;
    placedActor->sprite->flags = zero;
    placedActor->sprite->rotation = 0x8000;
    Import_ActorSetAnimation(ACTOR_FALLEN_SOLDIER_KICKED_ASIDE, 0);
    placedActor = Actor_GetById(ACTOR_FALLEN_MAN_SCORCHED_GROUND);
    placedActor->collisionFlags = zero;
    placedActor->priorityFlags = zero;
    placedActor->wait = 90;
    placedActor->sprite->image.priority = 3;
    placedActor->sprite->flags = zero;
    placedActor->sprite->rotation = 0x8000;
    Import_ActorSetAnimation(ACTOR_FALLEN_MAN_SCORCHED_GROUND, 0);
  } else if (mapCopy == (s32)&SceneId_BabiLighthouseLuff) {
    // Match the arrival effect and fall to the entrance used in this
    // room.
    switch (state->entrance) {
    case BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_STONE_DOOR_WEST:
      GameFlag_Set(FLAG_BABI_LIGHTHOUSE_BACK_FROM_STONE_DOOR);
      // Both entrances restore the same remembered cell changes.
      // fall through
    case BABI_LIGHTHOUSE_LUFF_LOCKED_DOOR_ROOM_FROM_SOUTH_ROOM:
      if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_BACK_FROM_STONE_DOOR) != 0) {
        MapGrid_CopyCellsTo(120, 7, 109, 7, 1, 3);
        MapGrid_CopyCellAttributes(45, 8, 1, 1, 45, 9);
      }
      break;
    case BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_SOUTHWEST:
      Actor_Prepare(PIXELS(440), 0, PIXELS(290), PIT_MASK);
      MapGrid_CopyCellAttributes(22, 13, 1, 1, 27, 13);
      BabiLighthouse_FallToExit(
          BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_FALL_SOUTH);
      break;
    case BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_NORTHWEST:
      Actor_Prepare(PIXELS(448), 0, PIXELS(290), PIT_MASK);
      MapGrid_CopyCellAttributes(22, 12, 1, 1, 28, 10);
      BabiLighthouse_FallToExit(
          BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_FALL_NORTH);
      break;
    case BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_FALL_NORTH:
      Actor_Prepare(PIXELS(232), 0, PIXELS(594), PIT_MASK);
      MapGrid_CopyCellAttributes(22, 12, 1, 1, 14, 33);
      BabiLighthouse_FallToExit(
          BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_STONE_DOOR_EAST);
      break;
    case BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_EAST:
    case BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_PIT:
    case BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_FALL_SOUTH:
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
        BabiLighthouse_LandWithParticles();
      }
      break;
    }
  } else if (sceneId == (s32)&SceneId_BabiLighthouseStoneDoor) {
    // Restore the truth door and the onlookers' positions after each
    // discussion.
    Actor_GetById(ACTOR_LEFT_DOOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_GetById(ACTOR_RIGHT_DOOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_SetSpeed(ACTOR_LEFT_DOOR, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_RIGHT_DOOR, 0x10000, 0x8000);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      if (state->entrance ==
          BABI_LIGHTHOUSE_STONE_DOOR_FROM_TUNNEL_RUINS_GREAT_STATUE) {
        GameFlag_Set(FLAG_BEYOND_DOOR);
      } else {
        GameFlag_Clear(FLAG_BEYOND_DOOR);
      }
    }
    if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_ASKED_TO_FOLLOW) == 0) {
      Actor_SetPosition(ACTOR_IODEM, -PIXELS(64), -PIXELS(64));
      Actor_SetPosition(ACTOR_WITNESS, PIXELS(280), PIXELS(296));
      Actor_SetPosition(ACTOR_DOUBTING_THE_DOOR, PIXELS(312), PIXELS(248));
      Actor_SetPosition(ACTOR_WORRIED_FOR_SHEBA, PIXELS(296), PIXELS(248));
      Actor_SetPosition(ACTOR_FARAN, PIXELS(320), PIXELS(296));
      Actor_FaceDirection(ACTOR_WITNESS, FACING_EAST, 0);
      Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_NORTH, 0);
      Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_NORTH, 0);
      Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
      Event_WaitFrames(5);
    } else if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN) != 0) {
      Actor_SetPosition(ACTOR_IODEM, PIXELS(312), PIXELS(312));
      Actor_FaceDirection(ACTOR_IODEM, FACING_NORTHWEST + FACING_STEP, 0);
      Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTHWEST + FACING_STEP, 0);
      Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR,
                          FACING_NORTHWEST + FACING_STEP, 0);
      Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA,
                          FACING_NORTHWEST + FACING_STEP, 0);
      Actor_FaceDirection(ACTOR_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
      Event_WaitFrames(5);
    }
    if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) != 0) {
      Actor_SetPosition(ACTOR_LEFT_DOOR, PIXELS(280), PIXELS(240));
      Actor_SetPosition(ACTOR_RIGHT_DOOR, PIXELS(328), PIXELS(240));
      Actor_FaceDirection(ACTOR_LEFT_DOOR, FACING_WEST, 0);
      Actor_FaceDirection(ACTOR_RIGHT_DOOR, FACING_EAST, 0);
      MapGrid_CopyCellAttributes(81, 14, 4, 1, 17, 14);
    }
    if (gSaveBlock.entrance ==
        BABI_LIGHTHOUSE_STONE_DOOR_FROM_BABI_LIGHTHOUSE_LUFF_EAST) {
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
        BabiLighthouse_LandWithParticles();
      }
    }
  } else {
    Import_ActorSetAnimation(ACTOR_ROAD_PILLAR, 2);
  }
  return 0;
}

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gPushedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

u32 gPushedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedActor_FootprintBounds[] = {
    -32, -8,  32, 8,  -8,  -32, 8,  32, -32, -16, 32, 0,
    -8,  -32, 8,  32, -32, -8,  32, 8,  -8,  -32, 8,  32,
};

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_RELEASE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gBabiLighthouse_TrembleScript[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(120),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct SceneEntrance gBabiLighthouse_EntryEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 136, 0, 376, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE, CONDITION_ALWAYS, 200,
     0, 424, FACING_NORTH, 0, 0, 0, 384, 448, 0},
    {BABI_LIGHTHOUSE_ENTRY_FROM_BABI_LIGHTHOUSE_LUFF_WEST, CONDITION_ALWAYS,
     200, 0, 200, FACING_SOUTH, 0, 0, 0, 384, 448, 0},
    {BABI_LIGHTHOUSE_ENTRY_FROM_BABI_LIGHTHOUSE_LUFF_EAST, CONDITION_ALWAYS,
     216, 0, 184, FACING_SOUTH, 0, 0, 0, 384, 448, 0},
    {BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_WEST,
     CONDITION_ALWAYS, 104, 0, 312, FACING_SOUTH, 0, 0, 0, 384, 448, 0},
    {BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTH,
     CONDITION_ALWAYS, 296, 0, 312, FACING_SOUTH, 0, 0, 0, 384, 448, 0},
    {BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_NORTH,
     CONDITION_ALWAYS, 312, 0, 88, FACING_SOUTH, 0, 0, 0, 384, 448, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBabiLighthouse_LuffEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 136, 0, 376, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_WEST_STEPS,
     CONDITION_ALWAYS, 232, 0, 488, FACING_SOUTH, 0, 128, 432, 368, 672, 0},
    {BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_EAST_STEPS,
     CONDITION_ALWAYS, 280, 0, 488, FACING_SOUTH, 0, 128, 432, 368, 672, 0},
    {BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_LOCKED_DOOR_ROOM, CONDITION_ALWAYS,
     248, 0, 648, FACING_SOUTH, 0, 128, 432, 368, 672, 0},
    {BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_STEPS, CONDITION_ALWAYS, 424,
     0, 120, FACING_SOUTH, 0, 352, 64, 592, 320, 0},
    {BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_SOUTH_ROOM_WEST_STEPS,
     CONDITION_ALWAYS, 456, 0, 120, FACING_SOUTH, 0, 352, 64, 592, 320, 0},
    {BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_SOUTH_ROOM_EAST_STEPS,
     CONDITION_ALWAYS, 504, 0, 120, FACING_SOUTH, 0, 352, 64, 592, 320, 0},
    {BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_ENTRY_EAST, CONDITION_ALWAYS,
     472, 0, 288, FACING_NORTH, 0, 352, 64, 592, 320, 0},
    {BABI_LIGHTHOUSE_LUFF_WEST_ROOM_FROM_EAST_ROOM_STEPS, CONDITION_ALWAYS, 120,
     0, 120, FACING_SOUTH, 0, 48, 64, 288, 320, 0},
    {BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_ENTRY_WEST, CONDITION_ALWAYS,
     184, 0, 288, FACING_NORTH, 0, 48, 64, 288, 320, 0},
    {BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_STONE_DOOR_WEST,
     CONDITION_ALWAYS, 728, 0, 168, FACING_SOUTH, 0, 624, 96, 864, 256, 0},
    {BABI_LIGHTHOUSE_LUFF_LOCKED_DOOR_ROOM_FROM_SOUTH_ROOM, CONDITION_ALWAYS,
     776, 0, 184, FACING_SOUTH, 0, 624, 96, 864, 256, 0},
    {BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_EAST, CONDITION_ALWAYS,
     488, 0, 184, FACING_SOUTH, 0, 352, 64, 592, 320, 0},
    {BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_PIT, CONDITION_ALWAYS, 248,
     0, 584, FACING_SOUTH, 0, 128, 432, 368, 672, 0},
    {BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_SOUTHWEST,
     CONDITION_ALWAYS, 440, 0, 216, FACING_SOUTH, 0, 352, 64, 592, 320, 0},
    {BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_FALL_SOUTH,
     CONDITION_ALWAYS, 216, 0, 584, FACING_SOUTH, 0, 128, 432, 368, 672, 0},
    {BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_NORTHWEST,
     CONDITION_ALWAYS, 456, 0, 168, FACING_SOUTH, 0, 352, 64, 592, 320, 0},
    {BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_FALL_NORTH,
     CONDITION_ALWAYS, 232, 0, 536, FACING_SOUTH, 0, 128, 432, 368, 672, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBabiLighthouse_StoneDoorEntrances[] = {
    {BABI_LIGHTHOUSE_STONE_DOOR_FROM_TUNNEL_RUINS_GREAT_STATUE,
     CONDITION_ALWAYS, 248, 0, 120, FACING_SOUTH, 0, 96, 48, 384, 432, 0},
    {BABI_LIGHTHOUSE_STONE_DOOR_FROM_BABI_LIGHTHOUSE_LUFF_WEST,
     CONDITION_ALWAYS, 136, 0, 368, FACING_NORTH, 0, 96, 48, 384, 432, 0},
    {BABI_LIGHTHOUSE_STONE_DOOR_FROM_BABI_LIGHTHOUSE_LUFF_EAST,
     CONDITION_ALWAYS, 168, 0, 344, FACING_SOUTH, 0, 96, 48, 384, 432, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBabiLighthouse_RoadEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 328, 0, 240, FACING_NORTH, 0, 0, 0,
     464, 400, 0},
    {ROAD_TO_BABI_LIGHTHOUSE_FROM_WORLD_MAP, CONDITION_ALWAYS, 112, 0, 360,
     FACING_NORTH, 0, 0, 0, 464, 400, 0},
    {ROAD_TO_BABI_LIGHTHOUSE_FROM_BABI_LIGHTHOUSE_ENTRY, CONDITION_ALWAYS, 136,
     0, 32, FACING_SOUTH, 0, 0, 0, 464, 400, 0},
    {SCENE_TABLE_END},
};

struct SceneExitRegion gBabiLighthouse_EntryRegions[] = {
    {96, -20, 320, 112, -4, 336, CONDITION_ALWAYS,
     BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_WEST},
    {288, -20, 320, 304, -4, 336, CONDITION_ALWAYS,
     BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_SOUTH},
    {304, -20, 96, 320, -4, 112, CONDITION_ALWAYS,
     BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_NORTH},
    {SCENE_TABLE_END},
};

u32 gBabiLighthouse_Exits[] = {
    SCENE_EXITS(BabiLighthouseEntry),
    SCENE_EXIT(BABI_LIGHTHOUSE_ENTRY_TO_ROAD_TO_BABI_LIGHTHOUSE,
               RoadToBabiLighthouse,
               ROAD_TO_BABI_LIGHTHOUSE_FROM_BABI_LIGHTHOUSE_ENTRY),
    SCENE_EXIT(BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_EAST,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_ENTRY_EAST),
    SCENE_EXIT(BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_WEST,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_ENTRY_WEST),
    SCENE_EXIT(BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_WEST,
               TunnelRuinsLadyStatues,
               TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_WEST),
    SCENE_EXIT(BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_SOUTH,
               TunnelRuinsLadyStatues,
               TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_SOUTH),
    SCENE_EXIT(BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_NORTH,
               TunnelRuinsLadyStatues,
               TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_NORTH),
    SCENE_EXITS(BabiLighthouseLuff),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_STEPS,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_STEPS),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_ENTRY_EAST,
               BabiLighthouseEntry,
               BABI_LIGHTHOUSE_ENTRY_FROM_BABI_LIGHTHOUSE_LUFF_EAST),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_WEST_ROOM_STEPS,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_WEST_ROOM_FROM_EAST_ROOM_STEPS),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_WEST_STEPS,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_WEST_STEPS),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_EAST_STEPS,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_EAST_STEPS),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_ENTRY_WEST,
               BabiLighthouseEntry,
               BABI_LIGHTHOUSE_ENTRY_FROM_BABI_LIGHTHOUSE_LUFF_WEST),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_WEST_STEPS,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_SOUTH_ROOM_WEST_STEPS),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_EAST_STEPS,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_SOUTH_ROOM_EAST_STEPS),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_LOCKED_DOOR_ROOM,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_LOCKED_DOOR_ROOM_FROM_SOUTH_ROOM),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_LOCKED_DOOR_ROOM_TO_SOUTH_ROOM,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_LOCKED_DOOR_ROOM),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_EAST,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_EAST),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_PIT,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_PIT),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_SOUTHWEST,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_SOUTHWEST),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_FALL_SOUTH,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_FALL_SOUTH),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_NORTHWEST,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_NORTHWEST),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_FALL_NORTH,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_FALL_NORTH),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_STONE_DOOR_EAST,
               BabiLighthouseStoneDoor,
               BABI_LIGHTHOUSE_STONE_DOOR_FROM_BABI_LIGHTHOUSE_LUFF_EAST),
    SCENE_EXIT(BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_STONE_DOOR_WEST,
               BabiLighthouseStoneDoor,
               BABI_LIGHTHOUSE_STONE_DOOR_FROM_BABI_LIGHTHOUSE_LUFF_WEST),
    SCENE_EXITS(BabiLighthouseStoneDoor),
    SCENE_EXIT(BABI_LIGHTHOUSE_STONE_DOOR_TO_TUNNEL_RUINS_GREAT_STATUE,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR),
    SCENE_EXIT(BABI_LIGHTHOUSE_STONE_DOOR_TO_BABI_LIGHTHOUSE_LUFF_WEST,
               BabiLighthouseLuff,
               BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_STONE_DOOR_WEST),
    SCENE_EXITS(RoadToBabiLighthouse),
    SCENE_EXIT(ROAD_TO_BABI_LIGHTHOUSE_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_ROAD_TO_BABI_LIGHTHOUSE),
    SCENE_EXIT(ROAD_TO_BABI_LIGHTHOUSE_TO_BABI_LIGHTHOUSE_ENTRY,
               BabiLighthouseEntry,
               BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE),
    SCENE_EXITS_END,
};

/// Unused: an empty actor template table nothing returns.
struct ActorTemplate gBabiLighthouse_NoActorTemplates[] = {
    {SCENE_TABLE_END},
};

struct ActorTemplate gBabiLighthouse_EntryActorTemplates[] = {
    {SPRITE_CAPPED_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
     PIXELS(88), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_CAPPED_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
     PIXELS(296), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP,
     ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(296), 0,
     PIXELS(130), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gBabiLighthouse_TrembleScript,
     PIXELS(176), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_STRONGMAN, CONDITION_ALWAYS, (s32)gBabiLighthouse_TrembleScript,
     PIXELS(240), 0, PIXELS(240), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gBabiLighthouse_TrembleScript,
     PIXELS(216), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS,
     (s32)gBabiLighthouse_TrembleScript, PIXELS(184), 0, PIXELS(248),
     FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SCENE_TABLE_END},
};

struct ActorTemplate gBabiLighthouse_LuffActorTemplates[] = {
    {JUPITER_DJINNI, FLAG_DJINNI(DJINNI_LUFF), MOVEMENT_TYPE_STAND, PIXELS(472),
     0, PIXELS(184), FACING_SOUTHEAST, TALK_FACING_PARTY, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorTemplate gBabiLighthouse_StoneDoorActorTemplates[] = {
    {ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(-64), 0, PIXELS(-64), FACING_EAST,
        TALK_FACING_PARTY, 0},
    {ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACING_PARTY,
        0},
    {ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(-64), 0, PIXELS(-64), FACING_EAST,
        TALK_FACING_PARTY, 0},
    {SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, PIXELS(296), 0, PIXELS(240), FACING_WEST, TALK_FACING_KEEP, 0},
    {SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, PIXELS(312), 0, PIXELS(240), FACING_EAST, TALK_FACING_KEEP, 0},
    {SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(312), 0, PIXELS(352), FACING_NORTH,
        TALK_FACING_PARTY_AND_BACK, 0},
    {SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(328), 0, PIXELS(328), FACING_WEST,
        TALK_FACING_PARTY_AND_BACK, 0},
    {SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(344), 0, PIXELS(336), FACING_WEST,
        TALK_FACING_PARTY_AND_BACK, 0},
    {SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(344), 0, PIXELS(312), FACING_WEST,
        TALK_FACING_PARTY_AND_BACK, 0},
    {SPRITE_FARAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(328), 0, PIXELS(312), FACING_WEST,
        TALK_FACING_PARTY_AND_BACK, 0},
    {SCENE_TABLE_END},
};

struct ActorTemplate gBabiLighthouse_RoadActorTemplates[] = {
    {SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, PIXELS(128), 0, PIXELS(264), FACING_WEST,
        TALK_FACING_PARTY, 0},
    {SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, PIXELS(96), 0, PIXELS(184), FACING_EAST, TALK_FACING_PARTY,
        0},
    {SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(184), 0, PIXELS(200), FACING_WEST, TALK_FACING_PARTY,
        0},
    {SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(232), 0, PIXELS(184), FACING_SOUTH,
        TALK_FACING_PARTY, 0},
    {SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(376), 0, PIXELS(216), FACING_SOUTH,
        TALK_FACING_PARTY, 0},
    {SCENE_TABLE_END},
};

struct Trigger gBabiLighthouse_EntryTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, BABI_LIGHTHOUSE_ENTRY_TO_ROAD_TO_BABI_LIGHTHOUSE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_EAST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_WEST},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT(BabiLighthouse_PushLedgePillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT(BabiLighthouse_PushLedgePillarOrClimbDown)},
    {TRIGGER_TALK, ACTOR_FALLEN_SOLDIER_NO_MATCH, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely)},
    {TRIGGER_TALK, ACTOR_FALLEN_STRONGMAN_ONLY_TWO, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseOnlyTwoOfThoseSixEver)},
    {TRIGGER_TALK, ACTOR_FALLEN_SOLDIER_KICKED_ASIDE, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseTheyJustKickedUsOutOf)},
    {TRIGGER_TALK, ACTOR_FALLEN_MAN_SCORCHED_GROUND, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseTheySetFireToEverythingThats)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_SOLDIER_NO_MATCH, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseWereTheOnesWhoAttackedBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_STRONGMAN_ONLY_TWO, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseTheOtherFourDidntSeemTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_SOLDIER_KICKED_ASIDE, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseWhatCouldTheyBePlanningFor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_MAN_SCORCHED_GROUND, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseNoneOfThemHadAnyTorches)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_PILLAR, CONDITION_ALWAYS, EVENT(BabiLighthouse_JumpFromLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_PILLAR, CONDITION_ALWAYS, EVENT(BabiLighthouse_JumpFromLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_PLANT, FLAG_GROWTH_PASSAGE_OPEN, EVENT(BabiLighthouse_OpenGrowthPassage)},
    {TRIGGERS_END},
};

struct Trigger gBabiLighthouse_LuffTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_STEPS},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_ENTRY_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_WEST_ROOM_STEPS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_WEST_STEPS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_EAST_STEPS},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_ENTRY_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_WEST_STEPS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_EAST_STEPS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_LOCKED_DOOR_ROOM},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_LOCKED_DOOR_ROOM_TO_SOUTH_ROOM},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_STONE_DOOR_WEST},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT(BabiLighthouse_DropFromWestRoomEastPit)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT(BabiLighthouse_DropFromEastRoomPit)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS, EVENT(BabiLighthouse_DropFromWestRoomSouthwestPit)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 28, CONDITION_ALWAYS, EVENT(BabiLighthouse_DropFromWestRoomNorthwestPit)},
    {TRIGGER_TALK, ACTOR_LUFF, CONDITION_ALWAYS, EVENT(BabiLighthouse_StartDjinnBattle)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 20, CONDITION_ALWAYS, EVENT(BabiLighthouse_InspectLockedDoor)},
    {TRIGGERS_END},
};

struct Trigger gBabiLighthouse_StoneDoorTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, BABI_LIGHTHOUSE_STONE_DOOR_TO_TUNNEL_RUINS_GREAT_STATUE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, BABI_LIGHTHOUSE_STONE_DOOR_TO_BABI_LIGHTHOUSE_LUFF_WEST},
    {TRIGGER_STEP, 10, FLAG_BABI_LIGHTHOUSE_ASKED_TO_FOLLOW, EVENT(BabiLighthouse_FaranAsksPartyToOpenDoor)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(FLAG_BEYOND_DOOR), EVENT(BabiLighthouse_CloseTruthDoor)},
    {TRIGGER_STEP, 21, FLAG_BEYOND_DOOR, EVENT(BabiLighthouse_CloseTruthDoor)},
    {TRIGGER_STEP, 22, CONDITION_ALWAYS, EVENT(BabiLighthouse_OpenTruthDoor)},
    {TRIGGER_TALK, ACTOR_IODEM, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIAmCountingOnYouIsaac)},
    {TRIGGER_TALK, ACTOR_WITNESS, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseWhenISawThemTheyHad)},
    {TRIGGER_TALK, ACTOR_DOUBTING_THE_DOOR, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIAmSorryButNotEven)},
    {TRIGGER_TALK, ACTOR_WORRIED_FOR_SHEBA, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseThisAllStartedWhenActorWas)},
    {TRIGGER_TALK, ACTOR_FARAN, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIfYouCanOpenThisDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIDoNotMeanToDoubt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN,
        MSGNO(MsgBabiLighthouseNobodyThinksTheDoorCanBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOUBTING_THE_DOOR, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN,
        MSGNO(MsgBabiLighthouseIDontKnowWhatTheyreWaiting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_FOR_SHEBA, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseThisIsAllAHugeWaste)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARAN, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIHopeTheyCanOpenThe)},
    {TRIGGER_TALK, ACTOR_IODEM, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseIBegOfYouIsaacPlease)},
    {TRIGGER_TALK, ACTOR_WITNESS, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseICantBelieveThisDoorActually)},
    {TRIGGER_TALK, ACTOR_DOUBTING_THE_DOOR, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseItTakesGreatCourageToContinue)},
    {TRIGGER_TALK, ACTOR_WORRIED_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseImVeryInterestedInWhatLies)},
    {TRIGGER_TALK, ACTOR_FARAN, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseIAmNotAfraidOfDanger)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseIfAnythingHappensToActorTolbi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseIfThisDoorOpensMaybeWe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOUBTING_THE_DOOR, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseIBetTheMonstersAroundHere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_FOR_SHEBA, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseThisIsABigWasteOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARAN, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseIDontWantToHinderActors)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 60, CONDITION_ALWAYS, EVENT(BabiLighthouse_InspectTruthDoorSwitch)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS,
        EVENT(BabiLighthouse_ShowTruthDoorSwitch)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(BabiLighthouse_HideTruthDoorSwitch)},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gBabiLighthouse_RoadTriggers[] = {
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 50, CONDITION_ALWAYS,
     EVENT(BabiLighthouse_PushRoadPillar)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 51, CONDITION_ALWAYS,
     EVENT(BabiLighthouse_ClimbOrPushRoadPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROAD_PILLAR, CONDITION_ALWAYS,
     EVENT(BabiLighthouse_UpdateMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_ROAD_PILLAR, CONDITION_ALWAYS,
     EVENT(BabiLighthouse_LandCarriedPillarInDust)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_ROAD_PILLAR, CONDITION_ALWAYS,
     EVENT(BabiLighthouse_UpdateMovedPillar)},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ROAD_TO_BABI_LIGHTHOUSE_TO_WORLD_MAP},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     ROAD_TO_BABI_LIGHTHOUSE_TO_BABI_LIGHTHOUSE_ENTRY},
    {TRIGGER_TALK, ACTOR_WORKER_DAY_OFF, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseWeHaventBeenOrderedToStart)},
    {TRIGGER_TALK, ACTOR_WORKER_STRANGE_FOLK, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseThereWereStrangeFolkGoingTo)},
    {TRIGGER_TALK, ACTOR_WORKER_ODD_PLACE, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseItsOddThatBabiWouldBuild)},
    {TRIGGER_TALK, ACTOR_WORKER_CUTTING_STONES, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseCuttingTheseStonesAndStackingThem)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORKER_DAY_OFF,
     CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseThereWasABigFightThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORKER_STRANGE_FOLK,
     CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseIfFaranWentAfterThemThen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORKER_ODD_PLACE,
     CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseWhyWouldTheyWantToRush)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORKER_CUTTING_STONES,
     CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseNowThatIThinkOfIt)},
    {EXAMINE_CHEST_FACING(FACING_EAST), 100, FLAG_BABI_LIGHTHOUSE_LUCKY_PEPPER,
     FIND_ITEM(ITEM_LUCKY_PEPPER)},
    {TRIGGERS_END},
};
