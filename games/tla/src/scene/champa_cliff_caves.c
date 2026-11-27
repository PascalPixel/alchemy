/* The caves in Champa's cliff, where the sailors and their families live, in
   four scenes of this overlay climbing the cliff level by level: the lowest
   caves, entered from the village by the mouth Briggs's sailors watch
   (SceneId_ChampaCliffCavesBriggs); the homes above them
   (ChampaCliffCavesHouses); the level with Obaba's door and Briggs's home
   (ChampaCliffCavesObaba); and at the top Obaba's forge room
   (ChampaCliffCavesAvimander). Before Briggs is home, knocking at Obaba's door
   only brings her out to ask after her grandson
   (scChampaCliffCavesObabaAsksForBriggs). Once he has fled up here from Felix,
   walking into the forge room finds Briggs with her, and she throws her
   salamander into the forge to come out as Avimander
   (scChampaCliffCavesFightAvimander). Beaten, she hears what Briggs has done
   (scChampaCliffCavesObabaMournsSalamander), and afterwards forges the three
   prongs the party brings her into the Trident of Ankohl
   (scChampaCliffCavesObabaForgesTrident), which Reveal's path across the pit
   reaches (scChampaCliffCavesRevealPath). */
#include "scene/script_layer.h"
#include "field/field_mode.h"
#include "field/script_inline.h"
#include "item_ids.h"
#include "field/actor_script.h"

/* Set once Avimander is beaten and Obaba has heard the truth about Briggs. */
#define FLAG_CHAMPA_AVIMANDER_BEATEN	 0x97e
/* The party hands Obaba the prongs one by one: set as she sees the first, the
   second, and as she forges the three into the Trident. */
#define FLAG_OBABA_SAW_FIRST_PRONG		 0x975
#define FLAG_OBABA_SAW_SECOND_PRONG		 0x976
#define FLAG_TRIDENT_FORGED				 0x977
/* The Trident has been picked up from Obaba's forge. */
#define FLAG_TRIDENT_TAKEN				 0x978
/* Set as Briggs first stops the party at Avimander's room ("Hold it right
   there!"); on a later visit he greets them with "Back for more?". */
#define FLAG_CHAMPA_BRIGGS_STOPPED_PARTY 0x974

/* Where the party comes back to the Avimander's room after losing to it
   (fdScriptSetDefeatDestination) or beating it, the second numbered 99 as The
   Broken Seal's Altin Peak numbers its own; the party then stands in the forge
   room by its way in from the passage west of it (ENTRANCE_FORGE_ROOM, which
   exit 22 leads to). */
enum {
	ENTRANCE_FORGE_ROOM = 23,
	ENTRANCE_AFTER_DEFEAT = 98,
	ENTRANCE_AFTER_AVIMANDER_BATTLE = 99
};

/* The map metatiles the caves' step and examine cues answer: Obaba's door,
   whose metatile takes the number of the exit into her home, the knock at it,
   and the way into her forge room from the passage west of it. */
typedef enum CHAMPA_CLIFF_CAVES_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_OBABAS_DOOR = 15,
	METATILE_BEHAVIOR_FORGE_ROOM_WAY_IN = 30,
	METATILE_BEHAVIOR_KNOCK_AT_OBABAS_DOOR = 40
} CHAMPA_CLIFF_CAVES_METATILE_BEHAVIOR;

/* Avimander's battle: slot 5 of ENCOUNTER_ZONE_SERPENT, between the Serpent's
   and Poseidon's. */
enum { ENCOUNTER_SLOT_AVIMANDER = 5 };

/* The five colours of Obaba's forge pit, from colour 2 of a background palette:
   dark blue as the room shows them (palette 7), and the pale ones (palette 6)
   and the fiery yellow to orange ones (palette 13) the map keeps for the pit to
   fade to. */
enum {
	FORGE_PALETTE = 7,
	FORGE_PALE_PALETTE = 6,
	FORGE_FIERY_PALETTE = 13,
	FORGE_FIRST_COLOR = 2,
	FORGE_COLORS = 5
};

/* The metatile column of the path Reveal shows across Obaba's forge pit, to the
   Trident floating over it, and the four metatiles from its north rim it
   covers. */
enum {
	FORGE_PATH_METATILE_X = 41,
	FORGE_PATH_METATILE_Z = 13,
	FORGE_PATH_METATILES = 4
};

/* Obaba's forge room (SceneId_ChampaCliffCavesAvimander), numbering
   ChampaCliffCavesAvimanderPlacementsBriggsHome; Obaba is actor 8 at her door
   as well (SceneId_ChampaCliffCavesObaba). */
typedef enum CHAMPA_CLIFF_CAVES_OBABAS_ROOM_ACTOR_ID {
	ACTOR_OBABA = ACTOR_FIRST_PLACED,
	/* The sailor across the room from the forge: "That pit is Obaba's forge."
	   Once Briggs is back he turns his back on the party
	   (scChampaCliffCavesSailorTurnsAway). */
	ACTOR_FORGE_ROOM_SAILOR,
	/* The prong or the Trident shown as an item icon in Obaba's hands, and the
	   forged Trident floating over the forge. */
	ACTOR_HELD_ITEM,
	ACTOR_BRIGGS,
	/* Obaba's salamander, which she throws into her forge. */
	ACTOR_SALAMANDER,
	ACTOR_AVIMANDER, /* the salamander grown into Avimander */
	ACTOR_CHAUCHA,
	ACTOR_KRADEN
} CHAMPA_CLIFF_CAVES_OBABAS_ROOM_ACTOR_ID;

/* Briggs's home once Avimander is beaten
   (ChampaCliffCavesObabaPlacementsAfterAvimander). */
typedef enum CHAMPA_CLIFF_CAVES_BRIGGS_HOME_ACTOR_ID {
	/* Briggs, lying in bed: "Once I'm over my cold, I'm setting out to get more
	   jewels and make amends with Madra and Daila." */
	ACTOR_BRIGGS_IN_BED = ACTOR_FIRST_PLACED,
	/* Eoleo, "Babu ba bubabu...", and Chaucha by the bed: "I wonder what's
	   wrong with Briggs." */
	ACTOR_EOLEO = ACTOR_FIRST_PLACED + 2,
	ACTOR_CHAUCHA_BY_THE_BED
} CHAMPA_CLIFF_CAVES_BRIGGS_HOME_ACTOR_ID;

/* The level with Obaba's door until Briggs is home
   (ChampaCliffCavesObabaPlacements): Obaba behind her door, and the man
   outside it, "Obaba's not seeing anyone right now." */
typedef enum CHAMPA_CLIFF_CAVES_OBABAS_DOOR_ACTOR_ID {
	ACTOR_MAN_AT_OBABAS_DOOR = ACTOR_FIRST_PLACED + 1
} CHAMPA_CLIFF_CAVES_OBABAS_DOOR_ACTOR_ID;

/* The two sailors on Obaba's level once Briggs has fled up here
   (ChampaCliffCavesObabaPlacementsBriggsHome): "Leave the women and children
   alone." and "Did you really think you could come storming into Champa,
   Briggs's home, without a fight?" */
typedef enum CHAMPA_CLIFF_CAVES_OBABAS_LEVEL_SAILOR_ACTOR_ID {
	ACTOR_SAILOR_GUARDING_THE_FAMILIES = ACTOR_FIRST_PLACED,
	ACTOR_SAILOR_SPOILING_FOR_A_FIGHT
} CHAMPA_CLIFF_CAVES_OBABAS_LEVEL_SAILOR_ACTOR_ID;

/* The lowest caves' women (ChampaCliffCavesBriggsPlacements), and once
   Avimander is beaten Briggs's crew home from the sea
   (ChampaCliffCavesBriggsPlacementsAfterAvimander). */
typedef enum CHAMPA_CLIFF_CAVES_LOWEST_ACTOR_ID {
	/* "All of our men went to sea with Briggs, even my sweetheart!" */
	ACTOR_WOMAN_MISSING_HER_SWEETHEART = ACTOR_FIRST_PLACED,
	/* "The Champa sailors who don't live in the cliffs are no better than
	   cowards." */
	ACTOR_WOMAN_SCORNING_HARBOR_SAILORS,
	/* "We're going to wait until Briggs feels better before we go out to sea
	   again." */
	ACTOR_SAILOR_WAITING_FOR_BRIGGS,
	/* "There were plenty of treasures on that island in the middle of the
	   sea." */
	ACTOR_SAILOR_ON_THE_ISLAND_TREASURE,
	/* "We really want to make amends." */
	ACTOR_SAILOR_WANTING_AMENDS,
	/* "It's all my fault. If I hadn't messed with that switch, we'd have gotten
	   everything." */
	ACTOR_SAILOR_WHO_HIT_THE_SWITCH
} CHAMPA_CLIFF_CAVES_LOWEST_ACTOR_ID;

/* The lowest caves once Briggs has fled up here
   (ChampaCliffCavesBriggsPlacementsBriggsHome). */
typedef enum CHAMPA_CLIFF_CAVES_LOWEST_BRIGGS_HOME_ACTOR_ID {
	/* "Why do you have to be so stubborn? Quit following us, you big bully!" */
	ACTOR_SAILOR_CALLING_FELIX_A_BULLY = ACTOR_FIRST_PLACED,
	/* The sailor who gives in (scChampaCliffCavesSailorSurrenders). */
	ACTOR_SAILOR_WHO_SURRENDERS,
	/* The woman in her doorway (scChampaCliffCavesAskToComeIn). */
	ACTOR_WOMAN_IN_HER_DOORWAY
} CHAMPA_CLIFF_CAVES_LOWEST_BRIGGS_HOME_ACTOR_ID;

/* The homes above the lowest caves (ChampaCliffCavesHousesPlacements): a
   sailor's family and the man Briggs left on watch; once Briggs has fled up
   here two of his sailors are there instead of the watchman and the wife
   (ChampaCliffCavesHousesPlacementsBriggsHome). */
typedef enum CHAMPA_CLIFF_CAVES_HOMES_ACTOR_ID {
	/* "Before he left, Briggs ordered me to keep watch in his absence." */
	ACTOR_MAN_KEEPING_WATCH = ACTOR_FIRST_PLACED,
	/* "My husband wants our children to be sailors like him, but I simply won't
	   have it." */
	ACTOR_SAILORS_WIFE,
	/* "Someday, I want to be a sailor, like my father before me." */
	ACTOR_SAILORS_SON,
	/* "Mom's had it pretty rough. I hope I meet a sailor with a future..." */
	ACTOR_SAILORS_DAUGHTER,
	/* "I'd rather fight than go back to jail. Bring it on, sissyboy!" */
	ACTOR_SAILOR_RATHER_FIGHTING = ACTOR_MAN_KEEPING_WATCH,
	/* "You're Felix, aren't you? You've come to bring in Briggs, haven't
	   you?" */
	ACTOR_SAILOR_WHO_KNOWS_FELIX = ACTOR_SAILORS_WIFE
} CHAMPA_CLIFF_CAVES_HOMES_ACTOR_ID;

#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "game/game.h"
#include "edition.h"

#include "field/call.h"
#include "macros.h"
#include "text/msg_ids.h"

/* Obaba's door opening or closing: the map edit that draws it and the metatile
   it plays at. */
typedef struct DOOR_EDIT {
	const Sint16 *ticks;
	Uint16		  x;
	Uint16		  z;
} DOOR_EDIT;

extern LATE_DATA DOOR_EDIT ChampaCliffCavesOpenDoor;
extern LATE_DATA DOOR_EDIT ChampaCliffCavesCloseDoor;

void scChampaCliffCavesOpenDoorAndEnter(
	const DOOR_EDIT *edit, Sint32 door, Sint32 mode);

extern ACTOR_PLACE ChampaCliffCavesBriggsPlacements[];
extern ACTOR_PLACE ChampaCliffCavesHousesPlacements[];
extern ACTOR_PLACE ChampaCliffCavesBriggsPlacementsBriggsHome[];
extern ACTOR_PLACE ChampaCliffCavesHousesPlacementsBriggsHome[];
extern ACTOR_PLACE ChampaCliffCavesObabaPlacementsBriggsHome[];
extern ACTOR_PLACE ChampaCliffCavesBriggsPlacementsAfterAvimander[];
extern ACTOR_PLACE ChampaCliffCavesHousesPlacementsAfterAvimander[];
extern ACTOR_PLACE ChampaCliffCavesObabaPlacementsAfterAvimander[];
extern CUE		   ChampaCliffCavesHousesCues[];
extern CUE		   ChampaCliffCavesObabaCues[];
extern CUE		   ChampaCliffCavesAvimanderCues[];

void scChampaCliffCavesAskIfBriggsLearned(void);
void scChampaCliffCavesObabaMournsSalamander(void);
void scChampaCliffCavesHandPieceToObaba(Sint32 value);
void scChampaCliffCavesObabaStepsForward(Sint32 mode);

void scChampaCliffCavesFlareAndDieDown(void);
void scChampaCliffCavesObabaSeesAWeapon(Sint32 item);
void scChampaCliffCavesObabaAsksForSecondPiece(Sint32 item);
void scChampaCliffCavesStartFlash(Sint32 id, Sint32 mode);

/* The state of the fire over Obaba's forge (heap slot HEAP_SLOT_MENU): the
   pictures of the glow, a glowing disc, and the column of fire, their sprites
   at the forge's spot, and their scales, each one's height trailing its
   width. */
typedef struct CHAMPA_CLIFF_CAVES_STATE {
	Uint8  tiles[64 * 64]; /* the larger picture, the glow's 64 by 64 pixels */
	Sint32 pos[3];
	EFFECT_SPR glow;
	EFFECT_SPR column;
	Sint16	   glowEntry;
	Uint16	   glowVram;
	Sint16	   columnEntry;
	Uint16	   columnVram;
	FIXED	   glowScale;
	FIXED	   glowTrailingScale;
	FIXED	   columnScale;
	FIXED	   columnTrailingScale;
	Sint16	   frm;
} CHAMPA_CLIFF_CAVES_STATE;

/* The state the forge fire's steps end on. */
enum { GLOW_DONE = 999 };

/* The forge fire's pictures are shaded in colours 224 to 255. Their brightest
   sixteen, of sprite palette 15, are moved onto palette 6, so that with palette
   14's they are the 32 colours scChampaCliffCavesDrawForgeFire cycles, at
   levels about these middles: full red, green 20 and blue 3. */
enum {
	GLOW_PICTURE_ROW = 15,
	GLOW_CYCLED_ROW = 6,
	GLOW_COLORS = 2 * PALETTE_COLORS,
	GLOW_GREEN_MIDDLE = 20,
	GLOW_BLUE_MIDDLE = 3
};

/* A 256-colour pixel's palette row (its high four bits) and its colour in that
   row. */
#define PIXEL_PALETTE_ROW_MASK 0xf0
#define PIXEL_ROW_COLOR_MASK   0xf

void scChampaCliffCavesLoadGlow(void);
void scChampaCliffCavesDrawForgeFire(void);
void scChampaCliffCavesLoadColumn(void);

extern Sint16 ChampaCliffCavesOpenDoorFrames[];
extern Sint16 ChampaCliffCavesCloseDoorFrames[];

/* Defines an update that grows an actor by step (a 16.16 scale) a tick until
   it is full size. */
#define DEFINE_CHAMPA_CLIFF_CAVES_GROW(name, step) \
	void name(ACTOR *actor) \
	{ \
		actor->scaleX += step; \
		if(actor->scaleX >= toFIXED(1)) { \
			actor->scaleX = toFIXED(1); \
			actor->update = 0; \
		} \
		actor->scaleY = actor->scaleX; \
	}

/* Each tick the Trident, thrown up from the rising column, grows by a
   sixteenth of its size until it is full size. */
DEFINE_CHAMPA_CLIFF_CAVES_GROW(ChampaCliffCaves_GrowThrown, toFIXED(1) / 16)

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

extern ACTOR_PLACE ChampaCliffCavesObabaPlacements[];
extern ACTOR_PLACE ChampaCliffCavesAvimanderPlacements[];
extern ACTOR_PLACE ChampaCliffCavesAvimanderPlacementsBriggsHome[];
extern ACTOR_PLACE ChampaCliffCavesAvimanderPlacementsAfterAvimander[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint32 scene;

	if(syGameFlagGet(FLAG_CHAMPA_AVIMANDER_BEATEN)) {
		scene = MainRoutineStateWork.next;
		if(scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
			return ChampaCliffCavesBriggsPlacementsAfterAvimander;
		if(scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
			return ChampaCliffCavesHousesPlacementsAfterAvimander;
		if(scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
			return ChampaCliffCavesObabaPlacementsAfterAvimander;
		if(scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
			return ChampaCliffCavesAvimanderPlacementsAfterAvimander;
	} else {
		if(syGameFlagGet(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
			scene = MainRoutineStateWork.next;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
				return ChampaCliffCavesBriggsPlacementsBriggsHome;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
				return ChampaCliffCavesHousesPlacementsBriggsHome;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
				return ChampaCliffCavesObabaPlacementsBriggsHome;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
				return ChampaCliffCavesAvimanderPlacementsBriggsHome;
		} else {
			scene = MainRoutineStateWork.next;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
				return ChampaCliffCavesBriggsPlacements;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
				return ChampaCliffCavesHousesPlacements;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
				return ChampaCliffCavesObabaPlacements;
			if(scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
				return ChampaCliffCavesAvimanderPlacements;
		}
	}
	return ChampaCliffCavesBriggsPlacements;
}

/* Walking into a door in the cliff caves: it opens with a sound, Felix walks
   through (lined up on the door first when `mode` is 0, straight on when 1) and
   the screen goes to the room behind it. */
void scChampaCliffCavesOpenDoorAndEnter(
	const DOOR_EDIT *edit, Sint32 door, Sint32 mode)
{
	Sint32 id;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(edit->ticks, edit->x, edit->z);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlg = ACTOR_FALLS;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	id = MainRoutineStateWork.leaderActorId;
	if(mode == 0)
		fdActorCenterAndWalk(id, 2, -8);
	else
		fdActorSetDestinationOffset(id, 0, -8);
	fdScriptWait(10);
	syImportScriptSetExitRequest(door);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}

/* Walking north into Obaba's door (metatile (11, 26)) once Avimander is beaten
   opens it, and Felix walks into the home behind it, where Briggs lies in
   bed. */
void scChampaCliffCavesEnterDoor(Sint32 door)
{
	scChampaCliffCavesOpenDoorAndEnter(&ChampaCliffCavesOpenDoor, door, 0);
}

/* Walking into or knocking at Obaba's door, at the west end of its passage
   (metatiles (11, 25) and (11, 26)), before Avimander is beaten: Felix steps up
   beside it and she opens it, asks "Has my grandson returned? Has my Briggs
   come home?", answers the yes ("You lie!") or the no, and shuts herself in
   again. */
void scChampaCliffCavesObabaAsksForBriggs(void)
{
	const DOOR_EDIT *edit;
	Sint32			 msg;

	edit = &ChampaCliffCavesOpenDoor;
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 168, 424);
	fdActorFaceDirectionAndRefreshById(
		MainRoutineStateWork.leaderActorId, FACING_EAST);
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(edit->ticks, edit->x, edit->z);
	fdActorGetById(ACTOR_OBABA)->motionFlg = 0;
	fdActorSetPos(ACTOR_OBABA, 184, 408);
	fdActorSetSpeed(ACTOR_OBABA, 1.8);
	fdActorWalkByAndWaitById(ACTOR_OBABA, 0, 16);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
	msg = (Sint32)MsgHasMyGrandson;
	fdScriptSetMessageId(msg);
	/* "Has my grandson returned? Has my Briggs come home?" Yes: "You lie! If
	   Briggs had truly returned..." No: "I asked not to be disturbed until my
	   grandson has returned home!..." */
	fdMessageShowAtActor(ACTOR_OBABA, 0);
	if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(10);
		fdScriptSetMessageId(msg + 1);
	} else {
		fdScriptWait(20);
		fdScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessageAndWait(ACTOR_OBABA, 0);
	fdActorSetSpeed(ACTOR_OBABA, 0.8);
	fdActorWalkByAndWait(ACTOR_OBABA, 0, -14);
	fdActorSetPos(ACTOR_OBABA, 0, 0);
	edit = &ChampaCliffCavesCloseDoor;
	fdMapGridPlayMapEdit(edit->ticks, edit->x, edit->z);
	sySndPlay(SE_DOOR_OPEN);
}

/* Talking to the woman standing in the doorway of her home at the west end of
   the lowest caves, once Briggs is back: "What, you want to come in?" A yes
   gets "As if! I'm a nice girl...", a no "You're wasting my time." */
void scChampaCliffCavesAskToComeIn(Sint32 actor)
{
	fdScriptSetMessageId((Sint32)MsgWhatYouWant);
	/* "What, you want to come in?" Yes: "As if! I'm a nice girl..." No: "Yeah?
	   So what do you want? You're wasting my time." */
	fdMessageShowAtActor(actor, 0);
	if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(10);
		fdScriptSetMessageId((Sint32)MsgWhatYouWant + 1);
	} else {
		fdScriptWait(20);
		fdScriptSetMessageId((Sint32)MsgWhatYouWant + 2);
	}
	fdScriptShowMessageAndWait(actor, 0);
}

/* Talking to the second of the two sailors on the east side of the lowest
   caves, once Briggs is back: he gives in ("I'll go back to Alhafra...but only
   if Briggs is going") and keeps his eyes on Felix. */
void scChampaCliffCavesSailorSurrenders(Sint32 actorId)
{
	ACTOR *actor;

	fdScriptSetMessageId((Sint32)MsgAllRightI);
	/* "All right, I surrender! I'll go back to Alhafra...but only if
	   Briggs..." */
	fdScriptShowMessageAndWait(actorId, 0);
	actor = fdActorGetById(actorId);
	fdActorWatchActor(
		actor, fdActorGetById(MainRoutineStateWork.leaderActorId));
}

/* Talking to the sailor in Obaba's forge room, once Briggs is back: "Aw, come
   on! I'm not doing anything! Leave me alone!", and he turns his back on the
   party, squashing and stretching from then on (MOVEMENT_TYPE_SQUASH). */
void scChampaCliffCavesSailorTurnsAway(Sint32 actor)
{
	fdScriptSetMessageId((Sint32)MsgAwComeOn);
	/* "Aw, come on! I'm not doing anything! Leave me alone!" */
	fdScriptShowMessageAndWait(actor, 0);
	fdActorFaceDirectionAndWait(actor, FACING_NORTH_NORTHEAST);
	fdActorSetMovementType(fdActorGetById(actor), MOVEMENT_TYPE_SQUASH);
}

/* Reveal in Obaba's forge room shows a path hidden across the forge pit, north
   to the forged Trident floating over it. The path's metatiles, kept further
   down the map, are copied there with their behaviours. */
void scChampaCliffCavesRevealPath(void)
{
	fdMapGridCopyMetatileTilesMetatilesRect(
		41, 97, 1, FORGE_PATH_METATILES, FORGE_PATH_METATILE_X, 77);
	fdMapCopyMetatileAttributeRect(41, 33, 1, FORGE_PATH_METATILES,
		FORGE_PATH_METATILE_X, FORGE_PATH_METATILE_Z);
}

/* When Reveal wears off, the pit's own metatiles cover the path again, and a
   leader standing on it is set down on the pit's south rim. */
void scChampaCliffCavesHidePath(void)
{
	ACTOR *actor;
	Sint32 x;
	Sint32 z;

	fdMapGridCopyMetatileTilesMetatilesRect(
		50, 97, 1, FORGE_PATH_METATILES, FORGE_PATH_METATILE_X, 77);
	fdMapCopyMetatileAttributeRect(46, 33, 1, FORGE_PATH_METATILES,
		FORGE_PATH_METATILE_X, FORGE_PATH_METATILE_Z);
	actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	x = FIELD_METATILE(actor->x.fixed);
	z = FIELD_METATILE(actor->z.fixed);
	/* On the path, or one row past it on the south rim (row 17), where he is
	   set down. */
	if(MainRoutineStateWork.next == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER &&
		x == FORGE_PATH_METATILE_X &&
		(Uint32)(z - FORGE_PATH_METATILE_Z) <= FORGE_PATH_METATILES)
		fdActorSetPos(MainRoutineStateWork.leaderActorId, 664, 280);
}

extern CUE ChampaCliffCavesBriggsCues[];

/* Talking to the forged Trident floating over Obaba's forge pit, reached by the
   path Reveal shows, takes it. */
void scChampaCliffCavesPickUpTrident(Sint32 a)
{
	fdScriptPickUpItem(a, ITEM_TRIDENT, FLAG_TRIDENT_TAKEN);
}

const CUE *scGetCues(void)
{
	Sint32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
		return ChampaCliffCavesBriggsCues;
	if(scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
		return ChampaCliffCavesHousesCues;
	if(scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
		return ChampaCliffCavesObabaCues;
	if(scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
		return ChampaCliffCavesAvimanderCues;
	return ChampaCliffCavesBriggsCues;
}

Sint32 scInit(void)
{
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

#if !REVISED_ENTRANCE_TRANSITIONS
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
	} else if(party->next == SCENE_CHAMPA_CLIFF_CAVES_OBABA) {
		if(syGameFlagGet(FLAG_CHAMPA_AVIMANDER_BEATEN)) {
			fdActorSetAnimationById(ACTOR_BRIGGS_IN_BED, 11);
			fdActorSetSprFlags(fdActorGetById(ACTOR_BRIGGS_IN_BED), 0);
		}
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* Felix walks up to Obaba's counter, holds the prong up over it, leaves it
   there and steps back. */
void scChampaCliffCavesHandPieceToObaba(Sint32 actor)
{
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 648, 200);
	fdActorFaceDirectionAndRefreshById(
		MainRoutineStateWork.leaderActorId, FACING_EAST);
	fdScriptWait(5);
	fdActorSetAnimationAndWait(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_SPRING);
	fdScriptWait(7);
	fdActorShowItemIconById(ACTOR_HELD_ITEM, actor);
	fdActorSetPos(ACTOR_HELD_ITEM, 648, 196);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorWalkByAndWaitById(MainRoutineStateWork.leaderActorId, -16, -8);
	fdScriptWait(1);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	fdActorFaceDirectionAndRefreshById(
		MainRoutineStateWork.leaderActorId, FACING_EAST);
	fdScriptWait(10);
}

/* Obaba steps up to her counter and takes the prong from it, and unless she is
   to forge it at once steps back. */
void scChampaCliffCavesObabaStepsForward(Sint32 value)
{
	fdActorSetSpeed(ACTOR_OBABA, 0.8);
	fdActorGetById(ACTOR_OBABA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWaitById(ACTOR_OBABA, 0, 6);
	fdScriptWait(1);
	fdActorGetById(ACTOR_OBABA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdScriptWait(10);
	fdActorSetAnimationById(ACTOR_OBABA, 8);
	fdScriptWait(15);
	fdActorSetPos(ACTOR_HELD_ITEM, 0, 0);
	fdScriptWait(15);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdScriptWait(15);
	if(value == 0) {
		fdActorGetById(ACTOR_OBABA)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkByAndWait(ACTOR_OBABA, 0, -6);
		fdScriptWait(1);
		fdActorGetById(ACTOR_OBABA)->steeringFlg |= ACTOR_FACES_MOTION;
		fdScriptWait(10);
	}
}

/* The forge pit's glow changes colour, its five colours fading from one set to
   another over the given number of ticks, a step a tick. */
void scChampaCliffCavesFadeCols(Uint16 *from, Uint16 *to, Sint32 ticks)
{
	Sint32 step;

	for(step = 0; step <= ticks; step++) {
		Sint32 ofs;
		Sint32 left;

		for(ofs = 0, left = 0; left < FORGE_COLORS; left++) {
			Sint32 red, green, blue, toRed, toGreen, toBlue;

			red = RGB_RED(*(Uint16 *)(ofs + (Sint32)from));
			green = RGB_GREEN(*(Uint16 *)(ofs + (Sint32)from));
			blue = RGB_BLUE(*(Uint16 *)(ofs + (Sint32)from));
			toRed = RGB_RED(*(Uint16 *)(ofs + (Sint32)to));
			toGreen = RGB_GREEN(*(Uint16 *)(ofs + (Sint32)to));
			toBlue = RGB_BLUE(*(Uint16 *)(ofs + (Sint32)to));
			red += step * (toRed - red) / ticks;
			green += step * (toGreen - green) / ticks;
			blue += step * (toBlue - blue) / ticks;
			SET_RGB(*(Uint16 *)((Uint32)&BG_PLTT_COLOR(
									FORGE_PALETTE, FORGE_FIRST_COLOR) +
						ofs),
				red, green, blue);
			ofs += 2;
		}
		sySynch(1);
	}
}

/* Obaba is handed the first prong: she knows it for part of the trident of
   Ankohl and offers to keep it until the party brings the other two. */
void scChampaCliffCavesObabaSeesAWeapon(Sint32 actor)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgThisLooksLike);
	scChampaCliffCavesHandPieceToObaba(actor);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationById(ACTOR_OBABA, 7);
	fdScriptWait(30);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdScriptWait(15);
	/* "This looks like a weapon!" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetAnimationById(ACTOR_OBABA, 7);
	fdScriptWait(30);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdScriptWait(15);
	/* "It looks like part of a trident, but it's only one third of the..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetAnimationById(ACTOR_OBABA, 7);
	fdScriptWait(30);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdScriptWait(15);
	/* "Why... Could it be? Is this the legendary trident of Ankohl?" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_OBABA, 2);
	fdScriptWait(8);
	/* "I'll hold on to this for you. It won't do you any good like this..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	scChampaCliffCavesObabaStepsForward(0);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
	fdScriptWait(10);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "You seem unsure... You don't think I'll pull a fast one on you, do you?"
	   Yes: "Kids today. So mistrustful." No: "Ah, that's nice. Don't you
	   worry!" */
	fdMessageShowAtActor(ACTOR_OBABA, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdScriptWait(20);
		/* "Kids today. So mistrustful." */
		fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(35);
		FieldStateWork->msg++;
		/* "Ah, that's nice. Don't you worry!" */
		fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	}
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_NOD);
	/* "If you bring me the other two pieces, I'll be happy to rebuild it..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	fdScriptEnd();
}

/* Obaba is handed the second prong and keeps it with the first. */
void scChampaCliffCavesObabaAsksForSecondPiece(Sint32 actor)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgAhHaveYou);
	scChampaCliffCavesHandPieceToObaba(actor);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationById(ACTOR_OBABA, 7);
	fdScriptWait(30);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdScriptWait(15);
	/* "Ah, have you found the second piece, dear?" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetAnimationById(ACTOR_OBABA, 7);
	fdScriptWait(30);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdScriptWait(15);
	/* "It's magnificent!" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	scChampaCliffCavesObabaStepsForward(0);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
	fdScriptWait(10);
	/* "I'll hold on to this as well. Wouldn't want it getting lost, now..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	fdScriptEnd();
}

/* The trident's three prongs, in the order Obaba throws them into the forge. */
typedef struct TRIDENT_PRONGS {
	Sint32 items[3];
} TRIDENT_PRONGS;

extern TRIDENT_PRONGS ChampaCliffCavesProngs;

/* Obaba is handed the last prong and throws the three into her forge, whose pit
   flashes pale at each, then flares, shakes the room and lifts out the Trident
   to float over it. */
void scChampaCliffCavesObabaForgesTrident(Sint32 value)
{
	ACTOR  *thrown = fdActorGetById(ACTOR_HELD_ITEM);
	Uint16	dark[FORGE_COLORS];
	Uint16	pale[FORGE_COLORS];
	Sint32	glow[3];
	Sint32	i;
	ACTOR  *actor;
	Uint16 *palette;

	glow[0] = toFIXED(662);
	glow[1] = 0;
	glow[2] = toFIXED(238);
	palette = (Uint16 *)&BG_PLTT_COLOR(FORGE_PALETTE, FORGE_FIRST_COLOR);
	dark[0] = *palette++;
	dark[1] = *palette++;
	dark[2] = *palette++;
	dark[3] = *palette++;
	dark[4] = *palette;
	palette = (Uint16 *)&BG_PLTT_COLOR(FORGE_PALE_PALETTE, FORGE_FIRST_COLOR);
	pale[0] = *palette++;
	pale[1] = *palette++;
	pale[2] = *palette++;
	pale[3] = *palette++;
	pale[4] = *palette;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgWellDoneYou);
	scChampaCliffCavesHandPieceToObaba(value);
	fdActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Well done! You found all three pieces of the trident!" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "That means I can begin work reforging the ancient trident of
	   Ankohl!..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	scChampaCliffCavesObabaStepsForward(1);
	fdActorWalkToAndWait(ACTOR_OBABA, 664, 200);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
	fdScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
	fdScriptWait(10);
	/* "We'll just toss all three pieces in here, shall we?" Yes: "Just leave it
	   to me..." No: "There's nothing to be afraid of, dear. Just..." */
	fdMessageShowAtActor(ACTOR_OBABA, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdScriptWait(20);
		/* "Just leave it to me..." */
		fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(40);
		FieldStateWork->msg++;
		/* "There's nothing to be afraid of, dear. Just leave it to me." */
		fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	}
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	fdScriptWait(10);
	fdCameraMoveTo(664, CAMERA_KEEP, 232, 1);
	fdCameraWaitForArrival();
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
	/* "Heart of the earth, fury of the forge's flames, grant me power!" */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	for(i = 0; i <= 2; i++) {
		TRIDENT_PRONGS prongs;
		FIXED		   landing[3];
		prongs = ChampaCliffCavesProngs;
		fdActorSetAnimationById(ACTOR_OBABA, 9);
		fdScriptWait(3);
		fdActorShowItemIconById(ACTOR_HELD_ITEM, prongs.items[i]);
		fdActorSetPos(ACTOR_HELD_ITEM, 668, 200);
		thrown->gravity = toFIXED(1) / 4;
		fdActorJump(ACTOR_HELD_ITEM, 4, 2);
		fdActorSetSprPriority(ACTOR_HELD_ITEM, 1);
		fdActorSetSpeed(ACTOR_HELD_ITEM, 1);
		fdActorSetDestinationOffsetById(ACTOR_HELD_ITEM, 0, 40);
		sySndPlay(SE_FALL);
		fdScriptWait(34);
		landing[0] = thrown->x.fixed;
		landing[1] = thrown->y.fixed;
		landing[2] = thrown->z.fixed;
		fdActorSetPos(ACTOR_HELD_ITEM, 0, 0);
		fdScriptWait(20);
		sySndPlay(SE_HEAVY_IMPACT);
		scChampaCliffCavesFadeCols(dark, pale, 12);
		scChampaCliffCavesFadeCols(pale, dark, 48);
		fdScriptWait(10);
	}
	actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	actor->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWaitById(MainRoutineStateWork.leaderActorId, 12, 12);
	fdScriptWait(1);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	fdActorFaceDirectionAndRefreshById(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST);
	fdScriptWait(10);
	fdActorSetAnimationById(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_LEAN_BACK);
	fdScriptWait(20);
	fdActorSetAnimationById(ACTOR_OBABA, 7);
	fdScriptWait(60);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "That's funny... Nothing happened." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	fdScriptWait(20);
	sySndPlay(SE_SHIMMER);
	scChampaCliffCavesFadeCols(dark, pale, 48);
	fdActorStartRepeatedMotion(MainRoutineStateWork.leaderActorId, 3);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(50);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	Call2((void (*)())fdStartColTransition, 16, RGB(16, 4, 4));
	fdMapGridSetShake(2, 2, 1);
	scChampaCliffCavesLightForge(glow);
	fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
	fdActorJump(ACTOR_FELIX, 6, 0);
	sySndPlay(SE_TABLET_RISE);
	fdActorGetById(ACTOR_FELIX)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_FELIX, -12, -12);
	fdScriptWait(1);
	fdActorGetById(ACTOR_FELIX)->steeringFlg |= ACTOR_FACES_MOTION;
	fdScriptWait(10);
	fdActorStartRepeatedMotion(ACTOR_OBABA, 3);
	fdEffectSetAttached(
		ACTOR_OBABA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(30);
	/* "What's going on?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	fdMapGridSetShake(4, 4, 1);
	scChampaCliffCavesRaiseColumn(glow);
	sySndPlay(SE_TABLET_RISE);
	fdScriptWait(10);
	fdActorJump(ACTOR_OBABA, 6, 15);
	fdActorJump(ACTOR_OBABA, 6, 23);
	sySndPlay(SE_TABLET_RISE);
	fdScriptWait(10);
	/* "Something's happening! Is that...the legendary trident?" */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
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
	gmPlayMusic();
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_OBABA, 2);
	fdScriptWait(5);
	/* "After all these years, the trident is complete again. But what's..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
	fdScriptWait(10);
	/* "Regardless, the trident has been fixed. It belongs to you now..." */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	fdScriptWait(10);
	fdActorSetSprPriority(ACTOR_HELD_ITEM, 2);
	fdScriptEnd();
}

/* Talking to Obaba at her counter: she takes the next prong the party carries,
   right, left then center, or talks of the trident when it has none. From east
   of the forge path's column the field menu opens instead. */
void scChampaCliffCavesObabaOffersHelp(Sint32 actor)
{
	Sint32 item;
	/* DEAD: each owner is stored and never read; the ROM keeps the copies of
	   the first two. */
	Sint32 owner;

	if(FIELD_METATILE(
		   fdActorGetById(MainRoutineStateWork.leaderActorId)->x.fixed) <=
		FORGE_PATH_METATILE_X)
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
		if(!fdScriptGetFlag(FLAG_OBABA_SAW_FIRST_PRONG)) {
			syGameFlagSet(FLAG_OBABA_SAW_FIRST_PRONG);
			scChampaCliffCavesObabaSeesAWeapon(item);
		} else if(!fdScriptGetFlag(FLAG_OBABA_SAW_SECOND_PRONG)) {
			syGameFlagSet(FLAG_OBABA_SAW_SECOND_PRONG);
			scChampaCliffCavesObabaAsksForSecondPiece(item);
		} else if(!fdScriptGetFlag(FLAG_TRIDENT_FORGED)) {
			syGameFlagSet(FLAG_TRIDENT_FORGED);
			syFieldMapGridSetMetatileHeight(toFIXED(656), toFIXED(208), 0, -4);
			syFieldMapGridSetMetatileHeight(toFIXED(656), toFIXED(224), 0, -4);
			syFieldMapGridSetMetatileHeight(toFIXED(656), toFIXED(240), 0, -4);
			syFieldMapGridSetMetatileHeight(toFIXED(656), toFIXED(256), 0, -4);
			scChampaCliffCavesObabaForgesTrident(item);
			syFieldMapGridSetMetatileHeight(toFIXED(656), toFIXED(208), 0, 0);
			syFieldMapGridSetMetatileHeight(toFIXED(656), toFIXED(224), 0, 0);
			syFieldMapGridSetMetatileHeight(toFIXED(656), toFIXED(240), 0, 0);
			syFieldMapGridSetMetatileHeight(toFIXED(656), toFIXED(256), 0, 0);
		}
	} else {
		fdActorFaceActorById(actor, MainRoutineStateWork.leaderActorId, 5);
		/* "I mean it. If there's ever anything you need, just call on me.",
		   then "The legend says that anyone who holds the whole lance has
		   nothing to fear from the sea.", "Yes, there's no doubt about it.
		   Those must truly be fragments of the legendary trident." and "The
		   trident seems to have some amazing power..." */
		if(!syGameFlagGet(FLAG_OBABA_SAW_FIRST_PRONG))
			fdScriptSetMessageId((Sint32)MsgIMeanIt);
		else if(!syGameFlagGet(FLAG_OBABA_SAW_SECOND_PRONG))
			fdScriptSetMessageId((Sint32)MsgTheLegendSaysThat);
		else if(!syGameFlagGet(FLAG_TRIDENT_FORGED))
			fdScriptSetMessageId((Sint32)MsgYesThereS);
		else
			fdScriptSetMessageId((Sint32)MsgTheTridentSeems);
		fdScriptShowMessageAndWait(actor, 0);
		fdActorFaceDirection(actor, FACING_SOUTH_SOUTHWEST, 0);
	}
}

/* Each tick Avimander, leaping out of the forge's fire, grows by three sixty-
   fourths of its size, from a quarter to full size. */
DEFINE_CHAMPA_CLIFF_CAVES_GROW(
	ChampaCliffCaves_GrowThrownSlowly, toFIXED(3) / 64)

/* Walking east into Obaba's forge room from the passage (metatiles (36, 12) to
   (36, 18)) once Briggs has fled up here: "Hold it right there!" Briggs stops
   the party, and Obaba, for her great-grandson Eoleo's sake, tells Felix he had
   better leave; if he will not, she throws her salamander into the forge, whose
   pit turns fiery, and it climbs out grown into Avimander to fight. Coming back
   after a defeat, Briggs greets the party with "Back for more?". */
void scChampaCliffCavesFightAvimander(void)
{
	Uint16		  dark[FORGE_COLORS];
	Uint16		  fiery[FORGE_COLORS];
	VECTOR3		  pos;
	const Uint16 *col;
	ACTOR		 *actor = fdActorGetById(ACTOR_AVIMANDER);

	if(fdScriptGetFlag(FLAG_CHAMPA_AVIMANDER_BEATEN))
		return;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 584, 248);
	if(fdScriptGetFlag(FLAG_CHAMPA_BRIGGS_STOPPED_PARTY)) {
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_EAST);
		fdScriptSetMessageId((Sint32)MsgBackForMore);
		/* "Back for more?" */
		syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		sySndPlay(MUS_CONFRONTATION);
		fdCameraMoveTo(664, CAMERA_KEEP, 256, 1);
		fdCameraWaitForArrival();
		fdScriptWait(5);
		fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
		/* "I wouldn't be doing this if it weren't for Eoleo, you know." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
		scFieldActorShowEmote(
			ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
		/* "I'm sorry. You look like quite a nice boy, but I just can't
		   allow..." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	} else {
		fdScriptSetFlag(FLAG_CHAMPA_BRIGGS_STOPPED_PARTY);
		fdScriptSetMessageId((Sint32)MsgHoldItRight);
		/* "Hold it right there!" */
		syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		sySndPlay(MUS_CONFRONTATION);
		fdActorFaceDirectionById(
			MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
		fdCameraMoveTo(664, CAMERA_KEEP, 256, 1);
		fdCameraWaitForArrival();
		fdScriptWait(5);
		fdActorJump(ACTOR_BRIGGS, 6, 15);
		fdActorJump(ACTOR_BRIGGS, 6, 23);
		/* "So, Felix, you've come." */
		syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		/* "Don't assume I'll go so easy on you this time!" */
		syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		scFieldActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
		fdScriptWait(10);
		/* "Is that boy him? The "mighty warrior" you told me about?" */
		syFieldScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
		scFieldActorShowEmote(
			ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
		fdScriptWait(10);
		/* "I told you he didn't look like much, Grandma..." */
		syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		/* "Weren't you even listening to me?" */
		syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_NOD);
		/* "All you told me is that his name is Felix and that he is a
		   "mighty..." */
		syFieldScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
		fdScriptWait(5);
		fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
		/* "He doesn't look like the sort of person who's interested in
		   stealing..." */
		syFieldScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
		fdScriptWait(30);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
		fdScriptWait(10);
		scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		/* "But that's not the point, Grandma!" */
		syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
		fdScriptWait(20);
		fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
		fdScriptWait(5);
		/* "If you don't destroy him now, he's going to take me far away,
		   and..." */
		syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		scFieldActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		/* "Oh, now he doesn't look like the kind of boy who would do
		   that..." */
		syFieldScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
		fdScriptWait(10);
		scFieldActorShowEmote(
			ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 45);
		fdActorFaceDirectionAndWait(
			ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST + FACING_NUDGE);
		fdScriptWait(10);
		/* "If he takes me away, poor Eoleo will be so sad! What do you
		   say..." */
		syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		scFieldActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
		fdScriptWait(10);
		/* "Oh, that's just not fair. You know I can't deny my darling..." */
		syFieldScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
		fdScriptWait(5);
		fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
		fdScriptWait(5);
		/* "I don't see why I should be the one to get you out of this mess,
		   but..." */
		syFieldScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
		fdScriptWait(5);
		/* "Grandma, please!" */
		syFieldScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
		fdScriptWait(5);
	}
	fdScriptSetMessageId((Sint32)MsgOhAllRightListen);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_NORTH_NORTHWEST);
	fdScriptWait(20);
	fdActorGetById(ACTOR_OBABA)->facing = FACING_SOUTH_SOUTHWEST;
	fdActorSetAnimationById(ACTOR_OBABA, 5);
	fdScriptWait(30);
	fdActorSetAnimationById(ACTOR_OBABA, 6);
	fdActorSetPos(ACTOR_SALAMANDER, 724, 252);
	fdActorGetById(ACTOR_SALAMANDER)->scaleX = toFIXED(6) / 5;
	fdActorGetById(ACTOR_SALAMANDER)->scaleY = toFIXED(6) / 5;
	fdActorSetSprFlags(fdActorGetById(ACTOR_SALAMANDER), 0);
	/* "Oh, all right. Listen, you have no idea what you're getting into. I
	   think you'd better leave." Yes: "Ah, you've made a good decision..." No:
	   "Stubborn, are you? Ah, well..." */
	fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		gmPlayMusic();
		fdScriptWait(15);
		fdActorSetPos(ACTOR_SALAMANDER, 0, 0);
		fdActorSetAnimationById(ACTOR_OBABA, ANIM_STAND);
		fdActorGetById(ACTOR_OBABA)->facing = FACING_WEST;
		/* "Ah, you've made a good decision. You seem like a reasonable boy." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
		FieldStateWork->msg++;
		fdActorWalkByAndWaitById(ACTOR_FELIX, -16, 0);
		fdScriptWait(5);
	} else {
		fdScriptWait(25);
		FieldStateWork->msg++;
		/* "Stubborn, are you? Ah, well. If that's the way it's going to be, get
		   ready for your medicine!" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
		scFieldActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		/* "Heart of the earth, fury of the forge's flames, grant me power!" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
		sySndPlay(SOUND_FADE_OUT_BGM);
		syFieldMapGridSetMetatileHeight(toFIXED(656), toFIXED(208), 0, -4);
		syFieldMapGridSetMetatileHeight(toFIXED(656), toFIXED(224), 0, -4);
		syFieldMapGridSetMetatileHeight(toFIXED(656), toFIXED(240), 0, -4);
		syFieldMapGridSetMetatileHeight(toFIXED(656), toFIXED(256), 0, -4);
		actor->gravity = toFIXED(1) / 4;
		actor->motionFlg = ACTOR_FALLS;
		actor->groundHeight = -toFIXED(999);
		fdActorJump(ACTOR_SALAMANDER, 10, 0);
		sySndPlay(SE_ENEMY_CRY);
		fdActorSetSprPriority(ACTOR_SALAMANDER, 1);
		fdActorSetSpeed(ACTOR_SALAMANDER, 3);
		fdActorWalkToById(ACTOR_SALAMANDER, 644, 248);
		fdScriptWait(21);
		fdActorSetPos(ACTOR_SALAMANDER, 0, 0);
		fdScriptWait(50);
		fdMapGridSetShake(2, 2, 1);
		fdScriptWait(70);
		sySndPlay(SE_BOSS_ROAR);
		fdScriptWait(25);
		col = (const Uint16 *)&BG_PLTT_COLOR(FORGE_PALETTE, FORGE_FIRST_COLOR);
		dark[0] = *col++;
		dark[1] = *col++;
		dark[2] = *col++;
		dark[3] = *col++;
		dark[4] = *col++;
		col = (const Uint16 *)&BG_PLTT_COLOR(
			FORGE_FIERY_PALETTE, FORGE_FIRST_COLOR);
		fiery[0] = *col++;
		fiery[1] = *col++;
		fiery[2] = *col++;
		fiery[3] = *col++;
		fiery[4] = *col++;
		scChampaCliffCavesFadeCols(dark, fiery, 48);
		fdScriptWait(20);
		sySndPlay(SE_TABLET_RISE);
		pos.x = toFIXED(662);
		pos.y = 0;
		pos.z = toFIXED(238);
		scChampaCliffCavesStartForgeFire(&pos);
		sySndPlay(SE_BEAST_CRY);
		fdActorSetPos(ACTOR_AVIMANDER, 664, 248);
		actor->gravity = toFIXED(1);
		actor->motionFlg = ACTOR_FALLS;
		actor->groundHeight = 0;
		actor->y.fixed = toFIXED(32);
		actor->scaleX = toFIXED(1) / 4;
		actor->update = (Sint32)ChampaCliffCaves_GrowThrownSlowly;
		actor->scaleY = toFIXED(1) / 4;
		fdActorJump(ACTOR_AVIMANDER, 11, 0);
		sySndPlay(SE_BEAST_CRY);
		fdActorSetSpeed(ACTOR_AVIMANDER, 3.2);
		fdActorWalkToAndWait(ACTOR_AVIMANDER, 584, 248);
		fdScriptSetDefeatDestination(
			SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER, ENTRANCE_AFTER_DEFEAT);
		fdScriptSetAfterBattleDestination(SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER,
			ENTRANCE_AFTER_AVIMANDER_BATTLE);
		SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
		fdRequestBattleFromEncounterTable(
			ENCOUNTER_ZONE_SERPENT, ENCOUNTER_SLOT_AVIMANDER);
	}
	fdScriptEnd();
}

/* Coming to after losing to Avimander, in the forge room: Briggs asks Felix
   whether he has learned his lesson, Obaba scolds, and the party is sent back
   down the passage. */
void scChampaCliffCavesAskIfBriggsLearned(void)
{
	FIELD_STATE *state;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgHaveYouLearned);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 568, 248);
	fdActorSetPos(ACTOR_OBABA, 584, 224);
	fdActorSetPos(ACTOR_AVIMANDER, 600, 240);
	fdActorSetPos(ACTOR_BRIGGS, 584, 264);
	fdActorFaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
	fdActorSetAnimationById(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_LIE_ON_BACK);
	fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(10);
	fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Have you learned your lesson, Felix?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "Ah, such a bright lad. Now then, don't you ever try to take my..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 20);
	state = Ram_HeapSlots->fieldState;
	state->transitionFrm = 40;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptSetDestination((Sint16)MainRoutineStateWork.restartScene,
		(Sint16)MainRoutineStateWork.restartEntrance);
}

extern Uint32 ChampaCliffCavesChaseToNorthScript[];
extern Uint32 ChampaCliffCavesChaseToEastScript[];
extern Uint32 ChampaCliffCavesChaseToWestScript[];
extern Uint32 ChampaCliffCavesChaseToSouthScript[];

/* After the party beats Avimander, in the forge room: Obaba mourns her
   salamander ("My salamander! You... beat my salamander...") and leaves Briggs
   to fight for himself, the party tells her he is a pirate, Chaucha comes to
   tell why he did it, and Briggs and Chaucha go home
   (FLAG_CHAMPA_AVIMANDER_BEATEN). */
void scChampaCliffCavesObabaMournsSalamander(void)
{
	const Uint8 *scriptA, *scriptB, *scriptC, *scriptD;
	ACTOR		*leader;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgMySalamanderYou);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 568, 256);
	fdActorSetPos(ACTOR_AVIMANDER, 592, 240);
	fdActorSetAnimationById(ACTOR_AVIMANDER, 3);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdCameraMoveTo(656, CAMERA_KEEP, 248, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(10);
	/* "My salamander! You... beat my salamander..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_AVIMANDER, 2);
	fdScriptWait(5);
	scChampaCliffCavesStartFlash(ACTOR_AVIMANDER, 1);
	fdActorSetSpeed(ACTOR_OBABA, 1);
	fdActorWalkByAndWait(ACTOR_OBABA, -12, -18);
	fdActorWalkByAndWait(ACTOR_OBABA, -10, -14);
	fdActorWalkByAndWait(ACTOR_OBABA, -18, -12);
	fdActorWalkToAndWait(ACTOR_OBABA, 664, 200);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	fdScriptWait(10);
	/* "I thought he was unbeatable... How could this be?" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorWalkByAndWait(ACTOR_OBABA, -16, 0);
	fdActorWalkByAndWait(ACTOR_OBABA, -16, 8);
	fdActorWalkByAndWait(ACTOR_OBABA, -16, 16);
	fdActorWalkByAndWait(ACTOR_OBABA, -8, 16);
	fdActorWalkToAndWait(ACTOR_OBABA, 608, 248);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
	fdScriptWait(5);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "How did you do that? What... What are you?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorSetSpeed(ACTOR_BRIGGS, 1.7);
	fdActorWalkByAndWait(ACTOR_BRIGGS, -16, 0);
	fdScriptWait(5);
	/* "Grandma, what's going on!? I thought you were going to protect me!" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_EAST);
	fdScriptWait(5);
	/* "What do you mean, what am I doing? I've done everything I can!" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorJump(ACTOR_BRIGGS, 6, 15);
	fdActorJump(ACTOR_BRIGGS, 6, 23);
	/* "Grandma!!! He's going to take me away! He's a bad guy! You have
	   to..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "I'm sorry, Briggs, but you'll have to fight him yourself." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(
		ACTOR_BRIGGS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(38);
	/* "Wha-What!?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "You heard me. I'm through pampering you. You have to get out of
	   this..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(
		ACTOR_BRIGGS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(37);
	/* "But...Grandma... Look at him! I mean, look what he can do!" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_WEST - FACING_NUDGE);
	fdScriptWait(25);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_EAST);
	fdScriptWait(10);
	/* "I know, dear, but there's nothing I can do to help. You don't
	   want..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	fdScriptWait(30);
	fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "But...what about poor Eoleo?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	scFieldActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "*Sigh* All right, all right." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 0);
	/* "Um. Excuse me... There seems to be a small misunderstanding." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST - FACING_NUDGE);
	fdScriptWait(10);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	scFieldActorLaunchFromLeader(ACTOR_PIERS, 0, -16, FACING_EAST);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_EAST);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, -16, -16, FACING_EAST);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 16, -16, FACING_EAST);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdScriptWait(10);
	fdCameraMoveTo(640, CAMERA_KEEP, 248, 1);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	/* "Yeah! I don't like standing here listening to a pirate call us
	   the..." */
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0, 5);
	fdActorStartRepeatedMotion(ACTOR_OBABA, 3);
	fdActorSetAttachedEffect(
		ACTOR_OBABA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(
		ACTOR_BRIGGS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	/* "Pirate! My Briggs?" */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	/* "He stole a ship from Alhafra and escaped from their jail! A ship
	   we..." */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_OBABA, FACING_EAST);
	fdScriptWait(5);
	/* "Briggs, is this true?" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdEffectSetAttached(
		ACTOR_BRIGGS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(35);
	/* "Aw... But, Grandma, they..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "We should tell you, we're not here to take Briggs back to Alhafra." */
	fdScriptShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	fdScriptWait(5);
	/* "So you've decided you don't want to run errands for the mayor of..."
	   Yes:
	   "Good. That greedy bag of hot air can do his..." No: "Felix, I know you
	   can't mean that. We didn't..." */
	fdMessageShowAtActor(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdScriptWait(20);
		fdActorFaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_SOUTHEAST);
		fdScriptWait(5);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
		fdScriptWait(10);
		/* "Good. That greedy bag of hot air can do his own dirty work, for
		   all..." */
		fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0, 5);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(35);
		fdActorFaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_SOUTHEAST);
		fdScriptWait(5);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
		FieldStateWork->msg++;
		/* "Felix, I know you can't mean that. We didn't come to Champa for
		   that..." */
		fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0, 5);
	}
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_EAST);
	/* "Briggs... Piracy? Why?" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetSpeed(ACTOR_OBABA, 2.8);
	fdActorSetSpeed(ACTOR_BRIGGS, 3.4);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(
		ACTOR_BRIGGS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scriptA = ChampaCliffCavesChaseToNorthScript;
	fdActorSetMovementTypeById(ACTOR_OBABA, scriptA);
	fdScriptWait(20);
	fdActorWalkToById(ACTOR_BRIGGS, 664, 290);
	fdActorWaitForScriptEndById(ACTOR_OBABA);
	scriptB = ChampaCliffCavesChaseToEastScript;
	fdActorSetMovementTypeById(ACTOR_OBABA, scriptB);
	fdActorWaitForMoveById(ACTOR_BRIGGS);
	scriptC = ChampaCliffCavesChaseToWestScript;
	fdActorSetMovementTypeById(ACTOR_BRIGGS, scriptC);
	fdActorWaitForScriptEndById(ACTOR_OBABA);
	scriptD = ChampaCliffCavesChaseToSouthScript;
	fdActorSetMovementTypeById(ACTOR_OBABA, scriptD);
	fdActorWaitForScriptEndById(ACTOR_BRIGGS);
	fdActorSetMovementTypeById(ACTOR_BRIGGS, scriptA);
	fdActorWaitForScriptEndById(ACTOR_OBABA);
	fdActorFaceDirection(ACTOR_OBABA, FACING_NORTH_NORTHWEST, 0);
	fdActorWaitForScriptEndById(ACTOR_BRIGGS);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST, 0);
	/* "Grandma, it's not what you think! These guys, they..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "Briggs, you just hush up!!! ...It all makes sense now. The gems,
	   the..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "What do you mean? What makes sense?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "They're telling the truth! And you, Briggs! You've been lying to
	   me..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(
		ACTOR_BRIGGS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetMovementTypeById(ACTOR_OBABA, scriptC);
	fdScriptWait(20);
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
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "You got that right! I should have known better than to believe
	   you..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	/* "He only did it for Champa!" */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
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
	fdScriptWait(10);
	fdCameraMoveTo(568, CAMERA_KEEP, 248, 1);
	fdCameraWaitForArrival();
	fdScriptWait(10);
	fdActorSetSprPriority(ACTOR_CHAUCHA, 2);
	fdActorGetById(ACTOR_CHAUCHA)->facing = FACING_SOUTH_SOUTHEAST;
	fdActorGetById(ACTOR_CHAUCHA)->motionFlg = 0;
	fdActorSetPos(ACTOR_CHAUCHA, 520, 200);
	fdActorSetSpeed(ACTOR_CHAUCHA, 1);
	fdActorWalkByAndWaitById(ACTOR_CHAUCHA, 0, 16);
	fdActorFaceDirectionById(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHEAST, 0);
	/* "Chaucha!" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
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
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdScriptWait(10);
	/* "You came!" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
	fdScriptWait(10);
	/* "I heard Felix had shown up looking for you. I was worried..." */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_OBABA, 2);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_OBABA, 1);
	fdActorWalkByAndWait(ACTOR_OBABA, -16, 0);
	fdActorWalkByAndWait(ACTOR_OBABA, -16, 8);
	fdActorWalkByAndWait(ACTOR_OBABA, -16, 16);
	fdActorWalkByAndWait(ACTOR_OBABA, -8, 8);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	/* "What did you mean, Briggs did it for Champa?" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_NORTH_NORTHEAST);
	fdScriptWait(15);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "All of that food... Stolen goods?" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
	/* "No, we paid for everthing we brought back with us." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "But you stole a boat to do it, didn't you?" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
	/* "Grandma, we found those jewels ourselves, and we paid Alhafra well..."
	   Yes: "Your story has the ring of truth to it." No: "So he's lying
	   again?" */
	fdMessageShowAtActor(ACTOR_BRIGGS, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdScriptWait(20);
		fdActorRunRepeatedMotion(ACTOR_OBABA, 2);
		fdScriptWait(5);
		/* "Your story has the ring of truth to it." */
		fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorFaceDirectionAndRefreshById(ACTOR_BRIGGS, FACING_WEST);
		fdScriptWait(10);
		fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
		/* "Thank you, Felix. Your honesty speaks well of you." */
		fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		FieldStateWork->msg += 2;
	} else {
		fdScriptWait(35);
		FieldStateWork->msg += 2;
		/* "So he's lying again?" */
		fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
		fdActorFaceDirectionAndRefreshById(ACTOR_BRIGGS, FACING_WEST);
		fdScriptWait(5);
		fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		/* "Dang it, Felix, I'm not lying!" */
		fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
		fdActorJump(ACTOR_BRIGGS, 6, 15);
		fdActorJump(ACTOR_BRIGGS, 6, 23);
	}
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHEAST);
	fdScriptWait(15);
	fdActorShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "We...were pirates, for a short while. Until we found the jewels." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_CHAUCHA, 2);
	fdScriptWait(5);
	/* "But if not for our efforts, Champa would have starved away long ago." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 5);
	scFieldActorShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "But that doesn't excuse resorting to piracy!" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
	fdScriptWait(10);
	/* "Hey, it's not like we were doing it for the money! We wanted to
	   save..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHEAST);
	fdScriptWait(5);
	/* "Those jewels we found are very valuable. We intend to repay each..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorFaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_EAST);
	fdScriptWait(5);
	/* "Isn't that true?" */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "What's the matter with you!? You said on the boat that we would..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdEffectSetAttached(
		ACTOR_BRIGGS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdScriptWait(10);
	/* "Oh yeah... You're right... We really ought to try to make amends
	   for..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	fdScriptWait(10);
	fdActorFaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_NORTH_NORTHEAST);
	fdScriptWait(10);
	/* "That's the truth, Grandmother. Can you find it in your heart to..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	/* "All right, all right. I'm too old for grudges. But what about Felix?" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
	fdScriptWait(10);
	/* "What do you say, Felix? Can you forgive my husband?" */
	fdMessageShowAtActor(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0);
	fdActorFaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdScriptWait(15);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		fdScriptWait(10);
		/* "I agree, Felix. If each town is repaid what was stolen, I see
		   no..." */
		fdScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 5);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(35);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		fdScriptWait(10);
		FieldStateWork->msg++;
		/* "I don't know, Felix. We didn't come to capture Briggs. Why should
		   we..." */
		fdScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 5);
	}
	fdActorFaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_EAST);
	fdScriptWait(10);
	/* "Do you hear that? They forgive you, too. That means you can leave..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(
		ACTOR_BRIGGS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(50);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdScriptWait(10);
	/* "I... I don't know what to say..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	scFieldActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "You know, you look a little strange..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST);
	fdScriptWait(10);
	/* "Oh... Oh, really?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 15);
	fdActorShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "You haven't caught a cold, have you? Do you have a fever?" */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 35);
	fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
	fdScriptWait(5);
	/* "What are you talking about?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
	/* "No, I'm positive... You look flush, too... You'd better lie down..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
	fdScriptWait(10);
	/* "She...worries about me." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	/* "Hurry up and get into bed so you can get some rest!" */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorWalkByAndWait(ACTOR_CHAUCHA, -8, 0);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
	fdActorWalkByAndWait(ACTOR_CHAUCHA, -8, 0);
	fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorWalkByAndWait(ACTOR_CHAUCHA, -56, 0);
	fdActorFaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
	scFieldActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_EAST);
	fdScriptWait(10);
	/* "Oh, for goodness' sake! You don't want to give them your cold, do..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	fdScriptWait(5);
	fdActorJump(ACTOR_BRIGGS, 6, 20);
	fdActorWatchAndSetScript(ACTOR_FELIX, ACTOR_BRIGGS);
	fdActorWatchAndSetScript(ACTOR_PIERS, ACTOR_BRIGGS);
	fdActorWatchAndSetScript(ACTOR_JENNA, ACTOR_BRIGGS);
	fdActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_BRIGGS);
	fdActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_BRIGGS);
	fdActorSetSpeed(ACTOR_BRIGGS, 2);
	fdActorWalkByAndWaitById(ACTOR_BRIGGS, -80, 0);
	fdScriptWait(10);
	fdActorSetSprPriority(ACTOR_CHAUCHA, 3);
	fdActorSetSprPriority(ACTOR_BRIGGS, 3);
	fdActorSetSpeed(ACTOR_BRIGGS, 1);
	fdActorWalkBy(ACTOR_CHAUCHA, 0, -64);
	fdActorWalkByAndWait(ACTOR_BRIGGS, -16, 0);
	fdActorWalkByById(ACTOR_BRIGGS, 0, -64);
	fdScriptWait(15);
	fdActorSetSprPriority(ACTOR_CHAUCHA, 2);
	fdActorSetSprPriority(ACTOR_BRIGGS, 2);
	fdActorWaitForMoveById(ACTOR_CHAUCHA);
	fdActorSetPos(ACTOR_CHAUCHA, 0, 0);
	fdActorWaitForMoveById(ACTOR_BRIGGS);
	fdActorSetPos(ACTOR_BRIGGS, 0, 0);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTHWEST, 0);
	fdScriptWait(10);
	fdScriptWait(60);
	fdActorWalkByAndWaitById(ACTOR_OBABA, 0, 16);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
	/* "So...what brings you to Champa?" */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
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
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_NOD);
	fdScriptWait(10);
	/* "I mean, you didn't come here to catch Briggs, from what I
	   gathered..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	fdScriptWait(40);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
	/* "Ah, well... To be young and foolish again. If you ever need to..." */
	fdScriptShowMessageThenWait(ACTOR_OBABA, 0, 5);
	fdActorSetSpeed(ACTOR_BRIGGS, 0.7);
	fdActorSetMovementTypeById(ACTOR_OBABA, ChampaCliffCavesChaseToNorthScript);
	fdScriptWait(30);
	fdActorStop(ACTOR_OBABA);
	fdActorWalkToAndWait(ACTOR_OBABA, 648, 188);
	fdActorFaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	fdScriptWait(20);
	fdActorSetSprPriority(ACTOR_FELIX, 2);
	fdActorGetById(ACTOR_FELIX)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(
			ACTOR_JENNA, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(
			ACTOR_SHEBA, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_SHEBA);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdActorSetSpeed(ACTOR_PIERS, 1.2);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(
			ACTOR_PIERS, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_PIERS);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(
			ACTOR_KRADEN, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	syGameFlagSet(FLAG_CHAMPA_AVIMANDER_BEATEN);
	fdScriptEnd();
}

/* scChampaCliffCavesStartFlash's explosion: its tiles, decompressed into the
   heap, and the sprite that shows them. */
typedef struct FLASH_STATE {
	Uint8	   tiles[1024];
	EFFECT_SPR spr;
} FLASH_STATE;

/* The explosion's sprite palette, and the tiles of each of its 16 by 16
   pictures. */
enum { FLASH_PALETTE = 15, FLASH_PICTURE_TILES = 16 * 16 / 64 };

/* Someone or something vanishes in a burst: it spins (`mode` 0) or not, is
   hidden, and the burst plays over it, twice the size in mode 1; then it is
   taken off the map. */
void scChampaCliffCavesStartFlash(Sint32 id, Sint32 mode)
{
	FLASH_STATE *state = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(FLASH_STATE));
	ACTOR		*actor = fdActorGetById(id);
	Sint32		 slot;
	Sint32		 tile;
	EFFECT_SPR	*spr;
	FIXED		 pos[3];
	Sint32		 frm;
	FIXED		 z;

	if(mode == 0) {
		for(frm = 0; frm < 16; frm++) {
			Sint32 ang = actor->facing;
			ang += DEGtoANG(45);
			actor->facing = ang;
			sySynch(1);
		}
	}
	sySynch(2);
	actor->scaleX = 0;
	fdActorSetSprFlags(actor, 0);
	syDMAXCopy(Field_ExplosionPalette,
		(void *)&OBJ_PLTT_COLOR(FLASH_PALETTE, 0),
		DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_FIELD_EFFECT_EXPLOSION), state);
	slot = syVRAMCacheFindFreeEntry();
	tile = mnVRAMLoad(slot, sizeof(state->tiles), state);
	spr = &state->spr;
	syFieldEffectSprInit(spr, 8, 16, OBJ_SIZE_16X16, tile);
	spr->depth = OAM_DEPTH_WINDOW;
	spr->oam.pri = 1;
	spr->oam.fullCol = 0;
	spr->oam.pal = FLASH_PALETTE;
	pos[0] = actor->x.fixed;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed;
	fdCameraWorldToScreen(pos);
	spr->x = pos[0];
	spr->y = z = pos[2];
	if(mode == 1) {
		spr->scaleX = toFIXED(2);
		spr->scaleY = toFIXED(2);
		spr->y = z + toFIXED(10);
	}
	sySndPlay(SE_CRASH);
	for(frm = 0; frm <= 71; frm++) {
		spr = &state->spr;
		switch(frm) {
		case 6:
			spr->oam.idx = tile + 1 * FLASH_PICTURE_TILES;
			break;
		case 12:
			spr->oam.idx = tile + 2 * FLASH_PICTURE_TILES;
			break;
		case 22:
			spr->oam.idx = tile + 3 * FLASH_PICTURE_TILES;
			break;
		case 32:
			spr->oam.idx = tile + 4 * FLASH_PICTURE_TILES;
			break;
		case 42:
			spr->oam.idx = tile + 5 * FLASH_PICTURE_TILES;
			break;
		case 52:
			spr->oam.idx = tile + 6 * FLASH_PICTURE_TILES;
			break;
		case 62:
			spr->oam.idx = tile + 7 * FLASH_PICTURE_TILES;
			break;
		}
		fdEffectSprDraw(spr);
		sySynch(1);
	}
	fdActorSetPos(id, 0, 0);
	syVRAMCacheFreeEntry(slot);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

/* Loads the forge's glow, a glowing disc, into the fire's state, its picture
   moved onto the colours the fire cycles, and sets its sprite at no size. */
void scChampaCliffCavesLoadGlow(void)
{
	CHAMPA_CLIFF_CAVES_STATE *state = Ram_HeapSlots->menuState;
	EFFECT_SPR				 *spr;
	Sint32					  i;
	Uint8					 *tile;
	Sint32					  entry;
	Sint32					  vram;

	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_GLOW_DISC), state);
	for(i = 0, tile = state->tiles; i < (Sint32)sizeof(state->tiles);
		tile++, i++) {
		if((*tile & PIXEL_PALETTE_ROW_MASK) ==
			GLOW_PICTURE_ROW * PALETTE_COLORS)
			*tile = (*tile & PIXEL_ROW_COLOR_MASK) +
				GLOW_CYCLED_ROW * PALETTE_COLORS;
	}
	entry = syVRAMCacheFindFreeEntry();
	vram = syVRAMCacheLoad(entry, 64 * 64, state);
	state->glowEntry = entry;
	state->glowVram = vram;
	spr = &state->glow;
	syFieldEffectSprInit(spr, 31, 31, OBJ_SIZE_64X64, vram);
	spr->oam.fullCol = 1;
	spr->oam.pal = 0;
	spr->oam.pri = 2;
	spr->depth = 1;
	state->glowScale = 0;
	state->glowTrailingScale = 0;
}

/* Loads the forge's column of fire into the fire's state as
   scChampaCliffCavesLoadGlow loads the glow. */
void scChampaCliffCavesLoadColumn(void)
{
	CHAMPA_CLIFF_CAVES_STATE *state = Ram_HeapSlots->menuState;
	EFFECT_SPR				 *spr;
	Sint32					  i;
	Uint8					 *tile;
	Sint32					  entry;
	Sint32					  vram;

	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_FIRE_COLUMN), state);
	for(i = 0, tile = state->tiles; i < (Sint32)sizeof(state->tiles);
		i++, tile++) {
		if((*tile & PIXEL_PALETTE_ROW_MASK) ==
			GLOW_PICTURE_ROW * PALETTE_COLORS)
			*tile = (*tile & PIXEL_ROW_COLOR_MASK) +
				GLOW_CYCLED_ROW * PALETTE_COLORS;
	}
	entry = syVRAMCacheFindFreeEntry();
	vram = syVRAMCacheLoad(entry, 32 * 64, state);
	state->columnEntry = entry;
	state->columnVram = vram;
	spr = &state->column;
	syFieldEffectSprInit(spr, 15, 47, OBJ_SIZE_32X64, vram);
	spr->oam.fullCol = 1;
	spr->oam.pal = 1;
	spr->oam.pri = 1;
	spr->depth = 1;
	state->columnScale = 0;
	state->columnTrailingScale = 0;
}

/* The height the glow and the column are drawn at: 31/40 of the width they had
   the tick before. */
#define FIRE_HEIGHT_SCALE(widthScale) ((widthScale) * 31 / 40)

/* Each tick while Obaba's forge burns, its fire shimmers and pulses: the
   fire's 32 colours ripple through waves of green and blue, a 32nd of a turn
   apart (the first sixteen onto colours 111 down to 96, the rest onto 239 down
   to 224); the glow's and the column's sprites are placed at the fire's spot at
   their sizes; and each one's height is set to trail its width
   (FIRE_HEIGHT_SCALE). */
void scChampaCliffCavesDrawForgeFire(void)
{
	CHAMPA_CLIFF_CAVES_STATE *state = Ram_HeapSlots->menuState;
	Sint32					  pos[3];
	EFFECT_SPR				 *spr;
	Sint32					  i;

	for(i = 0; i < GLOW_COLORS; i++) {
		Sint32 red = RGB_LEVEL_MAX;
		Sint32 green = sySin((state->frm + i) * (TRIG_TURN / 32)) * 6;
		Sint32 blue = sySin((state->frm / 2 + i) * (TRIG_TURN / 32) +
						  TRIG_TURN * 9 / 32) *
			3;
		Sint32 col;
		green = FIXEDtoINT(green);
		blue = FIXEDtoINT(blue);
		green += GLOW_GREEN_MIDDLE;
		blue += GLOW_BLUE_MIDDLE;
		if(i < PALETTE_COLORS) {
			col = RGB(red, green, blue);
			((Uint16 *)OBJ_PLTT)[GLOW_CYCLED_ROW * PALETTE_COLORS +
				PALETTE_COLORS - 1 - i] = col;
		} else {
			col = RGB(red, green, blue);
			((Uint16 *)OBJ_PLTT)[GLOW_PICTURE_ROW * PALETTE_COLORS +
				PALETTE_COLORS - 1 - i] = col;
		}
	}
	pos[0] = state->pos[0];
	pos[1] = state->pos[1];
	pos[2] = state->pos[2];
	spr = &state->glow;
	fdCameraWorldToScreen(pos);
	spr->x = pos[0];
	spr->y = pos[2];
	spr->scaleX = state->glowScale;
	spr->scaleY = state->glowTrailingScale;
	fdEffectSprDraw(spr);
	pos[0] = state->pos[0];
	pos[1] = state->pos[1];
	pos[2] = state->pos[2];
	spr = &state->column;
	fdCameraWorldToScreen(pos);
	spr->x = pos[0];
	spr->y = pos[2];
	spr->scaleX = state->columnScale;
	spr->scaleY = state->columnTrailingScale;
	fdEffectSprDraw(spr);
	state->glowTrailingScale = FIRE_HEIGHT_SCALE(state->glowScale);
	state->columnTrailingScale = FIRE_HEIGHT_SCALE(state->columnScale);
	state->frm++;
}

/* The forge's fire at a spot: the glow grows, holds, and the column rises
   through it, while the room shakes. */
void scChampaCliffCavesStartForgeFire(const Sint32 *pos)
{
	CHAMPA_CLIFF_CAVES_STATE *champaCliffCavesState =
		syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(CHAMPA_CLIFF_CAVES_STATE));
	Sint32 ticks;
	Sint32 state;

	champaCliffCavesState->pos[0] = pos[0];
	champaCliffCavesState->pos[1] = pos[1];
	champaCliffCavesState->pos[2] = pos[2];
	scChampaCliffCavesLoadGlow();
	scChampaCliffCavesLoadColumn();
	champaCliffCavesState->frm = 0;
	ticks = 0;
	sySetEvent(scChampaCliffCavesDrawForgeFire, EVENT_PASS_TICK);
	fdMapGridSetShake(4, 4, 1);
	state = 0;
	do {
		switch(state) {
		case 0:
			if((champaCliffCavesState->glowScale += toFIXED(3) / 128) >=
				toFIXED(13) / 10) {
				champaCliffCavesState->glowScale = toFIXED(13) / 10;
				state = 1;
				ticks = -1;
			}
			break;
		case 1:
			if(ticks == 10) {
				state = 2;
				ticks = -1;
			}
			break;
		case 2:
			if((champaCliffCavesState->columnScale += toFIXED(1) / 64) >=
				toFIXED(2) / 5) {
				champaCliffCavesState->columnScale = toFIXED(2) / 5;
				state = 3;
				ticks = -1;
			}
			break;
		case 3:
			if((champaCliffCavesState->columnScale += toFIXED(3) / 16) >=
				toFIXED(13) / 10) {
				champaCliffCavesState->columnScale = toFIXED(13) / 10;
				state = GLOW_DONE;
			}
			break;
		}
		syWait(1);
		ticks++;
	} while(state != GLOW_DONE);
}

/* Unused: puts out the forge's fire: the column and then the glow shrink away
   and the fire's state is released. */
UNUSED void scChampaCliffCavesPutOutForgeFire(void)
{
	CHAMPA_CLIFF_CAVES_STATE *champaCliffCavesState = Ram_HeapSlots->menuState;
	FIXED					  pos[3];
	Sint32					  state;
	Sint32					  ticks;

	pos[0] = champaCliffCavesState->pos[0];
	pos[1] = champaCliffCavesState->pos[1] - toFIXED(32);
	state = 0;
	ticks = 0;
	pos[2] = champaCliffCavesState->pos[2];
	do {
		switch(state) {
		case 0:
			if((champaCliffCavesState->columnScale -= toFIXED(1) / 16) < 0) {
				champaCliffCavesState->columnScale = 0;
				ticks = -1;
				state = 1;
			}
			break;
		case 1:
			if((champaCliffCavesState->glowScale -= toFIXED(1) / 32) < 0) {
				champaCliffCavesState->glowScale = 0;
				state = 2;
				ticks = -1;
			}
			break;
		case 2:
			if(ticks == 1)
				state = GLOW_DONE;
			break;
		}
		sySynch(1);
		ticks++;
	} while(state != GLOW_DONE);
	syCloseEvent(scChampaCliffCavesDrawForgeFire);
	fdMapGridSetShake(0, 0, 0);
	syVRAMCacheFreeEntry(champaCliffCavesState->glowEntry);
	syVRAMCacheFreeEntry(champaCliffCavesState->columnEntry);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

/* The forge lights at a spot: the glow alone grows to full size. */
void scChampaCliffCavesLightForge(const VECTOR3 *origin)
{
	CHAMPA_CLIFF_CAVES_STATE *champaCliffCavesState =
		syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(CHAMPA_CLIFF_CAVES_STATE));
	Sint32 state;

	champaCliffCavesState->pos[0] = origin->x;
	champaCliffCavesState->pos[1] = origin->y;
	champaCliffCavesState->pos[2] = origin->z;
	scChampaCliffCavesLoadGlow();
	champaCliffCavesState->columnScale = 0;
	champaCliffCavesState->columnTrailingScale = 0;
	champaCliffCavesState->frm = 0;
	sySetEvent(scChampaCliffCavesDrawForgeFire, EVENT_PASS_TICK);
	sySndPlay(SE_DJINNI_SET);
	state = 0;
	do {
		if(state == 0) {
			champaCliffCavesState->glowScale += toFIXED(1) / 16;
			if(champaCliffCavesState->glowScale >= toFIXED(13) / 10) {
				champaCliffCavesState->glowScale = toFIXED(13) / 10;
				state = GLOW_DONE;
			}
		}
		sySynch(1);
	} while(state != GLOW_DONE);
}

/* The column of fire rises through the glow already burning. */
void scChampaCliffCavesRaiseColumn(void)
{
	CHAMPA_CLIFF_CAVES_STATE *champaCliffCavesState =
		syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(CHAMPA_CLIFF_CAVES_STATE));
	Sint32 state;

	scChampaCliffCavesLoadColumn();
	sySetEvent(scChampaCliffCavesDrawForgeFire, EVENT_PASS_TICK);
	sySndPlay(SE_CLATTER);
	state = 0;
	do {
		switch(state) {
		case 0:
			champaCliffCavesState->columnScale += toFIXED(1) / 16;
			if(champaCliffCavesState->columnScale >= toFIXED(2) / 5) {
				champaCliffCavesState->columnScale = toFIXED(2) / 5;
				state = 1;
			}
			break;
		case 1:
			champaCliffCavesState->columnScale += toFIXED(3) / 16;
			if(champaCliffCavesState->columnScale >= toFIXED(13) / 10) {
				champaCliffCavesState->columnScale = toFIXED(13) / 10;
				state = GLOW_DONE;
			}
			break;
		}
		sySynch(1);
	} while(state != GLOW_DONE);
}

/* The forge's last flare: the column flares to twice its size, then it and the
   glow die away and the fire's state is released. */
void scChampaCliffCavesFlareAndDieDown(void)
{
	CHAMPA_CLIFF_CAVES_STATE *champaCliffCavesState = Ram_HeapSlots->menuState;
	Sint32					  state = 0;
	Sint32					  time = 0;

	do {
		switch(state) {
		case 0:
			if((champaCliffCavesState->columnScale += toFIXED(1) / 16) >=
				toFIXED(2)) {
				champaCliffCavesState->columnScale = toFIXED(2);
				state = 1;
				time = -1;
			}
			break;
		case 1:
			if((champaCliffCavesState->columnScale -= toFIXED(1) / 16) <
				toFIXED(13) / 10) {
				champaCliffCavesState->columnScale = toFIXED(13) / 10;
				state = 2;
				time = -1;
			}
			/* fall through */
		case 2:
			if((champaCliffCavesState->columnScale -= toFIXED(1) / 16) < 0)
				champaCliffCavesState->columnScale = 0;
			if((champaCliffCavesState->glowScale -= toFIXED(1) / 16) < 0) {
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
	syVRAMCacheFreeEntry(champaCliffCavesState->glowEntry);
	syVRAMCacheFreeEntry(champaCliffCavesState->columnEntry);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

/* The three prongs Obaba throws into her forge, in turn. */
TRIDENT_PRONGS ChampaCliffCavesProngs = {
	{ITEM_RIGHT_PRONG, ITEM_LEFT_PRONG, ITEM_CENTER_PRONG}};

#undef DEFINE_CHAMPA_CLIFF_CAVES_GROW

/* clang-format off */
LATE_DATA SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

LATE_DATA Uint32 SceneExits[] = {
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

LATE_DATA ACTOR_PLACE ChampaCliffCavesBriggsPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 568,
					FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 216, 0, 224,
					FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE ChampaCliffCavesHousesPlacements[] = {
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

LATE_DATA ACTOR_PLACE ChampaCliffCavesObabaPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_OBABA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
					FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 200, 0, 424, FACING_SOUTH_SOUTHWEST,
					TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE ChampaCliffCavesAvimanderPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_OBABA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
					FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 616, 0, 216, FACING_SOUTH_SOUTHEAST,
					TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE ChampaCliffCavesBriggsPlacementsBriggsHome[] =
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

LATE_DATA ACTOR_PLACE ChampaCliffCavesHousesPlacementsBriggsHome[] =
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

LATE_DATA ACTOR_PLACE ChampaCliffCavesObabaPlacementsBriggsHome[] =
	{
		ACTOR_PLACEMENT(
			SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520,
			0, 392, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
		ACTOR_PLACEMENT(
			SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184,
			0, 424, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
		{SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE ChampaCliffCavesAvimanderPlacementsBriggsHome[] = {
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
	ChampaCliffCavesBriggsPlacementsAfterAvimander[] = {
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
	ChampaCliffCavesHousesPlacementsAfterAvimander[] = {
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
	ChampaCliffCavesObabaPlacementsAfterAvimander[] = {
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

LATE_DATA ACTOR_PLACE ChampaCliffCavesAvimanderPlacementsAfterAvimander[] = {
	ACTOR_PLACEMENT(SPRITE_OBABA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 188, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 464, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* Obaba's door opening and closing, as MAPGRID_ANIMATION_STEP rows: two */
/* steps of six ticks each. */
LATE_DATA Sint16 ChampaCliffCavesOpenDoorFrames[] = {
	1, 64, 1, 2, 6, 2, 64, 1, 2, 6, -1,
};
LATE_DATA Sint16 ChampaCliffCavesCloseDoorFrames[] = {
	1, 64, 1, 2, 6, 0, 64, 1, 2, 6, -1,
};

/* Obaba's door opening and closing. */
LATE_DATA DOOR_EDIT ChampaCliffCavesOpenDoor = {
	ChampaCliffCavesOpenDoorFrames, 11, 88};
LATE_DATA DOOR_EDIT ChampaCliffCavesCloseDoor = {
	ChampaCliffCavesCloseDoorFrames, 11, 88};

LATE_DATA CUE ChampaCliffCavesBriggsCues[] = {
	{CUE_EXIT, CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA_CLIFF_CAVES_HOUSES, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_BRIGGS_TO_CHAMPA_CLIFF_CAVES_HOUSES},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_TALK, ACTOR_WOMAN_MISSING_HER_SWEETHEART, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
	 MSGNO(MsgArenTYou)},
	{CUE_TALK, ACTOR_WOMAN_SCORNING_HARBOR_SAILORS, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
	 MSGNO(MsgIThinkThose)},
	{CUE_TALK, ACTOR_SAILOR_WAITING_FOR_BRIGGS, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
	 MSGNO(MsgWeReGoing)},
	{CUE_TALK, ACTOR_SAILOR_ON_THE_ISLAND_TREASURE, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
	 MSGNO(MsgThereWerePlenty)},
	{CUE_TALK, ACTOR_SAILOR_WANTING_AMENDS, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
	 MSGNO(MsgWeReallyWant)},
	{CUE_TALK, ACTOR_SAILOR_WHO_HIT_THE_SWITCH, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
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
	{CUE_TALK, ACTOR_SAILOR_CALLING_FELIX_A_BULLY, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
	 MSGNO(MsgWhyDoYou)},
	{CUE_TALK, ACTOR_SAILOR_WHO_SURRENDERS, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
	 SCRIPT_REF(scChampaCliffCavesSailorSurrenders)},
	{CUE_TALK, ACTOR_WOMAN_IN_HER_DOORWAY, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
	 SCRIPT_REF(scChampaCliffCavesAskToComeIn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_CALLING_FELIX_A_BULLY,
	 IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgMaybeIfI)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WHO_SURRENDERS,
	 IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgHeSAfter)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_HER_DOORWAY,
	 IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgToughOrNot)},
	{CUE_TALK, ACTOR_WOMAN_MISSING_HER_SWEETHEART, CONDITION_ALWAYS, MSGNO(MsgAllOfOurMen)},
	{CUE_TALK, ACTOR_WOMAN_SCORNING_HARBOR_SAILORS, CONDITION_ALWAYS, MSGNO(MsgTheChampaSailors)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_MISSING_HER_SWEETHEART, CONDITION_ALWAYS,
	 MSGNO(MsgBriggsIsThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SCORNING_HARBOR_SAILORS, CONDITION_ALWAYS,
	 MSGNO(MsgIfYouLive)},
	{CUE_TABLE_END},
};

LATE_DATA CUE ChampaCliffCavesHousesCues[] = {
	{CUE_EXIT, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_BRIGGS, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_BRIGGS},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_OBABA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_HOUSES_TO_CHAMPA_CLIFF_CAVES_OBABA},
	{CUE_TALK, ACTOR_MAN_KEEPING_WATCH, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
	 MSGNO(MsgWhyDidYou)},
	{CUE_TALK, ACTOR_SAILORS_WIFE, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
	 MSGNO(MsgWeHaveSo)},
	{CUE_TALK, ACTOR_SAILORS_SON, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
	 MSGNO(MsgBriggsCameDown)},
	{CUE_TALK, ACTOR_SAILORS_DAUGHTER, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
	 MSGNO(MsgEoleoStillWon)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_WATCH,
	 IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIJustKnow)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_WIFE,
	 IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgBriggsSaysHe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_SON,
	 IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgBriggsSNever)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_DAUGHTER,
	 IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgEoleoIsYounger)},
	{CUE_TALK, ACTOR_SAILOR_RATHER_FIGHTING, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
	 MSGNO(MsgIDRatherFight)},
	{CUE_TALK, ACTOR_SAILOR_WHO_KNOWS_FELIX, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
	 MSGNO(MsgYouReAren)},
	{CUE_TALK, ACTOR_SAILORS_SON, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
	 MSGNO(MsgWhySEveryone)},
	{CUE_TALK, ACTOR_SAILORS_DAUGHTER, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
	 MSGNO(MsgIfYouSo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_RATHER_FIGHTING,
	 IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgItWouldBe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WHO_KNOWS_FELIX,
	 IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgBriggsSaidThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_SON,
	 IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgHeSNot)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_DAUGHTER,
	 IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhatSHe)},
	{CUE_TALK, ACTOR_MAN_KEEPING_WATCH, CONDITION_ALWAYS, MSGNO(MsgBeforeHeLeft)},
	{CUE_TALK, ACTOR_SAILORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgMyHusbandWants)},
	{CUE_TALK, ACTOR_SAILORS_SON, CONDITION_ALWAYS, MSGNO(MsgSomedayIWant)},
	{CUE_TALK, ACTOR_SAILORS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMomSHad)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_WATCH, CONDITION_ALWAYS,
	 MSGNO(MsgBriggsMayBe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_WIFE, CONDITION_ALWAYS,
	 MSGNO(MsgWhenIMarried)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_SON, CONDITION_ALWAYS,
	 MSGNO(MsgHasMomForgotten)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_DAUGHTER, CONDITION_ALWAYS,
	 MSGNO(MsgEoleoIsThe)},
	{CUE_TABLE_END},
};

LATE_DATA CUE ChampaCliffCavesObabaCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_OBABAS_DOOR,
	 IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
	 SCRIPT_REF(scChampaCliffCavesEnterDoor)},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_HOUSES, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_HOUSES},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_OBABAS_DOOR, CONDITION_ALWAYS,
	 SCRIPT_REF(scChampaCliffCavesObabaAsksForBriggs)},
	{CUE_EXIT, 16, CONDITION_ALWAYS, 16},
	{CUE_EXIT, 17, CONDITION_ALWAYS, 17},
	{CUE_EXIT, 18, CONDITION_ALWAYS, 18},
	{CUE_EXIT, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_AVIMANDER, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_OBABA_TO_CHAMPA_CLIFF_CAVES_AVIMANDER},
	{CUE_TALK, ACTOR_BRIGGS_IN_BED, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
	 MSGNO(MsgOnceIM)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_CHAUCHA_BY_THE_BED,
	 IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIWonderWhatS)},
	{CUE_TALK, ACTOR_EOLEO, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
	 MSGNO(MsgBabuBaBubabu)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIGGS_IN_BED,
	 IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIMCertain)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHAUCHA_BY_THE_BED,
	 IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgThereIsAbsolutely)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EOLEO,
	 IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgOneOfPapa)},
	{CUE_TALK, ACTOR_SAILOR_GUARDING_THE_FAMILIES, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
	 MSGNO(MsgLeaveTheWomen)},
	{CUE_TALK, ACTOR_SAILOR_SPOILING_FOR_A_FIGHT, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
	 MSGNO(MsgDidYouReally)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_GUARDING_THE_FAMILIES,
	 IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhatAreThey)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_SPOILING_FOR_A_FIGHT,
	 IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhyWouldBriggs)},
	{CUE_TALK, ACTOR_MAN_AT_OBABAS_DOOR, CONDITION_ALWAYS, MSGNO(MsgObabaSNot)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AT_OBABAS_DOOR, CONDITION_ALWAYS,
	 MSGNO(MsgIVeNeverSeen)},
	{CUE_EXAMINE, METATILE_BEHAVIOR_KNOCK_AT_OBABAS_DOOR, FLAG_CHAMPA_AVIMANDER_BEATEN,
	 SCRIPT_REF(scChampaCliffCavesObabaAsksForBriggs)},
	{CUE_TABLE_END},
};

LATE_DATA CUE ChampaCliffCavesAvimanderCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, CHAMPA_CLIFF_CAVES_AVIMANDER_TO_CHAMPA_CLIFF_CAVES_OBABA, CONDITION_ALWAYS, CHAMPA_CLIFF_CAVES_AVIMANDER_TO_CHAMPA_CLIFF_CAVES_OBABA},
	{CUE_EXIT, 22, CONDITION_ALWAYS, 22},
	{CUE_EXIT, 23, CONDITION_ALWAYS, 23},
	{CUE_STEP, METATILE_BEHAVIOR_FORGE_ROOM_WAY_IN, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
	 SCRIPT_REF(scChampaCliffCavesFightAvimander)},
	{CUE_TALK, ACTOR_OBABA, CONDITION_ALWAYS,
	 SCRIPT_REF(scChampaCliffCavesObabaOffersHelp)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OBABA, FLAG_OBABA_SAW_FIRST_PRONG,
	 MSGNO(MsgNowIVe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OBABA, FLAG_OBABA_SAW_SECOND_PRONG,
	 MSGNO(MsgAllOfMy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OBABA, FLAG_TRIDENT_FORGED,
	 MSGNO(MsgSoonILl)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OBABA, CONDITION_ALWAYS,
	 MSGNO(MsgAllOfMyTraining)},
	{CUE_TALK, ACTOR_HELD_ITEM, CONDITION_ALWAYS, SCRIPT_REF(scChampaCliffCavesPickUpTrident)},
	{CUE_TALK, ACTOR_FORGE_ROOM_SAILOR, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
	 MSGNO(MsgYouGuysDon)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORGE_ROOM_SAILOR,
	 IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgMustBeIncredibly)},
	{CUE_TALK, ACTOR_FORGE_ROOM_SAILOR, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
	 SCRIPT_REF(scChampaCliffCavesSailorTurnsAway)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORGE_ROOM_SAILOR,
	 IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgBriggsIsPretty)},
	{CUE_TALK, ACTOR_FORGE_ROOM_SAILOR, CONDITION_ALWAYS, MSGNO(MsgThatPitIs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORGE_ROOM_SAILOR, CONDITION_ALWAYS,
	 MSGNO(MsgIWonderHow)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
	 CONDITION_ALWAYS, SCRIPT_REF(scChampaCliffCavesRevealPath)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
	 CONDITION_ALWAYS, SCRIPT_REF(scChampaCliffCavesHidePath)},
	{CUE_TABLE_END},
};

/* Obaba chasing Briggs round her forge: the four quarters of the way round, */
/* each walked to its end. */
LATE_DATA Uint32 ChampaCliffCavesChaseToNorthScript[] = {
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

LATE_DATA Uint32 ChampaCliffCavesChaseToEastScript[] = {
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

LATE_DATA Uint32 ChampaCliffCavesChaseToSouthScript[] = {
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

LATE_DATA Uint32 ChampaCliffCavesChaseToWestScript[] = {
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
