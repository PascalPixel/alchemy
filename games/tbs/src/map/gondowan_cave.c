/// @file
/// Gondowan Passage and Gondowan Cave, the guarded bridge into Gondowan and the
/// cave east of it, which the party crosses on the way from Tolbi to Babi
/// Lighthouse. Three maps share this overlay:
///
/// - Gondowan Passage (MapId_GondowanPassage, area name Gondowan Passage):
///   three soldiers guard the bridge, and Iodem waits there to go on to Babi
///   Lighthouse with the party once Babi has spoken to it after Colosso
///   (FLAG_BABI_SPOKE_AFTER_COLOSSO).
/// - Gondowan Cave (MapId_GondowanCave, area name Gondowan Cave): both ways
///   in from the world map, a tree stump that Move sends leaping into the
///   water, a stone pillar to push or move in after it, and a boulder for
///   Lift.
/// - GondowanCaveRiver (MapId_GondowanCaveRiver): the river between the
///   cave's two halves, with a boulder for Lift and two chests.
///
/// One warp table serves every map; the other getters choose by
/// gGameState.mapId, Gondowan Passage's object event templates and map events
/// also by FLAG_BABI_SPOKE_AFTER_COLOSSO, and an empty set answers any other
/// map number. Every map's event table carries GondowanCaveRiver's two chests.
///
/// Gondowan Passage sets FLAG_RETREAT_BLOCKED on every entry. While Iodem
/// travels with the party (FLAG_IODEM_FOLLOWING_PARTY) his object event is kept
/// out of sight; talking to him or walking up to him while he waits (coord
/// event 11) makes him come along (EventScript_IodemRejoins), and until he has
/// reached Babi Lighthouse (FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED) turning back
/// (coord event 10) brings him out to ask why
/// (EventScript_IodemAsksWhyTurningBack). If he stays to wait, the saved place
/// becomes the Tolbi Sanctum, so a defeat returns the party there.
///
/// GondowanCave's way across the water is two things that float in it, and
/// coord events 10 and 11 jump the leader across them (EventScript_JumpAcross).
/// Both are kept in story flags, so the cave stays solved and Map_Initialize
/// puts them back: FLAG_GONDOWAN_CAVE_STUMP_MOVED once the stump has leapt in,
/// and FLAG_GONDOWAN_CAVE_PILLAR_DROPPED once the pillar has gone in past
/// column 42, shown from then on by LOCALID_DROPPED_PILLAR.
///
/// Cell (107, 17) of the map grid is cell (43, 17) of layout layer 2,
/// whose window starts 64 columns in: the crevice east of the pillar,
/// where it drops. The layout loads that cell with ground 6, as the two cells
/// west of it; copying (108, 17) over it gives it ground 0, as its east
/// neighbour. The drop makes that copy, and so does every arrival by
/// GONDOWAN_CAVE_FROM_WORLD_MAP_EAST, solved or not. Nothing the player sees
/// depends on it: in tbs-en the screen is the same either way, the leader
/// jumps the crevice at row 17 (coord events 10 and 11) from either side
/// either way, and the pillar settles at the same height in each of columns
/// 41 to 44 either way.
///
/// Open questions:
/// - XXX (no evidence): what flags 0x9a3, 0x9a4 and 0x9a6 hold;
///   EventScript_LeaveToWorldMapEast rearranges them only with FLAG_MAP_TEMP_04
///   set, which nothing in the game sets, and no other code reads them.
/// - XXX (no evidence): why arriving from the world map's east side makes the
///   drop's cell change; the change has no effect that play shows (the
///   screen, the leader's jumps and the pillar's height are the same either
///   way), so neither the ROM nor a render can show what it was for.

#include "game_state.h"
#include "heap_slots.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/gondowan_cave.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "pushed_object.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "object_event.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"
#include "script.h"

/// Gondowan Passage's placed object events, numbered in the order of
/// gGondowanPassageObjectEventTemplates and
/// gGondowanPassageObjectEventTemplatesWithIodem from LOCALID_FIRST_PLACED: the
/// soldiers who guard the bridge, and in the second table Iodem after them.
enum GondowanPassageLocalId {
  /// Asks whether the party will cross into Gondowan.
  /// ★ Named from the game: "Us? We're the soldiers who guard the bridge to
  /// Gondowan." (MsgGondowanCaveUsWereTheSoldiersWhoGuard)
  LOCALID_FIRST_SOLDIER = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "Us? We're the soldiers who guard the bridge to
  /// Gondowan." (MsgGondowanCaveUsWereTheSoldiersWhoGuard)
  LOCALID_SECOND_SOLDIER,
  /// Warns that the cave to the east leads nowhere.
  /// ★ Named from the game: "Us? We're the soldiers who guard the bridge to
  /// Gondowan." (MsgGondowanCaveUsWereTheSoldiersWhoGuard)
  LOCALID_THIRD_SOLDIER,
  /// Waits for the party once Babi has spoken after Colosso.
  /// ★ Named from the game: "Why would Iodem choose to wait for us"
  /// (MsgGondowanCaveWhyWouldIodemChooseToWait)
  LOCALID_IODEM
};

/// GondowanCaveRiver's placed object event, the one row of
/// gGondowanCaveRiverObjectEventTemplates that a map event names.
enum GondowanCaveRiverLocalId {
  /// Raised by Lift.
  /// ★ Named from what the player sees: a round grey boulder by the river
  /// (its render)
  LOCALID_RIVER_BOULDER = LOCALID_FIRST_PLACED
};

/// GondowanCave's placed object events, numbered in the order of
/// gGondowanCaveObjectEventTemplates from LOCALID_FIRST_PLACED.
enum GondowanCaveLocalId {
  /// Moved with Move, in the sprite of the Goma Cave stump.
  /// ★ Named from the game: "that stump" (MsgGomaNoUsePsynergy)
  LOCALID_STUMP = LOCALID_FIRST_PLACED,
  /// Pushed or moved with Move, in the sprite the stone pillars of other maps
  /// use (crossbone_isle_stepping_stones.c).
  /// ★ Named from what the player sees: a tall tan stone pillar standing on
  /// the cave floor (its render)
  LOCALID_PILLAR,
  /// Stands in for the pillar once it has dropped in; out of sight at first.
  /// It is drawn in the stump's sprite, splashing in (STUMP_ANIMATION_SPLASH)
  /// and
  /// on later visits afloat (STUMP_ANIMATION_SINK).
  /// ★ Named from what the player sees: the pillar's top in the water, after
  /// the splash it drops in with (its render)
  LOCALID_DROPPED_PILLAR,
  /// Raised by Lift.
  /// ★ Named from what the player sees: a round boulder in the cave passage,
  /// which floats up under Lift (its render)
  LOCALID_BOULDER,
  /// An invisible object event (SPRITE_BLANK, its shadow turned off) 16 pixels
  /// west
  /// of the boulder. It fills the gap between the boulder and the passage's
  /// west wall, so the boulder blocks the passage until Lift takes both away
  /// (https://alchemy.pascalpixel.com/places/gondowan-cave/map-4.png shows
  /// the gap).
  /// ★ Named from what the player sees: the open-looking gap between the
  /// boulder and the passage's west wall, map-4.png (its render)
  LOCALID_BOULDER_GAP
};

// The leap script also runs after the shared pushed object movement.
static void EventScript_MovePillar(void);
static void GondowanCave_RunSpinningLeap(s32 localId);

// Shared effect routines retain their position before the map's scripts.
#include "common/effect_create.h"

// Particle and movable-object routines precede this map's entry points.
#include "common/effect_spawn.h"
#include "common/object_staged_object_event.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

const struct MapWarpDest *Map_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_GondowanPassage) {
    return gGondowanPassageWarpDests;
  }
  if (mapId == (s32)&MapId_GondowanCaveRiver) {
    return gGondowanCaveRiverWarpDests;
  }
  if (mapId == (s32)&MapId_GondowanCave) {
    return gGondowanCaveWarpDests;
  }
  return gGondowanCaveWarpDestsOther;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

/// Every map of the overlay shares one warp table.
const u32 *Map_GetWarps(void) { return gGondowanCaveWarps; }

/// Iodem waits in Gondowan Passage once Babi has spoken to the party after
/// Colosso.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_GondowanPassage) {
    if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gGondowanPassageObjectEventTemplatesWithIodem;
    }
    return gGondowanPassageObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_GondowanCaveRiver) {
    return gGondowanCaveRiverObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_GondowanCave) {
    return gGondowanCaveObjectEventTemplates;
  }
  return gGondowanCaveObjectEventTemplatesOther;
}

/// The map events of each map, with Iodem's in Gondowan Passage when he is
/// there.
const struct MapEvent *Map_GetEvents(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_GondowanPassage) {
    if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gGondowanPassageEventsWithIodem;
    }
    return gGondowanPassageEvents;
  }
  if (mapId == (s32)&MapId_GondowanCaveRiver) {
    return gGondowanCaveRiverEvents;
  }
  if (mapId == (s32)&MapId_GondowanCave) {
    return gGondowanCaveEvents;
  }
  return gGondowanCaveEventsOther;
}

/// Turning back in Gondowan Passage (coord event 10) with Iodem in the party,
/// until he has reached Babi Lighthouse: he steps out and asks why Isaac is
/// going back. Yes: he waits here for the party's return. No: he rejoins the
/// party and Isaac steps away from the coord event.
static void EventScript_IodemAsksWhyTurningBack(void) {
  struct ObjectEvent *leader;

  if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED) == 0 &&
      Script_GetFlag(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
    SCRIPT_START();
    // Bring Iodem out of the party before opening his question.
    ObjectMotion_SetSpeedParameters(LOCALID_IODEM, PIXELS(1), PIXELS(1) / 2);
    leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      ObjectEvent_SetPosition(LOCALID_IODEM, leader->x.fixed, leader->z.fixed);
    }
    ObjectMotion_OffsetPositionAndResetMotion(LOCALID_IODEM, -8, 16);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IODEM);
    OBJECT_DIR(LOCALID_IODEM, FACING_NORTH + FACING_STEP, 0);
    WAIT(10);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IODEM, 0);
    MESSAGE_SET(MsgKaragolWhyGoingBackIsaacDo);
    Script_OpenMessage(LOCALID_IODEM, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      // "I see... In that case, I shall wait here for you to return." The
      // saved place becomes Tolbi's.
      Script_ShowMessage(LOCALID_IODEM, 0);
      OBJECT_WALK(LOCALID_IODEM, 152, 232);
      Script_ClearFlag(FLAG_IODEM_FOLLOWING_PARTY);
      ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IODEM);
      OBJECT_ANIMATION(LOCALID_IODEM, ANIM_STAND);
      gGameState.savedMap = (s32)&MapId_TolbiInteriors;
      gGameState.savedWarpDest = TOLBI_INTERIORS_SANCTUM_NORTH;
    } else {
      // "Then we should continue on our way to the lighthouse." He rejoins
      // the party, and Isaac steps away from the coord event.
      gFieldWork->message += 1;
      Script_ShowMessage(LOCALID_IODEM, 0);
      OBJECT_ANIMATION(LOCALID_IODEM, ANIM_WALK);
      leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
      if (leader != 0) {
        ObjectEvent_SetDestination(LOCALID_IODEM, leader->x.part.pixel,
                                   leader->z.part.pixel);
      }
      ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IODEM);
      OBJECT_POSITION_SET(LOCALID_IODEM, 0, 0);
      WAIT(30);
      OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIM_WALK);
      ObjectMotion_OffsetPositionAndResetMotion(LOCALID_PARTY_LEADER, 0, 16);
      ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
      OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIM_STAND);
    }
    SCRIPT_END();
  }
}

/// Iodem comes along with the party from Gondowan Passage, where he waits. He
/// comes along when he's talked to, or walked up to (coord event 11) while he
/// isn't with the party. `I've been waiting for you, Isaac! We should go to
/// Babi Lighthouse soon.`
static void EventScript_IodemRejoins(void) {
  SCRIPT_START();
  OBJECT_FACE(LOCALID_IODEM, LOCALID_PARTY_LEADER, 0);
  OBJECT_FACE(LOCALID_PARTY_LEADER, LOCALID_IODEM, 0);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIM_STAND);
  WAIT(10);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_IODEM, 0);
  MESSAGE_SET(MsgKaragolIveBeenWaitingForIsaac);
  MESSAGE_PUT(LOCALID_IODEM, 0);
  OBJECT_ANIMATION(LOCALID_IODEM, ANIM_WALK);
  {
    struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

    if (leader != 0)
      ObjectMotion_ResetAndSetPosition(LOCALID_IODEM, leader->x.part.pixel,
                                       leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IODEM);
  Engine_ObjectEventSetPosition(LOCALID_IODEM, 0, 0);
  WAIT(20);
  GameFlag_Set(FLAG_IODEM_FOLLOWING_PARTY);
  SCRIPT_END();
}

/// Talking to the first soldier in Gondowan Passage before Babi has spoken
/// after Colosso: he asks whether the party will cross into Gondowan. `Do you
/// wish to cross into Gondowan?` Yes: `I won't stop you, but I wouldn't go
/// unless I had business there.`, with advice to watch Colosso until it is
/// over. No: `That's a good idea. They wouldn't let you into Babi Lighthouse,
/// anyway.`
static void EventScript_SoldierAsksAboutCrossing(void) {
  s32 firstMessage;

  firstMessage = MsgKaragolDoYouWishCrossInto;
  MESSAGE_SET(firstMessage);
  MESSAGE_OPEN(LOCALID_FIRST_SOLDIER, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0) {
        // The shorter answer, without the advice to watch Colosso.
        MESSAGE_SET(firstMessage + 8);
      }
    }
    MESSAGE_PUT(LOCALID_FIRST_SOLDIER, 0);
  } else {
    gFieldWork->message++;
    MESSAGE_PUT(LOCALID_FIRST_SOLDIER, 0);
  }
}

/// Mind Read on the second soldier in Gondowan Passage, before Babi has
/// spoken after Colosso: once Colosso is over (FLAG_COLOSSO_OVER) he grumbles
/// that guard duty made him miss all of it; before, that the warriors fight
/// while he guards the bridge.
static void EventScript_ReadSoldierMind(void) {
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0 &&
      GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0)
    MESSAGE_SET(MsgKaragolWeMissedColossoBecauseWe);
  else
    MESSAGE_SET(MsgKaragolWarriorsHaveBeenFightingWhile);
  MESSAGE_PUT(LOCALID_SECOND_SOLDIER, 0);
}

/// Leaving GondowanCave by GONDOWAN_CAVE_TO_WORLD_MAP_EAST (warp 1),
/// out to the world map. With FLAG_MAP_TEMP_04 set, which nothing in the
/// game sets, three unnamed flags and the pillar's are first put into one
/// arrangement.
static void EventScript_LeaveToWorldMapEast(void) {
  struct FieldWork *work;

  work = gFieldWork;
  SCRIPT_START();
  if (GameFlag_Get(FLAG_MAP_TEMP_04) != 0) {
    GameFlag_Clear(0x9a3);
    Script_ClearFlag(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
    Script_ClearFlag(0x9a4);
    GameFlag_Clear(0x9a6);
    GameFlag_Set(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
    GameFlag_Set(0x9a4);
  }
  Engine_ScriptRequestWarp(work->coordEvent);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  SCRIPT_END();
}

/// Walking west or east into coord events 10 and 11 in GondowanCave: the leader
/// jumps two cells forward where the map allows (Field_TryJumpForward), across
/// the stump and the pillar once they float in the water.
static void EventScript_JumpAcross(void) { Field_TryJumpForward(); }

/// Move on the stump in GondowanCave, until it has leapt: from column 30 or
/// less it spins and leaps into the water, freeing the cell where it stood.
static void EventScript_MoveStump(void) {
  ObjectEvent_GetById(LOCALID_STUMP);
  SCRIPT_START();
  {
    struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_STUMP);

    if ((objectEvent->x.fixed >> 20) <= 30) {
      GondowanCave_RunSpinningLeap(LOCALID_STUMP);
      MapGrid_CopyCellAttributes(29, 19, 1, 1, 27, 19);
      GameFlag_Set(FLAG_GONDOWAN_CAVE_STUMP_MOVED);
    }
  }
  SCRIPT_END();
}

/// Pushing the pillar in GondowanCave (coord event 50): it moves a
/// step, then may have reached its place.
static void EventScript_PushPillar(void) {
  PushedObject_AdvancePair();
  EventScript_MovePillar();
}

/// Hides the pillar and shows the object event that stands in for it in its
/// place.
static inline void GondowanCave_PlaceDroppedPillar(s32 animation) {
  OBJECT_POSITION_SET(LOCALID_PILLAR, 0, 0);
  OBJECT_POSITION_SET(LOCALID_DROPPED_PILLAR, 696, 288);
  OBJECT_ANIMATION(LOCALID_DROPPED_PILLAR, animation);
}

/// Move on the pillar in GondowanCave, and after each push: past column 42 it
/// drops into the water with a splash, for good.
static void EventScript_MovePillar(void) {
  SCRIPT_START();
  if ((ObjectEvent_GetById(LOCALID_PILLAR)->x.fixed >> 20) > 42) {
    s32 destinationCellX = 107;
    s32 destinationCellZ = 17;

    MapGrid_CopyCellAttributes(108, 17, 1, 1, destinationCellX,
                               destinationCellZ);
    WAIT(8);
    GondowanCave_PlaceDroppedPillar(STUMP_ANIMATION_SPLASH);
    SE(SE_CAST);
    GameFlag_Set(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
  }
  SCRIPT_END();
}

/// Unused: an empty function nothing calls.
static UNUSED void GondowanCave_UnusedObjectEventCallback(void) {}

/// Rotates the sprite during the airborne part of the leap.
static void GondowanCave_RotateLeapingObject(union FieldObject *fieldObject) {
  fieldObject->objectEvent.sprite->rotation -= 0x400;
}

/// Waits at most forty frames for the object event to descend to the height
/// limit.
static void GondowanCave_WaitForDescent(struct ObjectEvent *objectEvent,
                                        s32 heightLimit) {
  s32 framesRemaining = 40;

  while (framesRemaining != 0) {
    WaitFrames(1);
    framesRemaining--;
    if (objectEvent->y.fixed <= heightLimit) {
      break;
    }
  }
}

/// The object spins in place, leaps and lands with a shake and a ring of dust,
/// then comes to rest a little further on.
static void GondowanCave_RunSpinningLeap(s32 localId) {
  struct ObjectEvent *objectEvent;
  u32 stepIndex;
  struct Vector3 particleVelocity;
  s32 angle;

  objectEvent = ObjectEvent_GetById(localId);
  // Wind up the spin while horizontal motion is disabled.
  objectEvent->motionFlags = 0;
  for (stepIndex = 0; stepIndex < 9; stepIndex++) {
    WaitFrames(1);
    objectEvent->sprite->rotation -= 0x100;
    objectEvent->x.fixed -= Trig_Cos(objectEvent->sprite->rotation) / 2;
    objectEvent->targetX = OBJ_EVENT_NO_TARGET;
  }
  objectEvent->update = GondowanCave_RotateLeapingObject;
  // Continue spinning through the first jump.
  Sound_PlaySong(SE_136);
  OBJECT_SPEED(localId, PIXELS(2), PIXELS(1));
  ObjectEvent_SetDestination(localId, 472, 288);
  objectEvent->gravity = 4 * PIXELS(1) / 5;
  objectEvent->motionFlags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
  objectEvent->layer = 0;
  ObjectMotion_CommitCurrentPositionAndActivate(localId);
  GondowanCave_WaitForDescent(objectEvent, PIXELS(32));
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  // Emit a complete particle fan, including its closing direction.
  for (stepIndex = 0; stepIndex < 17; stepIndex++) {
    angle = stepIndex << 12;
    particleVelocity.x = Trig_Cos(angle);
    particleVelocity.y = 0;
    particleVelocity.z = Trig_Sin(angle);
    particleVelocity.x -= particleVelocity.x / 4;
    particleVelocity.z -= particleVelocity.z / 2;
    FieldEffect_Spawn(objectEvent->x.fixed, objectEvent->y.fixed,
                      objectEvent->z.fixed, particleVelocity.x,
                      particleVelocity.y, particleVelocity.z, 0, 0);
  }
  ObjectEvent_SetDestination(localId, 440, 308);
  // Finish the second movement before restoring the resting animation.
  ObjectMotion_CommitCurrentPositionAndActivate(localId);
  GondowanCave_WaitForDescent(objectEvent, PIXELS(32));
  objectEvent->update = 0;
  objectEvent->sprite->rotation = 0x1000;
  Sound_PlaySong(SE_CAST);
  OBJECT_ANIMATION(localId, STUMP_ANIMATION_SPLASH);
  MapGrid_WaitForShakeToSettle();
  WAIT(10);
  OBJECT_ANIMATION(localId, STUMP_ANIMATION_SINK);
}

/// Unused: an empty function nothing calls.
static UNUSED void GondowanCave_UnusedMapCallback(void) {}

/// Lift on the boulder in GondowanCave also takes away the
/// invisible object event beside it.
static void EventScript_LiftBoulder(void) {
  Engine_ObjectEventSetPosition(LOCALID_BOULDER_GAP, 0, 0);
}

/// Sets up the map as the party enters. Gondowan Passage blocks Retreat and
/// keeps Iodem out of sight while he travels with the party; Gondowan Cave's
/// GondowanCave puts the stump and the pillar where the party left them.
s32 Map_Initialize(void) {
  struct ObjectEvent *objectEvent;

  if (gGameState.mapId == (s32)&MapId_GondowanPassage) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    if (GameFlag_Get(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
      OBJECT_POSITION_SET(LOCALID_IODEM, 0, 0);
    }
  }
  if (gGameState.mapId == (s32)&MapId_GondowanCave) {
    if (gGameState.warpDest == GONDOWAN_CAVE_FROM_WORLD_MAP_EAST) {
      MapGrid_CopyCellAttributes(108, 17, 1, 1, 107, 17);
    }
    if (GameFlag_Get(FLAG_GONDOWAN_CAVE_STUMP_MOVED) != 0) {
      OBJECT_POSITION_SET(LOCALID_STUMP, 440, 308);
      OBJECT_ANIMATION(LOCALID_STUMP, STUMP_ANIMATION_SINK);
      MapGrid_CopyCellAttributes(29, 19, 1, 1, 27, 19);
    }
    if (GameFlag_Get(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED) != 0) {
      GondowanCave_PlaceDroppedPillar(STUMP_ANIMATION_SINK);
    }
    objectEvent = ObjectEvent_Get(LOCALID_BOULDER_GAP);
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
  }
  return 0;
}

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

/// Packs signed pixel steps with X in the upper halfword and Z in the lower.
/// Shift the unsigned representation so westward steps remain defined C.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gPushedObject_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedObject_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedObject_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

const s32 *gEffectScripts[] = {
    gShortEffectScript,
    gMediumEffectScript,
    gLongEffectScript,
};

struct MapWarpDest gGondowanCaveWarpDestsOther[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gGondowanPassageWarpDests[] = {
    { GONDOWAN_PASSAGE_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 152, 0, 120, FACING_SOUTH, 0, 32, 32, 272, 480, 0 },
    { GONDOWAN_PASSAGE_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 152, 0, 440, FACING_NORTH, 0, 32, 32, 272, 480, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gGondowanCaveRiverWarpDests[] = {
    { GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_NORTH, CONDITION_ALWAYS, 728, 0, 312, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
    { GONDOWAN_CAVE_RIVER_FROM_BOULDER_PASSAGE, CONDITION_ALWAYS, 168, 0, 408, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
    { GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_FROM_RIVER, CONDITION_ALWAYS, 168, 0, 520, FACING_SOUTH, 0, 112, 464, 576, 704, 0 },
    { GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_SOUTH, CONDITION_ALWAYS, 504, 0, 664, FACING_NORTH, 0, 112, 464, 576, 704, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gGondowanCaveWarpDests[] = {
    { GONDOWAN_CAVE_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 792, 0, 456, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
    { GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_NORTH, CONDITION_ALWAYS, 360, 0, 152, FACING_SOUTH, 0, 240, 48, 864, 464, 0 },
    { GONDOWAN_CAVE_FROM_WEST_PASSAGE, CONDITION_ALWAYS, 344, 0, 440, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
    { GONDOWAN_CAVE_WEST_PASSAGE_FROM_EAST_CAVE, CONDITION_ALWAYS, 344, 0, 584, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
    { GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_SOUTH, CONDITION_ALWAYS, 136, 0, 504, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
    { GONDOWAN_CAVE_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 216, 0, 888, FACING_NORTH, 0, 48, 432, 408, 896, 0 },
    { MAP_TABLE_END },
};

u32 gGondowanCaveWarps[] = {
    MAP_WARPS(GondowanPassage),
    MAP_WARP(GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_PASSAGE_EAST),
    MAP_WARP(GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_PASSAGE_WEST),
    MAP_WARPS(GondowanCaveRiver),
    MAP_WARP(GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_NORTH, GondowanCave, GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_NORTH),
    MAP_WARP(GONDOWAN_CAVE_RIVER_TO_BOULDER_PASSAGE, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_FROM_RIVER),
    MAP_WARP(GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_TO_RIVER, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_FROM_BOULDER_PASSAGE),
    MAP_WARP(GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_SOUTH, GondowanCave, GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_SOUTH),
    MAP_WARPS(GondowanCave),
    MAP_WARP(GONDOWAN_CAVE_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_GONDOWAN_CAVE_NORTH),
    MAP_WARP(GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_NORTH, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_NORTH),
    MAP_WARP(GONDOWAN_CAVE_TO_WEST_PASSAGE, GondowanCave, GONDOWAN_CAVE_WEST_PASSAGE_FROM_EAST_CAVE),
    MAP_WARP(GONDOWAN_CAVE_WEST_PASSAGE_TO_EAST_CAVE, GondowanCave, GONDOWAN_CAVE_FROM_WEST_PASSAGE),
    MAP_WARP(GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_SOUTH, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_SOUTH),
    MAP_WARP(GONDOWAN_CAVE_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_GONDOWAN_CAVE_SOUTH),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gGondowanCaveObjectEventTemplatesOther[] = {
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gGondowanPassageObjectEventTemplates[] = {
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(144), 0, PIXELS(168), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gGondowanPassageObjectEventTemplatesWithIodem[] = {
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    // Iodem
    { SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(232), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gGondowanCaveRiverObjectEventTemplates[] = {
    { SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(624), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(440), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gGondowanCaveObjectEventTemplates[] = {
    { SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(496), 0, PIXELS(304), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(664), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(636), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(640), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gGondowanCaveEventsOther[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gGondowanPassageEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH },
    { MAP_EVENT_TALK, LOCALID_FIRST_SOLDIER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SoldierAsksAboutCrossing) },
    { MAP_EVENT_TALK, LOCALID_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveUsWereTheSoldiersWhoGuard) },
    { MAP_EVENT_TALK, LOCALID_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouHeadEastYou) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveThoseWhoGoToGondowanRarely) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SECOND_SOLDIER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadSoldierMind) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveYoudThinkYouCouldGetThrough) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { MAP_EVENTS_END },
};

struct MapEvent gGondowanPassageEventsWithIodem[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH },
    { MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_IodemAsksWhyTurningBack) },
    { MAP_EVENT_COORD, 11, FLAG_IODEM_FOLLOWING_PARTY, EVENT_SCRIPT(EventScript_IodemRejoins) },
    { MAP_EVENT_TALK, LOCALID_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWhyWouldIodemChooseToWait) },
    { MAP_EVENT_TALK, LOCALID_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaventSeenAnybodyUnusualCrossing) },
    { MAP_EVENT_TALK, LOCALID_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouGoEastFrom) },
    { MAP_EVENT_TALK, LOCALID_IODEM, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_IodemRejoins) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveIodemIsGoingToBabiLighthouse) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveItsALotOfStressBut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEveryoneComesDownThisWayWhen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_IODEM, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaveToHeadForThe) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { MAP_EVENTS_END },
};

struct MapEvent gGondowanCaveRiverEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_NORTH },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_BOULDER_PASSAGE },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_TO_RIVER },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_SOUTH },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_LIFT), LOCALID_RIVER_BOULDER, CONDITION_ALWAYS, 0 },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { MAP_EVENTS_END },
};

/// FLAG_MAP_TEMP_05 ends Move on the pillar.
struct MapEvent gGondowanCaveEvents[] = {
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToWorldMapEast) },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_NORTH },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_WEST_PASSAGE },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, GONDOWAN_CAVE_WEST_PASSAGE_TO_EAST_CAVE },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_SOUTH },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_WORLD_MAP_WEST },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_LIFT), LOCALID_BOULDER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LiftBoulder) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_STUMP, FLAG_GONDOWAN_CAVE_STUMP_MOVED, EVENT_SCRIPT(EventScript_MoveStump) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_PILLAR, FLAG_MAP_TEMP_05, EVENT_SCRIPT(EventScript_MovePillar) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpAcross) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpAcross) },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 50, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillar) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { MAP_EVENTS_END },
};
// clang-format on
