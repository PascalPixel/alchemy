/* @file */
/* Lunpa, the town the righteous thief Lunpa founded, and the Lunpa Fortress */
/* Gates above it. While Dodonpa, Donpa's son, holds Hammet in Lunpa Fortress,
 */
/* his men keep the town sealed: two guards stand outside the shut gate in the
 */
/* log palisade at the south end, where the road from the World Map ends, and */
/* turn the party away. The way in is through West Lunpa Cave, whose gate Frost
 */
/* opens; it comes out at the cave mouth in the town's north-west cliff (cell */
/* (7, 4)). */
/* */
/* Two areas share this overlay, each a scene of its own: */
/* */
/* - Lunpa (SceneId_Lunpa): the town. Doors lead into three homes, the weapon */
/*   and armor shop, the item shop, the inn, the temple and the crate room */
/*   (RockRooms, rock_rooms.c). The road out of the top of the town (cells */
/*   (13, 0) to (16, 0)) climbs to the Lunpa Fortress Gates. */
/* - Lunpa Fortress Gates (SceneId_LunpaFortressGates): the slope up to the */
/*   golden fortress door in the cliff. Two log palisades cross it, each with */
/*   a gap in the middle, and two guards stand in the yard between them, just */
/*   below the upper gap. The party slips past them under Cloak. */
/* */
/* The getters choose by MainRoutineStateWork.next. The town's triggers also */
/* choose by FLAG_DODONPA_LOCKED_UP, which Donpa's judgment in the fortress */
/* sets. Before it the townspeople live under Dodonpa's men */
/* (LunpaSealedTriggers). After it they talk of the intruders in the */
/* fortress (LunpaReopenedTriggers). Both tables share the roads and doors */
/* (LUNPA_ROADS_AND_DOORS) and Reveal's secrets (LUNPA_SECRETS). */
/* */
/* Reveal turns the town grey and shows three secrets */
/* (scLunpaRevealSecrets): a blue puddle between the two log pillars below the
 */
/* road to the gates, which Frost freezes into an ice pillar; a sparkle on the
 */
/* cliff at the west edge of town (cell (3, 16)), the switch that opens the */
/* passage to Lunpa's hidden room, searched from cell (3, 18) below it; and a */
/* Psynergy Stone, found by searching cell (7, 16), south of the inn. Every */
/* entry and the end of Reveal hide them again */
/* (scLunpaHideSecrets). Searching the sparkle opens a dark cave mouth in the */
/* cliff beside it, and the passage can be walked into only while Reveal */
/* shows it (scLunpaEnterHiddenPassage). The room beyond (StoneRooms) holds */
/* six chests. */
/* */
/* At the gates, walking uncloaked into the yard before the guards */
/* (CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_GUARDED_GROUND) makes them step into */
/* the upper gap and warn the party off. An uncloaked leader in that gap, or */
/* one still in the yard as Cloak fades, is caught and marched back down */
/* through the lower palisade (scLunpaGuardsCatchParty). */
/* */
/* A party caught inside the fortress is thrown out by the guards and down */
/* the slope (scLunpaGuardsThrowIsaacOut). After the rescue it comes out of */
/* the fortress door and Isaac sneaks past the guards under Cloak */
/* (scLunpaLeaderSneaksOut). */
/* */
/* Every arrival at the gates clears the fortress's puzzle and beaten-guard */
/* flags, so the fortress starts over. */
/* */
/* Between Donpa's judgment and the meeting with Bunza outside West Lunpa */
/* Cave (FLAG_LUNPA_CAVE_REUNION_SEEN), Garet stops Isaac from going back up */
/* through the upper gap (scLunpaGaretTurnsIsaacBack). */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "sounds.h"
#include "field/field_effect.h"
#include "scene/floating_nut.h"
#include "scene/catch_nut.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE LunpaEntrances[];
extern SCENE_ENTRANCE GateEntrances[];
extern u32 LunpaExits[];
extern u32 GateExits[];
extern ACTOR_PLACE LunpaActorPlacements[];
extern ACTOR_PLACE GateActorPlacements[];
extern TRIGGER LunpaSealedTriggers[];
extern TRIGGER LunpaReopenedTriggers[];
extern TRIGGER GateTriggers[];

/* Lunpa's and the gates' entrances and exits are in scene_exits.h. */

/* Unused: the exit of a third road out of the gates */
/* (CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_THIRD_ROAD), which no cell of the */
/* gates' map has. GateExits has no row for it either: scTakeExit, */
/* reaching the end of the gates' exits, would fall back to the event of */
/* their first exit at ENTRANCE_DEFAULT, LunpaFortressGroundFloor's default */
/* entrance. */
enum { GATE_EXIT_THIRD = 3 };

/* The first of the rooms in RockRooms that Lunpa's doors lead into. */
enum { ROOM_ENTRANCE_FIRST = 1 };

/* The RockRooms entrance a door's exit leads to. */
#define ROOM_ENTRANCE(exit) ((exit) - LUNPA_TO_FIRST_HOME + ROOM_ENTRANCE_FIRST)

/* The cell behaviors Lunpa's triggers answer. */
typedef enum LUNPA_CELL_BEHAVIOR {
	CELL_BEHAVIOR_LUNPA_WORLD_MAP_ROAD = 1,
	CELL_BEHAVIOR_LUNPA_GATE_ROAD,
	CELL_BEHAVIOR_LUNPA_CAVE_ROAD,
	CELL_BEHAVIOR_LUNPA_FIRST_HOME_DOOR,
	CELL_BEHAVIOR_LUNPA_SECOND_HOME_DOOR,
	CELL_BEHAVIOR_LUNPA_THIRD_HOME_DOOR,
	CELL_BEHAVIOR_LUNPA_ARMS_SHOP_DOOR,
	CELL_BEHAVIOR_LUNPA_ITEM_SHOP_DOOR,
	CELL_BEHAVIOR_LUNPA_INN_DOOR,
	CELL_BEHAVIOR_LUNPA_TEMPLE_DOOR,
	CELL_BEHAVIOR_LUNPA_CRATE_ROOM_DOOR,
	CELL_BEHAVIOR_LUNPA_HIDDEN_PASSAGE,
	CELL_BEHAVIOR_LUNPA_HIDDEN_SWITCH = 20,
	CELL_BEHAVIOR_LUNPA_WELL = 90,
	CELL_BEHAVIOR_LUNPA_GRAVE_SLEEP_BOMB = 100,
	CELL_BEHAVIOR_LUNPA_BARREL_NUT,
	CELL_BEHAVIOR_LUNPA_PSYNERGY_STONE
} LUNPA_CELL_BEHAVIOR;

/* The cell behaviors the Lunpa Fortress Gates' triggers answer. The gates' */
/* map has no cell of the third road, the fortress approach or the well: the */
/* approach is raised by scLunpaWatchForFortressApproach instead, and the other
 */
/* two rows never fire. */
typedef enum LUNPA_FORTRESS_GATES_CELL_BEHAVIOR {
	CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_DOOR = 1,
	CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_LUNPA_ROAD,
	CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_THIRD_ROAD,
	CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_GUARDED_GROUND = 10,
	CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH = 20,
	CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_WELL = 90
} LUNPA_FORTRESS_GATES_CELL_BEHAVIOR;

/* The event the gates' guards raise on spotting the uncloaked party. */
typedef enum LUNPA_FORTRESS_GATES_RAISED_TRIGGER {
	RAISED_PARTY_SPOTTED = 91
} LUNPA_FORTRESS_GATES_RAISED_TRIGGER;

/* Lunpa's doors that open as the leader walks in (LunpaDoors), counted from */
/* the first home's; at the temple's he goes through without first stepping */
/* into the doorway. */
enum {
	DOOR_COUNT = 8,
	DOOR_TEMPLE =
		CELL_BEHAVIOR_LUNPA_TEMPLE_DOOR - CELL_BEHAVIOR_LUNPA_FIRST_HOME_DOOR
};

/* Lunpa's placed actors, numbered in the order of LunpaActorPlacements */
/* from ACTOR_FIRST_PLACED. The townspeople are named by their sprites and by */
/* what they say before Donpa's judgment. */
typedef enum LUNPA_ACTOR_ID {
	/* The blue puddle Reveal shows and Frost freezes into an ice pillar. */
	ACTOR_HIDDEN_PUDDLE = ACTOR_FIRST_PLACED,
	/* A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR). */
	ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
	/* A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR). */
	ACTOR_WOMAN_WITH_NO_FREEDOM,
	/* A stout man with brown hair (SPRITE_STOUT_MAN). */
	ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
	/* A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS). */
	ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
	/* A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who wanders. */
	ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
	/* An old woman with white hair (SPRITE_OLD_WOMAN). */
	ACTOR_OLD_WOMAN_ON_DONPA,
	/* A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR). */
	ACTOR_BOY_ASKING_TO_SEE_DONPA,
	/* A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS), who */
	/* wanders. */
	ACTOR_GIRL_ON_THE_CHEFS_MEALS,
	/* The two soldiers in spiked helmets (SPRITE_SOLDIER_IN_SPIKED_HELMET); */
	/* this one keeps the road in from the World Map. */
	ACTOR_WEST_GUARD,
	ACTOR_EAST_GUARD, /* wants Dodonpa's authorization */
	/* The hidden switch's sparkle, shown by Reveal. */
	ACTOR_SWITCH_SPARKLE,
	/* The Nut that floats over the town, for Catch. */
	ACTOR_FLOATING_NUT
} LUNPA_ACTOR_ID;

/* The Lunpa Fortress Gates' placed actors, numbered in the order of */
/* GateActorPlacements from ACTOR_FIRST_PLACED. Garet's row, the third, */
/* takes his own actor, GARET, as every row of a party member's sprite does. */
typedef enum GATE_ACTOR_ID {
	ACTOR_LEFT_GUARD = ACTOR_FIRST_PLACED,
	ACTOR_RIGHT_GUARD
} GATE_ACTOR_ID;

/* How the switch's sparkle is stretched while Reveal shows it. */
enum {
	SPARKLE_SCALE_X = 6 * FIXED_ONE / 5,
	SPARKLE_SCALE_Y = 3 * FIXED_ONE / 2
};

/* Where Retreat at the gates sends the party */
/* (MAIN_ROUTINE_STATE.retreatEntrance). */
enum { GATE_RETREAT_ENTRANCE = 10 };

/* The gate guards' lines when they catch the party uncloaked, in order. */
typedef enum CATCH_MESSAGE {
	CATCH_LEFT_GUARD_CHALLENGES,
	CATCH_RIGHT_GUARD_WONDERS,
	CATCH_LEFT_GUARD_REFUSES_ENTRY,
	CATCH_RIGHT_GUARD_SENDS_PARTY_OFF
} CATCH_MESSAGE;

/* The left guard's two lines once he has seen Hammet with the party. */
typedef enum RECOGNITION_MESSAGE {
	RECOGNITION_SEEN_THAT_MAN,
	RECOGNITION_IMPOSSIBLE
} RECOGNITION_MESSAGE;

/* The leader lying where the guards threw him down the steps. */
enum { ANIM_SPRAWLED = 19 };

/* How high the ice pillar's top is, and a cell: 16 pixels each. */
enum { PILLAR_TOP_HEIGHT = toFIXED(16), CELL_SIZE = toFIXED(16) };

/* The treasure spot of the town's Psynergy Stone. */
enum { TREASURE_SPOT_PSYNERGY_STONE = 102 };

extern MAP_GRID_CELL_ANIMATION LunpaDoors[];

static void scLunpaWatchForUncloakedParty(void);

/* Unused: nothing in the scene reads them. */
enum { PALETTE_TRANSLUCENT_EFFECT = 14, PALETTE_EFFECT = 15 };

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/* Frost has frozen the hidden puddle into an ice pillar to stand on. */
#define FLAG_LUNPA_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
/* The town's secrets are hidden: Reveal is not shining. */
#define FLAG_LUNPA_SECRETS_HIDDEN FLAG_SCENE_TEMP_01
/* The hidden switch has opened the passage to the hidden room. */
#define FLAG_LUNPA_HIDDEN_PASSAGE_OPEN FLAG_SCENE_TEMP_02
/* The guards have stepped in front of the fortress door. */
#define FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR FLAG_SCENE_TEMP_40
/* Cloak has been cast at the gates and has not yet faded. */
#define FLAG_LUNPA_FORTRESS_GATES_CLOAK_CAST FLAG_SCENE_TEMP_41
/* The leader is going through the fortress door. */
#define FLAG_LUNPA_FORTRESS_GATES_ENTERING_FORTRESS FLAG_SCENE_TEMP_42
/* Garet's warning against going back up to the fortress is under way. */
#define FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING FLAG_SCENE_TEMP_43
/* The guards have caught the party and are marching it down the steps. */
#define FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY FLAG_SCENE_TEMP_44

#include "shared/field_effect_duration_scripts.c"

const s32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

const SCENE_ENTRANCE *scGetEntrances(void) {
	s32 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_LUNPA) {
		return LunpaEntrances;
	}
	if(sceneId == SCENE_LUNPA_FORTRESS_GATES) {
		return GateEntrances;
	}
	return LunpaEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

const u32 *scGetExits(void) {
	s32 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_LUNPA) {
		return LunpaExits;
	}
	if(sceneId == SCENE_LUNPA_FORTRESS_GATES) {
		return GateExits;
	}
	return LunpaExits;
}

/* The townsfolk of Lunpa, or the guards of the Lunpa Fortress Gates. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	s32 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_LUNPA) {
		return LunpaActorPlacements;
	}
	if(sceneId == SCENE_LUNPA_FORTRESS_GATES) {
		return GateActorPlacements;
	}
	return LunpaActorPlacements;
}

/* Catch on the Nut in the tree just west of Lunpa's shut south gate, outside */
/* the palisade, brings it down to Isaac. */
static void scLunpaCatchFloatingNut(void) {
	scCatchNut(ACTOR_FLOATING_NUT, FLAG_LUNPA_NUT_CAUGHT);
}

/* Makes the frozen puddle's cell, (14, 4), and the cells either side of it */
/* into the ice pillar's top and sides, from copies kept at (17, 4) and */
/* (15, 3). */
static inline void scLunpaApplyFrozenPuddleCollision(void) {
	fdMapGridCopyCellAttributes(17, 4, 1, 1, 14, 4);
	fdMapGridCopyCellAttributes(15, 3, 1, 1, 15, 4);
	fdMapGridCopyCellAttributes(15, 3, 1, 1, 13, 4);
}

/* Frost on the blue puddle that Reveal shows between the two log pillars at */
/* the top of Lunpa (cell (14, 4)) freezes it into an ice pillar to stand on. */
static void scLunpaFreezeHiddenPuddle(void) {
	ACTOR *pillar;

	pillar = fdActorGet(ACTOR_HIDDEN_PUDDLE);
	/* DEAD: the leader looked up and dropped; the ROM keeps the call. */
	fdActorGet(ACTOR_PARTY_LEADER);
	scLunpaApplyFrozenPuddleCollision();
	if(pillar != NULL) {
		fdActorSetSprFlags(pillar, 0);
		pillar->motionFlg = ACTOR_FALLS;
		pillar->priFlg = ACTOR_PRIORITY_AUTOMATIC;
	}
	gmFlagSet(FLAG_LUNPA_PUDDLE_FROZEN);
}

/* Every frame while Reveal shows the puddle: once Isaac is up on the ice */
/* pillar, it is drawn under him. */
static void scLunpaUpdateIcePillarDrawOrder(void) {
	if(fdActorGet(ACTOR_PARTY_LEADER)->y.fixed >= PILLAR_TOP_HEIGHT) {
		fdActorGet(ACTOR_HIDDEN_PUDDLE)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	}
	else {
		fdActorGet(ACTOR_HIDDEN_PUDDLE)->priFlg = ACTOR_PRIORITY_AUTOMATIC;
	}
}

/* Draws one picture of the hidden passage's cave mouth in the cliff at the */
/* west edge of town, from a set of copies kept at (sourceColumn, sourceRow). */
/* The second and third copies are the same mouth on the map's other layers. */
static inline void scLunpaDrawHiddenPassagePicture(s32 sourceColumn,
												   s32 sourceRow) {
	fdMapGridCopyCells(sourceColumn, sourceRow, 3, 4, 1, 14);
	fdMapGridCopyCells(sourceColumn + 3, sourceRow, 3, 4, 33, 14);
	fdMapGridCopyCells(sourceColumn + 6, sourceRow, 3, 4, 1, 46);
}

/* Casting Reveal in Lunpa turns the town grey and shows its secrets: the blue
 */
/* puddle between the log pillars at the top of town, the sparkle of the */
/* hidden switch on the west cliff (or, once it has been pressed, the cave */
/* mouth it opened), and the Psynergy Stone's spot at cell (7, 16). Isaac */
/* standing on that spot is first walked a step south off it. */
static void scLunpaRevealSecrets(void) {
	ACTOR *actor;
	s32 cellX;
	s32 cellZ;

	actor = fdActorGet(ACTOR_PARTY_LEADER);
	cellX = actor->x.fixed / CELL_SIZE;
	cellZ = actor->z.fixed / CELL_SIZE;
	if(gmFlagGet(FLAG_LUNPA_PSYNERGY_STONE) == 0) {
		if(cellX == 7 && cellZ == 16) {
			fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 16);
		}
		fdTreasureSpotSetPos(TREASURE_SPOT_PSYNERGY_STONE, -1, -1);
		/* The stone's cell takes the attributes kept for it at (28, 31). */
		fdMapGridCopyCellAttributes(28, 31, 1, 1, 7, 16);
	}
	/* The puddle's cell on the middle layer (the map's columns from 32), and
	 * the */
	/* cells round it, (13, 3) to (15, 5), take the copies kept for Reveal. */
	fdMapGridCopyCells(47, 4, 1, 1, 46, 4);
	fdMapGridCopyCellAttributes(34, 37, 3, 3, 13, 3);
	fdActorSetPos(ACTOR_HIDDEN_PUDDLE, 232, 72);
	fdActorGet(ACTOR_HIDDEN_PUDDLE)->y.fixed = 0;
#if TBS_PATCH_4
	if(gmFlagGet(FLAG_LUNPA_PUDDLE_FROZEN)) {
		scLunpaApplyFrozenPuddleCollision();
	}
#endif
	/* The opened passage replaces the sparkle; otherwise show the hidden
	 * switch. */
	if(gmFlagGet(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN) != 0) {
		scLunpaDrawHiddenPassagePicture(41, 49);
	}
	else {
		fdActorSetPos(ACTOR_SWITCH_SPARKLE, 56, 268);
		fdActorSetSprFlags(fdActorGet(ACTOR_SWITCH_SPARKLE), 0);
		actor = fdActorGet(ACTOR_SWITCH_SPARKLE);
		if(actor != NULL) {
			actor->motionFlg = ACTOR_FLOATS_HIGH;
			actor->y.fixed = toFIXED(16);
			actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
			actor->scaleX = SPARKLE_SCALE_X;
			actor->scaleY = SPARKLE_SCALE_Y;
		}
	}
	syTaskAddFrame(scLunpaUpdateIcePillarDrawOrder, TASK_PASS_FRAME);
	gmFlagClear(FLAG_LUNPA_SECRETS_HIDDEN);
}

/* As Reveal ends, and on every arrival in Lunpa, the puddle, the sparkle, the
 */
/* cave mouth and the Psynergy Stone's spot are hidden again, and a frozen */
/* puddle thaws. */
static void scLunpaHideSecrets(void) {
	/* DEAD: the leader looked up and dropped; the ROM keeps the call. */
	fdActorGet(ACTOR_PARTY_LEADER);
	fdActorSetPos(ACTOR_SWITCH_SPARKLE, 0, 0);
	fdActorSetPos(ACTOR_HIDDEN_PUDDLE, 0, 0);
	/* The puddle's cell and the cells round it as they are without it, from the
	 */
	/* copies kept at (38, 38) and (37, 37), whose attributes (14, 2) takes too.
	 */
	fdMapGridCopyCells(38, 38, 1, 1, 46, 4);
	fdMapGridCopyCellAttributes(37, 37, 3, 3, 13, 3);
	fdMapGridCopyCellAttributes(37, 37, 1, 1, 14, 2);
	/* The stone's cell plain again, as the cell east of it. */
	fdMapGridCopyCellAttributes(8, 16, 1, 1, 7, 16);
	fdTreasureSpotSetPos(TREASURE_SPOT_PSYNERGY_STONE, 0, 0);
	/* The cave mouth's lower cells, (1, 15) to (3, 16), shut again from the */
	/* copy kept at (32, 42). */
	fdMapGridCopyCells(32, 42, 3, 2, 1, 15);
	gmFlagClear(FLAG_LUNPA_PUDDLE_FROZEN);
	fdActorSetAnimation(ACTOR_HIDDEN_PUDDLE, ANIM_STAND);
	fdActorGet(ACTOR_HIDDEN_PUDDLE)->update = NULL;
	fdActorSetColMode(fdActorGet(ACTOR_HIDDEN_PUDDLE), 0);
	syCloseEvent(scLunpaUpdateIcePillarDrawOrder);
	gmFlagSet(FLAG_LUNPA_SECRETS_HIDDEN);
}

/* Searching the well in the middle of Lunpa (cells (16, 13) to (17, 14)), */
/* Isaac peers in and hears frogs inside. The gates' table has a row for a */
/* well too, but no cell there answers it. */
static void scLunpaSearchWell(void) {
	fdEventStart();
	mnFieldMessageShow(MsgFieldPeeredWell, 1);
	mnFieldMessageShow(MsgLunpaWellFrogs, 1);
	fdEventEnd();
}

/* The triggers of the Lunpa Fortress Gates, or the town's before and after */
/* Donpa's judgment. */
const TRIGGER *scGetTriggers(void) {
	s32 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_LUNPA_FORTRESS_GATES) {
		/* DEAD: the flag read and dropped; the ROM keeps the call. */
		gmFlagGet(FLAG_DODONPA_LOCKED_UP);
		return GateTriggers;
	}
	if(sceneId == SCENE_LUNPA && gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0) {
		return LunpaReopenedTriggers;
	}
	return LunpaSealedTriggers;
}

/* Before Donpa's judgment, the guard on the left of Lunpa's shut south gate */
/* winces and asks whether the party wants to enter Lunpa, and sends them back
 */
/* where they came from whatever they answer. */
static void scLunpaTalkToWestGuard(void) {
	fdEventStart();
	fdActorShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdEventSetMessageId(MsgLunpaWestGuardAsksAboutEntering);
	/* "What's with you? You wanna enter Lunpa?" */
	/* Yes: "Well, you can't! Go back to where you came from!" */
	/* No: "You came all this way for nothing? That doesn't make any..." */
	fdEventAskYesNo(ACTOR_WEST_GUARD, 0);
	fdEventEnd();
}

/* Before Donpa's judgment, the guard on the right of Lunpa's shut south gate */
/* says no one may enter without Dodonpa's authorization. Once Hammet's */
/* caravan has gone north to Lunpa (FLAG_LUNPA_GUARDS_SUSPECT_KALAY), he asks */
/* instead whether the party is from Kalay, and then whether they have come to
 */
/* save Hammet, and warns them off by their answers. */
static void scLunpaTalkToEastGuard(void) {
	fdEventStart();
	if(gmFlagGet(FLAG_LUNPA_GUARDS_SUSPECT_KALAY) == 0) {
		fdEventSetMessageId(MsgLunpaEastGuardDemandsAuthorization);
		/* "This is Lunpa. You cannot enter without Dodonpa's authorization." */
		fdEventShowMessage(ACTOR_EAST_GUARD, 0);
	}
	else {
		fdEventSetMessageId(MsgLunpaEastGuardAsksIfFrom);
		/* "You're not from Kalay, are you?" */
		fdEventOpenMessage(ACTOR_EAST_GUARD, 0);
		if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			FieldStateWork->msg++;
			/* "You're not here to save Hammet, are you?" */
			fdEventOpenMessage(ACTOR_EAST_GUARD, 0);
			if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
				FieldStateWork->msg++;
			}
		}
		fdEventShowMessage(ACTOR_EAST_GUARD, 0);
	}
	fdEventEnd();
}

/* Mind Read on the guard on the right of Lunpa's south gate. Before Donpa's */
/* judgment he fears Dodonpa will blame the guards for any trouble outsiders */
/* cause, or, once Hammet's caravan has come, trusts that the gate in the cave
 */
/* opens only from the inside; after it he thinks the merchant is harmless. */
static void scLunpaReadEastGuardsThoughts(void) {
	s32 thoughts;

	if(gmFlagGet(FLAG_DODONPA_LOCKED_UP) == 0) {
		thoughts = gmFlagGet(FLAG_LUNPA_GUARDS_SUSPECT_KALAY);
		if(thoughts == 0) {
			/* "If outsiders cause any trouble in the village, Dodonpa will..."
			 */
			thoughts = MsgLunpaEastGuardFearsBlame;
		}
		else {
			/* "Even if they enter through the cave, they can't get into..." */
			thoughts = MsgLunpaEastGuardTrustsCaveGate;
		}
		fdEventSetMessageId(thoughts);
		fdEventShowMessage(ACTOR_EAST_GUARD, 0);
	}
	else {
		fdEventSetMessageId(MsgLunpaEastGuardThinksMerchantHarmless);
		/* "That merchant was desperate to do business in the village...." */
		fdEventShowMessage(ACTOR_EAST_GUARD, 0);
	}
}

/* Before Donpa's judgment, a young man with brown hair near the inn asks */
/* whether the party has been in Lunpa long, and takes a No to mean they are */
/* new members of Dodonpa's gang. */
static void scLunpaAskAboutLengthOfStay(void) {
	fdEventStart();
	fdEventSetMessageId(MsgLunpaVillagerAAsksHowLong);
	/* "Have you been in Lunpa long?" */
	/* Yes: "Hmm... Your face doesn't ring a bell. I guess you don't..." */
	/* No: "Oh, so you're a new member of Dodonpa's gang?" */
	fdEventAskYesNo(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA, 0);
	fdEventEnd();
}

/* Before Donpa's judgment, a stout man in Lunpa asks whether it is true that */
/* Dodonpa kidnapped Hammet and extorted ransom for him. */
static void scLunpaAskAboutHammetsKidnapping(void) {
	fdEventStart();
	fdEventSetMessageId(MsgLunpaVillagerCAsksAboutKidnapping);
	/* "Is it true that Dodonpa kidnapped Hammet and extorted ransom..." */
	/* Yes: "Then I guess Dodonpa's men were telling the truth." */
	/* No: "I didn't think so. Not even Dodonpa would do such a terrible..." */
	fdEventAskYesNo(ACTOR_MAN_ON_HAMMETS_KIDNAPPING, 0);
	fdEventEnd();
}

/* Before Donpa's judgment, a small boy at the top of Lunpa asks whether the */
/* party wants to see Donpa: he is in the fortress, or, on a No, impossible to
 */
/* see now. */
static void scLunpaAskAboutSeeingDonpa(void) {
	fdEventStart();
	fdEventSetMessageId(MsgLunpaVillagerGAsksAboutDonpa);
	/* "You want to see Donpa?" */
	/* Yes: "Donpa is in the fortress. You'll have to go there if you..." */
	/* No: "I didn't think so... It's impossible to see Donpa now." */
	fdEventAskYesNo(ACTOR_BOY_ASKING_TO_SEE_DONPA, 0);
	fdEventEnd();
}

/* After Donpa's judgment, a woman with fair hair in Lunpa shivers and */
/* complains that it has gotten colder. */
static void scLunpaComplainAboutCold(void) {
	fdEventStart();
	fdActorRunRepeatedMotion(ACTOR_WOMAN_WITH_NO_FREEDOM, 3);
	fdEventSetMessageId(MsgLunpaVillagerBShivers);
	/* "It's gotten colder... I'm shiverin' just standin' here." */
	fdEventShowMessage(ACTOR_WOMAN_WITH_NO_FREEDOM, 0);
	fdEventEnd();
}

/* After Donpa's judgment, a woman in a green dress in Lunpa offers to tell */
/* what caused the commotion at the fortress: someone snuck in to save Hammet */
/* and ran away. */
static void scLunpaAskAboutFortressCommotion(void) {
	fdEventStart();
	fdEventSetMessageId(MsgLunpaVillagerDAsksAboutCommotion);
	/* "Do you want to know what caused the commotion at the fortress?" */
	/* Yes: "Someone snuck into the fortress to save the merchant Hammet!..." */
	/* No: "They're foolhardy to invade such a well- guarded fortress!..." */
	fdEventAskYesNo(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD, 0);
	fdEventEnd();
}

/* Talking to the left guard at the Lunpa Fortress Gates under Cloak, he */
/* could swear he just heard someone. Uncloaked, he grumbles that Dodonpa is */
/* devious; after Donpa's judgment, until the meeting with Bunza, he thinks */
/* he has seen the man with the party before, then decides it is impossible. */
static void scLunpaTalkToLeftGuard(void) {
	s32 recognitionMessageBase;

	if(MainRoutineStateWork.cloaked != 0) {
		fdEventSetMessageId(MsgLunpaLeftGuardHearsSomeone);
	}
	else if(gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0 &&
			gmFlagGet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
		fdActorShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
		recognitionMessageBase = MsgLunpaLeftGuardRecognizesHammet;
		fdEventSetMessageId(recognitionMessageBase + RECOGNITION_SEEN_THAT_MAN);
		fdEventShowMessage(ACTOR_LEFT_GUARD, 0);
		fdActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
		fdEventSetMessageId(recognitionMessageBase + RECOGNITION_IMPOSSIBLE);
		gmFlagSet(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
	}
	else {
		fdEventSetMessageId(MsgLunpaLeftGuardResentsDodonpa);
	}
	fdEventShowMessage(ACTOR_LEFT_GUARD, 0);
}

/* Talking to the right guard at the Lunpa Fortress Gates, he boasts that not */
/* even a mouse could get in while he is on watch. Under Cloak he answers his */
/* partner that the place is kind of creepy; after Donpa's judgment he */
/* wonders how the warriors from Kalay got in. */
static void scLunpaTalkToRightGuard(void) {
	if(MainRoutineStateWork.cloaked != 0) {
		fdEventSetMessageId(MsgLunpaRightGuardFeelsCreepy);
	}
	else if(gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0) {
		fdEventSetMessageId(MsgLunpaRightGuardWondersHow);
	}
	else {
		fdEventSetMessageId(MsgLunpaRightGuardBoasts);
	}
	fdEventShowMessage(ACTOR_RIGHT_GUARD, 0);
}

/* Mind Read on the left guard at the Lunpa Fortress Gates: he thinks keeping */
/* Hammet to squeeze money out of Kalay is genius. After Donpa's judgment, */
/* until the meeting with Bunza, he turns to Isaac sure he has seen that man */
/* with him before (later, that the man following them looks like that */
/* merchant), then dismisses the thought. */
static void scLunpaReadLeftGuardsThoughts(void) {
	s32 recognizedHammet;

	if(MainRoutineStateWork.cloaked == 0 &&
	   gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0 &&
	   gmFlagGet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
		recognizedHammet = fdEventGetFlag(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
		if(recognizedHammet == 0) {
			FieldStateWork->psynergy = recognizedHammet;
			fdPsynergyStopMindReadWave();
			fdActorFaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
			fdActorShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION,
							 60);
			fdEventSetMessageId(MsgLunpaLeftGuardRecognizesHammet);
			gmFlagSet(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
		}
		else {
			fdEventSetMessageId(MsgLunpaLeftGuardGlimpsedMerchant);
		}
		fdEventShowMessage(ACTOR_LEFT_GUARD, 0);
		fdActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
		fdEventSetMessageId(MsgLunpaLeftGuardDismissesThought);
	}
	else {
		fdEventSetMessageId(MsgLunpaLeftGuardThoughts);
	}
	fdEventShowMessage(ACTOR_LEFT_GUARD, 0);
}

/* Mind Read on the right guard at the Lunpa Fortress Gates: you would have */
/* to be a shadow to sneak past him. After Donpa's judgment he can't see how */
/* anyone got by them. */
static void scLunpaReadRightGuardsThoughts(void) {
	if(gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0) {
		fdEventSetMessageId(MsgLunpaRightGuardReopenedThoughts);
	}
	else {
		fdEventSetMessageId(MsgLunpaYoudShadowSneak);
	}
	fdEventShowMessage(ACTOR_RIGHT_GUARD, 0);
}

/* Every frame at the Lunpa Fortress Gates between Donpa's judgment and the */
/* meeting with Bunza: Isaac stepping into the upper palisade's gap (map row */
/* 10) raises Garet's warning, scLunpaGaretTurnsIsaacBack. */
static void scLunpaWatchForFortressApproach(void) {
	s32 leaderRow = fdActorGet(ACTOR_PARTY_LEADER)->z.fixed / CELL_SIZE;

	if(fdEventGetFlag(FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING) == 0 &&
	   leaderRow == 10) {
		gmFlagSet(FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING);
		FieldStateWork->stepTrigger =
			CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH;
	}
}

/* After Donpa's judgment, walking up into the gap in the upper palisade at */
/* the Lunpa Fortress Gates, Garet reminds Isaac they can't go in there, and */
/* Isaac winces and steps back. */
static void scLunpaGaretTurnsIsaacBack(void) {
	fdEventStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdEventSetMessageId(MsgLunpaGaretRefusesToReturn);
	/* "Isaac! Don't be stupid! We can't go in there, remember?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 100);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 12);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	gmFlagClear(FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING);
	fdEventEnd();
}

/* Walking north into one of Lunpa's doors, it swings open and Isaac goes */
/* through into the home, shop, inn, temple or crate room behind it. The */
/* townspeople stand still meanwhile. */
static void scLunpaEnterDoor(void) {
	FIELD_STATE *state;
	ACTOR *placedActor;
	u32 actorId;
	s32 doorIndex;

	state = FieldStateWork;
	fdEventStart();
	/* Freeze placed actors before opening the selected door and crossing */
	/* it. */
	for(actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_ID; actorId++) {
		placedActor = fdActorGet(actorId);
		if(placedActor != NULL) {
			placedActor->motionFlg = 0;
		}
	}
	sySndPlay(SE_DOOR_OPEN);
	doorIndex = state->stepTrigger - CELL_BEHAVIOR_LUNPA_FIRST_HOME_DOOR;
	fdMapGridAnimateCells(LunpaDoors[doorIndex].steps, LunpaDoors[doorIndex].x,
						  LunpaDoors[doorIndex].y);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	if(doorIndex != DOOR_TEMPLE) {
		fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
		fdEventWait(10);
	}
	syImportEventSetExitRequest(state->stepTrigger);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* Walking north into the cave mouth the hidden switch opened in Lunpa's west */
/* cliff (cell (2, 19)) takes Isaac into Lunpa's hidden room. It works only */
/* while Reveal shows the mouth; otherwise nothing happens there. */
static void scLunpaEnterHiddenPassage(void) {
	if(gmFlagGet(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN) != 0) {
		if(gmFlagGet(FLAG_LUNPA_SECRETS_HIDDEN) == 0) {
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
			fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
			fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
			fdEventWait(13);
			syImportEventSetExitRequest(LUNPA_TO_STONE_ROOMS);
		}
	}
}

/* Walking up to the golden door in the cliff at the top of the Lunpa */
/* Fortress Gates (cells (9, 5) and (10, 5)), it opens and Isaac goes into */
/* Lunpa Fortress. */
static void scLunpaEnterFortress(void) {
	gmFlagSet(FLAG_LUNPA_FORTRESS_GATES_ENTERING_FORTRESS);
	fdEventStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorWalkBy(ACTOR_PARTY_LEADER, 0, -8);
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridCopyCells(53, 4, 2, 2, 41, 4);
	fdEventWait(10);
	fdMapGridCopyCells(53, 6, 2, 2, 41, 4);
	fdEventWait(10);
	syImportEventSetExitRequest(
		LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_GROUND_FLOOR);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

static void UNUSED scLunpaTryJumpForward(void) { fdTryJumpForward(); }

/* Unused: plays the treasure jingle while Reveal is shining. Nothing calls */
/* it. */
UNUSED static void scLunpaPlayRevealCue(void) {
	if(gmFlagGet(FLAG_LUNPA_SECRETS_HIDDEN) == 0) {
		sySndPlay(MUS_TREASURE_FOUND);
	}
}

/* Searching the sparkle Reveal shows on Lunpa's west cliff, from cell (3, 18)
 */
/* below it, makes the rock crumble away into a dark cave mouth, the passage */
/* to Lunpa's hidden room. */
static void scLunpaPressHiddenSwitch(void) {
	if(gmFlagGet(FLAG_LUNPA_SECRETS_HIDDEN) != 0) {
		return;
	}
	if(fdEventGetFlag(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN) != 0) {
		return;
	}
	fdActorSetPos(ACTOR_SWITCH_SPARKLE, 0, 0);
	sySndPlay(SE_HIDDEN_PASSAGE_OPEN);
	sySynch(1);
	scLunpaDrawHiddenPassagePicture(32, 45);
	sySynch(10);
	scLunpaDrawHiddenPassagePicture(41, 45);
	sySynch(10);
	scLunpaDrawHiddenPassagePicture(50, 45);
	sySynch(10);
	scLunpaDrawHiddenPassagePicture(32, 49);
	sySynch(10);
	scLunpaDrawHiddenPassagePicture(41, 49);
	sySynch(10);
	gmFlagSet(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN);
}

/* Copies the cell at (sourceColumn, sourceRow) over the three cells (7, 11) */
/* to (9, 11) at the Lunpa Fortress Gates, just below the west half of the */
/* upper palisade. */
static inline void scLunpaCopyGatewayCollision(s32 sourceColumn,
											   s32 sourceRow) {
	fdMapGridCopyCellAttributes(sourceColumn, sourceRow, 1, 1, 7, 11);
	fdMapGridCopyCellAttributes(sourceColumn, sourceRow, 1, 1, 8, 11);
	fdMapGridCopyCellAttributes(sourceColumn, sourceRow, 1, 1, 9, 11);
}

/* Walking uncloaked into the yard between the palisades at the Lunpa */
/* Fortress Gates (cells (7, 11) to (14, 14)), the two guards cry out, step up
 */
/* into the upper palisade's gap and warn the party not to come any closer to */
/* the fortress. The three cells west of them are rewritten as Cloak's cast */
/* does. */
static void scLunpaGuardsBlockGate(void) {
	if(MainRoutineStateWork.cloaked == 0) {
		fdEventStart();
		fdActorShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
						 2);
		fdActorShowEmote(ACTOR_RIGHT_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
						 15);
		fdEventWait(30);
		fdActorWalkTo(ACTOR_LEFT_GUARD, 152, 168);
		fdActorWalkTo(ACTOR_RIGHT_GUARD, 168, 168);
		fdActorWaitForMoveById(ACTOR_LEFT_GUARD);
		fdActorWaitForMoveById(ACTOR_RIGHT_GUARD);
		fdActorStop(ACTOR_LEFT_GUARD);
		fdActorSetAnimation(ACTOR_LEFT_GUARD, ANIM_STILL);
		fdActorFaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTH_SOUTHEAST, 0);
		fdActorStop(ACTOR_RIGHT_GUARD);
		fdActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_STILL);
		fdActorFaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH_SOUTHWEST, 0);
		fdEventSetMessageId(MsgLunpaGuardsWarnPartyAway);
		/* "Don't come any closer to the fortress!" */
		fdEventShowMessage(ACTOR_LEFT_GUARD, 0);
		gmFlagSet(FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR);
		scLunpaCopyGatewayCollision(6, 11);
		fdEventEnd();
	}
}

static void UNUSED scLunpaDoNothing(void) {}

/* Casting Cloak at the Lunpa Fortress Gates rewrites the three cells (7, 11) */
/* to (9, 11) from the one at (6, 11); scLunpaCloakFades puts them back. */
static void scLunpaCastCloak(void) {
	scLunpaCopyGatewayCollision(6, 11);
	gmFlagSet(FLAG_LUNPA_FORTRESS_GATES_CLOAK_CAST);
}

/* When the guards at the Lunpa Fortress Gates spot the party, they turn and */
/* shout who are you, wonder how anyone got past them, and march Isaac back */
/* out through the gap in the lower palisade. */
static void scLunpaGuardsCatchParty(void) {
	ACTOR *leader;
	s32 warningMessageBase;

	if(fdEventGetFlag(FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY) != 0) {
		return;
	}
	gmFlagSet(FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY);
	fdEventStart();
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	fdActorFaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
	fdActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
	fdActorStartRepeatedMotion(ACTOR_RIGHT_GUARD, 1);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* Warn the party before escorting everyone down the two sections of the
	 * gate. */
	warningMessageBase = MsgLunpaGuardsCatchParty;
	fdEventSetMessageId(warningMessageBase + CATCH_LEFT_GUARD_CHALLENGES);
	fdEventShowMessage(ACTOR_LEFT_GUARD, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
	fdActorSetSpeed(ACTOR_LEFT_GUARD, 2);
	fdActorSetSpeed(ACTOR_RIGHT_GUARD, 2);
	fdActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_SHAKE_HEAD);
	fdEventWait(35);
	fdEventSetMessageId(warningMessageBase + CATCH_RIGHT_GUARD_WONDERS);
	fdEventShowMessage(ACTOR_RIGHT_GUARD, 0);
	fdActorShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	fdEventSetMessageId(warningMessageBase + CATCH_LEFT_GUARD_REFUSES_ENTRY);
	fdEventShowMessage(ACTOR_LEFT_GUARD, 0);
	fdActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_NOD);
	fdEventWait(25);
	fdEventSetMessageId(warningMessageBase + CATCH_RIGHT_GUARD_SENDS_PARTY_OFF);
	fdEventShowMessage(ACTOR_RIGHT_GUARD, 0);
	fdActorWalkTo(ACTOR_LEFT_GUARD, leader->x.part[0].integer - 1,
				  leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_LEFT_GUARD);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 160, 216);
	fdActorWalkTo(ACTOR_LEFT_GUARD, 152, 200);
	fdActorWalkTo(ACTOR_RIGHT_GUARD, 168, 200);
	fdActorWaitForMoveById(ACTOR_LEFT_GUARD);
	fdActorWaitForMoveById(ACTOR_RIGHT_GUARD);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorFaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
	fdEventWait(12);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 160, 272);
	fdActorWalkTo(ACTOR_LEFT_GUARD, 152, 256);
	fdActorWalkTo(ACTOR_RIGHT_GUARD, 168, 256);
	fdActorWaitForMoveById(ACTOR_LEFT_GUARD);
	fdActorWaitForMoveById(ACTOR_RIGHT_GUARD);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdEventEnd();
	syTaskAddFrame(scLunpaWatchForUncloakedParty, TASK_PASS_FRAME);
}

/* Puts back the three cells (7, 11) to (9, 11) that Cloak and the guards at */
/* the door rewrite, from the cell at (7, 12). */
static void scLunpaReopenGateway(void) { scLunpaCopyGatewayCollision(7, 12); }

/* As Cloak fades at the Lunpa Fortress Gates, a party still in the yard */
/* between the palisades is caught by the guards. */
static void scLunpaCloakFades(void) {
#if TBS_PATCH_1
	FIELD_STATE *state;
#endif
	ACTOR *leader;

#if TBS_PATCH_1
	state = FieldStateWork;
#endif
	gmFlagClear(FLAG_LUNPA_FORTRESS_GATES_CLOAK_CAST);
	gmFlagClear(FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader->x.fixed > toFIXED(104) && leader->x.fixed < toFIXED(240) &&
	   leader->z.fixed > toFIXED(160) && leader->z.fixed < toFIXED(248)) {
		syCloseEvent(scLunpaWatchForUncloakedParty);
#if TBS_PATCH_1
		state->raisedTrigger = RAISED_PARTY_SPOTTED;
#else
		scLunpaGuardsCatchParty();
#endif
	}
	scLunpaReopenGateway();
	gmFlagClear(FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY);
}

/* Every frame at the Lunpa Fortress Gates: an uncloaked Isaac in the gap of */
/* the upper palisade is spotted by the guards. */
static void scLunpaWatchForUncloakedParty(void) {
	FIELD_STATE *state;
	ACTOR *leader;

	state = FieldStateWork;
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(MainRoutineStateWork.cloaked == 0 &&
	   (leader->x.fixed >= toFIXED(144) && leader->x.fixed <= toFIXED(176)) &&
	   leader->z.fixed >= toFIXED(168) && leader->z.fixed < toFIXED(176)) {
		syCloseEvent(scLunpaWatchForUncloakedParty);
		state->raisedTrigger = RAISED_PARTY_SPOTTED;
	}
}

/* Every 16 frames while Isaac is dragged and thrown: a puff of dust at his */
/* feet. */
static void scLunpaKickUpLeaderDust(void) {
	ACTOR *leader;
	s32 dustAngle;
	s32 dustVelocity[3];

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if((FrameCount & 15) == 0) {
		dustAngle = (syRandomBelow(52)) * 64 + 230;
		SET_VECTOR(dustVelocity, syCos(dustAngle) / 4, 0, sySin(dustAngle) / 2);
		fdEffectSpawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
					  dustVelocity[0], dustVelocity[1], dustVelocity[2], 0,
					  NULL);
	}
}

/* Caught inside Lunpa Fortress, Isaac is dragged out of the door by the two */
/* guards, struggling, down through both palisades, and thrown sprawling onto */
/* the slope below. And don't come back, says one, and they return to their */
/* posts. */
static void scLunpaGuardsThrowIsaacOut(void) {
	fdEventStart();
	fdActorSetPos(ACTOR_PARTY_LEADER, 160, 128);
	fdActorSetPos(ACTOR_LEFT_GUARD, 152, 112);
	fdActorSetPos(ACTOR_RIGHT_GUARD, 168, 112);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#ifdef BUGFIX
	fdActorFaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH_SOUTHWEST, 0);
#else
	/* BUG: the town's guards, whose actor ids no actor at the gates holds, */
	/* so the gates' guards keep their headings. */
	fdActorFaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHWEST, 0);
#endif
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
	fdEventStartScrTransition();
	fdEventWait(30);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.8);
	fdActorSetSpeed(ACTOR_LEFT_GUARD, 1.8);
	fdActorSetSpeed(ACTOR_RIGHT_GUARD, 1.8);
	fdActorWalkTo(ACTOR_LEFT_GUARD, 152, 288);
	fdActorWalkTo(ACTOR_RIGHT_GUARD, 168, 288);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 160, 296);
	syTaskAddFrame(scLunpaKickUpLeaderDust, TASK_PASS_FRAME);
	fdEventWait(1);
	sySndPlay(SE_SCUFFLE);
	fdEventWait(20);
	fdActorSetSprPriority(ACTOR_LEFT_GUARD, 3);
	fdActorSetSprPriority(ACTOR_RIGHT_GUARD, 3);
	sySndPlay(SE_SCUFFLE);
	fdEventWait(30);
	fdActorGet(ACTOR_LEFT_GUARD)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_RIGHT_GUARD)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
	sySndPlay(SE_SCUFFLE);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
	fdActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
	syCloseEvent(scLunpaKickUpLeaderDust);
	/* Throw the leader down the steps, then wait for the first landing. */
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg |= ACTOR_FALLS;
	fdActorGet(ACTOR_PARTY_LEADER)->velY = toFIXED(6);
	fdActorGet(ACTOR_PARTY_LEADER)->velZ = toFIXED(6);
	fdEventWait(1);
	while(fdActorGet(ACTOR_PARTY_LEADER)->y.fixed != 0) {
		fdEventWait(1);
	}
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SPRAWLED);
	sySndPlay(SE_LANDING_THUD);
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syTaskAddFrame(scLunpaKickUpLeaderDust, TASK_PASS_FRAME);
	fdEventWait(2);
	/* A smaller second bounce ends with the guards' final warning. */
	fdActorGet(ACTOR_PARTY_LEADER)->velY = toFIXED(3);
	fdEventWait(1);
	while(fdActorGet(ACTOR_PARTY_LEADER)->y.fixed != 0) {
		fdEventWait(1);
	}
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	syCloseEvent(scLunpaKickUpLeaderDust);
	fdEventWait(50);
	fdEventSetMessageId(MsgLunpaGuardForbidsReturn);
	/* "And don't come back!" */
	fdEventShowMessage(ACTOR_LEFT_GUARD, 0);
	fdActorGet(ACTOR_LEFT_GUARD)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_RIGHT_GUARD)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSpeed(ACTOR_LEFT_GUARD, 1);
	fdActorSetSpeed(ACTOR_RIGHT_GUARD, 1);
	fdActorWalkTo(ACTOR_LEFT_GUARD, 144, 200);
	fdActorWalkTo(ACTOR_RIGHT_GUARD, 176, 200);
	fdActorWaitForMoveById(ACTOR_LEFT_GUARD);
	fdActorWaitForMoveById(ACTOR_RIGHT_GUARD);
	fdActorSetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
	fdActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdEventEnd();
}

/* After the rescue, Isaac comes out of the fortress door, casts Cloak in the */
/* upper palisade's gap and slips round behind the guards, west along the */
/* palisade and down the west side of the slope. */
static void scLunpaLeaderSneaksOut(void) {
	fdEventStart();
	fdEventStartScrTransition();
	fdActorWalkTo(ACTOR_PARTY_LEADER, 152, 168);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdEventWait(20);
	/* Cast Cloak before following the route around the guards. */
	fdPsynergyLoadResources(ABILITY_CLOAK, 1);
	fdPsynergySetupActorPair(ACTOR_PARTY_LEADER, 0);
	fdPsynergyStartCast();
	fdPsynergyRunHandler(1);
	fdPsynergyStopCasterUpdate();
	fdActorWalkTo(ACTOR_PARTY_LEADER, 144, 184);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 88, 184);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 88, 200);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 72, 200);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 72, 288);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 88, 288);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdEventEnd();
}

/* Sets up Lunpa (secrets hidden, the Nut in its tree) or the Lunpa Fortress */
/* Gates (the throw-out or the sneak-out, Garet's watch, the guards' watch and
 */
/* the fortress started over). */
s32 scInit(void) {
	ACTOR *puddle;

	if(MainRoutineStateWork.next == SCENE_LUNPA) {
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		scLunpaHideSecrets();
		if(gmFlagGet(FLAG_LUNPA_NUT_CAUGHT) == 0) {
			scFloatNut(ACTOR_FLOATING_NUT);
		}
		puddle = fdActorGet(ACTOR_HIDDEN_PUDDLE);
		if(puddle != NULL) {
			fdActorSetSprFlags(puddle, 0);
		}
		gmFlagSet(FLAG_LUNPA_SECRETS_HIDDEN);
	}
	if(MainRoutineStateWork.next == SCENE_LUNPA_FORTRESS_GATES) {
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		MainRoutineStateWork.retreatEntrance = GATE_RETREAT_ENTRANCE;
		if(MainRoutineStateWork.entrance == LUNPA_FORTRESS_GATES_AFTER_CAUGHT &&
		   gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			scLunpaGuardsThrowIsaacOut();
		}
		if(MainRoutineStateWork.entrance ==
			   LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA &&
		   gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			scLunpaLeaderSneaksOut();
		}
		if(gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0 &&
		   gmFlagGet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
			syTaskAddFrame(scLunpaWatchForFortressApproach, TASK_PASS_FRAME);
		}
#if TBS_PATCH_4
		if(MainRoutineStateWork.cloaked != 0)
			scLunpaCastCloak();
#endif
		syTaskAddFrame(scLunpaWatchForUncloakedParty, TASK_PASS_FRAME);
		/* The fortress's puzzles and beaten guards are reset. */
		gmFlagClear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_IVY_BLOWN_AWAY);
		gmFlagClear(FLAG_LUNPA_FORTRESS_PASSAGE_IVY_BLOWN_AWAY);
		gmFlagClear(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_IVY_BLOWN_AWAY);
		gmFlagClear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED);
		gmFlagClear(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED);
		gmFlagClear(FLAG_LUNPA_FORTRESS_CRATES_MOVED);
		gmFlagClear(FLAG_LUNPA_FORTRESS_MIDDLE_HALL_GUARD_BEATEN);
		gmFlagClear(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN);
		gmFlagClear(FLAG_LUNPA_FORTRESS_DONPAS_DOOR_GUARD_BEATEN);
		gmFlagClear(FLAG_LUNPA_FORTRESS_CELL_KEY_ROOM_GUARD_BEATEN);
	}
	return 0;
}

#include "shared/floating_nut.c"

/* An entrance of the town, with its camera free, and of the Fortress gates, */
/* its camera kept in their bounds. */
#define LUNPA_ENTRANCE(entrance, x, z, facing)                                 \
	{(entrance), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, -1, -1, -1, -1, 0}
#define GATE_ENTRANCE(entrance, x, z, facing)                                  \
	{(entrance), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, 32, 48, 272, 344, \
	 0}

SCENE_ENTRANCE LunpaEntrances[] = {
	LUNPA_ENTRANCE(ENTRANCE_DEFAULT, 172, 149, FACING_SOUTH),
	LUNPA_ENTRANCE(LUNPA_FROM_WORLD_MAP, 224, 472, FACING_NORTH),
	LUNPA_ENTRANCE(LUNPA_FROM_LUNPA_FORTRESS_GATES, 232, 32, FACING_SOUTH),
	LUNPA_ENTRANCE(LUNPA_FROM_WEST_LUNPA_CAVE, 120, 104, FACING_SOUTH),
	LUNPA_ENTRANCE(LUNPA_FROM_FIRST_HOME, 248, 184, FACING_SOUTH),
	LUNPA_ENTRANCE(LUNPA_FROM_SECOND_HOME, 344, 232, FACING_SOUTH),
	LUNPA_ENTRANCE(LUNPA_FROM_THIRD_HOME, 344, 328, FACING_SOUTH),
	LUNPA_ENTRANCE(LUNPA_FROM_WEAPON_AND_ARMOR_SHOP, 232, 360, FACING_SOUTH),
	LUNPA_ENTRANCE(LUNPA_FROM_ITEM_SHOP, 328, 120, FACING_SOUTH),
	LUNPA_ENTRANCE(LUNPA_FROM_INN, 104, 184, FACING_SOUTH),
	LUNPA_ENTRANCE(LUNPA_FROM_TEMPLE, 48, 168, FACING_SOUTH),
	LUNPA_ENTRANCE(LUNPA_FROM_CRATE_ROOM, 216, 344, FACING_SOUTH),
	LUNPA_ENTRANCE(LUNPA_FROM_STONE_ROOMS, 40, 328, FACING_SOUTH),
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE GateEntrances[] = {
	GATE_ENTRANCE(ENTRANCE_DEFAULT, 152, 328, FACING_NORTH),
	GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_GROUND_FLOOR, 160,
				  104, FACING_SOUTH),
	GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA, 152, 328, FACING_NORTH),
	GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA, 144, 104,
				  FACING_SOUTH),
	GATE_ENTRANCE(LUNPA_FORTRESS_GATES_AFTER_CAUGHT, 160, 224, FACING_EAST),
	{SCENE_TABLE_END},
};

/* A door's exit into its room of RockRooms. */
#define ROOM_EXIT(exit) SCENE_EXIT((exit), RockRooms, ROOM_ENTRANCE(exit))

u32 LunpaExits[] = {
	SCENE_EXITS(Lunpa),
	SCENE_EXIT(LUNPA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LUNPA),
	SCENE_EXIT(LUNPA_TO_LUNPA_FORTRESS_GATES, LunpaFortressGates,
			   LUNPA_FORTRESS_GATES_FROM_LUNPA),
	SCENE_EXIT(LUNPA_TO_WEST_LUNPA_CAVE, WestLunpaCave,
			   WEST_LUNPA_CAVE_FROM_LUNPA),
	ROOM_EXIT(LUNPA_TO_FIRST_HOME),
	ROOM_EXIT(LUNPA_TO_SECOND_HOME),
	ROOM_EXIT(LUNPA_TO_THIRD_HOME),
	ROOM_EXIT(LUNPA_TO_WEAPON_AND_ARMOR_SHOP),
	ROOM_EXIT(LUNPA_TO_ITEM_SHOP),
	ROOM_EXIT(LUNPA_TO_INN),
	ROOM_EXIT(LUNPA_TO_TEMPLE),
	ROOM_EXIT(LUNPA_TO_CRATE_ROOM),
	SCENE_EXIT(LUNPA_TO_STONE_ROOMS, StoneRooms, STONE_ROOMS_FROM_LUNPA),
	SCENE_EXITS_END,
};

u32 GateExits[] = {
	SCENE_EXITS(LunpaFortressGates),
	SCENE_EXIT(LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_GROUND_FLOOR,
			   LunpaFortressGroundFloor,
			   LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_GATES),
	SCENE_EXIT(LUNPA_FORTRESS_GATES_TO_LUNPA, Lunpa,
			   LUNPA_FROM_LUNPA_FORTRESS_GATES),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE LunpaActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      520, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      112, 0, 168, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      280, 0, 352, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      120, 0, 312, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      136, 0, 328, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      368, 0, 256, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      280, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      216, 0, 40, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      152, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      192, 0, 448, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      256, 0, 448, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      520, 0, 520, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_LUNPA_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
      120, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END, CONDITION_ALWAYS },
};

ACTOR_PLACE GateActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      144, 0, 200, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      176, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};
/* clang-format on */

/* A trigger for talking to an actor, for Mind Read on it, and for walking */
/* north into a door. */
#define TALK(actorId, value)                                                   \
	{TRIGGER_TALK, (actorId), CONDITION_ALWAYS, (value)}
#define MIND_READ(actorId, value)                                              \
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), (actorId), CONDITION_ALWAYS, (value)}
#define DOOR(trigger)                                                          \
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), (trigger), CONDITION_ALWAYS,     \
	 EVENT(scLunpaEnterDoor)}

/* The roads out of town and the doors, which every one of the town's trigger */
/* tables holds. */
#define LUNPA_ROADS_AND_DOORS                                                  \
	{TRIGGER_EXIT, CELL_BEHAVIOR_LUNPA_WORLD_MAP_ROAD, CONDITION_ALWAYS,       \
	 LUNPA_TO_WORLD_MAP},                                                      \
		{TRIGGER_EXIT, CELL_BEHAVIOR_LUNPA_GATE_ROAD, CONDITION_ALWAYS,        \
		 LUNPA_TO_LUNPA_FORTRESS_GATES},                                       \
		{TRIGGER_EXIT, CELL_BEHAVIOR_LUNPA_CAVE_ROAD, CONDITION_ALWAYS,        \
		 LUNPA_TO_WEST_LUNPA_CAVE},                                            \
		DOOR(CELL_BEHAVIOR_LUNPA_FIRST_HOME_DOOR),                             \
		DOOR(CELL_BEHAVIOR_LUNPA_SECOND_HOME_DOOR),                            \
		DOOR(CELL_BEHAVIOR_LUNPA_THIRD_HOME_DOOR),                             \
		DOOR(CELL_BEHAVIOR_LUNPA_ARMS_SHOP_DOOR),                              \
		DOOR(CELL_BEHAVIOR_LUNPA_ITEM_SHOP_DOOR),                              \
		DOOR(CELL_BEHAVIOR_LUNPA_INN_DOOR),                                    \
		DOOR(CELL_BEHAVIOR_LUNPA_TEMPLE_DOOR),                                 \
		DOOR(CELL_BEHAVIOR_LUNPA_CRATE_ROOM_DOOR), {                           \
		STEP_TRIGGER_WALKING_INTO(FACING_NORTH),                               \
			CELL_BEHAVIOR_LUNPA_HIDDEN_PASSAGE, CONDITION_ALWAYS,              \
			EVENT(scLunpaEnterHiddenPassage)                                   \
	}

/* The hidden puddle and what Reveal shows, which the town's trigger tables */
/* share. */
#define LUNPA_SECRETS                                                          \
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_HIDDEN_PUDDLE,                    \
	 FLAG_LUNPA_PUDDLE_FROZEN, EVENT(scLunpaFreezeHiddenPuddle)},              \
		{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,            \
		 CONDITION_ALWAYS, EVENT(scLunpaRevealSecrets)},                       \
		{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,           \
		 CONDITION_ALWAYS, EVENT(scLunpaHideSecrets)},                         \
		{EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_HIDDEN_SWITCH,   \
		 FLAG_LUNPA_SECRETS_HIDDEN, EVENT(scLunpaPressHiddenSwitch)},          \
		{EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_PSYNERGY_STONE,  \
		 FLAG_LUNPA_PSYNERGY_STONE, FIND_PSYNERGY_STONE},                      \
		{EXAMINE_TRIGGER(EXAMINE_GRAVE), CELL_BEHAVIOR_LUNPA_GRAVE_SLEEP_BOMB, \
		 FLAG_LUNPA_GRAVE_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB)},             \
		{EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_LUNPA_BARREL_NUT,      \
		 FLAG_LUNPA_BARREL_NUT, FIND_ITEM(ITEM_NUT)},                          \
		{PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FLOATING_NUT,                 \
		 FLAG_LUNPA_NUT_CAUGHT, EVENT(scLunpaCatchFloatingNut)},               \
		{EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_WELL,            \
		 CONDITION_ALWAYS, EVENT(scLunpaSearchWell)},                          \
	{                                                                          \
		TRIGGER_TABLE_END                                                      \
	}

TRIGGER LunpaSealedTriggers[] = {
	LUNPA_ROADS_AND_DOORS,
	TALK(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
		 EVENT(scLunpaAskAboutLengthOfStay)),
	TALK(ACTOR_WOMAN_WITH_NO_FREEDOM, MSGNO(MsgLunpaVillageVillagerBSealed)),
	TALK(ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
		 EVENT(scLunpaAskAboutHammetsKidnapping)),
	TALK(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
		 MSGNO(MsgLunpaVillageVillagerDSealed)),
	TALK(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
		 MSGNO(MsgLunpaVillageVillagerESealed)),
	TALK(ACTOR_OLD_WOMAN_ON_DONPA, MSGNO(MsgLunpaVillageVillagerFSealed)),
	TALK(ACTOR_BOY_ASKING_TO_SEE_DONPA, EVENT(scLunpaAskAboutSeeingDonpa)),
	TALK(ACTOR_GIRL_ON_THE_CHEFS_MEALS, MSGNO(MsgLunpaVillageVillagerHSealed)),
	TALK(ACTOR_WEST_GUARD, EVENT(scLunpaTalkToWestGuard)),
	TALK(ACTOR_EAST_GUARD, EVENT(scLunpaTalkToEastGuard)),
	MIND_READ(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
			  MSGNO(MsgLunpaVillageVillagerASealedThoughts)),
	MIND_READ(ACTOR_WOMAN_WITH_NO_FREEDOM,
			  MSGNO(MsgLunpaVillageVillagerBSealedThoughts)),
	MIND_READ(ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
			  MSGNO(MsgLunpaVillageVillagerCSealedThoughts)),
	MIND_READ(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
			  MSGNO(MsgLunpaVillageVillagerDSealedThoughts)),
	MIND_READ(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
			  MSGNO(MsgLunpaVillageVillagerESealedThoughts)),
	MIND_READ(ACTOR_OLD_WOMAN_ON_DONPA,
			  MSGNO(MsgLunpaVillageVillagerFSealedThoughts)),
	MIND_READ(ACTOR_BOY_ASKING_TO_SEE_DONPA,
			  MSGNO(MsgLunpaVillageVillagerGSealedThoughts)),
	MIND_READ(ACTOR_GIRL_ON_THE_CHEFS_MEALS,
			  MSGNO(MsgLunpaVillageVillagerHSealedThoughts)),
	MIND_READ(ACTOR_WEST_GUARD, MSGNO(MsgLunpaVillageWestGuardSealedThoughts)),
	MIND_READ(ACTOR_EAST_GUARD, EVENT(scLunpaReadEastGuardsThoughts)),
	LUNPA_SECRETS,
};

TRIGGER LunpaReopenedTriggers[] = {
	LUNPA_ROADS_AND_DOORS,
	TALK(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
		 MSGNO(MsgLunpaVillageVillagerAReopened)),
	TALK(ACTOR_WOMAN_WITH_NO_FREEDOM, EVENT(scLunpaComplainAboutCold)),
	TALK(ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
		 MSGNO(MsgLunpaVillageVillagerCReopened)),
	TALK(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
		 EVENT(scLunpaAskAboutFortressCommotion)),
	TALK(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
		 MSGNO(MsgLunpaVillageVillagerEReopened)),
	TALK(ACTOR_OLD_WOMAN_ON_DONPA, MSGNO(MsgLunpaVillageVillagerFReopened)),
	TALK(ACTOR_BOY_ASKING_TO_SEE_DONPA,
		 MSGNO(MsgLunpaVillageVillagerGReopened)),
	TALK(ACTOR_GIRL_ON_THE_CHEFS_MEALS,
		 MSGNO(MsgLunpaVillageVillagerHReopened)),
	TALK(ACTOR_WEST_GUARD, MSGNO(MsgLunpaVillageWestGuardReopened)),
	TALK(ACTOR_EAST_GUARD, MSGNO(MsgLunpaVillageEastGuardReopened)),
	MIND_READ(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
			  MSGNO(MsgLunpaVillageVillagerAReopenedThoughts)),
	MIND_READ(ACTOR_WOMAN_WITH_NO_FREEDOM,
			  MSGNO(MsgLunpaVillageVillagerBReopenedThoughts)),
	MIND_READ(ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
			  MSGNO(MsgLunpaVillageVillagerCReopenedThoughts)),
	MIND_READ(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
			  MSGNO(MsgLunpaVillageVillagerDReopenedThoughts)),
	MIND_READ(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
			  MSGNO(MsgLunpaVillageVillagerEReopenedThoughts)),
	MIND_READ(ACTOR_OLD_WOMAN_ON_DONPA,
			  MSGNO(MsgLunpaVillageVillagerFReopenedThoughts)),
	MIND_READ(ACTOR_BOY_ASKING_TO_SEE_DONPA,
			  MSGNO(MsgLunpaVillageVillagerGReopenedThoughts)),
	MIND_READ(ACTOR_GIRL_ON_THE_CHEFS_MEALS,
			  MSGNO(MsgLunpaVillageVillagerHReopenedThoughts)),
	MIND_READ(ACTOR_WEST_GUARD,
			  MSGNO(MsgLunpaVillageWestGuardReopenedThoughts)),
	MIND_READ(ACTOR_EAST_GUARD, EVENT(scLunpaReadEastGuardsThoughts)),
	LUNPA_SECRETS,
};

/* clang-format off */
TRIGGER GateTriggers[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_LUNPA_ROAD, CONDITION_ALWAYS, LUNPA_FORTRESS_GATES_TO_LUNPA },
    /* Unused: no cell of the gates' map has the third road's behavior. */
    { TRIGGER_EXIT, CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_THIRD_ROAD, CONDITION_ALWAYS, GATE_EXIT_THIRD },
    { PSYNERGY_ANYWHERE(ABILITY_CLOAK, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(scLunpaCastCloak) },
    { PSYNERGY_ANYWHERE(ABILITY_CLOAK, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT(scLunpaCloakFades) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_DOOR, FLAG_LUNPA_FORTRESS_GATES_ENTERING_FORTRESS,
      EVENT(scLunpaEnterFortress) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_GUARDED_GROUND, FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR,
      EVENT(scLunpaGuardsBlockGate) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH, CONDITION_ALWAYS,
      EVENT(scLunpaGaretTurnsIsaacBack) },
    { TRIGGER_RAISED, RAISED_PARTY_SPOTTED, CONDITION_ALWAYS, EVENT(scLunpaGuardsCatchParty) },
    TALK(ACTOR_LEFT_GUARD, EVENT(scLunpaTalkToLeftGuard)),
    TALK(ACTOR_RIGHT_GUARD, EVENT(scLunpaTalkToRightGuard)),
    MIND_READ(ACTOR_LEFT_GUARD, EVENT(scLunpaReadLeftGuardsThoughts)),
    MIND_READ(ACTOR_RIGHT_GUARD, EVENT(scLunpaReadRightGuardsThoughts)),
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_FORTRESS_GATES_WELL, CONDITION_ALWAYS, EVENT(scLunpaSearchWell) },
    { TRIGGER_TABLE_END },
};
/* clang-format on */

/* Each door's opening, as a map grid animation. */
u16 LunpaDoorSteps[DOOR_COUNT][11] = {
	{MAPGRID_ANIMATION_STEP(38, 35, 2, 2, 5),
	 MAPGRID_ANIMATION_STEP(40, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
	{MAPGRID_ANIMATION_STEP(42, 35, 2, 2, 5),
	 MAPGRID_ANIMATION_STEP(44, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
	{MAPGRID_ANIMATION_STEP(46, 33, 2, 2, 5),
	 MAPGRID_ANIMATION_STEP(48, 33, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
	{MAPGRID_ANIMATION_STEP(46, 35, 2, 2, 5),
	 MAPGRID_ANIMATION_STEP(48, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
	{MAPGRID_ANIMATION_STEP(50, 33, 2, 2, 5),
	 MAPGRID_ANIMATION_STEP(52, 33, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
	{MAPGRID_ANIMATION_STEP(50, 35, 2, 2, 5),
	 MAPGRID_ANIMATION_STEP(52, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
	{MAPGRID_ANIMATION_STEP(42, 33, 4, 2, 5),
	 MAPGRID_ANIMATION_STEP(38, 33, 4, 2, 5), MAPGRID_ANIMATION_STEPS_END},
	{MAPGRID_ANIMATION_STEP(54, 33, 2, 2, 5),
	 MAPGRID_ANIMATION_STEP(56, 33, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
};

/* Each door's opening animation and the map grid cell it plays at, by its */
/* step trigger from CELL_BEHAVIOR_LUNPA_FIRST_HOME_DOOR. */
MAP_GRID_CELL_ANIMATION LunpaDoors[] = {
	{LunpaDoorSteps[0], 46, 9},	 {LunpaDoorSteps[1], 52, 12},
	{LunpaDoorSteps[2], 52, 18}, {LunpaDoorSteps[3], 45, 20},
	{LunpaDoorSteps[4], 51, 5},	 {LunpaDoorSteps[5], 37, 9},
	{LunpaDoorSteps[6], 33, 6},	 {LunpaDoorSteps[7], 44, 17},
};

/* Unused: a zero word nothing reads. */
u32 LunpaUnusedWord = 0;
