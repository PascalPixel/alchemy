#ifndef ALCHEMY_FIELD_MODE_INTERNAL_H
#define ALCHEMY_FIELD_MODE_INTERNAL_H

/* What every part of field_mode.c shares: its includes and the macros and types
   more than one part uses. field_mode.c includes it before the parts, and each
   part includes it too, so that a part read on its own resolves. It places no
   bytes. */

#include "ability_ids.h"
#include "debug/debug.h"
#include "flags.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "field/actor_script.h"
#include "field/battle_music.h"
#include "field/encounter_zone.h"
#include "field/script.h"
#include "field/field_psynergy_state.h"
#include "field/ground_mask.h"
#include "field/map_grid_scroll.h"
#include "field/world_map_encounter_rules.h"
#include "graphics/display_transition.h"
#include "graphics/graphics.h"
#include "window/battle_status.h"
#include "text/msg_ids.h"

/* The speakers' definitions (field_mode_tables.c), read by
   fdSpeakerFindDefinition. */
extern const SPEAKER_DEFINITION SpeakerDefinitions[];

/* A position syVectorAddPolarOffset moves: as a vector, or as its three
   coordinates. */
typedef union POLAR_OFFSET_POS {
	VECTOR3 vector;
	Sint32	coordinates[3];
} POLAR_OFFSET_POS;

STATIC_ASSERT(sizeof(POLAR_OFFSET_POS) == 12);

/* No user named: fdUsePsynergy takes ISAAC, and fdUseItem whoever holds the
   item in the most inventory slots. */
#define UNIT_NONE 15

/* Shows message m with the user's and the Psynergy's names, and waits. */
#define PSYNERGY_MESSAGE(who, act, m) \
	(mnMessageArgumentPushStub((who), MESSAGE_ARGUMENT_ACTOR), \
		mnMessageArgumentPushStub((act), MESSAGE_ARGUMENT_ABILITY), \
		mnFieldMessageShowStub((m), FIELD_MESSAGE_ZOOM))

/* A scene's encounter zones (Scene_EncounterRules, encounter_rules.s), ended by
   scene -1. */
typedef struct SCENE_ENCOUNTER_RULE {
	Sint16 sceneId;	 /* -1 ends the table */
	Sint16 entrance; /* -1 for any entrance */
	Sint16 flg : 15; /* -1, or a flag that turns the rule off once set */
	Sint16 firstZoneOnGround : 1; /* the first zone is for ground 0, not 1 */
	/* The first of up to seven consecutive zones, rows of encounter_zones.c; 0
	   for none on any ground. */
	Sint16 firstZone;
} SCENE_ENCOUNTER_RULE;

/* The scenes' encounter rules; the first that matches the scene and entrance,
   with its flag unset, gives the encounter zones. */
extern const SCENE_ENCOUNTER_RULE Scene_EncounterRules[];

/* A rule choosing a scene's field music (Scene_MusicRules, music_rules.s),
   ended by id -1. */
typedef struct MAP_MUSIC_RULE {
	Sint16 id;			  /* a scene, or an area */
	Sint16 entrance : 15; /* -1 for any entrance */
	Uint16 idIsMap : 1;	  /* 1 when id is a scene, 0 when it is an area */
	Sint16 flg;			  /* -1, or a flag that must be set */
	Sint16 music;		  /* sounds.h's MUS_ numbers */
} MAP_MUSIC_RULE;

/* The field music rules; the first that matches the scene or its area, the
   entrance and the flag gives the music. */
extern const MAP_MUSIC_RULE Scene_MusicRules[];

/* A rule naming the area a scene is in (Area_NameRules, area_name_rules.s),
   ended by id -1. */
typedef struct AREA_NAME_RULE {
	Sint16 id;			  /* a scene, or an area */
	Sint16 entrance : 15; /* -1 for any entrance */
	Uint16 useMapId : 1;  /* 1 when id is a scene, 0 when it is an area */
	/* The area name, as an offset that most readers add to MsgDebugAreaName. 0
	   is the Title's name (area 0's rule), which a scene no rule matches also
	   gets. */
	Sint32 areaNameIdx;
} AREA_NAME_RULE;

/* The area name rules; the first that matches the scene or its area and the
   entrance gives the name. */
extern const AREA_NAME_RULE Area_NameRules[];

/* A run of screen lines in a transition shape: one window edge pair for every
   line, or, when that is 0, edges moving from left x0 and right y0 to left x1
   and right y1 down the run. */
typedef struct TRANSITION_SPAN {
	Uint16 cnt; /* lines; 0 for an unused run */
	/* The (left << 8 | right) pair every line of the run takes, or 0 for the
	   sloped edges below. */
	Uint16 value;
	Uint8  x0; /* screen pixels: the left edge on the run's first line */
	Uint8  x1; /* screen pixels: the left edge the run moves toward */
	Uint8  y0; /* screen pixels: the right edge on the run's first line */
	Uint8  y1; /* screen pixels: the right edge the run moves toward */
} TRANSITION_SPAN;

/* A window transition's value: from 0, the screen covered, it opens to
   TRANSITION_OPEN and closes again to TRANSITION_CLOSED; a mode draws the
   opening half and the closing half mirrored. */
#define TRANSITION_OPEN			 32
#define TRANSITION_CLOSED		 64
/* WININ or WINOUT showing every layer in both windows (bits 6 and 14 set
   too). */
#define TRANSITION_SHOW_ALL		 0x7f7f
/* WININ or WINOUT showing every layer in its low byte (window 0, or outside
   both windows; bit 6 set too) and nothing in its high byte. */
#define TRANSITION_SHOW_LOW_BYTE 0x007f
/* WININ or WINOUT showing BG0 alone, in both bytes. */
#define TRANSITION_SHOW_BG0		 (WININ_WIN0_BG0 | WININ_WIN1_BG0)

/* One step of WINDOW_TRANSITION_SHAPE, a row of ScreenTransition_Shapes chosen
   by the progress: the WININ and WINOUT values, swapped as the transition opens
   or closes, and three runs of lines written one after another from the top of
   the screen. */
typedef struct TRANSITION_SHAPE {
	Uint16			first;	/* WINOUT while opening, WININ while closing */
	Uint16			second; /* WININ while opening, WINOUT while closing */
	TRANSITION_SPAN spans[3];
} TRANSITION_SHAPE;

extern TRANSITION_SHAPE ScreenTransition_Shapes[TRANSITION_OPEN];

/* The party leader's actor placement table (leader_placement.c), which
   fdActorResetForLeader copies into the field state. */
extern const ACTOR_PLACE Field_DefaultActorPlacements[2];

/* Curves mapping a 5-bit colour channel to the colour buffer's channel, shifted
   up 10 bits: plain, lightened and with more contrast. */
extern Uint16 ColorBuffer_ChannelCurve[];
extern Uint16 ColorBuffer_ChannelCurveLight[];
extern Uint16 ColorBuffer_ChannelCurveContrast[];

#endif
