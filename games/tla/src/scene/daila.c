/* Daila, the village in the north of Indra that Felix reaches first after the
   tidal wave, in two scenes that share this overlay.

   On the first visit (SCENE_DAILA_FLOODED) the wave's salt water still stands
   in pools in the streets, an old man lies where it knocked him over, and the
   villagers wonder where Riki and Tavi have gone. Once Tavi is rescued from the
   Shrine of the Sea God the village is SCENE_DAILA: the water has drained to
   six small puddles, and walking up to the mayor's door on his rise in the east
   brings Alex out of it, off to Madra to find a boat (scDailaMeetAlex). The
   getters pick each scene's tables by MainRoutineStateWork.next.

   In both, six doors lead into the houses (scDailaEnterDoor), the sanctuary
   stands at the top of the steps in the north, and the village is left by the
   south road or the far northwest corner. In the drained village Felix hops
   over a puddle he walks into (scDailaJumpForward), Parch dries one up, Frost
   freezes one into a pillar of ice, and Scoop beside the puddle west of the big
   tree in the southeast digs up a Psy Crystal. */

#include "field/field.h"
#include "game/game.h"
#include "field/field_script.h"
#include "field/script_inline.h"
#include "scene/scene.h"
#include "text/msg_ids.h"
#include "field/actor_script.h"
#include "shop/shop.h"

/* The map metatiles the cue tables answer, as map.txt numbers them. Each door's
   metatile is the number of the exit it leaves by, in both scenes. */
typedef enum DAILA_METATILE {
	METATILE_RIKIS_HOUSE_DOOR = DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_WEST,
	METATILE_TAVIS_HOUSE_DOOR = DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_NORTHWEST,
	METATILE_MIDDLE_HOUSE_DOOR = DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_NORTHEAST,
	METATILE_MAYORS_HOUSE_DOOR = DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_EAST,
	METATILE_INN_DOOR = DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_SOUTHWEST,
	METATILE_WEAPON_AND_ARMOR_SHOP_DOOR =
		DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_SOUTHEAST,
	METATILE_SANCTUARY_DOOR = DAILA_FLOODED_TO_DAILA_ROOMS_SANCTUM,
	METATILE_SOUTH_ROAD = DAILA_FLOODED_TO_WORLD_MAP_SOUTH,
	METATILE_NORTHWEST_ROAD = DAILA_FLOODED_TO_WORLD_MAP_NORTH,
	/* Just south of the mayor's door, metatile (29, 17). */
	METATILE_BEFORE_MAYORS_DOOR = 20,
	/* The metatiles beside each puddle, named by the side they lie on: walking
	   from one into the puddle hops over it. */
	METATILE_NORTH_OF_PUDDLE = 30,
	METATILE_WEST_OF_PUDDLE,
	METATILE_EAST_OF_PUDDLE,
	METATILE_SOUTH_OF_PUDDLE,
	/* The puddles' own metatiles, in the order of map.txt's rows. */
	METATILE_FIRST_PUDDLE = 60,
	METATILE_FOURTH_PUDDLE,
	METATILE_SECOND_PUDDLE,
	METATILE_THIRD_PUDDLE,
	METATILE_FIFTH_PUDDLE,
	METATILE_SIXTH_PUDDLE,
	/* Metatile (30, 22), northeast of the third puddle, where the Psy Crystal
	   lies. */
	METATILE_BURIED_PSY_CRYSTAL = 100,
	/* The well west of the middle house, metatiles (14, 19) to (15, 20). */
	METATILE_WELL = 200
} DAILA_METATILE;

/* The drained village's puddles dried by Parch for this visit, one flag each,
   in the order of the puddles' rows. */
#define FLAG_DAILA_FIRST_PUDDLE_PARCHED	 0x210
#define FLAG_DAILA_SECOND_PUDDLE_PARCHED 0x211
#define FLAG_DAILA_THIRD_PUDDLE_PARCHED	 0x212
#define FLAG_DAILA_FOURTH_PUDDLE_PARCHED 0x213
#define FLAG_DAILA_FIFTH_PUDDLE_PARCHED	 0x214
#define FLAG_DAILA_SIXTH_PUDDLE_PARCHED	 0x215
/* The first of six flags, one for each of the drained village's puddles that
   Frost has frozen into a pillar for this visit, by
   DailaRoomsPillarFrozenFlags. */
#define FLAG_DAILA_PUDDLE_FROZEN_FIRST	 0x216

/* The people and puddles the scripts move, numbering the rows of
   DailaActorPlacements (the flooded village's rows to ACTOR_ITEM_SHOPKEEPER are
   alike). */
typedef enum DAILA_ACTOR_ID {
	/* "You don't look familiar. Have you been to Daila before?" */
	ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE = ACTOR_FIRST_PLACED,
	/* "Didn't it feel like all of Indra moved when that wave hit?" */
	ACTOR_WOMAN_WHO_FELT_INDRA_MOVE,
	/* "I saw a bright light in the northwestern sky earlier... Somewhere over
	   Gondowan." */
	ACTOR_MAN_WHO_SAW_A_LIGHT,
	/* "Thank goodness for that mountain range to the north..." */
	ACTOR_VILLAGER_THANKFUL_FOR_THE_MOUNTAINS,
	ACTOR_FALLEN_OLD_MAN,
	/* "That ancient tower to the east? It's the Shrine of the Sea God..." */
	ACTOR_VILLAGER_ON_THE_SHRINE,
	/* "Riki and Tavi sure are out late..." */
	ACTOR_VILLAGER_WAITING_UP_FOR_RIKI_AND_TAVI,
	/* "I wonder if those two are still out there... They know it's
	   off-limits." */
	ACTOR_VILLAGER_ON_RIKI_AND_TAVI,
	/* "Our village survived the wave, but all our boats were destroyed..." */
	ACTOR_SAILOR_WITHOUT_A_BOAT,
	/* "Hm? You want a boat?" */
	ACTOR_SAILOR_ON_THE_BOATS,
	/* "That tidal wave left a lot of standing water in our village..." */
	ACTOR_MAN_ON_THE_STANDING_WATER,
	/* "Yaaay! Puddles in the village! Yaaay!" */
	ACTOR_CHILD_WHO_LOVES_PUDDLES,
	/* "Bleah! I fell in a puddle and got my clothes dirty! Mom's gonna have a
	   fit!" */
	ACTOR_CHILD_WHO_FELL_IN_A_PUDDLE,
	/* The keeper of the item stall east of the inn. */
	ACTOR_ITEM_SHOPKEEPER,
	ACTOR_FIRST_PUDDLE,
	ACTOR_SECOND_PUDDLE,
	ACTOR_THIRD_PUDDLE,
	ACTOR_FOURTH_PUDDLE,
	ACTOR_FIFTH_PUDDLE,
	ACTOR_ALEX,
	ACTOR_KRADEN,
	/* After Jenna's and Sheba's rows, which place the party's own actors. */
	ACTOR_SIXTH_PUDDLE
} DAILA_ACTOR_ID;

/* The buried Psy Crystal: the village's third scene item, after the jar and the
   barrel. */
#define ACTOR_PSY_CRYSTAL (FIELD_TREASURE_ACTOR + 2)

extern SCENE_ENTRANCE DailaEntrances[];
extern Sint32		  DailaFloodedExits[];
extern Sint32		  DailaExits[];
extern ACTOR_PLACE	  DailaFloodedActorPlacements[];
extern ACTOR_PLACE	  DailaActorPlacements[];
extern CUE			  DailaFloodedCues[];
extern CUE			  DailaCues[];
extern Sint16		  DailaRoomsPillarFrozenFlags[];
extern Sint32		  DailaFallenOldManScript[];
extern Uint32		  DailaPuddleChildScript[];

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return DailaEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

Sint32 *scGetExits(void)
{
	if(MainRoutineStateWork.next == SCENE_DAILA)
		return DailaExits;
	else
		return DailaFloodedExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.next == SCENE_DAILA)
		return DailaActorPlacements;
	else
		return DailaFloodedActorPlacements;
}

/* Felix walks in at the door of one of the village's six houses: Riki's in the
   west (metatile (7, 20)), Tavi's (metatile (12, 17)), the middle house
   (metatile (21, 19)), the mayor's on its rise (metatile (29, 16)), the inn
   (metatile (13, 27)) or the weapon and armor shop (metatile (23, 27)). The
   door sound plays, he steps on into the dark doorway, and the screen closes in
   a circle on him before he appears inside. The door's metatile is the number
   of the exit taken. */
void scDailaEnterDoor(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 2);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlg = 0;
	sySndPlay(SE_MAP_EXIT);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorCenterAndWalk(MainRoutineStateWork.leaderActorId, 2, -6);
	fdScriptWait(10);
	syImportScriptSetExitRequest(state->stepCue);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}

/* Scoop just northeast of the puddle west of the big tree, in the village's
   southeast (metatile (30, 22)), digs up a Psy Crystal: a hand scoops at the
   earth, the crystal pops out and Felix holds it up. */
void scDailaDigUpPsyCrystal(Sint32 mode)
{
	fdScriptDigUpTreasure(mode, ACTOR_PSY_CRYSTAL, FLAG_DAILA_PSY_CRYSTAL_DUG);
}

const CUE *scGetCues(void)
{
	if(MainRoutineStateWork.next == SCENE_DAILA)
		return DailaCues;
	else
		return DailaFloodedCues;
}

/* The villager wandering at the west edge of the flooded village asks once
   whether Felix has been to Daila before, and answers yes or no; after that he
   marvels that the wave hurt no one. */
void scDailaTalkBeenBefore(void)
{
	Sint32 msg;

	if(syGameFlagGet(FLAG_DAILA_ASKED_IF_BEEN_BEFORE) == 0) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		msg = (Sint32)MsgDailaBeenBefore;
		fdScriptSetMessageId(msg);
		/* "You don't look familiar. Have you been to Daila before?" Yes:
		   "Strange... I just don't remember ever seeing you around here
		   before." No: "Well then, welcome to Daila, the northernmost town on
		   the continent of Indra!" */
		fdMessageShowAtActor(ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE, 0);
		if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) ==
			0) {
			fdScriptWait(10);
			fdScriptSetMessageId(msg + 1);
		} else {
			fdScriptWait(20);
			fdScriptSetMessageId(msg + 2);
		}
		fdScriptShowMessageAndWait(ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE, 0);
		syGameFlagSet(FLAG_DAILA_ASKED_IF_BEEN_BEFORE);
		fdScriptEnd();
	} else {
		fdScriptSetMessageId((Sint32)MsgDailaWaveNoInjuries);
		/* "We were hit by a gigantic tidal wave, but there were no
		   injuries......" */
		fdScriptShowMessageAndWait(ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE, 0);
	}
}

/* A woman in the flooded village asks whether all of Indra seemed to move when
   the tidal wave hit, and answers yes or no. */
void scDailaTalkIndraMoved(void)
{
	Sint32 msg;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	msg = (Sint32)MsgDailaIndraMoved;
	fdScriptSetMessageId(msg);
	/* "Didn't it feel like all of Indra moved when that wave hit?" Yes: "I knew
	   it! You felt it, too!..." No: "Wow... The wave must have knocked you
	   silly if you didn't notice it..." */
	fdMessageShowAtActor(ACTOR_WOMAN_WHO_FELT_INDRA_MOVE, 0);
	if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(10);
		fdScriptSetMessageId(msg + 1);
	} else {
		fdScriptWait(20);
		fdScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessageAndWait(ACTOR_WOMAN_WHO_FELT_INDRA_MOVE, 0);
	fdScriptEnd();
}

/* The actor the boat sailor's question and answer open at. */
/* BUG: the question and its answer open at the woman who felt Indra move, as in
   scDailaTalkIndraMoved, not at the sailor who asks. */
#define BOAT_ASKER \
	WHEN(FIX_DAILA_BOAT_SAILOR_SPEAKER, ACTOR_SAILOR_ON_THE_BOATS, \
		ACTOR_WOMAN_WHO_FELT_INDRA_MOVE)

/* A sailor in the flooded village asks whether Felix wants a boat, and answers
   yes or no. */
void scDailaTalkBoat(void)
{
	Sint32 msg;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	msg = (Sint32)MsgDailaWantBoat;
	fdScriptSetMessageId(msg);
	/* "Hm? You want a boat?" Yes: "What kind of insensitive jerk are you?..."
	   No: "Of course not! The tidal wave ruined all our boats..." */
	fdMessageShowAtActor(BOAT_ASKER, 0);
	if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(10);
		fdScriptSetMessageId(msg + 1);
	} else {
		fdScriptWait(20);
		fdScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessageAndWait(BOAT_ASKER, 0);
	fdScriptEnd();
}

/* The keeper of the item stall east of the inn sells to Felix when he talks to
   her from in front of the counter, facing north; from any other side she
   worries that Tavi and Riki have not been seen lately. */
void scDailaAskAfterTaviAndRiki(Sint32 keeper)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_DAILA_ITEMS, keeper);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgDailaTaviRikiWorry);
		/* "I haven't seen Tavi and Riki around lately... I like those
		   little..." */
		fdScriptShowMessageAndWait(keeper, 0);
		fdScriptEnd();
	}
}

/* The old man the wave knocked over, on his feet again once the village drains,
   asks whether Felix thought it was the wave that hurt his hip, and answers yes
   or no. */
void scDailaTalkHip(void)
{
	Sint32 msg;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	msg = (Sint32)MsgDailaHurtHip;
	fdScriptSetMessageId(msg);
	/* "I bet you thought it was the wave that knocked me over and hurt my hip,
	   huh?" Yes: "It'd take more than a little water to knock this old-timer
	   down!..." No: "Nobody believes me when I tell them the continent
	   moved..." */
	fdMessageShowAtActor(ACTOR_FALLEN_OLD_MAN, 0);
	if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(10);
		fdScriptSetMessageId(msg + 1);
	} else {
		fdScriptWait(20);
		fdScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessageAndWait(ACTOR_FALLEN_OLD_MAN, 0);
	fdScriptEnd();
}

/* Once Tavi and Riki are home, the keeper of the item stall sells to Felix from
   in front of the counter, as before, and from any other side tells of their
   return. */
void scDailaTaviAndRikiAreHome(Sint32 keeper)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnShopRun(SHOP_DAILA_ITEMS, keeper);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgDailaTaviRikiHome);
		/* "Tavi and Riki are back home, safe and sound... They stopped by
		   to..." */
		fdScriptShowMessageAndWait(keeper, 0);
		fdScriptEnd();
	}
}

/* The party member walks into Felix and vanishes, as the party does when a
   scene ends. */
static inline void scActorJoinLeader(Sint32 actor)
{
	ACTOR *leader;

	fdActorSetAnimationById(actor, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(
			actor, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(actor);
	fdActorSetPos(actor, 0, 0);
}

/* Where the meeting's lines, from MsgDailaFelixCall on, go their own way after
   Jenna asks whether to go to Madra with Alex. */
typedef enum MEET_ALEX_MESSAGE {
	/* "Ah... You...want to travel with me?" */
	MEET_ALEX_TRAVEL_WITH_ME = 11,
	/* "Ah. Felix doesn't think it's a good idea." */
	MEET_ALEX_NOT_A_GOOD_IDEA = 13,
	/* "No, that's not it. I'm just...preoccupied." */
	MEET_ALEX_PREOCCUPIED = 15
} MEET_ALEX_MESSAGE;

/* As Felix walks up to the mayor's door (metatile (29, 17)), Alex steps out of
   it and calls to him. The party gathers, Alex says the mayor told him to try
   Madra for a boat, Jenna asks whether to go with him, and whatever Felix
   answers Alex prefers to work alone and walks off south. It happens once, and
   only before Felix has been to Madra's gate and before Piers joins. */
void scDailaMeetAlex(void)
{
	Sint32 msg;

	if(syGameFlagGet(FLAG_DAILA_MET_ALEX))
		return;
	if(syGameFlagGet(FLAG_MADRA_GATE_INSPECTED))
		return;
	if(syGameFlagGet(FLAG_IN_PARTY(ACTOR_PIERS)))
		return;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	msg = (Sint32)MsgDailaFelixCall;
	fdScriptSetMessageId(msg);
	fdActorSetPos(ACTOR_ALEX, 472, 264);
	fdScriptWait(30);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 504, 296);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_NORTHWEST, 0);
	fdActorMoveToAndWait(ACTOR_ALEX, 472, 280);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
	fdScriptWait(30);
	fdActorShowEmoteById(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(60);
	/* "Felix..." */
	fdScriptShowMessageAndWait(ACTOR_ALEX, 0);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 8, 16, FACING_NORTHWEST);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 16, 0, FACING_NORTHWEST);
	fdActorLaunchFromLeader(ACTOR_KRADEN, 32, -8, FACING_WEST);
	fdScriptWait(60);
	/* "Well, well... Our happy little family is back together again." */
	fdScriptShowMessageAndWait(ACTOR_ALEX, 0);
	fdScriptWait(30);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdScriptWait(60);
	/* "Alex... Where did you go?" */
	fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	fdScriptWait(30);
	fdActorFaceDirectionById(ACTOR_ALEX, FACING_EAST, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
	fdScriptWait(30);
	fdActorFaceDirectionById(ACTOR_ALEX, FACING_SOUTH, 0);
	fdScriptWait(30);
	fdActorFaceDirectionById(ACTOR_ALEX, FACING_SOUTHEAST, 0);
	fdScriptWait(30);
	/* "To look for a ship, of course." */
	fdScriptShowMessageAndWait(ACTOR_ALEX, 0);
	fdScriptWait(30);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_KRADEN, 0);
	fdScriptWait(30);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(30);
	/* "That's what Kraden said..." */
	fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
	fdScriptWait(30);
	fdActorTurnToFaceEachOther(
		MainRoutineStateWork.leaderActorId, ACTOR_KRADEN, 0);
	fdScriptWait(30);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_ALEX, 0);
	fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_ALEX, 0);
	fdScriptWait(60);
	/* "And did you find one?" */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdScriptWait(30);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_ALEX, 0);
	fdActorSetAnimationById(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	fdScriptWait(60);
	/* "Unfortunately, there are none to be had." */
	fdScriptShowMessageAndWait(ACTOR_ALEX, 0);
	fdScriptWait(30);
	fdActorFaceDirectionById(ACTOR_ALEX, FACING_NORTH, 0);
	fdScriptWait(60);
	/* "I spoke to the mayor, and he suggested that we try the large town..." */
	fdScriptShowMessageAndWait(ACTOR_ALEX, 0);
	fdActorFaceActorById(ACTOR_ALEX, MainRoutineStateWork.leaderActorId, 0);
	fdScriptWait(60);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 1);
	fdScriptWait(60);
	/* "Is that where you're going?" */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdScriptWait(30);
	fdActorSetAnimationById(ACTOR_ALEX, ANIM_NOD);
	fdScriptWait(30);
	/* "Yes. Madra may have boats available. That would be the most
	   logical..." */
	fdScriptShowMessageAndWait(ACTOR_ALEX, 0);
	fdScriptWait(30);
	fdActorFaceActorById(ACTOR_JENNA, MainRoutineStateWork.leaderActorId, 0);
	/* "So what do yout think? Should we go to Madra with Alex?" */
	fdMessageShowAtActor(ACTOR_JENNA, 0);
	if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(30);
		fdScriptSetMessageId(msg + MEET_ALEX_TRAVEL_WITH_ME);
		/* "Ah... You...want to travel with me?" */
		fdScriptShowMessageAndWait(ACTOR_ALEX, 0);
		fdScriptWait(60);
		fdActorShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
		fdScriptWait(60);
		fdActorFaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
		/* "What, you don't like that idea?" */
		fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
		fdScriptSetMessageId(msg + MEET_ALEX_PREOCCUPIED);
	} else {
		fdScriptWait(30);
		fdScriptSetMessageId(msg + MEET_ALEX_NOT_A_GOOD_IDEA);
		/* "Ah. Felix doesn't think it's a good idea." */
		fdScriptShowMessageAndWait(ACTOR_ALEX, 0);
		fdScriptWait(60);
		fdActorRunRepeatedMotion(ACTOR_JENNA, 1);
		fdActorFaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
		/* "You seem awfully pleased about that..." */
		fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	}
	fdActorSetAnimationById(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	fdScriptWait(60);
	/* "No, that's not it. I'm just...preoccupied." */
	fdScriptShowMessageAndWait(ACTOR_ALEX, 0);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 1);
	fdScriptWait(60);
	/* "Self-absorbed is more like it!" */
	fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
	fdScriptWait(60);
	fdActorFaceActorById(ACTOR_ALEX, ACTOR_SHEBA, 0);
	fdScriptWait(60);
	/* "I simply prefer to work alone." */
	fdScriptShowMessageAndWait(ACTOR_ALEX, 0);
	fdScriptWait(60);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	/* "If that's the case, we'll let you be..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdScriptWait(60);
	fdActorSetAnimationById(ACTOR_ALEX, ANIM_NOD);
	fdScriptWait(60);
	/* "Perhaps you will see me in Madra..." */
	fdScriptShowMessageAndWait(ACTOR_ALEX, 0);
	fdActorWalkToAndWait(ACTOR_ALEX, 504, 328);
	fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_ALEX, 0);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_ALEX, 0);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_ALEX, 0);
	fdActorWalkToAndWait(ACTOR_ALEX, 456, 328);
	fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_ALEX, 0);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_ALEX, 0);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_ALEX, 0);
	fdActorWalkToAndWait(ACTOR_ALEX, 424, 472);
	fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_ALEX, 0);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_ALEX, 0);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_ALEX, 0);
	fdScriptWait(60);
	fdActorSetPos(ACTOR_ALEX, 0, 0);
	fdScriptWait(120);
	fdActorJump(ACTOR_JENNA, 4, 20);
	fdActorJump(ACTOR_JENNA, 4, 20);
	fdScriptWait(30);
	fdActorShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdScriptWait(60);
	/* "Gah! The nerve of that guy!" */
	fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	fdScriptWait(60);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(60);
	/* "Yeah, I know... Now, I just want to find a boat before he does!" */
	fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
	fdScriptWait(60);
	fdActorFaceActorById(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId, 0);
	fdScriptWait(60);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_SOUTH, 0);
	fdScriptWait(30);
	/* "Let's be on our way." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdScriptWait(60);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(120);
	scActorJoinLeader(ACTOR_JENNA);
	scActorJoinLeader(ACTOR_SHEBA);
	scActorJoinLeader(ACTOR_KRADEN);
	fdScriptEnd();
	syGameFlagSet(FLAG_DAILA_MET_ALEX);
}

/* Parch on a puddle dries it for the visit: the puddle disappears, its flag is
   set, and the metatile just west of it (column fromX of row y) is copied over
   its own at toX. */
#define DEFINE_DAILA_PARCH_PUDDLE(name, fromX, y, toX, actor, flag) \
	void name(void) \
	{ \
		fdScriptStart(); \
		fdScriptEndLastingPsynergy(0); \
		fdMapGridCopyMetatileAttributes(fromX, y, 1, 1, toX, y); \
		fdActorSetPos(actor, 0, 0); \
		syGameFlagSet(flag); \
		fdScriptEnd(); \
	}

/* Parch on the puddle west of the well (metatile (11, 19)) dries it up: the
   puddle disappears, and its metatile becomes like the ground beside it for the
   rest of the visit. */
DEFINE_DAILA_PARCH_PUDDLE(DailaParchFirstPuddle, 10, 19, 11, ACTOR_FIRST_PUDDLE,
	FLAG_DAILA_FIRST_PUDDLE_PARCHED)

/* Parch on the puddle east of the middle house (metatile (26, 20)) dries it up:
   the puddle disappears, and its metatile becomes like the ground beside it for
   the rest of the visit. */
DEFINE_DAILA_PARCH_PUDDLE(DailaParchSecondPuddle, 25, 20, 26,
	ACTOR_SECOND_PUDDLE, FLAG_DAILA_SECOND_PUDDLE_PARCHED)

/* Parch on the puddle west of the big tree in the southeast (metatile (29, 23))
   dries it up: the puddle disappears, and its metatile becomes like the ground
   beside it for the rest of the visit. */
DEFINE_DAILA_PARCH_PUDDLE(DailaParchThirdPuddle, 28, 23, 29, ACTOR_THIRD_PUDDLE,
	FLAG_DAILA_THIRD_PUDDLE_PARCHED)

/* Parch on the puddle below the sanctuary's steps (metatile (18, 11)) dries it
   up:
   the puddle disappears, and its metatile becomes like the ground beside it for
   the rest of the visit. */
DEFINE_DAILA_PARCH_PUDDLE(DailaParchFourthPuddle, 17, 11, 18,
	ACTOR_FOURTH_PUDDLE, FLAG_DAILA_FOURTH_PUDDLE_PARCHED)

/* Parch on the puddle beside the mayor's door (metatile (31, 17)) dries it up:
   the puddle disappears, and its metatile becomes like the ground beside it for
   the rest of the visit. */
DEFINE_DAILA_PARCH_PUDDLE(DailaParchFifthPuddle, 30, 17, 31, ACTOR_FIFTH_PUDDLE,
	FLAG_DAILA_FIFTH_PUDDLE_PARCHED)

/* The actor Parch on the sixth puddle hides. */
/* BUG: hides actor 31, which the sixth puddle would be if Jenna's and Sheba's
   rows took numbers of their own; they place the party's actors, so the puddle
   is actor 29, no actor 31 exists, and the puddle stays on its dried
   metatile. */
#define SIXTH_PUDDLE_HIDDEN \
	WHEN(FIX_DAILA_SIXTH_PUDDLE_STAYS_IN_SIGHT, ACTOR_SIXTH_PUDDLE, \
		(ACTOR_SIXTH_PUDDLE + 2))
/* Parch on the puddle north of the item stall (metatile (17, 23)) dries its
   metatile like the others, but without `BUGFIX` the puddle itself stays in
   sight. */
DEFINE_DAILA_PARCH_PUDDLE(DailaParchSixthPuddle, 16, 23, 17,
	SIXTH_PUDDLE_HIDDEN, FLAG_DAILA_SIXTH_PUDDLE_PARCHED)

#include "shared/scene_freeze_puddle_into_pillar.h"

Sint32 scInit(void)
{
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if(MainRoutineStateWork.next == SCENE_DAILA_FLOODED)
		fdActorFallOverById(ACTOR_FALLEN_OLD_MAN, FACING_EAST);
	else if(MainRoutineStateWork.next == SCENE_DAILA)
		fdFlagBlockPlaceAll(DailaRoomsPillarFrozenFlags);
	fdActorSetSprPriority(ACTOR_ITEM_SHOPKEEPER, 1);
	if(MainRoutineStateWork.next == SCENE_DAILA &&
		syGameFlagGet(FLAG_DAILA_PSY_CRYSTAL_DUG) == 0)
		fdActorSetWideSpr(ACTOR_PSY_CRYSTAL, TREASURE_SPOT_PUT_AWAY);
	return 0;
}

Sint32 scPrepareMap(void)
{
	if(MainRoutineStateWork.next == SCENE_DAILA) {
		if(syGameFlagGet(FLAG_DAILA_FIRST_PUDDLE_PARCHED))
			fdMapGridCopyMetatileAttributes(10, 19, 1, 1, 11, 19);
		if(syGameFlagGet(FLAG_DAILA_SECOND_PUDDLE_PARCHED))
			fdMapGridCopyMetatileAttributes(25, 20, 1, 1, 26, 20);
		if(syGameFlagGet(FLAG_DAILA_THIRD_PUDDLE_PARCHED))
			fdMapGridCopyMetatileAttributes(28, 23, 1, 1, 29, 23);
		if(syGameFlagGet(FLAG_DAILA_FOURTH_PUDDLE_PARCHED))
			fdMapGridCopyMetatileAttributes(17, 11, 1, 1, 18, 11);
		if(syGameFlagGet(FLAG_DAILA_FIFTH_PUDDLE_PARCHED))
			fdMapGridCopyMetatileAttributes(30, 17, 1, 1, 31, 17);
		if(syGameFlagGet(FLAG_DAILA_SIXTH_PUDDLE_PARCHED))
			fdMapGridCopyMetatileAttributes(16, 23, 1, 1, 17, 23);
	}
	return 0;
}

/* Felix hops over a puddle of the drained village when he walks into it from
   the metatile beside it on any side, landing on the far side if the ground
   there is clear. */
void scDailaJumpForward(void)
{
	fdTryJumpForward();
}

/* clang-format off */
/* The six puddles Frost freezes into pillars, each with the flag set while */
/* it stands (fdFlagBlockPlaceAll). */
Sint16 DailaRoomsPillarFrozenFlags[] = {
	ACTOR_FIRST_PUDDLE,  FLAG_DAILA_PUDDLE_FROZEN_FIRST + 0,
	ACTOR_SECOND_PUDDLE, FLAG_DAILA_PUDDLE_FROZEN_FIRST + 1,
	ACTOR_THIRD_PUDDLE,  FLAG_DAILA_PUDDLE_FROZEN_FIRST + 2,
	ACTOR_FOURTH_PUDDLE, FLAG_DAILA_PUDDLE_FROZEN_FIRST + 3,
	ACTOR_FIFTH_PUDDLE,  FLAG_DAILA_PUDDLE_FROZEN_FIRST + 4,
	ACTOR_SIXTH_PUDDLE,  FLAG_DAILA_PUDDLE_FROZEN_FIRST + 5,
	-1, 0,
};

Sint32 DailaFallenOldManScript[] = {
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
	SC_ANIMATION(0),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 4),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(40),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 4),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(5),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 4),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(30),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

Uint32 DailaPuddleChildScript[] = {
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
	SC_ACCELERATION(0.2),
	SC_SET(SW_GRAVITY, toFIXED(0.1)),
	SC_SPEED(0.4),
	SC_ANIMATION(0),
	SC_LABEL(1),
	SC_SET(SW_Y, toFIXED(4)),
	SC_WAIT(30),
	SC_SET(SW_Y, toFIXED(4)),
	SC_WAIT(60),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

SCENE_ENTRANCE DailaEntrances[] = {
	{0, CONDITION_ALWAYS, 120, 0, 338, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{10, CONDITION_ALWAYS, 368, 0, 224, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{20, CONDITION_ALWAYS, 80, 0, 352, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Sint32 DailaFloodedExits[] = {
	SCENE_EXITS(DailaFlooded),
	SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_WEST, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_NORTHWEST, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_NORTHEAST, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_EAST, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_SOUTHWEST, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_SOUTHEAST, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_SOUTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_SANCTUM, DailaRoomsSanctum, DAILA_ROOMS_SANCTUM_FROM_DAILA, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_DAILA_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_DAILA_NORTH, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

Sint32 DailaExits[] = {
	SCENE_EXITS(Daila),
	SCENE_EXIT_UNLESS(1, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(3, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(4, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(5, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(6, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_SOUTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(7, DailaRoomsSanctum, DAILA_ROOMS_SANCTUM_FROM_DAILA, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(8, WorldMap, WORLD_MAP_FROM_DAILA_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(9, WorldMap, WORLD_MAP_FROM_DAILA_NORTH, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

/* Unused: an empty placement table nothing reads. */
ACTOR_PLACE DailaUnusedPlacements[] = {
	{SCENE_TABLE_END},
};
/* clang-format on */

ACTOR_PLACE DailaFloodedActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
		MOVEMENT_TYPE_WANDER, 40, 0, 280, FACING_EAST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
		MOVEMENT_TYPE_WANDER_NEAR, 264, 0, 232, FACING_SOUTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS,
		MOVEMENT_TYPE_WANDER_NEAR, 88, 0, 376, FACING_EAST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT_SMALL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 264, 0, 312, FACING_WEST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS,
		(Sint32)DailaFallenOldManScript, 360, 0, 344, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND_SMALL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_NORTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR_SMALL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 248, 0, 456, FACING_SOUTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT_SMALL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 264, 0, 456, FACING_SOUTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS,
		MOVEMENT_TYPE_WANDER_NEAR, 376, 0, 472, FACING_SOUTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 392, 0, 312, FACING_SOUTHEAST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		136, 0, 184, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR_SMALL, CONDITION_ALWAYS,
		(Sint32)DailaPuddleChildScript, 584, 0, 384, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT_SMALL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 568, 0, 328, FACING_SOUTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 312, 0, 424, FACING_SOUTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE DailaActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
		MOVEMENT_TYPE_WANDER, 40, 0, 280, FACING_EAST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
		MOVEMENT_TYPE_WANDER_NEAR, 264, 0, 232, FACING_SOUTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS,
		MOVEMENT_TYPE_WANDER_NEAR, 88, 0, 376, FACING_EAST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT_SMALL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 264, 0, 312, FACING_WEST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE_SMALL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 360, 0, 344, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND_SMALL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_NORTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR_SMALL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 248, 0, 456, FACING_EAST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT_SMALL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 264, 0, 456, FACING_WEST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS,
		MOVEMENT_TYPE_WANDER_NEAR, 376, 0, 472, FACING_SOUTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 392, 0, 312, FACING_SOUTHEAST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS,
		MOVEMENT_TYPE_WANDER_NEAR, 120, 0, 216, FACING_WEST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR_SMALL, CONDITION_ALWAYS,
		(Sint32)DailaPuddleChildScript, 584, 0, 384, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT_SMALL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 584, 0, 344, FACING_WEST,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 312, 0, 424, FACING_SOUTH,
		TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_DAILA_FIRST_PUDDLE_PARCHED,
		MOVEMENT_TYPE_STAND, 184, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_DAILA_SECOND_PUDDLE_PARCHED,
		MOVEMENT_TYPE_STAND, 424, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_DAILA_THIRD_PUDDLE_PARCHED,
		MOVEMENT_TYPE_STAND, 472, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_DAILA_FOURTH_PUDDLE_PARCHED,
		MOVEMENT_TYPE_STAND, 296, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_DAILA_FIFTH_PUDDLE_PARCHED,
		MOVEMENT_TYPE_STAND, 504, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
		FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
		0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
		0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
		0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_DAILA_SIXTH_PUDDLE_PARCHED,
		MOVEMENT_TYPE_STAND, 280, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

CUE DailaFloodedCues[] = {
	{CUE_STEP, METATILE_RIKIS_HOUSE_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaEnterDoor)},
	{CUE_STEP, METATILE_TAVIS_HOUSE_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaEnterDoor)},
	{CUE_STEP, METATILE_MIDDLE_HOUSE_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaEnterDoor)},
	{CUE_STEP, METATILE_MAYORS_HOUSE_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaEnterDoor)},
	{CUE_STEP, METATILE_INN_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaEnterDoor)},
	{CUE_STEP, METATILE_WEAPON_AND_ARMOR_SHOP_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaEnterDoor)},
	{CUE_EXIT, METATILE_SANCTUARY_DOOR, CONDITION_ALWAYS,
		DAILA_FLOODED_TO_DAILA_ROOMS_SANCTUM},
	{CUE_EXIT, METATILE_SOUTH_ROAD, CONDITION_ALWAYS,
		DAILA_FLOODED_TO_WORLD_MAP_SOUTH},
	{CUE_EXIT, METATILE_NORTHWEST_ROAD, CONDITION_ALWAYS,
		DAILA_FLOODED_TO_WORLD_MAP_NORTH},
	{CUE_TALK, ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaTalkBeenBefore)},
	{CUE_TALK, ACTOR_WOMAN_WHO_FELT_INDRA_MOVE, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaTalkIndraMoved)},
	{CUE_TALK, ACTOR_MAN_WHO_SAW_A_LIGHT, CONDITION_ALWAYS,
		MSGNO(MsgFieldISawABrightLightIn)},
	{CUE_TALK, ACTOR_VILLAGER_THANKFUL_FOR_THE_MOUNTAINS, CONDITION_ALWAYS,
		MSGNO(MsgFieldThankGoodnessForThatMountainRange)},
	{CUE_TALK, ACTOR_FALLEN_OLD_MAN, CONDITION_ALWAYS,
		MSGNO(MsgFieldOhhhThatWaveKnockedMeOver)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_SHRINE, CONDITION_ALWAYS,
		MSGNO(MsgFieldThatAncientTowerToTheEast)},
	{CUE_TALK, ACTOR_VILLAGER_WAITING_UP_FOR_RIKI_AND_TAVI, CONDITION_ALWAYS,
		MSGNO(MsgFieldRikiAndTaviSureAreOut)},
	{CUE_TALK, ACTOR_VILLAGER_ON_RIKI_AND_TAVI, CONDITION_ALWAYS,
		MSGNO(MsgFieldIWonderIfThoseTwoAre)},
	{CUE_TALK, ACTOR_SAILOR_WITHOUT_A_BOAT, CONDITION_ALWAYS,
		MSGNO(MsgFieldOurVillageSurvivedTheWaveBut)},
	{CUE_TALK, ACTOR_SAILOR_ON_THE_BOATS, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaTalkBoat)},
	{CUE_TALK, ACTOR_MAN_ON_THE_STANDING_WATER, CONDITION_ALWAYS,
		MSGNO(MsgFieldThatTidalWaveLeftALot)},
	{CUE_TALK, ACTOR_CHILD_WHO_LOVES_PUDDLES, CONDITION_ALWAYS,
		MSGNO(MsgFieldYaaayPuddlesInTheVillageYaaay)},
	{CUE_TALK, ACTOR_CHILD_WHO_FELL_IN_A_PUDDLE, CONDITION_ALWAYS,
		MSGNO(MsgFieldBleahIFellInAPuddle)},
	{CUE_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaAskAfterTaviAndRiki)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE,
		CONDITION_ALWAYS, MSGNO(MsgFieldThatWaveTurnedThisPlaceInto)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WHO_FELT_INDRA_MOVE,
		CONDITION_ALWAYS, MSGNO(MsgFieldThatMustHaveBeenOneSerious)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_SAW_A_LIGHT,
		CONDITION_ALWAYS, MSGNO(MsgFieldIThoughtISawSomeLights)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
		ACTOR_VILLAGER_THANKFUL_FOR_THE_MOUNTAINS, CONDITION_ALWAYS,
		MSGNO(MsgFieldTheSeaGodMustHaveSaved)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_OLD_MAN,
		CONDITION_ALWAYS, MSGNO(MsgFieldMyHipsKillingMeAndAll)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_SHRINE,
		CONDITION_ALWAYS, MSGNO(MsgFieldICantBelieveThoseChildrenPlay)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
		ACTOR_VILLAGER_WAITING_UP_FOR_RIKI_AND_TAVI, CONDITION_ALWAYS,
		MSGNO(MsgFieldTheShrineOfTheSeaGod)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_RIKI_AND_TAVI,
		CONDITION_ALWAYS, MSGNO(MsgFieldICantBelieveThoseLosersWere)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WITHOUT_A_BOAT,
		CONDITION_ALWAYS, MSGNO(MsgFieldThatWaveWasBrutalItSmashed)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_BOATS,
		CONDITION_ALWAYS, MSGNO(MsgFieldWithAllTheBoatsGoneWe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STANDING_WATER,
		CONDITION_ALWAYS, MSGNO(MsgFieldWithAllThisSaltWaterOn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_WHO_LOVES_PUDDLES,
		CONDITION_ALWAYS, MSGNO(MsgFieldYayWeNeverGetPuddlesLike)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_WHO_FELL_IN_A_PUDDLE,
		CONDITION_ALWAYS, MSGNO(MsgFieldBleccchThisWatersSaaalty)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
		CONDITION_ALWAYS, MSGNO(MsgFieldImSureThoseTwoWentTo)},
	{EXAMINE_CUE(EXAMINE_WELL), METATILE_WELL, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgDailaWellSeawater))},
	{CUE_TABLE_END},
};

CUE DailaCues[] = {
	{CUE_STEP, METATILE_RIKIS_HOUSE_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaEnterDoor)},
	{CUE_STEP, METATILE_TAVIS_HOUSE_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaEnterDoor)},
	{CUE_STEP, METATILE_MIDDLE_HOUSE_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaEnterDoor)},
	{CUE_STEP, METATILE_MAYORS_HOUSE_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaEnterDoor)},
	{CUE_STEP, METATILE_INN_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaEnterDoor)},
	{CUE_STEP, METATILE_WEAPON_AND_ARMOR_SHOP_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaEnterDoor)},
	{CUE_EXIT, METATILE_SANCTUARY_DOOR, CONDITION_ALWAYS, 7},
	{CUE_EXIT, METATILE_SOUTH_ROAD, CONDITION_ALWAYS, 8},
	{CUE_EXIT, METATILE_NORTHWEST_ROAD, CONDITION_ALWAYS, 9},
	{CUE_STEP, METATILE_BEFORE_MAYORS_DOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaMeetAlex)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_NORTH_OF_PUDDLE,
		CONDITION_ALWAYS, SCRIPT_REF(scDailaJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_WEST_OF_PUDDLE,
		CONDITION_ALWAYS, SCRIPT_REF(scDailaJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_EAST_OF_PUDDLE,
		CONDITION_ALWAYS, SCRIPT_REF(scDailaJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_SOUTH_OF_PUDDLE,
		CONDITION_ALWAYS, SCRIPT_REF(scDailaJumpForward)},
	{CUE_TALK, ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE, CONDITION_ALWAYS,
		MSGNO(MsgFieldAfterThatWaveHitIWas)},
	{CUE_TALK, ACTOR_WOMAN_WHO_FELT_INDRA_MOVE, CONDITION_ALWAYS,
		MSGNO(MsgFieldSureAreALotOfTravelers)},
	{CUE_TALK, ACTOR_MAN_WHO_SAW_A_LIGHT, CONDITION_ALWAYS,
		MSGNO(MsgFieldThosePuddlesOfSeaWaterThe)},
	{CUE_TALK, ACTOR_VILLAGER_THANKFUL_FOR_THE_MOUNTAINS, CONDITION_ALWAYS,
		MSGNO(MsgFieldAhTheSeaIsCalmAgain)},
	{CUE_TALK, ACTOR_FALLEN_OLD_MAN, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaTalkHip)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_SHRINE, CONDITION_ALWAYS,
		MSGNO(MsgFieldIveAlwaysHeardThatTheShrine)},
	{CUE_TALK, ACTOR_VILLAGER_WAITING_UP_FOR_RIKI_AND_TAVI, CONDITION_ALWAYS,
		MSGNO(MsgFieldTheDreadPirateBriggsWasCaptured)},
	{CUE_TALK, ACTOR_VILLAGER_ON_RIKI_AND_TAVI, CONDITION_ALWAYS,
		MSGNO(MsgFieldThoseTwoLosersAreBackHome)},
	{CUE_TALK, ACTOR_SAILOR_WITHOUT_A_BOAT, CONDITION_ALWAYS,
		MSGNO(MsgFieldIThoughtISawSomethingWhen)},
	{CUE_TALK, ACTOR_SAILOR_ON_THE_BOATS, CONDITION_ALWAYS,
		MSGNO(MsgFieldSoIHearTheyCaughtThe)},
	{CUE_TALK, ACTOR_MAN_ON_THE_STANDING_WATER, CONDITION_ALWAYS,
		MSGNO(MsgFieldItWasCreepyRightBeforeThe)},
	{CUE_TALK, ACTOR_CHILD_WHO_LOVES_PUDDLES, CONDITION_ALWAYS,
		MSGNO(MsgFieldThePuddlesAreGoneAndNow)},
	{CUE_TALK, ACTOR_CHILD_WHO_FELL_IN_A_PUDDLE, CONDITION_ALWAYS,
		MSGNO(MsgFieldIGotHurtWhenIWas)},
	{CUE_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scDailaTaviAndRikiAreHome)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE,
		CONDITION_ALWAYS, MSGNO(MsgFieldThankOurLuckyStarsOurLittle)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WHO_FELT_INDRA_MOVE,
		CONDITION_ALWAYS, MSGNO(MsgFieldIsThatGuyStillMeetingWith)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_SAW_A_LIGHT,
		CONDITION_ALWAYS, MSGNO(MsgFieldThosePuddlesStankIThoughtId)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
		ACTOR_VILLAGER_THANKFUL_FOR_THE_MOUNTAINS, CONDITION_ALWAYS,
		MSGNO(MsgFieldImGladEverythingIsBackTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_OLD_MAN,
		CONDITION_ALWAYS, MSGNO(MsgFieldNobodyBelievesAWaveCouldBe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_SHRINE,
		CONDITION_ALWAYS, MSGNO(MsgFieldIdSureLikeToSeeWhat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
		ACTOR_VILLAGER_WAITING_UP_FOR_RIKI_AND_TAVI, CONDITION_ALWAYS,
		MSGNO(MsgFieldIWonderWhatHappenedToBriggs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_RIKI_AND_TAVI,
		CONDITION_ALWAYS, MSGNO(MsgFieldTaviAndRikiMustHaveBeen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WITHOUT_A_BOAT,
		CONDITION_ALWAYS, MSGNO(MsgFieldItsProbablyNothingButJunkAnd)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_BOATS,
		CONDITION_ALWAYS, MSGNO(MsgFieldTheSeasArePeacefulAndFree)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STANDING_WATER,
		CONDITION_ALWAYS, MSGNO(MsgFieldBeforeTheWaveHitTheTide)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_WHO_LOVES_PUDDLES,
		CONDITION_ALWAYS, MSGNO(MsgFieldIHopeSomeOfTheLittle)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_WHO_FELL_IN_A_PUDDLE,
		CONDITION_ALWAYS, MSGNO(MsgFieldICaughtMyFootOnSomething)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
		CONDITION_ALWAYS, MSGNO(MsgDailaTaviRikiPunishment)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
		METATILE_FIRST_PUDDLE, FLAG_DAILA_FIRST_PUDDLE_PARCHED,
		SCRIPT_REF(DailaParchFirstPuddle)},
/* BUG: the rows for the fourth, second and third puddles take the flags of the
   second, third and fourth, so drying the second puddle turns off Parch on the
   fourth, the third the second's, and the fourth the third's, for the rest of
   the visit. */
#if FIX_DAILA_PUDDLE_FLAGS_CROSSED
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
		METATILE_FOURTH_PUDDLE, FLAG_DAILA_FOURTH_PUDDLE_PARCHED,
		SCRIPT_REF(DailaParchFourthPuddle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
		METATILE_SECOND_PUDDLE, FLAG_DAILA_SECOND_PUDDLE_PARCHED,
		SCRIPT_REF(DailaParchSecondPuddle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
		METATILE_THIRD_PUDDLE, FLAG_DAILA_THIRD_PUDDLE_PARCHED,
		SCRIPT_REF(DailaParchThirdPuddle)},
#else
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
		METATILE_FOURTH_PUDDLE, FLAG_DAILA_SECOND_PUDDLE_PARCHED,
		SCRIPT_REF(DailaParchFourthPuddle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
		METATILE_SECOND_PUDDLE, FLAG_DAILA_THIRD_PUDDLE_PARCHED,
		SCRIPT_REF(DailaParchSecondPuddle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
		METATILE_THIRD_PUDDLE, FLAG_DAILA_FOURTH_PUDDLE_PARCHED,
		SCRIPT_REF(DailaParchThirdPuddle)},
#endif
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
		METATILE_FIFTH_PUDDLE, FLAG_DAILA_FIFTH_PUDDLE_PARCHED,
		SCRIPT_REF(DailaParchFifthPuddle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
		METATILE_SIXTH_PUDDLE, FLAG_DAILA_SIXTH_PUDDLE_PARCHED,
		SCRIPT_REF(DailaParchSixthPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE,
		FLAG_DAILA_PUDDLE_FROZEN_FIRST + 0,
		SCRIPT_REF(scFreezePuddleIntoPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE,
		FLAG_DAILA_PUDDLE_FROZEN_FIRST + 1,
		SCRIPT_REF(scFreezePuddleIntoPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE,
		FLAG_DAILA_PUDDLE_FROZEN_FIRST + 2,
		SCRIPT_REF(scFreezePuddleIntoPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FOURTH_PUDDLE,
		FLAG_DAILA_PUDDLE_FROZEN_FIRST + 3,
		SCRIPT_REF(scFreezePuddleIntoPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIFTH_PUDDLE,
		FLAG_DAILA_PUDDLE_FROZEN_FIRST + 4,
		SCRIPT_REF(scFreezePuddleIntoPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIXTH_PUDDLE,
		FLAG_DAILA_PUDDLE_FROZEN_FIRST + 5,
		SCRIPT_REF(scFreezePuddleIntoPillar)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BURIED_PSY_CRYSTAL, FLAG_DAILA_PSY_CRYSTAL_DUG,
		SCRIPT_REF(scDailaDigUpPsyCrystal)},
	{EXAMINE_CUE(EXAMINE_WELL), METATILE_WELL, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgDailaWellSeawater))},
	{CUE_TABLE_END},
};
