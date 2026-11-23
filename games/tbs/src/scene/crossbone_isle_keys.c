/// @file
/// Three of the side rooms of Crossbone Isle Cave, the cave beneath Crossbone
/// Isle, with four chests each.
///
/// Each opens off one of the nine floors the cave descends through
/// (crossbone_isle.c), as the other five do
/// (crossbone_isle_stepping_stones.c). The party comes in by the floor's east
/// door and the room is the way across to its west door. All three take the
/// area name Crossbone Isle Cave. In the order the party meets them:
///
/// - The room of keys (SceneId_CrossboneIsleKeys), off the third floor: a
///   pit full of square stone posts, with a walkway along its north wall.
///   Four blue doors in a row along the walkway lead to four small rooms with
///   a chest each: 333 coins, the Fairy Ring, a Cookie and a Smoke Bomb, west
///   to east. A red door on a ledge west of the pit is the way on to the
///   floor's west door.
///
///   Four Blue Keys lie on top of posts out of reach, and Catch pulls each
///   one in. Walking up to a blue door with a Blue Key opens it and uses the
///   key up. From the walkway in front of the fourth door Isaac can hop down
///   onto the posts, and Reveal there shows a hidden post with the Red Key
///   on it, which Catch takes too. The Red Key opens the red door the same
///   way. The keys taken and the doors opened stay so on later visits.
///
/// - The statue room (SceneId_CrossboneIsleStatues), off the fourth floor: a
///   dark hall where five statues of a robed woman stand in circles of light
///   round braziers.
///
///   The statue in the north-east, the one on the narrow walk in the west
///   and the one in the middle each guard the few tiles in front of them.
///   Stepping onto those tiles makes the statue glide onto Isaac with a thud
///   and shove him back south, then glide home, unless Cloak hides him. The
///   two statues in the alcoves on the south walkway near the east door slide
///   sideways to stay in front of Isaac as he walks past, so that he cannot
///   get by them. Halt stops a statue where it is, and while Reveal lasts the
///   darkness lifts from the whole hall.
///
/// - Bane's room (SceneId_CrossboneIsleBane), off the sixth floor, where
///   Bane, a Venus Djinni, waits at the west end of the ledge along the back
///   wall. Five stone pillars to push or shift with Move, the ladders and a
///   puddle that Frost freezes into a pillar of ice make the way across the
///   room. A chest on one pillar holds the Demon Axe.
///
///   The first two times Isaac comes near, Bane runs off along the room. The
///   third time it flees to a ledge in the south-east with three spikes on
///   it, and there it keeps running round the spikes away from him until
///   Halt holds it still. Talked to while it is held, Bane joins the party.

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "scene_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "system/dma.h"
#include "system/heap_slots.h"
#include "system/io_write.h"
#include "system/main_routine_work.h"
#include "system/system.h"

extern s32 gCrossboneIsleKeys_BaneSquashScript[];
extern s32 gCrossboneIsleKeys_BaneWalkTargets[][2];
extern struct SceneEntrance gScene_FallbackEntrances[];
extern struct SceneEntrance gCrossboneIsleKeysRoomEntrances[];
extern struct SceneEntrance gCrossboneIsleStatuesEntrances[];
extern struct SceneEntrance gCrossboneIsleBaneEntrances[];
extern u32 gCrossboneIsleKeysExits[];
extern struct ActorPlacement gCrossboneIsleKeysActorPlacementsOther[];
extern struct ActorPlacement gCrossboneIsleKeysRoomActorPlacements[];
extern struct ActorPlacement gCrossboneIsleStatuesActorPlacements[];
extern struct ActorPlacement gCrossboneIsleBaneActorPlacements[];
extern struct Trigger gCrossboneIsleKeysTriggersOther[];
extern struct Trigger gCrossboneIsleKeysRoomTriggers[];
extern struct Trigger gCrossboneIsleStatuesTriggers[];
extern struct Trigger gCrossboneIsleBaneTriggers[];
extern u32 gCrossboneIsleKeys_SlotColumns[];
extern s32 gCrossboneIsleKeys_IconTimer;

// Map-section temp flags: Scene_Arrive clears FLAG_AREA_TEMP_FIRST to
// FLAG_AREA_TEMP_LAST when the party enters another area, and other areas use
// the numbers for their own.
/// Bane's room: the pillar on the raised platform in the south-west has been
/// pushed off it and stands lower down.
#define FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED FLAG_AREA_TEMP_00

/// How far, in pixels, the first statue follows Isaac south as it shoves him
/// back, and how far the second shoves him; the two swapped from patch 1.
#if TBS_PATCH_1
#define CROSSBONE_PURSUER_STEP_Z 16
#define CROSSBONE_LEADER_STEP_Z 24
#else
#define CROSSBONE_PURSUER_STEP_Z 24
#define CROSSBONE_LEADER_STEP_Z 16
#endif

static void CrossboneIsleKeys_PushFirstPillar(void);
static void CrossboneIsleKeys_FollowSecondPillar(void);
static void CrossboneIsleKeys_PositionSecondPillarMarker(void);
static void CrossboneIsleKeys_InitializeKeys(void);
static void CrossboneIsleKeys_InitializeStatues(void);
static void CrossboneIsleKeys_InitializeBane(void);

/// The room of keys' placed actors, numbered in the order of
/// gCrossboneIsleKeysRoomActorPlacements.
///
/// They are the keys, each drawn as its item's icon on top of a stone post
/// (Colosso_ShowItemIcon) until Catch takes it.
enum CrossboneIsleKeysActorId {
  ACTOR_FIRST_BLUE_KEY = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_BLUE_KEY,
  ACTOR_THIRD_BLUE_KEY,
  ACTOR_FOURTH_BLUE_KEY,
  /// Shown only while Reveal lasts.
  ACTOR_RED_KEY
};

/// The statue room's placed actors, numbered in the order of
/// gCrossboneIsleStatuesActorPlacements.
///
/// They are the five statues of a robed woman, all of one sprite, each
/// waiting at its home in a circle of light.
enum CrossboneIsleStatuesActorId {
  /// In the north-east, beside the chest with the Storm Gear.
  ACTOR_FIRST_STATUE = ACTOR_FIRST_PLACED,
  /// On the narrow walk between two dark pits in the west.
  ACTOR_SECOND_STATUE,
  /// In the west alcove of the south walkway, barring the way north to the
  /// chest with the Sleep Bomb.
  ACTOR_THIRD_STATUE,
  /// In the east alcove of the south walkway.
  ACTOR_FOURTH_STATUE,
  /// In the middle of the hall, south of the chest with 444 coins.
  ACTOR_FIFTH_STATUE
};

/// The events the statue room's per-frame checks raise when Isaac steps onto
/// the tiles in front of the first, second or fifth statue.
///
/// The third and fourth statues answer cells of their own instead
/// (CrossboneIsleStatuesCellBehavior).
enum CrossboneIsleStatuesRaisedTrigger {
  RAISED_FIRST_CHASE = 91,
  RAISED_SECOND_CHASE = 92,
  RAISED_FIFTH_CHASE = 96
};

/// Bane's room's placed actors, numbered in the order of
/// gCrossboneIsleBaneActorPlacements.
enum CrossboneIsleBaneActorId {
  /// The five stone pillars, in Goma Cave's pillar sprite, pushed by walking
  /// into them or with Move. The first stands on the low ledge in the
  /// north-west, below where Bane first waits, at the top of a short ladder.
  ACTOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  /// In the channel between two rows of spikes in the middle of the room,
  /// with the Demon Axe's chest on top.
  ACTOR_SECOND_PILLAR,
  /// On the raised platform in the south-west.
  ACTOR_THIRD_PILLAR,
  /// The west one of the two side by side in the south-east, by the east
  /// doorway.
  ACTOR_FOURTH_PILLAR,
  /// The east one of the two.
  ACTOR_FIFTH_PILLAR,
  /// The puddle between two spikes in the south-west, frozen by Frost into a
  /// pillar of ice.
  ACTOR_PUDDLE,
  /// The chest on top of the second pillar, holding the Demon Axe.
  ///
  /// It is drawn in the sprite of McCoy's Palace's chests. It rides on the
  /// pillar until the pillar reaches the top of its channel, by the ladders,
  /// where the chest's examine trigger, TREASURE_SPOT_DEMON_AXE, takes its
  /// place. It is drawn open once FLAG_CROSSBONE_ISLE_DEMON_AXE is set.
  ACTOR_SECOND_PILLAR_CHEST,
  /// Bane, a Venus Djinni, until it is caught.
  ACTOR_BANE
};

/// The room of keys' cells that run its events, from
/// crossbone_isle_keys_and_bane's map.txt.
enum CrossboneIsleKeysCellBehavior {
  /// In front of the red door, cell (6, 52).
  CELL_BEHAVIOR_KEYS_RED_DOOR = 50,
  /// In front of each blue door on the walkway, west to east: cells (13, 43),
  /// (17, 43), (21, 43) and (25, 43).
  CELL_BEHAVIOR_KEYS_FIRST_BLUE_DOOR = 51,
  CELL_BEHAVIOR_KEYS_SECOND_BLUE_DOOR = 52,
  CELL_BEHAVIOR_KEYS_THIRD_BLUE_DOOR = 53,
  CELL_BEHAVIOR_KEYS_FOURTH_BLUE_DOOR = 54
};

/// The statue room's cells that run the third and fourth statues' events,
/// from crossbone_isle_statues's map.txt.
enum CrossboneIsleStatuesCellBehavior {
  /// The rug in front of the third statue's alcove, cells (15..17, 23).
  CELL_BEHAVIOR_STATUES_BEFORE_THIRD_STATUE = 53,
  /// The rug in front of the fourth statue's alcove, cells (20..22, 23).
  CELL_BEHAVIOR_STATUES_BEFORE_FOURTH_STATUE = 54,
  /// The third statue's alcove, cells (15..17, 22).
  CELL_BEHAVIOR_STATUES_THIRD_ALCOVE = 61,
  /// The fourth statue's alcove, cells (20..22, 22).
  CELL_BEHAVIOR_STATUES_FOURTH_ALCOVE = 62
};

/// Bane's room's cells that run its events, from
/// crossbone_isle_keys_and_bane's map.txt (the room is the east half of the
/// map the room of keys shares).
enum CrossboneIsleBaneCellBehavior {
  /// On the back ledge, cell (40, 41).
  CELL_BEHAVIOR_BANE_FIRST_ESCAPE = 20,
  /// In the passage down the west side, cell (36, 46).
  CELL_BEHAVIOR_BANE_SECOND_ESCAPE = 21,
  /// On the back ledge towards its east end, cell (51, 41).
  CELL_BEHAVIOR_BANE_LAST_ESCAPE = 22,
  /// The first of the twelve cells round the three spikes on the south-east
  /// ledge, (50..55, 51..54), one for each row of
  /// gCrossboneIsleKeys_BaneWalkTargets.
  CELL_BEHAVIOR_BANE_CORNER_FIRST = 30,
  /// The south-west platform and the floor below it, where the third pillar
  /// is pushed.
  CELL_BEHAVIOR_BANE_THIRD_PILLAR = 50,
  /// The first pillar's cell and those it is pushed through: the low ledge
  /// in the north-west, (36..41, 42), and the short ladder at (38, 41..43).
  CELL_BEHAVIOR_BANE_FIRST_PILLAR = 51,
  /// The spiked channel the second pillar is pushed up, (44..46, 43..49).
  CELL_BEHAVIOR_BANE_SECOND_PILLAR = 52,
  /// The row the fourth and fifth pillars are pushed along, (48..52, 55).
  CELL_BEHAVIOR_BANE_FOURTH_PILLAR = 53,
  /// The south edge of the south-west platform, cell (38, 54).
  CELL_BEHAVIOR_BANE_THIRD_PILLAR_EDGE = 54,
  /// The foot and top of the two ladders down from the back ledge, either
  /// side of the spiked channel: (42, 41..42) and (48, 41..42).
  CELL_BEHAVIOR_BANE_LADDER = 60,
  /// West of the first pillar's home, cell (37, 42).
  CELL_BEHAVIOR_BANE_WEST_OF_FIRST_PILLAR = 61,
  /// East of the first pillar's home, cell (39, 42).
  CELL_BEHAVIOR_BANE_EAST_OF_FIRST_PILLAR = 70
};

/// What CrossboneIsleKeys_OpenBlueDoor is given: the door's number in the low
/// byte, and PASSAGE_PLAY_OPENING to play its opening.
enum { PASSAGE_INDEX_MASK = 0xff, PASSAGE_PLAY_OPENING = 0x100 };

/// The Demon Axe chest's examine trigger, a treasure spot the code places
/// where the second pillar stands.
#define TREASURE_SPOT_DEMON_AXE (TREASURE_SPOT_FIRST_ID + 7)

/// Bane's room: Frost has frozen the puddle into a pillar of ice.
#define FLAG_CROSSBONE_ISLE_BANE_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
/// Bane's room: the chest on the second pillar has reached the top of the
/// spiked channel, by the ladders.
#define FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER FLAG_SCENE_TEMP_06
/// Bane's room: Isaac last left a ladder by hopping east off it, rather than
/// climbing up it. While it is set, the chest's cell at the top of the spiked
/// channel, (45, 43), answers the Demon Axe's examine trigger.
#define FLAG_CROSSBONE_ISLE_BANE_JUMPED_EAST_FROM_LADDER FLAG_SCENE_TEMP_07
// The Move events of Bane's room's pillars also test FLAG_SCENE_TEMP_01 and
// 03 to 05, which nothing sets.
/// The statue room: a statue is after Isaac.
#define FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER FLAG_SCENE_TEMP_20

/// Raises an event for the trigger table to run on the next frame.
static inline void MapLocalState_StoreStep(s16 *field, s32 step) {
  *field = step;
}

/// Stands the first pillar on the low ledge in the north-west, in the given
/// column, and makes its top ground level with the back ledge, so that Isaac
/// can walk onto it from there.
static inline void LiftPillar(s32 column, u8 motionFlags) {
  MapGrid_CopyCellAttributes(61, 36, 1, 1, column, 42);
  Actor_Get(ACTOR_FIRST_PILLAR)->motionFlags = motionFlags;
  Actor_Get(ACTOR_FIRST_PILLAR)->y.fixed = toFIXED(32);
}

#include "shared/staged_actor.c"

/// Talking to Bane on the spiked ledge in the south-east of Bane's room while
/// Halt holds it still: Bane, a Venus Djinni, joins the party.
static void CrossboneIsleKeys_BaneJoins(void) {
  Djinn_JoinParty(ACTOR_BANE, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_BANE));
}

/// Unused: an empty function nothing refers to.
UNUSED static void CrossboneIsleKeys_UnusedNoOpA(void) {}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 selector = gMainRoutineWork.next;

  if (selector == SCENE_CROSSBONE_ISLE_KEYS) {
    return gCrossboneIsleKeysRoomEntrances;
  }
  if (selector == SCENE_CROSSBONE_ISLE_STATUES) {
    return gCrossboneIsleStatuesEntrances;
  }
  if (selector == SCENE_CROSSBONE_ISLE_BANE) {
    return gCrossboneIsleBaneEntrances;
  }
  return gScene_FallbackEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gCrossboneIsleKeysExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 selector = gMainRoutineWork.next;

  if (selector == SCENE_CROSSBONE_ISLE_KEYS) {
    return gCrossboneIsleKeysRoomActorPlacements;
  }
  if (selector == SCENE_CROSSBONE_ISLE_STATUES) {
    return gCrossboneIsleStatuesActorPlacements;
  }
  if (selector == SCENE_CROSSBONE_ISLE_BANE) {
    return gCrossboneIsleBaneActorPlacements;
  }
  return gCrossboneIsleKeysActorPlacementsOther;
}

/// Halt on one of the five statues in the statue room: the statue stops
/// where it is, wrapped in Halt's light, and while it lasts Isaac can walk
/// past it.
///
/// The event itself does nothing: Halt's own effect holds the statue, and
/// each statue's chase checks gMainRoutineWork.haltedActor before it moves.
static void CrossboneIsleKeys_HaltPursuer(void) {}

/// Keeps Bane turned toward Isaac on the spiked ledge, every frame.
static UNREAD_RESULT
CrossboneIsleKeys_FaceBaneTowardLeader(struct Actor *actor) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  actor->facing = Trig_Atan(leader->z.fixed - actor->z.fixed,
                            leader->x.fixed - actor->x.fixed);
  RETURN_UNREAD(0);
}

/// Bane runs off the first time Isaac comes along the back ledge towards it,
/// at cell (40, 41).
///
/// The camera pans over to Bane at the ledge's west end. It turns with a
/// start and hops down off the back ledge onto the low ledge below, beside the
/// first pillar, then turns to face south.
static void CrossboneIsleKeys_BaneFirstEscape(void) {
  if (Event_GetFlag(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) == 0) {
    GameFlag_Set(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE);
    Event_Start();
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveToActorById(ACTOR_BANE, 1);
    FieldCamera_WaitForMove();
    Actor_FaceDirection(ACTOR_BANE, FACING_EAST, 20);
    Actor_SetAttachedEffect(ACTOR_BANE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_BANE, 2);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_BANE, 1);
    Sound_Play(SE_JUMP);
    Actor_Get(ACTOR_BANE)->velocityY = toFIXED(8);
    Actor_WalkToAndWait(ACTOR_BANE, 584, 680);
    Actor_FaceDirection(ACTOR_BANE, FACING_SOUTH, 20);
    Event_End();
  }
}

/// Bane runs off again when Isaac comes up the passage down the west side of
/// the room towards it, at cell (36, 46).
///
/// The camera pans to Bane on the low ledge. It hops back up onto the back
/// ledge and dashes east along it to the far end. It lands there with a
/// thud that shakes the room (SE_BANE_ESCAPE) and is left tipped over,
/// squashing and stretching over and over
/// (gCrossboneIsleKeys_BaneSquashScript).
static void CrossboneIsleKeys_BaneSecondEscape(void) {
  struct Actor *spinningDjinni;

  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) != 0) {
    if (Event_GetFlag(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) == 0) {
      GameFlag_Set(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE);
      Event_Start();

      // Focus on the Djinni before its next jump away.
      FieldCamera_SetSpeed(2);
      FieldCamera_MoveToActorById(ACTOR_BANE, 1);
      FieldCamera_WaitForMove();
      Actor_FaceDirection(ACTOR_BANE, FACING_SOUTH, 20);
      Actor_SetAttachedEffect(ACTOR_BANE,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_RunRepeatedMotion(ACTOR_BANE, 2);
      Event_Wait(20);
      Actor_SetSpeed(ACTOR_BANE, 1);
      Sound_Play(SE_JUMP);
      Actor_Get(ACTOR_BANE)->velocityY = toFIXED(10);
      Actor_WalkToAndWait(ACTOR_BANE, 584, 664);
      Actor_FaceDirection(ACTOR_BANE, FACING_SOUTH, 20);
      Actor_SetAttachedEffect(ACTOR_BANE,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_Wait(30);

      // Dash across the platforms, then shake the room on landing.
      Actor_SetSpeedAndAcceleration(ACTOR_BANE, 8, 0.25);
      Actor_WalkToAndWait(ACTOR_BANE, 664, 664);
      Actor_WalkToAndWait(ACTOR_BANE, 744, 664);
      Actor_WalkToAndWait(ACTOR_BANE, 824, 664);
      Event_Wait(10);
      Sound_Play(SE_BANE_ESCAPE);
      MapGrid_SetShake(4, 2, 1);
      Event_Wait(20);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      Event_Wait(30);

      Actor_SetPosition(ACTOR_BANE, 888, 664);
      spinningDjinni = Actor_Get(ACTOR_BANE);
      spinningDjinni->sprite->rotation = TRIG_TURN * 31 / 32;
      spinningDjinni->facing = FACING_EAST;
      Actor_SetAnimationSpeed(spinningDjinni, 0);
      Actor_StartScript(spinningDjinni, gCrossboneIsleKeys_BaneSquashScript);
      Event_End();
    }
  }
}

/// Bane flees a third time when Isaac comes east along the back ledge to it,
/// at cell (51, 41), and this time it runs to the ledge with three spikes in
/// the south-east.
///
/// It rights itself, hops down off the east end of the back ledge and
/// scurries south down the east side out of sight, wobbling from side to
/// side, to its last spot north of the spikes. From then on it keeps
/// turning to face Isaac (CrossboneIsleKeys_FaceBaneTowardLeader).
static void CrossboneIsleKeys_BaneLastEscape(void) {
  struct Actor *djinni;

  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) != 0 &&
      Event_GetFlag(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) == 0) {
    GameFlag_Set(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE);
    Event_Start();

    djinni = Actor_Get(ACTOR_BANE);
    djinni->sprite->rotation = 0;
    Actor_SetAnimationSpeed(djinni, ANIM_STEP_NORMAL);
    Sound_Play(SE_JUMP);
    Actor_Get(ACTOR_BANE)->velocityY = toFIXED(8);
    Actor_FaceDirection(ACTOR_BANE, FACING_WEST, 30);
    Actor_SetAttachedEffect(ACTOR_BANE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_BANE, 2);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_BANE, 1);
    Sound_Play(SE_JUMP);
    Actor_Get(ACTOR_BANE)->velocityY = toFIXED(4);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 680);
    Event_Wait(10);
    Actor_SetAttachedEffect(ACTOR_BANE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Actor_SetSpeed(ACTOR_BANE, 2);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 696);
    Actor_WalkToAndWait(ACTOR_BANE, 882, 704);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 712);
    Actor_WalkToAndWait(ACTOR_BANE, 878, 720);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 728);
    Actor_WalkToAndWait(ACTOR_BANE, 882, 736);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 744);
    Actor_WalkToAndWait(ACTOR_BANE, 878, 752);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 760);
    Actor_SetPosition(ACTOR_BANE, 856, 824);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_BANE, FACING_NORTH, 20);
    Actor_SetAttachedEffect(ACTOR_BANE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    Actor_Get(ACTOR_BANE)->update =
        (void (*)(struct Actor *))CrossboneIsleKeys_FaceBaneTowardLeader;
    Event_End();
  }
}

/// Bane keeps the three spikes on the south-east ledge between itself and
/// Isaac: whenever he steps onto one of the twelve cells round them, cells
/// (50..55, 51..54), it runs as fast as he walks to the spot across from
/// him, unless Halt holds it.
///
/// Each cell's behaviour, less CELL_BEHAVIOR_BANE_CORNER_FIRST, picks Bane's
/// spot from gCrossboneIsleKeys_BaneWalkTargets.
static void CrossboneIsleKeys_BaneKeepsAway(void) {
  struct Actor *djinni;
  s32 target;
  struct FieldWork *work;

  work = gFieldWork;
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) != 0) {
    if (gMainRoutineWork.haltedActor != ACTOR_BANE) {
      target = work->stepTrigger;
      djinni = Actor_Get(ACTOR_BANE);
      djinni->speedLimit = Actor_Get(ACTOR_PARTY_LEADER)->speedLimit;
      djinni = Actor_Get(ACTOR_BANE);
      djinni->acceleration = Actor_Get(ACTOR_PARTY_LEADER)->speedLimit;
      target -= CELL_BEHAVIOR_BANE_CORNER_FIRST;
      Actor_WalkTo(ACTOR_BANE, gCrossboneIsleKeys_BaneWalkTargets[target][0],
                   gCrossboneIsleKeys_BaneWalkTargets[target][1]);
    }
  }
}

/// Halt on Bane in Bane's room: it stops still where it is, and Isaac can
/// walk up and talk to it.
static void CrossboneIsleKeys_HaltBane(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_BANE, ANIM_STILL);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 selector = gMainRoutineWork.next;

  if (selector == SCENE_CROSSBONE_ISLE_KEYS) {
    return gCrossboneIsleKeysRoomTriggers;
  }
  if (selector == SCENE_CROSSBONE_ISLE_STATUES) {
    return gCrossboneIsleStatuesTriggers;
  }
  if (selector == SCENE_CROSSBONE_ISLE_BANE) {
    return gCrossboneIsleBaneTriggers;
  }
  return gCrossboneIsleKeysTriggersOther;
}

/// Unused: pushing south on the walkway in front of the third or fourth blue
/// door in the room of keys would hop Isaac down onto the stone post two
/// cells south, at cell (21, 45) or (25, 45).
///
/// Its trigger rows come after the plain step rows for the same cells, which
/// answer first and make the same hop themselves
/// (CrossboneIsleKeys_OpenThirdBlueDoor).
static void CrossboneIsleKeys_UnusedJumpOntoPost(void) {
  Field_TryJumpForward();
}

/// Opens a blue door in the room of keys, numbered 0 to 3 from the west, so
/// that the doorway leads through to its chest.
///
/// With PASSAGE_PLAY_OPENING the door grinds open as the floor shakes, a
/// half-open picture first; without it, as when the party comes back in, the
/// door is simply drawn open.
static void CrossboneIsleKeys_OpenBlueDoor(s32 passageInfo) {
  s32 columnOffset = (passageInfo & PASSAGE_INDEX_MASK) * 4;
  s32 cellColumn = columnOffset + 77;
  s32 attributeColumn = columnOffset + 13;

  if (passageInfo & PASSAGE_PLAY_OPENING) {
    Sound_Play(SE_GATE_MOVE);
    MapGrid_SetShake(2, 2, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    MapGrid_CopyCells(79, 29, 1, 3, cellColumn, 40);
    wait(40);
  }
  MapGrid_CopyCells(80, 29, 1, 3, cellColumn, 40);
  MapGrid_CopyCellAttributes(attributeColumn, 40, 1, 1, attributeColumn, 41);
  MapGrid_CopyCellAttributes(attributeColumn, 40, 1, 1, attributeColumn, 42);
}

/// Opens the red door on the ledge west of the pit, playing its grinding
/// opening with PASSAGE_PLAY_OPENING as a blue door's.
static void CrossboneIsleKeys_OpenRedDoor(s32 passageInfo) {
  if ((passageInfo & PASSAGE_PLAY_OPENING) != 0) {
    Sound_Play(SE_GATE_MOVE);
    MapGrid_SetShake(2, 2, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    MapGrid_CopyCells(84, 29, 1, 3, 70, 49);
    wait(60);
  }
  MapGrid_CopyCells(85, 29, 1, 3, 70, 49);
  MapGrid_CopyCellAttributes(6, 49, 1, 1, 6, 50);
  MapGrid_CopyCellAttributes(6, 49, 1, 1, 6, 51);
}

/// Walking up to the red door on the ledge west of the pit with the Red Key,
/// at cell (6, 52), opens it: it grinds open as the floor shakes, and the key
/// is used up.
///
/// Without the key, or facing any other way, nothing happens. Through the
/// door the way leads on to the floor's west door.
static void CrossboneIsleKeys_UseRedKey(void) {
  if (Actor_Get(ACTOR_PARTY_LEADER)->facing == FACING_NORTH) {
    if (Event_GetFlag(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN) == 0) {
      if (PartyInventory_FindMember(ITEM_RED_KEY) != -1) {
        GameFlag_Set(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN);
        CrossboneIsleKeys_OpenRedDoor(PASSAGE_PLAY_OPENING);
        PartyInventory_Remove(ITEM_RED_KEY);
      }
    }
  }
}

/// Walking up to a blue door with a Blue Key, numbered 0 to 3 from the west,
/// opens it: it grinds open as the floor shakes, and one Blue Key is used up.
///
/// Without a key, facing any other way, or once the door is open, nothing
/// happens.
static void CrossboneIsleKeys_UseBlueKey(s32 doorIndex) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 flag;

  if (leader->facing != FACING_NORTH) {
    return;
  }
  flag = doorIndex + FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN;
  if (GameFlag_Get(flag) != 0) {
    return;
  }
  if (PartyInventory_FindMember(ITEM_BLUE_KEY) == -1) {
    return;
  }
  GameFlag_Set(flag);
  CrossboneIsleKeys_OpenBlueDoor(PASSAGE_PLAY_OPENING | doorIndex);
  PartyInventory_Remove(ITEM_BLUE_KEY);
}

/// Walking up to the westmost blue door on the walkway along the pit's north
/// wall, at cell (13, 43), opens it with a Blue Key, showing the chest with
/// 333 coins behind it.
static void CrossboneIsleKeys_OpenFirstBlueDoor(void) {
  CrossboneIsleKeys_UseBlueKey(0);
}

/// Walking up to the second blue door from the west on the walkway, at cell
/// (17, 43), opens it with a Blue Key, showing the chest with the Fairy Ring
/// behind it.
static void CrossboneIsleKeys_OpenSecondBlueDoor(void) {
  CrossboneIsleKeys_UseBlueKey(1);
}

/// Walking up to the third blue door from the west on the walkway, at cell
/// (21, 43), opens it with a Blue Key, showing the chest with the Cookie
/// behind it.
///
/// Pushing south from the same spot hops Isaac down off the walkway onto the
/// stone post two cells south, at cell (21, 45).
static void CrossboneIsleKeys_OpenThirdBlueDoor(void) {
  s16 *blockedSteps;
  s32 resetSteps = 0;

  CrossboneIsleKeys_UseBlueKey(2);
  if (Actor_Get(ACTOR_PARTY_LEADER)->facing == FACING_SOUTH) {
    blockedSteps = &gFieldWork->blockedSteps;
    if (*blockedSteps > FIELD_PUSH_FRAMES) {
      Field_TryJumpForward();
      *blockedSteps = resetSteps;
    }
  }
}

/// Walking up to the eastmost blue door on the walkway, at cell (25, 43),
/// opens it with a Blue Key, showing the chest with the Smoke Bomb behind it.
///
/// Pushing south from the same spot hops Isaac down off the walkway onto the
/// stone post two cells south, at cell (25, 45), the way to where the Red
/// Key is hidden.
static void CrossboneIsleKeys_OpenFourthBlueDoor(void) {
  s16 *blockedSteps;
  s32 resetSteps = 0;

  CrossboneIsleKeys_UseBlueKey(3);
  if (Actor_Get(ACTOR_PARTY_LEADER)->facing == FACING_SOUTH) {
    blockedSteps = &gFieldWork->blockedSteps;
    if (*blockedSteps > FIELD_PUSH_FRAMES) {
      Field_TryJumpForward();
      *blockedSteps = resetSteps;
    }
  }
}

/// Reveal in the room of keys shows a hidden stone post, with the Red Key on
/// top, three cells south of the post below the fourth blue door, at cell
/// (25, 48).
///
/// The post is copied there, picture and cells, from the one at (25, 45),
/// and the key is put on it unless the party already has it. It appears
/// wherever in the room Reveal is cast, but is only seen when Isaac is near
/// it, as from the post below the fourth blue door.
static void CrossboneIsleKeys_ShowRedKey(void) {
  MapGrid_CopyCells(25, 45, 1, 2, 25, 48);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_RED_KEY) == 0)
    Actor_SetPosition(ACTOR_RED_KEY, 408, 776);
  Event_Wait(1);
}

/// When Reveal wears off in the room of keys, the hidden post south of the
/// fourth blue door vanishes again, at cell (25, 48), and so does the Red
/// Key if it is still on it.
static void CrossboneIsleKeys_HideRedKey(void) {
  MapGrid_CopyCells(24, 48, 1, 2, 25, 48);
  Actor_SetPosition(ACTOR_RED_KEY, 8, 8);
}

/// Gives the party a key that Catch has pulled in: "Isaac got the Blue Key."
/// (or the Red Key), and the key's icon is gone from its post for good.
static inline void CrossboneIsleKeys_CollectKey(s32 actorId, s32 itemId,
                                                s32 collectedFlag) {
  Item_ShowFound(itemId, 3);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Party_GiveItem(itemId, 0);
  Actor_SetPosition(actorId, 0, 0);
  GameFlag_Set(collectedFlag);
}

/// Catch on the Blue Key on the post just north of where Isaac comes into
/// the pit from the east door: a hand of light reaches out and pulls the key
/// to him.
static void CrossboneIsleKeys_CatchFirstBlueKey(void) {
  CrossboneIsleKeys_CollectKey(ACTOR_FIRST_BLUE_KEY, ITEM_BLUE_KEY,
                               FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY);
}

/// Catch on the Blue Key on a post in the middle of the pit: a hand of light
/// pulls it to Isaac.
static void CrossboneIsleKeys_CatchSecondBlueKey(void) {
  CrossboneIsleKeys_CollectKey(ACTOR_SECOND_BLUE_KEY, ITEM_BLUE_KEY,
                               FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 1);
}

/// Catch on the Blue Key on a post in the north of the pit, below the
/// walkway: a hand of light pulls it to Isaac.
static void CrossboneIsleKeys_CatchThirdBlueKey(void) {
  CrossboneIsleKeys_CollectKey(ACTOR_THIRD_BLUE_KEY, ITEM_BLUE_KEY,
                               FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 2);
}

/// Catch on the Blue Key on a post in the south of the pit, east of the
/// middle: a hand of light pulls it to Isaac.
static void CrossboneIsleKeys_CatchFourthBlueKey(void) {
  CrossboneIsleKeys_CollectKey(ACTOR_FOURTH_BLUE_KEY, ITEM_BLUE_KEY,
                               FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 3);
}

/// Catch on the Red Key, on the hidden post Reveal shows south of the
/// fourth blue door: a hand of light pulls it to Isaac.
static void CrossboneIsleKeys_CatchRedKey(void) {
  CrossboneIsleKeys_CollectKey(ACTOR_RED_KEY, ITEM_RED_KEY,
                               FLAG_CROSSBONE_ISLE_RED_KEY);
}

/// Watches, every frame, for Isaac on the tiles just south of the statue in
/// the north-east, cells (21..23, 10..11), and sets it on him unless Cloak
/// hides him, Halt holds the statue or another statue is already after him.
static void CrossboneIsleKeys_CheckFirstPursuerTrigger(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / toFIXED(16);
  s32 row = leader->z.fixed / toFIXED(16);
  struct FieldWork *work = gFieldWork;

  if (!Event_GetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER) &&
      gMainRoutineWork.cloaked == 0 &&
      gMainRoutineWork.haltedActor != ACTOR_FIRST_STATUE &&
      (column >= 21 && column <= 23) && row >= 10 && row <= 11) {
    GameFlag_Set(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
    work->raisedTrigger = RAISED_FIRST_CHASE;
  }
}

/// The statue in the north-east of the statue room glides down onto Isaac
/// when he steps onto the tiles in front of it, with a thud, shoves him back
/// south and glides home again.
///
/// CrossboneIsleKeys_CheckFirstPursuerTrigger raises it. Isaac is pushed 24
/// pixels south, and the statue follows him CROSSBONE_PURSUER_STEP_Z pixels
/// before it turns back.
static void CrossboneIsleKeys_FirstStatuePushesLeaderBack(void) {
  struct Actor *leader;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.9);
  Actor_SetSpeed(ACTOR_FIRST_STATUE, 1.9);
  Sound_Play(SE_THUMP);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_FIRST_STATUE, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_FIRST_STATUE);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
  Event_Wait(4);
  Sound_Play(SE_THUMP);
  Actor_SetDestinationOffset(ACTOR_FIRST_STATUE, 0, CROSSBONE_PURSUER_STEP_Z);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetDestination(ACTOR_FIRST_STATUE, 360, 152);
  Actor_WaitForMoveById(ACTOR_FIRST_STATUE);
  Event_End();
  GameFlag_Clear(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
}

/// Watches, every frame, for Isaac on the narrow walk below the statue in the
/// west, cells (10, 16..18), and sets it on him unless Cloak hides him, Halt
/// holds the statue or another statue is already after him.
static void CrossboneIsleKeys_CheckSecondPursuerTrigger(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / toFIXED(16);
  s32 row = leader->z.fixed / toFIXED(16);
  struct FieldWork *work = gFieldWork;

  if (Event_GetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER) == 0 &&
      gMainRoutineWork.cloaked == 0 &&
      gMainRoutineWork.haltedActor != ACTOR_SECOND_STATUE && column == 10 &&
      (row >= 16 && row <= 18)) {
    Event_SetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
    MapLocalState_StoreStep(&work->raisedTrigger, RAISED_SECOND_CHASE);
  }
}

/// The statue on the narrow walk between the dark pits in the west of the
/// statue room glides down onto Isaac as he comes north up the walk, with a
/// thud, shoves him back south and glides home again.
///
/// CrossboneIsleKeys_CheckSecondPursuerTrigger raises it.
static void CrossboneIsleKeys_SecondStatuePushesLeaderBack(void) {
  struct Actor *leader;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.7);
  Actor_SetSpeed(ACTOR_SECOND_STATUE, 1.7);
  Sound_Play(SE_THUMP);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_SECOND_STATUE, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_SECOND_STATUE);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
  Sound_Play(SE_THUMP);
  Event_Wait(4);
  Actor_SetDestinationOffset(ACTOR_SECOND_STATUE, 0, 16);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetDestination(ACTOR_SECOND_STATUE, 168, 264);
  Actor_WaitForMoveById(ACTOR_SECOND_STATUE);
  Event_End();
  GameFlag_Clear(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
}

/// Slides the third or fourth statue (0 or 1) along its alcove with a thud
/// to stand right in front of Isaac's column, unless Halt holds it, and
/// nudges him 8 pixels south if he has got into the alcove's row.
///
/// gCrossboneIsleKeys_SlotColumns keeps the column each statue stands in.
static void CrossboneIsleKeys_TrackPursuerColumn(s32 pursuerIndex) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / toFIXED(16);
  s32 row = leader->z.fixed / toFIXED(16);
  s32 pursuerId = pursuerIndex + ACTOR_THIRD_STATUE;

  if (gMainRoutineWork.haltedActor == pursuerId)
    return;
  if (column == gCrossboneIsleKeys_SlotColumns[pursuerIndex])
    return;

  Actor_SetSpeed(pursuerId, 4.5);
  Sound_Play(SE_THUMP);
  Actor_SetDestination(pursuerId, (column << 4) + 8, 360);

  gCrossboneIsleKeys_SlotColumns[pursuerIndex] = column;

  if (row <= 22) {
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 8);
  }
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
}

/// As Isaac walks along the rug in front of the third statue, in the west
/// alcove of the statue room's south walkway, cells (15..17, 23), the statue
/// slides sideways to stay right in front of him, barring the way north to
/// the chest with the Sleep Bomb.
///
/// Halt on the statue while it stands to one side lets him walk past.
static void CrossboneIsleKeys_ThirdStatueShadowsLeader(void) {
  CrossboneIsleKeys_TrackPursuerColumn(0);
}

/// As Isaac walks along the rug in front of the fourth statue, in the east
/// alcove of the statue room's south walkway, cells (20..22, 23), the statue
/// slides sideways to stay right in front of him and bar the way north.
///
/// Halt on the statue while it stands to one side lets him walk past.
static void CrossboneIsleKeys_FourthStatueShadowsLeader(void) {
  CrossboneIsleKeys_TrackPursuerColumn(1);
}

/// Unused: would make the fifth statue slide to stay in front of Isaac as
/// the third and fourth do; nothing calls it.
UNUSED static void CrossboneIsleKeys_TrackFifthPursuerColumn(void) {
  CrossboneIsleKeys_TrackPursuerColumn(2);
}

/// Stepping into the third statue's alcove on the south walkway, cells
/// (15..17, 22), while Halt does not hold the statue, makes it glide onto
/// Isaac with a thud, shove him back south and glide home again.
static void CrossboneIsleKeys_ThirdStatuePushesLeaderBack(void) {
  struct Actor *leader;

  if (gMainRoutineWork.haltedActor != ACTOR_THIRD_STATUE) {
    Event_Start();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.7);
    Actor_SetSpeed(ACTOR_THIRD_STATUE, 1.7);
    Sound_Play(SE_THUMP);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetDestination(ACTOR_THIRD_STATUE, leader->x.part[0].integer,
                           leader->z.part[0].integer);
    }
    Actor_WaitForMoveById(ACTOR_THIRD_STATUE);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
    Event_Wait(4);
    Sound_Play(SE_THUMP);
    Actor_SetDestinationOffset(ACTOR_THIRD_STATUE, 0, 16);
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    Actor_SetDestination(ACTOR_THIRD_STATUE, 264, 360);
    Actor_WaitForMoveById(ACTOR_THIRD_STATUE);
    Event_Wait(10);
    Event_End();
  }
}

/// Stepping into the fourth statue's alcove on the south walkway, cells
/// (20..22, 22), while Halt does not hold the statue, makes it shuffle half
/// a cell towards Isaac, glide onto him with a thud, shove him back south
/// and glide home again.
static void CrossboneIsleKeys_FourthStatuePushesLeaderBack(void) {
  struct Actor *leader;

  if (gMainRoutineWork.haltedActor != ACTOR_FOURTH_STATUE) {
    Event_Start();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.7);
    Actor_SetSpeed(ACTOR_FOURTH_STATUE, 1.7);
    Sound_Play(SE_THUMP);
    if (Actor_Get(ACTOR_PARTY_LEADER)->x.fixed / toFIXED(16) >
        Actor_Get(ACTOR_FOURTH_STATUE)->x.fixed / toFIXED(16)) {
      Actor_SetDestinationOffset(ACTOR_FOURTH_STATUE, 8, 0);
    }
    if (Actor_Get(ACTOR_PARTY_LEADER)->x.fixed / toFIXED(16) <
        Actor_Get(ACTOR_FOURTH_STATUE)->x.fixed / toFIXED(16)) {
      Actor_SetDestinationOffset(ACTOR_FOURTH_STATUE, -8, 0);
    }
    Actor_WaitForMoveById(ACTOR_FOURTH_STATUE);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetDestination(ACTOR_FOURTH_STATUE, leader->x.part[0].integer,
                           leader->z.part[0].integer);
    }
    Actor_WaitForMoveById(ACTOR_FOURTH_STATUE);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
    Event_Wait(4);
    Sound_Play(SE_THUMP);
    Actor_SetDestinationOffset(ACTOR_FOURTH_STATUE, 0, 16);
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    Actor_SetDestination(ACTOR_FOURTH_STATUE, 344, 360);
    Actor_WaitForMoveById(ACTOR_FOURTH_STATUE);
    Event_Wait(10);
    Event_End();
  }
}

/// Watches, every frame, for Isaac on the two tiles just south of the statue
/// in the middle of the hall, cells (19, 15..16), and sets it on him unless
/// Cloak hides him, Halt holds the statue or another statue is already after
/// him.
static void CrossboneIsleKeys_CheckFifthPursuerTrigger(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / toFIXED(16);
  s32 row = leader->z.fixed / toFIXED(16);
  struct FieldWork *work = gFieldWork;
  struct MainRoutineWork *state = &gMainRoutineWork;

  if (state->haltedActor != ACTOR_FIFTH_STATUE &&
      Event_GetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER) == 0 &&
      state->cloaked == 0 && column == 19 && (row >= 15 && row <= 16)) {
    Event_SetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
    MapLocalState_StoreStep(&work->raisedTrigger, RAISED_FIFTH_CHASE);
  }
}

/// The statue in the middle of the statue room glides down onto Isaac when
/// he steps onto the tiles in front of it, with a thud, shoves him back
/// south and glides home again.
///
/// CrossboneIsleKeys_CheckFifthPursuerTrigger raises it.
static void CrossboneIsleKeys_FifthStatuePushesLeaderBack(void) {
  struct Actor *leader;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.7);
  Actor_SetSpeed(ACTOR_FIFTH_STATUE, 1.7);
  Sound_Play(SE_THUMP);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_FIFTH_STATUE, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_FIFTH_STATUE);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
  Sound_Play(SE_THUMP);
  Actor_SetDestinationOffset(ACTOR_FIFTH_STATUE, 0, 16);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetDestination(ACTOR_FIFTH_STATUE, 312, 232);
  Actor_WaitForMoveById(ACTOR_FIFTH_STATUE);
  Event_End();
  GameFlag_Clear(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
}

/// Reveal in the statue room lifts the darkness: while it lasts the whole
/// hall shows evenly in Reveal's grey, not only the circles of light round
/// the braziers.
///
/// The darkness is a background layer laid over the room, half see-through
/// (CrossboneIsleKeys_InitializeStatues). Reveal's blend counts that layer
/// for nothing and the room under it for 13 sixteenths, so the hall shows
/// whole and a little dim.
static void CrossboneIsleKeys_CastRevealInStatueRoom(void) {
  u16 value = BLEND_ALPHA(0, 13);

  REG_BLDALPHA = value;
}

/// When Reveal wears off in the statue room, the darkness falls over the hall
/// again outside the circles of light.
static void CrossboneIsleKeys_RevealEndsInStatueRoom(void) {
  u16 value = BLEND_ALPHA(7, 6);

  REG_BLDALPHA = value;
}

/// At either of the two ladders down from the back ledge of Bane's room, by
/// the spiked channel in the middle, cells (42, 41..42) and (48, 41..42),
/// Isaac climbs up with Up and down with Down, and with Right hops east off
/// the ladder.
///
/// Hopping east from the west ladder, over the wall beside it, lands him on the
/// little ledge at the top of the spiked channel, by the Demon Axe's chest once
/// the second pillar has brought it there. Pushing west from the foot of the
/// west ladder pushes the first pillar back west if it has been pushed up to
/// it; from higher up the ladder he hops west instead.
static void CrossboneIsleKeys_UseLadder(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  u16 facing = leader->facing;

  if (facing == FACING_NORTH) {
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER) != 0) {
      {
        s32 destinationColumn = 45;
        s32 destinationRow = 43;
        MapGrid_CopyCellAttributes(46, 43, 1, 1, destinationColumn,
                                   destinationRow);
      }
    }
    GameFlag_Clear(FLAG_CROSSBONE_ISLE_BANE_JUMPED_EAST_FROM_LADDER);
    Field_ClimbUp();
  } else if (facing == FACING_SOUTH) {
    Field_ClimbDown();
  } else if (facing == FACING_EAST) {
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER) != 0) {
      {
        s32 destinationColumn = 45;
        s32 destinationRow = 43;
        MapGrid_CopyCellAttributes(58, 36, 1, 1, destinationColumn,
                                   destinationRow);
      }
    }
    GameFlag_Set(FLAG_CROSSBONE_ISLE_BANE_JUMPED_EAST_FROM_LADDER);
    Field_TryJumpForward();
  } else if (facing == FACING_WEST) {
    if (leader->y.fixed == 0) {
      CrossboneIsleKeys_PushFirstPillar();
    } else {
      Field_TryJumpForward();
    }
  }
}

/// Unused: an empty function nothing refers to.
UNUSED static void CrossboneIsleKeys_UnusedNoOpB(void) {}

/// Frost on the puddle between the two spikes in the south-west of Bane's
/// room freezes it into a pillar of ice, which Isaac can hop onto and across
/// like a stepping stone, at cell (40, 55).
static void CrossboneIsleKeys_FreezePuddle(void) {
  struct Actor *puddle = Actor_Get(ACTOR_PUDDLE);

  MapGrid_CopyCellAttributes(40, 54, 1, 1, 40, 55);
  if (puddle != NULL) {
    u8 *motionFlags = &Actor_Get(ACTOR_PUDDLE)->motionFlags;
    u8 *priorityFlags = &puddle->priorityFlags;

    *motionFlags = 0;
    *priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_CROSSBONE_ISLE_BANE_PUDDLE_FROZEN);
}

/// Just east of the first pillar's home on the low ledge in the north-west
/// of Bane's room, at cell (39, 42), pushing east hops Isaac east off the
/// ledge; pushing west into the pillar pushes it.
static void CrossboneIsleKeys_JumpEastOrPushFirstPillar(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (leader->facing == FACING_EAST) {
    Field_TryJumpForward();
  } else {
    CrossboneIsleKeys_PushFirstPillar();
  }
}

/// Lays the low ledge in the north-west as it is with the first pillar gone,
/// before the pillar's own cell is laid where it stands.
static void CrossboneIsleKeys_RestoreFirstPillarTerrain(void) {
  MapGrid_CopyCellAttributes(57, 42, 1, 1, 40, 42);
  MapGrid_CopyCellAttributes(57, 42, 1, 1, 41, 42);
  MapGrid_CopyCellAttributes(58, 42, 1, 1, 42, 42);
  MapGrid_CopyCellAttributes(62, 37, 3, 1, 37, 42);

  Actor_Get(ACTOR_FIRST_PILLAR)->motionFlags = ACTOR_FOLLOWS_TERRAIN;
}

/// The first pillar stands in the gap's east cell, at the foot of the west
/// ladder, cell (42, 42): its top raises the ladder's foot to the low
/// ledge's height.
static void CrossboneIsleKeys_RestoreEastUnderfootCell(void) {
  MapGrid_CopyCellAttributes(58, 41, 1, 1, 42, 42);
  Actor_Get(ACTOR_FIRST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
}

/// The first pillar stands in the middle of the gap, cell (41, 42): its top
/// is a square Isaac can hop across.
static void CrossboneIsleKeys_RestoreMiddleUnderfootCell(void) {
  MapGrid_CopyCellAttributes(44, 42, 1, 1, 41, 42);
  Actor_Get(ACTOR_FIRST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
}

/// The first pillar stands in the gap's west cell, cell (40, 42), its top
/// level with the low ledge, which it lengthens by a cell.
static void CrossboneIsleKeys_RestoreWestUnderfootCell(void) {
  MapGrid_CopyCellAttributes(39, 42, 1, 1, 40, 42);
  Actor_Get(ACTOR_FIRST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
}

/// Move on the first pillar, on the low ledge in the north-west of Bane's
/// room, slides it along the ledge; the cells under it follow it.
///
/// Pushed east off the ledge, it drops into the gap between the ledge and
/// the west ladder, and its top makes a step there. On the ledge, at the top
/// of the short ladder at column 38, its top is level with the back ledge.
/// The pushes and the room's setting up call this too, to lay its cells.
static void CrossboneIsleKeys_MoveFirstPillar(void) {
  struct Actor *actor = Actor_Get(ACTOR_FIRST_PILLAR);
  FIXED x = actor->x.fixed;
  FIXED height = actor->y.fixed;
  s32 column = x / toFIXED(16);

  if (height == 0) {
    Actor_Get(ACTOR_FIRST_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  CrossboneIsleKeys_RestoreFirstPillarTerrain();
  Actor_Get(ACTOR_FIRST_PILLAR)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (column == 40) {
    CrossboneIsleKeys_RestoreWestUnderfootCell();
  } else if (column == 42) {
    CrossboneIsleKeys_RestoreEastUnderfootCell();
  } else if (column == 41) {
    CrossboneIsleKeys_RestoreMiddleUnderfootCell();
  } else if (column == 39) {
    LiftPillar(column, 0);
  } else if (column == 38) {
    LiftPillar(column, 0);
  } else if (column == 37) {
    LiftPillar(column, 0);
  }
}

/// Walking into the first pillar on the low ledge in the north-west of
/// Bane's room pushes it a cell along the ledge.
///
/// Once the pillar has left the top of the short ladder at column 38,
/// pushing north or south there climbs the ladder instead.
static void CrossboneIsleKeys_PushFirstPillar(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  struct Actor *pillar = Actor_Get(ACTOR_FIRST_PILLAR);
  s32 leaderColumn = leader->x.fixed / toFIXED(16);
  s32 pillarColumn = pillar->x.fixed / toFIXED(16);

  if (leaderColumn == 38 && pillarColumn != 38) {
    u16 facing = leader->facing;

    if (facing == FACING_NORTH) {
      Field_ClimbUp();
      return;
    }
    if (facing == FACING_SOUTH) {
      Field_ClimbDown();
      return;
    }
  }

  CrossboneIsleKeys_RestoreFirstPillarTerrain();
  PushedActor_AdvancePair();
  CrossboneIsleKeys_MoveFirstPillar();
}

/// Leaves the Demon Axe's chest on the second pillar at the top of the
/// spiked channel, where the chest's examine trigger takes its place.
static void CrossboneIsleKeys_ReleaseSecondPillarChest(void) {
  Task_Remove(CrossboneIsleKeys_FollowSecondPillar);
  Actor_SetPosition(ACTOR_SECOND_PILLAR_CHEST, 0, 0);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_JUMPED_EAST_FROM_LADDER) != 0) {
    MapGrid_CopyCellAttributes(58, 36, 1, 1, 45, 43);
  } else {
    MapGrid_CopyCellAttributes(46, 43, 1, 1, 45, 43);
  }
  CrossboneIsleKeys_PositionSecondPillarMarker();
  GameFlag_Set(FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER);
}

/// Move on the second pillar, in the spiked channel in the middle of Bane's
/// room, slides it with the Demon Axe's chest riding on top; once it reaches
/// the top of the channel by the ladders, cell (45, 43), it stays there and
/// the chest can be opened from beside it.
static void CrossboneIsleKeys_MoveSecondPillar(void) {
  struct Actor *pillar = Actor_Get(ACTOR_SECOND_PILLAR);
  s32 column = pillar->x.fixed / toFIXED(16);
  s32 row = pillar->z.fixed / toFIXED(16);

  if (column == 45 && row == 43) {
    CrossboneIsleKeys_ReleaseSecondPillarChest();
  }
}

/// Walking into the second pillar, at the foot of the spiked channel in the
/// middle of Bane's room, pushes it a cell north up the channel towards the
/// back of the room, carrying the Demon Axe's chest on top.
static void CrossboneIsleKeys_PushSecondPillar(void) {
  PushedActor_AdvancePair();
  CrossboneIsleKeys_MoveSecondPillar();
}

/// Lays the floor along the south of the south-west platform as it is with
/// no pillar at either end, cells (38, 55) and (42, 55).
static void CrossboneIsleKeys_RestoreThirdPillarTerrain(void) {
  MapGrid_CopyCellAttributes(38, 56, 1, 1, 38, 55);
  MapGrid_CopyCellAttributes(42, 56, 1, 1, 42, 55);
}

/// The third pillar has come down at the west end of the floor below its
/// platform, cell (38, 55): its top becomes a square Isaac can hop across.
static void CrossboneIsleKeys_RestoreThirdPillarWestCell(void) {
  MapGrid_CopyCellAttributes(40, 54, 1, 1, 38, 55);
  Actor_Get(ACTOR_THIRD_PILLAR)->motionFlags = 0;
}

/// The third pillar has come down at the east end of the floor below its
/// platform, cell (42, 55): its top becomes a square Isaac can hop across.
static void CrossboneIsleKeys_RestoreThirdPillarEastCell(void) {
  MapGrid_CopyCellAttributes(40, 54, 1, 1, 42, 55);
  Actor_Get(ACTOR_THIRD_PILLAR)->motionFlags = 0;
}

/// Move on the third pillar, on the raised platform in the south-west of
/// Bane's room, slides it; pushed off the platform's south edge it drops to
/// the floor below, and its top becomes a stepping stone beside the puddle.
///
/// The pushes and the room's setting up call this too, to lay its cells.
static void CrossboneIsleKeys_MoveThirdPillar(void) {
  struct Actor *pillar = Actor_Get(ACTOR_THIRD_PILLAR);
  s32 heightCells = pillar->y.fixed / toFIXED(16);
  s32 column = pillar->x.fixed / toFIXED(16);
  s32 row = pillar->z.fixed / toFIXED(16);

  if (!Event_GetFlag(FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED) &&
      heightCells <= 2) {
    CrossboneIsleKeys_RestoreThirdPillarWestCell();
    GameFlag_Set(FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED);
  }
  if (row == 55) {
    if (column == 42) {
      CrossboneIsleKeys_RestoreThirdPillarEastCell();
    }
    if (column == 38) {
      CrossboneIsleKeys_RestoreThirdPillarWestCell();
    }
  } else {
    CrossboneIsleKeys_RestoreThirdPillarTerrain();
  }
}

/// Walking into the third pillar, on the raised platform in the south-west
/// of Bane's room or on the floor below it, pushes it a cell.
static void CrossboneIsleKeys_PushThirdPillar(void) {
  CrossboneIsleKeys_RestoreThirdPillarTerrain();
  PushedActor_AdvancePair();
  CrossboneIsleKeys_MoveThirdPillar();
}

/// At the south edge of the raised platform in the south-west of Bane's room,
/// cell (38, 54), pushing south jumps Isaac down off the platform to the
/// floor below in a puff of dust; pushing any other way pushes the third
/// pillar.
static void CrossboneIsleKeys_TouchThirdPillar(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (leader->facing == FACING_SOUTH) {
    FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
  } else {
    CrossboneIsleKeys_PushThirdPillar();
  }
}

/// Lays the row the fourth and fifth pillars are pushed along, cells
/// (49..52, 55), as plain floor.
static void CrossboneIsleKeys_RestoreFourthAndFifthPillarTerrain(void) {
  MapGrid_CopyCellAttributes(48, 55, 1, 1, 49, 55);
  MapGrid_CopyCellAttributes(48, 55, 1, 1, 50, 55);
  MapGrid_CopyCellAttributes(48, 55, 1, 1, 51, 55);
  MapGrid_CopyCellAttributes(48, 55, 1, 1, 52, 55);
}

/// Makes the tops of the fourth and fifth pillars squares Isaac can hop
/// across, wherever along their row they stand.
static void CrossboneIsleKeys_RepaintFourthAndFifthPillarCells(void) {
  struct Actor *pillar;
  s32 column;

  CrossboneIsleKeys_RestoreFourthAndFifthPillarTerrain();

  pillar = Actor_Get(ACTOR_FOURTH_PILLAR);
  column = pillar->x.fixed / toFIXED(16);
  MapGrid_CopyCellAttributes(53, 55, 1, 1, column, 55);
  pillar = Actor_Get(ACTOR_FIFTH_PILLAR);
  column = pillar->x.fixed / toFIXED(16);
  MapGrid_CopyCellAttributes(53, 55, 1, 1, column, 55);
}

/// Move on the west one of the two pillars by the east doorway of Bane's
/// room slides it along the south row; its top stays a stepping stone.
static void CrossboneIsleKeys_MoveFourthPillar(void) {
  CrossboneIsleKeys_RepaintFourthAndFifthPillarCells();
}

/// Walking into either of the two pillars by the east doorway of Bane's room
/// pushes it along the south row, cells (48..52, 55), and its top stays a
/// stepping stone wherever it stops.
///
/// The push moves whichever pillar Isaac walks into
/// (PushedActor_AdvancePair).
static void CrossboneIsleKeys_PushFourthOrFifthPillar(void) {
  CrossboneIsleKeys_RestoreFourthAndFifthPillarTerrain();
  PushedActor_AdvancePair();
  CrossboneIsleKeys_MoveFourthPillar();
}

/// Move on the east one of the two pillars by the east doorway of Bane's
/// room slides it along the south row; its top stays a stepping stone.
static void CrossboneIsleKeys_MoveFifthPillar(void) {
  CrossboneIsleKeys_RepaintFourthAndFifthPillarCells();
}

/// Unused: the same push as CrossboneIsleKeys_PushFourthOrFifthPillar's,
/// written for the fifth pillar; nothing refers to it.
UNUSED static void CrossboneIsleKeys_PushFifthPillar(void) {
  CrossboneIsleKeys_RestoreFourthAndFifthPillarTerrain();
  PushedActor_AdvancePair();
  CrossboneIsleKeys_MoveFifthPillar();
}

/// Keeps the Demon Axe's chest on top of the second pillar as it moves,
/// every frame.
static void CrossboneIsleKeys_FollowSecondPillar(void) {
  struct Actor *target = Actor_Get(ACTOR_SECOND_PILLAR_CHEST);
  struct Actor *source = Actor_Get(ACTOR_SECOND_PILLAR);

  target->y.fixed = toFIXED(32);
  target->x.fixed = source->x.fixed;
  target->z.fixed = source->z.fixed + toFIXED(1);
}

/// Puts the Demon Axe chest's examine trigger where the second pillar
/// stands.
static void CrossboneIsleKeys_PositionSecondPillarMarker(void) {
  struct Actor *frame = Actor_Get(ACTOR_SECOND_PILLAR);

  TreasureSpot_SetActorPosition(TREASURE_SPOT_DEMON_AXE, frame->x.fixed,
                                frame->z.fixed + toFIXED(1));
}

/// Draws the pillars in front of Isaac or behind him, every frame, by how
/// high he stands and, for the third, how far south he is.
static void CrossboneIsleKeys_UpdatePillarPriorities(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  u8 priority;

  if (leader->y.fixed > toFIXED(16)) {
    priority = ACTOR_PRIORITY_UNDERFOOT;
    Actor_Get(ACTOR_FIRST_PILLAR)->priorityFlags = priority;
    if (Actor_Get(ACTOR_THIRD_PILLAR)->y.fixed == 0)
      Actor_Get(ACTOR_THIRD_PILLAR)->priorityFlags = priority;
    Actor_Get(ACTOR_FOURTH_PILLAR)->priorityFlags = priority;
  } else {
    if (Actor_Get(ACTOR_THIRD_PILLAR)->y.fixed == 0 &&
        Actor_Get(ACTOR_PARTY_LEADER)->z.fixed / toFIXED(16) > 56) {
      Actor_SetSpritePriority(ACTOR_THIRD_PILLAR, 3);
    } else {
      Actor_SetSpritePriority(ACTOR_THIRD_PILLAR, 1);
      Actor_Get(ACTOR_THIRD_PILLAR)->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    }
    priority = 0;
    Actor_Get(ACTOR_FOURTH_PILLAR)->priorityFlags = priority;
  }
  Actor_Get(ACTOR_FIFTH_PILLAR)->priorityFlags = priority;
}

/// Sets up whichever of the three rooms the party enters: the keys and doors
/// of the room of keys, the statues of the statue room, or the pillars and
/// Bane of Bane's room.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineWork.next == SCENE_CROSSBONE_ISLE_KEYS) {
    CrossboneIsleKeys_InitializeKeys();
  }
  if (gMainRoutineWork.next == SCENE_CROSSBONE_ISLE_STATUES) {
    CrossboneIsleKeys_InitializeStatues();
  }
  if (gMainRoutineWork.next == SCENE_CROSSBONE_ISLE_BANE) {
    CrossboneIsleKeys_InitializeBane();
  }
  return 0;
}

/// Unused: nothing adds it as a task.
///
/// Every 17 frames it would put uncaught Blue Keys back on their cells and
/// redraw the five key actors, one every 2 frames from the Red Key to the
/// first Blue Key. It would draw them with item codes 0 and 1 (a gem and the
/// Long Sword) rather than ITEM_BLUE_KEY and ITEM_RED_KEY.
UNUSED static void CrossboneIsleKeys_UnusedRedrawKeyActors(void) {
  if (++gCrossboneIsleKeys_IconTimer > 16) {
    gCrossboneIsleKeys_IconTimer = 0;
  }
  switch (gCrossboneIsleKeys_IconTimer) {
  case 12:
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY) == 0) {
      Actor_SetPosition(ACTOR_FIRST_BLUE_KEY, 232, 872);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 1) == 0) {
      Actor_SetPosition(ACTOR_SECOND_BLUE_KEY, 296, 824);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 2) == 0) {
      Actor_SetPosition(ACTOR_THIRD_BLUE_KEY, 328, 760);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 3) == 0) {
      Actor_SetPosition(ACTOR_FOURTH_BLUE_KEY, 360, 872);
    }
    break;
  // XXX (no evidence): why item codes 0 and 1, whose icons are a yellow gem and
  // a sword, where CrossboneIsleKeys_InitializeKeys draws the keys' own;
  // nothing calls this, and no other code or text says what it was for.
  case 10:
    Colosso_ShowItemIcon(ACTOR_FIRST_BLUE_KEY, 0);
    break;
  case 8:
    Colosso_ShowItemIcon(ACTOR_SECOND_BLUE_KEY, 0);
    break;
  case 6:
    Colosso_ShowItemIcon(ACTOR_THIRD_BLUE_KEY, 0);
    break;
  case 4:
    Colosso_ShowItemIcon(ACTOR_FOURTH_BLUE_KEY, 0);
    break;
  case 2:
    Colosso_ShowItemIcon(ACTOR_RED_KEY, 1);
    break;
  }
}

#include "shared/colosso_item_icon.c"

/// Puts each key not yet taken on its post, and draws open the doors already
/// opened.
static void CrossboneIsleKeys_InitializeKeys(void) {
  wait(1);
  Colosso_ShowItemIcon(ACTOR_RED_KEY, ITEM_RED_KEY);
  Colosso_ShowItemIcon(ACTOR_FOURTH_BLUE_KEY, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(ACTOR_THIRD_BLUE_KEY, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(ACTOR_SECOND_BLUE_KEY, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(ACTOR_FIRST_BLUE_KEY, ITEM_BLUE_KEY);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY) == 0) {
    Actor_SetPosition(ACTOR_FIRST_BLUE_KEY, 232, 872);
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 1) == 0) {
    Actor_SetPosition(ACTOR_SECOND_BLUE_KEY, 296, 824);
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 2) == 0) {
    Actor_SetPosition(ACTOR_THIRD_BLUE_KEY, 328, 760);
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 3) == 0) {
    Actor_SetPosition(ACTOR_FOURTH_BLUE_KEY, 360, 872);
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN) != 0) {
    CrossboneIsleKeys_OpenBlueDoor(0);
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 1) != 0) {
    CrossboneIsleKeys_OpenBlueDoor(1);
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 2) != 0) {
    CrossboneIsleKeys_OpenBlueDoor(2);
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 3) != 0) {
    CrossboneIsleKeys_OpenBlueDoor(3);
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN) != 0) {
    CrossboneIsleKeys_OpenRedDoor(0);
  }
}

/// Makes the first four statues solid and draws all five a little narrower,
/// starts the watches for Isaac in front of the first, second and fifth, and
/// lays the darkness over the hall, half see-through.
static void CrossboneIsleKeys_InitializeStatues(void) {
  Actor_Get(ACTOR_FIRST_STATUE)->collisionFlags = ACTOR_SOLID;
  Actor_Get(ACTOR_SECOND_STATUE)->collisionFlags = ACTOR_SOLID;
  Actor_Get(ACTOR_THIRD_STATUE)->collisionFlags = ACTOR_SOLID;
  Actor_Get(ACTOR_FOURTH_STATUE)->collisionFlags = ACTOR_SOLID;
  Actor_Get(ACTOR_FIRST_STATUE)->scaleX = 7 * FIXED_ONE / 10;
  Actor_Get(ACTOR_SECOND_STATUE)->scaleX = 7 * FIXED_ONE / 10;
  Actor_Get(ACTOR_THIRD_STATUE)->scaleX = 7 * FIXED_ONE / 10;
  Actor_Get(ACTOR_FOURTH_STATUE)->scaleX = 7 * FIXED_ONE / 10;
  Actor_Get(ACTOR_FIFTH_STATUE)->scaleX = 7 * FIXED_ONE / 10;
  Task_AddOrUpdateFrame(CrossboneIsleKeys_CheckFifthPursuerTrigger,
                        TASK_PASS_FRAME);
  Task_AddOrUpdateFrame(CrossboneIsleKeys_CheckSecondPursuerTrigger,
                        TASK_PASS_FRAME);
  Task_AddFrame(CrossboneIsleKeys_CheckFirstPursuerTrigger, TASK_PASS_FRAME);
  Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
             &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(7, 6), &REG_BLDALPHA);
}

/// Lays the cells under each pillar where it stands, refreezes the puddle if
/// Frost froze it since the party came in, and puts Bane where its last
/// escape left it:
/// at the west end of the back ledge, on the low ledge below it, at the east
/// end of the back ledge, or on the spiked ledge in the south-east.
static void CrossboneIsleKeys_InitializeBane(void) {
  struct Actor *djinni;

  Actor_Get(ACTOR_SECOND_PILLAR_CHEST)->motionFlags = 0;
  Task_AddOrUpdateFrame(CrossboneIsleKeys_FollowSecondPillar, TASK_PASS_FRAME);
  Task_AddFrame(CrossboneIsleKeys_UpdatePillarPriorities, TASK_PASS_FRAME);
  TreasureSpot_SetPosition(TREASURE_SPOT_DEMON_AXE, 0, 0);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_DEMON_AXE) != 0) {
    Actor_SetAnimation(ACTOR_SECOND_PILLAR_CHEST, 2);
  }
  CrossboneIsleKeys_MoveFirstPillar();
  CrossboneIsleKeys_MoveSecondPillar();
  CrossboneIsleKeys_MoveThirdPillar();
  CrossboneIsleKeys_MoveFourthPillar();
  CrossboneIsleKeys_MoveFifthPillar();
#if TBS_PATCH_1
  Actor_SetSpritePriority(ACTOR_FIRST_PILLAR, 3);
#endif
  Actor_Get(ACTOR_FOURTH_PILLAR)->motionFlags = 0;
  Actor_Get(ACTOR_FIFTH_PILLAR)->motionFlags = 0;
  CrossboneIsleKeys_RepaintFourthAndFifthPillarCells();
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_PUDDLE_FROZEN) != 0) {
    CrossboneIsleKeys_FreezePuddle();
    Actor_SetAnimation(ACTOR_PUDDLE, PUDDLE_ANIMATION_PILLAR);
  }

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) != 0) {
      Actor_SetPosition(ACTOR_BANE, 856, 824);
      Actor_Get(ACTOR_BANE)->update =
          (void (*)(struct Actor *))CrossboneIsleKeys_FaceBaneTowardLeader;
    } else if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) != 0) {
      Actor_SetPosition(ACTOR_BANE, 888, 664);
      djinni = Actor_Get(ACTOR_BANE);
      djinni->sprite->rotation = 0;
      Actor_SetAnimationSpeed(djinni, ANIM_STEP_NORMAL);
    } else if (GameFlag_Get(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) != 0) {
      Actor_SetPosition(ACTOR_BANE, 584, 680);
    } else {
      Actor_SetPosition(ACTOR_BANE, 584, 664);
    }
  }
}

#include "shared/pushed_actor_tables.c"
// clang-format off

s32 gCrossboneIsleKeys_BaneSquashScript[] = {
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
    SC_WAIT(64),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Where Bane runs to, in pixels, when Isaac steps onto each of the twelve
/// cells round the three spikes on the south-east ledge, from
/// CELL_BEHAVIOR_BANE_CORNER_FIRST on: always the spot across the spikes
/// from him.
s32 gCrossboneIsleKeys_BaneWalkTargets[][2] = {
    { 888, 856 },
    { 872, 856 },
    { 856, 856 },
    { 840, 856 },
    { 824, 856 },
    { 824, 840 },
    { 824, 824 },
    { 840, 824 },
    { 856, 824 },
    { 872, 824 },
    { 888, 824 },
    { 888, 840 },
};

#include "shared/fallback_entrances.c"

struct SceneEntrance gCrossboneIsleKeysRoomEntrances[] = {
    { CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_EAST, CONDITION_ALWAYS, 200, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
    { CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_WEST, CONDITION_ALWAYS, 88, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleStatuesEntrances[] = {
    { CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_EAST, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
    { CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_WEST, CONDITION_ALWAYS, 88, 0, 392, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleBaneEntrances[] = {
    { CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_EAST, CONDITION_ALWAYS, 776, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
    { CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_WEST, CONDITION_ALWAYS, 664, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
    { SCENE_TABLE_END },
};

u32 gCrossboneIsleKeysExits[] = {
    SCENE_EXITS(CrossboneIsleKeys),
    SCENE_EXIT(CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_EAST, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_KEYS_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_WEST, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_KEYS_WEST),
    SCENE_EXITS(CrossboneIsleStatues),
    SCENE_EXIT(CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_EAST, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_STATUES_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_WEST, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_STATUES_WEST),
    SCENE_EXITS(CrossboneIsleBane),
    SCENE_EXIT(CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_EAST, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_BANE_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_WEST, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_BANE_WEST),
    SCENE_EXITS_END,
};

struct ActorPlacement gCrossboneIsleKeysActorPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gCrossboneIsleKeysRoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        80, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        80, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        80, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        80, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        80, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gCrossboneIsleStatuesActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gCrossboneIsleBaneActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        616, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        728, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        600, 0, 856, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        824, 0, 888, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        840, 0, 888, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        648, 0, 888, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        728, 0, 760, FACING_SOUTH, TALK_FACING_KEEP, 0),
    // Bane, until it is caught
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_BANE), MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct Trigger gCrossboneIsleKeysTriggersOther[] = {
    { TRIGGER_TABLE_END },
};

struct Trigger gCrossboneIsleKeysRoomTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_WEST },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_ShowRedKey) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_HideRedKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FIRST_BLUE_KEY, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_CatchFirstBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_SECOND_BLUE_KEY, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_CatchSecondBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_THIRD_BLUE_KEY, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_CatchThirdBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FOURTH_BLUE_KEY, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_CatchFourthBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_RED_KEY, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_CatchRedKey) },
    { TRIGGER_STEP, CELL_BEHAVIOR_KEYS_RED_DOOR, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_UseRedKey) },
    { TRIGGER_STEP, CELL_BEHAVIOR_KEYS_FIRST_BLUE_DOOR, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_OpenFirstBlueDoor) },
    { TRIGGER_STEP, CELL_BEHAVIOR_KEYS_SECOND_BLUE_DOOR, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_OpenSecondBlueDoor) },
    { TRIGGER_STEP, CELL_BEHAVIOR_KEYS_THIRD_BLUE_DOOR, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_OpenThirdBlueDoor) },
    { TRIGGER_STEP, CELL_BEHAVIOR_KEYS_FOURTH_BLUE_DOOR, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_OpenFourthBlueDoor) },
    // Unused: the plain step rows above answer the same cells first.
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_KEYS_THIRD_BLUE_DOOR, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_UnusedJumpOntoPost) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_KEYS_FOURTH_BLUE_DOOR, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_UnusedJumpOntoPost) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_CROSSBONE_ISLE_KEYS_333_COINS, FIND_COINS(333) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_CROSSBONE_ISLE_KEYS_FAIRY_RING, FIND_ITEM(ITEM_FAIRY_RING) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_CROSSBONE_ISLE_KEYS_COOKIE, FIND_ITEM(ITEM_COOKIE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_CROSSBONE_ISLE_KEYS_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { TRIGGER_TABLE_END },
};

struct Trigger gCrossboneIsleStatuesTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_WEST },
    { TRIGGER_STEP, CELL_BEHAVIOR_STATUES_BEFORE_THIRD_STATUE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_ThirdStatueShadowsLeader) },
    { TRIGGER_STEP, CELL_BEHAVIOR_STATUES_BEFORE_FOURTH_STATUE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_FourthStatueShadowsLeader) },
    { TRIGGER_STEP, CELL_BEHAVIOR_STATUES_THIRD_ALCOVE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_ThirdStatuePushesLeaderBack) },
    { TRIGGER_STEP, CELL_BEHAVIOR_STATUES_FOURTH_ALCOVE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_FourthStatuePushesLeaderBack) },
    { TRIGGER_RAISED, RAISED_FIFTH_CHASE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_FifthStatuePushesLeaderBack) },
    { TRIGGER_RAISED, RAISED_SECOND_CHASE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_SecondStatuePushesLeaderBack) },
    { TRIGGER_RAISED, RAISED_FIRST_CHASE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_FirstStatuePushesLeaderBack) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_CastRevealInStatueRoom) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_RevealEndsInStatueRoom) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_FIRST_STATUE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_SECOND_STATUE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_THIRD_STATUE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_FOURTH_STATUE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_FIFTH_STATUE, CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_HaltPursuer) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_CROSSBONE_ISLE_STATUES_444_COINS, FIND_COINS(444) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_CROSSBONE_ISLE_STATUES_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_CROSSBONE_ISLE_STATUES_STORM_GEAR, FIND_ITEM(ITEM_STORM_GEAR) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_CROSSBONE_ISLE_STATUES_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { TRIGGER_TABLE_END },
};
// clang-format on

struct Trigger gCrossboneIsleBaneTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_EAST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_WEST},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_BANE_THIRD_PILLAR,
     CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_PushThirdPillar)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_BANE_FIRST_PILLAR,
     CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_PushFirstPillar)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_BANE_SECOND_PILLAR,
     CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_PushSecondPillar)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_BANE_FOURTH_PILLAR,
     CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_PushFourthOrFifthPillar)},
#if TBS_PATCH_1
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     CELL_BEHAVIOR_BANE_WEST_OF_FIRST_PILLAR, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_PushFirstPillar)},
#endif
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     CELL_BEHAVIOR_BANE_THIRD_PILLAR_EDGE, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_TouchThirdPillar)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_BANE_LADDER,
     CONDITION_ALWAYS, EVENT(CrossboneIsleKeys_UseLadder)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     CELL_BEHAVIOR_BANE_EAST_OF_FIRST_PILLAR, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_JumpEastOrPushFirstPillar)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_FIRST_ESCAPE, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneFirstEscape)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_SECOND_ESCAPE, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneSecondEscape)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_LAST_ESCAPE, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneLastEscape)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_CORNER_FIRST, FLAG_DJINNI(DJINNI_BANE),
     EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_CORNER_FIRST + 1,
     FLAG_DJINNI(DJINNI_BANE), EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_CORNER_FIRST + 2,
     FLAG_DJINNI(DJINNI_BANE), EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_CORNER_FIRST + 3,
     FLAG_DJINNI(DJINNI_BANE), EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_CORNER_FIRST + 4,
     FLAG_DJINNI(DJINNI_BANE), EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_CORNER_FIRST + 5,
     FLAG_DJINNI(DJINNI_BANE), EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_CORNER_FIRST + 6,
     FLAG_DJINNI(DJINNI_BANE), EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_CORNER_FIRST + 7,
     FLAG_DJINNI(DJINNI_BANE), EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_CORNER_FIRST + 8,
     FLAG_DJINNI(DJINNI_BANE), EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_CORNER_FIRST + 9,
     FLAG_DJINNI(DJINNI_BANE), EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_CORNER_FIRST + 10,
     FLAG_DJINNI(DJINNI_BANE), EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BANE_CORNER_FIRST + 11,
     FLAG_DJINNI(DJINNI_BANE), EVENT(CrossboneIsleKeys_BaneKeepsAway)},
    {TRIGGER_TALK_HALTED, ACTOR_BANE, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_BaneJoins)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_BANE, CONDITION_ALWAYS,
     EVENT(CrossboneIsleKeys_HaltBane)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE,
     FLAG_CROSSBONE_ISLE_BANE_PUDDLE_FROZEN,
     EVENT(CrossboneIsleKeys_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, FLAG_SCENE_TEMP_01,
     EVENT(CrossboneIsleKeys_MoveFirstPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR,
     FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER,
     EVENT(CrossboneIsleKeys_MoveSecondPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_PILLAR, FLAG_SCENE_TEMP_03,
     EVENT(CrossboneIsleKeys_MoveThirdPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_PILLAR, FLAG_SCENE_TEMP_04,
     EVENT(CrossboneIsleKeys_MoveFourthPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIFTH_PILLAR, FLAG_SCENE_TEMP_05,
     EVENT(CrossboneIsleKeys_MoveFifthPillar)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 104, FLAG_CROSSBONE_ISLE_BANE_666_COINS,
     FIND_COINS(666)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 105,
     FLAG_CROSSBONE_ISLE_BANE_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 106, FLAG_CROSSBONE_ISLE_BANE_ELIXIR,
     FIND_ITEM(ITEM_ELIXIR)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_DEMON_AXE,
     FLAG_CROSSBONE_ISLE_DEMON_AXE, FIND_ITEM(ITEM_DEMON_AXE)},
    {TRIGGER_TABLE_END},
};

/// The column each pursuing statue stands in.
u32 gCrossboneIsleKeys_SlotColumns[] = {
    16,
    21,
    0,
};

/// Unused: the frame counter of CrossboneIsleKeys_UnusedRedrawKeyActors.
s32 gCrossboneIsleKeys_IconTimer = 0;
