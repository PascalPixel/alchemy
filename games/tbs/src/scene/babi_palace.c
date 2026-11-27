/* Babi's Palace in Tolbi, and the court before its door. From the town, the
   party crosses the court, past two guards and a well, to the palace's double
   door. Inside, the entrance hall has Babi's statue at its north end and stairs
   up on either side to the two doors of the big bedroom, where guests and
   Colosso's warriors sleep in four beds. From the hall a passage runs north
   between two doors, the soldiers' room's and the room west of it, to a
   crossroads with a red carpet. Its halls lead west and east to the stairs down
   to Babi's scholars, to the tunnel into the colosseum's stands and to the
   landing outside the room where Sheba is kept, and north to Babi's throne
   room, with Babi's own bedroom beyond.

   While Babi is missing, the attendant in the big bedroom offers the party the
   beds, and walking up the passage to the red carpet the party overhears Iodem
   and two soldiers back from Altmiller Cave, where Babi has slipped away
   before. Sheba, held hostage so that Lalivero builds Babi's lighthouse,
   wonders when she will see her homeland, and notices the first Mind Read on
   her.

   Once the party has saved Babi in Altmiller Cave the finals of Colosso are
   set: a guard outside passes on Babi's word that he waits at the colosseum,
   and the attendant offers the beds again. A match lost in the finals, or the
   last one fought, brings the party back to wake in the big bedroom. After
   Colosso, Babi and Iodem wait in the throne room: Babi gives the Lure Cap to
   the winner, tells of Lemuria and its draught, asks the party to sail there
   for him once Venus Lighthouse is dealt with, and sends Iodem to meet them at
   Gondowan Passage. In Babi's bedroom afterwards the Cloak Ball on show is his
   to lend.

   The code: the court (SceneId_BabiPalaceEntrance) and the palace's rooms on
   one map (SceneId_BabiPalace) share this overlay and one exit table, and each
   chooses one of three actor and cue tables by FLAG_BABI_SAVED and
   FLAG_COLOSSO_OVER. The exits name the rooms by what each shows: the statue
   room is the entrance hall, the carpet room the crossroads, the meeting room
   the throne room, the oven room the room west of the soldiers' room, with its
   stove, and the round rooms the small eight-sided rooms up the stairs. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "graphics/display_transition.h"
#include "graphics/graphics.h"
#include "scene/walk_into_leader.h"
#include "sound/sound_driver_symbols.h"
#include "text/msg_ids.h"

extern Uint16			 BabiPalace_DoorMetatiles[];
extern Sint32			 BabiPalace_IndigestionScript[];
extern SCENE_ENTRANCE	 BabiPalace_Entrances[];
extern SCENE_ENTRANCE	 BabiPalace_OutsideEntrances[];
extern SCENE_EXIT_REGION BabiPalace_Regions[];
extern Uint32			 BabiPalace_Exits[];
extern ACTOR_PLACE		 BabiPalace_OutsideActorPlacements[];
extern ACTOR_PLACE		 BabiPalace_ActorPlacementsBabiMissing[];
extern ACTOR_PLACE		 BabiPalace_ActorPlacementsFinals[];
extern ACTOR_PLACE		 BabiPalace_ActorPlacementsAfterColosso[];
extern CUE				 BabiPalace_CuesBabiMissing[];
extern CUE				 BabiPalace_CuesFinals[];
extern CUE				 BabiPalace_CuesAfterColosso[];
extern CUE				 BabiPalace_OutsideCuesBabiMissing[];
extern CUE				 BabiPalace_OutsideCuesFinals[];
extern CUE				 BabiPalace_OutsideCuesAfterColosso[];

/* The palace's actors are placed by one of three tables, as the story goes on.

   The three are before Babi is rescued (BabiPalace_ActorPlacementsBabiMissing),
   during the finals (BabiPalace_ActorPlacementsFinals) and after Colosso
   (BabiPalace_ActorPlacementsAfterColosso).

   Each numbers its rows from ACTOR_FIRST_PLACED, with Garet, Ivan, Mia and
   Sheba in their own party slots. Most of the people stand in all three under
   other numbers.

   The palace's residents, three old men with canes and a young man with brown
   hair, stand in all three. One sent Babi's soldiers to search the town, and
   another tells warriors who are not entering Colosso that they may not stay.
   The game names none of them.

   Before the rescue there are the attendant who shows the party its beds, Iodem
   and the two soldiers he sent to Altmiller Cave. Sheba is held here until
   Colosso is over. */
typedef enum BABI_PALACE_ACTOR_ID {
	/* An old man with a cane, in the same room and at the same number in every
	   table. He is the one who sent Babi's soldiers to search the town.

	   He asks "Did you run into any of Babi's soldiers in Town?", and if not,
	   says "Hmph! I told them to search every nook and cranny of this
	   town..." */
	ACTOR_ASKING_ABOUT_BABIS_SOLDIERS = ACTOR_FIRST_PLACED,
	/* The attendant, a woman with fair hair, whose number is the same after
	   Colosso: "No one's going to sleep in this big bedroom until the next
	   Colosso!" */
	ACTOR_BED_ATTENDANT,
	/* A warrior in a golden helmet. "So, did you guys get injured, too?"
	   Thoughts: "I hurt my back during my training, and now I can't move at
	   all." */
	ACTOR_INJURED_WARRIOR,
	/* A warrior in a golden helmet. "I can't fight because of a little
	   indigestion? That's pathetic!" */
	ACTOR_WARRIOR_WITH_INDIGESTION,
	/* A young man with brown hair in a purple shirt. "If you're entering
	   Colosso, hurry along to the colosseum." Thoughts: "Warriors are not
	   allowed to stay in the palace if they don't enter Colosso!" */
	ACTOR_HURRYING_WARRIORS_ALONG,
	/* Four of Babi's soldiers, in one room. The first says "Only warriors who
	   fight well in Colosso's finals can become soldiers here." */
	ACTOR_FIRST_PALACE_SOLDIER,
	/* An old man with a cane beside Babi's bed. "It looks like Lord Babi didn't
	   use his bed this morning." */
	ACTOR_BY_BABIS_BED = ACTOR_FIRST_PALACE_SOLDIER + 4,
	/* An old man with a cane. "Sheba has been staying as Lord Babi's guest for
	   some time." Thoughts: "It is heartbreaking to see her tears and hear her
	   pleas to be released." */
	ACTOR_SORRY_FOR_SHEBA,
	/* In Sheba's room, in SPRITE_OLD_WOMAN, as in vale_plaza.c. Thoughts:
	   "Sheba is not permitted to leave this room." */
	ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM,
	ACTOR_IODEM,
	ACTOR_FIRST_SOLDIER,
	ACTOR_SECOND_SOLDIER
} BABI_PALACE_ACTOR_ID;

/* During the finals, ACTOR_ASKING_ABOUT_BABIS_SOLDIERS keeps its number, and
   the others stand under new ones. */
typedef enum BABI_PALACE_FINALS_ACTOR_ID {
	/* The same resident as ACTOR_HURRYING_WARRIORS_ALONG: "What are you doing?
	   Hurry over to the colosseum!" */
	ACTOR_FINALS_HURRYING_WARRIORS_ALONG = ACTOR_FIRST_PLACED + 1,
	/* The same attendant, a number later. */
	ACTOR_FINALS_ATTENDANT,
	/* The same resident as ACTOR_BY_BABIS_BED: "Babi has returned and the
	   finals are set to begin!" */
	ACTOR_FINALS_BY_BABIS_BED,
	/* The same resident as ACTOR_SORRY_FOR_SHEBA: "What's going on? Sheba is
	   looking much happier." */
	ACTOR_FINALS_SORRY_FOR_SHEBA,
	/* The same woman as ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM: "The soldiers came to
	   see Sheba. She's been in a great mood ever since." */
	ACTOR_FINALS_OLD_WOMAN_IN_SHEBAS_ROOM
} BABI_PALACE_FINALS_ACTOR_ID;

/* After Colosso there are Babi and the Cloak Ball he keeps on show.
   ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, ACTOR_BED_ATTENDANT and Iodem keep their
   numbers. */
typedef enum BABI_PALACE_AFTER_COLOSSO_ACTOR_ID {
	/* One of Babi's soldiers. */
	ACTOR_RELIEVED_SOLDIER = ACTOR_FIRST_PLACED + 2,
	/* The same resident as ACTOR_HURRYING_WARRIORS_ALONG: "So, you're all from
	   the eastern shores of the Karagol Sea, huh?" */
	ACTOR_ASKING_ABOUT_THE_EASTERN_SHORES,
	/* One of Babi's soldiers. */
	ACTOR_GONDOWAN_BOUND_SOLDIER,
	/* "The southern continent is a dangerous place... you'll know why I prefer
	   Angara." */
	ACTOR_SOLDIER_PREFERRING_ANGARA,
	ACTOR_BABI,
	/* The same resident as ACTOR_BY_BABIS_BED: "Babi's face is very pale, like
	   that of a dead man." */
	ACTOR_AFTER_COLOSSO_BY_BABIS_BED,
	ACTOR_CLOAK_BALL,
	/* The same resident as ACTOR_SORRY_FOR_SHEBA: "The girl from Lalivero is no
	   longer here." */
	ACTOR_AFTER_COLOSSO_SORRY_FOR_SHEBA,
	/* The same woman as ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM: "I am surprised Sheba
	   was allowed to return to Lalivero like that." */
	ACTOR_AFTER_COLOSSO_OLD_WOMAN_IN_SHEBAS_ROOM
} BABI_PALACE_AFTER_COLOSSO_ACTOR_ID;

/* The two guards outside the palace (BabiPalaceEntrance). */
enum { ACTOR_FIRST_GUARD = 8, ACTOR_SECOND_GUARD = 9 };

/* The attendant has led the party to the beds. Talking again sends the
   attendant back. */
enum { FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS = FLAG_SCENE_TEMP_00 };

/* The palace map's tileset animations that open its doors, for
   fdTilesetAnimResume (play) and fdTilesetAnimPause (pause).

   Each scene entry pauses them, so every door is drawn shut. The recording of
   scBabiPalaceLeaveThroughDoor shows one swing open
   (https://alchemy.pascalpixel.com/places/babi-palace/events/scBabiPalaceLeaveThroughDoor.gif).
 */
typedef enum BABI_PALACE_DOOR_ANIMATION {
	BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION = 1, /* the throne room's door */
	BABI_PALACE_OTHER_DOORS_ANIMATION = 2, /* every door without its own */
	BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION = 4
} BABI_PALACE_DOOR_ANIMATION;

/* The leader's animations in the big bedroom's bed, after the four shared ones
   (ACTOR_ANIMATION).

   The nod and the shake of the head answer yes and no in
   scBabiPalaceWakeAfterFinals. */
typedef enum BABI_PALACE_BED_ANIMATION {
	ANIM_IN_BED_LYING = 31,
	/* Sitting up. The leader is raised by 3 pixels as it starts. */
	ANIM_IN_BED_SITTING_UP = 32,
	ANIM_IN_BED_NOD = 33,
	ANIM_IN_BED_SHAKE_HEAD = 34
} BABI_PALACE_BED_ANIMATION;

/* Metatile behaviors the cues answer besides the exits, whose metatile
   behaviors are their exits' numbers. */
typedef enum BABI_PALACE_METATILE_BEHAVIOR {
	/* The passage up to the red carpet, metatiles (32, 26) and (33, 26), where
	   the party overhears Iodem while Babi is missing. */
	METATILE_BEHAVIOR_BABI_PALACE_PASSAGE_TO_CARPET = 60,
	/* The big bedroom's four beds, searched to sleep. */
	METATILE_BEHAVIOR_BABI_PALACE_BEDS = 70,
	/* The well in the court, south-west of the palace door. */
	METATILE_BEHAVIOR_BABI_PALACE_WELL = 90,
	/* The barrel in the eight-sided room up the east stairs, metatile (34, 61),
	   with a Lucky Medal in it. */
	METATILE_BEHAVIOR_BABI_PALACE_LUCKY_MEDAL_BARREL = 100,
	/* The shelf at the north wall of the big bedroom, metatiles (24, 3) and
	   (25, 3):
	   "The armor looks heavy." */
	METATILE_BEHAVIOR_BABI_PALACE_ARMOR_SHELF = 200,
	/* The shelf in the room west of the soldiers' room, (9, 18) and (10, 18):
	   "There's nothing in there." */
	METATILE_BEHAVIOR_BABI_PALACE_EMPTY_SHELF = 201,
	/* The shelf in the soldiers' room, (55, 18) and (56, 18), with a headband
	   on it. */
	METATILE_BEHAVIOR_BABI_PALACE_HEADBAND_SHELF = 202,
	/* The big bedroom's bookcase, (6, 3) and (7, 3): "The Heart of the..." */
	METATILE_BEHAVIOR_BABI_PALACE_BIG_BEDROOM_BOOKCASE = 203,
	/* The bookcase in the room west of the soldiers' room, (12, 18) and (13,
	   18), with a notebook of records. */
	METATILE_BEHAVIOR_BABI_PALACE_NOTEBOOK_BOOKCASE = 204,
	/* The soldiers' room's bookcase, (58, 18) and (59, 18): "Become a Macho Man
	   in..." */
	METATILE_BEHAVIOR_BABI_PALACE_SOLDIERS_BOOKCASE = 205,
	/* The bookcase in Babi's bedroom, (58, 33) and (59, 33), with Babi's
	   diary. */
	METATILE_BEHAVIOR_BABI_PALACE_DIARY_BOOKCASE = 206
} BABI_PALACE_METATILE_BEHAVIOR;

/* Pauses the tileset animations of the palace's doors as the leader leaves. */
static inline void scBabiPalacePauseDoorAnimations(void)
{
	fdTilesetAnimPause(BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION);
	fdTilesetAnimPause(BABI_PALACE_OTHER_DOORS_ANIMATION);
	fdTilesetAnimPause(BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION);
}

/* After a night in the palace's beds, the party's HP and PP are full, Garet,
   Ivan and Mia rejoin, and the inventory Colosso held is returned. */
static inline void scBabiPalaceRestorePartyAndInventory(void)
{
	Sint16 members[8];
	UNIT  *state;
	Sint32 memberCount;
	Sint32 i;

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

const SCENE_ENTRANCE *scGetEntrances(void)
{
	if(MainRoutineStateWork.next == SCENE_BABI_PALACE_ENTRANCE) {
		return BabiPalace_OutsideEntrances;
	}
	return BabiPalace_Entrances;
}

/* The palace has no exit regions, so the table is empty. */
const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return BabiPalace_Regions;
}

/* Both scenes share one exit table. */
const Uint32 *scGetExits(void)
{
	return BabiPalace_Exits;
}

/* Returns the actor placements for the scene and the story, which is before
   Babi's rescue, during the finals or after Colosso. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
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
static inline void scBabiPalaceShowQuestionAnswer(
	Sint32 actorId, Sint32 question)
{
	fdScriptSetMessageId(question);
	fdScriptOpenMessage(actorId, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
		fdScriptSetMessageId(question + 1);
	else
		fdScriptSetMessageId(question + 2);
	fdScriptShowMessage(actorId, 0);
}

/* While Babi is missing, the old man with a cane in the entrance hall asks "Did
   you run into any of Babi's soldiers in Town?"; No gets him grumbling that he
   told them to search every nook and cranny. */
static void scBabiPalaceAskAboutSoldiers(Sint32 actorId)
{
	scBabiPalaceShowQuestionAnswer(actorId, MsgTolbiRunBabisSoldiers);
}

/* While Babi is missing, the attendant in the big bedroom, the woman with fair
   hair by its west wall, says there are just enough beds for everybody and
   walks over to them. Talking to her there, she offers the four beds ("Will you
   be going right to sleep?"), and from then on a search of a bed lets the party
   sleep in it. */
static void scBabiPalaceShowBeds(Sint32 actorId)
{
	Sint16 roundedFacing;
	Sint32 facingOrMessage;

	roundedFacing =
		FACING_NEAREST_CARDINAL(fdActorGet(ACTOR_PARTY_LEADER)->facing);
	fdScriptStart();
	fdEndBusyAndReveal();
	if(fdScriptGetFlag(FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS) == 0) {
		gmFlagSet(FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS);
		gmFlagClear(FLAG_BABI_PALACE_BEDS_OPEN);
		fdScriptSetMessageId(MsgTolbiWeHaveJustEnoughExtra);
		/* "We have just enough extra beds in this room for everybody...." */
		fdScriptShowMessage(actorId, 0);
		fdScriptWait(10);
		facingOrMessage = FACING_SOUTH;
		if((Uint16)roundedFacing == FACING_SOUTH) {
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 40, 104);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		}
		fdActorSetSpeed(actorId, 1);
		fdActorWalkByAndWait(actorId, 0, -48);
		fdActorWalkByAndWait(actorId, 64, 0);
		fdActorFaceDirection(actorId, facingOrMessage, 0);
	} else {
		gmFlagClear(FLAG_BABI_PALACE_ATTENDANT_SHOWED_BEDS);
		gmFlagSet(FLAG_BABI_PALACE_BEDS_OPEN);
		fdActorFaceDirection(actorId, FACING_SOUTH, 0);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdScriptWait(20);
		facingOrMessage = MsgTolbiUseFourBeds;
		fdScriptSetMessageId(facingOrMessage);
		/* "You may use these four beds. Will you be going right to sleep?" */
		fdScriptOpenMessage(actorId, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			fdScriptSetMessageId(facingOrMessage + 1);
			/* "Then, enjoy your rest." */
			fdScriptShowMessage(actorId, 0);
		} else {
			fdScriptSetMessageId(facingOrMessage + 2);
			/* "Well, if you do get tired, feel free to sleep here
			   anytime..." */
			fdScriptShowMessage(actorId, 0);
		}
		fdScriptWait(10);
		fdActorSetAnimationAndWait(actorId, ANIM_NOD);
		fdScriptWait(20);
		fdActorWalkByAndWait(actorId, -64, 0);
		fdActorWalkByAndWait(actorId, 0, 48);
#if BABI_PALACE_BED_ATTENDANT_SNAPS_TO_POST
		fdActorSetPos(actorId, 56, 120);
#endif
	}
	fdScriptEnd();
}

/* During the finals, the attendant in the big bedroom asks whether the party
   came to rest before its match, and the beds are open to it again. */
static void scBabiPalaceOfferRest(Sint32 actorId)
{
	Sint32 question;

	fdScriptStart();
	fdEndBusyAndReveal();
	if(fdScriptGetFlag(FLAG_BABI_PALACE_FINALS_REST_OFFERED) == 0) {
		gmFlagSet(FLAG_BABI_PALACE_FINALS_REST_OFFERED);
		gmFlagSet(FLAG_BABI_PALACE_FINALS_BEDS_OPEN);
		fdActorFaceDirection(actorId, FACING_SOUTH, 0);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdScriptWait(20);
		question = MsgTolbiCameRestBefore;
		fdScriptSetMessageId(question);
		/* "You all came to rest up before the match, didn't you? Would..." */
		fdScriptOpenMessage(actorId, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			fdScriptWait(10);
			fdScriptSetMessageId(question + 1);
		} else {
			fdScriptSetMessageId(question + 2);
		}
		fdScriptShowMessage(actorId, 0);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(actorId, ANIM_NOD);
		fdScriptWait(20);
		fdActorSetSpeed(actorId, 1);
		fdActorWalkByAndWait(actorId, -64, 0);
		fdActorWalkByAndWait(actorId, 0, 48);
	} else {
		fdScriptSetMessageId(MsgTolbiTiredFeelFreeRest);
		/* "Well, if you're tired, please feel free to get some rest." */
		fdScriptOpenMessage(actorId, 0);
	}
	fdScriptEnd();
}

/* While Babi is missing, the warrior in a golden helmet by the table at the
   north of the big bedroom asks "Are you planning to enter Colosso?". A Yes
   makes him wince and send the party to the colosseum; a No has him take it for
   another injured warrior. */
static void scBabiPalaceAskAboutColosso(Sint32 actorId)
{
	Sint32 question = MsgTolbiPlanningEnterColosso;

	fdScriptSetMessageId(question);
	/* "Are you planning to enter Colosso?" */
	fdScriptOpenMessage(actorId, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdScriptWait(10);
		fdActorShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		fdScriptSetMessageId(question + 1);
	} else {
		fdScriptWait(10);
		fdActorShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
		fdScriptSetMessageId(question + 2);
	}
	fdScriptShowMessage(actorId, 0);
}

/* While Babi is missing, the warrior in a golden helmet by the big bedroom's
   east wall tries to shrug off his indigestion to enter Colosso, and gives up:
   "It's no use! My stomach really hurts!" */
static void scBabiPalaceTalkToWarriorWithIndigestion(Sint32 actorId)
{
	ACTOR		 *actor;
	const Sint32 *indigestionScript;

	actor = fdActorGet(actorId);
	fdScriptStart();
	indigestionScript = BabiPalace_IndigestionScript;
	fdActorSetScript(actorId, indigestionScript);
	fdScriptSetMessageId(MsgTolbiCantFightBecauseLittleIndigestion);
	/* "I can't fight because of a little indigestion? That's pathetic!" */
	fdScriptShowMessage(actorId, 0);
	fdActorStop(actorId);
	actor->scaleY = toFIXED(1);
	actor->scaleX = toFIXED(1);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(actorId, 2);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(actorId, 2);
	fdScriptWait(60);
	/* "Maybe I can shrug off this pain in my gut... I have to enter..." */
	fdScriptShowMessage(actorId, 0);
	fdScriptWait(20);
	fdActorShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorRunRepeatedMotion(actorId, 2);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(actorId, 2);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(actorId, 2);
	fdScriptWait(30);
	fdActorSetScript(actorId, indigestionScript);
	/* "It's no use! My stomach really hurts!" */
	fdScriptShowMessage(actorId, 0);
	fdActorFaceDirection(actorId, FACING_NORTHEAST, 0);
	fdScriptWait(10);
	actor->scaleY = toFIXED(1);
	actor->scaleX = toFIXED(1);
	fdActorSetScript(actorId, indigestionScript);
	fdScriptEnd();
}

/* Mind Read on Sheba in her room, while Babi is missing, reads that even if she
   escaped the palace her power could not get her home to Lalivero. The first
   time, she turns with a start ("Did you just..."), then shakes her head: "No,
   it is nothing." */
static void scBabiPalaceReadShebasMind(Sint32 casterId, Sint32 actorId)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgTolbiEvenIfEscapedBabiPalace);
	/* "Even if I escaped Babi Palace, my power could not get me to..." */
	fdScriptShowMessage(actorId, 0);
	if(fdScriptGetFlag(FLAG_SHEBA_NOTICED_MIND_READ) == 0) {
		gmFlagSet(FLAG_SHEBA_NOTICED_MIND_READ);
		syPsynergyCancel();
		fdScriptWait(50);
		fdActorShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
		fdActorFaceActor(actorId, ACTOR_PARTY_LEADER, 40);
		/* "Did you just..." */
		fdScriptShowMessage(actorId, 0);
		fdScriptWait(30);
		fdActorSetAnimationAndWait(actorId, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "No, it is nothing." */
		fdScriptShowMessage(actorId, 0);
		fdActorFaceDirection(actorId, FACING_WEST, 0);
	}
	fdScriptEnd();
}

/* After Colosso, the old man with a cane in the entrance hall asks whether the
   party has met with Babi. */
static void scBabiPalaceAskAboutMeetingBabi(Sint32 actorId)
{
	scBabiPalaceShowQuestionAnswer(actorId, MsgTolbiMeetBabi);
}

/* After Colosso, the young man with brown hair in the room west of the
   soldiers' room asks "So, you're all from the eastern shores of the Karagol
   Sea, huh?". */
static void scBabiPalaceAskAboutEasternShores(Sint32 actorId)
{
	scBabiPalaceShowQuestionAnswer(actorId, MsgTolbiEasternShoresKaragol);
}

/* After Colosso, picking up the Cloak Ball on show beside Babi's bed: "You
   found the Cloak Ball." From his bed Babi asks whether Isaac desires it, and
   on a Yes lends it for the quest. On a No he asks to leave it alone. */
static void scBabiPalaceOfferCloakBall(void)
{
	Sint32 msg;

	fdScriptStart();
	fdEndBusyAndReveal();
	msg = MsgTolbiFoundCloakBall;
	fdScriptSetMessageId(msg);
	/* "You found the Cloak Ball." */
	fdScriptShowMessage(-1, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdScriptWait(30);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_BABI, 30);
	/* "Isaac! Is that Cloak Ball what you desire?" */
	fdScriptOpenMessage(ACTOR_BABI, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
		fdScriptSetMessageId(msg + 2);
		/* "Then please leave the Cloak Ball alone. Do not worry about..." */
		fdScriptShowMessage(ACTOR_BABI, 0);
	} else {
		fdScriptWait(20);
		fdScriptSetMessageId(msg + 3);
		/* "That Cloak Ball is very precious to me. I can't just give it..." */
		fdScriptShowMessage(ACTOR_BABI, 0);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdScriptWait(30);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		fdScriptWait(30);
		fdActorSetPos(ACTOR_CLOAK_BALL, 0, 0);
		gmItemShowFound(ITEM_CLOAK_BALL, 3);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
		gmPartyGiveItem(ITEM_CLOAK_BALL, 0);
		gmFlagSet(FLAG_BABI_PALACE_CLOAK_BALL);
	}
}

/* Losing the first or second match of the finals, Isaac wakes in a bed of the
   big bedroom: he only dreamt of fighting in them. The others tease him, and
   the party sets off for the colosseum. */
static void scBabiPalaceWakeFromFinalsDream(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgTolbiGetUp);
	fdActorGet(ACTOR_PARTY_LEADER)->sprStorageKind = ACTOR_SPRITE_NONE;
	fdActorGet(ACTOR_FINALS_ATTENDANT)->sprStorageKind = ACTOR_SPRITE_NONE;
	sySynch(1);
	REG_DISPCNT = DISPCNT_OBJ_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
	/* "Isaac! Get up, Isaac!" */
	fdScriptShowMessage(-1, 0);
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
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptWait(20);
	FieldStateWork->transitionFrm = 24;
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorWalkByAndWait(ACTOR_MIA, 16, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdScriptWait(30);
	/* "Are you okay? You were having nightmares for quite a while..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdScriptWait(10);
	fdActorGet(ACTOR_PARTY_LEADER)->z.fixed -= toFIXED(3);
	fdActorGet(ACTOR_PARTY_LEADER)->tgtZ -= toFIXED(3);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_SITTING_UP);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
	fdScriptWait(30);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
	fdScriptWait(50);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(30);
	/* "What's the matter, Isaac?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* "What? You entered and fought in the finals?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "How could you? You haven't even been to the colosseum yet,..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Sounds like you've been dreaming, Isaac." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(45);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdScriptWait(30);
	/* "You're so exhausted, it's giving you nightmares. Are you..." */
	fdScriptOpenMessage(ACTOR_GARET, 0);
	if(fdScriptAskYesNoWithActorAnimation(-1, 0) != 0) {
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdScriptWait(20);
		/* "You think you can manage? I understand, but don't overdo it,..." */
		fdScriptShowMessage(ACTOR_GARET, 0);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
		fdScriptWait(30);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdScriptWait(20);
		/* "Isaac's awake now, so let's head over to the colosseum." */
		fdScriptShowMessage(ACTOR_GARET, 0);
		FieldStateWork->msg += 1;
	} else {
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
		FieldStateWork->msg += 2;
		fdScriptWait(30);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdScriptWait(20);
		/* "Okay! We're off to the colosseum!" */
		fdScriptShowMessage(ACTOR_GARET, 0);
	}
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorWalkByAndWait(ACTOR_GARET, -16, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdScriptWait(35);
	fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.9);
	fdActorWalkByAndWait(ACTOR_PARTY_LEADER, -32, 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdScriptWait(40);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorSetSpeed(ACTOR_MIA, 1.2);
	fdActorSetSpeed(ACTOR_IVAN, 1.2);
	scWalkIntoLeader(ACTOR_GARET);
	scWalkIntoLeader(ACTOR_MIA);
	scWalkIntoLeader(ACTOR_IVAN);
	fdScriptWait(10);
	fdScriptEnd();
}

/* Searching one of the four beds in the big bedroom, once the attendant has
   offered them, asks "Would you like to sleep now?". The first night the party
   talks things over before it sleeps; later nights it sleeps as at an inn. */
static void scBabiPalaceRestInPalaceBeds(void)
{
	Sint16 roundedFacing;
	Uint16 facing;

	roundedFacing =
		FACING_NEAREST_CARDINAL(fdActorGet(ACTOR_PARTY_LEADER)->facing);
	fdScriptStart();
	fdEndBusyAndReveal();
	fdScriptSetMessageId(MsgTolbiLikeSleep);
	/* "Would you like to sleep now?" */
	fdScriptOpenMessage(-1, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		if(fdScriptGetFlag(FLAG_BABI_PALACE_FIRST_NIGHT) != 0) {
			fdScriptWait(20);
			mnInnPlaySleep(0);
			return;
		}
		fdScriptWait(20);
		facing = roundedFacing;
		if(facing == FACING_EAST) {
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 128, 120);
		}
		if(facing == FACING_WEST) {
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 240, 120);
		}
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 184, 120);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdScriptWait(10);
		syFieldActorLaunchFromLeader(ACTOR_GARET, 16, 0, FACING_WEST);
		fdActorWaitForScriptEndById(ACTOR_GARET);
		fdScriptWait(10);
		/* "Ohh, I'm so tired..." */
		fdScriptShowMessage(ACTOR_GARET, 0);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_GARET, 1);
		fdActorWalkByAndWait(ACTOR_GARET, 40, 0);
		fdActorWalkByAndWait(ACTOR_GARET, 0, -32);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
		fdScriptWait(20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
		fdScriptWait(20);
		syFieldActorLaunchFromLeader(ACTOR_IVAN, -16, 0, FACING_EAST);
		fdActorWaitForScriptEndById(ACTOR_IVAN);
		fdScriptWait(10);
		/* "I didn't think we'd ever find a place to stay. What a relief!" */
		fdScriptShowMessage(ACTOR_IVAN, 0);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_IVAN, 1);
		fdActorWalkByAndWait(ACTOR_IVAN, -40, 0);
		fdActorWalkByAndWait(ACTOR_IVAN, 0, 40);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
		fdScriptWait(20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdScriptWait(20);
		syFieldActorLaunchFromLeader(ACTOR_MIA, 16, 0, FACING_WEST);
		fdActorWaitForScriptEndById(ACTOR_MIA);
		fdScriptWait(10);
		/* "Let's rest up for tomorrow." */
		fdScriptShowMessage(ACTOR_MIA, 0);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_MIA, 1);
		fdActorWalkByAndWait(ACTOR_MIA, 40, 0);
		fdActorWalkByAndWait(ACTOR_MIA, 0, 40);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
		fdScriptWait(20);
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdActorWalkByAndWait(ACTOR_PARTY_LEADER, -56, 0);
		fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, -32);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
		fdScriptWait(30);
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdScriptWait(10);
		/* "Good night." */
		fdScriptShowMessage(ACTOR_MIA, 0);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdScriptWait(10);
		if(gmFlagGet(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
			/* "G'night." */
			fdScriptShowMessage(ACTOR_GARET, 0);
		} else {
			FieldStateWork->msg++;
		}
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdScriptWait(10);
		if(gmFlagGet(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
			/* "Sleep well." */
			fdScriptShowMessage(ACTOR_IVAN, 0);
		} else {
			FieldStateWork->msg++;
		}
		fdScriptWait(20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
		mnInnPlaySleep(0);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdScriptWait(10);
		/* "Morning." */
		fdScriptShowMessage(ACTOR_GARET, 0);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdScriptWait(10);
		if(gmFlagGet(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
			/* "Good morning." */
			fdScriptShowMessage(ACTOR_IVAN, 0);
		} else {
			FieldStateWork->msg++;
		}
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdScriptWait(10);
		if(gmFlagGet(FLAG_BABI_PALACE_FIRST_NIGHT) == 0) {
			/* "Good morning." */
			fdScriptShowMessage(ACTOR_MIA, 0);
		} else {
			FieldStateWork->msg++;
		}
		fdActorWalkByAndWait(ACTOR_GARET, 0, 32);
		fdActorWalkBy(ACTOR_GARET, -112, 0);
		fdActorWalkByAndWait(ACTOR_MIA, 0, -40);
		fdActorWalkBy(ACTOR_MIA, -112, 0);
		fdScriptWait(50);
		fdActorWalkByAndWait(ACTOR_IVAN, 0, -24);
		fdActorWaitForMoveById(ACTOR_GARET);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		fdActorWalkByAndWait(ACTOR_GARET, 0, -16);
		fdActorWaitForMoveById(ACTOR_MIA);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
		fdScriptWait(20);
		/* "All right, Isaac! We've got another hard day of work ahead..." */
		fdScriptShowMessage(ACTOR_GARET, 0);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_GARET, 1.2);
		fdActorSetSpeed(ACTOR_IVAN, 1.2);
		fdActorSetSpeed(ACTOR_MIA, 1.2);
		scWalkIntoLeader(ACTOR_GARET);
		scWalkIntoLeader(ACTOR_MIA);
		scWalkIntoLeader(ACTOR_IVAN);
		gmFlagSet(FLAG_BABI_PALACE_FIRST_NIGHT);
	}
	fdScriptEnd();
}

/* While Babi is missing, walking north up the passage towards the red carpet,
   metatile (32, 26) or (33, 26), Isaac stops short of the carpet as Iodem walks
   in from the west hall and two soldiers hurry up to him, back from Altmiller
   Cave: Babi has been gone since last night, and he has slipped into that cave
   before. Iodem then leaves for the colosseum in Babi's stead. It plays
   once. */
static void scBabiPalaceHearSoldiersReport(void)
{
	gmFlagSet(FLAG_IODEM_HEARD_SOLDIERS);
	fdScriptStart();
	fdEndBusyAndReveal();
	fdScriptSetMessageId(MsgTolbiIodemIodem);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 520, 424);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdCameraMoveTo(560, CAMERA_KEEP, 368, 1);
	fdCameraWaitForMove();
	fdActorSetSpeed(ACTOR_IODEM, 1);
	fdActorWalkByAndWait(ACTOR_IODEM, 40, 0);
	fdActorWalkToAndWait(ACTOR_IODEM, 584, 360);
	/* "Iodem, Iodem!" */
	fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_IODEM, FACING_WEST, 0);
	fdActorSetSpeed(ACTOR_FIRST_SOLDIER, 2);
	fdActorSetSpeed(ACTOR_SECOND_SOLDIER, 2);
	fdActorWalkTo(ACTOR_FIRST_SOLDIER, 528, 352);
	fdActorWalkToAndWait(ACTOR_SECOND_SOLDIER, 528, 368);
	sySynch(3);
	fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_STAND);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdScriptWait(30);
	/* "We went to Altmiller Cave, as you commanded." */
	fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	fdActorWalkByAndWait(ACTOR_IODEM, -16, 0);
	fdScriptWait(10);
	/* "And what have you? What news is there of Babi?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "This is futile..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorSetSpeed(ACTOR_FIRST_SOLDIER, 1);
	fdActorWalkByAndWait(ACTOR_FIRST_SOLDIER, 8, 0);
	fdScriptWait(20);
	/* "Iodem, why did you send us to look for Babi in a place like..." */
	fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorSetSpeed(ACTOR_SECOND_SOLDIER, 1);
	fdActorWalkByAndWait(ACTOR_SECOND_SOLDIER, 8, 0);
	fdScriptWait(20);
	/* "Why would Lord Babi enter such a dangerous cave alone? It's..." */
	fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Inconceivable? Is that what you were going to say?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdScriptWait(20);
	/* "Altmiller Cave is an evil place." */
	fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "If Babi went there alone, a monster might have..." */
	fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "I'm aware of Altmiller Cave's...eccentricities." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "You knew? Then why would you send us in there, Iodem?" */
	fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Lord Babi has left in secret before..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
	fdScriptWait(20);
	/* "You mean, this isn't the first time Lord Babi has..." */
	fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdScriptWait(20);
	/* "Do you know where he went before?" */
	fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "I followed Lord Babi once..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(20);
	fdActorFaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
	fdActorFaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
	fdActorFaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
	fdScriptWait(20);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "I was certain he had gone into Altmiller Cave..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorShowEmote(
		ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Did you ever investigate the cave, Iodem?" */
	fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdScriptWait(30);
	/* "What was he doing in the cave?" */
	fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdScriptWait(10);
	fdActorSetAttachedEffect(
		ACTOR_IODEM, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(30);
	/* "Once he entered the cave, Lord Babi vanished before my very..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Vanished?" */
	fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "You mean you lost sight of Lord Babi. Is that it?" */
	fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "After he entered the mouth of the cave, I followed him......" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(20);
	fdActorFaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
	fdActorFaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
	fdActorFaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
	fdScriptWait(20);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "I tried following him several times after that. Each time,..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdScriptWait(20);
	/* "And you think that's where Lord Babi went this morning, Iodem?" */
	fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdScriptWait(30);
	/* "But if he always returned from Altmiller Cave before..." */
	fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
	fdScriptWait(20);
	/* "Then he should be back soon, right?" */
	fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "No. This time is different. Something puzzles me..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdScriptWait(20);
	/* "What do you mean?" */
	fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "It is certainly strange that Lord Babi should go right..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorShowEmote(
		ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "What is it?" */
	fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Lord Babi's bedding was not disturbed this morning." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "What's unusual about that?" */
	fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Last night, Lord Babi did not use his bed. That is the problem." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(20);
	fdActorFaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
	fdActorFaceActor(ACTOR_FIRST_SOLDIER, ACTOR_IODEM, 0);
	fdActorFaceActor(ACTOR_SECOND_SOLDIER, ACTOR_IODEM, 0);
	fdScriptWait(20);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "Lord Babi has, in fact, been missing since last night." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAttachedEffect(
		ACTOR_FIRST_SOLDIER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_SECOND_SOLDIER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
	fdScriptWait(20);
	/* "And that is why I am so worried. He has never done this before." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "We didn't know he'd been missing so long..." */
	fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "You don't think Lord Babi's been ki-" */
	fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_IODEM, FACING_WEST, 0);
	fdScriptWait(20);
	/* "Speak no such foolishness! I am sure Lord Babi is alive and..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	fdScriptWait(30);
	/* "Ah, yes, of course." */
	fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "I will go to the colosseum in Lord Babi's stead. You must..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdScriptWait(30);
	/* "We will all conduct another round of searches." */
	fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdScriptWait(40);
	fdActorFaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
	fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_SOUTH, 0);
	fdScriptWait(40);
	fdActorSetSpeed(ACTOR_FIRST_SOLDIER, 1.3);
	fdActorSetSpeed(ACTOR_SECOND_SOLDIER, 1.3);
	fdActorWalkBy(ACTOR_FIRST_SOLDIER, 0, 120);
	fdActorWalkByAndWait(ACTOR_SECOND_SOLDIER, 0, 120);
	fdActorSetPos(ACTOR_FIRST_SOLDIER, 0, 0);
	fdActorSetPos(ACTOR_SECOND_SOLDIER, 0, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(40);
	fdActorSetSpeed(ACTOR_IODEM, 0.8);
	fdActorWalkByAndWait(ACTOR_IODEM, -16, 0);
	fdScriptWait(30);
	/* "Lord Babi, where have you gone?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
	fdScriptWait(30);
	fdActorSetSpeed(ACTOR_IODEM, 1);
	fdActorWalkByAndWait(ACTOR_IODEM, 120, 0);
	fdActorWalkByAndWait(ACTOR_IODEM, 60, 0);
	fdActorSetPos(ACTOR_IODEM, 0, 0);
	fdScriptEnd();
}

/* The waking's window round the leader (WINDOW_TRANSITION_LEADER): inside it
   every layer shows without the colour effect; outside it at first only BG0,
   then every layer once it is WAKE_WINDOW_OPEN. */
#define WAKE_WININ \
	((WININ_WIN0_ALL & ~WININ_WIN0_CLR) | (WININ_WIN1_ALL & ~WININ_WIN1_CLR))
#define WAKE_WINDOW_OPEN 31

/* After the last match of the finals, Isaac wakes in a bed of the big bedroom,
   having slept long after fighting to the end, and the others tell him Babi
   wants to see the party. */
static void scBabiPalaceWakeAfterFinals(void)
{
	Sint32					 step;
	Sint32					 pick;
	FIELD_STATE				*fieldState;
	SCREEN_TRANSITION_STATE *state;

	fdScriptStart();
	fdScriptSetMessageId(MsgTolbiCallsName);
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
	fdScreenTransitionStartHeldWindow(WINDOW_TRANSITION_LEADER);
	state = ScreenTransitionStateWork;
	state->value = 0;
	state->insideLayers = WAKE_WININ;
	state->outsideLayers = WINOUT_WIN01_BG0;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	for(step = 1; step <= 5; step++) {
		sySynch(3);
		state->value = step;
	}
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(30);
	for(step = 5; step <= WAKE_WINDOW_OPEN; step++) {
		sySynch(3);
		state->value = step;
	}
	state->outsideLayers = WINOUT_WIN01_ALL & ~WINOUT_WIN01_CLR;
	fieldState = FieldStateWork;
	fieldState->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fieldState->transitionFrm = 24;
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdScriptWait(30);
	/* "Isaac..." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdScriptWait(10);
	/* "Huh?" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdScriptWait(30);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorWalkByAndWait(ACTOR_MIA, 0, -40);
	fdActorWalkByAndWait(ACTOR_MIA, 32, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdScriptWait(10);
	/* "You've awakened." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorGet(ACTOR_PARTY_LEADER)->z.fixed -= toFIXED(3);
	fdActorGet(ACTOR_PARTY_LEADER)->tgtZ -= toFIXED(3);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_SITTING_UP);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
	fdScriptWait(30);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Colosso? Duh! That ended long ago!" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	/* "You fell into a deep sleep... Don't you remember?" */
	fdScriptOpenMessage(ACTOR_IVAN, 0);
	if(fdScriptAskYesNoWithActorAnimation(-1, 0) == 0) {
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
		fdScriptWait(20);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
		fdActorJump(ACTOR_GARET, 4, 13);
		fdActorJump(ACTOR_GARET, 4, 30);
		/* "Oh, gimme a break. You were out cold! You don't remember a..." */
		fdScriptShowMessage(ACTOR_GARET, 0);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
		fdScriptWait(20);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
		fdActorJump(ACTOR_GARET, 4, 13);
		fdActorJump(ACTOR_GARET, 4, 30);
		FieldStateWork->msg++;
		/* "Of course he doesn't! He was out cold! ...You had us worried!" */
		fdScriptShowMessage(ACTOR_GARET, 0);
	}
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(30);
	/* "I'm sorry for shouting at you." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
	fdScriptWait(50);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdScriptWait(35);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(30);
	/* "You fought brilliantly to the end, Isaac. Babi was singing..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	/* "Are you okay now? In body and mind?" */
	fdScriptOpenMessage(ACTOR_IVAN, 0);
	if(fdScriptAskYesNoWithActorAnimation(-1, 0) == 0) {
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_NOD);
		fdScriptWait(20);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
		/* "He'd better be all right, after sleeping this long. Sheesh!" */
		fdScriptShowMessage(ACTOR_GARET, 0);
		pick = 0;
		FieldStateWork->msg++;
	} else {
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_IN_BED_SHAKE_HEAD);
		fdScriptWait(20);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
		FieldStateWork->msg++;
		pick = 1;
		/* "If you feel out of sorts, it's probably because you slept..." */
		fdScriptShowMessage(ACTOR_GARET, 0);
	}
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(30);
	/* "I'm sorry for shouting again." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
	fdScriptWait(50);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdScriptWait(35);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(30);
	/* "Babi said he wanted to see you when you came to, Isaac." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	if(pick == 0) {
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdScriptWait(30);
		/* "If you're okay, shall we go see Babi?" */
		fdScriptShowMessage(ACTOR_IVAN, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdScriptWait(30);
		/* "You're fine! Aren't you, Isaac?" */
		fdScriptShowMessage(ACTOR_GARET, 0);
		FieldStateWork->msg += 2;
	} else {
		FieldStateWork->msg += 2;
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "I'm not sure Isaac's up to it just yet." */
		fdScriptShowMessage(ACTOR_IVAN, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdScriptWait(20);
		/* "What is it? Feeling weak? Fresh air and a walk will fix..." */
		fdScriptShowMessage(ACTOR_GARET, 0);
	}
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(30);
	/* "I'm more worried about what Babi wants. Let's get going!" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(20);
	fdActorFaceEachOther(ACTOR_IVAN, ACTOR_MIA, 40);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdScriptWait(30);
	while((PerData & KEYS_DPAD) == 0) {
		sySynch(1);
	}
	fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.9);
	fdActorWalkByAndWait(ACTOR_PARTY_LEADER, -32, -8);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdScriptWait(20);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdScriptWait(20);
	/* "Are you sure you should be getting up so suddenly like that?" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	/* "He's fine. He doesn't want to lounge around all day!" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdScriptWait(30);
	/* "I see. Let's go see Babi then." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorSetSpeed(ACTOR_MIA, 1.2);
	fdActorSetSpeed(ACTOR_IVAN, 1.2);
	scWalkIntoLeader(ACTOR_GARET);
	scWalkIntoLeader(ACTOR_MIA);
	scWalkIntoLeader(ACTOR_IVAN);
	fdScriptWait(10);
	fdScriptEnd();
}

/* After Colosso, the first time the party goes through the throne room's door
   north of the red carpet, Babi and Iodem are waiting. Iodem hands the winner
   the Lure Cap; Babi tells of Lemuria and the draught that has kept him alive,
   asks the party to sail there for him once its business at Venus Lighthouse is
   done, and sends Iodem with them, to meet at Gondowan Passage. */
static void scBabiPalaceMeetBabiAfterColosso(void)
{
	sySndPlay(MUS_BABIS_THEME);
	fdScriptStart();
	fdScriptSetMessageId(MsgTolbiWaitingCompanions);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdCameraMoveTo(216, CAMERA_KEEP, 736, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdScriptWait(10);
	/* "I've been waiting for you and your companions, Isaac!" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_BABI, 0.8);
	fdActorWalkByAndWait(ACTOR_BABI, 0, 16);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "I've been waiting here because I wanted to speak with you." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(20);
	/* "It's hard to talk when you're so far away... Come over here." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
	fdCameraWaitForMove();
	fdScriptWait(40);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 208, 760);
	fdScriptWait(10);
	fdCameraMoveTo(216, CAMERA_KEEP, 736, 1);
	syFieldActorLaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
	syFieldActorLaunchFromLeader(ACTOR_MIA, 0, 24, FACING_NORTH);
	syFieldActorLaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "The battles at Colosso were spectacular!" */
	fdScriptShowMessage(ACTOR_BABI, 0);

	/* If the leader won the finals, Iodem brings the victor's prize, the Lure
	   Cap; otherwise Babi only praises how far the party came. */
	if(gmFlagGet(FLAG_WON_COLOSSO) != 0) {
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdScriptWait(20);
		/* "It was most rewarding to see Babi's favored warrior steal..." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorFaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
		fdScriptWait(30);
		fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
		fdScriptWait(20);
		/* "Iodem, bring him the victor's prize." */
		fdScriptShowMessage(ACTOR_BABI, 0);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdScriptWait(20);
		/* "Yes, sir, right away." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_IODEM, 1);
		fdActorWalkByAndWait(ACTOR_IODEM, 0, -16);
		fdScriptWait(40);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdScriptWait(40);
		fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
		fdScriptWait(20);
		fdActorWalkByAndWait(ACTOR_IODEM, 0, 32);
		fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
		fdScriptWait(10);
		fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
		fdScriptWait(30);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdScriptWait(30);
		fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_IODEM, 0.8);
		fdActorGet(ACTOR_IODEM)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkByAndWait(ACTOR_IODEM, 0, -16);
		fdActorGet(ACTOR_IODEM)->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
		fdScriptWait(40);
		fdActorSetSpeed(ACTOR_BABI, 0.8);
		fdActorWalkByAndWait(ACTOR_BABI, 0, 16);
		fdScriptWait(40);
		mnMessageArgumentPush(ITEM_LURE_CAP, MESSAGE_ARGUMENT_ITEM);
		/* "Isaac received ...." */
		fdScriptShowMessage(-1, 0);
		gmItemShowFound(ITEM_LURE_CAP, 3);
		gmPartyGiveItem(ITEM_LURE_CAP, 0);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdScriptWait(30);
		fdActorWalkByAndWait(ACTOR_BABI, 0, -16);
		fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
		fdScriptWait(30);
		fdActorRunRepeatedMotion(ACTOR_BABI, 2);
		fdScriptWait(20);
		/* "Incidentally, Isaac, are you feeling better now?" */
		fdScriptOpenMessage(ACTOR_BABI, 0);
		FieldStateWork->msg += 2;
	} else {
		FieldStateWork->msg += 5;
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdScriptWait(20);
		/* "It was wonderful to see Babi's handpicked warrior make it so..." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdActorFaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
		fdScriptWait(40);
		fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdScriptWait(30);
		fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
		fdScriptWait(30);
		fdActorRunRepeatedMotion(ACTOR_BABI, 2);
		fdScriptWait(20);
		/* "Incidentally, Isaac, are you feeling better now?" */
		fdScriptOpenMessage(ACTOR_BABI, 0);
	}

	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdScriptWait(20);
		/* "Ah, the recuperative powers of youth...and after such fierce..." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdScriptWait(20);
		FieldStateWork->msg++;
		/* "You must be exhausted. I'm amazed to see you on your feet..." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
	}

	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_NORTHWEST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "Who could expect less from the warriors who rescued me?" */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorJump(ACTOR_GARET, 4, 13);
	fdActorJump(ACTOR_GARET, 4, 30);
	/* "Enough with the flattery. Let's get to the point: why were..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_BABI, 1.6);
	fdActorWalkByAndWait(ACTOR_IODEM, 0, 16);
	sySynch(2);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	fdScriptWait(10);
	/* "Such insolence! Who do you think you are, to speak to Babi so?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdScriptWait(30);
	fdActorFaceActor(ACTOR_BABI, ACTOR_IODEM, 30);
	/* "Let it be, Iodem. There are other ways to judge a man." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	fdActorFaceEachOther(ACTOR_BABI, ACTOR_GARET, 0);
	fdScriptWait(40);
	/* "What is it you said you were called?" */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(30);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 30);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_GARET, 30);
	/* "He's talking to you, Garet." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "That's right, Garet. We shall explain all." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "We intended to thank you and your companions for saving..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "But it would seem that gratitude is not enough..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "I have disclosed everything to Iodem." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "What do you mean by everything?" */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdScriptWait(20);
	/* "You do wish to know more about Lemuria, correct?" */
	fdScriptOpenMessage(ACTOR_BABI, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
		fdScriptWait(30);
		/* "Yes, indeed... Your desire is as plain as day." */
		fdScriptShowMessage(ACTOR_BABI, 0);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		FieldStateWork->msg++;
		/* "You can't hide your interest from me. It is written on your..." */
		fdScriptShowMessage(ACTOR_BABI, 0);
	}

	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "I have just seen Psynergy for the first time... It is an..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(50);
	fdActorFaceActor(ACTOR_BABI, ACTOR_IODEM, 60);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdScriptWait(20);
	/* "I used my Cloak Ball to show him Psynergy." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "No one else could see it, but I knew Isaac's tournament was..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "I thought it unlikely that you would fight using such power..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "No need to be ashamed, Isaac. Psynergy is also a warrior's..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "When I first learned of your power, I was shocked. I thought..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "You have mentioned Lemuria many times, Babi. What is it?" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_BABI, 50);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdScriptWait(20);
	/* "It is the city of an ancient people, lying far out at sea." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(20);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdScriptWait(50);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	fdScriptWait(20);
	/* "Many years ago, about the time of the great flood, Babi went..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	/* "I see..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
	fdScriptWait(40);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	/* "The great flood struck while I was traveling with Lunpa." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Lunpa... You mean Lunpa the thief?" */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdScriptWait(10);
	fdActorFaceEachOther(ACTOR_BABI, ACTOR_IVAN, 40);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "I know it is hard to believe... It happened a hundred years ago." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 60);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdScriptWait(50);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "You mean...those rumors about you are...true?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	/* "Kraden once mentioned the Stone of Sages. Do you think..." */
	fdScriptOpenMessage(ACTOR_GARET, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdScriptWait(20);
		/* "So the stone that grants immortality came from Lemuria?" */
		fdScriptShowMessage(ACTOR_GARET, 0);
		FieldStateWork->msg += 3;
	} else {
		fdScriptWait(10);
		FieldStateWork->msg++;
		/* "Huh? Don't tell me you forgot about the Stone of Sages..." */
		fdScriptOpenMessage(ACTOR_GARET, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			fdScriptWait(20);
			/* "It rules all, can change anything into gold, grants..." */
			fdScriptShowMessage(ACTOR_GARET, 0);
			FieldStateWork->msg++;
		} else {
			FieldStateWork->msg++;
			/* "I can't think of anything else that would let him live
			   for..." */
			fdScriptShowMessage(ACTOR_GARET, 0);
		}
	}

	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "So you've even heard of the Stone of Sages... I'm afraid I..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "It is the draught of Lemuria that has allowed me to live for..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "The draught of Lemuria?" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdScriptWait(20);
	/* "It is a magical draught once taken by those who lived in..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	/* "And just drinking stops you from aging?" */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdScriptWait(10);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "Is that what it does?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
	/* "Hmmm... It would be more accurate to say it slows the aging..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "There is death in Lemuria..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "But it only comes to those whose senses have failed from..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(30);
	/* "Do the people of Lemuria use Psynergy, like we do?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "You have that draught, Babi... That's why you've lived so long." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(40);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_MIA, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_IVAN, 0);
	fdScriptWait(50);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	/* "And become wealthy enough to rule Tolbi." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "What are you getting at?" */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "What is your purpose in making everyone study Alchemy?" */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdScriptWait(30);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_BABI, 30);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdScriptWait(20);
	/* "I'm starting to wonder myself. What do you hope to gain, Babi?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "Ah. If I had attained true immortality... There would be no..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "I'm not sure I understand." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "I had precious little draught, and now I am running out." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdScriptWait(30);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "You all saw it. There was but little left in the cave..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "How long will it last? And what will happen when it runs out?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(20);
	fdActorFaceEachOther(ACTOR_BABI, ACTOR_IODEM, 40);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdScriptWait(20);
	/* "It will run out any day now. And when it does, my life is at..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorSetAttachedEffect(
		ACTOR_IODEM, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "Why didn't you tell us sooner? We must hurry to Lemuria!" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdActorSetSpeed(ACTOR_IODEM, 1.6);
	fdActorWalkByAndWait(ACTOR_IODEM, 0, 24);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IODEM, 0);
	fdActorFaceActor(ACTOR_MIA, ACTOR_IODEM, 0);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_IODEM, 0);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	/* "It is pointless..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_BABI, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "What on earth do you mean, pointless?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(30);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "We cannot find Lemuria." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetSpeed(ACTOR_IODEM, 1.2);
	fdActorWalkByAndWait(ACTOR_IODEM, 0, -24);
	fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
	fdScriptWait(20);
	/* "But you just said that it lies far out at sea..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "If we search the seas, we're sure to find it!" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "I've sent so many ships in the past in search of Lemuria." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "This must have been long before I joined you... But still..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "I needed more Lemurian draught, but I couldn't find Lemuria." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	/* "Why did you collapse in the cave if you still had more draught?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "I have been rationing it, but this time, I waited too long..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "You were trying to make it last longer..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "I overexerted myself trying to find Lemuria before it ran out." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(30);
	/* "And your reason for calling us here has to do with Lemuria?" */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "Exactly. I want you to find Lemuria." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdScriptWait(20);
	/* "I have not been able to locate Lemuria by sea, but perhaps..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "I can hide it no longer... I have built Babi Lighthouse to..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "What? That's why you're building the lighthouse?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorFaceEachOther(ACTOR_BABI, ACTOR_IODEM, 30);
	/* "No, there's more to it than that." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "We've spotted something in the ocean southeast of the..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	/* "Something dangerous, no doubt. What, monsters? We can handle..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdScriptWait(20);
	/* "That may be... However, something else troubles me more." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "What is it, my lord?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
	fdScriptWait(20);
	/* "No sailor has been able to keep his bearings at sea." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "You're kidding! You mean they can't even keep a straight course?" */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdScriptWait(20);
	/* "Is there something that distorts distance or direction or..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "The currents shift there, subtly changing a ship's direction." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(30);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	/* "So the lighthouse will help the ships keep their bearings..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "So, how exactly did you get away from Lemuria after the..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "That is the thing, Garet!" */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "I crossed the sea in a Lemurian ship. I am hoping you can..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "But why us, when you have so many powerful soldiers at your..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "None of them have the ability to use Psynergy." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "One must have Psynergy to sail this ship!" */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Then that would mean that Babi is an Adept, wouldn't it?" */
	fdScriptOpenMessage(ACTOR_IVAN, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_BABI, 0) == 0) {
		fdScriptWait(10);
		/* "That must be it. No normal person could use that Cloak Ball." */
		fdScriptShowMessage(ACTOR_IVAN, 0);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(10);
		FieldStateWork->msg++;
		/* "But, Isaac, no normal person could master the Cloak Ball!" */
		fdScriptShowMessage(ACTOR_IVAN, 0);
	}

	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdScriptWait(20);
	/* "I learned to use a little Psynergy during my stay in Lemuria." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "With what little I'd learned, I was able to guide the ship." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
	fdScriptWait(20);
	/* "In that case, perhaps... I could sail the ship." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorFaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdScriptWait(30);
	/* "No. It must be an Adept." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 40);
	/* "What should we do, Isaac? Will you do what he wants?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	fdScriptWait(30);
	/* "If you do, I will grant you whatever you wish." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdScriptWait(30);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 60);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdScriptWait(30);
	/* "You shouldn't make vows you will later find you cannot keep..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "I haven't even considered my own wishes lately." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdScriptWait(10);
	fdActorFaceEachOther(ACTOR_IODEM, ACTOR_BABI, 60);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "In that case, I could provide you with a wealth of items you..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "Babi's offer is very generous... You will consider going,..." */
	fdScriptOpenMessage(ACTOR_IODEM, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdScriptWait(20);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		/* "Isaac, we have to beat Felix and the others to the lighthouse." */
		fdScriptShowMessage(ACTOR_GARET, 0);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(10);
		FieldStateWork->msg++;
		/* "Not this time. We have to beat Felix and the others to the..." */
		fdScriptShowMessage(ACTOR_GARET, 0);
	}

	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "The lighthouse!?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Do you mean Venus Lighthouse?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Well then, our paths are intertwined. I, too, must go to..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "But we cannot get to the top of Venus Lighthouse, can we?" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorFaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "These Adepts may be able to solve the riddle that blocks us." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "Ah, I see... The thought hadn't occurred to me." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdScriptWait(20);
	/* "Isaac! You shall go to Venus Lighthouse with Iodem." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Do not worry... You can take care of your own business first." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdScriptWait(30);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
	/* "I see... If they can solve the riddle of the lighthouse......" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	/* "So, we can search for Lemuria after they complete their own..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	/* "Is that acceptable?" */
	fdScriptOpenMessage(ACTOR_BABI, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdScriptWait(20);
		fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
		/* "Ah, you will undertake this quest! I am most grateful!" */
		fdScriptShowMessage(ACTOR_BABI, 0);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(10);
		fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
		FieldStateWork->msg++;
		/* "If you need time to make up your mind, then please do so." */
		fdScriptShowMessage(ACTOR_BABI, 0);
	}

	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "In any case, you will need my assistance to reach the..." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorFaceActor(ACTOR_BABI, ACTOR_IODEM, 40);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	/* "I'll make preparations so that I am ready to leave at any time." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorFaceActor(ACTOR_IODEM, ACTOR_BABI, 40);
	/* "I take my leave now to make preparations." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
	fdScriptWait(20);
	/* "Then you can all meet at Gondowan Passage..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "That's a brilliant idea!" */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdScriptWait(20);
	/* "One more thing. I'd like you to think about a suitable reward." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptWait(20);
	/* "Yes. I will take care of that immediately, as well." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
	fdScriptWait(20);
	/* The party tracks each departing speaker before gathering back
	   together. */
	fdActorWatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_IODEM);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_IODEM);
	fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_IODEM);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_IODEM);
	fdActorSetSpeed(ACTOR_IODEM, 1);
	fdActorWalkByAndWait(ACTOR_IODEM, 0, 32);
	fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdScriptWait(20);
	/* "I shall be waiting for you at the passage." */
	fdScriptShowMessage(ACTOR_IODEM, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
	fdScriptWait(30);
	fdActorWalkByAndWait(ACTOR_GARET, 16, 0);
	fdActorWalkBy(ACTOR_IODEM, 0, 80);
	fdScriptWait(40);
	fdActorWalkByAndWait(ACTOR_GARET, -16, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorWaitForMoveById(ACTOR_IODEM);
	fdScriptWait(80);
	fdActorSetPos(ACTOR_IODEM, 0, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdScriptWait(30);
	fdActorWatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_BABI);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_BABI);
	fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_BABI);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_BABI);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetSpeed(ACTOR_BABI, 0.8);
	fdActorWalkByAndWait(ACTOR_BABI, 0, 24);
	fdActorWalkByAndWait(ACTOR_BABI, -80, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_EAST, 0);
	fdScriptWait(20);
	/* "Your first event shall be the riddles of Venus Lighthouse. I..." */
	fdScriptShowMessage(ACTOR_BABI, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdScriptWait(20);
	fdActorWalkByAndWait(ACTOR_BABI, 0, 48);
	fdActorWalkByAndWait(ACTOR_BABI, -64, 0);
	fdActorStop(ACTOR_PARTY_LEADER);
	fdActorStop(ACTOR_GARET);
	fdActorStop(ACTOR_MIA);
	fdActorStop(ACTOR_IVAN);
	fdActorSetPos(ACTOR_BABI, 0, 0);
	fdScriptWait(20);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_MIA, 0);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_IVAN, 0);
	fdScriptWait(30);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(30);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorSetSpeed(ACTOR_IVAN, 1.2);
	fdActorSetSpeed(ACTOR_MIA, 1.2);
	scWalkIntoLeader(ACTOR_GARET);
	scWalkIntoLeader(ACTOR_IVAN);
	scWalkIntoLeader(ACTOR_MIA);
	fdScriptWait(10);
	gmPlayMusic();
	fdScriptEnd();
}

/* Walking north into one of the palace's wooden doors opens it with the sound
   of a door, and Isaac walks through. The doors are the big bedroom's two, up
   the stairs on either side of Babi's statue, metatiles (36, 3) and (59, 3);
   the throne room's, north of the red carpet, (32, 18) and (33, 18); the two at
   the south end of the passage up to the carpet, into the room west of the
   soldiers' room (29, 31) and into the soldiers' room (36, 31); Babi's
   bedroom's off its landing, (28, 50); and Sheba's, off hers, (5, 34).

   Each door opens by its own tileset animation (BABI_PALACE_DOOR_ANIMATION);
   the metatile behaviour is the exit's number. */
static void scBabiPalaceLeaveThroughDoor(void)
{
	FIELD_STATE *state;

	state = FieldStateWork;
	fdScriptStart();
	sySndPlay(SE_DOOR_OPEN);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	if(state->stepCue == BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM) {
		fdTilesetAnimResume(BABI_PALACE_MEETING_ROOM_DOOR_ANIMATION);
		fdScriptWait(10);
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
	} else {
		if(state->stepCue == BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM) {
			fdTilesetAnimResume(BABI_PALACE_OVEN_ROOM_DOOR_ANIMATION);
			fdScriptWait(10);
			fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
		} else {
			fdTilesetAnimResume(BABI_PALACE_OTHER_DOORS_ANIMATION);
			fdScriptWait(10);
			fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
		}
	}
	fdScriptWait(16);
	syImportScriptSetExitRequest(state->stepCue);
	scBabiPalacePauseDoorAnimations();
	fdScriptEnd();
}

/* Walking north into the palace's door between the two guards, metatile (12,
   15), opens it and Isaac goes in. */
static void scBabiPalaceLeaveThroughAnimatedDoor(void)
{
	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(BabiPalace_DoorMetatiles, 36, 10);
	fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
	fdScriptWait(16);
	syImportScriptSetExitRequest(BABI_PALACE_ENTRANCE_TO_BABI_PALACE);
	fdScriptEnd();
}

/* During the finals, talking to either guard at the palace door the first time
   brings Isaac before them: "You're Isaac, aren't you?" Either answer ends with
   Babi's word that he waits for the party at the colosseum, though No first has
   the guards confer and decide it was him all the same. */
static void scBabiPalacePassOnBabisWord(void)
{
	Sint32 question;
	gmFlagSet(FLAG_BABI_PALACE_GUARD_GAVE_WORD);
	fdScriptStart();
	fdEndBusyAndReveal();
	fdActorFaceDirection(ACTOR_FIRST_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 200, 272);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdScriptWait(20);
	question = MsgTolbiArent;
	fdScriptSetMessageId(question);
	/* "You're Isaac, aren't you?" */
	fdScriptOpenMessage(ACTOR_FIRST_GUARD, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdScriptWait(20);
		fdScriptSetMessageId(question + 1);
		/* "Babi left word that he would be waiting for you at the..." */
		fdScriptShowMessage(ACTOR_FIRST_GUARD, 0);
	} else {
		fdScriptWait(20);
		fdScriptSetMessageId(question + 2);
		/* "Oh, I guess I was mistaken..." */
		fdScriptShowMessage(ACTOR_FIRST_GUARD, 0);
		fdScriptWait(20);
		fdActorFaceEachOther(ACTOR_FIRST_GUARD, ACTOR_SECOND_GUARD, 60);
		fdActorFaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTH_SOUTHEAST, 0);
		fdScriptWait(40);
		fdActorRunRepeatedMotion(ACTOR_SECOND_GUARD, 2);
		fdScriptWait(30);
		fdActorFaceEachOther(ACTOR_FIRST_GUARD, ACTOR_SECOND_GUARD, 30);
		fdActorSetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
		fdScriptWait(30);
		fdActorShowEmote(ACTOR_FIRST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
		fdActorFaceDirection(ACTOR_FIRST_GUARD, FACING_SOUTH_SOUTHWEST, 0);
		fdActorFaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTH_SOUTHEAST, 0);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_FIRST_GUARD, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "No, I'm certain it was you." */
		fdScriptShowMessage(ACTOR_FIRST_GUARD, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_FIRST_GUARD, 2);
		fdScriptWait(20);
		/* "Babi left word that he would be waiting for you at the..." */
		fdScriptShowMessage(ACTOR_FIRST_GUARD, 0);
	}
	fdScriptEnd();
}

/* During the finals, the guard on the west side of the palace door thanks Isaac
   once for saving Babi, then reminds him that Babi waits at the colosseum. */
static void scBabiPalaceThankForSavingBabi(void)
{
	if(fdScriptGetFlag(FLAG_BABI_PALACE_GUARD_THANKED_PARTY) == 0) {
		gmFlagSet(FLAG_BABI_PALACE_GUARD_THANKED_PARTY);
		fdScriptSetMessageId(MsgTolbiIsaacIdReallyLikeThank);
		/* "Isaac, I'd really like to thank you for saving Babi. He is..." */
		fdScriptShowMessage(ACTOR_SECOND_GUARD, 0);
	} else {
		fdScriptSetMessageId(MsgTolbiBabiWaitingForAtColosseum);
		/* "Babi is waiting for you at the colosseum. He wants you to go..." */
		fdScriptShowMessage(ACTOR_SECOND_GUARD, 0);
	}
}

/* After Colosso, the guard on the west side of the palace door says all the
   warriors who stayed in the palace have left, and asks whether the party is
   going too. */
static void scBabiPalaceAskAboutRemainingWarriors(Sint32 actorId)
{
	Sint32 question = MsgTolbiWarriorsWhoStayed;

	fdScriptSetMessageId(question);
	/* "All the warriors who stayed in the palace left when Colosso..." */
	fdScriptOpenMessage(actorId, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdScriptSetMessageId(question + 1);
		/* "Oh, that's too bad. I bet we could have had a good time..." */
		fdScriptShowMessage(actorId, 0);
	} else {
		fdScriptSetMessageId(question + 2);
		/* "Huh? I thought for sure you'd be going, too. I guess not..." */
		fdScriptShowMessage(actorId, 0);
	}
}

/* Searching the well in the court, south-west of the palace door at metatile
   (6, 19): "It's filled with fresh, clean water." */
static void scBabiPalaceExamineWell(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgFieldPeeredWell, 1);
	mnFieldMessageShow(MsgTolbiItsFilledWithFreshClean, 1);
	fdScriptEnd();
}

/* Returns the cues for the scene and the story, as for the actor placements. */
const CUE *scGetCues(void)
{
	if(MainRoutineStateWork.next == SCENE_BABI_PALACE_ENTRANCE) {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			return BabiPalace_OutsideCuesAfterColosso;
		}
		if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
			return BabiPalace_OutsideCuesFinals;
		}
		return BabiPalace_OutsideCuesBabiMissing;
	}
	if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
		return BabiPalace_CuesAfterColosso;
	}
	if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
		return BabiPalace_CuesFinals;
	}
	return BabiPalace_CuesBabiMissing;
}

/* Sets up the scene being entered. Returns 0. */
Sint32 scInit(void)
{
	ACTOR		  *actor;
	SPR			  *sprite;
	ICON_TRANSFER *glyph;
	Sint32		   set;

	scBabiPalacePauseDoorAnimations();
	/* XXX (no evidence): what BABI_PALACE_AFTER_BABI_SAVED and
	   BABI_PALACE_AFTER_COLOSSO were made for; only the debug scene selector
	   reaches them, and nothing in the game says why they set these story
	   flags. */
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
			} else {
				actor = fdActorGet(ACTOR_CLOAK_BALL);
				actor->retainSprCache = 1;
				actor->motionFlg = set;
				sprite = actor->spr;
				actor->y.fixed = toFIXED(4);
				sprite->cnt = set;
				sprite->image.fullCol = 0;
				sprite->image.pal = 0;
				/* The icon's tiles are the 128 bytes of a 16 by 16 sprite. */
				glyph = syHeapSlotAlloc(
					HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
				gmItemLoadIcon(ITEM_CLOAK_BALL);
				mnVRAMLoad(sprite->slot, 128, glyph->tiles);
				syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
			}
			if(MainRoutineStateWork.entrance ==
					BABI_PALACE_MEETING_ROOM_FROM_CARPET_ROOM &&
				!fdScriptGetFlag(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
				gmFlagSet(FLAG_BABI_SPOKE_AFTER_COLOSSO);
				fdActorSetPos(ACTOR_BABI, 208, 704);
				scBabiPalaceMeetBabiAfterColosso();
			}
			/* After Colosso Babi lies in his bed, with no shadow
			   (https://alchemy.pascalpixel.com/places/babi-palace_colosso-over/map-18.png).
			 */
			fdActorSetAnimation(ACTOR_BABI, BABI_ANIMATION_IN_BED);
			fdActorSetSprFlags(fdActorGet(ACTOR_BABI), 0);
		} else if(gmFlagGet(FLAG_BABI_SAVED) &&
			!gmFlagGet(FLAG_BABI_PALACE_FINALS_REST_OFFERED)) {
			fdActorSetPos(ACTOR_FINALS_ATTENDANT, 120, 72);
		}
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		/* The attendant before the rescue and after Colosso; during the finals
		   ACTOR_FINALS_HURRYING_WARRIORS_ALONG has this number. */
		fdActorGet(ACTOR_BED_ATTENDANT)->collisionFlg |= ACTOR_NEAR_TALK;
		if(MainRoutineStateWork.entrance ==
			BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH) {
			scBabiPalaceRestorePartyAndInventory();
			scBabiPalaceWakeAfterFinals();
			/* Only this entrance chooses the waking, so rewriting it makes a
			   later load of the scene from the main routine's state, as when a
			   game saved here is continued, an arrival from the statue room,
			   not a second waking. */
			MainRoutineStateWork.entrance =
				BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST;
		}
		if(MainRoutineStateWork.entrance ==
			BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH) {
			scBabiPalaceRestorePartyAndInventory();
			gmFlagSet(FLAG_BABI_PALACE_FINALS_REST_OFFERED);
			gmFlagSet(FLAG_BABI_PALACE_FINALS_BEDS_OPEN);
			fdActorSetPos(ACTOR_FINALS_ATTENDANT, 56, 120);
			fdActorFaceDirection(
				ACTOR_FINALS_ATTENDANT, FACING_EAST_NORTHEAST, 0);
			scBabiPalaceWakeFromFinalsDream();
			MainRoutineStateWork.entrance =
				BABI_PALACE_BIG_BEDROOM_FROM_STATUE_ROOM_WEST;
		}
	}
	return 0;
}

/* clang-format off */
/* The palace door between the two guards opening, a column of three */
/* metatiles at a time. */
Uint16 BabiPalace_DoorMetatiles[] = {
	MAPGRID_ANIMATION_STEP(25, 28, 1, 3, 5), MAPGRID_ANIMATION_STEP(26, 28, 1, 3, 5), MAPGRID_ANIMATION_STEP(27, 28, 1, 3, 5), MAPGRID_ANIMATION_STEPS_END,
};

Sint32 BabiPalace_IndigestionScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
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

Uint32 BabiPalace_Exits[] = {
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
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, (Sint32)BabiPalace_IndigestionScript,
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

CUE BabiPalace_CuesBabiMissing[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST },
	{ CUE_EXIT, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST },
	{ CUE_EXIT, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, BABI_PALACE_TO_COTTAGE_ROOMS_B },
	{ CUE_EXIT, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
	{ CUE_EXIT, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ CUE_EXIT, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
	{ CUE_EXIT, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM },
	{ CUE_EXIT, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM },
	{ CUE_EXIT, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM },
	{ CUE_EXIT, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM },
	{ CUE_EXIT, 24, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST },
	{ CUE_EXIT, 25, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST },
	{ CUE_EXIT, 26, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING },
	{ CUE_EXIT, 27, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING },
	{ CUE_EXIT, 28, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM },
	{ CUE_EXIT, 29, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ CUE_EXIT, 33, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM },
	{ CUE_EXIT, 34, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING },
	{ CUE_EXIT, 35, CONDITION_ALWAYS, BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM },
	{ CUE_EXIT, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM },
	{ CUE_EXIT, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS },
	{ CUE_EXIT, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS },
	{ CUE_EXIT, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM },
	{ CUE_EXIT, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ CUE_EXIT, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
	{ CUE_STEP, METATILE_BEHAVIOR_BABI_PALACE_PASSAGE_TO_CARPET, FLAG_IODEM_HEARD_SOLDIERS, SCRIPT_REF(scBabiPalaceHearSoldiersReport) },
	{ CUE_TALK, ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceAskAboutSoldiers) },
	{ CUE_TALK, ACTOR_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfYoureEnteringColossoHurryAlong) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowDiligentlyBabisSoldiers) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWarriorsAreNotAllowedToStay) },
	{ CUE_TALK, ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceShowBeds) },
	{ CUE_TALK, ACTOR_INJURED_WARRIOR, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceAskAboutColosso) },
	{ CUE_TALK, ACTOR_WARRIOR_WITH_INDIGESTION, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceTalkToWarriorWithIndigestion) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantToWatchTheColosso) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INJURED_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGetSoAngrySeeingAll) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_WITH_INDIGESTION, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMaybeIShouldntHaveEatenQuite) },
	{ CUE_TALK, ACTOR_FIRST_PALACE_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnlyWarriorsWhoFightWellIn) },
	{ CUE_TALK, ACTOR_FIRST_PALACE_SOLDIER + 1, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceLordBabiIsBuildingTheLighthouse) },
	{ CUE_TALK, ACTOR_FIRST_PALACE_SOLDIER + 2, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisNameIsRidingOnThe) },
	{ CUE_TALK, ACTOR_FIRST_PALACE_SOLDIER + 3, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceEvenIfLordBabiDoesntReturn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMyFirstColossoWasSoExciting) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER + 1, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnceTheLighthouseIsCompleteBabis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER + 2, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWedNeverGetNearTheLighthouse) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_PALACE_SOLDIER + 3, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHearThatGirlIsGoing) },
	{ CUE_TALK, ACTOR_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItLooksLikeLordBabiDidnt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGuessLordBabiDidNot) },
	{ CUE_TALK, ACTOR_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorHasBeenStayingAsLord) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasReveredAsAChild) },
	{ CUE_TALK, ACTOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsHoldingMeHostageIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasTakenHostageToForce) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsNotPermittedToLeave) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHEBA, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceReadShebasMind) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_BABI_PALACE_BEDS, IF_FLAG_SET(FLAG_BABI_PALACE_BEDS_OPEN), SCRIPT_REF(scBabiPalaceRestInPalaceBeds) },
	{ EXAMINE_CUE(EXAMINE_SHELF), METATILE_BEHAVIOR_BABI_PALACE_ARMOR_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
	{ EXAMINE_CUE(EXAMINE_SHELF), METATILE_BEHAVIOR_BABI_PALACE_EMPTY_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
	{ EXAMINE_CUE(EXAMINE_SHELF), METATILE_BEHAVIOR_BABI_PALACE_HEADBAND_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_BABI_PALACE_BIG_BEDROOM_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_BABI_PALACE_NOTEBOOK_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_BABI_PALACE_SOLDIERS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_BABI_PALACE_DIARY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_BABI_PALACE_LUCKY_MEDAL_BARREL, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ CUE_TABLE_END },
};

CUE BabiPalace_CuesFinals[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST },
	{ CUE_EXIT, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST },
	{ CUE_EXIT, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, BABI_PALACE_TO_COTTAGE_ROOMS_B },
	{ CUE_EXIT, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
	{ CUE_EXIT, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ CUE_EXIT, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
	{ CUE_EXIT, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM },
	{ CUE_EXIT, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM },
	{ CUE_EXIT, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM },
	{ CUE_EXIT, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM },
	{ CUE_EXIT, 24, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST },
	{ CUE_EXIT, 25, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST },
	{ CUE_EXIT, 26, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING },
	{ CUE_EXIT, 27, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING },
	{ CUE_EXIT, 28, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM },
	{ CUE_EXIT, 29, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ CUE_EXIT, 33, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM },
	{ CUE_EXIT, 34, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING },
	{ CUE_EXIT, 35, CONDITION_ALWAYS, BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM },
	{ CUE_EXIT, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM },
	{ CUE_EXIT, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS },
	{ CUE_EXIT, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS },
	{ CUE_EXIT, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM },
	{ CUE_EXIT, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ CUE_EXIT, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
	{ CUE_TALK, ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisSoldiersHaveGoneToSee) },
	{ CUE_TALK, ACTOR_FINALS_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatAreYouDoingHurryOver) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceThoseGuysLookReallyConfidentAbout) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_HURRYING_WARRIORS_ALONG, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheFinalsAreAllThatsLeft) },
	{ CUE_TALK, ACTOR_FINALS_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceOfferRest) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHopeYoullAllDoWell) },
	{ CUE_TALK, ACTOR_FINALS_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiHasReturnedAndTheFinals) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhenBabiReturnedHeHeadedOff) },
	{ CUE_TALK, ACTOR_FINALS_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatsGoingOnActorIsLooking) },
	{ CUE_TALK, ACTOR_FINALS_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSoldiersCameToSeeActor) },
	{ CUE_TALK, ACTOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceHmphIWishColossoWouldHurry) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceASmileOnAPrettyGirl) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FINALS_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsAGoodGirlBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIDontHaveTheLeastInterest) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_BABI_PALACE_BEDS, IF_FLAG_SET(FLAG_BABI_PALACE_FINALS_BEDS_OPEN), SCRIPT_REF(scBabiPalaceRestInPalaceBeds) },
	{ EXAMINE_CUE(EXAMINE_SHELF), METATILE_BEHAVIOR_BABI_PALACE_ARMOR_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
	{ EXAMINE_CUE(EXAMINE_SHELF), METATILE_BEHAVIOR_BABI_PALACE_EMPTY_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
	{ EXAMINE_CUE(EXAMINE_SHELF), METATILE_BEHAVIOR_BABI_PALACE_HEADBAND_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_BABI_PALACE_BIG_BEDROOM_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_BABI_PALACE_NOTEBOOK_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_BABI_PALACE_SOLDIERS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_BABI_PALACE_DIARY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_BABI_PALACE_LUCKY_MEDAL_BARREL, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ CUE_TABLE_END },
};

CUE BabiPalace_CuesAfterColosso[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_TO_BABI_PALACE_ENTRANCE },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_WEST },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_CARPET_ROOM_EAST },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_WEST_STEPS },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, BABI_PALACE_STATUE_ROOM_TO_EAST_STEPS },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_WEST, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_STATUE_ROOM_TO_BIG_BEDROOM_EAST, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_WEST },
	{ CUE_EXIT, 9, CONDITION_ALWAYS, BABI_PALACE_BIG_BEDROOM_TO_STATUE_ROOM_EAST },
	{ CUE_EXIT, 10, CONDITION_ALWAYS, BABI_PALACE_SCHOLARS_LANDING_TO_CARPET_ROOM },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, BABI_PALACE_TO_COTTAGE_ROOMS_B },
	{ CUE_EXIT, 12, CONDITION_ALWAYS, BABI_PALACE_TUNNEL_LANDING_TO_CARPET_ROOM },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, BABI_PALACE_TO_COLOSSEUM_TUNNEL },
	{ CUE_EXIT, 16, CONDITION_ALWAYS, BABI_PALACE_BEDROOM_LANDING_TO_MEETING_ROOM },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_BEDROOM_LANDING_TO_BABIS_BEDROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ CUE_EXIT, 18, CONDITION_ALWAYS, BABI_PALACE_BABIS_BEDROOM_TO_BEDROOM_LANDING },
	{ CUE_EXIT, 20, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_STATUE_ROOM },
	{ CUE_EXIT, 21, CONDITION_ALWAYS, BABI_PALACE_WEST_STEPS_TO_WEST_ROUND_ROOM },
	{ CUE_EXIT, 22, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_STATUE_ROOM },
	{ CUE_EXIT, 23, CONDITION_ALWAYS, BABI_PALACE_EAST_STEPS_TO_EAST_ROUND_ROOM },
	{ CUE_EXIT, 24, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_WEST },
	{ CUE_EXIT, 25, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_STATUE_ROOM_EAST },
	{ CUE_EXIT, 26, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SCHOLARS_LANDING },
	{ CUE_EXIT, 27, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_TUNNEL_LANDING },
	{ CUE_EXIT, 28, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_NORTH_ROUND_ROOM },
	{ CUE_EXIT, 29, CONDITION_ALWAYS, BABI_PALACE_CARPET_ROOM_TO_SHEBAS_LANDING },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_OVEN_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_SOLDIERS_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_CARPET_ROOM_TO_MEETING_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ CUE_EXIT, 33, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_CARPET_ROOM },
	{ CUE_EXIT, 34, CONDITION_ALWAYS, BABI_PALACE_MEETING_ROOM_TO_BEDROOM_LANDING },
	{ CUE_EXIT, 35, CONDITION_ALWAYS, BABI_PALACE_OVEN_ROOM_TO_CARPET_ROOM },
	{ CUE_EXIT, 36, CONDITION_ALWAYS, BABI_PALACE_SOLDIERS_ROOM_TO_CARPET_ROOM },
	{ CUE_EXIT, 40, CONDITION_ALWAYS, BABI_PALACE_WEST_ROUND_ROOM_TO_WEST_STEPS },
	{ CUE_EXIT, 41, CONDITION_ALWAYS, BABI_PALACE_EAST_ROUND_ROOM_TO_EAST_STEPS },
	{ CUE_EXIT, 42, CONDITION_ALWAYS, BABI_PALACE_NORTH_ROUND_ROOM_TO_CARPET_ROOM },
	{ CUE_EXIT, 43, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_LANDING_TO_CARPET_ROOM },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_SHEBAS_LANDING_TO_SHEBAS_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughDoor) },
	{ CUE_EXIT, 45, CONDITION_ALWAYS, BABI_PALACE_SHEBAS_ROOM_TO_SHEBAS_LANDING },
	{ CUE_TALK, ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceAskAboutMeetingBabi) },
	{ CUE_TALK, ACTOR_ASKING_ABOUT_THE_EASTERN_SHORES, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceAskAboutEasternShores) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_BABIS_SOLDIERS, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHaventSeenMuchOfBabi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_THE_EASTERN_SHORES, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItWouldBeAWasteTo) },
	{ CUE_TALK, ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNoOnesGoingToSleepIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BED_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWouldThisRoomGetVeryDusty) },
	{ CUE_TALK, ACTOR_RELIEVED_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWasFoundSafeAndSound) },
	{ CUE_TALK, ACTOR_GONDOWAN_BOUND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNowThatColossoIsOverIm) },
	{ CUE_TALK, ACTOR_SOLDIER_PREFERRING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSouthernContinentIsADangerous) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RELIEVED_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWasHopingIdBeThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GONDOWAN_BOUND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceColossoHasEndedSoWeHave) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_PREFERRING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantThatLighthouseFinishedQuickly) },
	{ CUE_TALK, ACTOR_BABI, IF_FLAG_SET(FLAG_BABI_PALACE_CLOAK_BALL), MSGNO(MsgBabiPalaceIfYouUseTheCloakBall) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABI, IF_FLAG_SET(FLAG_BABI_PALACE_CLOAK_BALL), MSGNO(MsgBabiPalaceWhyWouldAnAdeptLikeYou) },
	{ CUE_TALK, ACTOR_BABI, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSleepingALotHelpsToDelay) },
	{ CUE_TALK, ACTOR_AFTER_COLOSSO_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisFaceIsVeryPaleLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABI, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWishIHadMetYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_COLOSSO_BY_BABIS_BED, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfSomethingHappensToBabiWhat) },
	{ CUE_TALK, ACTOR_CLOAK_BALL, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceOfferCloakBall) },
	{ CUE_TALK, ACTOR_AFTER_COLOSSO_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheGirlFromLaliveroIsNo) },
	{ CUE_TALK, ACTOR_AFTER_COLOSSO_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSurprisedActorWasAllowed) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_COLOSSO_SORRY_FOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItSeemedOnlyNaturalThatActor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_COLOSSO_OLD_WOMAN_IN_SHEBAS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSoRelievedActorHas) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_BABI_PALACE_BEDS, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceRestInPalaceBeds) },
	{ EXAMINE_CUE(EXAMINE_SHELF), METATILE_BEHAVIOR_BABI_PALACE_ARMOR_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
	{ EXAMINE_CUE(EXAMINE_SHELF), METATILE_BEHAVIOR_BABI_PALACE_EMPTY_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
	{ EXAMINE_CUE(EXAMINE_SHELF), METATILE_BEHAVIOR_BABI_PALACE_HEADBAND_SHELF, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_BABI_PALACE_BIG_BEDROOM_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_BABI_PALACE_NOTEBOOK_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_BABI_PALACE_SOLDIERS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_BABI_PALACE_DIARY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_BABI_PALACE_LUCKY_MEDAL_BARREL, FLAG_BABI_PALACE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ CUE_TABLE_END },
};

CUE BabiPalace_OutsideCuesBabiMissing[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_ENTRANCE_TO_BABI_PALACE, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughAnimatedDoor) },
	{ CUE_TALK, ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNormallyNoOnesAllowedIntoBabis) },
	{ CUE_TALK, ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSomeOfTheRoomsInThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceShouldThePalaceReallyBeOpen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowManyPeopleWill) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_BABI_PALACE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceExamineWell) },
	{ CUE_TABLE_END },
};

CUE BabiPalace_OutsideCuesFinals[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_ENTRANCE_TO_BABI_PALACE, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughAnimatedDoor) },
	{ CUE_TALK, ACTOR_FIRST_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, SCRIPT_REF(scBabiPalacePassOnBabisWord) },
	{ CUE_TALK, ACTOR_SECOND_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, SCRIPT_REF(scBabiPalacePassOnBabisWord) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_FIRST_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, SCRIPT_REF(scBabiPalacePassOnBabisWord) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_SECOND_GUARD, FLAG_BABI_PALACE_GUARD_GAVE_WORD, SCRIPT_REF(scBabiPalacePassOnBabisWord) },
	{ CUE_TALK, ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWantsYouToGoStraight) },
	{ CUE_TALK, ACTOR_SECOND_GUARD, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceThankForSavingBabi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSureTheySavedBabiButIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsADeliberateManHe) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_BABI_PALACE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceExamineWell) },
	{ CUE_TABLE_END },
};

CUE BabiPalace_OutsideCuesAfterColosso[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, BABI_PALACE_ENTRANCE_TO_TOLBI },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BABI_PALACE_ENTRANCE_TO_BABI_PALACE, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceLeaveThroughAnimatedDoor) },
	{ CUE_TALK, ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIveLostAllMyEnthusiasmSince) },
	{ CUE_TALK, ACTOR_SECOND_GUARD, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceAskAboutRemainingWarriors) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheEndOfAFestivalIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUARD, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceAllTheHeroicWarriorsWhoTook) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_BABI_PALACE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scBabiPalaceExamineWell) },
	{ CUE_TABLE_END },
};
/* clang-format on */
