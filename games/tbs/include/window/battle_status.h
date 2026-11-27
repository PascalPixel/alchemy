#ifndef ALCHEMY_BATTLE_STATUS_H
#define ALCHEMY_BATTLE_STATUS_H

/* The Status screen's cursors and the party status window's flags. */

#include "static_assert.h"
#include "tbs_edition.h"
#include "types.h"
#include "stubs.h"

/* A cursor stop among the Status screen's stats: its help entry, where the
   cursor points and the run of the row it highlights. */
typedef struct BATTLE_SUMMARY_STAT_CURSOR {
	Uint8 entry;		   /* the help line shown while the cursor is here */
	Uint8 column;		   /* tiles */
	Uint8 row;			   /* tiles */
	Uint8 highlightColumn; /* tiles */
	Uint8 highlightRow;	   /* tiles */
	Uint8 highlightWidth;  /* tiles */
	Uint8 reserved[2];	   /* 0 in every row */
} BATTLE_SUMMARY_STAT_CURSOR;

/* A cursor stop on the Status screen's equipment or statuses: its help entry
   and where the cursor points. */
typedef struct BATTLE_SUMMARY_CURSOR {
	Uint8 entry;	/* the help line shown while the cursor is here */
	Uint8 column;	/* tiles */
	Uint8 row;		/* tiles */
	Uint8 reserved; /* 0 in every row */
} BATTLE_SUMMARY_CURSOR;

/* A status the Status screen shows as an icon, which picks both the icon and
   its help (MsgOwnerStatusConditionHelp + condition). */
typedef enum BATTLE_SUMMARY_STAT {
	OWNER_CONDITION_EMPTY = -1, /* a place with no icon */
	OWNER_CONDITION_NORMAL = 0, /* no status: "Normal status." */
	OWNER_CONDITION_POISON = 1,
	OWNER_CONDITION_DEADLY_POISON = 2,
	OWNER_CONDITION_STUN = 3,
	OWNER_CONDITION_PSYNERGY_SEALED = 4,
	OWNER_CONDITION_SLEEP = 5,
	OWNER_CONDITION_DELUSION = 6,
	OWNER_CONDITION_HAUNT = 7,
	OWNER_CONDITION_GRIM_REAPER = 8, /* downed when its turns run out */
	OWNER_CONDITION_ATTACK_UP = 9,
	OWNER_CONDITION_ATTACK_DOWN = 10,
	OWNER_CONDITION_DEFENSE_UP = 11,
	OWNER_CONDITION_DEFENSE_DOWN = 12,
	OWNER_CONDITION_RESISTANCE_UP = 13,
	OWNER_CONDITION_RESISTANCE_DOWN = 14,
	OWNER_CONDITION_CURSED_ITEM = 15,
	OWNER_CONDITION_DOWNED = 16,
	OWNER_CONDITION_AGILITY_UP = 17,
	OWNER_CONDITION_AGILITY_DOWN = 18
} BATTLE_SUMMARY_STAT;

/* The Status screen's help lines, from MsgOwnerStatusEntryHelp, which the
   cursor stops (battle_status_cursors.c) name; the status icons' places
   follow. */
typedef enum BATTLE_SUMMARY_HELP {
	/* The name, whose help says how to switch party members with L and R. */
	OWNER_STATUS_HELP_NAME = 0,
	OWNER_STATUS_HELP_LEVEL = 1, /* no cursor stop shows it */
	OWNER_STATUS_HELP_EXPERIENCE = 2,
	OWNER_STATUS_HELP_HP = 3,
	OWNER_STATUS_HELP_PP = 4,
	OWNER_STATUS_HELP_CLASS = 5,
	OWNER_STATUS_HELP_DJINN = 6,
	OWNER_STATUS_HELP_ELEMENT_LEVEL = 7,
	OWNER_STATUS_HELP_POWER = 8,
	OWNER_STATUS_HELP_RESIST = 9,
	OWNER_STATUS_HELP_ATTACK = 10,
	OWNER_STATUS_HELP_DEFENSE = 11,
	OWNER_STATUS_HELP_AGILITY = 12,
	OWNER_STATUS_HELP_LUCK = 13,
	/* The first status icon's place: its help is its status's
	   (MsgOwnerStatusConditionHelp). */
	OWNER_STATUS_HELP_CONDITION = 14,
	OWNER_STATUS_HELP_LAST_CONDITION = 21 /* the eighth icon's place */
} BATTLE_SUMMARY_HELP;

STATIC_ASSERT(sizeof(BATTLE_SUMMARY_STAT_CURSOR) == 8);
STATIC_ASSERT(sizeof(BATTLE_SUMMARY_CURSOR) == 4);

/* The stat cursor stops: rows 0 to 8 for a character with Djinn, rows 9 to 15
   for one without (window/battle_status_cursors.c). */
extern const BATTLE_SUMMARY_STAT_CURSOR BattleStatusScreenStatCursors[];
/* The cursor stops of the screen's second group (STATUS_CURSOR_COMBAT_STATS):
   column 19, rows 1 to 4. */
extern const BATTLE_SUMMARY_CURSOR BattleStatusScreenEquipmentCursors[];
/* The eight status cursor stops, two tiles apart on row 0. */
extern const BATTLE_SUMMARY_CURSOR BattleStatusScreenConditionCursors[];
/* The level label: `Lv`, or `Niv` in French. */
extern const Uint8 BattleStatusScreenLevelString[];
extern const Uint8 BattleStatusScreenHpString[];
extern const Uint8 BattleStatusScreenSlashString[];
extern const Uint8 BattleStatusScreenPpString[];
#if BATTLE_LABELS_BY_HEADER_LANGUAGE
/* The Spanish and Italian editions' labels for a cartridge set to another
   language: the level's `Niv` (French) and `Nv.` (Spanish), and HP's `KP`
   (German) and `PV` (French and Spanish). */
extern const Uint8 BattleStatusScreenNivString[];
extern const Uint8 BattleStatusScreenNvString[];
extern const Uint8 BattleStatusScreenKpString[];
extern const Uint8 BattleStatusScreenPvString[];
#endif

/* The party status window's flags (mnPartyStatDraw, mnPartyStatCreateWindow):
   one column of six tiles per party member (name, HP and, when shown, PP with a
   bar under each), and in battle an optional five-tile column of standby Djinn
   counts on its left. */
typedef enum PARTY_STAT_FLAG {
	PARTY_STATUS_HP_ONLY = 0,	  /* names and HP */
	PARTY_STATUS_SHOW_PP = 1,	  /* add the PP row */
	PARTY_STATUS_SHOW_DJINN = 2,  /* add the Djinn column (needs the PP row) */
	PARTY_STATUS_CLOSED = 9,	  /* erase the window */
	PARTY_STATUS_KEEP_FLAGS = -1, /* redraw with the flags it has */
	/* BATTLE_STATE.partyStat while the party cannot summon. */
	PARTY_STATUS_NO_SUMMONS = PARTY_STATUS_SHOW_PP,
	/* BATTLE_STATE.partyStat while it can, drawn only while a Djinni is on
	   standby. */
	PARTY_STATUS_SUMMONS = PARTY_STATUS_SHOW_PP | PARTY_STATUS_SHOW_DJINN
} PARTY_STAT_FLAG;

#endif
