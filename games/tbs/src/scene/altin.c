/* @file */
/* Altin, the mining village north of the Alpine Crossing, in two scenes that */
/* share this overlay and the area name Altin: the village outdoors */
/* (SceneId_Altin), and its inn, shops and houses (SceneId_CottageRoomsD), a */
/* rooms map each in its own camera box. The getters pick each scene's */
/* tables by MainRoutineStateWork.next, and both share one exit table. A third,
 */
/* empty set of tables serves any other scene. */
/* */
/* When the party comes in from the World Map on the east, the village is under
 */
/* water. Since the day the gems fell from the sky, water beasts have flooded */
/* it, and one still spits water at the houses from the village's west edge. */
/* Walking up to it (scAltinScareOffBeast) makes it stop, cry out and hop away
 */
/* into the mine for good. Its spray and sound start in scAltinApplyWaterLevel,
 */
/* and it is drawn with its green and blue swapped (SPRITE_COLOR_RBG). */
/* */
/* The water goes down as the party beats the beasts in the mine below, which */
/* the area names call Altin Peak (altin_peak.c). Each beaten beast opens a */
/* drain (FLAG_ALTIN_MINE_FIRST_DRAIN to _THIRD_DRAIN), and the village shows */
/* three stages: the water dropped once, twice, or drained (the second and */
/* third drains both). scInit turns the drains into three scene temp */
/* flags, and scAltinApplyWaterLevel copies that stage's picture of the */
/* village, with its cells, over the map and leaves out whoever would be */
/* standing in the water. */
/* */
/* As the water drops, three villagers come home one by one, the two families */
/* sheltering in the inn's Room One and Room Two go home, and the shops open */
/* again: the item shop after the first drop, the armor shop after the second,
 */
/* and the weapon shop, whose owner waits in the inn, once the village is */
/* drained. Until then the armor and item merchants wait outside above the */
/* flood. */
/* */
/* Every villager has two sets of lines and thoughts, before and after */
/* FLAG_HYDROS_STATUES_DEFEATED, when the party has beaten the hydros statues */
/* at the bottom of the mine. The row that needs the flag clear comes first in
 */
/* each table, so the first row that matches picks the time. */
/* */
/* Walking into a door (scAltinEnterDoor) leads into the inn, under its sign on
 */
/* the north-west ledge; the Sanctum, by the wide double door in the cliff */
/* below the stone statue; the item shop, by its small door in the east cliff;
 */
/* the armor shop, the house with a round red sign; the weapon shop, by the */
/* door with a sword sign at the south; or one of three family houses. Three */
/* doorways lead down into the mine (scAltinWalkIntoMine): the rail tunnel in */
/* the north cliff, the doorway in the middle of the village where the rails */
/* run south, and one in the west cliff by the water beast. The stone statue on
 */
/* the top ledge stands in front of a fourth way in, until Move shifts it aside
 */
/* (scAltinCheckStatueMoved). Coming back out of the Sanctum, the area name is
 */
/* not shown again. */
/* */
/* In the inn a cook from Xian paces between his pots */
/* (CottageRoomsD_CookScript). */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field_effect.h"
#include "graphics/graphics.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE Altin_NoEntrances[];
extern SCENE_ENTRANCE Altin_Entrances[];
extern SCENE_ENTRANCE CottageRoomsD_Entrances[];
extern u32 Altin_Exits[];
extern ACTOR_PLACE Altin_NoActorPlacements[];
extern ACTOR_PLACE Altin_ActorPlacements[];
extern ACTOR_PLACE CottageRoomsD_ActorPlacements[];
extern TRIGGER Altin_NoTriggers[];
extern TRIGGER Altin_Triggers[];
extern TRIGGER CottageRoomsD_Triggers[];
extern MAP_GRID_CELL_ANIMATION Altin_Doors[];

/* The x of the easternmost pot the cook from Xian walks to and nods over, 4 */
/* pixels further west from patch 3. */
#if !TBS_PATCH_3
#define ALTIN_COOK_LAST_POT_X 384
#else
#define ALTIN_COOK_LAST_POT_X 380
#endif

/* The overlay's two scenes: the village, and its inn, shops and houses. */
#define MAP_ALTIN_VILLAGE (SCENE_ALTIN)
#define MAP_ALTIN_HOUSES  (SCENE_COTTAGE_ROOMS_D)

/* The village's actors, numbering the rows of Altin_ActorPlacements. */
/* */
/* The people without an event of their own are named by their sprite and by */
/* what they say before the hydros statues are beaten. */
typedef enum ALTIN_VILLAGE_ACTOR_ID {
	/* A bald, bare-chested strong man (SPRITE_BALD_MUSCULAR_MAN). His event is
	 */
	/* scAltinAskAboutFirstVisit. */
	ACTOR_GREETING_STRONGMAN = ACTOR_FIRST_PLACED,
	/* A stocky strong man flexing his arms (SPRITE_STRONGMAN). His event is */
	/* scAltinAskAboutHydros. */
	ACTOR_CURIOUS_STRONGMAN,
	/* A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR). */
	ACTOR_WOMAN_COATED_HOUSE,
	/* A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR), who wanders.
	 */
	ACTOR_MAN_SUBMERGED_MINE,
	/* A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE). */
	ACTOR_OLD_MAN_DESTROY_MONSTERS,
	/* A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS). */
	ACTOR_WOMAN_WATER_LUCK,
	/* An old woman with white hair (SPRITE_OLD_WOMAN). Her event is */
	/* scAltinTalkToThirstyOldWoman. */
	ACTOR_THIRSTY_OLD_WOMAN,
	/* Three villagers come back as the water recedes, one with each drop */
	/* (scAltinApplyWaterLevel). They are this man with brown hair */
	/* (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), then a strong man and an old man. */
	ACTOR_MAN_WATERS_SUBSIDED,
	/* A bald, bare-chested strong man (SPRITE_BALD_MUSCULAR_MAN). */
	ACTOR_STRONGMAN_GUARDIAN_STATUES,
	/* A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who wanders. */
	ACTOR_OLD_MAN_ROCK_FLUSHED_OUT,
	/* The beast spewing water at the houses until FLAG_ALTIN_WATER_BEAST_LEFT.
	 */
	ACTOR_WATER_BEAST,
	/* The stone statue on the top ledge, in front of the passage into the */
	/* mine that Move uncovers (scAltinCheckStatueMoved). */
	ACTOR_STONE_STATUE,
	/* The armor merchant on the west ledge and the item merchant on the ledge
	 */
	/* east of the Sanctum, waiting while their shops are under water. */
	ACTOR_FLOODED_ARMOR_MERCHANT,
	ACTOR_FLOODED_ITEM_MERCHANT,
	/* Three crates placed with SPRITE_CRATE. scAltinApplyWaterLevel hides them
	 */
	/* (SPRITE_COLOR_HIDDEN) but leaves them solid, as placed actors are. */
	/* */
	/* They are invisible blocks. One is at the top of the ladder on the east */
	/* cliff, and two are on the rails below the doorway into the mine (the */
	/* drained village's render with them drawn). */
	ACTOR_FIRST_CRATE,
	ACTOR_SECOND_CRATE,
	ACTOR_THIRD_CRATE
} ALTIN_VILLAGE_ACTOR_ID;

/* The houses' actors, numbering the rows of */
/* CottageRoomsD_ActorPlacements, named as the village's are. */
typedef enum ALTIN_HOUSE_ACTOR_ID {
	/* The family sheltering in the inn's Room One until the first drop. They
	   are */
	/* this man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) and a woman
	 */
	/* with fair hair. */
	ACTOR_ROOM_ONE_GUEST = ACTOR_FIRST_PLACED,
	/* A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR). */
	ACTOR_ROOM_ONE_SECOND_GUEST,
	/* The family in Room Two, until the second drop. They are this bent old man
	 */
	/* with a cane (SPRITE_OLD_MAN_WITH_CANE) and an old woman. */
	ACTOR_ROOM_TWO_GUEST,
	/* An old woman with white hair (SPRITE_OLD_WOMAN). */
	ACTOR_ROOM_TWO_SECOND_GUEST,
	/* A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR). */
	/* */
	/* He and ACTOR_WOMAN_NO_PLACE_LIKE_HOME are the kissing girl's "Mom and */
	/* Pop", who have been acting weird since the ruins were found. His thoughts
	 */
	/* won't give up a spot at the end of the mine's easternmost passage, and */
	/* hers say "My husband's sense for finding treasure has never failed." */
	ACTOR_MAN_BACK_IN_HIS_HOMETOWN,
	/* A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR). */
	ACTOR_WOMAN_NO_PLACE_LIKE_HOME,
	/* A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS). Her event
	 */
	/* is scCottageRoomsDAskAboutMonsters. */
	ACTOR_KISSING_GIRL,
	ACTOR_WEAPON_MERCHANT,
	ACTOR_ITEM_MERCHANT,
	ACTOR_ARMOR_MERCHANT,
	/* Opens the inn (scCottageRoomsDTalkToInnkeeper). */
	ACTOR_INNKEEPER,
	/* A woman with red-brown hair in a white apron
	   (SPRITE_WOMAN_IN_PINK_APRON). */
	ACTOR_WOMAN_FORMER_GLORY,
	ACTOR_COOK,
	/* A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) in the inn, */
	/* telling of its guests going home as the water drops. */
	ACTOR_MAN_WAITING_TO_GO_HOME
} ALTIN_HOUSE_ACTOR_ID;

/* The village's water level, set on entry from the mine's drain flags. */
enum {
	FLAG_ALTIN_WATER_DROPPED_ONCE = FLAG_SCENE_TEMP_40,
	FLAG_ALTIN_WATER_DROPPED_TWICE = FLAG_SCENE_TEMP_41,
	FLAG_ALTIN_WATER_DRAINED = FLAG_SCENE_TEMP_42
};

/* Altin's shops, by their numbers in the shop table. */
enum { SHOP_ALTIN_WEAPONS = 19, SHOP_ALTIN_ARMOR = 20, SHOP_ALTIN_ITEMS = 21 };

/* Altin's inn, as mnInnCheckIn numbers the inns. */
enum { ALTIN_INN = 6 };

/* The cell behaviors Altin's triggers answer. A door's or doorway's is the */
/* exit it leaves by. */
typedef enum ALTIN_CELL_BEHAVIOR {
	/* The road out to the World Map at the east edge. */
	CELL_BEHAVIOR_ALTIN_EAST_ROAD = ALTIN_TO_WORLD_MAP,
	/* The inn's door, under its sign on the north-west ledge. */
	CELL_BEHAVIOR_ALTIN_INN_DOOR = ALTIN_TO_COTTAGE_ROOMS_D_INN,
	/* The Sanctum's wide double door, in the cliff below the stone statue. */
	CELL_BEHAVIOR_ALTIN_SANCTUM_DOOR = ALTIN_TO_SANCTUM,
	/* The door of the house on the east side, the Room One family's home. */
	CELL_BEHAVIOR_ALTIN_EAST_HOUSE_DOOR = ALTIN_TO_COTTAGE_ROOMS_D_EAST_HOUSE,
	/* The door in the cliff north-west of the armor shop, the Room Two */
	/* family's home. */
	CELL_BEHAVIOR_ALTIN_CLIFF_HOUSE_DOOR = ALTIN_TO_COTTAGE_ROOMS_D_CLIFF_HOUSE,
	/* The door of the house by the rails at the south, the kissing girl's */
	/* home. */
	CELL_BEHAVIOR_ALTIN_SOUTH_HOUSE_DOOR = ALTIN_TO_COTTAGE_ROOMS_D_SOUTH_HOUSE,
	/* The item shop's small door in the east cliff. */
	CELL_BEHAVIOR_ALTIN_ITEM_SHOP_DOOR = ALTIN_TO_COTTAGE_ROOMS_D_ITEM_SHOP,
	/* The door of the armor shop, the house with a round red sign. */
	CELL_BEHAVIOR_ALTIN_ARMOR_SHOP_DOOR = ALTIN_TO_COTTAGE_ROOMS_D_ARMOR_SHOP,
	/* The weapon shop's door with a sword sign, at the south. */
	CELL_BEHAVIOR_ALTIN_WEAPON_SHOP_DOOR = ALTIN_TO_COTTAGE_ROOMS_D_WEAPON_SHOP,
	/* The doorway into the mine in the west cliff, by the water beast. */
	CELL_BEHAVIOR_ALTIN_WEST_MINE_DOORWAY =
		ALTIN_TO_ALTIN_PEAK_FIRST_WATER_BEAST,
	/* The rail tunnel into the mine in the north cliff. */
	CELL_BEHAVIOR_ALTIN_RAIL_TUNNEL = ALTIN_TO_ALTIN_PEAK_SOUTH_ENTRANCE,
	/* The doorway into the mine in the middle of the village, where the rails
	 */
	/* run south. */
	CELL_BEHAVIOR_ALTIN_MIDDLE_MINE_DOORWAY = ALTIN_TO_ALTIN_PEAK_JUNCTION,
	/* The passage behind the stone statue. */
	CELL_BEHAVIOR_ALTIN_PASSAGE_BEHIND_STATUE = ALTIN_TO_ALTIN_PEAK_SPRITZ,
	/* The ground beside the water beast, on the west edge. */
	CELL_BEHAVIOR_ALTIN_NEAR_BEAST = 20,
	/* The doors are numbered in a row from the inn's. */
	CELL_BEHAVIOR_ALTIN_FIRST_DOOR = CELL_BEHAVIOR_ALTIN_INN_DOOR
} ALTIN_CELL_BEHAVIOR;

/* The cell behaviors of the rooms' barrels, bed, pots and bookcase. */
typedef enum COTTAGE_ROOMS_D_CELL_BEHAVIOR {
	/* A barrel in the Room One family's house. */
	CELL_BEHAVIOR_COTTAGE_ROOMS_D_NUT_BARREL = 100,
	/* A barrel in the weapon shop. */
	CELL_BEHAVIOR_COTTAGE_ROOMS_D_COINS_BARREL = 101,
	/* A bed upstairs in the inn. */
	CELL_BEHAVIOR_COTTAGE_ROOMS_D_SLEEP_BOMB_BED = 102,
	/* The rusted pots in the Room Two family's house. */
	CELL_BEHAVIOR_COTTAGE_ROOMS_D_RUSTED_POTS = 200,
	/* The cook's roast beef in the inn. */
	CELL_BEHAVIOR_COTTAGE_ROOMS_D_ROAST_BEEF = 201,
	/* The cook's shrimp in the inn. */
	CELL_BEHAVIOR_COTTAGE_ROOMS_D_SHRIMP = 202,
	/* The bookcase in the inn. */
	CELL_BEHAVIOR_COTTAGE_ROOMS_D_BOOKCASE = 203
} COTTAGE_ROOMS_D_CELL_BEHAVIOR;

/* The doors of the village, by their index in Altin_Doors. */
enum {
	/* The Sanctum's door, which leads to SANCTUM_FROM_ALTIN. */
	DOOR_SANCTUM =
		CELL_BEHAVIOR_ALTIN_SANCTUM_DOOR - CELL_BEHAVIOR_ALTIN_FIRST_DOOR,
	/* The armor shop's door, where Isaac is only lined up with the doorway */
	/* before the screen fades, not walked into it. */
	DOOR_ARMOR_SHOP =
		CELL_BEHAVIOR_ALTIN_ARMOR_SHOP_DOOR - CELL_BEHAVIOR_ALTIN_FIRST_DOOR
};

/* The frames from one of the water beast's hops to the next. */
#define WATER_BEAST_HOP_PERIOD 60
/* The map column the stone statue stands in once moved, at (360, 168). */
#define STONE_STATUE_MOVED_COLUMN 22
/* The frames a drop of the water beast's spray lives. */
#define SPEW_LIFETIME 20
/* A drop of spray starts at three tenths of its sprite's size. */
#define SPEW_START_SCALE (3 * FIXED_ONE / 10)

/* Whether a heading faces north, across a counter. */
/* UB: the facing test overflows a signed left shift for many headings. */
#ifdef UBFIX
#define ALTIN_FACES_COUNTER(facing) FACING_IS_NORTH(facing)
#else
#define ALTIN_FACES_COUNTER(facing)                                            \
	((u32)(((facing) + TRIG_TURN - (FACING_NORTHWEST + 1)) << 16) <=           \
	 (u32)(FACING_NORTHEAST - FACING_NORTHWEST - 2) << 16)
#endif

/* How far a drop of spray drifts east this frame, in 16.16 pixels: a sixteenth
 */
/* of a pixel for each frame it has left, and half a pixel of random wobble */
/* either way. */
/* UB: the wobble can be negative, and a negative number is shifted left. */
#ifdef UBFIX
#define SPEW_LATERAL_STEP(timer, wobble) ((timer) * 4096 + (wobble) * 32768)
#else
#define SPEW_LATERAL_STEP(timer, wobble) (((timer) << 12) + ((wobble) << 15))
#endif

static void scAltinApplyWaterLevel(void);
static void scCottageRoomsDPlaceSheltering(void);
static void scAltinBeastSpewWater(void);

const SCENE_ENTRANCE *scGetEntrances(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == MAP_ALTIN_VILLAGE) {
		return Altin_Entrances;
	}
	if(sceneId == MAP_ALTIN_HOUSES) {
		return CottageRoomsD_Entrances;
	}
	return Altin_NoEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void) { return 0; }

/* Both scenes share one exit table. */
const u32 *scGetExits(void) { return Altin_Exits; }

/* Some villagers only turn back after talking once the flood is over or */
/* their family has gone home. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	s32 sceneId = MainRoutineStateWork.next;
	if(sceneId == MAP_ALTIN_VILLAGE) {
		if(gmFlagGet(FLAG_HYDROS_STATUES_DEFEATED)) {
			Altin_ActorPlacements[ACTOR_WOMAN_WATER_LUCK - ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY;
			Altin_ActorPlacements[ACTOR_THIRSTY_OLD_WOMAN - ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY;
		}
		return Altin_ActorPlacements;
	}
	if(sceneId == MAP_ALTIN_HOUSES) {
		if(gmFlagGet(FLAG_ALTIN_MINE_FIRST_DRAIN))
			CottageRoomsD_ActorPlacements[ACTOR_ROOM_ONE_SECOND_GUEST -
										  ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY_AND_BACK;
		if(gmFlagGet(FLAG_ALTIN_MINE_SECOND_DRAIN) ||
		   gmFlagGet(FLAG_ALTIN_MINE_THIRD_DRAIN))
			CottageRoomsD_ActorPlacements[ACTOR_ROOM_TWO_SECOND_GUEST -
										  ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY_AND_BACK;
		scAssignViewFlags(CottageRoomsD_ActorPlacements);
		return CottageRoomsD_ActorPlacements;
	}
	return Altin_NoActorPlacements;
}

const TRIGGER *scGetTriggers(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == MAP_ALTIN_VILLAGE) {
		return Altin_Triggers;
	}
	if(sceneId == MAP_ALTIN_HOUSES) {
		return CottageRoomsD_Triggers;
	}
	return Altin_NoTriggers;
}

/* The bald strong man by the east entrance asks whether this is the party's */
/* first visit to Altin, and tells how the flood has changed it. Once the */
/* hydros statues are beaten he only thanks them. */
static void scAltinAskAboutFirstVisit(void) {
	fdEventStart();
	fdEventSetMessageId(MsgAltinYourFirstTimeVisitAltin);

	/* "Is this your first time to visit Altin?" */
	fdEventOpenMessage(ACTOR_GREETING_STRONGMAN, 0);

	if(fdEventAskYesNoWithActorAnimation(0, 0) == 1) {
		/* "Look at how much Altin has changed! You can't do much here..." */
		fdEventShowMessage(ACTOR_GREETING_STRONGMAN, 0);
	}
	else {
		fdEventSkipMessages(1);
		/* "We must look a lot like a coastal town, huh?" */
		/* Yes: "Nothing gets by you... Altin never even had a well in the..."
		 */
		/* No: "The day the gems fell from the sky, these water-spewing..." */
		fdEventAskYesNo(ACTOR_GREETING_STRONGMAN, 0);
	}

	fdEventEnd();
}

/* Once the hydros statues are beaten, the strong man near the east entrance */
/* asks whether water gushed out of them when the party slew them. */
static void scAltinAskAboutHydros(void) {
	fdEventStart();
	fdEventSetMessageId(MsgAltinDidSeeWaterGushingOut);
	/* "Did you see water gushing out when you slew those hydros..." */
	/* Yes: "I see... So the water monsters are like giant water tanks..." */
	/* No: "Really... Then how could they have carried all that water?" */
	fdEventAskYesNo(ACTOR_CURIOUS_STRONGMAN, 0);
	fdEventEnd();
}

/* The old woman on the west ledge complains that nobody can drink the water */
/* the monsters spew. While the water beast is still there, the camera pans */
/* south to it spitting water and back before she shakes her head. Once the */
/* hydros statues are beaten, she asks instead whether the party defeated the */
/* water beasts. */
static void scAltinTalkToThirstyOldWoman(void) {
	ACTOR *beast;
	ACTOR *leader;

	fdEventStart();
	if(gmFlagGet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
		fdEventSetMessageId(MsgAltinOnesWhoDefeatedWaterBeasts);
		/* "Are you the ones who defeated the water beasts?" */
		/* Yes: "We'd be much obliged if you'd also help rebuild the village."
		 */
		/* No: "I heard our rescuers were young. I'd like to meet them." */
		fdEventAskYesNo(ACTOR_THIRSTY_OLD_WOMAN, 0);
	}
	else {
		fdActorSetAnimation(ACTOR_THIRSTY_OLD_WOMAN, ANIM_SHAKE_HEAD);
		fdEventSetMessageId(MsgAltinWeCantDrinkWaterMonsters);
		/* "We can't drink the water the monsters spew! It's disgusting...." */
		fdEventShowMessageThenWait(ACTOR_THIRSTY_OLD_WOMAN, 0, 10);
		if(gmFlagGet(FLAG_ALTIN_WATER_BEAST_LEFT) == 0) {
			beast = fdActorGet(ACTOR_WATER_BEAST);

			fdCameraGetActor()->motionFlg = 0;
			fdCameraSetSpeed(1);
			fdCameraMoveToFX(beast->x.fixed, beast->y.fixed, beast->z.fixed, 1);
			/* BUG: FACING_SOUTH's value is passed as an actor id. No actor has
			 */
			/* it, so the leader does not turn south toward the beast. */
#ifdef BUGFIX
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#else
			fdActorFaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#endif
			fdActorFaceDirection(ACTOR_THIRSTY_OLD_WOMAN,
								 FACING_SOUTH_SOUTHEAST, 0);
			fdCameraWaitForArrival();
			fdEventWait(120);
			leader = fdActorGet(ACTOR_PARTY_LEADER);
			fdCameraMoveToFX(leader->x.fixed, leader->y.fixed, leader->z.fixed,
							 1);
			fdCameraWaitForArrival();
		}
		fdActorSetAnimationAndWait(ACTOR_THIRSTY_OLD_WOMAN, ANIM_SHAKE_HEAD);
	}
	fdEventEnd();
}

/* Once the hydros statues are beaten, the old man who wanders the middle of */
/* the village, back home since the water drained, asks whether the party */
/* really found ancient ruins at the bottom of the mine. */
static void scAltinAskAboutRuins(void) {
	fdEventStart();
	fdEventSetMessageId(MsgAltinTrueFoundAncientRuinsIn);
	/* "Is it true you found ancient ruins in the deepest part of..." */
	/* Yes: "Really? I always thought that mine held some kind of secret." */
	/* No: "Hmmm, are you sure you even went down there? Someone ought..." */
	fdEventAskYesNo(ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, 0);
	fdEventEnd();
}

/* Once the hydros statues are beaten, the woman of the Room One family, back */
/* in their house on the east side, asks whether the party are the young */
/* warriors a girl from Xian was asking about. */
static void scCottageRoomsDAskAboutXianGirl(void) {
	fdEventStart();
	fdEventSetMessageId(MsgAltinGirlFromXianWasAsking);
	/* "A girl from Xian was asking questions about young warriors...." */
	/* Yes: "She was searching very hard." */
	/* No: "Well, if not, I hope she finds whoever she was looking for." */
	fdEventAskYesNo(ACTOR_ROOM_ONE_SECOND_GUEST, 0);
	fdEventEnd();
}

/* After the second drop, until the hydros statues are beaten, the old man of */
/* the Room Two family, back in their house in the cliff, asks whether the */
/* party is finding a way west through the mountains, and says a passage */
/* should lead west at the bottom of the mine. */
static void scCottageRoomsDAskAboutWayWest(void) {
	fdEventStart();
	fdEventSetMessageId(MsgAltinTryingFindYourWayWest);
	/* "Are you trying to find your way west through the mountains?" */
	/* Yes: "There should be a passage that leads west at the bottom of..." */
	/* No: "I don't know what you did, but we are grateful you defeated..." */
	fdEventAskYesNo(ACTOR_ROOM_TWO_GUEST, 0);
	fdEventEnd();
}

/* Once the hydros statues are beaten, the little girl in the house by the */
/* rails asks whether the party defeated the monsters, and thanks Isaac with a
 */
/* kiss either way. */
static void scCottageRoomsDAskAboutMonsters(void) {
	fdEventStart();
	fdEventSetMessageId(MsgAltinDefeatedThoseMonstersDidnt);
	/* "You defeated those monsters, didn't you?" */
	/* Yes: "Here, let me thank you with...a kiss! Hey, you're turning..." */
	/* No: "Oh, you didn't? Still, I'm happy to be back home, so here's..." */
	fdEventAskYesNo(ACTOR_KISSING_GIRL, 0);
	fdEventEnd();
}

/* Once the hydros statues are beaten, the man still staying upstairs in the */
/* inn asks whether the few beasts left in the mine are deep down. */
static void scCottageRoomsDAskAboutMineBeasts(void) {
	fdEventStart();
	fdEventSetMessageId(MsgAltinThereFewBeastsInMine);
	/* "There are a few beasts in the mine still, but they're down..." */
	/* Yes: "Good. Then we can just mine the upper passages." */
	/* No: "If they're in the upper passages, then we can't mine like we..." */
	fdEventAskYesNo(ACTOR_MAN_WAITING_TO_GO_HOME, 0);
	fdEventEnd();
}

/* Returns whether Isaac faces north, across a counter. */
static s32 scAltinLeaderFacesCounter(void) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	if(ALTIN_FACES_COUNTER(leader->facing)) {
		return 1;
	}
	return 0;
}

/* Until the village is drained, the weapon merchant waits in the inn and only
 */
/* asks whether the party wants weapons. Then he is back in the weapon shop by
 */
/* the sword sign and sells when spoken to across his counter; from the side */
/* he is glad to open up shop again, or, once the hydros statues are beaten, */
/* tells the party to find the passage west in the mine. */
static void scCottageRoomsDTalkToWeaponMerchant(void) {
	if(gmFlagGet(FLAG_ALTIN_WATER_DRAINED) == 0) {
		fdEventStart();
		fdEventSetMessageId(MsgAltinDoWantWeapons);

		/* "Do you want weapons?" */
		/* Yes: "I'll sell you anything you want once the waters have..." */
		/* No: "You wouldn't say that if you saw my weapons. I wish I could..."
		 */
		fdEventAskYesNo(ACTOR_WEAPON_MERCHANT, 0);
		fdEventEnd();
		return;
	}

	if(scAltinLeaderFacesCounter() != 0) {
		mnShopRun(SHOP_ALTIN_WEAPONS, ACTOR_WEAPON_MERCHANT);
		return;
	}

	fdEventStart();
	fdEventSetMessageId(MsgAltinThankGoodnessWaterHasReceded);
	if(gmFlagGet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
		fdEventSetMessageId(MsgAltinYoullHaveFindPassageIn);
	}
	fdEventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
	fdEventEnd();
}

/* Until the second drop, the armor merchant waits on the west ledge beside */
/* his flooded shop, saying his store is submerged. After it he sells from the
 */
/* counter in the armor shop, the house with the round red sign, when spoken */
/* to across it; from the side he is glad to sell armor again, or, once the */
/* hydros statues are beaten, asks if the party isn't impressed by his */
/* selection. */
static void scAltinTalkToArmorMerchant(void) {
	if(gmFlagGet(FLAG_ALTIN_WATER_DROPPED_TWICE) == 0) {
		fdEventStart();
		fdEventSetMessageId(MsgAltinMyStoreSubmergedWantSell);
		/* "My store is submerged... I want to sell armor, but there's..." */
		fdEventShowMessage(ACTOR_FLOODED_ARMOR_MERCHANT, 0);
		fdEventEnd();
		return;
	}

	if(scAltinLeaderFacesCounter() != 0) {
		mnShopRun(SHOP_ALTIN_ARMOR, ACTOR_ARMOR_MERCHANT);
		return;
	}

	fdEventStart();
	fdEventSetMessageId(MsgAltinItsGreatCanSellArmor);
	if(gmFlagGet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
		fdEventSetMessageId(MsgAltinHowAboutArentImpressedBy);
	}
	fdEventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
	fdEventEnd();
}

/* Until the first drop, the item merchant waits on the ledge east of the */
/* Sanctum, afraid her goods are soaked. After it she sells from the counter */
/* in the item shop, behind the small door in the east cliff, when spoken to */
/* across it; from the side she says none of her goods were damaged, or, once */
/* the hydros statues are beaten, that the girl from Xian bought a lot. */
static void scAltinTalkToItemMerchant(void) {
	if(gmFlagGet(FLAG_ALTIN_WATER_DROPPED_ONCE) == 0) {
		fdEventStart();
		fdEventSetMessageId(MsgAltinWillDoIfMyMerchandise);
		/* "Oh, no! What will I do if all my merchandise is soaked?" */
		fdEventShowMessage(ACTOR_FLOODED_ITEM_MERCHANT, 0);
		fdEventEnd();
		return;
	}

	if(scAltinLeaderFacesCounter() != 0) {
		mnShopRun(SHOP_ALTIN_ITEMS, ACTOR_ITEM_MERCHANT);
		return;
	}

	fdEventStart();
	fdEventSetMessageId(MsgAltinNoneMyGoodsWereDamaged);
	if(gmFlagGet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
		fdEventSetMessageId(MsgAltinGirlFromXianBoughtLot);
	}
	fdEventShowMessage(ACTOR_ITEM_MERCHANT, 0);
	fdEventEnd();
}

/* The innkeeper lets the party stay when spoken to across his counter. From */
/* the side he says they got a little damp but are doing okay, or, once the */
/* hydros statues are beaten, points them to a small temple west of the mine. */
static void scCottageRoomsDTalkToInnkeeper(void) {
	if(ALTIN_FACES_COUNTER(fdActorGet(ACTOR_PARTY_LEADER)->facing)) {
		mnInnCheckIn(ALTIN_INN, ACTOR_INNKEEPER);
		return;
	}

	fdEventStart();

	if(gmFlagGet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
		fdEventSetMessageId(MsgAltinThereSmallTempleWestAltin);
		/* "There is a small temple to the west of Altin Mine. They may..." */
		fdEventShowMessage(ACTOR_INNKEEPER, 0);
	}
	else {
		fdEventSetMessageId(MsgAltinWeGotLittleDampBut);
		/* "We got a little damp, but we're doing okay. We must count..." */
		/* Yes: "After all, a lot of good people have been hurt in this
		 * flooding." */
		/* No: "You have to remember, lots of people lost everything they..." */
		fdEventAskYesNo(ACTOR_INNKEEPER, 0);
	}

	fdEventEnd();
}

/* Walking north into one of the village's doors opens it, and Isaac walks in */
/* as the screen fades: the inn's under its sign (cell (10, 13)), the */
/* Sanctum's wide double door, which slides open (cells (20, 12) and */
/* (21, 12)), the item shop's in the east cliff (cell (28, 13)), the armor */
/* shop's (cell (11, 25)), the weapon shop's (cell (10, 27)), and the three */
/* houses': on the east side (cell (26, 21)), in the cliff north-west of the */
/* armor shop (cell (8, 19)), and by the rails at the south (cell (17, 25)). */
static void scAltinEnterDoor(void) {
	u32 actorId;
	FIELD_STATE *state;
	ACTOR *actor;
	s32 doorIndex;
	s32 column;
	s32 row;
	s32 rightColumn;

	state = FieldStateWork;
	fdEventStart();
	for(actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_ID; actorId++) {
		actor = fdActorGet(actorId);
		if(actor != NULL) {
			actor->motionFlg = 0;
		}
	}

	doorIndex = (s16)(state->stepTrigger - CELL_BEHAVIOR_ALTIN_FIRST_DOOR);
	column = (s16)Altin_Doors[doorIndex].x;
	row = (s16)Altin_Doors[doorIndex].y;
	if(doorIndex == DOOR_SANCTUM) {
		/* The Sanctum's door slides open with SE_THUMP, as Bilibin's does. */
		sySndPlay(SE_THUMP);
		fdMapGridCopyCellsTo(42, 33, column, row, 2, 2);
		rightColumn = column + 2;
		fdMapGridCopyCellsTo(42, 35, rightColumn, row, 2, 2);
		fdEventWait(4);
		fdMapGridCopyCellsTo(40, 33, column, row, 2, 2);
		fdMapGridCopyCellsTo(40, 35, rightColumn, row, 2, 2);
		fdEventWait(4);
	}
	else {
		sySndPlay(SE_DOOR_OPEN);
		/* The cliff house door's row in Altin_Doors plays its opening at */
		/* (39, 17), outside the village's camera box; its house stands at */
		/* (8, 17), above where the party comes back out */
		/* (ALTIN_FROM_COTTAGE_ROOMS_D_CLIFF_HOUSE), so that doorway is drawn
		 * open */
		/* here (a render with this copy made on entry shows the open doorway).
		 */
		if(doorIndex == CELL_BEHAVIOR_ALTIN_CLIFF_HOUSE_DOOR -
							CELL_BEHAVIOR_ALTIN_FIRST_DOOR) {
			fdMapGridCopyCellsTo(33, 42, 8, 17, 1, 2);
		}
		fdMapGridAnimateCells(Altin_Doors[doorIndex].steps, column, row);
	}

	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	if(doorIndex == DOOR_ARMOR_SHOP) {
		fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, 0);
	}
	else {
		if(doorIndex != DOOR_SANCTUM) {
			fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -4);
		}
		else {
			fdActorSetSprPriority(ACTOR_PARTY_LEADER, 2);
			fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
		}
	}
	fdEventWait(10);
	syImportEventSetExitRequest(state->stepTrigger);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* Walking north into one of the three doorways down into the mine plays the */
/* exit sound, and Isaac walks into the dark: the rail tunnel in the north */
/* cliff (cell (16, 13)), the doorway in the middle of the village where the */
/* rails run south (cell (20, 16)), or the one in the west cliff by the water */
/* beast (cell (1, 28)). */
static void scAltinWalkIntoMine(void) {
	FIELD_STATE *state;

	state = FieldStateWork;
	fdEventStart();
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	sySndPlay(SE_MAP_EXIT);
	fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
	syImportEventSetExitRequest(state->stepTrigger);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* The water beast's update function as it flees: it leaps every */
/* WATER_BEAST_HOP_PERIOD frames with SE_JUMP, and lands with a splash that */
/* shakes the screen for a few frames. */
static void scAltinBeastHop(ACTOR *actor) {
	ACTOR *beast = actor;

	/* userValue holds the frames left of the landing's shake, timer counts the
	 */
	/* frames of the hop, and the effect's countdown is set while the beast is
	 * in */
	/* the air, so that it lands with one splash. */
	if(beast->userValue != 0) {
		if(--beast->userValue == 1)
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	}
	if(beast->velY == 0) {
		fdActorSelectAnimation(beast, ANIM_STAND);
		beast->y.fixed -= toFIXED(3) / 2;
		if(beast->y.fixed < beast->groundHeight) {
			if(((FIELD_EFFECT *)actor)->countdown != 0) {
				sySndPlay(SE_SPLASH);
				((FIELD_EFFECT *)actor)->countdown = 0;
				/* Four frames of shake. */
				beast->userValue = 4;
				fdMapGridSetShake(0, 1, 1);
			}
			beast->y.fixed = beast->groundHeight;
		}
		beast->paused = 1;
	}
	else {
		beast->paused = 0;
	}
	if(beast->tmr == 0) {
		sySndPlay(SE_JUMP);
		((FIELD_EFFECT *)actor)->countdown = 1;
		fdActorSelectAnimation(beast, WATER_BEAST_ANIMATION_LEAP);
		beast->velY = toFIXED(3);
	}
	if(++beast->tmr == WATER_BEAST_HOP_PERIOD)
		beast->tmr = 0;
}

/* The water beast hops along the shore into the mine doorway in the west */
/* cliff (cell (1, 28)) and is gone with a splash. */
static void scAltinBeastHopAway(void) {
	ACTOR *beast;

	beast = fdActorGet(ACTOR_WATER_BEAST);
	beast->tmr = 0;
	beast->userValue = 0;
	beast->gravity = toFIXED(2) / 5;
	beast->update = scAltinBeastHop;
	fdActorSetSpeed(ACTOR_WATER_BEAST, 1.2);
	fdActorMoveToAndWait(ACTOR_WATER_BEAST, 28, 460);
	fdActorMoveToAndWait(ACTOR_WATER_BEAST, 24, 448);
	sySndPlay(SE_SPLASH);
	fdActorReleaseById(ACTOR_WATER_BEAST);
	fdMapGridSetShake(0, 1, 1);
	fdEventWait(4);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdEventWait(40);
	fdActorSetAnimation(ACTOR_WATER_BEAST, ANIM_STAND);
}

/* Move on the stone statue on the top ledge shifts it one cell east (to cell */
/* (22, 10)), uncovering the passage into the mine behind it. The game */
/* remembers whether it now stands there (FLAG_ALTIN_STATUE_MOVED), so it is */
/* still moved when the party comes back. */
static void scAltinCheckStatueMoved(void) {
	ACTOR *statue = fdActorGet(ACTOR_STONE_STATUE);

	if(FIELD_CELL(statue->x.fixed) == STONE_STATUE_MOVED_COLUMN) {
		gmFlagSet(FLAG_ALTIN_STATUE_MOVED);
	}
	else {
		gmFlagClear(FLAG_ALTIN_STATUE_MOVED);
	}
}

/* Sets up the scene being entered, at the village's water level. It works out
 */
/* the water level from the mine's drain flags first. Returns 0. */
s32 scInit(void) {
	s16 sceneId;

	if(gmFlagGet(FLAG_ALTIN_MINE_FIRST_DRAIN) != 0) {
		gmFlagSet(FLAG_ALTIN_WATER_DROPPED_ONCE);
	}

	if(gmFlagGet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0 ||
	   gmFlagGet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
		gmFlagSet(FLAG_ALTIN_WATER_DROPPED_TWICE);
	}

	if(gmFlagGet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0 &&
	   gmFlagGet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
		gmFlagSet(FLAG_ALTIN_WATER_DRAINED);
	}

	sceneId = MainRoutineStateWork.next;
	if(sceneId == MAP_ALTIN_VILLAGE) {
		scAltinApplyWaterLevel();
	}
	else if(sceneId == MAP_ALTIN_HOUSES) {
		scCottageRoomsDPlaceSheltering();
	}

	return 0;
}

/* Sets up the village at its water level: that stage's picture and cells, the
 */
/* villagers who are home, and the water beast while it is still there. */
static void scAltinApplyWaterLevel(void) {
	ACTOR *leader;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(gmFlagGet(FLAG_ALTIN_WATER_DRAINED) != 0) {
		fdMapGridCopyCellsTo(64, 32, 0, 32, 32, 32);
		fdMapGridCopyCellAttributes(64, 32, 32, 32, 0, 0);
		fdActorReleaseById(ACTOR_FLOODED_ARMOR_MERCHANT);
		fdActorReleaseById(ACTOR_FLOODED_ITEM_MERCHANT);
	}
	else if(gmFlagGet(FLAG_ALTIN_WATER_DROPPED_TWICE) != 0) {
		fdMapGridCopyCellsTo(64, 0, 0, 32, 32, 32);
		fdMapGridCopyCellAttributes(64, 0, 32, 32, 0, 0);
		fdActorReleaseById(ACTOR_OLD_MAN_ROCK_FLUSHED_OUT);
		fdActorReleaseById(ACTOR_FLOODED_ARMOR_MERCHANT);
		fdActorReleaseById(ACTOR_FLOODED_ITEM_MERCHANT);
	}
	else if(gmFlagGet(FLAG_ALTIN_WATER_DROPPED_ONCE) != 0) {
		fdMapGridCopyCellsTo(0, 64, 0, 32, 32, 32);
		fdMapGridCopyCellAttributes(0, 64, 32, 32, 0, 0);
		fdActorReleaseById(ACTOR_STRONGMAN_GUARDIAN_STATUES);
		fdActorReleaseById(ACTOR_OLD_MAN_ROCK_FLUSHED_OUT);
		fdActorReleaseById(ACTOR_FLOODED_ITEM_MERCHANT);
	}
	else {
		fdMapGridCopyCellAttributes(0, 32, 32, 32, 0, 0);
		fdActorReleaseById(ACTOR_MAN_WATERS_SUBSIDED);
		fdActorReleaseById(ACTOR_STRONGMAN_GUARDIAN_STATUES);
		fdActorReleaseById(ACTOR_OLD_MAN_ROCK_FLUSHED_OUT);
	}
	if(gmFlagGet(FLAG_ALTIN_WATER_BEAST_LEFT) != 0) {
		fdActorReleaseById(ACTOR_WATER_BEAST);
	}
	else {
		scQueueMusic(SE_AMBIENT_WATER);
		fdActorSetSprColById(ACTOR_WATER_BEAST, SPRITE_COLOR_RBG);
		fdActorSetAnimation(ACTOR_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
		syTaskAddFrame(scAltinBeastSpewWater, TASK_PASS_FRAME);
	}
	if(MainRoutineStateWork.entrance == ALTIN_FROM_SANCTUM) {
		gmFlagClear(FLAG_SHOW_AREA_NAME);
	}
	/* Map rows 32 to 63 are layer 2, the flood's stage, whose attributes the */
	/* ground took above. This lays layer 2's plain raised block at (0..3, 1..3)
	 */
	/* over its (20..23, 9..11), around the passage behind the statue; the */
	/* ground's cells there keep the passage and its walls (render probe Altin
	 */
	/* --cells 20,9,23,11), so the party walks as before. */
	/* XXX: what reads layer 2's attributes there afterwards; a probe of the */
	/* statue shoved by Move, or of any actor put on layer 2, would settle it.
	 */
	fdMapGridCopyCellAttributes(0, 33, 4, 3, 20, 41);
	if(gmFlagGet(FLAG_ALTIN_STATUE_MOVED) != 0) {
		fdActorSetPos(ACTOR_STONE_STATUE, 360, 168);
	}
	fdActorSetSprFlags(fdActorGet(ACTOR_STONE_STATUE), 0);
	fdActorSetSprColById(ACTOR_FIRST_CRATE, SPRITE_COLOR_HIDDEN);
	fdActorSetSprColById(ACTOR_SECOND_CRATE, SPRITE_COLOR_HIDDEN);
	fdActorSetSprColById(ACTOR_THIRD_CRATE, SPRITE_COLOR_HIDDEN);
	fdActorGet(ACTOR_FIRST_CRATE)->collisionFlg |= ACTOR_NOT_TARGETED;
	fdActorGet(ACTOR_SECOND_CRATE)->collisionFlg |= ACTOR_NOT_TARGETED;
	fdActorGet(ACTOR_THIRD_CRATE)->collisionFlg |= ACTOR_NOT_TARGETED;
	fdActorGet(ACTOR_FIRST_CRATE)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(ACTOR_SECOND_CRATE)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(ACTOR_THIRD_CRATE)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetSprPriority(ACTOR_FIRST_CRATE, 1);
	fdActorSetSprPriority(ACTOR_SECOND_CRATE, 1);
	fdActorSetSprPriority(ACTOR_THIRD_CRATE, 1);
	sySynch(1);
	fdEventStart();
	fdCameraMoveToFX(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0);
	fdMapGridCenterOnFollowedPoint();
	fdEventEnd();
	sySynch(1);
}

/* Sets up the rooms: until the first drop the Room One family shelter in the */
/* inn, until the second the Room Two family, and until the village is drained
 */
/* the weapon merchant waits there too. */
static void scCottageRoomsDPlaceSheltering(void) {
	ACTOR *actor;

	if(gmFlagGet(FLAG_ALTIN_WATER_DROPPED_ONCE) == 0) {
		fdActorSetPos(ACTOR_ROOM_ONE_GUEST, 808, 727);
		fdActorGet(ACTOR_ROOM_ONE_GUEST)->facing = FACING_SOUTH_SOUTHEAST;
		fdActorSetPos(ACTOR_ROOM_ONE_SECOND_GUEST, 794, 825);
	}
	if(gmFlagGet(FLAG_ALTIN_WATER_DROPPED_TWICE) == 0) {
		fdActorSetPos(ACTOR_ROOM_TWO_GUEST, 560, 710);
		fdActorGet(ACTOR_ROOM_TWO_GUEST)->facing = FACING_EAST_SOUTHEAST;
		fdActorSetPos(ACTOR_ROOM_TWO_SECOND_GUEST, 576, 710);
	}
	if(gmFlagGet(FLAG_ALTIN_WATER_DRAINED) == 0) {
		fdActorSetPos(ACTOR_WEAPON_MERCHANT, 295, 744);
		fdActorGet(ACTOR_WEAPON_MERCHANT)->facing = FACING_NORTH_NORTHWEST;
	}
	else {
		actor = fdActorGet(ACTOR_WEAPON_MERCHANT);
		actor->collisionFlg |= ACTOR_NEAR_TALK;
	}
	actor = fdActorGet(ACTOR_ARMOR_MERCHANT);
	if(actor != NULL) {
		actor->collisionFlg |= ACTOR_NEAR_TALK;
	}
	actor = fdActorGet(ACTOR_ITEM_MERCHANT);
	if(actor != NULL) {
		actor->collisionFlg |= ACTOR_NEAR_TALK;
	}
}

/* Gives a new drop of spray its look: drawn in front, its red and blue */
/* swapped, and small. */
static void scAltinInitSpew(ACTOR *drop) {
	drop->motionFlg = 0;
	drop->spr->image.pri = 1;
	fdActorSetSprCol(drop, SPRITE_COLOR_BGR);
	fdActorSetSprFlags(drop, 0);
	drop->scaleX = SPEW_START_SCALE;
	drop->scaleY = SPEW_START_SCALE;
}

/* A drop of spray's update function, through its SPEW_LIFETIME frames: it */
/* drifts east, slowing as it goes, with a random wobble, and moves two pixels
 */
/* a frame down the screen as it grows; in its last frames it spreads flat and
 */
/* rises a little as it splashes. It flashes white the more often the nearer */
/* its end, and is released. */
static void scAltinDriftSpew(ACTOR *drop) {
	drop->x.fixed +=
		SPEW_LATERAL_STEP(drop->tmr, (s16)((s32)(syRandomBelow(2)) - 1));
	if(drop->tmr <= 3) {
		drop->z.fixed -= syRandomBelow(toFIXED(1) / 2) + toFIXED(1);
		drop->scaleX += 3 * FIXED_ONE / 20;
		drop->scaleY -= FIXED_ONE / 25;
	}
	else {
		drop->z.fixed += toFIXED(2);
		drop->scaleX += 3 * FIXED_ONE / 100;
		drop->scaleY += 3 * FIXED_ONE / 100;
	}
	if(syRandomBelow(drop->tmr) == 0)
		fdActorSetSprCol(drop, SPRITE_COLOR_WHITE);
	if(drop->tmr != 0)
		drop->tmr--;
	else
		drop->tmr = (syRandomBelow(5)) * 2 + 2;
	if(--((FIELD_EFFECT *)drop)->countdown == 0) {
		drop->update = 0;
		fdActorRelease(drop);
	}
}

/* Task while the water beast spews ("We can't drink the water the monsters */
/* spew!"): every fourth frame a drop of water starts from its spout, living */
/* SPEW_LIFETIME frames. */
static void scAltinBeastSpewWater(void) {
	ACTOR *drop;
	if((FrameCount & 3) == 0) {
		drop = ((ACTOR *)syFieldActorCreate(SPRITE_DUST_PUFF, toFIXED(64), 0,
											toFIXED(400)));
		if(drop != NULL) {
			drop->tmr = SPEW_LIFETIME;
			drop->userValue = 0;
			((FIELD_EFFECT *)drop)->countdown = SPEW_LIFETIME;
			scAltinInitSpew(drop);
			drop->update = scAltinDriftSpew;
			fdActorSelectAnimation(drop, 1);
		}
	}
}

/* Walking down the west ledge to the ground above the water beast (cell */
/* (4, 26)) startles it: the camera goes to it, it stops spitting water, cries
 */
/* out, and hops away into the mine doorway beside it with a splash, for good.
 */
/* The old woman on the ledge shakes her head. */
static void scAltinScareOffBeast(void) {
	fdEventStart();
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(63, CAMERA_KEEP, 450, 1);
	fdCameraWaitForArrival();
	fdEventWait(30);
	fdActorSetAnimation(ACTOR_WATER_BEAST, ANIM_STAND);
	/* The spewing stops: its sound and its water end. */
	scQueueMusic(-1);
	syCloseEvent(scAltinBeastSpewWater);
	fdEventWait(20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_WATER_BEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_WATER_BEAST, FACING_EAST, 20);
	fdActorFaceDirection(ACTOR_WATER_BEAST, FACING_NORTH_NORTHEAST, 40);
	sySndPlay(SE_BEAST_CRY);
	fdActorRunRepeatedMotion(ACTOR_WATER_BEAST, 2);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_WATER_BEAST, FACING_NORTH_NORTHWEST, 40);
	scAltinBeastHopAway();
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
	fdCameraWaitForArrival();
	fdActorSetAnimationAndWait(ACTOR_THIRSTY_OLD_WOMAN, ANIM_SHAKE_HEAD);
	gmFlagSet(FLAG_ALTIN_WATER_BEAST_LEFT);
	fdEventEnd();
}

/* The cook from Xian paces between his pots, nodding over each. */
s32 CottageRoomsD_CookScript[] = {
	SC_ADD(SW_COLLISION, 128),
	SC_SPEED(2.4),
	SC_ACCELERATION(1.2),
	SC_LABEL(1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(342, 0, 746),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_ANIMATION(ANIM_NOD),
	SC_WAIT(30),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(356, 0, 746),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_ANIMATION(ANIM_NOD),
	SC_WAIT(50),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(342, 0, 746),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_ANIMATION(ANIM_NOD),
	SC_WAIT(20),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(ALTIN_COOK_LAST_POT_X, 0, 746),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_ANIMATION(ANIM_NOD),
	SC_WAIT(30),
	SC_SET(SW_FACING, FACING_EAST_SOUTHEAST),
	SC_WAIT(90),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

SCENE_ENTRANCE Altin_NoEntrances[] = {
	{0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE Altin_Entrances[] = {
	{0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ALTIN_FROM_WORLD_MAP, CONDITION_ALWAYS, 488, 0, 184, FACING_WEST, 0, -1,
	 -1, -1, -1, 0},
	{ALTIN_FROM_COTTAGE_ROOMS_D_INN, CONDITION_ALWAYS, 168, 0, 216,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ALTIN_FROM_SANCTUM, CONDITION_ALWAYS, 336, 0, 200, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{ALTIN_FROM_COTTAGE_ROOMS_D_EAST_HOUSE, CONDITION_ALWAYS, 424, 0, 344,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ALTIN_FROM_COTTAGE_ROOMS_D_CLIFF_HOUSE, CONDITION_ALWAYS, 136, 0, 312,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ALTIN_FROM_COTTAGE_ROOMS_D_SOUTH_HOUSE, CONDITION_ALWAYS, 280, 0, 408,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ALTIN_FROM_COTTAGE_ROOMS_D_ITEM_SHOP, CONDITION_ALWAYS, 456, 0, 224,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ALTIN_FROM_COTTAGE_ROOMS_D_ARMOR_SHOP, CONDITION_ALWAYS, 184, 0, 408,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ALTIN_FROM_COTTAGE_ROOMS_D_WEAPON_SHOP, CONDITION_ALWAYS, 168, 0, 440,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ALTIN_FROM_ALTIN_PEAK_FIRST_WATER_BEAST, CONDITION_ALWAYS, 24, 0, 456,
	 FACING_SOUTHEAST, 0, -1, -1, -1, -1, 0},
	{ALTIN_FROM_ALTIN_PEAK_SOUTH_ENTRANCE, CONDITION_ALWAYS, 264, 0, 224,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ALTIN_FROM_ALTIN_PEAK_JUNCTION, CONDITION_ALWAYS, 328, 0, 264,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ALTIN_FROM_ALTIN_PEAK_SPRITZ, CONDITION_ALWAYS, 343, 0, 177, FACING_SOUTH,
	 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE CottageRoomsD_Entrances[] = {
	{0, CONDITION_ALWAYS, 200, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{COTTAGE_ROOMS_D_EAST_HOUSE_FROM_ALTIN, CONDITION_ALWAYS, 159, 0, 258,
	 FACING_NORTH, 0, 25, 16, 255, 300, 0},
	{COTTAGE_ROOMS_D_CLIFF_HOUSE_FROM_ALTIN, CONDITION_ALWAYS, 336, 0, 213,
	 FACING_NORTH, 0, 268, 40, 500, 250, 0},
	{COTTAGE_ROOMS_D_SOUTH_HOUSE_FROM_ALTIN, CONDITION_ALWAYS, 688, 0, 204,
	 FACING_NORTH, 0, 542, 40, 768, 250, 0},
	{COTTAGE_ROOMS_D_WEAPON_SHOP_FROM_ALTIN, CONDITION_ALWAYS, 240, 0, 500,
	 FACING_NORTH, 0, 110, 336, 350, 540, 0},
	{COTTAGE_ROOMS_D_ITEM_SHOP_FROM_ALTIN, CONDITION_ALWAYS, 496, 0, 504,
	 FACING_NORTH, 0, 360, 336, 600, 540, 0},
	{COTTAGE_ROOMS_D_ARMOR_SHOP_FROM_ALTIN, CONDITION_ALWAYS, 752, 0, 516,
	 FACING_NORTH, 0, 635, 280, 845, 556, 0},
	{COTTAGE_ROOMS_D_INN_FROM_ALTIN, CONDITION_ALWAYS, 80, 0, 864, FACING_NORTH,
	 0, 16, 638, 432, 912, 0},
	{COTTAGE_ROOMS_D_INN_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 68, 0, 728,
	 FACING_SOUTH, 0, 16, 638, 432, 912, 0},
	{COTTAGE_ROOMS_D_INN_UPSTAIRS_FROM_INN, CONDITION_ALWAYS, 690, 0, 732,
	 FACING_SOUTH, 0, 480, 626, 896, 882, 0},
	{SCENE_TABLE_END},
};

u32 Altin_Exits[] = {
	SCENE_EXITS(Altin),
	SCENE_EXIT(ALTIN_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ALTIN),
	SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_INN, CottageRoomsD,
			   COTTAGE_ROOMS_D_INN_FROM_ALTIN),
	SCENE_EXIT(ALTIN_TO_SANCTUM, Sanctum, SANCTUM_FROM_ALTIN),
	SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_EAST_HOUSE, CottageRoomsD,
			   COTTAGE_ROOMS_D_EAST_HOUSE_FROM_ALTIN),
	SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_CLIFF_HOUSE, CottageRoomsD,
			   COTTAGE_ROOMS_D_CLIFF_HOUSE_FROM_ALTIN),
	SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_SOUTH_HOUSE, CottageRoomsD,
			   COTTAGE_ROOMS_D_SOUTH_HOUSE_FROM_ALTIN),
	SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_ITEM_SHOP, CottageRoomsD,
			   COTTAGE_ROOMS_D_ITEM_SHOP_FROM_ALTIN),
	SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_ARMOR_SHOP, CottageRoomsD,
			   COTTAGE_ROOMS_D_ARMOR_SHOP_FROM_ALTIN),
	SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_WEAPON_SHOP, CottageRoomsD,
			   COTTAGE_ROOMS_D_WEAPON_SHOP_FROM_ALTIN),
	SCENE_EXIT(ALTIN_TO_ALTIN_PEAK_FIRST_WATER_BEAST, AltinPeakFirstWaterBeast,
			   ALTIN_PEAK_FIRST_WATER_BEAST_FROM_ALTIN),
	SCENE_EXIT(ALTIN_TO_ALTIN_PEAK_SOUTH_ENTRANCE, AltinPeakSouthEntrance,
			   ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN),
	SCENE_EXIT(ALTIN_TO_ALTIN_PEAK_JUNCTION, AltinPeakJunction,
			   ALTIN_PEAK_JUNCTION_FROM_ALTIN),
	SCENE_EXIT(ALTIN_TO_ALTIN_PEAK_SPRITZ, AltinPeakSpritz,
			   ALTIN_PEAK_SPRITZ_FROM_ALTIN),
	SCENE_EXITS(CottageRoomsD),
	SCENE_EXIT(COTTAGE_ROOMS_D_EAST_HOUSE_TO_ALTIN, Altin,
			   ALTIN_FROM_COTTAGE_ROOMS_D_EAST_HOUSE),
	SCENE_EXIT(COTTAGE_ROOMS_D_CLIFF_HOUSE_TO_ALTIN, Altin,
			   ALTIN_FROM_COTTAGE_ROOMS_D_CLIFF_HOUSE),
	SCENE_EXIT(COTTAGE_ROOMS_D_SOUTH_HOUSE_TO_ALTIN, Altin,
			   ALTIN_FROM_COTTAGE_ROOMS_D_SOUTH_HOUSE),
	SCENE_EXIT(COTTAGE_ROOMS_D_WEAPON_SHOP_TO_ALTIN, Altin,
			   ALTIN_FROM_COTTAGE_ROOMS_D_WEAPON_SHOP),
	SCENE_EXIT(COTTAGE_ROOMS_D_ITEM_SHOP_TO_ALTIN, Altin,
			   ALTIN_FROM_COTTAGE_ROOMS_D_ITEM_SHOP),
	SCENE_EXIT(COTTAGE_ROOMS_D_ARMOR_SHOP_TO_ALTIN, Altin,
			   ALTIN_FROM_COTTAGE_ROOMS_D_ARMOR_SHOP),
	SCENE_EXIT(COTTAGE_ROOMS_D_INN_TO_ALTIN, Altin,
			   ALTIN_FROM_COTTAGE_ROOMS_D_INN),
	SCENE_EXIT(COTTAGE_ROOMS_D_INN_TO_INN_UPSTAIRS, CottageRoomsD,
			   COTTAGE_ROOMS_D_INN_UPSTAIRS_FROM_INN),
	SCENE_EXIT(COTTAGE_ROOMS_D_INN_UPSTAIRS_TO_INN, CottageRoomsD,
			   COTTAGE_ROOMS_D_INN_FROM_INN_UPSTAIRS),
	SCENE_EXITS_END,
};

ACTOR_PLACE Altin_NoActorPlacements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE Altin_ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 440, 0, 200, FACING_SOUTH_SOUTHWEST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					408, 0, 216, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 310, 0, 217, FACING_SOUTH_SOUTHWEST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_WANDER, 246, 0, 186, FACING_SOUTH_SOUTHWEST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_WANDER_NEAR, 58, 0, 206,
					FACING_EAST_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 104, 0, 286, FACING_EAST_SOUTHEAST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 41,
					0, 441, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK,
					0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 176, 0, 296, FACING_SOUTH_SOUTHEAST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 136, 0, 461, FACING_EAST_SOUTHEAST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_WANDER, 350, 0, 286, FACING_NORTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					60, 0, 474, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					344, 0, 168, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 88, 0, 397, FACING_EAST_SOUTHEAST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 422, 0, 208, FACING_EAST_SOUTHEAST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
					452, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
					332, 0, 360, FACING_SOUTH, TALK_FACING_KEEP,
					ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
					332, 0, 396, FACING_SOUTH, TALK_FACING_KEEP,
					ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

/* clang-format off */
ACTOR_PLACE CottageRoomsD_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 120, FACING_EAST_SOUTHEAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 105, 0, 233, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 182, FACING_WEST_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH,
        0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 120,
        FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 690, 0, 128, FACING_WEST_SOUTHWEST,
        TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 669, 0, 128, FACING_EAST_SOUTHEAST,
        TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 604, 0, 145, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 263, 0, 436, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 520, 0, 454, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 743, 0, 454, FACING_SOUTH_SOUTHEAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 103, 0, 744, FACING_SOUTH_SOUTHEAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER_NEAR, 228, 0, 756, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_IN_VIEW, (s32)CottageRoomsD_CookScript, 350, 0, 746,
        FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 646, 0, 734, FACING_SOUTH_SOUTHEAST,
        TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

TRIGGER Altin_NoTriggers[] = {
    {TRIGGER_TABLE_END},
};

TRIGGER Altin_Triggers[] = {
    {STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_ALTIN_WEST_MINE_DOORWAY, CONDITION_ALWAYS, EVENT(scAltinWalkIntoMine)},
    {STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_ALTIN_RAIL_TUNNEL, CONDITION_ALWAYS, EVENT(scAltinWalkIntoMine)},
    {STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_ALTIN_MIDDLE_MINE_DOORWAY, CONDITION_ALWAYS, EVENT(scAltinWalkIntoMine)},
    {TRIGGER_EXIT, CELL_BEHAVIOR_ALTIN_PASSAGE_BEHIND_STATUE, CONDITION_ALWAYS, ALTIN_TO_ALTIN_PEAK_SPRITZ},
    {TRIGGER_EXIT, CELL_BEHAVIOR_ALTIN_EAST_ROAD, CONDITION_ALWAYS, ALTIN_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_ALTIN_INN_DOOR, CONDITION_ALWAYS, EVENT(scAltinEnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_ALTIN_SANCTUM_DOOR, CONDITION_ALWAYS, EVENT(scAltinEnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_ALTIN_EAST_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(scAltinEnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_ALTIN_CLIFF_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(scAltinEnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_ALTIN_SOUTH_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(scAltinEnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_ALTIN_ITEM_SHOP_DOOR, CONDITION_ALWAYS, EVENT(scAltinEnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_ALTIN_ARMOR_SHOP_DOOR, CONDITION_ALWAYS, EVENT(scAltinEnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_ALTIN_WEAPON_SHOP_DOOR, CONDITION_ALWAYS, EVENT(scAltinEnterDoor)},
    {TRIGGER_TALK, ACTOR_GREETING_STRONGMAN, FLAG_HYDROS_STATUES_DEFEATED, EVENT(scAltinAskAboutFirstVisit)},
    {TRIGGER_TALK, ACTOR_GREETING_STRONGMAN, CONDITION_ALWAYS, MSGNO(MsgAltinThankYouSoMuchForDefeating)},
    {TRIGGER_TALK, ACTOR_CURIOUS_STRONGMAN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinHowCanThoseCreaturesSpitUp)},
    {TRIGGER_TALK, ACTOR_CURIOUS_STRONGMAN, CONDITION_ALWAYS, EVENT(scAltinAskAboutHydros)},
    {TRIGGER_TALK, ACTOR_WOMAN_COATED_HOUSE, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinHowHorribleMyHouseIsTotally)},
    {TRIGGER_TALK, ACTOR_WOMAN_COATED_HOUSE, CONDITION_ALWAYS, MSGNO(MsgAltinOnceTheMineOpensAgainWell)},
    {TRIGGER_TALK, ACTOR_MAN_SUBMERGED_MINE, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinEvenTheMineIsSubmergedNo)},
    {TRIGGER_TALK, ACTOR_MAN_SUBMERGED_MINE, CONDITION_ALWAYS, MSGNO(MsgAltinTheyAlreadyStartedMiningTheUpper)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_DESTROY_MONSTERS, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIfWeCouldJustDestroyThe)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_DESTROY_MONSTERS, CONDITION_ALWAYS, MSGNO(MsgAltinTheWaterDrainedOutOfThe)},
    {TRIGGER_TALK, ACTOR_WOMAN_WATER_LUCK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinAltinAlwaysUsedToRunShort)},
    {TRIGGER_TALK, ACTOR_WOMAN_WATER_LUCK, CONDITION_ALWAYS, MSGNO(MsgAltinThankGoodnessTheVillageIsFree)},
    {TRIGGER_TALK, ACTOR_THIRSTY_OLD_WOMAN, CONDITION_ALWAYS, EVENT(scAltinTalkToThirstyOldWoman)},
    {TRIGGER_TALK, ACTOR_MAN_WATERS_SUBSIDED, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWatersAroundHereHaveSubsided)},
    {TRIGGER_TALK, ACTOR_MAN_WATERS_SUBSIDED, CONDITION_ALWAYS, MSGNO(MsgAltinTheVillageWentRightBackTo)},
    {TRIGGER_TALK, ACTOR_STRONGMAN_GUARDIAN_STATUES, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWaterBeastsLookedExactlyLike)},
    {TRIGGER_TALK, ACTOR_STRONGMAN_GUARDIAN_STATUES, CONDITION_ALWAYS, MSGNO(MsgAltinSoThoseHydrosCreaturesReallyWere)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWatersHaveFlushedOutTons)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, CONDITION_ALWAYS, EVENT(scAltinAskAboutRuins)},
    {TRIGGER_TALK, ACTOR_FLOODED_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(scAltinTalkToArmorMerchant)},
    {TRIGGER_TALK, ACTOR_FLOODED_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(scAltinTalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_STRONGMAN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinYouCameToAltinAtA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_STRONGMAN, CONDITION_ALWAYS,
        MSGNO(MsgAltinWeCanGoBackToWork)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CURIOUS_STRONGMAN, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinItsAlmostLikeTheyreTurningThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CURIOUS_STRONGMAN, CONDITION_ALWAYS,
        MSGNO(MsgAltinIWasAfraidMoreWaterWould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_COATED_HOUSE, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinImGladISavedMyVery)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_COATED_HOUSE, CONDITION_ALWAYS,
        MSGNO(MsgAltinIHopeAltinMineWillThrive)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_SUBMERGED_MINE, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinEvenTheToughestOfMinersCant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_SUBMERGED_MINE, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheWaterSeepedIntoTheMine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_DESTROY_MONSTERS, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinIfTheWaterBeastsContinueTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_DESTROY_MONSTERS, CONDITION_ALWAYS,
        MSGNO(MsgAltinItsSoHumidInsideThatCave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WATER_LUCK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinWaterIsNiceButNotWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WATER_LUCK, CONDITION_ALWAYS,
        MSGNO(MsgAltinThatStrangeChamberInTheMines)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRSTY_OLD_WOMAN, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinUghTheMonstersSpewedOutAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRSTY_OLD_WOMAN, CONDITION_ALWAYS,
        MSGNO(MsgAltinYouFoughtPrettyWellDespiteAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WATERS_SUBSIDED, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinAsTheMonstersGoAwayThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WATERS_SUBSIDED, CONDITION_ALWAYS,
        MSGNO(MsgAltinThatMineIsFullOfHoles)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_GUARDIAN_STATUES, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinComeToThinkOfItIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_GUARDIAN_STATUES, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheOldGuardianStatuesAlwaysCreeped)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinItllBeBetterThanDiggingUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, CONDITION_ALWAYS,
        MSGNO(MsgAltinIUsedToFindAllSorts)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FLOODED_ARMOR_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinAllMyArmorInventoryIsRusting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FLOODED_ITEM_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinIfMyGoodsAreWetIll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_STATUE, CONDITION_ALWAYS, EVENT(scAltinCheckStatueMoved)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTIN_NEAR_BEAST, FLAG_ALTIN_WATER_BEAST_LEFT, EVENT(scAltinScareOffBeast)},
    {TRIGGER_TABLE_END},
};

TRIGGER CottageRoomsD_Triggers[] = {
    {TRIGGER_EXIT, COTTAGE_ROOMS_D_EAST_HOUSE_TO_ALTIN, CONDITION_ALWAYS, COTTAGE_ROOMS_D_EAST_HOUSE_TO_ALTIN},
    {TRIGGER_EXIT, COTTAGE_ROOMS_D_CLIFF_HOUSE_TO_ALTIN, CONDITION_ALWAYS, COTTAGE_ROOMS_D_CLIFF_HOUSE_TO_ALTIN},
    {TRIGGER_EXIT, COTTAGE_ROOMS_D_SOUTH_HOUSE_TO_ALTIN, CONDITION_ALWAYS, COTTAGE_ROOMS_D_SOUTH_HOUSE_TO_ALTIN},
    {TRIGGER_EXIT, COTTAGE_ROOMS_D_WEAPON_SHOP_TO_ALTIN, CONDITION_ALWAYS, COTTAGE_ROOMS_D_WEAPON_SHOP_TO_ALTIN},
    {TRIGGER_EXIT, COTTAGE_ROOMS_D_ITEM_SHOP_TO_ALTIN, CONDITION_ALWAYS, COTTAGE_ROOMS_D_ITEM_SHOP_TO_ALTIN},
    {TRIGGER_EXIT, COTTAGE_ROOMS_D_ARMOR_SHOP_TO_ALTIN, CONDITION_ALWAYS, COTTAGE_ROOMS_D_ARMOR_SHOP_TO_ALTIN},
    {TRIGGER_EXIT, COTTAGE_ROOMS_D_INN_TO_ALTIN, CONDITION_ALWAYS, COTTAGE_ROOMS_D_INN_TO_ALTIN},
    {TRIGGER_EXIT, COTTAGE_ROOMS_D_INN_TO_INN_UPSTAIRS, CONDITION_ALWAYS, COTTAGE_ROOMS_D_INN_TO_INN_UPSTAIRS},
    {TRIGGER_EXIT, COTTAGE_ROOMS_D_INN_UPSTAIRS_TO_INN, CONDITION_ALWAYS, COTTAGE_ROOMS_D_INN_UPSTAIRS_TO_INN},
    {TRIGGER_TALK, ACTOR_ROOM_ONE_GUEST, FLAG_ALTIN_WATER_DROPPED_ONCE, MSGNO(MsgAltinNoMatterWhatHappensIdNever)},
    {TRIGGER_TALK, ACTOR_ROOM_ONE_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOhNoTheWholeHouseIs)},
    {TRIGGER_TALK, ACTOR_ROOM_ONE_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinIHearThatSilkRoadIs)},
    {TRIGGER_TALK, ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_ALTIN_WATER_DROPPED_ONCE, MSGNO(MsgAltinOfCourseOurHouseWillBe)},
    {TRIGGER_TALK, ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIfTheMonsterThatDidThis)},
    {TRIGGER_TALK, ACTOR_ROOM_ONE_SECOND_GUEST, CONDITION_ALWAYS, EVENT(scCottageRoomsDAskAboutXianGirl)},
    {TRIGGER_TALK, ACTOR_ROOM_TWO_GUEST, FLAG_ALTIN_WATER_DROPPED_TWICE, MSGNO(MsgAltinNotManyPeopleComeUpSilk)},
    {TRIGGER_TALK, ACTOR_ROOM_TWO_GUEST, FLAG_HYDROS_STATUES_DEFEATED, EVENT(scCottageRoomsDAskAboutWayWest)},
    {TRIGGER_TALK, ACTOR_ROOM_TWO_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinAncientRuinsAndTreasureDefinitelyGo)},
    {TRIGGER_TALK, ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_ALTIN_WATER_DROPPED_TWICE, MSGNO(MsgAltinItsBeenALongTimeSince)},
    {TRIGGER_TALK, ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinDontBeTooHarshOnThose)},
    {TRIGGER_TALK, ACTOR_ROOM_TWO_SECOND_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinIfOnlyTheyHadLeftA)},
    {TRIGGER_TALK, ACTOR_MAN_BACK_IN_HIS_HOMETOWN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIDidntWantToLeaveMy)},
    {TRIGGER_TALK, ACTOR_MAN_BACK_IN_HIS_HOMETOWN, CONDITION_ALWAYS, MSGNO(MsgAltinReallyAncientRuinsAtTheBottom)},
    {TRIGGER_TALK, ACTOR_WOMAN_NO_PLACE_LIKE_HOME, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIHeardSomeoneKilledTheWater)},
    {TRIGGER_TALK, ACTOR_WOMAN_NO_PLACE_LIKE_HOME, CONDITION_ALWAYS, MSGNO(MsgAltinThereProbablyIsntMuchTreasureIn)},
    {TRIGGER_TALK, ACTOR_KISSING_GIRL, FLAG_HYDROS_STATUES_DEFEATED, EVENT(scCottageRoomsDAskAboutMonsters)},
    {TRIGGER_TALK, ACTOR_KISSING_GIRL, CONDITION_ALWAYS, MSGNO(MsgAltinSomethingsWrongWithMomAndPop)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(scCottageRoomsDTalkToWeaponMerchant)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(scAltinTalkToArmorMerchant)},
    {TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(scAltinTalkToItemMerchant)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(scCottageRoomsDTalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_WOMAN_FORMER_GLORY, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIdGiveAnythingForSomeoneTo)},
    {TRIGGER_TALK, ACTOR_WOMAN_FORMER_GLORY, CONDITION_ALWAYS, MSGNO(MsgAltinThereIsAnEndlessDesertWest)},
    {TRIGGER_TALK, ACTOR_COOK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinICameFromXianICook)},
    {TRIGGER_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgAltinWestOfTheMountainsIsVery)},
    {TRIGGER_TALK, ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_ALTIN_WATER_DROPPED_ONCE, MSGNO(MsgAltinNoneOfUsCanGoHome)},
    {TRIGGER_TALK, ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_ALTIN_WATER_DROPPED_TWICE, MSGNO(MsgAltinTheGuestsInRoomOneMoved)},
    {TRIGGER_TALK, ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheGuestsInRoomTwoAlso)},
    {TRIGGER_TALK, ACTOR_MAN_WAITING_TO_GO_HOME, CONDITION_ALWAYS, EVENT(scCottageRoomsDAskAboutMineBeasts)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_GUEST, FLAG_ALTIN_WATER_DROPPED_ONCE,
        MSGNO(MsgAltinLeavingAltinForTolbiWellThats)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinItsGoingToTakeALot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheMerchantsMustBeHavingA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_ALTIN_WATER_DROPPED_ONCE, MSGNO(MsgAltinNowIFinallyHaveAnExcuse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOneBlowsNotEnoughJabJab)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_SECOND_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinIWonderWhatBusinessThatGirl)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_GUEST, FLAG_ALTIN_WATER_DROPPED_TWICE, MSGNO(MsgAltinOfCourseYouCantGoWest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_GUEST, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinIThoughtTheVillageWasDone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheySayYouCanStillFind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_ALTIN_WATER_DROPPED_TWICE,
        MSGNO(MsgAltinIHateSittingAroundLikeThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinImJustGladToGetMy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_SECOND_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinHowCanITrulyBeHappy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_BACK_IN_HIS_HOMETOWN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheBottomOfTheMineThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_BACK_IN_HIS_HOMETOWN, CONDITION_ALWAYS,
        MSGNO(MsgAltinCripesAncientRuinsDownInThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_NO_PLACE_LIKE_HOME, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinMyHusbandsSenseForFindingTreasure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_NO_PLACE_LIKE_HOME, CONDITION_ALWAYS,
        MSGNO(MsgAltinIHadNoIdeaThereWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KISSING_GIRL, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinMomAndPopKnowSomethingAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KISSING_GIRL, CONDITION_ALWAYS,
        MSGNO(MsgAltinIWonderIfMomAndPop)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, FLAG_ALTIN_WATER_DRAINED, MSGNO(MsgAltinCripesIsThisHowMyWeapon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOnlyAMerchantKnowsTheJoy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinIveHeardOfAPassageIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinWeDriedOffAllThatArmor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinWellIfYouAreImpressedThen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinAFewThingsGotDampBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheGirlFromXianBoughtAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinWellComeThroughThisAllRight)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
        MSGNO(MsgAltinIThinkThatTempleWasCalled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FORMER_GLORY, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIKnowTheyDontLookSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FORMER_GLORY, CONDITION_ALWAYS,
        MSGNO(MsgAltinYouWontBeAbleToGo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIBecameACookAfterI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS,
        MSGNO(MsgAltinNoOneCanGoWestOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_ALTIN_WATER_DROPPED_ONCE, MSGNO(MsgAltinMostOfTheVillageIsStill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinItLooksLikeTheWaterHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WAITING_TO_GO_HOME, CONDITION_ALWAYS,
        MSGNO(MsgAltinItWillTakeSomeTimeBefore)},
    {EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_COTTAGE_ROOMS_D_NUT_BARREL, FLAG_ALTIN_NUT, FIND_ITEM(ITEM_NUT)},
    {EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_COTTAGE_ROOMS_D_COINS_BARREL, FLAG_ALTIN_COINS, FIND_COINS(9)},
    {EXAMINE_TRIGGER(EXAMINE_BED), CELL_BEHAVIOR_COTTAGE_ROOMS_D_SLEEP_BOMB_BED, FLAG_ALTIN_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_COTTAGE_ROOMS_D_RUSTED_POTS, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinThePotsAndPansAreRusted))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_COTTAGE_ROOMS_D_ROAST_BEEF, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinHeIsMakingRoastBeefEven))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_COTTAGE_ROOMS_D_SHRIMP, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinHeIsMakingShrimpInA))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_COTTAGE_ROOMS_D_BOOKCASE, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinThereIsABookTitledAltin))},
    {TRIGGER_TABLE_END},
};
/* clang-format on */

/* The opening pictures of the village's doors, two cells square. The Sanctum's
 */
/* door (the second) is wider, and scAltinEnterDoor redraws it by hand instead.
 */
u16 Altin_SanctumDoorCells[] = {
	MAPGRID_ANIMATION_STEP(42, 33, 2, 2, 0),
	MAPGRID_ANIMATION_STEP(42, 35, 2, 2, 5),
	MAPGRID_ANIMATION_STEP(40, 33, 2, 2, 0),
	MAPGRID_ANIMATION_STEP(40, 35, 2, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Altin_InnDoorCells[] = {
	MAPGRID_ANIMATION_STEP(35, 48, 2, 2, 5),
	MAPGRID_ANIMATION_STEP(33, 48, 2, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Altin_ItemShopDoorCells[] = {
	MAPGRID_ANIMATION_STEP(35, 35, 2, 2, 5),
	MAPGRID_ANIMATION_STEP(33, 35, 2, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Altin_ArmorShopDoorCells[] = {
	MAPGRID_ANIMATION_STEP(35, 46, 2, 2, 5),
	MAPGRID_ANIMATION_STEP(33, 46, 2, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Altin_EastHouseDoorCells[] = {
	MAPGRID_ANIMATION_STEP(35, 50, 2, 2, 5),
	MAPGRID_ANIMATION_STEP(33, 50, 2, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Altin_CliffHouseAndWeaponShopDoorCells[] = {
	MAPGRID_ANIMATION_STEP(35, 33, 2, 2, 5),
	MAPGRID_ANIMATION_STEP(33, 33, 2, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Altin_SouthHouseDoorCells[] = {
	MAPGRID_ANIMATION_STEP(35, 52, 2, 2, 5),
	MAPGRID_ANIMATION_STEP(33, 52, 2, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};

/* Each door's opening pictures and the cell they are drawn at, in the order of
 */
/* the doors' cell behaviors from the inn's. The doors are drawn on map layer */
/* 1, whose cells begin at the map's column 32: the inn's */
/* (41, 5) is that layer's (9, 5), on the inn's door just above the ground */
/* cell (10, 13) Isaac walks into (render probe Altin --entrance 2 */
/* --mark-cells 9,5,10,6,1). */
MAP_GRID_CELL_ANIMATION Altin_Doors[] = {
	{Altin_InnDoorCells, 41, 5},
	{Altin_SanctumDoorCells, 51, 6},
	{Altin_EastHouseDoorCells, 57, 17},
	{Altin_CliffHouseAndWeaponShopDoorCells, 39, 17},
	{Altin_SouthHouseDoorCells, 48, 25},
	{Altin_ItemShopDoorCells, 59, 9},
	{Altin_ArmorShopDoorCells, 42, 23},
	{Altin_CliffHouseAndWeaponShopDoorCells, 41, 27},
};
