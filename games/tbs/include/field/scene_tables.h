#ifndef ALCHEMY_SCENE_TABLES_H
#define ALCHEMY_SCENE_TABLES_H

/* The scene table formats: entrances, exits, actor placements and cues, and the
   macros that write them. */

#include "fixed_math.h"
#include "scene_exits.h"
#include "scene_ids.h"
#include "sprite_ids.h"
#include "static_assert.h"
#include "field/field_state.h"
#include "game/character_ids.h"

/* The sixteen compass headings in 1/65536 turns, from east through south: a
   quarter turn is DEGtoANG(90), and the screen's y axis points south. */
typedef enum FACING {
	FACING_EAST = 0,
	FACING_EAST_SOUTHEAST = TRIG_TURN / 16,
	FACING_SOUTHEAST = DEGtoANG(45),
	FACING_SOUTH_SOUTHEAST = TRIG_TURN * 3 / 16,
	FACING_SOUTH = DEGtoANG(90),
	FACING_SOUTH_SOUTHWEST = TRIG_TURN * 5 / 16,
	FACING_SOUTHWEST = TRIG_TURN * 3 / 8,
	FACING_WEST_SOUTHWEST = TRIG_TURN * 7 / 16,
	FACING_WEST = DEGtoANG(180),
	FACING_WEST_NORTHWEST = TRIG_TURN * 9 / 16,
	FACING_NORTHWEST = TRIG_TURN * 5 / 8,
	FACING_NORTH_NORTHWEST = TRIG_TURN * 11 / 16,
	FACING_NORTH = TRIG_TURN * 3 / 4,
	FACING_NORTH_NORTHEAST = TRIG_TURN * 13 / 16,
	FACING_NORTHEAST = TRIG_TURN * 7 / 8,
	FACING_EAST_NORTHEAST = TRIG_TURN * 15 / 16
} FACING;

/* A sixteenth of a turn, half the step between two headings. */
enum { FACING_STEP = TRIG_TURN / 16 };

/* True for a heading strictly between FACING_NORTHWEST and FACING_NORTHEAST. */
#define FACING_IS_NORTH(facing) \
	((Uint32)((facing) - (FACING_NORTHWEST + 1)) <= \
		FACING_NORTHEAST - FACING_NORTHWEST - 2)

/* A facing rounded to the nearest of east, south, west and north. */
#define FACING_NEAREST_CARDINAL(facing) (((facing) + DEGtoANG(45)) & 0xc000)
/* A facing rounded to the nearest eighth of a turn. */
#define FACING_NEAREST_EIGHTH(facing)	(((facing) + TRIG_TURN / 16) & 0xe000)
/* The quarter of a turn a facing lies in, what is past it dropped: east, south,
   west or north. */
#define FACING_QUARTER(facing)			((facing) & 0xc000)
/* The eighth of a turn a facing lies in, what is past it dropped. */
#define FACING_EIGHTH(facing)			((facing) & 0xe000)
/* The eighth of a turn a facing lies in, as a number from 0 (east) to 7. */
#define FACING_EIGHTH_INDEX(facing)		((facing) >> 13)
/* The sixteenth of a turn a facing lies in, what is past it dropped. */
#define FACING_SIXTEENTH(facing)		((facing) & 0xf000)
/* The sixteenth of a turn a facing lies in, as a number from 0 (east) to 15,
   which indexes the scenes' sixteen-direction step tables. */
#define FACING_INDEX(facing)			((facing) >> 12)

/* In the first column of a scene table, ends the table. */
enum { SCENE_TABLE_END = -1 };

/* A table row's condition: CONDITION_ALWAYS, a flag number that must be clear,
   or IF_FLAG_SET of a flag that must be set (fdGameFlagIsConditionActive). */
enum {
	CONDITION_ALWAYS = -1,
	/* An actor placement's condition that fdAssignViewFlags replaces with
	   whether the actor lies within the map's view bounds, in the scenes that
	   call it, so only the room entered is populated. */
	CONDITION_IN_VIEW = 0,
	CONDITION_FLAG_SET = 0x1000 /* the flag must be set, not clear */
};

/* A condition that holds while flag is set. */
#define IF_FLAG_SET(flag) ((flag) | CONDITION_FLAG_SET)

/* A scene's entrance: where and facing which way the party arrives, and the
   camera's bounds there (-1 for none). */
typedef struct SCENE_ENTRANCE {
	/* The entrance number the main routine's state names; -1 ends the table. */
	Sint16 entrance;
	Sint16 requiredFlag;	  /* a flag that must be set, or -1 for none */
	Sint16 x;				  /* pixels */
	Sint16 y;				  /* pixels */
	Sint16 z;				  /* pixels */
	Uint16 facing;			  /* 1/65536 turns (FACING) */
	Sint16 unusedAfterFacing; /* 0 in every table; not read */
	Sint16 cameraLeft;		  /* pixels */
	Sint16 cameraTop;		  /* pixels */
	Sint16 cameraRight;		  /* pixels */
	Sint16 cameraBottom;	  /* pixels */
	Sint16 unusedAfterCamera; /* 0 in every table; not read */
} SCENE_ENTRANCE;

STATIC_ASSERT(sizeof(SCENE_ENTRANCE) == 24);
STATIC_ASSERT(offsetof(SCENE_ENTRANCE, cameraLeft) == 14);

/* An exit table's header word: the scene whose exits follow, named by its name
   (`SCENE_EXITS(Bilibin)`). */
#define SCENE_EXITS(map) ((Uint32)&SceneId##map)
/* One exit: exit kind `exit` leads to `entrance` of the scene `scene`, both
   named in scene_exits.h. */
#define SCENE_EXIT(exit, map, entrance) \
	(((Uint32)(exit) << 20) + ((Uint32)(entrance) << 12) + \
		(Uint32)&SceneId##map)
/* An exit word's scene. */
#define EXIT_SCENE_MASK 0xfff
/* An exit word's entrance. */
#define ENTRANCE(word)	(((word) & 0xff000) >> 12)
/* An exit word's exit kind. */
#define EXIT_ID(word)	(((word) & 0x0ff00000) >> 20)
/* An exit word's bit 28: a flag word follows it. */
#define EXIT_HAS_FLAG	0x10000000
/* The exit kind that matches whatever exit the scene ended on. */
#define EXIT_ID_ANY		255
/* An exit with a flag word after it: the exit is taken unless `flag` is set,
   and when it is set the search goes on to the next exit. */
#define SCENE_EXIT_UNLESS(exit, map, entrance, flag) \
	(SCENE_EXIT(exit, map, entrance) + EXIT_HAS_FLAG), (Uint32)(flag)
/* SCENE_EXIT_UNLESS's flag for no flag: gmFlagGetStub reads it as flag 0xfff,
   which nothing sets, so the exit is always taken. */
#define EXIT_FLAG_NONE				  0xffffffff
/* The last header word: it ends the last scene's exits and the table. */
#define SCENE_EXITS_END				  511
/* Where a scene numbers its door or exit metatiles by their exits (Imil,
   Mercury Lighthouse), each one's metatile behavior is its exit's number plus
   this. */
#define EXIT_METATILE_BEHAVIOR_OFFSET 50

/* Mode ids above the scene table's rows: the modes other than the field mode,
   which gmMainRoutineRun runs in its place. */
enum {
	/* Runs nothing and returns 0; nothing in the game sends the party there. */
	MODE_NONE = 507,
	/* Lucky Wheels, from its room in Tolbi, SceneIdStuccoRoomsC
	   (lucky_wheels.c). */
	MODE_SLOTS = 508,
	/* Lucky Dice, from its room in Tolbi, SceneIdStuccoRoomsC
	   (lucky_dice.c). */
	MODE_DICE = 509,
	/* A battle; the entrance is the formation, and the result -1 a defeat. */
	MODE_BATTLE = 510
};

/* An exit into a battle: exit kind `exit` fights formation `group`. */
#define EXIT_TO_BATTLE(exit, group) \
	(((Uint32)(exit) << 20) + ((Uint32)(group) << 12) + MODE_BATTLE)

/* An actor a scene places: its sprite, the condition for placing it, how it
   behaves, where it stands and which way it faces, and how it turns to talk. */
typedef struct ACTOR_PLACE {
	/* The actor's sprite; 0 to 7 are the party's characters, each in its own
	   actor slot, and -1 ends the table. */
	Sint16 sprite;
	/* CONDITION_ALWAYS, a flag | CONDITION_FLAG_SET set, else the flag
	   clear. */
	Sint16 condition;
	/* A movement type (MOVEMENT_TYPE) or an actor script, for
	   fdActorSetMovementType. */
	Sint32 movement;
	FIXED  x;		   /* 16.16 fixed-point pixels */
	FIXED  y;		   /* 16.16 fixed-point pixels */
	FIXED  z;		   /* 16.16 fixed-point pixels */
	Uint16 facing;	   /* 1/65536 turns (FACING) */
	Uint8  talkFacing; /* ACTOR_TALK_FACING */
	Uint8  flags;	   /* ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE */
} ACTOR_PLACE;

/* One row of a scene's actor placements, its spot in plain pixels. */
#define ACTOR_PLACEMENT( \
	sprite, condition, movement, x, y, z, facing, talkFacing, flags) \
	{sprite, condition, movement, toFIXED(x), toFIXED(y), toFIXED(z), facing, \
		talkFacing, flags}

STATIC_ASSERT(sizeof(ACTOR_PLACE) == 24);
STATIC_ASSERT(offsetof(ACTOR_PLACE, talkFacing) == 22);

/* The movement types an actor placement's behavior can name, which
   fdActorSetMovementType turns into scripts. */
typedef enum MOVEMENT_TYPE {
	MOVEMENT_TYPE_STAND = 1, /* its script only ends */
	/* Wanders about where it stands (SC_WANDER_HOME), within 16 pixels. */
	MOVEMENT_TYPE_WANDER = 2,
	/* as MOVEMENT_TYPE_WANDER, within 8 pixels */
	MOVEMENT_TYPE_WANDER_NEAR = 3,
	MOVEMENT_TYPE_WANDER_CLOSE = 4,	  /* within 6 pixels, at its own speed */
	MOVEMENT_TYPE_FOLLOW_WATCHED = 5, /* moves toward the watched actor */
	MOVEMENT_TYPE_FACE_LEADER = 6, /* watches the leader and turns toward it */
	MOVEMENT_TYPE_RESET_ACTION = 7 /* MovementTypeResetActionScript */
} MOVEMENT_TYPE;

/* How a placed actor turns when the leader talks to it (fdTalkToActor). With
   all but TALK_FACING_KEEP the leader stops and each watches the other. */
typedef enum ACTOR_TALK_FACING {
	TALK_FACING_PARTY = 0,
	/* Faces the party, then turns back to its old heading. */
	TALK_FACING_PARTY_AND_BACK = 1,
	TALK_FACING_KEEP = 2,
	/* Faces the party, then keeps turning to face the leader once the talk is
	   over (ActorFaceWatchedScript). */
	TALK_FACING_PARTY_THEN_WATCH = 3
} ACTOR_TALK_FACING;

/* ACTOR_PLACE.flags: the actor takes the previous actor's sprite resource
   instead of loading its own, when both are single sprites. */
#define ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE 0x01

/* Actor ids: 0 to 7 the party's characters, the placed actors from 8. */
typedef enum ACTOR_ID {
	ACTOR_PARTY_LEADER = 0,
	ACTOR_GARET = 1,
	ACTOR_IVAN = 2,
	ACTOR_MIA = 3,
	ACTOR_JENNA = 5,
	ACTOR_SHEBA = 6,
	ACTOR_FIRST_PLACED = 8,
	/* The last actor id, as scenes loop over every placed actor. */
	ACTOR_LAST_ID = ACTOR_ID_COUNT - 1
} ACTOR_ID;

/* A cue: its kind and detail, the subject it answers, its condition and its
   value (a message or a script). */
typedef struct CUE {
	/* The kind and its detail, read by fdFindCue and the item, Psynergy and
	   examine cue lookups; -1 ends the table. */
	Uint32 kind;
	/* What the cue answers: an actor id, a metatile behavior, a treasure spot
	   id or a raised value. */
	Sint16 subject;
	/* CONDITION_ALWAYS, a flag | CONDITION_FLAG_SET set, else the flag
	   clear. */
	Sint16 condition;
	/* A message below CUE_RESULT_MESSAGE_END, else a script; for an exit, the
	   exit kind its scene's exit table matches, and for an examine cue, a FIND_
	   value. */
	Sint32 value;
} CUE;

STATIC_ASSERT(sizeof(CUE) == 12);

/* CUE.subject bit 10: a Psynergy cue runs in place of its field Psynergy, as
   Mind Read on someone who notices it does, and an examine cue shows no
   checking message. */
#define CUE_SUBJECT_OWN_EFFECT 0x400

/* CUE.subject for an item or Psynergy cue: bits 15 to 12 the leader's facing
   (an angle's top four bits), heeded only with bit 11 set, and bits 7 to 0 the
   actor id or metatile behavior it answers. */
#define CUE_SUBJECT_FACING		 0xf000
#define CUE_SUBJECT_NEEDS_FACING 0x0800
#define CUE_SUBJECT_TARGET		 0xff

/* CUE.kind's low four bits: what the cue answers. */
typedef enum CUE_KIND {
	CUE_TALK = 0, /* the A button on an actor; the subject is its id */
	/* Stepping onto a metatile whose metatile behavior is the subject, when no
	   step cue answers it; value is the exit kind. */
	CUE_EXIT = 1,
	CUE_STEP = 2,	 /* stepping onto or pushing against a metatile behavior */
	CUE_EXAMINE = 3, /* the A button on what is ahead, when it is no actor */
	CUE_ITEM = 4,	 /* an item used on a metatile or an actor */
	CUE_PSYNERGY = 5,
	CUE_RAISED = 6, /* raised by code through FIELD_STATE.raisedCue */
	/* Talking to an actor Halt has stopped (fdFindActorTalkCue). */
	CUE_TALK_HALTED = 7
} CUE_KIND;

/* An exit cue's control bits 5..4: how the leader leaves
   (fdRunStepCueOrExit). */
typedef enum EXIT_STYLE {
	EXIT_STYLE_NORMAL = 0x00, /* with SE_MAP_EXIT */
	EXIT_STYLE_SILENT = 0x10,
	/* With SE_STAIRS_UP and ANIM_LEADER_LEAVE; the next scene plays
	   ANIM_LEADER_ARRIVE (FLAG_ARRIVAL_STAIRS_UP). */
	EXIT_STYLE_STAIRS_UP = 0x20,
	/* With SE_STAIRS_DOWN and ANIM_LEADER_LEAVE_ALTERNATE; the next scene plays
	   ANIM_LEADER_ARRIVE_ALTERNATE (FLAG_ARRIVAL_STAIRS_DOWN). */
	EXIT_STYLE_STAIRS_DOWN = 0x30,
	EXIT_STYLE_MASK = 0x30
} EXIT_STYLE;

/* The raised cue the menus and shortcut keys raise instead of opening while
   FLAG_MENUS_RAISE_CUE is set, as Vault's inn sets it while the party corners
   the suspects. */
#define FIELD_RAISED_CUE_MENUS 250

/* A step cue that needs the leader to have pushed against it for more than
   FIELD_PUSH_TICKS ticks, or 2 with STEP_CUE_FLAG_QUICK. */
#define STEP_CUE_FLAG_WALKING	0x200
/* With STEP_CUE_FLAG_WALKING, 2 ticks of pushing are enough. */
#define STEP_CUE_FLAG_QUICK		0x800
/* A script that needs the leader to face within an eighth of a turn of the
   heading in control bits 15..12 (fdFindCue). */
#define CUE_FLAG_FACING			0x400
#define STEP_CUE_WALKING		(CUE_STEP | STEP_CUE_FLAG_WALKING)
/* facing is an FACING; only its top four bits are compared. */
#define STEP_CUE_FACING(facing) (CUE_STEP | CUE_FLAG_FACING | (facing))
#define STEP_CUE_WALKING_INTO(facing) \
	(CUE_STEP | STEP_CUE_FLAG_WALKING | CUE_FLAG_FACING | (facing))

/* When a Psynergy cue runs around its field Psynergy, control bits 30..28. */
typedef enum PSYNERGY_PHASE {
	/* After the field Psynergy (fdUsePsynergy), PSYNERGY_ON_ACTOR's phase. With
	   PSYNERGY_SCRIPT_PLAYS_CAST the script plays the cast itself. */
	PSYNERGY_PHASE_AFTER_CAST = 0,
	/* Once the PP is paid, before the field Psynergy. */
	PSYNERGY_PHASE_BEFORE_CAST = 1,
	/* After a Psynergy that runs out by itself has ended:
	   fdRunPsynergyEndScripts runs it for a use with ACT_PACKED, as Cloak fades
	   or Reveal runs out. */
	PSYNERGY_PHASE_AFTER_END = 2,
	PSYNERGY_PHASE_BEFORE_END = 3, /* as PSYNERGY_PHASE_AFTER_END, before it */
	/* As Reveal's colours come back (fdPsynergyEndReveal): the scenes hide
	   again what Reveal showed. */
	PSYNERGY_PHASE_FADES = 4,
	/* A script in this phase also makes fdUsePsynergy aim the field Psynergy at
	   the actor ahead. */
	PSYNERGY_PHASE_ACTS = 5,
	PSYNERGY_PHASE_ANY = 7 /* fdFindPsynergyCue matches every phase */
} PSYNERGY_PHASE;

/* The control word of a Psynergy cue (CUE_PSYNERGY) in the given phase, as
   fdFindPsynergyCue compares it (bits 30..28 phase | 3..0 kind). */
#define PSYNERGY_CUE(phase) ((phase) << 28 | CUE_PSYNERGY)
#define PSYNERGY_CUE_ANY	PSYNERGY_CUE(PSYNERGY_PHASE_ANY)

/* A Psynergy cue on the actor the leader faces. */
#define CUE_PSYNERGY_ON_ACTOR 0x10
#define PSYNERGY_ON_ACTOR(ability) \
	(CUE_PSYNERGY | CUE_PSYNERGY_ON_ACTOR | (ability) << 8)

/* Bits of an after-cast Psynergy cue's target word, beside the actor's
   number. */
typedef enum PSYNERGY_SCRIPT_TARGET {
	/* fdUsePsynergy clears FLAG_PLAY_PSYNERGY_CAST and plays no cast of its
	   own, leaving it to the script. */
	PSYNERGY_SCRIPT_PLAYS_CAST = 0x400
} PSYNERGY_SCRIPT_TARGET;

/* A Psynergy cue on an actor at one phase of the cast, as PSYNERGY_ANYWHERE is
   for any target. */
#define PSYNERGY_PHASE_ON_ACTOR(ability, phase) \
	((Uint32)(phase) << 28 | PSYNERGY_ON_ACTOR(ability))

/* A Psynergy cue for any target, not a particular actor or metatile. */
#define CUE_PSYNERGY_ANYWHERE 0x80
#define PSYNERGY_ANYWHERE(ability, phase) \
	((Uint32)(phase) << 28 | (ability) << 8 | CUE_PSYNERGY_ANYWHERE | \
		CUE_PSYNERGY)

/* Using an item (one byte) on a metatile behavior, or on an actor, as
   fdFindItemCue reads them. */
#define CUE_ITEM_ON_ACTOR		0x10
#define USE_ITEM(item)			(CUE_ITEM | (item) << 8)
#define USE_ITEM_ON_ACTOR(item) (CUE_ITEM | CUE_ITEM_ON_ACTOR | (item) << 8)

/* An examine cue: the leader presses A at `target` (EXAMINE) ahead, when it is
   no actor. */
#define EXAMINE_CUE(target)		   (CUE_EXAMINE | (target) << 4)
/* An examine cue searched while facing the given way, as a counter is searched
   across. */
#define EXAMINE_CUE_FACING(facing) (CUE_EXAMINE | CUE_FLAG_FACING | (facing))
/* A chest searched while facing the given way. */
#define EXAMINE_CHEST_FACING(facing) \
	(EXAMINE_CUE(EXAMINE_CHEST) | CUE_FLAG_FACING | (facing))
/* A talk cue that answers only while the leader faces the given way, so one
   actor can answer differently from each side. */
#define TALK_CUE_FACING(facing) (CUE_TALK | CUE_FLAG_FACING | (facing))

/* What an examine cue is on, EXAMINE_CUE()'s control bits 8..4: it picks the
   line fdExamineMetatile shows before the search's result, MsgTileChecked +
   target ("checked the jar..."), and EXAMINE_SILENT shows none. */
typedef enum EXAMINE {
	EXAMINE_SILENT = 0,
	EXAMINE_CHEST = 1,
	EXAMINE_JAR,
	EXAMINE_BARREL,
	EXAMINE_WALL,
	EXAMINE_GROUND,
	EXAMINE_ROCK,
	EXAMINE_HOLE,
	EXAMINE_GRAVE,
	EXAMINE_TREE,
	EXAMINE_UNDERBRUSH,
	EXAMINE_DOOR,
	EXAMINE_CHIMNEY,
	EXAMINE_WOODEN_BOX,
	EXAMINE_BED,
	EXAMINE_BOOKCASE,
	EXAMINE_STONE_COFFIN,
	EXAMINE_FIREPLACE,
	EXAMINE_WATER,
	EXAMINE_STONE_PILLAR,
	EXAMINE_STALACTITE,
	EXAMINE_BOARDS,
	EXAMINE_FOUNTAIN,
	EXAMINE_OVEN,
	EXAMINE_TABLE,
	EXAMINE_STONE_STATUE,
	EXAMINE_STONE_TABLET,
	EXAMINE_SHELF,
	EXAMINE_WARDROBE,
	EXAMINE_FIREWOOD,
	EXAMINE_BOOKS,
	EXAMINE_WELL
} EXAMINE;

/* What a search finds, an examine cue's value: a function when bits 27..24 are
   set, else a type in its top twelve bits. */
#define FIND_ITEM(item)		  (0x00100000 | (item))
#define FIND_COINS(coins)	  (0x00200000 | (coins))
/* A Psynergy Stone, which restores all PP. */
#define FIND_PSYNERGY_STONE	  0x00300000
#define FIND_MESSAGE(message) (0x00400000 | (message))
/* A mimic chest's battle, with the script's value its slot of
   ENCOUNTER_ZONE_MIMIC. */
#define FIND_MIMIC			  0x00500000

/* A script as a Sound value; its address is above */
/* CUE_RESULT_MESSAGE_END, so it is never taken for a message. */
#define SCRIPT_REF(script) ((Uint32)(script))
#define CUE_TABLE_END	   0xffffffff

/* SC_FACE_WATCHED every tick, forever: the script of an actor told to keep
   facing its watched actor. */
extern const Sint32 ActorFaceWatchedScript[];

/* Where the party returns after a defeat: {scene, entrance (-1 any), return
   scene, return entrance} rows, ended by scene -1 (fdSetRestartPoint). */
extern const Sint16 SceneRestartRules[];

/* Battle backgrounds by scene: a background word (bit 31 set, resource id
   below) and then its scenes, ended by 0; a scene not listed takes the last
   background (fdBattleBackgroundSelectForMap). */
extern Sint32 SceneBattleBackgroundRules[];

/* Entrance tables more than one scene file holds, each from a part of
   src/scene/shared/: the fallback of a cave or island scene
   (fallback_entrances.c) and of Kalay's (kalay_fallback_entrances.c), the
   title's and the File Screen's (screen_entrances.c), and Sol Sanctum's
   Elemental Star room's (sol_sanctum_star_room_entrances.c). */
extern SCENE_ENTRANCE SceneFallbackEntrances[];
extern SCENE_ENTRANCE KalayFallbackEntrances[];
extern SCENE_ENTRANCE ScreenEntrances[];
extern SCENE_ENTRANCE SolSanctumStarRoomEntrances[];

#endif
