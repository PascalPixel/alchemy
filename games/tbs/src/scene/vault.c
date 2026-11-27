/* @file */
/* Vault (SceneId_Vault), "the town at the heart of Angara", as the party */
/* first finds it after Mt. Aleph's eruption. */
/* */
/* This scene is its streets, with the townspeople, Ivan, the bell and the */
/* dog. The houses, the inn and its loft, where the search for Hammet's stolen
 */
/* Shaman's Rod plays out, are log_rooms_a.c. */
/* */
/* scGetActorPlacements and scGetTriggers choose their tables by */
/* FLAG_VAULT_THIEVES_JAILED. Once the party has reached Kalay */
/* (FLAG_KALAY_REACHED), scInit sends it straight on to the town's */
/* later scene (VAULT_TO_VAULT_REVISIT, SceneId_VaultRevisit, */
/* vault_revisit.c). */
/* */
/* The story flags change the streets in order: */
/* - While the rod is missing, the townspeople talk of the eruption, the */
/*   theft and the travelers who left in a hurry. Mind Read on them blames */
/*   the three strangers staying at the inn. One of the three loiters in town */
/*   until the party has confronted them in the loft */
/*   (FLAG_VAULT_THIEVES_ATTACKED). Once the suspects' minds have been read */
/*   (FLAG_VAULT_SUSPECTS_MIND_READ), the young man fixing a roof is missing, */
/*   and a woman walks about looking for him. */
/* - While Ivan is helping the search (FLAG_VAULT_IVAN_JOINED_ROD_SEARCH), */
/*   leaving by either gate has him stay behind (scVaultIvanStaysBehind). */
/*   scInit has him rejoin when the party comes back in by a gate */
/*   or out of the Vault Sanctum. */
/* - Once the thieves are jailed, the townspeople thank the party. The dog, */
/*   given a Bone (scVaultGiveBoneToDog, FLAG_VAULT_DOG_SHOWN_PASSAGE), runs */
/*   to a spot and jumps there three times. The passage it shows opens only */
/*   in the later scene. */
/* */
/* Sap, a Venus Djinni, hides in the tree east of the bell tower. The first */
/* ring since the party came in by a gate (FLAG_VAULT_SAP_CALLED_BY_BELL) calls
 */
/* it out: it leaps onto the cliff top above the cave mouth beyond the tree. */
/* It joins only in the later scene. */
/* */
/* The well in the south-west rumbles with water, and the wooden box beside */
/* the mayor's house in the north-west holds a Nut. */
/* */
/* The town lies on terraces. Its paths leave north, past the bell tower, and */
/* south, under the rope bridge in the south-east. The mayor's house is the */
/* big house in the north-west, with the jail's iron door in the cliff below */
/* it and the Sanctum's double door further south in the same cliff. The inn */
/* stands in the middle, a ladder against its west wall up to the hole a */
/* fiery rock burnt in its roof. East of it are a house, the weapon and armor */
/* shop's door in the cliff under the bell tower, the item shop's door in the */
/* cliff further east, and the house by the rope bridge. Six gravestones */
/* stand south of the inn, where the dog sits. */
/* */
/* The doors open with an animation (Vault_*DoorCells). They lead into the */
/* houses, shops and inn (SceneId_LogRoomsA), the jail (SceneId_StoneRooms) */
/* and the Vault Sanctum (VAULT_TO_SANCTUM). The cell behaviors the events */
/* answer are VAULT_CELL_BEHAVIOR (vault.h). */
/* */
/* The rope bridge in the town's south-east spans the path that runs south */
/* under it. Stepping onto either end of it or off its west end switches the */
/* cells beneath it between the deck and the path (shared/vault_toggle.c). */

#include "scene/vault.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "graphics/graphics.h"
#include "scene/vault_range.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

extern s32 Vault_BoyRoundTheWellRoute[];
extern s32 Vault_GirlRoundTheWellRoute[];
extern SCENE_ENTRANCE Vault_Entrances[];
extern u32 Vault_Exits[];
extern ACTOR_PLACE Vault_ActorPlacements[];
extern ACTOR_PLACE Vault_ActorPlacementsAfterThieves[];
extern TRIGGER Vault_Triggers[];
extern TRIGGER Vault_TriggersAfterThieves[];
extern u16 Vault_GemOwnersHouseDoorCells[];
extern u16 Vault_ChildsHouseDoorCells[];
extern u16 Vault_InnDoorCells[];
extern u16 Vault_ItemShopDoorCells[];
extern u16 Vault_WeaponAndArmorShopDoorCells[];
extern u16 Vault_MayorsHouseDoorCells[];
extern u16 Vault_JailDoorCells[];
extern s32 Vault_DriftScriptA[];
extern s32 Vault_DriftScriptB[];

/* The unit of the unused drift's random speeds: a tenth of a pixel a frame. */
enum { VAULT_DRIFT_RATE = toFIXED(1) / 10 };

/* The townspeople, named by what they say. */
enum {
	/* A young man with brown hair outside the house in the east, by the rope */
	/* bridge. "Welcome to Vault, the town at the heart of Angara." */
	ACTOR_WELCOMING_YOUNG_MAN = 8,
	/* A woman with fair hair who asks about the eruption, east of the six */
	/* gravestones. */
	ACTOR_ERUPTION_WITNESS = 9,
	/* A woman in a green dress who wanders north of the inn. */
	ACTOR_WOMAN_ON_STEALING = 11,
	/* A burly grey-haired man up on the bell tower, beside the bell. */
	ACTOR_VILLAGER_ON_HAMMET = 10,
	/* An old man with a cane, east of the jail's door. */
	ACTOR_OLD_MAN_ON_HAMMETS_SERVANT = 12,
	/* An old woman who wanders east of the inn's door. */
	ACTOR_OLD_WOMAN_ON_THE_TRAVELERS = 13,
	/* The two children who walk round the well, half a circle apart */
	/* (Vault_BoyRoundTheWellRoute, Vault_GirlRoundTheWellRoute). */
	ACTOR_BOY_ROUND_THE_WELL = 14,
	ACTOR_GIRL_ROUND_THE_WELL = 15,
	/* A stocky, muscular man who wanders near the west end of the rope bridge.
	 */
	ACTOR_STRONGMAN_ON_WATCH = 16,
	/* One of the three thieves, drawn with long purple hair as in the loft, */
	/* standing beside the Sanctum's door until the party confronts them */
	/* (FLAG_VAULT_THIEVES_ATTACKED). */
	ACTOR_THIEF_IN_TOWN = 17,
	/* A bearded man in a long robe beside the Sanctum's door. */
	ACTOR_STRANDED_TRAVELER = 18,
	/* The dog that sits among the six gravestones. */
	ACTOR_VAULT_DOG = 19,
	/* A young man at the foot of the ladder against the inn's west wall, who */
	/* is meant to fix the roof and is later found tied up in the inn's loft. */
	ACTOR_YOUNG_MAN_FIXING_THE_ROOF = 20,
	/* A woman with fair hair who paces in front of the inn's west end, looking
	 */
	/* for ACTOR_YOUNG_MAN_FIXING_THE_ROOF. */
	ACTOR_WOMAN_WANTING_THE_ROOF_FIXED = 21,
	/* Sap, a Venus Djinni, hiding in the tree east of the bell tower until the
	 */
	/* bell calls it. */
	ACTOR_SAP = 23
};

static void scVaultShowStandingActorMessage(s32 actorId);
static void scVaultShowMessageAndWait(s32 actorId, s32 delay);
static void scVaultFaceEachOtherAndWait(s32 actorId, s32 other, s32 delay);

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

#include "shared/vault_nearby.c"

/* The woman wanting the roof fixed paces in front of the inn looking for the */
/* man who should be fixing it, and stops when Isaac comes up in front of her,
 */
/* or while he talks to someone or reads a mind. */
static UNREAD_RESULT scVaultUpdateWomanWantingRoofFixed(ACTOR *woman) {
	WIN_STATE *window = (WIN_STATE *)WindowStateWork[0];
	FIELD_STATE *state = FieldStateWork;
	s32 force = 0;
	s32 range = STOP_RANGE;
	ACTOR *leader;

	if(woman->tgtX == ACTOR_NO_TARGET)
		RETURN_UNREAD(0);

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if((s16)state->talk != 0 || window->mindRead != 0) {
		range = TALK_STOP_RANGE;
		force = 1;
	}
	scVaultStopIfFacingNear(woman, leader, range, force);
	RETURN_UNREAD(0);
}

const SCENE_ENTRANCE *scGetEntrances(void) { return Vault_Entrances; }

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

const u32 *scGetExits(void) { return Vault_Exits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0)
		return Vault_ActorPlacementsAfterThieves;
	return Vault_ActorPlacements;
}

/* Counts the Venus Djinn that must leave Ivan before he leaves the party. */
static int scVaultGetIvanVenusDjinnCount(void) {
	return gmUnitGet(IVAN)->djinnHeldCnts[ELEMENT_VENUS];
}

#include "shared/vault_djinn.c"

/* While Ivan is helping look for the rod, leaving Vault by the path north, */
/* past the bell tower (cells (13, 7) and (14, 7)), or the path south, under */
/* the rope bridge (cells (24, 30) and (25, 30)), Ivan steps out of the party */
/* and stays behind. "Are you leaving? I'm still worried about the rod, so */
/* I'll just wait here until you two return." */
/* */
/* Before he joins, or once the thieves are jailed, the party simply leaves. */
/* If Ivan holds Flint, he gives it back to Isaac first. */
static void scVaultIvanStaysBehind(void) {
	ACTOR *actor;
	FIELD_STATE *state;

	state = FieldStateWork;
	if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0 ||
	   gmFlagGet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) == 0) {
		syImportEventSetExitRequest(
			state->stepTrigger -
			(CELL_BEHAVIOR_VAULT_SOUTH_GATE - VAULT_TO_WORLD_MAP_SOUTH));
		return;
	}
	fdEventStart();
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	if(state->stepTrigger == CELL_BEHAVIOR_VAULT_SOUTH_GATE) {
		fdActorWalkToAndWait(ACTOR_IVAN, 400, 448);
	}
	else {
		fdCameraSetSpeed(0.8);
		fdCameraMoveTo(224, CAMERA_KEEP, 162, 1);
		fdActorWalkToAndWait(ACTOR_IVAN, 224, 162);
		fdCameraWaitForArrival();
	}
	fdActorTurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdEventWait(20);
	fdEventSetMessageId(MsgVaultLeavingImStillWorriedAbout);
	/* "Are you leaving? I'm still worried about the rod, so I'll..." */
	fdEventShowMessageThenWait(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	if(scVaultGetIvanVenusDjinnCount() != 0) {
		fdEventSetMessageId(MsgVaultWaitDontWantTakeYour);
		/* "Oh, wait, I don't want to take your Djinni. I'll just leave..." */
		fdEventShowMessage(ACTOR_IVAN, 0);
		scVaultReturnIsaacsDjinni();
		sySynch(20);
	}
	fdPartyRemoveMemberRestored(IVAN);
	syImportEventSetExitRequest(
		state->stepTrigger -
		(CELL_BEHAVIOR_VAULT_SOUTH_GATE - VAULT_TO_WORLD_MAP_SOUTH));
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* Isaac peers into the well in the south-west of Vault (cells (7, 24) to */
/* (8, 25)): "You can hear water rumbling down there." */
static void scVaultPeerIntoWell(void) {
	fdEventStart();
	mnFieldMessageShow(MsgFieldPeeredWell, 1);
	mnFieldMessageShow(MsgVaultCanHearWaterRumblingDown, 1);
	fdEventEnd();
}

const TRIGGER *scGetTriggers(void) {
	if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0)
		return Vault_TriggersAfterThieves;
	return Vault_Triggers;
}

/* Asks the current question and selects its ordinary or alternate reply. */
static inline void scVaultAskYesNoAndShowAnswer(s32 speaker) {
	fdEventOpenMessage(speaker, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
		++FieldStateWork->msg;
	fdEventShowMessage(speaker, 0);
}

/* While the rod is missing, the fair-haired woman east of the six gravestones
 */
/* asks, "Wasn't the eruption of Mt. Aleph incredible?", and answers Isaac's */
/* yes or no. */
static void scVaultAskAboutEruption(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultWasntEruptionMtAlephIncredible);
	scVaultFaceEachOtherAndWait(ACTOR_ERUPTION_WITNESS, ACTOR_PARTY_LEADER, 2);
	fdEventOpenMessage(ACTOR_ERUPTION_WITNESS, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
		FieldStateWork->msg++;
	}
	fdEventShowMessage(ACTOR_ERUPTION_WITNESS, 0);
	fdActorFaceDirection(ACTOR_ERUPTION_WITNESS, FACING_SOUTH_SOUTHWEST, 0);
	fdEventEnd();
}

/* While the rod is missing, the old woman east of the inn's door says some */
/* travelers left in a big hurry, as if on the run, and asks whether the party
 */
/* saw them coming in. */
static void scVaultAskAboutFleeingTravelers(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultThoseTravelersLeftInBig);
	fdActorSetAnimation(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, ANIM_STAND);
	scVaultFaceEachOtherAndWait(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS,
								ACTOR_PARTY_LEADER, 2);
	scVaultAskYesNoAndShowAnswer(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS);
	fdEventEnd();
}

/* The purple-haired thief standing beside the Sanctum's door asks, "Are you */
/* accusing us of stealing Hammet's treasured Shaman's Rod?" */
static void scVaultThiefDeniesStealing(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultAccusingUsStealingHammetsTreasured);
	scVaultFaceEachOtherAndWait(ACTOR_THIEF_IN_TOWN, ACTOR_PARTY_LEADER, 2);
	scVaultAskYesNoAndShowAnswer(ACTOR_THIEF_IN_TOWN);
	fdEventEnd();
}

/* While the rod is missing, the robed traveler beside the Sanctum's door */
/* asks, "Hmm, off on an adventure, are you?" */
static void scVaultAskAboutAdventure(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultOffOnAdventure);
	scVaultFaceEachOtherAndWait(ACTOR_STRANDED_TRAVELER, ACTOR_PARTY_LEADER, 2);
	scVaultAskYesNoAndShowAnswer(ACTOR_STRANDED_TRAVELER);
	fdEventEnd();
}

/* While the rod is missing, the woman in green north of the inn stops and */
/* says some folks got robbed during the chaos of the eruption. */
static void scVaultTalkOfStealingInEruption(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultGuessFolksGot);
	scVaultShowStandingActorMessage(ACTOR_WOMAN_ON_STEALING);
	fdEventEnd();
}

/* While the rod is missing, the strongman by the rope bridge stops and says */
/* nobody steals on his watch, so the thieves must be from out of town. */
static void scVaultStrongmanOnWatchSuspectsOutsiders(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultNobodysStealingAnything);
	scVaultShowStandingActorMessage(ACTOR_STRONGMAN_ON_WATCH);
	fdEventEnd();
}

/* While the rod is missing, the dog among the six gravestones turns to Isaac */
/* and barks. "Ruff! Rrruff!!" */
static void scVaultTalkToDog(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultRuffRrruff);
	fdActorSetAnimation(ACTOR_VAULT_DOG, ANIM_STILL);
	scVaultFaceEachOtherAndWait(ACTOR_VAULT_DOG, ACTOR_PARTY_LEADER, 2);
	fdEventShowMessage(ACTOR_VAULT_DOG, 0);
	fdEventEnd();
}

/* While the rod is missing, the boy walking round the well stops: poor Ivan */
/* was left at the mayor's to look for the stolen rod. */
static void scVaultPityIvan(void) {
	ACTOR *actor = fdActorGet(ACTOR_BOY_ROUND_THE_WELL);
	s16 savedFacing = actor->facing;

	actor->tmr |= VILLAGER_TALKED_TO;
	fdEventStart();
	fdEventSetMessageId(MsgVaultPoorGuyLeft);
	fdActorSetAnimation(ACTOR_BOY_ROUND_THE_WELL, ANIM_STILL);
	scVaultFaceEachOtherAndWait(ACTOR_BOY_ROUND_THE_WELL, ACTOR_PARTY_LEADER,
								2);
	scVaultShowMessageAndWait(ACTOR_BOY_ROUND_THE_WELL, 10);
	actor->facing = savedFacing;
	sySynch(1);
	fdEventEnd();
	actor->tmr &= VILLAGER_IS_BOY;
}

/* While the rod is missing, the girl walking round the well stops: Master */
/* Hammet's caravan left town as soon as the eruption stopped. */
static void scVaultTalkOfHammetsCaravan(void) {
	ACTOR *actor = fdActorGet(ACTOR_GIRL_ROUND_THE_WELL);
	s16 savedFacing = actor->facing;

	actor->tmr |= VILLAGER_TALKED_TO;
	fdEventStart();
	fdEventSetMessageId(MsgVaultMasterHammetsCaravan);
	fdActorSetAnimation(ACTOR_GIRL_ROUND_THE_WELL, ANIM_STILL);
	scVaultFaceEachOtherAndWait(ACTOR_GIRL_ROUND_THE_WELL, ACTOR_PARTY_LEADER,
								2);
	scVaultShowMessageAndWait(ACTOR_GIRL_ROUND_THE_WELL, 10);
	actor->facing = savedFacing;
	sySynch(1);
	fdEventEnd();
	actor->tmr &= VILLAGER_IS_BOY;
}

/* Once the suspects' minds are read, the woman pacing in front of the inn */
/* turns to Isaac, cross, and says the young man should be fixing the roof */
/* but she can't find him anywhere; he's probably slacking off again. */
static void scVaultLookForManFixingRoof(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultFixingRoofCant);
	scVaultFaceEachOtherAndWait(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED,
								ACTOR_PARTY_LEADER, 2);
	fdActorShowEmote(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED,
					 EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdEventWait(30);
	fdEventOpenMessage(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, 0);
	fdEventEnd();
}

/* Stops an actor, faces the leader and shows the current message. */
static void scVaultShowStandingActorMessage(s32 actorId) {
	fdEventStart();
	fdActorSetAnimation(actorId, ANIM_STAND);
	scVaultFaceEachOtherAndWait(actorId, ACTOR_PARTY_LEADER, 2);
	fdEventShowMessage(actorId, 0);
	fdEventEnd();
}

#include "shared/vault_talk.c"

/* Once the thieves are jailed, the young man outside the house in the east */
/* asks, "So, you're hitting the road again?" */
static void scVaultAskAboutLeavingAgain(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultYoureHittingRoadAgain);
	scVaultFaceEachOtherAndWait(ACTOR_WELCOMING_YOUNG_MAN, ACTOR_PARTY_LEADER,
								2);
	scVaultAskYesNoAndShowAnswer(ACTOR_WELCOMING_YOUNG_MAN);
	fdEventEnd();
}

/* Once the thieves are jailed, the woman in green north of the inn says, */
/* "It's great that you caught those thieves!" */
static void scVaultPraiseThievesCapture(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultGreatCaughtThieves);
	scVaultShowStandingActorMessage(ACTOR_WOMAN_ON_STEALING);
	fdEventEnd();
}

/* Once the thieves are jailed, the old man east of the jail's door says that */
/* Hammet's servant, Ivan, went out to Lunpa. With Ivan in the party: "I see, */
/* Ivan... I guess it was impossible to rescue Master Hammet." */
static void scVaultTalkAboutIvan(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultTalkingAboutHammetsServantIvan);
	if(gmFlagGet(FLAG_IN_PARTY(IVAN)) != 0) {
		++FieldStateWork->msg;
	}
	scVaultShowStandingActorMessage(ACTOR_OLD_MAN_ON_HAMMETS_SERVANT);
	fdEventEnd();
}

/* Once the thieves are jailed, the old woman east of the inn's door says the */
/* eruption devastated roads throughout Angara, and the way to Kalay waits */
/* on the southern bridge's repair. */
static void scVaultTalkOfDamagedRoads(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultEruptionDevastatedRoads);
	scVaultShowStandingActorMessage(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS);
	fdEventEnd();
}

/* Once the thieves are jailed, the boy walking round the well asks, "Ivan */
/* didn't go to Lunpa all alone, did he?" With Ivan in the party he laughs */
/* that he shouldn't have worried, and asks when Ivan joined up with Isaac. */
static void scVaultAskWhetherIvanWentToLunpa(void) {
	ACTOR *actor = fdActorGet(ACTOR_BOY_ROUND_THE_WELL);
	s16 savedFacing = actor->facing;

	actor->tmr |= VILLAGER_TALKED_TO;
	fdEventStart();
	fdEventSetMessageId(MsgVaultDidntGoLunpa);
	if(gmFlagGet(FLAG_IN_PARTY(IVAN)) != 0)
		++FieldStateWork->msg;
	fdActorSetAnimation(ACTOR_BOY_ROUND_THE_WELL, ANIM_STILL);
	scVaultFaceEachOtherAndWait(ACTOR_BOY_ROUND_THE_WELL, ACTOR_PARTY_LEADER,
								2);
	scVaultShowMessageAndWait(ACTOR_BOY_ROUND_THE_WELL, 10);
	actor->facing = savedFacing;
	sySynch(1);
	fdEventEnd();
	actor->tmr &= VILLAGER_IS_BOY;
}

/* Once the thieves are jailed, the girl walking round the well stops: everyone
 */
/* is grateful the party captured the thieves. */
static void scVaultThankForCapturingThieves(void) {
	ACTOR *actor = fdActorGet(ACTOR_GIRL_ROUND_THE_WELL);
	s16 savedFacing = actor->facing;

	actor->tmr |= VILLAGER_TALKED_TO;
	fdEventStart();
	fdEventSetMessageId(MsgVaultEveryoneGratefulCaptured);
	fdActorSetAnimation(ACTOR_GIRL_ROUND_THE_WELL, ANIM_STILL);
	scVaultFaceEachOtherAndWait(ACTOR_GIRL_ROUND_THE_WELL, ACTOR_PARTY_LEADER,
								2);
	scVaultShowMessageAndWait(ACTOR_GIRL_ROUND_THE_WELL, 10);
	actor->facing = savedFacing;
	sySynch(1);
	fdEventEnd();
	actor->tmr &= VILLAGER_IS_BOY;
}

/* Once the thieves are jailed, the strongman by the rope bridge asks about the
 */
/* prisoners. "Those men you captured... They're in jail now?" */
static void scVaultAskAboutPrisoners(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultThoseMenCapturedTheyreIn);
	fdActorSetAnimation(ACTOR_STRONGMAN_ON_WATCH, ANIM_STAND);
	scVaultFaceEachOtherAndWait(ACTOR_STRONGMAN_ON_WATCH, ACTOR_PARTY_LEADER,
								2);
	scVaultAskYesNoAndShowAnswer(ACTOR_STRONGMAN_ON_WATCH);
	fdEventEnd();
}

/* Once the thieves are jailed, the robed traveler beside the Sanctum's door */
/* asks, "Are you the ones who captured the thieves?" */
static void scVaultAskWhoCapturedThieves(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultOnesWhoCapturedThieves);
	scVaultFaceEachOtherAndWait(ACTOR_STRANDED_TRAVELER, ACTOR_PARTY_LEADER, 2);
	scVaultAskYesNoAndShowAnswer(ACTOR_STRANDED_TRAVELER);
	fdEventEnd();
}

/* Once the thieves are jailed, the dog south of the six gravestones barks. */
/* "Ruff! Rrruff!!" If Isaac carries a Bone and the dog has not yet shown */
/* its spot, the item menu opens so that he can give it one. */
static void scVaultTalkToDogAfterCapture(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultRuffRrruffAfterCapture);
	fdActorSetAnimation(ACTOR_VAULT_DOG, ANIM_STILL);
	scVaultFaceEachOtherAndWait(ACTOR_VAULT_DOG, ACTOR_PARTY_LEADER, 2);
	fdEventShowMessage(ACTOR_VAULT_DOG, 0);
	fdActorSetAnimation(ACTOR_VAULT_DOG, ANIM_STAND);
	if(gmPartyInventoryFindMember(ITEM_BONE) != -1 &&
	   gmFlagGet(FLAG_VAULT_DOG_SHOWN_PASSAGE) == 0) {
		FieldStateWork->fieldMenu = 1;
	}
	fdEventEnd();
}

/* Once the thieves are jailed, the young man back at the foot of the inn's */
/* ladder nods and thanks the party for the other day, and will pray for */
/* their safety. */
static void scVaultThankBeforeDeparture(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultThankForOtherDayLeaving);
	scVaultFaceEachOtherAndWait(ACTOR_YOUNG_MAN_FIXING_THE_ROOF,
								ACTOR_PARTY_LEADER, 2);
	fdActorSetAnimationAndWait(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, ANIM_NOD);
	fdEventWait(20);
	fdEventShowMessage(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, 0);
	fdEventEnd();
}

/* Mind Read on the woman in green north of the inn: no man should steal */
/* during the panic of an eruption, it's unforgivable. Once the thieves are */
/* jailed she wonders how to punish them: "Public humiliation? Or perhaps a */
/* beating!" */
static void scVaultReadMindOnStealing(void) {
	fdEventStart();
	if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
		fdEventSetMessageId(MsgVaultManShouldStealFromAnother);
	else
		fdEventSetMessageId(MsgVaultHowPunishPrisoners);
	scVaultShowThought(ACTOR_WOMAN_ON_STEALING);
	fdEventEnd();
}

/* Mind Read on the old woman east of the inn's door: that group of travelers */
/* was a strange bunch, seemingly heading for the north-eastern Goma Range. */
/* Once the thieves are jailed she thinks that, with the road out, the only */
/* way on is the cave in the Goma Range. */
static void scVaultReadMindOnTravelers(void) {
	fdEventStart();
	if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0) {
		fdEventSetMessageId(MsgVaultGroupTravelersWasStrangeBunch);
	}
	else {
		fdEventSetMessageId(MsgVaultWithRoadOutOnlyWay);
	}
	scVaultShowThought(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS);
	fdEventEnd();
}

/* Mind Read on the boy walking round the well: "I bet it was those three */
/* creeps at the inn who stole the rod." Once the thieves are jailed he */
/* wonders where Ivan went all by himself, or, with Ivan in the party, "But */
/* when did they join Ivan?" */
static void scVaultReadMindOnIvan(void) {
	fdActorGet(ACTOR_BOY_ROUND_THE_WELL)->tmr |= VILLAGER_TALKED_TO;
	fdEventStart();
	if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0) {
		fdEventSetMessageId(MsgVaultBetWasThoseThreeCreeps);
	}
	else {
		fdEventSetMessageId(MsgVaultWhereDidIvanGoBy);
		if(gmFlagGet(FLAG_IN_PARTY(IVAN)) != 0)
			++FieldStateWork->msg;
	}
	scVaultShowThought(ACTOR_BOY_ROUND_THE_WELL);
	fdEventEnd();
	fdActorGet(ACTOR_BOY_ROUND_THE_WELL)->tmr &= VILLAGER_IS_BOY;
}

/* Mind Read on the girl walking round the well: it doesn't matter how rich */
/* you are if it can't save you from a volcano. Once the thieves are jailed, */
/* the party must be stronger than they look. */
static void scVaultReadMindOnWealth(void) {
	fdActorGet(ACTOR_GIRL_ROUND_THE_WELL)->tmr |= VILLAGER_TALKED_TO;
	fdEventStart();
	if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
		fdEventSetMessageId(MsgVaultSupposeDoesntMatterHowRich);
	else
		fdEventSetMessageId(MsgVaultMustStrongerThanLookHave);
	scVaultShowThought(ACTOR_GIRL_ROUND_THE_WELL);
	fdEventEnd();
	fdActorGet(ACTOR_GIRL_ROUND_THE_WELL)->tmr &= VILLAGER_IS_BOY;
}

/* Mind Read on the strongman by the rope bridge: everyone knows the three at */
/* the inn stole the rod. Once the thieves are jailed he feels much better. */
static void scVaultReadMindOnSuspects(void) {
	fdEventStart();
	if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
		fdEventSetMessageId(MsgVaultEveryoneKnowsThoseThreeAt);
	else
		fdEventSetMessageId(MsgVaultFeelMuchBetter);
	scVaultShowThought(ACTOR_STRONGMAN_ON_WATCH);
	fdEventEnd();
}

/* Mind Read on the dog among the six gravestones. While the rod is missing: */
/* "Stealing in the midst of a volcanic eruption... Humans value money more */
/* than they value their own lives." Once the thieves are jailed: "If you */
/* bring me a bone, I'll tell you something useful." Once it has had one: "A */
/* careful search will reveal a passage leading underground nearby." */
static void scVaultReadDogMind(void) {
	ACTOR *actor = fdActorGet(ACTOR_VAULT_DOG);
	actor->paused = 1;
	fdEventStart();
	if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0) {
		fdEventSetMessageId(MsgVaultStealingInMidstVolcanicEruption);
		fdActorSetAnimation(ACTOR_VAULT_DOG, ANIM_STILL);
		fdEventWait(2);
	}
	else if(gmFlagGet(FLAG_VAULT_DOG_SHOWN_PASSAGE) != 0) {
		fdEventSetMessageId(MsgVaultCarefulSearchWillRevealPassage);
	}
	else {
		fdEventSetMessageId(MsgVaultIfBringMeBoneIll);
	}
	fdEventShowMessage(ACTOR_VAULT_DOG, 0);
	fdEventEnd();
	actor->paused = 0;
}

/* Mind Read on the woman pacing in front of the inn: "That sneak... He's just
 */
/* wasting time instead of fixing the roof!" */
static void scVaultReadWomanWantingTheRoofFixedMind(void) {
	fdEventStart();
	fdEventSetMessageId(MsgVaultSneakWastingTime);
	scVaultShowThought(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED);
	fdEventEnd();
}

#include "shared/vault_toggle.c"

#include "shared/vault_door_exit.c"

/* Isaac opens the door of the house in the east, by the rope bridge (cell */
/* (25, 21)), the gem owner's, and goes in. */
static void scVaultEnterGemOwnersHouse(void) {
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(Vault_GemOwnersHouseDoorCells, 56, 19);
	scVaultWalkThroughExit(408, 320, VAULT_TO_LOG_ROOMS_A_EAST_SOUTHEAST);
}

/* Isaac opens the door of the house east of the inn (cell (19, 20)), a */
/* child's home, and goes in. */
static void scVaultEnterChildsHouse(void) {
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(Vault_ChildsHouseDoorCells, 50, 18);
	scVaultWalkThroughExit(312, 304, VAULT_TO_LOG_ROOMS_A_SOUTHEAST);
}

/* Isaac opens the inn's door, in the middle of town (cell (13, 19)), and */
/* goes in. */
static void scVaultEnterInn(void) {
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(Vault_InnDoorCells, 44, 17);
	scVaultWalkThroughExit(216, 288, VAULT_TO_LOG_ROOMS_A_SOUTHWEST);
}

/* Isaac opens the item shop's door, under the jar sign in the cliff in the */
/* east of town (cell (23, 15)), and goes in. The shop is an old couple's */
/* home too. */
/* */
/* He vanishes into the doorway as he walks in: the leader is drawn behind */
/* the cliff from the door's first step. */
static void scVaultEnterItemShop(void) {
	ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);
	SPR *sprite = actor->spr;

	sySndPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(Vault_ItemShopDoorCells, 54, 13);
	fdMapGridCopyCellAttributes(33, 20, 1, 3, 23, 12);
	actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	sprite->image.pri = 3;
	scVaultWalkThroughExit(376, 224, VAULT_TO_LOG_ROOMS_A_EAST);
}

/* Isaac opens the weapon and armor shop's door, under the sword sign in the */
/* cliff below the bell tower (cell (18, 12)), and goes in, as at the item */
/* shop. */
static void scVaultEnterWeaponAndArmorShop(void) {
	ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);
	SPR *sprite = actor->spr;

	sySndPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(Vault_WeaponAndArmorShopDoorCells, 49, 10);
	fdMapGridCopyCellAttributes(33, 20, 1, 3, 18, 10);
	actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	sprite->image.pri = 3;
	scVaultWalkThroughExit(296, 176, VAULT_TO_LOG_ROOMS_A_NORTH_NORTHEAST);
}

/* Isaac opens the door of the mayor's house, the big house in the town's */
/* north-west (cell (7, 10)), and goes in. Ivan waits there before he joins. */
static void scVaultEnterMayorsHouse(void) {
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(Vault_MayorsHouseDoorCells, 38, 6);
	scVaultWalkThroughExit(120, 144, VAULT_TO_LOG_ROOMS_A_NORTHWEST);
}

#include "shared/vault_map_cells.c"

/* Isaac opens the jail's grey iron door, in the cliff below the mayor's house
 */
/* (cell (4, 11)), and walks into the jail, a stone room of cells behind */
/* bars. */
/* */
/* He vanishes into the doorway as he goes, as at the shops. */
static void scVaultEnterJail(void) {
	ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);
	SPR *sprite = actor->spr;

	sySndPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(Vault_JailDoorCells, 35, 9);
	fdMapGridCopyCellAttributes(33, 20, 1, 3, 4, 10);
	actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	sprite->image.pri = 3;
	scVaultWalkThroughExit(72, 160, VAULT_TO_STONE_ROOMS);
}

/* Isaac climbs the ladder against the inn's west wall to the hole in its */
/* roof (cell (9, 16) at the top) and goes in, into the inn's loft, the room */
/* with the crate (VAULT_TO_LOG_ROOMS_A_WEST). */
/* */
/* The young man who should be fixing the roof stands at the ladder's foot */
/* until the suspects' minds have been read. */
static void scVaultClimbIntoInnLoft(void) {
	sySndPlay(SE_MAP_EXIT);
	scVaultWalkThroughExit(152, 264, VAULT_TO_LOG_ROOMS_A_WEST);
}

/* Moves an actor through a jump and waits up to sixty frames for it to */
/* settle. */
static void scVaultJumpActorToAndWait(s32 actorId, s32 x, s32 z,
									  s32 verticalSpeed) {
	ACTOR *actor = fdActorGet(actorId);
	int frames;
	fdActorSetSpeed(actorId, 3);
	actor->gravity = toFIXED(1) / 2;
	actor->bounce = 0;
	actor->velY = verticalSpeed;
	fdActorSetSprFlags(actor, 0);
	fdActorMoveToAndWait(actorId, x, z);
	fdActorSetPos(actorId, x, z);
	for(frames = 60; frames != 0; --frames) {
		sySynch(1);
		if((FIXEDtoINT(actor->velY)) == 0)
			break;
	}
	fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
	actor->gravity = toFIXED(1);
}

/* Up on the bell tower, Isaac rings the bell (cell (17, 8)). The first time */
/* since the party came in by a path, Sap the Venus Djinni pops up, startled, */
/* on top of the tree east of the tower, hops twice and leaps away north-east */
/* onto the cliff top above the cave mouth. */
static void scVaultRingBell(void) {
	fdEventStart();
	sySndPlay(SE_BELL);
	fdEventWait(40);
	if(fdEventGetFlag(FLAG_VAULT_SAP_CALLED_BY_BELL) == 0) {
		fdActorSetAttachedEffect(ACTOR_SAP, ATTACHED_EFFECT_IN_FRONT |
												ATTACHED_EFFECT_ONCE);
		fdActorJump(ACTOR_SAP, 4, 0);
		fdEventWait(12);
		fdActorJump(ACTOR_SAP, 4, 0);
		fdEventWait(20);
		scVaultJumpActorToAndWait(ACTOR_SAP, 392, 104, toFIXED(7));
		fdEventWait(20);
		fdActorWalkToAndWait(ACTOR_SAP, 408, 104);
		fdActorWalkToAndWait(ACTOR_SAP, 408, 120);
		gmFlagSet(FLAG_VAULT_SAP_CALLED_BY_BELL);
	}
	fdEventEnd();
}

/* Given a Bone, the dog runs into the middle of the six gravestones and jumps
 */
/* there three times, where Reveal later finds the way down to Vault Cave. */
static void scVaultGiveBoneToDog(void) {
	gmPartyInventoryDiscard(ITEM_BONE);
	fdEventStart();
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_VAULT_DOG, 2);
	fdActorSetSpeed(ACTOR_VAULT_DOG, 0.8);
	fdActorWalkToAndWait(ACTOR_VAULT_DOG, 216, 408);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
	fdActorJump(ACTOR_VAULT_DOG, 6, 0);
	fdEventWait(30);
	fdActorJump(ACTOR_VAULT_DOG, 6, 0);
	fdEventWait(30);
	fdActorJump(ACTOR_VAULT_DOG, 6, 0);
	fdEventWait(30);
	fdActorWalkToAndWait(ACTOR_VAULT_DOG, 216, 392);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
	gmFlagSet(FLAG_VAULT_DOG_SHOWN_PASSAGE);
	fdEventEnd();
}

/* Arriving in Vault puts everyone where the story has them. Coming back in by
 */
/* either path, or out of the Sanctum, while Ivan waits in town, he greets the
 */
/* party, "Welcome back, you two! Let's go find the rod!", and joins again. */
s32 scInit(void) {
	s32 suspectsMindRead;
	ACTOR *actor;

	/* Once the party has reached Kalay (FLAG_KALAY_REACHED), the town is its */
	/* later scene, SceneId_VaultRevisit (vault_revisit.c). */
	if(gmFlagGet(FLAG_KALAY_REACHED) != 0) {
		syImportEventSetExitRequest(VAULT_TO_VAULT_REVISIT);
	}
	if(gmFlagGet(FLAG_VAULT_ON_BRIDGE) != 0) {
		fdMapGridCopyCellAttributes(55, 26, 4, 2, 23, 26);
	}
	/* A hidden crate on the well, the one vault_revisit.c places too, where
	 * what */
	/* it is for is asked. */
	fdActorCreateConfigured(128, 0, 420, SPRITE_CRATE);
	actor = fdActorGet(ACTOR_BOY_ROUND_THE_WELL);
	actor->tmr = VILLAGER_IS_BOY;
	actor->update = (void (*)(ACTOR *))scVaultUpdateWalkingVillager;
	actor = fdActorGet(ACTOR_GIRL_ROUND_THE_WELL);
	actor->tmr = 0;
	actor->update = (void (*)(ACTOR *))scVaultUpdateWalkingVillager;
	if(gmFlagGet(FLAG_VAULT_DOG_SHOWN_PASSAGE) != 0) {
		fdActorSetPos(ACTOR_VAULT_DOG, 216, 392);
	}
	suspectsMindRead = gmFlagGet(FLAG_VAULT_SUSPECTS_MIND_READ);
	if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0) {
		/* DEAD: gmFlagGet answers 0 or 1, so the mask changes nothing; the ROM
		 */
		/* keeps it. */
		if((suspectsMindRead & 1) != 0) {
			fdActorGet(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED)->update =
				(void (*)(ACTOR *))scVaultUpdateWomanWantingRoofFixed;
		}
	}
	/* Back through the gates while Ivan waits in Vault: "Welcome back, you two!
	 */
	/* Let's go find the rod!" */
	/* The entrances up to VAULT_NORTH_UNUSED are the gates': the default one */
	/* inside the north gate, the south gate's and the unused one inside the */
	/* north gate. */
	/* XXX: the World Map's north exit leads to VAULT_FROM_WORLD_MAP_NORTH (3),
	 */
	/* which Vault_Entrances lacks, so the party stands at the default row's */
	/* place but keeps entrance 3 and fails this test, and Ivan's welcome back
	 */
	/* would not play there; a recording of that exit into Vault with */
	/* FLAG_VAULT_IVAN_JOINED_ROD_SEARCH set, Ivan out of the party, settles it.
	 */
	if(MainRoutineStateWork.entrance <= VAULT_NORTH_UNUSED) {
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			gmFlagClear(FLAG_VAULT_SAP_CALLED_BY_BELL);
			if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0) {
				if(gmFlagGet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
					fdEventStart();
					actor = fdActorGet(ACTOR_PARTY_LEADER);
					if(actor != NULL) {
						fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed,
										actor->z.fixed);
					}
					if(MainRoutineStateWork.entrance ==
					   VAULT_FROM_WORLD_MAP_SOUTH) {
						fdActorSetPos(ACTOR_IVAN, 400, 448);
					}
					else {
						fdActorSetPos(ACTOR_IVAN, 224, 162);
					}
					fdActorTurnToFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER,
											   0);
					fdEventStartScrTransition();
					fdEventWaitTransitionFrames();
					fdEventWait(30);
					fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
					fdEventSetMessageId(MsgVaultWelcomeBackTwo);
					/* "Welcome back, you two! Let's go find the rod!" */
					fdEventShowMessageThenWait(ACTOR_IVAN, 0, 20);
					fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
					fdActorSetSpeed(ACTOR_IVAN, 0.8);
					scWalkIntoLeader(ACTOR_IVAN);
					fdEventAddPartyMember(IVAN, 0);
					fdEventEnd();
				}
			}
		}
	}
	if(gmFlagGet(FLAG_VAULT_SAP_CALLED_BY_BELL) != 0) {
		fdActorSetPos(ACTOR_SAP, 408, 120);
	}
	/* The same welcome out of the Vault Sanctum while Ivan is out of the party.
	 */
	if(MainRoutineStateWork.entrance == VAULT_FROM_SANCTUM) {
		if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0) {
			if(gmFlagGet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
				if(gmFlagGet(FLAG_IN_PARTY(IVAN)) == 0) {
					fdEventStart();
					fdActorSetPos(ACTOR_IVAN, 40, 310);
					fdActorTurnToFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER,
											   0);
					fdEventStartScrTransition();
					fdEventWaitTransitionFrames();
					fdEventWait(30);
					fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
					fdEventSetMessageId(MsgVaultWelcomeBackTwo);
					/* "Welcome back, you two! Let's go find the rod!" */
					fdEventShowMessageThenWait(ACTOR_IVAN, 0, 20);
					fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
					fdActorSetSpeed(ACTOR_IVAN, 0.8);
					scWalkIntoLeader(ACTOR_IVAN);
					fdEventAddPartyMember(IVAN, 0);
					fdEventEnd();
				}
			}
		}
		gmFlagClear(FLAG_SHOW_AREA_NAME);
	}
	else {
		if(MainRoutineStateWork.entrance == VAULT_FROM_LOG_ROOMS_A_WEST) {
			if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0) {
				fdActorSetPos(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, 0, 0);
			}
		}
	}
	return 0;
}

#include "shared/actor_distance.c"
#include "shared/vault_range.c"

static void scVaultShowMessageAndWait(s32 actorId, s32 delay) {
	fdEventShowMessage(actorId, 0);
	fdEventWait(delay);
}

static void scVaultFaceEachOtherAndWait(s32 actorId, s32 other, s32 delay) {
	fdActorTurnToFaceEachOther(actorId, other, 0);
	fdEventWait(delay);
}

/* A drifting sprite loses this share of each rate a frame: a 28th. */
#define DRIFT_DAMPING 28

/* Applies drift and gradually damps both rates toward zero. */
static void scVaultUpdateDriftingEffect(ACTOR *effect) {
	s32 horizontalRate;
	s32 verticalRate;

	effect->x.fixed += effect->speedLimit;
	effect->tgtX = effect->x.fixed;

	if(effect->tmr != 0) {
		effect->y.fixed += effect->acceleration;
		effect->tgtY = effect->y.fixed;
	}
	else {
		effect->z.fixed += effect->acceleration;
		effect->tgtZ = effect->z.fixed;
		effect->y.fixed += toFIXED(1) / 64;
		effect->tgtY = effect->y.fixed;
	}

	horizontalRate = effect->speedLimit;
	effect->speedLimit =
		horizontalRate - syMathDivide(horizontalRate, DRIFT_DAMPING);
	verticalRate = effect->acceleration;
	effect->acceleration =
		verticalRate - syMathDivide(verticalRate, DRIFT_DAMPING);
}

/* Spawns a drifting sprite near the dog, sharing its drawing priority. */
UNUSED static void scVaultSpawnDriftingEffect(s32 flags) {
	ACTOR *anchor;
	ACTOR *effect;
	SPR *sprite;
	s32 x;
	FIXED y;
	s32 z;

	anchor = fdActorGet(ACTOR_VAULT_DOG);
	if(anchor == NULL)
		return;
	/* Up to four pixels either way of the dog, syRandomBelow(8) - 4 on each
	 * roll. */
	x = syRandomNext();
	z = syRandomNext();
	x <<= 3;
	x = (u32)x >> 16;
	z <<= 3;
	x -= 4;
	z = (u32)z >> 16;
	z -= 4;
	x = INTtoFIXED(x);
	z = INTtoFIXED(z);
	x += anchor->x.fixed;
	y = anchor->y.fixed;
	z += anchor->z.fixed;
	/* XXX (no evidence): what this drift was meant to draw; SPRITE_NO_IMAGE's
	 * row */
	/* in sprites.tsv is empty and nothing calls this function. */
	effect = fdActorCreate(SPRITE_NO_IMAGE, x, y, z);
	if(effect == NULL)
		return;
	sprite = effect->spr;
	/* UB: fdSprCreate makes no sprite for SPRITE_NO_IMAGE, so sprite is NULL */
	/* and the two stores through it below write near address 0. */
#ifdef UBFIX
	if(sprite == NULL)
		return;
#endif
	switch(syRandomNext() & 1) {
	case 1:
		fdActorSelectAnimation(effect, 3);
		fdActorStartScript(effect, Vault_DriftScriptA);
		break;
	default:
		fdActorSelectAnimation(effect, 2);
		fdActorStartScript(effect, Vault_DriftScriptB);
		break;
	}
	effect->motionFlg = 0;
	if(flags & 2) {
		s32 driftMultiplier;
		s32 directionBias;

		driftMultiplier = (u32)syRandomNext() % 10 + 5;
		/* x, done with once the effect is placed, carries the 1. */
		x = 1;
		flags &= x;
		directionBias = flags ^ x;
		directionBias <<= 2;
		driftMultiplier += directionBias;
		effect->acceleration =
			(2 * VAULT_DRIFT_RATE * flags - VAULT_DRIFT_RATE) * driftMultiplier;
		driftMultiplier = (u32)syRandomNext() % 15 - 7;
		effect->speedLimit = VAULT_DRIFT_RATE * driftMultiplier;
		effect->tmr = 0;
	}
	else {
		s32 driftMultiplier;

		driftMultiplier = (u32)syRandomNext() % 10 + 8;
		effect->speedLimit =
			(2 * VAULT_DRIFT_RATE * flags - VAULT_DRIFT_RATE) * driftMultiplier;
		driftMultiplier = (u32)syRandomNext() % 14 + 1;
		effect->acceleration = VAULT_DRIFT_RATE * driftMultiplier;
		effect->tmr = 1;
	}
	effect->update = scVaultUpdateDriftingEffect;
	sprite->flg = 0;
	sprite->image.pri = anchor->spr->image.pri;
}

#include "shared/field_effect_duration_scripts.c"

const s32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

/* ACTOR_WOMAN_WANTING_THE_ROOF_FIXED's search for the young man fixing the */
/* roof, in front of the inn's west end. She walks to one end of her two-cell */
/* path and the other, looks west and east, then shakes her head, for as long */
/* as the party stays on the map. */
s32 Vault_WomanWantingTheRoofFixedRoute[] = {
	SC_SPEED(0.4),
	SC_ACCELERATION(0.2),
	SC_LABEL(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(168, 0, 312),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(10),
	SC_ANIMATION(ANIM_SHAKE_HEAD),
	SC_WAIT(240),
	SC_SET(SW_FACING, FACING_EAST),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(200, 0, 312),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(10),
	SC_ANIMATION(ANIM_SHAKE_HEAD),
	SC_WAIT(240),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
	/* Unused: past the route's end, a solid stroll round her spot that no */
	/* actor is given. */
	SC_SPEED(0.4),
	SC_ACCELERATION(0.2),
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
	SC_SET(SW_RADIUS, toFIXED(40)),
	SC_LABEL(1),
	SC_ANIMATION(ANIM_WALK),
	SC_WANDER_HOME(8, 16, 24),
	SC_SYNC,
	SC_ANIMATION(ANIM_STILL),
	SC_WAIT(40),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

#include "shared/vault_well_routes.c"

SCENE_ENTRANCE Vault_Entrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 223, 0, 135, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{VAULT_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0,
	 -1, -1, -1, -1, 0},
	{VAULT_NORTH_UNUSED, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{VAULT_FROM_LOG_ROOMS_A_EAST_SOUTHEAST, CONDITION_ALWAYS, 408, 0, 344,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{VAULT_FROM_LOG_ROOMS_A_SOUTHEAST, CONDITION_ALWAYS, 312, 0, 328,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{VAULT_FROM_LOG_ROOMS_A_SOUTHWEST, CONDITION_ALWAYS, 216, 0, 312,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{VAULT_FROM_LOG_ROOMS_A_EAST, CONDITION_ALWAYS, 376, 0, 248, FACING_SOUTH,
	 0, -1, -1, -1, -1, 0},
	{VAULT_FROM_LOG_ROOMS_A_NORTH_NORTHEAST, CONDITION_ALWAYS, 296, 0, 200,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{VAULT_FROM_LOG_ROOMS_A_NORTHWEST, CONDITION_ALWAYS, 120, 0, 168,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{VAULT_FROM_SANCTUM, CONDITION_ALWAYS, 64, 0, 294, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{VAULT_FROM_STONE_ROOMS, CONDITION_ALWAYS, 70, 0, 183, FACING_SOUTH, 0, -1,
	 -1, -1, -1, 0},
	{VAULT_FROM_LOG_ROOMS_A_WEST, CONDITION_ALWAYS, 152, 0, 272, FACING_SOUTH,
	 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 Vault_Exits[] = {
	SCENE_EXITS(Vault),
	SCENE_EXIT(VAULT_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_VAULT_SOUTH),
	SCENE_EXIT(VAULT_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_VAULT_NORTH),
	SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_EAST_SOUTHEAST, LogRoomsA,
			   LOG_ROOMS_A_FROM_VAULT_EAST_SOUTHEAST),
	SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_SOUTHEAST, LogRoomsA,
			   LOG_ROOMS_A_FROM_VAULT_SOUTHEAST),
	SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_SOUTHWEST, LogRoomsA,
			   LOG_ROOMS_A_FROM_VAULT_SOUTHWEST),
	SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_EAST, LogRoomsA,
			   LOG_ROOMS_A_FROM_VAULT_EAST),
	SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_NORTH_NORTHEAST, LogRoomsA,
			   LOG_ROOMS_A_FROM_VAULT_NORTH_NORTHEAST),
	SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_NORTHWEST, LogRoomsA,
			   LOG_ROOMS_A_FROM_VAULT_NORTHWEST),
	SCENE_EXIT(VAULT_TO_SANCTUM, Sanctum, SANCTUM_FROM_VAULT),
	SCENE_EXIT(VAULT_TO_STONE_ROOMS, StoneRooms, STONE_ROOMS_FROM_VAULT),
	SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_WEST, LogRoomsA,
			   LOG_ROOMS_A_FROM_VAULT_WEST),
	SCENE_EXIT(VAULT_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_VAULT),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE Vault_ActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        381, 0, 342, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 396, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 150, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        244, 0, 204, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        124, 0, 196, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        244, 0, 300, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)Vault_BoyRoundTheWellRoute,
        132, 0, 380, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)Vault_GirlRoundTheWellRoute,
        132, 0, 420, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        332, 0, 412, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        92, 0, 284, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        92, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 392, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, FLAG_VAULT_SUSPECTS_MIND_READ, MOVEMENT_TYPE_STAND,
        152, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, IF_FLAG_SET(FLAG_VAULT_SUSPECTS_MIND_READ), (s32)Vault_WomanWantingTheRoofFixedRoute,
        168, 0, 312, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    /* XXX (no evidence): what this ivy was for; it stands above the map's top */
    /* edge where no camera shows it, is not solid, and only the field's passes */
    /* over every actor touch it. */
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_SAP), MOVEMENT_TYPE_STAND,
        344, 0, 96, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE Vault_ActorPlacementsAfterThieves[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        381, 0, 342, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 396, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 150, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        244, 0, 204, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        124, 0, 196, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        244, 0, 300, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)Vault_BoyRoundTheWellRoute,
        132, 0, 380, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)Vault_GirlRoundTheWellRoute,
        132, 0, 420, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        332, 0, 412, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        92, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 440, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, FLAG_VAULT_THIEVES_JAILED, (s32)Vault_WomanWantingTheRoofFixedRoute,
        168, 0, 312, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_SAP), MOVEMENT_TYPE_STAND,
        344, 0, 96, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

TRIGGER Vault_Triggers[] = {
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_SOUTH_GATE, CONDITION_ALWAYS, EVENT(scVaultIvanStaysBehind) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_NORTH_GATE, CONDITION_ALWAYS, EVENT(scVaultIvanStaysBehind) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_BRIDGE_END, CONDITION_ALWAYS, EVENT(scVaultRaisePathToBridge) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_WEST_OF_BRIDGE, CONDITION_ALWAYS, EVENT(scVaultLowerPathUnderBridge) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_GEM_OWNERS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(scVaultEnterGemOwnersHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_CHILDS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(scVaultEnterChildsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_INN_DOOR, CONDITION_ALWAYS, EVENT(scVaultEnterInn) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_ITEM_SHOP_DOOR, CONDITION_ALWAYS, EVENT(scVaultEnterItemShop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_WEAPON_AND_ARMOR_SHOP_DOOR, CONDITION_ALWAYS, EVENT(scVaultEnterWeaponAndArmorShop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_MAYORS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(scVaultEnterMayorsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_SANCTUM_DOORWAY, CONDITION_ALWAYS, EVENT(scVaultOpenDoorwayAndEnter) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_DOOR_TO_JAIL, CONDITION_ALWAYS, EVENT(scVaultEnterJail) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_INN_LOFT_LADDER, CONDITION_ALWAYS, EVENT(scVaultClimbIntoInnLoft) },
    { TRIGGER_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultWelcomeToVaultTheTownAt) },
    { TRIGGER_TALK, ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, EVENT(scVaultAskAboutEruption) },
    { TRIGGER_TALK, ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetAWealthyLandownerAnd) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT(scVaultTalkOfStealingInEruption) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetsServantCantReturnTo) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT(scVaultAskAboutFleeingTravelers) },
    { TRIGGER_TALK, ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(scVaultPityIvan) },
    { TRIGGER_TALK, ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(scVaultTalkOfHammetsCaravan) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT(scVaultStrongmanOnWatchSuspectsOutsiders) },
    { TRIGGER_TALK, ACTOR_THIEF_IN_TOWN, CONDITION_ALWAYS, EVENT(scVaultThiefDeniesStealing) },
    { TRIGGER_TALK, ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, EVENT(scVaultAskAboutAdventure) },
    { TRIGGER_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT(scVaultTalkToDog) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultAFieryRockBurntAHole) },
    { TRIGGER_TALK, ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, CONDITION_ALWAYS, EVENT(scVaultLookForManFixingRoof) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultISupposeTheseChildrenCantGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultMtAlephsEruptionWasBeyondDescription) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultWhyDoesAWealthyManLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT(scVaultReadMindOnStealing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultPoorIvanButTheRodMay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT(scVaultReadMindOnTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(scVaultReadMindOnIvan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(scVaultReadMindOnWealth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT(scVaultReadMindOnSuspects) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIEF_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgVaultHmphItLooksLikeTheyreOnto) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgVaultIWasJustAboutToLeave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT(scVaultReadDogMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultFixTheRoofWhyPayFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, CONDITION_ALWAYS, EVENT(scVaultReadWomanWantingTheRoofFixedMind) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VAULT_BELL, CONDITION_ALWAYS, EVENT(scVaultRingBell) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VAULT_WELL, CONDITION_ALWAYS, EVENT(scVaultPeerIntoWell) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), CELL_BEHAVIOR_VAULT_WOODEN_BOX, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { TRIGGER_TABLE_END },
};

TRIGGER Vault_TriggersAfterThieves[] = {
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_SOUTH_GATE, CONDITION_ALWAYS, EVENT(scVaultIvanStaysBehind) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_NORTH_GATE, CONDITION_ALWAYS, EVENT(scVaultIvanStaysBehind) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_BRIDGE_END, CONDITION_ALWAYS, EVENT(scVaultRaisePathToBridge) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_WEST_OF_BRIDGE, CONDITION_ALWAYS, EVENT(scVaultLowerPathUnderBridge) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_GEM_OWNERS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(scVaultEnterGemOwnersHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_CHILDS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(scVaultEnterChildsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_INN_DOOR, CONDITION_ALWAYS, EVENT(scVaultEnterInn) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_ITEM_SHOP_DOOR, CONDITION_ALWAYS, EVENT(scVaultEnterItemShop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_WEAPON_AND_ARMOR_SHOP_DOOR, CONDITION_ALWAYS, EVENT(scVaultEnterWeaponAndArmorShop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_MAYORS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(scVaultEnterMayorsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_SANCTUM_DOORWAY, CONDITION_ALWAYS, EVENT(scVaultOpenDoorwayAndEnter) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_VAULT_DOOR_TO_JAIL, CONDITION_ALWAYS, EVENT(scVaultEnterJail) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VAULT_INN_LOFT_LADDER, CONDITION_ALWAYS, EVENT(scVaultClimbIntoInnLoft) },
    { TRIGGER_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, EVENT(scVaultAskAboutLeavingAgain) },
    { TRIGGER_TALK, ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultSmokeIsStillRisingFromMt) },
    { TRIGGER_TALK, ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetTheWealthyMerchantI) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT(scVaultPraiseThievesCapture) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, EVENT(scVaultTalkAboutIvan) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT(scVaultTalkOfDamagedRoads) },
    { TRIGGER_TALK, ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(scVaultAskWhetherIvanWentToLunpa) },
    { TRIGGER_TALK, ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(scVaultThankForCapturingThieves) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT(scVaultAskAboutPrisoners) },
    { TRIGGER_TALK, ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, EVENT(scVaultAskWhoCapturedThieves) },
    { TRIGGER_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT(scVaultTalkToDogAfterCapture) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, EVENT(scVaultThankBeforeDeparture) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultYouveMadeOurTownPeacefulAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultNowMtAlephHasCalmedDown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultOfAllThePlacesWhyWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT(scVaultReadMindOnStealing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultNobodyGetsIntoLunpaExceptFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT(scVaultReadMindOnTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(scVaultReadMindOnIvan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(scVaultReadMindOnWealth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT(scVaultReadMindOnSuspects) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgVaultThereIsAlwaysAFightWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT(scVaultReadDogMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultSniffAhTheSmellOfCooking) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VAULT_BELL, CONDITION_ALWAYS, EVENT(scVaultRingBell) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VAULT_WELL, CONDITION_ALWAYS, EVENT(scVaultPeerIntoWell) },
    { USE_ITEM_ON_ACTOR(ITEM_BONE), ACTOR_VAULT_DOG, FLAG_VAULT_DOG_SHOWN_PASSAGE, EVENT(scVaultGiveBoneToDog) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), CELL_BEHAVIOR_VAULT_WOODEN_BOX, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { TRIGGER_TABLE_END },
};
/* clang-format on */

#include "shared/vault_door_cells.c"

s32 Vault_DriftScriptA[] = {
	SC_WAIT(1),
	SC_LOOP(118, SC_TOP),
	SC_RELEASE,
};

s32 Vault_DriftScriptB[] = {
	SC_WAIT(1),
	SC_LOOP(38, SC_TOP),
	SC_RELEASE,
};
