/// @file
/// Venus Lighthouse, six scenes in one overlay: the approach outside, the floor
/// the front door opens onto, and the four floors of the climb that ends in the
/// Lighthouse Aerie at the top (venus_lighthouse_aerie.c).
///
/// Each scene has its own entrance, actor placement and trigger tables
/// (VenusLighthouseGroundFloor's entrances are the fallback table). All share
/// one exit table. Scene_Initialize redraws each scene's puzzles as
/// their flags left them, case by entrance. Inside, only the actors of the
/// room entered are placed (CONDITION_IN_VIEW).
///
/// - Venus Lighthouse Entry (SceneId_VenusLighthouseEntry), outside. The
///   scholars and Babi's soldiers the wanderers attacked lie hurt and say what
///   they saw. A soldier who tells of the attack flickers and vanishes
///   (FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD), and his spot no longer answers a
///   search. VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR, the open
///   door, leads in.
/// - Venus Lighthouse (SceneId_VenusLighthouseGroundFloor, named by an area
///   rule of its own), the floor behind the front door, with three scholars.
///   The great statue (CELL_BEHAVIOR_VENUS_LIGHTHOUSE_GREAT_STATUE) opens one
///   of two paths (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or _SECOND_PATH). The
///   first time the frame counter's parity picks the path, and each later
///   search switches it. The statue lights the open path in gold lines between
///   the floor lamps before it, where a small model of the lighthouse stands
///   (gVenusMiniatureLighthouseScript). The Tunnel Ruins under Babi Lighthouse
///   redraw their ways to match (tunnel_ruins.c), and they lead up into
///   VenusLighthouseFirstFloor. The reliefs
///   (CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF) are read, and Reveal shows
///   a hidden path in two places. Move or a push brings a grey block up
///   against a wall (FLAG_VENUS_LIGHTHOUSE_PILLAR_AT_WALL), and Carry, from
///   the Carry Stone in one of the floor's chests, sets it down beyond
///   (FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED). In the room of the lady statue,
///   a band of lightning crackles across the floor south of her and throws
///   back whoever walks into it, until Move slides her onto the floor switch
///   in front of her. Standing on the switch presses it only while he stays.
/// - Venus Lighthouse (SceneId_VenusLighthouseFirstFloor), where the Tunnel
///   Ruins come up
///   (VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_TUNNEL_RUINS_LADY_STATUES). A wide
///   slide of flowing sand carries the party south down its big room, and a
///   hole drops the leader into the room below.
///   Three capped pillars are Moved or pushed. Two pillars by a gap are
///   carried across it, or dropped through the floor. A chest holds the Thunder
///   Crown, and a Psynergy Stone lies here.
/// - Venus Lighthouse (SceneId_VenusLighthouseSecondFloor). A sparkling
///   passage takes the leader through in a trail of sparks. A floor switch
///   slides a wall aside in a cloud of dust the first time
///   (FLAG_VENUS_LIGHTHOUSE_WALL_MOVED). A pillar Moved or pushed into its
///   socket sinks, and a light runs from the lady statue along the gold track
///   over it to open the gate (FLAG_VENUS_LIGHTHOUSE_GATE_OPEN).
/// - Venus Lighthouse (SceneId_VenusLighthouseThirdFloor). Three more
///   sparkling passages, slides of flowing sand, and three pillars carrying
///   pieces of track that sink into four sockets. With all three sunk two
///   lights run from the statue, and when the track leads one to the gate it
///   opens (FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN, with
///   FLAG_VENUS_LIGHTHOUSE_SOUTHEAST_SOCKET_EMPTY or
///   FLAG_VENUS_LIGHTHOUSE_NORTHWEST_SOCKET_EMPTY for where the pillars stand).
///   A chest holds Dragon Scales.
/// - Venus Lighthouse (SceneId_VenusLighthouseFourthFloor). A maze of sand
///   flowing north, south, west and east carries the party each way. A west and
///   an east floor switch slide their walls aside
///   (FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED and _EAST_WALL_MOVED, which
///   VenusLighthouseThirdFloor also redraws). Carry sets down four floating
///   pillars on two balance platforms that sink and rise to their weight
///   (gBalance_PlatformHeights). Four pillars carrying track sink into
///   eight sockets, and the lights run on to the pillar room's gate
///   (FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN). A chest holds the Gaia Blade,
///   and a Psynergy Stone lies here. A passage raises the leader on a
///   platform in rising sparkles to the Lighthouse Aerie
///   (VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_AERIE).
///
/// Locked doors stand on the second floor and above
/// (CELL_BEHAVIOR_VENUS_LIGHTHOUSE_LOCKED_DOOR).
///
/// The wall and gate flags (FLAG_VENUS_LIGHTHOUSE_WALL_MOVED to
/// FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN) and the Psynergy Stones' flags are
/// area flags. They last while the party stays in the scenes of the climb
/// (SceneId_VenusLighthouseFirstFloor to SceneId_VenusLighthouseAerie, one
/// area), so the floors stay solved between them.
///
/// The FLAG_SCENE_TEMP_ states last only until the next change of scene other
/// than a return from battle. Every arrival sets FLAG_IN_VENUS_LIGHTHOUSE.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/ground_mask.h"
#include "field/map_grid_scroll.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/actor_step.h"
#include "scene/field_effect_spawn.h"
#include "scene/import.h"
#include "scene/walk_into_leader.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/heap_slots.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "system/iwram_call.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern s32 gPushedActor_DirectionSteps[];
extern struct SwitchCell gVenusThirdFloorSockets[];
extern u32 gBalance_PlatformHeights[];
extern struct SwitchCell gVenusFourthFloorSockets[];

/// A socket index past the third floor's four (gVenusThirdFloorSockets) and
/// the fourth floor's eight (gVenusFourthFloorSockets): the pillar is in none.
#define NO_THIRD_FLOOR_SOCKET 5
#define NO_FOURTH_FLOOR_SOCKET 8
extern u8 gVenusTracksMovingWest[];
extern u8 gVenusTracksMovingEast[];
extern u8 gVenusTracksMovingNorth[];
extern u8 gVenusTracksMovingSouth[];
extern s32 gVenusLeaderShockedScript[];
extern s32 gGlint_TwinkleScript[];
extern s32 gVenusSecondFloorLightToGate[];
extern s32 gVenusThirdFloorLightToNortheast[];
extern s32 gVenusThirdFloorLightToSoutheast[];
extern s32 gVenusLightGoesOut[];
extern s32 gVenusFourthFloorLightFromStatue[];
extern s32 gVenusFourthFloorLightToSoutheast[];
extern s32 gVenusFourthFloorLightToSouthwest[];
extern u16 gVenusSecondFloorGateCells[];
extern u16 gVenusThirdFloorGateCells[];
extern u16 gVenusFourthFloorGateCells[];
extern struct SceneEntrance gVenusLighthouseEntryEntrances[];
extern struct SceneEntrance gVenusLighthouseEntrancesOther[];
extern struct SceneEntrance gVenusLighthouseFirstFloorEntrances[];
extern struct SceneEntrance gVenusLighthouseSecondFloorEntrances[];
extern struct SceneEntrance gVenusLighthouseThirdFloorEntrances[];
extern struct SceneEntrance gVenusLighthouseFourthFloorEntrances[];
extern u32 gVenusLighthouseExits[];
extern struct ActorPlacement gVenusLighthouseActorPlacementsOther[];
extern struct ActorPlacement gVenusLighthouseEntryActorPlacements[];
extern struct ActorPlacement gVenusLighthouseGroundFloorActorPlacements[];
extern struct ActorPlacement gVenusLighthouseFirstFloorActorPlacements[];
extern struct ActorPlacement gVenusLighthouseSecondFloorActorPlacements[];
extern struct ActorPlacement gVenusLighthouseThirdFloorActorPlacements[];
extern struct ActorPlacement gVenusLighthouseFourthFloorActorPlacements[];
extern struct Trigger gVenusLighthouseEntryTriggers[];
extern struct Trigger gVenusLighthouseGroundFloorTriggers[];
extern struct Trigger gVenusLighthouseFirstFloorTriggers[];
extern struct Trigger gVenusLighthouseSecondFloorTriggers[];
extern struct Trigger gVenusLighthouseThirdFloorTriggers[];
extern struct Trigger gVenusLighthouseFourthFloorTriggers[];
extern const u8 *gVenusThirdFloorSocketTracks[];
extern s32 **gVenusThirdFloorSocketPaths[];
extern const u8 *gVenusFourthFloorSocketTracks[];
extern s32 **gVenusFourthFloorSocketPaths[];

// Area flags, cleared when the party enters another area, whose numbers other
// areas use for their own.

/// The second floor's pink floor switch has slid the wall beside it aside.
#define FLAG_VENUS_LIGHTHOUSE_WALL_MOVED FLAG_AREA_TEMP_00
#define FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED FLAG_AREA_TEMP_06
#define FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED FLAG_AREA_TEMP_07
// The gates the sunken pillars opened: one pillar, three pillars (and which of
// two sets of sockets they sank into) and the pillar room's four.
/// The light from the second floor's lady statue has opened the gate at the
/// end of its track.
#define FLAG_VENUS_LIGHTHOUSE_GATE_OPEN FLAG_AREA_TEMP_01
/// The pillars stand in every socket of gVenusThirdFloorSockets but the
/// southeast one.
#define FLAG_VENUS_LIGHTHOUSE_SOUTHEAST_SOCKET_EMPTY FLAG_AREA_TEMP_02
/// The pillars stand in every socket of gVenusThirdFloorSockets but the
/// northwest one.
#define FLAG_VENUS_LIGHTHOUSE_NORTHWEST_SOCKET_EMPTY FLAG_AREA_TEMP_03
#define FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN FLAG_AREA_TEMP_04
#define FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN FLAG_AREA_TEMP_08
/// The first floor's Psynergy Stone, taken until the party leaves the climb.
#define FLAG_VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE FLAG_AREA_TEMP_50
/// The fourth floor's Psynergy Stone, as the first floor's.
#define FLAG_VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE FLAG_AREA_TEMP_51

/// The cell behaviors the floors' events answer, by floor. A number means
/// one thing on one floor and another on the next.
///
/// Each exit cell carries the number of the exit it leads by, and the event
/// rows name it by that exit. They name the sparkling passages the same way,
/// which leave by their own number (VenusLighthouse_EnterSparklingPassage).
enum VenusLighthouseCellBehavior {
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_LOCKED_DOOR =
      25, ///< a locked door, from the second floor up
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_SOUTH =
      30, ///< sand flowing south, which carries the party with it
  /// A floor's chest.
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_CHEST = 100,
  /// A floor's Psynergy Stone.
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_PSYNERGY_STONE = 110,
  // VenusLighthouseGroundFloor
  /// The first of the six reliefs, whose words VenusLighthouse_ReadRelief shows
  /// in their order.
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF = 35,
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_GREAT_STATUE = 41,
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_LIGHTNING =
      45, ///< the band of lightning south of the lady statue
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOOR_SWITCH = 46,
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_WALL_PILLAR =
      50, ///< walking into the grey block (ACTOR_WALL_PILLAR)
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_SECOND_CHEST = 101,
  // VenusLighthouseFirstFloor
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_HOLE = 35,
  /// The floor bounding the rows where the capped pillars stand, where
  /// stepping back out ends VenusLighthouse_EnterCappedPillarRows.
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_BESIDE_CAPPED_PILLARS = 31,
  /// The rows where the capped pillars stand. Walking into a pillar from
  /// either kind of cell pushes it.
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AMONG_CAPPED_PILLARS = 32,
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_CARRY_PILLARS =
      45, ///< the gap and the two pillars beside it
  // VenusLighthouseSecondFloor
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_WALL_SWITCH = 35,
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_GATE_PILLAR =
      36, ///< the gap and ACTOR_GATE_PILLAR
  // VenusLighthouseThirdFloor
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_SWITCH_PILLARS =
      35, ///< the gaps and the three switch pillars
  // VenusLighthouseFourthFloor
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_RISING_PASSAGE = 18,
  /// The sand maze's flowing sand, by the way it carries the party.
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_NORTH = 31,
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_WEST = 32,
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_EAST = 33,
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS =
      35, ///< pushing a floating pillar
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_GAP =
      36, ///< a gap jumped west or east
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_EAST_GAP =
      37, ///< a gap jumped east
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_WEST_WALL_SWITCH = 40,
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_EAST_WALL_SWITCH = 41,
  CELL_BEHAVIOR_VENUS_LIGHTHOUSE_SOCKET_PILLARS =
      45 ///< the gaps and the four socket pillars
};

// UB: passage offsets and dust directions can be negative. The callers use
// bounded pixel offsets or directions -1..1, so these products fit s32.
#ifdef UBFIX
#define VENUS_LIGHTHOUSE_FIXED_OFFSET(value, bits) ((value) * (1 << (bits)))
#else
#define VENUS_LIGHTHOUSE_FIXED_OFFSET(value, bits) ((value) << (bits))
#endif

// UB: this calls a void function through an s32 function type; UBFIX calls it
// as declared.
#ifdef UBFIX
#define VENUS_LIGHTHOUSE_ARRIVE_THROUGH_SPARK_PASSAGE                          \
  VenusLighthouse_ArriveThroughSparkPassage
#else
#define VENUS_LIGHTHOUSE_ARRIVE_THROUGH_SPARK_PASSAGE                          \
  ((s32 (*)(void))VenusLighthouse_ArriveThroughSparkPassage)
#endif

/// The cells of row 34 that take row 35's behaviours as Isaac drops through
/// the first floor's hole: their first column and their width, wider from
/// patch 3.
#if TBS_PATCH_3
#define VENUS_DESCENT_COLLISION_X 33
#define VENUS_DESCENT_COLLISION_WIDTH 7
#else
#define VENUS_DESCENT_COLLISION_X 34
#define VENUS_DESCENT_COLLISION_WIDTH 5
#endif

static void VenusLighthouse_StepActorWithDust(struct Vector3 *direction);
static void VenusLighthouse_MoveFirstFloorSandFall(void);
static void VenusLighthouse_PuffWestSlideHead(void);
static void VenusLighthouse_PuffEastSlideHead(void);
static void VenusLighthouse_PourWestSandSlide(void);
static void VenusLighthouse_PourEastSandSlide(void);
static void VenusLighthouse_RaiseLeaderInSparkles(void);

// The scene flags: on the ground floor, FLAG_SCENE_TEMP_00 is the lady statue
// holding the floor switch down and
// FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH the leader standing on it; on
// the fourth floor, FLAG_SCENE_TEMP_00 plus a floating pillar's number (0 to 3)
// is that pillar set down.

// Each scene numbers its actors by the rows of its own actor
// placement table, so each has an enum of its own; a row drawn as one of the
// party's characters (Garet in VenusLighthouseGroundFloor) is that party member
// in his own slot and takes no number.

/// Venus Lighthouse Entry's actors, numbering the rows of
/// gVenusLighthouseEntryActorPlacements. They are the scholars
/// (SPRITE_OLD_MAN_IN_BLUE_CAP) and Babi's soldiers (SPRITE_SOLDIER) the
/// wanderers attacked, each named by what they say.
enum VenusLighthouseEntryActorId {
  /// A soldier by the entrance: "Venus Lighthouse was... attacked by
  /// strange... wanderers...", and he flickers and is gone
  /// (VenusLighthouse_HearOfTheAttack).
  ACTOR_SOLDIER_TELLING_OF_THE_ATTACK = ACTOR_FIRST_PLACED,
  /// "Iodem, be happy. The entrance to Venus Lighthouse is open..."
  ACTOR_SCHOLAR_LIGHTHOUSE_OPEN,
  /// "Ohhh! How could they attack mere scholars? Those barbarians!"
  ACTOR_SCHOLAR_BARBARIANS,
  /// "I've waited so long to see what it's like inside Venus Lighthouse,
  /// but..." (VenusLighthouse_TalkToWaitingScholar)
  ACTOR_WAITING_SCHOLAR,
  /// "How... How did they open the door to Venus Lighthouse?"
  ACTOR_SOLDIER_OPENED_DOOR,
  /// "They are already... headed for Babi Lighthouse..."
  ACTOR_SOLDIER_HEADED_FOR_BABI,
  /// "Babi's soldiers are all Colosso victors... How could anyone beat all of
  /// us?" His thoughts: "Was it Kraden?"
  ACTOR_SOLDIER_COLOSSO_VICTORS
};

/// VenusLighthouseGroundFloor's actors, numbering the rows of
/// gVenusLighthouseGroundFloorActorPlacements after Garet's.
enum VenusLighthouseGroundFloorActorId {
  /// "Kraden told them to stop, but they attacked anyway."
  ACTOR_SCHOLAR_ATTACKED_ANYWAY = ACTOR_FIRST_PLACED,
  /// "Kraden went inside, but then came back and said this isn't the
  /// entrance."
  ACTOR_SCHOLAR_NOT_THE_ENTRANCE,
  /// VenusLighthouse_TalkToScholarByDoor.
  ACTOR_DOOR_SCHOLAR,
  /// The pillar Move pushes to the wall and Carry sets down beyond it
  /// (VenusLighthouse_MovePillarTowardWall).
  ACTOR_WALL_PILLAR = ACTOR_FIRST_PLACED + 4,
  /// The lady statue Move pushes onto the floor switch (SPRITE_LADY_STATUE)
  /// (VenusLighthouse_MovePillarOntoSwitch).
  ACTOR_FLOOR_SWITCH_STATUE
};

/// VenusLighthouseFirstFloor's actors, numbering the rows of
/// gVenusLighthouseFirstFloorActorPlacements.
enum VenusLighthouseFirstFloorActorId {
  /// The three pillars, SPRITE_CAPPED_PILLAR, that Move or a push shifts, the
  /// nearer drawn in front (VenusLighthouse_MoveCappedPillar).
  ACTOR_FIRST_CAPPED_PILLAR = ACTOR_FIRST_PLACED,
  /// The two pillars by the gap, carried across it or dropped into it
  /// (VenusLighthouse_CarryPillarAcrossGap).
  ACTOR_FIRST_CARRY_PILLAR = ACTOR_FIRST_CAPPED_PILLAR + 3
};

/// VenusLighthouseSecondFloor's actors, numbering the rows of
/// gVenusLighthouseSecondFloorActorPlacements.
enum VenusLighthouseSecondFloorActorId {
  /// A lady statue, as ACTOR_FLOOR_SWITCH_STATUE is, where
  /// ACTOR_GATE_SPARKLE sets out for the gate.
  ACTOR_GATE_STATUE = ACTOR_FIRST_PLACED,
  /// The pillar before the gate, which sinks into its socket
  /// (VenusLighthouse_MoveGatePillar).
  ACTOR_GATE_PILLAR,
  /// SPRITE_PARTICLE, the kind of actor VenusLighthouse_CreateLight makes.
  /// Once the pillar sinks it travels gVenusSecondFloorLightToGate to the gate,
  /// which opens, and stays there shedding particles.
  ACTOR_GATE_SPARKLE
};

/// VenusLighthouseThirdFloor's actors, numbering the rows of
/// gVenusLighthouseThirdFloorActorPlacements.
enum VenusLighthouseThirdFloorActorId {
  /// A lady statue (SPRITE_LADY_STATUE), where the two gate effects set out
  /// once all three switch pillars have sunk.
  ACTOR_SWITCH_STATUE = ACTOR_FIRST_PLACED,
  /// The three pillars that sink into free sockets
  /// (VenusLighthouse_MoveSwitchPillar).
  ACTOR_FIRST_SWITCH_PILLAR,
  /// SPRITE_PARTICLE, as ACTOR_GATE_SPARKLE, placed where the gate effects
  /// end once the gate is open.
  ACTOR_SWITCH_GATE_SPARKLE = ACTOR_FIRST_SWITCH_PILLAR + 3
};

/// VenusLighthouseFourthFloor's actors, numbering the rows of
/// gVenusLighthouseFourthFloorActorPlacements.
enum VenusLighthouseFourthFloorActorId {
  /// The two platforms, SPRITE_BALANCE_PLATFORM, that rise or sink to the
  /// weight of the floating pillars set down on them
  /// (VenusLighthouse_LowerFloatingPillars).
  ACTOR_BALANCE_WEST = ACTOR_FIRST_PLACED,
  ACTOR_BALANCE_EAST,
  /// The four floating pillars Carry sets down
  /// (VenusLighthouse_CarryFloatingPillar).
  ACTOR_FIRST_FLOATING_PILLAR,
  /// A lady statue (SPRITE_LADY_STATUE), where the first gate effect sets out
  /// once all four socket pillars have sunk. The camera goes to it first.
  ACTOR_SOCKET_STATUE = ACTOR_FIRST_FLOATING_PILLAR + 4,
  /// The four pillars of the pillar room (VenusLighthouse_MoveSocketPillar).
  ACTOR_FIRST_SOCKET_PILLAR,
  /// SPRITE_PARTICLE, as ACTOR_GATE_SPARKLE, placed where the gate effects
  /// end once the gate is open.
  ACTOR_SOCKET_GATE_SPARKLE = ACTOR_FIRST_SOCKET_PILLAR + 4,
  /// The platform, SPRITE_FLOATING_PLATFORM as the floating platforms of
  /// Mercury Lighthouse's aerie. It dips as the leader lands on it at
  /// VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING
  /// and then carries him up to the Lighthouse Aerie
  /// (VenusLighthouse_RaiseLeaderInSparkles).
  ACTOR_RISING_PLATFORM
};

/// Animations the lighthouse plays by number beyond enum ActorAnimation,
/// from each sprite's animation table (graphics/sprite/<name>.tsv).
enum {
  /// The hurt scholars and soldiers lying on the ground
  /// (old_man_in_blue_cap.tsv and
  /// soldier.tsv, the same pose).
  ANIM_HURT_LYING = 5,
  /// A light's sparkle, flickering between its two largest pictures
  /// (particle.tsv).
  PARTICLE_ANIMATION_FLICKER = 7
};

/// An animation timer step for Actor_SetAnimationSpeedById, in sixteenths of a
/// frame taken off each frame. 16 plays an animation at its own speed, so 24
/// plays it half again as fast.
enum { ANIM_STEPS_FAST = 24 };

/// The pieces of gold track a SPRITE_SPIRAL_PILLAR can carry, its animations
/// from 3. Each is named by the two edges of its cell it joins as the
/// pillar's top shows it, north up (graphics/sprite/spiral_pillar.png).
///
/// The lights the lady statues send out run along the track
/// (VenusLighthouse_RouteLight). A pillar sunk in a socket carries the track
/// on across it. A light that runs onto a piece that does not join the edge
/// it comes in by goes out.
enum VenusTrack {
  VENUS_TRACK_END = 0, ///< ends a list of pieces
  VENUS_TRACK_NORTH_SOUTH = 3,
  VENUS_TRACK_NORTH_WEST = 4,
  VENUS_TRACK_NORTH_EAST = 5,
  VENUS_TRACK_WEST_EAST = 6,
  VENUS_TRACK_SOUTH_EAST = 7,
  VENUS_TRACK_SOUTH_WEST = 8
};

/// An actor's timer as gVenusGreyStatueScript and gVenusGreyPillarScript
/// set it. It holds a tag in the upper bits and a colour mode (enum
/// SpriteColorMode) in the low four, so that
/// VenusLighthouse_RestoreMarkedColors can draw the actor in that mode
/// again after a battle.
#define VENUS_COLOR_MARK(mode) (VENUS_COLOR_MARK_TAG << 4 | (mode))
/// The tag VENUS_COLOR_MARK puts above the colour mode.
enum { VENUS_COLOR_MARK_TAG = 0x212 };

/// Sets bits in a byte of flags.
static inline void VenusLighthouse_SetFlagBits(u8 *flags, u8 bits) {
  *flags |= bits;
}

static UNUSED s32 MapGrid_FillCollisionRectangle(u32 layer, s32 x, s32 z,
                                                 u32 width, u32 height,
                                                 s32 value);

/// Makes a mask still, never a talk or Psynergy target, shadowless and drawn in
/// the given colour mode.
static void VenusLighthouse_ConfigurePassageEffect(struct Actor *actor,
                                                   s32 colorMode) {
  actor->motionFlags = 0;
  actor->collisionFlags = ACTOR_NOT_TARGETED;
  Actor_SetSpriteFlags(actor, 0);
  Actor_SetSpriteColor(actor, colorMode);
}

/// Creates a mask over a hole as the leader or a pillar falls through the
/// floor.
///
/// The mask is an actor of sprite type (SPRITE_CRATE or SPRITE_CAPPED_PILLAR)
/// drawn solid black (SPRITE_COLOR_BLACK) and translucent in front of the
/// actors. That shows the background through whatever falls behind it, so
/// that it vanishes below the hole's edge.
///
/// Babi Lighthouse's PIT_MASK is the same actor, and its render without the
/// mask shows the leader whole.
static struct Actor *
VenusLighthouse_CreateBlendedPassageEffect(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *effect;

  effect = FieldActor_Create(type, x, y, z);
  if (effect != NULL) {
    ((struct Sprite *)effect->sprite)->image.priority = 0;
    VenusLighthouse_ConfigurePassageEffect(effect, SPRITE_COLOR_BLACK);
    Actor_SetBlendMode(effect, ACTOR_BLEND_TRANSLUCENT);
    return effect;
  }
  return 0;
}

/// Creates an actor that keeps the leader on screen while a pillar sinks
/// into its socket with him standing north of it, where the sinking code puts
/// his sprite at priority 3.
///
/// It is a SPRITE_CHEST drawn in SPRITE_COLOR_HIDDEN, so it shows nothing
/// itself. Without it the leader would vanish for the 20 frames the pillar
/// takes to go down.
static struct Actor *
VenusLighthouse_CreateOccludingPassageEffect(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *actor;

  actor = FieldActor_Create(type, x, y, z);
  if (actor != NULL) {
    ((struct Sprite *)actor->sprite)->image.priority = 1;
    VenusLighthouse_ConfigurePassageEffect(actor, SPRITE_COLOR_HIDDEN);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    return actor;
  }
  return 0;
}

#include "shared/field_effect_spawn.c"

/// The scene's names for the bytes Actor keeps for scene code. A statue
/// with SW_VENUS_IMMOVABLE set cannot be pushed
/// (VenusLighthouse_PushPillarAhead). A light sets
/// SW_VENUS_LIGHT_ARRIVED as it reaches its gate, which then opens.
enum {
  SW_VENUS_IMMOVABLE = SW_SCENE_COUNTER,
  SW_VENUS_LIGHT_ARRIVED = SW_SCENE_BYTE
};

#include "shared/actor_distance.c"
#include "shared/actor_find_at_position.c"

/// Pushing a pillar moves the pillar ahead of the leader one cell on with him,
/// leaning into it (ANIM_PUSH).
static void VenusLighthouse_PushPillarAhead(void) {
  struct FieldPosition targetPosition;
  struct Actor *leader;
  struct Actor *pillar;
  struct Actor *blockingActor;
  s32 directionIndex;
  FIXED moveRate;
  s32 rising;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  directionIndex = FACING_INDEX(leader->facing);
  Actor_StepFrom(&targetPosition, leader,
                 gPushedActor_DirectionSteps[directionIndex]);
  pillar = Actor_FindAtPosition(&targetPosition, leader);
  if (pillar == NULL)
    return;

  Actor_StepFrom(&targetPosition, pillar,
                 gPushedActor_DirectionSteps[directionIndex]);
  blockingActor = Actor_FindAtPosition(&targetPosition, pillar);
  if (blockingActor != NULL &&
      (blockingActor->collisionFlags & ACTOR_SOLID) != 0)
    return;

  targetPosition.x = pillar->x.fixed;
  targetPosition.y = pillar->y.fixed + toFIXED(16);
  targetPosition.z = pillar->z.fixed;
  blockingActor = Actor_FindAtPosition(&targetPosition, pillar);
  if (blockingActor != NULL &&
      (blockingActor->collisionFlags & ACTOR_SOLID) != 0)
    return;

  pillar->layer = 2;
  Actor_StepFrom(&targetPosition, pillar,
                 gPushedActor_DirectionSteps[directionIndex]);
  if (FieldActor_CheckMovementCollision(pillar, &targetPosition) > 0)
    return;

  // A pillar still sinking into its socket stays where it is.
  rising = pillar->sceneCounter;
  if (rising != 0)
    return;

  Actor_SelectAnimation(leader, ANIM_PUSH);
  moveRate = toFIXED(1) / 5;
  wait(15);
  Sound_Play(SE_PUSH);
  pillar->speedLimit = moveRate;
  pillar->acceleration = moveRate;
  FieldActor_SetMoveTargetFX(pillar, targetPosition.x, targetPosition.y,
                             targetPosition.z);
  leader->speedLimit = moveRate;
  leader->acceleration = moveRate;
  FieldActor_SetMoveTargetFX(leader, targetPosition.x, targetPosition.y,
                             targetPosition.z);
  Actor_WaitForMove(pillar);
  Scene_PlayQueuedMusic();
  pillar->x.fixed = targetPosition.x;
  pillar->z.fixed = targetPosition.z;
  // Both stop: rising is 0 here.
  pillar->velocityX = rising;
  pillar->velocityZ = rising;
  leader->targetX = ACTOR_NO_TARGET;
  leader->targetZ = ACTOR_NO_TARGET;
  leader->velocityX = rising;
  leader->velocityZ = rising;
  leader->x.fixed = toFIXED(leader->x.part[0].integer);
  leader->z.fixed = toFIXED(leader->z.part[0].integer);
  Actor_SelectAnimation(leader, ANIM_STAND);
}

// Unused here: nothing in Venus Lighthouse calls it.

#include "shared/pushed_actor_fill_rectangle.c"

/// After a battle, draws every actor whose timer holds a
/// VENUS_COLOR_MARK in its colour mode again.
static void VenusLighthouse_RestoreMarkedColors(void) {
  struct Actor **actors = &gFieldState->actors[ACTOR_FIRST_PLACED];
  struct Actor *actor;
  u32 actorId;

  for (actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_ID; actorId++) {
    actor = actors[actorId - ACTOR_FIRST_PLACED];
    if (actor->timer >> 4 == VENUS_COLOR_MARK_TAG)
      Actor_SetSpriteColor(actor, actor->timer & 15);
  }
}

/// Draws the actor in the colour mode of its VENUS_COLOR_MARK.
static s32 VenusLighthouse_ApplyMarkedColor(struct Actor *actor) {
  Actor_SetSpriteColor(actor, actor->timer & 15);
  return 0;
}

/// The leader flashes white one frame in four as the lightning strikes him,
/// with a crackle every eight frames.
static UNREAD_RESULT VenusLighthouse_FlashStruckLeader(struct Actor *actor) {
  if ((gFrameCount & 3) == 0)
    Actor_SetSpriteColor(actor, SPRITE_COLOR_WHITE);
  else
    Actor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);

  if ((gFrameCount & 7) == 0)
    Sound_Play(SE_ITEM_BREAK);
  RETURN_UNREAD(0);
}

/// Small black puffs of smoke rise from the leader, one every eight frames,
/// after the lightning has struck him.
static UNREAD_RESULT VenusLighthouse_EmitRisingParticles(struct Actor *actor) {
  struct EffectOptions options;
  s32 framePhase;
  FIXED particleX, particleY;
  s32 riseSpeed;
  framePhase = gFrameCount & 7;
  if (framePhase == 0) {
    options.priority = 3 - (Random_Below(2));
    options.startScaleX = 2 * FIXED_ONE / 5;
    options.startScaleY = 2 * FIXED_ONE / 5;
    options.palette = SPRITE_COLOR_BLACK;
    particleX = actor->x.fixed + toFIXED(Random_Below(9) - 4);
    particleY = actor->y.fixed + toFIXED(32 - Random_Below(32));
    riseSpeed = Math_Divide(toFIXED(Random_Below(5)) + toFIXED(5), 10);
    FieldEffect_Spawn(
        particleX, particleY, actor->z.fixed, 0, riseSpeed, framePhase,
        EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE,
        &options);
  }
  RETURN_UNREAD(0);
}

/// Clears the scripted actor's sprite flags and advances its script.
static s32 VenusLighthouse_ClearScriptedSpriteFlags(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

/// The sparks that trail the leader through a sparkling passage: a stream
/// falling from his shoulders every frame and a larger one every 16.
static UNREAD_RESULT
VenusLighthouse_EmitPassageSparkTrail(struct Actor *actor) {
  struct EffectOptions options;
  FIXED fallSpeed;
  s32 framePhase;

  options.startScaleX = 4 * FIXED_ONE / 5;
  options.startScaleY = 4 * FIXED_ONE / 5;
  options.priority = 0;

  fallSpeed = (Random_Below(8)) * (toFIXED(1) / 5);
  framePhase = gFrameCount & 15;

  FieldEffect_Spawn(actor->x.fixed +
                        VENUS_LIGHTHOUSE_FIXED_OFFSET(8 - framePhase, 16),
                    actor->y.fixed + toFIXED(26), actor->z.fixed, 0, -fallSpeed,
                    0, EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE, &options);

  framePhase = gFrameCount & 15;
  if (framePhase == 0) {
    options.startScaleX = FIXED_ONE / 2;
    options.startScaleY = FIXED_ONE / 2;
    FieldEffect_Spawn(actor->x.fixed + toFIXED(Random_Below(9) - 4),
                      actor->y.fixed, actor->z.fixed, 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE, &options);
  }

  RETURN_UNREAD(0);
}

/// Isaac walks north into one of the sparkling passages on the second and
/// third floors and fades away in a trail of sparks, to come out of the
/// passage's other end.
///
/// The passage's cell carries the number of the exit it leaves by
/// (state->stepTrigger), and VenusLighthouse_ArriveThroughSparkPassage brings
/// him out sparkling at the far end.
static void VenusLighthouse_EnterSparklingPassage(void) {
  struct Actor *leader;
  struct FieldState *state;

  state = gFieldState;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
  Sound_Play(SE_SPARKLING_PASSAGE);
  leader->update =
      (void (*)(struct Actor *))VenusLighthouse_EmitPassageSparkTrail;
  leader->speedLimit = toFIXED(1) / 5;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  leader->update = 0;
  Event_Wait(30);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(state->stepTrigger);
  Event_End();
}

/// Brings the leader out of a sparkling passage on a fresh arrival.
static void VenusLighthouse_ArriveThroughSparkPassage(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    Event_Start();
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    leader->motionFlags = 0;
    Actor_SetPositionFX(ACTOR_PARTY_LEADER, toFIXED(leader->x.part[0].integer),
                        toFIXED(leader->z.part[0].integer) - toFIXED(16));
    Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Sound_Play(SE_SPARKLING_PASSAGE);
    leader->update =
        (void (*)(struct Actor *))VenusLighthouse_EmitPassageSparkTrail;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.4);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
    Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    leader->update = 0;
    Scene_PlayQueuedMusic();
    Event_End();
  }
}

/// Nothing visible happens when Isaac steps into the dotted square of floor
/// where the first floor's three capped pillars stand (cells (11..16,
/// 43..53)). The game starts drawing the ground mask on him, a see-through
/// strip on each side wherever the floor beside him is higher than his own,
/// but on this flat floor it leaves him looking just as before.
static void VenusLighthouse_EnterCappedPillarRows(void) {
  gGroundMaskState->actor = Actor_Get(ACTOR_PARTY_LEADER);
}

/// Nothing visible happens when Isaac steps out of the dotted square of the
/// capped pillars onto the floor round it. The game stops drawing the ground
/// mask (VenusLighthouse_EnterCappedPillarRows) on him.
static void VenusLighthouse_LeaveCappedPillarRows(void) {
  gGroundMaskState->actor = NULL;
}

#include "shared/actor_depth.c"
#include "shared/actor_wait_until_idle.c"
#include "shared/field_effect_advance.c"

/// Isaac drops from above into the first floor's lower room after falling
/// through the hole at the foot of the sand slide, and lands in a ring of
/// white dust.
static void VenusLighthouse_LandWithRadialDust(void) {
  struct Actor *leader;
  u32 particleIndex;
  struct EffectOptions options;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  leader->y.fixed = toFIXED(130);
  leader->gravity = toFIXED(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Sound_Play(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_Wait(24);
  options.palette = SPRITE_COLOR_WHITE;
  options.update = (void (*)(struct Actor *))FieldEffect_AdvanceMotion;
  options.startScaleX = 4 * FIXED_ONE / 5;
  options.startScaleY = 4 * FIXED_ONE / 5;
  // A puff each sixteenth of a turn, the last over the first.
  for (particleIndex = 0; particleIndex <= TRIG_TURN / FACING_STEP;
       particleIndex++) {
    s32 angle;
    s32 velocity[3];

    angle = particleIndex * FACING_STEP;
    SET_VECTOR(velocity, Trig_Cos(angle), 0, Trig_Sin(angle));
    velocity[0] += velocity[0] / 2;
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_PALETTE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_UPDATE | EFFECT_SCRIPT_MEDIUM,
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
  Scene_PlayQueuedMusic();
  leader->gravity = toFIXED(1);
  leader->bounce = FIXED_ONE / 4;
  Event_End();
}

#include "shared/field_effect_radial.c"

/// The leader stands on the pink floor switch in front of the ground floor's
/// lady statue and holds it down.
#define FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH FLAG_SCENE_TEMP_01

/// A light's update routine: every four frames a shrinking particle around it,
/// and now and then a sound while it moves.
static UNREAD_RESULT VenusLighthouse_TrailLight(struct Actor *actor) {
  struct EffectOptions options;
  s32 velocityX;
  s32 velocityZ;

  if ((gFrameCount & 3) != 0) {
    RETURN_UNREAD(0);
  }
  if (Random_Below(6) == 0) {
    if (actor->targetX != ACTOR_NO_TARGET ||
        actor->targetZ != ACTOR_NO_TARGET) {
      Sound_Play(SE_GLITTER);
    }
  }
  options.type = SPRITE_GLINT;
  options.startScaleX = FIXED_ONE;
  options.startScaleY = FIXED_ONE;
  options.targetScaleX = -FIXED_ONE / 200;
  options.targetScaleY = -FIXED_ONE / 200;
  velocityX = Math_Divide(toFIXED(Random_Below(9) - 4), 10);
  velocityZ = Math_Divide(toFIXED(Random_Below(9) - 4), 10);
  FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed - toFIXED(1),
                    velocityX, 0, velocityZ,
                    EFFECT_SCALE_TO_TARGET | EFFECT_USE_START_SCALE |
                        EFFECT_USE_TYPE | EFFECT_SCRIPT_MEDIUM,
                    &options);
  RETURN_UNREAD(0);
}

/// Creates a light at a lady statue, at pixel x and z, and starts it along its
/// path script, trailing particles (VenusLighthouse_TrailLight).
static struct Actor *VenusLighthouse_CreateLight(s32 x, s32 z,
                                                 const s32 *script) {
  struct Actor *effect;
  FIXED fixedX;
  FIXED fixedZ;

  fixedX = toFIXED(x);
  fixedZ = toFIXED(z);
  effect = FieldActor_Create(SPRITE_PARTICLE, fixedX, 0, fixedZ);
  if (effect == NULL) {
    return 0;
  }
  effect->speedLimit = toFIXED(1);
  effect->acceleration = toFIXED(1);
  Actor_SetSpriteFlags(effect, 0);
  Actor_SelectAnimation(effect, PARTICLE_ANIMATION_FLICKER);
  effect->motionFlags = 0;
  effect->y.fixed = 0;
  effect->collisionFlags = 0;
  effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  effect->update = (void (*)(struct Actor *))VenusLighthouse_TrailLight;
  effect->sceneByte = 0;
  Actor_StartScript(effect, script);
  return effect;
}

/// Walking into a one-cell gap in the floor, Isaac springs and leaps across
/// it, if the cell beyond is clear. These are the gaps beside the pillars Move
/// pushes on each floor of the climb, and on the fourth floor the gaps among
/// the floating pillars.
///
/// When something stands in the way there is no jump, and the callers try a
/// push instead (the result is 0).
static s32 VenusLighthouse_JumpAcrossGap(void) {
  struct Actor *leader;
  struct FieldPosition landing;
  u8 savedMotionFlags;
  s32 direction;
#if !TBS_PATCH_1
  u32 step;
#endif

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  savedMotionFlags = leader->motionFlags;
#if TBS_PATCH_1
  landing.x = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  landing.y = leader->y.fixed;
  landing.z = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  direction = FACING_NEAREST_CARDINAL(leader->facing);
  Vector_AddPolarOffset(toFIXED(16), direction, &landing);
#else
  direction = FACING_INDEX(leader->facing);
  Actor_StepFrom(&landing, leader, gPushedActor_DirectionSteps[direction]);
#endif
  if (FieldActor_CheckMovementCollision(leader, &landing) != 1 &&
      Actor_FindAtPosition(&landing, leader) == 0) {
#if TBS_PATCH_1
    landing.x = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
    landing.y = leader->y.fixed;
    landing.z = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
    Vector_AddPolarOffset(toFIXED(32), FACING_NEAREST_CARDINAL(leader->facing),
                          &landing);
#else
    step = gPushedActor_DirectionSteps[direction];
    // Two steps on.
    landing.x = leader->x.fixed + DIRECTION_STEP_X_FIXED(step) * 2;
    landing.y = leader->y.fixed;
    step = DIRECTION_STEP_Z_FIXED(step) * 2;
    landing.z = leader->z.fixed + step;
#endif
    if (Actor_FindAtPosition(&landing, leader) == 0 &&
        FieldActor_CheckMovementCollision(leader, &landing) == 0) {
      Event_Start();
      Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
      wait(6);
      Sound_Play(SE_JUMP);
      Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
      leader->speedLimit = toFIXED(3);
      leader->acceleration = toFIXED(2);
      leader->velocityY = toFIXED(4);
      leader->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
      Actor_SetSpriteFlags(leader, 0);
      Actor_MoveToAndWait(ACTOR_PARTY_LEADER, FIXEDtoINT(landing.x),
                          FIXEDtoINT(landing.z));
      Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
      Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
      leader->motionFlags = savedMotionFlags;
      Event_End();
      return 1;
    }
  }
  return 0;
}

/// Selects the scene's entrances. VenusLighthouseGroundFloor takes the
/// fallback table.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_VENUS_LIGHTHOUSE_ENTRY) {
    return gVenusLighthouseEntryEntrances;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_FIRST_FLOOR) {
    return gVenusLighthouseFirstFloorEntrances;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_SECOND_FLOOR) {
    return gVenusLighthouseSecondFloorEntrances;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_THIRD_FLOOR) {
    return gVenusLighthouseThirdFloorEntrances;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_FOURTH_FLOOR) {
    return gVenusLighthouseFourthFloorEntrances;
  }
  return gVenusLighthouseEntrancesOther;
}

/// These scenes supply no exit regions.
const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

/// Returns the packed exits shared by the six scenes.
const u32 *Scene_GetExits(void) { return gVenusLighthouseExits; }

/// Selects the scene's actor placements.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  struct ActorPlacement *placements;
  s16 sceneId;

  sceneId = gMainRoutineState.next;
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_ENTRY) {
    return gVenusLighthouseEntryActorPlacements;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_GROUND_FLOOR) {
    placements = gVenusLighthouseGroundFloorActorPlacements;
    Scene_AssignViewFlags(placements);
    return placements;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_FIRST_FLOOR) {
    placements = gVenusLighthouseFirstFloorActorPlacements;
    Scene_AssignViewFlags(placements);
    return placements;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_SECOND_FLOOR) {
    placements = gVenusLighthouseSecondFloorActorPlacements;
    Scene_AssignViewFlags(placements);
    return placements;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_THIRD_FLOOR) {
    placements = gVenusLighthouseThirdFloorActorPlacements;
    Scene_AssignViewFlags(placements);
    return placements;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_FOURTH_FLOOR) {
    placements = gVenusLighthouseFourthFloorActorPlacements;
    Scene_AssignViewFlags(placements);
    return placements;
  }
  return gVenusLighthouseActorPlacementsOther;
}

/// Isaac searches one of the pink gates at the end of a gold track, on the
/// second, third or fourth floor (cells (46, 42), (45, 38) and (13, 36)),
/// before a light has opened it, and a message says the door is tightly
/// locked.
static void VenusLighthouse_SearchLockedDoor(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldDoorTightlyLocked, 1);
  Event_End();
}

/// The times the soldier who tells of the attack flickers as he vanishes, the
/// pause after each a frame shorter than the last, from eight frames to three.
#define SOLDIER_FLICKERS 10

/// Outside Venus Lighthouse, Isaac talks to the dying soldier by the way in,
/// or reads his mind, and he gasps out that strange wanderers attacked the
/// lighthouse, then flickers and vanishes.
///
/// "Venus Lighthouse was... attacked by strange... wanderers..." Afterwards
/// his spot no longer answers a search.
static void VenusLighthouse_HearOfTheAttack(void) {
  u32 flickersLeft;
  u32 flickerDelay;

  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, 3);
  Event_SetMessageId(MsgFieldVenusLighthouseWasAttackedBy);
  flickersLeft = SOLDIER_FLICKERS;
  flickerDelay = 8;
  // "Venus Lighthouse was... attacked by strange... wanderers..."
  Event_ShowMessageThenWait(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, 0, 20);
  do {
    Actor_SetSpriteColorById(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK,
                             SPRITE_COLOR_HIDDEN);
    wait(2);
    Actor_SetSpriteColorById(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK,
                             SPRITE_COLOR_NORMAL);
    wait(flickerDelay);
    if (flickerDelay > 3) {
      flickerDelay--;
    }
    flickersLeft--;
  } while (flickersLeft != 0);
  GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD);
  Actor_SetPosition(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, 0, 0);
  MapGrid_CopyCellAttributes(7, 17, 2, 1, 7, 16);
  Event_End();
}

/// Outside Venus Lighthouse, a scholar lying hurt on the ground tells Isaac
/// he has waited so long to see inside, and now cannot move for the pain.
static void VenusLighthouse_TalkToWaitingScholar(void) {
  Event_Start();
  Event_SetMessageId(MsgVenusIveWaitedLongSeeIts);
  // "I've waited so long to see what it's like inside Venus..."
  Event_ShowMessageThenWait(ACTOR_WAITING_SCHOLAR, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_WAITING_SCHOLAR, 2);
  // "I can't even move without feeling terrible, terrible pain."
  Event_ShowMessage(ACTOR_WAITING_SCHOLAR, 0);
  Event_End();
}

/// Isaac lifts the grey spiral-marked block with Carry, in the room east of
/// the reliefs where Move and pushing could only bring it up against the
/// wall, and sets it down out of the way.
///
/// Set down in column 54 or row 6, it stays there on later visits
/// (FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED).
static void VenusLighthouse_CarryPillarPastWall(void) {
  struct Actor *pillar;

  pillar = Actor_Get(ACTOR_WALL_PILLAR);
  Event_Start();
  if (FIELD_CELL(pillar->x.fixed) == 54 || FIELD_CELL(pillar->z.fixed) == 6) {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED);
  }
  Event_End();
}

/// Isaac reads the relief on the wall of the room north of the entrance hall
/// (cells (19, 10) to (23, 10)): "There are words carved into the relief... If
/// ye seek to climb Venus Lighthouse, first seek the power within. To obtain
/// this power, see with the eyes of truth. Then the way shall open." In the
/// editions that keep them, two more reliefs further in (cells (47, 57) and
/// (58, 57)) say, "The true road can only be known by he who bears a true
/// heart."
///
/// Each relief's message is the first's plus its cell's place after
/// CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF. Editions with patch 1 but not
/// patch 2 show the first relief's words for every relief and leave the two
/// further in silent.
static void VenusLighthouse_ReadRelief(void) {
  s32 message;
#if !TBS_PATCH_1 || TBS_PATCH_2
  struct FieldState *state = gFieldState;
#endif
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
#if TBS_PATCH_1 && !TBS_PATCH_2
  message = (s32)MSGNO(MsgVenusThereWordsCarvedIntoRelief);
#else
  message = state->examineCell + MSGNO(MsgVenusThereWordsCarvedIntoRelief) -
            CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF;
#endif
  FieldMessage_Show(message, 1);
  Event_End();
}

/// When Reveal wears off on the lighthouse's ground floor, the two hidden ways
/// it showed vanish again: the doorway at the east end of the carved reliefs
/// (cells (23, 8..10)) and the stairs in the south of the lady statue's room
/// (cells (46..48, 55..57)).
static void VenusLighthouse_HidePath(void) {
  s32 column;
  s32 row;

#if TBS_PATCH_1
  GameFlag_Clear(FLAG_NO_ENCOUNTERS_IN_AREA);
#endif
  column = 23;
  row = 8;
  MapGrid_CopyCellAttributes(35, 8, 1, 3, column, row);
  MapGrid_CopyCellsTo(35, 8, 23, 8, 1, 3);
  MapGrid_CopyCellsTo(99, 8, 87, 8, 1, 3);
  column = 46;
  row = 55;
  MapGrid_CopyCellAttributes(57, 55, 3, 3, column, row);
  MapGrid_CopyCellsTo(57, 55, 46, 55, 3, 3);
  MapGrid_CopyCellsTo(121, 55, 110, 55, 3, 3);
}

/// Isaac casts Reveal on the lighthouse's ground floor, and the screen turns
/// grey as a doorway appears at the east end of the carved reliefs (cells
/// (23, 8..10)), the way to the Carry Stone.
///
/// Stairs appear the same way in the south of the lady statue's room (cells
/// (46..48, 55..57)), the way on to the great statue. Both can be walked
/// through while Reveal lasts. Editions with patch 1 also stop random battles
/// for that time.
static void VenusLighthouse_RevealPath(void) {
  s32 column;
  s32 row;

#if TBS_PATCH_1
  GameFlag_Set(FLAG_NO_ENCOUNTERS_IN_AREA);
#endif
  column = 23;
  row = 8;
  MapGrid_CopyCellAttributes(36, 8, 1, 3, column, row);
  MapGrid_CopyCellsTo(36, 8, 23, 8, 1, 3);
  MapGrid_CopyCellsTo(100, 8, 87, 8, 1, 3);
  column = 46;
  row = 55;
  MapGrid_CopyCellAttributes(53, 55, 3, 3, column, row);
  MapGrid_CopyCellsTo(53, 55, 46, 55, 3, 3);
  MapGrid_CopyCellsTo(117, 55, 110, 55, 3, 3);
}

/// Isaac walks into the band of yellow lightning that crackles across the
/// floor south of the lady statue (cells (41..43, 53..57)), and it strikes
/// him. He flashes white, turns grey and is thrown back south, sits dazed with
/// smoke rising from him, and shakes his head before he comes to.
///
/// It strikes until Move has slid the lady statue onto the floor switch in
/// front of her (FLAG_SCENE_TEMP_00). He lands on row 52 when he was struck at
/// row 54 or above, else on row 59.
static void VenusLighthouse_ThrowBackFromLightning(void) {
  struct Actor *leader;
  s32 landingQuarterZ;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
  Actor_SetScript(ACTOR_PARTY_LEADER, gVenusLeaderShockedScript);
  Actor_WaitForScriptEndById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_GREY);
  leader->velocityY = toFIXED(4);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 4);
  if (FIELD_CELL(leader->z.fixed) <= 54) {
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    landingQuarterZ = 210;
  } else {
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    landingQuarterZ = 238;
  }
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, leader->x.part[0].integer,
                      landingQuarterZ << 2);
  Event_Wait(1);
  /* FAKEMATCH(wrapper): the flag set through a helper. */
  VenusLighthouse_SetFlagBits(&Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags,
                              ACTOR_FACES_MOTION);
  Event_Wait(20);
  leader->update =
      (void (*)(struct Actor *))VenusLighthouse_EmitRisingParticles;
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  leader->update = 0;
  Event_End();
}

/// Redraws the great statue's room for the first of the two paths it opens
/// (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH).
static inline void VenusLighthouse_DrawFirstPath(void) {
  MapGrid_CopyCellsTo(103, 27, 89, 27, 7, 8);
  MapGrid_CopyCellsTo(41, 90, 27, 92, 3, 2);
  MapGrid_CopyCellsTo(41, 90, 29, 93, 3, 2);
  MapGrid_CopyCellsTo(41, 90, 27, 94, 3, 2);
  MapGrid_CopyCellsTo(41, 90, 27, 96, 3, 2);
  MapGrid_CopyCellsTo(41, 90, 29, 97, 3, 2);
  MapGrid_CopyCellsTo(41, 96, 25, 91, 3, 2);
  MapGrid_CopyCellsTo(41, 92, 25, 93, 3, 2);
  MapGrid_CopyCellsTo(41, 96, 25, 95, 3, 2);
  MapGrid_CopyCellsTo(41, 96, 25, 97, 3, 2);
  MapGrid_CopyCellsTo(41, 96, 27, 96, 3, 2);
  MapGrid_CopyCellsTo(41, 96, 29, 97, 3, 2);
}

/// Redraws the great statue's room with the second path open
/// (FLAG_VENUS_LIGHTHOUSE_SECOND_PATH), as VenusLighthouse_DrawFirstPath.
static inline void VenusLighthouse_DrawSecondPath(void) {
  MapGrid_CopyCellsTo(111, 27, 89, 27, 7, 8);
  MapGrid_CopyCellsTo(41, 90, 25, 91, 3, 2);
  MapGrid_CopyCellsTo(41, 90, 25, 93, 3, 2);
  MapGrid_CopyCellsTo(41, 90, 25, 95, 3, 2);
  MapGrid_CopyCellsTo(41, 90, 25, 97, 3, 2);
  MapGrid_CopyCellsTo(41, 90, 27, 96, 3, 2);
  MapGrid_CopyCellsTo(41, 90, 29, 97, 3, 2);
  MapGrid_CopyCellsTo(41, 94, 27, 92, 3, 2);
  MapGrid_CopyCellsTo(41, 96, 29, 93, 3, 2);
  MapGrid_CopyCellsTo(41, 94, 27, 94, 3, 2);
  MapGrid_CopyCellsTo(41, 96, 27, 96, 3, 2);
  MapGrid_CopyCellsTo(41, 96, 29, 97, 3, 2);
}

/// Isaac searches the great seated statue at the north end of its hall (cell
/// (28, 25)), and the statue speaks to his soul. The screen pales to white,
/// and when it clears the floor lamps before the statue are joined by gold
/// lines along one of two paths, which the camera follows south and back.
///
/// The first time the frame counter's parity picks the path, and each later
/// search switches it (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or _SECOND_PATH). The
/// Tunnel Ruins under Babi Lighthouse open their ways to match.
static void VenusLighthouse_SearchStatue(void) {
  struct FieldState *state;

  state = gFieldState;
  state->revealFrames = 0;
  state->busy = 1;
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MsgVenusStatueSpeaksIsaacSoulYe, 1);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_PALE, PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  Event_Wait(100);
  Sound_Play(SE_SWIRL);
  Event_Wait(30);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  Event_Wait(70);
  if (Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) == 0) {
    if (Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH) == 0) {
      if ((gFrameCount & 1) != 0) {
        GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
      } else {
        GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
      }
    }
  }
  if (Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) == 0) {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
    GameFlag_Clear(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
    VenusLighthouse_DrawFirstPath();
  } else {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
    GameFlag_Clear(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
    VenusLighthouse_DrawSecondPath();
  }
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(20);
  Event_Wait(40);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveTo(456, CAMERA_KEEP, 542, 1);
  FieldCamera_WaitForMove();
  Event_Wait(50);
  FieldCamera_MoveTo(456, CAMERA_KEEP, 423, 1);
  FieldCamera_WaitForMove();
  Event_End();
  state->busy = 0;
}

/// Isaac Moves the grey spiral-marked block in the room east of the reliefs.
/// Once it is up against the wall, the first
/// time only, Garet steps out and says they can't push the block farther,
/// that it would be great to carry it and drop it to the ground, and asks
/// whether their Psynergy can help (FLAG_VENUS_LIGHTHOUSE_PILLAR_AT_WALL).
static void VenusLighthouse_MovePillarTowardWall(void) {
  struct Actor *actor;
  s32 garetRemarkFlag = FLAG_VENUS_LIGHTHOUSE_PILLAR_AT_WALL;
  FIXED walkSpeed;
  FIXED walkAcceleration;
  s32 emote;
  walkSpeed = toFIXED(4) / 5;
  walkAcceleration = toFIXED(2) / 5;
  emote = EMOTE_IN_FRONT | EMOTE_EXCLAMATION;
  Event_Start();
  if (FIELD_CELL(Actor_Get(ACTOR_WALL_PILLAR)->x.fixed) == 53) {
    if (GameFlag_Get(garetRemarkFlag) == 0) {
      GameFlag_Set(garetRemarkFlag);
      actor = Actor_Get(ACTOR_PARTY_LEADER);
      if (actor != NULL) {
        Actor_SetPositionFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
      }
      Actor_SetSpeedFX(ACTOR_GARET, walkSpeed, walkAcceleration);
      Actor_WalkToAndWait(ACTOR_GARET, 824, 88);
      Actor_WalkToAndWait(ACTOR_GARET, 824, 104);
      Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
      Event_Wait(20);
      Event_SetMessageId(MsgVenusHmmmWeCantPushBlock);
      // "Hmmm. We can't push the block farther than this."
      Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
      Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 10);
      Actor_ShowEmote(ACTOR_GARET, emote, 60);
      Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
      Event_Wait(20);
      Actor_RunRepeatedMotion(ACTOR_GARET, 2);
      Event_Wait(20);
      // "It'd be great if we could carry that block and drop it to..."
      Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
      Event_Wait(30);
      Actor_WalkToAndWait(ACTOR_GARET, 824, 88);
      Scene_WalkIntoLeader(ACTOR_GARET);
      Event_End();
    }
  }
}

/// Isaac pushes the grey spiral-marked block in the room east of the reliefs
/// by walking into it, as Move would, with Garet's remark once it reaches the
/// wall.
static void VenusLighthouse_PushPillarTowardWall(void) {
  Event_Start();
  VenusLighthouse_PushPillarAhead();
  Event_Wait(20);
  Event_End();
  VenusLighthouse_MovePillarTowardWall();
}

/// Isaac Moves the lady statue in the north-west corner of her room east onto
/// the floor switch in front of her (cell (42, 49)). There she sinks in with
/// a thump and holds the switch down, and the lightning south of her stops
/// (FLAG_SCENE_TEMP_00).
static void VenusLighthouse_MovePillarOntoSwitch(void) {
  struct Actor *pillar;
  FIXED switchHeight;

  pillar = Actor_Get(ACTOR_FLOOR_SWITCH_STATUE);
  Event_Start();
  if (FIELD_CELL(pillar->x.fixed) == 42) {
    Event_Wait(30);
    Sound_Play(SE_THUMP);
    pillar->motionFlags = 0;
    switchHeight = -toFIXED(2);
    pillar->groundHeight = switchHeight;
    pillar->y.fixed = switchHeight;
    GameFlag_Set(FLAG_SCENE_TEMP_00);
    MapGrid_CopyCellsTo(44, 117, 41, 117, 3, 5);
  }
  Event_End();
}

/// Isaac steps onto the middle of the pink floor switch in front of the lady
/// statue (cell (42, 49)): it sinks under him with a rattle, and the band of
/// lightning across the floor south of her vanishes for as long as he stands
/// there (FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH). Stepping off raises
/// the switch and draws the lightning again. On the cell's edge nothing
/// happens: the switch stays up and the lightning keeps crackling.
static void VenusLighthouse_StepOnFloorSwitch(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 x = leader->x.part[0].integer;
  s32 z = leader->z.part[0].integer;

  if (x < 676 || x > 683 || z < 788 || z >= 796) {
    MapGrid_CopyCellsTo(53, 50, 42, 49, 1, 1);
    MapGrid_CopyCellsTo(55, 117, 41, 117, 3, 5);
    GameFlag_Clear(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH);
    leader->motionFlags |= ACTOR_FOLLOWS_TERRAIN;
    leader->groundHeight = 0;
    leader->y.fixed = 0;
  } else if (!Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH)) {
    Event_Start();
    Event_Wait(5);
    MapGrid_CopyCellsTo(52, 50, 42, 49, 1, 1);
    MapGrid_CopyCellsTo(52, 117, 41, 117, 3, 5);
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH);
    Sound_Play(SE_RATTLE);
    leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
    leader->groundHeight = -toFIXED(2);
    leader->y.fixed = -toFIXED(2);
    Event_End();
  }
}

/// A scholar standing at the west door of the great statue's hall tells Isaac
/// he came in to explore once the door opened, and the camera turns to the
/// statue as he says the wanderers did something to it that made the floor
/// light up.
static void VenusLighthouse_TalkToScholarByDoor(void) {
  struct FieldState *state;

  state = gFieldState;
  state->revealFrames = 0;
  state->busy = 1;
  Event_Start();
  Event_SetMessageId(MsgVenusThoughtIdExploreAfterDoor);
  Actor_FaceActor(ACTOR_DOOR_SCHOLAR, ACTOR_PARTY_LEADER, 0);
  Event_Wait(10);
  // "I thought I'd explore after the door opened. While I was..."
  Event_ShowMessageThenWait(ACTOR_DOOR_SCHOLAR, 0, 20);
  Actor_FaceDirection(ACTOR_DOOR_SCHOLAR, FACING_NORTHEAST, 0);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(448, CAMERA_KEEP, 432, 1);
  FieldCamera_WaitForMove();
  // "They did something to the statue that made the floor light up."
  Event_ShowMessage(ACTOR_DOOR_SCHOLAR, 0);
  Event_End();
}

/// On the first floor, Isaac steps onto the hole in the floor and drops
/// through it into the room below.
static void VenusLighthouse_FallThroughFloor(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  Event_Start();
#if TBS_PATCH_3
  leader->motionFlags = 0;
#endif
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  leader->facing = FACING_SOUTH;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 3);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, leader->x.part[0].integer, 552);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  Event_Wait(30);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  leader->facing = FACING_NORTH;
  // The run cycle: his legs kick in the air as he falls.
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_RUN);
  Actor_SetAnimationSpeedById(ACTOR_PARTY_LEADER, ANIM_STEPS_FAST);
  Event_Wait(40);
  // Then he drops.
  leader->gravity = toFIXED(3) / 5;
  leader->bounce = 0;
  VenusLighthouse_CreateBlendedPassageEffect(
      leader->x.fixed, 0, leader->z.fixed + toFIXED(72), SPRITE_CRATE);
  MapGrid_CopyCellAttributes(VENUS_DESCENT_COLLISION_X, 35,
                             VENUS_DESCENT_COLLISION_WIDTH, 1,
                             VENUS_DESCENT_COLLISION_X, 34);
#if TBS_PATCH_3
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
#endif
  // BUG: the fall set up above is the leader's, but the wait is given a null
  // pointer, so it reads velocityY from the BIOS, which outside a BIOS call
  // reads as its last instruction, never 0: the wait always takes its full
  // second, however soon the leader lands.
#ifdef BUGFIX
  Actor_WaitUntilIdle(leader);
#else
  Actor_WaitUntilIdle(0);
#endif
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Import_EventSetExitRequest(
      VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM_HOLE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Isaac Moves one of the three grey pillars with white tops that stand in the
/// dotted square of floor in the first floor's south-west room (cells (11..16,
/// 43..53)), so that Isaac can leap onto their tops.
///
/// Once a pillar has slid, the game marks the floor of the square afresh, so
/// that walking into any pillar pushes it and the cells the pillars left are
/// floor again.
static void VenusLighthouse_MoveCappedPillar(void) {
  u32 pillarIndex;
  struct Actor *pillar;
  s32 destinationColumn;
  s32 destinationRow;
  s32 column;
  s32 row;

  Actor_Get(ACTOR_FIRST_CAPPED_PILLAR);
  Event_Start();
  destinationColumn = 12;
  destinationRow = 44;
  MapGrid_CopyCellAttributes(19, 44, 4, 1, destinationColumn, destinationRow);
  destinationColumn = 11;
  destinationRow = 51;
  MapGrid_CopyCellAttributes(17, 51, 2, 2, destinationColumn, destinationRow);
  for (pillarIndex = 0; pillarIndex <= 2; pillarIndex++) {
    pillar = Actor_Get(pillarIndex + ACTOR_FIRST_CAPPED_PILLAR);
    column = FIELD_CELL(pillar->x.fixed);
    row = FIELD_CELL(pillar->z.fixed);
    MapGrid_CopyCellAttributes(12, 50, 1, 1, column, row);
  }
  Actor_SwapPositionsByDepth(ACTOR_FIRST_CAPPED_PILLAR + 2,
                             ACTOR_FIRST_CAPPED_PILLAR + 1);
  Event_End();
}

/// Isaac walks into one of the three white-topped pillars in the first
/// floor's south-west room and pushes it a cell, as Move would.
static void VenusLighthouse_PushCappedPillar(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(19, 44, 4, 1, 12, 44);
  VenusLighthouse_PushPillarAhead();
  VenusLighthouse_MoveCappedPillar();
  Event_End();
}

/// Nothing visible happens as Isaac begins Move or Carry on one of the two
/// grey blocks marked with a spiral by the pit on the first floor, or pushes
/// one. The game first frees the cells under both blocks, which walking into
/// then tries a jump or a push, since one of them is about to leave its cell.
static void VenusLighthouse_UnblockCarryPillarCells(void) {
  u32 pillarIndex;
  struct Actor *pillar;

  for (pillarIndex = 0; pillarIndex <= 1; pillarIndex++) {
    pillar = Actor_Get(pillarIndex + ACTOR_FIRST_CARRY_PILLAR);
    MapGrid_SetCellBehaviorAt(0, pillar->x.fixed, pillar->z.fixed,
                              CELL_BEHAVIOR_VENUS_LIGHTHOUSE_CARRY_PILLARS);
  }
}

/// Isaac Moves one of the two grey spiral-marked blocks by the pit in the
/// first floor's west room (cells (4..16, 21..27)). Nothing more happens than
/// the slide: a block still standing on the floor is made solid again where
/// it ends up.
static void VenusLighthouse_MoveCarryPillar(void) {
  u32 pillarIndex;
  struct Actor *pillar;

  for (pillarIndex = 0; pillarIndex <= 1; pillarIndex++) {
    pillar = Actor_Get(pillarIndex + ACTOR_FIRST_CARRY_PILLAR);
    if (pillar->y.fixed > -toFIXED(16)) {
      MapGrid_SetCellBehaviorAt(0, pillar->x.fixed, pillar->z.fixed,
                                CELL_BEHAVIOR_WALL);
    }
  }
}

/// Isaac walks into the pit or one of the two spiral-marked blocks beside it,
/// in the first floor's west room. He leaps a one-cell gap if the far side is
/// clear; otherwise he pushes the block ahead of him.
static void VenusLighthouse_JumpOrPushCarryPillar(void) {
  Event_Start();
  if (VenusLighthouse_JumpAcrossGap() == 0) {
    VenusLighthouse_UnblockCarryPillarCells();
    VenusLighthouse_PushPillarAhead();
    VenusLighthouse_MoveCarryPillar();
  }
  Event_End();
}

/// A block Carry has set down in the pit stays there, its top flush with the
/// floor, and its cell becomes floor Isaac can walk across.
static void VenusLighthouse_FixPillarAtDestination(struct Actor *pillar) {
  s32 column;
  s32 row;

  pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  pillar->motionFlags = 0;
  column = FIELD_CELL(pillar->x.fixed);
  row = FIELD_CELL(pillar->z.fixed);
  MapGrid_CopyCellAttributes(9, 24, 1, 1, column, row);
}

/// A block Carry has dropped outside the pit falls through the floor out of
/// sight; a block still standing is made solid again.
static void VenusLighthouse_ReleaseFallenCarryPillar(struct Actor *pillar) {
  struct Sprite *sprite;
  struct Actor *passageEffect;

  sprite = pillar->sprite;
  if (sprite->image.priority == 3) {
    sprite->image.priority = 1;
    pillar->bounce = 0;
    passageEffect = VenusLighthouse_CreateBlendedPassageEffect(
        pillar->x.fixed, 0, toFIXED(512), SPRITE_CRATE);
    Actor_WaitUntilIdle(pillar);
    pillar->x.fixed = 0;
    pillar->z.fixed = 0;
    Actor_Release(passageEffect);
  } else {
    VenusLighthouse_MoveCarryPillar();
  }
}

/// Isaac lifts one of the two spiral-marked blocks in the first floor's west
/// room with Carry. Set down in the pit (column 8 for the first block, 7 for
/// the second), it fills it and Isaac can walk on across. One that drops
/// lower falls through the floor and is gone.
static void VenusLighthouse_CarryPillarAcrossGap(void) {
  struct Actor *pillar;

  Event_Start();
  pillar = Actor_Get(ACTOR_FIRST_CARRY_PILLAR);
  if (FIELD_CELL(pillar->x.fixed) == 8) {
    Actor_WaitUntilIdle(pillar);
    VenusLighthouse_FixPillarAtDestination(pillar);
  } else {
    VenusLighthouse_ReleaseFallenCarryPillar(pillar);
  }
  pillar = Actor_Get(ACTOR_FIRST_CARRY_PILLAR + 1);
  if (FIELD_CELL(pillar->x.fixed) == 7) {
    Actor_WaitUntilIdle(pillar);
    VenusLighthouse_FixPillarAtDestination(pillar);
  } else {
    VenusLighthouse_ReleaseFallenCarryPillar(pillar);
  }
  Event_End();
}

/// Isaac steps onto the pink floor switch in the east of the second floor's
/// west room (cell (19, 33)). The first time, the stone wall beside it
/// slides aside in a cloud of dust and the floor shakes, then the view cuts
/// to the first floor, where the sand fall in the room of three doors moves
/// east and leaves dry the stone chute it ran down
/// (VenusLighthouse_MoveFirstFloorSandFall).
///
/// It counts only in the middle of the cell, where the switch sinks under him
/// by 2 pixels. The wall's picture layer slides 96 pixels over 480 frames,
/// and its cells follow every 40 (FLAG_VENUS_LIGHTHOUSE_WALL_MOVED).
static void VenusLighthouse_StepOnWallSwitch(void) {
  struct EffectOptions options;
  struct MapGridLayerScroll *layer;
  struct Actor *leader;
  s32 column;
  s32 leaderZ;
  s32 tileDelay;
  u32 frame;
  FIXED dustX;

  layer = &((struct MapGridScrollState *)gMapGridState[0])->layers[2];
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  column = leader->x.part[0].integer;
  leaderZ = leader->z.part[0].integer;
  leader->y.fixed = 0;
  if (column >= 308 && column <= 315 && leaderZ >= 532 && leaderZ < 540) {
    leader->y.fixed = -toFIXED(2);
    if (!Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
      Event_Start();
      Sound_Play(SE_RATTLE);
      GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED);
      MapGrid_CopyCellsTo(26, 33, 19, 33, 1, 1);
      Event_Wait(30);
      Sound_Play(SE_HEAVY_MOVE);
      MapGrid_SetShake(1, 1, 1);
      Event_Wait(20);
      dustX = toFIXED(288);
      column = 29;
      tileDelay = 40;
      for (frame = 0; frame <= 479; frame++, tileDelay--) {
        layer->offsetX += toFIXED(1) / 5;
        dustX -= toFIXED(1) / 5;
        options.priority = 2;
        options.startScaleX =
            (Random_Below(3)) * (FIXED_ONE / 5) + 4 * FIXED_ONE / 5;
        options.startScaleY =
            (Random_Below(3)) * (FIXED_ONE / 5) + 4 * FIXED_ONE / 5;
        options.spin = FIELD_EFFECT_RANDOM_SPIN();
        FieldEffect_Spawn(
            dustX, 0, toFIXED(528), 0, -toFIXED((gFrameCount & 1) * 3), 0,
            EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE | EFFECT_USE_SPIN,
            &options);
        if (frame == 240) {
          dustX -= toFIXED(48);
        }
        if (tileDelay == 0) {
          tileDelay = 40;
          if (frame <= 240) {
            column -= 4;
            MapGrid_CopyCellsTo(column, 50, 15, 32, 3, 4);
          } else {
            column += 4;
            MapGrid_CopyCellsTo(column, 45, 9, 32, 3, 4);
          }
        }
        wait(1);
      }
      layer->offsetX += toFIXED(1) / 2;
      layer->offsetX = toFIXED(layer->offsetX / toFIXED(1));
      MapGrid_CopyCellAttributes(15, 32, 3, 1, 9, 32);
      MapGrid_CopyCellAttributes(12, 32, 3, 1, 15, 32);
      Sound_Play(SE_STOP_ACTION_SE);
      Sound_Play(SE_THUMP);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      MapGrid_WaitForShakeToSettle();
      gFieldState->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
      Import_EventSetExitRequest(
          VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST);
      Event_End();
    }
  }
}

/// The cell of the socket west of the second floor's gate pillar, the dark
/// square in the gold track.
#define VENUS_GATE_PILLAR_SOCKET_COLUMN 46
#define VENUS_GATE_PILLAR_SOCKET_ROW 45

/// Isaac Moves the grey pillar carrying a straight piece of gold track, in the
/// second floor's room of the lady statue and the pink gate (the pillar starts
/// at cell (48, 44)). Once it is in the dark socket in the track (cell (46,
/// 45)) it sinks in with a thump, a light runs from the lady statue along the
/// track and over the pillar to the gate, and the gate opens.
///
/// That happens the first time only (FLAG_VENUS_LIGHTHOUSE_GATE_OPEN).
static void VenusLighthouse_MoveGatePillar(void) {
  s32 row;
  s32 column;
  struct Actor *occlusionEffect = 0;
  struct Actor *pillar = Actor_Get(ACTOR_GATE_PILLAR);
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  Event_Start();
  MapGrid_CopyCellAttributes(109, 43, 7, 5, 45, 43);
  if ((pillar->priorityFlags & ACTOR_PRIORITY_UNDERFOOT) != 0) {
    MapGrid_CopyCellAttributes(45, 45, 1, 1, 46, 45);
  } else {
    MapGrid_CopyCellAttributes(48, 42, 1, 1, FIELD_CELL(pillar->x.fixed),
                               FIELD_CELL(pillar->z.fixed));
  }
  column = FIELD_CELL(pillar->x.fixed);
  if (column == VENUS_GATE_PILLAR_SOCKET_COLUMN) {
    row = FIELD_CELL(pillar->z.fixed);
    if (row == VENUS_GATE_PILLAR_SOCKET_ROW) {
      // In its socket, it sinks in, the first time only.
      if (!GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN)) {
        if (FIELD_CELL(leader->z.fixed) <= VENUS_GATE_PILLAR_SOCKET_ROW) {
          occlusionEffect = VenusLighthouse_CreateOccludingPassageEffect(
              toFIXED(744), 0, toFIXED(704), SPRITE_CHEST);
          Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
        }
        Actor_SetSpriteFlags(Actor_Get(ACTOR_GATE_PILLAR), 0);
        pillar->layer = 0;
        pillar->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        pillar->gravity = toFIXED(1) / 10;
        pillar->bounce = 0;
        MapGrid_CopyCellAttributes(43, 45, 1, 1, column, row);
        Actor_WaitUntilIdle(pillar);
        Sound_Play(SE_THUMP);
        pillar->motionFlags = 0;
        pillar->y.fixed = -toFIXED(16);
        Actor_SetSpritePriority(ACTOR_GATE_PILLAR, 3);
        pillar->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        MapGrid_CopyCellAttributes(45, 45, 1, 1, column, row);
        Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |=
            ACTOR_PRIORITY_AUTOMATIC;
        Actor_Release(occlusionEffect);
        pillar->collisionFlags = 0;
        pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        Actor_Get(ACTOR_GATE_SPARKLE)->collisionFlags = 0;
        Actor_Get(ACTOR_GATE_SPARKLE)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(ACTOR_GATE_SPARKLE, 808, 728);
        Actor_SetScript(ACTOR_GATE_SPARKLE, gVenusSecondFloorLightToGate);
        Actor_WaitForScriptEndById(ACTOR_GATE_SPARKLE);
        Event_Wait(30);
        Sound_Play(SE_DOOR_OPEN);
        MapGrid_PlayMapEdit(gVenusSecondFloorGateCells, 110, 41);
        MapGrid_CopyCellAttributes(46, 41, 1, 1, column, 42);
        GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN);
      }
    }
  }
  Event_End();
}

/// Isaac walks into the second floor's track pillar, or the socket, and
/// pushes the pillar a cell, as Move would, or leaps a one-cell gap if the
/// far side is clear.
static void VenusLighthouse_JumpOrPushGatePillar(void) {
  Event_Start();
  if (VenusLighthouse_JumpAcrossGap() == 0) {
    MapGrid_CopyCellAttributes(109, 43, 7, 5, 45, 43);
    VenusLighthouse_PushPillarAhead();
  }
  Event_End();
  VenusLighthouse_MoveGatePillar();
}

/// Isaac Moves one of the three grey pillars carrying pieces of gold track,
/// in the third floor's room of the lady statue and four sockets. A pillar
/// that reaches a socket sinks in. With all three sunk, two lights run from
/// the statue along the track, and the gate opens if one of them reaches it.
///
/// Which three of the four sockets they fill decides where the track
/// carries the lights (gVenusThirdFloorLightToNortheast and
/// gVenusThirdFloorLightToSoutheast), and the flags remember which socket was
/// left empty, the south-east or the north-west.
static void VenusLighthouse_MoveSwitchPillar(void) {
  struct Actor *occlusionEffect = 0;
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  struct Actor *pillar;
  struct Actor *other;
  struct Actor *firstGateEffect;
  struct Actor *secondGateEffect;
  u32 pillarId;
  u32 candidateIndex;
  u32 socketIndex;
  s32 savedPriority;

  Event_Start();
  MapGrid_CopyCellAttributes(108, 39, 13, 7, 44, 39);
  for (pillarId = ACTOR_FIRST_SWITCH_PILLAR;
       pillarId <= ACTOR_FIRST_SWITCH_PILLAR + 2; pillarId++) {
    pillar = Actor_Get(pillarId);
    if (pillar->priorityFlags != ACTOR_PRIORITY_UNDERFOOT) {
      MapGrid_CopyCellAttributes(47, 39, 1, 1, FIELD_CELL(pillar->x.fixed),
                                 FIELD_CELL(pillar->z.fixed));
    } else {
      MapGrid_CopyCellAttributes(46, 39, 1, 1, FIELD_CELL(pillar->x.fixed),
                                 FIELD_CELL(pillar->z.fixed));
    }
    socketIndex = NO_THIRD_FLOOR_SOCKET;
    for (candidateIndex = 0; candidateIndex <= 3; candidateIndex++) {
      if (FIELD_CELL(pillar->x.fixed) ==
              gVenusThirdFloorSockets[candidateIndex].x &&
          FIELD_CELL(pillar->z.fixed) ==
              gVenusThirdFloorSockets[candidateIndex].z &&
          pillar->y.fixed >= 0) {
        socketIndex = candidateIndex;
        break;
      }
    }
    if (socketIndex == NO_THIRD_FLOOR_SOCKET) {
      continue;
    }
    for (candidateIndex = ACTOR_FIRST_SWITCH_PILLAR;
         candidateIndex <= ACTOR_FIRST_SWITCH_PILLAR + 2; candidateIndex++) {
      other = Actor_Get(candidateIndex);
      if (pillarId != candidateIndex &&
          FIELD_CELL(pillar->x.fixed) == FIELD_CELL(other->x.fixed) &&
          FIELD_CELL(pillar->z.fixed) == FIELD_CELL(other->z.fixed)) {
        socketIndex = NO_THIRD_FLOOR_SOCKET;
        break;
      }
    }
    if (socketIndex == NO_THIRD_FLOOR_SOCKET) {
      continue;
    }
    savedPriority = leader->sprite->image.priority;
    if (FIELD_CELL(leader->z.fixed) <= gVenusThirdFloorSockets[socketIndex].z) {
      occlusionEffect = VenusLighthouse_CreateOccludingPassageEffect(
          pillar->x.fixed, pillar->y.fixed, pillar->z.fixed - toFIXED(4),
          SPRITE_CHEST);
      Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    }
    Actor_SetSpriteFlags(Actor_Get(pillarId), 0);
    pillar->layer = 0;
    pillar->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    pillar->bounce = 0;
    pillar->gravity = toFIXED(1) / 10;
    MapGrid_CopyCellAttributes(42, 41, 1, 1,
                               gVenusThirdFloorSockets[socketIndex].x,
                               gVenusThirdFloorSockets[socketIndex].z);
    Actor_WaitUntilIdle(pillar);
    Sound_Play(SE_THUMP);
    pillar->collisionFlags = 0;
    pillar->motionFlags = 0;
    pillar->y.fixed = -toFIXED(16);
    Actor_SetSpritePriority(pillarId, 3);
    pillar->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(46, 39, 1, 1,
                               gVenusThirdFloorSockets[socketIndex].x,
                               gVenusThirdFloorSockets[socketIndex].z);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, savedPriority);
    Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    if (occlusionEffect != NULL) {
      Actor_Release(occlusionEffect);
    }
    if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN)) {
      Event_End();
      return;
    }
    if ((Actor_Get(ACTOR_FIRST_SWITCH_PILLAR)->priorityFlags &
         Actor_Get(ACTOR_FIRST_SWITCH_PILLAR + 1)->priorityFlags &
         Actor_Get(ACTOR_FIRST_SWITCH_PILLAR + 2)->priorityFlags &
         ACTOR_PRIORITY_UNDERFOOT) == 0) {
      continue;
    }
    // With all three sunk, two lights run from the statue along the track.
    firstGateEffect =
        VenusLighthouse_CreateLight(888, 680, gVenusThirdFloorLightToNortheast);
    secondGateEffect =
        VenusLighthouse_CreateLight(888, 680, gVenusThirdFloorLightToSoutheast);
    // The gate opens as soon as either light reaches it
    // (SW_VENUS_LIGHT_ARRIVED), while the other may still be running.
    while ((firstGateEffect)->script != 0 || (secondGateEffect)->script != 0) {
      if (firstGateEffect->sceneByte != 0 || secondGateEffect->sceneByte != 0) {
        Event_Wait(30);
        Sound_Play(SE_DOOR_OPEN);
        MapGrid_PlayMapEdit(gVenusThirdFloorGateCells, 109, 37);
        MapGrid_CopyCellAttributes(45, 37, 1, 1, 45, 38);
        if (FIELD_CELL(Actor_Get(ACTOR_FIRST_SWITCH_PILLAR)->x.fixed) ==
                gVenusThirdFloorSockets[0].x &&
            FIELD_CELL(Actor_Get(ACTOR_FIRST_SWITCH_PILLAR)->z.fixed) ==
                gVenusThirdFloorSockets[0].z) {
          GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_SOUTHEAST_SOCKET_EMPTY);
        } else {
          GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_NORTHWEST_SOCKET_EMPTY);
        }
        GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN);
        break;
      }
      wait(1);
    }
  }
  Event_End();
}

/// Isaac walks into one of the third floor's three track pillars and pushes
/// it a cell, as Move would, or leaps a one-cell gap if the far side is
/// clear.
static void VenusLighthouse_JumpOrPushSwitchPillar(void) {
  Event_Start();
  if (VenusLighthouse_JumpAcrossGap() == 0) {
    MapGrid_CopyCellAttributes(108, 39, 13, 7, 44, 39);
    VenusLighthouse_PushPillarAhead();
  }
  Event_End();
  VenusLighthouse_MoveSwitchPillar();
}

/// A block set down on a balance platform rides it up and down, 16 pixels
/// above the platform or the block under it.
static UNREAD_RESULT
VenusLighthouse_FollowSupportingPillarHeight(struct Actor *pillar) {
  pillar->y.fixed = Actor_Get(pillar->timer)->y.fixed + toFIXED(16);
  RETURN_UNREAD(0);
}

/// The two balance platforms of the fourth floor's west room sink and rise
/// to the heights their loads give them, with a grinding sound. A platform
/// that ends between the floor and 30 pixels below it can be stepped on.
static void VenusLighthouse_LowerFloatingPillars(s32 waitFrames) {
  struct Actor *westSupport = Actor_Get(ACTOR_BALANCE_WEST);
  struct Actor *eastSupport = Actor_Get(ACTOR_BALANCE_EAST);
  u32 pillarIndex;

  Actor_SetSpeed(ACTOR_BALANCE_WEST, 0.5);
  Actor_SetSpeed(ACTOR_BALANCE_EAST, 0.5);
  if (waitFrames != 0) {
    Sound_Play(SE_SETTLE);
  }
  FieldActor_SetMoveTargetFX(westSupport, westSupport->x.fixed,
                             gBalance_PlatformHeights[westSupport->timer],
                             westSupport->z.fixed);
  FieldActor_SetMoveTargetFX(eastSupport, eastSupport->x.fixed,
                             gBalance_PlatformHeights[eastSupport->timer],
                             eastSupport->z.fixed);
  Actor_WaitForMoveById(ACTOR_BALANCE_WEST);
  Actor_WaitForMoveById(ACTOR_BALANCE_EAST);
  westSupport->y.fixed = gBalance_PlatformHeights[westSupport->timer];
  eastSupport->y.fixed = gBalance_PlatformHeights[eastSupport->timer];
  if (waitFrames != 0) {
    Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  }
  // The two platforms and the first three floating pillars: the fourth,
  // ACTOR_FIRST_FLOATING_PILLAR + 3, is left out.
  // XXX: whether that shows in play, the fourth block's cell not opened when
  // it ends in reach; a recording of the four blocks set down with the fourth
  // last, on top of a stack that ends between the floor and 30 pixels below
  // it, settles it.
  for (pillarIndex = 0; pillarIndex < 5; pillarIndex++) {
    struct Actor *actor = Actor_Get(pillarIndex + ACTOR_BALANCE_WEST);

    if (actor->y.fixed / toFIXED(1) < 0 && actor->y.fixed / toFIXED(1) > -30) {
      MapGrid_CopyCellAttributes(4, 19, 1, 1, FIELD_CELL(actor->x.fixed),
                                 FIELD_CELL(actor->z.fixed));
    }
  }
  Event_Wait(waitFrames);
}

/// Sets a block Carry has dropped on a balance platform on top of whatever is
/// highest in its cell: the platform, or a block already on it.
static void VenusLighthouse_StackFloatingPillar(s32 pillarId) {
  struct Actor *candidate;
  FIXED supportHeight = -toFIXED(80);
  struct Actor *pillar = 0;
  u32 candidateIndex;

  for (candidateIndex = 0; candidateIndex <= 5; candidateIndex++) {
    s32 candidateId = candidateIndex + ACTOR_BALANCE_WEST;
    FIXED candidateTop;

    if (candidateId == pillarId) {
      continue;
    }

    candidate = Actor_Get(candidateId);
    pillar = Actor_Get(pillarId);

    if (FIELD_CELL(candidate->x.fixed) != FIELD_CELL(pillar->x.fixed) ||
        (FIELD_CELL(candidate->z.fixed)) != FIELD_CELL(pillar->z.fixed)) {
      continue;
    }

    candidateTop = candidate->y.fixed + toFIXED(16);
    if (supportHeight <= candidateTop) {
      pillar->timer = candidateId;
      supportHeight = candidateTop;
    }
  }

  Actor_SetSpeed(pillarId, 4);
  FieldActor_SetMoveTargetFX(pillar, pillar->x.fixed, supportHeight,
                             pillar->z.fixed);
  Actor_WaitForMoveById(pillarId);
  Sound_Play(SE_THUMP);
  FieldEffect_SpawnNineRadialEffects(pillarId);
  Event_Wait(30);
}

/// Stops a floating block where it is.
static inline void VenusLighthouse_SettleFloatingPillar(struct Actor *pillar) {
  pillar->targetY = ACTOR_NO_TARGET;
  pillar->groundHeight = 0;
  pillar->velocityY = 0;
  pillar->motionFlags = 0;
  pillar->timer = 0;
}

/// Copies all three fixed-point position components without changing motion.
static inline void VenusLighthouse_CopyPosition(struct Actor *destination,
                                                struct Actor *source) {
  destination->x.fixed = source->x.fixed;
  destination->y.fixed = source->y.fixed;
  destination->z.fixed = source->z.fixed;
}

/// Isaac lifts one of the four grey spiral-marked blocks floating in the
/// fourth floor's west room with Carry and drops it onto one of the two
/// balance platforms at the south of the room. The platforms sink and rise
/// to the blocks' weight; one that ends between the floor and 30 pixels below
/// it can be stepped on (VenusLighthouse_LowerFloatingPillars).
///
/// A block dropped on a platform stays there (FLAG_SCENE_TEMP_00 plus its
/// number), and the camera looks south at the platforms as they move.
static void VenusLighthouse_CarryFloatingPillar(void) {
  struct Actor savedPosition;
  struct Actor *firstPassageEffect = NULL;
  struct Actor *secondPassageEffect = NULL;
  struct Actor *pillar;
  struct Actor *other;
  s32 pillarIndex;
  s32 earlierIndex;
  s32 settledIndex;
  s32 column;
  s32 row;

  Event_Start();
  for (pillarIndex = 0; pillarIndex < 4; pillarIndex++) {
    pillar = Actor_Get(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR);
    column = FIELD_CELL(pillar->x.fixed);
    if (column == 13 && (row = FIELD_CELL(pillar->z.fixed)) == 7 &&
        !GameFlag_Get(FLAG_SCENE_TEMP_00 + pillarIndex)) {
      Actor_WaitUntilIdle(pillar);
      GameFlag_Set(FLAG_SCENE_TEMP_00 + pillarIndex);
      pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      pillar->collisionFlags = 0;
      pillar->motionFlags = 0;
      MapGrid_CopyCellAttributes(4, 19, 1, 1, column, row);
      break;
    }
    if (pillar->sprite->image.priority == 3 &&
        !Event_GetFlag(FLAG_SCENE_TEMP_00 + pillarIndex)) {
      // Default: 0 until the settling search below sets it.
      earlierIndex = 0;
      Actor_SetSpritePriority(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR, 1);
      pillar->bounce = 0;
      if (FIELD_CELL(pillar->z.fixed) <= 12) {
        firstPassageEffect = VenusLighthouse_CreateBlendedPassageEffect(
            pillar->x.fixed, 0, toFIXED(224), SPRITE_CAPPED_PILLAR);
        secondPassageEffect = VenusLighthouse_CreateBlendedPassageEffect(
            pillar->x.fixed, 0, toFIXED(240), SPRITE_CAPPED_PILLAR);
      }
      Actor_WaitUntilIdle(pillar);
      Actor_SetPosition(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR, 0, 0);
      Actor_Release(firstPassageEffect);
      Actor_Release(secondPassageEffect);
      GameFlag_Set(FLAG_SCENE_TEMP_00 + pillarIndex);
      break;
    }
    if (FIELD_CELL(pillar->z.fixed) == 19 &&
        !GameFlag_Get(FLAG_SCENE_TEMP_00 + pillarIndex)) {
      VenusLighthouse_SettleFloatingPillar(pillar);
      settledIndex = pillarIndex;
      for (earlierIndex = 0; earlierIndex < pillarIndex; earlierIndex++) {
        if (!GameFlag_Get(FLAG_SCENE_TEMP_00 + earlierIndex)) {
          other = Actor_Get(earlierIndex + ACTOR_FIRST_FLOATING_PILLAR);
          VenusLighthouse_CopyPosition(&savedPosition, pillar);
          VenusLighthouse_CopyPosition(pillar, other);
          VenusLighthouse_CopyPosition(other, &savedPosition);
          settledIndex = earlierIndex;
          break;
        }
      }
      other = Actor_Get(settledIndex + ACTOR_FIRST_FLOATING_PILLAR);
      VenusLighthouse_SettleFloatingPillar(other);
      FieldCamera_SetSpeed(3);
      FieldCamera_GetActor()->motionFlags = 0;
      FieldCamera_MoveTo(136, 8, 344, 1);
      FieldCamera_WaitForMove();
      // Stack the pillar, then rebalance the two supports and attach the height
      // follower.
      VenusLighthouse_StackFloatingPillar(settledIndex +
                                          ACTOR_FIRST_FLOATING_PILLAR);
      if (FIELD_CELL(
              Actor_Get(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)->x.fixed) ==
          6) {
        Actor_Get(ACTOR_BALANCE_WEST)->timer++;
        Actor_Get(ACTOR_BALANCE_EAST)->timer--;
      } else {
        Actor_Get(ACTOR_BALANCE_WEST)->timer--;
        Actor_Get(ACTOR_BALANCE_EAST)->timer++;
      }
#ifdef UBFIX
      Actor_Get(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)->update =
          VenusLighthouse_FollowSupportingPillarHeight;
#else
      Actor_Get(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)->update = (void (*)(
          struct Actor *))VenusLighthouse_FollowSupportingPillarHeight;
#endif
      VenusLighthouse_LowerFloatingPillars(40);
      Actor_Get(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      GameFlag_Set(FLAG_SCENE_TEMP_00 + settledIndex);
      break;
    }
  }
  Event_End();
}

/// Isaac walks into one of the floating blocks in the fourth floor's west room
/// and pushes it a cell.
static void VenusLighthouse_PushPuzzlePillar(void) {
  Event_Start();
  VenusLighthouse_PushPillarAhead();
  Event_End();
}

/// Isaac steps onto the pink floor switch on the west side of the fourth
/// floor's big middle room (cell (33, 20)). The first time, a wall slides
/// aside in a cloud of dust, and the view cuts to the third floor's room of
/// three sand slides, where sand pours down the west one
/// (VenusLighthouse_PourWestSandSlide).
static void VenusLighthouse_StepOnWestWallSwitch(void) {
  struct EffectOptions options;
  struct MapGridLayerScroll *layer;
  struct Actor *leader;
  s32 column;
  s32 leaderZ;
  s32 tileDelay;
  u32 frame;
  FIXED dustX;

  layer = &((struct MapGridScrollState *)gMapGridState[0])->layers[2];
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  column = leader->x.part[0].integer;
  leaderZ = leader->z.part[0].integer;
  leader->y.fixed = 0;
  if (column >= 532 && column <= 539 && leaderZ >= 324 && leaderZ < 332) {
    leader->y.fixed = -toFIXED(2);
    if (!GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
      Event_Start();
      MapGrid_CopyCellsTo(63, 29, 33, 20, 1, 1);
      Sound_Play(SE_RATTLE);
      MapGrid_CopyCellsTo(44, 83, 44, 80, 3, 3);
      Event_Wait(30);
      MapGrid_SetShake(1, 1, 1);
      Sound_Play(SE_HEAVY_MOVE);
      Event_Wait(20);
      dustX = toFIXED(616);
      column = 0;
      tileDelay = 60;
      for (frame = 0; frame <= 319; frame++, tileDelay--) {
        layer->offsetX += toFIXED(1) / 5;
        if (dustX >= toFIXED(576) && frame > 40) {
          dustX -= toFIXED(1) / 5;
          options.priority = 2;
          options.startScaleX =
              (Random_Below(3)) * (FIXED_ONE / 5) + 4 * FIXED_ONE / 5;
          options.startScaleY =
              (Random_Below(3)) * (FIXED_ONE / 5) + 4 * FIXED_ONE / 5;
          options.spin = FIELD_EFFECT_RANDOM_SPIN();
          FieldEffect_Spawn(
              dustX, 0, toFIXED(288), 0, -toFIXED((gFrameCount & 1) * 3), 0,
              EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE | EFFECT_USE_SPIN,
              &options);
          if (tileDelay == 0) {
            tileDelay = 40;
            column += 4;
            MapGrid_CopyCellsTo(column, 56, 36, 17, 3, 4);
          }
        }
        wait(1);
      }
      layer->offsetX += toFIXED(1) / 2;
      layer->offsetX = toFIXED(layer->offsetX / toFIXED(1));
      Sound_Play(SE_STOP_ACTION_SE);
      Sound_Play(SE_THUMP);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      MapGrid_WaitForShakeToSettle();
      gFieldState->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
      Import_EventSetExitRequest(
          VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST);
      Event_End();
    }
  }
}

/// Isaac steps onto the pink floor switch on the east side of the fourth
/// floor's big middle room (cell (49, 20)), and as with the west one sand
/// pours down the third floor's east slide
/// (VenusLighthouse_PourEastSandSlide).
static void VenusLighthouse_StepOnEastWallSwitch(void) {
  struct EffectOptions options;
  struct MapGridLayerScroll *layer;
  struct Actor *leader;
  s32 column;
  s32 leaderZ;
  s32 tileDelay;
  u32 frame;
  FIXED dustX;

  layer = &((struct MapGridScrollState *)gMapGridState[0])->layers[2];
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  column = leader->x.part[0].integer;
  leaderZ = leader->z.part[0].integer;
  leader->y.fixed = 0;
  if (column >= 788 && column <= 795 && leaderZ >= 324 && leaderZ < 332) {
    leader->y.fixed = -toFIXED(2);
    if (!GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
      Event_Start();
      MapGrid_CopyCellsTo(63, 29, 49, 20, 1, 1);
      Sound_Play(SE_RATTLE);
      Event_Wait(30);
      MapGrid_SetShake(1, 1, 1);
      Sound_Play(SE_HEAVY_MOVE);
      Event_Wait(20);
      dustX = toFIXED(704);
      column = 61;
      tileDelay = 60;
      for (frame = 0; frame <= 319; frame++, tileDelay--) {
        layer->offsetX -= toFIXED(1) / 5;
        dustX += toFIXED(1) / 5;
        if (dustX >= toFIXED(712) && dustX < toFIXED(752)) {
          options.priority = 2;
          options.startScaleX =
              (Random_Below(3)) * (FIXED_ONE / 5) + 4 * FIXED_ONE / 5;
          options.startScaleY =
              (Random_Below(3)) * (FIXED_ONE / 5) + 4 * FIXED_ONE / 5;
          options.spin = FIELD_EFFECT_RANDOM_SPIN();
          FieldEffect_Spawn(
              dustX, 0, toFIXED(288), 0, -toFIXED((gFrameCount & 1) * 3), 0,
              EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE | EFFECT_USE_SPIN,
              &options);
          if (tileDelay == 0) {
            tileDelay = 40;
            column -= 4;
            MapGrid_CopyCellsTo(column, 56, 44, 17, 3, 4);
          }
        }
        wait(1);
      }
      layer->offsetX += toFIXED(1) / 2;
      layer->offsetX = toFIXED(layer->offsetX / toFIXED(1));
      Sound_Play(SE_STOP_ACTION_SE);
      Sound_Play(SE_THUMP);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      MapGrid_WaitForShakeToSettle();
      gFieldState->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
      Import_EventSetExitRequest(
          VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST);
      Event_End();
    }
  }
}

/// Isaac Moves one of the four grey pillars carrying pieces of gold track, in
/// the fourth floor's south-west room, where the track runs from the lady
/// statue up to the pink gate. Each pillar that reaches one of the eight
/// sockets sinks in, and once the four are sunk the light runs from the
/// statue, opening the gate when it reaches it
/// (FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN).
static void VenusLighthouse_MoveSocketPillar(void) {
  struct Actor *occlusionEffect;
  struct Actor *pillar;
  struct Actor *otherPillar;
  struct Actor *leader;
  struct Actor *firstPillar;
  struct Actor *secondPillar;
  struct Actor *thirdPillar;
  struct Actor *fourthPillar;
  u32 pillarId;
  u32 candidateIndex;
  u32 socketIndex;
  u32 savedPriority;

  occlusionEffect = 0;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
#if TBS_PATCH_3
  Field_EndBusyAndReveal();
#endif
  MapGrid_CopyCellAttributes(69, 48, 4, 2, 5, 48);
  MapGrid_CopyCellAttributes(73, 37, 9, 13, 9, 37);
  for (pillarId = ACTOR_FIRST_SOCKET_PILLAR;
       pillarId <= ACTOR_FIRST_SOCKET_PILLAR + 3; pillarId++) {
    pillar = Actor_Get(pillarId);
    if (pillar->priorityFlags != ACTOR_PRIORITY_UNDERFOOT)
      MapGrid_CopyCellAttributes(72, 48, 1, 1, FIELD_CELL(pillar->x.fixed),
                                 FIELD_CELL(pillar->z.fixed));
    else
      MapGrid_CopyCellAttributes(73, 48, 1, 1, FIELD_CELL(pillar->x.fixed),
                                 FIELD_CELL(pillar->z.fixed));

    socketIndex = NO_FOURTH_FLOOR_SOCKET;
    for (candidateIndex = 0; candidateIndex < NO_FOURTH_FLOOR_SOCKET;
         candidateIndex++) {
      if (FIELD_CELL(pillar->x.fixed) ==
              gVenusFourthFloorSockets[candidateIndex].x &&
          (FIELD_CELL(pillar->z.fixed)) ==
              gVenusFourthFloorSockets[candidateIndex].z &&
          pillar->y.fixed >= 0) {
        socketIndex = candidateIndex;
        break;
      }
    }
    if (socketIndex == NO_FOURTH_FLOOR_SOCKET)
      continue;
    for (candidateIndex = ACTOR_FIRST_SOCKET_PILLAR;
         candidateIndex <= ACTOR_FIRST_SOCKET_PILLAR + 3; candidateIndex++) {
      otherPillar = Actor_Get(candidateIndex);
      if (pillarId != candidateIndex &&
          (FIELD_CELL(pillar->x.fixed)) == FIELD_CELL(otherPillar->x.fixed) &&
          (FIELD_CELL(pillar->z.fixed)) == FIELD_CELL(otherPillar->z.fixed)) {
        socketIndex = NO_FOURTH_FLOOR_SOCKET;
        break;
      }
    }
    if (socketIndex == NO_FOURTH_FLOOR_SOCKET)
      continue;

    savedPriority = leader->sprite->image.priority;
    if (FIELD_CELL(leader->z.fixed) <=
        gVenusFourthFloorSockets[socketIndex].z) {
      occlusionEffect = VenusLighthouse_CreateOccludingPassageEffect(
          pillar->x.fixed, pillar->y.fixed, pillar->z.fixed - toFIXED(4),
          SPRITE_CHEST);
      Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    }
    for (candidateIndex = ACTOR_FIRST_SOCKET_PILLAR;
         candidateIndex <= ACTOR_FIRST_SOCKET_PILLAR + 3; candidateIndex++) {
      otherPillar = Actor_Get(candidateIndex);
      if (pillarId != candidateIndex &&
          (FIELD_CELL(pillar->x.fixed)) == FIELD_CELL(otherPillar->x.fixed) &&
          (FIELD_CELL(pillar->z.fixed)) - 1 ==
              (FIELD_CELL(otherPillar->z.fixed)))
        Actor_SetSpritePriority(candidateIndex, 3);
    }
    Actor_SetSpriteFlags(Actor_Get(pillarId), 0);
    pillar->layer = 0;
    pillar->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    pillar->gravity = toFIXED(1) / 10;
    pillar->bounce = 0;
    MapGrid_CopyCellAttributes(6, 44, 1, 1,
                               gVenusFourthFloorSockets[socketIndex].x,
                               gVenusFourthFloorSockets[socketIndex].z);
    Actor_WaitUntilIdle(pillar);
    Sound_Play(SE_THUMP);
    pillar->collisionFlags = 0;
    pillar->motionFlags = 0;
    pillar->y.fixed = -toFIXED(16);
    Actor_SetSpritePriority(pillarId, 3);
    pillar->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(73, 48, 1, 1,
                               gVenusFourthFloorSockets[socketIndex].x,
                               gVenusFourthFloorSockets[socketIndex].z);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, savedPriority);
    Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    for (candidateIndex = ACTOR_FIRST_SOCKET_PILLAR;
         candidateIndex <= ACTOR_FIRST_SOCKET_PILLAR + 3; candidateIndex++) {
      otherPillar = Actor_Get(candidateIndex);
      if (pillarId != candidateIndex &&
          (FIELD_CELL(pillar->x.fixed)) == FIELD_CELL(otherPillar->x.fixed) &&
          (FIELD_CELL(pillar->z.fixed)) - 1 ==
              (FIELD_CELL(otherPillar->z.fixed))) {
        Actor_SetSpritePriority(candidateIndex, 1);
        Actor_Get(candidateIndex)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      }
    }
    Actor_Release(occlusionEffect);
    if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN)) {
      Event_End();
      return;
    }
    // With all four sunk, the lights run from the statue to the gate.
    firstPillar = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR);
    secondPillar = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR + 1);
    thirdPillar = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR + 2);
    fourthPillar = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR + 3);
    if ((firstPillar->priorityFlags & secondPillar->priorityFlags &
         thirdPillar->priorityFlags & fourthPillar->priorityFlags) &
        ACTOR_PRIORITY_UNDERFOOT) {
      struct Actor *firstGateEffect;
      struct Actor *secondGateEffect;

      FieldCamera_SetSpeed(1);
      FieldCamera_MoveToActorById(ACTOR_SOCKET_STATUE, 1);
      FieldCamera_WaitForMove();
      firstGateEffect = VenusLighthouse_CreateLight(
          136, 776, gVenusFourthFloorLightFromStatue);
      Event_Wait(30);
      FieldCamera_SetSpeed(0.4);
      FieldCamera_MoveTo(216, CAMERA_KEEP, 632, 1);
      Actor_WaitForScriptEnd(firstGateEffect);
      Actor_StartScript(firstGateEffect, gVenusFourthFloorLightToSoutheast);
      secondGateEffect = VenusLighthouse_CreateLight(
          216, 760, gVenusFourthFloorLightToSouthwest);
      while ((firstGateEffect)->script != 0 ||
             (secondGateEffect)->script != 0) {
        if (firstGateEffect->sceneByte != 0 ||
            secondGateEffect->sceneByte != 0) {
          Event_Wait(30);
          MapGrid_AnimateCells(gVenusFourthFloorGateCells, 77, 35);
          MapGrid_CopyCellAttributes(13, 35, 1, 1, 13, 36);
          GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN);
          break;
        }
        wait(1);
      }
    }
  }
  Event_End();
}

/// Isaac walks into one of the fourth floor's four track pillars and pushes it
/// a cell, as Move would, or leaps a one-cell gap if the far side is
/// clear.
static void VenusLighthouse_JumpOrPushSocketPillar(void) {
  Event_Start();
  if (VenusLighthouse_JumpAcrossGap() == 0) {
    MapGrid_CopyCellAttributes(69, 48, 4, 2, 5, 48);
    MapGrid_CopyCellAttributes(73, 37, 9, 13, 9, 37);
    VenusLighthouse_PushPillarAhead();
  }
  Event_End();
  VenusLighthouse_MoveSocketPillar();
}

/// Isaac walks south into the gap in the floor of the fourth floor's last
/// room (cell (32, 44)) and drops through it. He lands on a small round
/// platform, which carries him up to the Lighthouse Aerie
/// (VenusLighthouse_RaiseLeaderInSparkles).
static void VenusLighthouse_EnterRisingPassage(void) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 520, 712);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  (void)VenusLighthouse_CreateBlendedPassageEffect(toFIXED(520), 0,
                                                   toFIXED(784), SPRITE_CRATE);
  FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
  Event_Wait(60);
  Import_EventSetExitRequest(
      VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES_RISING);
  Event_End();
}

/// Selects the puzzle interactions and dialogue for the current lighthouse
/// scene.
const struct Trigger *Scene_GetTriggers(void) {
  s32 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_VENUS_LIGHTHOUSE_ENTRY) {
    return gVenusLighthouseEntryTriggers;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_GROUND_FLOOR) {
    return gVenusLighthouseGroundFloorTriggers;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_FIRST_FLOOR) {
    return gVenusLighthouseFirstFloorTriggers;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_SECOND_FLOOR) {
    return gVenusLighthouseSecondFloorTriggers;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_THIRD_FLOOR) {
    return gVenusLighthouseThirdFloorTriggers;
  }
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_FOURTH_FLOOR) {
    return gVenusLighthouseFourthFloorTriggers;
  }
  return gVenusLighthouseGroundFloorTriggers;
}

/// Lays one of the scholars or soldiers the wanderers hurt on the ground
/// (ANIM_HURT_LYING), drawn behind the backgrounds' fronts, still, and neither
/// solid nor pushable.
static void VenusLighthouse_LayDownHurtActor(s32 actorId) {
  struct Actor *actor;

  actor = Actor_Get(actorId);
  actor->motionFlags = 0;
  actor->collisionFlags &= ~(ACTOR_SOLID | ACTOR_PUSHABLE);
  Actor_SetSpriteFlags(actor, 0);
  Actor_SelectAnimation(actor, ANIM_HURT_LYING);
  Actor_SetSpritePriority(actorId, 3);
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
}

/// A task that looks up the fourth floor's four socket pillars each frame and
/// does nothing with them.
static void VenusLighthouse_VisitPillarActors(void) {
  u32 actorId;

  for (actorId = ACTOR_FIRST_SOCKET_PILLAR;
       actorId <= ACTOR_FIRST_SOCKET_PILLAR + 3; actorId++) {
    // DEAD: each pillar is looked up and dropped; the ROM keeps the calls.
    Actor_Get(actorId);
  }
}

/// Restores puzzle progress, actors and entrance effects for all six scenes,
/// by scene and entrance.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 sceneId;
  u32 pillarIndex;
  s32 column;
  s32 row;

  Event_Wait(1);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
    VenusLighthouse_RestoreMarkedColors();
  GameFlag_Set(FLAG_IN_VENUS_LIGHTHOUSE);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  sceneId = gMainRoutineState.next;
  if (sceneId == SCENE_VENUS_LIGHTHOUSE_ENTRY) {
    gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (!GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD))
      VenusLighthouse_LayDownHurtActor(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK);
    else
      MapGrid_CopyCellAttributes(7, 17, 2, 1, 7, 16);
    VenusLighthouse_LayDownHurtActor(ACTOR_SCHOLAR_LIGHTHOUSE_OPEN);
    VenusLighthouse_LayDownHurtActor(ACTOR_SCHOLAR_BARBARIANS);
    VenusLighthouse_LayDownHurtActor(ACTOR_WAITING_SCHOLAR);
    Actor_SetSpritePriority(ACTOR_WAITING_SCHOLAR, 2);
    VenusLighthouse_LayDownHurtActor(ACTOR_SOLDIER_OPENED_DOOR);
    Actor_SetSpritePriority(ACTOR_SOLDIER_OPENED_DOOR, 2);
    VenusLighthouse_LayDownHurtActor(ACTOR_SOLDIER_HEADED_FOR_BABI);
    VenusLighthouse_LayDownHurtActor(ACTOR_SOLDIER_COLOSSO_VICTORS);
  } else if (sceneId == SCENE_VENUS_LIGHTHOUSE_GROUND_FLOOR) {
    switch (gMainRoutineState.entrance) {
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_ENTRY:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_FROM_RELIEFS:
      VenusLighthouse_LayDownHurtActor(ACTOR_SCHOLAR_ATTACKED_ANYWAY);
      break;
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_WEST:
      GameFlag_Clear(FLAG_ARRIVAL_STAIRS_UP);
      // The other scholar setup still applies.
      // fall through
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_FRONT_ROOM:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_EAST:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_PILLARS:
      VenusLighthouse_LayDownHurtActor(ACTOR_SCHOLAR_NOT_THE_ENTRANCE);
      break;
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_FLOOR_SWITCH:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_SMALL_ROOM:
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH)) {
        VenusLighthouse_DrawFirstPath();
      } else {
        if (!GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
          break;
        VenusLighthouse_DrawSecondPath();
      }
      break;
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_GREAT_STATUE:
      GameFlag_Clear(FLAG_ARRIVAL_STAIRS_DOWN);
      // fall through
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_PILLARS:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR:
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
      // The cells VenusLighthouse_MovePillarOntoSwitch copies as it sets the
      // flag, redrawn on arrival.
      if (GameFlag_Get(FLAG_SCENE_TEMP_00))
        MapGrid_CopyCellsTo(44, 117, 41, 117, 3, 5);
      break;
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_RELIEFS:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_CARRY_STONE:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_FLOOR_SWITCH:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_NORTH:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_WEST:
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED)) {
        Actor_SetPosition(ACTOR_WALL_PILLAR, 872, 88);
        actor = Actor_Get(ACTOR_WALL_PILLAR);
        actor->y.fixed = -toFIXED(24);
        actor->targetY = ACTOR_NO_TARGET;
      }
      break;
    }
  } else if (sceneId == SCENE_VENUS_LIGHTHOUSE_FIRST_FLOOR) {
    switch (gMainRoutineState.entrance) {
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR:
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST:
      VenusLighthouse_MoveFirstFloorSandFall();
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR_HOLE:
      Scene_QueueMusic(SE_AMBIENT_WATER);
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        VenusLighthouse_LandWithRadialDust();
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_DRIFTING_FLOOR:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_LADDERS:
      actor = Actor_Get(ACTOR_FIRST_CARRY_PILLAR);
      VenusLighthouse_MoveCarryPillar();
      if (FIELD_CELL(actor->x.fixed) == 8)
        VenusLighthouse_FixPillarAtDestination(actor);
      actor = Actor_Get(ACTOR_FIRST_CARRY_PILLAR + 1);
      if (FIELD_CELL(actor->x.fixed) == 7)
        VenusLighthouse_FixPillarAtDestination(actor);
      VenusLighthouse_CreateBlendedPassageEffect(toFIXED(206), 0, toFIXED(449),
                                                 SPRITE_CRATE);
      VenusLighthouse_CreateBlendedPassageEffect(toFIXED(210), 0, toFIXED(449),
                                                 SPRITE_CRATE);
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_FROM_LADDERS:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST:
      FieldEffect_StartGroundMask(0);
      wait(2);
      actor = Actor_Get(ACTOR_FIRST_CAPPED_PILLAR);
      actor->motionFlags = 0;
      actor->update = Actor_UpdateBlockDrawOrder;
      actor = Actor_Get(ACTOR_FIRST_CAPPED_PILLAR + 1);
      actor->motionFlags = 0;
      actor->update = Actor_UpdateBlockDrawOrder;
      actor = Actor_Get(ACTOR_FIRST_CAPPED_PILLAR + 2);
      actor->motionFlags = 0;
      actor->update = Actor_UpdateBlockDrawOrder;
      VenusLighthouse_MoveCappedPillar();
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_GAP:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_THUNDER_CROWN:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PILLARS:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PSYNERGY_STONE:
      Scene_QueueMusic(SE_AMBIENT_WATER);
      FieldEffect_StartGroundMask(0);
      wait(2);
      // With the wall on the floor above moved, the sand that poured down the
      // middle of this room has stopped: the fall is drawn as the dry stone
      // chute it ran down, and the cells under it are copied in afresh.
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
        MapGrid_CopyCellsTo(111, 5, 117, 5, 5, 2);
        MapGrid_CopyCellsTo(111, 10, 117, 10, 5, 2);
        MapGrid_CopyCellsTo(111, 7, 111, 5, 5, 2);
        MapGrid_CopyCellsTo(111, 7, 111, 10, 5, 2);
        MapGrid_CopyCells(48, 3, 3, 10, 54, 3);
        MapGrid_CopyCells(55, 26, 3, 10, 48, 3);
      }
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_LOWER_ROOM:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_GAP:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_THUNDER_CROWN:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR:
      Scene_QueueMusic(SE_AMBIENT_WATER);
      break;
    }
  } else if (sceneId == SCENE_VENUS_LIGHTHOUSE_SECOND_FLOOR) {
    switch (gMainRoutineState.entrance) {
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_FROM_PILLAR_PIT:
      VenusLighthouse_ArriveThroughSparkPassage();
      Scene_QueueMusic(SE_AMBIENT_WATER);
      break;
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST:
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR:
      FieldEffect_StartGroundMask(0);
      break;
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_WALL_SWITCH:
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_PILLAR_PIT:
      actor = Actor_Get(ACTOR_GATE_STATUE);
      actor->motionFlags = 0;
      actor->y.fixed = 0;
      actor = Actor_Get(ACTOR_GATE_PILLAR);
      actor->motionFlags = 0;
      actor->collisionFlags = 0;
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN)) {
        wait(1);
        MapGrid_CopyCellsTo(124, 41, 110, 41, 1, 2);
        MapGrid_CopyCellAttributes(46, 41, 1, 1, 46, 42);
        Actor_SetPosition(ACTOR_GATE_PILLAR, 744, 728);
        actor->motionFlags = 0;
        actor->y.fixed = -toFIXED(16);
        Actor_SetSpritePriority(ACTOR_GATE_PILLAR, 3);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        MapGrid_CopyCellAttributes(45, 45, 1, 1, 46, 45);
        Actor_SetAnimation(ACTOR_GATE_SPARKLE, PARTICLE_ANIMATION_FLICKER);
        Actor_SetSpritePriority(ACTOR_GATE_SPARKLE, 1);
        actor = Actor_Get(ACTOR_GATE_SPARKLE);
        actor->collisionFlags = 0;
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(ACTOR_GATE_SPARKLE, 743, 696);
        actor->update = (void (*)(struct Actor *))VenusLighthouse_TrailLight;
      }
      VenusLighthouse_MoveGatePillar();
      break;
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH:
      gFieldState->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
      Actor_Get(ACTOR_PARTY_LEADER)->y.fixed = -toFIXED(2);
      // It then redraws the moved wall as the others do.
      // fall through
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST:
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_FROM_GATE:
      Scene_QueueMusic(SE_AMBIENT_WATER);
      BlendScript_Suspend();
      Io_Write16(0, &REG_BLDCNT);
      // With the wall moved, the stone wall and the fall of sand in the
      // recess have changed sides: the wall stands to the west and the sand
      // pours where it stood, to the east.
      if (Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
        MapGrid_CopyCellsTo(15, 96, 9, 96, 3, 3);
        MapGrid_CopyCellsTo(12, 96, 15, 96, 3, 3);
        MapGrid_CopyCellsTo(5, 50, 15, 32, 3, 4);
        MapGrid_CopyCellsTo(25, 45, 9, 32, 3, 4);
        MapGrid_CopyCellAttributes(15, 32, 3, 1, 9, 32);
        MapGrid_CopyCellAttributes(12, 32, 3, 1, 15, 32);
      }
      if (gMainRoutineState.entrance !=
          VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH)
        break;
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      gFieldState->transition = SCENE_TRANSITION(
          TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
      break;
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH:
      Scene_QueueMusic(SE_AMBIENT_WATER);
      break;
    }
  } else if (sceneId == SCENE_VENUS_LIGHTHOUSE_THIRD_FLOOR) {
    switch (gMainRoutineState.entrance) {
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST:
      VenusLighthouse_PourWestSandSlide();
      break;
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SPARKLING_PASSAGES:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SOUTHWEST_ROOM:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_PILLAR_SOCKETS:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST:
      Scene_QueueMusic(SE_AMBIENT_WATER);
      // With the west wall of the floor above moved, sand runs down the dry
      // stone chute west of the middle one; the east wall does the same for
      // the chute to its east.
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
        MapGrid_CopyCells(53, 12, 3, 13, 26, 12);
        MapGrid_CopyCellsTo(81, 41, 89, 14, 9, 2);
        wait(1);
        Task_AddFrame(VenusLighthouse_PuffWestSlideHead, TASK_PASS_FRAME);
      }
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
        MapGrid_CopyCells(58, 12, 3, 13, 34, 12);
        MapGrid_CopyCellsTo(81, 41, 97, 14, 5, 2);
        wait(1);
        Task_AddFrame(VenusLighthouse_PuffEastSlideHead, TASK_PASS_FRAME);
      }
      if (gMainRoutineState.entrance ==
          VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SOUTHWEST_ROOM)
        VenusLighthouse_ArriveThroughSparkPassage();
      else if (
          gMainRoutineState.entrance ==
          VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST)
        VenusLighthouse_PourEastSandSlide();
      break;
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_NORTHWEST_ROOM:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRAGON_SCALES:
      VENUS_LIGHTHOUSE_ARRIVE_THROUGH_SPARK_PASSAGE();
      Scene_QueueMusic(SE_AMBIENT_WATER);
      break;
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_DRIFTING_FLOOR:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_WEST_ROOM:
      actor = Actor_Get(ACTOR_SWITCH_STATUE);
      actor->motionFlags = 0;
      actor->y.fixed = 0;
      Actor_Get(ACTOR_FIRST_SWITCH_PILLAR)->motionFlags = 0;
      Actor_Get(ACTOR_FIRST_SWITCH_PILLAR + 1)->motionFlags = 0;
      Actor_Get(ACTOR_FIRST_SWITCH_PILLAR + 2)->motionFlags = 0;
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN)) {
        wait(1);
        MapGrid_CopyCellsTo(111, 59, 109, 37, 1, 2);
        MapGrid_CopyCellAttributes(45, 37, 1, 1, 45, 38);
        if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_SOUTHEAST_SOCKET_EMPTY)) {
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR, 776, 664);
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR + 1, 840, 664);
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR + 2, 776, 696);
        } else {
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR, 840, 664);
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR + 1, 776, 696);
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR + 2, 840, 696);
        }
        Actor_SetSpritePriority(ACTOR_FIRST_SWITCH_PILLAR, 3);
        actor = Actor_Get(ACTOR_FIRST_SWITCH_PILLAR);
        actor->y.fixed = -toFIXED(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetSpritePriority(ACTOR_FIRST_SWITCH_PILLAR + 1, 3);
        actor = Actor_Get(ACTOR_FIRST_SWITCH_PILLAR + 1);
        actor->y.fixed = -toFIXED(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetSpritePriority(ACTOR_FIRST_SWITCH_PILLAR + 2, 3);
        actor = Actor_Get(ACTOR_FIRST_SWITCH_PILLAR + 2);
        actor->y.fixed = -toFIXED(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetAnimation(ACTOR_SWITCH_GATE_SPARKLE,
                           PARTICLE_ANIMATION_FLICKER);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_SWITCH_GATE_SPARKLE), 0);
        Actor_SetSpritePriority(ACTOR_SWITCH_GATE_SPARKLE, 1);
        actor = Actor_Get(ACTOR_SWITCH_GATE_SPARKLE);
        actor->collisionFlags = 0;
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(ACTOR_SWITCH_GATE_SPARKLE, 727, 632);
        actor->update = (void (*)(struct Actor *))VenusLighthouse_TrailLight;
      }
      VenusLighthouse_MoveSwitchPillar();
      break;
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRIFTING_FLOOR:
      Scene_QueueMusic(SE_AMBIENT_WATER);
      break;
    }
  } else if (sceneId == SCENE_VENUS_LIGHTHOUSE_FOURTH_FLOOR) {
    switch (gMainRoutineState.entrance) {
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_FROM_WALL_SWITCHES:
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        VenusLighthouse_LowerFloatingPillars(0);
        Actor_WaitUntilIdle(Actor_Get(ACTOR_PARTY_LEADER));
        for (pillarIndex = 0; pillarIndex <= 3; pillarIndex++) {
          actor = Actor_Get(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR);
          column = FIELD_CELL(actor->x.fixed);
          if (column == 13) {
            row = FIELD_CELL(actor->z.fixed);
            if (row == 7 && GameFlag_Get(pillarIndex + FLAG_SCENE_TEMP_00)) {
              actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
              actor->collisionFlags = 0;
              actor->motionFlags = 0;
              MapGrid_CopyCellAttributes(4, 19, 1, 1, column, row);
              break;
            }
          }
        }
      } else {
        actor = Actor_Get(ACTOR_BALANCE_WEST);
        actor->motionFlags = 0;
        actor->y.fixed = -toFIXED(48);
        actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        actor->collisionFlags &= ~ACTOR_SOLID;
        actor->timer = 3;
        Actor_SetSpritePriority(ACTOR_BALANCE_WEST, 1);
        actor = Actor_Get(ACTOR_BALANCE_EAST);
        actor->motionFlags = 0;
        actor->y.fixed = -toFIXED(48);
        actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        actor->collisionFlags &= ~ACTOR_SOLID;
        actor->timer = 3;
        Actor_SetSpritePriority(ACTOR_BALANCE_EAST, 1);
        actor = Actor_Get(ACTOR_FIRST_FLOATING_PILLAR);
        actor->motionFlags = 0;
        actor->timer = 0;
        Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_FLOATING_PILLAR), 0);
        actor = Actor_Get(ACTOR_FIRST_FLOATING_PILLAR + 1);
        actor->motionFlags = 0;
        actor->timer = 0;
        Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_FLOATING_PILLAR + 1), 0);
        actor = Actor_Get(ACTOR_FIRST_FLOATING_PILLAR + 2);
        actor->motionFlags = 0;
        actor->timer = 0;
        Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_FLOATING_PILLAR + 2), 0);
        actor = Actor_Get(ACTOR_FIRST_FLOATING_PILLAR + 3);
        actor->motionFlags = 0;
        actor->timer = 0;
        Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_FLOATING_PILLAR + 3), 0);
      }
      break;
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_WEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_EAST:
      Scene_QueueMusic(SE_AMBIENT_WATER);
      break;
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST:
      gFieldState->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
      Actor_Get(ACTOR_PARTY_LEADER)->y.fixed = -toFIXED(2);
      // fall through
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLARS:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_EAST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_WEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_EAST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_SOUTH:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING:
      Actor_Get(ACTOR_RISING_PLATFORM)->motionFlags = ACTOR_FLOATS;
      Actor_Get(ACTOR_RISING_PLATFORM)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      Actor_Get(ACTOR_RISING_PLATFORM)->y.fixed = -(toFIXED(33) / 2);
      BlendScript_Suspend();
      Io_Write16(0, &REG_BLDCNT);
      // With a wall of this room moved, it has slid out of its recess and sand
      // pours from behind it: the west wall here, the east wall below. That
      // sand is what runs down the third floor's chutes.
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
        Scene_QueueMusic(SE_AMBIENT_WATER);
        MapGrid_CopyCellsTo(36, 81, 32, 81, 3, 2);
        MapGrid_CopyCellsTo(36, 83, 36, 81, 3, 2);
        MapGrid_CopyCellAttributes(36, 17, 3, 1, 32, 17);
        MapGrid_CopyCellAttributes(36, 18, 3, 1, 36, 17);
        MapGrid_CopyCellsTo(63, 29, 33, 20, 1, 1);
        MapGrid_CopyCellsTo(20, 56, 36, 17, 3, 4);
      }
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
        MapGrid_CopyCellsTo(44, 81, 48, 81, 3, 2);
        MapGrid_CopyCellsTo(44, 83, 44, 81, 3, 2);
        MapGrid_CopyCellAttributes(44, 17, 3, 1, 48, 17);
        MapGrid_CopyCellAttributes(44, 18, 3, 1, 44, 17);
        MapGrid_CopyCellsTo(63, 29, 49, 20, 1, 1);
        MapGrid_CopyCellsTo(41, 56, 44, 17, 3, 4);
      }
      if (gMainRoutineState.entrance ==
              VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST ||
          gMainRoutineState.entrance ==
              VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST) {
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        gFieldState->transition = SCENE_TRANSITION(
            TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
      }
      if (gMainRoutineState.entrance ==
          VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING)
        VenusLighthouse_RaiseLeaderInSparkles();
      break;
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_SOUTH:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_EAST:
      wait(1);
      Task_AddFrame(VenusLighthouse_VisitPillarActors, TASK_PASS_FRAME);
      actor = Actor_Get(ACTOR_SOCKET_STATUE);
      actor->motionFlags = 0;
      actor->y.fixed = 0;
      Actor_Get(ACTOR_FIRST_SOCKET_PILLAR)->motionFlags = 0;
      Actor_Get(ACTOR_FIRST_SOCKET_PILLAR + 1)->motionFlags = 0;
      Actor_Get(ACTOR_FIRST_SOCKET_PILLAR + 2)->motionFlags = 0;
      Actor_Get(ACTOR_FIRST_SOCKET_PILLAR + 3)->motionFlags = 0;
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN)) {
        wait(1);
        MapGrid_CopyCellsTo(95, 56, 77, 35, 1, 2);
        MapGrid_CopyCellAttributes(13, 35, 1, 1, 13, 36);
        Actor_SetPosition(ACTOR_FIRST_SOCKET_PILLAR, 264, 744);
        actor = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR);
        actor->y.fixed = -toFIXED(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR, 3);
        Actor_SetPosition(ACTOR_FIRST_SOCKET_PILLAR + 1, 184, 632);
        actor = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR + 1);
        actor->y.fixed = -toFIXED(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR + 1, 3);
        Actor_SetPosition(ACTOR_FIRST_SOCKET_PILLAR + 2, 232, 696);
        actor = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR + 2);
        actor->y.fixed = -toFIXED(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR + 2, 3);
        Actor_SetPosition(ACTOR_FIRST_SOCKET_PILLAR + 3, 184, 664);
        actor = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR + 3);
        actor->y.fixed = -toFIXED(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR + 3, 3);
        Actor_SetAnimation(ACTOR_SOCKET_GATE_SPARKLE,
                           PARTICLE_ANIMATION_FLICKER);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_SOCKET_GATE_SPARKLE), 0);
        Actor_SetSpritePriority(ACTOR_SOCKET_GATE_SPARKLE, 1);
        actor = Actor_Get(ACTOR_SOCKET_GATE_SPARKLE);
        actor->collisionFlags = 0;
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(ACTOR_SOCKET_GATE_SPARKLE, 215, 600);
        actor->update = (void (*)(struct Actor *))VenusLighthouse_TrailLight;
      }
      VenusLighthouse_MoveSocketPillar();
      break;
    }
  }
  return 0;
}

/// Isaac steps onto sand flowing north in the fourth floor's sand maze, and
/// it carries him north a cell at a time in puffs of sand. Where it runs over
/// a ledge he drops down.
static void VenusLighthouse_DriftNorth(void) {
  struct Vector3 args;

  args.x = 0;
  args.z = -1;
  VenusLighthouse_StepActorWithDust(&args);
}

/// Isaac steps onto a slide of flowing sand, and it carries him south a cell
/// at a time in puffs of sand until he reaches firm floor: down the middle of
/// the first floor's big room (cells (34, 8) to (50, 33)), the third floor's
/// slides, and the south-flowing sand of the fourth floor's maze.
static void VenusLighthouse_DriftSouth(void) {
  struct Vector3 args;

  args.x = 0;
  args.z = 1;
  VenusLighthouse_StepActorWithDust(&args);
}

/// Isaac steps onto sand flowing west in the fourth floor's sand maze, and it
/// carries him west a cell at a time in puffs of sand.
static void VenusLighthouse_DriftWest(void) {
  struct Vector3 args;

  args.x = -1;
  args.z = 0;
  VenusLighthouse_StepActorWithDust(&args);
}

/// Isaac steps onto sand flowing east in the fourth floor's sand maze, and it
/// carries him east a cell at a time in puffs of sand.
static void VenusLighthouse_DriftEast(void) {
  struct Vector3 args;

  args.x = 1;
  args.z = 0;
  VenusLighthouse_StepActorWithDust(&args);
}

/// Carries the leader along on flowing sand, a little at a time, kicking up
/// puffs of sand, and stops him at a wall or drops him down a ledge.
/// @param direction The way the drifting floor carries the party: -1, 0 or 1
/// on x and on z; y is not read.
static void VenusLighthouse_StepActorWithDust(struct Vector3 *direction) {
  s32 actorId;
  struct Actor *viewCenter;
  struct Actor *actor;
  struct FieldPosition nextPosition;
  struct EffectOptions options;
  s32 collision;
  s32 framePhase;
  FIXED particleX;

  {
    struct FieldState *state = gFieldState;

    actorId = gMainRoutineState.leaderActorId;
    viewCenter = state->camera;
  }
  actor = Actor_Get(actorId);
  nextPosition.x =
      actor->x.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
  nextPosition.y = actor->y.fixed;
  nextPosition.z =
      actor->z.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
  collision = FieldActor_CheckMovementCollision(actor, &nextPosition);
  framePhase = gFrameCount & 4;
  if (framePhase == 0) {
    options.spin = FIELD_EFFECT_RANDOM_SPIN();
    particleX = actor->x.fixed + toFIXED(Random_Below(12)) - toFIXED(6);
    FieldEffect_Spawn(
        particleX, actor->y.fixed, actor->z.fixed,
        ((Random_Below(5)) * (toFIXED(1) / 10) + 5 * (toFIXED(1) / 10)) *
            direction->x,
        framePhase,
        direction->z *
            ((Random_Below(5)) * (toFIXED(1) / 10) + 5 * (toFIXED(1) / 10)),
        EFFECT_USE_SPIN, &options);
  }
  // Where the sand runs over a ledge he drops down it; a wall beside him
  // nudges him along it.
  if (collision < 0) {
    Actor_SetAttachedEffect(actorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldActor_SetMoveTargetFX(actor, actor->x.fixed, actor->y.fixed,
                               actor->z.fixed + toFIXED(8));
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    Actor_WaitForMove(actor);
    do {
      wait(1);
    } while (actor->y.fixed != actor->groundHeight);
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(3);
    return;
  }
  nextPosition.x =
      actor->x.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x, 19);
  nextPosition.y = actor->y.fixed;
  nextPosition.z =
      actor->z.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z, 19);
  collision = FieldActor_CheckMovementCollision(actor, &nextPosition);
  if (collision > 0) {
    return;
  }
  nextPosition.x = actor->x.fixed + direction->x * (toFIXED(57) / 10) -
                   direction->z * (toFIXED(57) / 10);
  nextPosition.y = actor->y.fixed;
  nextPosition.z = actor->z.fixed + direction->z * (toFIXED(57) / 10) -
                   direction->x * (toFIXED(57) / 10);
  collision = FieldActor_CheckMovementCollision(actor, &nextPosition);
  if (collision > 0) {
    actor->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
    actor->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
    return;
  }
  nextPosition.x =
      (direction->x + direction->z) * (toFIXED(57) / 10) + actor->x.fixed;
  nextPosition.y = actor->y.fixed;
  nextPosition.z =
      (direction->x + direction->z) * (toFIXED(57) / 10) + actor->z.fixed;
  collision = FieldActor_CheckMovementCollision(actor, &nextPosition);
  if (collision > 0) {
    actor->x.fixed -= VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
    actor->z.fixed -= VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
    return;
  }
  viewCenter->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
  actor->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
  viewCenter->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
  actor->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
}

/// Chooses the size and spin of a burst of sand as a slide starts to pour.
static inline void
VenusLighthouse_SetSandParticleScaleAndSpin(struct EffectOptions *options) {
  options->startScaleX =
      (Random_Below(2)) * (FIXED_ONE * 3 / 10) + 5 * (FIXED_ONE * 3 / 10);
  options->startScaleY =
      (Random_Below(2)) * (FIXED_ONE * 3 / 10) + 5 * (FIXED_ONE * 3 / 10);
  options->spin = FIELD_EFFECT_RANDOM_SPIN();
}

/// After the second floor's switch, the view cuts to the first floor's room
/// of three doors with the party out of sight. A wave of sand pours down a new
/// fall east of the old one, row by row, and then the old fall runs dry from
/// the top down, leaving the stone chute it ran down. The party is then back
/// on the second floor beside the switch.
///
/// The return is by
/// VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH.
static void VenusLighthouse_MoveFirstFloorSandFall(void) {
  struct EffectOptions options;
  u32 extraBursts;
  u32 row;
  u32 particleIndex;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_Start();
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Scene_QueueMusic(SE_AMBIENT_WATER);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Sound_Play(SE_WAVE);
  extraBursts = 0;
  for (row = 0; row < 10; row++) {
    VenusLighthouse_SetSandParticleScaleAndSpin(&options);
    for (;;) {
      for (particleIndex = 0; particleIndex < 4 && row < 8; particleIndex++) {
        FieldEffect_Spawn(
            ((Random_Below(7)) << 19) + toFIXED(864), 0,
            toFIXED(48) + row * toFIXED(16) + particleIndex * toFIXED(4), 0, 0,
            0, EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
      }
      wait(3);
      if (row != 3 || extraBursts >= 3) {
        break;
      }
      extraBursts++;
    }
    MapGrid_CopyCellsTo(48, row + 3, 54, row + 3, 3, 1);
  }
  MapGrid_CopyCellsTo(111, 5, 117, 5, 5, 2);
  MapGrid_CopyCellsTo(111, 10, 117, 10, 5, 2);
  MapGrid_CopyCellsTo(111, 7, 111, 5, 5, 2);
  MapGrid_CopyCellsTo(111, 7, 111, 10, 5, 2);
  for (row = 0; row < 10; row++) {
    VenusLighthouse_SetSandParticleScaleAndSpin(&options);
    for (particleIndex = 0; particleIndex < 4 && row < 8; particleIndex++) {
      FieldEffect_Spawn(
          ((Random_Below(7)) << 19) + toFIXED(768), 0,
          toFIXED(48) + row * toFIXED(16) + particleIndex * toFIXED(4), 0, 0, 0,
          EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
    }
    wait(3);
    MapGrid_CopyCellsTo(55, row + 26, 48, row + 3, 3, 1);
  }
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Event_Wait(60);
  Import_EventSetExitRequest(
      VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH);
  Event_End();
}

/// Puffs of sand at the head of a sand slide that has started to pour, about
/// one frame in four.
static void VenusLighthouse_PuffSandAtSlideHead(s32 x, s32 z) {
  struct EffectOptions options;

  options.startScaleX = 7 * FIXED_ONE / 10;
  options.startScaleY = 7 * FIXED_ONE / 10;
  options.spin = FIELD_EFFECT_RANDOM_SPIN();
  if ((gFrameCount & 3) == 0) {
    if ((Random_Below(2)) != 0) {
      FieldEffect_Spawn(toFIXED(x + (Random_Below(2) << 4)), 0,
                        FIELD_HALF_CELL_ORIGIN(z), 0, 0,
                        (s32)(toFIXED(Random_Below(5)) + toFIXED(7)) / 10,
                        EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
    } else {
#ifdef BUGFIX
      FieldEffect_Spawn(toFIXED(x + Random_Below(17)), 0,
                        FIELD_HALF_CELL_ORIGIN(z) - toFIXED(4), 0, 0, 0,
                        EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
#else
      // BUG: takes x for the half-cell row, so these particles start far to
      // the south of the passage instead of just north of the others.
      FieldEffect_Spawn(toFIXED(x + Random_Below(17)), 0,
                        FIELD_HALF_CELL_ORIGIN(x) - toFIXED(4), 0, 0, 0,
                        EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
#endif
    }
  }
}

/// Tasks, each frame once a slide pours: puffs of sand at the head of the west
/// slide, and of the east.
static void VenusLighthouse_PuffWestSlideHead(void) {
  VenusLighthouse_PuffSandAtSlideHead(432, 32);
}

/// The east slide's puffs.
static void VenusLighthouse_PuffEastSlideHead(void) {
  VenusLighthouse_PuffSandAtSlideHead(560, 44);
}

/// After the fourth floor's west switch, the view cuts to the third floor's
/// room of three sand slides, where a wave of sand pours down the west slide,
/// row by row, and keeps puffing at its head. The party is then back on the
/// fourth floor.
///
/// The return is by
/// VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST.
static void VenusLighthouse_PourWestSandSlide(void) {
  struct EffectOptions options;
  u32 extraBursts;
  u32 row;
  u32 particleIndex;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_Start();
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Scene_QueueMusic(SE_AMBIENT_WATER);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Sound_Play(SE_WAVE);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 360, 1);
  // The wave holds on its fourth row for three more bursts, and the slide's
  // head starts to puff sand.
  extraBursts = 0;
  for (row = 0; row <= 12; row++) {
    VenusLighthouse_SetSandParticleScaleAndSpin(&options);
    // One burst of sand: four puffs down the row, each over a random one of
    // seven 8-pixel columns.
  burst:
    for (particleIndex = 0; particleIndex <= 3; particleIndex++) {
      FieldEffect_Spawn(
          ((Random_Below(7)) << 19) + toFIXED(416), 0,
          toFIXED(192) + row * toFIXED(16) + particleIndex * toFIXED(4), 0, 0,
          0, EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
    }
    wait(3);
    if (row == 3) {
      if (extraBursts <= 2) {
        extraBursts++;
        goto burst;
      }
      if (extraBursts == 3)
        Task_AddFrame(VenusLighthouse_PuffWestSlideHead, TASK_PASS_FRAME);
    }
    MapGrid_CopyCellsTo(53, row + 12, 26, row + 12, 3, 1);
  }
  MapGrid_CopyCellsTo(81, 41, 89, 14, 9, 2);
  FieldCamera_WaitForMove();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_Wait(60);
  GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED);
  Import_EventSetExitRequest(
      VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST);
  Event_End();
}

/// After the fourth floor's east switch, sand pours down the third floor's
/// east slide in the same way (VenusLighthouse_PourWestSandSlide).
///
/// The return is by
/// VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST.
static void VenusLighthouse_PourEastSandSlide(void) {
  struct EffectOptions options;
  u32 extraBursts;
  u32 row;
  u32 particleIndex;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_Start();
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Scene_QueueMusic(SE_AMBIENT_WATER);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Sound_Play(SE_WAVE);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveTo(568, CAMERA_KEEP, 360, 1);
  // As the western wave, but its puffing task is added again on every row.
  extraBursts = 0;
  for (row = 0; row <= 12; row++) {
    VenusLighthouse_SetSandParticleScaleAndSpin(&options);
  burst:
    for (particleIndex = 0; particleIndex <= 3; particleIndex++) {
      FieldEffect_Spawn(
          ((Random_Below(7)) << 19) + toFIXED(544), 0,
          toFIXED(192) + row * toFIXED(16) + particleIndex * toFIXED(4), 0, 0,
          0, EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
    }
    wait(3);
    if (row == 3 && extraBursts <= 2) {
      extraBursts++;
      goto burst;
    }
    Task_AddFrame(VenusLighthouse_PuffEastSlideHead, TASK_PASS_FRAME);
    MapGrid_CopyCellsTo(58, row + 12, 34, row + 12, 3, 1);
  }
  MapGrid_CopyCellsTo(86, 41, 97, 14, 5, 2);
  FieldCamera_WaitForMove();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_Wait(60);
  GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED);
  Import_EventSetExitRequest(
      VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST);
  Event_End();
}

/// Sparkles rise around the leader as the platform lifts him, one every four
/// frames.
static UNREAD_RESULT VenusLighthouse_EmitRisingSparkles(struct Actor *actor) {
  struct EffectOptions options;
  s32 framePhase;
  FIXED particleX, particleY, particleZ, riseSpeed;

  options.priority = 1;
  options.palette = SPRITE_COLOR_BRG;
  options.type = SPRITE_GLINT;
  options.script = gGlint_TwinkleScript;
  framePhase = gFrameCount & 3;
  if (framePhase == 0) {
    if ((gFrameCount & 7) == 0) {
      Sound_Play(SE_GLITTER);
    }
    particleX = actor->x.fixed + toFIXED(Random_Below(49) - 24);
    particleY = actor->y.fixed + toFIXED(Random_Below(49) - 24);
    particleZ = actor->z.fixed + toFIXED(Random_Below(49) - 24);
    riseSpeed = ((Random_Below(4)) << 15) + toFIXED(1) / 2;
    FieldEffect_Spawn(particleX, particleY, particleZ, 0, riseSpeed, framePhase,
                      EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_TYPE | EFFECT_USE_SCRIPT,
                      &options);
  }
  RETURN_UNREAD(0);
}

/// After the drop through the gap, Isaac lands on a small round platform
/// (ACTOR_RISING_PLATFORM), which rises with him in a column of sparkles until
/// the scene changes to the Lighthouse Aerie.
static void VenusLighthouse_RaiseLeaderInSparkles(void) {
  u32 riseFrame;
  struct Actor *leader;
  struct Actor *platform;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  platform = Actor_Get(ACTOR_RISING_PLATFORM);
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  // He lands on the platform, which dips under him and settles, carrying him
  // with it.
  leader->y.fixed = toFIXED(130);
  leader->gravity = toFIXED(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  Event_Wait(30);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_Wait(24);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
  platform->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
  leader->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
  leader->groundHeight += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
  wait(2);
  platform->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
  leader->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
  leader->groundHeight += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
  wait(10);
  platform->y.fixed += toFIXED(2);
  leader->y.fixed += toFIXED(2);
  leader->groundHeight += toFIXED(2);
  wait(4);
  platform->y.fixed += toFIXED(2);
  leader->y.fixed += toFIXED(2);
  leader->groundHeight += toFIXED(2);
  wait(4);
  platform->y.fixed += toFIXED(1);
  leader->y.fixed += toFIXED(1);
  leader->groundHeight += toFIXED(1);
  leader->motionFlags = 0;
  platform->motionFlags = 0;
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(40);
#ifdef UBFIX
  leader->update = VenusLighthouse_EmitRisingSparkles;
#else
  leader->update = (void *)VenusLighthouse_EmitRisingSparkles;
#endif
  Event_Wait(60);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpritePriority(ACTOR_RISING_PLATFORM, 1);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Sound_Play(SE_MATERIALIZE);
  GameFlag_Set(FLAG_SKIP_SCENE_CHANGE_SOUND);
  for (riseFrame = 0; riseFrame < 128; riseFrame++) {
    leader->y.fixed += toFIXED(1);
    leader->groundHeight += toFIXED(1);
    platform->y.fixed += toFIXED(1);
    wait(1);
  }
  Import_EventSetExitRequest(
      VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_AERIE);
}

/// A light reaching a socket runs on along the track of the pillar sunk there,
/// by its socket's path for that piece, or goes out when the piece does not
/// join the edge it comes in by.
static s32 VenusLighthouse_RouteLight(struct Actor *light) {
  struct FieldPosition pillarPosition;
  struct Actor *pillar;
  struct Sprite *sprite;
  const u8 *allowedTracks;
  u8 track;
  u32 socket;
  s32 pathIndex;
  const u8 *trackCursor;

  pillarPosition.x = light->x.fixed;
  pillarPosition.y = light->y.fixed - toFIXED(16);
  pillarPosition.z = light->z.fixed;
  pillar = Actor_FindAtPosition(&pillarPosition, 0);
  // A light does reach an empty socket: on the third floor one solution, the
  // one FLAG_VENUS_LIGHTHOUSE_SOUTHEAST_SOCKET_EMPTY records, leaves that
  // socket empty, and the light sent there always arrives. With no pillar
  // the reads below go through address 0, into the BIOS, and that light goes
  // out (gVenusLightGoesOut) while the other opens the gate.
  sprite = pillar->sprite;
  if (sprite->entries[0]->animationId == SPRITE_SPIRAL_PILLAR) {
    s32 velocityX = light->velocityX;
    s32 speedX = velocityX < 0 ? -velocityX : velocityX;
    s32 velocityZ = light->velocityZ;
    s32 speedZ = velocityZ < 0 ? -velocityZ : velocityZ;

    if (speedX > speedZ) {
      if (velocityX / toFIXED(1) < 0) {
        allowedTracks = gVenusTracksMovingWest;
      } else {
        allowedTracks = gVenusTracksMovingEast;
      }
    } else {
      if (velocityZ / toFIXED(1) < 0) {
        allowedTracks = gVenusTracksMovingNorth;
      } else {
        allowedTracks = gVenusTracksMovingSouth;
      }
    }
    for (; *allowedTracks != VENUS_TRACK_END; allowedTracks++) {
      if (sprite->lastNo == *allowedTracks) {
        break;
      }
    }
    if (*allowedTracks == VENUS_TRACK_END) {
      Actor_StartScript(light, gVenusLightGoesOut);
      return 0;
    }
    if (gMainRoutineState.next == SCENE_VENUS_LIGHTHOUSE_THIRD_FLOOR) {
      for (socket = 0;
           socket <= 3 &&
           !(FIELD_CELL(light->x.fixed) == gVenusThirdFloorSockets[socket].x &&
             FIELD_CELL(light->z.fixed) == gVenusThirdFloorSockets[socket].z);
           socket++) {
      }
      for (pathIndex = 0;; pathIndex++) {
        trackCursor = gVenusThirdFloorSocketTracks[socket];
        track = *trackCursor;
        if (track == VENUS_TRACK_END) {
          Actor_StartScript(light, gVenusLightGoesOut);
          return 0;
        }
        if (track == pillar->sprite->lastNo) {
          break;
        }
        gVenusThirdFloorSocketTracks[socket] = trackCursor + 1;
      }
      Actor_StartScript(light, gVenusThirdFloorSocketPaths[socket][pathIndex]);
    } else {
      for (socket = 0;
           socket <= 7 &&
           !(FIELD_CELL(light->x.fixed) == gVenusFourthFloorSockets[socket].x &&
             FIELD_CELL(light->z.fixed) == gVenusFourthFloorSockets[socket].z);
           socket++) {
      }
      for (pathIndex = 0;; pathIndex++) {
        trackCursor = gVenusFourthFloorSocketTracks[socket];
        track = *trackCursor;
        if (track == VENUS_TRACK_END) {
          Actor_StartScript(light, gVenusLightGoesOut);
          return 0;
        }
        if (track == pillar->sprite->lastNo) {
          break;
        }
        gVenusFourthFloorSocketTracks[socket] = trackCursor + 1;
      }
      Actor_StartScript(light, gVenusFourthFloorSocketPaths[socket][pathIndex]);
    }
  } else {
    Actor_StartScript(light, gVenusLightGoesOut);
  }
  return 0;
}

#include "shared/field_effect_duration_scripts.c"

#include "shared/pushed_actor_direction_steps.c"

/// The third floor's four sockets, by cell: northwest, northeast, southwest
/// and southeast.
struct SwitchCell gVenusThirdFloorSockets[] = {
    {48, 41},
    {52, 41},
    {48, 43},
    {52, 43},
};

// clang-format off
#include "shared/balance_heights.c"

/// The fourth floor's eight sockets, by cell: northwest, northeast, west,
/// east, far west, middle, southwest and southeast.
struct SwitchCell gVenusFourthFloorSockets[] = {
    { 11, 39 },
    { 14, 39 },
    { 11, 41 },
    { 16, 42 },
    { 10, 43 },
    { 14, 43 },
    { 10, 46 },
    { 16, 46 },
};

/// The pieces of track a light can run onto in each direction, each list ended
/// by VENUS_TRACK_END (VenusLighthouse_RouteLight).
u8 gVenusTracksMovingWest[] = {
    VENUS_TRACK_NORTH_EAST, VENUS_TRACK_WEST_EAST, VENUS_TRACK_SOUTH_EAST, VENUS_TRACK_END,
};
u8 gVenusTracksMovingEast[] = {
    VENUS_TRACK_NORTH_WEST, VENUS_TRACK_WEST_EAST, VENUS_TRACK_SOUTH_WEST, VENUS_TRACK_END,
};
u8 gVenusTracksMovingNorth[] = {
    VENUS_TRACK_NORTH_SOUTH, VENUS_TRACK_SOUTH_EAST, VENUS_TRACK_SOUTH_WEST, VENUS_TRACK_END,
};
u8 gVenusTracksMovingSouth[] = {
    VENUS_TRACK_NORTH_SOUTH, VENUS_TRACK_NORTH_WEST, VENUS_TRACK_NORTH_EAST, VENUS_TRACK_END,
};

/// The third floor's sockets' lists of the pieces they route, in the order of
/// each socket's paths (gVenusThirdFloorSocketTracks points into it).
u8 gVenusThirdFloorTrackLists[] = {
    VENUS_TRACK_WEST_EAST,  VENUS_TRACK_SOUTH_EAST, VENUS_TRACK_SOUTH_WEST, VENUS_TRACK_END, // northwest
    VENUS_TRACK_WEST_EAST,  VENUS_TRACK_SOUTH_EAST, VENUS_TRACK_SOUTH_WEST, VENUS_TRACK_END, // northeast
    VENUS_TRACK_NORTH_WEST, VENUS_TRACK_NORTH_EAST, VENUS_TRACK_WEST_EAST,  VENUS_TRACK_END, // southwest
    VENUS_TRACK_NORTH_WEST, VENUS_TRACK_NORTH_EAST, VENUS_TRACK_WEST_EAST,  VENUS_TRACK_END, // southeast
};

/// The fourth floor's sockets' lists, as gVenusThirdFloorTrackLists. Each
/// list ends where the next begins, and the northeast socket's is empty.
u8 gVenusFourthFloorTrackLists[] = {
    VENUS_TRACK_SOUTH_EAST, VENUS_TRACK_END, // northwest
    VENUS_TRACK_END,                   // northeast
    VENUS_TRACK_NORTH_EAST, VENUS_TRACK_END, // west
    VENUS_TRACK_SOUTH_WEST, VENUS_TRACK_END, // east
    VENUS_TRACK_NORTH_EAST, VENUS_TRACK_END, // far west
    VENUS_TRACK_SOUTH_EAST, VENUS_TRACK_SOUTH_WEST, VENUS_TRACK_END, // middle
    VENUS_TRACK_NORTH_SOUTH, VENUS_TRACK_END, // southwest
    VENUS_TRACK_NORTH_SOUTH, VENUS_TRACK_END, // southeast
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The lady statues' actor script. They are drawn grey (VENUS_COLOR_MARK),
/// and never pushed (SW_VENUS_IMMOVABLE).
s32 gVenusGreyStatueScript[] = {
    SC_SET(SW_TIMER, VENUS_COLOR_MARK(SPRITE_COLOR_GREY)),
    SC_SET(SW_VENUS_IMMOVABLE, 1),
    SC_CALL(VenusLighthouse_ApplyMarkedColor),
    SC_END,
};

/// The first floor's capped pillars' actor script: drawn grey.
s32 gVenusGreyPillarScript[] = {
    SC_SET(SW_TIMER, VENUS_COLOR_MARK(SPRITE_COLOR_GREY)),
    SC_CALL(VenusLighthouse_ApplyMarkedColor),
    SC_END,
};

/// The leader's actor script as the lightning strikes him
/// (VenusLighthouse_ThrowBackFromLightning). He swells and shrinks ten times,
/// flashing white and crackling (VenusLighthouse_FlashStruckLeader).
s32 gVenusLeaderShockedScript[] = {
    // Seen from behind, his arms flung wide.
    SC_ANIMATION(ANIM_LEADER_CLING),
    SC_SET(SW_HOOK, (s32)VenusLighthouse_FlashStruckLeader),
    SC_LABEL(1),
    SC_SET(SW_SCALE_X, 11 * FIXED_ONE / 10),
    SC_SET(SW_SCALE_Y, 11 * FIXED_ONE / 10),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * FIXED_ONE / 10),
    SC_SET(SW_SCALE_Y, 9 * FIXED_ONE / 10),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_END,
};

/// The actor script of the World Map's picture of Venus Lighthouse, placed
/// in the ground floor's statue room at three fifths of its size, with no
/// collision. It is a small purple tower standing among the floor lamps the
/// statue's routes light (the room's render).
s32 gVenusMiniatureLighthouseScript[] = {
    SC_SET(SW_SCALE_X, 3 * FIXED_ONE / 5),
    SC_SET(SW_SCALE_Y, 3 * FIXED_ONE / 5),
    SC_SET(SW_COLLISION, 0),
    SC_CALL(VenusLighthouse_ClearScriptedSpriteFlags),
    SC_END,
};

#include "shared/glint_twinkle_script.c"

/// The actor scripts that give a spiral pillar its piece of track.
s32 gVenusTrackNorthSouthScript[] = {
    SC_ANIMATION(VENUS_TRACK_NORTH_SOUTH),
    SC_END,
};

s32 gVenusTrackNorthWestScript[] = {
    SC_ANIMATION(VENUS_TRACK_NORTH_WEST),
    SC_END,
};

s32 gVenusTrackNorthEastScript[] = {
    SC_ANIMATION(VENUS_TRACK_NORTH_EAST),
    SC_END,
};

s32 gVenusTrackWestEastScript[] = {
    SC_ANIMATION(VENUS_TRACK_WEST_EAST),
    SC_END,
};

s32 gVenusTrackSouthEastScript[] = {
    SC_ANIMATION(VENUS_TRACK_SOUTH_EAST),
    SC_END,
};

s32 gVenusTrackSouthWestScript[] = {
    SC_ANIMATION(VENUS_TRACK_SOUTH_WEST),
    SC_END,
};

/// The second floor's light. It runs from the lady statue along the track,
/// over the sunk gate pillar, to the gate (VenusLighthouse_TrailLight trails
/// it).
s32 gVenusSecondFloorLightToGate[] = {
    SC_CALL(VenusLighthouse_ClearScriptedSpriteFlags),
    SC_ANIMATION(PARTICLE_ANIMATION_FLICKER),
    SC_SPEED(1),
    SC_SET(SW_HOOK, (s32)VenusLighthouse_TrailLight),
    SC_MOVE(760, 0, 728),
    SC_SYNC,
    SC_POSITION(760, 0, 728),
    SC_MOVE(760, 0, 744),
    SC_SYNC,
    SC_POSITION(760, 0, 744),
    SC_MOVE(744, 0, 744),
    SC_SYNC,
    SC_POSITION(743, 0, 744),
    SC_MOVE(744, 0, 696),
    SC_SYNC,
    SC_POSITION(743, 0, 696),
    SC_WAIT(1),
    SC_END,
};

/// The third floor's lights, from the lady statue to a socket, from socket to
/// socket, and from a socket to the gate, where SW_VENUS_LIGHT_ARRIVED opens
/// it. Each that reaches a socket asks VenusLighthouse_RouteLight for its
/// next path.
s32 gVenusThirdFloorLightToNortheast[] = {
    SC_MOVE(872, 0, 680),
    SC_SYNC,
    SC_POSITION(872, 0, 680),
    SC_MOVE(872, 0, 664),
    SC_SYNC,
    SC_POSITION(872, 0, 664),
    SC_MOVE(840, 0, 664),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(840)),
    SC_SET(SW_Z, toFIXED(664)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusThirdFloorLightToSoutheast[] = {
    SC_MOVE(888, 0, 696),
    SC_SYNC,
    SC_POSITION(888, 0, 696),
    SC_MOVE(840, 0, 696),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(840)),
    SC_SET(SW_Z, toFIXED(696)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

/// A light that cannot go on. It stops trailing particles and is released ten
/// frames later.
s32 gVenusLightGoesOut[] = {
    SC_SET(SW_HOOK, 0),
    SC_WAIT(10),
    SC_RELEASE,
};

s32 gVenusThirdFloorLightNorthwestToGate[] = {
    SC_MOVE(728, 0, 664),
    SC_SYNC,
    SC_POSITION(728, 0, 664),
    SC_MOVE(728, 0, 632),
    SC_SYNC,
    SC_POSITION(728, 0, 632),
    SC_SET(SW_VENUS_LIGHT_ARRIVED, 1),
    SC_END,
};

s32 gVenusThirdFloorLightNorthwestToSouthwest[] = {
    SC_MOVE(776, 0, 696),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(776)),
    SC_SET(SW_Z, toFIXED(696)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusThirdFloorLightNortheastToNorthwest[] = {
    SC_MOVE(776, 0, 664),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(776)),
    SC_SET(SW_Z, toFIXED(664)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusThirdFloorLightNortheastToSoutheast[] = {
    SC_MOVE(840, 0, 696),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(840)),
    SC_SET(SW_Z, toFIXED(696)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

// The same script as gVenusThirdFloorLightNortheastToNorthwest,
// under another name.
s32 gVenusThirdFloorLightSouthwestToNorthwest[] = {
    SC_MOVE(776, 0, 664),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(776)),
    SC_SET(SW_Z, toFIXED(664)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusThirdFloorLightSouthwestToGate[] = {
    SC_MOVE(728, 0, 696),
    SC_SYNC,
    SC_POSITION(728, 0, 696),
    SC_MOVE(728, 0, 632),
    SC_SYNC,
    SC_POSITION(728, 0, 632),
    SC_SET(SW_VENUS_LIGHT_ARRIVED, 1),
    SC_END,
};

s32 gVenusThirdFloorLightSoutheastToNortheast[] = {
    SC_MOVE(840, 0, 664),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(840)),
    SC_SET(SW_Z, toFIXED(664)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

// The same script as gVenusThirdFloorLightNorthwestToSouthwest,
// under another name.
s32 gVenusThirdFloorLightSoutheastToSouthwest[] = {
    SC_MOVE(776, 0, 696),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(776)),
    SC_SET(SW_Z, toFIXED(696)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

/// The fourth floor's lights, as the third floor's. The first runs from the
/// statue to the fork, where VenusLighthouse_MoveSocketPillar sends it on to
/// the southeast socket and a second light to the southwest one.
s32 gVenusFourthFloorLightFromStatue[] = {
    SC_MOVE(216, 0, 776),
    SC_SYNC,
    SC_POSITION(216, 0, 776),
    SC_MOVE(216, 0, 760),
    SC_SYNC,
    SC_POSITION(216, 0, 760),
    SC_END,
};

s32 gVenusFourthFloorLightToSoutheast[] = {
    SC_MOVE(264, 0, 760),
    SC_SYNC,
    SC_POSITION(264, 0, 760),
    SC_MOVE(264, 0, 744),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(264)),
    SC_SET(SW_Z, toFIXED(744)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightSoutheastToMiddle[] = {
    SC_MOVE(264, 0, 728),
    SC_SYNC,
    SC_POSITION(264, 0, 728),
    SC_MOVE(232, 0, 728),
    SC_SYNC,
    SC_POSITION(232, 0, 728),
    SC_MOVE(232, 0, 696),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(232)),
    SC_SET(SW_Z, toFIXED(696)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightMiddleToWest[] = {
    SC_MOVE(216, 0, 696),
    SC_SYNC,
    SC_POSITION(216, 0, 696),
    SC_MOVE(216, 0, 664),
    SC_SYNC,
    SC_POSITION(216, 0, 664),
    SC_MOVE(184, 0, 664),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(184)),
    SC_SET(SW_Z, toFIXED(664)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightMiddleToEast[] = {
    SC_MOVE(264, 0, 696),
    SC_SYNC,
    SC_POSITION(264, 0, 696),
    SC_MOVE(264, 0, 680),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(264)),
    SC_SET(SW_Z, toFIXED(680)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightWestToNorthwest[] = {
    SC_MOVE(184, 0, 632),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(184)),
    SC_SET(SW_Z, toFIXED(632)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightEastToNortheast[] = {
    SC_MOVE(248, 0, 680),
    SC_SYNC,
    SC_POSITION(248, 0, 680),
    SC_MOVE(248, 0, 664),
    SC_SYNC,
    SC_POSITION(248, 0, 664),
    SC_MOVE(264, 0, 664),
    SC_SYNC,
    SC_POSITION(264, 0, 664),
    SC_MOVE(264, 0, 648),
    SC_SYNC,
    SC_POSITION(264, 0, 648),
    SC_MOVE(248, 0, 648),
    SC_SYNC,
    SC_POSITION(248, 0, 648),
    SC_MOVE(248, 0, 632),
    SC_SYNC,
    SC_POSITION(248, 0, 632),
    SC_MOVE(232, 0, 632),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(232)),
    SC_SET(SW_Z, toFIXED(632)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightToSouthwest[] = {
    SC_MOVE(216, 0, 744),
    SC_SYNC,
    SC_POSITION(216, 0, 744),
    SC_MOVE(184, 0, 744),
    SC_SYNC,
    SC_POSITION(184, 0, 744),
    SC_MOVE(184, 0, 760),
    SC_SYNC,
    SC_POSITION(184, 0, 760),
    SC_MOVE(168, 0, 760),
    SC_SYNC,
    SC_POSITION(168, 0, 760),
    SC_MOVE(168, 0, 744),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(168)),
    SC_SET(SW_Z, toFIXED(744)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightSouthwestToFarWest[] = {
    SC_MOVE(168, 0, 728),
    SC_SYNC,
    SC_POSITION(168, 0, 728),
    SC_MOVE(184, 0, 728),
    SC_SYNC,
    SC_POSITION(184, 0, 728),
    SC_MOVE(184, 0, 696),
    SC_SYNC,
    SC_POSITION(184, 0, 696),
    SC_MOVE(168, 0, 696),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(168)),
    SC_SET(SW_Z, toFIXED(696)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightFarWestToWest[] = {
    SC_MOVE(168, 0, 664),
    SC_SYNC,
    SC_POSITION(168, 0, 664),
    SC_MOVE(184, 0, 664),
    SC_SYNC,
    SC_SET(SW_X, toFIXED(184)),
    SC_SET(SW_Z, toFIXED(664)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightNorthwestToGate[] = {
    SC_MOVE(216, 0, 632),
    SC_SYNC,
    SC_POSITION(216, 0, 632),
    SC_MOVE(216, 0, 600),
    SC_SYNC,
    SC_POSITION(216, 0, 600),
    SC_SET(SW_VENUS_LIGHT_ARRIVED, 1),
    SC_END,
};

/// The gates' cell animations as each opens, a cell column at a time.
u16 gVenusSecondFloorGateCells[] = {
    MAPGRID_ANIMATION_STEP(124, 50, 1, 2, 6), MAPGRID_ANIMATION_STEP(124, 47, 1, 2, 6), MAPGRID_ANIMATION_STEP(124, 44, 1, 2, 6), MAPGRID_ANIMATION_STEP(124, 41, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};
u16 gVenusThirdFloorGateCells[] = {
    MAPGRID_ANIMATION_STEP(117, 59, 1, 2, 6), MAPGRID_ANIMATION_STEP(115, 59, 1, 2, 6), MAPGRID_ANIMATION_STEP(113, 59, 1, 2, 6), MAPGRID_ANIMATION_STEP(111, 59, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};
u16 gVenusFourthFloorGateCells[] = {
    MAPGRID_ANIMATION_STEP(96, 58, 1, 2, 6), MAPGRID_ANIMATION_STEP(95, 58, 1, 2, 6), MAPGRID_ANIMATION_STEP(96, 56, 1, 2, 6), MAPGRID_ANIMATION_STEP(95, 56, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};

struct SceneEntrance gVenusLighthouseEntryEntrances[] = {
    { VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 40, 0, 264, FACING_EAST, 0, 0, 0, 512, 544, 0 },
    { VENUS_LIGHTHOUSE_ENTRY_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR, CONDITION_ALWAYS, 376, 0, 88, FACING_SOUTH, 0, 0, 0, 512, 544, 0 },
    { VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 488, 0, 424, FACING_WEST, 0, 0, 0, 512, 544, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseEntrancesOther[] = {
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_ENTRY, CONDITION_ALWAYS, 120, 0, 200, FACING_NORTH, 0, 0, 56, 240, 216, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_FROM_RELIEFS, CONDITION_ALWAYS, 120, 0, 104, FACING_SOUTH, 0, 0, 56, 240, 216, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_FRONT_ROOM, CONDITION_ALWAYS, 344, 0, 232, FACING_NORTH, 0, 256, 48, 512, 248, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_EAST, CONDITION_ALWAYS, 472, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_WEST, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_PILLARS, CONDITION_ALWAYS, 344, 0, 104, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_RELIEFS, CONDITION_ALWAYS, 840, 0, 248, FACING_NORTH, 0, 672, 32, 1008, 256, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_CARRY_STONE, CONDITION_ALWAYS, 968, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_FLOOR_SWITCH, CONDITION_ALWAYS, 856, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_NORTH, CONDITION_ALWAYS, 808, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_WEST, CONDITION_ALWAYS, 712, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_PILLARS_NORTH, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 16, 288, 256, 520, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_PILLARS_WEST, CONDITION_ALWAYS, 104, 0, 504, FACING_NORTH, 0, 16, 288, 256, 520, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_SMALL_ROOM, CONDITION_ALWAYS, 104, 0, 472, FACING_SOUTH, 0, 16, 288, 256, 520, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_FLOOR_SWITCH, CONDITION_ALWAYS, 520, 0, 696, FACING_SOUTH, 0, 304, 344, 576, 728, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_SMALL_ROOM, CONDITION_ALWAYS, 344, 0, 568, FACING_SOUTH, 0, 304, 344, 576, 728, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_PILLARS, CONDITION_ALWAYS, 728, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_FROM_STATUE_ROW, CONDITION_ALWAYS, 392, 0, 952, FACING_SOUTH, 0, 272, 808, 512, 984, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_FROM_GREAT_STATUE, CONDITION_ALWAYS, 392, 0, 856, FACING_SOUTH, 0, 272, 808, 512, 984, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_GREAT_STATUE, CONDITION_ALWAYS, 760, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_RELIEFS_WEST, CONDITION_ALWAYS, 88, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_RELIEFS_EAST, CONDITION_ALWAYS, 184, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
    { VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_PILLARS, CONDITION_ALWAYS, 184, 0, 648, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseFirstFloorEntrances[] = {
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_LOWER_ROOM, CONDITION_ALWAYS, 456, 0, 552, FACING_SOUTH, 0, 376, 344, 736, 624, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_GAP, CONDITION_ALWAYS, 456, 0, 592, FACING_NORTH, 0, 376, 344, 736, 624, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH, CONDITION_ALWAYS, 696, 0, 472, FACING_SOUTH, 0, 376, 344, 736, 624, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_THUNDER_CROWN, CONDITION_ALWAYS, 696, 0, 512, FACING_NORTH, 0, 376, 344, 736, 624, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_FROM_DRIFTING_FLOOR, CONDITION_ALWAYS, 424, 0, 136, FACING_SOUTH, 0, 344, 48, 592, 216, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_FROM_LADDERS, CONDITION_ALWAYS, 520, 0, 176, FACING_NORTH, 0, 344, 48, 592, 216, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_FROM_LADDERS, CONDITION_ALWAYS, 104, 0, 752, FACING_NORTH, 0, 56, 584, 304, 880, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST, CONDITION_ALWAYS, 200, 0, 680, FACING_SOUTH, 0, 56, 584, 304, 880, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_DRIFTING_FLOOR, CONDITION_ALWAYS, 88, 0, 344, FACING_SOUTH, 0, 40, 280, 288, 496, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_LADDERS, CONDITION_ALWAYS, 248, 0, 432, FACING_NORTH, 0, 40, 280, 288, 496, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_GAP, CONDITION_ALWAYS, 664, 0, 264, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH, CONDITION_ALWAYS, 728, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_THUNDER_CROWN, CONDITION_ALWAYS, 840, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PILLARS, CONDITION_ALWAYS, 952, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PSYNERGY_STONE, CONDITION_ALWAYS, 840, 0, 360, FACING_NORTH, 0, 616, 40, 1000, 360, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR, CONDITION_ALWAYS, 88, 0, 152, FACING_SOUTH, 0, 40, 40, 328, 208, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_FROM_LADDERS, CONDITION_ALWAYS, 184, 0, 104, FACING_SOUTH, 0, 40, 40, 328, 208, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_TUNNEL_RUINS_LADY_STATUES, CONDITION_ALWAYS, 280, 0, 152, FACING_SOUTH, 0, 40, 40, 328, 208, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR, CONDITION_ALWAYS, 472, 0, 776, FACING_SOUTH, 0, 424, 688, 672, 848, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR_HOLE, CONDITION_ALWAYS, 600, 0, 776, FACING_SOUTH, 0, 424, 688, 672, 848, 0 },
    { VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST, CONDITION_ALWAYS, 840, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseSecondFloorEntrances[] = {
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH, CONDITION_ALWAYS, 104, 0, 120, FACING_SOUTH, 0, 56, 56, 368, 272, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_FROM_PILLAR_PIT, CONDITION_ALWAYS, 312, 0, 200, FACING_SOUTH, 0, 56, 56, 368, 272, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_FROM_SPARKLING_PASSAGE, CONDITION_ALWAYS, 600, 0, 216, FACING_NORTH, 0, 552, 88, 848, 544, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST, CONDITION_ALWAYS, 696, 0, 312, FACING_SOUTH, 0, 552, 88, 848, 544, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_FROM_GATE, CONDITION_ALWAYS, 648, 0, 536, FACING_NORTH, 0, 552, 88, 848, 544, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR, CONDITION_ALWAYS, 776, 0, 536, FACING_SOUTH, 0, 552, 88, 848, 544, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST, CONDITION_ALWAYS, 104, 0, 520, FACING_SOUTH, 0, 56, 392, 352, 608, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_FROM_GATE, CONDITION_ALWAYS, 312, 0, 584, FACING_NORTH, 0, 56, 392, 352, 608, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_WALL_SWITCH, CONDITION_ALWAYS, 744, 0, 696, FACING_SOUTH, 0, 648, 632, 896, 800, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_PILLAR_PIT, CONDITION_ALWAYS, 808, 0, 696, FACING_SOUTH, 0, 648, 632, 896, 800, 0 },
    { VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH, CONDITION_ALWAYS, 312, 0, 536, FACING_SOUTH, 0, 56, 392, 352, 608, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseThirdFloorEntrances[] = {
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST, CONDITION_ALWAYS, 136, 0, 136, FACING_SOUTH, 0, 8, 56, 256, 224, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_FROM_SPARKLING_PASSAGES, CONDITION_ALWAYS, 136, 0, 184, FACING_NORTH, 0, 8, 56, 256, 224, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_FROM_SPARKLING_PASSAGES, CONDITION_ALWAYS, 136, 0, 392, FACING_NORTH, 0, 8, 264, 256, 432, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_NORTHWEST_ROOM, CONDITION_ALWAYS, 312, 0, 680, FACING_SOUTH, 0, 248, 600, 512, 768, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRAGON_SCALES, CONDITION_ALWAYS, 392, 0, 680, FACING_SOUTH, 0, 248, 600, 512, 768, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRIFTING_FLOOR, CONDITION_ALWAYS, 472, 0, 680, FACING_SOUTH, 0, 248, 600, 512, 768, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST, CONDITION_ALWAYS, 136, 0, 792, FACING_SOUTH, 0, 8, 728, 256, 928, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_FROM_DRIFTING_FLOOR, CONDITION_ALWAYS, 136, 0, 904, FACING_NORTH, 0, 8, 728, 256, 928, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SPARKLING_PASSAGES, CONDITION_ALWAYS, 312, 0, 232, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SOUTHWEST_ROOM, CONDITION_ALWAYS, 504, 0, 248, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST, CONDITION_ALWAYS, 680, 0, 120, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST, CONDITION_ALWAYS, 312, 0, 440, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_PILLAR_SOCKETS, CONDITION_ALWAYS, 632, 0, 392, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_DRIFTING_FLOOR, CONDITION_ALWAYS, 728, 0, 632, FACING_SOUTH, 0, 680, 568, 928, 768, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_WEST_ROOM, CONDITION_ALWAYS, 888, 0, 632, FACING_SOUTH, 0, 680, 568, 928, 768, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR, CONDITION_ALWAYS, 136, 0, 568, FACING_SOUTH, 0, 8, 480, 256, 648, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_FROM_PILLAR_SOCKETS, CONDITION_ALWAYS, 136, 0, 600, FACING_NORTH, 0, 8, 480, 256, 648, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST, CONDITION_ALWAYS, 440, 0, 264, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
    { VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST, CONDITION_ALWAYS, 568, 0, 264, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseFourthFloorEntrances[] = {
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST, CONDITION_ALWAYS, 120, 0, 248, FACING_SOUTH, 0, 72, 56, 320, 416, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_FROM_WALL_SWITCHES, CONDITION_ALWAYS, 280, 0, 280, FACING_NORTH, 0, 72, 56, 320, 416, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLARS, CONDITION_ALWAYS, 472, 0, 328, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST, CONDITION_ALWAYS, 536, 0, 216, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST, CONDITION_ALWAYS, 664, 0, 88, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE, CONDITION_ALWAYS, 744, 0, 152, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST, CONDITION_ALWAYS, 840, 0, 152, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_EAST, CONDITION_ALWAYS, 936, 0, 280, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST, CONDITION_ALWAYS, 472, 0, 472, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_WEST, CONDITION_ALWAYS, 616, 0, 456, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_EAST, CONDITION_ALWAYS, 728, 0, 456, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_SOUTH, CONDITION_ALWAYS, 824, 0, 456, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_WEST, CONDITION_ALWAYS, 792, 0, 648, FACING_SOUTH, 0, 712, 584, 976, 864, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_EAST, CONDITION_ALWAYS, 904, 0, 648, FACING_SOUTH, 0, 712, 584, 976, 864, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_SOUTH, CONDITION_ALWAYS, 104, 0, 776, FACING_SOUTH, 0, 56, 536, 304, 848, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_EAST, CONDITION_ALWAYS, 216, 0, 600, FACING_SOUTH, 0, 56, 536, 304, 848, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_FROM_WALL_SWITCHES, CONDITION_ALWAYS, 600, 0, 744, FACING_SOUTH, 0, 392, 648, 640, 832, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST, CONDITION_ALWAYS, 536, 0, 328, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST, CONDITION_ALWAYS, 792, 0, 328, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING, CONDITION_ALWAYS, 664, 0, 136, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { SCENE_TABLE_END },
};

u32 gVenusLighthouseExits[] = {
    SCENE_EXITS(VenusLighthouseEntry),
    SCENE_EXIT(VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_VENUS_LIGHTHOUSE_ENTRY_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_ENTRY),
    SCENE_EXIT(VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_VENUS_LIGHTHOUSE_ENTRY_WEST),
    SCENE_EXITS(VenusLighthouseGroundFloor),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_ENTRY, VenusLighthouseEntry, VENUS_LIGHTHOUSE_ENTRY_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_TO_RELIEFS, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_FRONT_ROOM),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_FRONT_ROOM, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_FROM_RELIEFS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_EAST, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_RELIEFS_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_WEST, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_RELIEFS_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_PILLARS, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_RELIEFS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_RELIEFS, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_PILLARS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_CARRY_STONE, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_FROM_PILLARS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_FLOOR_SWITCH, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_PILLARS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_NORTH, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_PILLARS_NORTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_WEST, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_PILLARS_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_NORTH, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_NORTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_WEST, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_SMALL_ROOM, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_FROM_STATUE_ROW),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_FLOOR_SWITCH, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_GREAT_STATUE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_SMALL_ROOM, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_FROM_GREAT_STATUE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_PILLARS, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_FLOOR_SWITCH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_STATUE_ROW, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_FROM_SMALL_ROOM),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_GREAT_STATUE, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_SMALL_ROOM),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_GREAT_STATUE, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_FLOOR_SWITCH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_WEST, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_EAST, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_PILLARS, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_CARRY_STONE),
    SCENE_EXITS(VenusLighthouseFirstFloor),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_GAP, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_DRIFTING_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_THUNDER_CROWN, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_FROM_DRIFTING_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_DRIFTING_FLOOR, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_THUNDER_CROWN),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_LADDERS, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_THUNDER_CROWN),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_TO_LADDERS, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PILLARS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_DRIFTING_FLOOR, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_GAP),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_LADDERS, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_GAP),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_GAP, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_LADDERS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_THUNDER_CROWN, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_FROM_LADDERS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PILLARS, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_FROM_LADDERS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PSYNERGY_STONE, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_FROM_LADDERS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR, VenusLighthouseGroundFloor, VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_TO_LADDERS, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PSYNERGY_STONE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_TUNNEL_RUINS_LADY_STATUES, TunnelRuinsLadyStatues, TUNNEL_RUINS_LADY_STATUES_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_TO_DRIFTING_FLOOR, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_LOWER_ROOM),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM_HOLE, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR_HOLE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH),
    SCENE_EXITS(VenusLighthouseSecondFloor),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_TO_PILLAR_PIT, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_FROM_SPARKLING_PASSAGE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_SPARKLING_PASSAGE, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_FROM_PILLAR_PIT),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_GATE, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_PILLAR_PIT),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_TO_GATE, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_WALL_SWITCH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_WALL_SWITCH, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_FROM_GATE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_PILLAR_PIT, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_FROM_GATE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST, VenusLighthouseFirstFloor, VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST),
    SCENE_EXITS(VenusLighthouseThirdFloor),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_TO_SPARKLING_PASSAGES, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_NORTHWEST_ROOM),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_TO_SPARKLING_PASSAGES, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRAGON_SCALES),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_NORTHWEST_ROOM, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_FROM_SPARKLING_PASSAGES),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRAGON_SCALES, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_FROM_SPARKLING_PASSAGES),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRIFTING_FLOOR, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SPARKLING_PASSAGES),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_TO_DRIFTING_FLOOR, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SOUTHWEST_ROOM),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SPARKLING_PASSAGES, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRIFTING_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SOUTHWEST_ROOM, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_FROM_DRIFTING_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_PILLAR_SOCKETS, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_DRIFTING_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_DRIFTING_FLOOR, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_PILLAR_SOCKETS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_WEST_ROOM, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_FROM_PILLAR_SOCKETS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR, VenusLighthouseSecondFloor, VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_TO_PILLAR_SOCKETS, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_WEST_ROOM),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST),
    SCENE_EXITS(VenusLighthouseFourthFloor),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_TO_WALL_SWITCHES, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLARS),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLARS, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_FROM_WALL_SWITCHES),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PSYNERGY_STONE, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_FROM_WALL_SWITCHES),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_EAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_WEST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_EAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_SOUTH, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_SOUTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_WEST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_WEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_EAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_GAIA_BLADE_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_SOUTH, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_SOUTH),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_EAST, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PILLAR_SOCKETS_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST, VenusLighthouseThirdFloor, VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES_RISING, VenusLighthouseFourthFloor, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING),
    SCENE_EXIT(VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_AERIE, VenusLighthouseAerie, VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR),
    SCENE_EXITS_END,
};

struct ActorPlacement gVenusLighthouseActorPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gVenusLighthouseEntryActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER, FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD, MOVEMENT_TYPE_STAND,
        128, 0, 268, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        352, 0, 300, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        272, 0, 428, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        368, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        400, 0, 172, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        416, 0, 412, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        192, 0, 364, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gVenusLighthouseGroundFloorActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        128, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_GLASSES, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        304, 0, 204, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        344, 0, 592, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        872, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        840, 0, 88, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        664, 0, 792, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, CONDITION_IN_VIEW, (s32)gVenusMiniatureLighthouseScript,
        488, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        72, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        72, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        168, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        168, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        72, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        104, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        136, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        168, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        200, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

struct ActorPlacement gVenusLighthouseFirstFloorActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, CONDITION_IN_VIEW, (s32)gVenusGreyPillarScript,
        248, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, CONDITION_IN_VIEW, (s32)gVenusGreyPillarScript,
        184, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, CONDITION_IN_VIEW, (s32)gVenusGreyPillarScript,
        200, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        168, 0, 392, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        88, 0, 376, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

struct ActorPlacement gVenusLighthouseSecondFloorActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        808, 0, 728, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackNorthSouthScript,
        776, 0, 712, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PARTICLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gVenusLighthouseThirdFloorActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        888, 0, 680, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackSouthEastScript,
        792, 0, 680, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackWestEastScript,
        808, 0, 648, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackNorthWestScript,
        824, 0, 680, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PARTICLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gVenusLighthouseFourthFloorActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        104, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        168, 0, 312, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        120, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        136, 0, 280, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        152, 0, 280, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        248, 0, 120, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        136, 0, 776, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackNorthSouthScript,
        184, 0, 616, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackSouthEastScript,
        200, 0, 616, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackSouthWestScript,
        232, 0, 616, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackNorthEastScript,
        248, 0, 616, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PARTICLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        664, 0, 136, FACING_NORTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct Trigger gVenusLighthouseEntryTriggers[] = {
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_EAST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_WEST },
    { TRIGGER_TALK, ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, CONDITION_ALWAYS, EVENT(VenusLighthouse_HearOfTheAttack) },
    { TRIGGER_TALK, ACTOR_SCHOLAR_LIGHTHOUSE_OPEN, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIodemBeHappyTheEntranceTo) },
    { TRIGGER_TALK, ACTOR_SCHOLAR_BARBARIANS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseOhhhHowCouldTheyAttackMere) },
    { TRIGGER_TALK, ACTOR_WAITING_SCHOLAR, CONDITION_ALWAYS, EVENT(VenusLighthouse_TalkToWaitingScholar) },
    { TRIGGER_TALK, ACTOR_SOLDIER_OPENED_DOOR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseHowHowDidTheyOpenThe) },
    { TRIGGER_TALK, ACTOR_SOLDIER_HEADED_FOR_BABI, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheyAreAlreadyHeadedForBabi) },
    { TRIGGER_TALK, ACTOR_SOLDIER_COLOSSO_VICTORS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseBabisSoldiersAreAllColossoVictors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, CONDITION_ALWAYS, EVENT(VenusLighthouse_HearOfTheAttack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_LIGHTHOUSE_OPEN, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseUhnnnIfOnlyICouldMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_BARBARIANS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheirPowersWereSoStrangeWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WAITING_SCHOLAR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIMustGetInsideVenusLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_OPENED_DOOR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheYoungBoySeemedToGlow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_HEADED_FOR_BABI, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheyWerentInVenusLighthouseVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_COLOSSO_VICTORS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIThoughtIRecognizedOneOf) },
    { TRIGGER_TABLE_END },
};

struct Trigger gVenusLighthouseGroundFloorTriggers[] = {
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_ENTRY, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_ENTRY },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_TO_RELIEFS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_TO_RELIEFS },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_FRONT_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_FRONT_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_EAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_WEST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_PILLARS },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_RELIEFS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_RELIEFS },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_CARRY_STONE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_CARRY_STONE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_FLOOR_SWITCH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_FLOOR_SWITCH },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_NORTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_NORTH },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_WEST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_NORTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_NORTH },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_SMALL_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_SMALL_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_FLOOR_SWITCH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_FLOOR_SWITCH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_SMALL_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_SMALL_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_PILLARS },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_STATUE_ROW, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_STATUE_ROW },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_GREAT_STATUE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_GREAT_STATUE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_GREAT_STATUE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_GREAT_STATUE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_EAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_PILLARS },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_LIGHTNING, FLAG_SCENE_TEMP_00, EVENT(VenusLighthouse_ThrowBackFromLightning) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOOR_SWITCH, CONDITION_ALWAYS, EVENT(VenusLighthouse_StepOnFloorSwitch) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_WALL_PILLAR, CONDITION_ALWAYS, EVENT(VenusLighthouse_PushPillarTowardWall) },
    { TRIGGER_TALK, ACTOR_SCHOLAR_ATTACKED_ANYWAY, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenToldThemToStopBut) },
    { TRIGGER_TALK, ACTOR_SCHOLAR_NOT_THE_ENTRANCE, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenWentInsideButThenCame) },
    { TRIGGER_TALK, ACTOR_DOOR_SCHOLAR, CONDITION_ALWAYS, EVENT(VenusLighthouse_TalkToScholarByDoor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_ATTACKED_ANYWAY, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIfKradenHadntBeenThereWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_NOT_THE_ENTRANCE, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenSaidSomethingAboutThisBeing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOOR_SCHOLAR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIWonderWhatMysteriesTheStatue) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF, CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief) },
#if LANGUAGE == LANGUAGE_JAPANESE
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 1, CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 2, CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 3, CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 4, CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief) },
#endif
#if !TBS_PATCH_1 || TBS_PATCH_2
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 5, CONDITION_ALWAYS, EVENT(VenusLighthouse_ReadRelief) },
#endif
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_GREAT_STATUE, CONDITION_ALWAYS, EVENT(VenusLighthouse_SearchStatue) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_CHEST, FLAG_VENUS_LIGHTHOUSE_LUCKY_CAP, FIND_ITEM(ITEM_LUCKY_CAP) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_SECOND_CHEST, FLAG_VENUS_LIGHTHOUSE_CARRY_STONE, FIND_ITEM(ITEM_CARRY_STONE) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WALL_PILLAR, CONDITION_ALWAYS, EVENT(VenusLighthouse_MovePillarTowardWall) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FLOOR_SWITCH_STATUE, FLAG_SCENE_TEMP_00, EVENT(VenusLighthouse_MovePillarOntoSwitch) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_WALL_PILLAR, CONDITION_ALWAYS, EVENT(VenusLighthouse_CarryPillarPastWall) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(VenusLighthouse_RevealPath) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(VenusLighthouse_HidePath) },
    { TRIGGER_TABLE_END },
};

struct Trigger gVenusLighthouseFirstFloorTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_GAP, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_GAP },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_THUNDER_CROWN, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_THUNDER_CROWN },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_DRIFTING_FLOOR },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_LADDERS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_LADDERS },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_TO_LADDERS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_TO_LADDERS },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_DRIFTING_FLOOR },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_LADDERS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_LADDERS },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_GAP, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_GAP },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_THUNDER_CROWN, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_THUNDER_CROWN },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PILLARS },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PSYNERGY_STONE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PSYNERGY_STONE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_TO_LADDERS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_TO_LADDERS },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_TUNNEL_RUINS_LADY_STATUES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_TUNNEL_RUINS_LADY_STATUES },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_TO_DRIFTING_FLOOR },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_SOUTH, CONDITION_ALWAYS, EVENT(VenusLighthouse_DriftSouth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_HOLE, CONDITION_ALWAYS, EVENT(VenusLighthouse_FallThroughFloor) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_BESIDE_CAPPED_PILLARS, CONDITION_ALWAYS, EVENT(VenusLighthouse_PushCappedPillar) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AMONG_CAPPED_PILLARS, CONDITION_ALWAYS, EVENT(VenusLighthouse_PushCappedPillar) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_BESIDE_CAPPED_PILLARS, CONDITION_ALWAYS, EVENT(VenusLighthouse_LeaveCappedPillarRows) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_AMONG_CAPPED_PILLARS, CONDITION_ALWAYS, EVENT(VenusLighthouse_EnterCappedPillarRows) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_CARRY_PILLARS, CONDITION_ALWAYS, EVENT(VenusLighthouse_JumpOrPushCarryPillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_CHEST, FLAG_VENUS_LIGHTHOUSE_THUNDER_CROWN, FIND_ITEM(ITEM_THUNDER_CROWN) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_PSYNERGY_STONE, FLAG_VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CAPPED_PILLAR, CONDITION_ALWAYS, EVENT(VenusLighthouse_MoveCappedPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CAPPED_PILLAR + 1, CONDITION_ALWAYS, EVENT(VenusLighthouse_MoveCappedPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CAPPED_PILLAR + 2, CONDITION_ALWAYS, EVENT(VenusLighthouse_MoveCappedPillar) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, EVENT(VenusLighthouse_UnblockCarryPillarCells) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, EVENT(VenusLighthouse_UnblockCarryPillarCells) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, EVENT(VenusLighthouse_UnblockCarryPillarCells) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, EVENT(VenusLighthouse_UnblockCarryPillarCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, EVENT(VenusLighthouse_MoveCarryPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, EVENT(VenusLighthouse_MoveCarryPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, EVENT(VenusLighthouse_CarryPillarAcrossGap) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, EVENT(VenusLighthouse_CarryPillarAcrossGap) },
    { TRIGGER_TABLE_END },
};

struct Trigger gVenusLighthouseSecondFloorTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_TO_PILLAR_PIT, CONDITION_ALWAYS, EVENT(VenusLighthouse_EnterSparklingPassage) },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_SPARKLING_PASSAGE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_SPARKLING_PASSAGE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_GATE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_GATE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_TO_GATE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_TO_GATE },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_WALL_SWITCH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_WALL_SWITCH },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_PILLAR_PIT, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_PILLAR_PIT },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_WALL_SWITCH, CONDITION_ALWAYS, EVENT(VenusLighthouse_StepOnWallSwitch) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_GATE_PILLAR, FLAG_VENUS_LIGHTHOUSE_GATE_OPEN, EVENT(VenusLighthouse_JumpOrPushGatePillar) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(VenusLighthouse_SearchLockedDoor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GATE_PILLAR, FLAG_VENUS_LIGHTHOUSE_GATE_OPEN, EVENT(VenusLighthouse_MoveGatePillar) },
    { TRIGGER_TABLE_END },
};

struct Trigger gVenusLighthouseThirdFloorTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_TO_SPARKLING_PASSAGES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_TO_SPARKLING_PASSAGES },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_TO_SPARKLING_PASSAGES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_TO_SPARKLING_PASSAGES },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_NORTHWEST_ROOM, CONDITION_ALWAYS, EVENT(VenusLighthouse_EnterSparklingPassage) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRAGON_SCALES, CONDITION_ALWAYS, EVENT(VenusLighthouse_EnterSparklingPassage) },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRIFTING_FLOOR },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_TO_DRIFTING_FLOOR },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SPARKLING_PASSAGES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SPARKLING_PASSAGES },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SOUTHWEST_ROOM, CONDITION_ALWAYS, EVENT(VenusLighthouse_EnterSparklingPassage) },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_PILLAR_SOCKETS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_PILLAR_SOCKETS },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_DRIFTING_FLOOR },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_WEST_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_WEST_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_TO_PILLAR_SOCKETS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_TO_PILLAR_SOCKETS },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_SOUTH, CONDITION_ALWAYS, EVENT(VenusLighthouse_DriftSouth) },
    { TRIGGER_STEP, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SOUTHWEST_ROOM, CONDITION_ALWAYS, EVENT(VenusLighthouse_DriftSouth) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_SWITCH_PILLARS, CONDITION_ALWAYS, EVENT(VenusLighthouse_JumpOrPushSwitchPillar) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(VenusLighthouse_SearchLockedDoor) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_CHEST, FLAG_VENUS_LIGHTHOUSE_DRAGON_SCALES, FIND_ITEM(ITEM_DRAGON_SCALES) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SWITCH_PILLAR, CONDITION_ALWAYS, EVENT(VenusLighthouse_MoveSwitchPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SWITCH_PILLAR + 1, CONDITION_ALWAYS, EVENT(VenusLighthouse_MoveSwitchPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SWITCH_PILLAR + 2, CONDITION_ALWAYS, EVENT(VenusLighthouse_MoveSwitchPillar) },
    { TRIGGER_TABLE_END },
};

struct Trigger gVenusLighthouseFourthFloorTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_TO_WALL_SWITCHES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_TO_WALL_SWITCHES },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLARS },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PSYNERGY_STONE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PSYNERGY_STONE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_EAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_WEST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_EAST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_SOUTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_SOUTH },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_WEST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_EAST },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_SOUTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_SOUTH },
    { TRIGGER_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_EAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_RISING_PASSAGE, CONDITION_ALWAYS, EVENT(VenusLighthouse_EnterRisingPassage) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS, CONDITION_ALWAYS, EVENT(VenusLighthouse_PushPuzzlePillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_GAP, CONDITION_ALWAYS, EVENT(VenusLighthouse_JumpAcrossGap) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_GAP, CONDITION_ALWAYS, EVENT(VenusLighthouse_JumpAcrossGap) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_GAP, CONDITION_ALWAYS, EVENT(VenusLighthouse_PushPuzzlePillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_EAST_GAP, CONDITION_ALWAYS, EVENT(VenusLighthouse_JumpAcrossGap) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_EAST_GAP, CONDITION_ALWAYS, EVENT(VenusLighthouse_PushPuzzlePillar) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_WEST_WALL_SWITCH, CONDITION_ALWAYS, EVENT(VenusLighthouse_StepOnWestWallSwitch) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_EAST_WALL_SWITCH, CONDITION_ALWAYS, EVENT(VenusLighthouse_StepOnEastWallSwitch) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_SOUTH, CONDITION_ALWAYS, EVENT(VenusLighthouse_DriftSouth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_NORTH, CONDITION_ALWAYS, EVENT(VenusLighthouse_DriftNorth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_WEST, CONDITION_ALWAYS, EVENT(VenusLighthouse_DriftWest) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_EAST, CONDITION_ALWAYS, EVENT(VenusLighthouse_DriftEast) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_SOCKET_PILLARS, CONDITION_ALWAYS, EVENT(VenusLighthouse_JumpOrPushSocketPillar) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(VenusLighthouse_SearchLockedDoor) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_CHEST, FLAG_VENUS_LIGHTHOUSE_GAIA_BLADE, FIND_ITEM(ITEM_GAIA_BLADE) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VENUS_LIGHTHOUSE_PSYNERGY_STONE, FLAG_VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR + 1, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR + 2, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR + 3, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR, CONDITION_ALWAYS, EVENT(VenusLighthouse_CarryFloatingPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR + 1, CONDITION_ALWAYS, EVENT(VenusLighthouse_CarryFloatingPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR + 2, CONDITION_ALWAYS, EVENT(VenusLighthouse_CarryFloatingPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR + 3, CONDITION_ALWAYS, EVENT(VenusLighthouse_CarryFloatingPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR, CONDITION_ALWAYS, EVENT(VenusLighthouse_MoveSocketPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR + 1, CONDITION_ALWAYS, EVENT(VenusLighthouse_MoveSocketPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR + 2, CONDITION_ALWAYS, EVENT(VenusLighthouse_MoveSocketPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR + 3, CONDITION_ALWAYS, EVENT(VenusLighthouse_MoveSocketPillar) },
    { TRIGGER_TABLE_END },
};
// clang-format on

/// Cursors into each third-floor socket's list of pieces in
/// gVenusThirdFloorTrackLists, which VenusLighthouse_RouteLight moves on.
const u8 *gVenusThirdFloorSocketTracks[] = {
    gVenusThirdFloorTrackLists,
    gVenusThirdFloorTrackLists + 4,
    gVenusThirdFloorTrackLists + 8,
    gVenusThirdFloorTrackLists + 12,
};

/// Each third-floor socket's light scripts, by the index of the piece it has
/// routed.
s32 *gVenusThirdFloorNorthwestPaths[] = {
    gVenusThirdFloorLightNorthwestToGate,
    gVenusThirdFloorLightNorthwestToSouthwest,
    gVenusLightGoesOut,
    gVenusLightGoesOut,
};
s32 *gVenusThirdFloorNortheastPaths[] = {
    gVenusThirdFloorLightNortheastToNorthwest,
    gVenusThirdFloorLightNortheastToSoutheast,
    gVenusLightGoesOut,
    gVenusLightGoesOut,
};
s32 *gVenusThirdFloorSouthwestPaths[] = {
    gVenusThirdFloorLightSouthwestToGate,
    gVenusThirdFloorLightSouthwestToNorthwest,
    gVenusThirdFloorLightSouthwestToGate,
    gVenusLightGoesOut,
};
s32 *gVenusThirdFloorSoutheastPaths[] = {
    gVenusThirdFloorLightSoutheastToSouthwest,
    gVenusThirdFloorLightSoutheastToNortheast,
    gVenusThirdFloorLightSoutheastToSouthwest,
    gVenusLightGoesOut,
};

/// Each third-floor socket's paths, by the index of the piece in its list.
s32 **gVenusThirdFloorSocketPaths[] = {
    gVenusThirdFloorNorthwestPaths,
    gVenusThirdFloorNortheastPaths,
    gVenusThirdFloorSouthwestPaths,
    gVenusThirdFloorSoutheastPaths,
};

/// As gVenusThirdFloorSocketTracks, for the fourth floor.
const u8 *gVenusFourthFloorSocketTracks[] = {
    gVenusFourthFloorTrackLists,      gVenusFourthFloorTrackLists + 2,
    gVenusFourthFloorTrackLists + 3,  gVenusFourthFloorTrackLists + 5,
    gVenusFourthFloorTrackLists + 7,  gVenusFourthFloorTrackLists + 9,
    gVenusFourthFloorTrackLists + 12, gVenusFourthFloorTrackLists + 14,
};

/// Each fourth-floor socket's light scripts, by the index of the piece it has
/// routed.
s32 *gVenusFourthFloorNorthwestPaths[] = {
    gVenusFourthFloorLightNorthwestToGate,
    gVenusLightGoesOut,
};
s32 *gVenusFourthFloorNortheastPaths[] = {
    gVenusLightGoesOut,
};
s32 *gVenusFourthFloorWestPaths[] = {
    gVenusFourthFloorLightWestToNorthwest,
    gVenusLightGoesOut,
};
s32 *gVenusFourthFloorEastPaths[] = {
    gVenusFourthFloorLightEastToNortheast,
    gVenusLightGoesOut,
};
s32 *gVenusFourthFloorFarWestPaths[] = {
    gVenusFourthFloorLightFarWestToWest,
    gVenusLightGoesOut,
};
s32 *gVenusFourthFloorMiddlePaths[] = {
    gVenusFourthFloorLightMiddleToEast,
    gVenusFourthFloorLightMiddleToWest,
    gVenusLightGoesOut,
};
s32 *gVenusFourthFloorSouthwestPaths[] = {
    gVenusFourthFloorLightSouthwestToFarWest,
    gVenusLightGoesOut,
};
s32 *gVenusFourthFloorSoutheastPaths[] = {
    gVenusFourthFloorLightSoutheastToMiddle,
    gVenusLightGoesOut,
};

/// As gVenusThirdFloorSocketPaths, for the fourth floor.
s32 **gVenusFourthFloorSocketPaths[] = {
    gVenusFourthFloorNorthwestPaths, gVenusFourthFloorNortheastPaths,
    gVenusFourthFloorWestPaths,      gVenusFourthFloorEastPaths,
    gVenusFourthFloorFarWestPaths,   gVenusFourthFloorMiddlePaths,
    gVenusFourthFloorSouthwestPaths, gVenusFourthFloorSoutheastPaths,
};
