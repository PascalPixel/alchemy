/* @file */
/* Imil, the snowbound village north of the Goma Range, near the lighthouse */
/* that long ago held the Water of Hermes, and its houses and Sanctum. */
/* */
/* Mia, the healer the party meets here, is of the Mercury Clan, who once */
/* lived in Imil and guarded the lighthouse. She is the last of them. */
/* */
/* An epidemic has struck the village. Until the party has beaten Saturos at */
/* Mercury Lighthouse (FLAG_SATUROS_BEATEN_AT_MERCURY), the villagers are */
/* sick or waiting for Mia. By that time the lighthouse is lit and the */
/* fountain at its foot flows again. */
/* */
/* Afterwards both scenes take their cured actor placements and lines, and */
/* the villagers talk of Mia leaving with the party. Some lines also change */
/* while she travels with them (FLAG_IN_PARTY(MIA)). */
/* */
/* - Imil (SceneId_Imil), the village, on both banks of a frozen river that */
/*   runs down its middle from a frozen waterfall. A young man shivers and */
/*   sneezes in the snow by the north-west house, waiting for Mia, and a */
/*   strongman counts out his exercises against the cold on the east ledge. */
/* */
/*   Six doors, whose cells are their exits past EXIT_CELL_BEHAVIOR_OFFSET, */
/*   open into the houses, the shops, the inn and the Sanctum */
/*   (SceneId_IceRooms). IMIL_TO_WORLD_MAP_SOUTH and IMIL_TO_WORLD_MAP_NORTH */
/*   lead out to the World Map, and the dark opening in the frozen waterfall */
/*   (CELL_BEHAVIOR_IMIL_PATH_TO_FALLS_CAVE) leads into Imil Falls Cave, */
/*   where Fever waits. Coming back out of it (IMIL_FROM_IMIL_FALLS_CAVE) */
/*   Isaac walks down into the village. */
/* */
/*   Two tasks run while the party is here. One makes smoke rise from the */
/*   five houses' chimneys, staggered over each second. The other turns */
/*   Isaac's footprints in the snow off on the frozen river and on the */
/*   raised ground under the big tree, and on elsewhere. */
/* */
/*   Stepping onto the river's ice (CELL_BEHAVIOR_IMIL_ICE) slides Isaac the */
/*   way the pad points, turning toward it as he goes, until a bank or a */
/*   block stops him. The snowman on the east bank at the foot of the falls */
/*   is the block the walkthrough's route needs: Move, cast from the snow, */
/*   pushes it west onto the ice, where it stays for the rest of the visit */
/*   and stops the last slide beside the way into the cave. */
/* */
/*   Searching finds 9 coins in the jar outside a house on the upper level, a */
/*   Vial in a chest above the first house, and a Lucky Medal in the upper */
/*   left grave by the Sanctum. */
/* */
/* - Ice rooms (SceneId_IceRooms), a rooms map drawn in ice, all of them */
/*   Imil's: inside the houses, the shops and the inn, whose area name is */
/*   Imil. By ICE_ROOMS_FROM_IMIL_EAST_NORTHEAST the same scene is the Imil */
/*   Sanctum. */
/* */
/*   While the epidemic lasts the weapon and armor sellers are sick in bed, */
/*   their two counters empty. Searching a counter shows the seller in his */
/*   bed, saying the shop is closed. The item seller lies sick too, and her */
/*   daughter minds the counter for her. */
/* */
/*   In the Sanctum Mia's two apprentices keep her place. Justin offers */
/*   healing across the counter, the first time saying he can act as the */
/*   party's healer (FLAG_IMIL_JUSTIN_OFFERED_HEALING). Megan asks whether */
/*   the party has visited the old couple who live near the entrance of town, */
/*   where Mia went (FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE). */
/* */
/*   In the old couple's house (ICE_ROOMS_HOUSE_SOUTHWEST) the old man moans */
/*   for help, and talking to him sends his wife hurrying to fetch Mia. */
/*   Either sets FLAG_IMIL_MIA_SENT_FOR, which changes the couple's lines and */
/*   where they stand. */
/* */
/*   Mia's healing is this scene's first event. It plays on entering the old */
/*   couple's house once Megan has told the party where Mia went, before */
/*   they have met her. The party finds her at the old man's bedside. She */
/*   heals him with Ply, asks who the party are, and the Mercury Lighthouse */
/*   flashes. "Was that... the lighthouse?" */
/* */
/*   She runs out, and the party follows her (FLAG_IMIL_MET_MIA). At the */
/*   lighthouse she goes on with them. Afterwards the old man lies in bed */
/*   feeling much better, and talking or Mind Read across the bed */
/*   (ACTOR_OLD_MANS_BED) reaches him. */
/* */
/*   Once Saturos is beaten the sellers are well and open their shops across */
/*   the counters. */
/* */
/*   The second event is Mia's farewell, the first time the party then */
/*   enters the Sanctum (FLAG_IMIL_MIA_SAID_FAREWELL). Her apprentices tell */
/*   her the lighthouse has been lit, and Mia tells them she will join */
/*   Isaac's quest to protect the world. They promise to protect Imil */
/*   themselves, as members of the Mercury Clan, and she leaves the village */
/*   in their hands, Megan and Justin. */
/* */
/*   Searching finds an Empty Bottle in the chest in the old couple's house, a
 */
/*   Sleep Bomb in a barrel in the weapon and armor shop, and Lucky Pepper in */
/*   the inn's oven. The oven's find always answers first, so the line about */
/*   the stew in it is unused. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event_inline.h"
#include "field/facing.h"
#include "field/field_effect.h"
#include "field/field_psynergy_state.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

extern s16 Ice_KeyHeadings[];
extern s16 Imil_SlideHeadings[];
extern s32 Imil_TrembleScript[];
extern s32 Imil_OldWomanHurryScript[];
extern s32 Imil_LeaderStepAsideScript[];
extern s32 Imil_BobScript[];
extern s32 Imil_SparkScript[];
extern s32 Imil_MiaCheckRoomScript[];
extern s32 Imil_MiaRunOutScript[];
extern s32 Imil_FaceTargetScript[];
extern s32 Imil_FollowMiaOutScript[];
extern SCENE_ENTRANCE Imil_TownEntrances[];
extern SCENE_ENTRANCE Imil_RoomEntrances[];
extern u32 Imil_Exits[];
extern ACTOR_PLACE Imil_TownActorPlacements[];
extern ACTOR_PLACE Imil_TownActorPlacementsCured[];
extern ACTOR_PLACE Imil_RoomActorPlacements[];
extern TRIGGER Imil_TownTriggers[];
extern TRIGGER Imil_RoomTriggers[];
extern u16 *Imil_DoorCellSteps[];
extern s16 Imil_DoorCellPoints[][2];
extern u16 Imil_OldManSitUpSteps[];
extern u16 Imil_OldManLieBackSteps[];
/* The point (x, y, z) the unused spiral and sway move around. */
extern s32 Imil_EffectCenter[];

/* The village's actors (Imil), numbering Imil_TownActorPlacements' rows, */
/* and Imil_TownActorPlacementsCured's, from ACTOR_FIRST_PLACED. */
typedef enum IMIL_VILLAGE_ACTOR_ID {
	/* A young man by the north-west house, shivering and sneezing. */
	ACTOR_FREEZING_YOUNG_MAN = ACTOR_FIRST_PLACED,
	/* A small boy who wanders the south of the village. */
	ACTOR_GREETING_BOY,
	/* A strongman counting his exercises in the snow on the east ledge. "One,
	 */
	/* two, three, four..." */
	ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
	/* The snowman on the east bank of the river at the foot of the frozen */
	/* falls, which Move pushes onto the ice. */
	ACTOR_SNOWMAN
} IMIL_VILLAGE_ACTOR_ID;

/* The actors of the houses and the Sanctum (IceRooms), numbering */
/* Imil_RoomActorPlacements' rows from ACTOR_FIRST_PLACED. */
/* */
/* The rows of Garet, Ivan and Mia take those characters' own slots. */
typedef enum ICE_ROOMS_ACTOR_ID {
	/* The old couple's house. */
	ACTOR_OLD_WOMAN = ACTOR_FIRST_PLACED,
	ACTOR_OLD_MAN,
	/* An unseen actor on the old man's bed. Once Mia has healed him, talking */
	/* or Mind Read here reaches him. */
	ACTOR_OLD_MANS_BED,
	/* An old man who shivers while the epidemic lasts. */
	ACTOR_SICK_OLD_MAN,
	/* An old man, behind the left counter of the weapon and armor shop once he
	 */
	/* is well, in bed before. */
	ACTOR_WEAPON_MERCHANT,
	/* An old man, behind the right counter once he is well, in bed before. */
	ACTOR_ARMOR_MERCHANT,
	/* A little girl who minds the shop while her mother is sick. */
	ACTOR_ITEM_MERCHANTS_DAUGHTER,
	/* The item seller, behind her counter once she is well, in bed before. */
	ACTOR_ITEM_MERCHANT,
	ACTOR_INNKEEPER,
	/* A woman who waits on the inn's guests. */
	ACTOR_INN_ATTENDANT,
	/* A young man who keeps the restaurant and offers its menu. */
	ACTOR_RESTAURANT_OWNER,
	/* Mia's apprentices, a small boy who keeps the Sanctum and a little girl */
	/* ("I will be a kind girl", MsgImilIWillBeAKindGirl). */
	/* */
	/* Her farewell names them together without saying which is which, so each
	 */
	/* takes the English name of the boy's or the girl's gender. */
	ACTOR_JUSTIN,
	ACTOR_MEGAN,
	/* Invisible in the old couple's house. */
	/* */
	/* It is SPRITE_CRATE in SPRITE_COLOR_HIDDEN, every pixel transparent, so */
	/* the player sees only the stack of logs in the room's cells. It is */
	/* ACTOR_NOT_TARGETED, never a talk or Psynergy target. */
	/* */
	/* It is kept under the party as altin.c keeps its ground sprites. Vault's
	 */
	/* crate (log_rooms_a.c) uses the same sprite. */
	ACTOR_INVISIBLE_CRATE
} ICE_ROOMS_ACTOR_ID;

/* The village's own temporary flags, which last until the party leaves the */
/* map. */
typedef enum IMIL_TOWN_FLAG {
	/* Whether the leader leaves footprints in the snow, from where he stands */
	/* (scImilUpdateFootprints). The one set says which he last did. */
	FLAG_IMIL_NO_FOOTPRINTS = FLAG_SCENE_TEMP_00,
	FLAG_IMIL_FOOTPRINTS = FLAG_SCENE_TEMP_01,
	/* Move has pushed the snowman to its place, for the rest of the visit. */
	FLAG_IMIL_SNOWMAN_MOVED = FLAG_SCENE_TEMP_03
} IMIL_TOWN_FLAG;

/* The map cell Move leaves the snowman on, the river's ice one west of where */
/* it stands (262, 124). */
#define SNOWMAN_MOVED_COLUMN 15
#define SNOWMAN_MOVED_ROW	 7

/* Imil's shops, by their numbers in the shop table. */
enum { SHOP_IMIL_WEAPONS = 10, SHOP_IMIL_ARMOR = 11, SHOP_IMIL_ITEMS = 12 };

/* Imil's inn, as mnInnCheckIn numbers the inns. */
enum { IMIL_INN = 4 };

/* The cell behaviors the village's step triggers answer. */
typedef enum IMIL_CELL_BEHAVIOR {
	/* The path north out of the village (scImilLeaveNorth). */
	CELL_BEHAVIOR_IMIL_PATH_TO_FALLS_CAVE = 4,
	/* The frozen river's ice, slid across (scImilSlideOnIce). */
	CELL_BEHAVIOR_IMIL_ICE = 90
} IMIL_CELL_BEHAVIOR;

/* The weapon and armor counters' cell behaviors in the shop. Searching one */
/* facing north reaches its seller. */
typedef enum ICE_ROOMS_CELL_BEHAVIOR {
	CELL_BEHAVIOR_ICE_ROOMS_WEAPON_COUNTER = 10,
	CELL_BEHAVIOR_ICE_ROOMS_ARMOR_COUNTER = 11
} ICE_ROOMS_CELL_BEHAVIOR;

/* The leader faces a counter: FACING_IS_NORTH, in the two spellings the sites
 */
/* match with, the shops' from the heading read as s16 and the inn's and the */
/* Sanctum's from the halfword. */
/* UB: the shops' form shifts a negative heading left. */
#ifdef UBFIX
#define IMIL_FACES_COUNTER(facing) FACING_IS_NORTH((u16)(facing))
#else
#define IMIL_FACES_COUNTER(facing)                                             \
	((u32)(((facing) << 16) + 0x5fff0000) <= 0x3ffe0000)
#endif
#define IMIL_FACES_COUNTER_HALFWORD(facing)                                    \
	((u16)((facing) - (FACING_NORTHWEST + 1)) <=                               \
	 FACING_NORTHEAST - FACING_NORTHWEST - 2)

/* The item merchant lying in bed, only her head above the covers */
/* (SPRITE_WOMAN_IN_GREEN_DRESS, woman_in_green_dress.png). */
enum { IMIL_ANIMATION_LIE_IN_BED = 5 };

/* Mia's healing, the first time the party enters the old couple's house */
/* once Megan has told them Mia went there: the party finds her at the old */
/* man's bedside. She heals him with Ply and asks who they are, and the */
/* whole room flashes pink. "Was that... the lighthouse?" She runs out, and */
/* the party hurries after her. */
static void scImilMeetMiaAtOldCouplesHouse(void);

/* Mia casts Ply on the old man in bed, and he bobs up and speaks as it ends. */
static void scImilMiaCastsPlyOnOldMan(void);

/* Smoke rises from the chimneys of Imil's five houses, staggered over each */
/* second. */
static void scImilSpawnChimneySmoke(void);

/* Isaac leaves no footprints on the frozen river or on the raised ground */
/* under the big tree, and leaves them everywhere else in the snow; checked */
/* each frame while the party is in the village. */
static void scImilUpdateFootprints(void);

/* Move on the snowman on the east bank at the foot of the frozen falls, */
/* pushed west onto the river's ice (cell (15, 7)): it stands there for the */
/* rest of the visit, a block that stops Isaac's slides up the river beside */
/* the way into Imil Falls Cave. */
/* */
/* The cell takes the attributes of the bank north of it (15, 6), so a slide */
/* stops against it. The row answers only while Isaac stands in the snow, */
/* not on the ice or the raised ground under the tree. Before TBS_PATCH_3 the */
/* cell is walled off wherever Move took the snowman; from then on only once */
/* the snowman is on it. scInit runs it again when the village */
/* reloads with FLAG_IMIL_SNOWMAN_MOVED set. */
static void scImilPlaceMovedSnowman(void);

/* Isaac comes back out of Imil Falls Cave through the frozen waterfall */
/* (IMIL_FROM_IMIL_FALLS_CAVE) and walks down onto the bank below it. */
static void scImilArriveFromFallsCave(void);

/* Mia's farewell, the first time the party enters the Imil Sanctum after */
/* Saturos is beaten: her apprentices Megan and Justin tell her the */
/* lighthouse is lit, and she leaves the village in their hands to go on */
/* with the party. */
static void scImilMiaSaysFarewellToApprentices(void);

/* Isaac slides along Imil's frozen river the way the pad points, turning */
/* toward it as he goes, until a bank or a block stops him. */
static void scLeaderSlideOnIce(void);

/* A sick person trembles: a quick squash, then a pause. */
static s32 scImilUpdateTremble(ACTOR *actor) {
	switch(actor->tmr) {
	case 6:
		actor->scaleX -= FIXED_ONE / 4;
		actor->scaleY += FIXED_ONE / 8;
		break;
	case 4:
		actor->scaleX += FIXED_ONE / 8;
		actor->scaleY -= FIXED_ONE / 16;
		break;
	case 2:
		actor->scaleX += FIXED_ONE / 16;
		actor->scaleY -= FIXED_ONE / 32;
		break;
	case 0:
		actor->scaleX += FIXED_ONE / 16;
		actor->scaleY -= FIXED_ONE / 32;
		if(actor->userValue != 0) {
			actor->tmr = (u32)syRandomNext() % 40 + 40;
		}
		else {
			actor->tmr = (u32)syRandomNext() % 20 + 20;
		}
		break;
	}
	actor->tmr--;
	return 1;
}

#include "shared/actor_facing.c"

/* Imil is two scenes: the village, and its houses and Sanctum. */
const SCENE_ENTRANCE *scGetEntrances(void) {
	if(MainRoutineStateWork.next == SCENE_ICE_ROOMS) {
		return Imil_RoomEntrances;
	}
	return Imil_TownEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

const u32 *scGetExits(void) { return Imil_Exits; }

/* Once Saturos is beaten at Mercury Lighthouse, the villagers are well again.
 */
const ACTOR_PLACE *scGetActorPlacements(void) {
	if(MainRoutineStateWork.next == SCENE_ICE_ROOMS) {
		scAssignViewFlags(Imil_RoomActorPlacements);
		if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
			Imil_RoomActorPlacements[ACTOR_RESTAURANT_OWNER -
									 ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY;
			Imil_RoomActorPlacements[ACTOR_SICK_OLD_MAN - ACTOR_FIRST_PLACED]
				.x = toFIXED(182);
			Imil_RoomActorPlacements[ACTOR_SICK_OLD_MAN - ACTOR_FIRST_PLACED]
				.z = toFIXED(564);
			Imil_RoomActorPlacements[ACTOR_SICK_OLD_MAN - ACTOR_FIRST_PLACED]
				.movement = MOVEMENT_TYPE_WANDER;
		}
		return Imil_RoomActorPlacements;
	}

	if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
		return Imil_TownActorPlacementsCured;
	}
	return Imil_TownActorPlacements;
}

/* Isaac steps onto the frozen river running down the middle of Imil and */
/* slides along it the way the pad points, until a bank or a block stops him. */
static void scImilSlideOnIce(void) { scLeaderSlideOnIce(); }

/* The village's, or the houses' and the Sanctum's, conversations and */
/* Psynergy targets. */
const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.next == SCENE_ICE_ROOMS) {
		return Imil_RoomTriggers;
	}
	return Imil_TownTriggers;
}

/* Isaac talks to the young man shivering by the north-west house, before */
/* Saturos is beaten: he sneezes and asks whether Mia is here yet, or, while */
/* she travels with the party, says the epidemic will just come back next */
/* year. */
static void scImilTalkToFreezingYoungMan(void) {
	fdEventStart();
	if(gmFlagGet(FLAG_IN_PARTY(MIA)) != 0) {
		/* "Even if Mia heals us, this epidemic will just come back next year."
		 */
		fdEventSetMessageId(MSGNO(MsgImilEvenIfMiaHealsUs));
	}
	else {
		/* "Brrrrr... Ah-choo!!! I'm freezing! Is Mia here yet?" */
		fdEventSetMessageId(MSGNO(MsgImilBrrrrrChooImFreezingMia));
	}
	fdEventShowMessage(ACTOR_FREEZING_YOUNG_MAN, 0);
	fdEventEnd();
}

/* Mind Read on the young man shivering by the north-west house, before */
/* Saturos is beaten: Mia should be here by now, or, while she travels with */
/* the party, he cannot see why anyone would exercise in this cold. */
static void scImilReadFreezingYoungMansMind(void) {
	fdEventStart();
	if(gmFlagGet(FLAG_IN_PARTY(MIA)) != 0) {
		/* "I can't understand why any one would exercise in this cold..." */
		fdEventSetMessageId(MSGNO(MsgImilCantUnderstandWhyAnyOne));
	}
	else {
		/* "Mia should be here by now! Where is she?" */
		fdEventSetMessageId(MSGNO(MsgImilMiaShouldHereByNow));
	}
	fdEventShowMessage(ACTOR_FREEZING_YOUNG_MAN, 0);
	fdEventEnd();
}

/* Isaac talks to the small boy wandering the south of Imil, before Saturos is
 */
/* beaten: he turns and asks whether Isaac is new in Imil, and warns of the */
/* cold going around or doubts the answer. */
static void scImilGreetVisitor(void) {
	fdEventStart();
	fdActorFaceActor(ACTOR_GREETING_BOY, ACTOR_PARTY_LEADER, 10);
	/* "Hi! Are you new in Imil?" */
	/* Yes: "I should warn you, there's a bad cold going around..." */
	/* No: "You don't look familiar. Are you sure you're not...fibbing?" */
	fdEventSetMessageId(MSGNO(MsgImilHiNewInImil));
	fdEventAskYesNo(ACTOR_GREETING_BOY, 0);
	fdEventEnd();
}

/* Isaac talks to the strongman exercising on the east ledge: he counts "One, */
/* two, three, four!", stops, and turns to talk. */
/* */
/* While the epidemic lasts he stays indoors too much in the cold, he says; */
/* once Saturos is beaten he asks whether the lighthouse fountain is really */
/* back, and wonders what he trained so hard for. */
static void scImilTalkToStrongmanKeepingInShape(void) {
	fdEventStart();

	if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
		/* "One, two, three, four...", then "I was training hard so that I */
		/* could withstand any disease... And now the lighthouse fountain is
		 * back?" */
		/* Yes: "What did I train so hard for all this time?" */
		/* No: "No, it can't be! Everyone is in perfect health!" */
		fdEventSetMessageId(MSGNO(MsgImilCountingAfterDeparture));
		fdEventShowMessage(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, 0);
		fdActorSetAttachedEffect(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
								 ATTACHED_EFFECT_IN_FRONT |
									 ATTACHED_EFFECT_ONCE);
		fdEventWait(40);
		fdActorSetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, ANIM_STAND);
		fdEventWait(20);
		fdActorFaceActor(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, ACTOR_PARTY_LEADER,
						 20);
		fdEventAskYesNo(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, 0);
		fdActorFaceDirection(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
							 FACING_SOUTH_SOUTHEAST, 10);
		fdActorSetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
							STRONGMAN_ANIMATION_SQUATS);
	}
	else {
		/* "One, two, three, four!", then "I tend to stay indoors a lot when the
		 */
		/* weather turns cold..." */
		fdEventSetMessageId(MSGNO(MsgImilOneTwoThreeFour));
		fdEventShowMessage(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, 0);
		fdActorSetAttachedEffect(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
								 ATTACHED_EFFECT_IN_FRONT |
									 ATTACHED_EFFECT_ONCE);
		fdEventWait(40);
		fdActorSetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, ANIM_STAND);
		fdEventWait(20);
		fdActorFaceActor(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, ACTOR_PARTY_LEADER,
						 20);
		fdEventShowMessage(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, 0);
		fdActorFaceDirection(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
							 FACING_SOUTH_SOUTHEAST, 10);
		fdActorSetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
							STRONGMAN_ANIMATION_SQUATS);
	}

	fdEventEnd();
}

/* Isaac talks to the old man in the old couple's house, the first house from */
/* the south entrance: before Mia has been sent for he moans "Oooh... Help */
/* me...", and his wife hurries over and cries for someone to get Mia */
/* (FLAG_IMIL_MIA_SENT_FOR). */
/* */
/* After Mia's visit he sits up in bed and says he feels much better; once */
/* Saturos is beaten he laughs at having said he would die of it. */
static void scImilTalkToOldMan(void) {
	if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
		fdEventStart();
		fdActorFaceActor(ACTOR_OLD_MAN, ACTOR_PARTY_LEADER, 0);
		fdEventWait(10);
		fdEventSetMessageId(MsgMercuryReallySayDie);
		/* "Did I really say I was going to die from that disease?" */
		/* Yes: "Ha ha ha! I must have really felt sick to say that! I'm
		 * good..." */
		/* No: "That's right! The strong old man of Imil, that's me!" */
		fdEventAskYesNo(ACTOR_OLD_MAN, 0);
		fdEventEnd();
	}
	else {
		if(gmFlagGet(FLAG_IMIL_MET_MIA) != 0) {
			fdEventStart();
			fdActorSetAnimation(ACTOR_OLD_MAN,
								OLD_MAN_WITH_CANE_ANIMATION_SIT_UP);
			fdMapGridPlayMapEdit(Imil_OldManSitUpSteps, 10, 69);
			fdEventSetMessageId(MsgMercuryFeelMuchBetter);
			/* "I feel much better, thanks to Mia. I should be fine now." */
			fdEventShowMessage(ACTOR_OLD_MAN, 0);
			fdActorSetAnimation(ACTOR_OLD_MAN,
								OLD_MAN_WITH_CANE_ANIMATION_LIE_BACK);
			fdMapGridAnimateCells(Imil_OldManLieBackSteps, 10, 69);
			fdEventEnd();
		}
		else {
			fdEventStart();
			fdActorGet(ACTOR_OLD_MAN)->tmr = 10;
			fdActorSetScript(ACTOR_OLD_MAN, Imil_TrembleScript);
			fdEventSetMessageId(MsgMercuryOoohHelpMe);
			/* "Oooh... Help me..." */
			fdEventShowMessage(ACTOR_OLD_MAN, 0);
			fdActorStop(ACTOR_OLD_WOMAN);
			fdActorShowEmote(ACTOR_OLD_WOMAN,
							 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
			fdActorFaceDirection(ACTOR_OLD_WOMAN, FACING_NORTH_NORTHEAST, 10);
			fdActorStartRepeatedMotion(ACTOR_OLD_WOMAN, 2);
			/* "Grandpa! Grandpa! Hold on!" */
			fdEventShowMessageThenWait(ACTOR_OLD_WOMAN, 0, 20);
			fdActorSetScript(ACTOR_PARTY_LEADER, Imil_LeaderStepAsideScript);
			fdActorSetSpeed(ACTOR_OLD_WOMAN, 1.6);
			fdActorRunScriptAndWait(ACTOR_OLD_WOMAN, Imil_OldWomanHurryScript);
			fdEventWait(40);
			fdActorJump(ACTOR_OLD_WOMAN, 2, 0);
			fdActorStartRepeatedMotion(ACTOR_OLD_WOMAN, 2);
			fdActorSetAttachedEffect(ACTOR_OLD_WOMAN, ATTACHED_EFFECT_IN_FRONT |
														  ATTACHED_EFFECT_ONCE);
			fdEventWait(60);
			/* "Oh, no! This is terrible!" */
			fdEventShowMessageThenWait(ACTOR_OLD_WOMAN, 0, 10);
			fdActorFaceDirection(ACTOR_OLD_WOMAN, FACING_SOUTH_SOUTHEAST, 20);
			fdActorStartRepeatedMotion(ACTOR_OLD_WOMAN, 2);
			/* "Get Mia... Please, get Mia!" */
			fdEventShowMessage(ACTOR_OLD_WOMAN, 0);
			fdActorGet(ACTOR_OLD_WOMAN)->collisionFlg ^= ACTOR_PUSHABLE;
			syImportGameFlagSet(FLAG_IMIL_MIA_SENT_FOR);
			fdEventEnd();
		}
	}
}

/* Isaac talks to the weapon seller lying sick in bed, before Saturos is */
/* beaten: he asks whether the party wants weapons, and coughs that he cannot */
/* sell until he is cured, or blames the travelers from down south for the */
/* disease and tells them to begone. */
static void scImilAskAboutWeapons(void) {
	fdEventStart();
	/* "Do you want weapons?" */
	/* Yes: "I can't sell weapons until I am cured..." */
	/* No: "It's the travelers from down south who brought disease to our..." */
	fdEventSetMessageId(MSGNO(MsgImilDoWantWeapons));
	fdEventAskYesNo(ACTOR_WEAPON_MERCHANT, 0);
	fdEventEnd();
}

/* Isaac talks to the young man who keeps Imil's restaurant, before Saturos */
/* is beaten: he asks whether the party wants to see the menu. */
static void scImilAskAboutRestaurantMenu(void) {
	fdEventStart();
	/* "Do you want to see the restaurant menu?" */
	/* Yes: "Here you go. We're a small town, mind you, so the selection..." */
	/* No: "Tonight's dinner is stew and rye bread..." */
	fdEventSetMessageId(MSGNO(MsgImilDoWantSeeRestaurantMenu));
	fdEventAskYesNo(ACTOR_RESTAURANT_OWNER, 0);
	fdEventEnd();
}

/* Isaac talks to Megan in the Imil Sanctum before Saturos is beaten: she asks
 */
/* whether the party has visited the old couple near the entrance of town, */
/* where Mia went (FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE). While Mia travels with
 */
/* them she says everyone counts on Mia instead. */
static void scImilTalkToMegan(void) {
	fdEventStart();
	if(gmFlagGet(FLAG_IN_PARTY(MIA)) != 0) {
		/* "Everyone counts on Mia. That's why she is always so busy." */
		fdEventSetMessageId(MSGNO(MsgImilEveryoneCountsOnMiaThats));
		fdEventShowMessage(ACTOR_MEGAN, 0);
	}
	else {
		fdEventSetMessageId(MsgImilHaveVisitedOldCoupleWho);
		/* "Have you visited the old couple who live near the entrance..." */
		/* Yes: "Oh. That's too bad. That's where Mia was headed. You..." */
		/* No: "Mia went to their house. You might have seen her if you..." */
		fdEventAskYesNo(ACTOR_MEGAN, 0);
		syImportGameFlagSet(FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE);
		syImportGameFlagSet(FLAG_IMIL_MIA_SENT_FOR);
	}
	fdEventEnd();
}

/* Mind Read on Megan in the Imil Sanctum, before Saturos is beaten: Mia had a
 */
/* bad feeling about something, or, while Mia travels with the party, they */
/* must help her. */
static void scImilReadMegansMind(void) {
	fdEventStart();
	if(gmFlagGet(FLAG_IN_PARTY(MIA)) != 0) {
		/* "We have to do whatever we can to help Mia. She's way too busy." */
		fdEventSetMessageId(MSGNO(MsgImilWeHaveDoWhateverWe));
	}
	else {
		/* "Mia was saying she has a bad feeling about something..." */
		fdEventSetMessageId(MSGNO(MsgImilMiaWasSayingSheHas));
	}
	fdEventShowMessage(ACTOR_MEGAN, 0);
	fdEventEnd();
}

/* Isaac talks to the old woman in the old couple's house after Mia has been */
/* sent for and met: she asks what happened in the lighthouse to the */
/* northeast. */
static void scImilTalkToOldWoman(void) {
	fdEventStart();
	/* "What happened in the lighthouse to the northeast?" */
	fdEventSetMessageId(MSGNO(MsgImilHappenedInLighthouseNortheast));
	fdEventShowMessage(ACTOR_OLD_WOMAN, 0);
	fdActorFaceDirection(ACTOR_OLD_WOMAN, FACING_SOUTH_SOUTHEAST, 10);
	fdEventEnd();
}

/* Mind Read on the old woman in the old couple's house, before Saturos is */
/* beaten: about Mia running all over town, then her husband's exaggerating */
/* once Mia is sent for, then Mia's clan once the party has met her. */
static void scImilReadOldWomanMind(void) {
	fdEventStart();
	if(gmFlagGet(FLAG_IMIL_MET_MIA) != 0) {
		/* "Mia's clan once lived here and guarded the lighthouse to the
		 * east..." */
		fdEventSetMessageId(MSGNO(MsgImilMiaClanOnceLivedHere));
	}
	else if(gmFlagGet(FLAG_IMIL_MIA_SENT_FOR) != 0) {
		/* "He's always exaggerating things, but I don't think he's..." */
		fdEventSetMessageId(MSGNO(MsgImilHesAlwaysExaggeratingThingsBut));
	}
	else {
		/* "Mia is running around town, caring for the sick..." */
		fdEventSetMessageId(MSGNO(MsgImilMiaRunningAroundTownCaring));
	}
	fdEventShowMessage(ACTOR_OLD_WOMAN, 0);
	fdEventEnd();
}

/* After Mia's visit, Isaac talks to the old man across his bed in the old */
/* couple's house, which reaches him as talking to him does. */
static void scImilTalkToOldManInBed(void) { scImilTalkToOldMan(); }

/* After Mia's visit, Mind Read across the bed in the old couple's house: the */
/* old man thinks Mia is a good girl. */
static void scImilReadOldManInBedMind(void) {
	fdEventStart();
	/* "Mia is a good girl. I wish I had a daughter like her..." */
	fdEventSetMessageId(MSGNO(MsgImilMiaIsAGoodGirlI));
	fdEventShowMessage(ACTOR_OLD_MANS_BED, 0);
	fdEventEnd();
}

/* Mind Read on Justin in the Imil Sanctum, before Saturos is beaten: he */
/* thinks the party are not bad people, or, while Mia travels with them, he */
/* feels grown-up serving as healer. */
static void scImilReadJustinsMind(void) {
	fdEventStart();
	if(gmFlagGet(FLAG_IN_PARTY(MIA)) != 0) {
		/* "I feel like a grown-up when I am serving as a healer..." */
		fdEventSetMessageId(MSGNO(MsgImilFeelLikeGrownUpWhen));
	}
	else {
		/* "These folk are okay! They don't seem like bad people." */
		fdEventSetMessageId(MSGNO(MsgImilTheseFolkOkayTheyDont));
	}
	fdEventShowMessage(ACTOR_JUSTIN, 0);
	fdEventEnd();
}

/* Isaac walks north into one of Imil's six doors: everyone stops, the door */
/* opens and he walks in. */
static void scImilOpenDoorAndExit(void) {
	FIELD_STATE *state;
	ACTOR *actor;
	u32 i;
	s32 exit;

	state = FieldStateWork;
	fdEventStart();
	for(i = ACTOR_FIRST_PLACED; i <= ACTOR_LAST_ID; i++) {
		actor = fdActorGet(i);
		if(actor != NULL) {
			actor->motionFlg = 0;
		}
	}
	exit = (s16)(state->stepTrigger - EXIT_CELL_BEHAVIOR_OFFSET);
	if(exit == IMIL_TO_ICE_ROOMS_NORTHEAST) {
		sySndPlay(SE_THUMP);
	}
	else {
		sySndPlay(SE_DOOR_OPEN);
	}
	{
		s32 x = Imil_DoorCellPoints[exit - 1][0];
		s32 y = Imil_DoorCellPoints[exit - 1][1];

		fdMapGridPlayMapEdit(Imil_DoorCellSteps[exit - 1], x, y);
	}
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if(exit == IMIL_TO_ICE_ROOMS_NORTHEAST) {
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
		fdActorSetSprPriority(ACTOR_PARTY_LEADER, 3);
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
	}
	else {
		fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
		fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
	}
	fdEventWait(16);
	syImportEventSetExitRequest(exit);
	fdEventEnd();
}

/* Isaac talks to the weapon seller across the left counter of the weapon */
/* and armor shop once Saturos is beaten: the shop opens. */
/* */
/* Spoken to from elsewhere he says he has lost business to catch up on. */
/* Before then, searching the empty counter shows him sick in bed, saying the */
/* store is closed until they get well. */
static void scImilTalkToWeaponMerchant(void) {
	s32 facing;

	facing = (s16)fdActorGet(ACTOR_PARTY_LEADER)->facing;
	if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
		if(IMIL_FACES_COUNTER(facing)) {
			mnShopRun(SHOP_IMIL_WEAPONS, ACTOR_WEAPON_MERCHANT);
			return;
		}
		fdEventStart();
		fdActorFaceActor(ACTOR_WEAPON_MERCHANT, ACTOR_PARTY_LEADER, 0);
		fdEventWait(10);
		fdEventSetMessageId(MsgMercuryCatchUpLostOpportunity);
		/* "I have to catch up on lost opportunity! It's time for business!" */
		fdEventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
		fdActorFaceDirection(ACTOR_WEAPON_MERCHANT, FACING_SOUTH, 10);
		fdEventEnd();
	}
	else {
		if(IMIL_FACES_COUNTER(facing)) {
			fdEventStart();
			fdCameraSetSpeed(6);
			fdCameraMoveTo(426, CAMERA_KEEP, 492, 1);
			fdCameraWaitForMove();
			fdEventWait(20);
			fdActorRunScriptAndWait(ACTOR_WEAPON_MERCHANT, Imil_BobScript);
			fdEventSetMessageId(MsgMercuryStoreClosedUntilWell);
			/* "Our store is closed until we get well..." */
			fdEventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
			fdCameraMoveTo(426, CAMERA_KEEP, 616, 1);
			fdCameraWaitForMove();
			fdEventEnd();
		}
	}
}

/* Isaac talks to the armor seller across the right counter of the weapon */
/* and armor shop once Saturos is beaten: the shop opens. */
/* */
/* Spoken to from elsewhere he says it was a shame to close for a cold. */
/* Before then, searching the empty counter shows him sick in bed, too sick */
/* to do business. */
static void scImilTalkToArmorMerchant(void) {
	s32 facing;

	facing = (s16)fdActorGet(ACTOR_PARTY_LEADER)->facing;
	if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
		if(IMIL_FACES_COUNTER(facing)) {
			mnShopRun(SHOP_IMIL_ARMOR, ACTOR_ARMOR_MERCHANT);
			return;
		}
		fdEventStart();
		fdActorFaceActor(ACTOR_ARMOR_MERCHANT, ACTOR_PARTY_LEADER, 0);
		fdEventWait(10);
		fdEventSetMessageId(MsgMercuryShameStoreClosed);
		/* "It's a shame the store was closed for something as minor as..." */
		fdEventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
		fdActorFaceDirection(ACTOR_ARMOR_MERCHANT, FACING_SOUTH, 10);
		fdEventEnd();
	}
	else {
		if(IMIL_FACES_COUNTER(facing)) {
			fdEventStart();
			fdCameraSetSpeed(6);
			fdCameraMoveTo(426, CAMERA_KEEP, 492, 1);
			fdCameraWaitForMove();
			fdEventWait(20);
			fdActorRunScriptAndWait(ACTOR_ARMOR_MERCHANT, Imil_BobScript);
			fdEventSetMessageId(MsgMercuryTooSickBusiness);
			/* "Sorry, but I'm too sick to do business today. Try again..." */
			fdEventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
			fdCameraMoveTo(426, CAMERA_KEEP, 616, 1);
			fdCameraWaitForMove();
			fdEventEnd();
		}
	}
}

/* Isaac talks to the item seller across her counter once Saturos is */
/* beaten, and the shop opens; spoken to from elsewhere she says the fountain */
/* has made her well. */
/* */
/* Before then her daughter minds the counter: across it she says her mother */
/* is sick and opens the shop herself, and from elsewhere she says her */
/* father is a lumberjack working in Kolima. */
static void scImilTalkToItemMerchant(void) {
	s32 facing;

	facing = (s16)fdActorGet(ACTOR_PARTY_LEADER)->facing;
	if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
		if(IMIL_FACES_COUNTER(facing)) {
			mnShopRun(SHOP_IMIL_ITEMS, ACTOR_ITEM_MERCHANT);
			return;
		}
		fdEventStart();
		fdActorFaceActor(ACTOR_ITEM_MERCHANT, ACTOR_PARTY_LEADER, 0);
		fdEventSetMessageId(MsgMercuryRecoveredThanksFountain);
		/* "I'm fully recovered, thanks to the fountain! I can get back..." */
		fdEventShowMessage(ACTOR_ITEM_MERCHANT, 0);
		fdActorFaceDirection(ACTOR_ITEM_MERCHANT, FACING_SOUTH, 0);
		fdEventEnd();
	}
	else {
		if(IMIL_FACES_COUNTER(facing)) {
			fdEventStart();
			fdEventSetMessageId(MsgMercuryMomsSickGetWhatever);
			/* "Mom's sick, but I can get you whatever you need..." */
			fdEventShowMessage(ACTOR_ITEM_MERCHANTS_DAUGHTER, 0);
			mnShopRun(SHOP_IMIL_ITEMS, ACTOR_ITEM_MERCHANTS_DAUGHTER);
			fdEventEnd();
		}
		else {
			fdActorFaceActor(ACTOR_ITEM_MERCHANTS_DAUGHTER, ACTOR_PARTY_LEADER,
							 10);
			fdEventSetMessageId(MsgMercuryDadLumberjack);
			/* "My dad is a lumberjack. He's down south working in Kolima." */
			fdEventShowMessage(ACTOR_ITEM_MERCHANTS_DAUGHTER, 0);
			fdActorFaceDirection(ACTOR_ITEM_MERCHANTS_DAUGHTER,
								 FACING_SOUTH_SOUTHWEST, 10);
		}
	}
}

/* Isaac talks to the innkeeper across the counter of Imil's inn, and the inn */
/* opens. From elsewhere the innkeeper wonders why two groups of travelers came
 */
/* through the snows, or, once Saturos is beaten, asks whether it is time for */
/* the party to leave. */
static void scImilTalkToInnkeeper(void) {
	if(IMIL_FACES_COUNTER_HALFWORD(fdActorGet(ACTOR_PARTY_LEADER)->facing)) {
		mnInnCheckIn(IMIL_INN, ACTOR_INNKEEPER);
	}
	else {
		fdEventStart();
		fdActorFaceActor(ACTOR_INNKEEPER, ACTOR_PARTY_LEADER, 10);
		if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
			/* "It's almost time for you to leave, isn't it?" */
			/* Yes: "Just like the legend said, the fountain began flowing
			 * when..." */
			/* No: "Oh! I'll cook a feast for you tonight!" */
			fdEventSetMessageId(MSGNO(MsgImilItsAlmostTimeForLeave));
			fdEventAskYesNo(ACTOR_INNKEEPER, 0);
		}
		else {
			/* "Why have two groups of travelers come through such heavy
			 * snows..." */
			fdEventSetMessageId(MSGNO(MsgImilWhyHaveTwoGroupsTravelers));
			fdEventShowMessage(ACTOR_INNKEEPER, 0);
		}
		fdActorFaceDirection(ACTOR_INNKEEPER, FACING_SOUTH_SOUTHEAST, 10);
		fdEventEnd();
	}
}

/* Isaac talks to Justin across the counter of the Imil Sanctum: he offers */
/* the Sanctum's healing, the first time saying that he is only a student but */
/* can act as their healer. */
/* */
/* From elsewhere he asks whether they are looking for Mia, says he is the */
/* healer while she is out, or, once Saturos is beaten, that she is going on */
/* a journey with them. */
static void scImilTalkToJustin(void) {
	if(IMIL_FACES_COUNTER_HALFWORD(fdActorGet(ACTOR_PARTY_LEADER)->facing)) {
		fdEventStart();
		if(fdEventGetFlag(FLAG_IMIL_JUSTIN_OFFERED_HEALING) == 0) {
			/* "I may only be a student, but I can act as your healer." */
			fdEventSetMessageId(MSGNO(MsgImilMayOnlyStudentButCan));
			fdEventShowMessage(ACTOR_JUSTIN, 0);
			syImportGameFlagSet(FLAG_IMIL_JUSTIN_OFFERED_HEALING);
		}
		fdEventEnd();
		mnSanctumRun(ACTOR_JUSTIN);
	}
	else {
		fdEventStart();
		if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
			/* "Mia's going on a journey with you, right?..." */
			fdEventSetMessageId(MSGNO(MsgImilMiaGoingOnJourneyWith));
			fdEventShowMessage(ACTOR_JUSTIN, 0);
		}
		else if(gmFlagGet(FLAG_IN_PARTY(MIA)) != 0) {
			/* "I am the healer while Mia is out, so tell me if you need..." */
			fdEventSetMessageId(MSGNO(MsgImilAmHealerWhileMiaOut));
			fdEventShowMessage(ACTOR_JUSTIN, 0);
		}
		else {
			/* "Are you looking for Mia?" */
			/* Yes: "Mia just left. An epidemic is sweeping the village..." */
			/* No: "Mia is a very kind healer. I am just her apprentice..." */
			fdEventSetMessageId(MSGNO(MsgImilLookingForMia));
			(void)fdEventAskYesNo(ACTOR_JUSTIN, 0);
			fdActorFaceDirection(ACTOR_JUSTIN, FACING_SOUTH_SOUTHEAST, 10);
		}
		fdEventEnd();
	}
}

/* Sets up the scene as the party enters. */
s32 scInit(void) {
	ACTOR *leader;
	s32 entrance;

	if(MainRoutineStateWork.next == SCENE_IMIL) {
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		fdActorSetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
							STRONGMAN_ANIMATION_SQUATS);
		if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION)) {
			gmFlagClear(FLAG_IMIL_NO_FOOTPRINTS);
			gmFlagClear(FLAG_IMIL_FOOTPRINTS);
		}
		leader->tmr = 0;
		leader->userValue = 0;
		syTaskAddFrame(scImilSpawnChimneySmoke, TASK_PASS_FRAME);
		syTaskAddFrame(scImilUpdateFootprints, TASK_PASS_FRAME);
		fdActorSetSprPriority(ACTOR_SNOWMAN, 1);
		if(gmFlagGet(FLAG_IMIL_SNOWMAN_MOVED)) {
			scImilPlaceMovedSnowman();
		}
		if(!gmFlagGet(FLAG_KEEP_PARTY_POSITION) &&
		   MainRoutineStateWork.entrance == IMIL_FROM_IMIL_FALLS_CAVE) {
			scImilArriveFromFallsCave();
		}
	}
	else if(MainRoutineStateWork.next == SCENE_ICE_ROOMS) {
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		entrance = MainRoutineStateWork.entrance;
		if(entrance == ICE_ROOMS_HOUSE_SOUTHWEST) {
			fdActorSetSprColById(ACTOR_INVISIBLE_CRATE, SPRITE_COLOR_HIDDEN);
			fdActorGet(ACTOR_INVISIBLE_CRATE)->collisionFlg |=
				ACTOR_NOT_TARGETED;
			fdActorSetSprPriority(ACTOR_INVISIBLE_CRATE, 1);
			if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
				fdMapGridCopyCellAttributes(10, 7, 1, 1, 10, 8);
				fdMapGridCopyCellsTo(3, 125, 9, 69, 3, 3);
				fdMapGridCenterOnFollowedPoint();
				sySynch(1);
				fdActorSetMovementTypeById(ACTOR_OLD_WOMAN,
										   MOVEMENT_TYPE_WANDER);
				fdActorSetPos(ACTOR_OLD_MANS_BED, 0, 0);
			}
			else if(gmFlagGet(FLAG_IMIL_MIA_SENT_FOR) &&
					gmFlagGet(FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE)) {
				fdActorSetSprFlags(fdActorGet(ACTOR_OLD_MANS_BED), 0);
				fdActorSetPos(ACTOR_OLD_MAN, 174, 164);
				fdActorSetSprFlags(fdActorGet(ACTOR_OLD_MAN), 0);
				fdActorSetAnimation(ACTOR_OLD_MAN,
									OLD_MAN_WITH_CANE_ANIMATION_LIE_IN_BED);
				fdActorSetPos(ACTOR_OLD_WOMAN, 168, 152);
				fdActorGet(ACTOR_OLD_WOMAN)->facing = FACING_SOUTH_SOUTHEAST;
				if(!gmFlagGet(FLAG_IMIL_MET_MIA)) {
					scImilMeetMiaAtOldCouplesHouse();
				}
			}
			else {
				fdMapGridCopyCellAttributes(10, 7, 1, 1, 10, 8);
				fdMapGridCopyCellsTo(3, 125, 9, 69, 3, 3);
				fdMapGridCenterOnFollowedPoint();
				sySynch(1);
				if(gmFlagGet(FLAG_IMIL_MIA_SENT_FOR)) {
					fdActorSetPos(ACTOR_OLD_WOMAN, 149, 116);
					fdActorGet(ACTOR_OLD_WOMAN)->facing = FACING_EAST;
					fdActorGet(ACTOR_OLD_MAN)->userValue = 0;
					fdActorSetScript(ACTOR_OLD_MAN, Imil_TrembleScript);
				}
				else {
					fdActorSetMovementTypeById(ACTOR_OLD_WOMAN,
											   MOVEMENT_TYPE_WANDER);
				}
			}
		}
		else if(entrance == ICE_ROOMS_FROM_IMIL_WEST_SOUTHWEST) {
			if(!gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
				fdActorGet(ACTOR_SICK_OLD_MAN)->userValue = 1;
				fdActorSetScript(ACTOR_SICK_OLD_MAN, Imil_TrembleScript);
			}
		}
		else if(entrance == ICE_ROOMS_HOUSE_NORTHEAST) {
			if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
				fdActorSetPos(ACTOR_WEAPON_MERCHANT, 364, 578);
				fdActorSetSprPriority(ACTOR_WEAPON_MERCHANT, 2);
				fdActorGet(ACTOR_WEAPON_MERCHANT)->collisionFlg |=
					ACTOR_NEAR_TALK;
				fdMapGridCopyCellsTo(6, 125, 22, 88, 3, 3);
				fdActorSetPos(ACTOR_ARMOR_MERCHANT, 492, 578);
				fdActorSetSprPriority(ACTOR_ARMOR_MERCHANT, 2);
				fdActorGet(ACTOR_ARMOR_MERCHANT)->collisionFlg |=
					ACTOR_NEAR_TALK;
				fdMapGridCopyCellsTo(9, 125, 28, 88, 3, 3);
			}
			else {
				fdActorGet(ACTOR_WEAPON_MERCHANT)->scaleX = -FIXED_ONE;
				fdActorSetSprFlags(fdActorGet(ACTOR_WEAPON_MERCHANT), 0);
				fdActorSetAnimation(ACTOR_WEAPON_MERCHANT,
									OLD_MAN_WITH_CANE_ANIMATION_LIE_IN_BED);
				fdActorSetSprFlags(fdActorGet(ACTOR_ARMOR_MERCHANT), 0);
				fdActorSetAnimation(ACTOR_ARMOR_MERCHANT,
									OLD_MAN_WITH_CANE_ANIMATION_LIE_IN_BED);
			}
		}
		else if(entrance == ICE_ROOMS_HOUSE_WEST) {
			if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
				fdActorSetPos(ACTOR_ITEM_MERCHANT, 460, 258);
				fdActorSetSprPriority(ACTOR_ITEM_MERCHANT, 2);
				fdActorGet(ACTOR_ITEM_MERCHANT)->collisionFlg |=
					ACTOR_NEAR_TALK;
				fdActorSetPos(ACTOR_ITEM_MERCHANTS_DAUGHTER, 408, 264);
				fdActorGet(ACTOR_ITEM_MERCHANTS_DAUGHTER)->facing =
					FACING_EAST_SOUTHEAST;
				fdMapGridCopyCellsTo(12, 125, 26, 70, 3, 3);
			}
			else {
				fdActorSetPos(ACTOR_ITEM_MERCHANTS_DAUGHTER, 460, 258);
				fdActorSetSprPriority(ACTOR_ITEM_MERCHANTS_DAUGHTER, 2);
				fdActorGet(ACTOR_ITEM_MERCHANTS_DAUGHTER)->collisionFlg |=
					ACTOR_NEAR_TALK;
				fdActorGet(ACTOR_ITEM_MERCHANT)->scaleX = -FIXED_ONE;
				fdActorSetSprFlags(fdActorGet(ACTOR_ITEM_MERCHANT), 0);
				fdActorSetAnimation(ACTOR_ITEM_MERCHANT,
									IMIL_ANIMATION_LIE_IN_BED);
			}
		}
		else if(entrance == ICE_ROOMS_FROM_IMIL_EAST_NORTHEAST) {
			if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
				fdActorGet(ACTOR_MEGAN)->facing = FACING_SOUTH_SOUTHEAST;
				if(!gmFlagGet(FLAG_IMIL_MIA_SAID_FAREWELL)) {
					fdActorSetPos(ACTOR_MEGAN, 650, 161);
					scImilMiaSaysFarewellToApprentices();
				}
				else {
					fdActorSetPos(ACTOR_MEGAN, 644, 166);
				}
			}
		}
	}
	return 0;
}

static void scImilMeetMiaAtOldCouplesHouse(void) {
	const s32 *table;
	FIELD_STATE *state;

	fdEventStart();
	fdActorSetPos(ACTOR_MIA, 182, 150);
	fdCameraMoveTo(141, CAMERA_KEEP, 221, 0);
	sySynch(1);
	fdCameraSetSpeed(0.3);
	fdCameraMoveTo(140, CAMERA_KEEP, 164, 1);
	state = FieldStateWork;
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->transitionFrm = 40;
	fdEventStartScrTransition();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.4);
	fdActorSetSpeed(ACTOR_GARET, 0.4);
	fdActorSetSpeed(ACTOR_IVAN, 0.4);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 142, 221);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
	scPlaceAtLeader(ACTOR_GARET);
	scPlaceAtLeader(ACTOR_IVAN);
	fdActorWalkTo(ACTOR_GARET, 150, 234);
	fdActorWalkToAndWait(ACTOR_IVAN, 134, 234);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	syFieldActorWatchAndStartScript(ACTOR_PARTY_LEADER,
									ACTOR_WATCH_KEEP_MOTION | ACTOR_MIA,
									Imil_FaceTargetScript);
	syFieldActorWatchAndStartScript(ACTOR_GARET,
									ACTOR_WATCH_KEEP_MOTION | ACTOR_MIA,
									Imil_FaceTargetScript);
	syFieldActorWatchAndStartScript(
		ACTOR_IVAN, ACTOR_WATCH_KEEP_MOTION | ACTOR_MIA, Imil_FaceTargetScript);
	fdCameraWaitForMove();
	table = Imil_BobScript;
	fdActorSetScript(ACTOR_OLD_MAN, table);
	fdEventWait(40);
	fdActorSetAttachedEffect(ACTOR_MIA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	fdEventSetMessageId(MSGNO(MsgImilHowFeeling));
	/* "How are you feeling?" */
	fdEventShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorSetScript(ACTOR_OLD_MAN, table);
	/* "Cough... Cough..." */
	fdEventShowMessageThenWait(ACTOR_OLD_MAN, 0, 20);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 20);
	fdActorFaceDirection(ACTOR_OLD_WOMAN, FACING_EAST, 10);
	fdActorSetAnimationAndWait(ACTOR_OLD_WOMAN, ANIM_SHAKE_HEAD);
	/* "He's been coughing for quite a while, now..." */
	fdEventShowMessageThenWait(ACTOR_OLD_WOMAN, 0, 40);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_OLD_WOMAN, FACING_SOUTH_SOUTHEAST, 20);
	/* "You'll feel better soon." */
	fdEventShowMessageThenWait(ACTOR_MIA, 0, 10);
	fdActorSetScript(ACTOR_OLD_MAN, table);

	scImilMiaCastsPlyOnOldMan();
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "How do you feel now?" */
	fdEventShowMessageThenWait(ACTOR_MIA, 0, 40);
	/* "Much better..." */
	fdEventShowMessageThenWait(ACTOR_OLD_MAN, 0, 20);
	fdActorShowEmote(ACTOR_OLD_WOMAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorSetAnimation(ACTOR_OLD_MAN, OLD_MAN_WITH_CANE_ANIMATION_SIT_UP);
	fdMapGridAnimateCells(Imil_OldManSitUpSteps, 10, 69);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	/* "No, Grandpa. You mustn't push yourself too hard!" */
	fdEventShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_OLD_MAN, 1);
	fdEventWait(40);
	fdActorSetAnimation(ACTOR_OLD_MAN, OLD_MAN_WITH_CANE_ANIMATION_LIE_BACK);
	fdMapGridAnimateCells(Imil_OldManLieBackSteps, 10, 69);
	fdEventWait(40);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_OLD_WOMAN, FACING_EAST, 20);
	fdActorSetAnimationAndWait(ACTOR_OLD_WOMAN, ANIM_NOD);
	/* "Thank you, Mia... I can't imagine how this village would ever manage */
	/* without you... Cough..." */
	fdEventShowMessageThenWait(ACTOR_OLD_WOMAN, 0, 10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 10);
	fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
	/* "Don't mention it... You should rest in bed, too, ma'am." */
	fdEventShowMessageThenWait(ACTOR_MIA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_OLD_WOMAN, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(40);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorGet(ACTOR_MIA)->tmr = 0;
	fdActorSetScript(ACTOR_MIA, Imil_MiaCheckRoomScript);
	while(fdActorGet(ACTOR_MIA)->tmr == 0) {
		sySynch(1);
	}
	fdCameraMoveTo(140, CAMERA_KEEP, 198, 1);
	fdActorWaitForScriptEndById(ACTOR_MIA);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	/* "Who are you?" */
	fdEventShowMessageThenWait(ACTOR_MIA, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	fdEventWait(10);
	/* "Can I help you?" */
	fdEventShowMessage(ACTOR_MIA, 0);
	sySndPlay(SE_PSYNERGY_SPARKLE);
	/* The lighthouse interrupts the visit; Mia leaves and the party follows. */
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(31, 20, 31),
							 PALETTES_ALL);
	fdPaletteFadeStartInterpolation(10);
	sySynch(1);
	sySndPlay(SE_SHIMMER);
	sySynch(40);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(60);
	sySynch(60);
	fdActorSetAttachedEffect(ACTOR_MIA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 10);
	fdActorSetSpeed(ACTOR_MIA, 2);
	fdActorWalkToAndWait(ACTOR_MIA, 202, 198);
	fdEventWait(40);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	/* "Was that... the lighthouse?" */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	/* "It can't be..." */
	fdEventShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorSetAttachedEffect(ACTOR_MIA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	/* "Only I can..." */
	fdEventShowMessageThenWait(ACTOR_MIA, 0, 40);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Oh, no... Alex!!!" */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdActorStop(ACTOR_PARTY_LEADER);
	fdActorStop(ACTOR_GARET);
	fdActorStop(ACTOR_IVAN);
	fdActorSetSpeed(ACTOR_MIA, 3);
	fdActorGet(ACTOR_MIA)->tmr = 0;
	fdActorSetScript(ACTOR_MIA, Imil_MiaRunOutScript);
	while(fdActorGet(ACTOR_MIA)->tmr == 0) {
		sySynch(1);
	}
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 10);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 4);
	fdActorSetSpeed(ACTOR_GARET, 4);
	fdActorSetSpeed(ACTOR_IVAN, 4);
	sySndPlay(SE_JUMP);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_GARET)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_IVAN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetDestination(ACTOR_PARTY_LEADER, 132, 206);
	fdActorSetDestination(ACTOR_GARET, 136, 221);
	fdActorSetDestination(ACTOR_IVAN, 122, 238);
	fdActorWaitForScriptEndById(ACTOR_MIA);
	fdEventWait(80);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_GARET)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_IVAN)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	table = Imil_FollowMiaOutScript;
	fdActorSetScript(ACTOR_GARET, table);
	fdActorRunScriptAndWait(ACTOR_IVAN, table);
	fdEventWait(20);
	state = FieldStateWork;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	state->transitionFrm = 24;
	syImportGameFlagSet(FLAG_IMIL_MET_MIA);
	fdEventEnd();
}

/* Unused: an update function that spirals an actor in to a point 8 pixels */
/* above Imil_EffectCenter, a pixel nearer and a 32nd of a turn round each */
/* frame, and releases it there. No actor is given it. */
UNUSED static void scImilUpdateSpiralIn(ACTOR *actor) {
	/* XXX (no evidence): what this and scImilSwayAndSpark were for; no */
	/* actor is given either, so nothing in the game shows them in use. */
	s32 buf[3];
	s32 n;
	s32 t;

	if(actor != NULL) {
		n = (u16)actor->tmr - 1;
		actor->tmr = n;
		t = (s16)n;
		if(t != 0) {
			buf[0] = Imil_EffectCenter[0];
			buf[1] = Imil_EffectCenter[1] + toFIXED(8);
			buf[2] = Imil_EffectCenter[2];
			syVectorAddPolarOffset(
				toFIXED(t), t * (TRIG_TURN / 32) + actor->userValue, buf);
			actor->x.fixed = buf[0];
			actor->y.fixed = buf[1];
			actor->z.fixed = buf[2];
		}
		else {
			fdActorRelease(actor);
		}
	}
}

/* Unused: an update function that sways an actor 6 pixels either side of */
/* Imil_EffectCenter every 64 frames and, every third frame, sheds a sparkle */
/* beside it (Imil_SparkScript). No actor is given it. */
UNUSED static void scImilSwayAndSpark(ACTOR *actor) {
	s32 pos[3];
	ACTOR *spark;
	s32 radius;

	actor->x.fixed = Imil_EffectCenter[0] +
					 syMulFX(toFIXED(6), sySin(actor->tmr * (TRIG_TURN / 64)));
	actor->tmr++;
	actor->tmr = (actor->tmr + 64) % 64;
	if(FrameCount % 3 == 0) {
		SET_VECTOR(pos, actor->x.fixed, actor->y.fixed + toFIXED(2),
				   actor->z.fixed);
		radius = syRandomNext();
		syVectorAddPolarOffset(radius * 6, syRandomNext(), pos);
		spark = syFieldActorCreate(SPRITE_SPARKLE, pos[0], pos[1], pos[2]);
		if(spark != NULL) {
			spark->spr->image.pri = 0;
			fdActorSetSprFlags(spark, 0);
			fdActorSelectAnimation(spark, 1);
			spark->scaleX = 3 * FIXED_ONE / 5;
			spark->scaleY = 3 * FIXED_ONE / 5;
			spark->priFlg = ACTOR_PRIORITY_UNDERFOOT;
			spark->motionFlg = 0;
			fdActorSetSprCol(spark, SPRITE_COLOR_BLUE_TINT);
			fdActorStartScript(spark, Imil_SparkScript);
		}
	}
}

/* The old man bobs and speaks as Ply ends (its finishFunc). */
static void scImilOldManBobsAndSpeaks(void) {
	fdActorSetScript(ACTOR_OLD_MAN, Imil_BobScript);
	/* "Cough cough... Cough... Cough... Cough..." */
	fdEventShowMessage(ACTOR_OLD_MAN, 0);
}

static void scImilMiaCastsPlyOnOldMan(void) {
	FIELD_EFFECT_STATE *effectState;

	fdPsynergyLoadResources(ABILITY_PLY, 1);
	effectState = FieldEffectStateWork;
	fdPsynergySetupActorPair(ACTOR_MIA, ACTOR_OLD_MAN);
	effectState->finishFunc = scImilOldManBobsAndSpeaks;
	fdPsynergyStartCast();
	fdPsynergyRunHandler(FIELD_PSYNERGY_HANDLER_ON_TARGET);
	fdPsynergyStopCasterUpdate();
	fdPsynergyEnd();
}

/* Readies a chimney's smoke puff: no motion of its own, at sprite priority */
/* 1, tinted blue and half its size. */
static void scImilInitSmoke(ACTOR *smoke) {
	SPR *sprite;

	smoke->motionFlg = 0;
	smoke->tmr = 0;
	smoke->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;

	sprite = smoke->spr;
	sprite->image.pri = 1;

	fdActorSetSprCol(smoke, SPRITE_COLOR_BLUE_TINT);
	fdActorSetSprFlags(smoke, 0);

	smoke->scaleX = FIXED_ONE / 2;
	smoke->scaleY = FIXED_ONE / 2;
}

/* A smoke puff's update: it drifts east ever faster, rises half a pixel a */
/* frame and grows, and is released when its second is up. */
static void scImilDriftSmoke(ACTOR *actor) {
	FIELD_EFFECT *effect = (FIELD_EFFECT *)actor;

	effect->x += (s16)effect->spin << 8;
	effect->y += toFIXED(1) / 2;
	effect->scaleX += 3 * FIXED_ONE / 100;
	effect->scaleY += 3 * FIXED_ONE / 100;
	effect->spin += 2;
	if(--effect->countdown == 0) {
		fdActorRelease(actor);
	}
}

/* Starts a newly created smoke puff on its one-second rise. */
static inline void scImilStartSmoke(ACTOR *actor) {
	if(actor != NULL) {
		scImilInitSmoke(actor);
		((FIELD_EFFECT *)actor)->countdown = 60;
		((FIELD_EFFECT *)actor)->update = scImilDriftSmoke;
		fdActorSelectAnimation(actor, 5);
	}
}

static void scImilSpawnChimneySmoke(void) {
	ACTOR *actor;

	if(FrameCount % 60 == 0) {
		actor = (ACTOR *)syFieldActorCreate(SPRITE_DUST_PUFF, toFIXED(463), 0,
											toFIXED(292));
		scImilStartSmoke(actor);
	}
	if((FrameCount + 30) % 60 == 0) {
		actor = (ACTOR *)syFieldActorCreate(SPRITE_DUST_PUFF, toFIXED(320),
											toFIXED(32), toFIXED(356));
		scImilStartSmoke(actor);
	}
	if((FrameCount + 10) % 60 == 0) {
		actor = (ACTOR *)syFieldActorCreate(SPRITE_DUST_PUFF, toFIXED(118), 0,
											toFIXED(70));
		scImilStartSmoke(actor);
	}
	if((FrameCount + 50) % 60 == 0) {
		actor = (ACTOR *)syFieldActorCreate(SPRITE_DUST_PUFF, toFIXED(342), 0,
											toFIXED(124));
		scImilStartSmoke(actor);
	}
	if((FrameCount + 80) % 60 == 0) {
		actor = (ACTOR *)syFieldActorCreate(SPRITE_DUST_PUFF, toFIXED(431), 0,
											toFIXED(171));
		scImilStartSmoke(actor);
	}
}

static void scImilUpdateFootprints(void) {
	ACTOR *actor;
	FIXED x;
	s32 column;
	s32 isSet;
	s32 zoneFlag = FLAG_IMIL_NO_FOOTPRINTS;
	s32 otherZoneFlag = FLAG_IMIL_FOOTPRINTS;
	MAP_GRID_SCROLL_STATE *grid;

	actor = fdActorGet(ACTOR_PARTY_LEADER);
	x = actor->x.fixed;
	column = FIELD_HALF_CELL(x);
	/* On the frozen river. */
	if((column >= 24 && column <= 31) ||
	   ((FIELD_HALF_CELL(actor->z.fixed) >= 36 &&
		 FIELD_HALF_CELL(actor->z.fixed) <= 45) &&
		(column >= 22 && column <= 31))) {
		isSet = gmFlagGet(zoneFlag);
		if(isSet != 0)
			return;
		((MAP_GRID_SCROLL_STATE *)MapGridState[0])->footprints = isSet;
		syImportGameFlagSet(zoneFlag);
		gmFlagClear(otherZoneFlag);
		return;
	}
	/* On the raised ground under the big tree. */
	if(x > toFIXED(232) && actor->y.fixed > toFIXED(30) &&
	   actor->z.fixed > toFIXED(212)) {
		grid = MapGridState[0];
		grid->footprints = 0;
		syImportGameFlagSet(zoneFlag);
		gmFlagClear(otherZoneFlag);
		return;
	}
	isSet = gmFlagGet(otherZoneFlag);
	if(isSet != 0)
		return;
	grid = MapGridState[0];
	grid->footprints = 1;
	syImportGameFlagSet(otherZoneFlag);
	gmFlagClear(zoneFlag);
	return;
}

static void scImilPlaceMovedSnowman(void) {
	s32 column;
	s32 row;

#if TBS_PATCH_3
	ACTOR *actor = fdActorGet(ACTOR_SNOWMAN);

	column = FIELD_CELL(actor->x.fixed);
	if(column != SNOWMAN_MOVED_COLUMN)
		return;
	row = FIELD_CELL(actor->z.fixed);
	if(row != SNOWMAN_MOVED_ROW)
		return;
	syImportGameFlagSet(FLAG_IMIL_SNOWMAN_MOVED);
	fdActorSetSprPriority(ACTOR_SNOWMAN, 3);
#else
	syImportGameFlagSet(FLAG_IMIL_SNOWMAN_MOVED);
	fdActorSetSprPriority(ACTOR_SNOWMAN, 3);
	column = SNOWMAN_MOVED_COLUMN;
	row = SNOWMAN_MOVED_ROW;
#endif
	fdMapGridCopyCellAttributes(SNOWMAN_MOVED_COLUMN, SNOWMAN_MOVED_ROW - 1, 1,
								1, column, row);
}

/* Walking onto the path north out of Imil: the leader faces north and goes */
/* into Imil Falls Cave (IMIL_TO_IMIL_FALLS_CAVE). */
static void scImilLeaveNorth(void) {
	fdEventStart();
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_NORTH;
	sySndPlay(SE_MAP_EXIT);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(IMIL_TO_IMIL_FALLS_CAVE);
}

static void scImilArriveFromFallsCave(void) {
	fdEventStart();
	fdEventStartScrTransition();
	fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 2, 0.1);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 232, 204);
	fdEventEnd();
}

static void scImilMiaSaysFarewellToApprentices(void) {
	FIELD_STATE *state;

	fdEventStart();
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 690, 200);

	fdCameraGetActor()->motionFlg = 0;
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(690, 0, 164, 1);
	state = FieldStateWork;
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->transitionFrm = 48;
	fdEventStartScrTransition();
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorSetSpeed(ACTOR_MIA, 0.6);
	scPlaceAtLeader(ACTOR_MIA);
	fdActorWalkToAndWait(ACTOR_MIA, 673, 183);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorStartRepeatedMotion(ACTOR_JUSTIN, 2);
	fdActorRunRepeatedMotion(ACTOR_MEGAN, 2);
	fdEventWait(40);
	fdEventSetMessageId(MSGNO(MsgImilMia));
	/* "Mia..." */
	fdEventShowMessageThenWait(ACTOR_JUSTIN, 0, 10);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdActorSetAttachedEffect(ACTOR_MEGAN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(20);
	/* "We were worried that you wouldn't come back!" */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MEGAN, 0, 10);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 40);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	/* "I'm so sorry, you two..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MEGAN, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAttachedEffect(ACTOR_JUSTIN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(20);
	/* "Mia, we have a problem! The Mercury Lighthouse... It's been lit!" */
	fdEventShowMessageThenWait(ACTOR_JUSTIN, 0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_EAST_NORTHEAST, 10);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 40);
	fdActorRunRepeatedMotion(ACTOR_MEGAN, 2);
	fdEventWait(20);
	/* "Mia, you already knew?" */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MEGAN, 0, 20);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(60);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorShowEmote(ACTOR_JUSTIN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MEGAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorRunRepeatedMotion(ACTOR_JUSTIN, 1);
	fdEventWait(20);
	/* "What's the matter? You look strange today, Mia." */
	fdEventShowMessageThenWait(ACTOR_JUSTIN, 0, 10);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 40);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 80);
	/* "I... How should I tell you?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 20);
	fdActorFaceDirection(ACTOR_MEGAN, FACING_EAST_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JUSTIN, FACING_WEST_SOUTHWEST, 40);
	fdActorFaceDirection(ACTOR_JUSTIN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MEGAN, FACING_SOUTH_SOUTHEAST, 20);
	fdActorSetSpeed(ACTOR_MEGAN, 1);
	fdActorGet(ACTOR_MEGAN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_MEGAN, 656, 166);
	fdEventWait(1);
	fdActorGet(ACTOR_MEGAN)->steeringFlg |= ACTOR_FACES_MOTION;
	fdEventWait(20);
	/* "You have something to say? What is it?" */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MEGAN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
	/* "Well, I... Uh..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 40);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
	/* "I... I'll be joining Isaac on his quest." */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0, 10);
	fdActorSetAttachedEffect(ACTOR_JUSTIN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_MEGAN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 20);
	fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
	/* "It's very hard for me to leave you, but it's the lighthouse." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_JUSTIN, 1);
	/* "You said the world would be in danger if the lighthouse was lit, right?"
	 */
	fdEventShowMessageThenWait(ACTOR_JUSTIN, 0, 10);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 40);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 20);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_MEGAN, 1);
	/* "Is that why you're leaving? To protect the world?" */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MEGAN, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	/* "Yes. That's right..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 80);
	fdActorShowEmote(ACTOR_JUSTIN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_MEGAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorSetAnimationAndWait(ACTOR_JUSTIN, ANIM_SHAKE_HEAD);
	/* "Then there's nothing we can do..." */
	fdEventShowMessageThenWait(ACTOR_JUSTIN, 0, 10);
	fdActorSetAnimation(ACTOR_MEGAN, ANIM_SHAKE_HEAD);
	/* "We won't try to stop you, then. After all, you are trying to protect the
	 */
	/* Mercury Clan." */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MEGAN, 0, 20);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* "Yes. I am..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 40);
	fdActorSetAnimationAndWait(ACTOR_JUSTIN, ANIM_NOD);
	/* "As members of the Mercury Clan...we will protect Imil ourselves. So
	 * don't */
	/* worry, OK?" */
	fdEventShowMessageThenWait(ACTOR_JUSTIN, 0, 10);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_MEGAN, ANIM_NOD);
	/* "I will try hard and learn to heal as well as you, Mia!" */
	fdEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MEGAN, 0, 10);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 60);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 40);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	/* "OK, I'll leave the village in your hands, Megan, Justin..." */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 10);
	fdActorSetAnimation(ACTOR_JUSTIN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MEGAN, ANIM_NOD);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
	/* "We can leave without any worries, now." */
	fdEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(20);
	fdActorWalkToAndWait(ACTOR_MIA, 688, 200);
	fdActorSetPos(ACTOR_MIA, 0, 0);
	fdActorGet(ACTOR_MEGAN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_MEGAN, 644, 166);
	fdEventWait(1);
	fdActorGet(ACTOR_MEGAN)->steeringFlg |= ACTOR_FACES_MOTION;
	/* FAKEMATCH(dead): the transition's store alone in a once-only block; as
	 * the plain store. */
	do {
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	} while(0);
	syImportGameFlagSet(FLAG_IMIL_MIA_SAID_FAREWELL);
	gmFlagClear(FLAG_IMIL_JUSTIN_OFFERED_HEALING);
	fdEventEnd();
}

#define ICE_SLIDE_HEADINGS Imil_SlideHeadings
#include "shared/leader_slide_on_ice.c"

/* clang-format off */
#include "shared/ice_key_headings.c"

/* The way the leader slides on the ice for each combination of held */
/* directions (DPAD_INDEX); a diagonal slides east or west. */
s16 Imil_SlideHeadings[] = {
    KEY_HEADING_NONE, FACING_EAST, FACING_WEST, KEY_HEADING_NONE, FACING_NORTH, FACING_EAST, FACING_WEST, FACING_NORTH,
    FACING_SOUTH, FACING_EAST, FACING_WEST, FACING_SOUTH, KEY_HEADING_NONE, FACING_EAST, FACING_WEST, KEY_HEADING_NONE,
};

#include "shared/tremble_script.c"

/* A sick person trembles (scImilUpdateTremble) for as long as it runs. */
s32 Imil_TrembleScript[] = {
    SC_CALL(scImilUpdateTremble),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 Imil_OldWomanHurryScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(136, 0, 192),
    SC_SYNC,
    SC_MOVE(136, 0, 136),
    SC_SYNC,
    SC_MOVE(149, 0, 116),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 Imil_LeaderStepAsideScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(155, 0, 135),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
    /* Unused: past its end, one bob of Imil_BobScript. */
    SC_ADD(SW_SCALE_Y, 3 * FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * FIXED_ONE / 32),
    SC_WAIT(4),
    SC_END,
};

/* Stretches an actor three eighths taller and lets it settle back, twice: */
/* the old man and the sick sellers bob up as they talk. */
s32 Imil_BobScript[] = {
    SC_ADD(SW_SCALE_Y, 3 * FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * FIXED_ONE / 32),
    SC_WAIT(4),
    SC_LOOP(2, SC_TOP),
    SC_END,
};

/* A sparkle scImilSwayAndSpark sheds sinks two fifths of a pixel a frame for */
/* 72 frames, then is released. */
s32 Imil_SparkScript[] = {
    SC_ADD(SW_Y, -toFIXED(2) / 5),
    SC_WAIT(1),
    SC_LOOP(72, SC_TOP),
    SC_RELEASE,
};

s32 Imil_MiaCheckRoomScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(177, 0, 136),
    SC_SYNC,
    SC_MOVE(161, 0, 124),
    SC_SYNC,
    SC_SET(SW_TIMER, 1),
    SC_MOVE(134, 0, 139),
    SC_SYNC,
    SC_MOVE(135, 0, 189),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 Imil_MiaRunOutScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(165, 0, 207),
    SC_SYNC,
    SC_SET(SW_TIMER, 1),
    SC_MOVE(147, 0, 252),
    SC_SYNC,
    SC_MOVE(150, 0, 268),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 Imil_FaceTargetScript[] = {
    SC_CALL(scTurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 Imil_FollowMiaOutScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(132, 0, 206),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

SCENE_ENTRANCE Imil_TownEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_HOUSE_SOUTHWEST, CONDITION_ALWAYS, 288, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_EAST_UNUSED, CONDITION_ALWAYS, 480, 0, 280, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_HOUSE_WEST, CONDITION_ALWAYS, 312, 0, 424, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_HOUSE_NORTHEAST, CONDITION_ALWAYS, 424, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_ICE_ROOMS_EAST, CONDITION_ALWAYS, 456, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_ICE_ROOMS_NORTHEAST, CONDITION_ALWAYS, 376, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_ICE_ROOMS_WEST, CONDITION_ALWAYS, 120, 0, 208, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_ICE_ROOMS_SOUTHWEST, CONDITION_ALWAYS, 64, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_IMIL_FALLS_CAVE, CONDITION_ALWAYS, 229, 0, 88, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

SCENE_ENTRANCE Imil_RoomEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 136, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { ICE_ROOMS_HOUSE_SOUTHWEST, CONDITION_ALWAYS, 144, 0, 242, FACING_NORTH, 0, 32, 52, 256, 280, 0 },
    { ICE_ROOMS_FROM_IMIL_WEST_SOUTHWEST, CONDITION_ALWAYS, 143, 0, 596, FACING_NORTH, 0, 42, 424, 256, 631, 0 },
    { ICE_ROOMS_HOUSE_WEST, CONDITION_ALWAYS, 399, 0, 322, FACING_NORTH, 0, 320, 58, 536, 360, 0 },
    { ICE_ROOMS_HOUSE_NORTHEAST, CONDITION_ALWAYS, 400, 0, 659, FACING_NORTH, 0, 307, 382, 545, 697, 0 },
    { ICE_ROOMS_INN, CONDITION_ALWAYS, 129, 0, 915, FACING_NORTH, 0, 50, 706, 350, 949, 0 },
    { ICE_ROOMS_INN_UPSTAIRS_FROM_INN, CONDITION_ALWAYS, 446, 0, 825, FACING_SOUTH, 0, 402, 740, 670, 921, 0 },
    { ICE_ROOMS_FROM_IMIL_EAST_NORTHEAST, CONDITION_ALWAYS, 687, 0, 264, FACING_NORTH, 0, 594, 49, 806, 295, 0 },
    { ICE_ROOMS_INN_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 182, 0, 772, FACING_SOUTH, 0, 50, 706, 350, 949, 0 },
    { SCENE_TABLE_END },
};

u32 Imil_Exits[] = {
    SCENE_EXITS(Imil),
    SCENE_EXIT(IMIL_TO_HOUSE_SOUTHWEST, IceRooms, ICE_ROOMS_HOUSE_SOUTHWEST),
    SCENE_EXIT(IMIL_TO_INN, IceRooms, ICE_ROOMS_INN),
    SCENE_EXIT(IMIL_TO_HOUSE_WEST, IceRooms, ICE_ROOMS_HOUSE_WEST),
    SCENE_EXIT(IMIL_TO_HOUSE_NORTHEAST, IceRooms, ICE_ROOMS_HOUSE_NORTHEAST),
    SCENE_EXIT(IMIL_TO_ICE_ROOMS_EAST, IceRooms, ICE_ROOMS_FROM_IMIL_WEST_SOUTHWEST),
    SCENE_EXIT(IMIL_TO_ICE_ROOMS_NORTHEAST, IceRooms, ICE_ROOMS_FROM_IMIL_EAST_NORTHEAST),
    SCENE_EXIT(IMIL_TO_IMIL_FALLS_CAVE, ImilFallsCave, IMIL_FALLS_CAVE_FROM_IMIL),
    SCENE_EXIT(IMIL_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_IMIL_SOUTH),
    SCENE_EXIT(IMIL_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_IMIL_NORTH),
    SCENE_EXITS(IceRooms),
    SCENE_EXIT(ICE_ROOMS_HOUSE_WEST_TO_IMIL, Imil, IMIL_FROM_HOUSE_WEST),
    SCENE_EXIT(ICE_ROOMS_TO_IMIL_WEST_SOUTHWEST, Imil, IMIL_FROM_ICE_ROOMS_WEST),
    SCENE_EXIT(ICE_ROOMS_TO_IMIL_EAST, Imil, IMIL_FROM_ICE_ROOMS_EAST),
    SCENE_EXIT(ICE_ROOMS_TO_IMIL, Imil, IMIL_FROM_ICE_ROOMS_NORTHEAST),
    SCENE_EXIT(ICE_ROOMS_TO_IMIL_SOUTH_SOUTHWEST, Imil, IMIL_FROM_HOUSE_NORTHEAST),
    SCENE_EXIT(ICE_ROOMS_INN_UPSTAIRS_TO_INN, IceRooms, ICE_ROOMS_INN_FROM_INN_UPSTAIRS),
    SCENE_EXIT(ICE_ROOMS_TO_IMIL_EAST_NORTHEAST, Imil, IMIL_FROM_ICE_ROOMS_SOUTHWEST),
    SCENE_EXIT(ICE_ROOMS_INN_TO_INN_UPSTAIRS, IceRooms, ICE_ROOMS_INN_UPSTAIRS_FROM_INN),
    SCENE_EXITS_END,
};

ACTOR_PLACE Imil_TownActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, (s32)Actor_TrembleScript,
        99, 0, 234, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        376, 0, 408, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        408, 0, 248, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SNOWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        262, 0, 124, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE Imil_TownActorPlacementsCured[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        99, 0, 234, FACING_EAST_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        376, 0, 408, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        408, 0, 248, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SNOWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        262, 0, 124, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE Imil_RoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        111, 0, 215, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        166, 0, 116, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        174, 0, 173, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        118, 0, 488, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        370, 0, 468, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        478, 0, 468, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        460, 0, 266, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        436, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        100, 0, 830, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        160, 0, 880, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        249, 0, 832, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        686, 0, 148, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        717, 0, 229, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        192, 0, 124, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

TRIGGER Imil_TownTriggers[] = {
    { TRIGGER_EXIT, 60, CONDITION_ALWAYS, IMIL_TO_WORLD_MAP_SOUTH },
    { TRIGGER_EXIT, 61, CONDITION_ALWAYS, IMIL_TO_WORLD_MAP_NORTH },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SNOWMAN, FLAG_IMIL_NO_FOOTPRINTS, EVENT(scImilPlaceMovedSnowman) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_CELL_BEHAVIOR_OFFSET + IMIL_TO_HOUSE_SOUTHWEST, CONDITION_ALWAYS, EVENT(scImilOpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_CELL_BEHAVIOR_OFFSET + IMIL_TO_INN, CONDITION_ALWAYS, EVENT(scImilOpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_CELL_BEHAVIOR_OFFSET + IMIL_TO_HOUSE_WEST, CONDITION_ALWAYS, EVENT(scImilOpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_CELL_BEHAVIOR_OFFSET + IMIL_TO_HOUSE_NORTHEAST, CONDITION_ALWAYS, EVENT(scImilOpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_CELL_BEHAVIOR_OFFSET + IMIL_TO_ICE_ROOMS_EAST, CONDITION_ALWAYS, EVENT(scImilOpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_CELL_BEHAVIOR_OFFSET + IMIL_TO_ICE_ROOMS_NORTHEAST, CONDITION_ALWAYS, EVENT(scImilOpenDoorAndExit) },
    { TRIGGER_STEP, CELL_BEHAVIOR_IMIL_PATH_TO_FALLS_CAVE, CONDITION_ALWAYS, EVENT(scImilLeaveNorth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_IMIL_ICE, CONDITION_ALWAYS, EVENT(scImilSlideOnIce) },
    { TRIGGER_TALK, ACTOR_FREEZING_YOUNG_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(scImilTalkToFreezingYoungMan) },
    { TRIGGER_TALK, ACTOR_FREEZING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgImilTheMercuryLighthouseIsShiningMia) },
    { TRIGGER_TALK, ACTOR_GREETING_BOY, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(scImilGreetVisitor) },
    { TRIGGER_TALK, ACTOR_GREETING_BOY, CONDITION_ALWAYS, MSGNO(MsgImilImilWillBeFineDiseaseIs) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_KEEPING_IN_SHAPE, CONDITION_ALWAYS, EVENT(scImilTalkToStrongmanKeepingInShape) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FREEZING_YOUNG_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(scImilReadFreezingYoungMansMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FREEZING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgImilMiaWasOnlyInImilTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_BOY, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWeveHadALotOfStrangers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_BOY, CONDITION_ALWAYS, MSGNO(MsgImilDontWorryAboutUsMiaWell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_KEEPING_IN_SHAPE, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilPeopleGetSickBecauseTheyStay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_KEEPING_IN_SHAPE, CONDITION_ALWAYS, MSGNO(MsgImilNowThatIveLetMyGuard) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 100, FLAG_IMIL_COINS, FIND_COINS(9) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_IMIL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_GRAVE), 102, FLAG_IMIL_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TABLE_END },
};

TRIGGER Imil_RoomTriggers[] = {
    { TRIGGER_TALK, ACTOR_OLD_WOMAN, FLAG_IMIL_MIA_SENT_FOR, MSGNO(MsgImilTheresBeenAnEpidemicGoingAround) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN, FLAG_IMIL_MET_MIA, MSGNO(MsgImilGetMiaHurry) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(scImilTalkToOldWoman) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgImilThatFountainWaterReallyMadeGrandpa) },
    { TRIGGER_TALK, ACTOR_OLD_MAN, FLAG_IMIL_MIA_SENT_FOR, EVENT(scImilTalkToOldMan) },
    { TRIGGER_TALK, ACTOR_OLD_MAN, FLAG_IMIL_MET_MIA, MSGNO(MsgImilItHurts) },
    { TRIGGER_TALK, ACTOR_OLD_MAN, CONDITION_ALWAYS, EVENT(scImilTalkToOldMan) },
    { TRIGGER_TALK, ACTOR_OLD_MANS_BED, IF_FLAG_SET(FLAG_IMIL_MET_MIA), EVENT(scImilTalkToOldManInBed) },
    { TRIGGER_TALK, ACTOR_SICK_OLD_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilCoughTheEpidemicHasHitAlmost) },
    { TRIGGER_TALK, ACTOR_SICK_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgImilItsNiceThatTheFountainCan) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(scImilAskAboutWeapons) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(scImilTalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWereClosedSorryIGetA) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(scImilTalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilIfYouWantToBuyAnything) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(scImilTalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANTS_DAUGHTER, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(scImilTalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANTS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgImilImSoHappyThatMommyIs) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(scImilTalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_INN_ATTENDANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWereTheOnlyInnThatStays) },
    { TRIGGER_TALK, ACTOR_INN_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgImilThankGoodnessTheTownIsBack) },
    { TRIGGER_TALK, ACTOR_RESTAURANT_OWNER, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(scImilAskAboutRestaurantMenu) },
    { TRIGGER_TALK, ACTOR_RESTAURANT_OWNER, CONDITION_ALWAYS, MSGNO(MsgImilImGladTheBeaconHasBeen) },
    { TRIGGER_TALK, ACTOR_JUSTIN, CONDITION_ALWAYS, EVENT(scImilTalkToJustin) },
    { TRIGGER_TALK, ACTOR_MEGAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(scImilTalkToMegan) },
    { TRIGGER_TALK, ACTOR_MEGAN, CONDITION_ALWAYS, MSGNO(MsgImilIWillBeAKindGirl) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(scImilReadOldWomanMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgImilGrandpasAllBetterNowThanksTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_OLD_MAN, FLAG_IMIL_MIA_SENT_FOR, EVENT(scImilTalkToOldMan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, FLAG_IMIL_MET_MIA, MSGNO(MsgImilOoohIDontFeelSoGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilMiaIsAGoodGirlI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgImilNowMiaCanLeaveTheVillage) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MANS_BED, IF_FLAG_SET(FLAG_IMIL_MET_MIA), EVENT(scImilReadOldManInBedMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SICK_OLD_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilHerPalmsShineWithABlue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SICK_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgImilThatBlueLightFromHerHands) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWhoAreThesePeopleTheyMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgImilCustomersAreHardToComeBy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWhyDoTheyAlwaysComeWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgImilTheCustomersAreBackIThought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilIfOnlyWeHadSomeOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgImilMiaSaidItWouldntBeGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANTS_DAUGHTER, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilIWonderIfTheyAreHeaded) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANTS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgImilMyDaddysStillInKolimaIll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilSoManyCustomersItJustGoes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgImilMiaWillBeLeavingWithYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_ATTENDANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWintersAreToughHereSoIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgImilTheyCameToOurVillageAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RESTAURANT_OWNER, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWeDontGetToEatMany) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RESTAURANT_OWNER, CONDITION_ALWAYS, MSGNO(MsgImilINeverToldMiaThatI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JUSTIN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(scImilReadJustinsMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JUSTIN, CONDITION_ALWAYS, MSGNO(MsgImilMiasGoingToDefendTheHonor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MEGAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(scImilReadMegansMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MEGAN, CONDITION_ALWAYS, MSGNO(MsgImilImSorryMiaButICant) },
    { EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_ICE_ROOMS_WEAPON_COUNTER, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(scImilTalkToWeaponMerchant) },
    { EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_ICE_ROOMS_ARMOR_COUNTER, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT(scImilTalkToArmorMerchant) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_IMIL_EMPTY_BOTTLE, FIND_ITEM(ITEM_EMPTY_BOTTLE) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 101, FLAG_IMIL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 102, FLAG_IMIL_LUCKY_PEPPER, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgImilItLooksLikePorridgeItIs)) },
    /* Unused: this oven's item row answers first (see the maps README). */
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgImilItsSomeHotStewItWarms)) },
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, ICE_ROOMS_HOUSE_WEST_TO_IMIL },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, ICE_ROOMS_TO_IMIL_WEST_SOUTHWEST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, ICE_ROOMS_TO_IMIL_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, ICE_ROOMS_TO_IMIL },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, ICE_ROOMS_TO_IMIL_SOUTH_SOUTHWEST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, ICE_ROOMS_INN_UPSTAIRS_TO_INN },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, ICE_ROOMS_TO_IMIL_EAST_NORTHEAST },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, ICE_ROOMS_INN_TO_INN_UPSTAIRS },
    { TRIGGER_TABLE_END },
};

/* Each door's opening, as map grid animations. */
u16 Imil_SanctumDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(40, 37, 4, 2, 5), MAPGRID_ANIMATION_STEP(40, 39, 4, 2, 5), MAPGRID_ANIMATION_STEPS_END,
    MAPGRID_ANIMATION_STEP(35, 35, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
u16 Imil_RoomsEastDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 37, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 37, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
u16 Imil_InnDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 43, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 43, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
u16 Imil_HouseSouthwestDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 39, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 39, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
u16 Imil_HouseWestDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(42, 48, 2, 2, 5), MAPGRID_ANIMATION_STEP(40, 48, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
u16 Imil_HouseNortheastDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(42, 46, 2, 2, 5), MAPGRID_ANIMATION_STEP(40, 46, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

/* Each door's opening, in the order of its exit (IMIL_TO_HOUSE_SOUTHWEST */
/* first), as scImilOpenDoorAndExit reads it. */
u16 *Imil_DoorCellSteps[] = {
    Imil_HouseSouthwestDoorSteps,
    Imil_InnDoorSteps,
    Imil_HouseWestDoorSteps,
    Imil_HouseNortheastDoorSteps,
    Imil_RoomsEastDoorSteps,
    Imil_SanctumDoorSteps,
};

/* Cell position (x, y) for each exit. */
s16 Imil_DoorCellPoints[][2] = {
    { 50, 24 }, { 57, 22 }, { 59, 13 }, { 54, 10 }, { 38, 8 }, { 34, 19 },
};
/* clang-format on */

/* The old man's bed as he sits up in it, and as he lies back. */
u16 Imil_OldManSitUpSteps[] = {
	MAPGRID_ANIMATION_STEP(1, 125, 1, 2, 10),
	MAPGRID_ANIMATION_STEP(0, 125, 1, 2, 10),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Imil_OldManLieBackSteps[] = {
	MAPGRID_ANIMATION_STEP(1, 125, 1, 2, 10),
	MAPGRID_ANIMATION_STEP(2, 125, 1, 2, 10),
	MAPGRID_ANIMATION_STEPS_END,
};

s32 Imil_EffectCenter[3];
