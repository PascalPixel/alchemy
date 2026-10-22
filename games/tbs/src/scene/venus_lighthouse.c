/// @file
/// Venus Lighthouse, six scenes in one overlay: the approach outside, the floor
/// the front door opens onto, and the four floors of the climb that ends in the
/// Lighthouse Aerie at the top (venus_lighthouse_aerie.c). Each scene has its
/// own entrance, placement and event tables (VenusLighthouseGroundFloor's
/// entrances are the fallback table), all share one exit table, and
/// VenusLighthouse_Initialize redraws each scene's puzzles as their flags left
/// them, case by entrance. Inside, only the actors of the room entered are
/// placed (CONDITION_IN_VIEW).
///
/// - Venus Lighthouse Entry (SceneId_VenusLighthouseEntry), outside. The
///   scholars and Babi's soldiers the wanderers attacked lie hurt and say what
///   they saw; a soldier who tells of the attack flickers and vanishes
///   (FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD), and the collision of his spot
///   changes. VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR, the
///   open door, leads in.
/// - Venus Lighthouse (SceneId_VenusLighthouseGroundFloor, named by a scene
///   rule of its own), the floor behind the front door, with three scholars.
///   The great statue (TRIGGER_GREAT_STATUE) opens one of two paths
///   (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or _SECOND_PATH, the first time by the
///   frame counter's parity, then switching at each search) and lights it in
///   gold lines between the floor lamps before it, where a small model of the
///   lighthouse stands (gVenusMiniatureLighthouseScript); the Tunnel Ruins
///   under Babi Lighthouse redraw their ways to match (tunnel_ruins.c), and
///   they lead up into VenusLighthouseFirstFloor. The reliefs
///   (TRIGGER_FIRST_RELIEF) are read, and Reveal shows a hidden path in two
///   places. Move pushes a pillar to a wall
///   (FLAG_VENUS_LIGHTHOUSE_PILLAR_AT_WALL) and Carry, from the Carry Stone in
///   one of the floor's chests, sets it down beyond
///   (FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED). A floor switch is pressed while
///   the leader stands on it and held by a statue Moved onto it; until then the
///   ledge (TRIGGER_LEDGE) drops the leader to the floor below.
/// - Venus Lighthouse (SceneId_VenusLighthouseFirstFloor), where the Tunnel
///   Ruins come up
///   (VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_TUNNEL_RUINS_LADY_STATUES). A floor
///   carries the party south, a hole drops the leader into the room below,
///   three capped pillars are Moved or pushed, and two pillars by a gap are
///   carried across it, or dropped through the floor. A chest holds the Thunder
///   Crown, and a Psynergy Stone lies here.
/// - Venus Lighthouse (SceneId_VenusLighthouseSecondFloor). A sparkling
///   passage takes the leader through in a trail of sparks; a floor switch
///   slides a wall aside in a cloud of dust the first time
///   (FLAG_VENUS_LIGHTHOUSE_WALL_MOVED); and a pillar Moved or pushed into its
///   socket sinks, and a light runs from the lady statue along the gold track
///   over it to open the gate (FLAG_VENUS_LIGHTHOUSE_GATE_OPEN).
/// - Venus Lighthouse (SceneId_VenusLighthouseThirdFloor). Three more
///   sparkling passages, a drifting floor, and three pillars carrying pieces
///   of track that sink into four sockets; with all three sunk two lights run
///   from the statue, and when the track leads one to the gate it opens
///   (FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN, with
///   FLAG_VENUS_LIGHTHOUSE_SOCKETS_A or _B for where the pillars stand). The
///   room with its sockets labelled, an arrangement of the pieces that opens
///   the gate and the light's path through it is
///   https://alchemy.pascalpixel.com/places/venus-lighthouse-third-floor/sockets.png.
///   A chest holds Dragon Scales.
/// - Venus Lighthouse (SceneId_VenusLighthouseFourthFloor). Floors carry the
///   party each way, a west and an east floor switch slide their walls aside
///   (FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED and _EAST_WALL_MOVED, which
///   VenusLighthouseThirdFloor also redraws), and Carry sets down four floating
///   pillars on two balance platforms that sink and rise to their weight
///   (gVenusBalancePlatformHeights). Four pillars carrying track sink into
///   eight sockets, and the lights run on to the pillar room's gate
///   (FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN). A chest holds the Gaia Blade,
///   a Psynergy Stone lies here, and a passage raises the leader on a platform
///   in a spray to the Lighthouse Aerie
///   (VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_AERIE).
///
/// Locked doors stand on the second floor and above (TRIGGER_LOCKED_DOOR). The
/// wall and gate flags (FLAG_VENUS_LIGHTHOUSE_WALL_MOVED to
/// FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN) and the Psynergy Stones' flags are
/// place flags: they last while the party stays in the scenes of the climb
/// (SceneId_VenusLighthouseFirstFloor to SceneId_VenusLighthouseAerie, one
/// place group), so the floors stay solved between them. The FLAG_SCENE_TEMP_
/// states last only until the next change of scene other than a return from
/// battle. Every arrival sets FLAG_VENUS_LIGHTHOUSE_LIT.

#include "script.h"
#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/venus_lighthouse.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "iowrite.h"
#include "object_effect.h"
#include "macros.h"

#define NULL ((void *)0)
#include "global_cells.h"
#include "types.h"
#include "animspr.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
#include "staged_actor.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "iwram_call.h"
#include "call.h"
#include "text/msg_ids.h"
#include "field_service.h"
#include "io_reg.h"
#include "scene_ids.h"
#include "map.h"
#include "map_scroll.h"
#include "field_runtime.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.

// The floor switches have slid the walls aside.
/// The second floor's pink floor switch has slid the wall beside it aside.
/// ★ Named from what the player sees: the pink floor switch on the second
/// floor and the stone wall it moves (its render)
#define FLAG_VENUS_LIGHTHOUSE_WALL_MOVED 0x300
/// ★ Named from what the player sees: the pink floor switch on the west side
/// of the fourth floor (its render)
#define FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED 0x306
/// ★ Named from what the player sees: the floor switch on the east side of
/// the fourth floor (its render)
#define FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED 0x307
// The gates the sunken pillars opened: one pillar, three pillars (and which of
// two sets of sockets they sank into) and the pillar room's four.
/// The light from the second floor's lady statue has opened the gate at the
/// end of its track.
/// ★ Named from what the player sees: the glowing arch at the end of the gold
/// track on the second floor (its render)
#define FLAG_VENUS_LIGHTHOUSE_GATE_OPEN 0x301
/// The first set of sockets.
/// ★ Named from what the player sees: the square sockets in the third floor
/// that the pillars sink into (its render)
#define FLAG_VENUS_LIGHTHOUSE_SOCKETS_A 0x302
/// The second set of sockets.
/// ★ Named from what the player sees: the square sockets in the third floor
/// that the pillars sink into (its render)
#define FLAG_VENUS_LIGHTHOUSE_SOCKETS_B 0x303
/// ★ Named from what the player sees: the gate the gold track leads to in the
/// third floor's room of three pillars and four sockets (its render)
#define FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN 0x304
/// ★ Named from what the player sees: the glowing arch over the fourth floor's
/// room of gold track and four pillars (its render)
#define FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN 0x308
/// The first floor's Psynergy Stone, taken until the party leaves the climb.
/// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
#define FLAG_VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE 0x350
/// The fourth floor's Psynergy Stone, as the first floor's.
/// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
#define FLAG_VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE 0x351

/// The cells the floors' events answer, by floor; a number means one thing on
/// one floor and another on the next. Each exit cell carries the number of the
/// exit it leads by, and the event rows name it by that exit, as they do the
/// sparkling passages, which leave by their own number
/// (Event_EnterSparklingPassage).
enum VenusLighthouseTrigger {
  TRIGGER_LOCKED_DOOR = 25,     ///< a locked door, from the second floor up
  TRIGGER_DRIFT_SOUTH = 30,     ///< a floor that carries the party south
  TRIGGER_CHEST = 100,          ///< a floor's chest
  TRIGGER_PSYNERGY_STONE = 110, ///< a floor's Psynergy Stone
  // VenusLighthouseGroundFloor
  /// The first of the six reliefs, whose words Event_ReadRelief shows in
  /// their order.
  TRIGGER_FIRST_RELIEF = 35,
  TRIGGER_GREAT_STATUE = 41,
  TRIGGER_LEDGE = 45, ///< where the leader jumps down until the switch is held
  TRIGGER_FLOOR_SWITCH = 46,
  TRIGGER_WALL_PILLAR = 50, ///< walking into ACTOR_WALL_PILLAR
  TRIGGER_SECOND_CHEST = 101,
  // VenusLighthouseFirstFloor
  TRIGGER_HOLE = 35,
  TRIGGER_CARRY_PILLARS = 45, ///< the gap and the two pillars beside it
  // VenusLighthouseSecondFloor
  TRIGGER_WALL_SWITCH = 35,
  TRIGGER_GATE_PILLAR = 36, ///< the gap and ACTOR_GATE_PILLAR
  // VenusLighthouseThirdFloor
  TRIGGER_SWITCH_PILLARS = 35, ///< the gaps and the three switch pillars
  // VenusLighthouseFourthFloor
  TRIGGER_RISING_PASSAGE = 18,
  TRIGGER_DRIFT_NORTH = 31,
  TRIGGER_DRIFT_WEST = 32,
  TRIGGER_DRIFT_EAST = 33,
  TRIGGER_FLOATING_PILLARS = 35,          ///< pushing a floating pillar
  TRIGGER_FLOATING_PILLARS_GAP = 36,      ///< a gap jumped west or east
  TRIGGER_FLOATING_PILLARS_EAST_GAP = 37, ///< a gap jumped east
  TRIGGER_WEST_WALL_SWITCH = 40,
  TRIGGER_EAST_WALL_SWITCH = 41,
  TRIGGER_SOCKET_PILLARS = 45 ///< the gaps and the four socket pillars
};

// UB: passage offsets and dust directions can be negative. The callers use
// bounded pixel offsets or directions -1..1, so these products fit s32.
#ifdef UBFIX
#define VENUS_LIGHTHOUSE_FIXED_OFFSET(value, bits) ((value) * (1 << (bits)))
#else
#define VENUS_LIGHTHOUSE_FIXED_OFFSET(value, bits) ((value) << (bits))
#endif

#ifdef UBFIX
#define VENUS_LIGHTHOUSE_RUN_OPENING_SEQUENCE                                  \
  VenusLighthouse_ArriveThroughSparkPassage
#else
#define VENUS_LIGHTHOUSE_RUN_OPENING_SEQUENCE                                  \
  ((s32 (*)(void))VenusLighthouse_ArriveThroughSparkPassage)
#endif

#if TBS_PATCH_3
#define VENUS_DESCENT_COLLISION_X 33
#define VENUS_DESCENT_COLLISION_WIDTH 7
#else
#define VENUS_DESCENT_COLLISION_X 34
#define VENUS_DESCENT_COLLISION_WIDTH 5
#endif

static void VenusLighthouse_StepActorWithDust(struct Vector3 *direction);
static void VenusLighthouse_AnimatePairedTileWaves(void);
static void VenusLighthouse_EmitWestPassageParticles(void);
static void VenusLighthouse_EmitEastPassageParticles(void);
static void VenusLighthouse_AnimateWestTileWave(void);
static void VenusLighthouse_AnimateEastTileWave(void);
static void VenusLighthouse_RaiseLeaderWithSpray(void);

// The lighthouse's scene flags:
// FLAG_SCENE_TEMP_00: in the room with the floor switch, the pillar holds the
// switch down; in the room of floating pillars, FLAG_SCENE_TEMP_00 plus a
// pillar's number (0 to 3): that pillar is set down in its place.
// FLAG_SCENE_TEMP_01: the leader stands on the floor switch.

// Each scene numbers its actors by the rows of its own placement table, so
// each has an enum of its own; a row drawn as one of the party's characters
// (Garet in VenusLighthouseGroundFloor) is that party member in his own slot
// and takes no number.

/// Venus Lighthouse Entry's actors, numbering the rows of
/// gVenusLighthouseEntryPlacements: the scholars (SPRITE_OLD_MAN_IN_BLUE_CAP)
/// and Babi's soldiers (SPRITE_SOLDIER) the wanderers attacked, each named by
/// what they say.
enum VenusLighthouseEntryActor {
  /// A soldier by the entrance: "Venus Lighthouse was... attacked by
  /// strange... wanderers...", and he flickers and is gone
  /// (Event_HearOfTheAttack).
  /// ★ Named from the game: "attacked by strange... wanderers..."
  /// (MsgFieldVenusLighthouseWasAttackedBy)
  ACTOR_SOLDIER_TELLING_OF_THE_ATTACK = ACTOR_FIRST_PLACED,
  /// "Iodem, be happy. The entrance to Venus Lighthouse is open..."
  /// ★ Named from the game: "The entrance to Venus Lighthouse is open"
  /// (MsgVenusLighthouseIodemBeHappyTheEntranceTo)
  ACTOR_SCHOLAR_LIGHTHOUSE_OPEN,
  /// "Ohhh! How could they attack mere scholars? Those barbarians!"
  /// ★ Named from the game: "Those barbarians!"
  /// (MsgVenusLighthouseOhhhHowCouldTheyAttackMere)
  ACTOR_SCHOLAR_BARBARIANS,
  /// "I've waited so long to see what it's like inside Venus Lighthouse,
  /// but..." (Event_TalkToWaitingScholar)
  /// ★ Named from the game: "I've waited so long" (MsgVenusIveWaitedLongSeeIts)
  ACTOR_WAITING_SCHOLAR,
  /// "How... How did they open the door to Venus Lighthouse?"
  /// ★ Named from the game: "How did they open the door to Venus Lighthouse?"
  /// (MsgVenusLighthouseHowHowDidTheyOpenThe)
  ACTOR_SOLDIER_OPENED_DOOR,
  /// "They are already... headed for Babi Lighthouse..."
  /// ★ Named from the game: "headed for Babi Lighthouse..."
  /// (MsgVenusLighthouseTheyAreAlreadyHeadedForBabi)
  ACTOR_SOLDIER_HEADED_FOR_BABI,
  /// "Babi's soldiers are all Colosso victors... How could anyone beat all of
  /// us?" His thoughts: "Was it Kraden?"
  /// ★ Named from the game: "Babi's soldiers are all Colosso victors..."
  /// (MsgVenusLighthouseBabisSoldiersAreAllColossoVictors)
  ACTOR_SOLDIER_COLOSSO_VICTORS
};

/// VenusLighthouseGroundFloor's actors, numbering the rows of
/// gVenusLighthouseGroundFloorPlacements after Garet's.
enum VenusLighthouseGroundFloorActor {
  /// "Kraden told them to stop, but they attacked anyway."
  /// ★ Named from the game: "Kraden told them to stop, but they attacked
  /// anyway." (MsgVenusLighthouseKradenToldThemToStopBut)
  ACTOR_SCHOLAR_ATTACKED_ANYWAY = ACTOR_FIRST_PLACED,
  /// "Kraden went inside, but then came back and said this isn't the
  /// entrance."
  /// ★ Named from the game: "said this isn't the entrance."
  /// (MsgVenusLighthouseKradenWentInsideButThenCame)
  ACTOR_SCHOLAR_NOT_THE_ENTRANCE,
  /// Event_TalkToScholarByDoor.
  /// ★ Named from the game: "I thought I'd explore after the door opened."
  /// (MsgVenusThoughtIdExploreAfterDoor)
  ACTOR_DOOR_SCHOLAR,
  /// The pillar Move pushes to the wall and Carry sets down beyond it
  /// (Event_MovePillarTowardWall).
  /// ★ Named from what the player sees: a grey pillar carved with spirals, two
  /// blocks tall, short of the wall (its render)
  ACTOR_WALL_PILLAR = ACTOR_FIRST_PLACED + 4,
  /// The lady statue Move pushes onto the floor switch (SPRITE_LADY_STATUE)
  /// (Event_MovePillarOntoSwitch).
  /// ★ Named from what the player sees: a white statue of a robed woman beside
  /// a floor switch (its render)
  ACTOR_FLOOR_SWITCH_STATUE
};

/// VenusLighthouseFirstFloor's actors, numbering the rows of
/// gVenusLighthouseFirstFloorPlacements.
enum VenusLighthouseFirstFloorActor {
  /// The three pillars, SPRITE_CAPPED_PILLAR, that Move or a push shifts, the
  /// nearer
  /// drawn in front (Event_MoveDepthPillar).
  /// ★ Named from what the player sees: grey pillars with white tops, one alone
  /// and two side by side (its render)
  ACTOR_FIRST_DEPTH_PILLAR = ACTOR_FIRST_PLACED,
  /// The two pillars by the gap, carried across it or dropped into it
  /// (Event_CarryPillarAcrossGap).
  /// ★ Named from what the player sees: two grey pillars carved with spirals
  /// beside the gap (its render)
  ACTOR_FIRST_CARRY_PILLAR = ACTOR_FIRST_DEPTH_PILLAR + 3
};

/// VenusLighthouseSecondFloor's actors, numbering the rows of
/// gVenusLighthouseSecondFloorPlacements.
enum VenusLighthouseSecondFloorActor {
  /// A lady statue, as ACTOR_FLOOR_SWITCH_STATUE is, where
  /// ACTOR_GATE_EFFECT sets out for the gate.
  /// ★ Named from what the player sees: a white statue of a robed woman beside
  /// the gate (its render)
  ACTOR_GATE_EFFECT_STATUE = ACTOR_FIRST_PLACED,
  /// The pillar before the gate, which sinks into its socket
  /// (Event_MoveGatePillar).
  /// ★ Named from what the player sees: a grey pillar carved with spirals on
  /// the track before the gate (its render)
  ACTOR_GATE_PILLAR,
  /// SPRITE_PARTICLE, the kind of object
  /// VenusLighthouse_CreateLight makes: once the pillar sinks it
  /// travels gVenusSecondFloorLightToGate to the gate, which opens, and stays
  /// there
  /// shedding particles.
  /// ★ Named from what the player sees: a small sparkle that rises by the
  /// gate as the pillar sinks (its render)
  ACTOR_GATE_EFFECT
};

/// VenusLighthouseThirdFloor's actors, numbering the rows of
/// gVenusLighthouseThirdFloorPlacements.
enum VenusLighthouseThirdFloorActor {
  /// A lady statue (SPRITE_LADY_STATUE), where the two gate effects set out
  /// once all
  /// three switch pillars have sunk.
  /// ★ Named from what the player sees: a white statue of a robed woman at the
  /// end of the gold track (its render)
  ACTOR_SWITCH_EFFECT_STATUE = ACTOR_FIRST_PLACED,
  /// The three pillars that sink into free sockets (Event_MoveSwitchPillar).
  /// ★ Named from what the player sees: three grey pillars carved with spirals
  /// on the gold track (its render)
  ACTOR_FIRST_SWITCH_PILLAR,
  /// SPRITE_PARTICLE, as ACTOR_GATE_EFFECT, placed where the gate effects end
  /// once the gate is open.
  /// ★ Named from what the player sees: a sparkle, as on the second floor,
  /// where a gate has opened (its render)
  ACTOR_SWITCH_GATE_EFFECT = ACTOR_FIRST_SWITCH_PILLAR + 3
};

/// VenusLighthouseFourthFloor's actors, numbering the rows of
/// gVenusLighthouseFourthFloorPlacements.
enum VenusLighthouseFourthFloorActor {
  /// The two platforms, SPRITE_BALANCE_PLATFORM, that rise or sink to the
  /// weight of the
  /// floating pillars set down on them (VenusLighthouse_LowerFloatingPillars).
  /// ★ Named from what the player sees: a white platform on a pole, the west of
  /// two (its render)
  ACTOR_BALANCE_WEST = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: the east white platform on its pole
  /// (its render)
  ACTOR_BALANCE_EAST,
  /// The four floating pillars Carry sets down (Event_CarryFloatingPillar).
  /// ★ Named from what the player sees: four grey pillars carved with spirals,
  /// three in a row above the platforms (its render)
  ACTOR_FIRST_FLOATING_PILLAR,
  /// A lady statue (SPRITE_LADY_STATUE), where the first gate effect sets out
  /// once all
  /// four socket pillars have sunk; the camera goes to it first.
  /// ★ Named from what the player sees: a white statue of a robed woman at the
  /// foot of the gold track (its render)
  ACTOR_SOCKET_EFFECT_STATUE = ACTOR_FIRST_FLOATING_PILLAR + 4,
  /// The four pillars of the pillar room (Event_MoveSocketPillar).
  /// ★ Named from what the player sees: four grey pillars carved with spirals
  /// on the gold track (its render)
  ACTOR_FIRST_SOCKET_PILLAR,
  /// SPRITE_PARTICLE, as ACTOR_GATE_EFFECT, placed where the gate effects end
  /// once the gate is open.
  /// ★ Named from what the player sees: a sparkle, as on the second floor,
  /// where a gate has opened (its render)
  ACTOR_SOCKET_GATE_EFFECT = ACTOR_FIRST_SOCKET_PILLAR + 4,
  /// The platform, SPRITE_FLOATING_PLATFORM as the floating platforms of
  /// Mercury
  /// Lighthouse's aerie, that dips as the leader lands on it at
  /// VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING
  /// and then carries him up to the Lighthouse Aerie
  /// (VenusLighthouse_RaiseLeaderWithSpray).
  /// ★ Named from what the player sees: a round white platform in a pit of its
  /// own (its render)
  ACTOR_RISING_PLATFORM
};

/// Animations the lighthouse plays by number beyond enum ActorAnimation, from
/// each sprite's animation table (graphics/sprite/spr_<number>.tsv).
enum {
  /// The hurt scholars and soldiers lying on the ground (spr_112.tsv and
  /// spr_152.tsv, the same pose).
  ANIM_HURT_LYING = 5,
  /// A light's sparkle, flickering between its two largest pictures
  /// (spr_284.tsv).
  PARTICLE_ANIM_FLICKER = 7
};

/// An animation timer step for Object_SetActionById, in sixteenths of a frame
/// taken off each frame: 16 plays an animation at its own speed, so 24 plays
/// it half again as fast.
enum { ANIM_STEPS_FAST = 24 };

/// The pieces of gold track a SPRITE_SPIRAL_PILLAR can carry, its animations
/// from 3, each named by the two edges of its cell it joins as the pillar's
/// top shows it, north up (graphics/sprite/spr_256.png). The lights the
/// lady statues send out run along the track (VenusLighthouse_RouteLight):
/// a pillar sunk in a socket carries the track on across it, and a light that
/// runs onto a piece that does not join the edge it comes in by goes out.
enum VenusTrack {
  TRACK_END = 0, ///< ends a list of pieces
  TRACK_NORTH_SOUTH = 3,
  TRACK_NORTH_WEST = 4,
  TRACK_NORTH_EAST = 5,
  TRACK_WEST_EAST = 6,
  TRACK_SOUTH_EAST = 7,
  TRACK_SOUTH_WEST = 8
};

/// An actor's timer as gVenusGreyStatueScript and gVenusGreyPillarScript set
/// it: a tag in the upper bits and a colour mode (enum SpriteColorMode) in the
/// low four, so that VenusLighthouse_RestoreScriptedChildValues can draw the
/// actor in that mode again after a battle.
#define VENUS_COLOR_MARK(mode) (VENUS_COLOR_MARK_TAG << 4 | (mode))
enum { VENUS_COLOR_MARK_TAG = 0x212 };

static __inline__ void SetFlagBits(u8 *flags, u8 bits) { *flags |= bits; }

/// Sets up a passage effect's motion, collision and child value.
static void VenusLighthouse_ConfigurePassageEffect(struct FieldActor *object,
                                                   s32 childValue) {
  object->motionFlags = 0;
  object->collisionFlags = 8;
  Engine_ActorSetSpriteFlags(object, 0);
  ObjectGroup_SetChildValue(object, childValue);
}

/// Creates a mask over a hole as the leader or a pillar falls through the
/// floor: an object of sprite type (SPRITE_CRATE or SPRITE_CAPPED_PILLAR)
/// drawn solid black (SPRITE_COLOR_BLACK) and translucent in front of the
/// actors, which shows the background through whatever falls behind it, so
/// that it vanishes below the hole's edge. Babi Lighthouse's PIT_MASK is the
/// same object, and its render without the mask shows the leader whole.
static struct FieldActor *
VenusLighthouse_CreateBlendedPassageEffect(s32 x, s32 y, s32 z, s32 type) {
  struct FieldActor *effect;

  effect = Engine_ObjectCreate(type, x, y, z);
  if (effect != 0) {
    ((struct AnimationObject *)effect->sprite)->image.priority = 0;
    VenusLighthouse_ConfigurePassageEffect(effect, SPRITE_COLOR_BLACK);
    Engine_ObjectSetBlendMode(effect, OBJECT_BLEND_TRANSLUCENT);
    return effect;
  }
  return 0;
}

/// Creates an object that keeps the leader on screen while a pillar sinks
/// into its socket with him standing north of it, where the sinking code puts
/// his sprite at priority 3. It is a SPRITE_CHEST drawn in
/// SPRITE_COLOR_HIDDEN, so it shows nothing itself; in tbs-en, with this
/// function returning at once, the leader vanishes for the 20 frames the
/// pillar takes to go down, and with it he stays in view
/// (https://alchemy.pascalpixel.com/places/venus-lighthouse-second-floor/occluding-chest.png,
/// with the chest on the left and without it on the right).
static struct FieldActor *
VenusLighthouse_CreateOccludingPassageEffect(s32 x, s32 y, s32 z, s32 type) {
  struct FieldActor *object;

  object = Engine_ObjectCreate(type, x, y, z);
  if (object != 0) {
    ((struct AnimationObject *)object->sprite)->image.priority = 1;
    VenusLighthouse_ConfigurePassageEffect(object, SPRITE_COLOR_HIDDEN);
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    return object;
  }
  return 0;
}

#include "common/effect_spawn.h"
#include "ability_ids.h"
#include "objcmd.h"

/// The scene's names for the bytes FieldActor keeps for scene code: a statue
/// with SW_VENUS_IMMOVABLE set cannot be pushed
/// (VenusLighthouse_PushAdjacentPillar), and a light sets
/// SW_VENUS_LIGHT_ARRIVED as it reaches its gate, which then opens.
enum {
  SW_VENUS_IMMOVABLE = SW_RISE_COUNTER,
  SW_VENUS_LIGHT_ARRIVED = SW_RISE_ENABLED
};

#include "common/object_distance.h"
#include "common/object_findslot.h"

/// Pushing a pillar: the pillar ahead of the leader moves one cell on with him,
/// leaning into it (ANIM_PUSH), when the cell beyond is free, nothing
/// stands on the pillar, and it is no statue (SW_VENUS_IMMOVABLE).
static void VenusLighthouse_PushAdjacentPillar(void) {
  struct FieldPosition targetPosition;
  struct FieldActor *leader;
  struct FieldActor *pillar;
  struct FieldActor *blockingActor;
  s32 directionIndex;
  u32 packedStep;
  s32 moveRate;
  s32 transitionBusy;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  directionIndex = leader->facing >> 12;
  // Look ahead for a pillar before testing its destination and overhead space.
  packedStep = gStagedActor_DirectionSteps[directionIndex];
  targetPosition.x = leader->x.fixed + (packedStep & 0xffff0000);
  targetPosition.y = leader->y.fixed;
  packedStep <<= 16;
  targetPosition.z = leader->z.fixed + packedStep;
  pillar = SceneData_FindSlotAtPosition(&targetPosition, leader);
  if (pillar == 0)
    return;

  packedStep = gStagedActor_DirectionSteps[directionIndex];
  targetPosition.x = pillar->x.fixed + (packedStep & 0xffff0000);
  targetPosition.y = pillar->y.fixed;
  packedStep <<= 16;
  targetPosition.z = pillar->z.fixed + packedStep;
  blockingActor = SceneData_FindSlotAtPosition(&targetPosition, pillar);
  if (blockingActor != 0 && (blockingActor->collisionFlags & 1) != 0)
    return;

  targetPosition.x = pillar->x.fixed;
  targetPosition.y = pillar->y.fixed + PIXELS(16);
  targetPosition.z = pillar->z.fixed;
  blockingActor = SceneData_FindSlotAtPosition(&targetPosition, pillar);
  if (blockingActor != 0 && (blockingActor->collisionFlags & 1) != 0)
    return;

  pillar->terrainId = 2;
  packedStep = gStagedActor_DirectionSteps[directionIndex];
  targetPosition.x = pillar->x.fixed + (packedStep & 0xffff0000);
  targetPosition.y = pillar->y.fixed;
  packedStep <<= 16;
  targetPosition.z = pillar->z.fixed + packedStep;
  if (FieldObject_CheckMovementCollision(pillar, &targetPosition) > 0)
    return;

  transitionBusy = pillar->riseCounter;
  if (transitionBusy != 0)
    return;

  // Move the pillar and leader together, then restore their standing state.
  Object_SetMode(leader, ANIM_PUSH);
  moveRate = PIXELS(1) / 5;
  Engine_TaskWait(15);
  Engine_AudioPlayCue(SE_185);
  pillar->speed = moveRate;
  pillar->acceleration = moveRate;
  Engine_ObjectSetPosition(pillar, targetPosition.x, targetPosition.y,
                           targetPosition.z);
  leader->speed = moveRate;
  leader->acceleration = moveRate;
  Engine_ObjectSetPosition(leader, targetPosition.x, targetPosition.y,
                           targetPosition.z);
  FieldObject_WaitForMove(pillar);
  FieldFx_PlayQueuedSound();
  pillar->x.fixed = targetPosition.x;
  pillar->z.fixed = targetPosition.z;
  pillar->velocityX = transitionBusy;
  pillar->velocityZ = transitionBusy;
  leader->targetX = ACTOR_NO_TARGET;
  leader->targetZ = ACTOR_NO_TARGET;
  leader->velocityX = transitionBusy;
  leader->velocityZ = transitionBusy;
  leader->x.fixed = leader->x.part.pixel << 16;
  leader->z.fixed = leader->z.part.pixel << 16;
  Object_SetMode(leader, ANIM_STAND);
}

/// Unused: fills collision codes in a rectangle of the requested map layer;
/// nothing in Venus Lighthouse calls it.
static UNUSED s32 StagedActor_FillGridAttributeRectangle(u32 layerIndex, s32 x,
                                                         s32 z, u32 width,
                                                         u32 height,
                                                         s32 collisionCode) {
  struct MapScrollWork *map = gMapWork[0];
  struct MapCell *cells;
  u32 row;
  u32 column;

  if (map != 0) {
    if (layerIndex <= 2) {
      cells = map->layers[layerIndex].cells;
    } else {
      cells = (struct MapCell *)gMapCellBuffer;
    }
    cells += x + (z << 7);
    for (row = 0; row < height; row++) {
      struct MapCell *cell = cells + (row << 7);

      for (column = 0; column < width; column++) {
        cell->collisionCode = collisionCode;
        cell++;
      }
    }
  }
  return 0;
}

/// After a battle, draws every actor whose timer holds a VENUS_COLOR_MARK in
/// its colour mode again.
static void VenusLighthouse_RestoreScriptedChildValues(void) {
  struct FieldActor **actors = gEventWork->placedActors;
  struct FieldActor *actor;
  u32 actorId;

  for (actorId = ACTOR_FIRST_PLACED; actorId <= 65; actorId++) {
    actor = actors[actorId - ACTOR_FIRST_PLACED];
    if (actor->timer >> 4 == VENUS_COLOR_MARK_TAG)
      ObjectGroup_SetChildValue(actor, actor->timer & 15);
  }
}

/// Called by gVenusGreyStatueScript and gVenusGreyPillarScript: draws the
/// actor in the colour mode of its VENUS_COLOR_MARK.
static s32
VenusLighthouse_UpdateChildFromTimer(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  ObjectGroup_SetChildValue(actor, actor->timer & 15);
  return 0;
}

// Pulse the child effect and play its timed sound cue.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 6 bytes of the 56-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void VenusLighthouse_PulseChildEffect(union FieldObject *fieldObject) {
#else
static s32 VenusLighthouse_PulseChildEffect(union FieldObject *fieldObject) {
#endif
  void *actor = (void *)fieldObject;
  if ((gFrameCount & 3) == 0)
    ObjectGroup_SetChildValue(actor, SPRITE_COLOR_WHITE);
  else
    ObjectGroup_SetChildValue(actor, SPRITE_COLOR_NORMAL);

  if ((gFrameCount & 7) == 0)
    Engine_AudioPlayCue(SE_ITEM_BREAK);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

// Emit a rising particle near the actor every eight frames.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 14 bytes of the 172-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void
VenusLighthouse_EmitRisingParticles(union FieldObject *fieldObject) {
#else
static s32 VenusLighthouse_EmitRisingParticles(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  s32 framePhase, particleX, particleY, riseSpeed;
  framePhase = gFrameCount & 7;
  if (framePhase == 0) {
    options.priority = 3 - ((u32)(Engine_RandomNext() * 2) >> 16);
    options.startScaleX = 2 * Q16_ONE / 5;
    options.startScaleY = 2 * Q16_ONE / 5;
    options.palette = 14;
    particleX =
        actor->x.fixed + ((((u32)(Engine_RandomNext() * 9) >> 16) - 4) << 16);
    particleY =
        actor->y.fixed + ((32 - ((u32)(Engine_RandomNext() * 32) >> 16)) << 16);
    riseSpeed = Math_Divide(
        (((u32)(Engine_RandomNext() * 5) >> 16) << 16) + 0x00050000, 10);
    FieldFx_Spawn(
        particleX, particleY, actor->z.fixed, 0, riseSpeed, framePhase,
        EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE,
        &options);
  }
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Clears the scripted actor's sprite flags and advances its script.
static s32 VenusLighthouse_ClearScriptedSpriteFlags(
    struct ScriptInterpreter *interpreter) {
  void *actor = (void *)interpreter;
  Engine_ActorSetSpriteFlags(actor, 0);
  return 0;
}

// Emit the two particle patterns used by the leader's passage trail.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 14 bytes of the 180-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void
VenusLighthouse_EmitPassageSparkTrail(union FieldObject *fieldObject) {
#else
static s32
VenusLighthouse_EmitPassageSparkTrail(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  s32 fallSpeed;
  s32 framePhase;

  options.startScaleX = 4 * Q16_ONE / 5;
  options.startScaleY = 4 * Q16_ONE / 5;
  options.priority = 0;

  fallSpeed = ((u32)(Engine_RandomNext() * 8) >> 16) * 0x3333;
  framePhase = gFrameCount & 15;

  FieldFx_Spawn(actor->x.fixed +
                    VENUS_LIGHTHOUSE_FIXED_OFFSET(8 - framePhase, 16),
                actor->y.fixed + PIXELS(26), actor->z.fixed, 0, -fallSpeed, 0,
                EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE, &options);

  framePhase = gFrameCount & 15;
  if (framePhase == 0) {
    options.startScaleX = Q16_ONE / 2;
    options.startScaleY = Q16_ONE / 2;
    FieldFx_Spawn(actor->x.fixed +
                      ((((u32)(Engine_RandomNext() * 9) >> 16) - 4) << 16),
                  actor->y.fixed, actor->z.fixed, 0, 0, 0,
                  EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE, &options);
  }
#ifdef UBFIX

  return;
#else

  return 0;
#endif
}

/// Walking north into a sparkling passage, on VenusLighthouseSecondFloor or
/// VenusLighthouseThirdFloor: the leader dissolves in sparks and leaves by the
/// exit of the passage's own number, coming out at its far end
/// (VenusLighthouse_ArriveThroughSparkPassage).
static void Event_EnterSparklingPassage(void) {
  struct FieldActor *leader;
  struct EventWork *eventWork;

  eventWork = gEventWork;
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Engine_AudioPlayCue(SE_228);
#ifdef UBFIX
  leader->update = VenusLighthouse_EmitPassageSparkTrail;
#else
  leader->update =
      (void (*)(union FieldObject *))VenusLighthouse_EmitPassageSparkTrail;
#endif
  leader->speed = PIXELS(1) / 5;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  leader->update = 0;
  Engine_EventWait(30);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(eventWork->touchedTrigger);
  Engine_EventEnd();
}

/// Brings the leader out of a sparkling passage on a fresh entrance.
static void VenusLighthouse_ArriveThroughSparkPassage(void) {
  struct FieldActor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    leader->motionFlags = 0;
    Actor_SetPosition(ACTOR_PARTY_LEADER, leader->x.part.pixel << 16,
                      (leader->z.part.pixel << 16) - 0x100000);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Event_PlayCue(SE_228);
#ifdef UBFIX
    leader->update = VenusLighthouse_EmitPassageSparkTrail;
#else
    leader->update =
        (void (*)(union FieldObject *))VenusLighthouse_EmitPassageSparkTrail;
#endif
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 1);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    leader->update = 0;
    FieldFx_PlayQueuedSound();
    Engine_EventEnd();
  }
}

/// Makes the leader the actor the fade overlay follows (struct FadeOverlayWork:
/// two translucent marks beside him wherever the ground beside him rises above
/// his floor). Reached by stepping onto cell code 32 of
/// VenusLighthouseFirstFloor, which the map gives the rows the three capped
/// pillars stand in; code 31, in the rows that bound them, turns the marks off
/// again (Event_ClearEffectTarget), and walking into a pillar from either
/// pushes it (Event_PushDepthPillar).
static void Event_SetLeaderAsEffectTarget(void) {
  gActorEffectWork->actor = Object_GetById(ACTOR_PARTY_LEADER);
}

/// Stepping onto cell code 31 of VenusLighthouseFirstFloor, in the rows that
/// bound the capped pillars': the fade overlay follows no one.
static void Event_ClearEffectTarget(void) { gActorEffectWork->actor = NULL; }

#include "common/object_depth.h"
#include "common/object_waitidle.h"
#include "common/effect_advance.h"

/// Drops the leader onto the floor and emits the radial landing dust.
static void VenusLighthouse_LandWithRadialDust(void) {
  struct FieldActor *leader;
  u32 particleIndex;
  struct EffectOptions options;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  leader->y.fixed = PIXELS(130);
  leader->gravity = PIXELS(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(30);
  Event_PlayCue(SE_204);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Engine_EventWait(24);
  options.palette = 7;
  options.update = (void (*)(union FieldObject *))FieldFx_AdvanceMotion;
  options.startScaleX = 4 * Q16_ONE / 5;
  options.startScaleY = 4 * Q16_ONE / 5;
  for (particleIndex = 0; particleIndex <= 16; particleIndex++) {
    s32 angle;
    s32 velocity[3];

    angle = particleIndex << 12;
    SET_VECTOR(velocity, Engine_MathCos(angle), 0, Engine_MathSin(angle));
    velocity[0] += velocity[0] / 2;
    FieldFx_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                  velocity[0], velocity[1], velocity[2],
                  EFFECT_USE_PALETTE | EFFECT_USE_START_SCALE |
                      EFFECT_USE_UPDATE | 1,
                  &options);
  }
  Event_PlayCue(SE_188);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  FieldFx_PlayQueuedSound();
  leader->gravity = PIXELS(1);
  leader->bounce = PIXELS(1) / 4;
  Engine_EventEnd();
}

#include "common/effect_radial.h"

// A light's update hook: every four frames a shrinking particle around it,
// and now and then a sound while it moves.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 172 bytes (extent 196 to 192 bytes). Keep the
// unused native result in matching builds.
#ifdef UBFIX
static void VenusLighthouse_TrailLight(union FieldObject *fieldObject) {
#else
static s32 VenusLighthouse_TrailLight(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  s32 velocityY;
  s32 velocityX;
  s32 velocityZ;
  s32 random;

  if ((gFrameCount & 3) != 0) {
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  }
  if ((u32)(Engine_RandomNext() * 6) >> 16 == 0) {
    if (actor->targetX != ACTOR_NO_TARGET ||
        actor->targetZ != ACTOR_NO_TARGET) {
      Engine_AudioPlayCue(SE_246);
    }
  }
  velocityY = 0;
  options.type = 286;
  options.startScaleX = Q16_ONE;
  options.startScaleY = Q16_ONE;
  options.targetScaleX = -327;
  options.targetScaleY = -327;
  random = Engine_RandomNext();
  velocityX = Math_Divide(((((u32)(random * 9)) >> 16) - 4) << 16, 10);
  random = Engine_RandomNext();
  velocityZ = Math_Divide(((((u32)(random * 9)) >> 16) - 4) << 16, 10);
  FieldFx_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed - PIXELS(1),
                velocityX, velocityY, velocityZ,
                EFFECT_SCALE_TO_TARGET | EFFECT_USE_START_SCALE |
                    EFFECT_USE_TYPE | 1,
                &options);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Creates a light at a lady statue, at pixel x and z, and starts it along its
/// path script, trailing particles (VenusLighthouse_TrailLight).
static struct FieldActor *VenusLighthouse_CreateLight(s32 x, s32 z,
                                                      const s32 *script) {
  struct FieldActor *effect;
  s32 fixedX;
  s32 fixedZ;

  fixedX = x << 16;
  fixedZ = z << 16;
  effect = Engine_ObjectCreate(SPRITE_PARTICLE, fixedX, 0, fixedZ);
  if (effect == 0) {
    return 0;
  }
  effect->speed = PIXELS(1);
  effect->acceleration = PIXELS(1);
  Engine_ActorSetSpriteFlags(effect, 0);
  Object_SetMode(effect, PARTICLE_ANIM_FLICKER);
  effect->motionFlags = 0;
  effect->y.fixed = 0;
  effect->collisionFlags = 0;
  effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
#ifdef UBFIX
  effect->update = VenusLighthouse_TrailLight;
#else
  effect->update = (void (*)(union FieldObject *))VenusLighthouse_TrailLight;
#endif
  effect->riseEnabled = 0;
  Engine_ObjectSetScript(effect, script);
  return effect;
}

/// The leader jumps a one-tile gap if the far side is clear.
/// It's reached by walking into the gap, in VenusLighthouseFourthFloor and
/// through the jump-or-push events of VenusLighthouseFirstFloor to
/// VenusLighthouseFourthFloor.
/// Returns 1 after a jump, 0 when there was none.
static s32 Event_JumpAcrossGap(void) {
  struct FieldActor *leader;
  struct FieldPosition landing;
  u8 *motionFlags;
  u8 savedMotionFlags;
  s32 direction;
#if !TBS_PATCH_1
  u32 step;
#endif

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  // Check the intervening cell before looking for a clear landing two cells
  // ahead.
  savedMotionFlags = *motionFlags;
#if TBS_PATCH_1
  landing.x = (leader->x.fixed & 0xfff00000) + PIXELS(8);
  landing.y = leader->y.fixed;
  landing.z = (leader->z.fixed & 0xfff00000) + PIXELS(8);
  direction = (leader->facing + 0x2000) & 0xc000;
  Vector_AddPolarOffset(PIXELS(16), direction, &landing);
#else
  direction = leader->facing >> 12;
  step = gStagedActor_DirectionSteps[direction];
  landing.x = leader->x.fixed + (step & 0xffff0000);
  landing.y = leader->y.fixed;
  step <<= 16;
  landing.z = leader->z.fixed + step;
#endif
  if (FieldObject_CheckMovementCollision(leader, &landing) != 1 &&
      SceneData_FindSlotAtPosition(&landing, leader) == 0) {
#if TBS_PATCH_1
    landing.x = (leader->x.fixed & 0xfff00000) + PIXELS(8);
    landing.y = leader->y.fixed;
    landing.z = (leader->z.fixed & 0xfff00000) + PIXELS(8);
    Vector_AddPolarOffset(PIXELS(32), (leader->facing + 0x2000) & 0xc000,
                          &landing);
#else
    step = gStagedActor_DirectionSteps[direction];
    landing.x = leader->x.fixed + ((step & 0xffff0000) << 1);
    landing.y = leader->y.fixed;
    step <<= 17;
    landing.z = leader->z.fixed + step;
#endif
    if (SceneData_FindSlotAtPosition(&landing, leader) == 0 &&
        FieldObject_CheckMovementCollision(leader, &landing) == 0) {
      Engine_EventBegin();
      Object_SetMode(leader, ANIM_LEADER_SPRING);
      Engine_TaskWait(6);
      Engine_AudioPlayCue(SE_JUMP);
      Object_SetMode(leader, ANIM_LEADER_LEAP);
      leader->speed = PIXELS(3);
      leader->acceleration = PIXELS(2);
      leader->velocityY = PIXELS(4);
      *motionFlags &= 0x7e;
      Engine_ActorSetSpriteFlags(leader, 0);
      Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, landing.x >> 16,
                                landing.z >> 16);
      Object_SetMode(leader, ANIM_LEADER_SPRING);
      Engine_ActorSetSpriteFlags(leader, 1);
      *motionFlags = savedMotionFlags;
      Engine_EventEnd();
      return 1;
    }
  }
  return 0;
}

/// Selects the scene's entrances; VenusLighthouseGroundFloor takes the fallback
/// table.
const struct SceneEntrance *VenusLighthouse_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VenusLighthouseEntry) {
    return gVenusLighthouseEntryEntrances;
  }
  if (scene == (s32)&SceneId_VenusLighthouseFirstFloor) {
    return gVenusLighthouseFirstFloorEntrances;
  }
  if (scene == (s32)&SceneId_VenusLighthouseSecondFloor) {
    return gVenusLighthouseSecondFloorEntrances;
  }
  if (scene == (s32)&SceneId_VenusLighthouseThirdFloor) {
    return gVenusLighthouseThirdFloorEntrances;
  }
  if (scene == (s32)&SceneId_VenusLighthouseFourthFloor) {
    return gVenusLighthouseFourthFloorEntrances;
  }
  return gVenusLighthouseEntrancesOther;
}

/// These scenes supply no exit regions.
const struct SceneRegion *VenusLighthouse_GetRegions(void) { return 0; }

/// Returns the packed exits shared by the six scenes.
const u32 *VenusLighthouse_GetExits(void) { return gVenusLighthouseExits; }

/// Selects the scene's placements. Inside the lighthouse,
/// FieldScene_PrepareActors first gives each row with condition 0 whether it
/// lies inside the view bounds, so only the room entered is populated.
const struct ScenePlacement *VenusLighthouse_GetPlacements(void) {
  struct ScenePlacement *placements;
  s16 scene;

  scene = gGameState.scene;
  if (scene == (s32)&SceneId_VenusLighthouseEntry) {
    return gVenusLighthouseEntryPlacements;
  }
  if (scene == (s32)&SceneId_VenusLighthouseGroundFloor) {
    placements = gVenusLighthouseGroundFloorPlacements;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  if (scene == (s32)&SceneId_VenusLighthouseFirstFloor) {
    placements = gVenusLighthouseFirstFloorPlacements;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  if (scene == (s32)&SceneId_VenusLighthouseSecondFloor) {
    placements = gVenusLighthouseSecondFloorPlacements;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  if (scene == (s32)&SceneId_VenusLighthouseThirdFloor) {
    placements = gVenusLighthouseThirdFloorPlacements;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  if (scene == (s32)&SceneId_VenusLighthouseFourthFloor) {
    placements = gVenusLighthouseFourthFloorPlacements;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  return gVenusLighthousePlacementsOther;
}

/// Searching a locked door, in VenusLighthouseSecondFloor to
/// VenusLighthouseFourthFloor. "The door is tightly locked."
static void Event_SearchLockedDoor(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
  Engine_EventEnd();
}

/// At Venus Lighthouse Entry, talking to the soldier by the entrance (or
/// reading his mind): "Venus Lighthouse was attacked by strange wanderers..."
/// He flickers ten times and is gone (FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD), and
/// the collision of his spot changes, as it does on later arrivals.
static void Event_HearOfTheAttack(void) {
  u32 flickersLeft;
  u32 flickerDelay;

  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, 3);
  Engine_EventSetMessage(MsgFieldVenusLighthouseWasAttackedBy);
  flickersLeft = 10;
  flickerDelay = 8;
  Engine_EventShowMessageAndWait(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, 0, 20);
  do {
    Engine_ActorSetChildValue(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK,
                              SPRITE_COLOR_HIDDEN);
    Engine_TaskWait(2);
    Engine_ActorSetChildValue(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK,
                              SPRITE_COLOR_NORMAL);
    Engine_TaskWait(flickerDelay);
    if (flickerDelay > 3) {
      flickerDelay--;
    }
    flickersLeft--;
  } while (flickersLeft != 0);
  Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD);
  Engine_ActorSetPosition(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, 0, 0);
  Map_CopyCellAttributes(7, 17, 2, 1, 7, 16);
  Engine_EventEnd();
}

/// A scholar who has waited so long to see inside can't move for the pain.
/// Talking to the scholar at Venus Lighthouse Entry plays it; it's the inside
/// of Venus Lighthouse the scholar has waited to see.
static void Event_TalkToWaitingScholar(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVenusIveWaitedLongSeeIts);
  Engine_EventShowMessageAndWait(ACTOR_WAITING_SCHOLAR, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_WAITING_SCHOLAR, 2);
  Engine_EventShowMessage(ACTOR_WAITING_SCHOLAR, 0);
  Engine_EventEnd();
}

/// In VenusLighthouseGroundFloor, Carry on the pillar Move could not push past
/// the wall: once it is set down beyond, it stays there
/// (FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED).
static void Event_CarryPillarPastWall(void) {
  struct FieldActor *pillar;

  pillar = Object_GetById(ACTOR_WALL_PILLAR);
  Engine_EventBegin();
  if (pillar->x.fixed >> 20 == 54 || pillar->z.fixed >> 20 == 6) {
    Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED);
  }
  Engine_EventEnd();
}

/// Searching a relief in VenusLighthouseGroundFloor shows its words, the relief
/// trigger's own message. "There are words carved into the relief..."
///
/// The Japanese text gives each of the six reliefs its own words; every
/// translation joins the first five into one message and leaves the next four
/// empty, so outside Japanese only the first and sixth reliefs can be read
/// (gVenusLighthouseGroundFloorEvents). The English edition, patch 1, also
/// dropped the sixth relief and showed the first message for every relief;
/// patch 2 put the per-relief message and the sixth relief back.
static void Event_ReadRelief(void) {
  s32 message;
#if !TBS_PATCH_1 || TBS_PATCH_2
  struct EventWork *work = gEventWork;
#endif
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
#if TBS_PATCH_1 && !TBS_PATCH_2
  message = (s32)MSGNO(MsgVenusThereWordsCarvedIntoRelief);
#else
  message = work->facingTrigger + MSGNO(MsgVenusThereWordsCarvedIntoRelief) -
            TRIGGER_FIRST_RELIEF;
#endif
  Engine_MessageShowCentered(message, 1);
  Engine_EventEnd();
}

/// Reveal fading in VenusLighthouseGroundFloor: the hidden path disappears
/// again, in both its places.
static void Event_HidePath(void) {
  s32 column;
  s32 row;

#if TBS_PATCH_1
  Engine_GameFlagClear(FLAG_NO_ENCOUNTERS_IN_PLACE);
#endif
  column = 23;
  row = 8;
  Map_CopyCellAttributes(35, 8, 1, 3, column, row);
  Map_CopyCellsTo(35, 8, 23, 8, 1, 3);
  Map_CopyCellsTo(99, 8, 87, 8, 1, 3);
  column = 46;
  row = 55;
  Map_CopyCellAttributes(57, 55, 3, 3, column, row);
  Map_CopyCellsTo(57, 55, 46, 55, 3, 3);
  Map_CopyCellsTo(121, 55, 110, 55, 3, 3);
}

/// Reveal in VenusLighthouseGroundFloor: the hidden path appears, in two
/// places, and can be walked on.
static void Event_RevealPath(void) {
  s32 column;
  s32 row;

#if TBS_PATCH_1
  Engine_GameFlagSet(FLAG_NO_ENCOUNTERS_IN_PLACE);
#endif
  column = 23;
  row = 8;
  Map_CopyCellAttributes(36, 8, 1, 3, column, row);
  Map_CopyCellsTo(36, 8, 23, 8, 1, 3);
  Map_CopyCellsTo(100, 8, 87, 8, 1, 3);
  column = 46;
  row = 55;
  Map_CopyCellAttributes(53, 55, 3, 3, column, row);
  Map_CopyCellsTo(53, 55, 46, 55, 3, 3);
  Map_CopyCellsTo(117, 55, 110, 55, 3, 3);
}

/// In VenusLighthouseGroundFloor, walking off the ledge (TRIGGER_LEDGE) before
/// the floor switch is held down: the leader jumps down to the floor below and
/// shakes his head.
static void Event_JumpDown(void) {
  struct FieldActor *leader;
  s32 landingQuarterZ;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVenusLeaderDazedScript);
  Object_RefreshSelectorById(ACTOR_PARTY_LEADER);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, SPRITE_COLOR_GREY);
  leader->velocityY = PIXELS(4);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4), PIXELS(2));
  if (leader->z.fixed >> 20 <= 54) {
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    landingQuarterZ = 210;
  } else {
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    landingQuarterZ = 238;
  }
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, leader->x.part.pixel,
                      landingQuarterZ << 2);
  Engine_EventWait(1);
  SetFlagBits(&Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags,
              ACTOR_FACES_MOTION);
  Engine_EventWait(20);
#ifdef UBFIX
  leader->update = VenusLighthouse_EmitRisingParticles;
#else
  leader->update =
      (void (*)(union FieldObject *))VenusLighthouse_EmitRisingParticles;
#endif
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  leader->update = 0;
  Engine_EventEnd();
}

/// Redraws the great statue's room for the first of the two paths it opens
/// (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH): a 7-by-8 block of cells and eleven
/// 3-by-2 pieces, which light gold lines between the round floor lamps before
/// the statue, from the lamp at its feet along one route across the grid of
/// fifteen (the room's renders with and without the flag).
static inline void VenusLighthouse_DrawFirstPath(void) {
  Map_CopyCellsTo(103, 27, 89, 27, 7, 8);
  Map_CopyCellsTo(41, 90, 27, 92, 3, 2);
  Map_CopyCellsTo(41, 90, 29, 93, 3, 2);
  Map_CopyCellsTo(41, 90, 27, 94, 3, 2);
  Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
  Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
  Map_CopyCellsTo(41, 96, 25, 91, 3, 2);
  Map_CopyCellsTo(41, 92, 25, 93, 3, 2);
  Map_CopyCellsTo(41, 96, 25, 95, 3, 2);
  Map_CopyCellsTo(41, 96, 25, 97, 3, 2);
  Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
  Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
}

/// Redraws the great statue's room with the second path open
/// (FLAG_VENUS_LIGHTHOUSE_SECOND_PATH), as VenusLighthouse_DrawFirstPath.
static inline void VenusLighthouse_DrawSecondPath(void) {
  Map_CopyCellsTo(111, 27, 89, 27, 7, 8);
  Map_CopyCellsTo(41, 90, 25, 91, 3, 2);
  Map_CopyCellsTo(41, 90, 25, 93, 3, 2);
  Map_CopyCellsTo(41, 90, 25, 95, 3, 2);
  Map_CopyCellsTo(41, 90, 25, 97, 3, 2);
  Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
  Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
  Map_CopyCellsTo(41, 94, 27, 92, 3, 2);
  Map_CopyCellsTo(41, 96, 29, 93, 3, 2);
  Map_CopyCellsTo(41, 94, 27, 94, 3, 2);
  Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
  Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
}

/// Searching the statue in VenusLighthouseGroundFloor: "The statue speaks to
/// Isaac's soul..." The room flashes, the path it opens changes (the first time
/// by the frame counter's parity, then to the other path) and its cells are
/// redrawn, and the camera shows it.
static void Event_SearchStatue(void) {
  struct FieldWork *work;

  work = (struct FieldWork *)gEventWork;
  work->revealFrames = 0;
  work->busy = 1;
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_MessageShowCentered(MsgVenusStatueSpeaksIsaacSoulYe, 1);
  ColorBuffer_ApplySource(PALETTE_TRANSFORM_PLAIN, 0);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PALE, 0);
  Engine_ColorBufferInterpolate(120);
  Engine_EventWait(100);
  Event_PlayCue(SE_142);
  Engine_EventWait(30);
  ColorBuffer_ApplyTarget(RGB(31, 31, 31), 0);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(70);
  // The first consultation selects a route; later consultations alternate it.
  if (GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) == 0) {
    if (GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH) == 0) {
      if ((gFrameCount & 1) != 0) {
        GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
      } else {
        GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
      }
    }
  }
  if (GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) == 0) {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
    GameFlag_Clear(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
    VenusLighthouse_DrawFirstPath();
  } else {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
    GameFlag_Clear(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
    VenusLighthouse_DrawSecondPath();
  }
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(20);
  Engine_EventWait(40);
  // Follow the revealed path after the statue and color effect have finished.
  Camera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  Camera_MoveTo(PIXELS(456), -1, PIXELS(542), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(50);
  Camera_MoveTo(PIXELS(456), -1, PIXELS(423), 1);
  Engine_CameraWaitForMove();
  Engine_EventEnd();
  work->busy = 0;
}

/// In VenusLighthouseGroundFloor, Move on the pillar by the wall: once it
/// reaches the wall, the first time only
/// (FLAG_VENUS_LIGHTHOUSE_PILLAR_AT_WALL), Garet comes out. "Hmmm. We can't
/// push the pillar farther than this." He wishes they could carry it, which is
/// what Carry then does.
static void Event_MovePillarTowardWall(void) {
  struct FieldActor *actor;
  struct FieldActor *leader;
  s32 conversationFlag;
  s32 walkSpeed;
  s32 walkAcceleration;
  s32 emote;
  s32 garetX;

  conversationFlag = FLAG_VENUS_LIGHTHOUSE_PILLAR_AT_WALL;
  walkSpeed = PIXELS(4) / 5;
  walkAcceleration = PIXELS(2) / 5;
  emote = EMOTE_IN_FRONT | EMOTE_EXCLAMATION;
  garetX = 824;
  Engine_EventBegin();
  actor = Object_GetById(ACTOR_WALL_PILLAR);
  if (actor->x.fixed >> 20 == 53) {
    if (Engine_GameFlagIsSet(conversationFlag) == 0) {
      // Record the one-time conversation before bringing Garet out to inspect
      // the pillar.
      Engine_GameFlagSet(conversationFlag);
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Engine_ActorSetPosition(GARET, actor->x.fixed, actor->z.fixed);
      }
      Engine_ActorSetSpeed(GARET, walkSpeed, walkAcceleration);
      Engine_ActorWalkToAndWait(GARET, garetX, 88);
      Engine_ActorWalkToAndWait(GARET, garetX, 104);
      Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
      Engine_EventWait(20);
      Engine_EventSetMessage(MsgVenusHmmmWeCantPushBlock);
      Engine_EventShowMessageAndWait(GARET, 0, 20);
      Engine_ActorFaceDirection(GARET, FACING_EAST, 10);
      Engine_ActorShowEmote(GARET, emote, 60);
      Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 0);
      Engine_EventWait(20);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Engine_EventWait(20);
      Engine_EventShowMessageAndWait(GARET, 0, 20);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
      Engine_EventWait(30);
      Engine_ActorWalkToAndWait(GARET, garetX, 88);
      Engine_ActorSetAnimation(GARET, ANIM_WALK);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                                   leader->z.part.pixel);
      }
      Engine_ActorWaitForMove(GARET);
      Engine_ActorSetPosition(GARET, 0, 0);
      Engine_EventEnd();
    }
  }
}

/// In VenusLighthouseGroundFloor, pushing the pillar by the wall (touch trigger
/// 50): as Move, with Garet's remark at the wall.
static void Event_PushPillarTowardWall(void) {

  Engine_EventBegin();
  VenusLighthouse_PushAdjacentPillar();
  Engine_EventWait(20);
  Engine_EventEnd();
  Event_MovePillarTowardWall();
}

/// In VenusLighthouseGroundFloor, Move on the pillar by the floor switch: on
/// the switch it sinks in and holds it down (FLAG_SCENE_TEMP_00).
static void Event_MovePillarOntoSwitch(void) {
  struct FieldActor *pillar;
  s32 switchHeight;

  pillar = Object_GetById(ACTOR_FLOOR_SWITCH_STATUE);
  Engine_EventBegin();
  if (pillar->x.fixed >> 20 == 42) {
    Engine_EventWait(30);
    Engine_AudioPlayCue(SE_188);
    pillar->motionFlags = 0;
    switchHeight = -PIXELS(2);
    pillar->terrainHeight = switchHeight;
    pillar->y.fixed = switchHeight;
    Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
    Map_CopyCellsTo(44, 117, 41, 117, 3, 5);
  }
  Engine_EventEnd();
}

/// In VenusLighthouseGroundFloor, standing on the floor switch presses it
/// (FLAG_SCENE_TEMP_01); stepping off releases it.
static void Event_StepOnFloorSwitch(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 x = leader->x.part.pixel;
  s32 z = leader->z.part.pixel;

  if ((u32)(x - 676) > 7 || z < 788 || z >= 796) {
    Map_CopyCellsTo(53, 50, 42, 49, 1, 1);
    Map_CopyCellsTo(55, 117, 41, 117, 3, 5);
    GameFlag_Clear(FLAG_SCENE_TEMP_01);
    leader->motionFlags |= ACTOR_FOLLOWS_TERRAIN;
    leader->terrainHeight = 0;
    leader->y.fixed = 0;
  } else if (!GameFlag_IsSet(FLAG_SCENE_TEMP_01)) {
    Engine_EventBegin();
    Engine_EventWait(5);
    Map_CopyCellsTo(52, 50, 42, 49, 1, 1);
    Map_CopyCellsTo(52, 117, 41, 117, 3, 5);
    GameFlag_Set(FLAG_SCENE_TEMP_01);
    Engine_AudioPlayCue(SE_161);
    leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
    leader->terrainHeight = -PIXELS(2);
    leader->y.fixed = -PIXELS(2);
    Engine_EventEnd();
  }
}

/// In VenusLighthouseGroundFloor, talking to the scholar by the door: he
/// explored once it opened, until Kraden and the others appeared and did
/// something to the statue that lit up the floor; the camera looks at the door.
static void Event_TalkToScholarByDoor(void) {
  struct FieldWork *work;

  work = (struct FieldWork *)gEventWork;
  work->revealFrames = 0;
  work->busy = 1;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVenusThoughtIdExploreAfterDoor);
  Actor_FaceActor(ACTOR_DOOR_SCHOLAR, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_DOOR_SCHOLAR, 0, 20);
  Actor_FaceDirection(ACTOR_DOOR_SCHOLAR, FACING_NORTHEAST, 0);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(448), -1, PIXELS(432), 1);
  Engine_CameraWaitForMove();
  Event_ShowMessage(ACTOR_DOOR_SCHOLAR, 0);
  Engine_EventEnd();
}

/// In VenusLighthouseFirstFloor, stepping onto the hole in the floor (trigger
/// 35): the leader looks down, falls through and lands in the room below (
/// VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM_HOLE).
static void Event_FallThroughFloor(void) {
  struct FieldActor *leader;
  struct FieldActor *leaderActor;
  s32 zero;
#if TBS_PATCH_3
  u8 *motionFlags;
#endif

#if !TBS_PATCH_3
  zero = 0;
#endif
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
#if TBS_PATCH_3
  motionFlags = (u8 *)&leader->motionFlags;
  zero = 0;
  *motionFlags = zero;
#endif
  Camera_MoveTo(-1, -1, -1, 0);
  leaderActor = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(leaderActor, 0);
  {
    u16 facing = FACING_SOUTH;

    leader->facing = facing;
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3), PIXELS(3) / 2);
  Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, leader->x.part.pixel, 552);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  Engine_EventWait(30);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  {
    s32 facing = FACING_NORTH;

    leader->facing = facing;
  }
  // The run cycle: his legs kick in the air as he falls.
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_RUN);
  Object_SetActionById(ACTOR_PARTY_LEADER, ANIM_STEPS_FAST);
  Engine_EventWait(40);
  // Start the fall only after the approach and descent animation have finished.
  leader->gravity = PIXELS(3) / 5;
  {
    s32 z = leader->z.fixed + PIXELS(72);

    leader->bounce = zero;
    VenusLighthouse_CreateBlendedPassageEffect(leader->x.fixed, 0, z,
                                               SPRITE_CRATE);
  }
  Map_CopyCellAttributes(VENUS_DESCENT_COLLISION_X, 35,
                         VENUS_DESCENT_COLLISION_WIDTH, 1,
                         VENUS_DESCENT_COLLISION_X, 34);
#if TBS_PATCH_3
  *motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
#endif
  OverlayObject_WaitUntilIdle(0);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Engine_EventRequestExit(
      VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM_HOLE);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// In VenusLighthouseFirstFloor, Move on one of the three pillars in a row: the
/// floor under each pillar is solid again and the nearer pillar draws in front.
static void Event_MoveDepthPillar(void) {

  u32 pillarIndex;
  struct FieldActor *pillar;
  s32 destinationColumn;
  s32 destinationRow;
  s32 column;
  s32 row;

  Object_GetById(ACTOR_FIRST_DEPTH_PILLAR);
  Engine_EventBegin();
  destinationColumn = 12;
  destinationRow = 44;
  Map_CopyCellAttributes(19, 44, 4, 1, destinationColumn, destinationRow);
  destinationColumn = 11;
  destinationRow = 51;
  Map_CopyCellAttributes(17, 51, 2, 2, destinationColumn, destinationRow);
  pillarIndex = 0;
  do {
    pillar = Object_GetById(pillarIndex + ACTOR_FIRST_DEPTH_PILLAR);
    column = pillar->x.fixed >> 20;
    row = pillar->z.fixed >> 20;
    Map_CopyCellAttributes(12, 50, 1, 1, column, row);
    pillarIndex++;
  } while (pillarIndex <= 2);
  SceneActor_SwapPositionsByDepth(ACTOR_FIRST_DEPTH_PILLAR + 2,
                                  ACTOR_FIRST_DEPTH_PILLAR + 1);
  Engine_EventEnd();
}

/// In VenusLighthouseFirstFloor, pushing one of the three pillars in a row.
static void Event_PushDepthPillar(void) {

  Engine_EventBegin();
  Map_CopyCellAttributes(19, 44, 4, 1, 12, 44);
  VenusLighthouse_PushAdjacentPillar();
  Event_MoveDepthPillar();
  Engine_EventEnd();
}

/// In VenusLighthouseFirstFloor, Move or Carry starting on one of the two
/// pillars by the gap: their tiles are marked before they move.
static void Event_LiftCarryPillar(void) {
  u32 pillarIndex;
  struct FieldActor *pillar;

  pillarIndex = 0;
  do {
    pillar = Object_GetById(pillarIndex + ACTOR_FIRST_CARRY_PILLAR);
    pillarIndex++;
    SetMapCellCollision(0, pillar->x.fixed, pillar->z.fixed, 45);
  } while (pillarIndex <= 1);
}

/// In VenusLighthouseFirstFloor, Move on one of the two pillars by the gap:
/// pillars still above the floor are solid again.
static void Event_MoveCarryPillar(void) {
  u32 pillarIndex;
  struct FieldActor *pillar;

  pillarIndex = 0;
  do {
    pillar = Object_GetById(pillarIndex + ACTOR_FIRST_CARRY_PILLAR);
    if (pillar->y.fixed > -PIXELS(16)) {
      SetMapCellCollision(0, pillar->x.fixed, pillar->z.fixed, 255);
    }
    pillarIndex++;
  } while (pillarIndex <= 1);
}

/// In VenusLighthouseFirstFloor, walking into a gap or a pillar by it: jump the
/// gap, else push the pillar.
static void Event_JumpOrPushCarryPillar(void) {

  Engine_EventBegin();
  if (Event_JumpAcrossGap() == 0) {
    Event_LiftCarryPillar();
    VenusLighthouse_PushAdjacentPillar();
    Event_MoveCarryPillar();
  }
  Engine_EventEnd();
}

/// Fixes a pillar at its destination and restores the cell beneath it.
static void VenusLighthouse_FixPillarAtDestination(struct FieldActor *pillar) {
  s32 column;
  s32 row;

  pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  pillar->motionFlags = 0;
  column = pillar->x.fixed >> 20;
  row = pillar->z.fixed >> 20;
  Map_CopyCellAttributes(9, 24, 1, 1, column, row);
}

/// Removes a fallen Carry pillar, or restores collision under the remaining
/// pillars.
static void
VenusLighthouse_ReleaseFallenCarryPillar(struct FieldActor *pillar) {
  struct AnimationObject *sprite;
  s32 zero;
  struct FieldActor *passageEffect;

  sprite = pillar->sprite;
  if (sprite->image.priority == 3) {
    sprite->image.priority = 1;
    zero = 0;
    pillar->bounce = zero;
    passageEffect = VenusLighthouse_CreateBlendedPassageEffect(
        pillar->x.fixed, 0, PIXELS(512), SPRITE_CRATE);
    OverlayObject_WaitUntilIdle(pillar);
    pillar->x.fixed = zero;
    pillar->z.fixed = zero;
    Engine_ObjectDispatchRelease(passageEffect);
  } else {
    Event_MoveCarryPillar();
  }
}

/// In VenusLighthouseFirstFloor, Carry on one of the two pillars by the gap:
/// set down in its place it stays fixed; dropped into the gap it falls through
/// the floor.
static void Event_CarryPillarAcrossGap(void) {
  struct FieldActor *pillar;

  Engine_EventBegin();
  pillar = Object_GetById(ACTOR_FIRST_CARRY_PILLAR);
  if (pillar->x.fixed >> 20 == 8) {
    OverlayObject_WaitUntilIdle(pillar);
    VenusLighthouse_FixPillarAtDestination(pillar);
  } else {
    VenusLighthouse_ReleaseFallenCarryPillar(pillar);
  }
  pillar = Object_GetById(ACTOR_FIRST_CARRY_PILLAR + 1);
  if (pillar->x.fixed >> 20 == 7) {
    OverlayObject_WaitUntilIdle(pillar);
    VenusLighthouse_FixPillarAtDestination(pillar);
  } else {
    VenusLighthouse_ReleaseFallenCarryPillar(pillar);
  }
  Engine_EventEnd();
}

/// In VenusLighthouseSecondFloor, stepping on the floor switch
/// (TRIGGER_WALL_SWITCH): the wall slides aside in a cloud of dust, the first
/// time only (FLAG_VENUS_LIGHTHOUSE_WALL_MOVED), and the scene cuts to
/// VenusLighthouseFirstFloor by
/// VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST, where
/// VenusLighthouse_AnimatePairedTileWaves shows what moved there before
/// bringing the party back.
static void Event_StepOnWallSwitch(void) {
  struct EffectOptions options;
  struct MapLayerScroll *scrollLayer;
  struct FieldActor *leader;
  s32 column;
  s32 leaderZ;
  s32 tileDelay;
  u32 frame;
  s32 dustX;

  scrollLayer = &((struct MapScrollWork *)gMapWork[0])->layers[2];
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  column = leader->x.part.pixel;
  leaderZ = leader->z.part.pixel;
  leader->y.fixed = 0;
  if (column >= 308 && column <= 315 && leaderZ >= 532 && leaderZ < 540) {
    leader->y.fixed = -PIXELS(2);
    if (!GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
      Engine_EventBegin();
      Engine_AudioPlayCue(SE_161);
      GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED);
      Map_CopyCellsTo(26, 33, 19, 33, 1, 1);
      Engine_EventWait(30);
      Engine_AudioPlayCue(SE_239);
      Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
      Engine_EventWait(20);
      dustX = PIXELS(288);
      column = 29;
      tileDelay = 40;
      // Scroll the layer while replacing the wall edge and emitting dust along
      // it.
      for (frame = 0; frame <= 479; frame++, tileDelay--) {
        scrollLayer->offsetX += PIXELS(1) / 5;
        dustX -= PIXELS(1) / 5;
        options.priority = 2;
        options.startScaleX =
            ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
        options.startScaleY =
            ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        FieldFx_Spawn(
            dustX, 0, PIXELS(528), 0, -(((gFrameCount & 1) * 3) << 16), 0,
            EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE | EFFECT_USE_SPIN,
            &options);
        if (frame == 240) {
          dustX += -PIXELS(48);
        }
        if (tileDelay == 0) {
          tileDelay = 40;
          if (frame <= 240) {
            column -= 4;
            Map_CopyCellsTo(column, 50, 15, 32, 3, 4);
          } else {
            column += 4;
            Map_CopyCellsTo(column, 45, 9, 32, 3, 4);
          }
        }
        Engine_TaskWait(1);
      }
      scrollLayer->offsetX += PIXELS(1) / 2;
      scrollLayer->offsetX = scrollLayer->offsetX / PIXELS(1) << 16;
      Map_CopyCellAttributes(15, 32, 3, 1, 9, 32);
      Map_CopyCellAttributes(12, 32, 3, 1, 15, 32);
      Engine_AudioPlayCue(SE_STOP_SLOT3);
      Engine_AudioPlayCue(SE_188);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapRenderWaitForValues();
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Engine_EventRequestExit(
          VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST);
      Engine_EventEnd();
    }
  }
}

/// In VenusLighthouseSecondFloor, Move on the pillar before the gate: in its
/// socket it sinks in and the gate opens, the first time only
/// (FLAG_VENUS_LIGHTHOUSE_GATE_OPEN).
static void Event_MoveGatePillar(void) {
  u8 *flags;
  s32 row;
  s32 column;
  s32 gateOpened;
  struct FieldActor *leader;
  struct FieldActor *pillar;
  struct FieldActor *pillarActor;
  struct FieldActor *occlusionEffect;

  occlusionEffect = 0;
  pillar = Object_GetById(ACTOR_GATE_PILLAR);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Map_CopyCellAttributes(109, 43, 7, 5, 45, 43);
  flags = &pillar->priorityFlags;
  if ((2 & pillar->priorityFlags) != 0) {
    Map_CopyCellAttributes(45, 45, 1, 1, 46, 45);
  } else {
    Map_CopyCellAttributes(48, 42, 1, 1, pillar->x.fixed >> 20,
                           pillar->z.fixed >> 20);
  }
  column = pillar->x.fixed >> 20;
  if (column != 46) {
  } else {
    row = pillar->z.fixed >> 20;
    if (row != 45) {
    } else {
      // Sink an unsolved pillar only when both of its target cell coordinates
      // match.
      gateOpened = Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN);
      if (gateOpened != 0) {
      } else {
        if ((leader->z.fixed >> 20) <= 45) {
          occlusionEffect = VenusLighthouse_CreateOccludingPassageEffect(
              PIXELS(744), 0, PIXELS(704), SPRITE_CHEST);
          Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
        }
        pillarActor = Object_GetById(ACTOR_GATE_PILLAR);
        Engine_ActorSetSpriteFlags(pillarActor, 0);
        pillar->terrainId = gateOpened;
        pillar->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        pillar->gravity = PIXELS(1) / 10;
        pillar->bounce = gateOpened;
        Map_CopyCellAttributes(43, 45, 1, 1, column, row);
        OverlayObject_WaitUntilIdle(pillar);
        Engine_AudioPlayCue(SE_188);
        pillar->motionFlags = gateOpened;
        pillar->y.fixed = -PIXELS(16);
        Engine_ActorSetSpritePriority(ACTOR_GATE_PILLAR, 3);
        {
          u8 two = 2;

          *flags = two;
        }
        Map_CopyCellAttributes(45, 45, 1, 1, column, row);
        Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
            ACTOR_PRIORITY_AUTOMATIC;
        Engine_ObjectDispatchRelease(occlusionEffect);
        pillar->collisionFlags = gateOpened;
        *flags |= 2;
        Object_GetById(ACTOR_GATE_EFFECT)->collisionFlags = gateOpened;
        Object_GetById(ACTOR_GATE_EFFECT)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetPosition(ACTOR_GATE_EFFECT, PIXELS(808), PIXELS(728));
        Actor_SetActionScript(ACTOR_GATE_EFFECT, gVenusSecondFloorLightToGate);
        Object_RefreshSelectorById(ACTOR_GATE_EFFECT);
        Engine_EventWait(30);
        Engine_AudioPlayCue(SE_DOOR_OPEN);
        Engine_MapAnimateCells(gVenusSecondFloorGateCells, 110, 41);
        Map_CopyCellAttributes(46, 41, 1, 1, column, 42);
        Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN);
      }
    }
  }
  Engine_EventEnd();
}

/// In VenusLighthouseSecondFloor, walking into the gap or the pillar before the
/// gate: jump the gap, else push the pillar, which may sink into its socket.
static void Event_JumpOrPushGatePillar(void) {

  Engine_EventBegin();
  if (Event_JumpAcrossGap() == 0) {
    Map_CopyCellAttributes(109, 43, 7, 5, 45, 43);
    VenusLighthouse_PushAdjacentPillar();
  }
  Engine_EventEnd();
  Event_MoveGatePillar();
}

/// In VenusLighthouseThirdFloor, Move on one of the three pillars: a pillar on
/// a free socket sinks in, and with all three sunk the gate opens
/// (FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN).
static void Event_MoveSwitchPillar(void) {
  struct FieldActor *occlusionEffect = 0;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *pillar;
  struct FieldActor *other;
  struct FieldActor *firstGateEffect;
  struct FieldActor *secondGateEffect;
  u8 *flags;
  u32 pillarId;
  u32 candidateIndex;
  u32 switchIndex;
  s32 savedPriority;

  Engine_EventBegin();
  Map_CopyCellAttributes(108, 39, 13, 7, 44, 39);
  // Find pillars on unoccupied switch cells before committing any sinking
  // motion.
  for (pillarId = ACTOR_FIRST_SWITCH_PILLAR;
       pillarId <= ACTOR_FIRST_SWITCH_PILLAR + 2; pillarId++) {
    pillar = Object_GetById(pillarId);
    flags = &pillar->priorityFlags;
    if (*flags != 2) {
      Map_CopyCellAttributes(47, 39, 1, 1, pillar->x.fixed >> 20,
                             pillar->z.fixed >> 20);
    } else {
      Map_CopyCellAttributes(46, 39, 1, 1, pillar->x.fixed >> 20,
                             pillar->z.fixed >> 20);
    }
    switchIndex = 5;
    for (candidateIndex = 0; candidateIndex <= 3; candidateIndex++) {
      if (pillar->x.fixed >> 20 == gVenusThirdFloorSockets[candidateIndex].x &&
          pillar->z.fixed >> 20 == gVenusThirdFloorSockets[candidateIndex].z &&
          pillar->y.fixed >= 0) {
        switchIndex = candidateIndex;
        break;
      }
    }
    if (switchIndex == 5) {
      continue;
    }
    for (candidateIndex = ACTOR_FIRST_SWITCH_PILLAR;
         candidateIndex <= ACTOR_FIRST_SWITCH_PILLAR + 2; candidateIndex++) {
      other = Object_GetById(candidateIndex);
      if (pillarId != candidateIndex &&
          pillar->x.fixed >> 20 == other->x.fixed >> 20 &&
          pillar->z.fixed >> 20 == other->z.fixed >> 20) {
        switchIndex = 5;
        break;
      }
    }
    if (switchIndex == 5) {
      continue;
    }
    // Preserve overlapping actors while the selected pillar sinks into its
    // socket.
    savedPriority = leader->sprite->image.priority;
    if (leader->z.fixed >> 20 <= gVenusThirdFloorSockets[switchIndex].z) {
      occlusionEffect = VenusLighthouse_CreateOccludingPassageEffect(
          pillar->x.fixed, pillar->y.fixed, pillar->z.fixed - PIXELS(4),
          SPRITE_CHEST);
      Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
    }
    Engine_ActorSetSpriteFlags(Object_GetById(pillarId), 0);
    pillar->terrainId = 0;
    pillar->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    pillar->bounce = 0;
    pillar->gravity = PIXELS(1) / 10;
    Map_CopyCellAttributes(42, 41, 1, 1, gVenusThirdFloorSockets[switchIndex].x,
                           gVenusThirdFloorSockets[switchIndex].z);
    OverlayObject_WaitUntilIdle(pillar);
    Engine_AudioPlayCue(SE_188);
    pillar->collisionFlags = 0;
    pillar->motionFlags = 0;
    pillar->y.fixed = -PIXELS(16);
    Engine_ActorSetSpritePriority(pillarId, 3);
    {
      u8 two = 2;

      *flags = two;
    }
    Map_CopyCellAttributes(46, 39, 1, 1, gVenusThirdFloorSockets[switchIndex].x,
                           gVenusThirdFloorSockets[switchIndex].z);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, savedPriority);
    Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
    if (occlusionEffect != 0) {
      Engine_ObjectDispatchRelease(occlusionEffect);
    }
    if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN)) {
      Engine_EventEnd();
      return;
    }
    if ((Object_GetById(ACTOR_FIRST_SWITCH_PILLAR)->priorityFlags &
         Object_GetById(ACTOR_FIRST_SWITCH_PILLAR + 1)->priorityFlags &
         Object_GetById(ACTOR_FIRST_SWITCH_PILLAR + 2)->priorityFlags & 2) ==
        0) {
      continue;
    }
    // Once all three pillars settle, run the paired scripts that open the gate.
    firstGateEffect =
        VenusLighthouse_CreateLight(888, 680, gVenusThirdFloorLightToNortheast);
    secondGateEffect =
        VenusLighthouse_CreateLight(888, 680, gVenusThirdFloorLightToSoutheast);
    // SC_FREE clears the script pointer; work value 0x28 signals the
    // opening cue earlier, while the gate effects are still running.
    while (((struct ScriptInterpreter *)firstGateEffect)->script != 0 ||
           ((struct ScriptInterpreter *)secondGateEffect)->script != 0) {
      if (firstGateEffect->riseEnabled != 0 ||
          secondGateEffect->riseEnabled != 0) {
        Engine_EventWait(30);
        Engine_AudioPlayCue(SE_DOOR_OPEN);
        Engine_MapAnimateCells(gVenusThirdFloorGateCells, 109, 37);
        Map_CopyCellAttributes(45, 37, 1, 1, 45, 38);
        if (Object_GetById(ACTOR_FIRST_SWITCH_PILLAR)->x.fixed >> 20 ==
                gVenusThirdFloorSockets[0].x &&
            Object_GetById(ACTOR_FIRST_SWITCH_PILLAR)->z.fixed >> 20 ==
                gVenusThirdFloorSockets[0].z) {
          Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_SOCKETS_A);
        } else {
          Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_SOCKETS_B);
        }
        Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN);
        break;
      }
      Engine_TaskWait(1);
    }
  }
  Engine_EventEnd();
}

/// In VenusLighthouseThirdFloor, walking into the gap or one of the three
/// pillars: jump the gap, else push the pillar, which may sink into a socket.
static void Event_JumpOrPushSwitchPillar(void) {
  Engine_EventBegin();
  if (Event_JumpAcrossGap() == 0) {
    Map_CopyCellAttributes(108, 39, 13, 7, 44, 39);
    VenusLighthouse_PushAdjacentPillar();
  }
  Engine_EventEnd();
  Event_MoveSwitchPillar();
}

// Keep a stacked pillar sixteen pixels above its supporting actor.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 7 bytes (extent 34 to 32 bytes). Keep the
// unused native result in matching builds.
#ifdef UBFIX
static void
VenusLighthouse_FollowSupportingPillarHeight(union FieldObject *fieldObject) {
#else
static s32
VenusLighthouse_FollowSupportingPillarHeight(union FieldObject *fieldObject) {
#endif
  struct FieldActor *pillar = (struct FieldActor *)fieldObject;
  struct FieldActor *support;

  support = Object_GetById(pillar->timer);
  pillar->y.fixed = support->y.fixed + PIXELS(16);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Moves the floating supports to the heights selected by their timers.
static void VenusLighthouse_LowerFloatingPillars(s32 waitFrames) {
  struct FieldActor *westSupport = Object_GetById(ACTOR_BALANCE_WEST);
  struct FieldActor *eastSupport = Object_GetById(ACTOR_BALANCE_EAST);
  u32 pillarIndex;

  Actor_SetSpeed(ACTOR_BALANCE_WEST, PIXELS(1) / 2, PIXELS(1) / 4);
  Engine_ActorSetSpeed(ACTOR_BALANCE_EAST, PIXELS(1) / 2, PIXELS(1) / 4);
  if (waitFrames != 0) {
    Engine_AudioPlayCue(SE_180);
  }
  FieldObject_SetPosition(westSupport, westSupport->x.fixed,
                          gVenusBalancePlatformHeights[westSupport->timer],
                          westSupport->z.fixed);
  FieldObject_SetPosition(eastSupport, eastSupport->x.fixed,
                          gVenusBalancePlatformHeights[eastSupport->timer],
                          eastSupport->z.fixed);
  Engine_ActorWaitForMove(ACTOR_BALANCE_WEST);
  Engine_ActorWaitForMove(ACTOR_BALANCE_EAST);
  westSupport->y.fixed = gVenusBalancePlatformHeights[westSupport->timer];
  eastSupport->y.fixed = gVenusBalancePlatformHeights[eastSupport->timer];
  if (waitFrames != 0) {
    Engine_AudioPlayCue(SE_289);
  }
  for (pillarIndex = 0; pillarIndex < 5; pillarIndex++) {
    struct FieldActor *actor = Object_GetById(pillarIndex + ACTOR_BALANCE_WEST);

    if (actor->y.fixed / PIXELS(1) < 0 && actor->y.fixed / PIXELS(1) > -30) {
      Map_CopyCellAttributes(4, 19, 1, 1, actor->x.fixed >> 20,
                             actor->z.fixed >> 20);
    }
  }
  Engine_EventWait(waitFrames);
}

/// Moves a floating pillar atop the highest support in its map cell.
static void VenusLighthouse_StackFloatingPillar(s32 pillarId) {

  struct FieldActor *candidate;
  s32 supportHeight = -PIXELS(80);
  struct FieldActor *pillar = 0;
  u32 candidateIndex;

  for (candidateIndex = 0; candidateIndex <= 5; candidateIndex++) {
    s32 candidateId = candidateIndex + ACTOR_BALANCE_WEST;
    s32 candidateTop;

    if (candidateId == pillarId) {
      continue;
    }

    candidate = Object_GetById(candidateId);
    pillar = Object_GetById(pillarId);

    if ((candidate->x.fixed >> 20) != (pillar->x.fixed >> 20) ||
        (candidate->z.fixed >> 20) != (pillar->z.fixed >> 20)) {
      continue;
    }

    candidateTop = candidate->y.fixed + PIXELS(16);
    if (supportHeight <= candidateTop) {
      pillar->timer = candidateId;
      supportHeight = candidateTop;
    }
  }

  Engine_ActorSetSpeed(pillarId, PIXELS(4), PIXELS(2));
  Engine_ObjectSetPosition(pillar, pillar->x.fixed, supportHeight,
                           pillar->z.fixed);
  Engine_ActorWaitForMove(pillarId);
  Engine_AudioPlayCue(SE_188);
  FieldFx_SpawnNineRadialEffects(pillarId);
  Engine_EventWait(30);
}

/// Stops a floating pillar and resets its vertical motion and timer.
static __inline__ void FloatingPillar_Settle(struct FieldActor *pillar,
                                             s32 resetValue) {
  pillar->targetY = ACTOR_NO_TARGET;
  pillar->terrainHeight = resetValue;
  pillar->velocityY = resetValue;
  pillar->motionFlags = resetValue;
  pillar->timer = resetValue;
}

/// Copies all three fixed-point position components without changing motion.
static __inline__ void Actor_CopyPosition(struct FieldActor *destination,
                                          struct FieldActor *source) {
  destination->x.fixed = source->x.fixed;
  destination->y.fixed = source->y.fixed;
  destination->z.fixed = source->z.fixed;
}

/// In VenusLighthouseFourthFloor, Carry on one of the four floating pillars: it
/// settles where it is set down (FLAG_SCENE_TEMP_00 plus its number) and the
/// platforms under the pillars rise or sink to their weight.
static void Event_CarryFloatingPillar(void) {
  struct FieldActor savedPosition;
  struct FieldActor *firstPassageEffect = NULL;
  struct FieldActor *secondPassageEffect = NULL;
  struct FieldActor *pillar;
  struct FieldActor *other;
  s32 pillarIndex;
  s32 earlierIndex;
  s32 settledIndex;
  s32 column;
  s32 row;

  Engine_EventBegin();
  // Handle completed destinations and fallen pillars before reordering a new
  // stack.
  for (pillarIndex = 0; pillarIndex < 4; pillarIndex++) {
    pillar = Object_GetById(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR);
    column = pillar->x.fixed >> 20;
    if (column == 13 && (row = pillar->z.fixed >> 20) == 7 &&
        !GameFlag_IsSet(FLAG_SCENE_TEMP_00 + pillarIndex)) {
      OverlayObject_WaitUntilIdle(pillar);
      GameFlag_Set(FLAG_SCENE_TEMP_00 + pillarIndex);
      pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      pillar->collisionFlags = 0;
      pillar->motionFlags = 0;
      Map_CopyCellAttributes(4, 19, 1, 1, column, row);
      break;
    }
    if (pillar->sprite->image.priority == 3 &&
        !GameFlag_IsSet(FLAG_SCENE_TEMP_00 + pillarIndex)) {
      /* FAKEMATCH(temporary): the zero in the search counter, set before the
       * priority call; as block->bounce = 0 after the call, 2 of 272
       * instructions differ in tbs-en. */
      earlierIndex = 0;
      Engine_ActorSetSpritePriority(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR,
                                    1);
      pillar->bounce = earlierIndex;
      if (pillar->z.fixed >> 20 <= 12) {
        firstPassageEffect = VenusLighthouse_CreateBlendedPassageEffect(
            pillar->x.fixed, 0, PIXELS(224), SPRITE_CAPPED_PILLAR);
        secondPassageEffect = VenusLighthouse_CreateBlendedPassageEffect(
            pillar->x.fixed, 0, PIXELS(240), SPRITE_CAPPED_PILLAR);
      }
      OverlayObject_WaitUntilIdle(pillar);
      Engine_ActorSetPosition(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR, 0, 0);
      Engine_ObjectDispatchRelease(firstPassageEffect);
      Engine_ObjectDispatchRelease(secondPassageEffect);
      GameFlag_Set(FLAG_SCENE_TEMP_00 + pillarIndex);
      break;
    }
    if (pillar->z.fixed >> 20 == 19 &&
        !GameFlag_IsSet(FLAG_SCENE_TEMP_00 + pillarIndex)) {
      // Keep settled pillars at the front of the actor slots before finding
      // their support.
      FloatingPillar_Settle(pillar, 0);
      settledIndex = pillarIndex;
      for (earlierIndex = 0; earlierIndex < pillarIndex; earlierIndex++) {
        if (!GameFlag_IsSet(FLAG_SCENE_TEMP_00 + earlierIndex)) {
          other = Object_GetById(earlierIndex + ACTOR_FIRST_FLOATING_PILLAR);
          Actor_CopyPosition(&savedPosition, pillar);
          Actor_CopyPosition(pillar, other);
          Actor_CopyPosition(other, &savedPosition);
          settledIndex = earlierIndex;
          break;
        }
      }
      other = Object_GetById(settledIndex + ACTOR_FIRST_FLOATING_PILLAR);
      FloatingPillar_Settle(other, 0);
      Engine_CameraSetSpeed(PIXELS(3), PIXELS(3) / 8);
      Camera_GetEventObject()->motionFlags = 0;
      Engine_CameraMoveTo(PIXELS(136), PIXELS(8), PIXELS(344), 1);
      Engine_CameraWaitForMove();
      // Stack the pillar, then rebalance the two supports and attach the height
      // follower.
      VenusLighthouse_StackFloatingPillar(settledIndex +
                                          ACTOR_FIRST_FLOATING_PILLAR);
      if (Object_GetById(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)->x.fixed >>
              20 ==
          6) {
        Object_GetById(ACTOR_BALANCE_WEST)->timer++;
        Object_GetById(ACTOR_BALANCE_EAST)->timer--;
      } else {
        Object_GetById(ACTOR_BALANCE_WEST)->timer--;
        Object_GetById(ACTOR_BALANCE_EAST)->timer++;
      }
#ifdef UBFIX
      Object_GetById(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)->update =
          VenusLighthouse_FollowSupportingPillarHeight;
#else
      Object_GetById(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)->update =
          (void (*)(
              union FieldObject *))VenusLighthouse_FollowSupportingPillarHeight;
#endif
      VenusLighthouse_LowerFloatingPillars(40);
      Object_GetById(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)
          ->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      GameFlag_Set(FLAG_SCENE_TEMP_00 + settledIndex);
      break;
    }
  }
  Engine_EventEnd();
}

/// In VenusLighthouseFourthFloor, pushing a floating pillar
/// (TRIGGER_FLOATING_PILLARS and the gaps beside it).
static void Event_PushPuzzlePillar(void) {

  Engine_EventBegin();
  VenusLighthouse_PushAdjacentPillar();
  Engine_EventEnd();
}

/// In VenusLighthouseFourthFloor, stepping on the west floor switch: the west
/// wall slides aside, the first time only
/// (FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED), and the scene cuts to
/// VenusLighthouseThirdFloor by
/// VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST,
/// where VenusLighthouse_AnimateWestTileWave shows what moved there before
/// bringing the party back.
static void Event_StepOnWestWallSwitch(void) {
  struct EffectOptions options;
  struct MapLayerScroll *scrollLayer;
  struct FieldActor *leader;
  s32 column;
  s32 leaderZ;
  s32 tileDelay;
  u32 frame;
  s32 dustX;

  scrollLayer = &((struct MapScrollWork *)gMapWork[0])->layers[2];
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  column = leader->x.part.pixel;
  leaderZ = leader->z.part.pixel;
  leader->y.fixed = 0;
  if (column >= 532 && column <= 539 && leaderZ >= 324 && leaderZ < 332) {
    leader->y.fixed = -PIXELS(2);
    if (!Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
      Engine_EventBegin();
      Map_CopyCellsTo(63, 29, 33, 20, 1, 1);
      Engine_AudioPlayCue(SE_161);
      Map_CopyCellsTo(44, 83, 44, 80, 3, 3);
      Engine_EventWait(30);
      Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
      Engine_AudioPlayCue(SE_239);
      Engine_EventWait(20);
      dustX = PIXELS(616);
      column = 0;
      tileDelay = 60;
      // Move the west wall while its visible edge exposes successive tile
      // strips.
      for (frame = 0; frame <= 319; frame++, tileDelay--) {
        scrollLayer->offsetX += PIXELS(1) / 5;
        if (dustX >= PIXELS(576) && frame > 40) {
          dustX -= PIXELS(1) / 5;
          options.priority = 2;
          options.startScaleX =
              ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
          options.startScaleY =
              ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
          options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
          FieldFx_Spawn(
              dustX, 0, PIXELS(288), 0, -(((gFrameCount & 1) * 3) << 16), 0,
              EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE | EFFECT_USE_SPIN,
              &options);
          if (tileDelay == 0) {
            tileDelay = 40;
            column += 4;
            Map_CopyCellsTo(column, 56, 36, 17, 3, 4);
          }
        }
        Engine_TaskWait(1);
      }
      scrollLayer->offsetX += PIXELS(1) / 2;
      scrollLayer->offsetX = scrollLayer->offsetX / PIXELS(1) << 16;
      Engine_AudioPlayCue(SE_STOP_SLOT3);
      Engine_AudioPlayCue(SE_188);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapRenderWaitForValues();
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Engine_EventRequestExit(
          VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST);
      Engine_EventEnd();
    }
  }
}

/// In VenusLighthouseFourthFloor, stepping on the east floor switch: the east
/// wall slides aside, the first time only
/// (FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED), and the scene cuts to
/// VenusLighthouseThirdFloor by
/// VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST,
/// where VenusLighthouse_AnimateEastTileWave shows what moved there before
/// bringing the party back.
static void Event_StepOnEastWallSwitch(void) {
  struct EffectOptions options;
  struct MapLayerScroll *scrollLayer;
  struct FieldActor *leader;
  s32 column;
  s32 leaderZ;
  s32 tileDelay;
  u32 frame;
  s32 dustX;

  scrollLayer = &((struct MapScrollWork *)gMapWork[0])->layers[2];
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  column = leader->x.part.pixel;
  leaderZ = leader->z.part.pixel;
  leader->y.fixed = 0;
  if (column >= 788 && column <= 795 && leaderZ >= 324 && leaderZ < 332) {
    leader->y.fixed = -PIXELS(2);
    if (!Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
      Engine_EventBegin();
      Map_CopyCellsTo(63, 29, 49, 20, 1, 1);
      Engine_AudioPlayCue(SE_161);
      Engine_EventWait(30);
      Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
      Engine_AudioPlayCue(SE_239);
      Engine_EventWait(20);
      dustX = PIXELS(704);
      column = 61;
      tileDelay = 60;
      // Move the east wall in the opposite direction, keeping its distinct dust
      // limits.
      for (frame = 0; frame <= 319; frame++, tileDelay--) {
        scrollLayer->offsetX -= PIXELS(1) / 5;
        dustX += PIXELS(1) / 5;
        if (dustX >= PIXELS(712) && dustX < PIXELS(752)) {
          options.priority = 2;
          options.startScaleX =
              ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
          options.startScaleY =
              ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
          options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
          FieldFx_Spawn(
              dustX, 0, PIXELS(288), 0, -(((gFrameCount & 1) * 3) << 16), 0,
              EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE | EFFECT_USE_SPIN,
              &options);
          if (tileDelay == 0) {
            tileDelay = 40;
            column -= 4;
            Map_CopyCellsTo(column, 56, 44, 17, 3, 4);
          }
        }
        Engine_TaskWait(1);
      }
      scrollLayer->offsetX += PIXELS(1) / 2;
      scrollLayer->offsetX = scrollLayer->offsetX / PIXELS(1) << 16;
      Engine_AudioPlayCue(SE_STOP_SLOT3);
      Engine_AudioPlayCue(SE_188);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_MapRenderWaitForValues();
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Engine_EventRequestExit(
          VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST);
      Engine_EventEnd();
    }
  }
}

/// In VenusLighthouseFourthFloor, Move on one of the four pillars in the pillar
/// room: on a free socket it sinks in, and with all four sunk the gate opens
/// (FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN).
static void Event_MoveSocketPillar(void) {
  struct FieldActor *occlusionEffect;
  struct FieldActor *pillar;
  struct FieldActor *otherPillar;
  struct FieldActor *leader;
  struct FieldActor *firstPillar;
  struct FieldActor *secondPillar;
  struct FieldActor *thirdPillar;
  struct FieldActor *fourthPillar;
  u8 *flags;
  u8 *motionFlags;
  u32 pillarId;
  u32 candidateIndex;
  u32 switchIndex;
  u32 savedPriority;

  occlusionEffect = 0;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Begin();
#if TBS_PATCH_3
  Field_EndBusyAndReveal();
#endif
  Map_CopyCellAttributes(69, 48, 4, 2, 5, 48);
  Map_CopyCellAttributes(73, 37, 9, 13, 9, 37);
  // Find a pillar on a clear pillar socket and preserve overlapping draw order.
  for (pillarId = ACTOR_FIRST_SOCKET_PILLAR;
       pillarId <= ACTOR_FIRST_SOCKET_PILLAR + 3; pillarId++) {
    pillar = Object_GetById(pillarId);
    flags = &pillar->priorityFlags;
    if (*flags != 2)
      Map_CopyCellAttributes(72, 48, 1, 1, pillar->x.fixed >> 20,
                             pillar->z.fixed >> 20);
    else
      Map_CopyCellAttributes(73, 48, 1, 1, pillar->x.fixed >> 20,
                             pillar->z.fixed >> 20);

    switchIndex = 8;
    for (candidateIndex = 0; candidateIndex < 8; candidateIndex++) {
      if ((pillar->x.fixed >> 20) ==
              gVenusFourthFloorSockets[candidateIndex].x &&
          (pillar->z.fixed >> 20) ==
              gVenusFourthFloorSockets[candidateIndex].z &&
          pillar->y.fixed >= 0) {
        switchIndex = candidateIndex;
        break;
      }
    }
    if (switchIndex == 8)
      continue;
    for (candidateIndex = ACTOR_FIRST_SOCKET_PILLAR;
         candidateIndex <= ACTOR_FIRST_SOCKET_PILLAR + 3; candidateIndex++) {
      otherPillar = Object_GetById(candidateIndex);
      if (pillarId != candidateIndex &&
          (pillar->x.fixed >> 20) == (otherPillar->x.fixed >> 20) &&
          (pillar->z.fixed >> 20) == (otherPillar->z.fixed >> 20)) {
        switchIndex = 8;
        break;
      }
    }
    if (switchIndex == 8)
      continue;

    savedPriority = leader->sprite->image.priority;
    if ((leader->z.fixed >> 20) <= gVenusFourthFloorSockets[switchIndex].z) {
      occlusionEffect = VenusLighthouse_CreateOccludingPassageEffect(
          pillar->x.fixed, pillar->y.fixed, pillar->z.fixed - PIXELS(4),
          SPRITE_CHEST);
      Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    }
    for (candidateIndex = ACTOR_FIRST_SOCKET_PILLAR;
         candidateIndex <= ACTOR_FIRST_SOCKET_PILLAR + 3; candidateIndex++) {
      otherPillar = Object_GetById(candidateIndex);
      if (pillarId != candidateIndex &&
          (pillar->x.fixed >> 20) == (otherPillar->x.fixed >> 20) &&
          (pillar->z.fixed >> 20) - 1 == (otherPillar->z.fixed >> 20))
        Actor_SetSpritePriority(candidateIndex, 3);
    }
    Actor_SetSpriteFlags(Object_GetById(pillarId), 0);
    pillar->terrainId = 0;
    motionFlags = &pillar->motionFlags;
    *motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    ((union FieldObject *)pillar)->effect.velocity.y = PIXELS(1) / 10;
    ((union FieldObject *)pillar)->effect.velocity.x = 0;
    Map_CopyCellAttributes(6, 44, 1, 1, gVenusFourthFloorSockets[switchIndex].x,
                           gVenusFourthFloorSockets[switchIndex].z);
    OverlayObject_WaitUntilIdle(pillar);
    Event_PlayCue(SE_188);
    pillar->collisionFlags = 0;
    *motionFlags = 0;
    pillar->y.fixed = -PIXELS(16);
    Actor_SetSpritePriority(pillarId, 3);
    *flags = 2;
    Map_CopyCellAttributes(73, 48, 1, 1,
                           gVenusFourthFloorSockets[switchIndex].x,
                           gVenusFourthFloorSockets[switchIndex].z);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, savedPriority);
    Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
    for (candidateIndex = ACTOR_FIRST_SOCKET_PILLAR;
         candidateIndex <= ACTOR_FIRST_SOCKET_PILLAR + 3; candidateIndex++) {
      otherPillar = Object_GetById(candidateIndex);
      if (pillarId != candidateIndex &&
          (pillar->x.fixed >> 20) == (otherPillar->x.fixed >> 20) &&
          (pillar->z.fixed >> 20) - 1 == (otherPillar->z.fixed >> 20)) {
        Actor_SetSpritePriority(candidateIndex, 1);
        Object_GetById(candidateIndex)->priorityFlags |=
            ACTOR_PRIORITY_AUTOMATIC;
      }
    }
    Engine_ObjectDispatchRelease(occlusionEffect);
    if (GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN)) {
      Event_End();
      return;
    }
    // Open the paired gate only after all four pillars are seated.
    firstPillar = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR);
    secondPillar = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR + 1);
    thirdPillar = Actor_Get(ACTOR_FIRST_SOCKET_PILLAR + 2);
    fourthPillar = Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 3);
    if ((firstPillar->priorityFlags & secondPillar->priorityFlags &
         thirdPillar->priorityFlags & fourthPillar->priorityFlags) &
        2) {
      struct FieldActor *firstGateEffect;
      struct FieldActor *secondGateEffect;

      Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
      Object_PlaceCurrentWithinCameraBounds(ACTOR_SOCKET_EFFECT_STATUE, 1);
      Camera_WaitForMove();
      firstGateEffect = VenusLighthouse_CreateLight(
          136, 776, gVenusFourthFloorLightFromStatue);
      Event_Wait(30);
      Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
      Camera_MoveTo(PIXELS(216), -1, PIXELS(632), 1);
      ObjectDispatch_WaitForValue16(firstGateEffect);
      Object_SetScript(firstGateEffect, gVenusFourthFloorLightToSoutheast);
      secondGateEffect = VenusLighthouse_CreateLight(
          216, 760, gVenusFourthFloorLightToSouthwest);
      while (((struct ScriptInterpreter *)firstGateEffect)->script != 0 ||
             ((struct ScriptInterpreter *)secondGateEffect)->script != 0) {
        if (firstGateEffect->riseEnabled != 0 ||
            secondGateEffect->riseEnabled != 0) {
          Event_Wait(30);
          Map_AnimateCells(gVenusFourthFloorGateCells, 77, 35);
          Map_CopyCellAttributes(13, 35, 1, 1, 13, 36);
          GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN);
          break;
        }
        Task_Wait(1);
      }
    }
  }
  Event_End();
}

/// In VenusLighthouseFourthFloor, walking into the gap or a pillar-room pillar:
/// jump the gap, else push the pillar, which may sink into a socket.
static void Event_JumpOrPushSocketPillar(void) {

  Engine_EventBegin();
  if (Event_JumpAcrossGap() == 0) {
    {
      Map_CopyCellAttributes(69, 48, 4, 2, 5, 48);
    }
    {
      Map_CopyCellAttributes(73, 37, 9, 13, 9, 37);
    }
    VenusLighthouse_PushAdjacentPillar();
  }
  Engine_EventEnd();
  Event_MoveSocketPillar();
}

/// In VenusLighthouseFourthFloor, walking south into the passage
/// (TRIGGER_RISING_PASSAGE): the leader rises out of the room to the floor
/// above (
/// VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES_RISING, where
/// VenusLighthouse_RaiseLeaderWithSpray lands him on ACTOR_RISING_PLATFORM).
static void Event_EnterRisingPassage(void) {
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 520, 712);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  (void)VenusLighthouse_CreateBlendedPassageEffect(PIXELS(520), 0, PIXELS(784),
                                                   SPRITE_CRATE);
  FieldActor_JumpDownLedge(0, 6, 0);
  Engine_EventWait(60);
  Engine_EventRequestExit(
      VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES_RISING);
  Engine_EventEnd();
}

/// Selects the puzzle interactions and dialogue for the current lighthouse
/// scene.
const struct SceneEvent *VenusLighthouse_GetEvents(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VenusLighthouseEntry) {
    return gVenusLighthouseEntryEvents;
  }
  if (scene == (s32)&SceneId_VenusLighthouseGroundFloor) {
    return gVenusLighthouseGroundFloorEvents;
  }
  if (scene == (s32)&SceneId_VenusLighthouseFirstFloor) {
    return gVenusLighthouseFirstFloorEvents;
  }
  if (scene == (s32)&SceneId_VenusLighthouseSecondFloor) {
    return gVenusLighthouseSecondFloorEvents;
  }
  if (scene == (s32)&SceneId_VenusLighthouseThirdFloor) {
    return gVenusLighthouseThirdFloorEvents;
  }
  if (scene == (s32)&SceneId_VenusLighthouseFourthFloor) {
    return gVenusLighthouseFourthFloorEvents;
  }
  return gVenusLighthouseGroundFloorEvents;
}

/// Lays one of the scholars or soldiers the wanderers hurt on the ground
/// (ANIM_HURT_LYING), drawn behind the backgrounds' fronts, still and without
/// the lower two bits of its collision.
static void VenusLighthouse_LayDownHurtActor(s32 actorId) {
  struct FieldActor *actor;
  s32 collisionFlags;

  actor = Object_GetById(actorId);
  actor->motionFlags = 0;
  collisionFlags = 252;
  collisionFlags &= actor->collisionFlags;
  actor->collisionFlags = collisionFlags;
  Engine_ActorSetSpriteFlags(actor, 0);
  Object_SetMode(actor, ANIM_HURT_LYING);
  Engine_ActorSetSpritePriority(actorId, 3);
  {
    s32 priorityFlags = 2;
    priorityFlags |= actor->priorityFlags;
    actor->priorityFlags = priorityFlags;
  }
}

/// Visits the four pillar actors through the object lookup service.
static void VenusLighthouse_VisitPillarActors(void) {
  u32 actorId;

  actorId = ACTOR_FIRST_SOCKET_PILLAR;
  do {
    Object_GetById(actorId);
    actorId++;
  } while (actorId <= ACTOR_FIRST_SOCKET_PILLAR + 3);
}

/// Restores puzzle progress, actors and entrance effects for all six scenes,
/// by scene and entrance; on every arrival it sets FLAG_VENUS_LIGHTHOUSE_LIT
/// and, after a battle, restores the scripted actors' child values.
/// VenusLighthouseGroundFloor's
/// VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR, with
/// VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_PILLARS and
/// _FROM_GREAT_STATUE into the same room, and VenusLighthouseFirstFloor's
/// VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR clear
/// FLAG_SHOW_LOCATION_NAME.
s32 VenusLighthouse_Initialize(void) {
  struct FieldActor *actor;
  s32 scene;
  u32 pillarIndex;
  s32 column;
  s32 row;

  Engine_EventWait(1);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION))
    VenusLighthouse_RestoreScriptedChildValues();
  Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_LIT);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  scene = gGameState.scene;
  // Restore the scholars and soldiers outside and each interior puzzle by its
  // entrance.
  if (scene == (s32)&SceneId_VenusLighthouseEntry) {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (!Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD))
      VenusLighthouse_LayDownHurtActor(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK);
    else
      Map_CopyCellAttributes(7, 17, 2, 1, 7, 16);
    VenusLighthouse_LayDownHurtActor(ACTOR_SCHOLAR_LIGHTHOUSE_OPEN);
    VenusLighthouse_LayDownHurtActor(ACTOR_SCHOLAR_BARBARIANS);
    VenusLighthouse_LayDownHurtActor(ACTOR_WAITING_SCHOLAR);
    Engine_ActorSetSpritePriority(ACTOR_WAITING_SCHOLAR, 2);
    VenusLighthouse_LayDownHurtActor(ACTOR_SOLDIER_OPENED_DOOR);
    Engine_ActorSetSpritePriority(ACTOR_SOLDIER_OPENED_DOOR, 2);
    VenusLighthouse_LayDownHurtActor(ACTOR_SOLDIER_HEADED_FOR_BABI);
    VenusLighthouse_LayDownHurtActor(ACTOR_SOLDIER_COLOSSO_VICTORS);
  } else if (scene == (s32)&SceneId_VenusLighthouseGroundFloor) {
    switch (gGameState.entrance) {
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_ENTRY:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_FROM_RELIEFS:
      VenusLighthouse_LayDownHurtActor(ACTOR_SCHOLAR_ATTACKED_ANYWAY);
      break;
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_WEST:
      Engine_GameFlagClear(FLAG_EXIT_ARRIVAL_PENDING);
      // The other scholar setup still applies.
      // fall through
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_FRONT_ROOM:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_EAST:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_PILLARS:
      VenusLighthouse_LayDownHurtActor(ACTOR_SCHOLAR_NOT_THE_ENTRANCE);
      break;
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_FLOOR_SWITCH:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_SMALL_ROOM:
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH)) {
        VenusLighthouse_DrawFirstPath();
      } else {
        if (!Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
          break;
        VenusLighthouse_DrawSecondPath();
      }
      break;
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_GREAT_STATUE:
      Engine_GameFlagClear(FLAG_EXIT_ARRIVAL_ALTERNATE);
      // Restore the same passage tiles as the room's other two entrances.
      // fall through
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_PILLARS:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR:
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00))
        Map_CopyCellsTo(44, 117, 41, 117, 3, 5);
      break;
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_RELIEFS:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_CARRY_STONE:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_FLOOR_SWITCH:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_NORTH:
    case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_WEST:
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED)) {
        Actor_SetPosition(ACTOR_WALL_PILLAR, PIXELS(872), PIXELS(88));
        actor = Object_GetById(ACTOR_WALL_PILLAR);
        actor->y.fixed = -PIXELS(24);
        actor->targetY = ACTOR_NO_TARGET;
      }
      break;
    }
  } else if (scene == (s32)&SceneId_VenusLighthouseFirstFloor) {
    // Restore depth pillars, the Carry puzzle and the paired tile-wave passage.
    switch (gGameState.entrance) {
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR:
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST:
      VenusLighthouse_AnimatePairedTileWaves();
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR_HOLE:
      FieldFx_SetQueuedSoundAndPlay(SE_170);
      if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION))
        VenusLighthouse_LandWithRadialDust();
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_DRIFTING_FLOOR:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_LADDERS:
      actor = Object_GetById(ACTOR_FIRST_CARRY_PILLAR);
      Event_MoveCarryPillar();
      if (actor->x.fixed >> 20 == 8)
        VenusLighthouse_FixPillarAtDestination(actor);
      actor = Object_GetById(ACTOR_FIRST_CARRY_PILLAR + 1);
      if (actor->x.fixed >> 20 == 7)
        VenusLighthouse_FixPillarAtDestination(actor);
      VenusLighthouse_CreateBlendedPassageEffect(PIXELS(206), 0, PIXELS(449),
                                                 SPRITE_CRATE);
      VenusLighthouse_CreateBlendedPassageEffect(PIXELS(210), 0, PIXELS(449),
                                                 SPRITE_CRATE);
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_FROM_LADDERS:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST:
      FieldFx_StartFadeOverlay(0);
      Engine_TaskWait(2);
      actor = Object_GetById(ACTOR_FIRST_DEPTH_PILLAR);
      actor->motionFlags = 0;
      actor->update = SceneActor_SetFlagBitByRelativeDepth;
      actor = Object_GetById(ACTOR_FIRST_DEPTH_PILLAR + 1);
      actor->motionFlags = 0;
      actor->update = SceneActor_SetFlagBitByRelativeDepth;
      actor = Object_GetById(ACTOR_FIRST_DEPTH_PILLAR + 2);
      actor->motionFlags = 0;
      actor->update = SceneActor_SetFlagBitByRelativeDepth;
      Event_MoveDepthPillar();
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_GAP:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_THUNDER_CROWN:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PILLARS:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PSYNERGY_STONE:
      FieldFx_SetQueuedSoundAndPlay(SE_170);
      FieldFx_StartFadeOverlay(0);
      Engine_TaskWait(2);
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
        Map_CopyCellsTo(111, 5, 117, 5, 5, 2);
        Map_CopyCellsTo(111, 10, 117, 10, 5, 2);
        Map_CopyCellsTo(111, 7, 111, 5, 5, 2);
        Map_CopyCellsTo(111, 7, 111, 10, 5, 2);
        Map_CopyCells(48, 3, 3, 10, 54, 3);
        Map_CopyCells(55, 26, 3, 10, 48, 3);
      }
      break;
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_LOWER_ROOM:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_GAP:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_THUNDER_CROWN:
    case VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR:
      FieldFx_SetQueuedSoundAndPlay(SE_170);
      break;
    }
  } else if (scene == (s32)&SceneId_VenusLighthouseSecondFloor) {
    // Restore the first sliding wall and its sunken-pillar gate.
    switch (gGameState.entrance) {
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_FROM_PILLAR_PIT:
      VenusLighthouse_ArriveThroughSparkPassage();
      FieldFx_SetQueuedSoundAndPlay(SE_170);
      break;
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST:
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR:
      FieldFx_StartFadeOverlay(0);
      break;
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_WALL_SWITCH:
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_PILLAR_PIT:
      actor = Object_GetById(ACTOR_GATE_EFFECT_STATUE);
      actor->motionFlags = 0;
      actor->y.fixed = 0;
      actor = Object_GetById(ACTOR_GATE_PILLAR);
      actor->motionFlags = 0;
      actor->collisionFlags = 0;
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN)) {
        Engine_TaskWait(1);
        Map_CopyCellsTo(124, 41, 110, 41, 1, 2);
        Map_CopyCellAttributes(46, 41, 1, 1, 46, 42);
        Actor_SetPosition(ACTOR_GATE_PILLAR, PIXELS(744), PIXELS(728));
        actor->motionFlags = 0;
        actor->y.fixed = -PIXELS(16);
        Engine_ActorSetSpritePriority(ACTOR_GATE_PILLAR, 3);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Map_CopyCellAttributes(45, 45, 1, 1, 46, 45);
        Engine_ActorSetAnimation(ACTOR_GATE_EFFECT, PARTICLE_ANIM_FLICKER);
        Engine_ActorSetSpritePriority(ACTOR_GATE_EFFECT, 1);
        actor = Object_GetById(ACTOR_GATE_EFFECT);
        actor->collisionFlags = 0;
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(ACTOR_GATE_EFFECT, PIXELS(743), PIXELS(696));
#ifdef UBFIX
        actor->update = VenusLighthouse_TrailLight;
#else
        actor->update =
            (void (*)(union FieldObject *))VenusLighthouse_TrailLight;
#endif
      }
      Event_MoveGatePillar();
      break;
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH:
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Object_GetById(ACTOR_PARTY_LEADER)->y.fixed = -PIXELS(2);
      // These entrances share the tile-wave setup.
      // fall through
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST:
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_FROM_GATE:
      FieldFx_SetQueuedSoundAndPlay(SE_170);
      DisplayBlend_SuspendScript();
      {
        s32 zero = 0;

        /* FAKEMATCH(barrier): the empty asm gives the zero the refs that put
         * it in r3 and the address in r2; as Io_Write16(0, &REG_BLDCNT), 3 of
         * 1431 instructions differ in tbs-en. */
        asm("" : "+l"(zero));
        Io_Write16(zero, &REG_BLDCNT);
      }
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
        Map_CopyCellsTo(15, 96, 9, 96, 3, 3);
        Map_CopyCellsTo(12, 96, 15, 96, 3, 3);
        Map_CopyCellsTo(5, 50, 15, 32, 3, 4);
        Map_CopyCellsTo(25, 45, 9, 32, 3, 4);
        Map_CopyCellAttributes(15, 32, 3, 1, 9, 32);
        Map_CopyCellAttributes(12, 32, 3, 1, 15, 32);
      }
      if (gGameState.entrance !=
          VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH)
        break;
      Engine_EventOpenScreen();
      Engine_EventWaitForScreen();
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
      break;
    case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH:
      FieldFx_SetQueuedSoundAndPlay(SE_170);
      break;
    }
  } else if (scene == (s32)&SceneId_VenusLighthouseThirdFloor) {
    // Restore the two tile waves and the three-pillar socket puzzle.
    switch (gGameState.entrance) {
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST:
      VenusLighthouse_AnimateWestTileWave();
      break;
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SPARKLING_PASSAGES:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SOUTHWEST_ROOM:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_PILLAR_SOCKETS:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST:
      FieldFx_SetQueuedSoundAndPlay(SE_170);
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
        Map_CopyCells(53, 12, 3, 13, 26, 12);
        Map_CopyCellsTo(81, 41, 89, 14, 9, 2);
        Engine_TaskWait(1);
        FieldScheduler_AddCallback(VenusLighthouse_EmitWestPassageParticles,
                                   TASK_PRIORITY_SCENE);
      }
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
        Map_CopyCells(58, 12, 3, 13, 34, 12);
        Map_CopyCellsTo(81, 41, 97, 14, 5, 2);
        Engine_TaskWait(1);
        FieldScheduler_AddCallback(VenusLighthouse_EmitEastPassageParticles,
                                   TASK_PRIORITY_SCENE);
      }
      if (gGameState.entrance ==
          VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SOUTHWEST_ROOM)
        VenusLighthouse_ArriveThroughSparkPassage();
      else if (
          gGameState.entrance ==
          VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST)
        VenusLighthouse_AnimateEastTileWave();
      break;
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_NORTHWEST_ROOM:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRAGON_SCALES:
      /* FAKEMATCH(wrapper): called through a cast to an s32 function, so it
       * does not cross-jump with the second floor's same call; as a call by
       * its void prototype, 2 of 1431 instructions differ in tbs-en. */
      VENUS_LIGHTHOUSE_RUN_OPENING_SEQUENCE();
      FieldFx_SetQueuedSoundAndPlay(SE_170);
      break;
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_DRIFTING_FLOOR:
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_WEST_ROOM:
      actor = Object_GetById(ACTOR_SWITCH_EFFECT_STATUE);
      actor->motionFlags = 0;
      actor->y.fixed = 0;
      Object_GetById(ACTOR_FIRST_SWITCH_PILLAR)->motionFlags = 0;
      Object_GetById(ACTOR_FIRST_SWITCH_PILLAR + 1)->motionFlags = 0;
      Object_GetById(ACTOR_FIRST_SWITCH_PILLAR + 2)->motionFlags = 0;
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN)) {
        Engine_TaskWait(1);
        Map_CopyCellsTo(111, 59, 109, 37, 1, 2);
        Map_CopyCellAttributes(45, 37, 1, 1, 45, 38);
        if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_SOCKETS_A)) {
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR, PIXELS(776),
                            PIXELS(664));
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR + 1, PIXELS(840),
                            PIXELS(664));
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR + 2, PIXELS(776),
                            PIXELS(696));
        } else {
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR, PIXELS(840),
                            PIXELS(664));
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR + 1, PIXELS(776),
                            PIXELS(696));
          Actor_SetPosition(ACTOR_FIRST_SWITCH_PILLAR + 2, PIXELS(840),
                            PIXELS(696));
        }
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SWITCH_PILLAR, 3);
        actor = Object_GetById(ACTOR_FIRST_SWITCH_PILLAR);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SWITCH_PILLAR + 1, 3);
        actor = Object_GetById(ACTOR_FIRST_SWITCH_PILLAR + 1);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SWITCH_PILLAR + 2, 3);
        actor = Object_GetById(ACTOR_FIRST_SWITCH_PILLAR + 2);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetAnimation(ACTOR_SWITCH_GATE_EFFECT,
                                 PARTICLE_ANIM_FLICKER);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SWITCH_GATE_EFFECT), 0);
        Engine_ActorSetSpritePriority(ACTOR_SWITCH_GATE_EFFECT, 1);
        actor = Object_GetById(ACTOR_SWITCH_GATE_EFFECT);
        actor->collisionFlags = 0;
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(ACTOR_SWITCH_GATE_EFFECT, PIXELS(727), PIXELS(632));
#ifdef UBFIX
        actor->update = VenusLighthouse_TrailLight;
#else
        actor->update =
            (void (*)(union FieldObject *))VenusLighthouse_TrailLight;
#endif
      }
      Event_MoveSwitchPillar();
      break;
    case VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRIFTING_FLOOR:
      FieldFx_SetQueuedSoundAndPlay(SE_170);
      break;
    }
  } else if (scene == (s32)&SceneId_VenusLighthouseFourthFloor) {
    // Restore floating supports, opposing walls and the four-pillar pillar
    // puzzle.
    switch (gGameState.entrance) {
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_FROM_WALL_SWITCHES:
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        VenusLighthouse_LowerFloatingPillars(0);
        OverlayObject_WaitUntilIdle(Object_GetById(ACTOR_PARTY_LEADER));
        for (pillarIndex = 0; pillarIndex <= 3; pillarIndex++) {
          actor = Object_GetById(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR);
          column = actor->x.fixed >> 20;
          if (column == 13) {
            row = actor->z.fixed >> 20;
            if (row == 7 &&
                Engine_GameFlagIsSet(pillarIndex + FLAG_SCENE_TEMP_00)) {
              actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
              actor->collisionFlags = 0;
              actor->motionFlags = 0;
              Map_CopyCellAttributes(4, 19, 1, 1, column, row);
              break;
            }
          }
        }
      } else {
        actor = Object_GetById(ACTOR_BALANCE_WEST);
        actor->motionFlags = 0;
        actor->y.fixed = -PIXELS(48);
        actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        actor->collisionFlags &= 0xfe;
        actor->timer = 3;
        Engine_ActorSetSpritePriority(ACTOR_BALANCE_WEST, 1);
        actor = Object_GetById(ACTOR_BALANCE_EAST);
        actor->motionFlags = 0;
        actor->y.fixed = -PIXELS(48);
        actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        actor->collisionFlags &= 0xfe;
        actor->timer = 3;
        Engine_ActorSetSpritePriority(ACTOR_BALANCE_EAST, 1);
        actor = Object_GetById(ACTOR_FIRST_FLOATING_PILLAR);
        actor->motionFlags = 0;
        actor->timer = 0;
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_FLOATING_PILLAR),
                                   0);
        actor = Object_GetById(ACTOR_FIRST_FLOATING_PILLAR + 1);
        actor->motionFlags = 0;
        actor->timer = 0;
        Engine_ActorSetSpriteFlags(
            Object_GetById(ACTOR_FIRST_FLOATING_PILLAR + 1), 0);
        actor = Object_GetById(ACTOR_FIRST_FLOATING_PILLAR + 2);
        actor->motionFlags = 0;
        actor->timer = 0;
        Engine_ActorSetSpriteFlags(
            Object_GetById(ACTOR_FIRST_FLOATING_PILLAR + 2), 0);
        actor = Object_GetById(ACTOR_FIRST_FLOATING_PILLAR + 3);
        actor->motionFlags = 0;
        actor->timer = 0;
        Engine_ActorSetSpriteFlags(
            Object_GetById(ACTOR_FIRST_FLOATING_PILLAR + 3), 0);
      }
      break;
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_WEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_EAST:
      FieldFx_SetQueuedSoundAndPlay(SE_170);
      break;
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST:
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Object_GetById(ACTOR_PARTY_LEADER)->y.fixed = -PIXELS(2);
      // Restore the same spray and passage state.
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
      Object_GetById(ACTOR_RISING_PLATFORM)->motionFlags = ACTOR_FLOATS;
      Object_GetById(ACTOR_RISING_PLATFORM)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      Object_GetById(ACTOR_RISING_PLATFORM)->y.fixed = -(PIXELS(33) / 2);
      DisplayBlend_SuspendScript();
      Io_Write16(0, &REG_BLDCNT);
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
        FieldFx_SetQueuedSoundAndPlay(SE_170);
        Map_CopyCellsTo(36, 81, 32, 81, 3, 2);
        Map_CopyCellsTo(36, 83, 36, 81, 3, 2);
        Map_CopyCellAttributes(36, 17, 3, 1, 32, 17);
        Map_CopyCellAttributes(36, 18, 3, 1, 36, 17);
        Map_CopyCellsTo(63, 29, 33, 20, 1, 1);
        Map_CopyCellsTo(20, 56, 36, 17, 3, 4);
      }
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
        Map_CopyCellsTo(44, 81, 48, 81, 3, 2);
        Map_CopyCellsTo(44, 83, 44, 81, 3, 2);
        Map_CopyCellAttributes(44, 17, 3, 1, 48, 17);
        Map_CopyCellAttributes(44, 18, 3, 1, 44, 17);
        Map_CopyCellsTo(63, 29, 49, 20, 1, 1);
        Map_CopyCellsTo(41, 56, 44, 17, 3, 4);
      }
      if (gGameState.entrance ==
              VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST ||
          gGameState.entrance ==
              VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST) {
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
      }
      if (gGameState.entrance ==
          VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING)
        VenusLighthouse_RaiseLeaderWithSpray();
      break;
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_SOUTH:
    case VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_EAST:
      Engine_TaskWait(1);
      FieldScheduler_AddCallback(VenusLighthouse_VisitPillarActors,
                                 TASK_PRIORITY_SCENE);
      actor = Object_GetById(ACTOR_SOCKET_EFFECT_STATUE);
      actor->motionFlags = 0;
      actor->y.fixed = 0;
      Object_GetById(ACTOR_FIRST_SOCKET_PILLAR)->motionFlags = 0;
      Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 1)->motionFlags = 0;
      Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 2)->motionFlags = 0;
      Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 3)->motionFlags = 0;
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN)) {
        Engine_TaskWait(1);
        Map_CopyCellsTo(95, 56, 77, 35, 1, 2);
        Map_CopyCellAttributes(13, 35, 1, 1, 13, 36);
        Engine_ActorSetPosition(ACTOR_FIRST_SOCKET_PILLAR, PIXELS(264),
                                PIXELS(744));
        actor = Object_GetById(ACTOR_FIRST_SOCKET_PILLAR);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR, 3);
        Actor_SetPosition(ACTOR_FIRST_SOCKET_PILLAR + 1, PIXELS(184),
                          PIXELS(632));
        actor = Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 1);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR + 1, 3);
        Engine_ActorSetPosition(ACTOR_FIRST_SOCKET_PILLAR + 2, PIXELS(232),
                                PIXELS(696));
        actor = Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 2);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR + 2, 3);
        Engine_ActorSetPosition(ACTOR_FIRST_SOCKET_PILLAR + 3, PIXELS(184),
                                PIXELS(664));
        actor = Object_GetById(ACTOR_FIRST_SOCKET_PILLAR + 3);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR + 3, 3);
        Engine_ActorSetAnimation(ACTOR_SOCKET_GATE_EFFECT,
                                 PARTICLE_ANIM_FLICKER);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SOCKET_GATE_EFFECT), 0);
        Engine_ActorSetSpritePriority(ACTOR_SOCKET_GATE_EFFECT, 1);
        actor = Object_GetById(ACTOR_SOCKET_GATE_EFFECT);
        actor->collisionFlags = 0;
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(ACTOR_SOCKET_GATE_EFFECT, PIXELS(215), PIXELS(600));
#ifdef UBFIX
        actor->update = VenusLighthouse_TrailLight;
#else
        actor->update =
            (void (*)(union FieldObject *))VenusLighthouse_TrailLight;
#endif
      }
      Event_MoveSocketPillar();
      break;
    }
  }
  return 0;
}

/// Standing on this floor (VenusLighthouseFourthFloor's TRIGGER_DRIFT_NORTH)
/// carries the
/// party north in a trail of dust; at a ledge they drop down. The floors of
/// TRIGGER_DRIFT_SOUTH, _WEST and _EAST do the same south, west and east, and
/// the southward floor also lies in VenusLighthouseFirstFloor and
/// VenusLighthouseThirdFloor.
static void Event_DriftNorth(void) {
  struct Vector3 args;

  args.x = 0;
  args.z = -1;
  VenusLighthouse_StepActorWithDust(&args);
}

/// As Event_DriftNorth, south.
static void Event_DriftSouth(void) {
  struct Vector3 args;

  args.x = 0;
  args.z = 1;
  VenusLighthouse_StepActorWithDust(&args);
}

/// As Event_DriftNorth, west.
static void Event_DriftWest(void) {
  struct Vector3 args;

  args.x = -1;
  args.z = 0;
  VenusLighthouse_StepActorWithDust(&args);
}

/// As Event_DriftNorth, east.
static void Event_DriftEast(void) {
  struct Vector3 args;

  args.x = 1;
  args.z = 0;
  VenusLighthouse_StepActorWithDust(&args);
}

/// Follows the selected actor through the passage, adjusting for walls and
/// drops.
/// @param direction The way the drifting floor carries the party: -1, 0 or 1
/// on x and on z; y is not read.
static void VenusLighthouse_StepActorWithDust(struct Vector3 *direction) {
  s32 actorId;
  struct FieldActor *viewCenter;
  struct FieldActor *actor;
  struct FieldPosition nextPosition;
  struct EffectOptions options;
  s32 collision;
  s32 framePhase;
  s32 particleX;

  {
    struct EventWork *event = gEventWork;

    actorId = gGameState.selectedActor;
    viewCenter = event->viewCenter;
  }
  actor = Object_GetById(actorId);
  nextPosition.x =
      actor->x.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
  nextPosition.y = actor->y.fixed;
  nextPosition.z =
      actor->z.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
  collision = FieldObject_CheckMovementCollision(actor, &nextPosition);
  framePhase = gFrameCount & 4;
  if (framePhase == 0) {
    options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    particleX = actor->x.fixed +
                (((u32)(Engine_RandomNext() * 12) >> 16) << 16) - 0x60000;
    FieldFx_Spawn(
        particleX, actor->y.fixed, actor->z.fixed,
        (((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd) *
            direction->x,
        framePhase,
        direction->z *
            (((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd),
        EFFECT_USE_SPIN, &options);
  }
  // A drop uses the fall animation; solid terrain instead constrains each step.
  if (collision < 0) {
    Engine_ActorSetAttachedEffect(actorId, ATTACHED_EFFECT_IN_FRONT |
                                               ATTACHED_EFFECT_ONCE);
    Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed,
                             actor->z.fixed + PIXELS(8));
    Object_SetMode(actor, ANIM_LEADER_LEAP);
    FieldObject_WaitForMove(actor);
    do {
      Engine_TaskWait(1);
    } while (actor->y.fixed != actor->terrainHeight);
    Object_SetMode(actor, ANIM_LEADER_SPRING);
    Engine_TaskWait(3);
    return;
  }
  nextPosition.x =
      actor->x.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x, 19);
  nextPosition.y = actor->y.fixed;
  nextPosition.z =
      actor->z.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z, 19);
  collision = FieldObject_CheckMovementCollision(actor, &nextPosition);
  if (collision > 0) {
    return;
  }
  nextPosition.x =
      actor->x.fixed + direction->x * 0x5b333 - direction->z * 0x5b333;
  nextPosition.y = actor->y.fixed;
  nextPosition.z =
      actor->z.fixed + direction->z * 0x5b333 - direction->x * 0x5b333;
  collision = FieldObject_CheckMovementCollision(actor, &nextPosition);
  if (collision > 0) {
    actor->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
    actor->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
    return;
  }
  nextPosition.x = (direction->x + direction->z) * 0x5b333 + actor->x.fixed;
  nextPosition.y = actor->y.fixed;
  nextPosition.z = (direction->x + direction->z) * 0x5b333 + actor->z.fixed;
  collision = FieldObject_CheckMovementCollision(actor, &nextPosition);
  if (collision > 0) {
    actor->x.fixed -= VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
    actor->z.fixed -= VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
    return;
  }
  // Advance the camera anchor together with an unobstructed actor.
  viewCenter->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
  actor->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
  viewCenter->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
  actor->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
}

/// Chooses the particle scales and spin used by the tile-wave animations.
static __inline__ void Rubble_SetScaleAndSpin(struct EffectOptions *options) {
  options->startScaleX =
      ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
  options->startScaleY =
      ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
  options->spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
}

/// Animates the two tile waves Event_StepOnWallSwitch sets off in
/// VenusLighthouseFirstFloor, then takes the party back to
/// VenusLighthouseSecondFloor by
/// VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH.
static void VenusLighthouse_AnimatePairedTileWaves(void) {
  struct EffectOptions options;
  u32 extraBursts;
  u32 row;
  u32 particleIndex;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventBegin();
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  FieldFx_SetQueuedSoundAndPlay(SE_170);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_162);
  extraBursts = 0;
  // Advance the first tile wave, holding its fourth row for three extra bursts.
  for (row = 0; row < 10; row++) {
    Rubble_SetScaleAndSpin(&options);
    for (;;) {
      for (particleIndex = 0; particleIndex < 4 && row < 8; particleIndex++) {
        FieldFx_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) +
                          0x3600000,
                      0, 0x300000 + row * 0x100000 + particleIndex * 0x40000, 0,
                      0, 0, EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
      }
      Engine_TaskWait(3);
      if (row != 3 || extraBursts >= 3) {
        break;
      }
      extraBursts++;
    }
    Map_CopyCellsTo(48, row + 3, 54, row + 3, 3, 1);
  }
  Map_CopyCellsTo(111, 5, 117, 5, 5, 2);
  Map_CopyCellsTo(111, 10, 117, 10, 5, 2);
  Map_CopyCellsTo(111, 7, 111, 5, 5, 2);
  Map_CopyCellsTo(111, 7, 111, 10, 5, 2);
  for (row = 0; row < 10; row++) {
    Rubble_SetScaleAndSpin(&options);
    for (particleIndex = 0; particleIndex < 4 && row < 8; particleIndex++) {
      FieldFx_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3000000,
                    0, 0x300000 + row * 0x100000 + particleIndex * 0x40000, 0,
                    0, 0, EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
    }
    Engine_TaskWait(3);
    Map_CopyCellsTo(55, row + 26, 48, row + 3, 3, 1);
  }
  Engine_AudioPlayCue(SE_289);
  Engine_EventWait(60);
  Engine_EventRequestExit(
      VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH);
  Engine_EventEnd();
}

/// Emits the scheduled particles at the supplied passage coordinates.
static void VenusLighthouse_SpawnRandomParticles(s32 x, s32 z) {
  struct EffectOptions options;

  options.startScaleX = 7 * Q16_ONE / 10;
  options.startScaleY = 7 * Q16_ONE / 10;
  options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
  if ((gFrameCount & 3) == 0) {
    if (((u32)(Engine_RandomNext() << 1) >> 16) != 0) {
      FieldFx_Spawn(
          (x + (((u32)(Engine_RandomNext() << 1) >> 16) << 4)) << 16, 0,
          z << 19, 0, 0,
          (s32)((((u32)(Engine_RandomNext() * 5) >> 16) << 16) + 0x70000) / 10,
          EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
    } else {
      FieldFx_Spawn((x + ((u32)(Engine_RandomNext() * 17) >> 16)) << 16, 0,
                    (x << 19) - 0x40000, 0, 0, 0,
                    EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
    }
  }
}

static void VenusLighthouse_EmitWestPassageParticles(void) {
  VenusLighthouse_SpawnRandomParticles(432, 32);
}

static void VenusLighthouse_EmitEastPassageParticles(void) {
  VenusLighthouse_SpawnRandomParticles(0x230, 44);
}

/// Animates the western tile wave Event_StepOnWestWallSwitch sets off and
/// starts its lingering particle task, then takes the party back to
/// VenusLighthouseFourthFloor by
/// VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST.
static void VenusLighthouse_AnimateWestTileWave(void) {
  struct EffectOptions options;
  u32 extraBursts;
  s32 zero;
  u32 row;
  s32 rowOffset;
  s32 particleZ;
  u32 particleIndex;
  struct EffectOptions *effectOptions;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventBegin();
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  FieldFx_SetQueuedSoundAndPlay(SE_170);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_162);
  Engine_CameraSetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  Engine_CameraMoveTo(PIXELS(440), -1, PIXELS(360), 1);
  extraBursts = 0;
  row = 0;
  effectOptions = &options;
  rowOffset = 0;
  do {
    Rubble_SetScaleAndSpin(&options);
  // Repeat particle bursts on the fourth row before advancing the copied tiles.
  again:
    particleZ = 0xc00000;
    for (particleIndex = 0, zero = 0, particleZ += rowOffset;
         particleIndex <= 3; particleIndex++) {
      FieldFx_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x1a00000,
                    0, particleZ, 0, zero, zero,
                    EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, effectOptions);
      particleZ += 0x40000;
    }
    Engine_TaskWait(3);
    if (row == 3) {
      if (extraBursts <= 2) {
        extraBursts++;
        goto again;
      }
      if (extraBursts == 3)
        FieldScheduler_AddCallback(VenusLighthouse_EmitWestPassageParticles,
                                   3200);
    }
    Map_CopyCellsTo(53, row + 12, 26, row + 12, 3, 1);
    rowOffset += 0x100000;
    row++;
  } while (row <= 12);
  Map_CopyCellsTo(81, 41, 89, 14, 9, 2);
  Engine_CameraWaitForMove();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_EventWait(60);
  Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED);
  Engine_EventRequestExit(
      VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST);
  Engine_EventEnd();
}

/// Animates the eastern tile wave Event_StepOnEastWallSwitch sets off and
/// starts its lingering particle task, then takes the party back to
/// VenusLighthouseFourthFloor by
/// VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST.
static void VenusLighthouse_AnimateEastTileWave(void) {
  struct EffectOptions options;
  u32 extraBursts;
  s32 zero;
  u32 row;
  s32 rowOffset;
  s32 particleZ;
  u32 particleIndex;
  struct EffectOptions *effectOptions;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventBegin();
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  FieldFx_SetQueuedSoundAndPlay(SE_170);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_162);
  Engine_CameraSetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  Engine_CameraMoveTo(PIXELS(568), -1, PIXELS(360), 1);
  extraBursts = 0;
  row = 0;
  effectOptions = &options;
  rowOffset = 0;
  do {
    Rubble_SetScaleAndSpin(&options);
  // Keep this route task scheduling after each completed row.
  again:
    particleZ = 0xc00000;
    for (particleIndex = 0, zero = 0, particleZ += rowOffset;
         particleIndex <= 3; particleIndex++) {
      FieldFx_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x2200000,
                    0, particleZ, 0, zero, zero,
                    EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, effectOptions);
      particleZ += 0x40000;
    }
    Engine_TaskWait(3);
    if (row == 3 && extraBursts <= 2) {
      extraBursts++;
      goto again;
    }
    FieldScheduler_AddCallback(VenusLighthouse_EmitEastPassageParticles, 3200);
    Map_CopyCellsTo(58, row + 12, 34, row + 12, 3, 1);
    rowOffset += 0x100000;
    row++;
  } while (row <= 12);
  Map_CopyCellsTo(86, 41, 97, 14, 5, 2);
  Engine_CameraWaitForMove();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_EventWait(60);
  Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED);
  Engine_EventRequestExit(
      VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST);
  Engine_EventEnd();
}

// Emit rising particles around the actor every four frames.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 14 bytes of the 196-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void VenusLighthouse_UpdateRisingSpray(union FieldObject *fieldObject) {
#else
static s32 VenusLighthouse_UpdateRisingSpray(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  struct EffectOptions *effectOptions = &options;
  s32 framePhase;
  s32 particleX, particleY, particleZ, riseSpeed;

  effectOptions->priority = 1;
  effectOptions->palette = 5;
  effectOptions->type = 286;
  effectOptions->script = gVenusSprayScript;
  framePhase = gFrameCount & 3;
  if (framePhase == 0) {
    if ((gFrameCount & 7) == 0) {
      Engine_AudioPlayCue(SE_246);
    }
    particleX =
        actor->x.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
    particleY =
        actor->y.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
    particleZ =
        actor->z.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
    riseSpeed = (((u32)(Engine_RandomNext() * 4) >> 16) << 15) + 0x8000;
    FieldFx_Spawn(particleX, particleY, particleZ, 0, riseSpeed, framePhase,
                  EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_TYPE |
                      EFFECT_USE_SCRIPT,
                  effectOptions);
  }
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Lands the leader on ACTOR_RISING_PLATFORM, then raises both in a rising
/// spray and leaves by VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_AERIE.
static void VenusLighthouse_RaiseLeaderWithSpray(void) {
  u32 riseFrame;
  struct FieldActor *leader;
  struct FieldActor *platform;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  platform = Object_GetById(ACTOR_RISING_PLATFORM);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  // Land the leader on the platform, then synchronize their small settling
  // offsets.
  leader->y.fixed = PIXELS(130);
  leader->gravity = PIXELS(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Event_PlayCue(SE_204);
  Engine_EventWait(30);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Engine_EventWait(24);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
  platform->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
  leader->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
  leader->terrainHeight += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
  Engine_TaskWait(2);
  platform->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
  leader->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
  leader->terrainHeight += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
  Engine_TaskWait(10);
  platform->y.fixed += PIXELS(2);
  leader->y.fixed += PIXELS(2);
  leader->terrainHeight += PIXELS(2);
  Engine_TaskWait(4);
  platform->y.fixed += PIXELS(2);
  leader->y.fixed += PIXELS(2);
  leader->terrainHeight += PIXELS(2);
  Engine_TaskWait(4);
  platform->y.fixed += PIXELS(1);
  leader->y.fixed += PIXELS(1);
  leader->terrainHeight += PIXELS(1);
  leader->motionFlags = 0;
  platform->motionFlags = 0;
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(40);
#ifdef UBFIX
  leader->update = VenusLighthouse_UpdateRisingSpray;
#else
  leader->update = (void *)VenusLighthouse_UpdateRisingSpray;
#endif
  Engine_EventWait(60);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetSpritePriority(ACTOR_RISING_PLATFORM, 1);
  Event_PlayCue(MUS_017);
  Event_PlayCue(SE_308);
  GameFlag_Set(FLAG_SKIP_SCENE_CHANGE_SOUND);
  // Raise the leader, terrain contact and platform by the same pixel each
  // frame.
  for (riseFrame = 0; riseFrame < 128; riseFrame++) {
    leader->y.fixed += PIXELS(1);
    leader->terrainHeight += PIXELS(1);
    platform->y.fixed += PIXELS(1);
    Engine_TaskWait(1);
  }
  Engine_EventRequestExit(
      VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_AERIE);
}

/// Called by a light's path script as it reaches a socket: it looks at the
/// pillar sunk there, 16 pixels below the light, and sends the light on along
/// the path that pillar's piece of track leads to, or puts it out
/// (gVenusLightGoesOut) when there is no spiral pillar, its piece does not join
/// the edge the light comes in by (gVenusTracksMovingWest and the rest), or the
/// socket has no path for it. Each socket's list of pieces is searched from a
/// cursor that the search moves on for good (gVenusThirdFloorSocketTracks), so
/// a second light through a socket whose cursor has moved would take the path
/// at the wrong index. No arrangement of the floors' pillars sends one: on the
/// third floor (pieces south-east, west-east and north-west) the only socket
/// both lights can reach is the south-east one, where whichever piece stands
/// there fails one of the two lights' edge check before any search; on the
/// fourth floor only the
/// middle socket has more than one piece, and only the light from the
/// south-east socket reaches it.
static s32 VenusLighthouse_RouteLight(struct ScriptInterpreter *interpreter) {
  struct FieldActor *light = (struct FieldActor *)interpreter;
  struct FieldPosition pillarPosition;
  struct FieldActor *pillar;
  struct AnimationObject *sprite;
  const u8 *allowedTracks;
  u8 track;
  u32 socket;
  s32 pathIndex;
  const u8 *trackCursor;

  pillarPosition.x = light->x.fixed;
  pillarPosition.y = light->y.fixed - PIXELS(16);
  pillarPosition.z = light->z.fixed;
  pillar = SceneData_FindSlotAtPosition(&pillarPosition, 0);
  // XXX: the paths end only on sockets, so a pillar is likely always there;
  // a light over an empty socket would read through a null pillar.
  sprite = pillar->sprite;
  if (sprite->entries[0]->animId == SPRITE_SPIRAL_PILLAR) {
    s32 velocityX = light->velocityX;
    s32 speedX = velocityX < 0 ? -velocityX : velocityX;
    s32 velocityZ = light->velocityZ;
    s32 speedZ = velocityZ < 0 ? -velocityZ : velocityZ;

    if (speedX > speedZ) {
      if (velocityX / PIXELS(1) < 0) {
        allowedTracks = gVenusTracksMovingWest;
      } else {
        allowedTracks = gVenusTracksMovingEast;
      }
    } else {
      if (velocityZ / PIXELS(1) < 0) {
        allowedTracks = gVenusTracksMovingNorth;
      } else {
        allowedTracks = gVenusTracksMovingSouth;
      }
    }
    for (; *allowedTracks != TRACK_END; allowedTracks++) {
      if (sprite->lastNo == *allowedTracks) {
        break;
      }
    }
    if (*allowedTracks == TRACK_END) {
      Engine_ObjectSetScript(light, gVenusLightGoesOut);
      return 0;
    }
    if (gGameState.scene == (s32)&SceneId_VenusLighthouseThirdFloor) {
      for (socket = 0;
           socket <= 3 &&
           !(light->x.fixed >> 20 == gVenusThirdFloorSockets[socket].x &&
             light->z.fixed >> 20 == gVenusThirdFloorSockets[socket].z);
           socket++) {
      }
      for (pathIndex = 0;; pathIndex++) {
        trackCursor = gVenusThirdFloorSocketTracks[socket];
        track = *trackCursor;
        if (track == TRACK_END) {
          Engine_ObjectSetScript(light, gVenusLightGoesOut);
          return 0;
        }
        if (track == pillar->sprite->lastNo) {
          break;
        }
        gVenusThirdFloorSocketTracks[socket] = trackCursor + 1;
      }
      Engine_ObjectSetScript(light,
                             gVenusThirdFloorSocketPaths[socket][pathIndex]);
    } else {
      for (socket = 0;
           socket <= 7 &&
           !(light->x.fixed >> 20 == gVenusFourthFloorSockets[socket].x &&
             light->z.fixed >> 20 == gVenusFourthFloorSockets[socket].z);
           socket++) {
      }
      for (pathIndex = 0;; pathIndex++) {
        trackCursor = gVenusFourthFloorSocketTracks[socket];
        track = *trackCursor;
        if (track == TRACK_END) {
          Engine_ObjectSetScript(light, gVenusLightGoesOut);
          return 0;
        }
        if (track == pillar->sprite->lastNo) {
          break;
        }
        gVenusFourthFloorSocketTracks[socket] = trackCursor + 1;
      }
      Engine_ObjectSetScript(light,
                             gVenusFourthFloorSocketPaths[socket][pathIndex]);
    }
  } else {
    Engine_ObjectSetScript(light, gVenusLightGoesOut);
  }
  return 0;
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

// Step for each of 16 directions: X in the upper halfword, Z in the lower.
// UB: westward rows contain a negative X step; pack its unsigned bits.
#ifdef UBFIX
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))
#else
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))
#endif

/// The cell ahead for each of sixteen headings, 16 pixels east-west in the
/// upper halfword and north-south in the lower (DIRECTION_STEP).
u32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

/// The third floor's four sockets, by cell: northwest, northeast, southwest
/// and southeast.
struct SwitchCell gVenusThirdFloorSockets[] = {
    {48, 41},
    {52, 41},
    {48, 43},
    {52, 43},
};

// clang-format off
/// The heights of a balance platform by the weight on it (its timer, from 0
/// to 6): the more floating pillars stand on its side, the lower it sinks.
u32 gVenusBalancePlatformHeights[] = {
    0xfff00000, 0xfff00000, 0xffe00000, 0xffd00000, 0xffc00000, 0xffb00000, 0xffb00000,
};

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
/// by TRACK_END (VenusLighthouse_RouteLight).
u8 gVenusTracksMovingWest[] = {
    TRACK_NORTH_EAST, TRACK_WEST_EAST, TRACK_SOUTH_EAST, TRACK_END,
};

u8 gVenusTracksMovingEast[] = {
    TRACK_NORTH_WEST, TRACK_WEST_EAST, TRACK_SOUTH_WEST, TRACK_END,
};

u8 gVenusTracksMovingNorth[] = {
    TRACK_NORTH_SOUTH, TRACK_SOUTH_EAST, TRACK_SOUTH_WEST, TRACK_END,
};

u8 gVenusTracksMovingSouth[] = {
    TRACK_NORTH_SOUTH, TRACK_NORTH_WEST, TRACK_NORTH_EAST, TRACK_END,
};

/// The third floor's sockets' lists of the pieces they route, in the order of
/// each socket's paths (gVenusThirdFloorSocketTracks points into it).
u8 gVenusThirdFloorTrackLists[] = {
    TRACK_WEST_EAST,  TRACK_SOUTH_EAST, TRACK_SOUTH_WEST, TRACK_END, // northwest
    TRACK_WEST_EAST,  TRACK_SOUTH_EAST, TRACK_SOUTH_WEST, TRACK_END, // northeast
    TRACK_NORTH_WEST, TRACK_NORTH_EAST, TRACK_WEST_EAST,  TRACK_END, // southwest
    TRACK_NORTH_WEST, TRACK_NORTH_EAST, TRACK_WEST_EAST,  TRACK_END, // southeast
};

/// The fourth floor's sockets' lists, as gVenusThirdFloorTrackLists; each list
/// ends where the next begins, and the northeast socket's is empty.
u8 gVenusFourthFloorTrackLists[] = {
    TRACK_SOUTH_EAST, TRACK_END, // northwest
    TRACK_END,                   // northeast
    TRACK_NORTH_EAST, TRACK_END, // west
    TRACK_SOUTH_WEST, TRACK_END, // east
    TRACK_NORTH_EAST, TRACK_END, // far west
    TRACK_SOUTH_EAST, TRACK_SOUTH_WEST, TRACK_END, // middle
    TRACK_NORTH_SOUTH, TRACK_END, // southwest
    TRACK_NORTH_SOUTH, TRACK_END, // southeast
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

/// The lady statues' object script: drawn grey (VENUS_COLOR_MARK), and never
/// pushed (SW_VENUS_IMMOVABLE).
s32 gVenusGreyStatueScript[] = {
    SC_SET(SW_TIMER, VENUS_COLOR_MARK(SPRITE_COLOR_GREY)),
    SC_SET(SW_VENUS_IMMOVABLE, 1),
    SC_CALL(VenusLighthouse_UpdateChildFromTimer),
    SC_END,
};

/// The first floor's capped pillars' object script: drawn grey.
s32 gVenusGreyPillarScript[] = {
    SC_SET(SW_TIMER, VENUS_COLOR_MARK(SPRITE_COLOR_GREY)),
    SC_CALL(VenusLighthouse_UpdateChildFromTimer),
    SC_END,
};

/// The leader's action script after he jumps down from the ledge
/// (Event_JumpDown): he swells and shrinks ten times, flashing
/// (VenusLighthouse_PulseChildEffect).
s32 gVenusLeaderDazedScript[] = {
    SC_ARG(12),
    SC_SET(SW_HOOK, (s32)VenusLighthouse_PulseChildEffect),
    SC_LABEL(1),
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 10),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 10),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 10),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 10),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_END,
};

/// The object script of the world map's picture of Venus Lighthouse, placed
/// in the ground floor's statue room at three fifths of its size, with no
/// collision: a small purple tower standing among the floor lamps the statue's
/// routes light (the room's render).
s32 gVenusMiniatureLighthouseScript[] = {
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 5),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 5),
    SC_SET(SW_COLLISION, 0),
    SC_CALL(VenusLighthouse_ClearScriptedSpriteFlags),
    SC_END,
};

/// A drop of the spray that carries the leader up to the aerie: it pulses
/// from full size to a quarter four times, then is freed.
s32 gVenusSprayScript[] = {
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 5 * Q16_ONE / 8),
    SC_SET(SW_SCALE_X, 5 * Q16_ONE / 8),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, Q16_ONE / 4),
    SC_SET(SW_SCALE_X, Q16_ONE / 4),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 5 * Q16_ONE / 8),
    SC_SET(SW_SCALE_X, 5 * Q16_ONE / 8),
    SC_WAIT(6),
    SC_LOOP(4, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

/// The object scripts that give a spiral pillar its piece of track.
s32 gVenusTrackNorthSouthScript[] = {
    SC_ARG(TRACK_NORTH_SOUTH),
    SC_END,
};

s32 gVenusTrackNorthWestScript[] = {
    SC_ARG(TRACK_NORTH_WEST),
    SC_END,
};

s32 gVenusTrackNorthEastScript[] = {
    SC_ARG(TRACK_NORTH_EAST),
    SC_END,
};

s32 gVenusTrackWestEastScript[] = {
    SC_ARG(TRACK_WEST_EAST),
    SC_END,
};

s32 gVenusTrackSouthEastScript[] = {
    SC_ARG(TRACK_SOUTH_EAST),
    SC_END,
};

s32 gVenusTrackSouthWestScript[] = {
    SC_ARG(TRACK_SOUTH_WEST),
    SC_END,
};

/// The second floor's light: from the lady statue along the track, over the
/// sunk gate pillar, to the gate (VenusLighthouse_TrailLight trails
/// it).
s32 gVenusSecondFloorLightToGate[] = {
    SC_CALL(VenusLighthouse_ClearScriptedSpriteFlags),
    SC_ARG(7),
    SC_SET(SW_SPEED, PIXELS(1)),
    SC_SET(SW_HOOK, (s32)VenusLighthouse_TrailLight),
    SC_MOVE(PIXELS(760), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(760), 0, PIXELS(728)),
    SC_MOVE(PIXELS(760), 0, PIXELS(744)),
    SC_SYNC,
    SC_POS(PIXELS(760), 0, PIXELS(744)),
    SC_MOVE(PIXELS(744), 0, PIXELS(744)),
    SC_SYNC,
    SC_POS(PIXELS(743), 0, PIXELS(744)),
    SC_MOVE(PIXELS(744), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(743), 0, PIXELS(696)),
    SC_WAIT(1),
    SC_END,
};

/// The third floor's lights, from the lady statue to a socket, from socket to
/// socket, and from a socket to the gate, where SW_VENUS_LIGHT_ARRIVED opens
/// it; each that reaches a socket asks VenusLighthouse_RouteLight for its next
/// path.
s32 gVenusThirdFloorLightToNortheast[] = {
    SC_MOVE(PIXELS(872), 0, PIXELS(680)),
    SC_SYNC,
    SC_POS(PIXELS(872), 0, PIXELS(680)),
    SC_MOVE(PIXELS(872), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(872), 0, PIXELS(664)),
    SC_MOVE(PIXELS(840), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(840)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusThirdFloorLightToSoutheast[] = {
    SC_MOVE(PIXELS(888), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(888), 0, PIXELS(696)),
    SC_MOVE(PIXELS(840), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(840)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

/// A light that cannot go on: it stops trailing particles and is freed ten
/// frames later.
s32 gVenusLightGoesOut[] = {
    SC_SET(SW_HOOK, 0),
    SC_WAIT(10),
    SC_FREE,
};

s32 gVenusThirdFloorLightNorthwestToGate[] = {
    SC_MOVE(PIXELS(728), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(728), 0, PIXELS(664)),
    SC_MOVE(PIXELS(728), 0, PIXELS(632)),
    SC_SYNC,
    SC_POS(PIXELS(728), 0, PIXELS(632)),
    SC_SET(SW_VENUS_LIGHT_ARRIVED, 1),
    SC_END,
};

s32 gVenusThirdFloorLightNorthwestToSouthwest[] = {
    SC_MOVE(PIXELS(776), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(776)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusThirdFloorLightNortheastToNorthwest[] = {
    SC_MOVE(PIXELS(776), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(776)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusThirdFloorLightNortheastToSoutheast[] = {
    SC_MOVE(PIXELS(840), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(840)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusThirdFloorLightSouthwestToNorthwest[] = {
    SC_MOVE(PIXELS(776), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(776)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusThirdFloorLightSouthwestToGate[] = {
    SC_MOVE(PIXELS(728), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(728), 0, PIXELS(696)),
    SC_MOVE(PIXELS(728), 0, PIXELS(632)),
    SC_SYNC,
    SC_POS(PIXELS(728), 0, PIXELS(632)),
    SC_SET(SW_VENUS_LIGHT_ARRIVED, 1),
    SC_END,
};

s32 gVenusThirdFloorLightSoutheastToNortheast[] = {
    SC_MOVE(PIXELS(840), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(840)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusThirdFloorLightSoutheastToSouthwest[] = {
    SC_MOVE(PIXELS(776), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(776)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

/// The fourth floor's lights, as the third floor's: the first from the
/// statue to the fork, where Event_MoveSocketPillar sends it on to the
/// southeast socket and a second light to the southwest one.
s32 gVenusFourthFloorLightFromStatue[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(776)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(776)),
    SC_MOVE(PIXELS(216), 0, PIXELS(760)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(760)),
    SC_END,
};

s32 gVenusFourthFloorLightToSoutheast[] = {
    SC_MOVE(PIXELS(264), 0, PIXELS(760)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(760)),
    SC_MOVE(PIXELS(264), 0, PIXELS(744)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(264)),
    SC_SET(SW_Z, PIXELS(744)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightSoutheastToMiddle[] = {
    SC_MOVE(PIXELS(264), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(728)),
    SC_MOVE(PIXELS(232), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(232), 0, PIXELS(728)),
    SC_MOVE(PIXELS(232), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(232)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightMiddleToWest[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(696)),
    SC_MOVE(PIXELS(216), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(664)),
    SC_MOVE(PIXELS(184), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(184)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightMiddleToEast[] = {
    SC_MOVE(PIXELS(264), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(696)),
    SC_MOVE(PIXELS(264), 0, PIXELS(680)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(264)),
    SC_SET(SW_Z, PIXELS(680)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightWestToNorthwest[] = {
    SC_MOVE(PIXELS(184), 0, PIXELS(632)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(184)),
    SC_SET(SW_Z, PIXELS(632)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightEastToNortheast[] = {
    SC_MOVE(PIXELS(248), 0, PIXELS(680)),
    SC_SYNC,
    SC_POS(PIXELS(248), 0, PIXELS(680)),
    SC_MOVE(PIXELS(248), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(248), 0, PIXELS(664)),
    SC_MOVE(PIXELS(264), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(664)),
    SC_MOVE(PIXELS(264), 0, PIXELS(648)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(648)),
    SC_MOVE(PIXELS(248), 0, PIXELS(648)),
    SC_SYNC,
    SC_POS(PIXELS(248), 0, PIXELS(648)),
    SC_MOVE(PIXELS(248), 0, PIXELS(632)),
    SC_SYNC,
    SC_POS(PIXELS(248), 0, PIXELS(632)),
    SC_MOVE(PIXELS(232), 0, PIXELS(632)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(232)),
    SC_SET(SW_Z, PIXELS(632)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightToSouthwest[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(744)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(744)),
    SC_MOVE(PIXELS(184), 0, PIXELS(744)),
    SC_SYNC,
    SC_POS(PIXELS(184), 0, PIXELS(744)),
    SC_MOVE(PIXELS(184), 0, PIXELS(760)),
    SC_SYNC,
    SC_POS(PIXELS(184), 0, PIXELS(760)),
    SC_MOVE(PIXELS(168), 0, PIXELS(760)),
    SC_SYNC,
    SC_POS(PIXELS(168), 0, PIXELS(760)),
    SC_MOVE(PIXELS(168), 0, PIXELS(744)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(168)),
    SC_SET(SW_Z, PIXELS(744)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightSouthwestToFarWest[] = {
    SC_MOVE(PIXELS(168), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(168), 0, PIXELS(728)),
    SC_MOVE(PIXELS(184), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(184), 0, PIXELS(728)),
    SC_MOVE(PIXELS(184), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(184), 0, PIXELS(696)),
    SC_MOVE(PIXELS(168), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(168)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightFarWestToWest[] = {
    SC_MOVE(PIXELS(168), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(168), 0, PIXELS(664)),
    SC_MOVE(PIXELS(184), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(184)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_RouteLight),
    SC_END,
};

s32 gVenusFourthFloorLightNorthwestToGate[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(632)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(632)),
    SC_MOVE(PIXELS(216), 0, PIXELS(600)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(600)),
    SC_SET(SW_VENUS_LIGHT_ARRIVED, 1),
    SC_END,
};

/// The gates' cell animations as each opens, a cell column at a time.
u16 gVenusSecondFloorGateCells[] = {
    MAP_CELL_STEP(124, 50, 1, 2, 6), MAP_CELL_STEP(124, 47, 1, 2, 6), MAP_CELL_STEP(124, 44, 1, 2, 6), MAP_CELL_STEP(124, 41, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 gVenusThirdFloorGateCells[] = {
    MAP_CELL_STEP(117, 59, 1, 2, 6), MAP_CELL_STEP(115, 59, 1, 2, 6), MAP_CELL_STEP(113, 59, 1, 2, 6), MAP_CELL_STEP(111, 59, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 gVenusFourthFloorGateCells[] = {
    MAP_CELL_STEP(96, 58, 1, 2, 6), MAP_CELL_STEP(95, 58, 1, 2, 6), MAP_CELL_STEP(96, 56, 1, 2, 6), MAP_CELL_STEP(95, 56, 1, 2, 6), MAP_CELL_STEPS_END,
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

struct ScenePlacement gVenusLighthousePlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthouseEntryPlacements[] = {
    { SPRITE_SOLDIER, FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD, ACTOR_STAND,
        PIXELS(128), 0, PIXELS(268), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(352), 0, PIXELS(300), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(428), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(368), 0, PIXELS(140), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(400), 0, PIXELS(172), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(412), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(364), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthouseGroundFloorPlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_IN_VIEW, ACTOR_STAND,
        PIXELS(128), 0, PIXELS(140), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_WITH_GLASSES, CONDITION_IN_VIEW, ACTOR_STAND,
        PIXELS(304), 0, PIXELS(204), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_IN_VIEW, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(592), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, ACTOR_STAND,
        PIXELS(872), 0, PIXELS(120), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(88), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        PIXELS(664), 0, PIXELS(792), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, CONDITION_IN_VIEW, (s32)gVenusMiniatureLighthouseScript,
        PIXELS(488), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        PIXELS(72), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        PIXELS(72), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        PIXELS(168), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        PIXELS(168), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        PIXELS(72), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        PIXELS(104), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        PIXELS(136), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        PIXELS(168), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        PIXELS(200), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthouseFirstFloorPlacements[] = {
    { SPRITE_CAPPED_PILLAR, CONDITION_IN_VIEW, (s32)gVenusGreyPillarScript,
        PIXELS(248), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CAPPED_PILLAR, CONDITION_IN_VIEW, (s32)gVenusGreyPillarScript,
        PIXELS(184), 0, PIXELS(824), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CAPPED_PILLAR, CONDITION_IN_VIEW, (s32)gVenusGreyPillarScript,
        PIXELS(200), 0, PIXELS(824), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(392), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(376), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthouseSecondFloorPlacements[] = {
    { SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        PIXELS(808), 0, PIXELS(728), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackNorthSouthScript,
        PIXELS(776), 0, PIXELS(712), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_PARTICLE, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthouseThirdFloorPlacements[] = {
    { SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        PIXELS(888), 0, PIXELS(680), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackSouthEastScript,
        PIXELS(792), 0, PIXELS(680), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackWestEastScript,
        PIXELS(808), 0, PIXELS(648), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackNorthWestScript,
        PIXELS(824), 0, PIXELS(680), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_PARTICLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthouseFourthFloorPlacements[] = {
    { SPRITE_BALANCE_PLATFORM, CONDITION_IN_VIEW, ACTOR_RESET_ACTION,
        PIXELS(104), 0, PIXELS(312), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BALANCE_PLATFORM, CONDITION_IN_VIEW, ACTOR_RESET_ACTION,
        PIXELS(168), 0, PIXELS(312), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(280), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(280), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(280), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(120), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (s32)gVenusGreyStatueScript,
        PIXELS(136), 0, PIXELS(776), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackNorthSouthScript,
        PIXELS(184), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackSouthEastScript,
        PIXELS(200), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackSouthWestScript,
        PIXELS(232), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (s32)gVenusTrackNorthEastScript,
        PIXELS(248), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_PARTICLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_FLOATING_PLATFORM, CONDITION_IN_VIEW, ACTOR_RESET_ACTION,
        PIXELS(664), 0, PIXELS(136), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gVenusLighthouseEntryEvents[] = {
    { EVENT_EXIT, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_EAST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_WEST },
    { EVENT_TALK, ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HearOfTheAttack) },
    { EVENT_TALK, ACTOR_SCHOLAR_LIGHTHOUSE_OPEN, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIodemBeHappyTheEntranceTo) },
    { EVENT_TALK, ACTOR_SCHOLAR_BARBARIANS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseOhhhHowCouldTheyAttackMere) },
    { EVENT_TALK, ACTOR_WAITING_SCHOLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWaitingScholar) },
    { EVENT_TALK, ACTOR_SOLDIER_OPENED_DOOR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseHowHowDidTheyOpenThe) },
    { EVENT_TALK, ACTOR_SOLDIER_HEADED_FOR_BABI, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheyAreAlreadyHeadedForBabi) },
    { EVENT_TALK, ACTOR_SOLDIER_COLOSSO_VICTORS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseBabisSoldiersAreAllColossoVictors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HearOfTheAttack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_LIGHTHOUSE_OPEN, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseUhnnnIfOnlyICouldMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_BARBARIANS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheirPowersWereSoStrangeWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WAITING_SCHOLAR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIMustGetInsideVenusLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_OPENED_DOOR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheYoungBoySeemedToGlow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_HEADED_FOR_BABI, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheyWerentInVenusLighthouseVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_COLOSSO_VICTORS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIThoughtIRecognizedOneOf) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseGroundFloorEvents[] = {
    { EVENT_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_ENTRY, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_ENTRY },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_TO_RELIEFS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_TO_RELIEFS },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_FRONT_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_FRONT_ROOM },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_EAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_WEST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_PILLARS },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_RELIEFS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_RELIEFS },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_CARRY_STONE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_CARRY_STONE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_FLOOR_SWITCH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_FLOOR_SWITCH },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_NORTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_NORTH },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_WEST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_NORTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_NORTH },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_WEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_SMALL_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_SMALL_ROOM },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_FLOOR_SWITCH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_FLOOR_SWITCH },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_SMALL_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_SMALL_ROOM },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_PILLARS },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_STATUE_ROW, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_STATUE_ROW },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_GREAT_STATUE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_GREAT_STATUE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_GREAT_STATUE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_GREAT_STATUE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_WEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_EAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_PILLARS },
    { EVENT_TOUCH, TRIGGER_LEDGE, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_JumpDown) },
    { EVENT_TOUCH, TRIGGER_FLOOR_SWITCH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnFloorSwitch) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, TRIGGER_WALL_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillarTowardWall) },
    { EVENT_TALK, ACTOR_SCHOLAR_ATTACKED_ANYWAY, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenToldThemToStopBut) },
    { EVENT_TALK, ACTOR_SCHOLAR_NOT_THE_ENTRANCE, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenWentInsideButThenCame) },
    { EVENT_TALK, ACTOR_DOOR_SCHOLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToScholarByDoor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_ATTACKED_ANYWAY, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIfKradenHadntBeenThereWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_NOT_THE_ENTRANCE, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenSaidSomethingAboutThisBeing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOOR_SCHOLAR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIWonderWhatMysteriesTheStatue) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_FIRST_RELIEF, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
#if LANGUAGE == LANGUAGE_JAPANESE
    { SEARCH(SEARCH_UNNAMED), TRIGGER_FIRST_RELIEF + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_FIRST_RELIEF + 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_FIRST_RELIEF + 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_FIRST_RELIEF + 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
#endif
#if !TBS_PATCH_1 || TBS_PATCH_2
    { SEARCH(SEARCH_UNNAMED), TRIGGER_FIRST_RELIEF + 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
#endif
    { SEARCH(SEARCH_UNNAMED), TRIGGER_GREAT_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchStatue) },
    { SEARCH(SEARCH_CHEST), TRIGGER_CHEST, FLAG_VENUS_LIGHTHOUSE_LUCKY_CAP, FIND_ITEM(ITEM_LUCKY_CAP) },
    { SEARCH(SEARCH_CHEST), TRIGGER_SECOND_CHEST, FLAG_VENUS_LIGHTHOUSE_CARRY_STONE, FIND_ITEM(ITEM_CARRY_STONE) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WALL_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MovePillarTowardWall) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FLOOR_SWITCH_STATUE, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_MovePillarOntoSwitch) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_WALL_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryPillarPastWall) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RevealPath) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HidePath) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseFirstFloorEvents[] = {
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_GAP, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_GAP },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_THUNDER_CROWN, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_THUNDER_CROWN },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_DRIFTING_FLOOR },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_LADDERS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_LADDERS },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_TO_LADDERS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_TO_LADDERS },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_DRIFTING_FLOOR },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_LADDERS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_LADDERS },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_GAP, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_GAP },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_THUNDER_CROWN, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_THUNDER_CROWN },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PILLARS },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PSYNERGY_STONE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PSYNERGY_STONE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_TO_LADDERS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_TO_LADDERS },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_TUNNEL_RUINS_LADY_STATUES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_TUNNEL_RUINS_LADY_STATUES },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_TO_DRIFTING_FLOOR },
    { EVENT_TOUCH, TRIGGER_DRIFT_SOUTH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftSouth) },
    { EVENT_TOUCH, TRIGGER_HOLE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FallThroughFloor) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushDepthPillar) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushDepthPillar) },
    { EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClearEffectTarget) },
    { EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetLeaderAsEffectTarget) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, TRIGGER_CARRY_PILLARS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushCarryPillar) },
    { SEARCH(SEARCH_CHEST), TRIGGER_CHEST, FLAG_VENUS_LIGHTHOUSE_THUNDER_CROWN, FIND_ITEM(ITEM_THUNDER_CROWN) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_PSYNERGY_STONE, FLAG_VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_DEPTH_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveDepthPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_DEPTH_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveDepthPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_DEPTH_PILLAR + 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveDepthPillar) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftCarryPillar) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftCarryPillar) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftCarryPillar) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftCarryPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveCarryPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveCarryPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryPillarAcrossGap) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryPillarAcrossGap) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseSecondFloorEvents[] = {
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH },
    { TOUCH_WALKING_INTO(FACING_NORTH), VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_TO_PILLAR_PIT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSparklingPassage) },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_SPARKLING_PASSAGE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_SPARKLING_PASSAGE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_GATE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_GATE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_TO_GATE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_TO_GATE },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_WALL_SWITCH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_WALL_SWITCH },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_PILLAR_PIT, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_PILLAR_PIT },
    { EVENT_TOUCH, TRIGGER_WALL_SWITCH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnWallSwitch) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, TRIGGER_GATE_PILLAR, FLAG_VENUS_LIGHTHOUSE_GATE_OPEN, EVENT_SCRIPT(Event_JumpOrPushGatePillar) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GATE_PILLAR, FLAG_VENUS_LIGHTHOUSE_GATE_OPEN, EVENT_SCRIPT(Event_MoveGatePillar) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseThirdFloorEvents[] = {
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_TO_SPARKLING_PASSAGES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_TO_SPARKLING_PASSAGES },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_TO_SPARKLING_PASSAGES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_TO_SPARKLING_PASSAGES },
    { TOUCH_WALKING_INTO(FACING_NORTH), VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_NORTHWEST_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSparklingPassage) },
    { TOUCH_WALKING_INTO(FACING_NORTH), VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRAGON_SCALES, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSparklingPassage) },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRIFTING_FLOOR },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_TO_DRIFTING_FLOOR },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SPARKLING_PASSAGES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SPARKLING_PASSAGES },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST },
    { TOUCH_WALKING_INTO(FACING_NORTH), VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SOUTHWEST_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSparklingPassage) },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_PILLAR_SOCKETS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_PILLAR_SOCKETS },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_DRIFTING_FLOOR },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_WEST_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_WEST_ROOM },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_TO_PILLAR_SOCKETS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_TO_PILLAR_SOCKETS },
    { EVENT_TOUCH, TRIGGER_DRIFT_SOUTH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftSouth) },
    { EVENT_TOUCH, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SOUTHWEST_ROOM, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftSouth) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, TRIGGER_SWITCH_PILLARS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushSwitchPillar) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor) },
    { SEARCH(SEARCH_CHEST), TRIGGER_CHEST, FLAG_VENUS_LIGHTHOUSE_DRAGON_SCALES, FIND_ITEM(ITEM_DRAGON_SCALES) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SWITCH_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSwitchPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SWITCH_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSwitchPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SWITCH_PILLAR + 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSwitchPillar) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseFourthFloorEvents[] = {
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_TO_WALL_SWITCHES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_TO_WALL_SWITCHES },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLARS },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PSYNERGY_STONE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PSYNERGY_STONE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_EAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_WEST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_EAST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_SOUTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_SOUTH },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_WEST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_EAST },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_SOUTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_SOUTH },
    { EVENT_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_EAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES },
    { TOUCH_WALKING_INTO(FACING_SOUTH), TRIGGER_RISING_PASSAGE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterRisingPassage) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, TRIGGER_FLOATING_PILLARS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPuzzlePillar) },
    { TOUCH_WALKING_INTO(FACING_WEST), TRIGGER_FLOATING_PILLARS_GAP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpAcrossGap) },
    { TOUCH_WALKING_INTO(FACING_EAST), TRIGGER_FLOATING_PILLARS_GAP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpAcrossGap) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, TRIGGER_FLOATING_PILLARS_GAP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPuzzlePillar) },
    { TOUCH_WALKING_INTO(FACING_EAST), TRIGGER_FLOATING_PILLARS_EAST_GAP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpAcrossGap) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, TRIGGER_FLOATING_PILLARS_EAST_GAP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPuzzlePillar) },
    { EVENT_TOUCH, TRIGGER_WEST_WALL_SWITCH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnWestWallSwitch) },
    { EVENT_TOUCH, TRIGGER_EAST_WALL_SWITCH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnEastWallSwitch) },
    { EVENT_TOUCH, TRIGGER_DRIFT_SOUTH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftSouth) },
    { EVENT_TOUCH, TRIGGER_DRIFT_NORTH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftNorth) },
    { EVENT_TOUCH, TRIGGER_DRIFT_WEST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftWest) },
    { EVENT_TOUCH, TRIGGER_DRIFT_EAST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftEast) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, TRIGGER_SOCKET_PILLARS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushSocketPillar) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor) },
    { SEARCH(SEARCH_CHEST), TRIGGER_CHEST, FLAG_VENUS_LIGHTHOUSE_GAIA_BLADE, FIND_ITEM(ITEM_GAIA_BLADE) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_PSYNERGY_STONE, FLAG_VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR + 1, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR + 2, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR + 3, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryFloatingPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryFloatingPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR + 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryFloatingPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR + 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryFloatingPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSocketPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSocketPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR + 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSocketPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR + 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSocketPillar) },
    { SCENE_EVENTS_END },
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
