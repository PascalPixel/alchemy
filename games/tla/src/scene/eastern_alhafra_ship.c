/* Briggs's ship at Alhafra's eastern harbour: the cabin the party enters from
   the deck, the rooms down its stairs, and a back room behind a tall crate.
   Below, the party overhears Briggs and his mate learn that Madra's mayor has
   come (scEasternAlhafraShipMayorArrives) and fights Briggs and a Sea Fighter;
   after the battle Briggs gives himself up and the mayors of Madra and Alhafra
   settle his fate and his ship's (scEasternAlhafraShipBriggsIsCaught). His wife
   Chaucha and their baby Eoleo then stay aboard, where Eoleo lifts a vase of
   flowers with Psynergy (scEasternAlhafraShipEoleoLifts). Move or a push shifts
   the tall crate from the doorway to the back room, and Tremor knocks the Large
   Bread down from the crates stacked inside
   (scEasternAlhafraShipKnockDownTheBread): given to the crying boy in Alhafra,
   it has his father let the party into the Alhafran Cave. */

#include "field/field_mode.h"
#include "game/game.h"
#include "item_ids.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "field/script_inline.h"
#include "text/msg_ids.h"
#include "field/actor_script.h"

/* The tall crate pushed or Moved off the back room's door, east onto metatile
   (24, 40). */
#define FLAG_CRATE_MOVED		0x900
/* Tremor has shaken the stacked crates down off the crate under them and
   knocked the Large Bread to the floor. */
#define FLAG_BREAD_KNOCKED_DOWN 0x901
/* The Large Bread has been picked up. */
#define FLAG_BREAD_TAKEN		0x902
/* Set the first time the baby Eoleo lifts the vase with Psynergy; his mother
   tells of his power only then. */
#define FLAG_EOLEO_LIFTED		0x8ae

/* Due north and due south nudged by the least angle (a 65536th of a turn)
   toward the west or the east. */
#define FACING_NORTH_BY_WEST (FACING_NORTH - 1)
#define FACING_NORTH_BY_EAST (FACING_NORTH + 1)
#define FACING_SOUTH_BY_WEST (FACING_SOUTH + 1)

/* Where the party comes back after the battle with Briggs and a Sea Fighter of
   his crew, numbered 99 as The Broken Seal's Altin Peak numbers its own, and
   the entrance it then counts as: the foot of the stairs down from the room the
   party boards into. */
enum { ENTRANCE_FOOT_OF_STAIRS = 3, ENTRANCE_AFTER_BRIGGS_BATTLE = 99 };

/* The metatiles just below the foot of those stairs, (28, 24) and (29, 24),
   where the party first overhears Briggs (scEasternAlhafraShipMayorArrives). */
enum { METATILE_BELOW_THE_STAIRS = 20 };

/* The people and things aboard, numbering the rows of
   EasternAlhafraShipActorPlacements whose sprites are not the party's. */
typedef enum EASTERN_ALHAFRA_SHIP_ACTOR_ID {
	ACTOR_BRIGGS = ACTOR_FIRST_PLACED,
	/* Briggs's mate in the cabin: "He knew we needed a ship, and he knew we
	   went east." */
	ACTOR_BRIGGS_MATE,
	ACTOR_CHAUCHA,
	ACTOR_EOLEO,
	/* The vase of flowers Eoleo lifts with Psynergy. */
	ACTOR_VASE,
	/* The tall crate before the back room's door, which Move or a push shifts
	   aside (FLAG_CRATE_MOVED). */
	ACTOR_TALL_CRATE,
	/* The stacked crates standing on another in the back room, which Tremor
	   shakes down beside it. */
	ACTOR_STACKED_CRATES,
	/* The Large Bread on top of them, an item icon on an unseen marker. */
	ACTOR_LARGE_BREAD,
	/* Those who come aboard after the battle, waiting off the map until
	   then. */
	ACTOR_ALHAFRAS_MAYOR,
	ACTOR_MADRAS_MAYOR,
	ACTOR_MADRAS_ELDER,
	ACTOR_MADRAN_SOLDIER,
	/* Alhafra's soldiers. */
	ACTOR_ALHAFRAN_SOLDIER,
	ACTOR_SECOND_ALHAFRAN_SOLDIER,
	/* The rest of Briggs's crew, who come aboard with him after the battle. */
	ACTOR_FIRST_PIRATE,
	ACTOR_SECOND_PIRATE,
	ACTOR_THIRD_PIRATE,
	ACTOR_KRADEN
} EASTERN_ALHAFRA_SHIP_ACTOR_ID;

extern ACTOR_PLACE EasternAlhafraShipActorPlacements[];
void			   scEasternAlhafraShipBriggsIsCaught(void);
extern Uint32	   EasternAlhafraShipBriggsRunsScript[];
extern Sint32	   EasternAlhafraShipMateRunsScript[];

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

Sint32 scGetActorPlacements(void)
{
	return (Sint32)EasternAlhafraShipActorPlacements;
}

/* Nothing visible happens as Felix begins to push or Move the tall crate in
   front of the back room's door; the push or Move goes on as anywhere else. */
void scEasternAlhafraShipStartMovingCrate(void)
{
}

/* Felix pushes or Moves the tall crate standing against the north wall below
   the stairs. Shifted east to metatile (24, 40), it uncovers a dark doorway in
   the wall to the back room, and stays aside when the party comes back; shifted
   back west to (23, 40), it covers the doorway again. */
void scEasternAlhafraShipFinishMovingCrate(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGetById(id);
	Sint32 x = FIELD_METATILE(actor->x.fixed);
	Sint32 z = FIELD_METATILE(actor->z.fixed);

	if(x == 24 && z == 40)
		syGameFlagSet(FLAG_CRATE_MOVED);
	if(x == 23 && z == 40)
		syGameFlagClear(FLAG_CRATE_MOVED);
}

/* Felix casts Tremor on the stacked crates in the back room: he hops back as
   they shift down beside the crate under them, and the Large Bread on top falls
   to the floor, where he can pick it up. */
void scEasternAlhafraShipKnockDownTheBread(void)
{
	syGameFlagSet(FLAG_BREAD_KNOCKED_DOWN);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->velY = toFIXED(6);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 2);
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 136, 488);
	fdScriptWait(10);
	fdActorSetDestination(ACTOR_LARGE_BREAD, 120, 472);
	fdActorMoveToAndWaitById(ACTOR_STACKED_CRATES, 136, 475);
	fdActorGetById(ACTOR_LARGE_BREAD)->velY = toFIXED(6);
	sySndPlay(SE_POUND);
	fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdScriptWait(20);
	fdActorWaitForMoveById(ACTOR_LARGE_BREAD);
	sySndPlay(SE_DOOR_CLOSE);
	return;
}

/* Felix picks up the Large Bread that Tremor knocked to the floor of the back
   room. */
void scEasternAlhafraShipTakeTheBread(Sint32 id)
{
	fdScriptPickUpItem(id, ITEM_LARGE_BREAD, FLAG_BREAD_TAKEN);
	return;
}

/* Felix talks to the baby Eoleo, who stays aboard with his mother Chaucha once
   Briggs is caught: "Goo goo ga ga!" He lifts the vase of flowers beside him
   with Psynergy and drops it with a splash. The first time, Chaucha says he
   only lately started doing that. The screen is tinted toward yellow
   (`fdStartColTransition`) while he holds the vase up. */
void scEasternAlhafraShipEoleoLifts(Sint32 id)
{
	ACTOR *actor = fdActorGetById(ACTOR_VASE);
	Uint8 *flg;
	Sint32 i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgGooGooGa);
	/* "Goo goo ga ga!" */
	fdScriptShowMessageAndWait(ACTOR_EOLEO, 0);
	fdStartColTransition(8, RGB(16, 16, 4));
	fdBattleEffectStartArcEffect();
	/* Eoleo cycles through colours with swelling arcs, as a caster does. */
	fdBattleEffectSetObjectUpdateMode(
		(Sint32)fdActorGetById(id), CASTER_LOOK_CYCLING_COLORS);
	sySndPlay(SE_ASCEND);
	fdScriptWait(30);
	flg = &actor->motionFlg;
	*flg = ACTOR_FLOATS;
	for(i = 0; i < 40; i++) {
		actor->y.fixed += toFIXED(1) / 5;
		fdScriptWait(1);
	}
	fdScriptWait(70);
	fdBattleEffectSetObjectUpdateMode(
		(Sint32)fdActorGetById(id), CASTER_LOOK_NORMAL);
	fdBattleEffectWaitForArcGroups();
	fdBattleEffectEndArcEffect();
	fdStartPaletteTransition(8);
	*flg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	actor->gravity = toFIXED(1) / 2;
	fdScriptWait(5);
	sySndPlay(SE_WATER);
	if(fdScriptGetFlag(FLAG_EOLEO_LIFTED) == 0) {
		syGameFlagSet(FLAG_EOLEO_LIFTED);
		fdScriptWait(30);
		fdActorFaceDirectionById(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHEAST, 0);
		fdScriptWait(30);
		fdActorRunRepeatedMotion(ACTOR_CHAUCHA, 2);
		fdScriptWait(10);
		/* "Eoleo's only just recently started doing that... After that
		   gem..." */
		fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
		fdActorFaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST - 1, 0);
	}
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Sets up the ship as the story stands: back from the battle, Briggs gives
   himself up; once he is caught his crew are gone, and Chaucha and Eoleo stay
   aboard after their visit to him, until the mast is cleared; and the tall
   crate, the stacked crates and the Large Bread are where the party left
   them. */
Sint32 scInit(void)
{
	ACTOR *actor;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	if(MainRoutineStateWork.entrance == ENTRANCE_AFTER_BRIGGS_BATTLE) {
		syGameFlagSet(FLAG_MAYOR_ARRIVED_AT_THE_SHIP);
		syGameFlagSet(FLAG_BRIGGS_CAUGHT);
		MainRoutineStateWork.returnEntrance = ENTRANCE_FOOT_OF_STAIRS;
		MainRoutineStateWork.entrance = ENTRANCE_FOOT_OF_STAIRS;
		scEasternAlhafraShipBriggsIsCaught();
	}
	if(syGameFlagGet(FLAG_ALHAFRA_MAST_CLEARED)) {
		fdActorSetPos(ACTOR_CHAUCHA, 0, 0);
		fdActorSetPos(ACTOR_EOLEO, 0, 0);
		fdActorSetPos(ACTOR_BRIGGS, 0, 0);
		fdActorSetPos(ACTOR_BRIGGS_MATE, 0, 0);
	} else if(syGameFlagGet(FLAG_BRIGGS_CAUGHT)) {
		fdActorSetPos(ACTOR_BRIGGS, 0, 0);
		fdActorSetPos(ACTOR_BRIGGS_MATE, 0, 0);
		if(!syGameFlagGet(FLAG_FAMILY_VISITED_BRIGGS)) {
			fdActorSetPos(ACTOR_CHAUCHA, 0, 0);
			fdActorSetPos(ACTOR_EOLEO, 0, 0);
		}
	} else {
		fdActorSetPos(ACTOR_CHAUCHA, 0, 0);
		fdActorSetPos(ACTOR_EOLEO, 0, 0);
	}
	if(syGameFlagGet(FLAG_CRATE_MOVED))
		fdActorSetPos(ACTOR_TALL_CRATE, 392, 648);
	fdActorGetById(ACTOR_TALL_CRATE)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorShowItemIconById(ACTOR_LARGE_BREAD, ITEM_LARGE_BREAD);
	if(!syGameFlagGet(FLAG_BREAD_KNOCKED_DOWN)) {
		if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			fdActorGetById(ACTOR_STACKED_CRATES)->priFlg |=
				ACTOR_PRIORITY_PLATFORM;
			actor = fdActorGetById(ACTOR_LARGE_BREAD);
			actor->priFlg |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
			actor->y.fixed += toFIXED(16);
			actor->groundHeight += toFIXED(16);
		}
	} else {
		fdActorSetPos(ACTOR_STACKED_CRATES, 136, 475);
		if(!syGameFlagGet(FLAG_BREAD_TAKEN))
			fdActorSetPos(ACTOR_LARGE_BREAD, 120, 472);
		else
			fdActorSetPos(ACTOR_LARGE_BREAD, 0, 0);
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

extern Uint32 QuestionMarkScript[];

#include "shared/question_mark_pop.h"

#include "shared/actor_pop_question_marks.h"

extern Uint32 EasternAlhafraShipLeaveUpTheStairsScript[];

/* Walking down from the foot of the stairs (metatiles (28, 24) and (29, 24)) on
   the first visit, the party overhears Briggs and his mate: Madra's mayor has
   come to Alhafra after them. Jenna's outburst gives the party away, and Briggs
   and a Sea Fighter of his crew fight it. */
void scEasternAlhafraShipMayorArrives(void)
{
	MAP_STATE *map;
	Sint32	   result;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgTheMayorOf);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	map = Ram_HeapSlots->mapGrid;
	map->minX = 0;
	map->maxX = toFIXED(1024);
	map->minZ = 0;
	map->maxZ = toFIXED(1024);
	/* "The mayor of Madra? Here?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorWalkToAndWait(ACTOR_FELIX, 456, 408);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 0, -16, FACING_SOUTH);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 16, 0, FACING_SOUTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 16, -16, FACING_SOUTH);
	fdActorWaitForMoveById(ACTOR_SHEBA);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdScriptWait(20);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_SOUTHWEST, 0);
	fdScriptWait(20);
	fdCameraMoveTo(376, CAMERA_KEEP, 416, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	fdScriptWait(10);
	/* "You're certain?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS_MATE, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(30);
	/* "How did he know we came to Alhafra?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorJump(ACTOR_BRIGGS_MATE, 6, 15);
	fdActorJump(ACTOR_BRIGGS_MATE, 6, 23);
	/* "He knew we needed a ship, and he knew we went east. It seems
	   pretty..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
	fdActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH, 0);
	fdScriptWait(20);
	/* "Arr... Well, yeah... When you put it that way, it does make sense..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS_MATE, ANIM_NOD);
	fdScriptWait(10);
	/* "He was looking to find the mayor of Alhafra..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
	fdActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "He must be hoping the mayor will tell him where we are..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS_MATE, 2);
	fdScriptWait(10);
	/* "He'll probably tell the Alhafrans we're pirates, too, huh?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(30);
	/* "Pirates!? That seems a little unfair!" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "What do you mean? We...are pirates, aren't we? I mean, we've
	   taken..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	fdScriptWait(10);
	/* "That's only because everyone in Champa would starve otherwise! We..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Ah..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdScriptWait(10);
	/* "And even then, it's never a lot." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "Wha-What's not?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
	/* "The stuff we take. We never take too much from any one town." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS_MATE, ANIM_NOD);
	fdScriptWait(10);
	/* "Yeah, I guess not..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
	fdActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "And why do you think that is?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_BRIGGS_MATE, 3);
	fdActorSetAttachedEffect(
		ACTOR_BRIGGS_MATE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorFaceDirectionById(ACTOR_BRIGGS_MATE, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(30);
	scActorPopQuestionMarks(ACTOR_BRIGGS_MATE);
	sySndPlay(SE_LIGHT_APPEAR);
	fdActorShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirectionById(ACTOR_BRIGGS_MATE, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(30);
	/* "Er... Why?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdScriptWait(10);
	/* "Because you never can tell how these wealthy towns will react.
	   It's..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS_MATE, 2);
	fdScriptWait(10);
	/* "Hey, yeah! Which is why we didn't steal anything from Alhafra..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdScriptWait(10);
	/* "Right! That's why we avoid the risky towns and just take what we..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "See? That's why you're our captain, Briggs! You've got brains
	   coming..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "Hmph! There's nothing brainy about that!" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(
		ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(30);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_BRIGGS_MATE, FACING_EAST, 0);
	fdActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmoteById(
		ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdScriptWait(20);
	/* "Did you hear that? It sounded like a girl! Who's out there!?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorSetSpeed(ACTOR_BRIGGS, 1.5);
	fdActorSetSpeed(ACTOR_BRIGGS_MATE, 1.5);
	fdActorSetMovementTypeById(
		ACTOR_BRIGGS_MATE, (Sint32)EasternAlhafraShipMateRunsScript);
	fdScriptWait(5);
	fdActorSetMovementTypeById(
		ACTOR_BRIGGS, (Sint32)EasternAlhafraShipBriggsRunsScript);
	fdActorWaitForScriptEndById(ACTOR_BRIGGS);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdCameraMoveTo(448, CAMERA_KEEP, 416, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorJump(ACTOR_BRIGGS_MATE, 6, 15);
	fdActorJump(ACTOR_BRIGGS_MATE, 6, 23);
	/* "What do you think you're doing?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(20);
	/* "Don't you know that someone has been falsely imprisoned for your
	   crimes?" */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Er... What's your point?" */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(10);
	/* "You have to come with us and attest to that man's innocence!" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_BRIGGS_MATE, FACING_WEST, 0);
	fdScriptWait(45);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_NORTH, 0);
	fdActorFaceDirectionById(ACTOR_BRIGGS_MATE, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(20);
	/* "And...why would I want to do something like that?" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(10);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Oh-ho! I was hoping we could avoid any ugliness, but you leave us
	   no..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdScriptWait(20);
	fdActorJump(ACTOR_KRADEN, 6, 15);
	fdActorJump(ACTOR_KRADEN, 6, 23);
	/* "We'll bring you in against your will if we must, but either way..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 50);
	/* "Isn't that right, Felix?" */
	fdScriptOpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	result = syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
	if(result == 0) {
		fdScriptWait(20);
		fdActorFaceDirectionById(ACTOR_JENNA, FACING_WEST, 0);
		fdScriptWait(20);
		/* Yes: "Good for you! Let's teach these guys a lesson!" */
		fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
		FieldStateWork->msg++;
	} else {
		scEasternAlhafraShipCountAnswer();
		fdScriptWait(20);
		fdActorFaceDirectionById(ACTOR_JENNA, FACING_WEST, 0);
		fdScriptWait(20);
		FieldStateWork->msg++;
		/* No: "After what they've done? Come on, we need to make them fess
		   up!" */
		fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	}
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorJump(ACTOR_BRIGGS_MATE, 6, 15);
	fdActorJump(ACTOR_BRIGGS_MATE, 6, 23);
	/* "You don't stand a chance! We're hardened criminal types! We eat
	   punks..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	fdScriptWait(10);
	/* "Watch yourselves, you self-righteous little brats! You're the
	   ones..." */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	fdActorWalkBy(ACTOR_BRIGGS, 0, -16);
	fdActorWalkByById(ACTOR_BRIGGS_MATE, 0, -16);
	fdScriptWait(10);
	fdScriptSetAfterBattleDestination(
		SCENE_EASTERN_ALHAFRA_SHIP, ENTRANCE_AFTER_BRIGGS_BATTLE);

	MainRoutineStateWork.battleType = BATTLE_TYPE_NO_ESCAPE;
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED_YAMPI_DESERT_ALHAFRA, 1);
	fdScriptEnd();
}

/* After the battle Briggs gives himself up. Chaucha pleads for him, and the
   mayors of Madra and Alhafra settle what becomes of him and his crew and take
   his ship for Madra. */
void scEasternAlhafraShipBriggsIsCaught(void)
{
	ACTOR *emote;
	ACTOR *briggs;
	ACTOR *leader;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgWellThatTakes);
	fdActorSetPos(ACTOR_BRIGGS, 436, 416);
	scFieldActorSetSprRotationById(ACTOR_BRIGGS, DEGtoANG(90));
	fdActorSetSprFlags((Sint32)fdActorGetById(ACTOR_BRIGGS), 0);
	fdActorSetPos(ACTOR_BRIGGS_MATE, 456, 432);
	scFieldActorSetSprRotationById(ACTOR_BRIGGS_MATE, DEGtoANG(180));
	fdActorSetSprFlags((Sint32)fdActorGetById(ACTOR_BRIGGS_MATE), 0);
	fdActorGetById(ACTOR_BRIGGS_MATE)->facing = FACING_SOUTH;
	fdActorSetPos(ACTOR_FIRST_PIRATE, 456, 460);
	scFieldActorSetSprRotationById(ACTOR_FIRST_PIRATE, DEGtoANG(180));
	fdActorSetSprFlags((Sint32)fdActorGetById(ACTOR_FIRST_PIRATE), 0);
	fdActorSetPos(ACTOR_SECOND_PIRATE, 420, 424);
	fdActorSetSprRotationById(ACTOR_SECOND_PIRATE, TRIG_TURN * 3 / 4);
	fdActorSetSprFlags((Sint32)fdActorGetById(ACTOR_SECOND_PIRATE), 0);
	fdActorSetPos(ACTOR_THIRD_PIRATE, 388, 440);
	fdActorSetSprRotationById(ACTOR_THIRD_PIRATE, DEGtoANG(90));
	fdActorSetSprFlags((Sint32)fdActorGetById(ACTOR_THIRD_PIRATE), 0);
	fdActorSetPos(ACTOR_KRADEN, 456, 376);
	fdActorSetPos(ACTOR_JENNA, 472, 392);
	fdActorSetPos(ACTOR_SHEBA, 472, 376);
	fdActorSetPos(ACTOR_CHAUCHA, 456, 552);
	fdActorSetPos(ACTOR_SECOND_ALHAFRAN_SOLDIER, 472, 280);
	fdActorSetPos(ACTOR_ALHAFRAN_SOLDIER, 440, 280);
	fdActorSetPos(ACTOR_MADRAN_SOLDIER, 456, 296);
	fdActorSetPos(ACTOR_MADRAS_MAYOR, 472, 296);
	fdActorSetPos(ACTOR_MADRAS_ELDER, 440, 296);
	fdActorSetPos(ACTOR_ALHAFRAS_MAYOR, 456, 304);
	fdActorSetSpeed(ACTOR_SECOND_ALHAFRAN_SOLDIER, 1);
	fdActorSetSpeed(ACTOR_ALHAFRAN_SOLDIER, 1);
	fdActorSetSpeed(ACTOR_MADRAN_SOLDIER, 1);
	fdActorSetSpeed(ACTOR_MADRAS_MAYOR, 1);
	fdActorSetSpeed(ACTOR_MADRAS_ELDER, 1);
	fdActorSetSpeed(ACTOR_ALHAFRAS_MAYOR, 1);
	fdActorSetSpeed(ACTOR_FELIX, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorSetSpeed(ACTOR_SHEBA, 1);
	fdActorSetSpeed(ACTOR_CHAUCHA, 1);
	fdActorSetSpeed(ACTOR_BRIGGS, 1);
	fdActorSetSpeed(ACTOR_BRIGGS_MATE, 1);
	fdActorSetSpeed(ACTOR_FIRST_PIRATE, 1);
	fdActorSetSpeed(ACTOR_SECOND_PIRATE, 1);
	fdActorSetSpeed(ACTOR_THIRD_PIRATE, 1);
	fdCameraMoveTo(464, CAMERA_KEEP, 408, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	/* "Well, that takes care of that!" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(10);
	/* "And I'm willing to bet everyone Briggs has been harassing will be..." */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	/* "This should set everyone in Indra at ease... They all seemed to
	   fear..." */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	fdScriptWait(10);
	/* "We lost..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH_BY_WEST, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_NORTH_BY_EAST, 0);
	fdScriptWait(50);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_SOUTH, 0);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
	/* "Even pirates have honor... I'll go quietly. I won't make a scene..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	fdScriptWait(10);
	/* "Go on... Hand us over to Madra or Alhafra or whoever..." */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(10);
	/* "Good... First off, you can tell the Madrans that the man called..." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdScriptWait(10);
	/* "Fine. So, what, I just have to say that Piers isn't one of my men?" */
	fdScriptOpenMessage(ACTOR_BRIGGS, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdScriptWait(30);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
		fdScriptWait(10);
		/* Yes: "That's right. You tell the Madrans that, and they can
		   release..." */
		fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
		FieldStateWork->msg += 1;
	} else {
		fdScriptWait(40);
		fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
		fdScriptWait(10);
		FieldStateWork->msg += 1;
		/* No: "Saying Piers is innocent won't clear you of your own
		   crimes..." */
		fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	}
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(10);
	/* "We've secured Briggs's vow... We should take him to the Madrans..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	/* "Wait!" */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdCameraFollowActor(ACTOR_CHAUCHA, 1);
	fdCameraWaitForArrival();
	fdActorWalkToAndWait(ACTOR_CHAUCHA, 456, 504);
	fdActorWalkByAndWaitById(ACTOR_CHAUCHA, 16, 0);
	fdActorWalkToAndWaitById(ACTOR_CHAUCHA, 472, 424);
	fdScriptWait(10);
	fdCameraMoveTo(464, CAMERA_KEEP, 408, 1);
	fdCameraWaitForArrival();
	fdScriptWait(10);
	/* "Can't you find it in your hearts to forgive Briggs and his men?" */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "And who are you?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_CHAUCHA, 3);
	fdActorSetAttachedEffect(
		ACTOR_CHAUCHA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	/* "My name is Chaucha. I am Briggs's wife." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "So you're a pirate, too?" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	fdScriptWait(10);
	/* "No! Chaucha hasn't done a thing! Let her be!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	fdActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "I know Briggs and his men have caused some great trouble with
	   their..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	fdScriptWait(10);
	/* "And I know their crimes cannot be ignored..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdScriptWait(10);
	/* "If you know all that, then how can you ask us just to forgive him?" */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
	/* "The only reason Briggs became a pirate was to help the people of
	   Champa." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 15);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH_BY_WEST, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_NORTH_BY_EAST, 0);
	fdScriptWait(50);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_CHAUCHA, 2);
	fdScriptWait(10);
	/* "We Champa are a seafaring folk. Our livelihood depends entirely
	   upon..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	fdScriptWait(10);
	/* "But in recent years, the sea has changed... We can't live the life
	   we..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	fdScriptWait(10);
	/* "The oceans have warmed, and the fish have vanished from our waters." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
	/* "The soil in Champa is rocky and barren. No crops can grow there." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(10);
	/* "No crops? So you have no grains? And no meat? It sounds like life
	   in..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	emote = (ACTOR *)fdActorShowEmote(
		ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	emote->update = 0;
	fdActorSetPosAndResetMotion(emote, (emote->x.fixed + toFIXED(22)),
		(emote->y.fixed + toFIXED(8)), emote->z.fixed);
	fdScriptWait(30);
	/* "It's not hard! It's unbearable! Our children can't eat! Our
	   village..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
	fdScriptWait(20);
	/* "These men promised to return with food enough for all our people,
	   but..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(10);
	/* "So you became pirates...thieves..." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "We don't know where Champa is..." */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdScriptWait(20);
	/* "It sits on the southeast corner of Angara..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdScriptWait(20);
	/* "Southeast Angara? Isn't that near Xian and Lama Temple?" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "You have a good memory! If you were to head south through the..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "That's so far away..." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	fdScriptWait(10);
	/* "Well, yeah! You think we could be successful pirates so close to
	   home?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	fdScriptWait(10);
	/* "If they raided any towns near Champa, it would be obvious to all
	   who..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
	/* "And no one back home could forgive me if they knew. Not even my..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 20);
	fdActorWalkByById(ACTOR_ALHAFRAS_MAYOR, 0, 8);
	/* "But where does that leave us? Are we simply to accept your
	   looting..." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	emote = (ACTOR *)fdActorShowEmote(
		ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	emote->update = 0;
	fdActorSetPosAndResetMotion(emote, (emote->x.fixed + toFIXED(22)),
		(emote->y.fixed + toFIXED(8)), emote->z.fixed);
	fdActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdScriptWait(20);
	fdCameraMoveTo(464, CAMERA_KEEP, 304, 1);
	fdCameraWaitForArrival();
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
	fdScriptWait(10);
	/* "I'm shocked, I tell you. Shocked!" */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTHEAST, 0);
	fdScriptWait(20);
	/* "What's so shocking?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0, 10);
	fdActorShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "It looks like they really are pirates, just like you said..." */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Mister Mayor, I am appalled that you could not take our word for it!" */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "He probably wouldn't even have believed we were from Madra if the..." */
	fdScriptShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH, 0);
	fdScriptWait(10);
	/* "Am I supposed to believe the accusations of a complete stranger?" */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Is this your position, Mayor?" */
	fdScriptShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	fdScriptWait(10);
	/* "How can I be expected to believe a tidal wave could wash Indra
	   into..." */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_MADRAN_SOLDIER, 2);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	fdScriptWait(10);
	/* "Or that a group of men could walk on foot! from Madra all the way
	   to..." */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MADRAN_SOLDIER, ANIM_SHAKE_HEAD);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
	fdScriptWait(10);
	/* "It sounds to me like you had forgotten about us altogether!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0, 10);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "I'm not sure you remember the last time we met. Do you?" */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "We met when I was just a child... It must have been more than ten..." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHEAST, 0);
	fdScriptWait(20);
	/* "Twenty years! It's been twenty years since I last saw you." */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
	fdScriptWait(10);
	/* "Has it really been so long?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0, 10);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
	fdScriptWait(20);
	/* "It's been a long time indeed!" */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_MADRAN_SOLDIER, 2);
	fdScriptWait(10);
	/* "I guess we can't blame you for forgetting his face." */
	fdScriptShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH, 0);
	fdScriptWait(20);
	fdActorShowEmote(
		ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "Don't be silly! I didn't forget his face! His face merely...
	   changed!" */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_MADRAS_ELDER, 3);
	fdActorSetAttachedEffect(
		ACTOR_MADRAS_ELDER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	/* "Ahem! Forgive my interrupting your interruption, but..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(
		ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(
		ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MADRAN_SOLDIER, FACING_SOUTH, 0);
	fdScriptWait(20);
	fdCameraMoveWithinMapBounds(toFIXED(464), CAMERA_KEEP, toFIXED(344), 1);
	fdCameraWaitForArrival();
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(10);
	/* "Would I be correct in assuming that I am speaking to the leaders
	   of..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdActorWalkByAndWaitById(ACTOR_MADRAS_MAYOR, 0, 16);
	fdScriptWait(10);
	/* "You are correct! I am the mayor of Madra." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorWalkByById(ACTOR_ALHAFRAS_MAYOR, 0, 16);
	fdActorWalkByAndWaitById(ACTOR_MADRAS_ELDER, 0, 16);
	fdActorSetAnimationById(ACTOR_ALHAFRAS_MAYOR, ANIM_STAND);
	fdScriptWait(10);
	/* "And I..." and "I am..." */
	syFieldScriptShowTwoMessagesAndWait(ACTOR_MADRAS_ELDER, 13, 1, 13, 4,
		ACTOR_ALHAFRAS_MAYOR, 17, 11, 22, 6, 0);
	fdActorStartRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
	fdScriptWait(30);
	fdActorShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorShowEmote(
		ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 60);
	fdActorShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_SHAKE_HEAD);
	fdActorShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	fdActorShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_SCOWL, 70);
	fdActorShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	fdScriptWait(10);
	/* "I am the mayor of Alhafra." */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
	fdScriptWait(10);
	/* "I am Madra's town elder." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_MADRAN_SOLDIER, 2);
	fdScriptWait(10);
	/* "And I am a close personal friend of Shin, whose lover was injured
	   by..." */
	fdScriptShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH - 1, 0);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	/* "I am the scholar Kraden." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_SOUTH - 1, 0);
	fdScriptWait(20);
	/* "He is Felix, the leader of our group." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_NORTH, 0);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	/* "I am Jenna, Felix's sister." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(10);
	/* "I am called Sheba. I come from the town of Lalivero." */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorSetAnimationById(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_MADRAS_MAYOR, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_MADRAS_ELDER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MADRAN_SOLDIER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	/* "Now, what errand has brought you all here?" */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
	fdScriptWait(10);
	/* "We came for Briggs..." and "The mayor of Madra claims..." */
	syFieldScriptShowTwoMessagesAndWait(ACTOR_MADRAS_MAYOR, 12, 1, 25, 4,
		ACTOR_ALHAFRAS_MAYOR, 9, 11, 14, 6, 0);
	fdActorStartRepeatedMotion(ACTOR_MADRAS_MAYOR, 3);
	fdActorSetAttachedEffect(
		ACTOR_MADRAS_MAYOR, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 3);
	fdEffectSetAttached(
		ACTOR_ALHAFRAS_MAYOR, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH, 0);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
	fdScriptWait(10);
	/* "We followed Briggs here from Madra." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Of course, we have good reason to suspect them of being pirates." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdCameraMoveTo(464, CAMERA_KEEP, 432, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	emote = (ACTOR *)fdActorShowEmote(
		ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	{
		FIXED x = emote->x.fixed;
		FIXED y = emote->y.fixed;
		emote->update = 0;
		fdActorSetPosAndResetMotion(
			emote, x + toFIXED(22), y + toFIXED(8), emote->z.fixed);
	}
	fdScriptWait(50);
	fdCameraMoveTo(464, CAMERA_KEEP, 344, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
	fdScriptWait(10);
	/* "Ahem! And I was honestly taken aback to hear a businessman being..." */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_SHAKE_HEAD);
	/* "If they truly are pirates, then why would they go to the trouble
	   of..." */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorJump(ACTOR_MADRAS_ELDER, 6, 15);
	fdActorJump(ACTOR_MADRAS_ELDER, 6, 23);
	/* "Because the very funds they used to purchase the ship were
	   pillaged..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0, 10);
	fdCameraMoveTo(464, CAMERA_KEEP, 424, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	fdScriptWait(10);
	/* "If the ship was bought with Madran gold, then it belongs to the
	   Madrans!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdScriptWait(30);
	/* "What do you say? If you give them the ship, will that fix
	   everything?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	briggs = fdActorGetById(ACTOR_BRIGGS);
	fdActorSetPos(ACTOR_BRIGGS, briggs->x.part[0].integer + 20,
		briggs->z.part[0].integer + 8);
	fdActorSetSprRotationById(ACTOR_BRIGGS, 0);
	fdActorSetSprFlags(
		(Sint32)fdActorGetById(ACTOR_BRIGGS), SPRITE_FLAG_SHADOW);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_NORTH, 0);
	fdActorJump(ACTOR_BRIGGS, 6, 40);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
	fdActorSetPos(ACTOR_BRIGGS_MATE, 456, 440);
	fdActorSetSprRotationById(ACTOR_BRIGGS_MATE, 0);
	fdActorSetSprFlags(
		(Sint32)fdActorGetById(ACTOR_BRIGGS_MATE), SPRITE_FLAG_SHADOW);
	fdActorSetPos(ACTOR_FIRST_PIRATE, 456, 456);
	fdActorSetSprRotationById(ACTOR_FIRST_PIRATE, 0);
	fdActorSetSprFlags(
		(Sint32)fdActorGetById(ACTOR_FIRST_PIRATE), SPRITE_FLAG_SHADOW);
	fdActorSetPos(ACTOR_SECOND_PIRATE, 424, 424);
	fdActorSetSprRotationById(ACTOR_SECOND_PIRATE, 0);
	fdActorSetSprFlags(
		(Sint32)fdActorGetById(ACTOR_SECOND_PIRATE), SPRITE_FLAG_SHADOW);
	fdActorSetPos(ACTOR_THIRD_PIRATE, 408, 440);
	fdActorSetSprRotationById(ACTOR_THIRD_PIRATE, 0);
	fdActorSetSprFlags(
		(Sint32)fdActorGetById(ACTOR_THIRD_PIRATE), SPRITE_FLAG_SHADOW);
	fdActorFaceDirection(ACTOR_BRIGGS_MATE, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_FIRST_PIRATE, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SECOND_PIRATE, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_THIRD_PIRATE, FACING_NORTH, 0);
	fdActorJump(ACTOR_FIRST_PIRATE, 6, 0);
	fdActorJump(ACTOR_SECOND_PIRATE, 6, 0);
	fdActorJump(ACTOR_THIRD_PIRATE, 6, 0);
	fdActorJump(ACTOR_BRIGGS_MATE, 6, 40);
	fdActorSetAnimationById(ACTOR_FIRST_PIRATE, ANIM_SHAKE_HEAD);
	fdActorSetAnimationById(ACTOR_SECOND_PIRATE, ANIM_SHAKE_HEAD);
	fdActorSetAnimationById(ACTOR_THIRD_PIRATE, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS_MATE, ANIM_SHAKE_HEAD);
	fdScriptWait(10);
	fdCameraMoveTo(464, CAMERA_KEEP, 368, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
	fdScriptWait(10);
	/* "Then where will you go?" */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
	/* "I won't run, and I won't hide. ...Actually, I don't feel up for
	   much..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
	fdScriptWait(10);
	/* "Then you probably won't mind being locked up, will you?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0, 10);
	fdCameraMoveTo(464, CAMERA_KEEP, 400, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorRunRepeatedMotion(ACTOR_CHAUCHA, 2);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
	fdScriptWait(20);
	/* "Briggs..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdScriptWait(10);
	/* "Don't worry, Chaucha! I'll be back!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_CHAUCHA, 2);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_HEART, 40);
	/* "Take care of Eoleo!" */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	fdScriptWait(30);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_SOUTH, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS_MATE, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_FIRST_PIRATE, ANIM_NOD);
	fdScriptWait(10);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_SOUTHWEST, 0);
	fdScriptWait(15);
	fdActorSetAnimationById(ACTOR_SECOND_PIRATE, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_THIRD_PIRATE, ANIM_NOD);
	fdScriptWait(20);
	fdScriptWait(20);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 16, -16);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
	fdActorWalkBy(ACTOR_BRIGGS_MATE, 0, -32);
	fdActorWalkBy(ACTOR_FIRST_PIRATE, 0, -32);
	fdActorWalkByById(ACTOR_SECOND_PIRATE, 16, 0);
	fdActorWalkByAndWait(ACTOR_THIRD_PIRATE, 16, -16);
	fdActorWalkBy(ACTOR_JENNA, -16, -32);
	fdActorWalkByAndWait(ACTOR_SHEBA, -16, -32);
	fdActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_STAND);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
	fdActorWalkByById(ACTOR_BRIGGS_MATE, 16, 0);
	fdActorWalkBy(ACTOR_FIRST_PIRATE, 0, -16);
	fdActorWalkByById(ACTOR_SECOND_PIRATE, 16, 0);
	fdActorWalkByById(ACTOR_THIRD_PIRATE, 16, 0);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, -16);
	fdCameraMoveTo(464, CAMERA_KEEP, 384, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_BRIGGS_MATE, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SECOND_PIRATE, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
	fdScriptWait(20);
	/* "Your strange powers surprised me, but even without them, you
	   would..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(
		ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_NORTH, 0);
	fdScriptWait(20);
	fdActorWalkBy(ACTOR_BRIGGS_MATE, 0, -16);
	fdActorWalkByById(ACTOR_FIRST_PIRATE, 16, 0);
	fdActorWalkBy(ACTOR_SECOND_PIRATE, 0, -16);
	fdActorWalkByById(ACTOR_THIRD_PIRATE, 16, 0);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, -16);
	fdActorWalkBy(ACTOR_BRIGGS_MATE, 0, -16);
	fdActorWalkBy(ACTOR_FIRST_PIRATE, 0, -16);
	fdActorWalkByById(ACTOR_SECOND_PIRATE, 16, 0);
	fdActorWalkBy(ACTOR_THIRD_PIRATE, 0, -16);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, -16);
	fdActorWalkBy(ACTOR_BRIGGS_MATE, 0, -16);
	fdActorWalkBy(ACTOR_FIRST_PIRATE, 0, -16);
	fdActorWalkBy(ACTOR_SECOND_PIRATE, 0, -16);
	fdActorWalkByById(ACTOR_THIRD_PIRATE, 16, 0);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, -16);
	fdCameraMoveTo(464, CAMERA_KEEP, 336, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
	fdScriptWait(10);
	/* "We'll be taking that boat, then." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS_MATE, 2);
	fdScriptWait(10);
	/* "You're taking our ship!? You're worse than we ever were!" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
	fdActorShowEmote(ACTOR_FIRST_PIRATE, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Aren't you at least going to pay us for the boat!?" */
	fdScriptShowMessageThenWait(ACTOR_FIRST_PIRATE, 0, 10);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(30);
	fdActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "Quiet! All of you!" */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorShowEmote(ACTOR_FIRST_PIRATE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_SECOND_PIRATE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_THIRD_PIRATE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_NORTH, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdScriptWait(10);
	/* "Do as you must, but I have one request..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(20);
	fdCameraMoveTo(464, CAMERA_KEEP, 416, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_CHAUCHA, 3);
	fdActorSetAttachedEffect(
		ACTOR_CHAUCHA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(50);
	fdCameraMoveTo(464, CAMERA_KEEP, 336, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(30);
	/* "You have to let Chaucha and Eoleo stay in the boat until it's
	   repaired." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_EAST, 0);
	fdScriptWait(10);
	fdActorGetById(ACTOR_MADRAS_MAYOR)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWaitById(ACTOR_MADRAS_MAYOR, 456, 312);
	fdScriptWait(1);
	fdActorGetById(ACTOR_MADRAS_MAYOR)->steeringFlg |= ACTOR_FACES_MOTION;
	fdScriptWait(20);
	fdActorWalkBy(ACTOR_BRIGGS_MATE, 0, -16);
	fdActorWalkBy(ACTOR_FIRST_PIRATE, 0, -16);
	fdActorWalkBy(ACTOR_SECOND_PIRATE, 0, -16);
	fdActorWalkBy(ACTOR_THIRD_PIRATE, 0, -16);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, -16);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorFaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
	fdScriptWait(10);
	/* "You haven't done Alhafra any harm, Briggs..." */
	fdScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
	fdScriptWait(10);
	/* "At least keep him locked up until the boat is repaired..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0, 10);
	fdActorFaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	fdScriptWait(30);
	fdActorFaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_EAST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdScriptWait(20);
	fdActorWalkBy(ACTOR_BRIGGS_MATE, 0, -32);
	fdActorWalkBy(ACTOR_FIRST_PIRATE, 0, -32);
	fdActorWalkBy(ACTOR_SECOND_PIRATE, 0, -32);
	fdActorWalkBy(ACTOR_THIRD_PIRATE, 0, -32);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, -32);
	fdActorFaceDirectionById(ACTOR_MADRAN_SOLDIER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH, 0);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_WEST, 0);
	fdScriptWait(20);
	fdCameraMoveTo(464, CAMERA_KEEP, 296, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MADRAN_SOLDIER, 2);
	fdScriptWait(10);
	/* "And our prisoner, Piers... Is he one of your men?" */
	fdScriptShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
	fdActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "I don't know any Piers! Sounds to me like you've made a pretty
	   big..." */
	fdScriptShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorFaceDirectionById(ACTOR_MADRAS_ELDER, FACING_NORTHEAST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
	fdScriptWait(10);
	/* "See! It's just as I said! Piers is innocent!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_MADRAN_SOLDIER, 3);
	fdActorSetAttachedEffect(
		ACTOR_MADRAN_SOLDIER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorFaceDirectionById(ACTOR_MADRAN_SOLDIER, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_MADRAN_SOLDIER, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
	fdScriptWait(10);
	/* "You must return to Madra and free him immediately!" */
	fdScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0, 10);
	fdActorShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "...Me? What, alone?!?" */
	fdScriptShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
	fdActorFaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_NORTH, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
	fdScriptWait(10);
	/* "Only two of us will remain here. You can return to Madra with the
	   others." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MADRAN_SOLDIER, ANIM_NOD);
	fdScriptWait(10);
	/* "Understood! We'll return immediately!" */
	fdScriptShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
	fdActorSetAnimationById(ACTOR_MADRAS_MAYOR, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
	fdActorSetSpeed(ACTOR_MADRAN_SOLDIER, 1.8);
	fdActorWalkByAndWait(ACTOR_MADRAN_SOLDIER, 0, -32);
	fdActorWalkByAndWait(ACTOR_MADRAN_SOLDIER, -16, 0);
	fdActorSetPos(ACTOR_MADRAN_SOLDIER, 0, 0);
	fdScriptWait(20);
	fdActorWalkByAndWaitById(ACTOR_ALHAFRAN_SOLDIER, 16, 16);
	fdActorFaceDirectionById(ACTOR_ALHAFRAN_SOLDIER, FACING_EAST, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_ALHAFRAN_SOLDIER, 2);
	fdScriptWait(10);
	/* "Can you come with us? ...Please?" */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAN_SOLDIER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetMovementTypeById(ACTOR_SECOND_ALHAFRAN_SOLDIER,
		EasternAlhafraShipLeaveUpTheStairsScript);
	fdScriptWait(10);
	fdActorSetMovementTypeById(
		ACTOR_BRIGGS, EasternAlhafraShipLeaveUpTheStairsScript);
	fdScriptWait(10);
	fdActorSetMovementTypeById(
		ACTOR_BRIGGS_MATE, EasternAlhafraShipLeaveUpTheStairsScript);
	fdScriptWait(10);
	fdActorSetMovementTypeById(
		ACTOR_FIRST_PIRATE, EasternAlhafraShipLeaveUpTheStairsScript);
	fdScriptWait(10);
	fdActorSetMovementTypeById(
		ACTOR_SECOND_PIRATE, EasternAlhafraShipLeaveUpTheStairsScript);
	fdScriptWait(10);
	fdActorSetMovementTypeById(
		ACTOR_THIRD_PIRATE, EasternAlhafraShipLeaveUpTheStairsScript);
	fdScriptWait(75);
	fdActorWalkByAndWaitById(ACTOR_ALHAFRAN_SOLDIER, 16, 0);
	fdActorWalkByAndWait(ACTOR_ALHAFRAN_SOLDIER, 0, -32);
	fdActorWalkByAndWait(ACTOR_ALHAFRAN_SOLDIER, -32, 0);
	fdActorSetPos(ACTOR_ALHAFRAN_SOLDIER, 0, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH, 0);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
	fdScriptWait(10);
	/* "It looks like that wraps this whole mess up..." */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
	fdScriptWait(20);
	/* "Let us return to my mansion for a nice cup of tea..." */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
	fdScriptWait(10);
	fdActorWalkByAndWaitById(ACTOR_ALHAFRAS_MAYOR, 16, 0);
	fdActorWalkBy(ACTOR_ALHAFRAS_MAYOR, 0, -32);
	fdActorWalkByAndWait(ACTOR_MADRAS_ELDER, 0, -16);
	fdActorWalkByAndWaitById(ACTOR_MADRAS_ELDER, 16, 0);
	fdActorFaceDirection(ACTOR_MADRAS_ELDER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTH, 0);
	fdScriptWait(10);
	fdActorShowEmote(
		ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH, 0);
	fdScriptWait(20);
	/* "You say your name is Kraden?" */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdCameraMoveTo(464, CAMERA_KEEP, 320, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH, 0);
	fdActorWalkByById(ACTOR_FELIX, 16, 0);
	fdActorWalkByAndWaitById(ACTOR_KRADEN, 16, 0);
	fdActorWalkBy(ACTOR_FELIX, 0, -32);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, -32);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	fdScriptWait(10);
	/* "Thank you very much for the help with Briggs." */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "If you have the time, please come see me at my mansion before you
	   go..." */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	fdScriptWait(10);
	/* "It's nothing special, but I would like to thank you for
	   everything..." */
	fdScriptShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorFaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirectionById(ACTOR_MADRAS_ELDER, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_ELDER, ANIM_NOD);
	fdScriptWait(10);
	fdActorWalkBy(ACTOR_ALHAFRAS_MAYOR, 0, -32);
	fdActorWalkByAndWait(ACTOR_MADRAS_ELDER, 0, -32);
	fdActorWalkBy(ACTOR_ALHAFRAS_MAYOR, -32, 0);
	fdActorWalkByAndWait(ACTOR_MADRAS_ELDER, -16, 0);
	fdActorSetPos(ACTOR_MADRAS_ELDER, 0, 0);
	fdActorWaitForMoveById(ACTOR_ALHAFRAS_MAYOR);
	fdActorSetPos(ACTOR_ALHAFRAS_MAYOR, 0, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTH, 0);
	fdScriptWait(30);
	/* "Why do I get the feeling the mayor of Alhafra prefers to deal with
	   my..." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
	fdScriptWait(10);
	/* "Thank all of you very much." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
	fdScriptWait(10);
	/* "Due to your efforts, Piers is going to be released." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_SOUTH_BY_WEST, 0);
	fdScriptWait(30);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_NORTH, 0);
	fdScriptWait(40);
	fdActorWalkByAndWaitById(ACTOR_MADRAS_MAYOR, -32, -8);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
	fdScriptWait(30);
	fdActorFaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_SOUTHWEST, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
	fdScriptWait(30);
	fdActorWalkByAndWait(ACTOR_MADRAS_MAYOR, 24, -16);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
	fdScriptWait(30);
	fdActorFaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_NORTHEAST, 0);
	fdScriptWait(30);
	fdActorFaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
	fdScriptWait(30);
	fdActorWalkByAndWaitById(ACTOR_MADRAS_MAYOR, 16, 16);
	fdActorFaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH, 0);
	fdScriptWait(20);
	/* "And now, this fine ship belongs to Madra." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
	fdScriptWait(10);
	/* "None of this would have come to pass were it not for you. Thank you." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
	fdScriptWait(10);
	/* "Unfortunately, I don't have any reward I can offer you here..." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_SOUTH, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(10);
	/* "Oh, pish posh! We don't need any reward, do we, Felix?" */
	fdScriptOpenMessage(ACTOR_KRADEN, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdScriptWait(30);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		fdScriptWait(10);
		fdActorFaceDirectionById(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_SHAKE_HEAD);
		/* Yes: "Ha! Your honesty is refreshing, Felix!" */
		fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
		FieldStateWork->msg += 1;
	} else {
		fdScriptWait(40);
		scEasternAlhafraShipCountAnswer();
		fdActorFaceDirectionById(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
		fdScriptWait(20);
		FieldStateWork->msg += 1;
		/* No: "That's as may be, but I still feel quite indebted to you." */
		fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	}
	fdActorShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "So, what shall we do?" */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	scActorPopQuestionMarks(ACTOR_MADRAS_MAYOR);
	fdActorShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	/* "I know... Come to Madra after our boat has been fixed and we've..." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
	fdScriptWait(10);
	/* "We are still recovering from the effects of the tidal wave, so it..." */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Well, shall we be going?" */
	fdScriptShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MADRAS_MAYOR, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(20);
	fdActorWalkByAndWait(ACTOR_MADRAS_MAYOR, 0, -40);
	fdActorWalkByAndWait(ACTOR_MADRAS_MAYOR, -24, 0);
	fdActorSetPos(ACTOR_MADRAS_MAYOR, 0, 0);
	fdScriptWait(20);
	fdCameraMoveTo(464, CAMERA_KEEP, 400, 1);
	fdCameraWaitForArrival();
	fdScriptWait(30);
	fdActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "I see, so..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdScriptWait(30);
	fdActorShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorStartRepeatedMotion(ACTOR_CHAUCHA, 3);
	fdEffectSetAttached(
		ACTOR_CHAUCHA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	/* "Oh! N-Nothing. Never mind..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	fdScriptWait(10);
	/* "I do get to stay on the boat, right? At least until it's repaired?" */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	fdScriptWait(10);
	/* "Yes. That's where I'll be if you need me. ...With Eoleo..." */
	fdScriptShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorWalkToAndWait(ACTOR_CHAUCHA, 472, 376);
	fdCameraFollowActor(ACTOR_CHAUCHA, 1);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorWalkBy(ACTOR_KRADEN, -16, -32);
	fdActorWalkByAndWait(ACTOR_FELIX, -16, -32);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_STAND);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
	fdActorWalkToAndWait(ACTOR_CHAUCHA, 472, 320);
	fdActorWalkBy(ACTOR_JENNA, 16, -32);
	fdActorWalkBy(ACTOR_SHEBA, 16, -32);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorWalkToAndWait(ACTOR_CHAUCHA, 472, 264);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
	fdActorWalkToAndWait(ACTOR_CHAUCHA, 440, 264);
	fdActorSetPos(ACTOR_CHAUCHA, 0, 0);
	fdScriptWait(30);
	/* "I feel kind of bad for Chaucha." */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(10);
	/* "What can we do about it? After all, she's the one married to a
	   pirate." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdScriptWait(25);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(10);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_SOUTH, 0);
	fdScriptWait(20);
	/* "So, what should we do now?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "I suggest that we try to find Piers. He could prove enlightening!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(10);
	/* "That's true... He seems to be a Water Adept. He might be able to
	   tell..." */
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_WEST, 0);
	fdScriptWait(20);
	/* "Do you think we should help them fix the boat?" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdScriptWait(20);
	/* "Say, if we help with the boat, maybe they'll give us a ride back
	   to..." */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdScriptWait(10);
	/* "What do you say, Felix?" */
	fdScriptOpenMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdScriptWait(30);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
		fdScriptWait(20);
		/* Yes: "All right! Let's go see what we can do to get this thing..." */
		fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
		FieldStateWork->msg += 1;
	} else {
		fdScriptWait(40);
		fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
		FieldStateWork->msg += 1;
		/* No: "You're opposed to it? Well, since you seem to have your
		   own..." */
		fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	}
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0) {
		fdActorSetDestinationById(
			ACTOR_JENNA, leader->x.part[0].integer, leader->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0) {
		fdActorSetDestinationById(
			ACTOR_SHEBA, leader->x.part[0].integer, leader->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_SHEBA);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0) {
		fdActorSetDestinationById(
			ACTOR_KRADEN, leader->x.part[0].integer, leader->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdScriptEnd();
	return;
}

/* clang-format off */
/* Briggs and his mate running out to the party once Jenna gives it away */
/* (scEasternAlhafraShipMayorArrives). */
Uint32 EasternAlhafraShipBriggsRunsScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_BY(32, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, 28),
	SC_SYNC,
	SC_MOVE_BY(48, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(12, 0, 12),
	SC_SYNC,
	SC_MOVE(456, 0, 440),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(1),
	SC_END,
};

Sint32 EasternAlhafraShipMateRunsScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_BY(32, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(12, 0, -12),
	SC_SYNC,
	SC_MOVE_BY(52, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(12, 0, 12),
	SC_SYNC,
	SC_MOVE(472, 0, 440),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_WAIT(1),
	SC_END,
};

/* The way up the stairs and off the ship that Briggs, his crew and their */
/* guard take once he is caught. */
Uint32 EasternAlhafraShipLeaveUpTheStairsScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(472, 0, 264),
	SC_SYNC,
	SC_MOVE(440, 0, 264),
	SC_SYNC,
	SC_RELEASE,
};

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 456, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(EasternAlhafraShip),
	SCENE_EXIT_UNLESS(EASTERN_ALHAFRA_SHIP_TO_EASTERN_ALHAFRA, EasternAlhafra, EASTERN_ALHAFRA_FROM_EASTERN_ALHAFRA_SHIP, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, EasternAlhafraShip, 3, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(3, EasternAlhafraShip, 2, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(4, EasternAlhafraShip, 5, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(5, EasternAlhafraShip, 4, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE EasternAlhafraShipActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 396, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 440, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CHAUCHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 88, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_EOLEO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 136, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOWER_VASE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 128, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 149, 0, 475, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 149, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ALHAFRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

CUE SceneCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_STEP, METATILE_BELOW_THE_STAIRS, FLAG_MAYOR_ARRIVED_AT_THE_SHIP,
		SCRIPT_REF(scEasternAlhafraShipMayorArrives)},
	{CUE_TALK, ACTOR_EOLEO, CONDITION_ALWAYS,
		SCRIPT_REF(scEasternAlhafraShipEoleoLifts)},
	{CUE_TALK, ACTOR_CHAUCHA, CONDITION_ALWAYS, MSGNO(MsgEoleoAndI)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EOLEO, CONDITION_ALWAYS,
		MSGNO(MsgIMGonna)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHAUCHA, CONDITION_ALWAYS,
		MSGNO(MsgIfEoleoGets)},
	{CUE_TALK, ACTOR_LARGE_BREAD, CONDITION_ALWAYS,
		SCRIPT_REF(scEasternAlhafraShipTakeTheBread)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_TALL_CRATE, CONDITION_ALWAYS,
		SCRIPT_REF(scEasternAlhafraShipStartMovingCrate)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TALL_CRATE, CONDITION_ALWAYS,
		SCRIPT_REF(scEasternAlhafraShipFinishMovingCrate)},
	/* CUE_PUSH and CUE_PUSHED answer any target (fdFindCue):
	   their 30 is never compared. */
	{CUE_PUSH, 30, CONDITION_ALWAYS,
		SCRIPT_REF(scEasternAlhafraShipStartMovingCrate)},
	{CUE_PUSHED, 30, CONDITION_ALWAYS,
		SCRIPT_REF(scEasternAlhafraShipFinishMovingCrate)},
	{PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_STACKED_CRATES,
		FLAG_BREAD_KNOCKED_DOWN,
		SCRIPT_REF(scEasternAlhafraShipKnockDownTheBread)},
	{CUE_TABLE_END},
};

Uint32 QuestionMarkScript[] = {
	SC_RELEASE,
};
