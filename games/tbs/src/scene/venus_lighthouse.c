/* Venus Lighthouse, six scenes in one overlay: the approach outside, the floor
   the front door opens onto, and the four floors of the climb that ends in the
   Lighthouse Aerie at the top (venus_lighthouse_aerie.c).

   Each scene has its own entrance, actor placement and cue tables
   (VenusLighthouseGroundFloor's entrances are the fallback table). All share
   one exit table. scInit redraws each scene's puzzles as their flags left them,
   case by entrance. Inside, only the actors of the room entered are placed
   (CONDITION_IN_VIEW).

   - Venus Lighthouse Entry (SceneIdVenusLighthouseEntry), outside. The
	 scholars and Babi's soldiers the wanderers attacked lie hurt and say what
	 they saw. A soldier who tells of the attack flickers and vanishes
	 (FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD), and his spot no longer answers a
	 search. VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR, the open
	 door, leads in.
   - Venus Lighthouse (SceneIdVenusLighthouseGroundFloor, named by an area
	 rule of its own), the floor behind the front door, with three scholars.
   The great statue (METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_GREAT_STATUE) opens
   one  of two paths (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or _SECOND_PATH). The
   first time the tick counter's parity picks the path, and each later
	 search switches it. The statue lights the open path in gold lines between
	 the floor lamps before it, where a small model of the lighthouse stands
	 (VenusMiniatureLighthouseScript). The Tunnel Ruins under Babi Lighthouse
	 redraw their ways to match (tunnel_ruins.c), and they lead up into
	 VenusLighthouseFirstFloor. The reliefs
   (METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF) are read, and Reveal
   shows  a hidden path in two places. Move or a push brings a grey block up
   against a wall (FLAG_VENUS_LIGHTHOUSE_PILLAR_AT_WALL), and Carry, from the
   Carry Stone in one of the floor's chests, sets it down beyond
   (FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED). In the room of the lady statue,  a
   band of lightning crackles across the floor south of her and throws  back
   whoever walks into it, until Move slides her onto the floor switch  in
   front of her. Standing on the switch presses it only while he stays.
   - Venus Lighthouse (SceneIdVenusLighthouseFirstFloor), where the Tunnel
	 Ruins come up
	 (VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_TUNNEL_RUINS_LADY_STATUES). A wide
	 slide of flowing sand carries the party south down its big room, and a
	 hole drops the leader into the room below.
	 Three capped pillars are Moved or pushed. Two pillars by a gap are
	 carried across it, or dropped through the floor. A chest holds the Thunder
	 Crown, and a Psynergy Stone lies here.
   - Venus Lighthouse (SceneIdVenusLighthouseSecondFloor). A sparkling
	 passage takes the leader through in a trail of sparks. A floor switch
	 slides a wall aside in a cloud of dust the first time
	 (FLAG_VENUS_LIGHTHOUSE_WALL_MOVED). A pillar Moved or pushed into its
	 socket sinks, and a light runs from the lady statue along the gold track
	 over it to open the gate (FLAG_VENUS_LIGHTHOUSE_GATE_OPEN).
   - Venus Lighthouse (SceneIdVenusLighthouseThirdFloor). Three more
	 sparkling passages, slides of flowing sand, and three pillars carrying
	 pieces of track that sink into four sockets. With all three sunk two
	 lights run from the statue, and when the track leads one to the gate it
	 opens (FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN, with
	 FLAG_VENUS_LIGHTHOUSE_SOUTHEAST_SOCKET_EMPTY or
	 FLAG_VENUS_LIGHTHOUSE_NORTHWEST_SOCKET_EMPTY for where the pillars stand).
	 A chest holds Dragon Scales.
   - Venus Lighthouse (SceneIdVenusLighthouseFourthFloor). A maze of sand
	 flowing north, south, west and east carries the party each way. A west and
	 an east floor switch slide their walls aside
	 (FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED and _EAST_WALL_MOVED, which
	 VenusLighthouseThirdFloor also redraws). Carry sets down four floating
	 pillars on two balance platforms that sink and rise to their weight
	 (BalancePlatformHeights). Four pillars carrying track sink into
	 eight sockets, and the lights run on to the pillar room's gate
	 (FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN). A chest holds the Gaia Blade,
	 and a Psynergy Stone lies here. A passage raises the leader on a
	 platform in rising sparkles to the Lighthouse Aerie
	 (VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_AERIE).

   Locked doors stand on the second floor and above
   (METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_LOCKED_DOOR).

   The wall and gate flags (FLAG_VENUS_LIGHTHOUSE_WALL_MOVED to
   FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN) and the Psynergy Stones' flags are
   area flags. They last while the party stays in the scenes of the climb
   (SceneIdVenusLighthouseFirstFloor to SceneIdVenusLighthouseAerie, one
   area), so the floors stay solved between them.

   The FLAG_SCENE_TEMP_ states last only until the next change of scene other
   than a return from battle. Every arrival sets FLAG_IN_VENUS_LIGHTHOUSE. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/ground_mask.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/actor_step.h"
#include "scene/field_effect_spawn.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

extern SWITCH_METATILE VenusThirdFloorSockets[];
extern Uint32		   BalancePlatformHeights[];
extern SWITCH_METATILE VenusFourthFloorSockets[];

/* A socket index past the third floor's four (VenusThirdFloorSockets) and the
   fourth floor's eight (VenusFourthFloorSockets): the pillar is in none. */
#define NO_THIRD_FLOOR_SOCKET  5
#define NO_FOURTH_FLOOR_SOCKET 8
extern Uint8		  VenusTracksMovingWest[];
extern Uint8		  VenusTracksMovingEast[];
extern Uint8		  VenusTracksMovingNorth[];
extern Uint8		  VenusTracksMovingSouth[];
extern Sint32		  VenusLeaderShockedScript[];
extern Sint32		  GlintTwinkleScript[];
extern Sint32		  VenusSecondFloorLightToGate[];
extern Sint32		  VenusThirdFloorLightToNortheast[];
extern Sint32		  VenusThirdFloorLightToSoutheast[];
extern Sint32		  VenusLightGoesOut[];
extern Sint32		  VenusFourthFloorLightFromStatue[];
extern Sint32		  VenusFourthFloorLightToSoutheast[];
extern Sint32		  VenusFourthFloorLightToSouthwest[];
extern Uint16		  VenusSecondFloorGateMetatiles[];
extern Uint16		  VenusThirdFloorGateMetatiles[];
extern Uint16		  VenusFourthFloorGateMetatiles[];
extern SCENE_ENTRANCE VenusLighthouseEntryEntrances[];
extern SCENE_ENTRANCE VenusLighthouseEntrancesOther[];
extern SCENE_ENTRANCE VenusLighthouseFirstFloorEntrances[];
extern SCENE_ENTRANCE VenusLighthouseSecondFloorEntrances[];
extern SCENE_ENTRANCE VenusLighthouseThirdFloorEntrances[];
extern SCENE_ENTRANCE VenusLighthouseFourthFloorEntrances[];
extern Uint32		  VenusLighthouseExits[];
extern ACTOR_PLACE	  VenusLighthouseActorPlacementsOther[];
extern ACTOR_PLACE	  VenusLighthouseEntryActorPlacements[];
extern ACTOR_PLACE	  VenusLighthouseGroundFloorActorPlacements[];
extern ACTOR_PLACE	  VenusLighthouseFirstFloorActorPlacements[];
extern ACTOR_PLACE	  VenusLighthouseSecondFloorActorPlacements[];
extern ACTOR_PLACE	  VenusLighthouseThirdFloorActorPlacements[];
extern ACTOR_PLACE	  VenusLighthouseFourthFloorActorPlacements[];
extern CUE			  VenusLighthouseEntryCues[];
extern CUE			  VenusLighthouseGroundFloorCues[];
extern CUE			  VenusLighthouseFirstFloorCues[];
extern CUE			  VenusLighthouseSecondFloorCues[];
extern CUE			  VenusLighthouseThirdFloorCues[];
extern CUE			  VenusLighthouseFourthFloorCues[];
extern const Uint8	 *VenusThirdFloorSocketTracks[];
extern Sint32		**VenusThirdFloorSocketPaths[];
extern const Uint8	 *VenusFourthFloorSocketTracks[];
extern Sint32		**VenusFourthFloorSocketPaths[];

/* Area flags, cleared when the party enters another area, whose numbers other
   areas use for their own. */

/* The second floor's pink floor switch has slid the wall beside it aside. */
#define FLAG_VENUS_LIGHTHOUSE_WALL_MOVED				  FLAG_AREA_TEMP_00
#define FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED			  FLAG_AREA_TEMP_06
#define FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED			  FLAG_AREA_TEMP_07
/* The gates the sunken pillars opened: one pillar, three pillars (and which of
   two sets of sockets they sank into) and the pillar room's four. The light
   from the second floor's lady statue has opened the gate at the end of its
   track. */
#define FLAG_VENUS_LIGHTHOUSE_GATE_OPEN					  FLAG_AREA_TEMP_01
/* The pillars stand in every socket of VenusThirdFloorSockets but the southeast
   one. */
#define FLAG_VENUS_LIGHTHOUSE_SOUTHEAST_SOCKET_EMPTY	  FLAG_AREA_TEMP_02
/* The pillars stand in every socket of VenusThirdFloorSockets but the northwest
   one. */
#define FLAG_VENUS_LIGHTHOUSE_NORTHWEST_SOCKET_EMPTY	  FLAG_AREA_TEMP_03
#define FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN	  FLAG_AREA_TEMP_04
#define FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN			  FLAG_AREA_TEMP_08
/* The first floor's Psynergy Stone, taken until the party leaves the climb. */
#define FLAG_VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE  FLAG_AREA_TEMP_50
/* The fourth floor's Psynergy Stone, as the first floor's. */
#define FLAG_VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE FLAG_AREA_TEMP_51

/* The metatile behaviors the floors' scripts answer, by floor. A number means
   one thing on one floor and another on the next.

   Each exit metatile carries the number of the exit it leads by, and the script
   rows name it by that exit. They name the sparkling passages the same way,
   which leave by their own number (scVenusLighthouseEnterSparklingPassage). */
typedef enum VENUS_LIGHTHOUSE_METATILE_BEHAVIOR {
	/* a locked door, from the second floor up */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_LOCKED_DOOR = 25,
	/* sand flowing south, which carries the party with it */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_SOUTH = 30,
	/* A floor's chest. */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_CHEST = 100,
	/* A floor's Psynergy Stone. */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_PSYNERGY_STONE = 110,
	/* VenusLighthouseGroundFloor */
	/* The first of the six reliefs, whose words scVenusLighthouseReadRelief
	   shows in their order. */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF = 35,
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_GREAT_STATUE = 41,
	/* the band of lightning south of the lady statue */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_LIGHTNING = 45,
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FLOOR_SWITCH = 46,
	/* walking into the grey block (ACTOR_WALL_PILLAR) */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_WALL_PILLAR = 50,
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_SECOND_CHEST = 101,
	/* VenusLighthouseFirstFloor */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_HOLE = 35,
	/* The floor bounding the rows where the capped pillars stand, where
	   stepping back out ends scVenusLighthouseEnterCappedPillarRows. */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_BESIDE_CAPPED_PILLARS = 31,
	/* The rows where the capped pillars stand. Walking into a pillar from
	   either kind of metatile pushes it. */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AMONG_CAPPED_PILLARS = 32,
	/* the gap and the two pillars beside it */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_CARRY_PILLARS = 45,
	/* VenusLighthouseSecondFloor */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_WALL_SWITCH = 35,
	/* the gap and ACTOR_GATE_PILLAR */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_GATE_PILLAR = 36,
	/* VenusLighthouseThirdFloor the gaps and the three switch pillars */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_SWITCH_PILLARS = 35,
	/* VenusLighthouseFourthFloor */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_RISING_PASSAGE = 18,
	/* The sand maze's flowing sand, by the way it carries the party. */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_NORTH = 31,
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_WEST = 32,
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_EAST = 33,
	/* pushing a floating pillar */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS = 35,
	/* a gap jumped west or east */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_GAP = 36,
	/* a gap jumped east */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_EAST_GAP = 37,
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_WEST_WALL_SWITCH = 40,
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_EAST_WALL_SWITCH = 41,
	/* the gaps and the four socket pillars */
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_SOCKET_PILLARS = 45
} VENUS_LIGHTHOUSE_METATILE_BEHAVIOR;

/* UB: passage offsets and dust directions can be negative. The callers use
   bounded pixel offsets or directions -1..1, so these products fit Sint32. */
#if FIX_UNDEFINED_SHIFT
#define VENUS_LIGHTHOUSE_FIXED_OFFSET(value, bits) ((value) * (1 << (bits)))
#else
#define VENUS_LIGHTHOUSE_FIXED_OFFSET(value, bits) ((value) << (bits))
#endif

/* UB: this calls a void function through an Sint32 function type; UBFIX calls
   it as declared. */
#if FIX_FUNCTION_SIGNATURES
#define VENUS_LIGHTHOUSE_ARRIVE_THROUGH_SPARK_PASSAGE \
	scVenusLighthouseArriveThroughSparkPassage
#else
#define VENUS_LIGHTHOUSE_ARRIVE_THROUGH_SPARK_PASSAGE \
	((Sint32 (*)(void))scVenusLighthouseArriveThroughSparkPassage)
#endif

/* The metatiles of row 34 that take row 35's behaviours as Isaac drops through
   the first floor's hole: their first column and their width, wider in the
   French, Spanish and Italian editions. */
#define VENUS_DESCENT_COLLISION_X	  WHEN(VENUS_HOLE_DROP_OPENS_WIDER, 33, 34)
#define VENUS_DESCENT_COLLISION_WIDTH WHEN(VENUS_HOLE_DROP_OPENS_WIDER, 7, 5)

static void scVenusLighthouseStepActorWithDust(VECTOR3 *direction);
static void scVenusLighthouseMoveFirstFloorSandFall(void);
static void scVenusLighthousePuffWestSlideHead(void);
static void scVenusLighthousePuffEastSlideHead(void);
static void scVenusLighthousePourWestSandSlide(void);
static void scVenusLighthousePourEastSandSlide(void);
static void scVenusLighthouseRaiseLeaderInSparkles(void);

/* The scene flags: on the ground floor, FLAG_SCENE_TEMP_00 is the lady statue
   holding the floor switch down and
   FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH the leader standing on it; on
   the fourth floor, FLAG_SCENE_TEMP_00 plus a floating pillar's number (0 to 3)
   is that pillar set down. */

/* Each scene numbers its actors by the rows of its own actor placement table,
   so each has an enum of its own; a row drawn as one of the party's characters
   (Garet in VenusLighthouseGroundFloor) is that party member in his own slot
   and takes no number. */

/* Venus Lighthouse Entry's actors, numbering the rows of
   VenusLighthouseEntryActorPlacements. They are the scholars
   (SPRITE_OLD_MAN_IN_BLUE_CAP) and Babi's soldiers (SPRITE_SOLDIER) the
   wanderers attacked, each named by what they say. */
typedef enum VENUS_LIGHTHOUSE_ENTRY_ACTOR_ID {
	/* A soldier by the entrance: "Venus Lighthouse was... attacked by
	   strange... wanderers...", and he flickers and is gone
	   (scVenusLighthouseHearOfTheAttack). */
	ACTOR_SOLDIER_TELLING_OF_THE_ATTACK = ACTOR_FIRST_PLACED,
	/* "Iodem, be happy. The entrance to Venus Lighthouse is open..." */
	ACTOR_SCHOLAR_LIGHTHOUSE_OPEN,
	/* "Ohhh! How could they attack mere scholars? Those barbarians!" */
	ACTOR_SCHOLAR_BARBARIANS,
	/* "I've waited so long to see what it's like inside Venus Lighthouse,
	   but..." (scVenusLighthouseTalkToWaitingScholar) */
	ACTOR_WAITING_SCHOLAR,
	/* "How... How did they open the door to Venus Lighthouse?" */
	ACTOR_SOLDIER_OPENED_DOOR,
	/* "They are already... headed for Babi Lighthouse..." */
	ACTOR_SOLDIER_HEADED_FOR_BABI,
	/* "Babi's soldiers are all Colosso victors... How could anyone beat all of
	   us?" His thoughts: "Was it Kraden?" */
	ACTOR_SOLDIER_COLOSSO_VICTORS
} VENUS_LIGHTHOUSE_ENTRY_ACTOR_ID;

/* VenusLighthouseGroundFloor's actors, numbering the rows of
   VenusLighthouseGroundFloorActorPlacements after Garet's. */
typedef enum VENUS_LIGHTHOUSE_GROUND_FLOOR_ACTOR_ID {
	/* "Kraden told them to stop, but they attacked anyway." */
	ACTOR_SCHOLAR_ATTACKED_ANYWAY = ACTOR_FIRST_PLACED,
	/* "Kraden went inside, but then came back and said this isn't the
	   entrance." */
	ACTOR_SCHOLAR_NOT_THE_ENTRANCE,
	/* scVenusLighthouseTalkToScholarByDoor. */
	ACTOR_DOOR_SCHOLAR,
	/* The pillar Move pushes to the wall and Carry sets down beyond it
	   (scVenusLighthouseMovePillarTowardWall). */
	ACTOR_WALL_PILLAR = ACTOR_FIRST_PLACED + 4,
	/* The lady statue Move pushes onto the floor switch (SPRITE_LADY_STATUE)
	   (scVenusLighthouseMovePillarOntoSwitch). */
	ACTOR_FLOOR_SWITCH_STATUE
} VENUS_LIGHTHOUSE_GROUND_FLOOR_ACTOR_ID;

/* VenusLighthouseFirstFloor's actors, numbering the rows of
   VenusLighthouseFirstFloorActorPlacements. */
typedef enum VENUS_LIGHTHOUSE_FIRST_FLOOR_ACTOR_ID {
	/* The three pillars, SPRITE_CAPPED_PILLAR, that Move or a push shifts, the
	   nearer drawn in front (scVenusLighthouseMoveCappedPillar). */
	ACTOR_FIRST_CAPPED_PILLAR = ACTOR_FIRST_PLACED,
	/* The two pillars by the gap, carried across it or dropped into it
	   (scVenusLighthouseCarryPillarAcrossGap). */
	ACTOR_FIRST_CARRY_PILLAR = ACTOR_FIRST_CAPPED_PILLAR + 3
} VENUS_LIGHTHOUSE_FIRST_FLOOR_ACTOR_ID;

/* VenusLighthouseSecondFloor's actors, numbering the rows of
   VenusLighthouseSecondFloorActorPlacements. */
typedef enum VENUS_LIGHTHOUSE_SECOND_FLOOR_ACTOR_ID {
	/* A lady statue, as ACTOR_FLOOR_SWITCH_STATUE is, where ACTOR_GATE_SPARKLE
	   sets out for the gate. */
	ACTOR_GATE_STATUE = ACTOR_FIRST_PLACED,
	/* The pillar before the gate, which sinks into its socket
	   (scVenusLighthouseMoveGatePillar). */
	ACTOR_GATE_PILLAR,
	/* SPRITE_PARTICLE, the kind of actor scVenusLighthouseCreateLight makes.
	   Once the pillar sinks it travels VenusSecondFloorLightToGate to the gate,
	   which opens, and stays there shedding particles. */
	ACTOR_GATE_SPARKLE
} VENUS_LIGHTHOUSE_SECOND_FLOOR_ACTOR_ID;

/* VenusLighthouseThirdFloor's actors, numbering the rows of
   VenusLighthouseThirdFloorActorPlacements. */
typedef enum VENUS_LIGHTHOUSE_THIRD_FLOOR_ACTOR_ID {
	/* A lady statue (SPRITE_LADY_STATUE), where the two gate effects set out
	   once all three switch pillars have sunk. */
	ACTOR_SWITCH_STATUE = ACTOR_FIRST_PLACED,
	/* The three pillars that sink into free sockets
	   (scVenusLighthouseMoveSwitchPillar). */
	ACTOR_FIRST_SWITCH_PILLAR,
	/* SPRITE_PARTICLE, as ACTOR_GATE_SPARKLE, placed where the gate effects end
	   once the gate is open. */
	ACTOR_SWITCH_GATE_SPARKLE = ACTOR_FIRST_SWITCH_PILLAR + 3
} VENUS_LIGHTHOUSE_THIRD_FLOOR_ACTOR_ID;

/* VenusLighthouseFourthFloor's actors, numbering the rows of
   VenusLighthouseFourthFloorActorPlacements. */
typedef enum VENUS_LIGHTHOUSE_FOURTH_FLOOR_ACTOR_ID {
	/* The two platforms, SPRITE_BALANCE_PLATFORM, that rise or sink to the
	   weight of the floating pillars set down on them
	   (scVenusLighthouseLowerFloatingPillars). */
	ACTOR_BALANCE_WEST = ACTOR_FIRST_PLACED,
	ACTOR_BALANCE_EAST,
	/* The four floating pillars Carry sets down
	   (scVenusLighthouseCarryFloatingPillar). */
	ACTOR_FIRST_FLOATING_PILLAR,
	/* A lady statue (SPRITE_LADY_STATUE), where the first gate effect sets out
	   once all four socket pillars have sunk. The camera goes to it first. */
	ACTOR_SOCKET_STATUE = ACTOR_FIRST_FLOATING_PILLAR + 4,
	/* The four pillars of the pillar room
	   (scVenusLighthouseMoveSocketPillar). */
	ACTOR_FIRST_SOCKET_PILLAR,
	/* SPRITE_PARTICLE, as ACTOR_GATE_SPARKLE, placed where the gate effects end
	   once the gate is open. */
	ACTOR_SOCKET_GATE_SPARKLE = ACTOR_FIRST_SOCKET_PILLAR + 4,
	/* The platform, SPRITE_FLOATING_PLATFORM as the floating platforms of
	   Mercury Lighthouse's aerie. It dips as the leader lands on it at
	   VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING
	   and then carries him up to the Lighthouse Aerie
	   (scVenusLighthouseRaiseLeaderInSparkles). */
	ACTOR_RISING_PLATFORM
} VENUS_LIGHTHOUSE_FOURTH_FLOOR_ACTOR_ID;

/* Animations the lighthouse plays by number beyond ACTOR_ANIMATION, from each
   sprite's animation table (graphics/sprite/<name>.tsv). */
enum {
	/* The hurt scholars and soldiers lying on the ground
	   (old_man_in_blue_cap.tsv and soldier.tsv, the same pose). */
	ANIM_HURT_LYING = 5
};

/* An animation timer step for fdActorSetAnimationSpeedById, in sixteenths of a
   tick taken off each tick. 16 plays an animation at its own speed, so 24
   plays it half again as fast. */
enum { ANIM_STEPS_FAST = 24 };

/* The pieces of gold track a SPRITE_SPIRAL_PILLAR can carry, its animations
   from 3. Each is named by the two edges of its metatile it joins as the
   pillar's top shows it, north up (graphics/sprite/spiral_pillar.png).

   The lights the lady statues send out run along the track
   (scVenusLighthouseRouteLight). A pillar sunk in a socket carries the track on
   across it. A light that runs onto a piece that does not join the edge it
   comes in by goes out. */
typedef enum VENUS_TRACK {
	VENUS_TRACK_END = 0, /* ends a list of pieces */
	VENUS_TRACK_NORTH_SOUTH = 3,
	VENUS_TRACK_NORTH_WEST = 4,
	VENUS_TRACK_NORTH_EAST = 5,
	VENUS_TRACK_WEST_EAST = 6,
	VENUS_TRACK_SOUTH_EAST = 7,
	VENUS_TRACK_SOUTH_WEST = 8
} VENUS_TRACK;

/* An actor's timer as VenusGreyStatueScript and VenusGreyPillarScript set it.
   It holds a tag in the upper bits and a colour mode (enum SPR_COL_MODE) in the
   low four, so that scVenusLighthouseRestoreMarkedColors can draw the actor in
   that mode again after a battle. */
#define VENUS_COLOR_MARK(mode) (VENUS_COLOR_MARK_TAG << 4 | (mode))

/* The tag VENUS_COLOR_MARK puts above the colour mode. */
enum { VENUS_COLOR_MARK_TAG = 0x212 };

static UNUSED Sint32 scMapGridFillCollisionRectangle(Uint32 layer, Sint32 x,
	Sint32 z, Uint32 width, Uint32 height, Sint32 value);

/* Makes a mask still, never a talk or Psynergy target, shadowless and drawn in
   the given colour mode. */
static void scVenusLighthouseConfigurePassageEffect(
	ACTOR *actor, Sint32 colorMode)
{
	actor->motionFlags = 0;
	actor->collisionFlags = ACTOR_NOT_TARGETED;
	fdActorSetSpriteFlags(actor, 0);
	fdActorSetSpriteColor(actor, colorMode);
}

/* Creates a mask over a hole as the leader or a pillar falls through the floor.

   The mask is an actor of sprite type (SPRITE_CRATE or SPRITE_CAPPED_PILLAR)
   drawn solid black (SPRITE_COLOR_BLACK) and translucent in front of the
   actors. That shows the background through whatever falls behind it, so that
   it vanishes below the hole's edge.

   Babi Lighthouse's PIT_MASK is the same actor, and its render without the mask
   shows the leader whole. */
static ACTOR *scVenusLighthouseCreateBlendedPassageEffect(
	Sint32 x, Sint32 y, Sint32 z, Sint32 type)
{
	ACTOR *effect;

	effect = syFieldActorCreate(type, x, y, z);
	if(effect != NULL) {
		((SPR *)effect->sprite)->image.priority = 0;
		scVenusLighthouseConfigurePassageEffect(effect, SPRITE_COLOR_BLACK);
		fdActorSetBlendMode(effect, ACTOR_BLEND_TRANSLUCENT);
		return effect;
	}
	return 0;
}

/* Creates an actor that keeps the leader on screen while a pillar sinks into
   its socket with him standing north of it, where the sinking code puts his
   sprite at priority 3.

   It is a SPRITE_CHEST drawn in SPRITE_COLOR_HIDDEN, so it shows nothing
   itself. Without it the leader would vanish for the 20 ticks the pillar takes
   to go down. */
static ACTOR *scVenusLighthouseCreateOccludingPassageEffect(
	Sint32 x, Sint32 y, Sint32 z, Sint32 type)
{
	ACTOR *actor;

	actor = syFieldActorCreate(type, x, y, z);
	if(actor != NULL) {
		((SPR *)actor->sprite)->image.priority = 1;
		scVenusLighthouseConfigurePassageEffect(actor, SPRITE_COLOR_HIDDEN);
		actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
		return actor;
	}
	return 0;
}

#include "shared/field_effect_spawn.c"

/* The scene's names for the bytes Actor keeps for scene code. A statue with
   SW_VENUS_IMMOVABLE set cannot be pushed (scVenusLighthousePushPillarAhead). A
   light sets SW_VENUS_LIGHT_ARRIVED as it reaches its gate, which then
   opens. */
enum {
	SW_VENUS_IMMOVABLE = SW_SCENE_COUNTER,
	SW_VENUS_LIGHT_ARRIVED = SW_SCENE_BYTE
};

#include "shared/actor_distance.c"
#include "shared/actor_find_at_position.c"

/* Pushing a pillar moves the pillar ahead of the leader one metatile on with
   him, leaning into it (ANIM_PUSH). */
static void scVenusLighthousePushPillarAhead(void)
{
	FIELD_POS targetPos;
	ACTOR	 *leader;
	ACTOR	 *pillar;
	ACTOR	 *blockingActor;
	Sint32	  directionIndex;
	FIXED	  moveRate;
	Sint32	  rising;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	directionIndex = FACING_INDEX(leader->facing);
	fdActorStepFrom(
		&targetPos, leader, PushedActorDirectionSteps[directionIndex]);
	pillar = scActorFindAtPos(&targetPos, leader);
	if(pillar == NULL)
		return;

	fdActorStepFrom(
		&targetPos, pillar, PushedActorDirectionSteps[directionIndex]);
	blockingActor = scActorFindAtPos(&targetPos, pillar);
	if(blockingActor != NULL &&
		(blockingActor->collisionFlags & ACTOR_SOLID) != 0)
		return;

	targetPos.x = pillar->x.fixed;
	targetPos.y = pillar->y.fixed + toFIXED(16);
	targetPos.z = pillar->z.fixed;
	blockingActor = scActorFindAtPos(&targetPos, pillar);
	if(blockingActor != NULL &&
		(blockingActor->collisionFlags & ACTOR_SOLID) != 0)
		return;

	pillar->layer = 2;
	fdActorStepFrom(
		&targetPos, pillar, PushedActorDirectionSteps[directionIndex]);
	if(syFieldActorCheckMovementCollision(pillar, &targetPos) > 0)
		return;

	/* A pillar still sinking into its socket stays where it is. */
	rising = pillar->sceneCount;
	if(rising != 0)
		return;

	fdActorSelectAnimation(leader, ANIM_PUSH);
	moveRate = toFIXED(1) / 5;
	sySynch(15);
	sySoundPlay(SE_PUSH);
	pillar->speedLimit = moveRate;
	pillar->acceleration = moveRate;
	syFieldActorSetMoveTargetFX(pillar, targetPos.x, targetPos.y, targetPos.z);
	leader->speedLimit = moveRate;
	leader->acceleration = moveRate;
	syFieldActorSetMoveTargetFX(leader, targetPos.x, targetPos.y, targetPos.z);
	fdActorWaitForMove(pillar);
	fdPlayQueuedMusic();
	pillar->x.fixed = targetPos.x;
	pillar->z.fixed = targetPos.z;
	/* Both stop: rising is 0 here. */
	pillar->velocityX = rising;
	pillar->velocityZ = rising;
	leader->targetX = ACTOR_NO_TARGET;
	leader->targetZ = ACTOR_NO_TARGET;
	leader->velocityX = rising;
	leader->velocityZ = rising;
	leader->x.fixed = toFIXED(leader->x.part[0].integer);
	leader->z.fixed = toFIXED(leader->z.part[0].integer);
	fdActorSelectAnimation(leader, ANIM_STAND);
}

/* Unused here: nothing in Venus Lighthouse calls it. */

#include "shared/pushed_actor_fill_rectangle.c"

/* After a battle, draws every actor whose timer holds a VENUS_COLOR_MARK in its
   colour mode again. */
static void scVenusLighthouseRestoreMarkedColors(void)
{
	ACTOR **actors = &FieldStateWork->actors[ACTOR_FIRST_PLACED];
	ACTOR  *actor;
	Uint32	actorId;

	for(actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_ID; actorId++) {
		actor = actors[actorId - ACTOR_FIRST_PLACED];
		if(actor->timer >> 4 == VENUS_COLOR_MARK_TAG)
			fdActorSetSpriteColor(actor, actor->timer & 15);
	}
}

/* Draws the actor in the colour mode of its VENUS_COLOR_MARK. */
static Sint32 scVenusLighthouseApplyMarkedColor(ACTOR *actor)
{
	fdActorSetSpriteColor(actor, actor->timer & 15);
	return 0;
}

/* The leader flashes white one tick in four as the lightning strikes him, with
   a crackle every eight ticks. */
static UNREAD_RESULT scVenusLighthouseFlashStruckLeader(ACTOR *actor)
{
	if((TickCount & 3) == 0)
		fdActorSetSpriteColor(actor, SPRITE_COLOR_WHITE);
	else
		fdActorSetSpriteColor(actor, SPRITE_COLOR_NORMAL);

	if((TickCount & 7) == 0)
		sySoundPlay(SE_ITEM_BREAK);
	RETURN_UNREAD(0);
}

/* Small black puffs of smoke rise from the leader, one every eight ticks,
   after the lightning has struck him. */
static UNREAD_RESULT scVenusLighthouseEmitRisingParticles(ACTOR *actor)
{
	EFFECT_OPTIONS options;
	Sint32		   framePhase;
	FIXED		   particleX, particleY;
	Sint32		   riseSpeed;
	framePhase = TickCount & 7;
	if(framePhase == 0) {
		options.priority = 3 - (syRandomBelow(2));
		options.startScaleX = toFIXED(2) / 5;
		options.startScaleY = toFIXED(2) / 5;
		options.palette = SPRITE_COLOR_BLACK;
		particleX = actor->x.fixed + toFIXED(syRandomBelow(9) - 4);
		particleY = actor->y.fixed + toFIXED(32 - syRandomBelow(32));
		riseSpeed = syMathDivide(toFIXED(syRandomBelow(5)) + toFIXED(5), 10);
		fdEffectSpawn(particleX, particleY, actor->z.fixed, 0, riseSpeed,
			framePhase,
			EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE,
			&options);
	}
	RETURN_UNREAD(0);
}

/* Clears the scripted actor's sprite flags and advances its script. */
static Sint32 scVenusLighthouseClearScriptedSpriteFlags(ACTOR *actor)
{
	fdActorSetSpriteFlags(actor, 0);
	return 0;
}

/* The sparks that trail the leader through a sparkling passage: a stream
   falling from his shoulders every tick and a larger one every 16. */
static UNREAD_RESULT scVenusLighthouseEmitPassageSparkTrail(ACTOR *actor)
{
	EFFECT_OPTIONS options;
	FIXED		   fallSpeed;
	Sint32		   framePhase;

	options.startScaleX = toFIXED(4) / 5;
	options.startScaleY = toFIXED(4) / 5;
	options.priority = 0;

	fallSpeed = (syRandomBelow(8)) * (toFIXED(1) / 5);
	framePhase = TickCount & 15;

	fdEffectSpawn(
		actor->x.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(8 - framePhase, 16),
		actor->y.fixed + toFIXED(26), actor->z.fixed, 0, -fallSpeed, 0,
		EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE, &options);

	framePhase = TickCount & 15;
	if(framePhase == 0) {
		options.startScaleX = toFIXED(1) / 2;
		options.startScaleY = toFIXED(1) / 2;
		fdEffectSpawn(actor->x.fixed + toFIXED(syRandomBelow(9) - 4),
			actor->y.fixed, actor->z.fixed, 0, 0, 0,
			EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE, &options);
	}

	RETURN_UNREAD(0);
}

/* Isaac walks north into one of the sparkling passages on the second and third
   floors and fades away in a trail of sparks, to come out of the passage's
   other end.

   The passage's metatile carries the number of the exit it leaves by
   (state->stepCue), and scVenusLighthouseArriveThroughSparkPassage brings him
   out sparkling at the far end. */
static void scVenusLighthouseEnterSparklingPassage(void)
{
	ACTOR		*leader;
	FIELD_STATE *state;

	state = FieldStateWork;
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	fdScriptStart();
	sySoundPlay(SE_SPARKLING_PASSAGE);
	leader->update = (void (*)(ACTOR *))scVenusLighthouseEmitPassageSparkTrail;
	leader->speedLimit = toFIXED(1) / 5;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	leader->update = 0;
	fdScriptWait(30);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(state->stepCue);
	fdScriptEnd();
}

/* Brings the leader out of a sparkling passage on a fresh arrival. */
static void scVenusLighthouseArriveThroughSparkPassage(void)
{
	ACTOR *leader;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		fdScriptStart();
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		leader->motionFlags = 0;
		fdActorSetPosFX(ACTOR_PARTY_LEADER, toFIXED(leader->x.part[0].integer),
			toFIXED(leader->z.part[0].integer) - toFIXED(16));
		fdActorSetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		sySoundPlay(SE_SPARKLING_PASSAGE);
		leader->update =
			(void (*)(ACTOR *))scVenusLighthouseEmitPassageSparkTrail;
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.4);
		fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
		fdActorSetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
		fdActorSetSpriteFlags(
			fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
		fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
		leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		leader->update = 0;
		fdPlayQueuedMusic();
		fdScriptEnd();
	}
}

/* Nothing visible happens when Isaac steps into the dotted square of floor
   where the first floor's three capped pillars stand (metatiles (11..16,
   43..53)). The game starts drawing the ground mask on him, a see-through strip
   on each side wherever the floor beside him is higher than his own, but on
   this flat floor it leaves him looking just as before. */
static void scVenusLighthouseEnterCappedPillarRows(void)
{
	GroundMaskStateWork->actor = fdActorGet(ACTOR_PARTY_LEADER);
}

/* Nothing visible happens when Isaac steps out of the dotted square of the
   capped pillars onto the floor round it. The game stops drawing the ground
   mask (scVenusLighthouseEnterCappedPillarRows) on him. */
static void scVenusLighthouseLeaveCappedPillarRows(void)
{
	GroundMaskStateWork->actor = NULL;
}

#include "shared/actor_depth.c"
#include "shared/actor_wait_until_idle.c"
#include "shared/field_effect_advance.c"

/* Isaac drops from above into the first floor's lower room after falling
   through the hole at the foot of the sand slide, and lands in a ring of white
   dust. */
static void scVenusLighthouseLandWithRadialDust(void)
{
	ACTOR		  *leader;
	Uint32		   particleIndex;
	EFFECT_OPTIONS options;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	leader->y.fixed = toFIXED(130);
	leader->gravity = toFIXED(1) / 2;
	leader->bounce = 0;
	leader->motionFlags = 0;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(30);
	sySoundPlay(SE_DUST_PUFF);
	leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdScriptWait(24);
	options.palette = SPRITE_COLOR_WHITE;
	options.update = (void (*)(ACTOR *))scFieldEffectAdvanceMotion;
	options.startScaleX = toFIXED(4) / 5;
	options.startScaleY = toFIXED(4) / 5;
	/* A puff each sixteenth of a turn, the last over the first. */
	for(particleIndex = 0; particleIndex <= TRIG_TURN / FACING_STEP;
		particleIndex++) {
		Sint32 angle;
		Sint32 velocity[3];

		angle = particleIndex * FACING_STEP;
		SET_VECTOR(velocity, syCos(angle), 0, sySin(angle));
		velocity[0] += velocity[0] / 2;
		fdEffectSpawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
			velocity[0], velocity[1], velocity[2],
			EFFECT_USE_PALETTE | EFFECT_USE_START_SCALE | EFFECT_USE_UPDATE |
				EFFECT_SCRIPT_MEDIUM,
			&options);
	}
	sySoundPlay(SE_THUMP);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
	fdMapGridSetShake(5, 5, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	fdPlayQueuedMusic();
	leader->gravity = toFIXED(1);
	leader->bounce = toFIXED(1) / 4;
	fdScriptEnd();
}

#include "shared/field_effect_radial.c"

/* The leader stands on the pink floor switch in front of the ground floor's
   lady statue and holds it down. */
#define FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH FLAG_SCENE_TEMP_01

/* A light's update function: every four ticks a shrinking particle around it,
   and now and then a sound while it moves. */
static UNREAD_RESULT scVenusLighthouseTrailLight(ACTOR *actor)
{
	EFFECT_OPTIONS options;
	Sint32		   velocityX;
	Sint32		   velocityZ;

	if((TickCount & 3) != 0) {
		RETURN_UNREAD(0);
	}
	if(syRandomBelow(6) == 0) {
		if(actor->targetX != ACTOR_NO_TARGET ||
			actor->targetZ != ACTOR_NO_TARGET) {
			sySoundPlay(SE_GLITTER);
		}
	}
	options.type = SPRITE_GLINT;
	options.startScaleX = toFIXED(1);
	options.startScaleY = toFIXED(1);
	options.targetScaleX = -toFIXED(1) / 200;
	options.targetScaleY = -toFIXED(1) / 200;
	velocityX = syMathDivide(toFIXED(syRandomBelow(9) - 4), 10);
	velocityZ = syMathDivide(toFIXED(syRandomBelow(9) - 4), 10);
	fdEffectSpawn(actor->x.fixed, actor->y.fixed, actor->z.fixed - toFIXED(1),
		velocityX, 0, velocityZ,
		EFFECT_SCALE_TO_TARGET | EFFECT_USE_START_SCALE | EFFECT_USE_TYPE |
			EFFECT_SCRIPT_MEDIUM,
		&options);
	RETURN_UNREAD(0);
}

/* Creates a light at a lady statue, at pixel x and z, and starts it along its
   path script, trailing particles (scVenusLighthouseTrailLight). */
static ACTOR *scVenusLighthouseCreateLight(
	Sint32 x, Sint32 z, const Sint32 *script)
{
	ACTOR *effect;
	FIXED  fixedX;
	FIXED  fixedZ;

	fixedX = toFIXED(x);
	fixedZ = toFIXED(z);
	effect = syFieldActorCreate(SPRITE_PARTICLE, fixedX, 0, fixedZ);
	if(effect == NULL) {
		return 0;
	}
	effect->speedLimit = toFIXED(1);
	effect->acceleration = toFIXED(1);
	fdActorSetSpriteFlags(effect, 0);
	fdActorSelectAnimation(effect, PARTICLE_ANIMATION_FLICKER);
	effect->motionFlags = 0;
	effect->y.fixed = 0;
	effect->collisionFlags = 0;
	effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	effect->update = (void (*)(ACTOR *))scVenusLighthouseTrailLight;
	effect->sceneByte = 0;
	fdActorStartScript(effect, script);
	return effect;
}

/* Walking into a one-metatile gap in the floor, Isaac springs and leaps across
   it, if the metatile beyond is clear. These are the gaps beside the pillars
   Move pushes on each floor of the climb, and on the fourth floor the gaps
   among the floating pillars.

   When something stands in the way there is no jump, and the callers try a push
   instead (the result is 0). */
static Bool scVenusLighthouseJumpAcrossGap(void)
{
	ACTOR	 *leader;
	FIELD_POS landing;
	Uint8	  savedMotionFlags;
	Sint32	  direction;
#if !JUMP_LANDS_ON_CELL_CENTER
	Uint32 step;
#endif

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	savedMotionFlags = leader->motionFlags;
#if JUMP_LANDS_ON_CELL_CENTER
	landing.x = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	landing.y = leader->y.fixed;
	landing.z = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	direction = FACING_NEAREST_CARDINAL(leader->facing);
	syVectorAddPolarOffset(toFIXED(16), direction, (FIXED *)&landing);
#else
	direction = FACING_INDEX(leader->facing);
	fdActorStepFrom(&landing, leader, PushedActorDirectionSteps[direction]);
#endif
	if(syFieldActorCheckMovementCollision(leader, &landing) != 1 &&
		scActorFindAtPos(&landing, leader) == 0) {
#if JUMP_LANDS_ON_CELL_CENTER
		landing.x = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
		landing.y = leader->y.fixed;
		landing.z = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
		syVectorAddPolarOffset(toFIXED(32),
			FACING_NEAREST_CARDINAL(leader->facing), (FIXED *)&landing);
#else
		step = PushedActorDirectionSteps[direction];
		/* Two steps on. */
		landing.x = leader->x.fixed + DIRECTION_STEP_X_FIXED(step) * 2;
		landing.y = leader->y.fixed;
		step = DIRECTION_STEP_Z_FIXED(step) * 2;
		landing.z = leader->z.fixed + step;
#endif
		if(scActorFindAtPos(&landing, leader) == 0 &&
			syFieldActorCheckMovementCollision(leader, &landing) == 0) {
			fdScriptStart();
			fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
			sySynch(6);
			sySoundPlay(SE_JUMP);
			fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
			leader->speedLimit = toFIXED(3);
			leader->acceleration = toFIXED(2);
			leader->velocityY = toFIXED(4);
			leader->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
			fdActorSetSpriteFlags(leader, 0);
			fdActorMoveToAndWait(ACTOR_PARTY_LEADER, FIXEDtoINT(landing.x),
				FIXEDtoINT(landing.z));
			fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
			fdActorSetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
			leader->motionFlags = savedMotionFlags;
			fdScriptEnd();
			return TRUE;
		}
	}
	return FALSE;
}

/* Selects the scene's entrances. VenusLighthouseGroundFloor takes the fallback
   table. */
const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_VENUS_LIGHTHOUSE_ENTRY) {
		return VenusLighthouseEntryEntrances;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_FIRST_FLOOR) {
		return VenusLighthouseFirstFloorEntrances;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_SECOND_FLOOR) {
		return VenusLighthouseSecondFloorEntrances;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_THIRD_FLOOR) {
		return VenusLighthouseThirdFloorEntrances;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_FOURTH_FLOOR) {
		return VenusLighthouseFourthFloorEntrances;
	}
	return VenusLighthouseEntrancesOther;
}

/* These scenes supply no exit regions. */
const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return 0;
}

/* Returns the packed exits shared by the six scenes. */
const Uint32 *scGetExits(void)
{
	return VenusLighthouseExits;
}

/* Selects the scene's actor placements. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	ACTOR_PLACE *placements;
	Sint16		 sceneId;

	sceneId = MainRoutineStateWork.next;
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_ENTRY) {
		return VenusLighthouseEntryActorPlacements;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_GROUND_FLOOR) {
		placements = VenusLighthouseGroundFloorActorPlacements;
		fdAssignViewFlags(placements);
		return placements;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_FIRST_FLOOR) {
		placements = VenusLighthouseFirstFloorActorPlacements;
		fdAssignViewFlags(placements);
		return placements;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_SECOND_FLOOR) {
		placements = VenusLighthouseSecondFloorActorPlacements;
		fdAssignViewFlags(placements);
		return placements;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_THIRD_FLOOR) {
		placements = VenusLighthouseThirdFloorActorPlacements;
		fdAssignViewFlags(placements);
		return placements;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_FOURTH_FLOOR) {
		placements = VenusLighthouseFourthFloorActorPlacements;
		fdAssignViewFlags(placements);
		return placements;
	}
	return VenusLighthouseActorPlacementsOther;
}

/* Isaac searches one of the pink gates at the end of a gold track, on the
   second, third or fourth floor (metatiles (46, 42), (45, 38) and (13, 36)),
   before a light has opened it, and a message says the door is tightly
   locked. */
static void scVenusLighthouseSearchLockedDoor(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgFieldDoorTightlyLocked, 1);
	fdScriptEnd();
}

/* The times the soldier who tells of the attack flickers as he vanishes, the
   pause after each a tick shorter than the last, from eight ticks to
   three. */
#define SOLDIER_FLICKERS 10

/* Outside Venus Lighthouse, Isaac talks to the dying soldier by the way in, or
   reads his mind, and he gasps out that strange wanderers attacked the
   lighthouse, then flickers and vanishes.

   "Venus Lighthouse was... attacked by strange... wanderers..." Afterwards his
   spot no longer answers a search. */
static void scVenusLighthouseHearOfTheAttack(void)
{
	Uint32 flickersLeft;
	Uint32 flickerDelay;

	fdScriptStart();
	fdActorRunRepeatedMotion(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, 3);
	fdScriptSetMessageId(MsgFieldVenusLighthouseWasAttackedBy);
	flickersLeft = SOLDIER_FLICKERS;
	flickerDelay = 8;
	/* "Venus Lighthouse was... attacked by strange... wanderers..." */
	fdScriptShowMessageThenWait(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, 0, 20);
	do {
		fdActorSetSpriteColorById(
			ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, SPRITE_COLOR_HIDDEN);
		sySynch(2);
		fdActorSetSpriteColorById(
			ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, SPRITE_COLOR_NORMAL);
		sySynch(flickerDelay);
		if(flickerDelay > 3) {
			flickerDelay--;
		}
		flickersLeft--;
	} while(flickersLeft != 0);
	gmFlagSet(FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD);
	fdActorSetPos(ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, 0, 0);
	fdMapGridCopyMetatileAttributes(7, 17, 2, 1, 7, 16);
	fdScriptEnd();
}

/* Outside Venus Lighthouse, a scholar lying hurt on the ground tells Isaac he
   has waited so long to see inside, and now cannot move for the pain. */
static void scVenusLighthouseTalkToWaitingScholar(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgVenusIveWaitedLongSeeIts);
	/* "I've waited so long to see what it's like inside Venus..." */
	fdScriptShowMessageThenWait(ACTOR_WAITING_SCHOLAR, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_WAITING_SCHOLAR, 2);
	/* "I can't even move without feeling terrible, terrible pain." */
	fdScriptShowMessage(ACTOR_WAITING_SCHOLAR, 0);
	fdScriptEnd();
}

/* Isaac lifts the grey spiral-marked block with Carry, in the room east of the
   reliefs where Move and pushing could only bring it up against the wall, and
   sets it down out of the way.

   Set down in column 54 or row 6, it stays there on later visits
   (FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED). */
static void scVenusLighthouseCarryPillarPastWall(void)
{
	ACTOR *pillar;

	pillar = fdActorGet(ACTOR_WALL_PILLAR);
	fdScriptStart();
	if(FIELD_METATILE(pillar->x.fixed) == 54 ||
		FIELD_METATILE(pillar->z.fixed) == 6) {
		gmFlagSet(FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED);
	}
	fdScriptEnd();
}

/* Isaac reads the relief on the wall of the room north of the entrance hall
   (metatiles (19, 10) to (23, 10)): "There are words carved into the relief...
   If ye seek to climb Venus Lighthouse, first seek the power within. To obtain
   this power, see with the eyes of truth. Then the way shall open." In the
   editions that keep them, two more reliefs further in (metatiles (47, 57) and
   (58, 57)) say, "The true road can only be known by he who bears a true
   heart."

   Each relief's message is the first's plus its metatile's place after
   METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF. The English edition shows
   the first relief's words for every relief and leave the two further in
   silent. */
static void scVenusLighthouseReadRelief(void)
{
	Sint32 msg;
#if !INNER_RELIEFS_SILENT
	FIELD_STATE *state = FieldStateWork;
#endif
	fdScriptStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
#if INNER_RELIEFS_SILENT
	msg = (Sint32)MSGNO(MsgVenusThereWordsCarvedIntoRelief);
#else
	msg = state->examineMetatile + MSGNO(MsgVenusThereWordsCarvedIntoRelief) -
		METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF;
#endif
	mnFieldMessageShow(msg, 1);
	fdScriptEnd();
}

/* When Reveal wears off on the lighthouse's ground floor, the two hidden ways
   it showed vanish again: the doorway at the east end of the carved reliefs
   (metatiles (23, 8..10)) and the stairs in the south of the lady statue's room
   (metatiles (46..48, 55..57)). */
static void scVenusLighthouseHidePath(void)
{
	Sint32 column;
	Sint32 row;

#if VENUS_LIGHTHOUSE_REVEAL_STOPS_ENCOUNTERS
	gmFlagClear(FLAG_NO_ENCOUNTERS_IN_AREA);
#endif
	column = 23;
	row = 8;
	fdMapGridCopyMetatileAttributes(35, 8, 1, 3, column, row);
	fdMapGridCopyMetatilesTo(35, 8, 23, 8, 1, 3);
	fdMapGridCopyMetatilesTo(99, 8, 87, 8, 1, 3);
	column = 46;
	row = 55;
	fdMapGridCopyMetatileAttributes(57, 55, 3, 3, column, row);
	fdMapGridCopyMetatilesTo(57, 55, 46, 55, 3, 3);
	fdMapGridCopyMetatilesTo(121, 55, 110, 55, 3, 3);
}

/* Isaac casts Reveal on the lighthouse's ground floor, and the screen turns
   grey as a doorway appears at the east end of the carved reliefs (metatiles
   (23, 8..10)), the way to the Carry Stone.

   Stairs appear the same way in the south of the lady statue's room (metatiles
   (46..48, 55..57)), the way on to the great statue. Both can be walked through
   while Reveal lasts. Every edition but the Japanese also stops random battles
   for that time. */
static void scVenusLighthouseRevealPath(void)
{
	Sint32 column;
	Sint32 row;

#if VENUS_LIGHTHOUSE_REVEAL_STOPS_ENCOUNTERS
	gmFlagSet(FLAG_NO_ENCOUNTERS_IN_AREA);
#endif
	column = 23;
	row = 8;
	fdMapGridCopyMetatileAttributes(36, 8, 1, 3, column, row);
	fdMapGridCopyMetatilesTo(36, 8, 23, 8, 1, 3);
	fdMapGridCopyMetatilesTo(100, 8, 87, 8, 1, 3);
	column = 46;
	row = 55;
	fdMapGridCopyMetatileAttributes(53, 55, 3, 3, column, row);
	fdMapGridCopyMetatilesTo(53, 55, 46, 55, 3, 3);
	fdMapGridCopyMetatilesTo(117, 55, 110, 55, 3, 3);
}

/* Isaac walks into the band of yellow lightning that crackles across the floor
   south of the lady statue (metatiles (41..43, 53..57)), and it strikes him. He
   flashes white, turns grey and is thrown back south, sits dazed with smoke
   rising from him, and shakes his head before he comes to.

   It strikes until Move has slid the lady statue onto the floor switch in front
   of her (FLAG_SCENE_TEMP_00). He lands on row 52 when he was struck at row 54
   or above, else on row 59. */
static void scVenusLighthouseThrowBackFromLightning(void)
{
	ACTOR *leader;
	Sint32 landingQuarterZ;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	fdScriptStart();
	fdActorSetScript(ACTOR_PARTY_LEADER, VenusLeaderShockedScript);
	fdActorWaitForScriptEndById(ACTOR_PARTY_LEADER);
	fdActorSetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_GREY);
	leader->velocityY = toFIXED(4);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 4);
	if(FIELD_METATILE(leader->z.fixed) <= 54) {
		fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
		landingQuarterZ = 210;
	} else {
		fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
		landingQuarterZ = 238;
	}
	fdActorWalkToAndWait(
		ACTOR_PARTY_LEADER, leader->x.part[0].integer, landingQuarterZ << 2);
	fdScriptWait(1);
	syActorFlagsSet(
		&fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags, ACTOR_FACES_MOTION);
	fdScriptWait(20);
	leader->update = (void (*)(ACTOR *))scVenusLighthouseEmitRisingParticles;
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
	fdActorSetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
	leader->update = 0;
	fdScriptEnd();
}

/* Redraws the great statue's room for the first of the two paths it opens
   (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH). */
static inline void scVenusLighthouseDrawFirstPath(void)
{
	fdMapGridCopyMetatilesTo(103, 27, 89, 27, 7, 8);
	fdMapGridCopyMetatilesTo(41, 90, 27, 92, 3, 2);
	fdMapGridCopyMetatilesTo(41, 90, 29, 93, 3, 2);
	fdMapGridCopyMetatilesTo(41, 90, 27, 94, 3, 2);
	fdMapGridCopyMetatilesTo(41, 90, 27, 96, 3, 2);
	fdMapGridCopyMetatilesTo(41, 90, 29, 97, 3, 2);
	fdMapGridCopyMetatilesTo(41, 96, 25, 91, 3, 2);
	fdMapGridCopyMetatilesTo(41, 92, 25, 93, 3, 2);
	fdMapGridCopyMetatilesTo(41, 96, 25, 95, 3, 2);
	fdMapGridCopyMetatilesTo(41, 96, 25, 97, 3, 2);
	fdMapGridCopyMetatilesTo(41, 96, 27, 96, 3, 2);
	fdMapGridCopyMetatilesTo(41, 96, 29, 97, 3, 2);
}

/* Redraws the great statue's room with the second path open
   (FLAG_VENUS_LIGHTHOUSE_SECOND_PATH), as scVenusLighthouseDrawFirstPath. */
static inline void scVenusLighthouseDrawSecondPath(void)
{
	fdMapGridCopyMetatilesTo(111, 27, 89, 27, 7, 8);
	fdMapGridCopyMetatilesTo(41, 90, 25, 91, 3, 2);
	fdMapGridCopyMetatilesTo(41, 90, 25, 93, 3, 2);
	fdMapGridCopyMetatilesTo(41, 90, 25, 95, 3, 2);
	fdMapGridCopyMetatilesTo(41, 90, 25, 97, 3, 2);
	fdMapGridCopyMetatilesTo(41, 90, 27, 96, 3, 2);
	fdMapGridCopyMetatilesTo(41, 90, 29, 97, 3, 2);
	fdMapGridCopyMetatilesTo(41, 94, 27, 92, 3, 2);
	fdMapGridCopyMetatilesTo(41, 96, 29, 93, 3, 2);
	fdMapGridCopyMetatilesTo(41, 94, 27, 94, 3, 2);
	fdMapGridCopyMetatilesTo(41, 96, 27, 96, 3, 2);
	fdMapGridCopyMetatilesTo(41, 96, 29, 97, 3, 2);
}

/* Isaac searches the great seated statue at the north end of its hall (metatile
   (28, 25)), and the statue speaks to his soul. The screen pales to white, and
   when it clears the floor lamps before the statue are joined by gold lines
   along one of two paths, which the camera follows south and back.

   The first time the tick counter's parity picks the path, and each later
   search switches it (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or _SECOND_PATH). The
   Tunnel Ruins under Babi Lighthouse open their ways to match. */
static void scVenusLighthouseSearchStatue(void)
{
	FIELD_STATE *state;

	state = FieldStateWork;
	state->revealFrame = 0;
	state->busy = 1;
	fdScriptStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	mnFieldMessageShow(MsgVenusStatueSpeaksIsaacSoulYe, 1);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_PALE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(120);
	fdScriptWait(100);
	sySoundPlay(SE_SWIRL);
	fdScriptWait(30);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(60);
	fdScriptWait(70);
	if(fdScriptGetFlag(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) == 0) {
		if(fdScriptGetFlag(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH) == 0) {
			if((TickCount & 1) != 0) {
				gmFlagSet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
			} else {
				gmFlagSet(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
			}
		}
	}
	if(fdScriptGetFlag(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) == 0) {
		gmFlagSet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
		gmFlagClear(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
		scVenusLighthouseDrawFirstPath();
	} else {
		gmFlagSet(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
		gmFlagClear(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
		scVenusLighthouseDrawSecondPath();
	}
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(20);
	fdScriptWait(40);
	fdCameraSetSpeed(0.5);
	fdCameraMoveTo(456, CAMERA_KEEP, 542, 1);
	fdCameraWaitForMove();
	fdScriptWait(50);
	fdCameraMoveTo(456, CAMERA_KEEP, 423, 1);
	fdCameraWaitForMove();
	fdScriptEnd();
	state->busy = 0;
}

/* Isaac Moves the grey spiral-marked block in the room east of the reliefs.
   Once it is up against the wall, the first time only, Garet steps out and says
   they can't push the block farther, that it would be great to carry it and
   drop it to the ground, and asks whether their Psynergy can help
   (FLAG_VENUS_LIGHTHOUSE_PILLAR_AT_WALL). */
static void scVenusLighthouseMovePillarTowardWall(void)
{
	ACTOR *actor;
	Sint32 garetRemarkFlag = FLAG_VENUS_LIGHTHOUSE_PILLAR_AT_WALL;
	FIXED  walkSpeed;
	FIXED  walkAcceleration;
	Sint32 emote;
	walkSpeed = toFIXED(4) / 5;
	walkAcceleration = toFIXED(2) / 5;
	emote = EMOTE_IN_FRONT | EMOTE_EXCLAMATION;
	fdScriptStart();
	if(FIELD_METATILE(fdActorGet(ACTOR_WALL_PILLAR)->x.fixed) == 53) {
		if(gmFlagGet(garetRemarkFlag) == 0) {
			gmFlagSet(garetRemarkFlag);
			actor = fdActorGet(ACTOR_PARTY_LEADER);
			if(actor != NULL) {
				fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
			}
			fdActorSetSpeedFX(ACTOR_GARET, walkSpeed, walkAcceleration);
			fdActorWalkToAndWait(ACTOR_GARET, 824, 88);
			fdActorWalkToAndWait(ACTOR_GARET, 824, 104);
			fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
			fdScriptWait(20);
			fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
			fdScriptWait(20);
			fdScriptSetMessageId(MsgVenusHmmmWeCantPushBlock);
			/* "Hmmm. We can't push the block farther than this." */
			fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
			fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 10);
			fdActorShowEmote(ACTOR_GARET, emote, 60);
			fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
			fdScriptWait(20);
			fdActorRunRepeatedMotion(ACTOR_GARET, 2);
			fdScriptWait(20);
			/* "It'd be great if we could carry that block and drop it to..." */
			fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
			fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
			fdScriptWait(30);
			fdActorWalkToAndWait(ACTOR_GARET, 824, 88);
			scWalkIntoLeader(ACTOR_GARET);
			fdScriptEnd();
		}
	}
}

/* Isaac pushes the grey spiral-marked block in the room east of the reliefs by
   walking into it, as Move would, with Garet's remark once it reaches the
   wall. */
static void scVenusLighthousePushPillarTowardWall(void)
{
	fdScriptStart();
	scVenusLighthousePushPillarAhead();
	fdScriptWait(20);
	fdScriptEnd();
	scVenusLighthouseMovePillarTowardWall();
}

/* Isaac Moves the lady statue in the north-west corner of her room east onto
   the floor switch in front of her (metatile (42, 49)). There she sinks in with
   a thump and holds the switch down, and the lightning south of her stops
   (FLAG_SCENE_TEMP_00). */
static void scVenusLighthouseMovePillarOntoSwitch(void)
{
	ACTOR *pillar;
	FIXED  switchHeight;

	pillar = fdActorGet(ACTOR_FLOOR_SWITCH_STATUE);
	fdScriptStart();
	if(FIELD_METATILE(pillar->x.fixed) == 42) {
		fdScriptWait(30);
		sySoundPlay(SE_THUMP);
		pillar->motionFlags = 0;
		switchHeight = -toFIXED(2);
		pillar->groundHeight = switchHeight;
		pillar->y.fixed = switchHeight;
		gmFlagSet(FLAG_SCENE_TEMP_00);
		fdMapGridCopyMetatilesTo(44, 117, 41, 117, 3, 5);
	}
	fdScriptEnd();
}

/* Isaac steps onto the middle of the pink floor switch in front of the lady
   statue (metatile (42, 49)): it sinks under him with a rattle, and the band of
   lightning across the floor south of her vanishes for as long as he stands
   there (FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH). Stepping off raises the
   switch and draws the lightning again. On the metatile's edge nothing happens:
   the switch stays up and the lightning keeps crackling. */
static void scVenusLighthouseStepOnFloorSwitch(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	Sint32 x = leader->x.part[0].integer;
	Sint32 z = leader->z.part[0].integer;

	if(x < 676 || x > 683 || z < 788 || z >= 796) {
		fdMapGridCopyMetatilesTo(53, 50, 42, 49, 1, 1);
		fdMapGridCopyMetatilesTo(55, 117, 41, 117, 3, 5);
		gmFlagClear(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH);
		leader->motionFlags |= ACTOR_FOLLOWS_TERRAIN;
		leader->groundHeight = 0;
		leader->y.fixed = 0;
	} else if(!fdScriptGetFlag(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH)) {
		fdScriptStart();
		fdScriptWait(5);
		fdMapGridCopyMetatilesTo(52, 50, 42, 49, 1, 1);
		fdMapGridCopyMetatilesTo(52, 117, 41, 117, 3, 5);
		gmFlagSet(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH);
		sySoundPlay(SE_RATTLE);
		leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
		leader->groundHeight = -toFIXED(2);
		leader->y.fixed = -toFIXED(2);
		fdScriptEnd();
	}
}

/* A scholar standing at the west door of the great statue's hall tells Isaac he
   came in to explore once the door opened, and the camera turns to the statue
   as he says the wanderers did something to it that made the floor light up. */
static void scVenusLighthouseTalkToScholarByDoor(void)
{
	FIELD_STATE *state;

	state = FieldStateWork;
	state->revealFrame = 0;
	state->busy = 1;
	fdScriptStart();
	fdScriptSetMessageId(MsgVenusThoughtIdExploreAfterDoor);
	fdActorFaceActor(ACTOR_DOOR_SCHOLAR, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(10);
	/* "I thought I'd explore after the door opened. While I was..." */
	fdScriptShowMessageThenWait(ACTOR_DOOR_SCHOLAR, 0, 20);
	fdActorFaceDirection(ACTOR_DOOR_SCHOLAR, FACING_NORTHEAST, 0);
	fdCameraSetSpeed(1);
	fdCameraMoveTo(448, CAMERA_KEEP, 432, 1);
	fdCameraWaitForMove();
	/* "They did something to the statue that made the floor light up." */
	fdScriptShowMessage(ACTOR_DOOR_SCHOLAR, 0);
	fdScriptEnd();
}

/* On the first floor, Isaac steps onto the hole in the floor and drops through
   it into the room below. */
static void scVenusLighthouseFallThroughFloor(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	fdScriptStart();
#if VENUS_HOLE_DROP_HOLDS_LEADER
	leader->motionFlags = 0;
#endif
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	leader->facing = FACING_SOUTH;
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 3);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, leader->x.part[0].integer, 552);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
	fdScriptWait(30);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(20);
	leader->facing = FACING_NORTH;
	/* The run cycle: his legs kick in the air as he falls. */
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_RUN);
	fdActorSetAnimationSpeedById(ACTOR_PARTY_LEADER, ANIM_STEPS_FAST);
	fdScriptWait(40);
	/* Then he drops. */
	leader->gravity = toFIXED(3) / 5;
	leader->bounce = 0;
	scVenusLighthouseCreateBlendedPassageEffect(
		leader->x.fixed, 0, leader->z.fixed + toFIXED(72), SPRITE_CRATE);
	fdMapGridCopyMetatileAttributes(VENUS_DESCENT_COLLISION_X, 35,
		VENUS_DESCENT_COLLISION_WIDTH, 1, VENUS_DESCENT_COLLISION_X, 34);
#if VENUS_HOLE_DROP_HOLDS_LEADER
	leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
#endif
	/* BUG: the fall set up above is the leader's, but the wait is given a null
	   pointer, so it reads velocityY from the BIOS, which outside a BIOS call
	   reads as its last instruction, never 0: the wait always takes its full
	   second, however soon the leader lands. */
#if FIX_FALL_WAIT_NULL_ACTOR
	scActorWaitUntilIdle(leader);
#else
	scActorWaitUntilIdle(0);
#endif
	fdActorSetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	syImportScriptSetExitRequest(
		VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM_HOLE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}

/* Isaac Moves one of the three grey pillars with white tops that stand in the
   dotted square of floor in the first floor's south-west room (metatiles
   (11..16, 43..53)), so that Isaac can leap onto their tops.

   Once a pillar has slid, the game marks the floor of the square afresh, so
   that walking into any pillar pushes it and the metatiles the pillars left are
   floor again. */
static void scVenusLighthouseMoveCappedPillar(void)
{
	Uint32 pillarIndex;
	ACTOR *pillar;
	Sint32 destColumn;
	Sint32 destRow;
	Sint32 column;
	Sint32 row;

	fdActorGet(ACTOR_FIRST_CAPPED_PILLAR);
	fdScriptStart();
	destColumn = 12;
	destRow = 44;
	fdMapGridCopyMetatileAttributes(19, 44, 4, 1, destColumn, destRow);
	destColumn = 11;
	destRow = 51;
	fdMapGridCopyMetatileAttributes(17, 51, 2, 2, destColumn, destRow);
	for(pillarIndex = 0; pillarIndex <= 2; pillarIndex++) {
		pillar = fdActorGet(pillarIndex + ACTOR_FIRST_CAPPED_PILLAR);
		column = FIELD_METATILE(pillar->x.fixed);
		row = FIELD_METATILE(pillar->z.fixed);
		fdMapGridCopyMetatileAttributes(12, 50, 1, 1, column, row);
	}
	scActorSwapPositionsByDepth(
		ACTOR_FIRST_CAPPED_PILLAR + 2, ACTOR_FIRST_CAPPED_PILLAR + 1);
	fdScriptEnd();
}

/* Isaac walks into one of the three white-topped pillars in the first floor's
   south-west room and pushes it a metatile, as Move would. */
static void scVenusLighthousePushCappedPillar(void)
{
	fdScriptStart();
	fdMapGridCopyMetatileAttributes(19, 44, 4, 1, 12, 44);
	scVenusLighthousePushPillarAhead();
	scVenusLighthouseMoveCappedPillar();
	fdScriptEnd();
}

/* Nothing visible happens as Isaac begins Move or Carry on one of the two grey
   blocks marked with a spiral by the pit on the first floor, or pushes one. The
   game first frees the metatiles under both blocks, which walking into then
   tries a jump or a push, since one of them is about to leave its metatile. */
static void scVenusLighthouseUnblockCarryPillarMetatiles(void)
{
	Uint32 pillarIndex;
	ACTOR *pillar;

	for(pillarIndex = 0; pillarIndex <= 1; pillarIndex++) {
		pillar = fdActorGet(pillarIndex + ACTOR_FIRST_CARRY_PILLAR);
		fdMapGridSetMetatileBehaviorAt(0, pillar->x.fixed, pillar->z.fixed,
			METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_CARRY_PILLARS);
	}
}

/* Isaac Moves one of the two grey spiral-marked blocks by the pit in the first
   floor's west room (metatiles (4..16, 21..27)). Nothing more happens than the
   slide: a block still standing on the floor is made solid again where it ends
   up. */
static void scVenusLighthouseMoveCarryPillar(void)
{
	Uint32 pillarIndex;
	ACTOR *pillar;

	for(pillarIndex = 0; pillarIndex <= 1; pillarIndex++) {
		pillar = fdActorGet(pillarIndex + ACTOR_FIRST_CARRY_PILLAR);
		if(pillar->y.fixed > -toFIXED(16)) {
			fdMapGridSetMetatileBehaviorAt(
				0, pillar->x.fixed, pillar->z.fixed, METATILE_BEHAVIOR_WALL);
		}
	}
}

/* Isaac walks into the pit or one of the two spiral-marked blocks beside it, in
   the first floor's west room. He leaps a one-metatile gap if the far side is
   clear; otherwise he pushes the block ahead of him. */
static void scVenusLighthouseJumpOrPushCarryPillar(void)
{
	fdScriptStart();
	if(scVenusLighthouseJumpAcrossGap() == 0) {
		scVenusLighthouseUnblockCarryPillarMetatiles();
		scVenusLighthousePushPillarAhead();
		scVenusLighthouseMoveCarryPillar();
	}
	fdScriptEnd();
}

/* A block Carry has set down in the pit stays there, its top flush with the
   floor, and its metatile becomes floor Isaac can walk across. */
static void scVenusLighthouseFixPillarAtDestination(ACTOR *pillar)
{
	Sint32 column;
	Sint32 row;

	pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
	pillar->motionFlags = 0;
	column = FIELD_METATILE(pillar->x.fixed);
	row = FIELD_METATILE(pillar->z.fixed);
	fdMapGridCopyMetatileAttributes(9, 24, 1, 1, column, row);
}

/* A block Carry has dropped outside the pit falls through the floor out of
   sight; a block still standing is made solid again. */
static void scVenusLighthouseReleaseFallenCarryPillar(ACTOR *pillar)
{
	SPR	  *sprite;
	ACTOR *passageEffect;

	sprite = pillar->sprite;
	if(sprite->image.priority == 3) {
		sprite->image.priority = 1;
		pillar->bounce = 0;
		passageEffect = scVenusLighthouseCreateBlendedPassageEffect(
			pillar->x.fixed, 0, toFIXED(512), SPRITE_CRATE);
		scActorWaitUntilIdle(pillar);
		pillar->x.fixed = 0;
		pillar->z.fixed = 0;
		fdActorRelease(passageEffect);
	} else {
		scVenusLighthouseMoveCarryPillar();
	}
}

/* Isaac lifts one of the two spiral-marked blocks in the first floor's west
   room with Carry. Set down in the pit (column 8 for the first block, 7 for the
   second), it fills it and Isaac can walk on across. One that drops lower falls
   through the floor and is gone. */
static void scVenusLighthouseCarryPillarAcrossGap(void)
{
	ACTOR *pillar;

	fdScriptStart();
	pillar = fdActorGet(ACTOR_FIRST_CARRY_PILLAR);
	if(FIELD_METATILE(pillar->x.fixed) == 8) {
		scActorWaitUntilIdle(pillar);
		scVenusLighthouseFixPillarAtDestination(pillar);
	} else {
		scVenusLighthouseReleaseFallenCarryPillar(pillar);
	}
	pillar = fdActorGet(ACTOR_FIRST_CARRY_PILLAR + 1);
	if(FIELD_METATILE(pillar->x.fixed) == 7) {
		scActorWaitUntilIdle(pillar);
		scVenusLighthouseFixPillarAtDestination(pillar);
	} else {
		scVenusLighthouseReleaseFallenCarryPillar(pillar);
	}
	fdScriptEnd();
}

/* Isaac steps onto the pink floor switch in the east of the second floor's west
   room (metatile (19, 33)). The first time, the stone wall beside it slides
   aside in a cloud of dust and the floor shakes, then the view cuts to the
   first floor, where the sand fall in the room of three doors moves east and
   leaves dry the stone chute it ran down
   (scVenusLighthouseMoveFirstFloorSandFall).

   It counts only in the middle of the metatile, where the switch sinks under
   him by 2 pixels. The wall's picture layer slides 96 pixels over 480 ticks,
   and its metatiles follow every 40 (FLAG_VENUS_LIGHTHOUSE_WALL_MOVED). */
static void scVenusLighthouseStepOnWallSwitch(void)
{
	EFFECT_OPTIONS		   options;
	MAP_GRID_LAYER_SCROLL *layer;
	ACTOR				  *leader;
	Sint32				   column;
	Sint32				   leaderZ;
	Sint32				   tileDelay;
	Uint32				   frame;
	FIXED				   dustX;

	layer = &((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers[2];
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	column = leader->x.part[0].integer;
	leaderZ = leader->z.part[0].integer;
	leader->y.fixed = 0;
	if(column >= 308 && column <= 315 && leaderZ >= 532 && leaderZ < 540) {
		leader->y.fixed = -toFIXED(2);
		if(!fdScriptGetFlag(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
			fdScriptStart();
			sySoundPlay(SE_RATTLE);
			gmFlagSet(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED);
			fdMapGridCopyMetatilesTo(26, 33, 19, 33, 1, 1);
			fdScriptWait(30);
			sySoundPlay(SE_HEAVY_MOVE);
			fdMapGridSetShake(1, 1, 1);
			fdScriptWait(20);
			dustX = toFIXED(288);
			column = 29;
			tileDelay = 40;
			for(frame = 0; frame <= 479; frame++, tileDelay--) {
				layer->offsetX += toFIXED(1) / 5;
				dustX -= toFIXED(1) / 5;
				options.priority = 2;
				options.startScaleX =
					(syRandomBelow(3)) * (toFIXED(1) / 5) + toFIXED(4) / 5;
				options.startScaleY =
					(syRandomBelow(3)) * (toFIXED(1) / 5) + toFIXED(4) / 5;
				options.spin = FIELD_EFFECT_RANDOM_SPIN();
				fdEffectSpawn(dustX, 0, toFIXED(528), 0,
					-toFIXED((TickCount & 1) * 3), 0,
					EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE |
						EFFECT_USE_SPIN,
					&options);
				if(frame == 240) {
					dustX -= toFIXED(48);
				}
				if(tileDelay == 0) {
					tileDelay = 40;
					if(frame <= 240) {
						column -= 4;
						fdMapGridCopyMetatilesTo(column, 50, 15, 32, 3, 4);
					} else {
						column += 4;
						fdMapGridCopyMetatilesTo(column, 45, 9, 32, 3, 4);
					}
				}
				sySynch(1);
			}
			layer->offsetX += toFIXED(1) / 2;
			layer->offsetX = toFIXED(layer->offsetX / toFIXED(1));
			fdMapGridCopyMetatileAttributes(15, 32, 3, 1, 9, 32);
			fdMapGridCopyMetatileAttributes(12, 32, 3, 1, 15, 32);
			sySoundPlay(SE_STOP_ACTION_SE);
			sySoundPlay(SE_THUMP);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			fdMapGridWaitForShakeToSettle();
			FieldStateWork->transition =
				SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
			syImportScriptSetExitRequest(
				VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST);
			fdScriptEnd();
		}
	}
}

/* The metatile of the socket west of the second floor's gate pillar, the dark
   square in the gold track. */
#define VENUS_GATE_PILLAR_SOCKET_COLUMN 46
#define VENUS_GATE_PILLAR_SOCKET_ROW	45

/* Isaac Moves the grey pillar carrying a straight piece of gold track, in the
   second floor's room of the lady statue and the pink gate (the pillar starts
   at metatile (48, 44)). Once it is in the dark socket in the track (metatile
   (46, 45)) it sinks in with a thump, a light runs from the lady statue along
   the track and over the pillar to the gate, and the gate opens.

   That happens the first time only (FLAG_VENUS_LIGHTHOUSE_GATE_OPEN). */
static void scVenusLighthouseMoveGatePillar(void)
{
	Sint32 row;
	Sint32 column;
	ACTOR *occlusionEffect = 0;
	ACTOR *pillar = fdActorGet(ACTOR_GATE_PILLAR);
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	fdScriptStart();
	fdMapGridCopyMetatileAttributes(109, 43, 7, 5, 45, 43);
	if((pillar->priorityFlags & ACTOR_PRIORITY_UNDERFOOT) != 0) {
		fdMapGridCopyMetatileAttributes(45, 45, 1, 1, 46, 45);
	} else {
		fdMapGridCopyMetatileAttributes(48, 42, 1, 1,
			FIELD_METATILE(pillar->x.fixed), FIELD_METATILE(pillar->z.fixed));
	}
	column = FIELD_METATILE(pillar->x.fixed);
	if(column == VENUS_GATE_PILLAR_SOCKET_COLUMN) {
		row = FIELD_METATILE(pillar->z.fixed);
		if(row == VENUS_GATE_PILLAR_SOCKET_ROW) {
			/* In its socket, it sinks in, the first time only. */
			if(!gmFlagGet(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN)) {
				if(FIELD_METATILE(leader->z.fixed) <=
					VENUS_GATE_PILLAR_SOCKET_ROW) {
					occlusionEffect =
						scVenusLighthouseCreateOccludingPassageEffect(
							toFIXED(744), 0, toFIXED(704), SPRITE_CHEST);
					fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
				}
				fdActorSetSpriteFlags(fdActorGet(ACTOR_GATE_PILLAR), 0);
				pillar->layer = 0;
				pillar->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
				pillar->gravity = toFIXED(1) / 10;
				pillar->bounce = 0;
				fdMapGridCopyMetatileAttributes(43, 45, 1, 1, column, row);
				scActorWaitUntilIdle(pillar);
				sySoundPlay(SE_THUMP);
				pillar->motionFlags = 0;
				pillar->y.fixed = -toFIXED(16);
				fdActorSetSpritePriority(ACTOR_GATE_PILLAR, 3);
				pillar->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
				fdMapGridCopyMetatileAttributes(45, 45, 1, 1, column, row);
				fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags |=
					ACTOR_PRIORITY_AUTOMATIC;
				fdActorRelease(occlusionEffect);
				pillar->collisionFlags = 0;
				pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
				fdActorGet(ACTOR_GATE_SPARKLE)->collisionFlags = 0;
				fdActorGet(ACTOR_GATE_SPARKLE)->priorityFlags |=
					ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetPos(ACTOR_GATE_SPARKLE, 808, 728);
				fdActorSetScript(
					ACTOR_GATE_SPARKLE, VenusSecondFloorLightToGate);
				fdActorWaitForScriptEndById(ACTOR_GATE_SPARKLE);
				fdScriptWait(30);
				sySoundPlay(SE_DOOR_OPEN);
				fdMapGridPlayMapEdit(VenusSecondFloorGateMetatiles, 110, 41);
				fdMapGridCopyMetatileAttributes(46, 41, 1, 1, column, 42);
				gmFlagSet(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN);
			}
		}
	}
	fdScriptEnd();
}

/* Isaac walks into the second floor's track pillar, or the socket, and pushes
   the pillar a metatile, as Move would, or leaps a one-metatile gap if the far
   side is clear. */
static void scVenusLighthouseJumpOrPushGatePillar(void)
{
	fdScriptStart();
	if(scVenusLighthouseJumpAcrossGap() == 0) {
		fdMapGridCopyMetatileAttributes(109, 43, 7, 5, 45, 43);
		scVenusLighthousePushPillarAhead();
	}
	fdScriptEnd();
	scVenusLighthouseMoveGatePillar();
}

/* Isaac Moves one of the three grey pillars carrying pieces of gold track, in
   the third floor's room of the lady statue and four sockets. A pillar that
   reaches a socket sinks in. With all three sunk, two lights run from the
   statue along the track, and the gate opens if one of them reaches it.

   Which three of the four sockets they fill decides where the track carries the
   lights (VenusThirdFloorLightToNortheast and VenusThirdFloorLightToSoutheast),
   and the flags remember which socket was left empty, the south-east or the
   north-west. */
static void scVenusLighthouseMoveSwitchPillar(void)
{
	ACTOR *occlusionEffect = 0;
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	ACTOR *pillar;
	ACTOR *other;
	ACTOR *firstGateEffect;
	ACTOR *secondGateEffect;
	Uint32 pillarId;
	Uint32 candidateIndex;
	Uint32 socketIndex;
	Sint32 savedPriority;

	fdScriptStart();
	fdMapGridCopyMetatileAttributes(108, 39, 13, 7, 44, 39);
	for(pillarId = ACTOR_FIRST_SWITCH_PILLAR;
		pillarId <= ACTOR_FIRST_SWITCH_PILLAR + 2; pillarId++) {
		pillar = fdActorGet(pillarId);
		if(pillar->priorityFlags != ACTOR_PRIORITY_UNDERFOOT) {
			fdMapGridCopyMetatileAttributes(47, 39, 1, 1,
				FIELD_METATILE(pillar->x.fixed),
				FIELD_METATILE(pillar->z.fixed));
		} else {
			fdMapGridCopyMetatileAttributes(46, 39, 1, 1,
				FIELD_METATILE(pillar->x.fixed),
				FIELD_METATILE(pillar->z.fixed));
		}
		socketIndex = NO_THIRD_FLOOR_SOCKET;
		for(candidateIndex = 0; candidateIndex <= 3; candidateIndex++) {
			if(FIELD_METATILE(pillar->x.fixed) ==
					VenusThirdFloorSockets[candidateIndex].x &&
				FIELD_METATILE(pillar->z.fixed) ==
					VenusThirdFloorSockets[candidateIndex].z &&
				pillar->y.fixed >= 0) {
				socketIndex = candidateIndex;
				break;
			}
		}
		if(socketIndex == NO_THIRD_FLOOR_SOCKET) {
			continue;
		}
		for(candidateIndex = ACTOR_FIRST_SWITCH_PILLAR;
			candidateIndex <= ACTOR_FIRST_SWITCH_PILLAR + 2; candidateIndex++) {
			other = fdActorGet(candidateIndex);
			if(pillarId != candidateIndex &&
				FIELD_METATILE(pillar->x.fixed) ==
					FIELD_METATILE(other->x.fixed) &&
				FIELD_METATILE(pillar->z.fixed) ==
					FIELD_METATILE(other->z.fixed)) {
				socketIndex = NO_THIRD_FLOOR_SOCKET;
				break;
			}
		}
		if(socketIndex == NO_THIRD_FLOOR_SOCKET) {
			continue;
		}
		savedPriority = leader->sprite->image.priority;
		if(FIELD_METATILE(leader->z.fixed) <=
			VenusThirdFloorSockets[socketIndex].z) {
			occlusionEffect = scVenusLighthouseCreateOccludingPassageEffect(
				pillar->x.fixed, pillar->y.fixed, pillar->z.fixed - toFIXED(4),
				SPRITE_CHEST);
			fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
		}
		fdActorSetSpriteFlags(fdActorGet(pillarId), 0);
		pillar->layer = 0;
		pillar->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		pillar->bounce = 0;
		pillar->gravity = toFIXED(1) / 10;
		fdMapGridCopyMetatileAttributes(42, 41, 1, 1,
			VenusThirdFloorSockets[socketIndex].x,
			VenusThirdFloorSockets[socketIndex].z);
		scActorWaitUntilIdle(pillar);
		sySoundPlay(SE_THUMP);
		pillar->collisionFlags = 0;
		pillar->motionFlags = 0;
		pillar->y.fixed = -toFIXED(16);
		fdActorSetSpritePriority(pillarId, 3);
		pillar->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		fdMapGridCopyMetatileAttributes(46, 39, 1, 1,
			VenusThirdFloorSockets[socketIndex].x,
			VenusThirdFloorSockets[socketIndex].z);
		fdActorSetSpritePriority(ACTOR_PARTY_LEADER, savedPriority);
		fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags |=
			ACTOR_PRIORITY_AUTOMATIC;
		if(occlusionEffect != NULL) {
			fdActorRelease(occlusionEffect);
		}
		if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN)) {
			fdScriptEnd();
			return;
		}
		if((fdActorGet(ACTOR_FIRST_SWITCH_PILLAR)->priorityFlags &
			   fdActorGet(ACTOR_FIRST_SWITCH_PILLAR + 1)->priorityFlags &
			   fdActorGet(ACTOR_FIRST_SWITCH_PILLAR + 2)->priorityFlags &
			   ACTOR_PRIORITY_UNDERFOOT) == 0) {
			continue;
		}
		/* With all three sunk, two lights run from the statue along the
		   track. */
		firstGateEffect = scVenusLighthouseCreateLight(
			888, 680, VenusThirdFloorLightToNortheast);
		secondGateEffect = scVenusLighthouseCreateLight(
			888, 680, VenusThirdFloorLightToSoutheast);
		/* The gate opens as soon as either light reaches it
		   (SW_VENUS_LIGHT_ARRIVED), while the other may still be running. */
		while(
			(firstGateEffect)->script != 0 || (secondGateEffect)->script != 0) {
			if(firstGateEffect->sceneByte != 0 ||
				secondGateEffect->sceneByte != 0) {
				fdScriptWait(30);
				sySoundPlay(SE_DOOR_OPEN);
				fdMapGridPlayMapEdit(VenusThirdFloorGateMetatiles, 109, 37);
				fdMapGridCopyMetatileAttributes(45, 37, 1, 1, 45, 38);
				if(FIELD_METATILE(
					   fdActorGet(ACTOR_FIRST_SWITCH_PILLAR)->x.fixed) ==
						VenusThirdFloorSockets[0].x &&
					FIELD_METATILE(
						fdActorGet(ACTOR_FIRST_SWITCH_PILLAR)->z.fixed) ==
						VenusThirdFloorSockets[0].z) {
					gmFlagSet(FLAG_VENUS_LIGHTHOUSE_SOUTHEAST_SOCKET_EMPTY);
				} else {
					gmFlagSet(FLAG_VENUS_LIGHTHOUSE_NORTHWEST_SOCKET_EMPTY);
				}
				gmFlagSet(FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN);
				break;
			}
			sySynch(1);
		}
	}
	fdScriptEnd();
}

/* Isaac walks into one of the third floor's three track pillars and pushes it a
   metatile, as Move would, or leaps a one-metatile gap if the far side is
   clear. */
static void scVenusLighthouseJumpOrPushSwitchPillar(void)
{
	fdScriptStart();
	if(scVenusLighthouseJumpAcrossGap() == 0) {
		fdMapGridCopyMetatileAttributes(108, 39, 13, 7, 44, 39);
		scVenusLighthousePushPillarAhead();
	}
	fdScriptEnd();
	scVenusLighthouseMoveSwitchPillar();
}

/* A block set down on a balance platform rides it up and down, 16 pixels above
   the platform or the block under it. */
static UNREAD_RESULT scVenusLighthouseFollowSupportingPillarHeight(
	ACTOR *pillar)
{
	pillar->y.fixed = fdActorGet(pillar->timer)->y.fixed + toFIXED(16);
	RETURN_UNREAD(0);
}

/* The two balance platforms of the fourth floor's west room sink and rise to
   the heights their loads give them, with a grinding sound. A platform that
   ends between the floor and 30 pixels below it can be stepped on. */
static void scVenusLighthouseLowerFloatingPillars(Sint32 waitTicks)
{
	ACTOR *westSupport = fdActorGet(ACTOR_BALANCE_WEST);
	ACTOR *eastSupport = fdActorGet(ACTOR_BALANCE_EAST);
	Uint32 pillarIndex;

	fdActorSetSpeed(ACTOR_BALANCE_WEST, 0.5);
	fdActorSetSpeed(ACTOR_BALANCE_EAST, 0.5);
	if(waitTicks != 0) {
		sySoundPlay(SE_SETTLE);
	}
	syFieldActorSetMoveTargetFX(westSupport, westSupport->x.fixed,
		BalancePlatformHeights[westSupport->timer], westSupport->z.fixed);
	syFieldActorSetMoveTargetFX(eastSupport, eastSupport->x.fixed,
		BalancePlatformHeights[eastSupport->timer], eastSupport->z.fixed);
	fdActorWaitForMoveById(ACTOR_BALANCE_WEST);
	fdActorWaitForMoveById(ACTOR_BALANCE_EAST);
	westSupport->y.fixed = BalancePlatformHeights[westSupport->timer];
	eastSupport->y.fixed = BalancePlatformHeights[eastSupport->timer];
	if(waitTicks != 0) {
		sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	}
	/* The two platforms and the first three floating pillars: the fourth,
	   ACTOR_FIRST_FLOATING_PILLAR + 3, is left out. */
	/* XXX: whether that shows in play, the fourth block's metatile not opened
	   when it ends in reach; a recording of the four blocks set down with the
	   fourth last, on top of a stack that ends between the floor and 30 pixels
	   below it, settles it. */
	for(pillarIndex = 0; pillarIndex < 5; pillarIndex++) {
		ACTOR *actor = fdActorGet(pillarIndex + ACTOR_BALANCE_WEST);

		if(actor->y.fixed / toFIXED(1) < 0 &&
			actor->y.fixed / toFIXED(1) > -30) {
			fdMapGridCopyMetatileAttributes(4, 19, 1, 1,
				FIELD_METATILE(actor->x.fixed), FIELD_METATILE(actor->z.fixed));
		}
	}
	fdScriptWait(waitTicks);
}

/* Sets a block Carry has dropped on a balance platform on top of whatever is
   highest in its metatile: the platform, or a block already on it. */
static void scVenusLighthouseStackFloatingPillar(Sint32 pillarId)
{
	ACTOR *candidate;
	FIXED  supportHeight = -toFIXED(80);
	ACTOR *pillar = 0;
	Uint32 candidateIndex;

	for(candidateIndex = 0; candidateIndex <= 5; candidateIndex++) {
		Sint32 candidateId = candidateIndex + ACTOR_BALANCE_WEST;
		FIXED  candidateTop;

		if(candidateId == pillarId) {
			continue;
		}

		candidate = fdActorGet(candidateId);
		pillar = fdActorGet(pillarId);

		if(FIELD_METATILE(candidate->x.fixed) !=
				FIELD_METATILE(pillar->x.fixed) ||
			(FIELD_METATILE(candidate->z.fixed)) !=
				FIELD_METATILE(pillar->z.fixed)) {
			continue;
		}

		candidateTop = candidate->y.fixed + toFIXED(16);
		if(supportHeight <= candidateTop) {
			pillar->timer = candidateId;
			supportHeight = candidateTop;
		}
	}

	fdActorSetSpeed(pillarId, 4);
	syFieldActorSetMoveTargetFX(
		pillar, pillar->x.fixed, supportHeight, pillar->z.fixed);
	fdActorWaitForMoveById(pillarId);
	sySoundPlay(SE_THUMP);
	scFieldEffectSpawnNineRadialEffects(pillarId);
	fdScriptWait(30);
}

/* Stops a floating block where it is. */
static inline void scVenusLighthouseSettleFloatingPillar(ACTOR *pillar)
{
	pillar->targetY = ACTOR_NO_TARGET;
	pillar->groundHeight = 0;
	pillar->velocityY = 0;
	pillar->motionFlags = 0;
	pillar->timer = 0;
}

/* Copies all three fixed-point position components without changing motion. */
static inline void scVenusLighthouseCopyPos(ACTOR *destination, ACTOR *source)
{
	destination->x.fixed = source->x.fixed;
	destination->y.fixed = source->y.fixed;
	destination->z.fixed = source->z.fixed;
}

/* Isaac lifts one of the four grey spiral-marked blocks floating in the fourth
   floor's west room with Carry and drops it onto one of the two balance
   platforms at the south of the room. The platforms sink and rise to the
   blocks' weight; one that ends between the floor and 30 pixels below it can be
   stepped on (scVenusLighthouseLowerFloatingPillars).

   A block dropped on a platform stays there (FLAG_SCENE_TEMP_00 plus its
   number), and the camera looks south at the platforms as they move. */
static void scVenusLighthouseCarryFloatingPillar(void)
{
	ACTOR  savedPosition;
	ACTOR *firstPassageEffect = NULL;
	ACTOR *secondPassageEffect = NULL;
	ACTOR *pillar;
	ACTOR *other;
	Sint32 pillarIndex;
	Sint32 earlierIndex;
	Sint32 settledIndex;
	Sint32 column;
	Sint32 row;

	fdScriptStart();
	for(pillarIndex = 0; pillarIndex < 4; pillarIndex++) {
		pillar = fdActorGet(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR);
		column = FIELD_METATILE(pillar->x.fixed);
		if(column == 13 && (row = FIELD_METATILE(pillar->z.fixed)) == 7 &&
			!gmFlagGet(FLAG_SCENE_TEMP_00 + pillarIndex)) {
			scActorWaitUntilIdle(pillar);
			gmFlagSet(FLAG_SCENE_TEMP_00 + pillarIndex);
			pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
			pillar->collisionFlags = 0;
			pillar->motionFlags = 0;
			fdMapGridCopyMetatileAttributes(4, 19, 1, 1, column, row);
			break;
		}
		if(pillar->sprite->image.priority == 3 &&
			!fdScriptGetFlag(FLAG_SCENE_TEMP_00 + pillarIndex)) {
			/* Default: 0 until the settling search below sets it. */
			earlierIndex = 0;
			fdActorSetSpritePriority(
				pillarIndex + ACTOR_FIRST_FLOATING_PILLAR, 1);
			pillar->bounce = 0;
			if(FIELD_METATILE(pillar->z.fixed) <= 12) {
				firstPassageEffect =
					scVenusLighthouseCreateBlendedPassageEffect(
						pillar->x.fixed, 0, toFIXED(224), SPRITE_CAPPED_PILLAR);
				secondPassageEffect =
					scVenusLighthouseCreateBlendedPassageEffect(
						pillar->x.fixed, 0, toFIXED(240), SPRITE_CAPPED_PILLAR);
			}
			scActorWaitUntilIdle(pillar);
			fdActorSetPos(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR, 0, 0);
			fdActorRelease(firstPassageEffect);
			fdActorRelease(secondPassageEffect);
			gmFlagSet(FLAG_SCENE_TEMP_00 + pillarIndex);
			break;
		}
		if(FIELD_METATILE(pillar->z.fixed) == 19 &&
			!gmFlagGet(FLAG_SCENE_TEMP_00 + pillarIndex)) {
			scVenusLighthouseSettleFloatingPillar(pillar);
			settledIndex = pillarIndex;
			for(earlierIndex = 0; earlierIndex < pillarIndex; earlierIndex++) {
				if(!gmFlagGet(FLAG_SCENE_TEMP_00 + earlierIndex)) {
					other =
						fdActorGet(earlierIndex + ACTOR_FIRST_FLOATING_PILLAR);
					scVenusLighthouseCopyPos(&savedPosition, pillar);
					scVenusLighthouseCopyPos(pillar, other);
					scVenusLighthouseCopyPos(other, &savedPosition);
					settledIndex = earlierIndex;
					break;
				}
			}
			other = fdActorGet(settledIndex + ACTOR_FIRST_FLOATING_PILLAR);
			scVenusLighthouseSettleFloatingPillar(other);
			fdCameraSetSpeed(3);
			fdCameraGetActor()->motionFlags = 0;
			fdCameraMoveTo(136, 8, 344, 1);
			fdCameraWaitForMove();
			/* Stack the pillar, then rebalance the two supports and attach the
			   height follower. */
			scVenusLighthouseStackFloatingPillar(
				settledIndex + ACTOR_FIRST_FLOATING_PILLAR);
			if(FIELD_METATILE(
				   fdActorGet(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)
					   ->x.fixed) == 6) {
				fdActorGet(ACTOR_BALANCE_WEST)->timer++;
				fdActorGet(ACTOR_BALANCE_EAST)->timer--;
			} else {
				fdActorGet(ACTOR_BALANCE_WEST)->timer--;
				fdActorGet(ACTOR_BALANCE_EAST)->timer++;
			}
#if FIX_FUNCTION_SIGNATURES
			fdActorGet(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)->update =
				scVenusLighthouseFollowSupportingPillarHeight;
#else
			fdActorGet(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)->update =
				(void (*)(
					ACTOR *))scVenusLighthouseFollowSupportingPillarHeight;
#endif
			scVenusLighthouseLowerFloatingPillars(40);
			fdActorGet(settledIndex + ACTOR_FIRST_FLOATING_PILLAR)
				->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
			gmFlagSet(FLAG_SCENE_TEMP_00 + settledIndex);
			break;
		}
	}
	fdScriptEnd();
}

/* Isaac walks into one of the floating blocks in the fourth floor's west room
   and pushes it a metatile. */
static void scVenusLighthousePushPuzzlePillar(void)
{
	fdScriptStart();
	scVenusLighthousePushPillarAhead();
	fdScriptEnd();
}

/* Isaac steps onto the pink floor switch on the west side of the fourth floor's
   big middle room (metatile (33, 20)). The first time, a wall slides aside in a
   cloud of dust, and the view cuts to the third floor's room of three sand
   slides, where sand pours down the west one
   (scVenusLighthousePourWestSandSlide). */
static void scVenusLighthouseStepOnWestWallSwitch(void)
{
	EFFECT_OPTIONS		   options;
	MAP_GRID_LAYER_SCROLL *layer;
	ACTOR				  *leader;
	Sint32				   column;
	Sint32				   leaderZ;
	Sint32				   tileDelay;
	Uint32				   frame;
	FIXED				   dustX;

	layer = &((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers[2];
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	column = leader->x.part[0].integer;
	leaderZ = leader->z.part[0].integer;
	leader->y.fixed = 0;
	if(column >= 532 && column <= 539 && leaderZ >= 324 && leaderZ < 332) {
		leader->y.fixed = -toFIXED(2);
		if(!gmFlagGet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
			fdScriptStart();
			fdMapGridCopyMetatilesTo(63, 29, 33, 20, 1, 1);
			sySoundPlay(SE_RATTLE);
			fdMapGridCopyMetatilesTo(44, 83, 44, 80, 3, 3);
			fdScriptWait(30);
			fdMapGridSetShake(1, 1, 1);
			sySoundPlay(SE_HEAVY_MOVE);
			fdScriptWait(20);
			dustX = toFIXED(616);
			column = 0;
			tileDelay = 60;
			for(frame = 0; frame <= 319; frame++, tileDelay--) {
				layer->offsetX += toFIXED(1) / 5;
				if(dustX >= toFIXED(576) && frame > 40) {
					dustX -= toFIXED(1) / 5;
					options.priority = 2;
					options.startScaleX =
						(syRandomBelow(3)) * (toFIXED(1) / 5) + toFIXED(4) / 5;
					options.startScaleY =
						(syRandomBelow(3)) * (toFIXED(1) / 5) + toFIXED(4) / 5;
					options.spin = FIELD_EFFECT_RANDOM_SPIN();
					fdEffectSpawn(dustX, 0, toFIXED(288), 0,
						-toFIXED((TickCount & 1) * 3), 0,
						EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE |
							EFFECT_USE_SPIN,
						&options);
					if(tileDelay == 0) {
						tileDelay = 40;
						column += 4;
						fdMapGridCopyMetatilesTo(column, 56, 36, 17, 3, 4);
					}
				}
				sySynch(1);
			}
			layer->offsetX += toFIXED(1) / 2;
			layer->offsetX = toFIXED(layer->offsetX / toFIXED(1));
			sySoundPlay(SE_STOP_ACTION_SE);
			sySoundPlay(SE_THUMP);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			fdMapGridWaitForShakeToSettle();
			FieldStateWork->transition =
				SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
			syImportScriptSetExitRequest(
				VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST);
			fdScriptEnd();
		}
	}
}

/* Isaac steps onto the pink floor switch on the east side of the fourth floor's
   big middle room (metatile (49, 20)), and as with the west one sand pours down
   the third floor's east slide (scVenusLighthousePourEastSandSlide). */
static void scVenusLighthouseStepOnEastWallSwitch(void)
{
	EFFECT_OPTIONS		   options;
	MAP_GRID_LAYER_SCROLL *layer;
	ACTOR				  *leader;
	Sint32				   column;
	Sint32				   leaderZ;
	Sint32				   tileDelay;
	Uint32				   frame;
	FIXED				   dustX;

	layer = &((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers[2];
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	column = leader->x.part[0].integer;
	leaderZ = leader->z.part[0].integer;
	leader->y.fixed = 0;
	if(column >= 788 && column <= 795 && leaderZ >= 324 && leaderZ < 332) {
		leader->y.fixed = -toFIXED(2);
		if(!gmFlagGet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
			fdScriptStart();
			fdMapGridCopyMetatilesTo(63, 29, 49, 20, 1, 1);
			sySoundPlay(SE_RATTLE);
			fdScriptWait(30);
			fdMapGridSetShake(1, 1, 1);
			sySoundPlay(SE_HEAVY_MOVE);
			fdScriptWait(20);
			dustX = toFIXED(704);
			column = 61;
			tileDelay = 60;
			for(frame = 0; frame <= 319; frame++, tileDelay--) {
				layer->offsetX -= toFIXED(1) / 5;
				dustX += toFIXED(1) / 5;
				if(dustX >= toFIXED(712) && dustX < toFIXED(752)) {
					options.priority = 2;
					options.startScaleX =
						(syRandomBelow(3)) * (toFIXED(1) / 5) + toFIXED(4) / 5;
					options.startScaleY =
						(syRandomBelow(3)) * (toFIXED(1) / 5) + toFIXED(4) / 5;
					options.spin = FIELD_EFFECT_RANDOM_SPIN();
					fdEffectSpawn(dustX, 0, toFIXED(288), 0,
						-toFIXED((TickCount & 1) * 3), 0,
						EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE |
							EFFECT_USE_SPIN,
						&options);
					if(tileDelay == 0) {
						tileDelay = 40;
						column -= 4;
						fdMapGridCopyMetatilesTo(column, 56, 44, 17, 3, 4);
					}
				}
				sySynch(1);
			}
			layer->offsetX += toFIXED(1) / 2;
			layer->offsetX = toFIXED(layer->offsetX / toFIXED(1));
			sySoundPlay(SE_STOP_ACTION_SE);
			sySoundPlay(SE_THUMP);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			fdMapGridWaitForShakeToSettle();
			FieldStateWork->transition =
				SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
			syImportScriptSetExitRequest(
				VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST);
			fdScriptEnd();
		}
	}
}

/* Isaac Moves one of the four grey pillars carrying pieces of gold track, in
   the fourth floor's south-west room, where the track runs from the lady statue
   up to the pink gate. Each pillar that reaches one of the eight sockets sinks
   in, and once the four are sunk the light runs from the statue, opening the
   gate when it reaches it (FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN). */
static void scVenusLighthouseMoveSocketPillar(void)
{
	ACTOR *occlusionEffect;
	ACTOR *pillar;
	ACTOR *otherPillar;
	ACTOR *leader;
	ACTOR *firstPillar;
	ACTOR *secondPillar;
	ACTOR *thirdPillar;
	ACTOR *fourthPillar;
	Uint32 pillarId;
	Uint32 candidateIndex;
	Uint32 socketIndex;
	Uint32 savedPriority;

	occlusionEffect = 0;
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	fdScriptStart();
#if SOCKET_PILLAR_ENDS_REVEAL
	fdEndBusyAndReveal();
#endif
	fdMapGridCopyMetatileAttributes(69, 48, 4, 2, 5, 48);
	fdMapGridCopyMetatileAttributes(73, 37, 9, 13, 9, 37);
	for(pillarId = ACTOR_FIRST_SOCKET_PILLAR;
		pillarId <= ACTOR_FIRST_SOCKET_PILLAR + 3; pillarId++) {
		pillar = fdActorGet(pillarId);
		if(pillar->priorityFlags != ACTOR_PRIORITY_UNDERFOOT)
			fdMapGridCopyMetatileAttributes(72, 48, 1, 1,
				FIELD_METATILE(pillar->x.fixed),
				FIELD_METATILE(pillar->z.fixed));
		else
			fdMapGridCopyMetatileAttributes(73, 48, 1, 1,
				FIELD_METATILE(pillar->x.fixed),
				FIELD_METATILE(pillar->z.fixed));

		socketIndex = NO_FOURTH_FLOOR_SOCKET;
		for(candidateIndex = 0; candidateIndex < NO_FOURTH_FLOOR_SOCKET;
			candidateIndex++) {
			if(FIELD_METATILE(pillar->x.fixed) ==
					VenusFourthFloorSockets[candidateIndex].x &&
				(FIELD_METATILE(pillar->z.fixed)) ==
					VenusFourthFloorSockets[candidateIndex].z &&
				pillar->y.fixed >= 0) {
				socketIndex = candidateIndex;
				break;
			}
		}
		if(socketIndex == NO_FOURTH_FLOOR_SOCKET)
			continue;
		for(candidateIndex = ACTOR_FIRST_SOCKET_PILLAR;
			candidateIndex <= ACTOR_FIRST_SOCKET_PILLAR + 3; candidateIndex++) {
			otherPillar = fdActorGet(candidateIndex);
			if(pillarId != candidateIndex &&
				(FIELD_METATILE(pillar->x.fixed)) ==
					FIELD_METATILE(otherPillar->x.fixed) &&
				(FIELD_METATILE(pillar->z.fixed)) ==
					FIELD_METATILE(otherPillar->z.fixed)) {
				socketIndex = NO_FOURTH_FLOOR_SOCKET;
				break;
			}
		}
		if(socketIndex == NO_FOURTH_FLOOR_SOCKET)
			continue;

		savedPriority = leader->sprite->image.priority;
		if(FIELD_METATILE(leader->z.fixed) <=
			VenusFourthFloorSockets[socketIndex].z) {
			occlusionEffect = scVenusLighthouseCreateOccludingPassageEffect(
				pillar->x.fixed, pillar->y.fixed, pillar->z.fixed - toFIXED(4),
				SPRITE_CHEST);
			fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
		}
		for(candidateIndex = ACTOR_FIRST_SOCKET_PILLAR;
			candidateIndex <= ACTOR_FIRST_SOCKET_PILLAR + 3; candidateIndex++) {
			otherPillar = fdActorGet(candidateIndex);
			if(pillarId != candidateIndex &&
				(FIELD_METATILE(pillar->x.fixed)) ==
					FIELD_METATILE(otherPillar->x.fixed) &&
				(FIELD_METATILE(pillar->z.fixed)) - 1 ==
					(FIELD_METATILE(otherPillar->z.fixed)))
				fdActorSetSpritePriority(candidateIndex, 3);
		}
		fdActorSetSpriteFlags(fdActorGet(pillarId), 0);
		pillar->layer = 0;
		pillar->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		pillar->gravity = toFIXED(1) / 10;
		pillar->bounce = 0;
		fdMapGridCopyMetatileAttributes(6, 44, 1, 1,
			VenusFourthFloorSockets[socketIndex].x,
			VenusFourthFloorSockets[socketIndex].z);
		scActorWaitUntilIdle(pillar);
		sySoundPlay(SE_THUMP);
		pillar->collisionFlags = 0;
		pillar->motionFlags = 0;
		pillar->y.fixed = -toFIXED(16);
		fdActorSetSpritePriority(pillarId, 3);
		pillar->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		fdMapGridCopyMetatileAttributes(73, 48, 1, 1,
			VenusFourthFloorSockets[socketIndex].x,
			VenusFourthFloorSockets[socketIndex].z);
		fdActorSetSpritePriority(ACTOR_PARTY_LEADER, savedPriority);
		fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags |=
			ACTOR_PRIORITY_AUTOMATIC;
		for(candidateIndex = ACTOR_FIRST_SOCKET_PILLAR;
			candidateIndex <= ACTOR_FIRST_SOCKET_PILLAR + 3; candidateIndex++) {
			otherPillar = fdActorGet(candidateIndex);
			if(pillarId != candidateIndex &&
				(FIELD_METATILE(pillar->x.fixed)) ==
					FIELD_METATILE(otherPillar->x.fixed) &&
				(FIELD_METATILE(pillar->z.fixed)) - 1 ==
					(FIELD_METATILE(otherPillar->z.fixed))) {
				fdActorSetSpritePriority(candidateIndex, 1);
				fdActorGet(candidateIndex)->priorityFlags |=
					ACTOR_PRIORITY_AUTOMATIC;
			}
		}
		fdActorRelease(occlusionEffect);
		if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN)) {
			fdScriptEnd();
			return;
		}
		/* With all four sunk, the lights run from the statue to the gate. */
		firstPillar = fdActorGet(ACTOR_FIRST_SOCKET_PILLAR);
		secondPillar = fdActorGet(ACTOR_FIRST_SOCKET_PILLAR + 1);
		thirdPillar = fdActorGet(ACTOR_FIRST_SOCKET_PILLAR + 2);
		fourthPillar = fdActorGet(ACTOR_FIRST_SOCKET_PILLAR + 3);
		if((firstPillar->priorityFlags & secondPillar->priorityFlags &
			   thirdPillar->priorityFlags & fourthPillar->priorityFlags) &
			ACTOR_PRIORITY_UNDERFOOT) {
			ACTOR *firstGateEffect;
			ACTOR *secondGateEffect;

			fdCameraSetSpeed(1);
			fdCameraMoveToActorById(ACTOR_SOCKET_STATUE, 1);
			fdCameraWaitForMove();
			firstGateEffect = scVenusLighthouseCreateLight(
				136, 776, VenusFourthFloorLightFromStatue);
			fdScriptWait(30);
			fdCameraSetSpeed(0.4);
			fdCameraMoveTo(216, CAMERA_KEEP, 632, 1);
			fdActorWaitForScriptEnd(firstGateEffect);
			fdActorStartScript(
				firstGateEffect, VenusFourthFloorLightToSoutheast);
			secondGateEffect = scVenusLighthouseCreateLight(
				216, 760, VenusFourthFloorLightToSouthwest);
			while((firstGateEffect)->script != 0 ||
				(secondGateEffect)->script != 0) {
				if(firstGateEffect->sceneByte != 0 ||
					secondGateEffect->sceneByte != 0) {
					fdScriptWait(30);
					fdMapGridAnimateMetatiles(
						VenusFourthFloorGateMetatiles, 77, 35);
					fdMapGridCopyMetatileAttributes(13, 35, 1, 1, 13, 36);
					gmFlagSet(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN);
					break;
				}
				sySynch(1);
			}
		}
	}
	fdScriptEnd();
}

/* Isaac walks into one of the fourth floor's four track pillars and pushes it a
   metatile, as Move would, or leaps a one-metatile gap if the far side is
   clear. */
static void scVenusLighthouseJumpOrPushSocketPillar(void)
{
	fdScriptStart();
	if(scVenusLighthouseJumpAcrossGap() == 0) {
		fdMapGridCopyMetatileAttributes(69, 48, 4, 2, 5, 48);
		fdMapGridCopyMetatileAttributes(73, 37, 9, 13, 9, 37);
		scVenusLighthousePushPillarAhead();
	}
	fdScriptEnd();
	scVenusLighthouseMoveSocketPillar();
}

/* Isaac walks south into the gap in the floor of the fourth floor's last room
   (metatile (32, 44)) and drops through it. He lands on a small round platform,
   which carries him up to the Lighthouse Aerie
   (scVenusLighthouseRaiseLeaderInSparkles). */
static void scVenusLighthouseEnterRisingPassage(void)
{
	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 520, 712);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
	(void)scVenusLighthouseCreateBlendedPassageEffect(
		toFIXED(520), 0, toFIXED(784), SPRITE_CRATE);
	syFieldActorJumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
	fdScriptWait(60);
	syImportScriptSetExitRequest(
		VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES_RISING);
	fdScriptEnd();
}

/* Selects the puzzle interactions and dialogue for the current lighthouse
   scene. */
const CUE *scGetCues(void)
{
	Sint32 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_VENUS_LIGHTHOUSE_ENTRY) {
		return VenusLighthouseEntryCues;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_GROUND_FLOOR) {
		return VenusLighthouseGroundFloorCues;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_FIRST_FLOOR) {
		return VenusLighthouseFirstFloorCues;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_SECOND_FLOOR) {
		return VenusLighthouseSecondFloorCues;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_THIRD_FLOOR) {
		return VenusLighthouseThirdFloorCues;
	}
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_FOURTH_FLOOR) {
		return VenusLighthouseFourthFloorCues;
	}
	return VenusLighthouseGroundFloorCues;
}

/* Lays one of the scholars or soldiers the wanderers hurt on the ground
   (ANIM_HURT_LYING), drawn behind the backgrounds' fronts, still, and neither
   solid nor pushable. */
static void scVenusLighthouseLayDownHurtActor(Sint32 actorId)
{
	ACTOR *actor;

	actor = fdActorGet(actorId);
	actor->motionFlags = 0;
	actor->collisionFlags &= ~(ACTOR_SOLID | ACTOR_PUSHABLE);
	fdActorSetSpriteFlags(actor, 0);
	fdActorSelectAnimation(actor, ANIM_HURT_LYING);
	fdActorSetSpritePriority(actorId, 3);
	actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
}

/* An event that looks up the fourth floor's four socket pillars each tick and
   does nothing with them. */
static void scVenusLighthouseVisitPillarActors(void)
{
	Uint32 actorId;

	for(actorId = ACTOR_FIRST_SOCKET_PILLAR;
		actorId <= ACTOR_FIRST_SOCKET_PILLAR + 3; actorId++) {
		/* DEAD: each pillar is looked up and dropped; the ROM keeps the
		   calls. */
		fdActorGet(actorId);
	}
}

/* Restores puzzle progress, actors and entrance effects for all six scenes, by
   scene and entrance. */
Sint32 scInit(void)
{
	ACTOR *actor;
	Sint32 sceneId;
	Uint32 pillarIndex;
	Sint32 column;
	Sint32 row;

	fdScriptWait(1);
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION))
		scVenusLighthouseRestoreMarkedColors();
	gmFlagSet(FLAG_IN_VENUS_LIGHTHOUSE);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	sceneId = MainRoutineStateWork.next;
	if(sceneId == SCENE_VENUS_LIGHTHOUSE_ENTRY) {
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		if(!gmFlagGet(FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD))
			scVenusLighthouseLayDownHurtActor(
				ACTOR_SOLDIER_TELLING_OF_THE_ATTACK);
		else
			fdMapGridCopyMetatileAttributes(7, 17, 2, 1, 7, 16);
		scVenusLighthouseLayDownHurtActor(ACTOR_SCHOLAR_LIGHTHOUSE_OPEN);
		scVenusLighthouseLayDownHurtActor(ACTOR_SCHOLAR_BARBARIANS);
		scVenusLighthouseLayDownHurtActor(ACTOR_WAITING_SCHOLAR);
		fdActorSetSpritePriority(ACTOR_WAITING_SCHOLAR, 2);
		scVenusLighthouseLayDownHurtActor(ACTOR_SOLDIER_OPENED_DOOR);
		fdActorSetSpritePriority(ACTOR_SOLDIER_OPENED_DOOR, 2);
		scVenusLighthouseLayDownHurtActor(ACTOR_SOLDIER_HEADED_FOR_BABI);
		scVenusLighthouseLayDownHurtActor(ACTOR_SOLDIER_COLOSSO_VICTORS);
	} else if(sceneId == SCENE_VENUS_LIGHTHOUSE_GROUND_FLOOR) {
		switch(MainRoutineStateWork.entrance) {
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_ENTRY:
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_FROM_RELIEFS:
			scVenusLighthouseLayDownHurtActor(ACTOR_SCHOLAR_ATTACKED_ANYWAY);
			break;
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_WEST:
			gmFlagClear(FLAG_ARRIVAL_STAIRS_UP);
			/* The other scholar setup still applies. fall through */
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_FRONT_ROOM:
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_CARRY_STONE_EAST:
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_FROM_PILLARS:
			scVenusLighthouseLayDownHurtActor(ACTOR_SCHOLAR_NOT_THE_ENTRANCE);
			break;
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_FLOOR_SWITCH:
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_FROM_SMALL_ROOM:
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH)) {
				scVenusLighthouseDrawFirstPath();
			} else {
				if(!gmFlagGet(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
					break;
				scVenusLighthouseDrawSecondPath();
			}
			break;
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_GREAT_STATUE:
			gmFlagClear(FLAG_ARRIVAL_STAIRS_DOWN);
			/* fall through */
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_FROM_PILLARS:
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR:
			gmFlagClear(FLAG_SHOW_AREA_NAME);
			/* The metatiles scVenusLighthouseMovePillarOntoSwitch copies as it
			   sets the flag, redrawn on arrival. */
			if(gmFlagGet(FLAG_SCENE_TEMP_00))
				fdMapGridCopyMetatilesTo(44, 117, 41, 117, 3, 5);
			break;
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_RELIEFS:
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_CARRY_STONE:
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_FLOOR_SWITCH:
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_NORTH:
		case VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_FROM_STATUE_ROW_WEST:
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED)) {
				fdActorSetPos(ACTOR_WALL_PILLAR, 872, 88);
				actor = fdActorGet(ACTOR_WALL_PILLAR);
				actor->y.fixed = -toFIXED(24);
				actor->targetY = ACTOR_NO_TARGET;
			}
			break;
		}
	} else if(sceneId == SCENE_VENUS_LIGHTHOUSE_FIRST_FLOOR) {
		switch(MainRoutineStateWork.entrance) {
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR:
			gmFlagClear(FLAG_SHOW_AREA_NAME);
			break;
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST:
			scVenusLighthouseMoveFirstFloorSandFall();
			break;
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR_HOLE:
			fdQueueMusic(SE_AMBIENT_WATER);
			if(!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
				scVenusLighthouseLandWithRadialDust();
			break;
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_DRIFTING_FLOOR:
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_FROM_LADDERS:
			actor = fdActorGet(ACTOR_FIRST_CARRY_PILLAR);
			scVenusLighthouseMoveCarryPillar();
			if(FIELD_METATILE(actor->x.fixed) == 8)
				scVenusLighthouseFixPillarAtDestination(actor);
			actor = fdActorGet(ACTOR_FIRST_CARRY_PILLAR + 1);
			if(FIELD_METATILE(actor->x.fixed) == 7)
				scVenusLighthouseFixPillarAtDestination(actor);
			scVenusLighthouseCreateBlendedPassageEffect(
				toFIXED(206), 0, toFIXED(449), SPRITE_CRATE);
			scVenusLighthouseCreateBlendedPassageEffect(
				toFIXED(210), 0, toFIXED(449), SPRITE_CRATE);
			break;
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_FROM_LADDERS:
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST:
			fdEffectStartGroundMask(0);
			sySynch(2);
			actor = fdActorGet(ACTOR_FIRST_CAPPED_PILLAR);
			actor->motionFlags = 0;
			actor->update = scActorUpdateBlockDrawOrder;
			actor = fdActorGet(ACTOR_FIRST_CAPPED_PILLAR + 1);
			actor->motionFlags = 0;
			actor->update = scActorUpdateBlockDrawOrder;
			actor = fdActorGet(ACTOR_FIRST_CAPPED_PILLAR + 2);
			actor->motionFlags = 0;
			actor->update = scActorUpdateBlockDrawOrder;
			scVenusLighthouseMoveCappedPillar();
			break;
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_GAP:
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH:
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_THUNDER_CROWN:
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PILLARS:
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_FROM_PSYNERGY_STONE:
			fdQueueMusic(SE_AMBIENT_WATER);
			fdEffectStartGroundMask(0);
			sySynch(2);
			/* With the wall on the floor above moved, the sand that poured down
			   the middle of this room has stopped: the fall is drawn as the dry
			   stone chute it ran down, and the metatiles under it are copied in
			   afresh. */
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
				fdMapGridCopyMetatilesTo(111, 5, 117, 5, 5, 2);
				fdMapGridCopyMetatilesTo(111, 10, 117, 10, 5, 2);
				fdMapGridCopyMetatilesTo(111, 7, 111, 5, 5, 2);
				fdMapGridCopyMetatilesTo(111, 7, 111, 10, 5, 2);
				fdMapGridCopyMetatiles(48, 3, 3, 10, 54, 3);
				fdMapGridCopyMetatiles(55, 26, 3, 10, 48, 3);
			}
			break;
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_LOWER_ROOM:
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_GAP:
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH:
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_FROM_THUNDER_CROWN:
		case VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_FROM_DRIFTING_FLOOR:
			fdQueueMusic(SE_AMBIENT_WATER);
			break;
		}
	} else if(sceneId == SCENE_VENUS_LIGHTHOUSE_SECOND_FLOOR) {
		switch(MainRoutineStateWork.entrance) {
		case VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_FROM_PILLAR_PIT:
			scVenusLighthouseArriveThroughSparkPassage();
			fdQueueMusic(SE_AMBIENT_WATER);
			break;
		case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_EAST:
		case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR:
			fdEffectStartGroundMask(0);
			break;
		case VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_WALL_SWITCH:
		case VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_FROM_PILLAR_PIT:
			actor = fdActorGet(ACTOR_GATE_STATUE);
			actor->motionFlags = 0;
			actor->y.fixed = 0;
			actor = fdActorGet(ACTOR_GATE_PILLAR);
			actor->motionFlags = 0;
			actor->collisionFlags = 0;
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN)) {
				sySynch(1);
				fdMapGridCopyMetatilesTo(124, 41, 110, 41, 1, 2);
				fdMapGridCopyMetatileAttributes(46, 41, 1, 1, 46, 42);
				fdActorSetPos(ACTOR_GATE_PILLAR, 744, 728);
				actor->motionFlags = 0;
				actor->y.fixed = -toFIXED(16);
				fdActorSetSpritePriority(ACTOR_GATE_PILLAR, 3);
				actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
				fdMapGridCopyMetatileAttributes(45, 45, 1, 1, 46, 45);
				fdActorSetAnimation(
					ACTOR_GATE_SPARKLE, PARTICLE_ANIMATION_FLICKER);
				fdActorSetSpritePriority(ACTOR_GATE_SPARKLE, 1);
				actor = fdActorGet(ACTOR_GATE_SPARKLE);
				actor->collisionFlags = 0;
				actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetPos(ACTOR_GATE_SPARKLE, 743, 696);
				actor->update = (void (*)(ACTOR *))scVenusLighthouseTrailLight;
			}
			scVenusLighthouseMoveGatePillar();
			break;
		case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH:
			FieldStateWork->transition =
				SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
			fdActorGet(ACTOR_PARTY_LEADER)->y.fixed = -toFIXED(2);
			/* It then redraws the moved wall as the others do. fall through */
		case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST:
		case VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_FROM_GATE:
			fdQueueMusic(SE_AMBIENT_WATER);
			fdBlendScriptSuspend();
			IO_WRITE16(0, &REG_BLDCNT);
			/* With the wall moved, the stone wall and the fall of sand in the
			   recess have changed sides: the wall stands to the west and the
			   sand pours where it stood, to the east. */
			if(fdScriptGetFlag(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
				fdMapGridCopyMetatilesTo(15, 96, 9, 96, 3, 3);
				fdMapGridCopyMetatilesTo(12, 96, 15, 96, 3, 3);
				fdMapGridCopyMetatilesTo(5, 50, 15, 32, 3, 4);
				fdMapGridCopyMetatilesTo(25, 45, 9, 32, 3, 4);
				fdMapGridCopyMetatileAttributes(15, 32, 3, 1, 9, 32);
				fdMapGridCopyMetatileAttributes(12, 32, 3, 1, 15, 32);
			}
			if(MainRoutineStateWork.entrance !=
				VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH)
				break;
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionTicks();
			FieldStateWork->transition = SCENE_TRANSITION(
				TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
			break;
		case VENUS_LIGHTHOUSE_SECOND_FLOOR_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH:
			fdQueueMusic(SE_AMBIENT_WATER);
			break;
		}
	} else if(sceneId == SCENE_VENUS_LIGHTHOUSE_THIRD_FLOOR) {
		switch(MainRoutineStateWork.entrance) {
		case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST:
			scVenusLighthousePourWestSandSlide();
			break;
		case VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SPARKLING_PASSAGES:
		case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST:
		case VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SOUTHWEST_ROOM:
		case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST:
		case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST:
		case VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_PILLAR_SOCKETS:
		case VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST:
			fdQueueMusic(SE_AMBIENT_WATER);
			/* With the west wall of the floor above moved, sand runs down the
			   dry stone chute west of the middle one; the east wall does the
			   same for the chute to its east. */
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
				fdMapGridCopyMetatiles(53, 12, 3, 13, 26, 12);
				fdMapGridCopyMetatilesTo(81, 41, 89, 14, 9, 2);
				sySynch(1);
				sySetTickEvent(
					scVenusLighthousePuffWestSlideHead, EVENT_PASS_TICK);
			}
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
				fdMapGridCopyMetatiles(58, 12, 3, 13, 34, 12);
				fdMapGridCopyMetatilesTo(81, 41, 97, 14, 5, 2);
				sySynch(1);
				sySetTickEvent(
					scVenusLighthousePuffEastSlideHead, EVENT_PASS_TICK);
			}
			if(MainRoutineStateWork.entrance ==
				VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_FROM_SOUTHWEST_ROOM)
				scVenusLighthouseArriveThroughSparkPassage();
			else if(MainRoutineStateWork.entrance ==
				VENUS_LIGHTHOUSE_THIRD_FLOOR_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST)
				scVenusLighthousePourEastSandSlide();
			break;
		case VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_NORTHWEST_ROOM:
		case VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRAGON_SCALES:
			VENUS_LIGHTHOUSE_ARRIVE_THROUGH_SPARK_PASSAGE();
			fdQueueMusic(SE_AMBIENT_WATER);
			break;
		case VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_DRIFTING_FLOOR:
		case VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_FROM_WEST_ROOM:
			actor = fdActorGet(ACTOR_SWITCH_STATUE);
			actor->motionFlags = 0;
			actor->y.fixed = 0;
			fdActorGet(ACTOR_FIRST_SWITCH_PILLAR)->motionFlags = 0;
			fdActorGet(ACTOR_FIRST_SWITCH_PILLAR + 1)->motionFlags = 0;
			fdActorGet(ACTOR_FIRST_SWITCH_PILLAR + 2)->motionFlags = 0;
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_THREE_PILLAR_GATE_OPEN)) {
				sySynch(1);
				fdMapGridCopyMetatilesTo(111, 59, 109, 37, 1, 2);
				fdMapGridCopyMetatileAttributes(45, 37, 1, 1, 45, 38);
				if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_SOUTHEAST_SOCKET_EMPTY)) {
					fdActorSetPos(ACTOR_FIRST_SWITCH_PILLAR, 776, 664);
					fdActorSetPos(ACTOR_FIRST_SWITCH_PILLAR + 1, 840, 664);
					fdActorSetPos(ACTOR_FIRST_SWITCH_PILLAR + 2, 776, 696);
				} else {
					fdActorSetPos(ACTOR_FIRST_SWITCH_PILLAR, 840, 664);
					fdActorSetPos(ACTOR_FIRST_SWITCH_PILLAR + 1, 776, 696);
					fdActorSetPos(ACTOR_FIRST_SWITCH_PILLAR + 2, 840, 696);
				}
				fdActorSetSpritePriority(ACTOR_FIRST_SWITCH_PILLAR, 3);
				actor = fdActorGet(ACTOR_FIRST_SWITCH_PILLAR);
				actor->y.fixed = -toFIXED(16);
				actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetSpritePriority(ACTOR_FIRST_SWITCH_PILLAR + 1, 3);
				actor = fdActorGet(ACTOR_FIRST_SWITCH_PILLAR + 1);
				actor->y.fixed = -toFIXED(16);
				actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetSpritePriority(ACTOR_FIRST_SWITCH_PILLAR + 2, 3);
				actor = fdActorGet(ACTOR_FIRST_SWITCH_PILLAR + 2);
				actor->y.fixed = -toFIXED(16);
				actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetAnimation(
					ACTOR_SWITCH_GATE_SPARKLE, PARTICLE_ANIMATION_FLICKER);
				fdActorSetSpriteFlags(fdActorGet(ACTOR_SWITCH_GATE_SPARKLE), 0);
				fdActorSetSpritePriority(ACTOR_SWITCH_GATE_SPARKLE, 1);
				actor = fdActorGet(ACTOR_SWITCH_GATE_SPARKLE);
				actor->collisionFlags = 0;
				actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetPos(ACTOR_SWITCH_GATE_SPARKLE, 727, 632);
				actor->update = (void (*)(ACTOR *))scVenusLighthouseTrailLight;
			}
			scVenusLighthouseMoveSwitchPillar();
			break;
		case VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_FROM_DRIFTING_FLOOR:
			fdQueueMusic(SE_AMBIENT_WATER);
			break;
		}
	} else if(sceneId == SCENE_VENUS_LIGHTHOUSE_FOURTH_FLOOR) {
		switch(MainRoutineStateWork.entrance) {
		case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST:
		case VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_FROM_WALL_SWITCHES:
			if(gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
				scVenusLighthouseLowerFloatingPillars(0);
				scActorWaitUntilIdle(fdActorGet(ACTOR_PARTY_LEADER));
				for(pillarIndex = 0; pillarIndex <= 3; pillarIndex++) {
					actor =
						fdActorGet(pillarIndex + ACTOR_FIRST_FLOATING_PILLAR);
					column = FIELD_METATILE(actor->x.fixed);
					if(column == 13) {
						row = FIELD_METATILE(actor->z.fixed);
						if(row == 7 &&
							gmFlagGet(pillarIndex + FLAG_SCENE_TEMP_00)) {
							actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
							actor->collisionFlags = 0;
							actor->motionFlags = 0;
							fdMapGridCopyMetatileAttributes(
								4, 19, 1, 1, column, row);
							break;
						}
					}
				}
			} else {
				actor = fdActorGet(ACTOR_BALANCE_WEST);
				actor->motionFlags = 0;
				actor->y.fixed = -toFIXED(48);
				actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
				actor->collisionFlags &= ~ACTOR_SOLID;
				actor->timer = 3;
				fdActorSetSpritePriority(ACTOR_BALANCE_WEST, 1);
				actor = fdActorGet(ACTOR_BALANCE_EAST);
				actor->motionFlags = 0;
				actor->y.fixed = -toFIXED(48);
				actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
				actor->collisionFlags &= ~ACTOR_SOLID;
				actor->timer = 3;
				fdActorSetSpritePriority(ACTOR_BALANCE_EAST, 1);
				actor = fdActorGet(ACTOR_FIRST_FLOATING_PILLAR);
				actor->motionFlags = 0;
				actor->timer = 0;
				fdActorSetSpriteFlags(
					fdActorGet(ACTOR_FIRST_FLOATING_PILLAR), 0);
				actor = fdActorGet(ACTOR_FIRST_FLOATING_PILLAR + 1);
				actor->motionFlags = 0;
				actor->timer = 0;
				fdActorSetSpriteFlags(
					fdActorGet(ACTOR_FIRST_FLOATING_PILLAR + 1), 0);
				actor = fdActorGet(ACTOR_FIRST_FLOATING_PILLAR + 2);
				actor->motionFlags = 0;
				actor->timer = 0;
				fdActorSetSpriteFlags(
					fdActorGet(ACTOR_FIRST_FLOATING_PILLAR + 2), 0);
				actor = fdActorGet(ACTOR_FIRST_FLOATING_PILLAR + 3);
				actor->motionFlags = 0;
				actor->timer = 0;
				fdActorSetSpriteFlags(
					fdActorGet(ACTOR_FIRST_FLOATING_PILLAR + 3), 0);
			}
			break;
		case VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_WEST:
		case VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_FROM_WALL_SWITCHES_EAST:
			fdQueueMusic(SE_AMBIENT_WATER);
			break;
		case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST:
		case VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST:
			FieldStateWork->transition =
				SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
			fdActorGet(ACTOR_PARTY_LEADER)->y.fixed = -toFIXED(2);
			/* fall through */
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
			fdActorGet(ACTOR_RISING_PLATFORM)->motionFlags = ACTOR_FLOATS;
			fdActorGet(ACTOR_RISING_PLATFORM)->priorityFlags |=
				ACTOR_PRIORITY_UNDERFOOT;
			fdActorGet(ACTOR_RISING_PLATFORM)->y.fixed = -(toFIXED(33) / 2);
			fdBlendScriptSuspend();
			IO_WRITE16(0, &REG_BLDCNT);
			/* With a wall of this room moved, it has slid out of its recess and
			   sand pours from behind it: the west wall here, the east wall
			   below. That sand is what runs down the third floor's chutes. */
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
				fdQueueMusic(SE_AMBIENT_WATER);
				fdMapGridCopyMetatilesTo(36, 81, 32, 81, 3, 2);
				fdMapGridCopyMetatilesTo(36, 83, 36, 81, 3, 2);
				fdMapGridCopyMetatileAttributes(36, 17, 3, 1, 32, 17);
				fdMapGridCopyMetatileAttributes(36, 18, 3, 1, 36, 17);
				fdMapGridCopyMetatilesTo(63, 29, 33, 20, 1, 1);
				fdMapGridCopyMetatilesTo(20, 56, 36, 17, 3, 4);
			}
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
				fdMapGridCopyMetatilesTo(44, 81, 48, 81, 3, 2);
				fdMapGridCopyMetatilesTo(44, 83, 44, 81, 3, 2);
				fdMapGridCopyMetatileAttributes(44, 17, 3, 1, 48, 17);
				fdMapGridCopyMetatileAttributes(44, 18, 3, 1, 44, 17);
				fdMapGridCopyMetatilesTo(63, 29, 49, 20, 1, 1);
				fdMapGridCopyMetatilesTo(41, 56, 44, 17, 3, 4);
			}
			if(MainRoutineStateWork.entrance ==
					VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST ||
				MainRoutineStateWork.entrance ==
					VENUS_LIGHTHOUSE_FOURTH_FLOOR_FROM_VENUS_LIGHTHOUSE_THIRD_FLOOR_EAST_NORTHEAST) {
				fdScriptStartScreenTransition();
				fdScriptWaitTransitionTicks();
				FieldStateWork->transition = SCENE_TRANSITION(
					TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
			}
			if(MainRoutineStateWork.entrance ==
				VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_FROM_PSYNERGY_STONE_RISING)
				scVenusLighthouseRaiseLeaderInSparkles();
			break;
		case VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_SOUTH:
		case VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_FROM_WALL_SWITCHES_EAST:
			sySynch(1);
			sySetTickEvent(scVenusLighthouseVisitPillarActors, EVENT_PASS_TICK);
			actor = fdActorGet(ACTOR_SOCKET_STATUE);
			actor->motionFlags = 0;
			actor->y.fixed = 0;
			fdActorGet(ACTOR_FIRST_SOCKET_PILLAR)->motionFlags = 0;
			fdActorGet(ACTOR_FIRST_SOCKET_PILLAR + 1)->motionFlags = 0;
			fdActorGet(ACTOR_FIRST_SOCKET_PILLAR + 2)->motionFlags = 0;
			fdActorGet(ACTOR_FIRST_SOCKET_PILLAR + 3)->motionFlags = 0;
			if(gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN)) {
				sySynch(1);
				fdMapGridCopyMetatilesTo(95, 56, 77, 35, 1, 2);
				fdMapGridCopyMetatileAttributes(13, 35, 1, 1, 13, 36);
				fdActorSetPos(ACTOR_FIRST_SOCKET_PILLAR, 264, 744);
				actor = fdActorGet(ACTOR_FIRST_SOCKET_PILLAR);
				actor->y.fixed = -toFIXED(16);
				actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR, 3);
				fdActorSetPos(ACTOR_FIRST_SOCKET_PILLAR + 1, 184, 632);
				actor = fdActorGet(ACTOR_FIRST_SOCKET_PILLAR + 1);
				actor->y.fixed = -toFIXED(16);
				actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR + 1, 3);
				fdActorSetPos(ACTOR_FIRST_SOCKET_PILLAR + 2, 232, 696);
				actor = fdActorGet(ACTOR_FIRST_SOCKET_PILLAR + 2);
				actor->y.fixed = -toFIXED(16);
				actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR + 2, 3);
				fdActorSetPos(ACTOR_FIRST_SOCKET_PILLAR + 3, 184, 664);
				actor = fdActorGet(ACTOR_FIRST_SOCKET_PILLAR + 3);
				actor->y.fixed = -toFIXED(16);
				actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetSpritePriority(ACTOR_FIRST_SOCKET_PILLAR + 3, 3);
				fdActorSetAnimation(
					ACTOR_SOCKET_GATE_SPARKLE, PARTICLE_ANIMATION_FLICKER);
				fdActorSetSpriteFlags(fdActorGet(ACTOR_SOCKET_GATE_SPARKLE), 0);
				fdActorSetSpritePriority(ACTOR_SOCKET_GATE_SPARKLE, 1);
				actor = fdActorGet(ACTOR_SOCKET_GATE_SPARKLE);
				actor->collisionFlags = 0;
				actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
				fdActorSetPos(ACTOR_SOCKET_GATE_SPARKLE, 215, 600);
				actor->update = (void (*)(ACTOR *))scVenusLighthouseTrailLight;
			}
			scVenusLighthouseMoveSocketPillar();
			break;
		}
	}
	return 0;
}

/* Isaac steps onto sand flowing north in the fourth floor's sand maze, and it
   carries him north a metatile at a time in puffs of sand. Where it runs over a
   ledge he drops down. */
static void scVenusLighthouseDriftNorth(void)
{
	VECTOR3 args;

	args.x = 0;
	args.z = -1;
	scVenusLighthouseStepActorWithDust(&args);
}

/* Isaac steps onto a slide of flowing sand, and it carries him south a metatile
   at a time in puffs of sand until he reaches firm floor: down the middle of
   the first floor's big room (metatiles (34, 8) to (50, 33)), the third floor's
   slides, and the south-flowing sand of the fourth floor's maze. */
static void scVenusLighthouseDriftSouth(void)
{
	VECTOR3 args;

	args.x = 0;
	args.z = 1;
	scVenusLighthouseStepActorWithDust(&args);
}

/* Isaac steps onto sand flowing west in the fourth floor's sand maze, and it
   carries him west a metatile at a time in puffs of sand. */
static void scVenusLighthouseDriftWest(void)
{
	VECTOR3 args;

	args.x = -1;
	args.z = 0;
	scVenusLighthouseStepActorWithDust(&args);
}

/* Isaac steps onto sand flowing east in the fourth floor's sand maze, and it
   carries him east a metatile at a time in puffs of sand. */
static void scVenusLighthouseDriftEast(void)
{
	VECTOR3 args;

	args.x = 1;
	args.z = 0;
	scVenusLighthouseStepActorWithDust(&args);
}

/* Carries the leader along on flowing sand, a little at a time, kicking up
   puffs of sand, and stops him at a wall or drops him down a ledge. direction
   is the way the drifting floor carries the party: -1, 0 or 1 on x and on z; y
   is not read. */
static void scVenusLighthouseStepActorWithDust(VECTOR3 *direction)
{
	Sint32		   actorId;
	ACTOR		  *viewCenter;
	ACTOR		  *actor;
	FIELD_POS	   nextPos;
	EFFECT_OPTIONS options;
	Sint32		   collision;
	Sint32		   framePhase;
	FIXED		   particleX;

	{
		FIELD_STATE *state = FieldStateWork;

		actorId = MainRoutineStateWork.leaderActorId;
		viewCenter = state->camera;
	}
	actor = fdActorGet(actorId);
	nextPos.x =
		actor->x.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
	nextPos.y = actor->y.fixed;
	nextPos.z =
		actor->z.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
	collision = syFieldActorCheckMovementCollision(actor, &nextPos);
	framePhase = TickCount & 4;
	if(framePhase == 0) {
		options.spin = FIELD_EFFECT_RANDOM_SPIN();
		particleX = actor->x.fixed + toFIXED(syRandomBelow(12)) - toFIXED(6);
		fdEffectSpawn(particleX, actor->y.fixed, actor->z.fixed,
			((syRandomBelow(5)) * (toFIXED(1) / 10) + 5 * (toFIXED(1) / 10)) *
				direction->x,
			framePhase,
			direction->z *
				((syRandomBelow(5)) * (toFIXED(1) / 10) +
					5 * (toFIXED(1) / 10)),
			EFFECT_USE_SPIN, &options);
	}
	/* Where the sand runs over a ledge he drops down it; a wall beside him
	   nudges him along it. */
	if(collision < 0) {
		fdActorSetAttachedEffect(
			actorId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		syFieldActorSetMoveTargetFX(
			actor, actor->x.fixed, actor->y.fixed, actor->z.fixed + toFIXED(8));
		fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
		fdActorWaitForMove(actor);
		do {
			sySynch(1);
		} while(actor->y.fixed != actor->groundHeight);
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		sySynch(3);
		return;
	}
	nextPos.x =
		actor->x.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x, 19);
	nextPos.y = actor->y.fixed;
	nextPos.z =
		actor->z.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z, 19);
	collision = syFieldActorCheckMovementCollision(actor, &nextPos);
	if(collision > 0) {
		return;
	}
	nextPos.x = actor->x.fixed + direction->x * (toFIXED(57) / 10) -
		direction->z * (toFIXED(57) / 10);
	nextPos.y = actor->y.fixed;
	nextPos.z = actor->z.fixed + direction->z * (toFIXED(57) / 10) -
		direction->x * (toFIXED(57) / 10);
	collision = syFieldActorCheckMovementCollision(actor, &nextPos);
	if(collision > 0) {
		actor->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
		actor->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
		return;
	}
	nextPos.x =
		(direction->x + direction->z) * (toFIXED(57) / 10) + actor->x.fixed;
	nextPos.y = actor->y.fixed;
	nextPos.z =
		(direction->x + direction->z) * (toFIXED(57) / 10) + actor->z.fixed;
	collision = syFieldActorCheckMovementCollision(actor, &nextPos);
	if(collision > 0) {
		actor->x.fixed -= VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
		actor->z.fixed -= VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
		return;
	}
	viewCenter->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
	actor->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
	viewCenter->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
	actor->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
}

/* Chooses the size and spin of a burst of sand as a slide starts to pour. */
static inline void scVenusLighthouseSetSandParticleScaleAndSpin(
	EFFECT_OPTIONS *options)
{
	options->startScaleX =
		(syRandomBelow(2)) * (toFIXED(3) / 10) + 5 * (toFIXED(3) / 10);
	options->startScaleY =
		(syRandomBelow(2)) * (toFIXED(3) / 10) + 5 * (toFIXED(3) / 10);
	options->spin = FIELD_EFFECT_RANDOM_SPIN();
}

/* After the second floor's switch, the view cuts to the first floor's room of
   three doors with the party out of sight. A wave of sand pours down a new fall
   east of the old one, row by row, and then the old fall runs dry from the top
   down, leaving the stone chute it ran down. The party is then back on the
   second floor beside the switch.

   The return is by
   VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH. */
static void scVenusLighthouseMoveFirstFloorSandFall(void)
{
	EFFECT_OPTIONS options;
	Uint32		   extraBursts;
	Uint32		   row;
	Uint32		   particleIndex;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptStart();
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdQueueMusic(SE_AMBIENT_WATER);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	sySoundPlay(SE_WAVE);
	extraBursts = 0;
	for(row = 0; row < 10; row++) {
		scVenusLighthouseSetSandParticleScaleAndSpin(&options);
		for(;;) {
			for(particleIndex = 0; particleIndex < 4 && row < 8;
				particleIndex++) {
				fdEffectSpawn(((syRandomBelow(7)) << 19) + toFIXED(864), 0,
					toFIXED(48) + row * toFIXED(16) +
						particleIndex * toFIXED(4),
					0, 0, 0, EFFECT_USE_START_SCALE | EFFECT_USE_SPIN,
					&options);
			}
			sySynch(3);
			if(row != 3 || extraBursts >= 3) {
				break;
			}
			extraBursts++;
		}
		fdMapGridCopyMetatilesTo(48, row + 3, 54, row + 3, 3, 1);
	}
	fdMapGridCopyMetatilesTo(111, 5, 117, 5, 5, 2);
	fdMapGridCopyMetatilesTo(111, 10, 117, 10, 5, 2);
	fdMapGridCopyMetatilesTo(111, 7, 111, 5, 5, 2);
	fdMapGridCopyMetatilesTo(111, 7, 111, 10, 5, 2);
	for(row = 0; row < 10; row++) {
		scVenusLighthouseSetSandParticleScaleAndSpin(&options);
		for(particleIndex = 0; particleIndex < 4 && row < 8; particleIndex++) {
			fdEffectSpawn(((syRandomBelow(7)) << 19) + toFIXED(768), 0,
				toFIXED(48) + row * toFIXED(16) + particleIndex * toFIXED(4), 0,
				0, 0, EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
		}
		sySynch(3);
		fdMapGridCopyMetatilesTo(55, row + 26, 48, row + 3, 3, 1);
	}
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	fdScriptWait(60);
	syImportScriptSetExitRequest(
		VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_SOUTH);
	fdScriptEnd();
}

/* Puffs of sand at the head of a sand slide that has started to pour, about one
   tick in four. */
static void scVenusLighthousePuffSandAtSlideHead(Sint32 x, Sint32 z)
{
	EFFECT_OPTIONS options;

	options.startScaleX = toFIXED(7) / 10;
	options.startScaleY = toFIXED(7) / 10;
	options.spin = FIELD_EFFECT_RANDOM_SPIN();
	if((TickCount & 3) == 0) {
		if((syRandomBelow(2)) != 0) {
			fdEffectSpawn(toFIXED(x + (syRandomBelow(2) << 4)), 0,
				FIELD_HALF_METATILE_ORIGIN(z), 0, 0,
				(Sint32)(toFIXED(syRandomBelow(5)) + toFIXED(7)) / 10,
				EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
		} else {
#if FIX_SAND_PUFF_ROW
			fdEffectSpawn(toFIXED(x + syRandomBelow(17)), 0,
				FIELD_HALF_METATILE_ORIGIN(z) - toFIXED(4), 0, 0, 0,
				EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
#else
			/* BUG: takes x for the half-metatile row, so these particles start
			   far to the south of the passage instead of just north of the
			   others. */
			fdEffectSpawn(toFIXED(x + syRandomBelow(17)), 0,
				FIELD_HALF_METATILE_ORIGIN(x) - toFIXED(4), 0, 0, 0,
				EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
#endif
		}
	}
}

/* Events, each tick once a slide pours: puffs of sand at the head of the west
   slide, and of the east. */
static void scVenusLighthousePuffWestSlideHead(void)
{
	scVenusLighthousePuffSandAtSlideHead(432, 32);
}

/* The east slide's puffs. */
static void scVenusLighthousePuffEastSlideHead(void)
{
	scVenusLighthousePuffSandAtSlideHead(560, 44);
}

/* After the fourth floor's west switch, the view cuts to the third floor's room
   of three sand slides, where a wave of sand pours down the west slide, row by
   row, and keeps puffing at its head. The party is then back on the fourth
   floor.

   The return is by
   VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST. */
static void scVenusLighthousePourWestSandSlide(void)
{
	EFFECT_OPTIONS options;
	Uint32		   extraBursts;
	Uint32		   row;
	Uint32		   particleIndex;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptStart();
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdQueueMusic(SE_AMBIENT_WATER);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	sySoundPlay(SE_WAVE);
	fdCameraSetSpeed(0.5);
	fdCameraMoveTo(440, CAMERA_KEEP, 360, 1);
	/* The wave holds on its fourth row for three more bursts, and the slide's
	   head starts to puff sand. */
	extraBursts = 0;
	for(row = 0; row <= 12; row++) {
		scVenusLighthouseSetSandParticleScaleAndSpin(&options);
		/* One burst of sand: four puffs down the row, each over a random one of
		   seven 8-pixel columns. */
	burst:
		for(particleIndex = 0; particleIndex <= 3; particleIndex++) {
			fdEffectSpawn(((syRandomBelow(7)) << 19) + toFIXED(416), 0,
				toFIXED(192) + row * toFIXED(16) + particleIndex * toFIXED(4),
				0, 0, 0, EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
		}
		sySynch(3);
		if(row == 3) {
			if(extraBursts <= 2) {
				extraBursts++;
				goto burst;
			}
			if(extraBursts == 3)
				sySetTickEvent(
					scVenusLighthousePuffWestSlideHead, EVENT_PASS_TICK);
		}
		fdMapGridCopyMetatilesTo(53, row + 12, 26, row + 12, 3, 1);
	}
	fdMapGridCopyMetatilesTo(81, 41, 89, 14, 9, 2);
	fdCameraWaitForMove();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdScriptWait(60);
	gmFlagSet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED);
	syImportScriptSetExitRequest(
		VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHWEST);
	fdScriptEnd();
}

/* After the fourth floor's east switch, sand pours down the third floor's east
   slide in the same way (scVenusLighthousePourWestSandSlide).

   The return is by
   VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST.
 */
static void scVenusLighthousePourEastSandSlide(void)
{
	EFFECT_OPTIONS options;
	Uint32		   extraBursts;
	Uint32		   row;
	Uint32		   particleIndex;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptStart();
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdQueueMusic(SE_AMBIENT_WATER);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	sySoundPlay(SE_WAVE);
	fdCameraSetSpeed(0.5);
	fdCameraMoveTo(568, CAMERA_KEEP, 360, 1);
	/* As the western wave, but its puffing event is added again on every
	   row. */
	extraBursts = 0;
	for(row = 0; row <= 12; row++) {
		scVenusLighthouseSetSandParticleScaleAndSpin(&options);
	burst:
		for(particleIndex = 0; particleIndex <= 3; particleIndex++) {
			fdEffectSpawn(((syRandomBelow(7)) << 19) + toFIXED(544), 0,
				toFIXED(192) + row * toFIXED(16) + particleIndex * toFIXED(4),
				0, 0, 0, EFFECT_USE_START_SCALE | EFFECT_USE_SPIN, &options);
		}
		sySynch(3);
		if(row == 3 && extraBursts <= 2) {
			extraBursts++;
			goto burst;
		}
		sySetTickEvent(scVenusLighthousePuffEastSlideHead, EVENT_PASS_TICK);
		fdMapGridCopyMetatilesTo(58, row + 12, 34, row + 12, 3, 1);
	}
	fdMapGridCopyMetatilesTo(86, 41, 97, 14, 5, 2);
	fdCameraWaitForMove();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdScriptWait(60);
	gmFlagSet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED);
	syImportScriptSetExitRequest(
		VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_EAST_NORTHEAST);
	fdScriptEnd();
}

/* Sparkles rise around the leader as the platform lifts him, one every four
   ticks. */
static UNREAD_RESULT scVenusLighthouseEmitRisingSparkles(ACTOR *actor)
{
	EFFECT_OPTIONS options;
	Sint32		   framePhase;
	FIXED		   particleX, particleY, particleZ, riseSpeed;

	options.priority = 1;
	options.palette = SPRITE_COLOR_BRG;
	options.type = SPRITE_GLINT;
	options.script = GlintTwinkleScript;
	framePhase = TickCount & 3;
	if(framePhase == 0) {
		if((TickCount & 7) == 0) {
			sySoundPlay(SE_GLITTER);
		}
		particleX = actor->x.fixed + toFIXED(syRandomBelow(49) - 24);
		particleY = actor->y.fixed + toFIXED(syRandomBelow(49) - 24);
		particleZ = actor->z.fixed + toFIXED(syRandomBelow(49) - 24);
		riseSpeed = ((syRandomBelow(4)) << 15) + toFIXED(1) / 2;
		fdEffectSpawn(particleX, particleY, particleZ, 0, riseSpeed, framePhase,
			EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_TYPE |
				EFFECT_USE_SCRIPT,
			&options);
	}
	RETURN_UNREAD(0);
}

/* After the drop through the gap, Isaac lands on a small round platform
   (ACTOR_RISING_PLATFORM), which rises with him in a column of sparkles until
   the scene changes to the Lighthouse Aerie. */
static void scVenusLighthouseRaiseLeaderInSparkles(void)
{
	Uint32 riseFrame;
	ACTOR *leader;
	ACTOR *platform;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	platform = fdActorGet(ACTOR_RISING_PLATFORM);
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	/* He lands on the platform, which dips under him and settles, carrying him
	   with it. */
	leader->y.fixed = toFIXED(130);
	leader->gravity = toFIXED(1) / 2;
	leader->bounce = 0;
	leader->motionFlags = 0;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	sySoundPlay(SE_DUST_PUFF);
	fdScriptWait(30);
	leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdScriptWait(24);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
	leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
	platform->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
	leader->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
	leader->groundHeight += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
	sySynch(2);
	platform->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
	leader->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
	leader->groundHeight += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
	sySynch(10);
	platform->y.fixed += toFIXED(2);
	leader->y.fixed += toFIXED(2);
	leader->groundHeight += toFIXED(2);
	sySynch(4);
	platform->y.fixed += toFIXED(2);
	leader->y.fixed += toFIXED(2);
	leader->groundHeight += toFIXED(2);
	sySynch(4);
	platform->y.fixed += toFIXED(1);
	leader->y.fixed += toFIXED(1);
	leader->groundHeight += toFIXED(1);
	leader->motionFlags = 0;
	platform->motionFlags = 0;
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdScriptWait(40);
#if FIX_FUNCTION_SIGNATURES
	leader->update = scVenusLighthouseEmitRisingSparkles;
#else
	leader->update = (void *)scVenusLighthouseEmitRisingSparkles;
#endif
	fdScriptWait(60);
	fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
	fdActorSetSpritePriority(ACTOR_RISING_PLATFORM, 1);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	sySoundPlay(SE_MATERIALIZE);
	gmFlagSet(FLAG_SKIP_SCENE_CHANGE_SOUND);
	for(riseFrame = 0; riseFrame < 128; riseFrame++) {
		leader->y.fixed += toFIXED(1);
		leader->groundHeight += toFIXED(1);
		platform->y.fixed += toFIXED(1);
		sySynch(1);
	}
	syImportScriptSetExitRequest(
		VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_AERIE);
}

/* A light reaching a socket runs on along the track of the pillar sunk there,
   by its socket's path for that piece, or goes out when the piece does not join
   the edge it comes in by. */
static Sint32 scVenusLighthouseRouteLight(ACTOR *light)
{
	FIELD_POS	 pillarPos;
	ACTOR		*pillar;
	SPR			*sprite;
	const Uint8 *allowedTracks;
	Uint8		 track;
	Uint32		 socket;
	Sint32		 pathIndex;
	const Uint8 *trackCursor;

	pillarPos.x = light->x.fixed;
	pillarPos.y = light->y.fixed - toFIXED(16);
	pillarPos.z = light->z.fixed;
	pillar = scActorFindAtPos(&pillarPos, 0);
	/* A light does reach an empty socket: on the third floor one solution, the
	   one FLAG_VENUS_LIGHTHOUSE_SOUTHEAST_SOCKET_EMPTY records, leaves that
	   socket empty, and the light sent there always arrives. With no pillar the
	   reads below go through address 0, into the BIOS, and that light goes out
	   (VenusLightGoesOut) while the other opens the gate. */
	sprite = pillar->sprite;
	if(sprite->entries[0]->animationId == SPRITE_SPIRAL_PILLAR) {
		Sint32 velocityX = light->velocityX;
		Sint32 speedX = velocityX < 0 ? -velocityX : velocityX;
		Sint32 velocityZ = light->velocityZ;
		Sint32 speedZ = velocityZ < 0 ? -velocityZ : velocityZ;

		if(speedX > speedZ) {
			if(velocityX / toFIXED(1) < 0) {
				allowedTracks = VenusTracksMovingWest;
			} else {
				allowedTracks = VenusTracksMovingEast;
			}
		} else {
			if(velocityZ / toFIXED(1) < 0) {
				allowedTracks = VenusTracksMovingNorth;
			} else {
				allowedTracks = VenusTracksMovingSouth;
			}
		}
		for(; *allowedTracks != VENUS_TRACK_END; allowedTracks++) {
			if(sprite->lastNo == *allowedTracks) {
				break;
			}
		}
		if(*allowedTracks == VENUS_TRACK_END) {
			fdActorStartScript(light, VenusLightGoesOut);
			return 0;
		}
		if(MainRoutineStateWork.next == SCENE_VENUS_LIGHTHOUSE_THIRD_FLOOR) {
			for(socket = 0; socket <= 3 &&
				!(FIELD_METATILE(light->x.fixed) ==
						VenusThirdFloorSockets[socket].x &&
					FIELD_METATILE(light->z.fixed) ==
						VenusThirdFloorSockets[socket].z);
				socket++) {
			}
			for(pathIndex = 0;; pathIndex++) {
				trackCursor = VenusThirdFloorSocketTracks[socket];
				track = *trackCursor;
				if(track == VENUS_TRACK_END) {
					fdActorStartScript(light, VenusLightGoesOut);
					return 0;
				}
				if(track == pillar->sprite->lastNo) {
					break;
				}
				VenusThirdFloorSocketTracks[socket] = trackCursor + 1;
			}
			fdActorStartScript(
				light, VenusThirdFloorSocketPaths[socket][pathIndex]);
		} else {
			for(socket = 0; socket <= 7 &&
				!(FIELD_METATILE(light->x.fixed) ==
						VenusFourthFloorSockets[socket].x &&
					FIELD_METATILE(light->z.fixed) ==
						VenusFourthFloorSockets[socket].z);
				socket++) {
			}
			for(pathIndex = 0;; pathIndex++) {
				trackCursor = VenusFourthFloorSocketTracks[socket];
				track = *trackCursor;
				if(track == VENUS_TRACK_END) {
					fdActorStartScript(light, VenusLightGoesOut);
					return 0;
				}
				if(track == pillar->sprite->lastNo) {
					break;
				}
				VenusFourthFloorSocketTracks[socket] = trackCursor + 1;
			}
			fdActorStartScript(
				light, VenusFourthFloorSocketPaths[socket][pathIndex]);
		}
	} else {
		fdActorStartScript(light, VenusLightGoesOut);
	}
	return 0;
}

#include "shared/field_effect_duration_scripts.c"

#include "shared/pushed_actor_direction_steps.c"

/* The third floor's four sockets, by metatile: northwest, northeast, southwest
   and southeast. */
SWITCH_METATILE VenusThirdFloorSockets[] = {
	{48, 41},
	{52, 41},
	{48, 43},
	{52, 43},
};

/* clang-format off */
#include "shared/balance_heights.c"

/* The fourth floor's eight sockets, by metatile: northwest, northeast, west, */
/* east, far west, middle, southwest and southeast. */
SWITCH_METATILE VenusFourthFloorSockets[] = {
	{ 11, 39 },
	{ 14, 39 },
	{ 11, 41 },
	{ 16, 42 },
	{ 10, 43 },
	{ 14, 43 },
	{ 10, 46 },
	{ 16, 46 },
};

/* The pieces of track a light can run onto in each direction, each list */
/* ended by VENUS_TRACK_END (scVenusLighthouseRouteLight). */
Uint8 VenusTracksMovingWest[] = {
	VENUS_TRACK_NORTH_EAST, VENUS_TRACK_WEST_EAST, VENUS_TRACK_SOUTH_EAST, VENUS_TRACK_END,
};
Uint8 VenusTracksMovingEast[] = {
	VENUS_TRACK_NORTH_WEST, VENUS_TRACK_WEST_EAST, VENUS_TRACK_SOUTH_WEST, VENUS_TRACK_END,
};
Uint8 VenusTracksMovingNorth[] = {
	VENUS_TRACK_NORTH_SOUTH, VENUS_TRACK_SOUTH_EAST, VENUS_TRACK_SOUTH_WEST, VENUS_TRACK_END,
};
Uint8 VenusTracksMovingSouth[] = {
	VENUS_TRACK_NORTH_SOUTH, VENUS_TRACK_NORTH_WEST, VENUS_TRACK_NORTH_EAST, VENUS_TRACK_END,
};

/* The third floor's sockets' lists of the pieces they route, in the order of */
/* each socket's paths (VenusThirdFloorSocketTracks points into it). */
Uint8 VenusThirdFloorTrackLists[] = {
	VENUS_TRACK_WEST_EAST,  VENUS_TRACK_SOUTH_EAST, VENUS_TRACK_SOUTH_WEST, VENUS_TRACK_END, /* northwest */
	VENUS_TRACK_WEST_EAST,  VENUS_TRACK_SOUTH_EAST, VENUS_TRACK_SOUTH_WEST, VENUS_TRACK_END, /* northeast */
	VENUS_TRACK_NORTH_WEST, VENUS_TRACK_NORTH_EAST, VENUS_TRACK_WEST_EAST,  VENUS_TRACK_END, /* southwest */
	VENUS_TRACK_NORTH_WEST, VENUS_TRACK_NORTH_EAST, VENUS_TRACK_WEST_EAST,  VENUS_TRACK_END, /* southeast */
};

/* The fourth floor's sockets' lists, as VenusThirdFloorTrackLists. Each */
/* list ends where the next begins, and the northeast socket's is empty. */
Uint8 VenusFourthFloorTrackLists[] = {
	VENUS_TRACK_SOUTH_EAST, VENUS_TRACK_END, /* northwest */
	VENUS_TRACK_END,                   /* northeast */
	VENUS_TRACK_NORTH_EAST, VENUS_TRACK_END, /* west */
	VENUS_TRACK_SOUTH_WEST, VENUS_TRACK_END, /* east */
	VENUS_TRACK_NORTH_EAST, VENUS_TRACK_END, /* far west */
	/* middle */
	VENUS_TRACK_SOUTH_EAST, VENUS_TRACK_SOUTH_WEST, VENUS_TRACK_END,
	VENUS_TRACK_NORTH_SOUTH, VENUS_TRACK_END, /* southwest */
	VENUS_TRACK_NORTH_SOUTH, VENUS_TRACK_END, /* southeast */
};

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

/* The lady statues' actor script. They are drawn grey (VENUS_COLOR_MARK), */
/* and never pushed (SW_VENUS_IMMOVABLE). */
Sint32 VenusGreyStatueScript[] = {
	SC_SET(SW_TIMER, VENUS_COLOR_MARK(SPRITE_COLOR_GREY)),
	SC_SET(SW_VENUS_IMMOVABLE, 1),
	SC_CALL(scVenusLighthouseApplyMarkedColor),
	SC_END,
};

/* The first floor's capped pillars' actor script: drawn grey. */
Sint32 VenusGreyPillarScript[] = {
	SC_SET(SW_TIMER, VENUS_COLOR_MARK(SPRITE_COLOR_GREY)),
	SC_CALL(scVenusLighthouseApplyMarkedColor),
	SC_END,
};

/* The leader's actor script as the lightning strikes him */
/* (scVenusLighthouseThrowBackFromLightning). He swells and shrinks ten */
/* times, flashing white and crackling (scVenusLighthouseFlashStruckLeader). */
Sint32 VenusLeaderShockedScript[] = {
	/* Seen from behind, his arms flung wide. */
	SC_ANIMATION(ANIM_LEADER_CLING),
	SC_SET(SW_HOOK, (Sint32)scVenusLighthouseFlashStruckLeader),
	SC_LABEL(1),
	SC_SET(SW_SCALE_X, toFIXED(11) / 10),
	SC_SET(SW_SCALE_Y, toFIXED(11) / 10),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(9) / 10),
	SC_SET(SW_SCALE_Y, toFIXED(9) / 10),
	SC_WAIT(2),
	SC_LOOP(10, SC_TO(1)),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_END,
};

/* The actor script of the World Map's picture of Venus Lighthouse, placed */
/* in the ground floor's statue room at three fifths of its size, with no */
/* collision. It is a small purple tower standing among the floor lamps the */
/* statue's routes light (the room's render). */
Sint32 VenusMiniatureLighthouseScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(3) / 5),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 5),
	SC_SET(SW_COLLISION, 0),
	SC_CALL(scVenusLighthouseClearScriptedSpriteFlags),
	SC_END,
};

#include "shared/glint_twinkle_script.c"

/* The actor scripts that give a spiral pillar its piece of track. */
Sint32 VenusTrackNorthSouthScript[] = {
	SC_ANIMATION(VENUS_TRACK_NORTH_SOUTH),
	SC_END,
};

Sint32 VenusTrackNorthWestScript[] = {
	SC_ANIMATION(VENUS_TRACK_NORTH_WEST),
	SC_END,
};

Sint32 VenusTrackNorthEastScript[] = {
	SC_ANIMATION(VENUS_TRACK_NORTH_EAST),
	SC_END,
};

Sint32 VenusTrackWestEastScript[] = {
	SC_ANIMATION(VENUS_TRACK_WEST_EAST),
	SC_END,
};

Sint32 VenusTrackSouthEastScript[] = {
	SC_ANIMATION(VENUS_TRACK_SOUTH_EAST),
	SC_END,
};

Sint32 VenusTrackSouthWestScript[] = {
	SC_ANIMATION(VENUS_TRACK_SOUTH_WEST),
	SC_END,
};

/* The second floor's light. It runs from the lady statue along the track, */
/* over the sunk gate pillar, to the gate (scVenusLighthouseTrailLight trails */
/* it). */
Sint32 VenusSecondFloorLightToGate[] = {
	SC_CALL(scVenusLighthouseClearScriptedSpriteFlags),
	SC_ANIMATION(PARTICLE_ANIMATION_FLICKER),
	SC_SPEED(1),
	SC_SET(SW_HOOK, (Sint32)scVenusLighthouseTrailLight),
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

/* The third floor's lights, from the lady statue to a socket, from socket to */
/* socket, and from a socket to the gate, where SW_VENUS_LIGHT_ARRIVED opens */
/* it. Each that reaches a socket asks scVenusLighthouseRouteLight for its */
/* next path. */
Sint32 VenusThirdFloorLightToNortheast[] = {
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
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

Sint32 VenusThirdFloorLightToSoutheast[] = {
	SC_MOVE(888, 0, 696),
	SC_SYNC,
	SC_POSITION(888, 0, 696),
	SC_MOVE(840, 0, 696),
	SC_SYNC,
	SC_SET(SW_X, toFIXED(840)),
	SC_SET(SW_Z, toFIXED(696)),
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

/* A light that cannot go on. It stops trailing particles and is released ten */
/* ticks later. */
Sint32 VenusLightGoesOut[] = {
	SC_SET(SW_HOOK, 0),
	SC_WAIT(10),
	SC_RELEASE,
};

Sint32 VenusThirdFloorLightNorthwestToGate[] = {
	SC_MOVE(728, 0, 664),
	SC_SYNC,
	SC_POSITION(728, 0, 664),
	SC_MOVE(728, 0, 632),
	SC_SYNC,
	SC_POSITION(728, 0, 632),
	SC_SET(SW_VENUS_LIGHT_ARRIVED, 1),
	SC_END,
};

Sint32 VenusThirdFloorLightNorthwestToSouthwest[] = {
	SC_MOVE(776, 0, 696),
	SC_SYNC,
	SC_SET(SW_X, toFIXED(776)),
	SC_SET(SW_Z, toFIXED(696)),
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

Sint32 VenusThirdFloorLightNortheastToNorthwest[] = {
	SC_MOVE(776, 0, 664),
	SC_SYNC,
	SC_SET(SW_X, toFIXED(776)),
	SC_SET(SW_Z, toFIXED(664)),
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

Sint32 VenusThirdFloorLightNortheastToSoutheast[] = {
	SC_MOVE(840, 0, 696),
	SC_SYNC,
	SC_SET(SW_X, toFIXED(840)),
	SC_SET(SW_Z, toFIXED(696)),
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

/* The same script as VenusThirdFloorLightNortheastToNorthwest, */
/* under another name. */
Sint32 VenusThirdFloorLightSouthwestToNorthwest[] = {
	SC_MOVE(776, 0, 664),
	SC_SYNC,
	SC_SET(SW_X, toFIXED(776)),
	SC_SET(SW_Z, toFIXED(664)),
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

Sint32 VenusThirdFloorLightSouthwestToGate[] = {
	SC_MOVE(728, 0, 696),
	SC_SYNC,
	SC_POSITION(728, 0, 696),
	SC_MOVE(728, 0, 632),
	SC_SYNC,
	SC_POSITION(728, 0, 632),
	SC_SET(SW_VENUS_LIGHT_ARRIVED, 1),
	SC_END,
};

Sint32 VenusThirdFloorLightSoutheastToNortheast[] = {
	SC_MOVE(840, 0, 664),
	SC_SYNC,
	SC_SET(SW_X, toFIXED(840)),
	SC_SET(SW_Z, toFIXED(664)),
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

/* The same script as VenusThirdFloorLightNorthwestToSouthwest, */
/* under another name. */
Sint32 VenusThirdFloorLightSoutheastToSouthwest[] = {
	SC_MOVE(776, 0, 696),
	SC_SYNC,
	SC_SET(SW_X, toFIXED(776)),
	SC_SET(SW_Z, toFIXED(696)),
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

/* The fourth floor's lights, as the third floor's. The first runs from the */
/* statue to the fork, where scVenusLighthouseMoveSocketPillar sends it on to */
/* the southeast socket and a second light to the southwest one. */
Sint32 VenusFourthFloorLightFromStatue[] = {
	SC_MOVE(216, 0, 776),
	SC_SYNC,
	SC_POSITION(216, 0, 776),
	SC_MOVE(216, 0, 760),
	SC_SYNC,
	SC_POSITION(216, 0, 760),
	SC_END,
};

Sint32 VenusFourthFloorLightToSoutheast[] = {
	SC_MOVE(264, 0, 760),
	SC_SYNC,
	SC_POSITION(264, 0, 760),
	SC_MOVE(264, 0, 744),
	SC_SYNC,
	SC_SET(SW_X, toFIXED(264)),
	SC_SET(SW_Z, toFIXED(744)),
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

Sint32 VenusFourthFloorLightSoutheastToMiddle[] = {
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
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

Sint32 VenusFourthFloorLightMiddleToWest[] = {
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
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

Sint32 VenusFourthFloorLightMiddleToEast[] = {
	SC_MOVE(264, 0, 696),
	SC_SYNC,
	SC_POSITION(264, 0, 696),
	SC_MOVE(264, 0, 680),
	SC_SYNC,
	SC_SET(SW_X, toFIXED(264)),
	SC_SET(SW_Z, toFIXED(680)),
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

Sint32 VenusFourthFloorLightWestToNorthwest[] = {
	SC_MOVE(184, 0, 632),
	SC_SYNC,
	SC_SET(SW_X, toFIXED(184)),
	SC_SET(SW_Z, toFIXED(632)),
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

Sint32 VenusFourthFloorLightEastToNortheast[] = {
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
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

Sint32 VenusFourthFloorLightToSouthwest[] = {
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
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

Sint32 VenusFourthFloorLightSouthwestToFarWest[] = {
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
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

Sint32 VenusFourthFloorLightFarWestToWest[] = {
	SC_MOVE(168, 0, 664),
	SC_SYNC,
	SC_POSITION(168, 0, 664),
	SC_MOVE(184, 0, 664),
	SC_SYNC,
	SC_SET(SW_X, toFIXED(184)),
	SC_SET(SW_Z, toFIXED(664)),
	SC_CALL(scVenusLighthouseRouteLight),
	SC_END,
};

Sint32 VenusFourthFloorLightNorthwestToGate[] = {
	SC_MOVE(216, 0, 632),
	SC_SYNC,
	SC_POSITION(216, 0, 632),
	SC_MOVE(216, 0, 600),
	SC_SYNC,
	SC_POSITION(216, 0, 600),
	SC_SET(SW_VENUS_LIGHT_ARRIVED, 1),
	SC_END,
};

/* The gates' metatile animations as each opens, a metatile column at a time. */
Uint16 VenusSecondFloorGateMetatiles[] = {
	MAPGRID_ANIMATION_STEP(124, 50, 1, 2, 6), MAPGRID_ANIMATION_STEP(124, 47, 1, 2, 6), MAPGRID_ANIMATION_STEP(124, 44, 1, 2, 6), MAPGRID_ANIMATION_STEP(124, 41, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 VenusThirdFloorGateMetatiles[] = {
	MAPGRID_ANIMATION_STEP(117, 59, 1, 2, 6), MAPGRID_ANIMATION_STEP(115, 59, 1, 2, 6), MAPGRID_ANIMATION_STEP(113, 59, 1, 2, 6), MAPGRID_ANIMATION_STEP(111, 59, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 VenusFourthFloorGateMetatiles[] = {
	MAPGRID_ANIMATION_STEP(96, 58, 1, 2, 6), MAPGRID_ANIMATION_STEP(95, 58, 1, 2, 6), MAPGRID_ANIMATION_STEP(96, 56, 1, 2, 6), MAPGRID_ANIMATION_STEP(95, 56, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};

SCENE_ENTRANCE VenusLighthouseEntryEntrances[] = {
	{ VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 40, 0, 264, FACING_EAST, 0, 0, 0, 512, 544, 0 },
	{ VENUS_LIGHTHOUSE_ENTRY_FROM_VENUS_LIGHTHOUSE_GROUND_FLOOR, CONDITION_ALWAYS, 376, 0, 88, FACING_SOUTH, 0, 0, 0, 512, 544, 0 },
	{ VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 488, 0, 424, FACING_WEST, 0, 0, 0, 512, 544, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE VenusLighthouseEntrancesOther[] = {
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

SCENE_ENTRANCE VenusLighthouseFirstFloorEntrances[] = {
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

SCENE_ENTRANCE VenusLighthouseSecondFloorEntrances[] = {
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

SCENE_ENTRANCE VenusLighthouseThirdFloorEntrances[] = {
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

SCENE_ENTRANCE VenusLighthouseFourthFloorEntrances[] = {
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

Uint32 VenusLighthouseExits[] = {
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

ACTOR_PLACE VenusLighthouseActorPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

ACTOR_PLACE VenusLighthouseEntryActorPlacements[] = {
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

ACTOR_PLACE VenusLighthouseGroundFloorActorPlacements[] = {
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
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (Sint32)VenusGreyStatueScript,
		664, 0, 792, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, CONDITION_IN_VIEW, (Sint32)VenusMiniatureLighthouseScript,
		488, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (Sint32)VenusGreyStatueScript,
		72, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (Sint32)VenusGreyStatueScript,
		72, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (Sint32)VenusGreyStatueScript,
		168, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (Sint32)VenusGreyStatueScript,
		168, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (Sint32)VenusGreyStatueScript,
		72, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (Sint32)VenusGreyStatueScript,
		104, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (Sint32)VenusGreyStatueScript,
		136, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (Sint32)VenusGreyStatueScript,
		168, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (Sint32)VenusGreyStatueScript,
		200, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE VenusLighthouseFirstFloorActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, CONDITION_IN_VIEW, (Sint32)VenusGreyPillarScript,
		248, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, CONDITION_IN_VIEW, (Sint32)VenusGreyPillarScript,
		184, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, CONDITION_IN_VIEW, (Sint32)VenusGreyPillarScript,
		200, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		168, 0, 392, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		88, 0, 376, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE VenusLighthouseSecondFloorActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (Sint32)VenusGreyStatueScript,
		808, 0, 728, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (Sint32)VenusTrackNorthSouthScript,
		776, 0, 712, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PARTICLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE VenusLighthouseThirdFloorActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (Sint32)VenusGreyStatueScript,
		888, 0, 680, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (Sint32)VenusTrackSouthEastScript,
		792, 0, 680, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (Sint32)VenusTrackWestEastScript,
		808, 0, 648, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (Sint32)VenusTrackNorthWestScript,
		824, 0, 680, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PARTICLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE VenusLighthouseFourthFloorActorPlacements[] = {
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
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_IN_VIEW, (Sint32)VenusGreyStatueScript,
		136, 0, 776, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (Sint32)VenusTrackNorthSouthScript,
		184, 0, 616, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (Sint32)VenusTrackSouthEastScript,
		200, 0, 616, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (Sint32)VenusTrackSouthWestScript,
		232, 0, 616, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_IN_VIEW, (Sint32)VenusTrackNorthEastScript,
		248, 0, 616, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PARTICLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
		664, 0, 136, FACING_NORTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

CUE VenusLighthouseEntryCues[] = {
	{ CUE_EXIT, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_EAST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_ENTRY_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_ENTRY_TO_WORLD_MAP_WEST },
	{ CUE_TALK, ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseHearOfTheAttack) },
	{ CUE_TALK, ACTOR_SCHOLAR_LIGHTHOUSE_OPEN, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIodemBeHappyTheEntranceTo) },
	{ CUE_TALK, ACTOR_SCHOLAR_BARBARIANS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseOhhhHowCouldTheyAttackMere) },
	{ CUE_TALK, ACTOR_WAITING_SCHOLAR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseTalkToWaitingScholar) },
	{ CUE_TALK, ACTOR_SOLDIER_OPENED_DOOR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseHowHowDidTheyOpenThe) },
	{ CUE_TALK, ACTOR_SOLDIER_HEADED_FOR_BABI, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheyAreAlreadyHeadedForBabi) },
	{ CUE_TALK, ACTOR_SOLDIER_COLOSSO_VICTORS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseBabisSoldiersAreAllColossoVictors) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_SOLDIER_TELLING_OF_THE_ATTACK, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseHearOfTheAttack) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_LIGHTHOUSE_OPEN, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseUhnnnIfOnlyICouldMove) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_BARBARIANS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheirPowersWereSoStrangeWho) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WAITING_SCHOLAR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIMustGetInsideVenusLighthouse) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_OPENED_DOOR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheYoungBoySeemedToGlow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_HEADED_FOR_BABI, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheyWerentInVenusLighthouseVery) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_COLOSSO_VICTORS, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIThoughtIRecognizedOneOf) },
	{ CUE_TABLE_END },
};

CUE VenusLighthouseGroundFloorCues[] = {
	{ CUE_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_ENTRY, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_ENTRY },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_TO_RELIEFS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_FRONT_ROOM_TO_RELIEFS },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_FRONT_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_FRONT_ROOM },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_EAST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_CARRY_STONE_WEST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_RELIEFS_TO_PILLARS },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_RELIEFS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_RELIEFS },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_CARRY_STONE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_CARRY_STONE },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_FLOOR_SWITCH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_FLOOR_SWITCH },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_NORTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_NORTH },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_PILLARS_TO_STATUE_ROW_WEST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_NORTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_NORTH },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_PILLARS_WEST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_SMALL_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_STATUE_ROW_TO_SMALL_ROOM },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_FLOOR_SWITCH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_FLOOR_SWITCH },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_SMALL_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_GREAT_STATUE_TO_SMALL_ROOM },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_PILLARS },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_STATUE_ROW, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_STATUE_ROW },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_GREAT_STATUE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_SMALL_ROOM_TO_GREAT_STATUE },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_GREAT_STATUE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_FLOOR_SWITCH_TO_GREAT_STATUE },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_WEST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_RELIEFS_EAST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_GROUND_FLOOR_CARRY_STONE_TO_PILLARS },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_LIGHTNING, FLAG_SCENE_TEMP_00, SCRIPT_REF(scVenusLighthouseThrowBackFromLightning) },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FLOOR_SWITCH, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseStepOnFloorSwitch) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_WALL_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthousePushPillarTowardWall) },
	{ CUE_TALK, ACTOR_SCHOLAR_ATTACKED_ANYWAY, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenToldThemToStopBut) },
	{ CUE_TALK, ACTOR_SCHOLAR_NOT_THE_ENTRANCE, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenWentInsideButThenCame) },
	{ CUE_TALK, ACTOR_DOOR_SCHOLAR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseTalkToScholarByDoor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_ATTACKED_ANYWAY, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIfKradenHadntBeenThereWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_NOT_THE_ENTRANCE, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenSaidSomethingAboutThisBeing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOOR_SCHOLAR, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIWonderWhatMysteriesTheStatue) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseReadRelief) },
#if !RELIEF_WALL_SINGLE_TRIGGER
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 1, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseReadRelief) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 2, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseReadRelief) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 3, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseReadRelief) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 4, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseReadRelief) },
#endif
#if !INNER_RELIEFS_SILENT
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 5, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseReadRelief) },
#endif
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_GREAT_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseSearchStatue) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_CHEST, FLAG_VENUS_LIGHTHOUSE_LUCKY_CAP, FIND_ITEM(ITEM_LUCKY_CAP) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_SECOND_CHEST, FLAG_VENUS_LIGHTHOUSE_CARRY_STONE, FIND_ITEM(ITEM_CARRY_STONE) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WALL_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseMovePillarTowardWall) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FLOOR_SWITCH_STATUE, FLAG_SCENE_TEMP_00, SCRIPT_REF(scVenusLighthouseMovePillarOntoSwitch) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_WALL_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseCarryPillarPastWall) },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseRevealPath) },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseHidePath) },
	{ CUE_TABLE_END },
};

CUE VenusLighthouseFirstFloorCues[] = {
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_LOWER_ROOM },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_GAP, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_GAP },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_NORTH },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_THUNDER_CROWN, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_DRIFTING_FLOOR_TO_THUNDER_CROWN },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_DRIFTING_FLOOR },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_LADDERS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_THUNDER_CROWN_TO_LADDERS },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_TO_LADDERS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_PILLARS_TO_LADDERS },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_EAST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_DRIFTING_FLOOR },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_LADDERS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_GAP_TO_LADDERS },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_GAP, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_GAP },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR_WEST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_THUNDER_CROWN, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_THUNDER_CROWN },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PILLARS },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PSYNERGY_STONE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LADDERS_TO_PSYNERGY_STONE },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_VENUS_LIGHTHOUSE_GROUND_FLOOR },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_TO_LADDERS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE_TO_LADDERS },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_TUNNEL_RUINS_LADY_STATUES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_TO_TUNNEL_RUINS_LADY_STATUES },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FIRST_FLOOR_LOWER_ROOM_TO_DRIFTING_FLOOR },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseDriftSouth) },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_HOLE, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseFallThroughFloor) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_BESIDE_CAPPED_PILLARS, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthousePushCappedPillar) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AMONG_CAPPED_PILLARS, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthousePushCappedPillar) },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_BESIDE_CAPPED_PILLARS, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseLeaveCappedPillarRows) },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AMONG_CAPPED_PILLARS, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseEnterCappedPillarRows) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_CARRY_PILLARS, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseJumpOrPushCarryPillar) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_CHEST, FLAG_VENUS_LIGHTHOUSE_THUNDER_CROWN, FIND_ITEM(ITEM_THUNDER_CROWN) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_PSYNERGY_STONE, FLAG_VENUS_LIGHTHOUSE_FIRST_FLOOR_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CAPPED_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseMoveCappedPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CAPPED_PILLAR + 1, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseMoveCappedPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CAPPED_PILLAR + 2, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseMoveCappedPillar) },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseUnblockCarryPillarMetatiles) },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseUnblockCarryPillarMetatiles) },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseUnblockCarryPillarMetatiles) },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseUnblockCarryPillarMetatiles) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseMoveCarryPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseMoveCarryPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_CARRY_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseCarryPillarAcrossGap) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_CARRY_PILLAR + 1, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseCarryPillarAcrossGap) },
	{ CUE_TABLE_END },
};

CUE VenusLighthouseSecondFloorCues[] = {
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_SOUTH },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VENUS_LIGHTHOUSE_SECOND_FLOOR_SPARKLING_PASSAGE_TO_PILLAR_PIT, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseEnterSparklingPassage) },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_SPARKLING_PASSAGE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_SPARKLING_PASSAGE },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_WEST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_GATE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_PILLAR_PIT_TO_GATE },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR_NORTH },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_TO_GATE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_WALL_SWITCH_TO_GATE },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_WALL_SWITCH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_WALL_SWITCH },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_PILLAR_PIT, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_SECOND_FLOOR_GATE_TO_PILLAR_PIT },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_WALL_SWITCH, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseStepOnWallSwitch) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_GATE_PILLAR, FLAG_VENUS_LIGHTHOUSE_GATE_OPEN, SCRIPT_REF(scVenusLighthouseJumpOrPushGatePillar) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_LOCKED_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseSearchLockedDoor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GATE_PILLAR, FLAG_VENUS_LIGHTHOUSE_GATE_OPEN, SCRIPT_REF(scVenusLighthouseMoveGatePillar) },
	{ CUE_TABLE_END },
};

CUE VenusLighthouseThirdFloorCues[] = {
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_WEST_NORTHWEST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_TO_SPARKLING_PASSAGES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHWEST_ROOM_TO_SPARKLING_PASSAGES },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_TO_SPARKLING_PASSAGES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRAGON_SCALES_TO_SPARKLING_PASSAGES },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_NORTHWEST_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseEnterSparklingPassage) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRAGON_SCALES, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseEnterSparklingPassage) },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_SPARKLING_PASSAGES_TO_DRIFTING_FLOOR },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHEAST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST_ROOM_TO_DRIFTING_FLOOR },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SPARKLING_PASSAGES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SPARKLING_PASSAGES },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTH_NORTHWEST },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SOUTHWEST_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseEnterSparklingPassage) },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_NORTHEAST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_FOURTH_FLOOR_SOUTHWEST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_PILLAR_SOCKETS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_PILLAR_SOCKETS },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_DRIFTING_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_DRIFTING_FLOOR },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_WEST_ROOM, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_PILLAR_SOCKETS_TO_WEST_ROOM },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_TO_VENUS_LIGHTHOUSE_SECOND_FLOOR },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_TO_PILLAR_SOCKETS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_ROOM_TO_PILLAR_SOCKETS },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseDriftSouth) },
	{ CUE_STEP, VENUS_LIGHTHOUSE_THIRD_FLOOR_DRIFTING_FLOOR_TO_SOUTHWEST_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseDriftSouth) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_SWITCH_PILLARS, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseJumpOrPushSwitchPillar) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_LOCKED_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseSearchLockedDoor) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_CHEST, FLAG_VENUS_LIGHTHOUSE_DRAGON_SCALES, FIND_ITEM(ITEM_DRAGON_SCALES) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SWITCH_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseMoveSwitchPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SWITCH_PILLAR + 1, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseMoveSwitchPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SWITCH_PILLAR + 2, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseMoveSwitchPillar) },
	{ CUE_TABLE_END },
};

CUE VenusLighthouseFourthFloorCues[] = {
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_WEST_NORTHWEST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_TO_WALL_SWITCHES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLARS_TO_WALL_SWITCHES },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLARS, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLARS },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHWEST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTH_NORTHEAST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PSYNERGY_STONE, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PSYNERGY_STONE },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_NORTHEAST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_EAST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_TO_VENUS_LIGHTHOUSE_THIRD_FLOOR_SOUTHWEST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_WEST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_GAIA_BLADE_EAST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_SOUTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_WALL_SWITCHES_TO_PILLAR_SOCKETS_SOUTH },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_WEST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_WEST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_GAIA_BLADE_TO_WALL_SWITCHES_EAST },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_SOUTH, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_SOUTH },
	{ CUE_EXIT, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_EAST, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PILLAR_SOCKETS_TO_WALL_SWITCHES_EAST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES, CONDITION_ALWAYS, VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE_TO_WALL_SWITCHES },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_RISING_PASSAGE, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseEnterRisingPassage) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthousePushPuzzlePillar) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_GAP, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseJumpAcrossGap) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_GAP, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseJumpAcrossGap) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_GAP, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthousePushPuzzlePillar) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_EAST_GAP, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseJumpAcrossGap) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_FLOATING_PILLARS_EAST_GAP, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthousePushPuzzlePillar) },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_WEST_WALL_SWITCH, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseStepOnWestWallSwitch) },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_EAST_WALL_SWITCH, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseStepOnEastWallSwitch) },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseDriftSouth) },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseDriftNorth) },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_WEST, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseDriftWest) },
	{ CUE_STEP, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_DRIFT_EAST, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseDriftEast) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_SOCKET_PILLARS, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseJumpOrPushSocketPillar) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_LOCKED_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseSearchLockedDoor) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_CHEST, FLAG_VENUS_LIGHTHOUSE_GAIA_BLADE, FIND_ITEM(ITEM_GAIA_BLADE) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_PSYNERGY_STONE, FLAG_VENUS_LIGHTHOUSE_FOURTH_FLOOR_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR + 1, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR + 2, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_FLOATING_PILLAR + 3, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseCarryFloatingPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR + 1, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseCarryFloatingPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR + 2, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseCarryFloatingPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_FLOATING_PILLAR + 3, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseCarryFloatingPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseMoveSocketPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR + 1, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseMoveSocketPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR + 2, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseMoveSocketPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_SOCKET_PILLAR + 3, CONDITION_ALWAYS, SCRIPT_REF(scVenusLighthouseMoveSocketPillar) },
	{ CUE_TABLE_END },
};
/* clang-format on */

/* Cursors into each third-floor socket's list of pieces in
   VenusThirdFloorTrackLists, which scVenusLighthouseRouteLight moves on. */
const Uint8 *VenusThirdFloorSocketTracks[] = {
	VenusThirdFloorTrackLists,
	VenusThirdFloorTrackLists + 4,
	VenusThirdFloorTrackLists + 8,
	VenusThirdFloorTrackLists + 12,
};

/* Each third-floor socket's light scripts, by the index of the piece it has
   routed. */
Sint32 *VenusThirdFloorNorthwestPaths[] = {
	VenusThirdFloorLightNorthwestToGate,
	VenusThirdFloorLightNorthwestToSouthwest,
	VenusLightGoesOut,
	VenusLightGoesOut,
};
Sint32 *VenusThirdFloorNortheastPaths[] = {
	VenusThirdFloorLightNortheastToNorthwest,
	VenusThirdFloorLightNortheastToSoutheast,
	VenusLightGoesOut,
	VenusLightGoesOut,
};
Sint32 *VenusThirdFloorSouthwestPaths[] = {
	VenusThirdFloorLightSouthwestToGate,
	VenusThirdFloorLightSouthwestToNorthwest,
	VenusThirdFloorLightSouthwestToGate,
	VenusLightGoesOut,
};
Sint32 *VenusThirdFloorSoutheastPaths[] = {
	VenusThirdFloorLightSoutheastToSouthwest,
	VenusThirdFloorLightSoutheastToNortheast,
	VenusThirdFloorLightSoutheastToSouthwest,
	VenusLightGoesOut,
};

/* Each third-floor socket's paths, by the index of the piece in its list. */
Sint32 **VenusThirdFloorSocketPaths[] = {
	VenusThirdFloorNorthwestPaths,
	VenusThirdFloorNortheastPaths,
	VenusThirdFloorSouthwestPaths,
	VenusThirdFloorSoutheastPaths,
};

/* As VenusThirdFloorSocketTracks, for the fourth floor. */
const Uint8 *VenusFourthFloorSocketTracks[] = {
	VenusFourthFloorTrackLists,
	VenusFourthFloorTrackLists + 2,
	VenusFourthFloorTrackLists + 3,
	VenusFourthFloorTrackLists + 5,
	VenusFourthFloorTrackLists + 7,
	VenusFourthFloorTrackLists + 9,
	VenusFourthFloorTrackLists + 12,
	VenusFourthFloorTrackLists + 14,
};

/* Each fourth-floor socket's light scripts, by the index of the piece it has
   routed. */
Sint32 *VenusFourthFloorNorthwestPaths[] = {
	VenusFourthFloorLightNorthwestToGate,
	VenusLightGoesOut,
};
Sint32 *VenusFourthFloorNortheastPaths[] = {
	VenusLightGoesOut,
};
Sint32 *VenusFourthFloorWestPaths[] = {
	VenusFourthFloorLightWestToNorthwest,
	VenusLightGoesOut,
};
Sint32 *VenusFourthFloorEastPaths[] = {
	VenusFourthFloorLightEastToNortheast,
	VenusLightGoesOut,
};
Sint32 *VenusFourthFloorFarWestPaths[] = {
	VenusFourthFloorLightFarWestToWest,
	VenusLightGoesOut,
};
Sint32 *VenusFourthFloorMiddlePaths[] = {
	VenusFourthFloorLightMiddleToEast,
	VenusFourthFloorLightMiddleToWest,
	VenusLightGoesOut,
};
Sint32 *VenusFourthFloorSouthwestPaths[] = {
	VenusFourthFloorLightSouthwestToFarWest,
	VenusLightGoesOut,
};
Sint32 *VenusFourthFloorSoutheastPaths[] = {
	VenusFourthFloorLightSoutheastToMiddle,
	VenusLightGoesOut,
};

/* As VenusThirdFloorSocketPaths, for the fourth floor. */
Sint32 **VenusFourthFloorSocketPaths[] = {
	VenusFourthFloorNorthwestPaths,
	VenusFourthFloorNortheastPaths,
	VenusFourthFloorWestPaths,
	VenusFourthFloorEastPaths,
	VenusFourthFloorFarWestPaths,
	VenusFourthFloorMiddlePaths,
	VenusFourthFloorSouthwestPaths,
	VenusFourthFloorSoutheastPaths,
};
