/// @file
/// Babi Lighthouse, the half-built tower north of Lalivero, and the road up to
/// it. The kidnappers who took Sheba came this way, and the party follows them
/// through the lighthouse to a stone door, beyond which lie the Tunnel Ruins.
///
/// On the Road to Babi Lighthouse four workers have downed tools. A grey
/// carved pillar stands on a ledge east of the path, below the chest with the
/// Lucky Pepper. Moved west and lifted with Carry into the dip at cell (20,
/// 12), on the chest's ledge, it fills the dip and Isaac can walk across to the
/// chest.
///
/// At the Babi Lighthouse Entry two of Babi's soldiers and two burly men lie
/// trembling in the court, knocked down by the kidnappers. A grey pillar stands
/// at the lighthouse's foot on each side, beside a hole with a ladder down to
/// the Tunnel Ruins; the east one starts on top of its hole. Pushed or moved,
/// the pillars cover and uncover the holes. Growth on the sprout in the
/// north-east corner makes a vine up the lighthouse's side, the way to its
/// upper doorway.
///
/// Inside are four rooms. The upper doorway opens on the second floor, a room
/// round a deep pit with three slides down its sides. The lower doorway opens
/// on the ground floor, where Luff, a Jupiter Djinni, stands on a pillar in the
/// middle of the pit; the east slide from upstairs lands Isaac beside it. A
/// slide from Luff's pillar and stairs lead down to a basement room, and stairs
/// from there to a room with a locked door. The north-west slide upstairs drops
/// Isaac all the way through the floors into the cave before the stone door.
///
/// There Faran and three men wait, sure that no one can open the door the
/// kidnappers went through, and Iodem, who came with the party, steps out to
/// meet them. Faran asks the party to prove itself by opening it. Reveal shows
/// a switch beside the door, and searching it under Reveal's light opens the
/// door; without it, the party reads "The truth of this door is open to those
/// seeing with their true heart." The door slides shut behind the party
/// whichever way it goes through, and from the far side it opens as Isaac
/// walks up to it. The cave's south-west passage comes out
/// through the locked door downstairs, which stays open from then on.
///
/// The four places share this overlay and one exit table; the other tables are
/// chosen by gMainRoutineWork.next, with the road as the fallback. Retreat
/// anywhere but on the road leads back to the Entry.

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "scene_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_psynergy_work.h"
#include "field/pushed_actor.h"
#include "field/scene.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "scene/import.h"
#include "scene/walk_into_leader.h"
#include "sound/sound.h"
#include "system/heap_slots.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern struct SceneEntrance gBabiLighthouse_EntryEntrances[];
extern struct SceneEntrance gBabiLighthouse_LuffEntrances[];
extern struct SceneEntrance gBabiLighthouse_StoneDoorEntrances[];
extern struct SceneEntrance gBabiLighthouse_RoadEntrances[];
extern struct SceneExitRegion gBabiLighthouse_EntryRegions[];
extern u32 gBabiLighthouse_Exits[];
extern struct ActorPlacement gBabiLighthouse_EntryActorPlacements[];
extern struct ActorPlacement gBabiLighthouse_LuffActorPlacements[];
extern struct ActorPlacement gBabiLighthouse_StoneDoorActorPlacements[];
extern struct ActorPlacement gBabiLighthouse_RoadActorPlacements[];
extern struct Trigger gBabiLighthouse_EntryTriggers[];
extern struct Trigger gBabiLighthouse_LuffTriggers[];
extern struct Trigger gBabiLighthouse_StoneDoorTriggers[];
extern struct Trigger gBabiLighthouse_RoadTriggers[];

/// The cave before the stone door (BabiLighthouseStoneDoor): the door's two
/// halves, Iodem, the strongman who saw the kidnappers go through, a
/// grey-haired man, a young man and Faran.
enum {
  ACTOR_LEFT_DOOR = ACTOR_FIRST_PLACED,
  ACTOR_RIGHT_DOOR,
  ACTOR_IODEM,
  /// A strongman.
  ACTOR_WITNESS,
  /// A burly grey-haired man with his arms folded.
  ACTOR_DOUBTING_THE_DOOR,
  /// A young man with brown hair in a purple shirt.
  ACTOR_WORRIED_FOR_SHEBA,
  ACTOR_FARAN
};

/// Luff, on its pillar in the ground floor's pit (BabiLighthouseLuff).
enum { ACTOR_LUFF = ACTOR_FIRST_PLACED };

/// The Entry (BabiLighthouseEntry): the two pillars at the lighthouse's foot,
/// the sprout for Growth and the four the kidnappers knocked down on their way
/// in.
///
/// The four are two of Babi's soldiers, a strongman and a bald, muscular man.
/// They lie on the ground, each named by what he says.
///
/// The pillars have the sprite of the pillars Move or a push shifts in Venus
/// Lighthouse.
enum {
  ACTOR_LEFT_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_RIGHT_PILLAR,
  /// The sprout in the north-east corner, which Growth grows into a vine.
  ACTOR_SPROUT,
  /// A soldier lying on the ground.
  ACTOR_FALLEN_SOLDIER_NO_MATCH,
  /// A strongman in a red top lying on the ground.
  ACTOR_FALLEN_STRONGMAN_ONLY_TWO,
  /// A soldier lying on the ground.
  ACTOR_FALLEN_SOLDIER_KICKED_ASIDE,
  /// A bald, muscular man lying on the ground.
  ACTOR_FALLEN_MAN_SCORCHED_GROUND
};

/// The Road to Babi Lighthouse (RoadToBabiLighthouse): four workers and the
/// grey pillar for Move and Carry.
///
/// The workers are a bald, muscular man, a strongman and two men with pickaxes,
/// each named by what he says. The pillar has the sprite of the pillars Carry
/// takes across a gap in Venus Lighthouse.
enum {
  /// A bald, muscular man.
  ACTOR_WORKER_DAY_OFF = ACTOR_FIRST_PLACED,
  /// A strongman in a red top.
  ACTOR_WORKER_STRANGE_FOLK,
  /// A man with a pickaxe.
  ACTOR_WORKER_ODD_PLACE,
  /// A man with a pickaxe.
  ACTOR_WORKER_CUTTING_STONES,
  ACTOR_ROAD_PILLAR
};

/// SPRITE_SPIRAL_PILLAR's look on the road: a grey block carved with a spiral.
enum { SPIRAL_PILLAR_ANIMATION_ROAD = 2 };

/// The cells that start an event, by place. An exit's cell carries its exit's
/// number, except inside the lighthouse, where the first exits' cells are
/// numbered otherwise (gBabiLighthouse_LuffTriggers).
enum BabiLighthouseCellBehavior {
  /// The Entry: the cells round each grey pillar, from which Isaac pushes it.
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_BESIDE_PILLAR = 10,
  /// The Entry: the ladder holes down to the Tunnel Ruins and the cells north
  /// of them, (6, 19) to (6, 20) and (18, 19) to (18, 20).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_LADDER_HOLE = 11,
  /// The locked stone door in the room at the foot of the basement stairs,
  /// (45, 9).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_LOCKED_DOOR = 20,
  /// The second floor's east slide, (11, 10).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_EAST_SLIDE = 25,
  /// The slide below Luff's pillar, (29, 12).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_LUFF_SLIDE = 26,
  /// The second floor's south-west slide, (8, 12).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_SOUTHWEST_SLIDE = 27,
  /// The second floor's north-west slide, (9, 9).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_NORTHWEST_SLIDE = 28,
  /// The stone door's cave: across its mouth, (17, 23) to (20, 23).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_CAVE_MOUTH = 10,
  /// Just south of the stone door, (18, 16) and (19, 16).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_SOUTH_OF_DOOR = 20,
  /// Two rows north of the stone door, (18, 10) and (19, 10).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_PAST_DOOR = 21,
  /// Just north of the stone door, (18, 11) and (19, 11).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_NORTH_OF_DOOR = 22,
  /// The hidden switch beside the stone door, (17, 15).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_DOOR_SWITCH = 60,
  /// The road: the ledge where the grey pillar stands, (15, 13) to (25, 16).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_PILLAR_LEDGE = 50,
  /// The road: the ladders up to and on the ledge, (16, 12) to (16, 13), (22,
  /// 13) to (22, 14) and (24, 13) to (24, 14).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_LADDER = 51,
  /// The road: the chest with the Lucky Pepper, (23, 12).
  CELL_BEHAVIOR_BABI_LIGHTHOUSE_LUCKY_PEPPER_CHEST = 100,
};

enum {
  /// Growth has grown the Entry's sprout into a vine.
  FLAG_GROWTH_PASSAGE_OPEN = FLAG_AREA_TEMP_00,
  /// The party is on the far side of the stone door, towards the Tunnel Ruins.
  FLAG_BEYOND_DOOR = FLAG_AREA_TEMP_01
};

/// What Actor_Prepare places at the far edge of a slide's pit, so that Isaac
/// vanishes below the edge as he drops over it.
///
/// It is a crate drawn solid black (SPRITE_COLOR_BLACK) and translucent in
/// front of the actors, so it shows the background through him where they
/// overlap.
enum { PIT_MASK = SPRITE_CRATE };

/// Walking through the open stone door slides it shut behind the party: on the
/// far side at cells (18, 10) and (19, 10), coming back at (18, 16) and (19,
/// 16).
///
/// The first opening's scene closes it too, in Iodem's face as he steps up to
/// it.
static void BabiLighthouse_CloseTruthDoor(void);
static void BabiLighthouse_RedrawPillarCells(void);

#include "shared/staged_actor.c"

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/// Talking to Luff, the Jupiter Djinni on the pillar in the ground floor's pit,
/// starts a battle with it.
static void BabiLighthouse_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(ACTOR_LUFF, DJINNI_LUFF);
}

#include "shared/actor_wait_until_idle.c"
#include "shared/field_effect_advance.c"

/// Coming down a slide, Isaac drops in from the floor above, lands with a thud
/// that shakes the screen, throws up a ring of dust and is startled. It plays
/// on Luff's pillar, in the basement room and in the cave before the stone
/// door.
static void BabiLighthouse_LandWithParticles(void) {
  u32 particle;
  struct Actor *leader;
  struct EffectOptions options;
  s32 velocity[3];

  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Actor_Get(ACTOR_PARTY_LEADER)->y.fixed = toFIXED(130);
  Actor_Get(ACTOR_PARTY_LEADER)->gravity = toFIXED(1) / 2;
  Actor_Get(ACTOR_PARTY_LEADER)->bounce = 0;
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Sound_Play(SE_DUST_PUFF);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_Wait(24);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  options.palette = SPRITE_COLOR_WHITE;
  options.update = (void (*)(struct Actor *))FieldEffect_AdvanceMotion;
  options.startScaleX = 4 * FIXED_ONE / 5;
  options.startScaleY = 4 * FIXED_ONE / 5;
  for (particle = 0; particle <= TRIG_TURN / FACING_STEP; particle++) {
    SET_VECTOR(velocity, Trig_Cos(particle * FACING_STEP), 0,
               Trig_Sin(particle * FACING_STEP));
    velocity[0] += velocity[0] / 2;
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                      &options);
  }
  Sound_Play(SE_THUMP);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Actor_Get(ACTOR_PARTY_LEADER)->gravity = toFIXED(1);
  Actor_Get(ACTOR_PARTY_LEADER)->bounce = FIXED_ONE / 4;
  Event_End();
}

/// Coming down the north-west or south-west slide, Isaac drops through a room
/// without landing and falls on through the hole below, out through the given
/// exit.
static void BabiLighthouse_FallToExit(s32 exit) {
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Actor_Get(ACTOR_PARTY_LEADER)->y.fixed = toFIXED(130);
  Actor_Get(ACTOR_PARTY_LEADER)->gravity = toFIXED(1) / 4;
  Actor_Get(ACTOR_PARTY_LEADER)->bounce = 0;
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Sound_Play(SE_DUST_PUFF);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = -toFIXED(5);
  Actor_WaitUntilIdle(Actor_Get(ACTOR_PARTY_LEADER));
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Import_EventSetExitRequest(exit);
  Event_End();
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_BABI_LIGHTHOUSE_ENTRY) {
    return gBabiLighthouse_EntryEntrances;
  }
  if (sceneId == SCENE_BABI_LIGHTHOUSE_LUFF) {
    return gBabiLighthouse_LuffEntrances;
  }
  if (sceneId == SCENE_BABI_LIGHTHOUSE_STONE_DOOR) {
    return gBabiLighthouse_StoneDoorEntrances;
  }
  return gBabiLighthouse_RoadEntrances;
}

/// Only the Entry has regions: the three holes with ladders down to the Tunnel
/// Ruins, beside the two pillars and in the north-east corner. A leader below
/// the ground there has climbed down one.
const struct SceneExitRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_BABI_LIGHTHOUSE_ENTRY) {
    return gBabiLighthouse_EntryRegions;
  }
  return 0;
}

/// The four scenes share one exit table.
const u32 *Scene_GetExits(void) { return gBabiLighthouse_Exits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_BABI_LIGHTHOUSE_ENTRY) {
    return gBabiLighthouse_EntryActorPlacements;
  }
  if (sceneId == SCENE_BABI_LIGHTHOUSE_LUFF) {
    return gBabiLighthouse_LuffActorPlacements;
  }
  if (sceneId == SCENE_BABI_LIGHTHOUSE_STONE_DOOR) {
    return gBabiLighthouse_StoneDoorActorPlacements;
  }
  return gBabiLighthouse_RoadActorPlacements;
}

/// The holes with ladders down to the Tunnel Ruins at the Babi Lighthouse
/// Entry, beside the west pillar at cell (6, 20) and under the east one at (18,
/// 20), and the last column of the Entry's west half.
#define HOLE_WEST_COLUMN 6
#define HOLE_EAST_COLUMN 18
#define HOLE_ROW 20
#define ENTRY_WEST_HALF_LAST_COLUMN 12

/// Move on the west or east grey pillar at the lighthouse's foot slides it a
/// cell, pushing it the way Isaac faces. If it slides onto the ladder hole
/// where Isaac stands, cell (6, 20) or (18, 20), he starts ("!") and hops out
/// of its way, east or north. Each push of a pillar ends here too.
///
/// Last the pillars' cells are redrawn, so that the moved pillar blocks its
/// new cell and covers the hole it stands on.
static void BabiLighthouse_HopClearOfPillar(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
  if ((FIELD_CELL(leader->x.fixed) == HOLE_WEST_COLUMN ||
       (FIELD_CELL(leader->x.fixed)) == HOLE_EAST_COLUMN) &&
      (FIELD_CELL(leader->z.fixed)) == HOLE_ROW) {
    leader->targetX = ACTOR_NO_TARGET;
    leader->targetZ = ACTOR_NO_TARGET;
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    {
      u16 facing = leader->facing;
      // Facing roughly north or south, he steps aside to the east.
      if ((facing > FACING_NORTH_NORTHWEST &&
           facing <= FACING_NORTH_NORTHEAST) ||
          (facing > FACING_SOUTH_SOUTHEAST &&
           facing <= FACING_SOUTH_SOUTHWEST)) {
        Actor_SetDestinationOffsetById(ACTOR_PARTY_LEADER, 16, 0);
        Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
      } else {
        FieldActor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
        Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
      }
    }
  }
  BabiLighthouse_RedrawPillarCells();
  Event_End();
}

/// Walking into the west or east grey pillar at the lighthouse's foot pushes it
/// a cell. Pushed east from where it starts, the west pillar slides onto the
/// ladder hole at cell (6, 20) and covers it.
static void BabiLighthouse_PushEntryPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  BabiLighthouse_HopClearOfPillar();
  Event_End();
}

/// Walking south into the ladder hole beside the west pillar or under the east
/// one, from cell (6, 19) or (18, 19), climbs Isaac down the ladder into the
/// Tunnel Ruins. If the grey
/// pillar stands on the hole, he pushes it south instead.
static void BabiLighthouse_PushPillarOrClimbDownHole(void) {
  struct Actor *pillar = Actor_Get(ACTOR_RIGHT_PILLAR);

  if (FIELD_CELL(Actor_Get(ACTOR_PARTY_LEADER)->x.fixed) <=
      ENTRY_WEST_HALF_LAST_COLUMN) {
    pillar = Actor_Get(ACTOR_LEFT_PILLAR);
    if (FIELD_CELL(pillar->x.fixed) == HOLE_WEST_COLUMN) {
      if (FIELD_CELL(pillar->z.fixed) == HOLE_ROW) {
        BabiLighthouse_PushEntryPillar();
        return;
      }
    }
  } else {
    if (FIELD_CELL(pillar->x.fixed) == HOLE_EAST_COLUMN) {
      if (FIELD_CELL(pillar->z.fixed) == HOLE_ROW) {
        BabiLighthouse_PushEntryPillar();
        return;
      }
    }
  }
  Field_ClimbDown();
}

/// Isaac jumps down the slide ahead (FieldActor_JumpDownLedge), and the screen
/// closes behind him.
static void BabiLighthouse_JumpIntoPit(void) {
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

/// Walking south onto the east slide of the second floor's pit, cell (11, 10),
/// Isaac jumps down it and lands on the pillar beside Luff on the ground
/// floor.
static void BabiLighthouse_DropFromWestRoomEastPit(void) {
  Event_Start();
  BabiLighthouse_JumpIntoPit();
  Import_EventSetExitRequest(
      BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_EAST);
  Event_End();
}

/// Walking south onto the slide below Luff's pillar on the ground floor, cell
/// (29, 12), Isaac jumps down it and lands in the basement room.
static void BabiLighthouse_DropFromEastRoomPit(void) {
  Event_Start();
  Actor_Prepare(464, 0, 290, PIT_MASK);
  BabiLighthouse_JumpIntoPit();
  Import_EventSetExitRequest(BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_PIT);
  Event_End();
}

/// Walking south onto the south-west slide of the second floor's pit, cell (8,
/// 12), Isaac jumps down it, drops through the ground floor and lands in the
/// basement room.
///
/// The pit's far edge and the pillar beside it are staged in front of him so
/// that he vanishes below them.
static void BabiLighthouse_DropFromWestRoomSouthwestPit(void) {
  Event_Start();
  Actor_Prepare(143, 0, 290, PIT_MASK);
  Actor_Prepare(121, 0, 286, SPRITE_CAPPED_PILLAR);
  BabiLighthouse_JumpIntoPit();
  Import_EventSetExitRequest(
      BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_SOUTHWEST);
  Event_End();
}

/// Walking south onto the north-west slide of the second floor's pit, cell (9,
/// 9), Isaac jumps down it and drops through the ground floor and the basement
/// into the cave before the stone door.
static void BabiLighthouse_DropFromWestRoomNorthwestPit(void) {
  Event_Start();
  BabiLighthouse_JumpIntoPit();
  Import_EventSetExitRequest(
      BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_PIT_NORTHWEST);
  Event_End();
}

/// Searching the stone door in the room at the foot of the basement stairs,
/// cell (45, 9): "It seems to be locked." It opens only from the cave beyond.
static void BabiLighthouse_InspectLockedDoor(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MsgBabiSeemsLocked, 1);
  Event_End();
}

/// The first time the party walks north into the cave before the stone door,
/// across cells (17, 23) to (20, 23), Iodem steps out beside Isaac to meet
/// Faran and three men, who say the door has never opened. Faran
/// doubts the party, and asks it to prove its power by opening the door that
/// the kidnappers went through.
static void BabiLighthouse_FaranAsksPartyToOpenDoor(void) {
  GameFlag_Set(FLAG_BABI_LIGHTHOUSE_ASKED_TO_FOLLOW);
  GameFlag_Set(FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED);
  Event_Start();
  Field_EndBusyAndReveal();
  Event_SetMessageId(MsgBabiYoureSureTheyWentThrough);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 296, 352);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_Wait(10);
  FieldActor_LaunchFromLeader(ACTOR_IODEM, 16, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_GARET, -8, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_IVAN, 8, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_MIA, 24, 16, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_MIA);
  Event_Wait(20);
  // "You're sure they went through this door?"
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_Wait(10);
  FieldCamera_SetSpeed(3);
  FieldCamera_MoveTo(280, CAMERA_KEEP, 288, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_WITNESS, ANIM_NOD);
  Event_Wait(30);
  // "Yes, they opened the stone door and entered!"
  Event_ShowMessage(ACTOR_WITNESS, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_Wait(20);
  // "Well? Does the door open?"
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_Wait(10);
  // The witnesses try the stone door and disagree about who passed through it.
  Actor_StartRepeatedMotion(ACTOR_WORRIED_FOR_SHEBA, 2);
  Actor_RunRepeatedMotion(ACTOR_DOUBTING_THE_DOOR, 2);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_WORRIED_FOR_SHEBA, 2);
  Actor_RunRepeatedMotion(ACTOR_DOUBTING_THE_DOOR, 2);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_WORRIED_FOR_SHEBA, 2);
  Actor_RunRepeatedMotion(ACTOR_DOUBTING_THE_DOOR, 2);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  // "It's hopeless. It won't budge..."
  Event_ShowMessage(ACTOR_DOUBTING_THE_DOOR, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_EAST, 0);
  Event_Wait(25);
  Actor_RunRepeatedMotion(ACTOR_WORRIED_FOR_SHEBA, 2);
  Event_Wait(20);
  // "Of course not! I told you, this door doesn't open!"
  Event_ShowMessage(ACTOR_WORRIED_FOR_SHEBA, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_WORRIED_FOR_SHEBA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "If it did open, we would have gone in long ago to study the..."
  Event_ShowMessage(ACTOR_WORRIED_FOR_SHEBA, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_WITNESS, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "But I'm certain they went through this door!"
  Event_ShowMessage(ACTOR_WITNESS, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_WORRIED_FOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_SOUTH, 0);
  Event_Wait(20);
  // "And I'm saying no one has ever gone through here..."
  Event_ShowMessage(ACTOR_WORRIED_FOR_SHEBA, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_WORRIED_FOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 75);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Event_Wait(30);
  // "Oh! Iodem..."
  Event_ShowMessage(ACTOR_FARAN, 0);
  FieldCamera_MoveTo(280, CAMERA_KEEP, 320, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  // The party learns where the kidnappers went and is asked to pursue them.
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  // "Faran!"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I am so sorry about Sheba..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  // "Why did this sort of thing have to happen?"
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Are Felix and the others beyond this door?"
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "Yes. Beyond the door that no normal man can open..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  // "If they got through, then we can, too. Right?"
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(30);
    // "Will you go for us?"
    Event_ShowMessage(ACTOR_IODEM, 0);
    gFieldWork->message++;
  } else {
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    gFieldWork->message++;
    // "There is no one else who can go after them."
    Event_ShowMessage(ACTOR_IODEM, 0);
  }
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetSpeed(ACTOR_FARAN, 1);
  Actor_WalkByAndWait(ACTOR_FARAN, 0, 16);
  Event_Wait(20);
  // "Iodem, just who are these people?"
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
  Event_Wait(35);
  // "They come from the far north. They have chased the ones who..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_Wait(20);
  // "They've been chasing that fearsome group?"
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Yes. They are most fearsome."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "Such awful power... And what they're trying to do seems very..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  // "We're the only ones who can stop them."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "You think you can stop them?"
  Event_OpenMessage(ACTOR_FARAN, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "You seem quite sure of yourself, but what do you think you..."
    Event_ShowMessage(ACTOR_FARAN, 0);
    gFieldWork->message++;
  } else {
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    gFieldWork->message++;
    // "Hmph! Not with that attitude, you can't!"
    Event_ShowMessage(ACTOR_FARAN, 0);
  }
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  // "I have never heard such harsh words from you, Faran."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_WORRIED_FOR_SHEBA, 2);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_WORRIED_FOR_SHEBA, 1.3);
  Actor_WalkByAndWait(ACTOR_WORRIED_FOR_SHEBA, 0, 16);
  Event_Wait(20);
  // "What do you expect when, in the midst of such dangers... you..."
  Event_ShowMessage(ACTOR_WORRIED_FOR_SHEBA, 0);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_DOUBTING_THE_DOOR, 1.3);
  Actor_WalkByAndWait(ACTOR_DOUBTING_THE_DOOR, 0, 16);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 50);
  // "If you want me to trust them, they must give me reason to..."
  Event_ShowMessage(ACTOR_DOUBTING_THE_DOOR, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Event_Wait(25);
  // "First, you will have to demonstrate your powers to us."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  // "Can you follow the same paths that they took?"
  Event_OpenMessage(ACTOR_IODEM, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(30);
    // "Your confidence means nothing unless you show that you can..."
    Event_ShowMessage(ACTOR_IODEM, 0);
    gFieldWork->message++;
  } else {
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    gFieldWork->message++;
    // "Well, you have no choice... You will have to try."
    Event_ShowMessage(ACTOR_IODEM, 0);
  }
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
  Event_Wait(35);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(40);
  // "Iodem asks only that you stand back and watch."
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_DOUBTING_THE_DOOR, ACTOR_WORRIED_FOR_SHEBA, 50);
  Actor_SetAnimation(ACTOR_DOUBTING_THE_DOOR, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_WORRIED_FOR_SHEBA, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_DOUBTING_THE_DOOR, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_WORRIED_FOR_SHEBA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_DOUBTING_THE_DOOR, 1);
  Actor_SetSpeed(ACTOR_WORRIED_FOR_SHEBA, 1);
  Actor_WalkBy(ACTOR_DOUBTING_THE_DOOR, 32, 0);
  Actor_WalkByAndWait(ACTOR_WORRIED_FOR_SHEBA, 32, 0);
  Actor_WalkBy(ACTOR_DOUBTING_THE_DOOR, 0, 16);
  Actor_WalkByAndWait(ACTOR_WORRIED_FOR_SHEBA, 16, 0);
  Actor_WalkTo(ACTOR_WORRIED_FOR_SHEBA, 344, 312);
  Actor_WalkToAndWait(ACTOR_DOUBTING_THE_DOOR, 344, 336);
  Actor_SetAnimation(ACTOR_WORRIED_FOR_SHEBA, ANIM_STAND);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_WEST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Event_Wait(20);
  // "Now, show me what you can do."
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_FARAN, 1);
  Actor_WalkToAndWait(ACTOR_FARAN, 328, 312);
  Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_WITNESS, 1);
  Actor_WalkToAndWait(ACTOR_WITNESS, 328, 328);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
  Event_Wait(20);
  // The companions walk back into the leader.
  Actor_SetSpeed(ACTOR_GARET, 1.2);
  Actor_SetSpeed(ACTOR_IVAN, 1.2);
  Actor_SetSpeed(ACTOR_MIA, 1.2);
  Scene_WalkIntoLeader(ACTOR_GARET);
  Scene_WalkIntoLeader(ACTOR_IVAN);
  Scene_WalkIntoLeader(ACTOR_MIA);
  Event_Wait(10);
  Event_End();
}

/// The first time the stone door opens, Iodem, Faran and the men gather before
/// it in wonder. As Iodem steps up to follow, the door slides shut in his face:
/// it will not let those without Psynergy through. Faran and Iodem agree to
/// stay behind and let the party go on alone.
static void BabiLighthouse_ExplainTruthDoor(void) {
  // Everyone reacts to the opened door before trying to follow the party.
  GameFlag_Set(FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN);
  Event_Start();
  Field_EndBusyAndReveal();
  Event_SetMessageId(MsgBabiTheDoor);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 296, 312);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Event_Wait(10);
  Actor_LaunchFromLeader(ACTOR_GARET, 0, 16, FACING_EAST);
  FieldActor_LaunchFromLeader(ACTOR_IVAN, -16, -8, FACING_EAST);
  Actor_LaunchFromLeader(ACTOR_MIA, -16, 24, FACING_EAST);
  Actor_WaitForMoveById(ACTOR_MIA);
  Event_Wait(20);
  FieldCamera_SetSpeed(3);
  FieldCamera_MoveTo(280, CAMERA_KEEP, 328, 1);
  FieldCamera_WaitForMove();
  Event_Wait(10);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "The door!"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Actor_Jump(ACTOR_IODEM, 4, 13);
  Actor_Jump(ACTOR_IODEM, 4, 30);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_WORRIED_FOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_Wait(20);
  // "I don't believe it! The door opened!"
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_WORRIED_FOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Incredible! It really does open."
  Event_ShowMessage(ACTOR_WORRIED_FOR_SHEBA, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_WEST, 0);
  Event_Wait(25);
  // "How in the world did you open the door?"
  Event_ShowMessage(ACTOR_DOUBTING_THE_DOOR, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_WEST, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  // "I don't know how to explain it..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "We did it because we're us!"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "It would take too long to explain."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "There's no time for an explanation, anyway."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_Wait(25);
  // "That's right. We must rescue Sheba!"
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_IODEM, 1);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
  Event_Wait(20);
  // "Let's go, Faran. We've got to save Sheba!"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_DOUBTING_THE_DOOR, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_WORRIED_FOR_SHEBA, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  FieldCamera_FollowActor(ACTOR_IODEM, 1);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, -32);
  // Demonstrate that the door closes again when someone without Psynergy
  // enters.
  BabiLighthouse_CloseTruthDoor();
  GameFlag_Clear(FLAG_BEYOND_DOOR);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_Start();
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Hey, the door's closing!"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_WORRIED_FOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  FieldCamera_MoveTo(280, CAMERA_KEEP, 312, 1);
  FieldCamera_WaitForMove();
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_DOUBTING_THE_DOOR, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Why did it close?"
  Event_ShowMessage(ACTOR_DOUBTING_THE_DOOR, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_WORRIED_FOR_SHEBA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I don't know... Why does anything close?"
  Event_ShowMessage(ACTOR_WORRIED_FOR_SHEBA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  // "It closed to prevent people with no Psynergy from entering!"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  // "It would be perilous for those without Psynergy to try to get through."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I agree! You'll have to let us handle it from here!"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
  Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
  Event_Wait(20);
  // "What? You're telling us to stay back?"
  Event_OpenMessage(ACTOR_FARAN, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
    Event_Wait(20);
    // "Since we can't even open the door, we'd just be getting in your way."
    Event_ShowMessage(ACTOR_FARAN, 0);
    gFieldWork->message++;
  } else {
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
    Event_Wait(20);
    gFieldWork->message++;
    // "It won't stop you, but if we can't open the door, we can't go with you."
    Event_ShowMessage(ACTOR_FARAN, 0);
  }
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_IODEM, 0, 16);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  // "This is true..."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "But we can't leave Sheba in their hands just because it's dangerous."
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_Wait(10);
  // Iodem and Faran agree to stay behind and leave the rescue to the party.
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  // "They have greater power than you can even imagine."
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_Wait(20);
  // "And without such power, this door will not open for us?"
  Event_OpenMessage(ACTOR_FARAN, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(30);
    // "That is how it seems."
    Event_ShowMessage(ACTOR_IODEM, 0);
    gFieldWork->message++;
  } else {
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    gFieldWork->message++;
    // "The power they just displayed is but a fraction of their true might."
    Event_ShowMessage(ACTOR_IODEM, 0);
  }
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  // "Without that sort of power, we would just get in their way."
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_Wait(15);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
  Event_Wait(20);
  // "I understand. Sheba's life is in your hands."
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Event_Wait(30);
  // "Do not fail us. You must bring Sheba safely back!"
  Event_ShowMessage(ACTOR_FARAN, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(25);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_IODEM, 312, 312);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Event_Wait(25);
  // "Make haste, Isaac. All our hopes lie with you!"
  Event_ShowMessage(ACTOR_IODEM, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  // The companions walk back into the leader.
  Actor_SetSpeed(ACTOR_GARET, 1.2);
  Actor_SetSpeed(ACTOR_IVAN, 1.2);
  Actor_SetSpeed(ACTOR_MIA, 1.2);
  Scene_WalkIntoLeader(ACTOR_GARET);
  Scene_WalkIntoLeader(ACTOR_IVAN);
  Scene_WalkIntoLeader(ACTOR_MIA);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FARAN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  Event_End();
}

/// Reveal in the cave shows a switch on the wall beside the stone door, cell
/// (17, 15), flipped one way or the other as the door stands.
static void BabiLighthouse_ShowTruthDoorSwitch(void) {
  if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) == 0) {
    MapGrid_CopyCellsLayered(36, 78, 1, 2, 17, 78);
  } else {
    MapGrid_CopyCellsLayered(34, 78, 1, 2, 17, 78);
  }
}

/// As Reveal's light fades in the cave, the switch beside the stone door is
/// hidden again.
static void BabiLighthouse_HideTruthDoorSwitch(void) {
  MapGrid_CopyCellsLayered(32, 78, 1, 2, 17, 78);
}

/// The stone door's two halves slide apart with a grinding of stone. It runs
/// when the switch is flipped, and when Isaac walks up to the closed door from
/// the far side, cells (18, 11) and (19, 11).
static void BabiLighthouse_OpenTruthDoor(void) {
  if (Event_GetFlag(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) == 0) {
    GameFlag_Set(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN);
    Sound_Play(SE_GATE_MOVE);
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

static void BabiLighthouse_CloseTruthDoor(void) {
  if (Event_GetFlag(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) != 0) {
    GameFlag_Clear(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN);
    Sound_Play(SE_GATE_MOVE);
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

/// Searching the switch beside the stone door, cell (17, 15), under Reveal's
/// light: "flipped the switch", and the door opens. Without Reveal, the wall
/// there reads "The truth of this door is open to those seeing with their true
/// heart."
static void BabiLighthouse_InspectTruthDoorSwitch(void) {
  struct FieldWork *work = gFieldWork;

  Event_Start();
  if (work->revealActive != 0) {
    if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) == 0) {
      FieldMessage_Show(MsgFieldFlippedSwitch, 1);
      Sound_Play(SE_SWITCH);
      MapGrid_CopyCellsLayered(35, 78, 1, 2, 17, 78);
      Event_Wait(10);
      MapGrid_CopyCellsLayered(34, 78, 1, 2, 17, 78);
      Event_Wait(10);
      BabiLighthouse_OpenTruthDoor();
    }
  } else {
    Event_SetMessageId(MsgBabiTruthDoorOpenThoseSeeing);
    // "The truth of this door is open to those seeing with their..."
    Event_ShowMessage(-1, 0);
  }
  Event_End();
}

/// The cell one direction-table step ahead of the actor, at its ground height.
static inline void BabiLighthouse_FindCellAhead(struct FieldPosition *ahead,
                                                struct Actor *actor) {
  s32 step = gPushedActor_DirectionSteps[FACING_INDEX(actor->facing)];

  ahead->x = actor->x.fixed + DIRECTION_STEP_X_FIXED(step);
  ahead->z = actor->z.fixed + DIRECTION_STEP_Z_FIXED(step);
  ahead->y = MapGrid_GetTerrainHeight(actor->layer, ahead->x, ahead->z);
}

/// Finds the pushed actor one direction-table step ahead, including
/// ground height.
static struct Actor *BabiLighthouse_FindActorAhead(struct Actor *actor) {
  struct FieldPosition ahead;

  BabiLighthouse_FindCellAhead(&ahead, actor);
  return PushedActor_FindAtTile(&ahead, actor);
}

/// When Carry is cast on the grey pillar on the Road to Babi Lighthouse, the
/// pillar lands in a ring of puffs of dust where it is set down.
///
/// It only asks Carry (through childOption) to wait for the dropped pillar to
/// land and throw the puffs.
static void BabiLighthouse_LandCarriedPillarInPuffs(void) {
  ((struct FieldEffectWork *)gFieldEffectWork)->childOption = 1;
}

/// After Move or Carry on the grey pillar on the Road to Babi Lighthouse, a
/// pillar set down in the dip at cell (20, 12), on the chest's ledge, settles
/// into it level with the ledge, and from then on Isaac can walk over it.
static void BabiLighthouse_UpdateMovedPillar(void) {
  struct Actor *pillar = Actor_Get(ACTOR_ROAD_PILLAR);
  s32 column = FIELD_CELL(pillar->x.fixed);

  if (column == 20) {
    s32 row = FIELD_CELL(pillar->z.fixed);

    if (row == 12) {
      pillar->motionFlags = ACTOR_FALLS;
      pillar->groundHeight = toFIXED(48);
      pillar->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      {
        s32 targetColumn = column, targetRow = row;

        MapGrid_CopyCellAttributes(38, 12, 1, 1, targetColumn, targetRow);
      }
    }
  }
}

/// Walking into the grey pillar on the Road to Babi Lighthouse pushes it a
/// cell, but only when Isaac stands on the same level as it.
static void BabiLighthouse_PushRoadPillar(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  struct Actor *ahead = BabiLighthouse_FindActorAhead(leader);
  FIXED heightDifference;

  if (ahead == NULL) {
    return;
  }

  heightDifference = ahead->y.fixed - leader->y.fixed;

  // Either way up, 8 pixels or more apart is too far to push.
  if (heightDifference >= 0 ? heightDifference >= toFIXED(8)
                            : leader->y.fixed - ahead->y.fixed >= toFIXED(8))
    return;

  PushedActor_AdvancePair();
}

/// On the ledge of the Road to Babi Lighthouse, walking north or south on a
/// ladder climbs it, and walking into the grey pillar from a ladder's cell
/// pushes it.
static void BabiLighthouse_ClimbOrPushRoadPillar(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  struct Actor *ahead = BabiLighthouse_FindActorAhead(leader);
  s32 climbDirection = FACING_NEAREST_CARDINAL(leader->facing);
  s32 climbResult = -1;

  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_CLIMB || ahead == NULL) {
    if (climbDirection == FACING_NORTH) {
      climbResult = Field_ClimbUp();
    }
    if (climbDirection == FACING_SOUTH) {
      climbResult = Field_ClimbDown();
    }
  }
  if (climbResult != 0) {
    if (gMainRoutineWork.movementMode != LEADER_MOVEMENT_CLIMB) {
      BabiLighthouse_PushRoadPillar();
    }
  }
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_BABI_LIGHTHOUSE_ENTRY) {
    return gBabiLighthouse_EntryTriggers;
  }
  if (sceneId == SCENE_BABI_LIGHTHOUSE_LUFF) {
    return gBabiLighthouse_LuffTriggers;
  }
  if (sceneId == SCENE_BABI_LIGHTHOUSE_STONE_DOOR) {
    return gBabiLighthouse_StoneDoorTriggers;
  }
  return gBabiLighthouse_RoadTriggers;
}

/// Growth on the sprout in the north-east corner of the Babi Lighthouse Entry,
/// cell (18, 8), grows it into a vine Isaac can climb.
static void BabiLighthouse_OpenGrowthPassage(void) {
  MapGrid_CopyCellAttributes(82, 7, 1, 2, 18, 7);
  wait(1);
  GameFlag_Set(FLAG_GROWTH_PASSAGE_OPEN);
}

/// Redraws the cells round the Entry's two pillars and their ladder holes,
/// then blocks the cells under the pillars.
static void BabiLighthouse_RedrawPillarCells(void) {
  struct Actor *leftPillar = Actor_Get(ACTOR_LEFT_PILLAR);

  Actor_SetSpritePriority(ACTOR_LEFT_PILLAR, 1);
  Actor_SetSpritePriority(ACTOR_RIGHT_PILLAR, 1);
  MapGrid_CopyCellAttributes(69, 19, 3, 3, 5, 19);
  MapGrid_CopyCellAttributes(69, 19, 3, 3, 17, 19);
  {
    s32 column = FIELD_CELL(leftPillar->x.fixed),
        row = FIELD_CELL(leftPillar->z.fixed);

    MapGrid_CopyCellAttributes(3, 3, 1, 1, column, row);
  }
  {
    struct Actor *rightPillar = Actor_Get(ACTOR_RIGHT_PILLAR);
    s32 column = FIELD_CELL(rightPillar->x.fixed),
        row = FIELD_CELL(rightPillar->z.fixed);

    MapGrid_CopyCellAttributes(3, 3, 1, 1, column, row);
  }
}

#if TBS_PATCH_2
/// The Entry's pillars' update routine, from the second patch: a pillar north
/// of Isaac is drawn at his own sprite priority, and one level in front of him
/// otherwise, so that he passes behind a pillar south of him.
static void BabiLighthouse_MatchLeaderPriority(struct Actor *updated) {
  struct Sprite *sprite;
  struct Actor *actor;
  s32 priority;

  sprite = Actor_Get(ACTOR_PARTY_LEADER)->sprite;
  if (Actor_Get(ACTOR_LEFT_PILLAR)->z.fixed <
      Actor_Get(ACTOR_PARTY_LEADER)->z.fixed) {
    Actor_SetSpritePriority(ACTOR_LEFT_PILLAR, sprite->image.priority);
  } else {
    priority = sprite->image.priority;
    if (priority != 1)
      priority--;
    Actor_SetSpritePriority(ACTOR_LEFT_PILLAR, priority);
    Actor_Get(ACTOR_LEFT_PILLAR)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  }
  if (Actor_Get(ACTOR_RIGHT_PILLAR)->z.fixed <
      Actor_Get(ACTOR_PARTY_LEADER)->z.fixed) {
    Actor_SetSpritePriority(ACTOR_RIGHT_PILLAR, sprite->image.priority);
  } else {
    priority = sprite->image.priority;
    if (priority != 1)
      priority--;
    Actor_SetSpritePriority(ACTOR_RIGHT_PILLAR, priority);
    Actor_Get(ACTOR_RIGHT_PILLAR)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  }
}
#endif

/// Sets up the scene being entered: on the road, the pillar where it was left;
/// at the Entry, the pillars, the vine and the four lying trembling; inside,
/// Isaac landing or falling on through as the slide he took sends him; and
/// before the stone door, its halves and those waiting there as far as the
/// story has gone. Returns 0.
s32 Scene_Initialize(void) {
  struct Actor *placedActor;
  s32 sceneId;
  s32 lighthouseSceneId;
  s32 x;
  s32 z;

  wait(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  sceneId = gMainRoutineWork.next;
  if (sceneId != SCENE_ROAD_TO_BABI_LIGHTHOUSE) {
    gMainRoutineWork.retreatEntrance =
        BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE;
    gMainRoutineWork.retreatScene = SCENE_BABI_LIGHTHOUSE_ENTRY;
    lighthouseSceneId = gMainRoutineWork.next;
  }
  // The road: the moved pillar's cell behavior where it stands.
  if (sceneId == SCENE_ROAD_TO_BABI_LIGHTHOUSE) {
    placedActor = Actor_Get(ACTOR_ROAD_PILLAR);
    x = FIELD_CELL(placedActor->x.fixed);
    if (x == 20) {
      z = FIELD_CELL(placedActor->z.fixed);
      if (z == 12) {
        MapGrid_CopyCellAttributes(38, 12, 1, 1, x, z);
      }
    }
  } else if (lighthouseSceneId == SCENE_BABI_LIGHTHOUSE_ENTRY) {
    Actor_SetSpriteColorById(ACTOR_LEFT_PILLAR, SPRITE_COLOR_GREY);
    Actor_SetSpriteColorById(ACTOR_RIGHT_PILLAR, SPRITE_COLOR_GREY);
#if TBS_PATCH_2
    // Through union ActorOrEffect: as struct Actor the stores move.
    ((union ActorOrEffect *)Actor_Get(ACTOR_LEFT_PILLAR))->actor.update =
        BabiLighthouse_MatchLeaderPriority;
    ((union ActorOrEffect *)Actor_Get(ACTOR_RIGHT_PILLAR))->actor.update =
        BabiLighthouse_MatchLeaderPriority;
#endif
    if (gMainRoutineWork.entrance ==
            BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTH &&
        GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Actor_SetPosition(ACTOR_RIGHT_PILLAR, 312, 328);
    }
    BabiLighthouse_RedrawPillarCells();
    Actor_Get(ACTOR_RIGHT_PILLAR)->y.fixed =
        Actor_Get(ACTOR_RIGHT_PILLAR)->groundHeight;
    if (GameFlag_Get(FLAG_GROWTH_PASSAGE_OPEN) != 0) {
      Actor_SetAnimation(ACTOR_SPROUT, SPROUT_ANIMATION_GROWN);
      Actor_Get(ACTOR_SPROUT)->collisionFlags = (u8)~ACTOR_SOLID;
      BabiLighthouse_OpenGrowthPassage();
    }
    // Their script waits stagger the first animations of the four lying on the
    // ground.
    placedActor = Actor_Get(ACTOR_FALLEN_SOLDIER_NO_MATCH);
    placedActor->collisionFlags = 0;
    placedActor->priorityFlags = 0;
    placedActor->wait = 0;
    placedActor->sprite->image.priority = 3;
    placedActor->sprite->flags = 0;
    placedActor->sprite->rotation = TRIG_TURN * 3 / 4;
    Actor_SetAnimation(ACTOR_FALLEN_SOLDIER_NO_MATCH, ANIM_STILL);
    placedActor = Actor_Get(ACTOR_FALLEN_STRONGMAN_ONLY_TWO);
    placedActor->collisionFlags = 0;
    placedActor->priorityFlags = 0;
    placedActor->wait = 30;
    placedActor->sprite->image.priority = 3;
    placedActor->sprite->flags = 0;
    placedActor->sprite->rotation = DEGtoANG(90);
    Actor_SetAnimation(ACTOR_FALLEN_STRONGMAN_ONLY_TWO, ANIM_STILL);
    placedActor = Actor_Get(ACTOR_FALLEN_SOLDIER_KICKED_ASIDE);
    placedActor->collisionFlags = 0;
    placedActor->priorityFlags = 0;
    placedActor->wait = 60;
    placedActor->sprite->image.priority = 3;
    placedActor->sprite->flags = 0;
    placedActor->sprite->rotation = DEGtoANG(180);
    Actor_SetAnimation(ACTOR_FALLEN_SOLDIER_KICKED_ASIDE, ANIM_STILL);
    placedActor = Actor_Get(ACTOR_FALLEN_MAN_SCORCHED_GROUND);
    placedActor->collisionFlags = 0;
    placedActor->priorityFlags = 0;
    placedActor->wait = 90;
    placedActor->sprite->image.priority = 3;
    placedActor->sprite->flags = 0;
    placedActor->sprite->rotation = DEGtoANG(180);
    Actor_SetAnimation(ACTOR_FALLEN_MAN_SCORCHED_GROUND, ANIM_STILL);
  } else if (lighthouseSceneId == SCENE_BABI_LIGHTHOUSE_LUFF) {
    // Isaac comes in as the way he took drops him: landing from a slide,
    // or falling on through this floor to the one below.
    switch (gMainRoutineWork.entrance) {
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
      Actor_Prepare(440, 0, 290, PIT_MASK);
      MapGrid_CopyCellAttributes(22, 13, 1, 1, 27, 13);
      BabiLighthouse_FallToExit(
          BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_FALL_SOUTH);
      break;
    case BABI_LIGHTHOUSE_LUFF_EAST_ROOM_FROM_WEST_ROOM_PIT_NORTHWEST:
      Actor_Prepare(448, 0, 290, PIT_MASK);
      MapGrid_CopyCellAttributes(22, 12, 1, 1, 28, 10);
      BabiLighthouse_FallToExit(
          BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_FALL_NORTH);
      break;
    case BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_FROM_EAST_ROOM_FALL_NORTH:
      Actor_Prepare(232, 0, 594, PIT_MASK);
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
  } else if (sceneId == SCENE_BABI_LIGHTHOUSE_STONE_DOOR) {
    Actor_Get(ACTOR_LEFT_DOOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_Get(ACTOR_RIGHT_DOOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_SetSpeed(ACTOR_LEFT_DOOR, 1);
    Actor_SetSpeed(ACTOR_RIGHT_DOOR, 1);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      if (gMainRoutineWork.entrance ==
          BABI_LIGHTHOUSE_STONE_DOOR_FROM_TUNNEL_RUINS_GREAT_STATUE) {
        GameFlag_Set(FLAG_BEYOND_DOOR);
      } else {
        GameFlag_Clear(FLAG_BEYOND_DOOR);
      }
    }
    if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_ASKED_TO_FOLLOW) == 0) {
      Actor_SetPosition(ACTOR_IODEM, -64, -64);
      Actor_SetPosition(ACTOR_WITNESS, 280, 296);
      Actor_SetPosition(ACTOR_DOUBTING_THE_DOOR, 312, 248);
      Actor_SetPosition(ACTOR_WORRIED_FOR_SHEBA, 296, 248);
      Actor_SetPosition(ACTOR_FARAN, 320, 296);
      Actor_FaceDirection(ACTOR_WITNESS, FACING_EAST, 0);
      Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_NORTH, 0);
      Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_NORTH, 0);
      Actor_FaceDirection(ACTOR_FARAN, FACING_WEST, 0);
      Event_Wait(5);
    } else if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN) != 0) {
      Actor_SetPosition(ACTOR_IODEM, 312, 312);
      Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHWEST, 0);
      Actor_FaceDirection(ACTOR_WITNESS, FACING_NORTH_NORTHWEST, 0);
      Actor_FaceDirection(ACTOR_DOUBTING_THE_DOOR, FACING_NORTH_NORTHWEST, 0);
      Actor_FaceDirection(ACTOR_WORRIED_FOR_SHEBA, FACING_NORTH_NORTHWEST, 0);
      Actor_FaceDirection(ACTOR_FARAN, FACING_NORTH_NORTHWEST, 0);
      Event_Wait(5);
    }
    if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_DOOR_OPEN) != 0) {
      Actor_SetPosition(ACTOR_LEFT_DOOR, 280, 240);
      Actor_SetPosition(ACTOR_RIGHT_DOOR, 328, 240);
      Actor_FaceDirection(ACTOR_LEFT_DOOR, FACING_WEST, 0);
      Actor_FaceDirection(ACTOR_RIGHT_DOOR, FACING_EAST, 0);
      MapGrid_CopyCellAttributes(81, 14, 4, 1, 17, 14);
    }
    if (gMainRoutineWork.entrance ==
        BABI_LIGHTHOUSE_STONE_DOOR_FROM_BABI_LIGHTHOUSE_LUFF_EAST) {
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
        BabiLighthouse_LandWithParticles();
      }
    }
  } else {
    Actor_SetAnimation(ACTOR_ROAD_PILLAR, SPIRAL_PILLAR_ANIMATION_ROAD);
  }
  return 0;
}

#include "shared/pushed_actor_tables.c"

#include "shared/field_effect_duration_scripts.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The four the kidnappers knocked down tremble where they lie: two quick
/// twitches, narrower and taller and back, then two seconds still.
s32 gBabiLighthouse_TrembleScript[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
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

/// Unused: an empty actor placement table nothing returns.
struct ActorPlacement gBabiLighthouse_NoActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gBabiLighthouse_EntryActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION, 88, 0, 328, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION, 296, 0, 328, FACING_SOUTH,
                    TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296,
                    0, 130, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS,
                    (s32)gBabiLighthouse_TrembleScript, 176, 0, 360,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS,
                    (s32)gBabiLighthouse_TrembleScript, 240, 0, 240,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS,
                    (s32)gBabiLighthouse_TrembleScript, 216, 0, 280,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS,
                    (s32)gBabiLighthouse_TrembleScript, 184, 0, 248,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gBabiLighthouse_LuffActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_LUFF),
                    MOVEMENT_TYPE_STAND, 472, 0, 184, FACING_SOUTHEAST,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorPlacement gBabiLighthouse_StoneDoorActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -64, 0, -64, FACING_EAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -64, 0, -64, FACING_EAST, TALK_FACING_PARTY,
        0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -64, 0, -64, FACING_EAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, 296, 0, 240, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, 312, 0, 240, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 352, FACING_NORTH,
        TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 328, FACING_WEST,
        TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 336, FACING_WEST,
        TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 312, FACING_WEST,
        TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_FARAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 312, FACING_WEST,
        TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gBabiLighthouse_RoadActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 128, 0, 264, FACING_WEST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 96, 0, 184, FACING_EAST, TALK_FACING_PARTY,
        0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 200, FACING_WEST, TALK_FACING_PARTY,
        0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 184, FACING_SOUTH,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 216, FACING_SOUTH,
        TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gBabiLighthouse_EntryTriggers[] = {
    {TRIGGER_EXIT, BABI_LIGHTHOUSE_ENTRY_TO_ROAD_TO_BABI_LIGHTHOUSE, CONDITION_ALWAYS, BABI_LIGHTHOUSE_ENTRY_TO_ROAD_TO_BABI_LIGHTHOUSE},
    {TRIGGER_EXIT, BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_EAST, CONDITION_ALWAYS, BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_EAST},
    {TRIGGER_EXIT, BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_WEST, CONDITION_ALWAYS, BABI_LIGHTHOUSE_ENTRY_TO_BABI_LIGHTHOUSE_LUFF_WEST},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_BABI_LIGHTHOUSE_BESIDE_PILLAR, CONDITION_ALWAYS, EVENT(BabiLighthouse_PushEntryPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_BABI_LIGHTHOUSE_LADDER_HOLE, CONDITION_ALWAYS, EVENT(BabiLighthouse_PushPillarOrClimbDownHole)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_PILLAR, CONDITION_ALWAYS, EVENT(BabiLighthouse_HopClearOfPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_PILLAR, CONDITION_ALWAYS, EVENT(BabiLighthouse_HopClearOfPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_GROWTH_PASSAGE_OPEN, EVENT(BabiLighthouse_OpenGrowthPassage)},
    {TRIGGER_TABLE_END},
};

// Inside, the first seven exits' cells carry numbers of their own.
struct Trigger gBabiLighthouse_LuffTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_WEST_ROOM_TO_EAST_ROOM_STEPS},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_ENTRY_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_WEST_ROOM_STEPS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_WEST_STEPS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_EAST_ROOM_TO_SOUTH_ROOM_EAST_STEPS},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_ENTRY_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_WEST_STEPS, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_WEST_STEPS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_EAST_STEPS, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_EAST_ROOM_EAST_STEPS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_LOCKED_DOOR_ROOM, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_SOUTH_ROOM_TO_LOCKED_DOOR_ROOM},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, BABI_LIGHTHOUSE_LUFF_LOCKED_DOOR_ROOM_TO_SOUTH_ROOM, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_LOCKED_DOOR_ROOM_TO_SOUTH_ROOM},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, BABI_LIGHTHOUSE_LUFF_TO_BABI_LIGHTHOUSE_STONE_DOOR_WEST},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_BABI_LIGHTHOUSE_EAST_SLIDE, CONDITION_ALWAYS, EVENT(BabiLighthouse_DropFromWestRoomEastPit)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_BABI_LIGHTHOUSE_LUFF_SLIDE, CONDITION_ALWAYS, EVENT(BabiLighthouse_DropFromEastRoomPit)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_BABI_LIGHTHOUSE_SOUTHWEST_SLIDE, CONDITION_ALWAYS, EVENT(BabiLighthouse_DropFromWestRoomSouthwestPit)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_BABI_LIGHTHOUSE_NORTHWEST_SLIDE, CONDITION_ALWAYS, EVENT(BabiLighthouse_DropFromWestRoomNorthwestPit)},
    {TRIGGER_TALK, ACTOR_LUFF, CONDITION_ALWAYS, EVENT(BabiLighthouse_StartDjinnBattle)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_LIGHTHOUSE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(BabiLighthouse_InspectLockedDoor)},
    {TRIGGER_TABLE_END},
};

struct Trigger gBabiLighthouse_StoneDoorTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, BABI_LIGHTHOUSE_STONE_DOOR_TO_TUNNEL_RUINS_GREAT_STATUE, CONDITION_ALWAYS, BABI_LIGHTHOUSE_STONE_DOOR_TO_TUNNEL_RUINS_GREAT_STATUE},
    {TRIGGER_EXIT, BABI_LIGHTHOUSE_STONE_DOOR_TO_BABI_LIGHTHOUSE_LUFF_WEST, CONDITION_ALWAYS, BABI_LIGHTHOUSE_STONE_DOOR_TO_BABI_LIGHTHOUSE_LUFF_WEST},
    {TRIGGER_STEP, CELL_BEHAVIOR_BABI_LIGHTHOUSE_CAVE_MOUTH, FLAG_BABI_LIGHTHOUSE_ASKED_TO_FOLLOW, EVENT(BabiLighthouse_FaranAsksPartyToOpenDoor)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BABI_LIGHTHOUSE_SOUTH_OF_DOOR, IF_FLAG_SET(FLAG_BEYOND_DOOR), EVENT(BabiLighthouse_CloseTruthDoor)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BABI_LIGHTHOUSE_PAST_DOOR, FLAG_BEYOND_DOOR, EVENT(BabiLighthouse_CloseTruthDoor)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BABI_LIGHTHOUSE_NORTH_OF_DOOR, CONDITION_ALWAYS, EVENT(BabiLighthouse_OpenTruthDoor)},
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
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_LIGHTHOUSE_DOOR_SWITCH, CONDITION_ALWAYS, EVENT(BabiLighthouse_InspectTruthDoorSwitch)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS,
        EVENT(BabiLighthouse_ShowTruthDoorSwitch)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(BabiLighthouse_HideTruthDoorSwitch)},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gBabiLighthouse_RoadTriggers[] = {
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     CELL_BEHAVIOR_BABI_LIGHTHOUSE_PILLAR_LEDGE, CONDITION_ALWAYS,
     EVENT(BabiLighthouse_PushRoadPillar)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     CELL_BEHAVIOR_BABI_LIGHTHOUSE_LADDER, CONDITION_ALWAYS,
     EVENT(BabiLighthouse_ClimbOrPushRoadPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROAD_PILLAR, CONDITION_ALWAYS,
     EVENT(BabiLighthouse_UpdateMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_ROAD_PILLAR, CONDITION_ALWAYS,
     EVENT(BabiLighthouse_LandCarriedPillarInPuffs)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_ROAD_PILLAR, CONDITION_ALWAYS,
     EVENT(BabiLighthouse_UpdateMovedPillar)},
    {TRIGGER_EXIT, ROAD_TO_BABI_LIGHTHOUSE_TO_WORLD_MAP, CONDITION_ALWAYS,
     ROAD_TO_BABI_LIGHTHOUSE_TO_WORLD_MAP},
    {TRIGGER_EXIT, ROAD_TO_BABI_LIGHTHOUSE_TO_BABI_LIGHTHOUSE_ENTRY,
     CONDITION_ALWAYS, ROAD_TO_BABI_LIGHTHOUSE_TO_BABI_LIGHTHOUSE_ENTRY},
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
    {EXAMINE_CHEST_FACING(FACING_EAST),
     CELL_BEHAVIOR_BABI_LIGHTHOUSE_LUCKY_PEPPER_CHEST,
     FLAG_BABI_LIGHTHOUSE_LUCKY_PEPPER, FIND_ITEM(ITEM_LUCKY_PEPPER)},
    {TRIGGER_TABLE_END},
};
