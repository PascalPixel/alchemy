/* @file */
/* Babi's Palace in Tolbi, and the court before its door. From the town, the */
/* party crosses the court, past two guards and a well, to the palace's double
 */
/* door. Inside, the entrance hall has Babi's statue at its north end and */
/* stairs up on either side to the two doors of the big bedroom, where guests */
/* and Colosso's warriors sleep in four beds. From the hall a passage runs */
/* north between two doors, the soldiers' room's and the room west of it, to a
 */
/* crossroads with a red carpet. Its halls lead west and east to the stairs */
/* down to Babi's scholars, to the tunnel into the colosseum's stands and to */
/* the landing outside the room where Sheba is kept, and north to Babi's */
/* throne room, with Babi's own bedroom beyond. */
/* */
/* While Babi is missing, the attendant in the big bedroom offers the party the
 */
/* beds, and walking up the passage to the red carpet the party overhears Iodem
 */
/* and two soldiers back from Altmiller Cave, where Babi has slipped away */
/* before. Sheba, held hostage so that Lalivero builds Babi's lighthouse, */
/* wonders when she will see her homeland, and notices the first Mind Read on */
/* her. */
/* */
/* Once the party has saved Babi in Altmiller Cave the finals of Colosso are */
/* set: a guard outside passes on Babi's word that he waits at the colosseum, */
/* and the attendant offers the beds again. A match lost in the finals, or the
 */
/* last one fought, brings the party back to wake in the big bedroom. After */
/* Colosso, Babi and Iodem wait in the throne room: Babi gives the Lure Cap to
 */
/* the winner, tells of Lemuria and its draught, asks the party to sail there */
/* for him once Venus Lighthouse is dealt with, and sends Iodem to meet them at
 */
/* Gondowan Passage. In Babi's bedroom afterwards the Cloak Ball on show is his
 */
/* to lend. */
/* */
/* The code: the court (SceneId_BabiPalaceEntrance) and the palace's rooms on */
/* one map (SceneId_BabiPalace) share this overlay and one exit table, and each
 */
/* chooses one of three actor and trigger tables by FLAG_BABI_SAVED and */
/* FLAG_COLOSSO_OVER. The exits name the rooms by what each shows: the statue */
/* room is the entrance hall, the carpet room the crossroads, the meeting room
 */
/* the throne room, the oven room the room west of the soldiers' room, with its
 */
/* stove, and the round rooms the small eight-sided rooms up the stairs. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event_inline.h"
#include "graphics/display_transition.h"
#include "graphics/graphics.h"
#include "scene/walk_into_leader.h"
#include "sdk/sound_driver_symbols.h"
#include "text/msg_ids.h"

extern u16 BabiPalace_DoorCells[];
extern s32 BabiPalace_IndigestionScript[];
extern SCENE_ENTRANCE BabiPalace_Entrances[];
extern SCENE_ENTRANCE BabiPalace_OutsideEntrances[];
extern SCENE_EXIT_REGION BabiPalace_Regions[];
extern u32 BabiPalace_Exits[];
extern ACTOR_PLACE BabiPalace_OutsideActorPlacements[];
extern ACTOR_PLACE BabiPalace_ActorPlacementsBabiMissing[];
extern ACTOR_PLACE BabiPalace_ActorPlacementsFinals[];
extern ACTOR_PLACE BabiPalace_ActorPlacementsAfterColosso[];
extern TRIGGER BabiPalace_TriggersBabiMissing[];
extern TRIGGER BabiPalace_TriggersFinals[];
extern TRIGGER BabiPalace_TriggersAfterColosso[];
extern TRIGGER BabiPalace_OutsideTriggersBabiMissing[];
extern TRIGGER BabiPalace_OutsideTriggersFinals[];
extern TRIGGER BabiPalace_OutsideTriggersAfterColosso[];

/* The palace's actors are placed by one of three tables, as the story goes on.
 */
/* */
/* The three are before Babi is rescued */
/* (BabiPalace_ActorPlacementsBabiMissing), during the finals */
/* (BabiPalace_ActorPlacementsFinals) and after Colosso */
/* (BabiPalace_ActorPlacementsAfterColosso). */
/* */
/* Each numbers its rows from ACTOR_FIRST_PLACED, with Garet, Ivan, Mia and */
/* Sheba in their own party slots. Most of the people stand in all three under
 */
/* other numbers. */
/* */
/* The palace's residents, three old men with canes and a young man with brown
 */
/* hair, stand in all three. One sent Babi's soldiers to search the town, and */
/* another tells warriors who are not entering Colosso that they may not stay.
 */
/* The game names none of them. */
/* */
/* Before the rescue there are the attendant who shows the party its beds, */
/* Iodem and the two soldiers he sent to Altmiller Cave. Sheba is held here */
/* until Colosso is over. */
typedef enum BABI_PALACE_ACTOR_ID {
	/* An old man with a cane, in the same room and at the same number in every
	 */
	/* table. He is the one who sent Babi's soldiers to search the town. */
	/* */
	/* He asks "Did you run into any of Babi's soldiers in Town?", and if not,
	 */
	/* says "Hmph! I told them to search every nook and cranny of this town..."
	 */
	ACTOR_ASKING_ABOUT_BABIS_SOLDIERS = ACTOR_FIRST_PLACED,
	/* The attendant, a woman with fair hair, whose number is the same after */
	/* Colosso: "No one's going to sleep in this big bedroom until the next */
	/* Colosso!" */
	ACTOR_BED_ATTENDANT,
	/* A warrior in a golden helmet. "So, did you guys get injured, too?" */
	/* Thoughts: "I hurt my back during my training, and now I can't move at */
	/* all." */
	ACTOR_INJURED_WARRIOR,
	/* A warrior in a golden helmet. "I can't fight because of a little */
	/* indigestion? That's pathetic!" */
	ACTOR_WARRIOR_WITH_INDIGESTION,
	/* A young man with brown hair in a purple shirt. "If you're entering */
	/* Colosso, hurry along to the colosseum." Thoughts: "Warriors are not */
	/* allowed to stay in the palace if they don't enter Colosso!" */
	ACTOR_HURRYING_WARRIORS_ALONG,
	/* Four of Babi's soldiers, in one room. The first says "Only warriors who
	 */
	/* fight well in Colosso's finals can become soldiers here." */
	ACTOR_FIRST_PALACE_SOLDIER,
	/* An old man with a cane beside Babi's bed. "It looks like Lord Babi didn't
	 */
	/* use his bed this morning." */
	ACTOR_BY_BABIS_BED = ACTOR_FIRST_PALACE_SOLDIER + 4,
	/* An old man with a cane. "Sheba has been staying as Lord Babi's guest for
	 */
	/* some time." Thoughts: "It is heartbreaking to see her tears and hear her
	 */
	/* pleas to be released." */
	ACTOR_SORRY_FOR_SHEBA,
	/* In Sheba's room, in SPRITE_OLD_WOMAN, as in vale_plaza.c. Thoughts:
	   "Sheba */
	/* is not permitted to leave this room." */
	ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM,
	ACTOR_IODEM,
	ACTOR_FIRST_SOLDIER,
	ACTOR_SECOND_SOLDIER
} BABI_PALACE_ACTOR_ID;

/* During the finals, ACTOR_ASKING_ABOUT_BABIS_SOLDIERS keeps its number, and */
/* the others stand under new ones. */
typedef enum BABI_PALACE_FINALS_ACTOR_ID {
	/* The same resident as ACTOR_HURRYING_WARRIORS_ALONG: "What are you doing?
	 */
	/* Hurry over to the colosseum!" */
	ACTOR_FINALS_HURRYING_WARRIORS_ALONG = ACTOR_FIRST_PLACED + 1,
	/* The same attendant, a number later. */
	ACTOR_FINALS_ATTENDANT,
	/* The same resident as ACTOR_BY_BABIS_BED: "Babi has returned and the */
	/* finals are set to begin!" */
	ACTOR_FINALS_BY_BABIS_BED,
	/* The same resident as ACTOR_SORRY_FOR_SHEBA: "What's going on? Sheba is */
	/* looking much happier." */
	ACTOR_FINALS_SORRY_FOR_SHEBA,
	/* The same woman as ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM: "The soldiers came to
	 */
	/* see Sheba. She's been in a great mood ever since." */
	ACTOR_FINALS_OLD_WOMAN_IN_SHEBAS_ROOM
} BABI_PALACE_FINALS_ACTOR_ID;

/* After Colosso there are Babi and the Cloak Ball he keeps on show. */
/* ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, ACTOR_BED_ATTENDANT and Iodem keep their
 */
/* numbers. */
typedef enum BABI_PALACE_AFTER_COLOSSO_ACTOR_ID {
	/* One of Babi's soldiers. */
	ACTOR_RELIEVED_SOLDIER = ACTOR_FIRST_PLACED + 2,
	/* The same resident as ACTOR_HURRYING_WARRIORS_ALONG: "So, you're all from
	 */
	/* the eastern shores of the Karagol Sea, huh?" */
	ACTOR_ASKING_ABOUT_THE_EASTERN_SHORES,
	/* One of Babi's soldiers. */
	ACTOR_GONDOWAN_BOUND_SOLDIER,
	/* "The southern continent is a dangerous place... you'll know why I prefer
	 */
	/* Angara." */
	ACTOR_SOLDIER_PREFERRING_ANGARA,
	ACTOR_BABI,
	/* The same resident as ACTOR_BY_BABIS_BED: "Babi's face is very pale, */
	/* like that of a dead man." */
	ACTOR_AFTER_COLOSSO_BY_BABIS_BED,
	ACTOR_CLOAK_BALL,
	/* The same resident as ACTOR_SORRY_FOR_SHEBA: "The girl from Lalivero is */
	/* no longer here." */
	ACTOR_AFTER_COLOSSO_SORRY_FOR_SHEBA,
	/* The same woman as ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM: "I am surprised Sheba
	 */
	/* was allowed to return to Lalivero like that." */
	ACTOR_AFTER_COLOSSO_OLD_WOMAN_IN_SHEBAS_ROOM
} BABI_PALACE_AFTER_COLOSSO_ACTOR_ID;

/* The two guards outside the palace (BabiPalaceEntrance). */
enum { ACTOR_FIRST_GUARD = 8, ACTOR_SECOND_GUARD = 9 };

/* The attendant has led the party to the beds. Talking again sends the */
/* attendant back. */
enum { FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS = FLAG_SCENE_TEMP_00 };

/* The palace map's tileset animations that open its doors, for */
/* fdTilesetAnimResume (play) and fdTilesetAnimPause (pause). */
/* */
/* Each scene entry pauses them, so every door is drawn shut. The recording of
 */
/* scBabiPalaceLeaveThroughDoor shows one swing open */
/* (https://alchemy.pascalpixel.com/places/babi-palace/events/scBabiPalaceLeaveThroughDoor.gif).
 */
typedef enum BABI_PALACE_DOOR_ANIMATION {
	BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION = 1, /* the throne room's door */
	BABI_PALACE_OTHER_DOORS_ANIMATION = 2, /* every door without its own */
	BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION = 4
} BABI_PALACE_DOOR_ANIMATION;

/* The leader's animations in the big bedroom's bed, after the four shared ones
 */
/* (ACTOR_ANIMATION). */
/* */
/* The nod and the shake of the head answer yes and no in */
/* scBabiPalaceWakeAfterFinals. */
typedef enum BABI_PALACE_BED_ANIMATION {
	ANIM_IN_BED_LYING = 31,
	/* Sitting up. The leader is raised by 3 pixels as it starts. */
	ANIM_IN_BED_SITTING_UP = 32,
	ANIM_IN_BED_NOD = 33,
	ANIM_IN_BED_SHAKE_HEAD = 34
} BABI_PALACE_BED_ANIMATION;

/* Cell behaviors the triggers answer besides the exits, whose cell */
/* behaviors are their exits' numbers. */
typedef enum BABI_PALACE_CELL_BEHAVIOR {
	/* The passage up to the red carpet, cells (32, 26) and (33, 26), where the
	 */
	/* party overhears Iodem while Babi is missing. */
	CELL_BEHAVIOR_BABI_PALACE_PASSAGE_TO_CARPET = 60,
	/* The big bedroom's four beds, searched to sleep. */
	CELL_BEHAVIOR_BABI_PALACE_BEDS = 70,
	/* The well in the court, south-west of the palace door. */
	CELL_BEHAVIOR_BABI_PALACE_WELL = 90,
	/* The barrel in the eight-sided room up the east stairs, cell (34, 61), */
	/* with a Lucky Medal in it. */
	CELL_BEHAVIOR_BABI_PALACE_LUCKY_MEDAL_BARREL = 100,
	/* The shelf at the north wall of the big bedroom, cells (24, 3) and (25,
	   3): */
	/* "The armor looks heavy." */
	CELL_BEHAVIOR_BABI_PALACE_ARMOR_SHELF = 200,
	/* The shelf in the room west of the soldiers' room, (9, 18) and (10, 18):
	 */
	/* "There's nothing in there." */
	CELL_BEHAVIOR_BABI_PALACE_EMPTY_SHELF = 201,
	/* The shelf in the soldiers' room, (55, 18) and (56, 18), with a headband
	 */
	/* on it. */
	CELL_BEHAVIOR_BABI_PALACE_HEADBAND_SHELF = 202,
	/* The big bedroom's bookcase, (6, 3) and (7, 3): "The Heart of the..." */
	CELL_BEHAVIOR_BABI_PALACE_BIG_BEDROOM_BOOKCASE = 203,
	/* The bookcase in the room west of the soldiers' room, (12, 18) and (13, */
	/* 18), with a notebook of records. */
	CELL_BEHAVIOR_BABI_PALACE_NOTEBOOK_BOOKCASE = 204,
	/* The soldiers' room's bookcase, (58, 18) and (59, 18): "Become a Macho Man
	 */
	/* in..." */
	CELL_BEHAVIOR_BABI_PALACE_SOLDIERS_BOOKCASE = 205,
	/* The bookcase in Babi's bedroom, (58, 33) and (59, 33), with Babi's diary.
	 */
	CELL_BEHAVIOR_BABI_PALACE_DIARY_BOOKCASE = 206
} BABI_PALACE_CELL_BEHAVIOR;

/* Pauses the tileset animations of the palace's doors as the leader leaves. */
static inline void scBabiPalacePauseDoorAnimations(void) {
	fdTilesetAnimPause(BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION);
	fdTilesetAnimPause(BABI_PALACE_OTHER_DOORS_ANIMATION);
	fdTilesetAnimPause(BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION);
}

/* After a night in the palace's beds, the party's HP and PP are full, Garet, */
/* Ivan and Mia rejoin, and the inventory Colosso held is returned. */
static inline void scBabiPalaceRestorePartyAndInventory(void) {
	s16 members[8];
	UNIT *state;
	s32 memberCount;
	s32 i;

	memberCount = syFieldPartyListMembers(members);
	for(i = 0; i < memberCount; i++) {
		state = gmUnitGet(members[i]);
		state->hp = state->hpMax;
		state->pp = state->ppMax;
		gmUnitRecalculateGauges(members[i]);
	}
	gmPartyAddMember(GARET);
	gmPartyAddMember(IVAN);
	gmPartyAddMember(MIA);
	gmInventorySnapshotRestore();
}

const SCENE_ENTRANCE *scGetEntrances(void) {
	if(MainRoutineStateWork.next == SCENE_BABI_PALACE_ENTRANCE) {
		return BabiPalace_OutsideEntrances;
	}
	return BabiPalace_Entrances;
}

/* The palace has no exit regions, so the table is empty. */
const SCENE_EXIT_REGION *scGetExitRegions(void) { return BabiPalace_Regions; }

/* Both scenes share one exit table. */
const u32 *scGetExits(void) { return BabiPalace_Exits; }

/* Returns the actor placements for the scene and the story, which is before */
/* Babi's rescue, during the finals or after Colosso. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	if(MainRoutineStateWork.next == SCENE_BABI_PALACE_ENTRANCE) {
		return BabiPalace_OutsideActorPlacements;
	}
	if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
		return BabiPalace_ActorPlacementsAfterColosso;
	}
	if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
		return BabiPalace_ActorPlacementsFinals;
	}
	return BabiPalace_ActorPlacementsBabiMissing;
}

/* Asks a yes or no question. The answers are the two messages after it. */
static inline void scBabiPalaceShowQuestionAnswer(s32 actorId, s32 question) {
	fdEventSetMessageId(question);
	fdEventOpenMessage(actorId, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
		fdEventSetMessageId(question + 1);
	else
		fdEventSetMessageId(question + 2);
	fdEventShowMessage(actorId, 0);
}

/* While Babi is missing, the old man with a cane in the entrance hall asks */
/* "Did you run into any of Babi's soldiers in Town?"; No gets him grumbling */
/* that he told them to search every nook and cranny. */
static void scBabiPalaceAskAboutSoldiers(s32 actorId) {
	scBabiPalaceShowQuestionAnswer(actorId, MsgTolbiRunBabisSoldiers);
}

/* While Babi is missing, the attendant in the big bedroom, the woman with */
/* fair hair by its west wall, says there are just enough beds for everybody */
/* and walks over to them. Talking to her there, she offers the four beds */
/* ("Will you be going right to sleep?"), and from then on a search of a bed */
/* lets the party sleep in it. */
static void scBabiPalaceShowBeds(s32 actorId) {
	s16 roundedFacing;
	s32 facingOrMessage;

	roundedFacing =
		FACING_NEAREST_CARDINAL(fdActorGet(ACTOR_PARTY_LEADER)->facing);
	fdEventStart();
	fdEndBusyAndReveal();
	if(fdEventGetFlag(FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS) == 0) {
		gmFlagSet(FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS);
		gmFlagClear(FLAG_BABI_PALACE_BEDS_OPEN);
		fdEventSetMessageId(MsgTolbiWeHaveJustEnoughExtra);
		/* "We have just enough extra beds in this room for everybody...." */
		fdEventShowMessage(actorId, 0);
		fdEventWait(10);
		facingOrMessage = FACING_SOUTH;
		if((u16)roundedFacing == FACING_SOUTH) {
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 40, 104);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		}
		fdActorSetSpeed(actorId, 1);
		fdActorWalkByAndWait(actorId, 0, -48);
		fdActorWalkByAndWait(actorId, 64, 0);
		fdActorFaceDirection(actorId, facingOrMessage, 0);
	}
	else {
		gmFlagClear(FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS);
		gmFlagSet(FLAG_BABI_PALACE_BEDS_OPEN);
		fdActorFaceDirection(actorId, FACING_SOUTH, 0);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdEventWait(20);
		facingOrMessage = MsgTolbiUseFourBeds;
		fdEventSetMessageId(facingOrMessage);
		/* "You may use these four beds. Will you be going right to sleep?" */
		fdEventOpenMessage(actorId, 0);
		if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			fdEventSetMessageId(facingOrMessage + 1);
			/* "Then, enjoy your rest." */
			fdEventShowMessage(actorId, 0);
		}
		else {
			fdEventSetMessageId(facingOrMessage + 2);
			/* "Well, if you do get tired, feel free to sleep here anytime..."
			 */
			fdEventShowMessage(actorId, 0);
		}
		fdEventWait(10);
		fdActorSetAnimationAndWait(actorId, ANIM_NOD);
		fdEventWait(20);
		fdActorWalkByAndWait(actorId, -64, 0);
		fdActorWalkByAndWait(actorId, 0, 48);
#if TBS_PATCH_3
		fdActorSetPos(actorId, 56, 120);
#endif
	}
	fdEventEnd();
}

/* During the finals, the attendant in the big bedroom asks whether the party */
/* came to rest before its match, and the beds are open to it again. */
static void scBabiPalaceOfferRest(s32 actorId) {
	s32 question;

	fdEventStart();
	fdEndBusyAndReveal();
	if(fdEventGetFlag(FLAG_BABI_PALACE_FINALS_REST_OFFERED) == 0) {
		gmFlagSet(FLAG_BABI_PALACE_FINALS_REST_OFFERED);
		gmFlagSet(FLAG_BABI_PALACE_FINALS_BEDS_OPEN);
		fdActorFaceDirection(actorId, FACING_SOUTH, 0);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdEventWait(20);
		question = MsgTolbiCameRestBefore;
		fdEventSetMessageId(question);
		/* "You all came to rest up before the match, didn't you? Would..." */
		fdEventOpenMessage(actorId, 0);
		if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			fdEventWait(10);
			fdEventSetMessageId(question + 1);
		}
		else {
			fdEventSetMessageId(question + 2);
		}
		fdEventShowMessage(actorId, 0);
		fdEventWait(10);
		fdActorSetAnimationAndWait(actorId, ANIM_NOD);
		fdEventWait(20);
		fdActorSetSpeed(actorId, 1);
		fdActorWalkByAndWait(actorId, -64, 0);
		fdActorWalkByAndWait(actorId, 0, 48);
	}
	else {
		fdEventSetMessageId(MsgTolbiTiredFeelFreeRest);
		/* "Well, if you're tired, please feel free to get some rest." */
		fdEventOpenMessage(actorId, 0);
	}
	fdEventEnd();
}

/* While Babi is missing, the warrior in a golden helmet by the table at the */
/* north of the big bedroom asks "Are you planning to enter Colosso?". A Yes */
/* makes him wince and send the party to the colosseum; a No has him take it */
/* for another injured warrior. */
static void scBabiPalaceAskAboutColosso(s32 actorId) {
	s32 question = MsgTolbiPlanningEnterColosso;

	fdEventSetMessageId(question);
	/* "Are you planning to enter Colosso?" */
	fdEventOpenMessage(actorId, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(10);
		fdActorShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		fdEventSetMessageId(question + 1);
	}
	else {
		fdEventWait(10);
		fdActorShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
		fdEventSetMessageId(question + 2);
	}
	fdEventShowMessage(actorId, 0);
}

/* While Babi is missing, the warrior in a golden helmet by the big bedroom's */
/* east wall tries to shrug off his indigestion to enter Colosso, and */
/* gives up: "It's no use! My stomach really hurts!" */
static void scBabiPalaceTalkToWarriorWithIndigestion(s32 actorId) {
	ACTOR *actor;
	const s32 *indigestionScript;

	actor = fdActorGet(actorId);
	fdEventStart();
	indigestionScript = BabiPalace_IndigestionScript;
	fdActorSetScript(actorId, indigestionScript);
	fdEventSetMessageId(MsgTolbiCantFightBecauseLittleIndigestion);
	/* "I can't fight because of a little indigestion? That's pathetic!" */
	fdEventShowMessage(actorId, 0);
	fdActorStop(actorId);
	actor->scaleY = FIXED_ONE;
	actor->scaleX = FIXED_ONE;
	fdEventWait(30);
	fdActorRunRepeatedMotion(actorId, 2);
	fdEventWait(30);
	fdActorRunRepeatedMotion(actorId, 2);
	fdEventWait(60);
	/* "Maybe I can shrug off this pain in my gut... I have to enter..." */
	fdEventShowMessage(actorId, 0);
	fdEventWait(20);
	fdActorShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorRunRepeatedMotion(actorId, 2);
	fdEventWait(30);
	fdActorRunRepeatedMotion(actorId, 2);
	fdEventWait(30);
	fdActorRunRepeatedMotion(actorId, 2);
	fdEventWait(30);
	fdActorSetScript(actorId, indigestionScript);
	/* "It's no use! My stomach really hurts!" */
	fdEventShowMessage(actorId, 0);
	fdActorFaceDirection(actorId, FACING_NORTHEAST, 0);
	fdEventWait(10);
	actor->scaleY = FIXED_ONE;
	actor->scaleX = FIXED_ONE;
	fdActorSetScript(actorId, indigestionScript);
	fdEventEnd();
}

/* Mind Read on Sheba in her room, while Babi is missing, reads that even if */
/* she escaped the palace her power could not get her home to Lalivero. The */
/* first time, she turns with a start ("Did you just..."), then shakes her */
/* head: "No, it is nothing." */
static void scBabiPalaceReadShebasMind(s32 casterId, s32 actorId) {
	fdEventStart();
	fdEventSetMessageId(MsgTolbiEvenIfEscapedBabiPalace);
	/* "Even if I escaped Babi Palace, my power could not get me to..." */
	fdEventShowMessage(actorId, 0);
	if(fdEventGetFlag(FLAG_SHEBA_NOTICED_MIND_READ) == 0) {
		gmFlagSet(FLAG_SHEBA_NOTICED_MIND_READ);
		syPsynergyCancel();
		fdEventWait(50);
		fdActorShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
		fdActorFaceActor(actorId, ACTOR_PARTY_LEADER, 40);
		/* "Did you just..." */
		fdEventShowMessage(actorId, 0);
		fdEventWait(30);
		fdActorSetAnimationAndWait(actorId, ANIM_SHAKE_HEAD);
		fdEventWait(20);
		/* "No, it is nothing." */
		fdEventShowMessage(actorId, 0);
		fdActorFaceDirection(actorId, FACING_WEST, 0);
	}
	fdEventEnd();
}

/* After Colosso, the old man with a cane in the entrance hall asks whether the
 */
/* party has met with Babi. */
static void scBabiPalaceAskAboutMeetingBabi(s32 actorId) {
	scBabiPalaceShowQuestionAnswer(actorId, MsgTolbiMeetBabi);
}

/* After Colosso, the young man with brown hair in the room west of the */
/* soldiers' room asks "So, you're all from the eastern shores of the Karagol */
/* Sea, huh?". */
static void scBabiPalaceAskAboutEasternShores(s32 actorId) {
	scBabiPalaceShowQuestionAnswer(actorId, MsgTolbiEasternShoresKaragol);
}

/* After Colosso, picking up the Cloak Ball on show beside Babi's bed: "You */
/* found the Cloak Ball." From his bed Babi asks whether Isaac desires it, and
 */
/* on a Yes lends it for the quest. On a No he asks to leave it alone. */
static void scBabiPalaceOfferCloakBall(void) {
	s32 msg;

	fdEventStart();
	fdEndBusyAndReveal();
	msg = MsgTolbiFoundCloakBall;
	fdEventSetMessageId(msg);
	/* "You found the Cloak Ball." */
	fdEventShowMessage(-1, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(30);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_BABI, 30);
	/* "Isaac! Is that Cloak Ball what you desire?" */
	fdEventOpenMessage(ACTOR_BABI, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
		fdEventSetMessageId(msg + 2);
		/* "Then please leave the Cloak Ball alone. Do not worry about..." */
		fdEventShowMessage(ACTOR_BABI, 0);
	}
	else {
		fdEventWait(20);
		fdEventSetMessageId(msg + 3);
		/* "That Cloak Ball is very precious to me. I can't just give it..." */
		fdEventShowMessage(ACTOR_BABI, 0);
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdEventWait(30);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		fdEventWait(30);
		fdActorSetPos(ACTOR_CLOAK_BALL, 0, 0);
		gmItemShowFound(ITEM_CLOAK_BALL, 3);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
		gmPartyGiveItem(ITEM_CLOAK_BALL, 0);
		gmFlagSet(FLAG_BABI_PALACE_CLOAK_BALL);
	}
}

/* Losing the first or second match of the finals, Isaac wakes in a bed of the
 */
/* big bedroom: he only dreamt of fighting in them. The others tease him, and */
/* the party sets off for the colosseum. */
static void scBabiPalaceWakeFromFinalsDream(void) {
	fdEventStart();
	fdEventSetMessageId(MsgTolbiGetUp);
	fdActorGet(ACTOR_PARTY_LEADER)->sprStorageKind = ACTOR_SPRITE_NONE;
	fdActorGet(ACTOR_FINALS_ATTENDANT)->sprStorageKind = ACTOR_SPRITE_NONE;
	sySynch(1);
	REG_DISPCNT = DISPCNT_OBJ_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
	/* "Isaac! Get up, Isaac!" */
	fdEventShowMessage(-1, 0);
	REG_DISPCNT = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
	fdActorGet(ACTOR_PARTY_LEADER)->sprStorageKind = ACTOR_SPRITE_ONE;
	fdActorGet(ACTOR_FINALS_ATTENDANT)->sprStorageKind = ACTOR_SPRITE_ONE;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_LYING);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetPos(ACTOR_GARET, 120, 104);
	fdActorSetPos(ACTOR_MIA, 104, 80);
	fdActorSetPos(ACTOR_IVAN, 120, 120);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	FieldStateWork->transitionFrm = 60;
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	FieldStateWork->transitionFrm = 24;
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorWalkByAndWait(ACTOR_MIA, 16, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdEventWait(30);
	/* "Are you okay? You were having nightmares for quite a while..." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorGet(ACTOR_PARTY_LEADER)->z.fixed -= toFIXED(3);
	fdActorGet(ACTOR_PARTY_LEADER)->tgtZ -= toFIXED(3);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_SITTING_UP);
	fdEventWait(40);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
	fdEventWait(30);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
	fdEventWait(50);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(30);
	/* "What's the matter, Isaac?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* "What? You entered and fought in the finals?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "How could you? You haven't even been to the colosseum yet,..." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "Sounds like you've been dreaming, Isaac." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(45);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdEventWait(30);
	/* "You're so exhausted, it's giving you nightmares. Are you..." */
	fdEventOpenMessage(ACTOR_GARET, 0);
	if(fdEventAskYesNoWithActorAnimation(-1, 0) != 0) {
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
		fdEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdEventWait(20);
		/* "You think you can manage? I understand, but don't overdo it,..." */
		fdEventShowMessage(ACTOR_GARET, 0);
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
		fdEventWait(30);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdEventWait(20);
		/* "Isaac's awake now, so let's head over to the colosseum." */
		fdEventShowMessage(ACTOR_GARET, 0);
		FieldStateWork->msg += 1;
	}
	else {
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
		FieldStateWork->msg += 2;
		fdEventWait(30);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdEventWait(20);
		/* "Okay! We're off to the colosseum!" */
		fdEventShowMessage(ACTOR_GARET, 0);
	}
	fdEventWait(10);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorWalkByAndWait(ACTOR_GARET, -16, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdEventWait(35);
	fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.9);
	fdActorWalkByAndWait(ACTOR_PARTY_LEADER, -32, 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(40);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(30);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(30);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorSetSpeed(ACTOR_MIA, 1.2);
	fdActorSetSpeed(ACTOR_IVAN, 1.2);
	scWalkIntoLeader(ACTOR_GARET);
	scWalkIntoLeader(ACTOR_MIA);
	scWalkIntoLeader(ACTOR_IVAN);
	fdEventWait(10);
	fdEventEnd();
}

/* Searching one of the four beds in the big bedroom, once the attendant has */
/* offered them, asks "Would you like to sleep now?". The first night the */
/* party talks things over before it sleeps; later nights it sleeps as at an */
/* inn. */
static void scBabiPalaceRestInPalaceBeds(void) {
	s16 roundedFacing;
	u16 facing;

	roundedFacing =
		FACING_NEAREST_CARDINAL(fdActorGet(ACTOR_PARTY_LEADER)->facing);
	fdEventStart();
	fdEndBusyAndReveal();
	fdEventSetMessageId(MsgTolbiLikeSleep);
	/* "Would you like to sleep now?" */
	fdEventOpenMessage(-1, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		if(fdEventGetFlag(FLAG_BABI_PALACE_FIRST_NIGHT) != 0) {
			fdEventWait(20);
			mnInnPlaySleep(0);
			return;
		}
		fdEventWait(20);
		facing = roundedFacing;
		if(facing == FACING_EAST) {
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 128, 120);
		}
		if(facing == FACING_WEST) {
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 240, 120);
		}
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 184, 120);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdEventWait(10);
		syFieldActorLaunchFromLeader(ACTOR_GARET, 16, 0, FACING_WEST);
		fdActorWaitForScriptEndById(ACTOR_GARET);
		fdEventWait(10);
		/* "Ohh, I'm so tired..." */
		fdEventShowMessage(ACTOR_GARET, 0);
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdEventWait(20);
		fdActorSetSpeed(ACTOR_GARET, 1);
		fdActorWalkByAndWait(ACTOR_GARET, 40, 0);
		fdActorWalkByAndWait(ACTOR_GARET, 0, -32);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
		fdEventWait(20);
		syFieldActorLaunchFromLeader(ACTOR_IVAN, -16, 0, FACING_EAST);
		fdActorWaitForScriptEndById(ACTOR_IVAN);
		fdEventWait(10);
		/* "I didn't think we'd ever find a place to stay. What a relief!" */
		fdEventShowMessage(ACTOR_IVAN, 0);
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdEventWait(20);
		fdActorSetSpeed(ACTOR_IVAN, 1);
		fdActorWalkByAndWait(ACTOR_IVAN, -40, 0);
		fdActorWalkByAndWait(ACTOR_IVAN, 0, 40);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdEventWait(20);
		syFieldActorLaunchFromLeader(ACTOR_MIA, 16, 0, FACING_WEST);
		fdActorWaitForScriptEndById(ACTOR_MIA);
		fdEventWait(10);
		/* "Let's rest up for tomorrow." */
		fdEventShowMessage(ACTOR_MIA, 0);
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdEventWait(20);
		fdActorSetSpeed(ACTOR_MIA, 1);
		fdActorWalkByAndWait(ACTOR_MIA, 40, 0);
		fdActorWalkByAndWait(ACTOR_MIA, 0, 40);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
		fdEventWait(20);
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdActorWalkByAndWait(ACTOR_PARTY_LEADER, -56, 0);
		fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, -32);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
		fdEventWait(30);
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdEventWait(10);
		/* "Good night." */
		fdEventShowMessage(ACTOR_MIA, 0);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdEventWait(10);
		if(gmFlagGet(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
			/* "G'night." */
			fdEventShowMessage(ACTOR_GARET, 0);
		}
		else {
			FieldStateWork->msg++;
		}
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdEventWait(10);
		if(gmFlagGet(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
			/* "Sleep well." */
			fdEventShowMessage(ACTOR_IVAN, 0);
		}
		else {
			FieldStateWork->msg++;
		}
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
		mnInnPlaySleep(0);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
		fdEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdEventWait(10);
		/* "Morning." */
		fdEventShowMessage(ACTOR_GARET, 0);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdEventWait(10);
		if(gmFlagGet(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
			/* "Good morning." */
			fdEventShowMessage(ACTOR_IVAN, 0);
		}
		else {
			FieldStateWork->msg++;
		}
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdEventWait(10);
		if(gmFlagGet(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
			/* "Good morning." */
			fdEventShowMessage(ACTOR_MIA, 0);
		}
		else {
			FieldStateWork->msg++;
		}
		fdActorWalkByAndWait(ACTOR_GARET, 0, 32);
		fdActorWalkBy(ACTOR_GARET, -112, 0);
		fdActorWalkByAndWait(ACTOR_MIA, 0, -40);
		fdActorWalkBy(ACTOR_MIA, -112, 0);
		fdEventWait(50);
		fdActorWalkByAndWait(ACTOR_IVAN, 0, -24);
		fdActorWaitForMoveById(ACTOR_GARET);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		fdActorWalkByAndWait(ACTOR_GARET, 0, -16);
		fdActorWaitForMoveById(ACTOR_MIA);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
		fdEventWait(20);
		/* "All right, Isaac! We've got another hard day of work ahead..." */
		fdEventShowMessage(ACTOR_GARET, 0);
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdEventWait(20);
		fdActorSetSpeed(ACTOR_GARET, 1.2);
		fdActorSetSpeed(ACTOR_IVAN, 1.2);
		fdActorSetSpeed(ACTOR_MIA, 1.2);
		scWalkIntoLeader(ACTOR_GARET);
		scWalkIntoLeader(ACTOR_MIA);
		scWalkIntoLeader(ACTOR_IVAN);
		gmFlagSet(FLAG_BABI_PALACE_FIRST_NIGHT);
	}
	fdEventEnd();
}

/* While Babi is missing, walking north up the passage towards the red carpet,
 */
/* cell (32, 26) or (33, 26), Isaac stops short of the carpet as Iodem walks in
 */
/* from the west hall and two soldiers hurry up to him, back from Altmiller */
/* Cave: Babi has been gone since last night, and he has slipped into that cave
 */
/* before. Iodem then leaves for the colosseum in Babi's stead. It plays once.
 */
static void scBabiPalaceHearSoldiersReport(void) {
	gmFlagSet(FLAG_IODEM_HEARD_SOLDIERS);
	fdEventStart();
	fdEndBusyAndReveal();
	fdEventSetMessageId(MsgTolbiIodemIodem);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 520, 424);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdCameraMoveTo(560, CAMERA_KEEP, 368, 1);
	fdCameraWaitForMove();
	fdActorSetSpeed(ACTOR_IODEM, 1);
	fdActorWalkByAndWait(ACTOR_IODEM, 40, 0);
	fdActorWalkToAndWait(ACTOR_IODEM, 584, 360);
	/* "Iodem, Iodem!" */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_IODEM, FACING_WEST, 0);
	fdActorSetSpeed(ACTOR_FIRST_SOLDIER, 2);
	fdActorSetSpeed(ACTOR_SECOND_SOLDIER, 2);
	fdActorWalkTo(ACTOR_FIRST_SOLDIER, 528, 352);
	fdActorWalkToAndWait(ACTOR_SECOND_SOLDIER, 528, 368);
	sySynch(3);
	fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_STAND);
	fdEventWait(30);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdEventWait(30);
	/* "We went to Altmiller Cave, as you commanded." */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	fdActorWalkByAndWait(ACTOR_IODEM, -16, 0);
	fdEventWait(10);
	/* "And what have you? What news is there of Babi?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "This is futile..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorSetSpeed(ACTOR_FIRST_SOLDIER, 1);
	fdActorWalkByAndWait(ACTOR_FIRST_SOLDIER, 8, 0);
	fdEventWait(20);
	/* "Iodem, why did you send us to look for Babi in a place like..." */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorSetSpeed(ACTOR_SECOND_SOLDIER, 1);
	fdActorWalkByAndWait(ACTOR_SECOND_SOLDIER, 8, 0);
	fdEventWait(20);
	/* "Why would Lord Babi enter such a dangerous cave alone? It's..." */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "Inconceivable? Is that what you were going to say?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdEventWait(20);
	/* "Altmiller Cave is an evil place." */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "If Babi went there alone, a monster might have..." */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "I'm aware of Altmiller Cave's...eccentricities." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "You knew? Then why would you send us in there, Iodem?" */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Lord Babi has left in secret before..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
	fdEventWait(20);
	/* "You mean, this isn't the first time Lord Babi has..." */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdEventWait(20);
	/* "Do you know where he went before?" */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "I followed Lord Babi once..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(20);
	fdActorFaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
	fdActorFaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
	fdActorFaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
	fdEventWait(20);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "I was certain he had gone into Altmiller Cave..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 40);
	/* "Did you ever investigate the cave, Iodem?" */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdEventWait(30);
	/* "What was he doing in the cave?" */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAttachedEffect(ACTOR_IODEM,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(30);
	/* "Once he entered the cave, Lord Babi vanished before my very..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Vanished?" */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "You mean you lost sight of Lord Babi. Is that it?" */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "After he entered the mouth of the cave, I followed him......" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(20);
	fdActorFaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
	fdActorFaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
	fdActorFaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
	fdEventWait(20);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "I tried following him several times after that. Each time,..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdEventWait(20);
	/* "And you think that's where Lord Babi went this morning, Iodem?" */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdEventWait(30);
	/* "But if he always returned from Altmiller Cave before..." */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
	fdEventWait(20);
	/* "Then he should be back soon, right?" */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "No. This time is different. Something puzzles me..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdEventWait(20);
	/* "What do you mean?" */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "It is certainly strange that Lord Babi should go right..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 40);
	/* "What is it?" */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Lord Babi's bedding was not disturbed this morning." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "What's unusual about that?" */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "Last night, Lord Babi did not use his bed. That is the problem." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(20);
	fdActorFaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
	fdActorFaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
	fdActorFaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
	fdEventWait(20);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "Lord Babi has, in fact, been missing since last night." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAttachedEffect(ACTOR_FIRST_SOLDIER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_SECOND_SOLDIER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
	fdEventWait(30);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
	fdEventWait(20);
	/* "And that is why I am so worried. He has never done this before." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "We didn't know he'd been missing so long..." */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "You don't think Lord Babi's been ki-" */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_IODEM, FACING_WEST, 0);
	fdEventWait(20);
	/* "Speak no such foolishness! I am sure Lord Babi is alive and..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	fdEventWait(30);
	/* "Ah, yes, of course." */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "I will go to the colosseum in Lord Babi's stead. You must..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdEventWait(30);
	/* "We will all conduct another round of searches." */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdEventWait(40);
	fdActorFaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
	fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_SOUTH, 0);
	fdEventWait(40);
	fdActorSetSpeed(ACTOR_FIRST_SOLDIER, 1.3);
	fdActorSetSpeed(ACTOR_SECOND_SOLDIER, 1.3);
	fdActorWalkBy(ACTOR_FIRST_SOLDIER, 0, 120);
	fdActorWalkByAndWait(ACTOR_SECOND_SOLDIER, 0, 120);
	fdActorSetPos(ACTOR_FIRST_SOLDIER, 0, 0);
	fdActorSetPos(ACTOR_SECOND_SOLDIER, 0, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(40);
	fdActorSetSpeed(ACTOR_IODEM, 0.8);
	fdActorWalkByAndWait(ACTOR_IODEM, -16, 0);
	fdEventWait(30);
	/* "Lord Babi, where have you gone?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
	fdEventWait(30);
	fdActorSetSpeed(ACTOR_IODEM, 1);
	fdActorWalkByAndWait(ACTOR_IODEM, 120, 0);
	fdActorWalkByAndWait(ACTOR_IODEM, 60, 0);
	fdActorSetPos(ACTOR_IODEM, 0, 0);
	fdEventEnd();
}

/* The waking's window round the leader (WINDOW_TRANSITION_LEADER): inside it */
/* every layer shows without the colour effect; outside it at first only BG0, */
/* then every layer once it is WAKE_WINDOW_OPEN. */
#define WAKE_WININ                                                             \
	((WININ_WIN0_ALL & ~WININ_WIN0_CLR) | (WININ_WIN1_ALL & ~WININ_WIN1_CLR))
#define WAKE_WINDOW_OPEN 31

/* After the last match of the finals, Isaac wakes in a bed of the big bedroom,
 */
/* having slept long after fighting to the end, and the others tell him Babi */
/* wants to see the party. */
static void scBabiPalaceWakeAfterFinals(void) {
	s32 step;
	s32 pick;
	FIELD_STATE *fieldState;
	SCR_TRANSITION_STATE *state;

	fdEventStart();
	fdEventSetMessageId(MsgTolbiCallsName);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_LYING);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetPos(ACTOR_GARET, 104, 104);
	fdActorSetPos(ACTOR_MIA, 88, 120);
	fdActorSetPos(ACTOR_IVAN, 120, 120);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fieldState = FieldStateWork;
	fieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fieldState->transitionFrm = 12;
	sySynch(1);
	fdScrTransitionStartHeldWindow(WINDOW_TRANSITION_LEADER);
	state = ScreenTransitionStateWork;
	state->value = 0;
	state->insideLayers = WAKE_WININ;
	state->outsideLayers = WINOUT_WIN01_BG0;
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	for(step = 1; step <= 5; step++) {
		sySynch(3);
		state->value = step;
	}
	fdEventWait(40);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdEventWait(30);
	for(step = 5; step <= WAKE_WINDOW_OPEN; step++) {
		sySynch(3);
		state->value = step;
	}
	state->outsideLayers = WINOUT_WIN01_ALL & ~WINOUT_WIN01_CLR;
	fieldState = FieldStateWork;
	fieldState->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fieldState->transitionFrm = 24;
	fdEventWait(20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(30);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdEventWait(30);
	/* "Isaac..." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	/* "Huh?" */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdEventWait(30);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorWalkByAndWait(ACTOR_MIA, 0, -40);
	fdActorWalkByAndWait(ACTOR_MIA, 32, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdEventWait(10);
	/* "You've awakened." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdActorGet(ACTOR_PARTY_LEADER)->z.fixed -= toFIXED(3);
	fdActorGet(ACTOR_PARTY_LEADER)->tgtZ -= toFIXED(3);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_SITTING_UP);
	fdEventWait(40);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
	fdEventWait(30);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
	fdEventWait(40);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "Colosso? Duh! That ended long ago!" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	/* "You fell into a deep sleep... Don't you remember?" */
	fdEventOpenMessage(ACTOR_IVAN, 0);
	if(fdEventAskYesNoWithActorAnimation(-1, 0) == 0) {
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
		fdEventWait(20);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
		fdActorJump(ACTOR_GARET, 4, 13);
		fdActorJump(ACTOR_GARET, 4, 30);
		/* "Oh, gimme a break. You were out cold! You don't remember a..." */
		fdEventShowMessage(ACTOR_GARET, 0);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
		fdEventWait(20);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
		fdActorJump(ACTOR_GARET, 4, 13);
		fdActorJump(ACTOR_GARET, 4, 30);
		FieldStateWork->msg++;
		/* "Of course he doesn't! He was out cold! ...You had us worried!" */
		fdEventShowMessage(ACTOR_GARET, 0);
	}
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdEventWait(30);
	/* "I'm sorry for shouting at you." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
	fdEventWait(50);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdEventWait(35);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(30);
	/* "You fought brilliantly to the end, Isaac. Babi was singing..." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	/* "Are you okay now? In body and mind?" */
	fdEventOpenMessage(ACTOR_IVAN, 0);
	if(fdEventAskYesNoWithActorAnimation(-1, 0) == 0) {
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
		fdEventWait(20);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
		/* "He'd better be all right, after sleeping this long. Sheesh!" */
		fdEventShowMessage(ACTOR_GARET, 0);
		pick = 0;
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
		fdEventWait(20);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
		FieldStateWork->msg++;
		pick = 1;
		/* "If you feel out of sorts, it's probably because you slept..." */
		fdEventShowMessage(ACTOR_GARET, 0);
	}
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdEventWait(30);
	/* "I'm sorry for shouting again." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
	fdEventWait(50);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdEventWait(35);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(30);
	/* "Babi said he wanted to see you when you came to, Isaac." */
	fdEventShowMessage(ACTOR_MIA, 0);
	if(pick == 0) {
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdEventWait(30);
		/* "If you're okay, shall we go see Babi?" */
		fdEventShowMessage(ACTOR_IVAN, 0);
		fdEventWait(10);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdEventWait(30);
		/* "You're fine! Aren't you, Isaac?" */
		fdEventShowMessage(ACTOR_GARET, 0);
		FieldStateWork->msg += 2;
	}
	else {
		FieldStateWork->msg += 2;
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		fdEventWait(20);
		/* "I'm not sure Isaac's up to it just yet." */
		fdEventShowMessage(ACTOR_IVAN, 0);
		fdEventWait(10);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdEventWait(20);
		/* "What is it? Feeling weak? Fresh air and a walk will fix..." */
		fdEventShowMessage(ACTOR_GARET, 0);
	}
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdEventWait(30);
	/* "I'm more worried about what Babi wants. Let's get going!" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(20);
	fdActorFaceEachOther(ACTOR_IVAN, ACTOR_MIA, 40);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdEventWait(30);
	while((KeysHeld & KEYS_DPAD) == 0) {
		sySynch(1);
	}
	fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.9);
	fdActorWalkByAndWait(ACTOR_PARTY_LEADER, -32, -8);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(20);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdEventWait(20);
	/* "Are you sure you should be getting up so suddenly like that?" */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	/* "He's fine. He doesn't want to lounge around all day!" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdEventWait(30);
	/* "I see. Let's go see Babi then." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(30);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorSetSpeed(ACTOR_MIA, 1.2);
	fdActorSetSpeed(ACTOR_IVAN, 1.2);
	scWalkIntoLeader(ACTOR_GARET);
	scWalkIntoLeader(ACTOR_MIA);
	scWalkIntoLeader(ACTOR_IVAN);
	fdEventWait(10);
	fdEventEnd();
}

/* After Colosso, the first time the party goes through the throne room's door
 */
/* north of the red carpet, Babi and Iodem are waiting. Iodem hands the winner
 */
/* the Lure Cap; Babi tells of Lemuria and the draught that has kept him alive,
 */
/* asks the party to sail there for him once its business at Venus Lighthouse */
/* is done, and sends Iodem with them, to meet at Gondowan Passage. */
static void scBabiPalaceMeetBabiAfterColosso(void) {
	sySndPlay(MUS_BABIS_THEME);
	fdEventStart();
	fdEventSetMessageId(MsgTolbiWaitingCompanions);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdCameraMoveTo(216, CAMERA_KEEP, 736, 1);
	fdCameraWaitForMove();
	fdEventWait(20);
	fdEventWait(10);
	/* "I've been waiting for you and your companions, Isaac!" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetSpeed(ACTOR_BABI, 0.8);
	fdActorWalkByAndWait(ACTOR_BABI, 0, 16);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "I've been waiting here because I wanted to speak with you." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(20);
	/* "It's hard to talk when you're so far away... Come over here." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
	fdCameraWaitForMove();
	fdEventWait(40);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 208, 760);
	fdEventWait(10);
	fdCameraMoveTo(216, CAMERA_KEEP, 736, 1);
	syFieldActorLaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
	syFieldActorLaunchFromLeader(ACTOR_MIA, 0, 24, FACING_NORTH);
	syFieldActorLaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdEventWait(30);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "The battles at Colosso were spectacular!" */
	fdEventShowMessage(ACTOR_BABI, 0);

	/* If the leader won the finals, Iodem brings the victor's prize, the Lure
	 */
	/* Cap; otherwise Babi only praises how far the party came. */
	if(gmFlagGet(FLAG_WON_COLOSSO) != 0) {
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdEventWait(20);
		/* "It was most rewarding to see Babi's favored warrior steal..." */
		fdEventShowMessage(ACTOR_IODEM, 0);
		fdEventWait(10);
		fdActorFaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
		fdEventWait(30);
		fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
		fdEventWait(20);
		/* "Iodem, bring him the victor's prize." */
		fdEventShowMessage(ACTOR_BABI, 0);
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdEventWait(20);
		/* "Yes, sir, right away." */
		fdEventShowMessage(ACTOR_IODEM, 0);
		fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
		fdEventWait(20);
		fdActorSetSpeed(ACTOR_IODEM, 1);
		fdActorWalkByAndWait(ACTOR_IODEM, 0, -16);
		fdEventWait(40);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdEventWait(40);
		fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
		fdEventWait(20);
		fdActorWalkByAndWait(ACTOR_IODEM, 0, 32);
		fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
		fdEventWait(10);
		fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
		fdEventWait(30);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdEventWait(30);
		fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
		fdEventWait(20);
		fdActorSetSpeed(ACTOR_IODEM, 0.8);
		fdActorGet(ACTOR_IODEM)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkByAndWait(ACTOR_IODEM, 0, -16);
		fdActorGet(ACTOR_IODEM)->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
		fdEventWait(40);
		fdActorSetSpeed(ACTOR_BABI, 0.8);
		fdActorWalkByAndWait(ACTOR_BABI, 0, 16);
		fdEventWait(40);
		mnMessageArgumentPush(ITEM_LURE_CAP, MESSAGE_ARGUMENT_ITEM);
		/* "Isaac received ...." */
		fdEventShowMessage(-1, 0);
		gmItemShowFound(ITEM_LURE_CAP, 3);
		gmPartyGiveItem(ITEM_LURE_CAP, 0);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdEventWait(30);
		fdActorWalkByAndWait(ACTOR_BABI, 0, -16);
		fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
		fdEventWait(30);
		fdActorRunRepeatedMotion(ACTOR_BABI, 2);
		fdEventWait(20);
		/* "Incidentally, Isaac, are you feeling better now?" */
		fdEventOpenMessage(ACTOR_BABI, 0);
		FieldStateWork->msg += 2;
	}
	else {
		FieldStateWork->msg += 5;
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdEventWait(20);
		/* "It was wonderful to see Babi's handpicked warrior make it so..." */
		fdEventShowMessage(ACTOR_IODEM, 0);
		fdActorFaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
		fdEventWait(40);
		fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
		fdEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdEventWait(30);
		fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
		fdEventWait(30);
		fdActorRunRepeatedMotion(ACTOR_BABI, 2);
		fdEventWait(20);
		/* "Incidentally, Isaac, are you feeling better now?" */
		fdEventOpenMessage(ACTOR_BABI, 0);
	}

	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdEventWait(20);
		/* "Ah, the recuperative powers of youth...and after such fierce..." */
		fdEventShowMessage(ACTOR_IODEM, 0);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdEventWait(20);
		FieldStateWork->msg++;
		/* "You must be exhausted. I'm amazed to see you on your feet..." */
		fdEventShowMessage(ACTOR_IODEM, 0);
	}

	fdEventWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "Who could expect less from the warriors who rescued me?" */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorJump(ACTOR_GARET, 4, 13);
	fdActorJump(ACTOR_GARET, 4, 30);
	/* "Enough with the flattery. Let's get to the point: why were..." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	fdActorSetSpeed(ACTOR_BABI, 1.6);
	fdActorWalkByAndWait(ACTOR_IODEM, 0, 16);
	sySynch(2);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	fdEventWait(10);
	/* "Such insolence! Who do you think you are, to speak to Babi so?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(30);
	fdActorFaceActor(ACTOR_BABI, ACTOR_IODEM, 30);
	/* "Let it be, Iodem. There are other ways to judge a man." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	fdActorFaceEachOther(ACTOR_BABI, ACTOR_GARET, 0);
	fdEventWait(40);
	/* "What is it you said you were called?" */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(30);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 30);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_GARET, 30);
	/* "He's talking to you, Garet." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "That's right, Garet. We shall explain all." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(30);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "We intended to thank you and your companions for saving..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(30);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "But it would seem that gratitude is not enough..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "I have disclosed everything to Iodem." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "What do you mean by everything?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	/* "You do wish to know more about Lemuria, correct?" */
	fdEventOpenMessage(ACTOR_BABI, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
		fdEventWait(30);
		/* "Yes, indeed... Your desire is as plain as day." */
		fdEventShowMessage(ACTOR_BABI, 0);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
		fdEventWait(20);
		FieldStateWork->msg++;
		/* "You can't hide your interest from me. It is written on your..." */
		fdEventShowMessage(ACTOR_BABI, 0);
	}

	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "I have just seen Psynergy for the first time... It is an..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(50);
	fdActorFaceActor(ACTOR_BABI, ACTOR_IODEM, 60);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(20);
	/* "I used my Cloak Ball to show him Psynergy." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "No one else could see it, but I knew Isaac's tournament was..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "I thought it unlikely that you would fight using such power..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "No need to be ashamed, Isaac. Psynergy is also a warrior's..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdEventWait(30);
	fdEventWait(10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "When I first learned of your power, I was shocked. I thought..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "You have mentioned Lemuria many times, Babi. What is it?" */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_BABI, 50);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	/* "It is the city of an ancient people, lying far out at sea." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(20);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(50);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	fdEventWait(20);
	/* "Many years ago, about the time of the great flood, Babi went..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	/* "I see..." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
	fdEventWait(40);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	/* "The great flood struck while I was traveling with Lunpa." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Lunpa... You mean Lunpa the thief?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_BABI, ACTOR_IVAN, 40);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "I know it is hard to believe... It happened a hundred years ago." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 60);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(50);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "You mean...those rumors about you are...true?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	/* "Kraden once mentioned the Stone of Sages. Do you think..." */
	fdEventOpenMessage(ACTOR_GARET, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(20);
		/* "So the stone that grants immortality came from Lemuria?" */
		fdEventShowMessage(ACTOR_GARET, 0);
		FieldStateWork->msg += 3;
	}
	else {
		fdEventWait(10);
		FieldStateWork->msg++;
		/* "Huh? Don't tell me you forgot about the Stone of Sages..." */
		fdEventOpenMessage(ACTOR_GARET, 0);
		if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			fdEventWait(20);
			/* "It rules all, can change anything into gold, grants..." */
			fdEventShowMessage(ACTOR_GARET, 0);
			FieldStateWork->msg++;
		}
		else {
			FieldStateWork->msg++;
			/* "I can't think of anything else that would let him live for..."
			 */
			fdEventShowMessage(ACTOR_GARET, 0);
		}
	}

	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "So you've even heard of the Stone of Sages... I'm afraid I..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdEventWait(40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "It is the draught of Lemuria that has allowed me to live for..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "The draught of Lemuria?" */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	/* "It is a magical draught once taken by those who lived in..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	/* "And just drinking stops you from aging?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "Is that what it does?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
	/* "Hmmm... It would be more accurate to say it slows the aging..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "There is death in Lemuria..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "But it only comes to those whose senses have failed from..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdEventWait(30);
	/* "Do the people of Lemuria use Psynergy, like we do?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "You have that draught, Babi... That's why you've lived so long." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(40);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_MIA, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_IVAN, 0);
	fdEventWait(50);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	/* "And become wealthy enough to rule Tolbi." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "What are you getting at?" */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "What is your purpose in making everyone study Alchemy?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdEventWait(30);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_BABI, 30);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdEventWait(20);
	/* "I'm starting to wonder myself. What do you hope to gain, Babi?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "Ah. If I had attained true immortality... There would be no..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(30);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "I'm not sure I understand." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "I had precious little draught, and now I am running out." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(30);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "You all saw it. There was but little left in the cave..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "How long will it last? And what will happen when it runs out?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(20);
	fdActorFaceEachOther(ACTOR_BABI, ACTOR_IODEM, 40);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	/* "It will run out any day now. And when it does, my life is at..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorSetAttachedEffect(ACTOR_IODEM,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "Why didn't you tell us sooner? We must hurry to Lemuria!" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdActorSetSpeed(ACTOR_IODEM, 1.6);
	fdActorWalkByAndWait(ACTOR_IODEM, 0, 24);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IODEM, 0);
	fdActorFaceActor(ACTOR_MIA, ACTOR_IODEM, 0);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_IODEM, 0);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	/* "It is pointless..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_BABI, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "What on earth do you mean, pointless?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(30);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "We cannot find Lemuria." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetSpeed(ACTOR_IODEM, 1.2);
	fdActorWalkByAndWait(ACTOR_IODEM, 0, -24);
	fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
	fdEventWait(20);
	/* "But you just said that it lies far out at sea..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "If we search the seas, we're sure to find it!" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "I've sent so many ships in the past in search of Lemuria." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "This must have been long before I joined you... But still..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "I needed more Lemurian draught, but I couldn't find Lemuria." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	/* "Why did you collapse in the cave if you still had more draught?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "I have been rationing it, but this time, I waited too long..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "You were trying to make it last longer..." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "I overexerted myself trying to find Lemuria before it ran out." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	/* "And your reason for calling us here has to do with Lemuria?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "Exactly. I want you to find Lemuria." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(30);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	/* "I have not been able to locate Lemuria by sea, but perhaps..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "I can hide it no longer... I have built Babi Lighthouse to..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "What? That's why you're building the lighthouse?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_BABI, ACTOR_IODEM, 30);
	/* "No, there's more to it than that." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "We've spotted something in the ocean southeast of the..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	/* "Something dangerous, no doubt. What, monsters? We can handle..." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(20);
	/* "That may be... However, something else troubles me more." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "What is it, my lord?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
	fdEventWait(20);
	/* "No sailor has been able to keep his bearings at sea." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "You're kidding! You mean they can't even keep a straight course?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdEventWait(20);
	/* "Is there something that distorts distance or direction or..." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "The currents shift there, subtly changing a ship's direction." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	/* "So the lighthouse will help the ships keep their bearings..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "So, how exactly did you get away from Lemuria after the..." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "That is the thing, Garet!" */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdEventWait(30);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "I crossed the sea in a Lemurian ship. I am hoping you can..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "But why us, when you have so many powerful soldiers at your..." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "None of them have the ability to use Psynergy." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "One must have Psynergy to sail this ship!" */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Then that would mean that Babi is an Adept, wouldn't it?" */
	fdEventOpenMessage(ACTOR_IVAN, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_BABI, 0) == 0) {
		fdEventWait(10);
		/* "That must be it. No normal person could use that Cloak Ball." */
		fdEventShowMessage(ACTOR_IVAN, 0);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(10);
		FieldStateWork->msg++;
		/* "But, Isaac, no normal person could master the Cloak Ball!" */
		fdEventShowMessage(ACTOR_IVAN, 0);
	}

	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	/* "I learned to use a little Psynergy during my stay in Lemuria." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "With what little I'd learned, I was able to guide the ship." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
	fdEventWait(20);
	/* "In that case, perhaps... I could sail the ship." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorFaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(30);
	/* "No. It must be an Adept." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 40);
	/* "What should we do, Isaac? Will you do what he wants?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	fdEventWait(30);
	/* "If you do, I will grant you whatever you wish." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdEventWait(30);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 60);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(30);
	/* "You shouldn't make vows you will later find you cannot keep..." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "I haven't even considered my own wishes lately." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_IODEM, ACTOR_BABI, 60);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "In that case, I could provide you with a wealth of items you..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "Babi's offer is very generous... You will consider going,..." */
	fdEventOpenMessage(ACTOR_IODEM, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(20);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		/* "Isaac, we have to beat Felix and the others to the lighthouse." */
		fdEventShowMessage(ACTOR_GARET, 0);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(10);
		FieldStateWork->msg++;
		/* "Not this time. We have to beat Felix and the others to the..." */
		fdEventShowMessage(ACTOR_GARET, 0);
	}

	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "The lighthouse!?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Do you mean Venus Lighthouse?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Well then, our paths are intertwined. I, too, must go to..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "But we cannot get to the top of Venus Lighthouse, can we?" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorFaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "These Adepts may be able to solve the riddle that blocks us." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "Ah, I see... The thought hadn't occurred to me." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	/* "Isaac! You shall go to Venus Lighthouse with Iodem." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "Do not worry... You can take care of your own business first." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(30);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
	/* "I see... If they can solve the riddle of the lighthouse......" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	/* "So, we can search for Lemuria after they complete their own..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	/* "Is that acceptable?" */
	fdEventOpenMessage(ACTOR_BABI, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(20);
		fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
		/* "Ah, you will undertake this quest! I am most grateful!" */
		fdEventShowMessage(ACTOR_BABI, 0);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(10);
		fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
		FieldStateWork->msg++;
		/* "If you need time to make up your mind, then please do so." */
		fdEventShowMessage(ACTOR_BABI, 0);
	}

	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "In any case, you will need my assistance to reach the..." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorFaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "I'll make preparations so that I am ready to leave at any time." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
	/* "I take my leave now to make preparations." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
	fdEventWait(20);
	/* "Then you can all meet at Gondowan Passage..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "That's a brilliant idea!" */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	/* "One more thing. I'd like you to think about a suitable reward." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdEventWait(20);
	/* "Yes. I will take care of that immediately, as well." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	fdEventWait(20);
	/* The party tracks each departing speaker before gathering back together.
	 */
	fdActorWatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_IODEM);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_IODEM);
	fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_IODEM);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_IODEM);
	fdActorSetSpeed(ACTOR_IODEM, 1);
	fdActorWalkByAndWait(ACTOR_IODEM, 0, 32);
	fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(20);
	/* "I shall be waiting for you at the passage." */
	fdEventShowMessage(ACTOR_IODEM, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdEventWait(30);
	fdActorWalkByAndWait(ACTOR_GARET, 16, 0);
	fdActorWalkBy(ACTOR_IODEM, 0, 80);
	fdEventWait(40);
	fdActorWalkByAndWait(ACTOR_GARET, -16, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorWaitForMoveById(ACTOR_IODEM);
	fdEventWait(80);
	fdActorSetPos(ACTOR_IODEM, 0, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(30);
	fdActorWatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_BABI);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_BABI);
	fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_BABI);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_BABI);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	fdActorSetSpeed(ACTOR_BABI, 0.8);
	fdActorWalkByAndWait(ACTOR_BABI, 0, 24);
	fdActorWalkByAndWait(ACTOR_BABI, -80, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_EAST, 0);
	fdEventWait(20);
	/* "Your first task shall be the riddles of Venus Lighthouse. I..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(20);
	fdActorWalkByAndWait(ACTOR_BABI, 0, 48);
	fdActorWalkByAndWait(ACTOR_BABI, -64, 0);
	fdActorStop(ACTOR_PARTY_LEADER);
	fdActorStop(ACTOR_GARET);
	fdActorStop(ACTOR_MIA);
	fdActorStop(ACTOR_IVAN);
	fdActorSetPos(ACTOR_BABI, 0, 0);
	fdEventWait(20);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_MIA, 0);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_IVAN, 0);
	fdEventWait(30);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorSetSpeed(ACTOR_IVAN, 1.2);
	fdActorSetSpeed(ACTOR_MIA, 1.2);
	scWalkIntoLeader(ACTOR_GARET);
	scWalkIntoLeader(ACTOR_IVAN);
	scWalkIntoLeader(ACTOR_MIA);
	fdEventWait(10);
	scPlayMusic();
	fdEventEnd();
}

/* Walking north into one of the palace's wooden doors opens it with the sound
 */
/* of a door, and Isaac walks through. The doors are the big bedroom's two, up
 */
/* the stairs on either side of Babi's statue, cells (36, 3) and (59, 3); the */
/* throne room's, north of the red carpet, (32, 18) and (33, 18); the two at */
/* the south end of the passage up to the carpet, into the room west of the */
/* soldiers' room (29, 31) and into the soldiers' room (36, 31); Babi's */
/* bedroom's off its landing, (28, 50); and Sheba's, off hers, (5, 34). */
/* */
/* Each door opens by its own tileset animation (BABI_PALACE_DOOR_ANIMATION);
 */
/* the cell behaviour is the exit's number. */
static void scBabiPalaceLeaveThroughDoor(void) {
	FIELD_STATE *state;

	state = FieldStateWork;
	fdEventStart();
	sySndPlay(SE_DOOR_OPEN);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	if(state->stepTrigger == BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM) {
		fdTilesetAnimResume(BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION);
		fdEventWait(10);
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
	}
	else {
		if(state->stepTrigger == BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM) {
			fdTilesetAnimResume(BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION);
			fdEventWait(10);
			fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
		}
		else {
			fdTilesetAnimResume(BABI_PALACE_OTHER_DOORS_ANIMATION);
			fdEventWait(10);
			fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
		}
	}
	fdEventWait(16);
	syImportEventSetExitRequest(state->stepTrigger);
	scBabiPalacePauseDoorAnimations();
	fdEventEnd();
}

/* Walking north into the palace's door between the two guards, cell (12, 15),
 */
/* opens it and Isaac goes in. */
static void scBabiPalaceLeaveThroughAnimatedDoor(void) {
	fdEventStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridAnimateCells(BabiPalace_DoorCells, 36, 10);
	fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
	fdEventWait(16);
	syImportEventSetExitRequest(BABI_PALACE_ENTRANCE_TO_BABI_PALACE);
	fdEventEnd();
}

/* During the finals, talking to either guard at the palace door the first time
 */
/* brings Isaac before them: "You're Isaac, aren't you?" Either answer ends */
/* with Babi's word that he waits for the party at the colosseum, though No */
/* first has the guards confer and decide it was him all the same. */
static void scBabiPalacePassOnBabisWord(void) {
	s32 question;
	gmFlagSet(FLAG_BABI_PALACE_GUARD_GAVE_WORD);
	fdEventStart();
	fdEndBusyAndReveal();
	fdActorFaceDirection(ACTOR_FIRST_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 200, 272);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdEventWait(20);
	question = MsgTolbiArent;
	fdEventSetMessageId(question);
	/* "You're Isaac, aren't you?" */
	fdEventOpenMessage(ACTOR_FIRST_GUARD, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(20);
		fdEventSetMessageId(question + 1);
		/* "Babi left word that he would be waiting for you at the..." */
		fdEventShowMessage(ACTOR_FIRST_GUARD, 0);
	}
	else {
		fdEventWait(20);
		fdEventSetMessageId(question + 2);
		/* "Oh, I guess I was mistaken..." */
		fdEventShowMessage(ACTOR_FIRST_GUARD, 0);
		fdEventWait(20);
		fdActorFaceEachOther(ACTOR_FIRST_GUARD, ACTOR_SECOND_GUARD, 60);
		fdActorFaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTH_SOUTHEAST, 0);
		fdEventWait(40);
		fdActorRunRepeatedMotion(ACTOR_SECOND_GUARD, 2);
		fdEventWait(30);
		fdActorFaceEachOther(ACTOR_FIRST_GUARD, ACTOR_SECOND_GUARD, 30);
		fdActorSetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
		fdEventWait(30);
		fdActorShowEmote(ACTOR_FIRST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
		fdActorFaceDirection(ACTOR_FIRST_GUARD, FACING_SOUTH_SOUTHWEST, 0);
		fdActorFaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTH_SOUTHEAST, 0);
		fdEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_FIRST_GUARD, ANIM_SHAKE_HEAD);
		fdEventWait(20);
		/* "No, I'm certain it was you." */
		fdEventShowMessage(ACTOR_FIRST_GUARD, 0);
		fdEventWait(10);
		fdActorRunRepeatedMotion(ACTOR_FIRST_GUARD, 2);
		fdEventWait(20);
		/* "Babi left word that he would be waiting for you at the..." */
		fdEventShowMessage(ACTOR_FIRST_GUARD, 0);
	}
	fdEventEnd();
}

/* During the finals, the guard on the west side of the palace door thanks */
/* Isaac once for saving Babi, then reminds him that Babi waits at the */
/* colosseum. */
static void scBabiPalaceThankForSavingBabi(void) {
	if(fdEventGetFlag(FLAG_BABI_PALACE_GUARD_THANKED_PARTY) == 0) {
		gmFlagSet(FLAG_BABI_PALACE_GUARD_THANKED_PARTY);
		fdEventSetMessageId(MsgTolbiIsaacIdReallyLikeThank);
		/* "Isaac, I'd really like to thank you for saving Babi. He is..." */
		fdEventShowMessage(ACTOR_SECOND_GUARD, 0);
	}
	else {
		fdEventSetMessageId(MsgTolbiBabiWaitingForAtColosseum);
		/* "Babi is waiting for you at the colosseum. He wants you to go..." */
		fdEventShowMessage(ACTOR_SECOND_GUARD, 0);
	}
}

/* After Colosso, the guard on the west side of the palace door says all the */
/* warriors who stayed in the palace have left, and asks whether the party is */
/* going too. */
static void scBabiPalaceAskAboutRemainingWarriors(s32 actorId) {
	s32 question = MsgTolbiWarriorsWhoStayed;

	fdEventSetMessageId(question);
	/* "All the warriors who stayed in the palace left when Colosso..." */
	fdEventOpenMessage(actorId, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventSetMessageId(question + 1);
		/* "Oh, that's too bad. I bet we could have had a good time..." */
		fdEventShowMessage(actorId, 0);
	}
	else {
		fdEventSetMessageId(question + 2);
		/* "Huh? I thought for sure you'd be going, too. I guess not..." */
		fdEventShowMessage(actorId, 0);
	}
}

/* Searching the well in the court, south-west of the palace door at cell (6, */
/* 19): "It's filled with fresh, clean water." */
static void scBabiPalaceExamineWell(void) {
	fdEventStart();
	mnFieldMessageShow(MsgFieldPeeredWell, 1);
	mnFieldMessageShow(MsgTolbiItsFilledWithFreshClean, 1);
	fdEventEnd();
}

/* Returns the triggers for the scene and the story, as for the actor */
/* placements. */
const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.next == SCENE_BABI_PALACE_ENTRANCE) {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			return BabiPalace_OutsideTriggersAfterColosso;
		}
		if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
			return BabiPalace_OutsideTriggersFinals;
		}
		return BabiPalace_OutsideTriggersBabiMissing;
	}
	if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
		return BabiPalace_TriggersAfterColosso;
	}
	if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
		return BabiPalace_TriggersFinals;
	}
	return BabiPalace_TriggersBabiMissing;
}

/* Sets up the scene being entered. Returns 0. */
s32 scInit(void) {
	ACTOR *actor;
	SPR *sprite;
	ICON_TRANSFER *glyph;
	s32 set;

	scBabiPalacePauseDoorAnimations();
	/* XXX (no evidence): what BABI_PALACE_AFTER_BABI_SAVED and */
	/* BABI_PALACE_AFTER_COLOSSO were made for; only the debug scene selector */
	/* reaches them, and nothing in the game says why they set these story
	 * flags. */
	if(MainRoutineStateWork.entrance == BABI_PALACE_AFTER_BABI_SAVED) {
		gmFlagSet(FLAG_BABI_SAVED);
	}
	if(MainRoutineStateWork.entrance == BABI_PALACE_AFTER_COLOSSO) {
		gmFlagSet(FLAG_BABI_SAVED);
		gmFlagSet(FLAG_COLOSSO_OVER);
	}
	if(MainRoutineStateWork.next != SCENE_BABI_PALACE_ENTRANCE) {
		if(MainRoutineStateWork.entrance == BABI_PALACE_FROM_COTTAGE_ROOMS_B) {
			gmFlagClear(FLAG_SHOW_AREA_NAME);
		}
		if(gmFlagGet(FLAG_COLOSSO_OVER)) {
			set = gmFlagGet(FLAG_BABI_PALACE_CLOAK_BALL);
			if(set) {
				fdActorSetPos(ACTOR_CLOAK_BALL, 0, 0);
			}
			else {
				actor = fdActorGet(ACTOR_CLOAK_BALL);
				actor->retainSprCache = 1;
				actor->motionFlg = set;
				sprite = actor->spr;
				actor->y.fixed = toFIXED(4);
				sprite->cnt = set;
				sprite->image.fullCol = 0;
				sprite->image.pal = 0;
				/* The icon's tiles are the 128 bytes of a 16 by 16 sprite. */
				glyph = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER,
										sizeof(ICON_TRANSFER));
				gmItemLoadIcon(ITEM_CLOAK_BALL);
				mnVramLoad(sprite->slot, 128, glyph->tiles);
				syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
			}
			if(MainRoutineStateWork.entrance ==
				   BABI_PALACE_MEETING_ROOM_FROM_CARPET_ROOM &&
			   !fdEventGetFlag(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
				gmFlagSet(FLAG_BABI_SPOKE_AFTER_COLOSSO);
				fdActorSetPos(ACTOR_BABI, 208, 704);
				scBabiPalaceMeetBabiAfterColosso();
			}
			/* After Colosso Babi lies in his bed, with no shadow */
			/* (https://alchemy.pascalpixel.com/places/babi-palace_colosso-over/map-18.png).
			 */
			fdActorSetAnimation(ACTOR_BABI, BABI_ANIMATION_IN_BED);
			fdActorSetSprFlags(fdActorGet(ACTOR_BABI), 0);
		}
		else if(gmFlagGet(FLAG_BABI_SAVED) &&
				!gmFlagGet(FLAG_BABI_PALACE_FINALS_REST_OFFERED)) {
			fdActorSetPos(ACTOR_FINALS_ATTENDANT, 120, 72);
		}
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		/* The attendant before the rescue and after Colosso; during the finals
		 */
		/* ACTOR_FINALS_HURRYING_WARRIORS_ALONG has this number. */
		fdActorGet(ACTOR_BED_ATTENDANT)->collisionFlg |= ACTOR_NEAR_TALK;
		if(MainRoutineStateWork.entrance ==
		   BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH) {
			scBabiPalaceRestorePartyAndInventory();
			scBabiPalaceWakeAfterFinals();
			/* Only this entrance chooses the waking, so rewriting it makes a */
			/* later load of the scene from the main routine's state, as when a
			 * game */
			/* saved here is continued, an arrival from the statue room, not a
			 * second */
			/* waking. */
			MainRoutineStateWork.entrance =
				BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST;
		}
		if(MainRoutineStateWork.entrance ==
		   BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH) {
			scBabiPalaceRestorePartyAndInventory();
			gmFlagSet(FLAG_BABI_PALACE_FINALS_REST_OFFERED);
			gmFlagSet(FLAG_BABI_PALACE_FINALS_BEDS_OPEN);
			fdActorSetPos(ACTOR_FINALS_ATTENDANT, 56, 120);
			fdActorFaceDirection(ACTOR_FINALS_ATTENDANT, FACING_EAST_NORTHEAST,
								 0);
			scBabiPalaceWakeFromFinalsDream();
			MainRoutineStateWork.entrance =
				BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST;
		}
	}
	return 0;
}

/* clang-format off */
/* The palace door between the two guards opening, a column of three cells at a */
/* time. */
u16 BabiPalace_DoorCells[] = {
    MAPGRID_ANIMATION_STEP(25, 28, 1, 3, 5), MAPGRID_ANIMATION_STEP(26, 28, 1, 3, 5), MAPGRID_ANIMATION_STEP(27, 28, 1, 3, 5), MAPGRID_ANIMATION_STEPS_END,
};

s32 BabiPalace_IndigestionScript[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

SCENE_ENTRANCE BabiPalace_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { BABI_PALACE_FROM_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, 768, 0, 200, FACING_NORTH, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_ROOM_FROM_CARPET_ROOM_WEST, CONDITION_ALWAYS, 728, 0, 88, FACING_EAST, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_ROOM_FROM_CARPET_ROOM_EAST, CONDITION_ALWAYS, 808, 0, 88, FACING_WEST, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_ROOM_FROM_WEST_STEPS, CONDITION_ALWAYS, 600, 0, 152, FACING_EAST, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_ROOM_FROM_EAST_STEPS, CONDITION_ALWAYS, 936, 0, 152, FACING_WEST, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_ROOM_FROM_BIG_BEDROOM_WEST, CONDITION_ALWAYS, 584, 0, 56, FACING_SOUTH, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_STATUE_ROOM_FROM_BIG_BEDROOM_EAST, CONDITION_ALWAYS, 952, 0, 56, FACING_SOUTH, 0, 512, 0, 1024, 224, 0 },
    { BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST, CONDITION_ALWAYS, 72, 0, 216, FACING_NORTH, 0, 0, 0, 512, 240, 0 },
    { BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_EAST, CONDITION_ALWAYS, 440, 0, 216, FACING_NORTH, 0, 0, 0, 512, 240, 0 },
    { BABI_PALACE_SCHOLARS_LANDING_FROM_CARPET_ROOM, CONDITION_ALWAYS, 72, 0, 968, FACING_EAST, 0, 0, 864, 240, 1024, 0 },
    { BABI_PALACE_FROM_COTTAGE_ROOMS_B, CONDITION_ALWAYS, 168, 0, 936, FACING_SOUTH, 0, 0, 864, 240, 1024, 0 },
    { BABI_PALACE_TUNNEL_LANDING_FROM_CARPET_ROOM, CONDITION_ALWAYS, 424, 0, 680, FACING_WEST, 0, 328, 600, 568, 760, 0 },
    { BABI_PALACE_FROM_COLOSSEUM_TUNNEL, CONDITION_ALWAYS, 472, 0, 696, FACING_SOUTH, 0, 328, 600, 568, 760, 0 },
    { BABI_PALACE_BEDROOM_LANDING_FROM_MEETING_ROOM, CONDITION_ALWAYS, 408, 0, 824, FACING_EAST, 0, 320, 728, 560, 888, 0 },
    { BABI_PALACE_BEDROOM_LANDING_FROM_BABIS_BEDROOM, CONDITION_ALWAYS, 456, 0, 808, FACING_SOUTH, 0, 320, 728, 560, 888, 0 },
    { BABI_PALACE_BABIS_BEDROOM_FROM_BEDROOM_LANDING, CONDITION_ALWAYS, 824, 0, 648, FACING_NORTH, 0, 768, 480, 1024, 672, 0 },
    { BABI_PALACE_WEST_STEPS_FROM_STATUE_ROOM, CONDITION_ALWAYS, 872, 0, 936, FACING_WEST, 0, 784, 864, 1024, 1024, 0 },
    { BABI_PALACE_WEST_STEPS_FROM_WEST_ROUND_ROOM, CONDITION_ALWAYS, 888, 0, 968, FACING_WEST, 0, 784, 864, 1024, 1024, 0 },
    { BABI_PALACE_EAST_STEPS_FROM_STATUE_ROOM, CONDITION_ALWAYS, 760, 0, 936, FACING_EAST, 0, 608, 864, 848, 1024, 0 },
    { BABI_PALACE_EAST_STEPS_FROM_EAST_ROUND_ROOM, CONDITION_ALWAYS, 744, 0, 968, FACING_EAST, 0, 608, 864, 848, 1024, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_STATUE_ROOM_WEST, CONDITION_ALWAYS, 440, 0, 584, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_STATUE_ROOM_EAST, CONDITION_ALWAYS, 616, 0, 584, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_SCHOLARS_LANDING, CONDITION_ALWAYS, 328, 0, 312, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_TUNNEL_LANDING, CONDITION_ALWAYS, 728, 0, 312, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_NORTH_ROUND_ROOM, CONDITION_ALWAYS, 312, 0, 296, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_SHEBAS_LANDING, CONDITION_ALWAYS, 744, 0, 296, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_OVEN_ROOM, CONDITION_ALWAYS, 472, 0, 504, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_SOLDIERS_ROOM, CONDITION_ALWAYS, 584, 0, 504, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_CARPET_ROOM_FROM_MEETING_ROOM, CONDITION_ALWAYS, 528, 0, 296, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { BABI_PALACE_MEETING_ROOM_FROM_CARPET_ROOM, CONDITION_ALWAYS, 208, 0, 840, FACING_NORTH, 0, 0, 624, 336, 864, 0 },
    { BABI_PALACE_MEETING_ROOM_FROM_BEDROOM_LANDING, CONDITION_ALWAYS, 40, 0, 760, FACING_WEST, 0, 0, 624, 336, 864, 0 },
    { BABI_PALACE_OVEN_ROOM_FROM_CARPET_ROOM, CONDITION_ALWAYS, 200, 0, 360, FACING_NORTH, 0, 0, 240, 256, 480, 0 },
    { BABI_PALACE_SOLDIERS_ROOM_FROM_CARPET_ROOM, CONDITION_ALWAYS, 824, 0, 360, FACING_NORTH, 0, 768, 240, 1024, 480, 0 },
    { BABI_PALACE_WEST_ROUND_ROOM_FROM_WEST_STEPS, CONDITION_ALWAYS, 376, 0, 936, FACING_EAST, 0, 232, 864, 472, 1024, 0 },
    { BABI_PALACE_EAST_ROUND_ROOM_FROM_EAST_STEPS, CONDITION_ALWAYS, 520, 0, 936, FACING_WEST, 0, 424, 864, 664, 1024, 0 },
    { BABI_PALACE_NORTH_ROUND_ROOM_FROM_CARPET_ROOM, CONDITION_ALWAYS, 904, 0, 776, FACING_EAST, 0, 776, 704, 1016, 864, 0 },
    { BABI_PALACE_SHEBAS_LANDING_FROM_CARPET_ROOM, CONDITION_ALWAYS, 216, 0, 552, FACING_WEST, 0, 0, 472, 304, 632, 0 },
    { BABI_PALACE_SHEBAS_LANDING_FROM_SHEBAS_ROOM, CONDITION_ALWAYS, 88, 0, 552, FACING_SOUTH, 0, 0, 472, 304, 632, 0 },
    { BABI_PALACE_SHEBAS_ROOM_FROM_SHEBAS_LANDING, CONDITION_ALWAYS, 728, 0, 824, FACING_NORTH, 0, 552, 656, 792, 848, 0 },
    { BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH, CONDITION_ALWAYS, 152, 0, 104, FACING_SOUTH, 0, 0, 0, 512, 240, 0 },
    { BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH, CONDITION_ALWAYS, 152, 0, 104, FACING_SOUTH, 0, 0, 0, 512, 240, 0 },
    { SCENE_TABLE_END },
};

SCENE_ENTRANCE BabiPalace_OutsideEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 264, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { BABI_PALACE_ENTRANCE_FROM_TOLBI, CONDITION_ALWAYS, 264, 0, 424, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { BABI_PALACE_ENTRANCE_FROM_BABI_PALACE, CONDITION_ALWAYS, 200, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

SCENE_EXIT_REGION BabiPalace_Regions[] = {
    { SCENE_TABLE_END },
};

u32 BabiPalace_Exits[] = {
    SCENE_EXITS(BabiPalace),
    SCENE_EXIT(BABI_PALACE_TO_BABI_PALACE_ENTRANCE, BabiPalaceEntrance, BABI_PALACE_ENTRANCE_FROM_BABI_PALACE),
    SCENE_EXIT(BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_STATUE_ROOM_WEST),
    SCENE_EXIT(BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_STATUE_ROOM_EAST),
    SCENE_EXIT(BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS, BabiPalace, BABI_PALACE_WEST_STEPS_FROM_STATUE_ROOM),
    SCENE_EXIT(BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS, BabiPalace, BABI_PALACE_EAST_STEPS_FROM_STATUE_ROOM),
    SCENE_EXIT(BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, BabiPalace, BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST),
    SCENE_EXIT(BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, BabiPalace, BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_EAST),
    SCENE_EXIT(BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_BIG_BEDROOM_WEST),
    SCENE_EXIT(BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_BIG_BEDROOM_EAST),
    SCENE_EXIT(BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_SCHOLARS_LANDING),
    SCENE_EXIT(BABI_PALACE_TO_COTTAGE_ROOMS_B, CottageRoomsB, COTTAGE_ROOMS_B_FROM_BABI_PALACE),
    SCENE_EXIT(BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_TUNNEL_LANDING),
    SCENE_EXIT(BABI_PALACE_TO_COLOSSEUM_TUNNEL, ColosseumTunnel, COLOSSEUM_TUNNEL_FROM_BABI_PALACE),
    SCENE_EXIT(BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM, BabiPalace, BABI_PALACE_MEETING_ROOM_FROM_BEDROOM_LANDING),
    SCENE_EXIT(BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, BabiPalace, BABI_PALACE_BABIS_BEDROOM_FROM_BEDROOM_LANDING),
    SCENE_EXIT(BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING, BabiPalace, BABI_PALACE_BEDROOM_LANDING_FROM_BABIS_BEDROOM),
    SCENE_EXIT(BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_WEST_STEPS),
    SCENE_EXIT(BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM, BabiPalace, BABI_PALACE_WEST_ROUND_ROOM_FROM_WEST_STEPS),
    SCENE_EXIT(BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_EAST_STEPS),
    SCENE_EXIT(BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM, BabiPalace, BABI_PALACE_EAST_ROUND_ROOM_FROM_EAST_STEPS),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_CARPET_ROOM_WEST),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST, BabiPalace, BABI_PALACE_STATUE_ROOM_FROM_CARPET_ROOM_EAST),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING, BabiPalace, BABI_PALACE_SCHOLARS_LANDING_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING, BabiPalace, BABI_PALACE_TUNNEL_LANDING_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM, BabiPalace, BABI_PALACE_NORTH_ROUND_ROOM_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING, BabiPalace, BABI_PALACE_SHEBAS_LANDING_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, BabiPalace, BABI_PALACE_OVEN_ROOM_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, BabiPalace, BABI_PALACE_SOLDIERS_ROOM_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, BabiPalace, BABI_PALACE_MEETING_ROOM_FROM_CARPET_ROOM),
    SCENE_EXIT(BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_MEETING_ROOM),
    SCENE_EXIT(BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING, BabiPalace, BABI_PALACE_BEDROOM_LANDING_FROM_MEETING_ROOM),
    SCENE_EXIT(BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_OVEN_ROOM),
    SCENE_EXIT(BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_SOLDIERS_ROOM),
    SCENE_EXIT(BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS, BabiPalace, BABI_PALACE_WEST_STEPS_FROM_WEST_ROUND_ROOM),
    SCENE_EXIT(BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS, BabiPalace, BABI_PALACE_EAST_STEPS_FROM_EAST_ROUND_ROOM),
    SCENE_EXIT(BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_NORTH_ROUND_ROOM),
    SCENE_EXIT(BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM, BabiPalace, BABI_PALACE_CARPET_ROOM_FROM_SHEBAS_LANDING),
    SCENE_EXIT(BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, BabiPalace, BABI_PALACE_SHEBAS_ROOM_FROM_SHEBAS_LANDING),
    SCENE_EXIT(BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING, BabiPalace, BABI_PALACE_SHEBAS_LANDING_FROM_SHEBAS_ROOM),
    SCENE_EXITS(BabiPalaceEntrance),
    SCENE_EXIT(BABI_PALACE_ENTRANCE_TO_TOLBI, Tolbi, TOLBI_FROM_BABI_PALACE_ENTRANCE),
    SCENE_EXIT(BABI_PALACE_ENTRANCE_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_BABI_PALACE_ENTRANCE),
    SCENE_EXITS_END,
};

ACTOR_PLACE BabiPalace_OutsideActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 248, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 248, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE BabiPalace_ActorPlacementsBabiMissing[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        752, 0, 136, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        56, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 96, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, (s32)BabiPalace_IndigestionScript,
        472, 0, 136, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        88, 0, 296, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        888, 0, 344, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        984, 0, 440, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        984, 0, 312, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        944, 0, 384, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        920, 0, 560, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        112, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        728, 0, 728, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(ACTOR_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        600, 0, 744, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IODEM, FLAG_IODEM_HEARD_SOLDIERS, MOVEMENT_TYPE_STAND,
        392, 0, 360, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, FLAG_IODEM_HEARD_SOLDIERS, MOVEMENT_TYPE_STAND,
        392, 0, 352, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, FLAG_IODEM_HEARD_SOLDIERS, MOVEMENT_TYPE_STAND,
        392, 0, 368, FACING_EAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE BabiPalace_ActorPlacementsFinals[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        712, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        472, 0, 696, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        56, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        920, 0, 560, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        112, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        728, 0, 744, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(ACTOR_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        600, 0, 744, FACING_SOUTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE BabiPalace_ActorPlacementsAfterColosso[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        712, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        392, 0, 112, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        472, 0, 696, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        88, 0, 296, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        984, 0, 440, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        944, 0, 384, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BABI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        888, 0, 553, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        920, 0, 560, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        856, 0, 532, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        112, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        712, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IODEM, FLAG_BABI_SPOKE_AFTER_COLOSSO, MOVEMENT_TYPE_STAND,
        184, 0, 704, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

TRIGGER BabiPalace_TriggersBabiMissing[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, BABI_PALACE_TO_COTTAGE_ROOMS_B },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
    { TRIGGER_EXIT, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { TRIGGER_EXIT, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
    { TRIGGER_EXIT, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM },
    { TRIGGER_EXIT, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM },
    { TRIGGER_EXIT, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM },
    { TRIGGER_EXIT, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM },
    { TRIGGER_EXIT, 24, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST },
    { TRIGGER_EXIT, 25, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST },
    { TRIGGER_EXIT, 26, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING },
    { TRIGGER_EXIT, 27, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING },
    { TRIGGER_EXIT, 28, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM },
    { TRIGGER_EXIT, 29, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { TRIGGER_EXIT, 33, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 34, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING },
    { TRIGGER_EXIT, 35, CONDITION_ALWAYS, BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS },
    { TRIGGER_EXIT, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS },
    { TRIGGER_EXIT, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { TRIGGER_EXIT, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
    { TRIGGER_STEP, CELL_BEHAVIOR_BABI_PALACE_PASSAGE_TO_CARPET, FLAG_IODEM_HEARD_SOLDIERS, EVENT(scBabiPalaceHearSoldiersReport) },
    { TRIGGER_TALK, ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, EVENT(scBabiPalaceAskAboutSoldiers) },
    { TRIGGER_TALK, ACTOR_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfYoureEnteringColossoHurryAlong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowDiligentlyBabisSoldiers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWarriorsAreNotAllowedToStay) },
    { TRIGGER_TALK, ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, EVENT(scBabiPalaceShowBeds) },
    { TRIGGER_TALK, ACTOR_INJURED_WARRIOR, CONDITION_ALWAYS, EVENT(scBabiPalaceAskAboutColosso) },
    { TRIGGER_TALK, ACTOR_WARRIOR_WITH_INDIGESTION, CONDITION_ALWAYS, EVENT(scBabiPalaceTalkToWarriorWithIndigestion) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantToWatchTheColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INJURED_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGetSoAngrySeeingAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_WITH_INDIGESTION, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMaybeIShouldntHaveEatenQuite) },
    { TRIGGER_TALK, ACTOR_FIRST_PALACE_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnlyWarriorsWhoFightWellIn) },
    { TRIGGER_TALK, ACTOR_FIRST_PALACE_SOLDIER + 1, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceLordBabiIsBuildingTheLighthouse) },
    { TRIGGER_TALK, ACTOR_FIRST_PALACE_SOLDIER + 2, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisNameIsRidingOnThe) },
    { TRIGGER_TALK, ACTOR_FIRST_PALACE_SOLDIER + 3, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceEvenIfLordBabiDoesntReturn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMyFirstColossoWasSoExciting) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER + 1, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnceTheLighthouseIsCompleteBabis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER + 2, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWedNeverGetNearTheLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER + 3, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHearThatGirlIsGoing) },
    { TRIGGER_TALK, ACTOR_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItLooksLikeLordBabiDidnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGuessLordBabiDidNot) },
    { TRIGGER_TALK, ACTOR_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorHasBeenStayingAsLord) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasReveredAsAChild) },
    { TRIGGER_TALK, ACTOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsHoldingMeHostageIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasTakenHostageToForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsNotPermittedToLeave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHEBA, CONDITION_ALWAYS, EVENT(scBabiPalaceReadShebasMind) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_PALACE_BEDS, IF_FLAG_SET(FLAG_BABI_PALACE_BEDS_OPEN), EVENT(scBabiPalaceRestInPalaceBeds) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), CELL_BEHAVIOR_BABI_PALACE_ARMOR_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), CELL_BEHAVIOR_BABI_PALACE_EMPTY_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), CELL_BEHAVIOR_BABI_PALACE_HEADBAND_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BABI_PALACE_BIG_BEDROOM_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BABI_PALACE_NOTEBOOK_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BABI_PALACE_SOLDIERS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BABI_PALACE_DIARY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_BABI_PALACE_LUCKY_MEDAL_BARREL, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TABLE_END },
};

TRIGGER BabiPalace_TriggersFinals[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, BABI_PALACE_TO_COTTAGE_ROOMS_B },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
    { TRIGGER_EXIT, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { TRIGGER_EXIT, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
    { TRIGGER_EXIT, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM },
    { TRIGGER_EXIT, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM },
    { TRIGGER_EXIT, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM },
    { TRIGGER_EXIT, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM },
    { TRIGGER_EXIT, 24, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST },
    { TRIGGER_EXIT, 25, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST },
    { TRIGGER_EXIT, 26, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING },
    { TRIGGER_EXIT, 27, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING },
    { TRIGGER_EXIT, 28, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM },
    { TRIGGER_EXIT, 29, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { TRIGGER_EXIT, 33, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 34, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING },
    { TRIGGER_EXIT, 35, CONDITION_ALWAYS, BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS },
    { TRIGGER_EXIT, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS },
    { TRIGGER_EXIT, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { TRIGGER_EXIT, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
    { TRIGGER_TALK, ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisSoldiersHaveGoneToSee) },
    { TRIGGER_TALK, ACTOR_FINALS_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatAreYouDoingHurryOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceThoseGuysLookReallyConfidentAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheFinalsAreAllThatsLeft) },
    { TRIGGER_TALK, ACTOR_FINALS_ATTENDANT, CONDITION_ALWAYS, EVENT(scBabiPalaceOfferRest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHopeYoullAllDoWell) },
    { TRIGGER_TALK, ACTOR_FINALS_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiHasReturnedAndTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhenBabiReturnedHeHeadedOff) },
    { TRIGGER_TALK, ACTOR_FINALS_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatsGoingOnActorIsLooking) },
    { TRIGGER_TALK, ACTOR_FINALS_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSoldiersCameToSeeActor) },
    { TRIGGER_TALK, ACTOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceHmphIWishColossoWouldHurry) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceASmileOnAPrettyGirl) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsAGoodGirlBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIDontHaveTheLeastInterest) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_PALACE_BEDS, IF_FLAG_SET(FLAG_BABI_PALACE_FINALS_BEDS_OPEN), EVENT(scBabiPalaceRestInPalaceBeds) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), CELL_BEHAVIOR_BABI_PALACE_ARMOR_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), CELL_BEHAVIOR_BABI_PALACE_EMPTY_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), CELL_BEHAVIOR_BABI_PALACE_HEADBAND_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BABI_PALACE_BIG_BEDROOM_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BABI_PALACE_NOTEBOOK_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BABI_PALACE_SOLDIERS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BABI_PALACE_DIARY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_BABI_PALACE_LUCKY_MEDAL_BARREL, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TABLE_END },
};

TRIGGER BabiPalace_TriggersAfterColosso[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, BABI_PALACE_TO_COTTAGE_ROOMS_B },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
    { TRIGGER_EXIT, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { TRIGGER_EXIT, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
    { TRIGGER_EXIT, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM },
    { TRIGGER_EXIT, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM },
    { TRIGGER_EXIT, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM },
    { TRIGGER_EXIT, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM },
    { TRIGGER_EXIT, 24, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST },
    { TRIGGER_EXIT, 25, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST },
    { TRIGGER_EXIT, 26, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING },
    { TRIGGER_EXIT, 27, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING },
    { TRIGGER_EXIT, 28, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM },
    { TRIGGER_EXIT, 29, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { TRIGGER_EXIT, 33, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 34, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING },
    { TRIGGER_EXIT, 35, CONDITION_ALWAYS, BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS },
    { TRIGGER_EXIT, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS },
    { TRIGGER_EXIT, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM },
    { TRIGGER_EXIT, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughDoor) },
    { TRIGGER_EXIT, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
    { TRIGGER_TALK, ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, EVENT(scBabiPalaceAskAboutMeetingBabi) },
    { TRIGGER_TALK, ACTOR_ASKING_ABOUT_THE_EASTERN_SHORES, CONDITION_ALWAYS, EVENT(scBabiPalaceAskAboutEasternShores) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHaventSeenMuchOfBabi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_THE_EASTERN_SHORES, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItWouldBeAWasteTo) },
    { TRIGGER_TALK, ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNoOnesGoingToSleepIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWouldThisRoomGetVeryDusty) },
    { TRIGGER_TALK, ACTOR_RELIEVED_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWasFoundSafeAndSound) },
    { TRIGGER_TALK, ACTOR_GONDOWAN_BOUND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNowThatColossoIsOverIm) },
    { TRIGGER_TALK, ACTOR_SOLDIER_PREFERRING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSouthernContinentIsADangerous) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RELIEVED_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWasHopingIdBeThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GONDOWAN_BOUND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceColossoHasEndedSoWeHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_PREFERRING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantThatLighthouseFinishedQuickly) },
    { TRIGGER_TALK, ACTOR_BABI, IF_FLAG_SET(FLAG_BABI_PALACE_CLOAK_BALL), MSGNO(MsgBabiPalaceIfYouUseTheCloakBall) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABI, IF_FLAG_SET(FLAG_BABI_PALACE_CLOAK_BALL), MSGNO(MsgBabiPalaceWhyWouldAnAdeptLikeYou) },
    { TRIGGER_TALK, ACTOR_BABI, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSleepingALotHelpsToDelay) },
    { TRIGGER_TALK, ACTOR_AFTER_COLOSSO_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisFaceIsVeryPaleLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABI, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWishIHadMetYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_COLOSSO_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfSomethingHappensToBabiWhat) },
    { TRIGGER_TALK, ACTOR_CLOAK_BALL, CONDITION_ALWAYS, EVENT(scBabiPalaceOfferCloakBall) },
    { TRIGGER_TALK, ACTOR_AFTER_COLOSSO_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheGirlFromLaliveroIsNo) },
    { TRIGGER_TALK, ACTOR_AFTER_COLOSSO_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSurprisedActorWasAllowed) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_COLOSSO_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItSeemedOnlyNaturalThatActor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_COLOSSO_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSoRelievedActorHas) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_PALACE_BEDS, CONDITION_ALWAYS, EVENT(scBabiPalaceRestInPalaceBeds) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), CELL_BEHAVIOR_BABI_PALACE_ARMOR_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), CELL_BEHAVIOR_BABI_PALACE_EMPTY_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), CELL_BEHAVIOR_BABI_PALACE_HEADBAND_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BABI_PALACE_BIG_BEDROOM_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BABI_PALACE_NOTEBOOK_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BABI_PALACE_SOLDIERS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BABI_PALACE_DIARY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_BABI_PALACE_LUCKY_MEDAL_BARREL, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TABLE_END },
};

TRIGGER BabiPalace_OutsideTriggersBabiMissing[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_ENTRANCE_TO_BABI_PALACE, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughAnimatedDoor) },
    { TRIGGER_TALK, ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNormallyNoOnesAllowedIntoBabis) },
    { TRIGGER_TALK, ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSomeOfTheRoomsInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceShouldThePalaceReallyBeOpen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowManyPeopleWill) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_PALACE_WELL, CONDITION_ALWAYS, EVENT(scBabiPalaceExamineWell) },
    { TRIGGER_TABLE_END },
};

TRIGGER BabiPalace_OutsideTriggersFinals[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_ENTRANCE_TO_BABI_PALACE, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughAnimatedDoor) },
    { TRIGGER_TALK, ACTOR_FIRST_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT(scBabiPalacePassOnBabisWord) },
    { TRIGGER_TALK, ACTOR_SECOND_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT(scBabiPalacePassOnBabisWord) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_FIRST_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT(scBabiPalacePassOnBabisWord) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_SECOND_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, EVENT(scBabiPalacePassOnBabisWord) },
    { TRIGGER_TALK, ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWantsYouToGoStraight) },
    { TRIGGER_TALK, ACTOR_SECOND_GUARD, CONDITION_ALWAYS, EVENT(scBabiPalaceThankForSavingBabi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSureTheySavedBabiButIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsADeliberateManHe) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_PALACE_WELL, CONDITION_ALWAYS, EVENT(scBabiPalaceExamineWell) },
    { TRIGGER_TABLE_END },
};

TRIGGER BabiPalace_OutsideTriggersAfterColosso[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BABI_PALACE_ENTRANCE_TO_BABI_PALACE, CONDITION_ALWAYS, EVENT(scBabiPalaceLeaveThroughAnimatedDoor) },
    { TRIGGER_TALK, ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIveLostAllMyEnthusiasmSince) },
    { TRIGGER_TALK, ACTOR_SECOND_GUARD, CONDITION_ALWAYS, EVENT(scBabiPalaceAskAboutRemainingWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheEndOfAFestivalIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceAllTheHeroicWarriorsWhoTook) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BABI_PALACE_WELL, CONDITION_ALWAYS, EVENT(scBabiPalaceExamineWell) },
    { TRIGGER_TABLE_END },
};
/* clang-format on */
