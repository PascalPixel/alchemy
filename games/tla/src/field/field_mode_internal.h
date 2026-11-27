#ifndef ALCHEMY_FIELD_MODE_INTERNAL_H
#define ALCHEMY_FIELD_MODE_INTERNAL_H

/* What every part of field_mode.c shares: its includes, and the declarations
   its parts call before their definitions or once declared differently.
   field_mode.c includes it before the parts, and each part includes it too, so
   that a part read on its own resolves. It places no bytes. */

#include "types.h"
#include "resource_ids.h"
#include "window/message_argument.h"
#include "window/window.h"
#include "field/world_map.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "scene_ids.h"
#include "field/scene_tables.h"
#include "field/actor.h"
#include "field/field_mode_script.h"
#include "field/map_grid.h"
#include "field/field_sprite.h"
#include "game/unit_hp.h"
#include "battle/battle_unit.h"
#include "game/class_djinn.h"
#include "fixed_math.h"
#include "battle/range_falloff.h"
#include "field/field_script.h"
#include "macros.h"
#include "game/item.h"
#include "battle/battle_system.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "field/objdisp.h"
#include "graphics/display_transition.h"
#include "graphics/palette_buffers.h"
#include "field/sprite.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "window/stubs.h"
#include "field/field_mode.h"
#include "field/object.h"
#include "field/stubs.h"
#include "field/encounter_zone.h"
#include "game/inventory.h"
#include "graphics/uiwindow.h"
#include "field/actor_view.h"

/* No user named: fdUsePsynergy takes the first party member, and fdUseItem
   whoever holds the item in the most inventory slots. */
#define UNIT_NONE 15

/* The running scene's entry table, at the start of OverlayArea. */
#define SCENE_ENTRIES ((SCENE_ENTRY_TABLE *)OverlayArea)

/* Outside functions and data the parts had declared each in its own way, with
   the one type they share. */

typedef void (*ITEM_CUE_CALLBACK)(Sint32 item, Sint32 actor, Sint32 slot);

/* An actor's home, as an actor placement sets it (ACTOR.homeX and homeZ). */
typedef struct SPAWN_ACTOR {
	Uint8  unused[100]; /* no code names it */
	Sint16 homeX;
	Sint16 homeZ;
} SPAWN_ACTOR;

/* An actor placement as fdAssignViewFlags reads it: its sprite, its condition
   and its place. */
typedef struct VIEWED_PLACEMENT {
	Sint16 sprite; /* -1 ends the table */
	Sint16 condition;
	Uint8  unusedAfterFlags[4]; /* no code names it */
	Sint32 x;
	Uint8  unusedAfterX[4]; /* no code names it */
	Sint32 z;
	Uint8  unusedAfterZ[4]; /* no code names it */
} VIEWED_PLACEMENT;

/* A row of the scenes' encounter rules (SceneEncounterRules): a scene and
   entrance and, while a flag is clear, the encounter zones of its two kinds of
   ground. */
typedef struct SCENE_ENCOUNTER_RULE {
	Sint16 scene;
	Sint16 entrance; /* -1 for any */
	Sint16 flags;	 /* -1 for none; the rule applies while the flag is clear */
	Uint8  groundZone; /* ordinary ground */
	Uint8  secondZone; /* MAP_METATILE_SECOND_ZONE metatiles */
} SCENE_ENCOUNTER_RULE;

/* A point that syVectorAddPolarOffset moves as three coordinates and the item
   display reads as a Vector3. */
typedef union POLAR_OFFSET_POS {
	VECTOR3 vector;
	Sint32	coordinates[3];
} POLAR_OFFSET_POS;

/* What a scene's container actor is, CONTAINER_ROW.type, named by what the
   leader checks when talking to it. */
typedef enum CONTAINER_KIND {
	CONTAINER_CHEST = 128,			/* "checked the chest..." */
	CONTAINER_MIMIC = 129,			/* a chest that is "really a Mimic!" */
	CONTAINER_PSYNERGY_STONE = 130, /* restores the party's PP */
	/* "There's something buried here!": the Scoop spot, actor 79. */
	CONTAINER_GROUND = 131,
	CONTAINER_STONE_TABLET = 132, /* a summon tablet */
	CONTAINER_POND = 133		  /* "...gazed into the pond..." */
} CONTAINER_KIND;

/* A row of the scene's container table (fdSceneItemGetRows), numbered from
   actor 64. */
typedef struct CONTAINER_ROW {
	Sint16 type; /* CONTAINER_KIND */
	/* The scene point it stands on (fdSceneItemFindPos); in Venus */
	/* Lighthouse the chest on point 0 is "already open" until its flag is
	   set. */
	Sint16 point;
	Sint16 flags; /* set once it has been opened; -1 for none */
	/* The item inside, 0 for none; a Mimic's formation slot; a Psynergy Stone's
	   number; a tablet's summon. */
	Sint16 contents;
} CONTAINER_ROW;

/* CONTAINER_ROW.contents and FIELD_STATE.scoopedItem: coins rather than an
   item, their count in the bits under it. */
#define CONTAINER_COINS			  (1 << 15)
/* The most treasure spots a scene places, each an actor from
   FIELD_TREASURE_ACTOR on. */
#define TREASURE_SPOT_MAX		  16
/* ACTOR.sceneWord of a treasure spot that stands for a map metatile rather than
   an actor: its metatile is in timer and userValue, and it stands at x and z
   0. */
#define TREASURE_SPOT_ON_METATILE 0x31415927
/* SceneItems: bit 14 marks a scene's header (SCENE_ITEMS), its scene number in
   the bits under it. */
#define SCENE_ITEMS_HEADER		  (1 << 14)

/* A box of the map in whole metatiles (16.16 when compared) and the value it
   selects while its condition holds; a row with minX -1 ends the list. */
typedef struct MAP_REGION {
	Sint16 minX;
	Sint16 minY;
	Sint16 minZ;
	Sint16 maxX;
	Sint16 maxY;
	Sint16 maxZ;
	Sint16 condition;
	Sint16 value;
} MAP_REGION;

/* Each scene's encounters by scene, entrance and flag, ended by scene -1
   (encounter_rules.s). */
extern const SCENE_ENCOUNTER_RULE SceneEncounterRules[];

extern const ENCOUNTER_ZONE EncounterZones[];

/* Each scene's music by scene or group, entrance and flag (music_rules.s). */
extern const MAP_MUSIC_RULE SceneMusicRules[];

/* Each scene's area name by scene or group and entrance (area_name_rules.s). */
extern const AREA_NAME_RULE AreaNameRules[];

extern const Sint16 SceneReturnRules[];

extern const Sint32 SceneBattleBackgroundRules[];

/* The battle music table: each song as ENCOUNTER_BATTLE_MUSIC(song), followed
   by the encounters that use it as ENCOUNTER_KEY(zone, slot), ended by 0. */
extern const Sint16 EncounterBattleMusic[];

extern Sint16 SceneItems[];

extern const Uint8 FieldExamineCueMessageOffsets[];

/* The chest sprite's animations, for the targets and treasure spot parts. */
enum {
	TREASURE_SPOT_CHEST_OPEN = 2,	 /* standing open */
	TREASURE_SPOT_CHEST_OPENING = 4, /* its lid lifting */
	/* closing again on what nobody could carry */
	TREASURE_SPOT_CHEST_CLOSING = 5,
	/* its lid lifting on nothing inside */
	TREASURE_SPOT_CHEST_OPENING_EMPTY = 6,
	TREASURE_SPOT_CHEST_MIMIC = 7 /* jumping up as a mimic */
};

#endif
