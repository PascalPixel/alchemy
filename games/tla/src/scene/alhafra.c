/* Alhafra, the port town of Osenia north of the Yampi Desert, where the tidal
   wave wrecked every ship but one sailboat, whose mast it broke.

   As Felix comes into town, the mayor and elder of Madra and their soldiers
   arrive behind him, hunting Briggs, and ask the way to Alhafra's mayor
   (scAlhafraMadransArrive); one of Briggs's pirates, watching the road by a
   wall, sees the mayor of Madra and runs off to warn his crew
   (scAlhafraPirateSeesMadrasMayor). Once Briggs is caught, the hungry boy too
   weak to move eats the party's Large Bread (scAlhafraGiveBreadToHungryBoy) and
   his father, at the back way into the cave by the mayor's home, lets the party
   pass (scAlhafraFatherLetsPartyToCave). Frost freezes the puddles of seawater,
   Reveal finds the Psy Crystal chest among the gravestones, and the townsfolk
   change once the mast is cleared and again once Briggs has stolen the ship
   (FLAG_ALHAFRA_SHIP_GONE). */

/* This unit declares its own form of fdActorTakeLeaderLayer, below. */
#define OWN_fdActorTakeLeaderLayer

#include "game/game.h"
#include "item_ids.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "field/script_inline.h"
#include "field/actor_script.h"
#include "field/field_mode.h"

#include "text/msg_ids.h"
#include "shop/shop.h"

/* How the pirate's scene starts: the leader stepping past him, or reading his
   mind, which lets the party question him before he runs. */
typedef enum ALHAFRA_PIRATE_SCENE {
	PIRATE_STEPPED_PAST,
	PIRATE_READ_BY_MIND_READ
} ALHAFRA_PIRATE_SCENE;

void scAlhafraPirateSeesMadrasMayor(Sint32 how);
void scAlhafraTalkToHungryBoy(Sint32 id);
void scAlhafraTakeLeadersLayer(void);
/* The ROM's caller here is compiled against this form, with no parameter list
   where the definition takes the actor. */
Sint32		  fdActorTakeLeaderLayer();
void		  scAlhafraReadPiratesMind(void);
void		  scAlhafraDrawPirateBehind(void);
void		  scAlhafraDrawPirateInFront(void);
void		  scAlhafraSailorToldToWork(void);
void		  scAlhafraRevealTreasureSpot(void);
void		  scAlhafraHideTreasureSpot(void);
extern Uint32 AlhafraLookAroundScript[];
extern Uint16 AlhafraNoDoor[];
extern Uint16 AlhafraDoorOpening[];
extern Uint16 AlhafraSecondDoorOpening[];

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

/* The town's own story, which no other scene reads. */
#define FLAG_TOLD_INDRA_MOVED 0x8b0 /* told the man asking that Indra moved */
#define FLAG_PIRATE_LEFT	  0x8b4 /* the pirate ran to warn his crew */
#define FLAG_HUNGRY_BOY_FED	  0x8b5 /* the boy at the inn ate the bread */
#define FLAG_BOY_TOLD_OF_CAVE 0x8b6 /* he told of the cave under the manor */
#define FLAG_GOLD_COINS_FOUND 0x8b7 /* his mother found five gold coins */
#define FLAG_LET_INTO_CAVE	  0x8b9 /* his father let the party pass */

/* Set while Frost holds each of the three puddles frozen into a pillar. */
#define FLAG_PUDDLE_A_FROZEN FLAG_SCENE_TEMP_00
#define FLAG_PUDDLE_B_FROZEN FLAG_SCENE_TEMP_01
#define FLAG_PUDDLE_C_FROZEN FLAG_SCENE_TEMP_02

/* The chest Reveal shows among the six gravestones, holding a Psy Crystal. */
#define ACTOR_PSY_CRYSTAL_CHEST 64

/* The Alhafrans of AlhafraActorPlacements, who keep their rows, sprites and
   places to ACTOR_SAILOR_BUILDING_HOUSES once Briggs is caught and once the
   ship is stolen; what each says changes with the story. */
typedef enum ALHAFRA_ACTOR_ID {
	ACTOR_WEAPON_MERCHANT = ACTOR_FIRST_PLACED,
	ACTOR_ARMOR_MERCHANT,
	/* The soldiers at the mayor's manor: "The mayor is in a meeting..." */
	ACTOR_MANOR_GUARD,
	ACTOR_SECOND_MANOR_GUARD,
	/* The man at the back way into the cave under the manor, the hungry boy's
	   father: "No one is allowed beyond this point." */
	ACTOR_HUNGRY_BOYS_FATHER,
	/* "Welcome to Alhafra. This is the largest town in western Osenia..." */
	ACTOR_WELCOMING_YOUNG_MAN,
	/* "A massive tidal wave hit us and destroyed every ship in Alhafra." */
	ACTOR_WOMAN_ON_THE_LOST_SHIPS,
	/* "I hear the tidal wave knocked Indra into Osenia. Is that true?" */
	ACTOR_MAN_ASKING_ABOUT_INDRA,
	/* "Right after my boat got crushed, a group of men came by..." */
	ACTOR_BOY_WITH_A_CRUSHED_BOAT,
	/* "Uh... The mayor? Just keep going down this road." */
	ACTOR_ALHAFRAN_GIVING_DIRECTIONS,
	/* "Those people that just came through looked totally exhausted." */
	ACTOR_OLD_WOMAN_ON_THE_TRAVELERS,
	/* "We've had a lot of strangers coming through here lately"; once Briggs is
	   caught, "Hey! You there! Sailor boy! Get back to work!" */
	ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK,
	/* "Our port was ruined by the tidal wave, so kids aren't allowed
	   there..." */
	ACTOR_GIRL_KEPT_FROM_THE_PORT,
	/* "I wonder if my partner has finished up that canoe yet..." */
	ACTOR_PARTNER_WAITING_FOR_THE_CANOE,
	/* "Let me guess... You're here because you want to buy a canoe, right?" */
	ACTOR_MAN_FINISHING_A_CANOE,
	/* "That boat at the wharf uses a sail..." */
	ACTOR_SAILOR_ON_THE_SAILBOAT,
	/* "Why would the mayor go and sell our boat to a bunch of foreigners?";
	   once Briggs is caught, "What does a sailor like me know about building
	   houses, anyway?" */
	ACTOR_SAILOR_BUILDING_HOUSES,
	/* One of Briggs's pirates, watching the road before Briggs is caught. */
	ACTOR_PIRATE,
	/* The three puddles of seawater Frost freezes. */
	ACTOR_PUDDLE_A,
	ACTOR_PUDDLE_B,
	ACTOR_PUDDLE_C,
	/* Madra's elder, the mayor's father (his sprite, as in the Yampi Desert,
	   where he calls the mayor his son): "We're here to bring in Briggs the
	   Champa." */
	ACTOR_MADRAS_ELDER,
	/* Madra's mayor (his sprite, as Madra's ACTOR_MAYOR): "Madra's a pretty big
	   place, but this town is huge!" */
	ACTOR_MADRAS_MAYOR,
	/* Madra's soldiers (the drawbridge guard's sprite). */
	ACTOR_FIRST_MADRAN_SOLDIER,
	ACTOR_SECOND_MADRAN_SOLDIER,
	/* "We-We're here?" ... "He's coming now..." */
	ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
	/* A weary Madran who arrives before the elder: "Haaa hoo hooo... Are
	   we...here?" */
	ACTOR_WEARY_MADRAN,
	/* A bald, bearded Madran who walks in beside the one waiting for the elder
	   and never speaks. */
	ACTOR_SILENT_MADRAN
} ALHAFRA_ACTOR_ID;

/* The families whose houses are being rebuilt, after the townsfolk once Briggs
   is caught and again once the ship is stolen. */
typedef enum ALHAFRA_ONCE_BRIGGS_IS_CAUGHT_ACTOR_ID {
	/* "Once they fix the old folks' home, it'll be our turn..." */
	ACTOR_YOUNG_MAN_WAITING_AT_THE_INN = ACTOR_SAILOR_BUILDING_HOUSES + 1,
	/* "I was afraid my house would never get repaired..." */
	ACTOR_WOMAN_WAITING_AT_THE_INN,
	/* The boy too hungry to move. */
	ACTOR_HUNGRY_BOY,
	/* The family on the step where the mother finds five gold coins: "What a
	   stroke of luck!" */
	ACTOR_LUCKY_MOTHERS_HUSBAND,
	ACTOR_LUCKY_MOTHER,
	ACTOR_LUCKY_MOTHERS_CHILD,
	/* "I wonder if there's any more money hidden around here..." */
	ACTOR_BOY_HUNTING_FOR_COINS
} ALHAFRA_ONCE_BRIGGS_IS_CAUGHT_ACTOR_ID;

/* The Alhafrans the town's setup moves once the mast is cleared, until the ship
   is stolen (AlhafraActorPlacementsOnceTheMastIsCleared). */
typedef enum ALHAFRA_ONCE_THE_MAST_IS_CLEARED_ACTOR_ID {
	/* "The whole town is grateful to you..." */
	ACTOR_GRATEFUL_YOUNG_MAN = ACTOR_FIRST_PLACED,
	/* "I'm just standing guard, in case Briggs decides to..." */
	ACTOR_YOUNG_MAN_STANDING_GUARD,
	/* "Our sailboat looks fabulous with its mast repaired!" */
	ACTOR_WOMAN_ADMIRING_THE_MAST,
	/* "I wonder where Briggs and his crew went..." */
	ACTOR_WOMAN_WONDERING_WHERE_BRIGGS_WENT,
	/* "I heard you made it so we could fix our sailboat!" */
	ACTOR_WOMAN_GLAD_OF_THE_SAILBOAT,
	/* "Ever since we heard Briggs broke out of jail..." */
	ACTOR_MAN_WARY_OF_BRIGGS,
	/* "I don't know why everyone's so excited about fixing the ship..." */
	ACTOR_MAN_NOT_EXCITED_BY_THE_SHIP,
	/* "Oh! It's you! Where have you been?" */
	ACTOR_MAN_GLAD_TO_SEE_FELIX,
	/* "With that ship, Alhafra's going to become a big, important town..." */
	ACTOR_WOMAN_ON_ALHAFRAS_FUTURE,
	/* "Once that stone was taken care of, it was really quite..." */
	ACTOR_WOMAN_ON_THE_STONE,
	/* "If Briggs is smart, he's probably long gone..." */
	ACTOR_WOMAN_SURE_BRIGGS_IS_GONE,
	/* "I've been waiting for this day for years!" */
	ACTOR_OLD_MAN_WAITING_FOR_THIS_DAY,
	/* "The secret to harnessing the wind's power is building a small..." */
	ACTOR_OLD_MAN_ON_SAILS,
	/* "I'm sure it's just a coincidence, but whenever you show up..." */
	ACTOR_OLD_MAN_ON_COINCIDENCES,
	/* "The mayor was actually singing and skipping..." */
	ACTOR_OLD_WOMAN_ON_THE_SINGING_MAYOR,
	/* "Weird... Have you ever had the feeling that someone just..." */
	ACTOR_OLD_WOMAN_FEELING_WATCHED,
	/* "Huge boats carry more, but we need smaller ships to catch the
	   fish..." */
	ACTOR_OLD_WOMAN_ON_FISHING_BOATS
} ALHAFRA_ONCE_THE_MAST_IS_CLEARED_ACTOR_ID;

extern ACTOR_PLACE				   AlhafraActorPlacements[];
extern ACTOR_PLACE				   AlhafraActorPlacementsOnceBriggsIsCaught[];
extern ACTOR_PLACE				   AlhafraActorPlacementsOnceTheShipIsStolen[];
extern ACTOR_PLACE				   AlhafraActorPlacementsOnceTheMastIsCleared[];
extern MAP_GRID_METATILE_ANIMATION AlhafraDoors[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(syGameFlagGet(FLAG_ALHAFRA_MAST_CLEARED)) {
		if(syGameFlagGet(FLAG_ALHAFRA_SHIP_GONE) == 0)
			return AlhafraActorPlacementsOnceTheMastIsCleared;
		return AlhafraActorPlacementsOnceTheShipIsStolen;
	}
	if(syGameFlagGet(FLAG_BRIGGS_CAUGHT))
		return AlhafraActorPlacementsOnceBriggsIsCaught;
	return AlhafraActorPlacements;
}

/* A door of the town opens as Felix walks into it, he walks through, and the
   screen wipes to the room behind it (AlhafraRooms, the jail, the cave or the
   manor, by the exit given). */
void scAlhafraOpenDoorAndEnter(
	MAP_GRID_METATILE_ANIMATION *step, Sint32 exit, Sint32 unused)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(step->steps, step->x, step->y);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlg = ACTOR_FALLS;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorCenterAndWalk(MainRoutineStateWork.leaderActorId, 2, -8);
	fdScriptWait(10);
	syImportScriptSetExitRequest(exit);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}

/* Felix walks north into one of the town's doors: the houses' and the inn's,
   the jail's, the back way into the cave by the mayor's home, or the manor's.
   The door opens, he walks in, and the screen wipes to the room inside
   (scAlhafraOpenDoorAndEnter).

   Each door's metatile behaviour is the number of the exit it leads by, and of
   its row of AlhafraDoors. */
void scAlhafraEnterDoor(Sint32 idx)
{
	MAP_GRID_METATILE_ANIMATION *tbl = AlhafraDoors;

	scAlhafraOpenDoorAndEnter(&tbl[idx], idx, 0);
}

/* Alhafra's shops, as mnShopRun numbers them. */
enum { SHOP_ALHAFRA_WEAPONS = 7, SHOP_ALHAFRA_ARMOR = 8 };

/* The metatile behaviors of the town's step cues: the pirate drawn behind or in
   front of the wall as the leader passes, the Madrans arriving (from the first
   Felix stands in their way and steps aside, from the second he only turns to
   them), and the pirate running off to warn the crew. */
typedef enum ALHAFRA_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_PIRATE_BEHIND = 18,
	METATILE_BEHAVIOR_PIRATE_IN_FRONT,
	METATILE_BEHAVIOR_MADRANS_ARRIVE_BEHIND_FELIX,
	METATILE_BEHAVIOR_MADRANS_ARRIVE_BESIDE_FELIX,
	METATILE_BEHAVIOR_PIRATE_RUNS_OFF
} ALHAFRA_METATILE_BEHAVIOR;

/* Felix talks to the weapon merchant across his counter, and the shop opens. */
void scAlhafraOpenWeaponShop(Sint32 keeperId)
{
	mnShopRun(SHOP_ALHAFRA_WEAPONS, keeperId);
}

/* Felix talks to the armor merchant across his counter, and the shop opens. */
void scAlhafraOpenArmorShop(Sint32 keeperId)
{
	mnShopRun(SHOP_ALHAFRA_ARMOR, keeperId);
}

/* A townsman asks whether the tidal wave really knocked Indra into Osenia; a
   yes is remembered, and after it he only marvels that it is true. */
void scAlhafraAskWhetherIndraMoved(Sint32 actor)
{
	Sint32 msg;

	if(fdScriptGetFlag(FLAG_TOLD_INDRA_MOVED) == 0) {
		msg = (Sint32)&MsgAlhafraIHearTheTidalWave;
		fdScriptSetMessageId(msg);
		/* "I hear the tidal wave knocked Indra into Osenia. Is that true?" */
		fdMessageShowAtActor(actor, 0);
		if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) ==
			0) {
			fdScriptSetMessageId(msg + 1);
			syGameFlagSet(FLAG_TOLD_INDRA_MOVED);
		} else {
			fdScriptSetMessageId(msg + 2);
		}
		fdScriptShowMessageAndWait(actor, 0);
	} else {
		fdScriptSetMessageId((Sint32)&MsgAlhafraIGuessWhatThoseMen);
		/* "I guess what those men told me was true... I'd figured they
		   were..." */
		fdScriptShowMessageAndWait(actor, 0);
	}
}

/* The man finishing a canoe guesses the party wants to buy one; yes or no, he
   is too backlogged with preorders to make it. */
void scAlhafraBackloggedWithCanoes(Sint32 actor)
{
	Sint32 msg = (Sint32)&MsgAlhafraLetMeGuessYoureHere;

	fdScriptSetMessageId(msg);
	/* "Let me guess... You're here because you want to buy a canoe, right?" */
	fdMessageShowAtActor(actor, 0);
	if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(10);
		/* Yes: "Sorry, but I'm totally backlogged with preorders..." */
		fdScriptSetMessageId(msg + 1);
	} else {
		fdScriptWait(20);
		/* No: "That's good, because I've got a ton of work to do already..." */
		fdScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessageAndWait(actor, 0);
}

/* The manor's guard asks whether the party wants to see the mayor, who is in a
   meeting. */
void scAlhafraMayorInAMeeting(Sint32 actor)
{
	Sint32 msg = (Sint32)&MsgAlhafraDidYouWantToSee;

	fdScriptSetMessageId(msg);
	/* "Did you want to see the mayor?" */
	fdMessageShowAtActor(actor, 0);
	if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(10);
		/* Yes: "Sorry, but he's meeting with some visitors from Madra right
		   now..." */
		fdScriptSetMessageId(msg + 1);
	} else {
		fdScriptWait(20);
		/* No: "The mayor is very open to meeting visitors, but he's in
		   an..." */
		fdScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessageAndWait(actor, 0);
}

/* Mind Read on the pirate: he sees the mayor of Madra, and the party can
   question him before he runs to warn the crew. */
void scAlhafraReadPiratesMind(void)
{
	scAlhafraPirateSeesMadrasMayor(PIRATE_READ_BY_MIND_READ);
}

/* Stepping past the pirate: he sees the mayor of Madra and runs to warn the
   crew. */
void scAlhafraPirateRunsToWarnTheCrew(void)
{
	scAlhafraPirateSeesMadrasMayor(PIRATE_STEPPED_PAST);
}

/* Nothing visible happens as Felix walks past the wall where Briggs's pirate
   keeps watch (metatiles (10, 44) to (11, 45)); the game sets the pirate to be
   drawn behind the wall from where Felix now stands. */
void scAlhafraDrawPirateBehind(void)
{
	scFieldActorSetSprPriority(ACTOR_PIRATE, 2);
}

/* Nothing visible happens as Felix walks back past the pirate's wall, south of
   it (metatiles (12, 46) and (12, 47)); the game sets the pirate to be drawn in
   front of the wall again. */
void scAlhafraDrawPirateInFront(void)
{
	scFieldActorSetSprPriority(ACTOR_PIRATE, 3);
}

extern Sint16 AlhafraPuddles[];

/* Felix casts Frost on one of the three puddles of seawater the tidal wave left
   in town, and it freezes into a pillar of ice. */
void scAlhafraFreezePuddle(Sint32 unused, Sint32 actorId)
{
	fdFlagBlockApplyFlags((Sint32)AlhafraPuddles);
	fdActorGetById(actorId)->priFlg = 0;
}

/* The sailor building houses grumbles, and the woman beside him tells him to
   get back to work. */
void scAlhafraSailorToldToWork(void)
{
	fdScriptSetMessageId((Sint32)MsgAlhafraWhatDoesASailorLike);
	/* "What does a sailor like me know about building houses, anyway?" */
	syFieldScriptShowMessageThenWait(ACTOR_SAILOR_BUILDING_HOUSES, 0, 10);
	/* She turns to the sailor, just north of east. */
	fdActorFaceDirectionById(ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK,
		FACING_EAST - TRIG_TURN / 256, 0);
	fdScriptWait(10);
	fdActorShowEmoteById(ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK,
		EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Hey! You there! Sailor boy! Get back to work!" */
	syFieldScriptShowMessageThenWait(
		ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, 0, 10);
	scFieldActorStartRepeatedMotion(ACTOR_SAILOR_BUILDING_HOUSES, 3);
	fdEffectSetAttached(ACTOR_SAILOR_BUILDING_HOUSES,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorFaceDirection(
		ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, FACING_WEST, 0);
	/* "Hey! Give me a break! I'm on it!" */
	syFieldScriptShowMessageThenWait(ACTOR_SAILOR_BUILDING_HOUSES, 0, 0);
}

/* Giving the hungry boy Large Bread: he eats it, jumps up full of energy and
   talks to the party. */
void scAlhafraGiveBreadToHungryBoy(void)
{
	syGameFlagSet(FLAG_HUNGRY_BOY_FED);
	gmPartyInventoryRemove(ITEM_LARGE_BREAD);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgAlhafraThanksMister);
	fdScriptWait(10);
	fdActorFaceActorById(
		ACTOR_HUNGRY_BOY, MainRoutineStateWork.leaderActorId, 0);
	fdScriptWait(10);
	/* "Thanks, mister." */
	fdScriptShowMessageThenWait(ACTOR_HUNGRY_BOY, 0, 10);
	/* "The kid ate the bread." */
	fdScriptShowMessageThenWait(-1, 0, 10);
	fdActorSetAnimationById(ACTOR_HUNGRY_BOY, ANIM_STAND);
	fdActorJump(ACTOR_HUNGRY_BOY, 6, 15);
	fdActorJump(ACTOR_HUNGRY_BOY, 6, 23);
	/* "Oh, wow, I feel so much better! Look at how much energy I've got
	   now!" */
	fdScriptShowMessageThenWait(ACTOR_HUNGRY_BOY, 0, 0);
	scAlhafraTalkToHungryBoy(ACTOR_HUNGRY_BOY);
	fdActorFaceDirection(ACTOR_HUNGRY_BOY, FACING_SOUTH, 0);
	fdScriptEnd();
}

/* The hungry boy: too hungry to move until fed, then he tells the party of the
   caves under the mayor's manor if they like adventures. */
void scAlhafraTalkToHungryBoy(Sint32 id)
{
	if(!syGameFlagGet(FLAG_HUNGRY_BOY_FED)) {
		FieldStateWork->fieldMenu = 1;
		fdScriptSetMessageId(MsgAlhafraImSoHungryICant);
		/* "I'm so hungry I can't even move..." */
		fdScriptShowMessageAndWait(id, 0);
	} else if(fdScriptGetFlag(FLAG_BOY_TOLD_OF_CAVE)) {
		fdScriptSetMessageId(MsgAlhafraImFeelingALotBetter);
		/* "I'm feeling a lot better now thanks to you, mister. If you get
		   any..." */
		fdScriptShowMessageAndWait(id, 0);
	} else {
		fdScriptSetMessageId(MsgAlhafraGeezMisterIDontKnow);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(id, 3);
		/* "Geez, mister, I don't know how I can repay you..." */
		fdScriptShowMessageThenWait(id, 0, 10);
		fdActorRunRepeatedMotion(id, 2);
		/* "Wait, I've got it! Do you like...adventures?" */
		fdMessageShowAtActor(id, 0);
		fdScriptWait(10);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
			syGameFlagSet(FLAG_BOY_TOLD_OF_CAVE);
			fdScriptWait(20);
			/* "Then you totally have to go check out the caves under the
			   mayor's..." */
			fdScriptShowMessageThenWait(id, 0, 10);
			FieldStateWork->msg++;
		} else {
			fdScriptWait(30);
			FieldStateWork->msg++;
			/* "Really? Wow... You're so boring. Well, if you change your
			   mind..." */
			fdScriptShowMessageThenWait(id, 0, 10);
		}
	}
}

/* Stepping by the family on the step: the mother finds five gold coins, and her
   husband and child cheer. */
void scAlhafraFindGoldCoins(void)
{
	syGameFlagSet(FLAG_GOLD_COINS_FOUND);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgAlhafraWhyLookAtThis);
	fdCameraMoveTo(560, CAMERA_KEEP, 496, 1);
	fdCameraWaitForArrival();
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_LUCKY_MOTHER, 2);
	fdScriptWait(10);
	/* "Why, look at this!" */
	syFieldScriptShowMessageThenWait(ACTOR_LUCKY_MOTHER, 0, 10);
	fdActorFaceDirection(ACTOR_LUCKY_MOTHERS_HUSBAND, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_BOY_HUNTING_FOR_COINS, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_LUCKY_MOTHERS_CHILD, FACING_SOUTHWEST, 0);
	fdActorSetAnimationById(ACTOR_LUCKY_MOTHER, 5);
	sySndPlay(MUS_LEVEL_UP);
	MainRoutineStateWork.leaderActorId = ACTOR_LUCKY_MOTHER;
	fdItemDispShowItem(0, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
	MainRoutineStateWork.leaderActorId = ACTOR_FELIX;
	fdScriptWait(50);
	scFieldActorJump(ACTOR_LUCKY_MOTHERS_HUSBAND, 6, 15);
	scFieldActorJump(ACTOR_LUCKY_MOTHERS_HUSBAND, 6, 23);
	/* "Wow, honey! You found some gold coins!" */
	syFieldScriptShowMessageThenWait(ACTOR_LUCKY_MOTHERS_HUSBAND, 0, 10);
	fdActorFaceDirectionById(ACTOR_LUCKY_MOTHER, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_LUCKY_MOTHER, ANIM_NOD);
	fdScriptWait(20);
	fdActorShowEmoteById(
		ACTOR_LUCKY_MOTHERS_CHILD, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Way to go, Mom! So... How much did you find?" */
	syFieldScriptShowMessageThenWait(ACTOR_LUCKY_MOTHERS_CHILD, 0, 10);
	fdActorShowEmoteById(ACTOR_LUCKY_MOTHER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorFaceDirection(ACTOR_LUCKY_MOTHER, FACING_SOUTH, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_LUCKY_MOTHER, 2);
	fdScriptWait(10);
	/* "Oh! Well, it's not too much... It's only 5 coins." */
	syFieldScriptShowMessageThenWait(ACTOR_LUCKY_MOTHER, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_LUCKY_MOTHERS_CHILD, 2);
	fdScriptWait(10);
	/* "That's great, though! We can use it to buy some food!" */
	syFieldScriptShowMessageThenWait(ACTOR_LUCKY_MOTHERS_CHILD, 0, 10);
	fdActorFaceDirectionById(ACTOR_LUCKY_MOTHER, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_LUCKY_MOTHER, ANIM_NOD);
	fdScriptWait(20);
	scFieldActorJump(ACTOR_BOY_HUNTING_FOR_COINS, 6, 15);
	scFieldActorJump(ACTOR_BOY_HUNTING_FOR_COINS, 6, 23);
	/* "I'm going to start looking, too!" */
	syFieldScriptShowMessageThenWait(ACTOR_BOY_HUNTING_FOR_COINS, 0, 10);
	fdActorFaceDirection(ACTOR_LUCKY_MOTHERS_CHILD, FACING_EAST, 0);
	fdActorSetAnimationAndWait(ACTOR_LUCKY_MOTHERS_CHILD, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_BOY_HUNTING_FOR_COINS, FACING_SOUTH, 0);
	fdActorSetAnimation(ACTOR_BOY_HUNTING_FOR_COINS, 6);
	fdActorFaceDirection(ACTOR_LUCKY_MOTHERS_CHILD, FACING_NORTH, 0);
	fdActorSetAnimation(ACTOR_LUCKY_MOTHERS_CHILD, 6);
	fdActorFaceDirection(ACTOR_LUCKY_MOTHER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_LUCKY_MOTHERS_HUSBAND, FACING_NORTH, 0);
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdScriptEnd();
}

/* The hungry boy's father, by the way into the cave under the mayor's manor:
   once the boy is fed he asks whether the party wants to see the cave, and a
   yes has him step aside for good. */
void scAlhafraFatherLetsPartyToCave(Sint32 actor)
{
	Sint32 msg = (Sint32)MsgAlhafraYoureTheOneWhoGave;

	if(fdScriptGetFlag(FLAG_LET_INTO_CAVE) != 0) {
		fdScriptSetMessageId(msg + 4);
		/* "I've heard that the deeper you go into the cave, the more
		   dangerous..." */
		fdMessageShowAtActor(actor, 0);
	} else {
		fdScriptSetMessageId(msg);
		/* "You're the one who gave my son some food, aren't you? Did you want
		   to go see the cave?" */
		fdMessageShowAtActor(actor, 0);
		if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) !=
			0) {
			fdScriptWait(20);
			fdScriptSetMessageId(msg + 1);
			/* No: "If you were up for a little spelunking, I'd be glad to let
			   you..." */
			fdScriptShowMessageAndWait(actor, 0);
		} else {
			syGameFlagSet(FLAG_LET_INTO_CAVE);
			fdScriptWait(20);
			fdScriptSetMessageId(msg + 2);
			/* Yes: "I'm only letting you pass because you did good by my
			   son..." */
			fdScriptShowMessage(actor, 0);
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 72, 344);
			fdActorFaceDirectionById(
				MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
			fdScriptWait(10);
			fdActorSetSpeed(actor, 1);
			fdActorWalkToAndWait(actor, 56, 328);
			fdActorFaceDirection(
				MainRoutineStateWork.leaderActorId, FACING_NORTHWEST, 0);
			fdActorFaceDirectionById(actor, FACING_SOUTHEAST, 0);
			fdScriptWait(20);
			fdActorSetAnimationAndWait(actor, 3);
			fdScriptWait(10);
			/* "I'm only letting you by so you can explore a little. Make sure
			   you don't lay a hand on any of the mayor's treasure." */
			fdScriptShowMessageAndWait(actor, 0);
		}
	}
}

/* Felix casts Reveal in town: in the grey world Reveal shows, a red treasure
   chest stands in the middle of the six gravestones by the cliff, the hidden
   Psy Crystal chest, which can be opened while it lasts. */
void scAlhafraRevealTreasureSpot(void)
{
	fdTreasureSpotSetPos(ACTOR_PSY_CRYSTAL_CHEST, toFIXED(184), toFIXED(552));
}

/* Nothing visible: as Reveal fades, the Psy Crystal chest's spot is hidden
   again. */
void scAlhafraHideTreasureSpot(void)
{
	fdTreasureSpotSetPos(ACTOR_PSY_CRYSTAL_CHEST, -1, -1);
}

extern CUE AlhafraCues[];
extern CUE AlhafraCuesOnceBriggsIsCaught[];
extern CUE AlhafraCuesOnceTheShipIsStolen[];
extern CUE AlhafraCuesOnceTheMastIsCleared[];

const CUE *scGetCues(void)
{
	if(syGameFlagGet(FLAG_ALHAFRA_MAST_CLEARED)) {
		if(syGameFlagGet(FLAG_ALHAFRA_SHIP_GONE) == 0)
			return AlhafraCuesOnceTheMastIsCleared;
		return AlhafraCuesOnceTheShipIsStolen;
	}
	if(syGameFlagGet(FLAG_BRIGGS_CAUGHT))
		return AlhafraCuesOnceBriggsIsCaught;
	return AlhafraCues;
}

Sint32 scInit(void)
{
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if(syGameFlagGet(FLAG_ALHAFRA_MAST_CLEARED)) {
		if(!syGameFlagGet(FLAG_ALHAFRA_SHIP_GONE)) {
			fdActorGetById(ACTOR_OLD_WOMAN_ON_FISHING_BOATS)->update =
				(OBJECT_UPDATE)scAlhafraTakeLeadersLayer;
			fdActorGetById(ACTOR_OLD_MAN_WAITING_FOR_THIS_DAY)->update =
				(OBJECT_UPDATE)scAlhafraTakeLeadersLayer;
			fdActorSetSprPriority(ACTOR_OLD_MAN_ON_SAILS, 2);
			fdActorSetSprPriority(ACTOR_MAN_NOT_EXCITED_BY_THE_SHIP, 3);
			fdActorSetSprPriority(ACTOR_WOMAN_ON_ALHAFRAS_FUTURE, 3);
			fdActorSetSprPriority(ACTOR_WOMAN_ADMIRING_THE_MAST, 1);
		} else {
			fdActorGet(ACTOR_WOMAN_ON_THE_LOST_SHIPS)->update =
				(OBJECT_UPDATE)scAlhafraTakeLeadersLayer;
			fdActorGetById(ACTOR_GIRL_KEPT_FROM_THE_PORT)->update =
				(OBJECT_UPDATE)scAlhafraTakeLeadersLayer;
			fdActorGetById(ACTOR_MAN_FINISHING_A_CANOE)->update =
				(OBJECT_UPDATE)scAlhafraTakeLeadersLayer;
			fdActorSetSprPriority(ACTOR_WELCOMING_YOUNG_MAN, 2);
			fdActorSetSprPriority(ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, 3);
			fdActorSetSprPriority(ACTOR_SAILOR_ON_THE_SAILBOAT, 2);
			fdActorSetSprPriority(ACTOR_SAILOR_BUILDING_HOUSES, 3);
			fdActorSetSprPriority(ACTOR_HUNGRY_BOY, 2);
			fdActorSetSprPriority(ACTOR_LUCKY_MOTHERS_HUSBAND, 3);
			fdActorSetSprPriority(ACTOR_LUCKY_MOTHER, 1);
			fdActorSetSprPriority(ACTOR_LUCKY_MOTHERS_CHILD, 3);
			fdActorSetSprPriority(ACTOR_BOY_HUNTING_FOR_COINS, 3);
			fdActorSetSprPriority(ACTOR_HUNGRY_BOYS_FATHER, 3);
			fdActorSetSprPriority(ACTOR_MANOR_GUARD, 3);
		}
	} else if(syGameFlagGet(FLAG_BRIGGS_CAUGHT)) {
		fdActorGetById(ACTOR_WOMAN_ON_THE_LOST_SHIPS)->update =
			(OBJECT_UPDATE)scAlhafraTakeLeadersLayer;
		fdActorGetById(ACTOR_GIRL_KEPT_FROM_THE_PORT)->update =
			(OBJECT_UPDATE)scAlhafraTakeLeadersLayer;
		fdActorSetSprPriority(ACTOR_WELCOMING_YOUNG_MAN, 2);
		fdActorSetSprPriority(ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, 3);
		fdActorSetSprPriority(ACTOR_SAILOR_ON_THE_SAILBOAT, 3);
		fdActorSetSprPriority(ACTOR_SAILOR_BUILDING_HOUSES, 3);
		fdActorSetSprPriority(ACTOR_MAN_FINISHING_A_CANOE, 2);
		fdActorSetSprPriority(ACTOR_HUNGRY_BOY, 2);
		fdActorSetSprPriority(ACTOR_LUCKY_MOTHERS_HUSBAND, 3);
		fdActorSetSprPriority(ACTOR_LUCKY_MOTHER, 1);
		fdActorSetSprPriority(ACTOR_LUCKY_MOTHERS_CHILD, 3);
		fdActorSetSprPriority(ACTOR_BOY_HUNTING_FOR_COINS, 3);
		fdActorSetSprPriority(ACTOR_HUNGRY_BOYS_FATHER, 3);
		fdActorSetMovementType(fdActorGetById(ACTOR_WOMAN_WAITING_AT_THE_INN),
			MOVEMENT_TYPE_TAKE_LEADER_LAYER);
		if(!syGameFlagGet(FLAG_HUNGRY_BOY_FED))
			fdActorSetAnimationById(ACTOR_HUNGRY_BOY, 5);
		if(syGameFlagGet(FLAG_GOLD_COINS_FOUND)) {
			fdActorSetAnimationById(ACTOR_LUCKY_MOTHERS_CHILD, 6);
			fdActorSetAnimationById(ACTOR_BOY_HUNTING_FOR_COINS, 6);
		}
		if(syGameFlagGet(FLAG_LET_INTO_CAVE)) {
			fdActorSetPos(ACTOR_HUNGRY_BOYS_FATHER, 56, 328);
			fdActorFaceDirectionById(ACTOR_HUNGRY_BOYS_FATHER, FACING_EAST, 0);
		}
	} else {
		fdActorSetSprPriority(ACTOR_WELCOMING_YOUNG_MAN, 2);
		fdActorSetSprPriority(ACTOR_SAILOR_ON_THE_SAILBOAT, 2);
		fdActorSetSprPriority(ACTOR_MANOR_GUARD, 3);
		fdActorGetById(ACTOR_SAILOR_BUILDING_HOUSES)->update =
			(OBJECT_UPDATE)scAlhafraTakeLeadersLayer;
		if(syGameFlagGet(FLAG_ALHAFRA_MADRANS_ARRIVED))
			fdActorSetPos(ACTOR_WOMAN_ON_THE_LOST_SHIPS, 536, 760);
		if(syGameFlagGet(FLAG_PIRATE_LEFT))
			fdActorSetPos(ACTOR_PIRATE, 0, 0);
		fdFlagBlockPlaceAllMarkingEdges((Sint32)&AlhafraPuddles);
		fdActorSetMovementType(
			fdActorGetById(ACTOR_PUDDLE_A), MOVEMENT_TYPE_TAKE_LEADER_LAYER);
		fdActorSetMovementType(
			fdActorGetById(ACTOR_PUDDLE_B), MOVEMENT_TYPE_TAKE_LEADER_LAYER);
		fdActorSetMovementType(
			fdActorGetById(ACTOR_PUDDLE_C), MOVEMENT_TYPE_TAKE_LEADER_LAYER);
	}
	return 0;
}

/* The pool of seawater the tidal wave left by the east road: its four colours
   in their palette, and the palette holding the dry ground's. */
enum {
	SEAWATER_POOL_PALETTE = 7,
	DRY_GROUND_PALETTE = 8,
	SEAWATER_POOL_COLOR = 11
};

/* Once Briggs is caught the pool has dried up: its colours take the dry
   ground's. */
Sint32 scPrepareMap(void)
{
	if(syGameFlagGet(FLAG_BRIGGS_CAUGHT)) {
		Uint16 *src =
			(Uint16 *)&BG_PLTT_COLOR(DRY_GROUND_PALETTE, SEAWATER_POOL_COLOR);

		syIOWrite16(
			*src, &BG_PLTT_COLOR(SEAWATER_POOL_PALETTE, SEAWATER_POOL_COLOR));
		src++;
		syIOWrite16(*src,
			&BG_PLTT_COLOR(SEAWATER_POOL_PALETTE, SEAWATER_POOL_COLOR + 1));
		src++;
		syIOWrite16(*src,
			&BG_PLTT_COLOR(SEAWATER_POOL_PALETTE, SEAWATER_POOL_COLOR + 2));
		syIOWrite16(src[1],
			&BG_PLTT_COLOR(SEAWATER_POOL_PALETTE, SEAWATER_POOL_COLOR + 3));
		fdMapGridCopyMetatileAttributes(37, 18, 2, 2, 37, 33);
		fdMapGridCopyMetatileAttributes(37, 18, 2, 3, 39, 31);
		fdMapGridCopyMetatileAttributes(50, 80, 7, 5, 35, 95);
	}
	return 0;
}

/* An actor's update: it takes the party leader's display layer, so it is drawn
   over or under what the leader is. */
void scAlhafraTakeLeadersLayer(void)
{
	fdActorTakeLeaderLayer();
}

extern Sint32 AlhafraElderStepsBackScript[];
extern Uint32 AlhafraWomanWalksOffScript[];
extern Uint32 AlhafraAlhafranLeadsBesideFelixScript[];
extern Uint32 AlhafraElderFollowsBesideFelixScript[];
extern Uint32 AlhafraWaitingMadranFollowsBesideFelixScript[];
extern Uint32 AlhafraMayorFollowsBesideFelixScript[];
extern Uint32 AlhafraSecondSoldierFollowsBesideFelixScript[];
extern Uint32 AlhafraFirstSoldierFollowsBesideFelixScript[];
extern Uint32 AlhafraWearyMadranFollowsBesideFelixScript[];
extern Uint32 AlhafraSilentMadranFollowsBesideFelixScript[];
extern Sint32 AlhafraAlhafranLeadsBehindFelixScript[];
extern Uint32 AlhafraElderFollowsBehindFelixScript[];
extern Uint32 AlhafraWaitingMadranFollowsBehindFelixScript[];
extern Uint32 AlhafraMayorFollowsBehindFelixScript[];
extern Uint32 AlhafraWearyMadranFollowsBehindFelixScript[];
extern Uint32 AlhafraFirstSoldierFollowsBehindFelixScript[];
extern Sint32 AlhafraSilentMadranFollowsBehindFelixScript[];
extern Uint32 AlhafraSecondSoldierFollowsBehindFelixScript[];

/* The mayor and elder of Madra arrive with their soldiers and townsfolk, ask
   the way to Alhafra's mayor and follow an Alhafran to the manor. */
void scAlhafraMadransArrive(Sint32 metatileBehavior)
{
	syGameFlagSet(FLAG_ALHAFRA_MADRANS_ARRIVED);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgAlhafraFinallyWeMadeIt);
	fdActorSetSpeed(ACTOR_MADRAS_ELDER, 1);
	fdActorSetSpeed(ACTOR_MADRAS_MAYOR, 1);
	fdActorSetSpeed(ACTOR_FIRST_MADRAN_SOLDIER, 1);
	fdActorSetSpeed(ACTOR_SECOND_MADRAN_SOLDIER, 1);
	fdActorSetSpeed(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 1);
	fdActorSetSpeed(ACTOR_WEARY_MADRAN, 1);
	fdActorSetSpeed(ACTOR_SILENT_MADRAN, 1);
	fdActorSetSpeed(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 0.5);
	fdActorSetSpeed(ACTOR_WOMAN_ON_THE_LOST_SHIPS, 0.5);
	if(metatileBehavior == METATILE_BEHAVIOR_MADRANS_ARRIVE_BEHIND_FELIX) {
		fdActorWalkByAndWait(ACTOR_FELIX, 0, -8);
	}
	fdActorSetPos(ACTOR_FIRST_MADRAN_SOLDIER, 408, 872);
	fdActorSetPos(ACTOR_SECOND_MADRAN_SOLDIER, 392, 888);
	/* "Finally! We made it!" */
	fdScriptShowMessageThenWait(ACTOR_FIRST_MADRAN_SOLDIER, 0, 10);
	if(metatileBehavior == METATILE_BEHAVIOR_MADRANS_ARRIVE_BEHIND_FELIX) {
		fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	} else {
		fdActorFaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
	}
	fdScriptWait(20);
	fdCameraMoveTo(400, CAMERA_KEEP, 808, 1);
	fdCameraWaitForArrival();
	fdActorWalkBy(ACTOR_FIRST_MADRAN_SOLDIER, 0, -96);
	fdActorWalkByAndWait(ACTOR_SECOND_MADRAN_SOLDIER, 0, -96);
	fdActorSetAnimationById(ACTOR_FIRST_MADRAN_SOLDIER, ANIM_STAND);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTHEAST, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTHEAST, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_EAST, 0);
	fdActorSetPos(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 392, 872);
	fdActorSetPos(ACTOR_SILENT_MADRAN, 408, 872);
	/* "We-We're here?" */
	syFieldScriptShowMessageThenWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, 10);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH, 0);
	fdActorFaceDirection(
		ACTOR_SECOND_MADRAN_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(20);
	fdActorWalkBy(ACTOR_SILENT_MADRAN, 0, -48);
	fdActorWalkByAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, -48);
	fdActorWaitForMoveById(ACTOR_SILENT_MADRAN);
	fdActorFaceDirectionById(ACTOR_SILENT_MADRAN, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_MADRAN_SOLDIER, 2);
	fdScriptWait(10);
	/* "Where's the Elder?" */
	syFieldScriptShowMessageThenWait(ACTOR_FIRST_MADRAN_SOLDIER, 0, 10);
	fdActorShowEmote(
		ACTOR_MADRAN_WAITING_FOR_THE_ELDER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorFaceDirection(
		ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_NORTH, 0);
	fdScriptWait(20);
	/* "He's coming now..." */
	syFieldScriptShowMessageThenWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, 10);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTHEAST, 0);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_FIRST_MADRAN_SOLDIER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SECOND_MADRAN_SOLDIER, ANIM_NOD);
	fdScriptWait(10);
	fdActorWalkBy(ACTOR_SILENT_MADRAN, 32, -48);
	fdActorWalkByAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 32, -48);
	fdActorWaitForMoveById(ACTOR_SILENT_MADRAN);
	fdActorFaceDirection(ACTOR_SILENT_MADRAN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_SOUTH, 0);
	fdActorSetSprPriority(ACTOR_SILENT_MADRAN, 3);
	fdScriptWait(20);
	fdActorSetPos(ACTOR_WEARY_MADRAN, 400, 872);
	/* "Haaa hoo hooo... Are we...here? I feel like I just walked 500 miles." */
	syFieldScriptShowMessageThenWait(ACTOR_WEARY_MADRAN, 0, 10);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH, 0);
	fdActorFaceDirection(
		ACTOR_SECOND_MADRAN_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_SOUTH, 0);
	fdScriptWait(20);
	fdActorWalkByAndWait(ACTOR_WEARY_MADRAN, 0, -32);
	fdScriptWait(1);
	fdActorSetAnimationById(ACTOR_WEARY_MADRAN, 5);
	fdScriptWait(90);
	fdActorRunRepeatedMotion(ACTOR_WEARY_MADRAN, 2);
	fdScriptWait(20);
	/* "I'm fine... I feel like I could walk 500 more!" */
	syFieldScriptShowMessageThenWait(ACTOR_WEARY_MADRAN, 0, 10);
	fdActorWalkByAndWaitById(ACTOR_WEARY_MADRAN, 0, -16);
	fdScriptWait(1);
	fdActorSetAnimationById(ACTOR_WEARY_MADRAN, 5);
	fdScriptWait(60);
	fdActorWalkByAndWait(ACTOR_WEARY_MADRAN, 8, -32);
	fdActorFaceDirection(ACTOR_WEARY_MADRAN, FACING_SOUTH, 0);
	fdScriptWait(1);
	fdActorSetAnimationById(ACTOR_WEARY_MADRAN, 5);
	fdScriptWait(30);
	fdActorSetPos(ACTOR_MADRAS_MAYOR, 392, 872);
	fdActorSetPos(ACTOR_MADRAS_ELDER, 408, 872);
	fdActorWalkBy(ACTOR_MADRAS_MAYOR, 0, -32);
	fdActorWalkByAndWait(ACTOR_MADRAS_ELDER, 0, -32);
	fdActorSetAnimationById(ACTOR_MADRAS_MAYOR, ANIM_STAND);
	fdScriptWait(1);
	fdActorSetAnimation(ACTOR_MADRAS_ELDER, 5);
	scFieldActorWatchAndSetScript(
		ACTOR_FIRST_MADRAN_SOLDIER, ACTOR_MADRAS_MAYOR);
	scFieldActorWatchAndSetScript(
		ACTOR_SECOND_MADRAN_SOLDIER, ACTOR_MADRAS_MAYOR);
	scFieldActorWatchAndSetScript(
		ACTOR_MADRAN_WAITING_FOR_THE_ELDER, ACTOR_MADRAS_MAYOR);
	scFieldActorWatchAndSetScript(ACTOR_WEARY_MADRAN, ACTOR_MADRAS_MAYOR);
	fdActorWatchAndSetScript(ACTOR_SILENT_MADRAN, ACTOR_MADRAS_MAYOR);
	fdScriptWait(20);
	fdActorFaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
	fdScriptWait(20);
	fdActorFaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTH, 0);
	fdScriptWait(10);
	scFieldActorJump(ACTOR_MADRAS_MAYOR, 6, 15);
	scFieldActorJump(ACTOR_MADRAS_MAYOR, 6, 23);
	fdActorSetAnimation(ACTOR_WEARY_MADRAN, ANIM_STAND);
	/* "So...is this Alhafra?" */
	syFieldScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorSetMovementTypeById(
		ACTOR_MADRAS_ELDER, (Sint32)AlhafraElderStepsBackScript);
	fdScriptWait(20);
	fdActorWalkByAndWait(ACTOR_MADRAS_MAYOR, 0, -32);
	fdActorWalkByAndWaitById(ACTOR_MADRAS_MAYOR, -32, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
	fdScriptWait(10);
	/* "Madra's a pretty big place, but this town is huge!" */
	syFieldScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_SHAKE_HEAD);
	/* "We didn't come to Alhafra to shop, Mister Mayor." */
	syFieldScriptShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
	fdActorShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorFaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
	fdScriptWait(20);
	/* "We're here to bring in Briggs the Champa." */
	syFieldScriptShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
	fdActorSetAnimation(ACTOR_MADRAS_ELDER, 5);
	fdActorWalkByAndWait(ACTOR_MADRAS_MAYOR, 64, 0);
	fdActorFaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTHEAST, 0);
	fdScriptWait(20);
	/* "I know that!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorSetAnimationById(ACTOR_MADRAS_ELDER, ANIM_STAND);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
	fdScriptWait(10);
	/* "Then we should go straight to speak with Alhafra's mayor." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
	fdActorStop(ACTOR_FIRST_MADRAN_SOLDIER);
	fdActorStop(ACTOR_SECOND_MADRAN_SOLDIER);
	fdActorStop(ACTOR_MADRAN_WAITING_FOR_THE_ELDER);
	fdActorStop(ACTOR_WEARY_MADRAN);
	fdActorStop(ACTOR_SILENT_MADRAN);
	fdActorSetAnimationById(ACTOR_MADRAS_ELDER, 5);
	fdScriptWait(50);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTHEAST, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_SOUTHEAST, 0);
	fdScriptWait(20);
	fdActorSetMovementTypeById(
		ACTOR_WOMAN_ON_THE_LOST_SHIPS, (Sint32)AlhafraWomanWalksOffScript);
	scFieldActorJump(ACTOR_FIRST_MADRAN_SOLDIER, 6, 15);
	scFieldActorJump(ACTOR_FIRST_MADRAN_SOLDIER, 6, 23);
	/* "We're in no shape to go present ourselves to the mayor right now!" */
	fdScriptShowMessageThenWait(ACTOR_FIRST_MADRAN_SOLDIER, 0, 10);
	fdScriptWait(10);
	fdActorFaceDirectionById(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTH, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_SOUTH, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorFaceDirectionById(
		ACTOR_SECOND_MADRAN_SOLDIER, FACING_EAST_SOUTHEAST, 0);
	fdScriptWait(20);
	/* "Look, Briggs is obviously not here, and besides, we already know..." */
	syFieldScriptShowMessageThenWait(ACTOR_SECOND_MADRAN_SOLDIER, 0, 10);
	fdActorSetAnimationById(ACTOR_MADRAS_ELDER, ANIM_STAND);
	fdScriptWait(10);
	fdActorShowEmoteById(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	fdActorFaceDirection(ACTOR_MADRAS_ELDER, FACING_WEST, 0);
	fdScriptWait(20);
	/* "Silence!" */
	syFieldScriptShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
	syScriptCameraFollowActor(ACTOR_MADRAS_ELDER, 1);
	fdCameraWaitForArrival();
	fdActorWalkToAndWait(ACTOR_MADRAS_ELDER, 488, 808);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_WEARY_MADRAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SILENT_MADRAN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MADRAS_ELDER, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_SHAKE_HEAD);
	fdCameraSetSpeed(4);
	fdCameraMoveTo(648, CAMERA_KEEP, 536, 1);
	fdCameraWaitForArrival();
	fdScriptWait(10);
	/* "It seems Alhafra has felt the effects of the tidal wave as well..." */
	syFieldScriptShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
	fdCameraMoveTo(440, CAMERA_KEEP, 808, 1);
	fdCameraWaitForArrival();
	fdCameraSetSpeed(1);
	fdScriptWait(20);
	fdActorWalkByAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 32, 36);
	fdActorWalkByAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 32, 0);
	fdActorWalkToAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 504, 808);
	fdActorFaceDirectionById(
		ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(30);
	scFieldActorStartRepeatedMotion(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 3);
	fdEffectSetAttached(ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(50);
	fdActorFaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, ANIM_NOD);
	fdScriptWait(10);
	/* "It looks like they suffered a good deal of damage..." */
	syFieldScriptShowMessageThenWait(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
	fdScriptWait(20);
	fdActorFaceDirectionById(ACTOR_MADRAS_ELDER, FACING_EAST, 0);
	fdScriptWait(20);
	/* "That would explain why Alhafra doesn't seem to have any boats for..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_MADRAS_ELDER, 0, 10);
	fdActorShowEmoteById(
		ACTOR_WEARY_MADRAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "You know, just looking around, I get the impression they only had..." */
	syFieldScriptShowMessageThenWait(ACTOR_WEARY_MADRAN, 0, 10);
	fdActorFaceDirection(ACTOR_MADRAS_ELDER, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
	fdScriptWait(10);
	/* "So if you really needed a boat, the only way to get one would be
	   to..." */
	syFieldScriptShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
	fdScriptWait(10);
	/* "...Which means the pirates must still be here, trying to get a boat!" */
	syFieldScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirectionById(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTHEAST, 0);
	fdScriptWait(30);
	scFieldActorJump(ACTOR_FIRST_MADRAN_SOLDIER, 6, 15);
	scFieldActorJump(ACTOR_FIRST_MADRAN_SOLDIER, 6, 23);
	/* "So, you think we've actually caught up with Briggs?" */
	syFieldScriptShowMessageThenWait(ACTOR_FIRST_MADRAN_SOLDIER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_MADRAN_SOLDIER, ANIM_NOD);
	fdScriptWait(10);
	/* "No doubt about it!" */
	syFieldScriptShowMessageThenWait(ACTOR_SECOND_MADRAN_SOLDIER, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_SOUTHEAST, 0);
	/* "That's why I want to meet the mayor of Alhafra before the Champa..." */
	syFieldScriptShowMessageThenWait(ACTOR_MADRAS_ELDER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
	fdScriptWait(10);
	/* "Oh yeah... I get it now. In that case, let's go see Alhafra's
	   mayor..." */
	syFieldScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorSetPos(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 408, 696);
	fdActorWalkToAndWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 408, 760);
	fdActorWalkToAndWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 448, 792);
	fdActorWalkToAndWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 448, 808);
	fdActorWalkByAndWait(ACTOR_MADRAS_MAYOR, 8, 0);
	/* "Um... Say, excuse me..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorShowEmoteById(
		ACTOR_ALHAFRAN_GIVING_DIRECTIONS, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorFaceDirection(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorShowEmoteById(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Where would we find the mayor of Alhafra?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 2);
	fdScriptWait(10);
	FieldStateWork->fastForward = 0;
	/* "Uh... The mayor?" */
	syFieldScriptShowMessageThenWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_MADRAS_ELDER, 0.5);
	fdActorSetSpeed(ACTOR_MADRAS_MAYOR, 0.5);
	fdActorSetSpeed(ACTOR_FIRST_MADRAN_SOLDIER, 0.5);
	fdActorSetSpeed(ACTOR_SECOND_MADRAN_SOLDIER, 0.5);
	fdActorSetSpeed(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0.5);
	fdActorSetSpeed(ACTOR_WEARY_MADRAN, 0.5);
	fdActorSetSpeed(ACTOR_SILENT_MADRAN, 0.5);
	fdActorSetSpeed(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 0.5);
	syScriptCameraFollowActor(ACTOR_MADRAS_MAYOR, 1);
	fdCameraWaitForArrival();
	if(metatileBehavior == METATILE_BEHAVIOR_MADRANS_ARRIVE_BESIDE_FELIX) {
		scFieldActorSetSprPriority(ACTOR_MADRAS_MAYOR, 2);
		scFieldActorWatchAndSetScript(
			ACTOR_FIRST_MADRAN_SOLDIER, ACTOR_MADRAS_ELDER);
		scFieldActorWatchAndSetScript(
			ACTOR_SECOND_MADRAN_SOLDIER, ACTOR_MADRAS_ELDER);
		scFieldActorWatchAndSetScript(ACTOR_WEARY_MADRAN, ACTOR_MADRAS_ELDER);
		fdActorSetMovementTypeById(ACTOR_ALHAFRAN_GIVING_DIRECTIONS,
			(Sint32)AlhafraAlhafranLeadsBesideFelixScript);
		fdActorSetMovementTypeById(
			ACTOR_MADRAS_ELDER, (Sint32)AlhafraElderFollowsBesideFelixScript);
		fdActorSetMovementTypeById(ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
			(Sint32)AlhafraWaitingMadranFollowsBesideFelixScript);
		fdScriptWait(80);
		scFieldActorSetSprPriority(ACTOR_MADRAS_ELDER, 3);
		scFieldActorSetSprPriority(ACTOR_MADRAS_MAYOR, 3);
		scFieldActorSetSprPriority(ACTOR_FIRST_MADRAN_SOLDIER, 3);
		scFieldActorSetSprPriority(ACTOR_SECOND_MADRAN_SOLDIER, 3);
		scFieldActorSetSprPriority(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 3);
		scFieldActorSetSprPriority(ACTOR_WEARY_MADRAN, 3);
		scFieldActorSetSprPriority(ACTOR_SILENT_MADRAN, 3);
		scFieldActorSetSprPriority(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 3);
		scFieldActorWatchAndSetScript(ACTOR_SILENT_MADRAN, ACTOR_MADRAS_ELDER);
		fdActorSetMovementTypeById(
			ACTOR_MADRAS_MAYOR, (Sint32)AlhafraMayorFollowsBesideFelixScript);
		fdScriptWait(170);
		fdActorSetMovementTypeById(ACTOR_SECOND_MADRAN_SOLDIER,
			(Sint32)AlhafraSecondSoldierFollowsBesideFelixScript);
		fdScriptWait(10);
		fdActorSetMovementTypeById(ACTOR_FIRST_MADRAN_SOLDIER,
			(Sint32)AlhafraFirstSoldierFollowsBesideFelixScript);
		fdActorSetMovementTypeById(ACTOR_WEARY_MADRAN,
			(Sint32)AlhafraWearyMadranFollowsBesideFelixScript);
		fdScriptWait(60);
		fdActorSetMovementTypeById(ACTOR_SILENT_MADRAN,
			(Sint32)AlhafraSilentMadranFollowsBesideFelixScript);
	} else {
		scFieldActorSetSprPriority(ACTOR_MADRAS_MAYOR, 2);
		scFieldActorWatchAndSetScript(
			ACTOR_MADRAS_MAYOR, ACTOR_ALHAFRAN_GIVING_DIRECTIONS);
		scFieldActorWatchAndSetScript(
			ACTOR_FIRST_MADRAN_SOLDIER, ACTOR_MADRAS_MAYOR);
		scFieldActorWatchAndSetScript(
			ACTOR_SECOND_MADRAN_SOLDIER, ACTOR_MADRAS_MAYOR);
		scFieldActorWatchAndSetScript(ACTOR_WEARY_MADRAN, ACTOR_MADRAS_MAYOR);
		fdActorSetMovementTypeById(ACTOR_ALHAFRAN_GIVING_DIRECTIONS,
			(Sint32)AlhafraAlhafranLeadsBehindFelixScript);
		fdScriptWait(60);
		fdActorSetMovementTypeById(
			ACTOR_MADRAS_ELDER, (Sint32)AlhafraElderFollowsBehindFelixScript);
		fdActorSetMovementTypeById(ACTOR_MADRAN_WAITING_FOR_THE_ELDER,
			(Sint32)AlhafraWaitingMadranFollowsBehindFelixScript);
		fdScriptWait(70);
		scFieldActorWatchAndSetScript(ACTOR_SILENT_MADRAN, ACTOR_MADRAS_ELDER);
		fdActorSetMovementTypeById(
			ACTOR_MADRAS_MAYOR, (Sint32)AlhafraMayorFollowsBehindFelixScript);
		scFieldActorSetSprPriority(ACTOR_MADRAS_ELDER, 3);
		scFieldActorSetSprPriority(ACTOR_MADRAS_MAYOR, 3);
		scFieldActorSetSprPriority(ACTOR_FIRST_MADRAN_SOLDIER, 3);
		scFieldActorSetSprPriority(ACTOR_SECOND_MADRAN_SOLDIER, 3);
		scFieldActorSetSprPriority(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 3);
		scFieldActorSetSprPriority(ACTOR_WEARY_MADRAN, 3);
		scFieldActorSetSprPriority(ACTOR_SILENT_MADRAN, 3);
		fdActorSetSprPriority(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 3);
		fdScriptWait(120);
		fdActorSetMovementTypeById(ACTOR_WEARY_MADRAN,
			(Sint32)AlhafraWearyMadranFollowsBehindFelixScript);
		fdScriptWait(40);
		fdActorSetMovementTypeById(ACTOR_FIRST_MADRAN_SOLDIER,
			(Sint32)AlhafraFirstSoldierFollowsBehindFelixScript);
		fdActorSetMovementTypeById(ACTOR_SILENT_MADRAN,
			(Sint32)AlhafraSilentMadranFollowsBehindFelixScript);
		fdScriptWait(30);
		fdActorSetMovementTypeById(ACTOR_SECOND_MADRAN_SOLDIER,
			(Sint32)AlhafraSecondSoldierFollowsBehindFelixScript);
	}
	fdScriptWait(300);
	fdActorWaitForScriptEndById(ACTOR_WEARY_MADRAN);
	fdScriptWait(50);
	fdCameraMoveTo(248, CAMERA_KEEP, 664, 1);
	fdCameraWaitForArrival();
	/* "Just keep going down this road. It's the last house. You can't miss
	   it." */
	syFieldScriptShowMessageThenWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, 0, 10);
	fdActorFaceDirectionById(
		ACTOR_ALHAFRAN_GIVING_DIRECTIONS, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
	fdScriptWait(20);
	fdActorWalkByAndWait(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, -16, 0);
	fdActorFaceDirectionById(ACTOR_ALHAFRAN_GIVING_DIRECTIONS, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_MADRAS_ELDER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_WEARY_MADRAN, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_MADRAS_MAYOR, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MADRAS_ELDER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FIRST_MADRAN_SOLDIER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SECOND_MADRAN_SOLDIER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SILENT_MADRAN, 3);
	fdActorSetAnimationAndWait(ACTOR_WEARY_MADRAN, ANIM_NOD);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_MADRAS_ELDER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_FIRST_MADRAN_SOLDIER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SECOND_MADRAN_SOLDIER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_WEARY_MADRAN, FACING_NORTH, 0);
	fdActorSetSpeed(ACTOR_MADRAS_ELDER, 1.9);
	fdActorSetSpeed(ACTOR_MADRAS_MAYOR, 1.9);
	fdActorSetSpeed(ACTOR_FIRST_MADRAN_SOLDIER, 1.9);
	fdActorSetSpeed(ACTOR_SECOND_MADRAN_SOLDIER, 1.9);
	fdActorSetSpeed(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 1.9);
	fdActorSetSpeed(ACTOR_WEARY_MADRAN, 1.9);
	fdActorSetSpeed(ACTOR_SILENT_MADRAN, 1.9);
	fdActorWalkBy(ACTOR_MADRAS_ELDER, 0, -150);
	fdActorWalkBy(ACTOR_MADRAS_MAYOR, 0, -150);
	fdActorWalkBy(ACTOR_FIRST_MADRAN_SOLDIER, 0, -150);
	fdActorWalkBy(ACTOR_SECOND_MADRAN_SOLDIER, 0, -150);
	fdActorWalkBy(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, -150);
	fdActorWalkBy(ACTOR_SILENT_MADRAN, 0, -150);
	fdActorWalkByAndWait(ACTOR_WEARY_MADRAN, 0, -150);
	fdActorSetPos(ACTOR_MADRAS_ELDER, 0, 0);
	fdActorSetPos(ACTOR_MADRAS_MAYOR, 0, 0);
	fdActorSetPos(ACTOR_FIRST_MADRAN_SOLDIER, 0, 0);
	fdActorSetPos(ACTOR_SECOND_MADRAN_SOLDIER, 0, 0);
	fdActorSetPos(ACTOR_MADRAN_WAITING_FOR_THE_ELDER, 0, 0);
	fdActorSetPos(ACTOR_WEARY_MADRAN, 0, 0);
	fdActorSetPos(ACTOR_SILENT_MADRAN, 0, 0);
	fdScriptWait(15);
	fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdScriptEnd();
}

/* The pirate recognizes the mayor of Madra and runs to warn Briggs's crew,
   after answering the party if they read his mind. */
void scAlhafraPirateSeesMadrasMayor(Sint32 how)
{
	fdScriptSetFlag(FLAG_PIRATE_LEFT);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgAlhafraOooThisAintGood);
	if(how == PIRATE_READ_BY_MIND_READ) {
		syFieldScriptWait(10);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 168, 736);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	}
	fdActorShowEmote(ACTOR_PIRATE, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Ooo... This ain't good!" */
	syFieldScriptShowMessageThenWait(ACTOR_PIRATE, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PIRATE, ANIM_NOD);
	syFieldScriptWait(10);
	/* "That guy looked like the mayor of Madra..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIRATE, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PIRATE, ANIM_SHAKE_HEAD);
	/* "He must have come here looking for Briggs." */
	syFieldScriptShowMessageThenWait(ACTOR_PIRATE, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PIRATE, 2);
	syFieldScriptWait(20);
	/* "I'd better go tell the others!" */
	syFieldScriptShowMessageThenWait(ACTOR_PIRATE, 0, 10);
	if(how == PIRATE_READ_BY_MIND_READ) {
		fdActorWalkByAndWaitById(ACTOR_PIRATE, -8, 0);
		fdScriptWait(15);
		fdActorStartRepeatedMotion(ACTOR_PIRATE, 3);
		fdEffectSetAttached(
			ACTOR_PIRATE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(40);
		fdActorGetById(ACTOR_PIRATE)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkToAndWaitById(ACTOR_PIRATE, 192, 736);
		fdScriptWait(1);
		fdActorGetById(ACTOR_PIRATE)->steeringFlg |= ACTOR_FACES_MOTION;
		fdScriptWait(10);
		/* "What's your problem? What are you, some kind of freak?" */
		fdScriptShowMessageThenWait(ACTOR_PIRATE, 0, 10);
		fdActorShowEmote(ACTOR_PIRATE, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		/* "Well? What!? You got a problem with me?" */
		fdScriptOpenMessage(ACTOR_PIRATE, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
			fdScriptWait(30);
			fdActorRunRepeatedMotion(ACTOR_PIRATE, 2);
			fdScriptWait(20);
			/* Yes: "Ah, who cares what you think? I've got stuff to do, so
			   scram, kid!" */
			fdScriptShowMessageThenWait(ACTOR_PIRATE, 0, 10);
			FieldStateWork->msg = (FieldStateWork->msg + 1);
		} else {
			fdScriptWait(30);
			FieldStateWork->msg = (FieldStateWork->msg + 1);
			/* No: "So where do you get off invading my personal space, huh?" */
			fdScriptShowMessageThenWait(ACTOR_PIRATE, 0, 10);
		}
		fdActorGetById(ACTOR_FELIX)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkToAndWait(ACTOR_FELIX, 152, 752);
		fdScriptWait(1);
		fdActorGetById(ACTOR_FELIX)->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_PIRATE, ANIM_NOD);
		fdScriptWait(10);
		fdActorFaceDirectionById(ACTOR_PIRATE, FACING_NORTHEAST, 0);
		fdScriptWait(70);
		fdActorShowEmote(ACTOR_PIRATE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
		/* "Where was I? Oh yeah, I was going to tell them about the mayor of
		   Madra!" */
		fdScriptShowMessageThenWait(ACTOR_PIRATE, 0, 10);
	}
	fdActorWalkToAndWait(ACTOR_PIRATE, 184, 728);
	fdActorWalkByAndWait(ACTOR_PIRATE, 0, -110);
	if(how == PIRATE_STEPPED_PAST) {
		fdScriptWait(50);
		fdActorSetPos(ACTOR_PIRATE, 264, 648);
		fdActorWalkByAndWaitById(ACTOR_PIRATE, 0, 32);
		fdActorWalkByAndWaitById(ACTOR_PIRATE, 150, 0);
	}
	fdActorSetPos(ACTOR_PIRATE, 0, 0);
	fdScriptEnd();
}

/* clang-format off */
/* The three puddles Frost freezes into pillars, each with the flag set while */
/* it stands (fdFlagBlockPlaceAll). */
Sint16 AlhafraPuddles[] = {
	ACTOR_PUDDLE_A, FLAG_PUDDLE_A_FROZEN, ACTOR_PUDDLE_B, FLAG_PUDDLE_B_FROZEN,
	ACTOR_PUDDLE_C, FLAG_PUDDLE_C_FROZEN, -1, 0,
};

Uint32 AlhafraLookAroundScript[] = {
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_WAIT(40),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_WAIT(60),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_WAIT(60),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(Alhafra),
	SCENE_EXIT_UNLESS(1, AlhafraRooms, 1, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, AlhafraRooms, 2, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(5, AlhafraRooms, 5, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(6, AlhafraRooms, 6, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(7, AlhafraRooms, 7, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(8, AlhafraJail, 8, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(9, WorldMap, 20, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(11, EasternAlhafra, 1, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(12, AlhafranCave, 1, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(14, AlhafraManor, 1, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE AlhafraActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 280, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 272, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 320, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 824, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 808, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 728, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 744, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 664, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 576, 0, 520, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 560, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 568, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 632, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 648, 0, 568, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 736, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 568, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 584, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE AlhafraActorPlacementsOnceBriggsIsCaught[] = {
	ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 312, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 312, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 824, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 568, 0, 792, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 728, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 744, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 248, 0, 616, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 520, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 712, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 536, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 600, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 504, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 504, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 552, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 576, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 616, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 488, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 472, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 456, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE AlhafraActorPlacementsOnceTheShipIsStolen[] = {
	ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 280, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 272, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 328, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 824, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 568, 0, 792, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 728, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 744, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 248, 0, 616, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 520, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 712, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 536, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 440, 0, 800, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 656, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 552, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 568, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 568, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 488, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 472, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 456, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 456, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE AlhafraActorPlacementsOnceTheMastIsCleared[] = {
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 600, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 544, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 536, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 616, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 648, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 320, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 520, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 568, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 600, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 552, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 624, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 592, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 576, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, (Sint32)AlhafraLookAroundScript, 648, 0, 504, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 608, FACING_EAST, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* A doorway with no door: nothing to open. */
Uint16 AlhafraNoDoor[] = {
	MAPGRID_ANIMATION_STEPS_END,
};

/* A door opening: one step of metatiles copied over it. */
Uint16 AlhafraDoorOpening[] = {
	MAPGRID_ANIMATION_STEP(63, 2, 1, 2, 6),
	MAPGRID_ANIMATION_STEPS_END,
};

/* The other shape of door's opening, from the metatiles beside the first's. */
Uint16 AlhafraSecondDoorOpening[] = {
	MAPGRID_ANIMATION_STEP(62, 2, 1, 2, 6),
	MAPGRID_ANIMATION_STEPS_END,
};

/* The doors Felix opens by walking into them, a row for each exit number */
/* (scAlhafraEnterDoor): the door's opening and its metatile; a row with none */
/* opens nothing. */
MAP_GRID_METATILE_ANIMATION AlhafraDoors[] = {
	{NULL, 0, 0},
	{AlhafraDoorOpening, 27, 37},
	{AlhafraDoorOpening, 27, 43},
	{NULL, 0, 0},
	{NULL, 0, 0},
	{AlhafraDoorOpening, 21, 36},
	{AlhafraDoorOpening, 38, 46},
	{AlhafraNoDoor, 0, 0},
	{AlhafraSecondDoorOpening, 11, 33},
	{NULL, 0, 0},
	{NULL, 0, 0},
	{NULL, 0, 0},
	{AlhafraSecondDoorOpening, 5, 9},
	{NULL, 0, 0},
	{AlhafraDoorOpening, 16, 9},
};

CUE AlhafraCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, 97, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{EXAMINE_CUE(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafraItLooksLikeThisWell))},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraRevealTreasureSpot)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraHideTreasureSpot)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraOpenWeaponShop)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraOpenArmorShop)},
	{CUE_STEP, METATILE_BEHAVIOR_PIRATE_BEHIND, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraDrawPirateBehind)},
	{CUE_STEP, METATILE_BEHAVIOR_PIRATE_IN_FRONT, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraDrawPirateInFront)},
	{CUE_STEP, METATILE_BEHAVIOR_MADRANS_ARRIVE_BEHIND_FELIX, FLAG_ALHAFRA_MADRANS_ARRIVED, SCRIPT_REF(scAlhafraMadransArrive)},
	{CUE_STEP, METATILE_BEHAVIOR_MADRANS_ARRIVE_BESIDE_FELIX, FLAG_ALHAFRA_MADRANS_ARRIVED, SCRIPT_REF(scAlhafraMadransArrive)},
	{CUE_STEP, METATILE_BEHAVIOR_PIRATE_RUNS_OFF, FLAG_PIRATE_LEFT, SCRIPT_REF(scAlhafraPirateRunsToWarnTheCrew)},
	{CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeWereHitByA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraLosingAllMyMostExpensive)},
	{CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraThatTidalWaveMayWell)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraItDoesntMatterHowWell)},
	{CUE_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraWelcomeToAlhafraThisIs)},
	{CUE_TALK, ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraAMassiveTidalWaveHit)},
	{CUE_TALK, ACTOR_MAN_ASKING_ABOUT_INDRA, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraAskWhetherIndraMoved)},
	{CUE_TALK, ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraRightAfterMyBoatGot)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraThisGuysProbablyWonderingAbout)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraLosingOurBoatsWasAwful)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_ABOUT_INDRA, CONDITION_ALWAYS, MSGNO(MsgAlhafraIJustCantBelieveA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraThoseGuysLookingForA)},
	{CUE_TALK, ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraThoseMenDidntLookLike)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraThosePeopleThatJustCame)},
	{CUE_TALK, ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeveHadALotOf)},
	{CUE_TALK, ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraOurPortWasRuinedBy)},
	{CUE_TALK, ACTOR_PARTNER_WAITING_FOR_THE_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheOnlyBoatSparedBy)},
	{CUE_TALK, ACTOR_MAN_FINISHING_A_CANOE, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraBackloggedWithCanoes)},
	{CUE_TALK, ACTOR_SAILOR_ON_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraThatBoatAtTheWharf)},
	{CUE_TALK, ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhyWouldTheMayorGo)},
	{CUE_TALK, ACTOR_PIRATE, CONDITION_ALWAYS, MSGNO(MsgAlhafraSilence)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfThisGuysLookingTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhoWouldComeToAlhafra)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, CONDITION_ALWAYS, MSGNO(MsgAlhafraThisTidalWavesActuallyBeen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIDontKnowWhyThey)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PARTNER_WAITING_FOR_THE_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderIfMyPartner)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FINISHING_A_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeCantRaiseTheMast)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIStillCantBelieveThose)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraItMayBeNewfangledBut)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIRATE | CUE_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraReadPiratesMind)},
	{CUE_TALK, ACTOR_MANOR_GUARD, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraMayorInAMeeting)},
	{CUE_TALK, ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraListenYouLookLikeAn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraIHateMyJobI)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraMyPartnersAlwaysInSuch)},
	{CUE_TALK, ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraNoOneIsAllowedBeyond)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorSaidNoOne)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_A, FLAG_PUDDLE_A_FROZEN, SCRIPT_REF(scAlhafraFreezePuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_B, FLAG_PUDDLE_B_FROZEN, SCRIPT_REF(scAlhafraFreezePuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_C, FLAG_PUDDLE_C_FROZEN, SCRIPT_REF(scAlhafraFreezePuddle)},
	{CUE_TABLE_END},
};

CUE AlhafraCuesOnceBriggsIsCaught[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, 97, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{EXAMINE_CUE(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafraItLooksLikeThisWell))},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraRevealTreasureSpot)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraHideTreasureSpot)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraOpenWeaponShop)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraOpenArmorShop)},
	{CUE_STEP, 25, FLAG_GOLD_COINS_FOUND, SCRIPT_REF(scAlhafraFindGoldCoins)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgAlhafraItLooksLikeTheMayor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgAlhafraIGuessOurMayorDidnt)},
	{CUE_TALK, ACTOR_ALHAFRAN_GIVING_DIRECTIONS, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgAlhafraIHearTheMayorOf)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALHAFRAN_GIVING_DIRECTIONS, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgAlhafraHeSeemsLikeAnHonest)},
	{CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhatBriggsWasTheLeader)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraBriggsWasVeryGenerousHe)},
	{CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraThatGroupFromMadraWas)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraGettingABoatForNothing)},
	{CUE_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraWereAllGoingToWork)},
	{CUE_TALK, ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraOnceThingsAreBackTo)},
	{CUE_TALK, ACTOR_MAN_ASKING_ABOUT_INDRA, CONDITION_ALWAYS, MSGNO(MsgAlhafraSoTheGuysWhoBought)},
	{CUE_TALK, ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraEveryoneIsTalkingAboutHow)},
	{CUE_TALK, ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorIsTheOnly)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorOfMadraApparently)},
	{CUE_TALK, ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, CONDITION_ALWAYS, MSGNO(MsgAlhafraINeverThoughtAFine)},
	{CUE_TALK, ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraICanHelpBuildHouses)},
	{CUE_TALK, ACTOR_PARTNER_WAITING_FOR_THE_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraWevePutOffFixingThe)},
	{CUE_TALK, ACTOR_MAN_FINISHING_A_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraOnceRepairsAreDoneHere)},
	{CUE_TALK, ACTOR_SAILOR_ON_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheyToldMeTheydHelp)},
	{CUE_TALK, ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraSailorToldToWork)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraItsSoNiceOfEveryone)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraEveryoneInTownIsSo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_ABOUT_INDRA, CONDITION_ALWAYS, MSGNO(MsgAlhafraYouKnowPiratesWhoPay)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraThosePiratesHaveTheCoolest)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraICantBelieveTheyLeft)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfOurMayorThinksIm)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, CONDITION_ALWAYS, MSGNO(MsgAlhafraYouKnowSittingHereFixing)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraHelpingPeopleOutFeelsReally)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PARTNER_WAITING_FOR_THE_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheFactThatMadraTook)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FINISHING_A_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderIfTheCrew)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraItsStrangeTheQuartersDown)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraWorkingOnDeckWithThe)},
	{CUE_TALK, ACTOR_YOUNG_MAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraOnceTheyFixTheOld)},
	{CUE_TALK, ACTOR_WOMAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWasAfraidMyHouse)},
	{CUE_TALK, ACTOR_HUNGRY_BOY, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraTalkToHungryBoy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfWeDontGetOut)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraYouKnowSinceWereRebuilding)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOY, FLAG_HUNGRY_BOY_FED, MSGNO(MsgAlhafraIWishSomeoneWouldJust)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOY, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderIfIShould)},
	{USE_ITEM_ON_ACTOR(ITEM_LARGE_BREAD), 27, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraGiveBreadToHungryBoy)},
	{CUE_TALK, ACTOR_LUCKY_MOTHERS_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgAlhafraAllMyNeighborsAreGoing)},
	{CUE_TALK, ACTOR_LUCKY_MOTHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhatAStrokeOfLuck)},
	{CUE_TALK, ACTOR_LUCKY_MOTHERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgAlhafraMyDadLostTheDraw)},
	{CUE_TALK, ACTOR_BOY_HUNTING_FOR_COINS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderIfTheresAny)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_MOTHERS_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgAlhafraLosingTheLotteryAndBeing)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_MOTHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraIThoughtTheHardPart)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_MOTHERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgAlhafraAllBecauseOfMyDads)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_HUNTING_FOR_COINS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIReallyShouldntBePeeking)},
	{CUE_TALK, ACTOR_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraAhFelixWelcomeWelcomeListen)},
	{CUE_TALK, ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraOhFelixGoOnIn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfIdKnownBriggsWas)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorWasReallyImpressed)},
	{CUE_TALK, ACTOR_HUNGRY_BOYS_FATHER, FLAG_BOY_TOLD_OF_CAVE, MSGNO(MsgAlhafraNoOneIsAllowedBeyondThis)},
	{CUE_TALK, ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraFatherLetsPartyToCave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOYS_FATHER, FLAG_LET_INTO_CAVE, MSGNO(MsgAlhafraTheMayorSaidNoOneGoes)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraNotThatItMattersThe)},
	{CUE_TABLE_END},
};

CUE AlhafraCuesOnceTheShipIsStolen[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, 97, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{EXAMINE_CUE(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafraItLooksLikeThisWell))},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraRevealTreasureSpot)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraHideTreasureSpot)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraOpenWeaponShop)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraOpenArmorShop)},
	{CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraEveryonesSayingThatBriggsProbably)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraSupposedlyTheCaveUnderThe)},
	{CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraSoBriggsStoleMadrasShip)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIDontKnowINever)},
	{CUE_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraAlhafraLostItsShipNow)},
	{CUE_TALK, ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraEvenThoughWeLostThe)},
	{CUE_TALK, ACTOR_MAN_ASKING_ABOUT_INDRA, CONDITION_ALWAYS, MSGNO(MsgAlhafraAfterWeDidAllThat)},
	{CUE_TALK, ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraItWasGreatToSee)},
	{CUE_TALK, ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhenTheyFindThatTheir)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraItsNotLikeTheBoat)},
	{CUE_TALK, ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheRepairsAreComingAlong)},
	{CUE_TALK, ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhenIsThisHouseGoing)},
	{CUE_TALK, ACTOR_PARTNER_WAITING_FOR_THE_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraItFeltSoGreatWhen)},
	{CUE_TALK, ACTOR_MAN_FINISHING_A_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraBackBeforeBriggsDestroyedThe)},
	{CUE_TALK, ACTOR_SAILOR_ON_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeFixedTheMastSo)},
	{CUE_TALK, ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfTheyCaughtAGood)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfOnlyWeHadThat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_LOST_SHIPS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIHopeTradeDoesntMake)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_ABOUT_INDRA, CONDITION_ALWAYS, MSGNO(MsgAlhafraItWasMadrasBoatIt)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WITH_A_CRUSHED_BOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorSeemedSoHappy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALHAFRAN_GIVING_DIRECTIONS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorOfMadraSeems)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheresNoPointInWorrying)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_PUTTING_THE_SAILOR_TO_WORK, CONDITION_ALWAYS, MSGNO(MsgAlhafraWereAllPullingTogetherTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_KEPT_FROM_THE_PORT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfMomWouldStopMaking)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PARTNER_WAITING_FOR_THE_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraIKnowItsTimeTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FINISHING_A_CANOE, CONDITION_ALWAYS, MSGNO(MsgAlhafraFirstHeStealsAShip)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhyWorryAboutBriggsIts)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_BUILDING_HOUSES, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfBriggsHadntStolenThat)},
	{CUE_TALK, ACTOR_YOUNG_MAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraThatGuysHouseIsntFixed)},
	{CUE_TALK, ACTOR_WOMAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorsBeenHandingOut)},
	{CUE_TALK, ACTOR_HUNGRY_BOY, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeHaveToStoreWhat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeCouldUseSomeHelp)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WAITING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgAlhafraIDontKnowWhatWill)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOY, CONDITION_ALWAYS, MSGNO(MsgAlhafraIveSeenFatherTryingTo)},
	{CUE_TALK, ACTOR_LUCKY_MOTHERS_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgAlhafraEvenIfWeFixIt)},
	{CUE_TALK, ACTOR_LUCKY_MOTHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraWithEveryonesHelpWeWere)},
	{CUE_TALK, ACTOR_BOY_HUNTING_FOR_COINS, CONDITION_ALWAYS, MSGNO(MsgAlhafraMomWasHappyToHear)},
	{CUE_TALK, ACTOR_LUCKY_MOTHERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgAlhafraThereAreALotOf)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_MOTHERS_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgAlhafraThisIsJustTooMuch)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_MOTHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraHesNeverGoingToGet)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_HUNTING_FOR_COINS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheresNoWayDadCan)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_MOTHERS_CHILD, CONDITION_ALWAYS, MSGNO(MsgAlhafraCarryingTheseThingsAllOver)},
	{CUE_TALK, ACTOR_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorOfAlhafraDoes)},
	{CUE_TALK, ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraYouReallyMadeAMess)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorWasSoMad)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_MANOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraOneThingsForSureWere)},
	{CUE_TALK, ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraBriggsCameThroughHereAnd)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_BOYS_FATHER, CONDITION_ALWAYS, MSGNO(MsgAlhafraThePoorGuardWasBlindfolded)},
	{CUE_TABLE_END},
};

CUE AlhafraCuesOnceTheMastIsCleared[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraEnterDoor)},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, 97, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{EXAMINE_CUE(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafraItLooksLikeThisWell))},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraRevealTreasureSpot)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAlhafraHideTreasureSpot)},
	{CUE_TALK, ACTOR_GRATEFUL_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheWholeTownIsGrateful)},
	{CUE_TALK, ACTOR_YOUNG_MAN_STANDING_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraImJustStandingGuardIn)},
	{CUE_TALK, ACTOR_WOMAN_ADMIRING_THE_MAST, CONDITION_ALWAYS, MSGNO(MsgAlhafraOurSailboatLooksFabulousWith)},
	{CUE_TALK, ACTOR_WOMAN_WONDERING_WHERE_BRIGGS_WENT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIWonderWhereBriggsAnd)},
	{CUE_TALK, ACTOR_WOMAN_GLAD_OF_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIHeardYouMadeIt)},
	{CUE_TALK, ACTOR_MAN_WARY_OF_BRIGGS, CONDITION_ALWAYS, MSGNO(MsgAlhafraEverSinceWeHeardBriggs)},
	{CUE_TALK, ACTOR_MAN_NOT_EXCITED_BY_THE_SHIP, CONDITION_ALWAYS, MSGNO(MsgAlhafraIDontKnowWhyEveryones)},
	{CUE_TALK, ACTOR_MAN_GLAD_TO_SEE_FELIX, CONDITION_ALWAYS, MSGNO(MsgAlhafraOhItsYouWhereHave)},
	{CUE_TALK, ACTOR_WOMAN_ON_ALHAFRAS_FUTURE, CONDITION_ALWAYS, MSGNO(MsgAlhafraWithThatShipAlhafrasGoing)},
	{CUE_TALK, ACTOR_WOMAN_ON_THE_STONE, CONDITION_ALWAYS, MSGNO(MsgAlhafraOnceThatStoneWasTaken)},
	{CUE_TALK, ACTOR_WOMAN_SURE_BRIGGS_IS_GONE, CONDITION_ALWAYS, MSGNO(MsgAlhafraIfBriggsIsSmartHes)},
	{CUE_TALK, ACTOR_OLD_MAN_WAITING_FOR_THIS_DAY, CONDITION_ALWAYS, MSGNO(MsgAlhafraIveBeenWaitingForThis)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_SAILS, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheSecretToHarnessingThe)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_COINCIDENCES, CONDITION_ALWAYS, MSGNO(MsgAlhafraImSureItsJustA)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_SINGING_MAYOR, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorWasActuallySinging)},
	{CUE_TALK, ACTOR_OLD_WOMAN_FEELING_WATCHED, CONDITION_ALWAYS, MSGNO(MsgAlhafraWeirdHaveYouEverHad)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_FISHING_BOATS, CONDITION_ALWAYS, MSGNO(MsgAlhafraHugeBoatsCarryMoreBut)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRATEFUL_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgAlhafraNowThatTheBoatIs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_STANDING_GUARD, CONDITION_ALWAYS, MSGNO(MsgAlhafraImSupposedToBeGuarding)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ADMIRING_THE_MAST, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhoCaresIfTheBoat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WONDERING_WHERE_BRIGGS_WENT, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhyDoIHaveTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_GLAD_OF_THE_SAILBOAT, CONDITION_ALWAYS, MSGNO(MsgAlhafraIHateToTellFelix)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WARY_OF_BRIGGS, CONDITION_ALWAYS, MSGNO(MsgAlhafraIdBetThatBriggsFled)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_NOT_EXCITED_BY_THE_SHIP, CONDITION_ALWAYS, MSGNO(MsgAlhafraBriggsPaidALotFor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_GLAD_TO_SEE_FELIX, CONDITION_ALWAYS, MSGNO(MsgAlhafraFelixMustHaveFoundHimself)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRAS_FUTURE, CONDITION_ALWAYS, MSGNO(MsgAlhafraImCertainTheOnlyPerson)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_STONE, CONDITION_ALWAYS, MSGNO(MsgAlhafraNowThatAllTheDebris)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SURE_BRIGGS_IS_GONE, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorSaidIHad)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WAITING_FOR_THIS_DAY, CONDITION_ALWAYS, MSGNO(MsgAlhafraImProbablyTooOldFor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_SAILS, CONDITION_ALWAYS, MSGNO(MsgAlhafraSmallerShipsUseTheirSails)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_COINCIDENCES, CONDITION_ALWAYS, MSGNO(MsgAlhafraItsBeenSoBusyAround)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_SINGING_MAYOR, CONDITION_ALWAYS, MSGNO(MsgAlhafraTheMayorOnlyHadOne)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_FEELING_WATCHED, CONDITION_ALWAYS, MSGNO(MsgAlhafraWhatIfThePiratesWere)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_FISHING_BOATS, CONDITION_ALWAYS, MSGNO(MsgAlhafraWithoutSmallerShipsHowCan)},
	{CUE_TABLE_END},
};

Sint32 AlhafraElderStepsBackScript[] = {
	SC_ANIMATION(2),
	SC_MOVE_BY(0, 0, -8),
	SC_SYNC,
	SC_MOVE_BY(32, 0, -32),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(1),
	SC_ANIMATION(5),
	SC_END,
};

Uint32 AlhafraWomanWalksOffScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(552, 0, 808),
	SC_SYNC,
	SC_MOVE(536, 0, 760),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_END,
};

Uint32 AlhafraAlhafranLeadsBesideFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(440, 0, 800),
	SC_SYNC,
	SC_MOVE(408, 0, 760),
	SC_SYNC,
	SC_MOVE(408, 0, 760),
	SC_SYNC,
	SC_MOVE(408, 0, 680),
	SC_SYNC,
	SC_MOVE(248, 0, 680),
	SC_SYNC,
	SC_MOVE(248, 0, 632),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Uint32 AlhafraMayorFollowsBesideFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(424, 0, 792),
	SC_SYNC,
	SC_MOVE(408, 0, 760),
	SC_SYNC,
	SC_MOVE(408, 0, 680),
	SC_SYNC,
	SC_MOVE(248, 0, 680),
	SC_SYNC,
	SC_MOVE(248, 0, 664),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Uint32 AlhafraElderFollowsBesideFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(440, 0, 808),
	SC_SYNC,
	SC_MOVE(440, 0, 800),
	SC_SYNC,
	SC_MOVE(408, 0, 760),
	SC_SYNC,
	SC_MOVE(408, 0, 680),
	SC_SYNC,
	SC_MOVE(264, 0, 680),
	SC_SYNC,
	SC_MOVE(264, 0, 664),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Uint32 AlhafraWaitingMadranFollowsBesideFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(440, 0, 808),
	SC_SYNC,
	SC_MOVE(440, 0, 800),
	SC_SYNC,
	SC_MOVE(408, 0, 760),
	SC_SYNC,
	SC_MOVE(408, 0, 680),
	SC_SYNC,
	SC_MOVE(248, 0, 680),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Uint32 AlhafraFirstSoldierFollowsBesideFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(408, 0, 760),
	SC_SYNC,
	SC_MOVE(408, 0, 680),
	SC_SYNC,
	SC_MOVE(264, 0, 680),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Uint32 AlhafraWearyMadranFollowsBesideFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(408, 0, 760),
	SC_SYNC,
	SC_MOVE(408, 0, 680),
	SC_SYNC,
	SC_MOVE(280, 0, 680),
	SC_SYNC,
	SC_MOVE(280, 0, 696),
	SC_SYNC,
	SC_MOVE(248, 0, 696),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Uint32 AlhafraSecondSoldierFollowsBesideFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE_BY(16, 0, 0),
	SC_SYNC,
	SC_MOVE(408, 0, 760),
	SC_SYNC,
	SC_MOVE(408, 0, 760),
	SC_SYNC,
	SC_MOVE(408, 0, 680),
	SC_SYNC,
	SC_MOVE(280, 0, 680),
	SC_SYNC,
	SC_MOVE(280, 0, 696),
	SC_SYNC,
	SC_MOVE(264, 0, 696),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Uint32 AlhafraSilentMadranFollowsBesideFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(408, 0, 760),
	SC_SYNC,
	SC_MOVE(408, 0, 760),
	SC_SYNC,
	SC_MOVE(408, 0, 680),
	SC_SYNC,
	SC_MOVE(280, 0, 680),
	SC_SYNC,
	SC_MOVE(280, 0, 696),
	SC_SYNC,
	SC_MOVE(256, 0, 712),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Sint32 AlhafraAlhafranLeadsBehindFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE_BY(0, 0, -16),
	SC_SYNC,
	SC_MOVE_BY(-16, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(-16, 0, 16),
	SC_SYNC,
	SC_MOVE(248, 0, 808),
	SC_SYNC,
	SC_MOVE(248, 0, 632),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Uint32 AlhafraMayorFollowsBehindFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(328, 0, 808),
	SC_SYNC,
	SC_MOVE(248, 0, 808),
	SC_SYNC,
	SC_MOVE(248, 0, 664),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Uint32 AlhafraElderFollowsBehindFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(328, 0, 808),
	SC_SYNC,
	SC_MOVE(264, 0, 808),
	SC_SYNC,
	SC_MOVE(264, 0, 664),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Uint32 AlhafraWaitingMadranFollowsBehindFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(328, 0, 808),
	SC_SYNC,
	SC_MOVE(248, 0, 808),
	SC_SYNC,
	SC_MOVE(248, 0, 680),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Uint32 AlhafraWearyMadranFollowsBehindFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE_BY(0, 0, 16),
	SC_SYNC,
	SC_MOVE(328, 0, 808),
	SC_SYNC,
	SC_MOVE(248, 0, 808),
	SC_SYNC,
	SC_MOVE(248, 0, 696),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Uint32 AlhafraSecondSoldierFollowsBehindFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE_BY(0, 0, 16),
	SC_SYNC,
	SC_MOVE(328, 0, 808),
	SC_SYNC,
	SC_MOVE(264, 0, 808),
	SC_SYNC,
	SC_MOVE(264, 0, 680),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Uint32 AlhafraFirstSoldierFollowsBehindFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE_BY(0, 0, 32),
	SC_SYNC,
	SC_MOVE(328, 0, 808),
	SC_SYNC,
	SC_MOVE(264, 0, 808),
	SC_SYNC,
	SC_MOVE(264, 0, 696),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Sint32 AlhafraSilentMadranFollowsBehindFelixScript[] = {
	SC_ANIMATION(2),
	SC_MOVE_BY(-20, 0, 32),
	SC_SYNC,
	SC_MOVE(328, 0, 808),
	SC_SYNC,
	SC_MOVE(256, 0, 808),
	SC_SYNC,
	SC_MOVE(256, 0, 712),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};
/* clang-format on */
