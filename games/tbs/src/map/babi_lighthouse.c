/// @file Babi Lighthouse and the road up to it: four maps in one overlay, in
/// three of the game's areas. MapId_RoadToBabiLighthouse is the Road to Babi
/// Lighthouse, entered from the world map; MapId_BabiLighthouseEntry is the
/// Babi Lighthouse Entry; MapId_BabiLighthouseLuff and
/// MapId_BabiLighthouseStoneDoor are both Babi Lighthouse. The four share one
/// warp table, the other getters choose by gGameState.mapId with the road as
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
/// following. Three warp regions drop the party to the floor below,
/// TunnelRuinsLadyStatues (tunnel_ruins.c), and the way back up comes out
/// beside the same holes.
///
/// BabiLighthouseLuff is three rooms joined by steps and a fourth behind a
/// locked door. Walking south into one of the pits at coord events 25 to 28,
/// the leader jumps in and drops from above into a lower room, where he either
/// lands in dust (BabiLighthouse_LandWithParticles) or falls on through
/// another hole to the room below that (BabiLighthouse_FallToWarp). Luff, a
/// Jupiter Djinni, starts its battle when spoken to.
///
/// BabiLighthouseStoneDoor is the cave before a stone door, and its two
/// cutscenes are ordered by flags: walking up to the door the first time plays
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
///
/// Open questions: none.

#include "game_state.h"
#include "version.h"
#include "global_cells.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/babi_lighthouse.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "macros.h"

#define FIELD_STAGED_OBJ_EVENT_IMPORTS
#include "map_grid_copy.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "djinn_ids.h"
#include "field_effect.h"
#include "staged_object_event.h"
#include "select_overlay_data_by_runtime_selector.h"
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
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "object_event.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

/// BabiLighthouseStoneDoor, before the stone door: the door's two halves,
/// Iodem, the strongman who saw the kidnappers go through, a grey-haired man, a
/// young man and Faran.
enum {
  /// ★ Named from what the player sees: the left half of a dark stone door
  /// (its render)
  LOCALID_LEFT_DOOR = 8,
  /// ★ Named from what the player sees: the right half of the stone door (its
  /// render)
  LOCALID_RIGHT_DOOR = 9,
  /// ★ Named from the game: "I will trust Iodem once more..."
  /// (MsgBabiLighthouseIfYouCanOpenThisDoor)
  LOCALID_IODEM = 10,
  /// A strongman.
  /// ★ Named from the game: "When I saw them, they had already gone through to
  /// the other side." (MsgBabiLighthouseWhenISawThemTheyHad)
  LOCALID_WITNESS = 11,
  /// A burly grey-haired man with his arms folded.
  /// ★ Named from the game: "There is no way this door will open."
  /// (MsgBabiLighthouseIAmSorryButNotEven)
  LOCALID_DOUBTING_THE_DOOR = 12,
  /// A young man with brown hair in a purple shirt.
  /// ★ Named from the game: "her life may be in danger..."
  /// (MsgBabiLighthouseThisIsAllAHugeWaste)
  LOCALID_WORRIED_FOR_SHEBA = 13,
  /// ★ Named from the game: "There were strange folk going to the ruins...
  /// Faran followed them." (MsgBabiLighthouseThereWereStrangeFolkGoingTo)
  LOCALID_FARAN = 14
};

/// BabiLighthouseLuff's Djinni, Luff.
enum {
  /// ★ Named from the game: "Luff" (MsgDjinnName)
  LOCALID_DJINNI = 8
};

/// The Entry (BabiLighthouseEntry): the two pillars on the ledges, the plant
/// for Growth and the four the kidnappers knocked down on their way in (two of
/// Babi's soldiers, a strongman and a bald, muscular man), lying on the ground,
/// each named by what he says. The pillars have the sprite of the pillars Move
/// or a push shifts in Venus Lighthouse.
enum {
  /// ★ Named from what the player sees: a stone pillar with a glowing yellow
  /// cap on the left ledge (its render)
  LOCALID_LEFT_PILLAR = 8,
  /// ★ Named from what the player sees: the same capped pillar on the right
  /// ledge (its render)
  LOCALID_RIGHT_PILLAR = 9,
  /// ★ Named from what the player sees: a small green plant, SPRITE_SPROUT (its
  /// render)
  LOCALID_PLANT = 10,
  /// A soldier lying on the ground.
  /// ★ Named from the game: "We were absolutely no match for them."
  /// (MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely)
  LOCALID_FALLEN_SOLDIER_NO_MATCH = 11,
  /// A strongman in a red top lying on the ground.
  /// ★ Named from the game: "Only two of those six ever attacked anyone..."
  /// (MsgBabiLighthouseOnlyTwoOfThoseSixEver)
  LOCALID_FALLEN_STRONGMAN_ONLY_TWO = 12,
  /// A soldier lying on the ground.
  /// ★ Named from the game: "They just kicked us out of their way"
  /// (MsgBabiLighthouseTheyJustKickedUsOutOf)
  LOCALID_FALLEN_SOLDIER_KICKED_ASIDE = 13,
  /// A bald, muscular man lying on the ground.
  /// ★ Named from the game: "That's why the ground is scorched."
  /// (MsgBabiLighthouseTheySetFireToEverythingThats)
  LOCALID_FALLEN_MAN_SCORCHED_GROUND = 14
};

/// The Road to Babi Lighthouse (RoadToBabiLighthouse): four workers (a bald,
/// muscular man, a strongman and two men with pickaxes), each named by what he
/// says, and the pillar for Move and Carry, with the sprite of the pillars
/// Carry takes across a gap in Venus Lighthouse.
enum {
  /// A bald, muscular man.
  /// ★ Named from the game: "Do we get today off?"
  /// (MsgBabiLighthouseWeHaventBeenOrderedToStart)
  LOCALID_WORKER_DAY_OFF = 8,
  /// A strongman in a red top.
  /// ★ Named from the game: "There were strange folk going to the ruins..."
  /// (MsgBabiLighthouseThereWereStrangeFolkGoingTo)
  LOCALID_WORKER_STRANGE_FOLK = 9,
  /// A man with a pickaxe.
  /// ★ Named from the game: "It's odd that Babi would build a lighthouse in a
  /// place like this." (MsgBabiLighthouseItsOddThatBabiWouldBuild)
  LOCALID_WORKER_ODD_PLACE = 10,
  /// A man with a pickaxe.
  /// ★ Named from the game: "Cutting these stones and stacking them on the
  /// ruins isn't easy." (MsgBabiLighthouseCuttingTheseStonesAndStackingThem)
  LOCALID_WORKER_CUTTING_STONES = 11,
  /// ★ Named from what the player sees: a short grey stone pillar carved with a
  /// spiral (its render)
  LOCALID_ROAD_PILLAR = 12
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

/// What ObjectEvent_PrepareObject places at the far edge of a pit: a crate
/// drawn solid black (SPRITE_COLOR_BLACK) and translucent in front of the
/// object events, which shows the background through the leader where they
/// overlap, so that he vanishes below the edge as he drops over it. ★ Named
/// from what the player sees: the leader's body hidden as he drops into a pit,
/// and shown when the mask is left out (its render)
enum { PIT_MASK = SPRITE_CRATE };

/// A chest searched while facing the given way.
#define BG_EVENT_CHEST_FACING(facing)                                          \
  (BG_EVENT(BG_EVENT_CHEST) | MAP_EVENT_FLAG_FACING | (facing))

static void BabiLighthouse_CloseTruthDoor(void);
static void BabiLighthouse_RedrawLedgePillarCells(void);

/// Unused: a signed division nothing calls.
static __inline__ s32 UNUSED Entrance_Divide(s32 dividend, s32 divisor) {
  return dividend / divisor;
}

#include "common/object_staged_object_event.h"

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Talking to the Djinni in BabiLighthouseLuff starts the battle with
/// Luff, a Jupiter Djinni.
static void BabiLighthouse_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(LOCALID_DJINNI, DJINNI_LUFF);
}

#include "common/object_waitidle.h"
#include "common/effect_advance.h"

/// Arriving by falling from the floor above: the leader drops in, lands with
/// a thud that shakes the screen and throws up a ring of dust, and is startled.
static void BabiLighthouse_LandWithParticles(void) {
  u32 particle;
  struct ObjectEvent *leader;
  struct EffectOptions options;
  s32 velocity[3];

  // Start above the room with movement disabled until the fall begins.
  Engine_ScriptStart();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_MapGridRedraw();
  Engine_TaskWait(1);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed = PIXELS(130);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->gravity = 0x8000;
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->bounce = 0;
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(30);
  Engine_AudioPlayCue(SE_204);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags =
      OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
  Engine_ScriptWait(24);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  // Spread the landing particles around the leader before restoring bounce.
  options.palette = 7;
  options.update = (void (*)(union FieldObject *))FieldEffect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (particle = 0; particle < 17; particle++) {
    SET_VECTOR(velocity, Engine_MathCos(particle << 12), 0,
               Engine_MathSin(particle << 12));
    velocity[0] += velocity[0] / 2;
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                      &options);
  }
  Engine_AudioPlayCue(SE_188);
  ObjectEvent_SetAttachedEffect(
      LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                 ANIMATION_LEADER_LEAN_BACK);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  Engine_MapGridRenderWaitForValues();
  ObjectEvent_SetAttachedEffect(
      LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->gravity = 0x10000;
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->bounce = 0x4000;
  Engine_ScriptEnd();
}

/// Arriving at one of the drop warp destinations of BabiLighthouseLuff: the
/// leader drops in and falls on, out through the given warp.
static void BabiLighthouse_FallToWarp(s32 warp) {
  struct ObjectEvent *leader;

  // Start above the room with movement disabled until the fall begins.
  Engine_ScriptStart();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_MapGridRedraw();
  Engine_TaskWait(1);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  leader->y.fixed = PIXELS(130);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  leader->gravity = 0x4000;
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  leader->bounce = 0;
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(LOCALID_PARTY_LEADER), 0);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(10);
  Engine_AudioPlayCue(SE_204);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags =
      OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  leader->velocityY = -0x50000;
  ObjectEvent_WaitUntilIdle(ObjectEvent_Get(LOCALID_PARTY_LEADER));
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 15);
  Engine_ScriptRequestWarp(warp);
  Engine_ScriptEnd();
}

const struct MapWarpDest *Map_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_BabiLighthouseEntry) {
    return gBabiLighthouse_EntryWarpDests;
  }
  if (mapId == (s32)&MapId_BabiLighthouseLuff) {
    return gBabiLighthouse_LuffWarpDests;
  }
  if (mapId == (s32)&MapId_BabiLighthouseStoneDoor) {
    return gBabiLighthouse_StoneDoorWarpDests;
  }
  return gBabiLighthouse_RoadWarpDests;
}

/// Only the Entry has regions: the three spots that drop into the Tunnel
/// Ruins.
const struct MapWarpRegion *Map_GetWarpRegions(void) {
  if (gGameState.mapId == (s32)&MapId_BabiLighthouseEntry) {
    return gBabiLighthouse_EntryRegions;
  }
  return 0;
}

/// The four maps share one warp table.
const u32 *BabiLighthouse_GetWarps(void) { return gBabiLighthouse_Warps; }

const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_BabiLighthouseEntry) {
    return gBabiLighthouse_EntryObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_BabiLighthouseLuff) {
    return gBabiLighthouse_LuffObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_BabiLighthouseStoneDoor) {
    return gBabiLighthouse_StoneDoorObjectEventTemplates;
  }
  return gBabiLighthouse_RoadObjectEventTemplates;
}

/// After Move on one of the pillars at the Babi Lighthouse Entry, or a push: a
/// leader left standing on a pillar's ledge cell jumps off it, and the pillars'
/// cells are redrawn.
static void BabiLighthouse_JumpFromLedge(void) {
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  Engine_ScriptStart();
  if (((leader->x.fixed >> 20) == 6 || (leader->x.fixed >> 20) == 18) &&
      (leader->z.fixed >> 20) == 20) {
    leader->targetX = OBJ_EVENT_NO_TARGET;
    leader->targetZ = OBJ_EVENT_NO_TARGET;
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x20000, 0x10000);
    Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 4, 0);
    {
      u16 facing = leader->facing;
      if ((u16)(facing + 0x4fff) <= 0x1fff ||
          (u16)(facing - 0x3001) <= 0x1fff) {
        Engine_ObjectEventSetDestinationOffset(LOCALID_PARTY_LEADER, 16, 0);
        Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
        ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 20);
      } else {
        FieldObjectMotion_OffsetPositionAndResetMotion(LOCALID_PARTY_LEADER, 0,
                                                       -16);
        Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
        ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 20);
      }
    }
  }
  BabiLighthouse_RedrawLedgePillarCells();
  Engine_ScriptEnd();
}

/// Walking into one of the pillars at the Babi Lighthouse Entry pushes it.
static void BabiLighthouse_PushLedgePillar(void) {
  Engine_ScriptStart();
  PushedObject_AdvancePair();
  BabiLighthouse_JumpFromLedge();
  Engine_ScriptEnd();
}

/// Walking south off the ledge at the Babi Lighthouse Entry: pushes the pillar
/// below on the leader's side if it stands at the ledge's foot, and otherwise
/// climbs down.
static void BabiLighthouse_PushLedgePillarOrClimbDown(void) {
  struct ObjectEvent *pillar = ObjectEvent_GetById(LOCALID_RIGHT_PILLAR);

  if ((ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed >> 20) <= 12) {
    pillar = ObjectEvent_GetById(LOCALID_LEFT_PILLAR);
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

/// The leader jumps down into the pit ahead (FieldObjectEvent_JumpDownLedge),
/// and the screen closes behind him.
static void BabiLighthouse_JumpIntoPit(void) {
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldObjectEvent_JumpDownLedge(LOCALID_PARTY_LEADER, 6, 0);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
}

/// Walking south into the east pit of BabiLighthouseLuff's west room: the
/// leader jumps in and drops into the east room.
static void BabiLighthouse_DropFromWestRoomEastPit(void) {
  Engine_ScriptStart();
  BabiLighthouse_JumpIntoPit();
  Engine_ScriptRequestWarp(
      BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_EAST);
  Engine_ScriptEnd();
}

/// Walking south into the pit of BabiLighthouseLuff's east room: the pit mask
/// is placed by it, and the leader jumps in and drops into the south room.
static void BabiLighthouse_DropFromEastRoomPit(void) {
  Engine_ScriptStart();
  ObjectEvent_PrepareObject(PIXELS(464), 0, PIXELS(290), PIT_MASK);
  BabiLighthouse_JumpIntoPit();
  Engine_ScriptRequestWarp(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_PIT);
  Engine_ScriptEnd();
}

/// Walking south into the southwest pit of BabiLighthouseLuff's west room:
/// the pit mask and a capped pillar are placed by it, and the leader jumps in
/// and drops into the east room.
static void BabiLighthouse_DropFromWestRoomSouthwestPit(void) {
  Engine_ScriptStart();
  ObjectEvent_PrepareObject(PIXELS(143), 0, PIXELS(290), PIT_MASK);
  ObjectEvent_PrepareObject(PIXELS(121), 0, PIXELS(286), SPRITE_CAPPED_PILLAR);
  BabiLighthouse_JumpIntoPit();
  Engine_ScriptRequestWarp(
      BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_SOUTHWEST);
  Engine_ScriptEnd();
}

/// Walking south into the northwest pit of BabiLighthouseLuff's west room: the
/// leader jumps in and drops into the east room.
static void BabiLighthouse_DropFromWestRoomNorthwestPit(void) {
  Engine_ScriptStart();
  BabiLighthouse_JumpIntoPit();
  Engine_ScriptRequestWarp(
      BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_NORTHWEST);
  Engine_ScriptEnd();
}

/// Searching the locked door in BabiLighthouseLuff. "It seems to be locked."
static void BabiLighthouse_InspectLockedDoor(void) {
  Engine_ScriptStart();
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_MessageShowCentered(MsgBabiSeemsLocked, 1);
  Engine_ScriptEnd();
}

/// A companion walks back to the leader and is moved out of sight to the map's
/// origin, so that the party walks as the leader alone again.
static inline void BabiLighthouse_WalkBackToLeader(s32 localId) {
  struct ObjectEvent *leader;

  Engine_ObjectEventSetAnimation(localId, ANIMATION_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetDestination(localId, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(localId);
  ObjectEvent_SetPosition(localId, 0, 0);
}

/// Walking up to the stone door in BabiLighthouseStoneDoor the first time:
/// Faran asks whether the kidnappers really went through it; the onlookers
/// argue, and Iodem, arriving with the party, tells who they are, and Faran,
/// doubting them, asks them to prove they can follow by opening the door.
static void BabiLighthouse_FaranAsksPartyToOpenDoor(void) {
  // Bring the party to the witnesses gathered around the sealed door.
  GameFlag_Set(FLAG_BABI_LIGHTHOUSE_ASKED_TO_FOLLOW);
  GameFlag_Set(FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED);
  Engine_ScriptStart();
  Field_EndBusyAndReveal();
  Engine_ScriptSetMessage(MsgBabiYoureSureTheyWentThrough);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x10000, 0x8000);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 296, 352);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ScriptWait(10);
  FieldMotion_LaunchFromFocusedObject(LOCALID_IODEM, 16, 0, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(GARET, -8, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(IVAN, 8, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(MIA, 24, 16, FACING_NORTH);
  Engine_ObjectEventWaitForMove(MIA);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(10);
  FieldCamera_SetSpeed(0x30000, 0x6000);
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(288), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_WITNESS, ANIMATION_NOD);
  Engine_ScriptWait(30);
  Script_ShowMessage(LOCALID_WITNESS, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_WITNESS, FACING_NORTH, 0);
  Engine_ScriptWait(30);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FARAN, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(10);
  // The witnesses try the stone door and disagree about who passed through it.
  Engine_ObjectEventStartRepeatedMotion(LOCALID_WORRIED_FOR_SHEBA, 2);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DOUBTING_THE_DOOR, 2);
  Engine_ScriptWait(40);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_WORRIED_FOR_SHEBA, 2);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DOUBTING_THE_DOOR, 2);
  Engine_ScriptWait(40);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_WORRIED_FOR_SHEBA, 2);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DOUBTING_THE_DOOR, 2);
  Engine_ScriptWait(40);
  ObjectEvent_ShowEmote(LOCALID_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_WINCE,
                        50);
  Script_ShowMessage(LOCALID_DOUBTING_THE_DOOR, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_WORRIED_FOR_SHEBA, FACING_EAST, 0);
  Engine_ScriptWait(25);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_WORRIED_FOR_SHEBA, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_WORRIED_FOR_SHEBA, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_DOUBTING_THE_DOOR, FACING_WEST, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_WORRIED_FOR_SHEBA,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_WORRIED_FOR_SHEBA, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_WITNESS, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_WITNESS, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_WORRIED_FOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL,
                        40);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_WORRIED_FOR_SHEBA, FACING_SOUTH, 0);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_WORRIED_FOR_SHEBA, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_WORRIED_FOR_SHEBA,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 75);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectEvent_FaceDirection(LOCALID_DOUBTING_THE_DOOR, FACING_SOUTH, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_WITNESS, FACING_SOUTH, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_SOUTH, 0);
  Engine_ScriptWait(30);
  Script_ShowMessage(LOCALID_FARAN, 0);
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(320), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  // The party learns where the kidnappers went and is asked to pursue them.
  Engine_ObjectEventSetAnimationAndWait(LOCALID_IODEM, ANIMATION_NOD);
  Engine_ScriptWait(30);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_IODEM, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Script_ShowMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessage(MIA, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(IVAN, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(20);
  Script_OpenMessage(GARET, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Engine_ScriptWait(30);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_WEST, 0);
    Engine_ScriptWait(30);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_IODEM, ANIMATION_NOD);
    Engine_ScriptWait(30);
    Script_ShowMessage(LOCALID_IODEM, 0);
    gFieldWork->message++;
  } else {
    Engine_ScriptWait(30);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_WEST, 0);
    Engine_ScriptWait(30);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_IODEM, ANIMATION_SHAKE_HEAD);
    Engine_ScriptWait(20);
    gFieldWork->message++;
    Script_ShowMessage(LOCALID_IODEM, 0);
  }
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectEvent_SetSpeed(LOCALID_FARAN, 0x10000, 0x8000);
  ObjectEvent_WalkByAndWait(LOCALID_FARAN, 0, 16);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 0);
  Engine_ScriptWait(35);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FARAN, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(MIA, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(IVAN, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(30);
  Script_ShowMessage(GARET, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_OpenMessage(LOCALID_FARAN, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Engine_ScriptWait(30);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_FARAN, ANIMATION_SHAKE_HEAD);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_FARAN, 0);
    gFieldWork->message++;
  } else {
    Engine_ScriptWait(30);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_FARAN, ANIMATION_SHAKE_HEAD);
    Engine_ScriptWait(20);
    gFieldWork->message++;
    Script_ShowMessage(LOCALID_FARAN, 0);
  }
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_WORRIED_FOR_SHEBA, 2);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_WORRIED_FOR_SHEBA, 0x14ccc, 0xa666);
  ObjectEvent_WalkByAndWait(LOCALID_WORRIED_FOR_SHEBA, 0, 16);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_WORRIED_FOR_SHEBA, 0);
  Engine_ScriptWait(10);
  ObjectEvent_SetSpeed(LOCALID_DOUBTING_THE_DOOR, 0x14ccc, 0xa666);
  ObjectEvent_WalkByAndWait(LOCALID_DOUBTING_THE_DOOR, 0, 16);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_SCOWL,
                        50);
  Script_ShowMessage(LOCALID_DOUBTING_THE_DOOR, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FARAN, ANIMATION_NOD);
  Engine_ScriptWait(30);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_WEST, 0);
  Engine_ScriptWait(25);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_IODEM, 2);
  Engine_ScriptWait(20);
  Script_OpenMessage(LOCALID_IODEM, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Engine_ScriptWait(30);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_IODEM, ANIMATION_NOD);
    Engine_ScriptWait(30);
    Script_ShowMessage(LOCALID_IODEM, 0);
    gFieldWork->message++;
  } else {
    Engine_ScriptWait(30);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_IODEM, ANIMATION_SHAKE_HEAD);
    Engine_ScriptWait(20);
    gFieldWork->message++;
    Script_ShowMessage(LOCALID_IODEM, 0);
  }
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 0);
  Engine_ScriptWait(35);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FARAN, ANIMATION_NOD);
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(40);
  Script_ShowMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventFaceEachOther(LOCALID_DOUBTING_THE_DOOR,
                                  LOCALID_WORRIED_FOR_SHEBA, 50);
  Engine_ObjectEventSetAnimation(LOCALID_DOUBTING_THE_DOOR, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_WORRIED_FOR_SHEBA,
                                        ANIMATION_NOD);
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(LOCALID_DOUBTING_THE_DOOR, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_WORRIED_FOR_SHEBA, FACING_SOUTH, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_DOUBTING_THE_DOOR, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_WORRIED_FOR_SHEBA,
                                        ANIMATION_NOD);
  Engine_ScriptWait(30);
  ObjectEvent_SetSpeed(LOCALID_DOUBTING_THE_DOOR, 0x10000, 0x8000);
  ObjectEvent_SetSpeed(LOCALID_WORRIED_FOR_SHEBA, 0x10000, 0x8000);
  ObjectEvent_WalkBy(LOCALID_DOUBTING_THE_DOOR, 32, 0);
  ObjectEvent_WalkByAndWait(LOCALID_WORRIED_FOR_SHEBA, 32, 0);
  ObjectEvent_WalkBy(LOCALID_DOUBTING_THE_DOOR, 0, 16);
  ObjectEvent_WalkByAndWait(LOCALID_WORRIED_FOR_SHEBA, 16, 0);
  ObjectEvent_WalkTo(LOCALID_WORRIED_FOR_SHEBA, 344, 312);
  ObjectEvent_WalkToAndWait(LOCALID_DOUBTING_THE_DOOR, 344, 336);
  Engine_ObjectEventSetAnimation(LOCALID_WORRIED_FOR_SHEBA, ANIMATION_STAND);
  ObjectEvent_FaceDirection(LOCALID_DOUBTING_THE_DOOR, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_WORRIED_FOR_SHEBA, FACING_WEST, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_SOUTH, 0);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(10);
  ObjectEvent_SetSpeed(LOCALID_FARAN, 0x10000, 0x8000);
  ObjectEvent_WalkToAndWait(LOCALID_FARAN, 328, 312);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_WEST, 0);
  Engine_ScriptWait(30);
  ObjectEvent_SetSpeed(LOCALID_WITNESS, 0x10000, 0x8000);
  ObjectEvent_WalkToAndWait(LOCALID_WITNESS, 328, 328);
  ObjectEvent_FaceDirection(LOCALID_WITNESS, FACING_WEST, 0);
  Engine_ScriptWait(20);
  // The companions walk back into the leader.
  ObjectEvent_SetSpeed(GARET, 0x13333, 0x9999);
  ObjectEvent_SetSpeed(IVAN, 0x13333, 0x9999);
  ObjectEvent_SetSpeed(MIA, 0x13333, 0x9999);
  BabiLighthouse_WalkBackToLeader(GARET);
  BabiLighthouse_WalkBackToLeader(IVAN);
  BabiLighthouse_WalkBackToLeader(MIA);
  Engine_ScriptWait(10);
  Engine_ScriptEnd();
}

/// The stone door of BabiLighthouseStoneDoor opens before Iodem and Faran the
/// first time: everyone marvels, the door shuts again in front of Iodem, the
/// party reasons that it keeps out those without Psynergy, and Iodem and Faran
/// leave Sheba's rescue to the party.
static void BabiLighthouse_ExplainTruthDoor(void) {
  // Everyone reacts to the opened door before trying to follow the party.
  GameFlag_Set(FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN);
  Engine_ScriptStart();
  Field_EndBusyAndReveal();
  Engine_ScriptSetMessage(MsgBabiTheDoor);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x10000, 0x8000);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 296, 312);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  Engine_ScriptWait(10);
  ObjectMotion_LaunchFromFocusedObject(GARET, 0, 16, FACING_EAST);
  FieldMotion_LaunchFromFocusedObject(IVAN, -16, -8, FACING_EAST);
  ObjectMotion_LaunchFromFocusedObject(MIA, -16, 24, FACING_EAST);
  Engine_ObjectEventWaitForMove(MIA);
  Engine_ScriptWait(20);
  FieldCamera_SetSpeed(0x30000, 0x6000);
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(328), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(10);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Engine_ObjectEventJump(LOCALID_IODEM, 4, 13);
  Engine_ObjectEventJump(LOCALID_IODEM, 4, 30);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_DOUBTING_THE_DOOR,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_WORRIED_FOR_SHEBA,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_WITNESS, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_DOUBTING_THE_DOOR,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_WORRIED_FOR_SHEBA,
                            FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FARAN, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_WORRIED_FOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  Script_ShowMessage(LOCALID_WORRIED_FOR_SHEBA, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_DOUBTING_THE_DOOR,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  ObjectEvent_FaceDirection(LOCALID_DOUBTING_THE_DOOR, FACING_WEST, 0);
  Engine_ScriptWait(25);
  Script_ShowMessage(LOCALID_DOUBTING_THE_DOOR, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_WITNESS, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_WORRIED_FOR_SHEBA, FACING_WEST, 0);
  Engine_ScriptWait(30);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(MIA, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(MIA, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(IVAN, 0);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(GARET, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_IODEM, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FARAN, 2);
  Engine_ScriptWait(25);
  Script_ShowMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_IODEM, 2);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_IODEM, 0x10000, 0x8000);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, -40);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 0);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FARAN, ANIMATION_NOD);
  Engine_ScriptWait(30);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_WORRIED_FOR_SHEBA, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_DOUBTING_THE_DOOR, FACING_NORTH, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimation(LOCALID_DOUBTING_THE_DOOR, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_WORRIED_FOR_SHEBA,
                                        ANIMATION_NOD);
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(LOCALID_WORRIED_FOR_SHEBA, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_DOUBTING_THE_DOOR, FACING_WEST, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_IODEM, ANIMATION_NOD);
  Engine_ScriptWait(30);
  Engine_CameraFollowObjectEvent(LOCALID_IODEM, 1);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, -32);
  // Demonstrate that the door closes again when someone without Psynergy
  // enters.
  BabiLighthouse_CloseTruthDoor();
  GameFlag_Clear(FLAG_BEYOND_DOOR);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_ScriptStart();
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_WITNESS, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_DOUBTING_THE_DOOR,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_WORRIED_FOR_SHEBA,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  Engine_ScriptWait(30);
  ObjectEvent_ShowEmote(LOCALID_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_DOUBTING_THE_DOOR,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_WORRIED_FOR_SHEBA,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(312), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  Script_ShowMessage(LOCALID_DOUBTING_THE_DOOR, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_WORRIED_FOR_SHEBA,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_WORRIED_FOR_SHEBA, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHEAST, 0);
  Engine_ScriptWait(20);
  Script_ShowMessage(IVAN, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 0);
  Engine_ScriptWait(30);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ScriptWait(20);
  Script_ShowMessage(MIA, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessage(GARET, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_WEST, 0);
  Engine_ScriptWait(20);
  // Explain the restriction and ask the onlookers to leave the pursuit to us.
  Script_OpenMessage(LOCALID_FARAN, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_WITNESS, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_DOUBTING_THE_DOOR, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_WORRIED_FOR_SHEBA, FACING_WEST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_EAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Engine_ScriptWait(30);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_FARAN, 2);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_FARAN, 0);
    gFieldWork->message++;
  } else {
    Engine_ScriptWait(30);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_FARAN, 2);
    Engine_ScriptWait(20);
    gFieldWork->message++;
    Script_ShowMessage(LOCALID_FARAN, 0);
  }
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_IODEM, 2);
  Engine_ScriptWait(20);
  ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, 16);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_NORTHWEST, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FARAN, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(10);
  // Iodem and Faran agree to stay behind and leave the rescue to the party.
  Engine_ObjectEventSetAnimationAndWait(LOCALID_IODEM, ANIMATION_NOD);
  Engine_ScriptWait(30);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FARAN, 2);
  Engine_ScriptWait(20);
  Script_OpenMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Engine_ScriptWait(30);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_IODEM, ANIMATION_NOD);
    Engine_ScriptWait(30);
    Script_ShowMessage(LOCALID_IODEM, 0);
    gFieldWork->message++;
  } else {
    Engine_ScriptWait(30);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_IODEM, ANIMATION_SHAKE_HEAD);
    Engine_ScriptWait(20);
    gFieldWork->message++;
    Script_ShowMessage(LOCALID_IODEM, 0);
  }
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Script_ShowMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(15);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_IODEM, ANIMATION_NOD);
  Engine_ScriptWait(30);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FARAN, 2);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_WEST, 0);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FARAN, ANIMATION_NOD);
  Engine_ScriptWait(30);
  Script_ShowMessage(LOCALID_FARAN, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptWait(25);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_IODEM, 2);
  Engine_ScriptWait(20);
  ObjectEvent_WalkToAndWait(LOCALID_IODEM, 312, 312);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_WEST, 0);
  Engine_ScriptWait(25);
  Script_ShowMessage(LOCALID_IODEM, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, GARET, 0);
  Engine_ObjectEventFaceEachOther(MIA, IVAN, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(MIA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_ScriptWait(30);
  // The companions walk back into the leader.
  ObjectEvent_SetSpeed(GARET, 0x13333, 0x9999);
  ObjectEvent_SetSpeed(IVAN, 0x13333, 0x9999);
  ObjectEvent_SetSpeed(MIA, 0x13333, 0x9999);
  BabiLighthouse_WalkBackToLeader(GARET);
  BabiLighthouse_WalkBackToLeader(IVAN);
  BabiLighthouse_WalkBackToLeader(MIA);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_WITNESS, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_DOUBTING_THE_DOOR,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_WORRIED_FOR_SHEBA,
                            FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(30);
  Engine_ScriptEnd();
}

/// Reveal at BabiLighthouseStoneDoor shows the stone door's switch, flipped one
/// way or the other as the door stands.
static void BabiLighthouse_ShowTruthDoorSwitch(void) {
  if (Engine_GameFlagIsSet(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) == 0) {
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
  if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) == 0) {
    GameFlag_Set(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN);
    Engine_AudioPlayCue(SE_GATE_MOVE);
    Engine_ScriptStart();
    ObjectEvent_SetDestination(LOCALID_LEFT_DOOR, 280, 240);
    ObjectEvent_SetDestination(LOCALID_RIGHT_DOOR, 328, 240);
    Engine_ObjectEventWaitForMove(LOCALID_LEFT_DOOR);
    Engine_ObjectEventWaitForMove(LOCALID_RIGHT_DOOR);
    MapGrid_CopyCellAttributes(81, 14, 4, 1, 17, 14);
    Engine_ScriptEnd();
    if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN) == 0) {
      BabiLighthouse_ExplainTruthDoor();
    }
  }
}

/// Walking through the open door: it slides shut behind the party, now on its
/// other side.
static void BabiLighthouse_CloseTruthDoor(void) {
  if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) != 0) {
    GameFlag_Clear(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN);
    Engine_AudioPlayCue(SE_GATE_MOVE);
    Engine_ScriptStart();
    ObjectEvent_SetDestination(LOCALID_LEFT_DOOR, 296, 240);
    ObjectEvent_SetDestination(LOCALID_RIGHT_DOOR, 312, 240);
    Engine_ObjectEventWaitForMove(LOCALID_LEFT_DOOR);
    Engine_ObjectEventWaitForMove(LOCALID_RIGHT_DOOR);
    MapGrid_CopyCellAttributes(0, 14, 4, 1, 17, 14);
    Engine_ScriptEnd();
    if (GameFlag_IsSet(FLAG_BEYOND_DOOR) != 0) {
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
  struct FieldWork *work = (struct FieldWork *)gFieldWork;

  Engine_ScriptStart();
  if (work->revealActive != 0) {
    if (Engine_GameFlagIsSet(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) == 0) {

      Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
      Engine_AudioPlayCue(SE_155);
      MapGrid_CopyCellsLayered(35, 78, 1, 2, 17, 78);
      Engine_ScriptWait(10);
      MapGrid_CopyCellsLayered(34, 78, 1, 2, 17, 78);
      Engine_ScriptWait(10);
      BabiLighthouse_OpenTruthDoor();
    }
  } else {
    Engine_ScriptSetMessage(MsgBabiTruthDoorOpenThoseSeeing);
    Engine_ScriptShowMessage(-1, 0);
  }
  Engine_ScriptEnd();
}

/// Finds the pushed object one direction-table step ahead, including
/// ground height.
static struct ObjectEvent *
BabiLighthouse_FindObjectEventAhead(struct ObjectEvent *objectEvent) {
  struct FieldPosition position;
  struct FieldPosition *target = &position;
  s32 directionStep = gPushedObject_DirectionSteps[objectEvent->facing >> 12];

  {
    s32 x = objectEvent->x.fixed;
    s32 z = objectEvent->z.fixed;

    x += -0x10000 & directionStep;
    z += directionStep << 16;
    target->x = x;
    target->z = z;
  }
  target->y =
      MapGrid_GetTerrainHeight(objectEvent->layer, target->x, target->z);
  return PushedObject_FindAtTile(target, objectEvent);
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
  struct ObjectEvent *pillar = ObjectEvent_GetById(LOCALID_ROAD_PILLAR);
  s32 column = pillar->x.fixed >> 20;

  if (column == 20) {
    s32 row = pillar->z.fixed >> 20;

    if (row == 12) {
      pillar->motionFlags = OBJ_EVENT_FALLS;
      pillar->groundHeight = PIXELS(48);
      pillar->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
      {
        s32 targetColumn = column, targetRow = row;

        MapGrid_CopyCellAttributes(38, 12, 1, 1, targetColumn, targetRow);
      }
    }
  }
}

/// Walking into the pillar on the Road to Babi Lighthouse: the leader pushes it
/// (PushedObject_AdvancePair) when it stands within 8 pixels of his
/// height.
static void BabiLighthouse_PushRoadPillar(void) {
  // Trial: inverting the empty success arm changes two branch instructions
  // (three text bytes in tbs-en); retain the existing branch order.
  struct ObjectEvent *leader =
      (struct ObjectEvent *)ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct ObjectEvent *ahead = BabiLighthouse_FindObjectEventAhead(leader);
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

  PushedObject_AdvancePair();
}

/// Walking into the climbable cells by the Road to Babi Lighthouse's pillar:
/// the leader climbs up or down where he faces north or south, already
/// climbing or with no pillar ahead, and pushes the pillar
/// (BabiLighthouse_PushRoadPillar) when he does not climb.
static void BabiLighthouse_ClimbOrPushRoadPillar(void) {
  struct ObjectEvent *leader =
      (struct ObjectEvent *)ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct ObjectEvent *ahead = BabiLighthouse_FindObjectEventAhead(leader);
  s32 climbDirection = (leader->facing + 0x2000) & 0xc000;
  s32 climbResult = -1;

  if (gGameState.movementMode == LEADER_MOVEMENT_CLIMB || ahead == 0) {
    if (climbDirection == FACING_NORTH) {
      climbResult = Field_ClimbUp();
    }
    if (climbDirection == FACING_SOUTH) {
      climbResult = Field_ClimbDown();
    }
  }
  if (climbResult != 0) {
    if (gGameState.movementMode != 1) {
      BabiLighthouse_PushRoadPillar();
    }
  }
}

const struct MapEvent *Map_GetEvents(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_BabiLighthouseEntry) {
    return gBabiLighthouse_EntryEvents;
  }
  if (mapId == (s32)&MapId_BabiLighthouseLuff) {
    return gBabiLighthouse_LuffEvents;
  }
  if (mapId == (s32)&MapId_BabiLighthouseStoneDoor) {
    return gBabiLighthouse_StoneDoorEvents;
  }
  return gBabiLighthouse_RoadEvents;
}

/// Growth on the sprout at the Babi Lighthouse Entry: it grows into a vine the
/// party can climb.
static void BabiLighthouse_OpenGrowthPassage(void) {

  MapGrid_CopyCellAttributes(82, 7, 1, 2, 18, 7);
  Engine_TaskWait(1);
  Engine_GameFlagSet(FLAG_GROWTH_PASSAGE_OPEN);
}

/// Redraws the ledge cells of the Babi Lighthouse Entry, then blocks the cells
/// under its two pillars.
static void BabiLighthouse_RedrawLedgePillarCells(void) {
  struct ObjectEvent *leftPillar = ObjectEvent_GetById(LOCALID_LEFT_PILLAR);

  Engine_ObjectEventSetSpritePriority(LOCALID_LEFT_PILLAR, 1);
  Engine_ObjectEventSetSpritePriority(LOCALID_RIGHT_PILLAR, 1);
  MapGrid_CopyCellAttributes(69, 19, 3, 3, 5, 19);
  MapGrid_CopyCellAttributes(69, 19, 3, 3, 17, 19);
  {
    s32 column = leftPillar->x.fixed >> 20, row = leftPillar->z.fixed >> 20;

    MapGrid_CopyCellAttributes(3, 3, 1, 1, column, row);
  }
  {
    struct ObjectEvent *rightPillar = ObjectEvent_GetById(LOCALID_RIGHT_PILLAR);
    s32 column = rightPillar->x.fixed >> 20, row = rightPillar->z.fixed >> 20;

    MapGrid_CopyCellAttributes(3, 3, 1, 1, column, row);
  }
}

#if TBS_PATCH_2
/// Update callback of the pillars at the Babi Lighthouse Entry (TBS_PATCH_2 and
/// later): each is drawn in front of the leader when nearer the screen, and
/// on its layer otherwise.
static void BabiLighthouse_MatchLeaderPriority(union FieldObject *fieldObject) {
  struct Sprite *sprite;
  struct ObjectEvent *objectEvent;
  s32 priority;

  sprite = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->sprite;
  objectEvent = ObjectEvent_GetById(LOCALID_LEFT_PILLAR);
  if (objectEvent->z.fixed <
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed) {
    Engine_ObjectEventSetSpritePriority(LOCALID_LEFT_PILLAR,
                                        sprite->image.priority);
  } else {
    priority = sprite->image.priority;
    if (priority != 1)
      priority--;
    Engine_ObjectEventSetSpritePriority(LOCALID_LEFT_PILLAR, priority);
    ObjectEvent_GetById(LOCALID_LEFT_PILLAR)->priorityFlags |=
        OBJ_EVENT_PRIORITY_AUTOMATIC;
  }
  objectEvent = ObjectEvent_GetById(LOCALID_RIGHT_PILLAR);
  if (objectEvent->z.fixed <
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed) {
    Engine_ObjectEventSetSpritePriority(LOCALID_RIGHT_PILLAR,
                                        sprite->image.priority);
  } else {
    priority = sprite->image.priority;
    if (priority != 1)
      priority--;
    Engine_ObjectEventSetSpritePriority(LOCALID_RIGHT_PILLAR, priority);
    ObjectEvent_GetById(LOCALID_RIGHT_PILLAR)->priorityFlags |=
        OBJ_EVENT_PRIORITY_AUTOMATIC;
  }
}
#endif

/// Sets up the map being entered: Retreat leads back to the Entry; the road's
/// pillar, the Entry's pillars, vine and the four lying on the ground,
/// BabiLighthouseLuff's arrivals by falling, and BabiLighthouseStoneDoor's door
/// and onlookers are restored. Returns 0.
s32 BabiLighthouse_SetupMap(void) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *placedObjectEvent;
  s32 mapId;
  s32 mapCopy;
  struct GameState *state;
  s32 x;
  s32 z;
  u8 zero;

  Engine_TaskWait(1);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  state = &gGameState;
  mapId = state->mapId;
  if (mapId != (s32)&MapId_RoadToBabiLighthouse) {
    state->retreatWarpDest = 1;
    state->retreatMap = (s32)&MapId_BabiLighthouseEntry;
    mapCopy = state->mapId;
  }
  // The road: the moved pillar's metatile behavior where it stands.
  if (mapId == (s32)&MapId_RoadToBabiLighthouse) {
    placedObjectEvent = ObjectEvent_GetById(LOCALID_ROAD_PILLAR);
    x = placedObjectEvent->x.fixed >> 20;
    if (x == 20) {
      z = placedObjectEvent->z.fixed >> 20;
      if (z == 12) {
        MapGrid_CopyCellAttributes(38, 12, 1, 1, x, z);
      }
    }
  } else if (mapCopy == (s32)&MapId_BabiLighthouseEntry) {
    // Set up the movable ledge pillars and the staggered object event
    // animations.
    ObjectEvent_SetChildValue(LOCALID_LEFT_PILLAR, 6);
    ObjectEvent_SetChildValue(LOCALID_RIGHT_PILLAR, 6);
#if TBS_PATCH_2
    ((union FieldObject *)ObjectEvent_GetById(LOCALID_LEFT_PILLAR))
        ->objectEvent.update = BabiLighthouse_MatchLeaderPriority;
    ((union FieldObject *)ObjectEvent_GetById(LOCALID_RIGHT_PILLAR))
        ->objectEvent.update = BabiLighthouse_MatchLeaderPriority;
#endif
    if (state->warpDest ==
            BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTH &&
        GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      ObjectEvent_SetPosition(LOCALID_RIGHT_PILLAR, PIXELS(312), PIXELS(328));
    }
    BabiLighthouse_RedrawLedgePillarCells();
    objectEvent = ObjectEvent_GetById(LOCALID_RIGHT_PILLAR);
    objectEvent->y.fixed =
        ObjectEvent_GetById(LOCALID_RIGHT_PILLAR)->groundHeight;
    if (GameFlag_IsSet(FLAG_GROWTH_PASSAGE_OPEN) != 0) {
      Engine_ObjectEventSetAnimation(LOCALID_PLANT, 4);
      ObjectEvent_GetById(LOCALID_PLANT)->collisionFlags = 0xfe;
      BabiLighthouse_OpenGrowthPassage();
    }
    // Their script waits stagger the first animations of the four lying on the
    // ground.
    placedObjectEvent = ObjectEvent_GetById(LOCALID_FALLEN_SOLDIER_NO_MATCH);
    placedObjectEvent->collisionFlags = 0;
    placedObjectEvent->priorityFlags = 0;
    placedObjectEvent->wait = 0;
    placedObjectEvent->sprite->image.priority = 3;
    zero = 0;
    placedObjectEvent->sprite->flags = zero;
    placedObjectEvent->sprite->rotation = 0xc000;
    Engine_ObjectEventSetAnimation(LOCALID_FALLEN_SOLDIER_NO_MATCH, 0);
    placedObjectEvent = ObjectEvent_GetById(LOCALID_FALLEN_STRONGMAN_ONLY_TWO);
    placedObjectEvent->collisionFlags = zero;
    placedObjectEvent->priorityFlags = zero;
    placedObjectEvent->wait = 30;
    placedObjectEvent->sprite->image.priority = 3;
    placedObjectEvent->sprite->flags = zero;
    placedObjectEvent->sprite->rotation = 0x4000;
    Engine_ObjectEventSetAnimation(LOCALID_FALLEN_STRONGMAN_ONLY_TWO, 0);
    placedObjectEvent =
        ObjectEvent_GetById(LOCALID_FALLEN_SOLDIER_KICKED_ASIDE);
    placedObjectEvent->collisionFlags = zero;
    placedObjectEvent->priorityFlags = zero;
    placedObjectEvent->wait = 60;
    placedObjectEvent->sprite->image.priority = 3;
    placedObjectEvent->sprite->flags = zero;
    placedObjectEvent->sprite->rotation = 0x8000;
    Engine_ObjectEventSetAnimation(LOCALID_FALLEN_SOLDIER_KICKED_ASIDE, 0);
    placedObjectEvent = ObjectEvent_GetById(LOCALID_FALLEN_MAN_SCORCHED_GROUND);
    placedObjectEvent->collisionFlags = zero;
    placedObjectEvent->priorityFlags = zero;
    placedObjectEvent->wait = 90;
    placedObjectEvent->sprite->image.priority = 3;
    placedObjectEvent->sprite->flags = zero;
    placedObjectEvent->sprite->rotation = 0x8000;
    Engine_ObjectEventSetAnimation(LOCALID_FALLEN_MAN_SCORCHED_GROUND, 0);
  } else if (mapCopy == (s32)&MapId_BabiLighthouseLuff) {
    // Match the arrival effect and fall to the warp destination used in this
    // room.
    switch (state->warpDest) {
    case BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_STONE_DOOR_WEST:
      GameFlag_Set(FLAG_BABI_LIGHTHOUSE_BACK_FROM_STONE_DOOR);
      // Both warp destinations restore the same remembered cell changes.
      // fall through
    case BABI_LIGHTHOUSE_LUFF_LOCKED_DOOR_ROOM_FROM_SOUTH_ROOM:
      if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_BACK_FROM_STONE_DOOR) != 0) {
        MapGrid_CopyCellsTo(120, 7, 109, 7, 1, 3);
        MapGrid_CopyCellAttributes(45, 8, 1, 1, 45, 9);
      }
      break;
    case BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_SOUTHWEST:
      ObjectEvent_PrepareObject(PIXELS(440), 0, PIXELS(290), PIT_MASK);
      MapGrid_CopyCellAttributes(22, 13, 1, 1, 27, 13);
      BabiLighthouse_FallToWarp(
          BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_FALL_SOUTH);
      break;
    case BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_NORTHWEST:
      ObjectEvent_PrepareObject(PIXELS(448), 0, PIXELS(290), PIT_MASK);
      MapGrid_CopyCellAttributes(22, 12, 1, 1, 28, 10);
      BabiLighthouse_FallToWarp(
          BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_FALL_NORTH);
      break;
    case BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_FALL_NORTH:
      ObjectEvent_PrepareObject(PIXELS(232), 0, PIXELS(594), PIT_MASK);
      MapGrid_CopyCellAttributes(22, 12, 1, 1, 14, 33);
      BabiLighthouse_FallToWarp(
          BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_STONE_DOOR_EAST);
      break;
    case BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_EAST:
    case BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_PIT:
    case BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_FALL_SOUTH:
      if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
        BabiLighthouse_LandWithParticles();
      }
      break;
    }
  } else if (mapId == (s32)&MapId_BabiLighthouseStoneDoor) {
    // Restore the truth door and the onlookers' positions after each
    // discussion.
    ObjectEvent_GetById(LOCALID_LEFT_DOOR)->steeringFlags &=
        ~OBJ_EVENT_FACES_MOTION;
    ObjectEvent_GetById(LOCALID_RIGHT_DOOR)->steeringFlags &=
        ~OBJ_EVENT_FACES_MOTION;
    ObjectEvent_SetSpeed(LOCALID_LEFT_DOOR, 0x10000, 0x8000);
    ObjectEvent_SetSpeed(LOCALID_RIGHT_DOOR, 0x10000, 0x8000);
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      if (state->warpDest ==
          BABI_LIGHTHOUSE_STONE_DOOR_FROM_TUNNEL_RUINS_GREAT_STATUE) {
        GameFlag_Set(FLAG_BEYOND_DOOR);
      } else {
        GameFlag_Clear(FLAG_BEYOND_DOOR);
      }
    }
    if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_ASKED_TO_FOLLOW) == 0) {
      ObjectEvent_SetPosition(LOCALID_IODEM, -PIXELS(64), -PIXELS(64));
      ObjectEvent_SetPosition(LOCALID_WITNESS, PIXELS(280), PIXELS(296));
      ObjectEvent_SetPosition(LOCALID_DOUBTING_THE_DOOR, PIXELS(312),
                              PIXELS(248));
      ObjectEvent_SetPosition(LOCALID_WORRIED_FOR_SHEBA, PIXELS(296),
                              PIXELS(248));
      ObjectEvent_SetPosition(LOCALID_FARAN, PIXELS(320), PIXELS(296));
      ObjectEvent_FaceDirection(LOCALID_WITNESS, FACING_EAST, 0);
      ObjectEvent_FaceDirection(LOCALID_DOUBTING_THE_DOOR, FACING_NORTH, 0);
      ObjectEvent_FaceDirection(LOCALID_WORRIED_FOR_SHEBA, FACING_NORTH, 0);
      ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_WEST, 0);
      Engine_ScriptWait(5);
    } else if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN) != 0) {
      ObjectEvent_SetPosition(LOCALID_IODEM, PIXELS(312), PIXELS(312));
      ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTHWEST + FACING_STEP,
                                0);
      ObjectEvent_FaceDirection(LOCALID_WITNESS, FACING_NORTHWEST + FACING_STEP,
                                0);
      ObjectEvent_FaceDirection(LOCALID_DOUBTING_THE_DOOR,
                                FACING_NORTHWEST + FACING_STEP, 0);
      ObjectEvent_FaceDirection(LOCALID_WORRIED_FOR_SHEBA,
                                FACING_NORTHWEST + FACING_STEP, 0);
      ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_NORTHWEST + FACING_STEP,
                                0);
      Engine_ScriptWait(5);
    }
    if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) != 0) {
      ObjectEvent_SetPosition(LOCALID_LEFT_DOOR, PIXELS(280), PIXELS(240));
      ObjectEvent_SetPosition(LOCALID_RIGHT_DOOR, PIXELS(328), PIXELS(240));
      ObjectEvent_FaceDirection(LOCALID_LEFT_DOOR, FACING_WEST, 0);
      ObjectEvent_FaceDirection(LOCALID_RIGHT_DOOR, FACING_EAST, 0);
      MapGrid_CopyCellAttributes(81, 14, 4, 1, 17, 14);
    }
    if (gGameState.warpDest ==
        BABI_LIGHTHOUSE_STONE_DOOR_FROM_BABI_LIGHTHOUSE_LUFF_EAST) {
      if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
        BabiLighthouse_LandWithParticles();
      }
    }
  } else {
    Engine_ObjectEventSetAnimation(LOCALID_ROAD_PILLAR, 2);
  }
  return 0;
}

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gPushedObject_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

u32 gPushedObject_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedObject_FootprintBounds[] = {
    -32, -8,  32, 8,  -8,  -32, 8,  32, -32, -16, 32, 0,
    -8,  -32, 8,  32, -32, -8,  32, 8,  -8,  -32, 8,  32,
};

s32 gShortEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gMediumEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gLongEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gShortEffectScript,
    gMediumEffectScript,
    gLongEffectScript,
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

struct MapWarpDest gBabiLighthouse_EntryWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 136, 0, 376, FACING_SOUTH, 0, -1, -1,
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
    {MAP_TABLE_END},
};

struct MapWarpDest gBabiLighthouse_LuffWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 136, 0, 376, FACING_SOUTH, 0, -1, -1,
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
    {MAP_TABLE_END},
};

struct MapWarpDest gBabiLighthouse_StoneDoorWarpDests[] = {
    {BABI_LIGHTHOUSE_STONE_DOOR_FROM_TUNNEL_RUINS_GREAT_STATUE,
     CONDITION_ALWAYS, 248, 0, 120, FACING_SOUTH, 0, 96, 48, 384, 432, 0},
    {BABI_LIGHTHOUSE_STONE_DOOR_FROM_BABI_LIGHTHOUSE_LUFF_WEST,
     CONDITION_ALWAYS, 136, 0, 368, FACING_NORTH, 0, 96, 48, 384, 432, 0},
    {BABI_LIGHTHOUSE_STONE_DOOR_FROM_BABI_LIGHTHOUSE_LUFF_EAST,
     CONDITION_ALWAYS, 168, 0, 344, FACING_SOUTH, 0, 96, 48, 384, 432, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gBabiLighthouse_RoadWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 328, 0, 240, FACING_NORTH, 0, 0, 0,
     464, 400, 0},
    {ROAD_TO_BABI_LIGHTHOUSE_FROM_WORLD_MAP, CONDITION_ALWAYS, 112, 0, 360,
     FACING_NORTH, 0, 0, 0, 464, 400, 0},
    {ROAD_TO_BABI_LIGHTHOUSE_FROM_BABI_LIGHTHOUSE_ENTRY, CONDITION_ALWAYS, 136,
     0, 32, FACING_SOUTH, 0, 0, 0, 464, 400, 0},
    {MAP_TABLE_END},
};

struct MapWarpRegion gBabiLighthouse_EntryRegions[] = {
    {96, -20, 320, 112, -4, 336, CONDITION_ALWAYS,
     BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_WEST},
    {288, -20, 320, 304, -4, 336, CONDITION_ALWAYS,
     BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_SOUTH},
    {304, -20, 96, 320, -4, 112, CONDITION_ALWAYS,
     BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_NORTH},
    {MAP_TABLE_END},
};

u32 gBabiLighthouse_Warps[] = {
    MAP_WARPS(BabiLighthouseEntry),
    MAP_WARP(BABI_LIGHTHOUSE_ENTRY_TO_ROAD_TO_BABI_LIGHTHOUSE,
             RoadToBabiLighthouse,
             ROAD_TO_BABI_LIGHTHOUSE_FROM_BABI_LIGHTHOUSE_ENTRY),
    MAP_WARP(BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_EAST,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_ENTRY_EAST),
    MAP_WARP(BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_WEST,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_ENTRY_WEST),
    MAP_WARP(BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_WEST,
             TunnelRuinsLadyStatues,
             TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_WEST),
    MAP_WARP(BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_SOUTH,
             TunnelRuinsLadyStatues,
             TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_SOUTH),
    MAP_WARP(BABI_LIGHTHOUSE_ENTRY_TO_TUNNEL_RUINS_LADY_STATUES_NORTH,
             TunnelRuinsLadyStatues,
             TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_NORTH),
    MAP_WARPS(BabiLighthouseLuff),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_STEPS,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_STEPS),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_ENTRY_EAST,
             BabiLighthouseEntry,
             BABI_LIGHTHOUSE_ENTRY_FROM_BABI_LIGHTHOUSE_LUFF_EAST),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_WEST_ROOM_STEPS,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_WEST_ROOM_FROM_EAST_ROOM_STEPS),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_WEST_STEPS,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_WEST_STEPS),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_EAST_STEPS,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_EAST_STEPS),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_ENTRY_WEST,
             BabiLighthouseEntry,
             BABI_LIGHTHOUSE_ENTRY_FROM_BABI_LIGHTHOUSE_LUFF_WEST),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_WEST_STEPS,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_SOUTH_ROOM_WEST_STEPS),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_EAST_STEPS,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_SOUTH_ROOM_EAST_STEPS),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_LOCKED_DOOR_ROOM,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_LOCKED_DOOR_ROOM_FROM_SOUTH_ROOM),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_LOCKED_DOOR_ROOM_TO_SOUTH_ROOM,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_LOCKED_DOOR_ROOM),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_EAST,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_EAST),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_PIT,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_PIT),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_SOUTHWEST,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_SOUTHWEST),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_FALL_SOUTH,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_FALL_SOUTH),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_NORTHWEST,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_NORTHWEST),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_FALL_NORTH,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_FALL_NORTH),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_STONE_DOOR_EAST,
             BabiLighthouseStoneDoor,
             BABI_LIGHTHOUSE_STONE_DOOR_FROM_BABI_LIGHTHOUSE_LUFF_EAST),
    MAP_WARP(BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_STONE_DOOR_WEST,
             BabiLighthouseStoneDoor,
             BABI_LIGHTHOUSE_STONE_DOOR_FROM_BABI_LIGHTHOUSE_LUFF_WEST),
    MAP_WARPS(BabiLighthouseStoneDoor),
    MAP_WARP(BABI_LIGHTHOUSE_STONE_DOOR_TO_TUNNEL_RUINS_GREAT_STATUE,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR),
    MAP_WARP(BABI_LIGHTHOUSE_STONE_DOOR_TO_BABI_LIGHTHOUSE_LUFF_WEST,
             BabiLighthouseLuff,
             BABI_LIGHTHOUSE_LUFF_FROM_BABI_LIGHTHOUSE_STONE_DOOR_WEST),
    MAP_WARPS(RoadToBabiLighthouse),
    MAP_WARP(ROAD_TO_BABI_LIGHTHOUSE_TO_WORLD_MAP, WorldMap,
             WORLD_MAP_FROM_ROAD_TO_BABI_LIGHTHOUSE),
    MAP_WARP(ROAD_TO_BABI_LIGHTHOUSE_TO_BABI_LIGHTHOUSE_ENTRY,
             BabiLighthouseEntry,
             BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE),
    MAP_WARPS_END,
};

/// Unused: an empty object event template table nothing returns.
struct ObjectEventTemplate gBabiLighthouse_NoObjectEventTemplates[] = {
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gBabiLighthouse_EntryObjectEventTemplates[] = {
    {SPRITE_CAPPED_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
     PIXELS(88), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_CAPPED_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
     PIXELS(296), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP,
     OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE},
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
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gBabiLighthouse_LuffObjectEventTemplates[] = {
    {JUPITER_DJINNI, FLAG_DJINNI(DJINNI_LUFF), MOVEMENT_TYPE_STAND, PIXELS(472),
     0, PIXELS(184), FACING_SOUTHEAST, TALK_FACING_PARTY, 0},
    {MAP_TABLE_END},
};

// clang-format off
struct ObjectEventTemplate gBabiLighthouse_StoneDoorObjectEventTemplates[] = {
    {GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(-64), 0, PIXELS(-64), FACING_EAST,
        TALK_FACING_PARTY, 0},
    {IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACING_PARTY,
        0},
    {MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(-64), 0, PIXELS(-64), FACING_EAST,
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
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gBabiLighthouse_RoadObjectEventTemplates[] = {
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
    {MAP_TABLE_END},
};

struct MapEvent gBabiLighthouse_EntryEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, BABI_LIGHTHOUSE_ENTRY_TO_ROAD_TO_BABI_LIGHTHOUSE},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_EAST},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS, BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_WEST},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_PushLedgePillar)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_PushLedgePillarOrClimbDown)},
    {MAP_EVENT_TALK, LOCALID_FALLEN_SOLDIER_NO_MATCH, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely)},
    {MAP_EVENT_TALK, LOCALID_FALLEN_STRONGMAN_ONLY_TWO, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseOnlyTwoOfThoseSixEver)},
    {MAP_EVENT_TALK, LOCALID_FALLEN_SOLDIER_KICKED_ASIDE, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseTheyJustKickedUsOutOf)},
    {MAP_EVENT_TALK, LOCALID_FALLEN_MAN_SCORCHED_GROUND, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseTheySetFireToEverythingThats)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FALLEN_SOLDIER_NO_MATCH, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseWereTheOnesWhoAttackedBut)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FALLEN_STRONGMAN_ONLY_TWO, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseTheOtherFourDidntSeemTo)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FALLEN_SOLDIER_KICKED_ASIDE, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseWhatCouldTheyBePlanningFor)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FALLEN_MAN_SCORCHED_GROUND, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseNoneOfThemHadAnyTorches)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_LEFT_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_JumpFromLedge)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_RIGHT_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_JumpFromLedge)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_PLANT, FLAG_GROWTH_PASSAGE_OPEN, EVENT_SCRIPT(BabiLighthouse_OpenGrowthPassage)},
    {MAP_EVENTS_END},
};

struct MapEvent gBabiLighthouse_LuffEvents[] = {
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_STEPS},
    {MAP_EVENT_WARP, 6, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_ENTRY_EAST},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_WEST_ROOM_STEPS},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_WEST_STEPS},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_EAST_STEPS},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_ENTRY_WEST},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_WEST_STEPS},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_EAST_STEPS},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_LOCKED_DOOR_ROOM},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_LOCKED_DOOR_ROOM_TO_SOUTH_ROOM},
    {MAP_EVENT_WARP, 11, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_STONE_DOOR_WEST},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_DropFromWestRoomEastPit)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_DropFromEastRoomPit)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_DropFromWestRoomSouthwestPit)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 28, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_DropFromWestRoomNorthwestPit)},
    {MAP_EVENT_TALK, LOCALID_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_StartDjinnBattle)},
    {BG_EVENT(BG_EVENT_SILENT), 20, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_InspectLockedDoor)},
    {MAP_EVENTS_END},
};

struct MapEvent gBabiLighthouse_StoneDoorEvents[] = {
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, BABI_LIGHTHOUSE_STONE_DOOR_TO_TUNNEL_RUINS_GREAT_STATUE},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, BABI_LIGHTHOUSE_STONE_DOOR_TO_BABI_LIGHTHOUSE_LUFF_WEST},
    {MAP_EVENT_COORD, 10, FLAG_BABI_LIGHTHOUSE_ASKED_TO_FOLLOW, EVENT_SCRIPT(BabiLighthouse_FaranAsksPartyToOpenDoor)},
    {MAP_EVENT_COORD, 20, IF_FLAG_SET(FLAG_BEYOND_DOOR), EVENT_SCRIPT(BabiLighthouse_CloseTruthDoor)},
    {MAP_EVENT_COORD, 21, FLAG_BEYOND_DOOR, EVENT_SCRIPT(BabiLighthouse_CloseTruthDoor)},
    {MAP_EVENT_COORD, 22, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_OpenTruthDoor)},
    {MAP_EVENT_TALK, LOCALID_IODEM, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIAmCountingOnYouIsaac)},
    {MAP_EVENT_TALK, LOCALID_WITNESS, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseWhenISawThemTheyHad)},
    {MAP_EVENT_TALK, LOCALID_DOUBTING_THE_DOOR, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIAmSorryButNotEven)},
    {MAP_EVENT_TALK, LOCALID_WORRIED_FOR_SHEBA, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseThisAllStartedWhenActorWas)},
    {MAP_EVENT_TALK, LOCALID_FARAN, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIfYouCanOpenThisDoor)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_IODEM, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIDoNotMeanToDoubt)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WITNESS, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN,
        MSGNO(MsgBabiLighthouseNobodyThinksTheDoorCanBe)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_DOUBTING_THE_DOOR, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN,
        MSGNO(MsgBabiLighthouseIDontKnowWhatTheyreWaiting)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WORRIED_FOR_SHEBA, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseThisIsAllAHugeWaste)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FARAN, FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN, MSGNO(MsgBabiLighthouseIHopeTheyCanOpenThe)},
    {MAP_EVENT_TALK, LOCALID_IODEM, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseIBegOfYouIsaacPlease)},
    {MAP_EVENT_TALK, LOCALID_WITNESS, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseICantBelieveThisDoorActually)},
    {MAP_EVENT_TALK, LOCALID_DOUBTING_THE_DOOR, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseItTakesGreatCourageToContinue)},
    {MAP_EVENT_TALK, LOCALID_WORRIED_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseImVeryInterestedInWhatLies)},
    {MAP_EVENT_TALK, LOCALID_FARAN, CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseIAmNotAfraidOfDanger)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_IODEM, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseIfAnythingHappensToActorTolbi)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WITNESS, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseIfThisDoorOpensMaybeWe)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_DOUBTING_THE_DOOR, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseIBetTheMonstersAroundHere)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WORRIED_FOR_SHEBA, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseThisIsABigWasteOf)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FARAN, CONDITION_ALWAYS,
        MSGNO(MsgBabiLighthouseIDontWantToHinderActors)},
    {BG_EVENT(BG_EVENT_SILENT), 60, CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_InspectTruthDoorSwitch)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(BabiLighthouse_ShowTruthDoorSwitch)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(BabiLighthouse_HideTruthDoorSwitch)},
    {MAP_EVENTS_END},
};
// clang-format on

struct MapEvent gBabiLighthouse_RoadEvents[] = {
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 50, CONDITION_ALWAYS,
     EVENT_SCRIPT(BabiLighthouse_PushRoadPillar)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 51, CONDITION_ALWAYS,
     EVENT_SCRIPT(BabiLighthouse_ClimbOrPushRoadPillar)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_ROAD_PILLAR, CONDITION_ALWAYS,
     EVENT_SCRIPT(BabiLighthouse_UpdateMovedPillar)},
    {PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST),
     LOCALID_ROAD_PILLAR, CONDITION_ALWAYS,
     EVENT_SCRIPT(BabiLighthouse_LandCarriedPillarInDust)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_CARRY), LOCALID_ROAD_PILLAR,
     CONDITION_ALWAYS, EVENT_SCRIPT(BabiLighthouse_UpdateMovedPillar)},
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, ROAD_TO_BABI_LIGHTHOUSE_TO_WORLD_MAP},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     ROAD_TO_BABI_LIGHTHOUSE_TO_BABI_LIGHTHOUSE_ENTRY},
    {MAP_EVENT_TALK, LOCALID_WORKER_DAY_OFF, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseWeHaventBeenOrderedToStart)},
    {MAP_EVENT_TALK, LOCALID_WORKER_STRANGE_FOLK, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseThereWereStrangeFolkGoingTo)},
    {MAP_EVENT_TALK, LOCALID_WORKER_ODD_PLACE, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseItsOddThatBabiWouldBuild)},
    {MAP_EVENT_TALK, LOCALID_WORKER_CUTTING_STONES, CONDITION_ALWAYS,
     MSGNO(MsgBabiLighthouseCuttingTheseStonesAndStackingThem)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WORKER_DAY_OFF,
     CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseThereWasABigFightThat)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WORKER_STRANGE_FOLK,
     CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseIfFaranWentAfterThemThen)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WORKER_ODD_PLACE,
     CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseWhyWouldTheyWantToRush)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WORKER_CUTTING_STONES,
     CONDITION_ALWAYS, MSGNO(MsgBabiLighthouseNowThatIThinkOfIt)},
    {BG_EVENT_CHEST_FACING(FACING_EAST), 100, FLAG_BABI_LIGHTHOUSE_LUCKY_PEPPER,
     FIND_ITEM(ITEM_LUCKY_PEPPER)},
    {MAP_EVENTS_END},
};
