/* @file */
/* The caves in Champa's cliff, where the sailors and their families live, in */
/* four scenes of this overlay climbing the cliff level by level: the lowest */
/* caves, entered from the village by the mouth Briggs's sailors watch */
/* (SceneId_ChampaCliffCavesBriggs); the homes above them */
/* (ChampaCliffCavesHouses); the level with Obaba's door and Briggs's home */
/* (ChampaCliffCavesObaba); and at the top Obaba's forge room */
/* (ChampaCliffCavesAvimander). Before Briggs is home, knocking at Obaba's */
/* door only brings her out to ask after her grandson */
/* (scChampaCliffCavesObabaAsksForBriggs). Once he has fled up here from */
/* Felix, walking into the forge room finds Briggs with her, and she throws */
/* her salamander into the forge to come out as Avimander */
/* (scChampaCliffCavesFightAvimander). Beaten, she hears what Briggs has done */
/* (scChampaCliffCavesObabaMournsSalamander), and afterwards forges the three */
/* prongs the party brings her into the Trident of Ankohl */
/* (scChampaCliffCavesObabaForgesTrident), which Reveal's path across the pit */
/* reaches (scChampaCliffCavesRevealPath). */
#include "scene/event_layer.h"
#include "field/field_sequence.h"
#include "field/event_inline.h"
#include "item_ids.h"
#include "field/actor_script.h"

/* Set once Avimander is beaten and Obaba has heard the truth about Briggs. */
#define FLAG_CHAMPA_AVIMANDER_BEATEN 0x97e
/* The party hands Obaba the prongs one by one: set as she sees the first, the
 */
/* second, and as she forges the three into the Trident. */
#define FLAG_OBABA_SAW_FIRST_PRONG	0x975
#define FLAG_OBABA_SAW_SECOND_PRONG 0x976
#define FLAG_TRIDENT_FORGED			0x977
/* The Trident has been picked up from Obaba's forge. */
#define FLAG_TRIDENT_TAKEN 0x978
/* Set as Briggs first stops the party at Avimander's room ("Hold it right */
/* there!"); on a later visit he greets them with "Back for more?". */
#define FLAG_CHAMPA_BRIGGS_STOPPED_PARTY 0x974

/* Where the party comes back to the Avimander's room after losing to it */
/* (fdEventSetDefeatDestination) or beating it, the second numbered 99 as The */
/* Broken Seal's Altin Peak numbers its own; the party then stands in the forge
 */
/* room by its way in from the passage west of it (ENTRANCE_FORGE_ROOM, which */
/* exit 22 leads to). */
enum {
	ENTRANCE_FORGE_ROOM = 23,
	ENTRANCE_AFTER_DEFEAT = 98,
	ENTRANCE_AFTER_AVIMANDER_BATTLE = 99
};

/* The map cells the caves' step and examine triggers answer: Obaba's door, */
/* whose cell takes the number of the exit into her home, the knock at it, */
/* and the way into her forge room from the passage west of it. */
typedef enum CHAMPA_CLIFF_CAVES_CELL_BEHAVIOR {
	CELL_BEHAVIOR_OBABAS_DOOR = 15,
	CELL_BEHAVIOR_FORGE_ROOM_WAY_IN = 30,
	CELL_BEHAVIOR_KNOCK_AT_OBABAS_DOOR = 40
} CHAMPA_CLIFF_CAVES_CELL_BEHAVIOR;

/* Avimander's battle: slot 5 of ENCOUNTER_ZONE_SERPENT, between the */
/* Serpent's and Poseidon's. */
enum { ENCOUNTER_SLOT_AVIMANDER = 5 };

/* The five colours of Obaba's forge pit, from colour 2 of a background */
/* palette: dark blue as the room shows them (palette 7), and the pale ones */
/* (palette 6) and the fiery yellow to orange ones (palette 13) the map keeps */
/* for the pit to fade to. */
enum {
	FORGE_PALETTE = 7,
	FORGE_PALE_PALETTE = 6,
	FORGE_FIERY_PALETTE = 13,
	FORGE_FIRST_COLOR = 2,
	FORGE_COLORS = 5
};

/* The cell column of the path Reveal shows across Obaba's forge pit, to the */
/* Trident floating over it, and the four cells from its north rim it covers. */
enum { FORGE_PATH_CELL_X = 41, FORGE_PATH_CELL_Z = 13, FORGE_PATH_CELLS = 4 };

/* Obaba's forge room (SceneId_ChampaCliffCavesAvimander), numbering */
/* ChampaCliffCaves_AvimanderPlacementsBriggsHome; Obaba is actor 8 at her */
/* door as well (SceneId_ChampaCliffCavesObaba). */
typedef enum CHAMPA_CLIFF_CAVES_OBABAS_ROOM_ACTOR_ID {
	ACTOR_OBABA = ACTOR_FIRST_PLACED,
	/* The sailor across the room from the forge: "That pit is Obaba's forge."
	 */
	/* Once Briggs is back he turns his back on the party */
	/* (scChampaCliffCavesSailorTurnsAway). */
	ACTOR_FORGE_ROOM_SAILOR,
	/* The prong or the Trident shown as an item icon in Obaba's hands, and the
	 */
	/* forged Trident floating over the forge. */
	ACTOR_HELD_ITEM,
	ACTOR_BRIGGS,
	/* Obaba's salamander, which she throws into her forge. */
	ACTOR_SALAMANDER,
	ACTOR_AVIMANDER, /* the salamander grown into Avimander */
	ACTOR_CHAUCHA,
	ACTOR_KRADEN
} CHAMPA_CLIFF_CAVES_OBABAS_ROOM_ACTOR_ID;

/* Briggs's home once Avimander is beaten */
/* (ChampaCliffCaves_ObabaPlacementsAfterAvimander). */
typedef enum CHAMPA_CLIFF_CAVES_BRIGGS_HOME_ACTOR_ID {
	/* Briggs, lying in bed: "Once I'm over my cold, I'm setting out to get */
	/* more jewels and make amends with Madra and Daila." */
	ACTOR_BRIGGS_IN_BED = ACTOR_FIRST_PLACED,
	/* Eoleo, "Babu ba bubabu...", and Chaucha by the bed: "I wonder what's
	   wrong */
	/* with Briggs." */
	ACTOR_EOLEO = ACTOR_FIRST_PLACED + 2,
	ACTOR_CHAUCHA_BY_THE_BED
} CHAMPA_CLIFF_CAVES_BRIGGS_HOME_ACTOR_ID;

/* The level with Obaba's door until Briggs is home */
/* (ChampaCliffCaves_ObabaPlacements): Obaba behind her door, and the man */
/* outside it, "Obaba's not seeing anyone right now." */
typedef enum CHAMPA_CLIFF_CAVES_OBABAS_DOOR_ACTOR_ID {
	ACTOR_MAN_AT_OBABAS_DOOR = ACTOR_FIRST_PLACED + 1
} CHAMPA_CLIFF_CAVES_OBABAS_DOOR_ACTOR_ID;

/* The two sailors on Obaba's level once Briggs has fled up here */
/* (ChampaCliffCaves_ObabaPlacementsBriggsHome): "Leave the women and */
/* children alone." and "Did you really think you could come storming into */
/* Champa, Briggs's home, without a fight?" */
typedef enum CHAMPA_CLIFF_CAVES_OBABAS_LEVEL_SAILOR_ACTOR_ID {
	ACTOR_SAILOR_GUARDING_THE_FAMILIES = ACTOR_FIRST_PLACED,
	ACTOR_SAILOR_SPOILING_FOR_A_FIGHT
} CHAMPA_CLIFF_CAVES_OBABAS_LEVEL_SAILOR_ACTOR_ID;

/* The lowest caves' women (ChampaCliffCaves_BriggsPlacements), and once */
/* Avimander is beaten Briggs's crew home from the sea */
/* (ChampaCliffCaves_BriggsPlacementsAfterAvimander). */
typedef enum CHAMPA_CLIFF_CAVES_LOWEST_ACTOR_ID {
	/* "All of our men went to sea with Briggs, even my sweetheart!" */
	ACTOR_WOMAN_MISSING_HER_SWEETHEART = ACTOR_FIRST_PLACED,
	/* "The Champa sailors who don't live in the cliffs are no better than */
	/* cowards." */
	ACTOR_WOMAN_SCORNING_HARBOR_SAILORS,
	/* "We're going to wait until Briggs feels better before we go out to sea */
	/* again." */
	ACTOR_SAILOR_WAITING_FOR_BRIGGS,
	/* "There were plenty of treasures on that island in the middle of the */
	/* sea." */
	ACTOR_SAILOR_ON_THE_ISLAND_TREASURE,
	/* "We really want to make amends." */
	ACTOR_SAILOR_WANTING_AMENDS,
	/* "It's all my fault. If I hadn't messed with that switch, we'd have gotten
	 */
	/* everything." */
	ACTOR_SAILOR_WHO_HIT_THE_SWITCH
} CHAMPA_CLIFF_CAVES_LOWEST_ACTOR_ID;

/* The lowest caves once Briggs has fled up here */
/* (ChampaCliffCaves_BriggsPlacementsBriggsHome). */
typedef enum CHAMPA_CLIFF_CAVES_LOWEST_BRIGGS_HOME_ACTOR_ID {
	/* "Why do you have to be so stubborn? Quit following us, you big bully!" */
	ACTOR_SAILOR_CALLING_FELIX_A_BULLY = ACTOR_FIRST_PLACED,
	/* The sailor who gives in (scChampaCliffCavesSailorSurrenders). */
	ACTOR_SAILOR_WHO_SURRENDERS,
	/* The woman in her doorway (scChampaCliffCavesAskToComeIn). */
	ACTOR_WOMAN_IN_HER_DOORWAY
} CHAMPA_CLIFF_CAVES_LOWEST_BRIGGS_HOME_ACTOR_ID;

/* The homes above the lowest caves (ChampaCliffCaves_HousesPlacements): a */
/* sailor's family and the man Briggs left on watch; once Briggs has fled up */
/* here two of his sailors are there instead of the watchman and the wife */
/* (ChampaCliffCaves_HousesPlacementsBriggsHome). */
typedef enum CHAMPA_CLIFF_CAVES_HOMES_ACTOR_ID {
	/* "Before he left, Briggs ordered me to keep watch in his absence." */
	ACTOR_MAN_KEEPING_WATCH = ACTOR_FIRST_PLACED,
	/* "My husband wants our children to be sailors like him, but I simply won't
	 */
	/* have it." */
	ACTOR_SAILORS_WIFE,
	/* "Someday, I want to be a sailor, like my father before me." */
	ACTOR_SAILORS_SON,
	/* "Mom's had it pretty rough. I hope I meet a sailor with a future..." */
	ACTOR_SAILORS_DAUGHTER,
	/* "I'd rather fight than go back to jail. Bring it on, sissyboy!" */
	ACTOR_SAILOR_RATHER_FIGHTING = ACTOR_MAN_KEEPING_WATCH,
	/* "You're Felix, aren't you? You've come to bring in Briggs, haven't you?"
	 */
	ACTOR_SAILOR_WHO_KNOWS_FELIX = ACTOR_SAILORS_WIFE
} CHAMPA_CLIFF_CAVES_HOMES_ACTOR_ID;
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "game/game.h"
#include "version.h"

#include "field/call.h"
#include "macros.h"
#include "text/msg_ids.h"

/* Obaba's door opening or closing: the map edit that draws it and the cell */
/* it plays at. */
typedef struct DOOR_EDIT {
	const s16 *frames;
	u16 x;
	u16 z;
} DOOR_EDIT;

extern LATE_DATA DOOR_EDIT ChampaCliffCaves_OpenDoor;
extern LATE_DATA DOOR_EDIT ChampaCliffCaves_CloseDoor;

void scChampaCliffCavesOpenDoorAndEnter(const DOOR_EDIT *edit, s32 door,
										s32 mode);

extern ACTOR_PLACE ChampaCliffCaves_BriggsPlacements[];
extern ACTOR_PLACE ChampaCliffCaves_HousesPlacements[];
extern ACTOR_PLACE ChampaCliffCaves_BriggsPlacementsBriggsHome[];
extern ACTOR_PLACE ChampaCliffCaves_HousesPlacementsBriggsHome[];
extern ACTOR_PLACE ChampaCliffCaves_ObabaPlacementsBriggsHome[];
extern ACTOR_PLACE ChampaCliffCaves_BriggsPlacementsAfterAvimander[];
extern ACTOR_PLACE ChampaCliffCaves_HousesPlacementsAfterAvimander[];
extern ACTOR_PLACE ChampaCliffCaves_ObabaPlacementsAfterAvimander[];
extern TRIGGER ChampaCliffCaves_HousesTriggers[];
extern TRIGGER ChampaCliffCaves_ObabaTriggers[];
extern TRIGGER ChampaCliffCaves_AvimanderTriggers[];

void scChampaCliffCavesAskIfBriggsLearned(void);
void scChampaCliffCavesObabaMournsSalamander(void);
void scChampaCliffCavesHandPieceToObaba(s32 value);
void scChampaCliffCavesObabaStepsForward(s32 mode);

void scChampaCliffCavesFlareAndDieDown(void);
void scChampaCliffCavesObabaSeesAWeapon(s32 item);
void scChampaCliffCavesObabaAsksForSecondPiece(s32 item);
void scChampaCliffCavesStartFlash(s32 id, s32 mode);

/* The state of the fire over Obaba's forge (heap slot HEAP_SLOT_MENU): the */
/* pictures of the glow, a glowing disc, and the column of fire, their sprites
 */
/* at the forge's spot, and their scales, each one's height trailing its */
/* width. */
typedef struct CHAMPA_CLIFF_CAVES_STATE {
	u8 tiles[64 * 64]; /* the larger picture, the glow's 64 by 64 pixels */
	s32 pos[3];
	EFFECT_SPR glow;
	EFFECT_SPR column;
	s16 glowEntry;
	u16 glowVram;
	s16 columnEntry;
	u16 columnVram;
	FIXED glowScale;
	FIXED glowTrailingScale;
	FIXED columnScale;
	FIXED columnTrailingScale;
	s16 frm;
} CHAMPA_CLIFF_CAVES_STATE;

/* The state the forge fire's steps end on. */
enum { GLOW_DONE = 999 };

/* The forge fire's pictures are shaded in colours 224 to 255. Their */
/* brightest sixteen, of sprite palette 15, are moved onto palette 6, so that */
/* with palette 14's they are the 32 colours scChampaCliffCavesDrawForgeFire */
/* cycles, at levels about these middles: full red, green 20 and blue 3. */
enum {
	GLOW_PICTURE_ROW = 15,
	GLOW_CYCLED_ROW = 6,
	GLOW_COLORS = 2 * PALETTE_COLORS,
	GLOW_GREEN_MIDDLE = 20,
	GLOW_BLUE_MIDDLE = 3
};

/* A 256-colour pixel's palette row (its high four bits) and its colour in */
/* that row. */
#define PIXEL_PALETTE_ROW_MASK 0xf0
#define PIXEL_ROW_COLOR_MASK   0xf

void scChampaCliffCavesLoadGlow(void);
void scChampaCliffCavesDrawForgeFire(void);
void scChampaCliffCavesLoadColumn(void);

extern s16 ChampaCliffCaves_OpenDoorFrames[];
extern s16 ChampaCliffCaves_CloseDoorFrames[];

/* Defines an update that grows an actor by step (a 16.16 scale) a frame until
 */
/* it is full size. */
#define DEFINE_CHAMPA_CLIFF_CAVES_GROW(name, step)                             \
	void name(ACTOR *actor) {                                                  \
		actor->scaleX += step;                                                 \
		if(actor->scaleX >= FIXED_ONE) {                                       \
			actor->scaleX = FIXED_ONE;                                         \
			actor->update = 0;                                                 \
		}                                                                      \
		actor->scaleY = actor->scaleX;                                         \
	}

/* Each frame the Trident, thrown up from the rising column, grows by a */
/* sixteenth of its size until it is full size. */
DEFINE_CHAMPA_CLIFF_CAVES_GROW(ChampaCliffCaves_GrowThrown, FIXED_ONE / 16)

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE ChampaCliffCaves_ObabaPlacements[];
extern ACTOR_PLACE ChampaCliffCaves_AvimanderPlacements[];
extern ACTOR_PLACE ChampaCliffCaves_AvimanderPlacementsBriggsHome[];
extern ACTOR_PLACE ChampaCliffCaves_AvimanderPlacementsAfterAvimander[];

const ACTOR_PLACE *scGetActorPlacements(void) {
	s32 scene;

	if(syGameFlagGet(FLAG_CHAMPA_AVIMANDER_BEATEN)) {
		scene = MainRoutineStateWork.next;
		if(scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
			return ChampaCliffCaves_BriggsPlacementsAfterAvimander;
		if(scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
			return ChampaCliffCaves_HousesPlacementsAfterAvimander;
		if(scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
			return ChampaCliffCaves_ObabaPlacementsAfterAvimander;
		if(scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
			return ChampaCliffCaves_AvimanderPlacementsAfterAvimander;
	}
	else {
		if(syGameFlagGet(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
			scene = MainRoutineStateWork.next;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
				return ChampaCliffCaves_BriggsPlacementsBriggsHome;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
				return ChampaCliffCaves_HousesPlacementsBriggsHome;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
				return ChampaCliffCaves_ObabaPlacementsBriggsHome;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
				return ChampaCliffCaves_AvimanderPlacementsBriggsHome;
		}
		else {
			scene = MainRoutineStateWork.next;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
				return ChampaCliffCaves_BriggsPlacements;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
				return ChampaCliffCaves_HousesPlacements;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
				return ChampaCliffCaves_ObabaPlacements;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
				return ChampaCliffCaves_AvimanderPlacements;
		}
	}
	return ChampaCliffCaves_BriggsPlacements;
}

/* Walking into a door in the cliff caves: it opens with a sound, Felix walks */
/* through (lined up on the door first when `mode` is 0, straight on when 1) */
/* and the screen goes to the room behind it. */
void scChampaCliffCavesOpenDoorAndEnter(const DOOR_EDIT *edit, s32 door,
										s32 mode) {
	s32 id;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(edit->frames, edit->x, edit->z);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlg = ACTOR_FALLS;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	id = MainRoutineStateWork.leaderActorId;
	if(mode == 0)
		fdActorCenterAndWalk(id, 2, -8);
	else
		fdActorSetDestinationOffset(id, 0, -8);
	fdEventWait(10);
	syImportEventSetExitRequest(door);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* Walking north into Obaba's door (cell (11, 26)) once Avimander is beaten */
/* opens it, and Felix walks into the home behind it, where Briggs lies in */
/* bed. */
void scChampaCliffCavesEnterDoor(s32 door) {
	scChampaCliffCavesOpenDoorAndEnter(&ChampaCliffCaves_OpenDoor, door, 0);
}

/* Walking into or knocking at Obaba's door, at the west end of its passage */
/* (cells (11, 25) and (11, 26)), before Avimander is beaten: Felix steps up */
/* beside it and she opens it, asks "Has my grandson returned? Has my Briggs */
/* come home?", answers the yes ("You lie!") or the no, and shuts herself in */
/* again. */
void scChampaCliffCavesObabaAsksForBriggs(void) {
	const DOOR_EDIT *edit;
	s32 msg;

	edit = &ChampaCliffCaves_OpenDoor;
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 168, 424);
	fdActorFaceDirectionAndRefreshById(MainRoutineStateWork.leaderActorId,
									   FACING_EAST);
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(edit->frames, edit->x, edit->z);
	fdActorGetById(ACTOR_OBABA)->motionFlg = 0;
	fdActorSetPos(ACTOR_OBABA, 184, 408);
	fdActorSetSpeed(ACTOR_OBABA, 1.8);
	fdActorWalkByAndWaitById(ACTOR_OBABA, 0, 16);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
	msg = (s32)MsgHasMyGrandson;
	fdEventSetMessageId(msg);
	/* "Has my grandson returned? Has my Briggs come home?" Yes: "You lie! If */
	/* Briggs had truly returned..." No: "I asked not to be disturbed until my
	 */
	/* grandson has returned home!..." */
	fdMessageShowAtActor(ACTOR_OBABA, 0);
	if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdEventWait(10);
		fdEventSetMessageId(msg + 1);
	}
	else {
		fdEventWait(20);
		fdEventSetMessageId(msg + 2);
	}
	fdEventShowMessageAndWait(ACTOR_OBABA, 0);
	fdActorSetSpeed(ACTOR_OBABA, 0.8);
	fdActorWalkByAndWait(ACTOR_OBABA, 0, -14);
	fdActorSetPos(ACTOR_OBABA, 0, 0);
	edit = &ChampaCliffCaves_CloseDoor;
	fdMapGridPlayMapEdit(edit->frames, edit->x, edit->z);
	sySndPlay(SE_DOOR_OPEN);
}

/* Talking to the woman standing in the doorway of her home at the west end of
 */
/* the lowest caves, once Briggs is back: "What, you want to come in?" A yes */
/* gets "As if! I'm a nice girl...", a no "You're wasting my time." */
void scChampaCliffCavesAskToComeIn(s32 actor) {
	fdEventSetMessageId((s32)MsgWhatYouWant);
	/* "What, you want to come in?" Yes: "As if! I'm a nice girl..." No: "Yeah?
	 */
	/* So what do you want? You're wasting my time." */
	fdMessageShowAtActor(actor, 0);
	if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdEventWait(10);
		fdEventSetMessageId((s32)MsgWhatYouWant + 1);
	}
	else {
		fdEventWait(20);
		fdEventSetMessageId((s32)MsgWhatYouWant + 2);
	}
	fdEventShowMessageAndWait(actor, 0);
}

/* Talking to the second of the two sailors on the east side of the lowest */
/* caves, once Briggs is back: he gives in ("I'll go back to */
/* Alhafra...but only if Briggs is going") and keeps his eyes on Felix. */
void scChampaCliffCavesSailorSurrenders(s32 actorId) {
	ACTOR *actor;

	fdEventSetMessageId((s32)MsgAllRightI);
	/* "All right, I surrender! I'll go back to Alhafra...but only if Briggs..."
	 */
	fdEventShowMessageAndWait(actorId, 0);
	actor = fdActorGetById(actorId);
	fdActorWatchActor(actor,
					  fdActorGetById(MainRoutineStateWork.leaderActorId));
}

/* Talking to the sailor in Obaba's forge room, once Briggs is back: "Aw, come
 */
/* on! I'm not doing anything! Leave me alone!", and he turns his back on the */
/* party, squashing and stretching from then on (MOVEMENT_TYPE_SQUASH). */
void scChampaCliffCavesSailorTurnsAway(s32 actor) {
	fdEventSetMessageId((s32)MsgAwComeOn);
	/* "Aw, come on! I'm not doing anything! Leave me alone!" */
	fdEventShowMessageAndWait(actor, 0);
	fdActorFaceDirectionAndWait(actor, FACING_NORTH_NORTHEAST);
	fdActorSetMovementType(fdActorGetById(actor), MOVEMENT_TYPE_SQUASH);
}

/* Reveal in Obaba's forge room shows a path hidden across the forge pit, */
/* north to the forged Trident floating over it. The path's cells, kept */
/* further down the map, are copied there with their behaviours. */
void scChampaCliffCavesRevealPath(void) {
	fdMapGridCopyCellTilesCellsRect(41, 97, 1, FORGE_PATH_CELLS,
									FORGE_PATH_CELL_X, 77);
	fdMapCopyCellAttributeRect(41, 33, 1, FORGE_PATH_CELLS, FORGE_PATH_CELL_X,
							   FORGE_PATH_CELL_Z);
}

/* When Reveal wears off, the pit's own cells cover the path again, and a */
/* leader standing on it is set down on the pit's south rim. */
void scChampaCliffCavesHidePath(void) {
	ACTOR *actor;
	s32 x;
	s32 z;

	fdMapGridCopyCellTilesCellsRect(50, 97, 1, FORGE_PATH_CELLS,
									FORGE_PATH_CELL_X, 77);
	fdMapCopyCellAttributeRect(46, 33, 1, FORGE_PATH_CELLS, FORGE_PATH_CELL_X,
							   FORGE_PATH_CELL_Z);
	actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	x = FIELD_CELL(actor->x.fixed);
	z = FIELD_CELL(actor->z.fixed);
	/* On the path, or one row past it on the south rim (row 17), where he is */
	/* set down. */
	if(MainRoutineStateWork.next == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER &&
	   x == FORGE_PATH_CELL_X &&
	   (u32)(z - FORGE_PATH_CELL_Z) <= FORGE_PATH_CELLS)
		fdActorSetPos(MainRoutineStateWork.leaderActorId, 664, 280);
}

extern TRIGGER ChampaCliffCaves_BriggsTriggers[];

/* Talking to the forged Trident floating over Obaba's forge pit, reached by */
/* the path Reveal shows, takes it. */
void scChampaCliffCavesPickUpTrident(s32 a) {
	fdEventPickUpItem(a, ITEM_TRIDENT, FLAG_TRIDENT_TAKEN);
}

const TRIGGER *scGetTriggers(void) {
	s32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
		return ChampaCliffCaves_BriggsTriggers;
	if(scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
		return ChampaCliffCaves_HousesTriggers;
	if(scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
		return ChampaCliffCaves_ObabaTriggers;
	if(scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
		return ChampaCliffCaves_AvimanderTriggers;
	return ChampaCliffCaves_BriggsTriggers;
}

s32 scInit(void) {
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

#if !TLA_PATCH_1
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
#else
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
#endif
	if(party->next != SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
		fdActorGetById(party->leaderActorId)->priFlg |=
			ACTOR_PRIORITY_IGNORES_RISES;
	fdEffectStartBlankOverlay(0);
	if(party->next == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER) {
		if(party->entrance == ENTRANCE_AFTER_DEFEAT)
			scChampaCliffCavesAskIfBriggsLearned();
		if(party->entrance == ENTRANCE_AFTER_AVIMANDER_BATTLE) {
			party->returnEntrance = ENTRANCE_FORGE_ROOM;
			party->entrance = ENTRANCE_FORGE_ROOM;
			scChampaCliffCavesObabaMournsSalamander();
		}
		if(syGameFlagGet(FLAG_TRIDENT_FORGED) &&
		   !syGameFlagGet(FLAG_TRIDENT_TAKEN)) {
			scFieldActorShowItemIconById(ACTOR_HELD_ITEM, ITEM_TRIDENT);
			fdActorSetPos(ACTOR_HELD_ITEM, 664, 232);
			fdActorGetById(ACTOR_HELD_ITEM)->motionFlg = ACTOR_FLOATS;
		}
		if(syGameFlagGet(FLAG_TRIDENT_FORGED))
			fdActorSetPos(ACTOR_OBABA, 664, 200);
	}
	else if(party->next == SCENE_CHAMPA_CLIFF_CAVES_OBABA) {
		if(syGameFlagGet(FLAG_CHAMPA_AVIMANDER_BEATEN)) {
			fdActorSetAnimationById(ACTOR_BRIGGS_IN_BED, 11);
			fdActorSetSprFlags(fdActorGetById(ACTOR_BRIGGS_IN_BED), 0);
		}
	}
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Felix walks up to Obaba's counter, holds the prong up over it, leaves it */
/* there and steps back. */
void scChampaCliffCavesHandPieceToObaba(s32 actor) {
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 648, 200);
	fdActorFaceDirectionAndRefreshById(MainRoutineStateWork.leaderActorId,
									   FACING_EAST);
	fdEventWait(5);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId,
							   ANIM_LEADER_SPRING);
	fdEventWait(7);
	fdActorShowItemIconById(ACTOR_HELD_ITEM, actor);
	fdActorSetPos(ACTOR_HELD_ITEM, 648, 196);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorWalkByAndWaitById(MainRoutineStateWork.leaderActorId, -16, -8);
	fdEventWait(1);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	fdActorFaceDirectionAndRefreshById(MainRoutineStateWork.leaderActorId,
									   FACING_EAST);
	fdEventWait(10);
}

/* Obaba steps up to her counter and takes the prong from it, and unless she */
/* is to forge it at once steps back. */
void scChampaCliffCavesObabaStepsForward(s32 value) {
	fdActorSetSpeed(ACTOR_OBABA, 0.8);
	fdActorGetById(ACTOR_OBABA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWaitById(ACTOR_OBABA, 0, 6);
	fdEventWait(1);
	fdActorGetById(ACTOR_OBABA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdEventWait(10);
	fdActorSetAnimationById(ACTOR_OBABA, 8);
	fdEventWait(15);
	fdActorSetPos(ACTOR_HELD_ITEM, 0, 0);
	fdEventWait(15);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdEventWait(15);
	if(value == 0) {
		fdActorGetById(ACTOR_OBABA)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkByAndWait(ACTOR_OBABA, 0, -6);
		fdEventWait(1);
		fdActorGetById(ACTOR_OBABA)->steeringFlg |= ACTOR_FACES_MOTION;
		fdEventWait(10);
	}
}

/* The forge pit's glow changes colour, its five colours fading from one set to
 */
/* another over the given number of frames, a step a frame. */
void scChampaCliffCavesFadeCols(u16 *from, u16 *to, s32 frames) {
	s32 step;

	for(step = 0; step <= frames; step++) {
		s32 offset;
		s32 left;

		for(offset = 0, left = 0; left < FORGE_COLORS; left++) {
			s32 red, green, blue, toRed, toGreen, toBlue;

			red = RGB_RED(*(u16 *)(offset + (s32)from));
			green = RGB_GREEN(*(u16 *)(offset + (s32)from));
			blue = RGB_BLUE(*(u16 *)(offset + (s32)from));
			toRed = RGB_RED(*(u16 *)(offset + (s32)to));
			toGreen = RGB_GREEN(*(u16 *)(offset + (s32)to));
			toBlue = RGB_BLUE(*(u16 *)(offset + (s32)to));
			red += step * (toRed - red) / frames;
			green += step * (toGreen - green) / frames;
			blue += step * (toBlue - blue) / frames;
			SET_RGB(
				*(u16 *)((u32)&BG_PLTT_COLOR(FORGE_PALETTE, FORGE_FIRST_COLOR) +
						 offset),
				red, green, blue);
			offset += 2;
		}
		sySynch(1);
	}
}

/* Obaba is handed the first prong: she knows it for part of the trident of */
/* Ankohl and offers to keep it until the party brings the other two. */
void scChampaCliffCavesObabaSeesAWeapon(s32 actor) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgThisLooksLike);
	scChampaCliffCavesHandPieceToObaba(actor);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationById(ACTOR_OBABA, 7);
	fdEventWait(30);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdEventWait(15);
	/* "This looks like a weapon!" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetAnimationById(ACTOR_OBABA, 7);
	fdEventWait(30);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdEventWait(15);
	/* "It looks like part of a trident, but it's only one third of the..." */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetAnimationById(ACTOR_OBABA, 7);
	fdEventWait(30);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdEventWait(15);
	/* "Why... Could it be? Is this the legendary trident of Ankohl?" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_OBABA, 2);
	fdEventWait(8);
	/* "I'll hold on to this for you. It won't do you any good like this..." */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	scChampaCliffCavesObabaStepsForward(0);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
	fdEventWait(10);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "You seem unsure... You don't think I'll pull a fast one on you, do you?"
	 */
	/* Yes: "Kids today. So mistrustful." No: "Ah, that's nice. Don't you
	 * worry!" */
	fdMessageShowAtActor(ACTOR_OBABA, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdEventWait(20);
		/* "Kids today. So mistrustful." */
		fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(35);
		FieldStateWork->msg++;
		/* "Ah, that's nice. Don't you worry!" */
		fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	}
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_NOD);
	/* "If you bring me the other two pieces, I'll be happy to rebuild it..." */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	fdEventEnd();
}

/* Obaba is handed the second prong and keeps it with the first. */
void scChampaCliffCavesObabaAsksForSecondPiece(s32 actor) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgAhHaveYou);
	scChampaCliffCavesHandPieceToObaba(actor);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationById(ACTOR_OBABA, 7);
	fdEventWait(30);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdEventWait(15);
	/* "Ah, have you found the second piece, dear?" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetAnimationById(ACTOR_OBABA, 7);
	fdEventWait(30);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdEventWait(15);
	/* "It's magnificent!" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	scChampaCliffCavesObabaStepsForward(0);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
	fdEventWait(10);
	/* "I'll hold on to this as well. Wouldn't want it getting lost, now..." */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	fdEventEnd();
}

/* The trident's three prongs, in the order Obaba throws them into the forge. */
typedef struct TRIDENT_PRONGS {
	s32 items[3];
} TRIDENT_PRONGS;

extern TRIDENT_PRONGS ChampaCliffCaves_Prongs;

/* Obaba is handed the last prong and throws the three into her forge, whose */
/* pit flashes pale at each, then flares, shakes the room and lifts out the */
/* Trident to float over it. */
void scChampaCliffCavesObabaForgesTrident(s32 value) {
	ACTOR *thrown = fdActorGetById(ACTOR_HELD_ITEM);
	u16 dark[FORGE_COLORS];
	u16 pale[FORGE_COLORS];
	s32 glow[3];
	s32 i;
	ACTOR *actor;
	u16 *palette;

	glow[0] = toFIXED(662);
	glow[1] = 0;
	glow[2] = toFIXED(238);
	palette = (u16 *)&BG_PLTT_COLOR(FORGE_PALETTE, FORGE_FIRST_COLOR);
	dark[0] = *palette++;
	dark[1] = *palette++;
	dark[2] = *palette++;
	dark[3] = *palette++;
	dark[4] = *palette;
	palette = (u16 *)&BG_PLTT_COLOR(FORGE_PALE_PALETTE, FORGE_FIRST_COLOR);
	pale[0] = *palette++;
	pale[1] = *palette++;
	pale[2] = *palette++;
	pale[3] = *palette++;
	pale[4] = *palette;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgWellDoneYou);
	scChampaCliffCavesHandPieceToObaba(value);
	fdActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Well done! You found all three pieces of the trident!" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "That means I can begin work reforging the ancient trident of Ankohl!..."
	 */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	scChampaCliffCavesObabaStepsForward(1);
	fdActorWalkToAndWait(ACTOR_OBABA, 664, 200);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
	fdEventWait(10);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
	fdEventWait(10);
	/* "We'll just toss all three pieces in here, shall we?" Yes: "Just leave it
	 */
	/* to me..." No: "There's nothing to be afraid of, dear. Just..." */
	fdMessageShowAtActor(ACTOR_OBABA, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdEventWait(20);
		/* "Just leave it to me..." */
		fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(40);
		FieldStateWork->msg++;
		/* "There's nothing to be afraid of, dear. Just leave it to me." */
		fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	}
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST,
						 0);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	fdEventWait(10);
	fdCameraMoveTo(664, CAMERA_KEEP, 232, 1);
	fdCameraWaitForArrival();
	fdEventWait(10);
	fdActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
	/* "Heart of the earth, fury of the forge's flames, grant me power!" */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	for(i = 0; i <= 2; i++) {
		TRIDENT_PRONGS prongs;
		FIXED landing[3];
		prongs = ChampaCliffCaves_Prongs;
		fdActorSetAnimationById(ACTOR_OBABA, 9);
		fdEventWait(3);
		fdActorShowItemIconById(ACTOR_HELD_ITEM, prongs.items[i]);
		fdActorSetPos(ACTOR_HELD_ITEM, 668, 200);
		thrown->gravity = toFIXED(1) / 4;
		fdActorJump(ACTOR_HELD_ITEM, 4, 2);
		fdActorSetSprPriority(ACTOR_HELD_ITEM, 1);
		fdActorSetSpeed(ACTOR_HELD_ITEM, 1);
		fdActorSetDestinationOffsetById(ACTOR_HELD_ITEM, 0, 40);
		sySndPlay(SE_FALL);
		fdEventWait(34);
		landing[0] = thrown->x.fixed;
		landing[1] = thrown->y.fixed;
		landing[2] = thrown->z.fixed;
		fdActorSetPos(ACTOR_HELD_ITEM, 0, 0);
		fdEventWait(20);
		sySndPlay(SE_HEAVY_IMPACT);
		scChampaCliffCavesFadeCols(dark, pale, 12);
		scChampaCliffCavesFadeCols(pale, dark, 48);
		fdEventWait(10);
	}
	actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	actor->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWaitById(MainRoutineStateWork.leaderActorId, 12, 12);
	fdEventWait(1);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	fdActorFaceDirectionAndRefreshById(MainRoutineStateWork.leaderActorId,
									   FACING_SOUTHEAST);
	fdEventWait(10);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_LEAN_BACK);
	fdEventWait(20);
	fdActorSetAnimationById(ACTOR_OBABA, 7);
	fdEventWait(60);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "That's funny... Nothing happened." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	fdEventWait(20);
	sySndPlay(SE_SHIMMER);
	scChampaCliffCavesFadeCols(dark, pale, 48);
	fdActorStartRepeatedMotion(MainRoutineStateWork.leaderActorId, 3);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(50);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	Call2((void (*)())fdStartColTransition, 16, RGB(16, 4, 4));
	fdMapGridSetShake(2, 2, 1);
	scChampaCliffCavesLightForge(glow);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdActorJump(ACTOR_FELIX, 6, 0);
	sySndPlay(SE_TABLET_RISE);
	fdActorGetById(ACTOR_FELIX)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_FELIX, -12, -12);
	fdEventWait(1);
	fdActorGetById(ACTOR_FELIX)->steeringFlg |= ACTOR_FACES_MOTION;
	fdEventWait(10);
	fdActorStartRepeatedMotion(ACTOR_OBABA, 3);
	fdEffectSetAttached(ACTOR_OBABA,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(30);
	/* "What's going on?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	fdMapGridSetShake(4, 4, 1);
	scChampaCliffCavesRaiseColumn(glow);
	sySndPlay(SE_TABLET_RISE);
	fdEventWait(10);
	fdActorJump(ACTOR_OBABA, 6, 15);
	fdActorJump(ACTOR_OBABA, 6, 23);
	sySndPlay(SE_TABLET_RISE);
	fdEventWait(10);
	/* "Something's happening! Is that...the legendary trident?" */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	scFieldActorShowItemIconById(ACTOR_HELD_ITEM, ITEM_TRIDENT);
	fdActorSetPos(ACTOR_HELD_ITEM, 664, 232);
	fdActorSetSprPriority(ACTOR_HELD_ITEM, 1);
	thrown->motionFlg = ACTOR_FALLS;
	thrown->y.fixed = toFIXED(24);
	thrown->update = ChampaCliffCaves_GrowThrown;
	thrown->groundHeight = 0;
	thrown->scaleX = 0;
	thrown->scaleY = 0;
	fdActorJump(ACTOR_HELD_ITEM, 5, 0);
	fdStartPaletteTransition(8);
	scChampaCliffCavesFlareAndDieDown();
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	while(thrown->y.fixed > 0)
		sySynch(1);
	thrown->gravity = toFIXED(1);
	thrown->y.fixed = 0;
	thrown->motionFlg = ACTOR_FLOATS;
	scPlayMusic();
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_OBABA, 2);
	fdEventWait(5);
	/* "After all these years, the trident is complete again. But what's..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
	fdEventWait(10);
	/* "Regardless, the trident has been fixed. It belongs to you now..." */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	fdEventWait(10);
	fdActorSetSprPriority(ACTOR_HELD_ITEM, 2);
	fdEventEnd();
}

/* Talking to Obaba at her counter: she takes the next prong the party carries,
 */
/* right, left then center, or talks of the trident when it has none. From */
/* east of the forge path's column the field menu opens instead. */
void scChampaCliffCavesObabaOffersHelp(s32 actor) {
	s32 item;
	/* DEAD: each owner is stored and never read; the ROM keeps the copies of */
	/* the first two. */
	s32 owner;

	if(FIELD_CELL(
		   fdActorGetById(MainRoutineStateWork.leaderActorId)->x.fixed) <=
	   FORGE_PATH_CELL_X)
		goto first;
	FieldStateWork->fieldMenu = 1;
	return;
remove:
	gmPartyInventoryRemove(item);
	goto chosen;
last:
	item = ITEM_CENTER_PRONG;
	if((owner = gmPartyInventoryFindMember(item)) != -1)
		goto remove;
	item = -1;
	goto chosen;
first:
	item = ITEM_RIGHT_PRONG;
	if((owner = gmPartyInventoryFindMember(item)) != -1)
		goto remove;
	item++;
	if((owner = gmPartyInventoryFindMember(item)) == -1)
		goto last;
	gmPartyInventoryRemove(item);
chosen:
	if(item != -1) {
		if(!fdEventGetFlag(FLAG_OBABA_SAW_FIRST_PRONG)) {
			syGameFlagSet(FLAG_OBABA_SAW_FIRST_PRONG);
			scChampaCliffCavesObabaSeesAWeapon(item);
		}
		else if(!fdEventGetFlag(FLAG_OBABA_SAW_SECOND_PRONG)) {
			syGameFlagSet(FLAG_OBABA_SAW_SECOND_PRONG);
			scChampaCliffCavesObabaAsksForSecondPiece(item);
		}
		else if(!fdEventGetFlag(FLAG_TRIDENT_FORGED)) {
			syGameFlagSet(FLAG_TRIDENT_FORGED);
			syFieldMapGridSetCellHeight(toFIXED(656), toFIXED(208), 0, -4);
			syFieldMapGridSetCellHeight(toFIXED(656), toFIXED(224), 0, -4);
			syFieldMapGridSetCellHeight(toFIXED(656), toFIXED(240), 0, -4);
			syFieldMapGridSetCellHeight(toFIXED(656), toFIXED(256), 0, -4);
			scChampaCliffCavesObabaForgesTrident(item);
			syFieldMapGridSetCellHeight(toFIXED(656), toFIXED(208), 0, 0);
			syFieldMapGridSetCellHeight(toFIXED(656), toFIXED(224), 0, 0);
			syFieldMapGridSetCellHeight(toFIXED(656), toFIXED(240), 0, 0);
			syFieldMapGridSetCellHeight(toFIXED(656), toFIXED(256), 0, 0);
		}
	}
	else {
		fdActorFaceActorById(actor, MainRoutineStateWork.leaderActorId, 5);
		/* "I mean it. If there's ever anything you need, just call on me.",
		 * then */
		/* "The legend says that anyone who holds the whole lance has nothing to
		 */
		/* fear from the sea.", "Yes, there's no doubt about it. Those must
		 * truly */
		/* be fragments of the legendary trident." and "The trident seems to
		 * have */
		/* some amazing power..." */
		if(!syGameFlagGet(FLAG_OBABA_SAW_FIRST_PRONG))
			fdEventSetMessageId((s32)MsgIMeanIt);
		else if(!syGameFlagGet(FLAG_OBABA_SAW_SECOND_PRONG))
			fdEventSetMessageId((s32)MsgTheLegendSaysThat);
		else if(!syGameFlagGet(FLAG_TRIDENT_FORGED))
			fdEventSetMessageId((s32)MsgYesThereS);
		else
			fdEventSetMessageId((s32)MsgTheTridentSeems);
		fdEventShowMessageAndWait(actor, 0);
		fdActorFaceDirection(actor, FACING_SOUTH_SOUTHWEST, 0);
	}
}

/* Each frame Avimander, leaping out of the forge's fire, grows by three sixty-
 */
/* fourths of its size, from a quarter to full size. */
DEFINE_CHAMPA_CLIFF_CAVES_GROW(ChampaCliffCaves_GrowThrownSlowly,
							   FIXED_ONE * 3 / 64)

/* Walking east into Obaba's forge room from the passage (cells (36, 12) to */
/* (36, 18)) once Briggs has fled up here: "Hold it right there!" Briggs */
/* stops the party, and Obaba, for her great-grandson Eoleo's sake, tells Felix
 */
/* he had better leave; if he will not, she throws her salamander into the */
/* forge, whose pit turns fiery, and it climbs out grown into Avimander to */
/* fight. Coming back after a defeat, Briggs greets the party with "Back for */
/* more?". */
void scChampaCliffCavesFightAvimander(void) {
	u16 dark[FORGE_COLORS];
	u16 fiery[FORGE_COLORS];
	VECTOR3 position;
	const u16 *color;
	ACTOR *actor = fdActorGetById(ACTOR_AVIMANDER);

	if(fdEventGetFlag(FLAG_CHAMPA_AVIMANDER_BEATEN))
		return;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 584, 248);
	if(fdEventGetFlag(FLAG_CHAMPA_BRIGGS_STOPPED_PARTY)) {
		fdActorFaceDirectionAndWait(MainRoutineStateWork.leaderActorId,
									FACING_EAST);
		fdEventSetMessageId((s32)MsgBackForMore);
		/* "Back for more?" */
		syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		sySndPlay(MUS_CONFRONTATION);
		fdCameraMoveTo(664, CAMERA_KEEP, 256, 1);
		fdCameraWaitForArrival();
		fdEventWait(5);
		fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
		/* "I wouldn't be doing this if it weren't for Eoleo, you know." */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0,
										5);
		scFieldActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
							  30);
		/* "I'm sorry. You look like quite a nice boy, but I just can't
		 * allow..." */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0,
										5);
	}
	else {
		fdEventSetFlag(FLAG_CHAMPA_BRIGGS_STOPPED_PARTY);
		fdEventSetMessageId((s32)MsgHoldItRight);
		/* "Hold it right there!" */
		syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		sySndPlay(MUS_CONFRONTATION);
		fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
								 FACING_EAST, 0);
		fdCameraMoveTo(664, CAMERA_KEEP, 256, 1);
		fdCameraWaitForArrival();
		fdEventWait(5);
		fdActorJump(ACTOR_BRIGGS, 6, 15);
		fdActorJump(ACTOR_BRIGGS, 6, 23);
		/* "So, Felix, you've come." */
		syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		/* "Don't assume I'll go so easy on you this time!" */
		syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		scFieldActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
		fdEventWait(10);
		/* "Is that boy him? The "mighty warrior" you told me about?" */
		syFieldEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
		scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
							  30);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
		fdEventWait(10);
		/* "I told you he didn't look like much, Grandma..." */
		syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		/* "Weren't you even listening to me?" */
		syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_NOD);
		/* "All you told me is that his name is Felix and that he is a
		 * "mighty..." */
		syFieldEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
		fdEventWait(5);
		fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
		/* "He doesn't look like the sort of person who's interested in
		 * stealing..." */
		syFieldEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
		fdEventWait(30);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
		fdEventWait(10);
		scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		/* "But that's not the point, Grandma!" */
		syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
		fdEventWait(20);
		fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
		fdEventWait(5);
		/* "If you don't destroy him now, he's going to take me far away,
		 * and..." */
		syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		scFieldActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		/* "Oh, now he doesn't look like the kind of boy who would do that..."
		 */
		syFieldEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
		fdEventWait(10);
		scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
							  45);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS,
									FACING_SOUTH_SOUTHEAST + FACING_NUDGE);
		fdEventWait(10);
		/* "If he takes me away, poor Eoleo will be so sad! What do you say..."
		 */
		syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		scFieldActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
		fdEventWait(10);
		/* "Oh, that's just not fair. You know I can't deny my darling..." */
		syFieldEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
		fdEventWait(5);
		fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
		fdEventWait(5);
		/* "I don't see why I should be the one to get you out of this mess,
		 * but..." */
		syFieldEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
		fdEventWait(5);
		/* "Grandma, please!" */
		syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
		fdEventWait(5);
	}
	fdEventSetMessageId((s32)MsgOhAllRightListen);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_NORTH_NORTHWEST);
	fdEventWait(20);
	fdActorGetById(ACTOR_OBABA)->facing = FACING_SOUTH_SOUTHWEST;
	fdActorSetAnimationById(ACTOR_OBABA, 5);
	fdEventWait(30);
	fdActorSetAnimationById(ACTOR_OBABA, 6);
	fdActorSetPos(ACTOR_SALAMANDER, 724, 252);
	fdActorGetById(ACTOR_SALAMANDER)->scaleX = FIXED_ONE * 6 / 5;
	fdActorGetById(ACTOR_SALAMANDER)->scaleY = FIXED_ONE * 6 / 5;
	fdActorSetSprFlags(fdActorGetById(ACTOR_SALAMANDER), 0);
	/* "Oh, all right. Listen, you have no idea what you're getting into. I */
	/* think you'd better leave." Yes: "Ah, you've made a good decision..." No:
	 */
	/* "Stubborn, are you? Ah, well..." */
	fdEventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		scPlayMusic();
		fdEventWait(15);
		fdActorSetPos(ACTOR_SALAMANDER, 0, 0);
		fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
		fdActorGetById(ACTOR_OBABA)->facing = FACING_WEST;
		/* "Ah, you've made a good decision. You seem like a reasonable boy." */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0,
										5);
		FieldStateWork->msg++;
		fdActorWalkByAndWaitById(ACTOR_FELIX, -16, 0);
		fdEventWait(5);
	}
	else {
		fdEventWait(25);
		FieldStateWork->msg++;
		/* "Stubborn, are you? Ah, well. If that's the way it's going to be, get
		 */
		/* ready for your medicine!" */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0,
										5);
		scFieldActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		/* "Heart of the earth, fury of the forge's flames, grant me power!" */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0,
										5);
		sySndPlay(SOUND_FADE_OUT_BGM);
		syFieldMapGridSetCellHeight(toFIXED(656), toFIXED(208), 0, -4);
		syFieldMapGridSetCellHeight(toFIXED(656), toFIXED(224), 0, -4);
		syFieldMapGridSetCellHeight(toFIXED(656), toFIXED(240), 0, -4);
		syFieldMapGridSetCellHeight(toFIXED(656), toFIXED(256), 0, -4);
		actor->gravity = toFIXED(1) / 4;
		actor->motionFlg = ACTOR_FALLS;
		actor->groundHeight = -toFIXED(999);
		fdActorJump(ACTOR_SALAMANDER, 10, 0);
		sySndPlay(SE_ENEMY_CRY);
		fdActorSetSprPriority(ACTOR_SALAMANDER, 1);
		fdActorSetSpeed(ACTOR_SALAMANDER, 3);
		fdActorWalkToById(ACTOR_SALAMANDER, 644, 248);
		fdEventWait(21);
		fdActorSetPos(ACTOR_SALAMANDER, 0, 0);
		fdEventWait(50);
		fdMapGridSetShake(2, 2, 1);
		fdEventWait(70);
		sySndPlay(SE_BOSS_ROAR);
		fdEventWait(25);
		color = (const u16 *)&BG_PLTT_COLOR(FORGE_PALETTE, FORGE_FIRST_COLOR);
		dark[0] = *color++;
		dark[1] = *color++;
		dark[2] = *color++;
		dark[3] = *color++;
		dark[4] = *color++;
		color =
			(const u16 *)&BG_PLTT_COLOR(FORGE_FIERY_PALETTE, FORGE_FIRST_COLOR);
		fiery[0] = *color++;
		fiery[1] = *color++;
		fiery[2] = *color++;
		fiery[3] = *color++;
		fiery[4] = *color++;
		scChampaCliffCavesFadeCols(dark, fiery, 48);
		fdEventWait(20);
		sySndPlay(SE_TABLET_RISE);
		position.x = toFIXED(662);
		position.y = 0;
		position.z = toFIXED(238);
		scChampaCliffCavesStartForgeFire(&position);
		sySndPlay(SE_BEAST_CRY);
		fdActorSetPos(ACTOR_AVIMANDER, 664, 248);
		actor->gravity = toFIXED(1);
		actor->motionFlg = ACTOR_FALLS;
		actor->groundHeight = 0;
		actor->y.fixed = toFIXED(32);
		actor->scaleX = FIXED_ONE / 4;
		actor->update = (s32)ChampaCliffCaves_GrowThrownSlowly;
		actor->scaleY = FIXED_ONE / 4;
		fdActorJump(ACTOR_AVIMANDER, 11, 0);
		sySndPlay(SE_BEAST_CRY);
		fdActorSetSpeed(ACTOR_AVIMANDER, 3.2);
		fdActorWalkToAndWait(ACTOR_AVIMANDER, 584, 248);
		fdEventSetDefeatDestination(SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER,
									ENTRANCE_AFTER_DEFEAT);
		fdEventSetAfterBattleDestination(SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER,
										 ENTRANCE_AFTER_AVIMANDER_BATTLE);
		btSetType(BATTLE_TYPE_NO_ESCAPE);
		fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SERPENT,
										  ENCOUNTER_SLOT_AVIMANDER);
	}
	fdEventEnd();
}

/* Coming to after losing to Avimander, in the forge room: Briggs asks Felix */
/* whether he has learned his lesson, Obaba scolds, and the party is sent back
 */
/* down the passage. */
void scChampaCliffCavesAskIfBriggsLearned(void) {
	FIELD_STATE *state;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgHaveYouLearned);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 568, 248);
	fdActorSetPos(ACTOR_OBABA, 584, 224);
	fdActorSetPos(ACTOR_AVIMANDER, 600, 240);
	fdActorSetPos(ACTOR_BRIGGS, 584, 264);
	fdActorFaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_LIE_ON_BACK);
	fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(10);
	fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Have you learned your lesson, Felix?" */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "Ah, such a bright lad. Now then, don't you ever try to take my..." */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 20);
	state = Ram_HeapSlots->fieldState;
	state->transitionFrm = 40;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventSetDestination((s16)MainRoutineStateWork.restartScene,
						  (s16)MainRoutineStateWork.restartEntrance);
}

extern u32 ChampaCliffCaves_ChaseToNorthScript[];
extern u32 ChampaCliffCaves_ChaseToEastScript[];
extern u32 ChampaCliffCaves_ChaseToWestScript[];
extern u32 ChampaCliffCaves_ChaseToSouthScript[];

/* After the party beats Avimander, in the forge room: Obaba mourns her */
/* salamander ("My salamander! You... beat my salamander...") and leaves */
/* Briggs to fight for himself, the party tells her he is a pirate, Chaucha */
/* comes to tell why he did it, and Briggs and Chaucha go home */
/* (FLAG_CHAMPA_AVIMANDER_BEATEN). */
void scChampaCliffCavesObabaMournsSalamander(void) {
	const u8 *scriptA, *scriptB, *scriptC, *scriptD;
	ACTOR *leader;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgMySalamanderYou);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 568, 256);
	fdActorSetPos(ACTOR_AVIMANDER, 592, 240);
	fdActorSetAnimationById(ACTOR_AVIMANDER, 3);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_EAST,
							 0);
	fdCameraMoveTo(656, CAMERA_KEEP, 248, 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(10);
	/* "My salamander! You... beat my salamander..." */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_AVIMANDER, 2);
	fdEventWait(5);
	scChampaCliffCavesStartFlash(ACTOR_AVIMANDER, 1);
	fdActorSetSpeed(ACTOR_OBABA, 1);
	fdActorWalkByAndWait(ACTOR_OBABA, -12, -18);
	fdActorWalkByAndWait(ACTOR_OBABA, -10, -14);
	fdActorWalkByAndWait(ACTOR_OBABA, -18, -12);
	fdActorWalkToAndWait(ACTOR_OBABA, 664, 200);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	fdEventWait(10);
	/* "I thought he was unbeatable... How could this be?" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorWalkByAndWait(ACTOR_OBABA, -16, 0);
	fdActorWalkByAndWait(ACTOR_OBABA, -16, 8);
	fdActorWalkByAndWait(ACTOR_OBABA, -16, 16);
	fdActorWalkByAndWait(ACTOR_OBABA, -8, 16);
	fdActorWalkToAndWait(ACTOR_OBABA, 608, 248);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
	fdEventWait(5);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "How did you do that? What... What are you?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorSetSpeed(ACTOR_BRIGGS, 1.7);
	fdActorWalkByAndWait(ACTOR_BRIGGS, -16, 0);
	fdEventWait(5);
	/* "Grandma, what's going on!? I thought you were going to protect me!" */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_EAST);
	fdEventWait(5);
	/* "What do you mean, what am I doing? I've done everything I can!" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorJump(ACTOR_BRIGGS, 6, 15);
	fdActorJump(ACTOR_BRIGGS, 6, 23);
	/* "Grandma!!! He's going to take me away! He's a bad guy! You have to..."
	 */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "I'm sorry, Briggs, but you'll have to fight him yourself." */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(ACTOR_BRIGGS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(38);
	/* "Wha-What!?" */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "You heard me. I'm through pampering you. You have to get out of this..."
	 */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(ACTOR_BRIGGS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(37);
	/* "But...Grandma... Look at him! I mean, look what he can do!" */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_WEST - FACING_NUDGE);
	fdEventWait(25);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_EAST);
	fdEventWait(10);
	/* "I know, dear, but there's nothing I can do to help. You don't want..."
	 */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	fdEventWait(30);
	fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "But...what about poor Eoleo?" */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	scFieldActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "*Sigh* All right, all right." */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 0);
	/* "Um. Excuse me... There seems to be a small misunderstanding." */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST - FACING_NUDGE);
	fdEventWait(10);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	scFieldActorLaunchFromLeader(ACTOR_PIERS, 0, -16, FACING_EAST);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_EAST);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, -16, -16, FACING_EAST);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 16, -16, FACING_EAST);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdEventWait(10);
	fdCameraMoveTo(640, CAMERA_KEEP, 248, 1);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	/* "Yeah! I don't like standing here listening to a pirate call us the..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0, 5);
	fdActorStartRepeatedMotion(ACTOR_OBABA, 3);
	fdActorSetAttachedEffect(ACTOR_OBABA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(ACTOR_BRIGGS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	/* "Pirate! My Briggs?" */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdEventWait(10);
	/* "He stole a ship from Alhafra and escaped from their jail! A ship we..."
	 */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_EAST);
	fdEventWait(5);
	/* "Briggs, is this true?" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdEffectSetAttached(ACTOR_BRIGGS,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(35);
	/* "Aw... But, Grandma, they..." */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "We should tell you, we're not here to take Briggs back to Alhafra." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	fdEventWait(5);
	/* "So you've decided you don't want to run errands for the mayor of..."
	 * Yes: */
	/* "Good. That greedy bag of hot air can do his..." No: "Felix, I know you
	 */
	/* can't mean that. We didn't..." */
	fdMessageShowAtActor(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdEventWait(20);
		fdActorFaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_SOUTHEAST);
		fdEventWait(5);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
		fdEventWait(10);
		/* "Good. That greedy bag of hot air can do his own dirty work, for
		 * all..." */
		fdEventShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0, 5);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(35);
		fdActorFaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_SOUTHEAST);
		fdEventWait(5);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
		FieldStateWork->msg++;
		/* "Felix, I know you can't mean that. We didn't come to Champa for
		 * that..." */
		fdEventShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0, 5);
	}
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_EAST);
	/* "Briggs... Piracy? Why?" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetSpeed(ACTOR_OBABA, 2.8);
	fdActorSetSpeed(ACTOR_BRIGGS, 3.4);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(ACTOR_BRIGGS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scriptA = ChampaCliffCaves_ChaseToNorthScript;
	fdActorSetMovementTypeById(ACTOR_OBABA, scriptA);
	fdEventWait(20);
	fdActorWalkToById(ACTOR_BRIGGS, 664, 290);
	fdActorWaitForScriptEndById(ACTOR_OBABA);
	scriptB = ChampaCliffCaves_ChaseToEastScript;
	fdActorSetMovementTypeById(ACTOR_OBABA, scriptB);
	fdActorWaitForMoveById(ACTOR_BRIGGS);
	scriptC = ChampaCliffCaves_ChaseToWestScript;
	fdActorSetMovementTypeById(ACTOR_BRIGGS, scriptC);
	fdActorWaitForScriptEndById(ACTOR_OBABA);
	scriptD = ChampaCliffCaves_ChaseToSouthScript;
	fdActorSetMovementTypeById(ACTOR_OBABA, scriptD);
	fdActorWaitForScriptEndById(ACTOR_BRIGGS);
	fdActorSetMovementTypeById(ACTOR_BRIGGS, scriptA);
	fdActorWaitForScriptEndById(ACTOR_OBABA);
	fdActorFaceDirection(ACTOR_OBABA, FACING_NORTH_NORTHWEST, 0);
	fdActorWaitForScriptEndById(ACTOR_BRIGGS);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST, 0);
	/* "Grandma, it's not what you think! These guys, they..." */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "Briggs, you just hush up!!! ...It all makes sense now. The gems, the..."
	 */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "What do you mean? What makes sense?" */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "They're telling the truth! And you, Briggs! You've been lying to me..."
	 */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(ACTOR_BRIGGS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetMovementTypeById(ACTOR_OBABA, scriptC);
	fdEventWait(20);
	fdActorWaitForMoveById(ACTOR_BRIGGS);
	fdActorSetMovementTypeById(ACTOR_BRIGGS, scriptB);
	fdActorWaitForScriptEndById(ACTOR_OBABA);
	fdActorSetMovementTypeById(ACTOR_OBABA, scriptA);
	fdActorWaitForScriptEndById(ACTOR_BRIGGS);
	fdActorSetMovementTypeById(ACTOR_BRIGGS, scriptD);
	fdActorWaitForScriptEndById(ACTOR_OBABA);
	fdActorFaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorWaitForScriptEndById(ACTOR_BRIGGS);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
	/* "You're going to take their word over your own grandson's?" */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "You got that right! I should have known better than to believe you..."
	 */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	/* "He only did it for Champa!" */
	fdEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorSetSprPriority(ACTOR_FELIX, 3);
	fdActorSetSprPriority(ACTOR_JENNA, 3);
	fdActorSetSprPriority(ACTOR_SHEBA, 3);
	fdActorSetSprPriority(ACTOR_PIERS, 3);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
	fdEventWait(10);
	fdCameraMoveTo(568, CAMERA_KEEP, 248, 1);
	fdCameraWaitForArrival();
	fdEventWait(10);
	fdActorSetSprPriority(ACTOR_CHAUCHA, 2);
	fdActorGetById(ACTOR_CHAUCHA)->facing = FACING_SOUTH_SOUTHEAST;
	fdActorGetById(ACTOR_CHAUCHA)->motionFlg = 0;
	fdActorSetPos(ACTOR_CHAUCHA, 520, 200);
	fdActorSetSpeed(ACTOR_CHAUCHA, 1);
	fdActorWalkByAndWaitById(ACTOR_CHAUCHA, 0, 16);
	fdActorFaceDirectionById(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHEAST, 0);
	/* "Chaucha!" */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorWatchAndSetScript(ACTOR_FELIX, ACTOR_CHAUCHA);
	fdActorWatchAndSetScript(ACTOR_PIERS, ACTOR_CHAUCHA);
	fdActorWatchAndSetScript(ACTOR_JENNA, ACTOR_CHAUCHA);
	fdActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_CHAUCHA);
	fdActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_CHAUCHA);
	fdActorWatchAndSetScript(ACTOR_OBABA, ACTOR_CHAUCHA);
	fdActorWalkByAndWaitById(ACTOR_CHAUCHA, 0, 16);
	fdActorSetSprPriority(ACTOR_CHAUCHA, 3);
	fdActorWalkByAndWaitById(ACTOR_CHAUCHA, 0, 32);
	fdActorWalkByAndWaitById(ACTOR_CHAUCHA, 8, 8);
	fdActorWalkByAndWaitById(ACTOR_CHAUCHA, 64, 0);
	fdActorFaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
	fdCameraMoveTo(600, CAMERA_KEEP, 256, 1);
	fdActorSetSpeed(ACTOR_BRIGGS, 1);
	fdActorWalkByAndWait(ACTOR_BRIGGS, -32, 0);
	fdActorWalkByAndWait(ACTOR_BRIGGS, -16, -16);
	fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdEventWait(10);
	/* "You came!" */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
	fdEventWait(10);
	/* "I heard Felix had shown up looking for you. I was worried..." */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_OBABA, 2);
	fdEventWait(10);
	fdActorSetSpeed(ACTOR_OBABA, 1);
	fdActorWalkByAndWait(ACTOR_OBABA, -16, 0);
	fdActorWalkByAndWait(ACTOR_OBABA, -16, 8);
	fdActorWalkByAndWait(ACTOR_OBABA, -16, 16);
	fdActorWalkByAndWait(ACTOR_OBABA, -8, 8);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	/* "What did you mean, Briggs did it for Champa?" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_NORTH_NORTHEAST);
	fdEventWait(15);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "All of that food... Stolen goods?" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
	/* "No, we paid for everthing we brought back with us." */
	fdEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "But you stole a boat to do it, didn't you?" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
	/* "Grandma, we found those jewels ourselves, and we paid Alhafra well..."
	 */
	/* Yes: "Your story has the ring of truth to it." No: "So he's lying again?"
	 */
	fdMessageShowAtActor(ACTOR_BRIGGS, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdEventWait(20);
		fdActorRunRepeatedMotion(ACTOR_OBABA, 2);
		fdEventWait(5);
		/* "Your story has the ring of truth to it." */
		fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorFaceDirectionAndRefreshById(ACTOR_BRIGGS, FACING_WEST);
		fdEventWait(10);
		fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
		/* "Thank you, Felix. Your honesty speaks well of you." */
		fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		FieldStateWork->msg += 2;
	}
	else {
		fdEventWait(35);
		FieldStateWork->msg += 2;
		/* "So he's lying again?" */
		fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorFaceDirectionAndRefreshById(ACTOR_BRIGGS, FACING_WEST);
		fdEventWait(5);
		fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		/* "Dang it, Felix, I'm not lying!" */
		fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		fdActorJump(ACTOR_BRIGGS, 6, 15);
		fdActorJump(ACTOR_BRIGGS, 6, 23);
	}
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHEAST);
	fdEventWait(15);
	fdActorShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "We...were pirates, for a short while. Until we found the jewels." */
	fdEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_CHAUCHA, 2);
	fdEventWait(5);
	/* "But if not for our efforts, Champa would have starved away long ago." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0,
									5);
	scFieldActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "But that doesn't excuse resorting to piracy!" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
	fdEventWait(10);
	/* "Hey, it's not like we were doing it for the money! We wanted to save..."
	 */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHEAST);
	fdEventWait(5);
	/* "Those jewels we found are very valuable. We intend to repay each..." */
	fdEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_EAST);
	fdEventWait(5);
	/* "Isn't that true?" */
	fdEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "What's the matter with you!? You said on the boat that we would..." */
	fdEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdEffectSetAttached(ACTOR_BRIGGS,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdEventWait(10);
	/* "Oh yeah... You're right... We really ought to try to make amends for..."
	 */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	fdEventWait(10);
	fdActorFaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_NORTH_NORTHEAST);
	fdEventWait(10);
	/* "That's the truth, Grandmother. Can you find it in your heart to..." */
	fdEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	/* "All right, all right. I'm too old for grudges. But what about Felix?" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
	fdEventWait(10);
	/* "What do you say, Felix? Can you forgive my husband?" */
	fdMessageShowAtActor(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0);
	fdActorFaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdEventWait(15);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		fdEventWait(10);
		/* "I agree, Felix. If each town is repaid what was stolen, I see no..."
		 */
		fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 5);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(35);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		fdEventWait(10);
		FieldStateWork->msg++;
		/* "I don't know, Felix. We didn't come to capture Briggs. Why should
		 * we..." */
		fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 5);
	}
	fdActorFaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_EAST);
	fdEventWait(10);
	/* "Do you hear that? They forgive you, too. That means you can leave..." */
	fdEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(ACTOR_BRIGGS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(50);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdEventWait(10);
	/* "I... I don't know what to say..." */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	scFieldActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "You know, you look a little strange..." */
	fdEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST);
	fdEventWait(10);
	/* "Oh... Oh, really?" */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 15);
	fdActorShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "You haven't caught a cold, have you? Do you have a fever?" */
	fdEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 35);
	fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
	fdEventWait(5);
	/* "What are you talking about?" */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
	/* "No, I'm positive... You look flush, too... You'd better lie down..." */
	fdEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
	fdEventWait(10);
	/* "She...worries about me." */
	fdEventShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	/* "Hurry up and get into bed so you can get some rest!" */
	fdEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorWalkByAndWait(ACTOR_CHAUCHA, -8, 0);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
	fdActorWalkByAndWait(ACTOR_CHAUCHA, -8, 0);
	fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorWalkByAndWait(ACTOR_CHAUCHA, -56, 0);
	fdActorFaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
	scFieldActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_EAST);
	fdEventWait(10);
	/* "Oh, for goodness' sake! You don't want to give them your cold, do..." */
	fdEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	fdEventWait(5);
	fdActorJump(ACTOR_BRIGGS, 6, 20);
	fdActorWatchAndSetScript(ACTOR_FELIX, ACTOR_BRIGGS);
	fdActorWatchAndSetScript(ACTOR_PIERS, ACTOR_BRIGGS);
	fdActorWatchAndSetScript(ACTOR_JENNA, ACTOR_BRIGGS);
	fdActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_BRIGGS);
	fdActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_BRIGGS);
	fdActorSetSpeed(ACTOR_BRIGGS, 2);
	fdActorWalkByAndWaitById(ACTOR_BRIGGS, -80, 0);
	fdEventWait(10);
	fdActorSetSprPriority(ACTOR_CHAUCHA, 3);
	fdActorSetSprPriority(ACTOR_BRIGGS, 3);
	fdActorSetSpeed(ACTOR_BRIGGS, 1);
	fdActorWalkBy(ACTOR_CHAUCHA, 0, -64);
	fdActorWalkByAndWait(ACTOR_BRIGGS, -16, 0);
	fdActorWalkByById(ACTOR_BRIGGS, 0, -64);
	fdEventWait(15);
	fdActorSetSprPriority(ACTOR_CHAUCHA, 2);
	fdActorSetSprPriority(ACTOR_BRIGGS, 2);
	fdActorWaitForMoveById(ACTOR_CHAUCHA);
	fdActorSetPos(ACTOR_CHAUCHA, 0, 0);
	fdActorWaitForMoveById(ACTOR_BRIGGS);
	fdActorSetPos(ACTOR_BRIGGS, 0, 0);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTHWEST, 0);
	fdEventWait(10);
	fdEventWait(60);
	fdActorWalkByAndWaitById(ACTOR_OBABA, 0, 16);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
	/* "So...what brings you to Champa?" */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_NOD);
	fdEventWait(10);
	/* "I mean, you didn't come here to catch Briggs, from what I gathered..."
	 */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	fdEventWait(40);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "Ah, well... To be young and foolish again. If you ever need to..." */
	fdEventShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetSpeed(ACTOR_BRIGGS, 0.7);
	fdActorSetMovementTypeById(ACTOR_OBABA,
							   ChampaCliffCaves_ChaseToNorthScript);
	fdEventWait(30);
	fdActorStop(ACTOR_OBABA);
	fdActorWalkToAndWait(ACTOR_OBABA, 648, 188);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	fdEventWait(20);
	fdActorSetSprPriority(ACTOR_FELIX, 2);
	fdActorGetById(ACTOR_FELIX)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(ACTOR_JENNA, leader->x.part[0].integer,
								  leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(ACTOR_SHEBA, leader->x.part[0].integer,
								  leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_SHEBA);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdActorSetSpeed(ACTOR_PIERS, 1.2);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(ACTOR_PIERS, leader->x.part[0].integer,
								  leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_PIERS);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(ACTOR_KRADEN, leader->x.part[0].integer,
								  leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	syGameFlagSet(FLAG_CHAMPA_AVIMANDER_BEATEN);
	fdEventEnd();
}

/* scChampaCliffCavesStartFlash's explosion: its tiles, decompressed into the */
/* heap, and the sprite that shows them. */
typedef struct FLASH_STATE {
	u8 tiles[1024];
	EFFECT_SPR spr;
} FLASH_STATE;

/* The explosion's sprite palette, and the tiles of each of its 16 by 16 */
/* pictures. */
enum { FLASH_PALETTE = 15, FLASH_PICTURE_TILES = 16 * 16 / 64 };

/* Someone or something vanishes in a burst: it spins (`mode` 0) or not, is */
/* hidden, and the burst plays over it, twice the size in mode 1; then it is */
/* taken off the map. */
void scChampaCliffCavesStartFlash(s32 id, s32 mode) {
	FLASH_STATE *state = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(FLASH_STATE));
	ACTOR *actor = fdActorGetById(id);
	s32 slot;
	s32 tile;
	EFFECT_SPR *sprite;
	FIXED position[3];
	s32 frame;
	FIXED z;

	if(mode == 0) {
		for(frame = 0; frame < 16; frame++) {
			s32 angle = actor->facing;
			angle += DEGtoANG(45);
			actor->facing = angle;
			sySynch(1);
		}
	}
	sySynch(2);
	actor->scaleX = 0;
	fdActorSetSprFlags(actor, 0);
	syDMAXCopy(Field_ExplosionPalette,
			   (void *)&OBJ_PLTT_COLOR(FLASH_PALETTE, 0),
			   DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	syDecompressTaggedLz(
		syResourceGetTableEntry(RESOURCE_FIELD_EFFECT_EXPLOSION), state);
	slot = syVramCacheFindFreeEntry();
	tile = mnVramLoad(slot, sizeof(state->tiles), state);
	sprite = &state->spr;
	syFieldEffectSprInit(sprite, 8, 16, OBJ_SIZE_16X16, tile);
	sprite->depth = OAM_DEPTH_WINDOW;
	sprite->oam.pri = 1;
	sprite->oam.fullCol = 0;
	sprite->oam.pal = FLASH_PALETTE;
	position[0] = actor->x.fixed;
	position[1] = actor->y.fixed;
	position[2] = actor->z.fixed;
	fdCameraWorldToScr(position);
	sprite->x = position[0];
	sprite->y = z = position[2];
	if(mode == 1) {
		sprite->scaleX = FIXED_ONE * 2;
		sprite->scaleY = FIXED_ONE * 2;
		sprite->y = z + toFIXED(10);
	}
	sySndPlay(SE_CRASH);
	for(frame = 0; frame <= 71; frame++) {
		sprite = &state->spr;
		switch(frame) {
		case 6:
			sprite->oam.idx = tile + 1 * FLASH_PICTURE_TILES;
			break;
		case 12:
			sprite->oam.idx = tile + 2 * FLASH_PICTURE_TILES;
			break;
		case 22:
			sprite->oam.idx = tile + 3 * FLASH_PICTURE_TILES;
			break;
		case 32:
			sprite->oam.idx = tile + 4 * FLASH_PICTURE_TILES;
			break;
		case 42:
			sprite->oam.idx = tile + 5 * FLASH_PICTURE_TILES;
			break;
		case 52:
			sprite->oam.idx = tile + 6 * FLASH_PICTURE_TILES;
			break;
		case 62:
			sprite->oam.idx = tile + 7 * FLASH_PICTURE_TILES;
			break;
		}
		fdEffectSprDraw(sprite);
		sySynch(1);
	}
	fdActorSetPos(id, 0, 0);
	syVramCacheFreeEntry(slot);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

/* Loads the forge's glow, a glowing disc, into the fire's state, its picture */
/* moved onto the colours the fire cycles, and sets its sprite at no size. */
void scChampaCliffCavesLoadGlow(void) {
	CHAMPA_CLIFF_CAVES_STATE *state = Ram_HeapSlots->menuState;
	EFFECT_SPR *sprite;
	s32 i;
	u8 *tile;
	s32 entry;
	s32 vram;

	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_GLOW_DISC), state);
	for(i = 0, tile = state->tiles; i < (s32)sizeof(state->tiles);
		tile++, i++) {
		if((*tile & PIXEL_PALETTE_ROW_MASK) ==
		   GLOW_PICTURE_ROW * PALETTE_COLORS)
			*tile = (*tile & PIXEL_ROW_COLOR_MASK) +
					GLOW_CYCLED_ROW * PALETTE_COLORS;
	}
	entry = syVramCacheFindFreeEntry();
	vram = syVramCacheLoad(entry, 64 * 64, state);
	state->glowEntry = entry;
	state->glowVram = vram;
	sprite = &state->glow;
	syFieldEffectSprInit(sprite, 31, 31, OBJ_SIZE_64X64, vram);
	sprite->oam.fullCol = 1;
	sprite->oam.pal = 0;
	sprite->oam.pri = 2;
	sprite->depth = 1;
	state->glowScale = 0;
	state->glowTrailingScale = 0;
}

/* Loads the forge's column of fire into the fire's state as */
/* scChampaCliffCavesLoadGlow loads the glow. */
void scChampaCliffCavesLoadColumn(void) {
	CHAMPA_CLIFF_CAVES_STATE *state = Ram_HeapSlots->menuState;
	EFFECT_SPR *sprite;
	s32 i;
	u8 *tile;
	s32 entry;
	s32 vram;

	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_FIRE_COLUMN), state);
	for(i = 0, tile = state->tiles; i < (s32)sizeof(state->tiles);
		i++, tile++) {
		if((*tile & PIXEL_PALETTE_ROW_MASK) ==
		   GLOW_PICTURE_ROW * PALETTE_COLORS)
			*tile = (*tile & PIXEL_ROW_COLOR_MASK) +
					GLOW_CYCLED_ROW * PALETTE_COLORS;
	}
	entry = syVramCacheFindFreeEntry();
	vram = syVramCacheLoad(entry, 32 * 64, state);
	state->columnEntry = entry;
	state->columnVram = vram;
	sprite = &state->column;
	syFieldEffectSprInit(sprite, 15, 47, OBJ_SIZE_32X64, vram);
	sprite->oam.fullCol = 1;
	sprite->oam.pal = 1;
	sprite->oam.pri = 1;
	sprite->depth = 1;
	state->columnScale = 0;
	state->columnTrailingScale = 0;
}

/* The height the glow and the column are drawn at: 31/40 of the width they */
/* had the frame before. */
#define FIRE_HEIGHT_SCALE(widthScale) ((widthScale) * 31 / 40)

/* Each frame while Obaba's forge burns, its fire shimmers and pulses: the */
/* fire's 32 colours ripple through waves of green and blue, a 32nd of a turn */
/* apart (the first sixteen onto colours 111 down to 96, the rest onto 239 */
/* down to 224); the glow's and the column's sprites are placed at the fire's */
/* spot at their sizes; and each one's height is set to trail its width */
/* (FIRE_HEIGHT_SCALE). */
void scChampaCliffCavesDrawForgeFire(void) {
	CHAMPA_CLIFF_CAVES_STATE *state = Ram_HeapSlots->menuState;
	s32 position[3];
	EFFECT_SPR *sprite;
	s32 i;

	for(i = 0; i < GLOW_COLORS; i++) {
		s32 red = RGB_LEVEL_MAX;
		s32 green = sySin((state->frm + i) * (TRIG_TURN / 32)) * 6;
		s32 blue = sySin((state->frm / 2 + i) * (TRIG_TURN / 32) +
						 TRIG_TURN * 9 / 32) *
				   3;
		s32 color;
		green = FIXEDtoINT(green);
		blue = FIXEDtoINT(blue);
		green += GLOW_GREEN_MIDDLE;
		blue += GLOW_BLUE_MIDDLE;
		if(i < PALETTE_COLORS) {
			color = RGB(red, green, blue);
			((u16 *)OBJ_PLTT)[GLOW_CYCLED_ROW * PALETTE_COLORS +
							  PALETTE_COLORS - 1 - i] = color;
		}
		else {
			color = RGB(red, green, blue);
			((u16 *)OBJ_PLTT)[GLOW_PICTURE_ROW * PALETTE_COLORS +
							  PALETTE_COLORS - 1 - i] = color;
		}
	}
	position[0] = state->pos[0];
	position[1] = state->pos[1];
	position[2] = state->pos[2];
	sprite = &state->glow;
	fdCameraWorldToScr(position);
	sprite->x = position[0];
	sprite->y = position[2];
	sprite->scaleX = state->glowScale;
	sprite->scaleY = state->glowTrailingScale;
	fdEffectSprDraw(sprite);
	position[0] = state->pos[0];
	position[1] = state->pos[1];
	position[2] = state->pos[2];
	sprite = &state->column;
	fdCameraWorldToScr(position);
	sprite->x = position[0];
	sprite->y = position[2];
	sprite->scaleX = state->columnScale;
	sprite->scaleY = state->columnTrailingScale;
	fdEffectSprDraw(sprite);
	state->glowTrailingScale = FIRE_HEIGHT_SCALE(state->glowScale);
	state->columnTrailingScale = FIRE_HEIGHT_SCALE(state->columnScale);
	state->frm++;
}

/* The forge's fire at a spot: the glow grows, holds, and the column rises */
/* through it, while the room shakes. */
void scChampaCliffCavesStartForgeFire(const s32 *position) {
	CHAMPA_CLIFF_CAVES_STATE *champaCliffCavesState =
		syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(CHAMPA_CLIFF_CAVES_STATE));
	s32 frames;
	s32 state;

	champaCliffCavesState->pos[0] = position[0];
	champaCliffCavesState->pos[1] = position[1];
	champaCliffCavesState->pos[2] = position[2];
	scChampaCliffCavesLoadGlow();
	scChampaCliffCavesLoadColumn();
	champaCliffCavesState->frm = 0;
	frames = 0;
	sySetEvent(scChampaCliffCavesDrawForgeFire, TASK_PASS_FRAME);
	fdMapGridSetShake(4, 4, 1);
	state = 0;
	do {
		switch(state) {
		case 0:
			if((champaCliffCavesState->glowScale += FIXED_ONE * 3 / 128) >=
			   FIXED_ONE * 13 / 10) {
				champaCliffCavesState->glowScale = FIXED_ONE * 13 / 10;
				state = 1;
				frames = -1;
			}
			break;
		case 1:
			if(frames == 10) {
				state = 2;
				frames = -1;
			}
			break;
		case 2:
			if((champaCliffCavesState->columnScale += FIXED_ONE / 64) >=
			   FIXED_ONE * 2 / 5) {
				champaCliffCavesState->columnScale = FIXED_ONE * 2 / 5;
				state = 3;
				frames = -1;
			}
			break;
		case 3:
			if((champaCliffCavesState->columnScale += FIXED_ONE * 3 / 16) >=
			   FIXED_ONE * 13 / 10) {
				champaCliffCavesState->columnScale = FIXED_ONE * 13 / 10;
				state = GLOW_DONE;
			}
			break;
		}
		syWait(1);
		frames++;
	} while(state != GLOW_DONE);
}

/* Unused: puts out the forge's fire: the column and then the glow shrink */
/* away and the fire's state is released. */
UNUSED void scChampaCliffCavesPutOutForgeFire(void) {
	CHAMPA_CLIFF_CAVES_STATE *champaCliffCavesState = Ram_HeapSlots->menuState;
	FIXED position[3];
	s32 state;
	s32 frames;

	position[0] = champaCliffCavesState->pos[0];
	position[1] = champaCliffCavesState->pos[1] - toFIXED(32);
	state = 0;
	frames = 0;
	position[2] = champaCliffCavesState->pos[2];
	do {
		switch(state) {
		case 0:
			if((champaCliffCavesState->columnScale -= FIXED_ONE / 16) < 0) {
				champaCliffCavesState->columnScale = 0;
				frames = -1;
				state = 1;
			}
			break;
		case 1:
			if((champaCliffCavesState->glowScale -= FIXED_ONE / 32) < 0) {
				champaCliffCavesState->glowScale = 0;
				state = 2;
				frames = -1;
			}
			break;
		case 2:
			if(frames == 1)
				state = GLOW_DONE;
			break;
		}
		sySynch(1);
		frames++;
	} while(state != GLOW_DONE);
	syCloseEvent(scChampaCliffCavesDrawForgeFire);
	fdMapGridSetShake(0, 0, 0);
	syVramCacheFreeEntry(champaCliffCavesState->glowEntry);
	syVramCacheFreeEntry(champaCliffCavesState->columnEntry);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

/* The forge lights at a spot: the glow alone grows to full size. */
void scChampaCliffCavesLightForge(const VECTOR3 *origin) {
	CHAMPA_CLIFF_CAVES_STATE *champaCliffCavesState =
		syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(CHAMPA_CLIFF_CAVES_STATE));
	s32 state;

	champaCliffCavesState->pos[0] = origin->x;
	champaCliffCavesState->pos[1] = origin->y;
	champaCliffCavesState->pos[2] = origin->z;
	scChampaCliffCavesLoadGlow();
	champaCliffCavesState->columnScale = 0;
	champaCliffCavesState->columnTrailingScale = 0;
	champaCliffCavesState->frm = 0;
	sySetEvent(scChampaCliffCavesDrawForgeFire, TASK_PASS_FRAME);
	sySndPlay(SE_DJINNI_SET);
	state = 0;
	do {
		if(state == 0) {
			champaCliffCavesState->glowScale += FIXED_ONE / 16;
			if(champaCliffCavesState->glowScale >= FIXED_ONE * 13 / 10) {
				champaCliffCavesState->glowScale = FIXED_ONE * 13 / 10;
				state = GLOW_DONE;
			}
		}
		sySynch(1);
	} while(state != GLOW_DONE);
}

/* The column of fire rises through the glow already burning. */
void scChampaCliffCavesRaiseColumn(void) {
	CHAMPA_CLIFF_CAVES_STATE *champaCliffCavesState =
		syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(CHAMPA_CLIFF_CAVES_STATE));
	s32 state;

	scChampaCliffCavesLoadColumn();
	sySetEvent(scChampaCliffCavesDrawForgeFire, TASK_PASS_FRAME);
	sySndPlay(SE_CLATTER);
	state = 0;
	do {
		switch(state) {
		case 0:
			champaCliffCavesState->columnScale += FIXED_ONE / 16;
			if(champaCliffCavesState->columnScale >= FIXED_ONE * 2 / 5) {
				champaCliffCavesState->columnScale = FIXED_ONE * 2 / 5;
				state = 1;
			}
			break;
		case 1:
			champaCliffCavesState->columnScale += FIXED_ONE * 3 / 16;
			if(champaCliffCavesState->columnScale >= FIXED_ONE * 13 / 10) {
				champaCliffCavesState->columnScale = FIXED_ONE * 13 / 10;
				state = GLOW_DONE;
			}
			break;
		}
		sySynch(1);
	} while(state != GLOW_DONE);
}

/* The forge's last flare: the column flares to twice its size, then it and the
 */
/* glow die away and the fire's state is released. */
void scChampaCliffCavesFlareAndDieDown(void) {
	CHAMPA_CLIFF_CAVES_STATE *champaCliffCavesState = Ram_HeapSlots->menuState;
	s32 state = 0;
	s32 time = 0;

	do {
		switch(state) {
		case 0:
			if((champaCliffCavesState->columnScale += FIXED_ONE / 16) >=
			   FIXED_ONE * 2) {
				champaCliffCavesState->columnScale = FIXED_ONE * 2;
				state = 1;
				time = -1;
			}
			break;
		case 1:
			if((champaCliffCavesState->columnScale -= FIXED_ONE / 16) <
			   FIXED_ONE * 13 / 10) {
				champaCliffCavesState->columnScale = FIXED_ONE * 13 / 10;
				state = 2;
				time = -1;
			}
			/* fall through */
		case 2:
			if((champaCliffCavesState->columnScale -= FIXED_ONE / 16) < 0)
				champaCliffCavesState->columnScale = 0;
			if((champaCliffCavesState->glowScale -= FIXED_ONE / 16) < 0) {
				champaCliffCavesState->glowScale = 0;
				state++;
				time = -1;
			}
			break;
		case 3:
			if(time == 1)
				state = GLOW_DONE;
			break;
		}
		sySynch(1);
		time++;
	} while(state != GLOW_DONE);
	syCloseEvent(scChampaCliffCavesDrawForgeFire);
	syVramCacheFreeEntry(champaCliffCavesState->glowEntry);
	syVramCacheFreeEntry(champaCliffCavesState->columnEntry);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

/* The three prongs Obaba throws into her forge, in turn. */
TRIDENT_PRONGS ChampaCliffCaves_Prongs = {
	{ITEM_RIGHT_PRONG, ITEM_LEFT_PRONG, ITEM_CENTER_PRONG}};

#undef DEFINE_CHAMPA_CLIFF_CAVES_GROW

/* clang-format off */
LATE_DATA SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 SceneExits[] = {
    SCENE_EXITS(ChampaCliffCavesBriggs),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA, Champa, CHAMPA_FROM_CHAMPA_CLIFF_CAVES_BRIGGS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA_CLIFF_CAVES_HOUSES, ChampaCliffCavesHouses, CHAMPA_CLIFF_CAVES_HOUSES_FROM_CHAMPA_CLIFF_CAVES_BRIGGS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, ChampaCliffCavesBriggs, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, ChampaCliffCavesBriggs, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, ChampaCliffCavesBriggs, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, ChampaCliffCavesBriggs, 5, EXIT_FLAG_NONE),
    SCENE_EXITS(ChampaCliffCavesHouses),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA, Champa, CHAMPA_FROM_CHAMPA_CLIFF_CAVES_HOUSES, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, ChampaCliffCavesHouses, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, ChampaCliffCavesHouses, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, ChampaCliffCavesHouses, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, ChampaCliffCavesHouses, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_BRIGGS, ChampaCliffCavesBriggs, CHAMPA_CLIFF_CAVES_BRIGGS_FROM_CHAMPA_CLIFF_CAVES_HOUSES, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_OBABA, ChampaCliffCavesObaba, CHAMPA_CLIFF_CAVES_OBABA_FROM_CHAMPA_CLIFF_CAVES_HOUSES, EXIT_FLAG_NONE),
    SCENE_EXITS(ChampaCliffCavesObaba),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_HOUSES, ChampaCliffCavesHouses, CHAMPA_CLIFF_CAVES_HOUSES_FROM_CHAMPA_CLIFF_CAVES_OBABA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, ChampaCliffCavesObaba, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, ChampaCliffCavesObaba, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(17, ChampaCliffCavesObaba, 18, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(18, ChampaCliffCavesObaba, 17, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA, Champa, CHAMPA_FROM_CHAMPA_CLIFF_CAVES_OBABA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_AVIMANDER, ChampaCliffCavesAvimander, CHAMPA_CLIFF_CAVES_AVIMANDER_FROM_CHAMPA_CLIFF_CAVES_OBABA, EXIT_FLAG_NONE),
    SCENE_EXITS(ChampaCliffCavesAvimander),
    SCENE_EXIT_UNLESS(CHAMPA_CLIFF_CAVES_AVIMANDER_TO_CHAMPA_CLIFF_CAVES_OBABA, ChampaCliffCavesObaba, CHAMPA_CLIFF_CAVES_OBABA_FROM_CHAMPA_CLIFF_CAVES_AVIMANDER, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(22, ChampaCliffCavesAvimander, ENTRANCE_FORGE_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(23, ChampaCliffCavesAvimander, 22, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

LATE_DATA ACTOR_PLACE ChampaCliffCaves_BriggsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 568,
                    FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 216, 0, 224,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE ChampaCliffCaves_HousesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 192, 0, 184, FACING_SOUTH_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216,
                    FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 568, 0, 248, FACING_WEST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 488, 0, 248, FACING_NORTH_NORTHEAST,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE ChampaCliffCaves_ObabaPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OBABA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
                    FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 200, 0, 424, FACING_SOUTH_SOUTHWEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE ChampaCliffCaves_AvimanderPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OBABA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
                    FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 616, 0, 216, FACING_SOUTH_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE ChampaCliffCaves_BriggsPlacementsBriggsHome[] =
    {
        ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 344, 0, 552, FACING_WEST,
                        TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_SQUASH, 344, 0, 568,
                        FACING_WEST, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 504,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH,
                        0),
        {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE ChampaCliffCaves_HousesPlacementsBriggsHome[] =
    {
        ACTOR_PLACEMENT(
            SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312,
            0, 728, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(
            SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328,
            0, 728, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 568, 0, 248, FACING_WEST,
                        TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 488, 0, 248,
                        FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH,
                        0),
        {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE ChampaCliffCaves_ObabaPlacementsBriggsHome[] =
    {
        ACTOR_PLACEMENT(
            SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520,
            0, 392, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(
            SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184,
            0, 424, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
        {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE ChampaCliffCaves_AvimanderPlacementsBriggsHome[] = {
    ACTOR_PLACEMENT(SPRITE_OBABA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 736, 0, 248, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_SQUASH, 216, 0, 464, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 752, 0, 264, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SALAMANDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AVIMANDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHAUCHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE
    ChampaCliffCaves_BriggsPlacementsAfterAvimander[] = {
        ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 568,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH,
                        0),
        ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 216, 0,
                        224, FACING_SOUTH, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 488, 0, 208,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 600, 0, 216,
                        FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_WANDER, 576, 0, 248, FACING_SOUTH,
                        TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 504, 0, 264, FACING_EAST,
                        TALK_FACING_PARTY, 0),
        {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE
    ChampaCliffCaves_HousesPlacementsAfterAvimander[] = {
        ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 192, 0, 184,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 568, 0, 248, FACING_WEST,
                        TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 488, 0, 248, FACING_EAST,
                        TALK_FACING_PARTY, 0),
        {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE
    ChampaCliffCaves_ObabaPlacementsAfterAvimander[] = {
        ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                        560, 0, 704, FACING_SOUTH, TALK_FACING_KEEP, 0),
        ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                        FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(SPRITE_EOLEO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                        480, 0, 704, FACING_EAST, TALK_FACING_PARTY_THEN_WATCH,
                        0),
        ACTOR_PLACEMENT(SPRITE_CHAUCHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                        504, 0, 712, FACING_WEST, TALK_FACING_PARTY_AND_BACK,
                        0),
        {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE ChampaCliffCaves_AvimanderPlacementsAfterAvimander[] = {
    ACTOR_PLACEMENT(SPRITE_OBABA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 188, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 464, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* Obaba's door opening and closing, as MAPGRID_ANIMATION_STEP rows: two steps */
/* of six frames each. */
LATE_DATA s16 ChampaCliffCaves_OpenDoorFrames[] = {
    1, 64, 1, 2, 6, 2, 64, 1, 2, 6, -1,
};
LATE_DATA s16 ChampaCliffCaves_CloseDoorFrames[] = {
    1, 64, 1, 2, 6, 0, 64, 1, 2, 6, -1,
};

/* Obaba's door opening and closing. */
LATE_DATA DOOR_EDIT ChampaCliffCaves_OpenDoor = {
    ChampaCliffCaves_OpenDoorFrames, 11, 88};
LATE_DATA DOOR_EDIT ChampaCliffCaves_CloseDoor = {
    ChampaCliffCaves_CloseDoorFrames, 11, 88};

LATE_DATA TRIGGER ChampaCliffCaves_BriggsTriggers[] = {
    {TRIGGER_EXIT, CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA_CLIFF_CAVES_HOUSES, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA_CLIFF_CAVES_HOUSES},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_WOMAN_MISSING_HER_SWEETHEART, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgArenTYou)},
    {TRIGGER_TALK, ACTOR_WOMAN_SCORNING_HARBOR_SAILORS, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgIThinkThose)},
    {TRIGGER_TALK, ACTOR_SAILOR_WAITING_FOR_BRIGGS, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgWeReGoing)},
    {TRIGGER_TALK, ACTOR_SAILOR_ON_THE_ISLAND_TREASURE, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgThereWerePlenty)},
    {TRIGGER_TALK, ACTOR_SAILOR_WANTING_AMENDS, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgWeReallyWant)},
    {TRIGGER_TALK, ACTOR_SAILOR_WHO_HIT_THE_SWITCH, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgItSAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_MISSING_HER_SWEETHEART,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgThatSureComes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SCORNING_HARBOR_SAILORS,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgAllThoseSailors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WAITING_FOR_BRIGGS,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgSupposeWeWent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_ISLAND_TREASURE,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgBriggsIsJust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WANTING_AMENDS,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIWasAs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WHO_HIT_THE_SWITCH,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgAtLeastWe)},
    {TRIGGER_TALK, ACTOR_SAILOR_CALLING_FELIX_A_BULLY, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgWhyDoYou)},
    {TRIGGER_TALK, ACTOR_SAILOR_WHO_SURRENDERS, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(scChampaCliffCavesSailorSurrenders)},
    {TRIGGER_TALK, ACTOR_WOMAN_IN_HER_DOORWAY, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(scChampaCliffCavesAskToComeIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_CALLING_FELIX_A_BULLY,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgMaybeIfI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WHO_SURRENDERS,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgHeSAfter)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_HER_DOORWAY,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgToughOrNot)},
    {TRIGGER_TALK, ACTOR_WOMAN_MISSING_HER_SWEETHEART, CONDITION_ALWAYS, MSGNO(MsgAllOfOurMen)},
    {TRIGGER_TALK, ACTOR_WOMAN_SCORNING_HARBOR_SAILORS, CONDITION_ALWAYS, MSGNO(MsgTheChampaSailors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_MISSING_HER_SWEETHEART, CONDITION_ALWAYS,
     MSGNO(MsgBriggsIsThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SCORNING_HARBOR_SAILORS, CONDITION_ALWAYS,
     MSGNO(MsgIfYouLive)},
    {TRIGGER_TABLE_END},
};

LATE_DATA TRIGGER ChampaCliffCaves_HousesTriggers[] = {
    {TRIGGER_EXIT, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_BRIGGS, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_BRIGGS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_OBABA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_OBABA},
    {TRIGGER_TALK, ACTOR_MAN_KEEPING_WATCH, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgWhyDidYou)},
    {TRIGGER_TALK, ACTOR_SAILORS_WIFE, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgWeHaveSo)},
    {TRIGGER_TALK, ACTOR_SAILORS_SON, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgBriggsCameDown)},
    {TRIGGER_TALK, ACTOR_SAILORS_DAUGHTER, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgEoleoStillWon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_WATCH,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIJustKnow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_WIFE,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgBriggsSaysHe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_SON,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgBriggsSNever)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_DAUGHTER,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgEoleoIsYounger)},
    {TRIGGER_TALK, ACTOR_SAILOR_RATHER_FIGHTING, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgIDRatherFight)},
    {TRIGGER_TALK, ACTOR_SAILOR_WHO_KNOWS_FELIX, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgYouReAren)},
    {TRIGGER_TALK, ACTOR_SAILORS_SON, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgWhySEveryone)},
    {TRIGGER_TALK, ACTOR_SAILORS_DAUGHTER, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgIfYouSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_RATHER_FIGHTING,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgItWouldBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WHO_KNOWS_FELIX,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgBriggsSaidThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_SON,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgHeSNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_DAUGHTER,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhatSHe)},
    {TRIGGER_TALK, ACTOR_MAN_KEEPING_WATCH, CONDITION_ALWAYS, MSGNO(MsgBeforeHeLeft)},
    {TRIGGER_TALK, ACTOR_SAILORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgMyHusbandWants)},
    {TRIGGER_TALK, ACTOR_SAILORS_SON, CONDITION_ALWAYS, MSGNO(MsgSomedayIWant)},
    {TRIGGER_TALK, ACTOR_SAILORS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMomSHad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_WATCH, CONDITION_ALWAYS,
     MSGNO(MsgBriggsMayBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_WIFE, CONDITION_ALWAYS,
     MSGNO(MsgWhenIMarried)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_SON, CONDITION_ALWAYS,
     MSGNO(MsgHasMomForgotten)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_DAUGHTER, CONDITION_ALWAYS,
     MSGNO(MsgEoleoIsThe)},
    {TRIGGER_TABLE_END},
};

LATE_DATA TRIGGER ChampaCliffCaves_ObabaTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_OBABAS_DOOR,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     EVENT(scChampaCliffCavesEnterDoor)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_HOUSES, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_HOUSES},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_OBABAS_DOOR, CONDITION_ALWAYS,
     EVENT(scChampaCliffCavesObabaAsksForBriggs)},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_AVIMANDER, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_AVIMANDER},
    {TRIGGER_TALK, ACTOR_BRIGGS_IN_BED, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgOnceIM)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_CHAUCHA_BY_THE_BED,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIWonderWhatS)},
    {TRIGGER_TALK, ACTOR_EOLEO, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgBabuBaBubabu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIGGS_IN_BED,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIMCertain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHAUCHA_BY_THE_BED,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgThereIsAbsolutely)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EOLEO,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgOneOfPapa)},
    {TRIGGER_TALK, ACTOR_SAILOR_GUARDING_THE_FAMILIES, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgLeaveTheWomen)},
    {TRIGGER_TALK, ACTOR_SAILOR_SPOILING_FOR_A_FIGHT, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgDidYouReally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_GUARDING_THE_FAMILIES,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhatAreThey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_SPOILING_FOR_A_FIGHT,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhyWouldBriggs)},
    {TRIGGER_TALK, ACTOR_MAN_AT_OBABAS_DOOR, CONDITION_ALWAYS, MSGNO(MsgObabaSNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AT_OBABAS_DOOR, CONDITION_ALWAYS,
     MSGNO(MsgIVeNeverSeen)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_KNOCK_AT_OBABAS_DOOR, FLAG_CHAMPA_AVIMANDER_BEATEN,
     EVENT(scChampaCliffCavesObabaAsksForBriggs)},
    {TRIGGER_TABLE_END},
};

LATE_DATA TRIGGER ChampaCliffCaves_AvimanderTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, CHAMPA_CLIFF_CAVES_AVIMANDER_TO_CHAMPA_CLIFF_CAVES_OBABA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_AVIMANDER_TO_CHAMPA_CLIFF_CAVES_OBABA},
    {TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_STEP, CELL_BEHAVIOR_FORGE_ROOM_WAY_IN, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(scChampaCliffCavesFightAvimander)},
    {TRIGGER_TALK, ACTOR_OBABA, CONDITION_ALWAYS,
     EVENT(scChampaCliffCavesObabaOffersHelp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OBABA, FLAG_OBABA_SAW_FIRST_PRONG,
     MSGNO(MsgNowIVe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OBABA, FLAG_OBABA_SAW_SECOND_PRONG,
     MSGNO(MsgAllOfMy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OBABA, FLAG_TRIDENT_FORGED,
     MSGNO(MsgSoonILl)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OBABA, CONDITION_ALWAYS,
     MSGNO(MsgAllOfMyTraining)},
    {TRIGGER_TALK, ACTOR_HELD_ITEM, CONDITION_ALWAYS, EVENT(scChampaCliffCavesPickUpTrident)},
    {TRIGGER_TALK, ACTOR_FORGE_ROOM_SAILOR, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgYouGuysDon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORGE_ROOM_SAILOR,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgMustBeIncredibly)},
    {TRIGGER_TALK, ACTOR_FORGE_ROOM_SAILOR, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(scChampaCliffCavesSailorTurnsAway)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORGE_ROOM_SAILOR,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgBriggsIsPretty)},
    {TRIGGER_TALK, ACTOR_FORGE_ROOM_SAILOR, CONDITION_ALWAYS, MSGNO(MsgThatPitIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORGE_ROOM_SAILOR, CONDITION_ALWAYS,
     MSGNO(MsgIWonderHow)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(scChampaCliffCavesRevealPath)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(scChampaCliffCavesHidePath)},
    {TRIGGER_TABLE_END},
};

/* Obaba chasing Briggs round her forge: the four quarters of the way round, */
/* each walked to its end. */
LATE_DATA u32 ChampaCliffCaves_ChaseToNorthScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(616, 0, 220),
    SC_SYNC,
    SC_MOVE(632, 0, 206),
    SC_SYNC,
    SC_MOVE(648, 0, 200),
    SC_SYNC,
    SC_MOVE(664, 0, 198),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 ChampaCliffCaves_ChaseToEastScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(680, 0, 200),
    SC_SYNC,
    SC_MOVE(694, 0, 206),
    SC_SYNC,
    SC_MOVE(710, 0, 216),
    SC_SYNC,
    SC_MOVE(720, 0, 232),
    SC_SYNC,
    SC_MOVE(720, 0, 246),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 ChampaCliffCaves_ChaseToSouthScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(710, 0, 264),
    SC_SYNC,
    SC_MOVE(698, 0, 278),
    SC_SYNC,
    SC_MOVE(664, 0, 290),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 ChampaCliffCaves_ChaseToWestScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(632, 0, 280),
    SC_SYNC,
    SC_MOVE(616, 0, 266),
    SC_SYNC,
    SC_MOVE(608, 0, 248),
    SC_SYNC,
    SC_MOVE(608, 0, 230),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};
/* clang-format on */
