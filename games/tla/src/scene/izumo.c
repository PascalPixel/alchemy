/* @file */
/* Izumo, the village of houses on stilts on the far eastern island of the */
/* Eastern Sea. Kushinada is to be offered there to the Serpent of Mt. Mikage */
/* (Gaia Rock). A river winds through it. Susa and Lady Uzume's house stands */
/* in the north, Kushinada's in the north-west, the inn and the shop in the */
/* west and the Sanctum on the raised ground in the east. */
/* */
/* North of the houses a ring of giant stones stands on an island in the */
/* river, guarded by a woman at its bridge, and one stone of the ring is out */
/* of line. Move pushes it east back into place (scIzumoFinishMovingStone), and
 */
/* with it there Reveal shows the way down into the Izumo Ruins in the ring's */
/* centre (scIzumoRevealTheStoneCircle, scIzumoDescendIntoTheRuins). Cyclone */
/* scatters the village's weeds: under them lie two Antidotes in the */
/* south-east corner and a Lucky Medal on the raised ground north of the inn. */
/* */
/* Once the Serpent is slain the village holds a festival, its dancers turning
 */
/* on the spot in the square (Izumo_FirstDanceScript). The first time the */
/* party leaves Susa's house after being hailed there as heroes, Susa runs out
 */
/* after it to thank it and tells it the legend of the Cloud Brand */
/* (scIzumoStopThePartyToThankThem). */

#include "scene/event_layer.h"
#include "field/emote.h"
#include "sounds.h"
#include "field/field_event.h"
#include "field/call.h"
#include "field/flag_call.h"
#include "field/field.h"
#include "game/game.h"
#include "version.h"
#include "field/actor_script.h"

/* Set once the stone circle's guard has seen the stone back in its place. */
#define FLAG_GUARD_SAW_STONE_MOVED 0x924
/* Set while the out-of-line stone stands back in the ring, for this visit. */
#define FLAG_STONE_IN_PLACE FLAG_SCENE_TEMP_50
/* Set while Reveal is showing. */
#define FLAG_REVEALING FLAG_AREA_TEMP_01

/* The village's map cells that answer Cyclone: the weeds over the two */
/* Antidotes and the Lucky Medal, and the rest of each patch. */
enum {
	CELL_SOUTHEAST_WEEDS = 21,
	CELL_WEEDS_OVER_FIRST_ANTIDOTE,
	CELL_WEEDS_OVER_SECOND_ANTIDOTE,
	CELL_WEEDS_BY_THE_SHOP,
	CELL_RIVERBANK_WEEDS,
	CELL_LEDGE_WEEDS,
	CELL_WEEDS_OVER_LUCKY_MEDAL,
	CELL_WEEDS_BY_THE_SANCTUM,
	CELL_STONE_CIRCLE_WEEDS
};

/* The village's actors: the six stones of the ring, the one out of line */
/* among them; the villagers, named by how they look or what they do (the */
/* woman with a ponytail who warns the party off, Okuni the dancer, the woman */
/* who guards the ring, the woman Susa knocked flat, the woman on the porch */
/* west of Kushinada's door); and, once the Serpent is slain, the six who */
/* dance and the old man who drums for them, and Susa. */
enum {
	ACTOR_MISALIGNED_STONE = ACTOR_FIRST_PLACED + 4,
	ACTOR_WOMAN_WITH_PONYTAIL = ACTOR_FIRST_PLACED + 6,
	ACTOR_OKUNI,
	ACTOR_MAN_WITH_TOPKNOT,
	ACTOR_WOMAN_IN_GREEN,
	ACTOR_OLD_MAN,
	ACTOR_OLD_WOMAN,
	ACTOR_BOY,
	ACTOR_GIRL,
	ACTOR_STONE_CIRCLE_GUARD,
	ACTOR_KNOCKED_DOWN_WOMAN,
	ACTOR_PORCH_WOMAN,
	ACTOR_DANCER_A,
	ACTOR_DANCER_B,
	ACTOR_DANCER_C,
	ACTOR_DRUMMER,
	ACTOR_DANCER_E,
	ACTOR_DANCER_F,
	ACTOR_DANCER_G,
	ACTOR_SUSA
};

/* The dancers' and the drummer's animations: the women sway their arms to one
 */
/* side and the other; the old men dance the same on their own sheet, where */
/* animation 5 sits beating a drum. */
enum { ANIM_DANCE = 5, ANIM_OLD_MAN_DRUM = 5, ANIM_OLD_MAN_DANCE = 6 };

/* Steering with ACTOR_FACES_MOTION clear, so that the dancers keep the facings
 */
/* their scripts set; nothing in the field engine's C tests its 0x02 bit. */
#define STEERING_KEEP_FACING 2
/* Set while Susa, his thanks refused, waits in the doorway of his house. */
#define FLAG_SUSA_WAITS FLAG_AREA_TEMP_02
/* Set as Susa stops the party outside his house to thank it. */
#define FLAG_STOPPED_TO_THANK_THE_PARTY 0x929
/* Set once Susa has told the party the legend of the Cloud Brand. */
#define FLAG_TOKEN_OFFERED 0x92a

/* The out-of-line stone's cell, and the cell east of it where it belongs in */
/* the ring. */
#define STONE_MISALIGNED_CELL_X 19
#define STONE_IN_PLACE_CELL_X	20
#define STONE_CELL_Z			9
/* The ring's centre, where Reveal shows the way down: a leader standing there
 */
/* as it opens is set on the cell north of it. */
#define CIRCLE_CENTER_CELL_X 23
#define CIRCLE_CENTER_CELL_Z 9

/* Where the two Antidotes and the Lucky Medal lie under the weeds, in pixels:
 */
/* cells (28, 36), (29, 34) and (10, 20). */
#define FIRST_ANTIDOTE_SPOT_X  456
#define FIRST_ANTIDOTE_SPOT_Z  584
#define SECOND_ANTIDOTE_SPOT_X 472
#define SECOND_ANTIDOTE_SPOT_Z 552
#define LUCKY_MEDAL_SPOT_X	   168
#define LUCKY_MEDAL_SPOT_Z	   328

/* The river's three palettes, from the first, and the colours of each that */
/* shift to make it shimmer. */
#define FIRST_WATER_PALETTE 3
#define WATER_FIRST_COLOR	1
#define WATER_LAST_COLOR	14

extern u32 Izumo_FirstDanceScript[];
extern u32 Izumo_SecondDanceScript[];
extern u32 Izumo_ThirdDanceScript[];
extern u32 Izumo_FourthDanceScript[];
extern u32 Izumo_FifthDanceScript[];
extern u32 Izumo_SixthDanceScript[];
void scIzumoRevealTheStoneCircle(void);

extern s16 Izumo_StonesToMove[];
extern ACTOR_PLACE Izumo_ActorPlacementsOnceTheSerpentIsSlain[];
extern ACTOR_PLACE Izumo_ActorPlacements[];
extern TRIGGER Izumo_TriggersOnceTheSerpentIsSlain[];
extern TRIGGER Izumo_Triggers[];
extern u8 MsgCanYouImagine[];
extern u8 MsgSeeThisRing[];
extern u8 MsgButItS[];
extern u8 MsgSeeThisRingOf[];
extern u8 MsgLadyUzumeAnd[];
extern u8 MsgWouldYouBe[];
extern u8 MsgTheLegendSays[];
extern u8 MsgFelixWait[];
extern u8 MsgThisTokenIsn[];
extern u8 MsgTheMisalignedStone[];

void syFieldEventWait(s32 frames);
/* The ROM's callers here are compiled against this form, all signed, though */
/* the definition takes the destination and the size unsigned. */
void fdMapGridCopyCellTilesIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e,
									   s32 f);
/* Leaves Izumo by one of its exits, as fdEventSetExitRequest does. */
void scIzumoSetExitRequest(s32 value);
void scIzumoRotateWaterCols(void);
void scIzumoStopThePartyToThankThem(void);
void scIzumoTellTheCloudBrandLegend(void);

/* Move (or a push) takes hold of the stone out of line in the ring of giant */
/* stones, on the west side of the ring at cell (19, 9). */
void scIzumoStartMovingStone(void) {
	fdActorLiftTargetBlock(Izumo_StonesToMove);
}

/* Move sets the out-of-line stone down. Pushed east into the gap in the */
/* ring, cell (20, 9), it is back in place: the first time, the woman guarding
 */
/* the ring turns to look, cries "The misaligned stone! It's back in its */
/* original position!" and decides Susa or Kushinada must have moved it. Pushed
 */
/* back west to cell (19, 9), it is out of line again. */
/* */
/* While Reveal is showing, the way down in the ring's centre opens or closes */
/* with the stone. */
void scIzumoFinishMovingStone(void) {
	ACTOR *actor = fdActorGetById(ACTOR_MISALIGNED_STONE);
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);

	fdActorPlaceMovedBlock();
	sySndPlay(SE_WATER);
	if(x == STONE_IN_PLACE_CELL_X && z == STONE_CELL_Z) {
		if(syFlagIsSet(FLAG_REVEALING)) {
			fdMapGridCopyCellTilesIndicesRect(36, 12, 22, 8, 3, 3);
			Call6((void (*)())fdMapCopyCellAttributeRect, 36, 12, 3, 3, 22, 8);
		}
		if(!syFlagIsSet(FLAG_GUARD_SAW_STONE_MOVED)) {
			fdEventStart();
			fdEventEndLastingPsynergy(0);
			fdCameraFollowActor(ACTOR_STONE_CIRCLE_GUARD, 1);
			fdCameraWaitForArrival();
			fdActorShowEmoteById(ACTOR_STONE_CIRCLE_GUARD,
								 EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
			syFieldEventWait(60);
			fdActorFaceDirectionById(ACTOR_STONE_CIRCLE_GUARD, FACING_WEST, 0);
			syFieldEventWait(60);
			fdActorShowEmoteById(ACTOR_STONE_CIRCLE_GUARD,
								 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
			syFieldEventWait(60);
			fdEventSetMessageId(MsgTheMisalignedStone);
			/* "The misaligned stone! It's back in its original position! How
			 * did..." */
			fdEventShowMessageAndWait(ACTOR_STONE_CIRCLE_GUARD, 0);
			syFieldEventWait(30);
			fdActorShowEmoteById(ACTOR_STONE_CIRCLE_GUARD,
								 EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
			syFieldEventWait(30);
			/* "Hmmm... Either Susa or Kushinada must have moved it when I
			 * wasn't..." */
			fdEventShowMessageAndWait(ACTOR_STONE_CIRCLE_GUARD, 0);
			fdActorFaceDirectionById(ACTOR_STONE_CIRCLE_GUARD, FACING_SOUTH, 0);
			fdEventEnd();
			syFlagSet(FLAG_GUARD_SAW_STONE_MOVED);
		}
		syFlagSet(FLAG_STONE_IN_PLACE);
	}
	else if(x == STONE_MISALIGNED_CELL_X && z == STONE_CELL_Z) {
		syFlagClear(FLAG_STONE_IN_PLACE);
		if(syFlagIsSet(FLAG_REVEALING)) {
			fdMapGridCopyCellTilesIndicesRect(36, 8, 22, 8, 3, 3);
			Call6((void (*)())fdMapCopyCellAttributeRect, 36, 8, 3, 3, 22, 8);
		}
	}
}

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(syGameFlagGet(FLAG_SERPENT_SLAIN))
		return Izumo_ActorPlacementsOnceTheSerpentIsSlain;
	return Izumo_ActorPlacements;
}

/* Cyclone cast in the weeds of the village's south-east corner, between the */
/* river and the trees along the east edge (cells (23, 30) to (30, 38)), */
/* scatters those within two cells of Felix. */
void scIzumoScatterSoutheastWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(53, 5, 8, 9, 23, 30, 0);
}

/* Cyclone cast standing on cell (28, 36) in the south-east weeds scatters */
/* them and, until it is found, uncovers an Antidote under Felix's feet: he */
/* jumps a cell west off it, and it lies in the cleared grass. */
void scIzumoScatterWeedsOverFirstAntidote(void) {
	ACTOR *actor = fdActorGetById(MainRoutineStateWork.leaderActorId);

	syFieldTileCopiesCopyTouchedCells(53, 5, 8, 9, 23, 30, 0);
	if(!syGameFlagGet(FLAG_IZUMO_FIRST_ANTIDOTE)) {
		Call3((void (*)())fdActorSetHorizontalPosWithTerrain,
			  FIELD_TREASURE_ACTOR, toFIXED(FIRST_ANTIDOTE_SPOT_X),
			  toFIXED(FIRST_ANTIDOTE_SPOT_Z));
		if(FIELD_CELL(actor->x.fixed) ==
			   FIELD_CELL(toFIXED(FIRST_ANTIDOTE_SPOT_X)) &&
		   FIELD_CELL(actor->z.fixed) ==
			   FIELD_CELL(toFIXED(FIRST_ANTIDOTE_SPOT_Z))) {
			fdEventStart();
			fdEventEndLastingPsynergy(0);
			fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
								ATTACHED_EFFECT_IN_FRONT |
									ATTACHED_EFFECT_ONCE);
			fdActorSetDestinationOffsetById(MainRoutineStateWork.leaderActorId,
											-16, 0);
			actor->velY = toFIXED(6);
			fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdEventEnd();
		}
	}
}

/* Cyclone cast standing on cell (29, 34) in the south-east weeds scatters */
/* them and, until it is found, uncovers the second Antidote under Felix's */
/* feet: he jumps a cell north off it, and it lies in the cleared grass. */
void scIzumoScatterWeedsOverSecondAntidote(void) {
	ACTOR *actor = fdActorGetById(MainRoutineStateWork.leaderActorId);

	syFieldTileCopiesCopyTouchedCells(53, 5, 8, 9, 23, 30, 0);
	if(!syGameFlagGet(FLAG_IZUMO_SECOND_ANTIDOTE)) {
		Call3((void (*)())fdActorSetHorizontalPosWithTerrain,
			  FIELD_TREASURE_ACTOR + 1, toFIXED(SECOND_ANTIDOTE_SPOT_X),
			  toFIXED(SECOND_ANTIDOTE_SPOT_Z));
		if(FIELD_CELL(actor->x.fixed) ==
			   FIELD_CELL(toFIXED(SECOND_ANTIDOTE_SPOT_X)) &&
		   FIELD_CELL(actor->z.fixed) ==
			   FIELD_CELL(toFIXED(SECOND_ANTIDOTE_SPOT_Z))) {
			fdEventStart();
			fdEventEndLastingPsynergy(0);
			fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
								ATTACHED_EFFECT_IN_FRONT |
									ATTACHED_EFFECT_ONCE);
			Call3((void (*)())fdActorSetDestinationOffsetById,
				  MainRoutineStateWork.leaderActorId, 0, -16);
			actor->velY = toFIXED(6);
			fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdEventEnd();
		}
	}
}

/* Cyclone cast in the weeds around the shop south of the inn (on the riverbank
 */
/* west of it at cells (7, 32) and (7, 33), and south and east of its ladder) */
/* scatters those within two cells of Felix. */
void scIzumoScatterWeedsByTheShop(void) {
	syFieldTileCopiesCopyTouchedCells(44, 5, 8, 5, 7, 32, 0);
}

/* Cyclone cast in the weeds on the bank where the river turns west, north-east
 */
/* of the inn (cells (16, 24), (17, 24) and (19, 23)), scatters those within */
/* two cells of Felix. */
void scIzumoScatterRiverbankWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(55, 0, 4, 2, 16, 23, 0);
}

/* Cyclone cast in the weeds along the edge of the raised ground north of the */
/* inn scatters those within two cells of Felix. The ground stands 32 pixels */
/* high, so the cells Felix casts from, (9, 20) to (14, 21), lie two rows south
 */
/* of where the weeds show on screen. */
void scIzumoScatterLedgeWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(42, 0, 6, 2, 9, 18, 0);
}

/* Cyclone cast standing on cell (10, 20), in the weeds on the raised ground */
/* north of the inn, scatters them and, until it is found, uncovers the Lucky */
/* Medal under Felix's feet: he jumps a cell east off it, and it lies in the */
/* cleared grass. */
void scIzumoScatterWeedsOverLuckyMedal(void) {
	ACTOR *actor = fdActorGetById(MainRoutineStateWork.leaderActorId);

	syFieldTileCopiesCopyTouchedCells(42, 0, 6, 2, 9, 18, 0);
	if(!syGameFlagGet(FLAG_IZUMO_LUCKY_MEDAL)) {
		Call3((void (*)())fdActorSetHorizontalPosWithTerrain,
			  FIELD_TREASURE_ACTOR + 2, toFIXED(LUCKY_MEDAL_SPOT_X),
			  toFIXED(LUCKY_MEDAL_SPOT_Z));
		if(FIELD_CELL(actor->x.fixed) ==
			   FIELD_CELL(toFIXED(LUCKY_MEDAL_SPOT_X)) &&
		   FIELD_CELL(actor->z.fixed) ==
			   FIELD_CELL(toFIXED(LUCKY_MEDAL_SPOT_Z))) {
			fdEventStart();
			fdEventEndLastingPsynergy(0);
			fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
								ATTACHED_EFFECT_IN_FRONT |
									ATTACHED_EFFECT_ONCE);
			fdActorSetDestinationOffsetById(MainRoutineStateWork.leaderActorId,
											16, 0);
			actor->velY = toFIXED(6);
			fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdEventEnd();
		}
	}
}

/* Cyclone cast in the weeds by the ladder of the Sanctum, on the raised ground
 */
/* in the east (cells (24, 18) to (28, 21)), scatters those within two cells of
 */
/* Felix. */
void scIzumoScatterWeedsByTheSanctum(void) {
	syFieldTileCopiesCopyTouchedCells(49, 0, 5, 4, 24, 16, 0);
}

/* Cyclone cast in the weeds just west of the ring of giant stones (cells */
/* (16, 11) and (20, 11)) scatters those within two cells of Felix. */
void scIzumoScatterStoneCircleWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(36, 0, 5, 3, 16, 9, 0);
}

/* Felix climbs the ladder of one of the village's houses on stilts to its */
/* door and goes in: the inn, the shop, the Sanctum, Kushinada's house, Susa's
 */
/* house or one of the two houses in the east. The door's cell names the */
/* exit, and so the room. */
void scIzumoEnterDoor(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorSetSpeedParms(MainRoutineStateWork.leaderActorId, toFIXED(1) / 2,
						 toFIXED(1) / 4);
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlg = 0;
	sySndPlay(SE_MAP_EXIT);
	scIzumoSetExitRequest(state->stepTrigger);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

const TRIGGER *scGetTriggers(void) {
	if(syGameFlagGet(FLAG_SERPENT_SLAIN))
		return Izumo_TriggersOnceTheSerpentIsSlain;
	return Izumo_Triggers;
}

/* The woman guarding the ring of giant stones, at the east end of its bridge,
 */
/* asks whether the party sees that one stone is out of line: yes brings */
/* "Susa moved it... He didn't even use his hands.", no "You're...not terribly
 */
/* observant, are you?". Once the stone is back she only marvels at a power */
/* that moves stones without touching them. */
void scIzumoTalkToStoneCircleGuard(s32 actor) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(syFlagIsSet(FLAG_GUARD_SAW_STONE_MOVED)) {
		fdEventSetMessageId(MsgCanYouImagine);
		/* "Can you imagine what it must be like to have the power to move..."
		 */
		fdEventShowMessageAndWait(actor, 0);
	}
	else {
		const u8 *message = MsgSeeThisRing;

		fdEventSetMessageId(message);
		fdMessageShowAtActor(actor, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
			syFieldEventWait(10);
			fdEventSetMessageId(message + 1);
		}
		else {
			syFieldEventWait(20);
			fdEventSetMessageId(message + 2);
		}
		fdEventShowMessageAndWait(actor, 0);
	}
	fdEventEnd();
}

/* The ring's guard during the festival asks the same question about the */
/* out-of-line stone, or once it is back complains "But it's a festival! Why do
 */
/* I have to keep guarding the stone circle?" */
void scIzumoTalkToFestivalStoneCircleGuard(s32 actor) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(syFlagIsSet(FLAG_GUARD_SAW_STONE_MOVED)) {
		fdEventSetMessageId(MsgButItS);
		/* "But it's a festival! Why do I have to keep guarding the stone
		 * circle?" */
		fdEventShowMessageAndWait(actor, 0);
	}
	else {
		const u8 *message = MsgSeeThisRingOf;

		fdEventSetMessageId(message);
		fdMessageShowAtActor(actor, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
			syFieldEventWait(10);
			fdEventSetMessageId(message + 1);
		}
		else {
			syFieldEventWait(20);
			fdEventSetMessageId(message + 2);
		}
		fdEventShowMessageAndWait(actor, 0);
	}
	fdEventEnd();
}

/* An old woman dancing in the square at the festival asks whether it */
/* surprises the party that Lady Uzume and the others aren't blood relatives: */
/* yes brings that their power may make them siblings after all, no that she is
 */
/* glad Lady Uzume used her powers for the good of all. */
void scIzumoTellOfUzumesFamily(s32 actor) {
	const u8 *message;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	message = MsgLadyUzumeAnd;
	fdEventSetMessageId(message);
	fdMessageShowAtActor(actor, 0);
	if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		syFieldEventWait(10);
		fdEventSetMessageId(message + 1);
	}
	else {
		syFieldEventWait(20);
		fdEventSetMessageId(message + 2);
	}
	fdEventShowMessageAndWait(actor, 0);
	fdEventEnd();
}

/* The river shimmers: every fourth frame the shifting colours of each of its */
/* palettes move along by one. */
void scIzumoRotateWaterCols(void) {
	s16 saved;
	s32 index;

	if((FrameCount & 3) == 0) {
		saved = BG_PLTT_COLOR(FIRST_WATER_PALETTE, WATER_LAST_COLOR);
		for(index = WATER_LAST_COLOR; index != WATER_FIRST_COLOR; index--)
			BG_PLTT_COLOR(FIRST_WATER_PALETTE, index) =
				BG_PLTT_COLOR(FIRST_WATER_PALETTE, index - 1);
		BG_PLTT_COLOR(FIRST_WATER_PALETTE, WATER_FIRST_COLOR) = saved;

		saved = BG_PLTT_COLOR(FIRST_WATER_PALETTE + 1, WATER_LAST_COLOR);
		for(index = WATER_LAST_COLOR; index != WATER_FIRST_COLOR; index--)
			BG_PLTT_COLOR(FIRST_WATER_PALETTE + 1, index) =
				BG_PLTT_COLOR(FIRST_WATER_PALETTE + 1, index - 1);
		BG_PLTT_COLOR(FIRST_WATER_PALETTE + 1, WATER_FIRST_COLOR) = saved;

		saved = BG_PLTT_COLOR(FIRST_WATER_PALETTE + 2, WATER_LAST_COLOR);
		for(index = WATER_LAST_COLOR; index != WATER_FIRST_COLOR; index--)
			BG_PLTT_COLOR(FIRST_WATER_PALETTE + 2, index) =
				BG_PLTT_COLOR(FIRST_WATER_PALETTE + 2, index - 1);
		BG_PLTT_COLOR(FIRST_WATER_PALETTE + 2, WATER_FIRST_COLOR) = saved;
	}
}

/* As the party comes down from Susa's house after the heroes' welcome, Susa */
/* comes out of the door after it ("Felix, wait!"): he could not let it leave */
/* without thanks, and asks whether warriors need weapons. Yes, and he tells */
/* the legend of the Cloud Brand. No, and he goes back up to his doorway and */
/* waits there, blocking it. */
void scIzumoStopThePartyToThankThem(void) {
	syGameFlagSet(FLAG_STOPPED_TO_THANK_THE_PARTY);
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId(MsgFelixWait);
	fdActorWalkToById(MainRoutineStateWork.leaderActorId, 200, 264);
	fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	/* "Felix, wait!" */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	syFieldEventWait(15);
	fdActorShowEmoteById(MainRoutineStateWork.leaderActorId,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	Call3((void (*)())fdActorSetHorizontalPosWithTerrain, ACTOR_SUSA,
		  toFIXED(200), toFIXED(216));
	fdActorWalkToById(ACTOR_SUSA, 200, 248);
	fdActorWaitForMoveById(ACTOR_SUSA);
	syFieldEventWait(30);
	/* "It troubled me to let you leave like this..." */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorFaceActorById(MainRoutineStateWork.leaderActorId, ACTOR_SUSA, 0);
	syFieldEventWait(30);
	fdActorSetAnimationById(ACTOR_SUSA, ANIM_SHAKE_HEAD);
	syFieldEventWait(30);
	/* "I owe you much for your help, but I didn't have any way to thank you..."
	 */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorShowEmoteById(MainRoutineStateWork.leaderActorId,
						 EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "And then I thought of something." */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorSetAnimationById(ACTOR_SUSA, ANIM_NOD);
	syFieldEventWait(15);
	/* "You're a warrior, and warriors need weapons. Wouldn't you agree?" */
	fdMessageShowAtActor(ACTOR_SUSA, 0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 1) {
		syFieldEventWait(90);
		Call3((void (*)())fdActorSetSpeedParms, ACTOR_SUSA, toFIXED(2) / 5,
			  toFIXED(1) / 5);
		fdActorWalkToById(ACTOR_SUSA, 200, 216);
		fdActorWaitForMoveById(ACTOR_SUSA);
		fdCameraMoveToActorById(ACTOR_SUSA, 1);
		syFieldEventWait(60);
		Call3((void (*)())fdActorFaceDirectionById, ACTOR_SUSA, FACING_SOUTH,
			  0);
		fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
		/* "Are you sure about that? I was only hoping to show you my
		 * appreciation." */
		fdEventShowMessageAndWait(ACTOR_SUSA, 0);
		fdActorSetSprPriority(ACTOR_SUSA, 3);
		fdActorGetById(ACTOR_SUSA)->motionFlg |= ACTOR_FALLS;
		syGameFlagSet(FLAG_SUSA_WAITS);
	}
	else {
		scIzumoTellTheCloudBrandLegend();
	}
	fdEventEnd();
}

/* Susa tells the legend of the Cloud Brand, hidden where the Serpent's tail */
/* fell, says he left something for the party at the dragon, and goes back into
 */
/* his house to Kushinada. */
void scIzumoTellTheCloudBrandLegend(void) {
	fdEventSetMessageId(MsgThisTokenIsn);
	fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "This token isn't something I possess. It's a legend, one that might..."
	 */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorFaceDirectionById(ACTOR_SUSA, FACING_SOUTHEAST, 0);
	syFieldEventWait(30);
	/* "Long ago, the Great Serpent of Mikage would sleep with its body..." */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorFaceActorById(ACTOR_SUSA, MainRoutineStateWork.leaderActorId, 0);
	fdActorSetAnimationById(ACTOR_SUSA, ANIM_NOD);
	syFieldEventWait(30);
	/* "Where its tail fell, the Cloud Brand lay hidden. That's the legend..."
	 */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorStartRepeatedMotion(ACTOR_SUSA, 1);
	syFieldEventWait(30);
	/* "The serpent was once the sword's protector, but it lost the sword..." */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorSetAnimationById(ACTOR_SUSA, ANIM_SHAKE_HEAD);
	syFieldEventWait(30);
	/* "Anyway, maybe you want to go check out the dragon again... I left..." */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	syFieldEventWait(15);
	fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Well, I've said what I came to say. I'm going back to Kushinada." */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorFaceActorById(ACTOR_SUSA, MainRoutineStateWork.leaderActorId, 0);
	syFieldEventWait(15);
	/* "I hope you are successful in your quest." */
	fdEventShowMessageAndWait(ACTOR_SUSA, 0);
	fdActorSetAnimationById(ACTOR_SUSA, ANIM_NOD);
	syFieldEventWait(15);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	syFieldEventWait(30);
	fdActorSetSpeedParms(ACTOR_SUSA, toFIXED(1) / 5, toFIXED(1) / 10);
	fdActorGetById(ACTOR_SUSA)->motionFlg = 0;
	fdActorSetSprPriority(ACTOR_SUSA, 3);
	fdActorWalkToById(ACTOR_SUSA, 200, 208);
	fdActorWaitForMoveById(ACTOR_SUSA);
	fdActorSetHorizontalPosWithTerrain(ACTOR_SUSA, 0, 0);
	syGameFlagSet(FLAG_TOKEN_OFFERED);
	syGameFlagClear(FLAG_SUSA_WAITS);
}

/* Talking to Susa in his doorway, or reading his mind: he offers his token of
 */
/* gratitude again ("Would you be willing to accept a token of my */
/* gratitude?"), and yes brings the legend of the Cloud Brand. Once it is told
 */
/* he says the legend calls it a powerful weapon for fighting dragons. */
void scIzumoOfferATokenOfGratitude(void) {
	if(!syGameFlagGet(FLAG_TOKEN_OFFERED)) {
		const u8 *message;

		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdActorShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
							 60);
		message = MsgWouldYouBe;
		fdEventSetMessageId(message);
		fdMessageShowAtActor(ACTOR_SUSA, 0);
		if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
											 0) == 0) {
			scIzumoTellTheCloudBrandLegend();
		}
		else {
			fdEventSetMessageId(message - 1);
			fdEventShowMessageAndWait(ACTOR_SUSA, 0);
		}
		fdEventEnd();
	}
	else {
		/* XXX: the message is passed where the speaker goes; recording a talk
		 * with */
		/* Susa once FLAG_TOKEN_OFFERED is set would show what the player sees.
		 */
		fdEventShowMessageAndWait((s32)MsgTheLegendSays, 0);
	}
}

/* Sets up Izumo as the party enters: the fade in, the river's shimmer, the */
/* ring's stones, and once the Serpent is slain the dancers, Susa's thanks the
 */
/* first time the party leaves his house as heroes, and Susa waiting in his */
/* doorway while his thanks are refused. */
s32 scInit(void) {
	syGameFlagClear(FLAG_SUSA_WAITS);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	sySetEvent(scIzumoRotateWaterCols, TASK_PASS_VBLANK);
	fdActorPlaceBlocks(Izumo_StonesToMove);
	if(syGameFlagGet(FLAG_SERPENT_SLAIN)) {
		fdActorGetById(ACTOR_OKUNI)->steeringFlg = STEERING_KEEP_FACING;
		fdActorSetAnimationById(ACTOR_DANCER_A, ANIM_DANCE);
		fdActorSetAnimationById(ACTOR_DANCER_B, ANIM_DANCE);
		fdActorSetAnimationById(ACTOR_DANCER_C, ANIM_DANCE);
		fdActorSetAnimationById(ACTOR_DRUMMER, ANIM_OLD_MAN_DRUM);
		fdActorSetAnimationById(ACTOR_DANCER_E, ANIM_OLD_MAN_DANCE);
		fdActorSetAnimationById(ACTOR_DANCER_F, ANIM_OLD_MAN_DANCE);
		fdActorSetAnimationById(ACTOR_DANCER_G, ANIM_DANCE);
		if(MainRoutineStateWork.entrance == IZUMO_FROM_IZUMO_ROOMS_NORTH &&
		   syGameFlagGet(FLAG_IZUMO_RECOGNIZED_THE_HEROES) &&
		   !syGameFlagGet(FLAG_STOPPED_TO_THANK_THE_PARTY) &&
		   !syGameFlagGet(FLAG_TOKEN_OFFERED)) {
			fdEventStartScrTransition();
			scIzumoStopThePartyToThankThem();
		}
		if(syGameFlagGet(FLAG_STOPPED_TO_THANK_THE_PARTY) &&
		   !syGameFlagGet(FLAG_TOKEN_OFFERED)) {
			Call3((void (*)())fdActorSetHorizontalPosWithTerrain, ACTOR_SUSA,
				  toFIXED(200), toFIXED(216));
			fdActorSetSprPriority(ACTOR_SUSA, 3);
			fdActorGetById(ACTOR_SUSA)->motionFlg |= ACTOR_FALLS;
			syGameFlagSet(FLAG_SUSA_WAITS);
		}
	}
	fdActorSetSprPriority(ACTOR_PORCH_WOMAN, 3);
	fdActorGetById(ACTOR_PORCH_WOMAN)->motionFlg |= ACTOR_FALLS;
	return 0;
}

/* Nothing to prepare before the map is built. Returns 0. */
s32 scPrepareMap(void) { return 0; }

/* Reveal shows a way down in the centre of the ring of giant stones, cell */
/* (23, 9). It can be climbed down only while the out-of-line stone is back in
 */
/* place; a leader standing on the centre then is set on the cell north of it.
 */
void scIzumoRevealTheStoneCircle(void) {
	ACTOR *actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);

	syGameFlagSet(FLAG_REVEALING);
	if(syGameFlagGet(FLAG_STONE_IN_PLACE)) {
		fdMapGridCopyCellTilesIndicesRect(36, 12, 22, 8, 3, 3);
		Call6((void (*)())fdMapCopyCellAttributeRect, 36, 12, 3, 3, 22, 8);
		if(x == CIRCLE_CENTER_CELL_X && z == CIRCLE_CENTER_CELL_Z)
			fdActorSetHorizontalPosWithTerrain(
				MainRoutineStateWork.leaderActorId, toFIXED(376), toFIXED(136));
	}
	else {
		fdMapGridCopyCellTilesIndicesRect(36, 8, 22, 8, 3, 3);
		Call6((void (*)())fdMapCopyCellAttributeRect, 36, 8, 3, 3, 22, 8);
	}
}

/* As Reveal fades the way down in the ring's centre is hidden again. */
void scIzumoHideTheStoneCircle(void) {
	syGameFlagClear(FLAG_REVEALING);
	fdMapGridCopyCellTilesIndicesRect(40, 8, 22, 8, 3, 3);
	Call6((void (*)())fdMapCopyCellAttributeRect, 40, 8, 3, 3, 22, 8);
}

/* Felix, facing north, backs into the opening in the ring's centre and climbs
 */
/* down it as down a ladder, and the scene leaves for the Izumo Ruins. */
void scIzumoDescendIntoTheRuins(void) {
	s32 value = FieldStateWork->stepTrigger;

	fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldEventWait(10);
	fdActorGetById(ACTOR_FELIX)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetDestinationOffsetById(ACTOR_FELIX, 0, 4);
	fdActorWaitForMoveById(ACTOR_FELIX);
	fdActorSetSprFlags(fdActorGetById(ACTOR_FELIX), 0);
#if TLA_PATCH_1 /* From patch 1. */
	sySndPlay(SE_STAIRS_DOWN);
#endif
	fdActorSetDestinationOffsetById(ACTOR_FELIX, 0, 16);
	fdActorWaitForMoveById(ACTOR_FELIX);
	fdActorSetAnimationById(ACTOR_FELIX, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	syFieldEventWait(14);
	scIzumoSetExitRequest(value);
}

/* Felix walks into the opening in the ring from its west side, cell (22, 9): */
/* he steps round to its north side and climbs down into the ruins. */
void scIzumoEnterTheRuinsFromTheLeft(void) {
	Call3((void (*)())fdActorWalkByAndWaitById, ACTOR_FELIX, 0, -14);
	fdActorWalkByAndWaitById(ACTOR_FELIX, 16, 0);
	syFieldEventWait(10);
	scIzumoDescendIntoTheRuins();
}

/* Felix walks into the opening in the ring from its east side, cell (24, 9): */
/* he steps round to its north side and climbs down into the ruins. */
void scIzumoEnterTheRuinsFromTheRight(void) {
	Call3((void (*)())fdActorWalkByAndWaitById, ACTOR_FELIX, 0, -14);
	fdActorWalkByAndWaitById(ACTOR_FELIX, -16, 0);
	syFieldEventWait(10);
	scIzumoDescendIntoTheRuins();
}

/* Felix walks south into the opening in the ring from its north side, cell */
/* (23, 8), turns round and climbs down into the ruins. */
void scIzumoEnterTheRuins(void) {
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldEventWait(10);
	scIzumoDescendIntoTheRuins();
}

#include "text/msg_ids.h"

/* The stones Move can push, ended by -1: the one out of line in the ring. */
s16 Izumo_StonesToMove[] = {
	ACTOR_MISALIGNED_STONE,
	-1,
};

/* The festival dancers' scripts: each turns once round on the spot, a quarter
 */
/* turn every 5 frames, then stands a second and a half facing as it began, */
/* over and over. They differ only in the way each starts out facing. */
/* clang-format off */
u32 Izumo_FirstDanceScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SPEED(0.6),
    SC_SET(SW_STEERING, STEERING_KEEP_FACING),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_LABEL(1),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(60),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

u32 Izumo_SecondDanceScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SPEED(0.6),
    SC_SET(SW_STEERING, STEERING_KEEP_FACING),
    SC_SET(SW_FACING, FACING_EAST),
    SC_LABEL(1),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(60),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

u32 Izumo_ThirdDanceScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SPEED(0.6),
    SC_SET(SW_STEERING, STEERING_KEEP_FACING),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_LABEL(1),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(60),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

/* The same script as Izumo_FirstDanceScript, under another name. */
u32 Izumo_FourthDanceScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SPEED(0.6),
    SC_SET(SW_STEERING, STEERING_KEEP_FACING),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_LABEL(1),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(60),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

/* The same script as Izumo_SecondDanceScript, under another name. */
u32 Izumo_FifthDanceScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SPEED(0.6),
    SC_SET(SW_STEERING, STEERING_KEEP_FACING),
    SC_SET(SW_FACING, FACING_EAST),
    SC_LABEL(1),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(60),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

u32 Izumo_SixthDanceScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.3),
    SC_SPEED(0.6),
    SC_SET(SW_STEERING, STEERING_KEEP_FACING),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_LABEL(1),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(60),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
    SCENE_EXITS(Izumo),
    SCENE_EXIT_UNLESS(IZUMO_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_IZUMO,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_SOUTHEAST_A, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_SOUTHEAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_SOUTHEAST_B, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_SOUTHEAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_SOUTHWEST, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_SOUTH, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_NORTHWEST, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_NORTHEAST, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_ROOMS_NORTH, IzumoRooms,
                      IZUMO_ROOMS_FROM_IZUMO_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(IZUMO_TO_IZUMO_RUINS_FIRST_ROOM, IzumoRuinsFirstRoom,
                      IZUMO_RUINS_FIRST_ROOM_FROM_IZUMO, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

ACTOR_PLACE Izumo_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 120, 0, 488, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 488, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 568, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 248, 0, 464, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 344, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 424, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 488, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 360, 0, 392, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 117, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 155, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 155, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
/* clang-format on */

/* clang-format off */
ACTOR_PLACE Izumo_ActorPlacementsOnceTheSerpentIsSlain[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 120, 0, 488, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 568, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 472, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 344, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 424, 0, 568, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 488, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 360, 0, 392, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 117, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 155, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 155, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (s32)Izumo_FirstDanceScript, 256, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (s32)Izumo_SecondDanceScript, 264, 0, 504, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (s32)Izumo_ThirdDanceScript, 360, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 472, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, (s32)Izumo_FourthDanceScript, 232, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, (s32)Izumo_FifthDanceScript, 328, 0, 504, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_WOMAN, CONDITION_ALWAYS, (s32)Izumo_SixthDanceScript, 336, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUSA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
/* clang-format on */

TRIGGER Izumo_Triggers[] = {
	{TRIGGER_EXIT, IZUMO_TO_WORLD_MAP, CONDITION_ALWAYS, IZUMO_TO_WORLD_MAP},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTHEAST_A,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTHEAST_B,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTHWEST,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTH,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_NORTHWEST,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_NORTHEAST,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_NORTH,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), IZUMO_TO_IZUMO_RUINS_FIRST_ROOM,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterTheRuins)},
	{STEP_TRIGGER_WALKING_INTO(FACING_WEST), IZUMO_TO_IZUMO_RUINS_FIRST_ROOM,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterTheRuinsFromTheRight)},
	{STEP_TRIGGER_WALKING_INTO(FACING_EAST), IZUMO_TO_IZUMO_RUINS_FIRST_ROOM,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterTheRuinsFromTheLeft)},
	{TRIGGER_TALK, ACTOR_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS,
	 MSGNO(MsgWeVeHad)},
	{TRIGGER_TALK, ACTOR_OKUNI, CONDITION_ALWAYS, MSGNO(MsgMyNameS)},
	{TRIGGER_TALK, ACTOR_MAN_WITH_TOPKNOT, CONDITION_ALWAYS,
	 MSGNO(MsgLadyUzumeDoesn)},
	{TRIGGER_TALK, ACTOR_WOMAN_IN_GREEN, CONDITION_ALWAYS,
	 MSGNO(MsgSusaSGoing)},
	{TRIGGER_TALK, ACTOR_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgAsAChild)},
	{TRIGGER_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS,
	 MSGNO(MsgChoosingTheNext)},
	{TRIGGER_TALK, ACTOR_BOY, CONDITION_ALWAYS, MSGNO(MsgTheDaughtersOf)},
	{TRIGGER_TALK, ACTOR_GIRL, CONDITION_ALWAYS, MSGNO(MsgABraveWarrior)},
	{TRIGGER_TALK, ACTOR_STONE_CIRCLE_GUARD, CONDITION_ALWAYS,
	 EVENT(scIzumoTalkToStoneCircleGuard)},
	{TRIGGER_TALK, ACTOR_KNOCKED_DOWN_WOMAN, CONDITION_ALWAYS,
	 MSGNO(MsgYeahSusaCame)},
	{TRIGGER_TALK, ACTOR_PORCH_WOMAN, CONDITION_ALWAYS, MSGNO(MsgEvenIfSusa)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WITH_PONYTAIL,
	 CONDITION_ALWAYS, MSGNO(MsgTheGreatSerpent)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OKUNI, CONDITION_ALWAYS,
	 MSGNO(MsgIMNotGoing)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WITH_TOPKNOT,
	 CONDITION_ALWAYS, MSGNO(MsgIMSureThat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_GREEN,
	 CONDITION_ALWAYS, MSGNO(MsgWhatIfLosing)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, CONDITION_ALWAYS,
	 MSGNO(MsgLadyUzumeLoves)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS,
	 MSGNO(MsgSacrificesAsGreat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY, CONDITION_ALWAYS,
	 MSGNO(MsgKushinadaSAlways)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL, CONDITION_ALWAYS,
	 MSGNO(MsgWhyWonT)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STONE_CIRCLE_GUARD,
	 CONDITION_ALWAYS, MSGNO(MsgWhyDoesLady)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KNOCKED_DOWN_WOMAN,
	 CONDITION_ALWAYS, MSGNO(MsgYouKnowI)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PORCH_WOMAN, CONDITION_ALWAYS,
	 MSGNO(MsgLadyUzumeSays)},
	{TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scIzumoStartMovingStone)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scIzumoFinishMovingStone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_MISALIGNED_STONE, CONDITION_ALWAYS, EVENT(scIzumoStartMovingStone)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MISALIGNED_STONE, CONDITION_ALWAYS,
	 EVENT(scIzumoFinishMovingStone)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_SOUTHEAST_WEEDS, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterSoutheastWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_WEEDS_OVER_FIRST_ANTIDOTE, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterWeedsOverFirstAntidote)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_WEEDS_OVER_SECOND_ANTIDOTE, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterWeedsOverSecondAntidote)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_WEEDS_BY_THE_SHOP, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterWeedsByTheShop)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_RIVERBANK_WEEDS, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterRiverbankWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_LEDGE_WEEDS, CONDITION_ALWAYS, EVENT(scIzumoScatterLedgeWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_WEEDS_OVER_LUCKY_MEDAL, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterWeedsOverLuckyMedal)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_WEEDS_BY_THE_SANCTUM, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterWeedsByTheSanctum)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_STONE_CIRCLE_WEEDS, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterStoneCircleWeeds)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
	 CONDITION_ALWAYS, EVENT(scIzumoRevealTheStoneCircle)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
	 CONDITION_ALWAYS, EVENT(scIzumoHideTheStoneCircle)},
	{TRIGGER_TABLE_END},
};

TRIGGER Izumo_TriggersOnceTheSerpentIsSlain[] = {
	{TRIGGER_EXIT, IZUMO_TO_WORLD_MAP, CONDITION_ALWAYS, IZUMO_TO_WORLD_MAP},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTHEAST_A,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTHEAST_B,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTHWEST,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_SOUTH,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_NORTHWEST,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_NORTHEAST,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), IZUMO_TO_IZUMO_ROOMS_NORTH,
	 FLAG_SUSA_WAITS, EVENT(scIzumoEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), IZUMO_TO_IZUMO_RUINS_FIRST_ROOM,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterTheRuins)},
	{STEP_TRIGGER_WALKING_INTO(FACING_WEST), IZUMO_TO_IZUMO_RUINS_FIRST_ROOM,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterTheRuinsFromTheRight)},
	{STEP_TRIGGER_WALKING_INTO(FACING_EAST), IZUMO_TO_IZUMO_RUINS_FIRST_ROOM,
	 CONDITION_ALWAYS, EVENT(scIzumoEnterTheRuinsFromTheLeft)},
	{TRIGGER_TALK, ACTOR_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS,
	 MSGNO(MsgThoseScreamsFrom)},
	{TRIGGER_TALK, ACTOR_OKUNI, CONDITION_ALWAYS, MSGNO(MsgWeRePlanning)},
	{TRIGGER_TALK, ACTOR_MAN_WITH_TOPKNOT, CONDITION_ALWAYS,
	 MSGNO(MsgSusaHasChanged)},
	{TRIGGER_TALK, ACTOR_WOMAN_IN_GREEN, CONDITION_ALWAYS,
	 MSGNO(MsgKushinadaHasBeen)},
	{TRIGGER_TALK, ACTOR_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgIVeHeardPeople)},
	{TRIGGER_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSusaHasGrown)},
	{TRIGGER_TALK, ACTOR_BOY, CONDITION_ALWAYS, MSGNO(MsgSusaHasPut)},
	{TRIGGER_TALK, ACTOR_GIRL, CONDITION_ALWAYS, MSGNO(MsgLadyUzumePredicted)},
	{TRIGGER_TALK, ACTOR_STONE_CIRCLE_GUARD, CONDITION_ALWAYS,
	 EVENT(scIzumoTalkToFestivalStoneCircleGuard)},
	{TRIGGER_TALK, ACTOR_KNOCKED_DOWN_WOMAN, CONDITION_ALWAYS,
	 MSGNO(MsgIfYouReLooking)},
	{TRIGGER_TALK, ACTOR_PORCH_WOMAN, CONDITION_ALWAYS, MSGNO(MsgDidYouHear)},
	{TRIGGER_TALK, ACTOR_DANCER_A, CONDITION_ALWAYS, MSGNO(MsgIWonderIf)},
	{TRIGGER_TALK, ACTOR_DANCER_B, CONDITION_ALWAYS, MSGNO(MsgDancingLikeThis)},
	{TRIGGER_TALK, ACTOR_DANCER_C, CONDITION_ALWAYS,
	 MSGNO(MsgEveryoneSaysThat)},
	{TRIGGER_TALK, ACTOR_DRUMMER, CONDITION_ALWAYS, MSGNO(MsgWeMayNever)},
	{TRIGGER_TALK, ACTOR_DANCER_E, CONDITION_ALWAYS, MSGNO(MsgOurIslandIs)},
	{TRIGGER_TALK, ACTOR_DANCER_F, CONDITION_ALWAYS,
	 MSGNO(MsgTheSerpentTaught)},
	{TRIGGER_TALK, ACTOR_DANCER_G, CONDITION_ALWAYS,
	 EVENT(scIzumoTellOfUzumesFamily)},
	{TRIGGER_TALK, ACTOR_SUSA, CONDITION_ALWAYS,
	 EVENT(scIzumoOfferATokenOfGratitude)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WITH_PONYTAIL,
	 CONDITION_ALWAYS, MSGNO(MsgIHadAlways)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OKUNI, CONDITION_ALWAYS,
	 MSGNO(MsgThisCelebrationWill)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WITH_TOPKNOT,
	 CONDITION_ALWAYS, MSGNO(MsgItSOdd)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_GREEN,
	 CONDITION_ALWAYS, MSGNO(MsgKushinadaIsSuch)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, CONDITION_ALWAYS,
	 MSGNO(MsgWhatWouldMake)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS,
	 MSGNO(MsgAllTheGirls)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY, CONDITION_ALWAYS,
	 MSGNO(MsgEveryoneSBeen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL, CONDITION_ALWAYS,
	 MSGNO(MsgSoSusaReally)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STONE_CIRCLE_GUARD,
	 CONDITION_ALWAYS, MSGNO(MsgSusaDidA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KNOCKED_DOWN_WOMAN,
	 CONDITION_ALWAYS, MSGNO(MsgLadyUzumeAndKushinada)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PORCH_WOMAN, CONDITION_ALWAYS,
	 MSGNO(MsgKushinadaMustBe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DANCER_A, CONDITION_ALWAYS,
	 MSGNO(MsgAllAnyoneTalks)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DANCER_B, CONDITION_ALWAYS,
	 MSGNO(MsgADragonA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DANCER_C, CONDITION_ALWAYS,
	 MSGNO(MsgSusaSaidThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DRUMMER, CONDITION_ALWAYS,
	 MSGNO(MsgWeNeedTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DANCER_E, CONDITION_ALWAYS,
	 MSGNO(MsgDancingIsGood)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DANCER_F, CONDITION_ALWAYS,
	 MSGNO(MsgThatSusaUsed)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DANCER_G, CONDITION_ALWAYS,
	 MSGNO(MsgAsLongAsWe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_SUSA | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
	 EVENT(scIzumoOfferATokenOfGratitude)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_MISALIGNED_STONE, CONDITION_ALWAYS, EVENT(scIzumoStartMovingStone)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MISALIGNED_STONE, CONDITION_ALWAYS,
	 EVENT(scIzumoFinishMovingStone)},
	{TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scIzumoStartMovingStone)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scIzumoFinishMovingStone)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
	 CONDITION_ALWAYS, EVENT(scIzumoRevealTheStoneCircle)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
	 CONDITION_ALWAYS, EVENT(scIzumoHideTheStoneCircle)},
#if TLA_PATCH_1 /* From patch 1. */
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_SOUTHEAST_WEEDS, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterSoutheastWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_WEEDS_OVER_FIRST_ANTIDOTE, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterWeedsOverFirstAntidote)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_WEEDS_OVER_SECOND_ANTIDOTE, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterWeedsOverSecondAntidote)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_WEEDS_BY_THE_SHOP, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterWeedsByTheShop)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_RIVERBANK_WEEDS, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterRiverbankWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_LEDGE_WEEDS, CONDITION_ALWAYS, EVENT(scIzumoScatterLedgeWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_WEEDS_OVER_LUCKY_MEDAL, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterWeedsOverLuckyMedal)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_WEEDS_BY_THE_SANCTUM, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterWeedsByTheSanctum)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_STONE_CIRCLE_WEEDS, CONDITION_ALWAYS,
	 EVENT(scIzumoScatterStoneCircleWeeds)},
#endif
	{TRIGGER_TABLE_END},
};

/* Unused: a halfword nothing reads. */
u16 Izumo_Unused[] = {
	1,
};
