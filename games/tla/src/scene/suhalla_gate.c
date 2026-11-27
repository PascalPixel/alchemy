/* The Suhalla Gate, in the prologue: Jenna and Kraden cross it on their way to
   Idejima, while Alex holds back the soldiers behind them. Its three rooms
   share this overlay:

   - The road past the gate (SceneIdSuhallaGateRoad), coming from the Venus
	 Lighthouse in the east, where a Ruffian spots the two and fights them
	 (scSuhallaGateRuffianSpotsParty), and a door in the rock at its west end
	 leads on to the cliffs.
   - The cliffs (SceneIdSuhallaGateAmbush), whose two vines lead down to the
	 crevice where three Ruffians wait in ambush
	 (scSuhallaGateRuffiansAmbushParty).
   - The cave beyond the crevice (SceneIdSuhallaGatePsynergyStone), where a
	 Punch Ant jumps out at the two on the way to the Psynergy Stone
	 (scSuhallaGatePunchAntJumpsOut), and whose far end leads to Idejima.

   Every battle sends the party back to ENTRANCE_AFTER_SUHALLA_GATE_BATTLE in
   its room, and a lost one to the Venus Lighthouse's entrance
   (VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT). Back from each, its flag set, the
   beaten Ruffians lie where they fell. */
#include "item_ids.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/field.h"
#include "field/field_mode_script.h"
#include "game/game.h"
#include "scene/scene.h"
#include "text/msg_ids.h"

/* Set as the party comes back from beating the Ruffian on the road; he then
   lies beaten by it. */
#define FLAG_SUHALLA_GATE_RUFFIAN_BEATEN   0x894
/* Set as the party comes back from beating the three Ruffians at the crevice;
   they then lie beaten before it. */
#define FLAG_SUHALLA_GATE_AMBUSH_BEATEN	   0x895
/* Set once Kraden has talked over the Punch Ant's battle; the cave's Punch Ant
   then stays away. */
#define FLAG_SUHALLA_GATE_PUNCH_ANT_BEATEN 0x896
/* Set once the chest on the cliffs, at metatile (10, 22), has given its
   Mint. */
#define FLAG_SUHALLA_GATE_MINT			   0xfb4

/* The entrance each room's battle sends the party back to, 99 in each of the
   three. */
#define ENTRANCE_AFTER_SUHALLA_GATE_BATTLE 99

/* The road's one placed actor. */
typedef enum SUHALLA_GATE_ROAD_ACTOR_ID {
	/* The Ruffian who spots Jenna and Kraden on the road. */
	ACTOR_ROAD_RUFFIAN = ACTOR_FIRST_PLACED
} SUHALLA_GATE_ROAD_ACTOR_ID;

/* The cliffs' placed actors. */
typedef enum SUHALLA_GATE_CLIFFS_ACTOR_ID {
	/* Five sprouts (SPRITE_SPROUT, the sprout Growth grows), with no collision,
	   each drawn half or fully grown as the party enters. */
	ACTOR_FIRST_SPROUT = ACTOR_FIRST_PLACED,
	/* The three Ruffians of the ambush, west to east as they line up. */
	ACTOR_WEST_RUFFIAN = ACTOR_FIRST_PLACED + 5,
	ACTOR_MIDDLE_RUFFIAN,
	ACTOR_EAST_RUFFIAN,
	/* Kraden, who jumps out from behind Jenna to point the way. */
	ACTOR_CLIFFS_KRADEN
} SUHALLA_GATE_CLIFFS_ACTOR_ID;

/* The cave's placed actors. */
typedef enum SUHALLA_GATE_CAVE_ACTOR_ID {
	ACTOR_PUNCH_ANT = ACTOR_FIRST_PLACED,
	ACTOR_CAVE_KRADEN
} SUHALLA_GATE_CAVE_ACTOR_ID;

/* SPRITE_SPROUT's animations as the sprouts here stand, half grown or fully
   grown. */
enum { SPROUT_ANIMATION_HALF_GROWN = 3, SPROUT_ANIMATION_GROWN = 4 };

/* The metatiles that run the rooms' scripts. */
typedef enum SUHALLA_GATE_METATILE {
	/* The door in the rock at the road's west end, metatile (8, 18), holding
	   the number of the exit it leads by. */
	ROAD_DOOR_METATILE = 3,
	/* The road's metatiles (29, 21), (30, 21) and (30, 22), where the Ruffian
	   spots the party. */
	RUFFIAN_SPOTS_PARTY_METATILE = 20,
	/* Unused: no metatile of the road's map holds it. */
	ROAD_UNUSED_METATILE = 12,
	/* On the cliffs, metatiles (31, 54), (32, 54) and (31, 55), away from the
	   crevice, where Kraden calls Jenna back. */
	KRADEN_POINTS_THE_WAY_METATILE = 21,
	/* The cliffs' chest, metatile (10, 22). */
	CLIFFS_CHEST_METATILE = 100,
	/* On the way to the Psynergy Stone, metatiles (31, 26) to (33, 29), where
	   the Punch Ant jumps out. */
	PUNCH_ANT_METATILE = 22,
	/* Unused: no metatile of the cave's map holds it. */
	CAVE_UNUSED_METATILE = 23
} SUHALLA_GATE_METATILE;

void scSuhallaGateAfterPunchAntBattle(void);
void scSuhallaGateFightPunchAnt(void);

extern SCENE_ENTRANCE SuhallaGateRoadEntrances[];
extern SCENE_ENTRANCE SuhallaGateCliffsEntrances[];
extern SCENE_ENTRANCE SuhallaGateCaveEntrances[];
extern ACTOR_PLACE	  SuhallaGateCavePlacements[];
extern ACTOR_PLACE	  SuhallaGateCliffsPlacements[];
extern ACTOR_PLACE	  SuhallaGateRoadPlacements[];
extern CUE			  SuhallaGateRoadCues[];
extern CUE			  SuhallaGateCliffsCues[];
extern CUE			  SuhallaGateCaveCues[];

/* Unused: talking to the west Ruffian at the crevice would start a battle with
   the Djinni Dew, but his own line, "Maybe I should have eaten a bigger
   breakfast.", always answers first. */
void scSuhallaGateFightDew(void)
{
	fdRequestDjinnBattle(ACTOR_WEST_RUFFIAN, DJINNI_DEW);
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_SUHALLA_GATE_AMBUSH)
		return SuhallaGateCliffsEntrances;
	if(scene == SCENE_SUHALLA_GATE_PSYNERGY_STONE)
		return SuhallaGateCaveEntrances;
	return SuhallaGateRoadEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_SUHALLA_GATE_AMBUSH)
		return SuhallaGateCliffsPlacements;
	if(scene == SCENE_SUHALLA_GATE_ROAD)
		return SuhallaGateRoadPlacements;
	return SuhallaGateCavePlacements;
}

const CUE *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_SUHALLA_GATE_AMBUSH)
		return SuhallaGateCliffsCues;
	if(scene == SCENE_SUHALLA_GATE_PSYNERGY_STONE)
		return SuhallaGateCaveCues;
	return SuhallaGateRoadCues;
}

/* Back from his battle, the road's Ruffian lies beaten by it. */
void scSuhallaGateSetUpRoad(void)
{
	if(MainRoutineStateWork.entrance == ENTRANCE_AFTER_SUHALLA_GATE_BATTLE)
		fdScriptSetFlag(FLAG_SUHALLA_GATE_RUFFIAN_BEATEN);
	if(fdScriptGetFlag(FLAG_SUHALLA_GATE_RUFFIAN_BEATEN)) {
		fdActorSetPos(ACTOR_ROAD_RUFFIAN, 456, 312);
		fdActorFallOverWithAnimationById(
			ACTOR_ROAD_RUFFIAN, FACING_SOUTH, ANIM_STILL);
		fdActorSetMovementType(
			fdActorGetById(ACTOR_ROAD_RUFFIAN), MOVEMENT_TYPE_SQUASH);
	}
}

/* Back from the ambush, its three Ruffians lie beaten before the crevice. */
void scSuhallaGateSetUpCliffs(void)
{
	fdMapGridCopyMetatileAttributes(108, 38, 1, 1, 102, 56);
	if(MainRoutineStateWork.entrance == ENTRANCE_AFTER_SUHALLA_GATE_BATTLE)
		fdScriptSetFlag(FLAG_SUHALLA_GATE_AMBUSH_BEATEN);
	if(fdScriptGetFlag(FLAG_SUHALLA_GATE_AMBUSH_BEATEN)) {
		fdActorSetPos(ACTOR_WEST_RUFFIAN, 590, 920);
		fdActorSetPos(ACTOR_MIDDLE_RUFFIAN, 624, 939);
		fdActorSetPos(ACTOR_EAST_RUFFIAN, 632, 920);
		fdActorGetById(ACTOR_WEST_RUFFIAN)->facing = FACING_SOUTH;
		fdActorFallOverById(ACTOR_WEST_RUFFIAN, FACING_SOUTHWEST);
		fdActorGetById(ACTOR_MIDDLE_RUFFIAN)->facing = FACING_EAST;
		fdActorFallOverById(ACTOR_MIDDLE_RUFFIAN, FACING_EAST);
		fdActorGetById(ACTOR_EAST_RUFFIAN)->facing = FACING_NORTH;
		fdActorFallOverById(ACTOR_EAST_RUFFIAN, FACING_EAST);
		fdActorGetById(ACTOR_WEST_RUFFIAN)->collisionFlg = ACTOR_SOLID;
		fdActorGetById(ACTOR_MIDDLE_RUFFIAN)->collisionFlg = ACTOR_SOLID;
		fdActorGetById(ACTOR_EAST_RUFFIAN)->collisionFlg = ACTOR_SOLID;
	}
	fdActorSetAnimation(ACTOR_FIRST_SPROUT, SPROUT_ANIMATION_GROWN);
	fdActorSetAnimation(ACTOR_FIRST_SPROUT + 1, SPROUT_ANIMATION_GROWN);
	fdActorSetAnimation(ACTOR_FIRST_SPROUT + 2, SPROUT_ANIMATION_HALF_GROWN);
	fdActorSetAnimation(ACTOR_FIRST_SPROUT + 3, SPROUT_ANIMATION_GROWN);
	fdActorSetAnimation(ACTOR_FIRST_SPROUT + 4, SPROUT_ANIMATION_HALF_GROWN);
}

/* Back from the Punch Ant's battle, Kraden talks it over. */
void scSuhallaGateSetUpCave(void)
{
	if(fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) == 0 &&
		MainRoutineStateWork.entrance == ENTRANCE_AFTER_SUHALLA_GATE_BATTLE)
		scSuhallaGateAfterPunchAntBattle();
}

/* Suhalla Gate's setup for each of its rooms: the road by the house, the cliffs
   of the ambush, and the cave of the Psynergy Stone. */
Sint32 scInit(void)
{
	FIELD_STATE *state;
	Sint16		 scene;

	state = RamHeapSlots->fieldState;
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_SUHALLA_GATE_ROAD)
		scSuhallaGateSetUpRoad();
	else if(scene == SCENE_SUHALLA_GATE_AMBUSH)
		scSuhallaGateSetUpCliffs();
	else if(scene == SCENE_SUHALLA_GATE_PSYNERGY_STONE)
		scSuhallaGateSetUpCave();
	return 0;
}

/* Unused: copies the cliffs' metatile as their set-up does, and sets
   FLAG_SHIP_CAN_SAIL, which the Lemurian Ship's deck sets once the party can
   sail it; no cue runs it. */
UNUSED void scSuhallaGateOpenCliffsMetatile(void)
{
	fdMapGridCopyMetatileAttributes(108, 38, 1, 1, 102, 56);
	fdScriptSetFlag(FLAG_SHIP_CAN_SAIL);
}

/* Walking into the door in the rock at the road's west end, metatile (8, 18),
   the door opens and the screen fades out with the leader still standing before
   it; the party comes out on the cliffs. The code walks `ACTOR_ISAAC` in
   through the doorway, as The Broken Seal's doors walk their leader, but here
   Jenna leads and Isaac is not on stage, so nobody is seen to go in. */
void scSuhallaGateEnterRockDoor(void)
{
	Sint32 exit = FieldStateWork->stepCue;

	fdActorGetById(ACTOR_ISAAC)->motionFlg = 0;
	fdScriptPlaySnd(SE_DOOR_OPEN);
	fdMapGridCopyMetatilesTo(66, 36, 71, 8, 2, 2);
	sySynch(4);
	fdMapGridCopyMetatilesTo(68, 36, 71, 8, 2, 2);
	sySynch(4);
	fdActorCenterAndWalk(ACTOR_ISAAC, 3, -16);
	fdScriptPlaySnd(SE_MAP_EXIT);
	syImportScriptSetExitRequest(exit);
}

/* Walking out of the cave's far end, metatile (41, 39), Jenna and Kraden leave
   for Idejima, and Felix becomes the leader again. */
void scSuhallaGateLeaveForIdejima(void)
{
	Sint32 exit = FieldStateWork->stepCue;

	MainRoutineStateWork.leaderActorId = ACTOR_FELIX;
	fdScriptPlaySnd(SE_MAP_EXIT);
	syImportScriptSetExitRequest(exit);
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* Walking west along the road, metatiles (29, 21) to (30, 22), Jenna and Kraden
   are spotted by a Ruffian: "Waahhh! It's them!" He looks for Alex, decides he
   can handle "some girl and her grandpa", and fights them. */
void scSuhallaGateRuffianSpotsParty(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgWaahhhItS);
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 472, 320);
	fdCameraMoveTo(456, CAMERA_KEEP, 280, 1);
	fdCameraWaitForArrival();
	fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_NORTHWEST, 0);
	fdScriptWait(10);
	fdActorJump(ACTOR_ROAD_RUFFIAN, 6, 20);
	fdScriptWait(20);
	/* "Waahhh! It's them!" */
	fdScriptShowMessageThenWait(ACTOR_ROAD_RUFFIAN, 0, 10);
	{
		Sint32 emote = EMOTE_IN_FRONT | EMOTE_EXCLAMATION;
		Sint32 ticks = 50;

		scFieldActorShowEmote(ACTOR_ROAD_RUFFIAN, emote, ticks);
	}
	/* "Whoa there... Where'd the blue-haired guy go?" */
	fdScriptShowMessageThenWait(ACTOR_ROAD_RUFFIAN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_ROAD_RUFFIAN, ANIM_NOD);
	fdScriptWait(30);
	/* "Who cares! I can handle some girl and her grandpa!" */
	fdScriptShowMessageThenWait(ACTOR_ROAD_RUFFIAN, 0, 10);
	fdActorWalkToAndWait(ACTOR_ROAD_RUFFIAN, 456, 312);
	fdScriptSetAfterBattleDestination(
		SCENE_SUHALLA_GATE_ROAD, ENTRANCE_AFTER_SUHALLA_GATE_BATTLE);
	fdScriptSetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
		VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT);
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_EARLY_STORY, 1);
	fdScriptEnd();
}

/* Trying to enter the crevice below the cliffs' vines, metatile (38, 57), Jenna
   steps up to it and backs away as three Ruffians step out of it one by one:
   "Ha! That was a good idea, waiting here for 'em!" Then they fight her and
   Kraden. */
void scSuhallaGateRuffiansAmbushParty(void)
{
	MAIN_ROUTINE_STATE *state;
	Sint32			   *owner;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgHaThatWas);
	state = &MainRoutineStateWork;
	owner = &state->leaderActorId;
	fdActorWalkByAndWait(*owner, 0, -4);
	syFieldScriptWait(30);
	fdActorGetById(*owner)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(*owner, 0, 12);
	syFieldScriptWait(10);
	fdActorWalkByAndWait(*owner, 0, 16);
	syFieldScriptWait(30);
	fdActorSetPos(ACTOR_WEST_RUFFIAN, 616, 912);
	fdActorWalkToAndWait(ACTOR_WEST_RUFFIAN, 600, 920);
	fdActorSetPos(ACTOR_EAST_RUFFIAN, 616, 912);
	fdActorFaceDirection(ACTOR_WEST_RUFFIAN, FACING_SOUTH, 0);
	fdActorWalkToAndWait(ACTOR_EAST_RUFFIAN, 632, 920);
	fdActorFaceDirection(ACTOR_EAST_RUFFIAN, FACING_SOUTH, 0);
	fdActorSetPos(ACTOR_MIDDLE_RUFFIAN, 616, 912);
	fdActorWalkToAndWait(ACTOR_MIDDLE_RUFFIAN, 616, 920);
	fdActorRunRepeatedMotion(ACTOR_WEST_RUFFIAN, 2);
	syFieldScriptWait(20);
	/* "Ha! That was a good idea, waiting here for 'em!" */
	syFieldScriptShowMessageThenWait(ACTOR_WEST_RUFFIAN, 0, 10);
	/* "You two aren't going anywhere!" */
	syFieldScriptShowMessageThenWait(ACTOR_MIDDLE_RUFFIAN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_EAST_RUFFIAN, ANIM_NOD);
	syFieldScriptWait(30);
	/* "Except with us, back to Master Iodem!" */
	syFieldScriptShowMessageThenWait(ACTOR_EAST_RUFFIAN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_WEST_RUFFIAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIDDLE_RUFFIAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_EAST_RUFFIAN, ANIM_NOD);
	syFieldScriptSetAfterBattleDestination(
		SCENE_SUHALLA_GATE_AMBUSH, ENTRANCE_AFTER_SUHALLA_GATE_BATTLE);
	fdScriptSetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
		VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT);
	state->battleType = BATTLE_TYPE_NO_ESCAPE;
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_EARLY_STORY, 2);
	fdScriptEnd();
}

/* The Punch Ant leaps at Jenna and its battle begins. */
void scSuhallaGateFightPunchAnt(void)
{
	ACTOR *owner;

	fdActorJump(ACTOR_PUNCH_ANT, 4, 0);
	owner = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(owner != NULL)
		fdActorSetDestination(ACTOR_PUNCH_ANT, owner->x.part[0].integer,
			owner->z.part[0].integer);
	fdScriptWait(20);
	fdScriptSetAfterBattleDestination(
		SCENE_SUHALLA_GATE_PSYNERGY_STONE, ENTRANCE_AFTER_SUHALLA_GATE_BATTLE);
	fdScriptSetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
		VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT);
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_EARLY_STORY, 3);
	fdScriptEnd();
}

/* On the way to the Psynergy Stone, metatiles (31, 26) to (33, 29), a Punch Ant
   jumps out from the south-west; Jenna turns to it with a start, and it hops up
   to her and attacks. */
void scSuhallaGatePunchAntJumpsOut(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetPos(ACTOR_PUNCH_ANT, 472, 480);
	fdScriptWait(16);
	fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_PUNCH_ANT, 0);
	scFieldActorShowEmote(MainRoutineStateWork.leaderActorId,
		EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdCameraFollowActor(ACTOR_PUNCH_ANT, 1);
	fdCameraWaitForArrival();
	fdActorJump(ACTOR_PUNCH_ANT, 4, 0);
	fdScriptWait(16);
	fdActorJump(ACTOR_PUNCH_ANT, 4, 0);
	fdActorMoveToAndWait(ACTOR_PUNCH_ANT, 480, 472);
	fdScriptWait(30);
	fdActorJump(ACTOR_PUNCH_ANT, 2, 0);
	fdActorMoveToAndWait(ACTOR_PUNCH_ANT, 496, 464);
	fdActorJump(ACTOR_PUNCH_ANT, 4, 0);
	fdActorMoveToAndWait(ACTOR_PUNCH_ANT, 488, 456);
	fdActorJump(ACTOR_PUNCH_ANT, 4, 0);
	fdActorMoveToAndWait(ACTOR_PUNCH_ANT, 496, 448);
	scSuhallaGateFightPunchAnt();
}

/* Unused: the Punch Ant would jump out as in scSuhallaGatePunchAntJumpsOut, but
   with Jenna first stepping aside and the ant hopping further east after her;
   no spot in the cave sets it off. */
void scSuhallaGatePunchAntJumpsOutAsLeaderStepsAside(void)
{
	Sint32 emote, ticks;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetPos(ACTOR_PUNCH_ANT, 472, 480);
	fdScriptWait(16);
	fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_PUNCH_ANT, 0);
	emote = EMOTE_IN_FRONT | EMOTE_EXCLAMATION;
	ticks = 0;
	scFieldActorShowEmote(MainRoutineStateWork.leaderActorId, emote, ticks);
	fdScriptWait(16);
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 488, 440);
	fdScriptWait(16);
	fdCameraFollowActor(ACTOR_PUNCH_ANT, 1);
	fdCameraWaitForArrival();
	fdActorJump(ACTOR_PUNCH_ANT, 4, 0);
	fdScriptWait(16);
	fdActorJump(ACTOR_PUNCH_ANT, 4, 0);
	fdActorMoveToAndWait(ACTOR_PUNCH_ANT, 488, 480);
	fdScriptWait(30);
	fdActorJump(ACTOR_PUNCH_ANT, 2, 0);
	fdActorMoveToAndWait(ACTOR_PUNCH_ANT, 504, 472);
	fdActorJump(ACTOR_PUNCH_ANT, 4, 0);
	fdActorMoveToAndWait(ACTOR_PUNCH_ANT, 520, 464);
	fdActorJump(ACTOR_PUNCH_ANT, 4, 0);
	fdActorMoveToAndWait(ACTOR_PUNCH_ANT, 528, 456);
	scSuhallaGateFightPunchAnt();
}

/* Back from the Punch Ant's battle, Kraden finds the fight invigorating: "My!
   That was invigorating!" Then he falls in behind Jenna again. */
void scSuhallaGateAfterPunchAntBattle(void)
{
	ACTOR *leader;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)&MsgMyThatWas);
	fdActorSetPos(ACTOR_CAVE_KRADEN, 512, 432);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 528, 432);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	/* "My! That was invigorating! He wasn't too strong..." */
	fdScriptShowMessageThenWait(ACTOR_CAVE_KRADEN, 0, 10);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_CAVE_KRADEN, 2);
	fdScriptWait(20);
	/* "I never knew fighting could be so fun! Ah, but we'd better get on to the
	   peninsula..." */
	fdScriptShowMessageThenWait(ACTOR_CAVE_KRADEN, 0, 10);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdScriptWait(30);
	fdScriptSetFlag(FLAG_SUHALLA_GATE_PUNCH_ANT_BEATEN);
	fdActorSetAnimation(ACTOR_CAVE_KRADEN, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader)
		fdActorSetDestination(ACTOR_CAVE_KRADEN, leader->x.part[0].integer,
			leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_CAVE_KRADEN);
	fdActorSetPos(ACTOR_CAVE_KRADEN, 0, 0);
	fdScriptWait(10);
	fdScriptEnd();
}

/* Walking onto metatiles (31, 54), (32, 54) or (31, 55) on the cliffs, away
   from the crevice, Kraden jumps out of the party beside Jenna: "Not that way,
   Jenna! The meeting spot is down this way." She steps back east. */
void scSuhallaGatePointToMeetingSpot(void)
{
	ACTOR *leader;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)&MsgNotThatWay);
	fdActorLaunchFromLeader(ACTOR_CLIFFS_KRADEN, -16, 0, FACING_EAST);
	fdActorWaitForMoveById(ACTOR_CLIFFS_KRADEN);
	/* "Not that way, Jenna! The meeting spot is down this way." "We must climb
	   down here and go into that cave." */
	fdScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_CLIFFS_KRADEN, 0, 10);
	fdActorSetAnimation(ACTOR_CLIFFS_KRADEN, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader)
		fdActorSetDestination(ACTOR_CLIFFS_KRADEN, leader->x.part[0].integer,
			leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_CLIFFS_KRADEN);
	fdActorSetPos(ACTOR_CLIFFS_KRADEN, 0, 0);
	fdScriptWait(10);
	fdActorWalkByAndWait(MainRoutineStateWork.leaderActorId, 16, 0);
	fdScriptEnd();
}

/* clang-format off */
/* Unused: a script that squashes and stretches its actor over and over; */
/* nothing runs it. */
Sint32 SuhallaGateSquashScript[] = {
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
	SC_WAIT(4),
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
	SC_WAIT(8),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_WAIT(5),
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
	SC_WAIT(4),
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
	SC_WAIT(8),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

SCENE_ENTRANCE SuhallaGateRoadEntrances[] = {
	{0, CONDITION_ALWAYS, 152, 0, 504, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
	{1, CONDITION_ALWAYS, 152, 0, 520, FACING_NORTH, 0, 32, 32, 272, 544, 0},
	{SUHALLA_GATE_ROAD_FROM_SUHALLA_GATE_AMBUSH_A, CONDITION_ALWAYS, 248, 0, 280, FACING_WEST, 0, 32, 32, 272, 544, 0},
	{3, CONDITION_ALWAYS, 136, 0, 304, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
	{SUHALLA_GATE_ROAD_FROM_SUHALLA_GATE_AMBUSH_B, CONDITION_ALWAYS, 360, 0, 296, FACING_EAST, 0, 336, 32, 736, 496, 0},
	{SUHALLA_GATE_ROAD_FROM_VENUS_LIGHTHOUSE_ENTRANCE_EAST, CONDITION_ALWAYS, 664, 0, 424, FACING_WEST, 0, 336, 32, 736, 496, 0},
	{ENTRANCE_AFTER_SUHALLA_GATE_BATTLE, CONDITION_ALWAYS, 472, 0, 312, FACING_WEST, 0, 336, 32, 736, 496, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE SuhallaGateCliffsEntrances[] = {
	{SUHALLA_GATE_AMBUSH_FROM_SUHALLA_GATE_ROAD_A, CONDITION_ALWAYS, 40, 0, 304, FACING_EAST, 0, 16, 32, 416, 480, 0},
	{2, CONDITION_ALWAYS, 392, 0, 368, FACING_WEST, 0, 16, 32, 416, 480, 0},
	{3, CONDITION_ALWAYS, 40, 0, 912, FACING_EAST, 0, 16, 608, 720, 1008, 0},
	{SUHALLA_GATE_AMBUSH_FROM_SUHALLA_GATE_ROAD_B, CONDITION_ALWAYS, 680, 0, 904, FACING_WEST, 0, 16, 608, 720, 1008, 0},
	{SUHALLA_GATE_AMBUSH_FROM_SUHALLA_GATE_PSYNERGY_STONE, CONDITION_ALWAYS, 616, 0, 920, FACING_SOUTH, 0, 16, 608, 720, 1008, 0},
	{ENTRANCE_AFTER_SUHALLA_GATE_BATTLE, CONDITION_ALWAYS, 616, 0, 920, FACING_SOUTH, 0, 16, 608, 720, 1008, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE SuhallaGateCaveEntrances[] = {
	{SUHALLA_GATE_PSYNERGY_STONE_FROM_SUHALLA_GATE_AMBUSH, CONDITION_ALWAYS, 72, 0, 264, FACING_NORTH, 0, 24, 24, 280, 256, 0},
	{2, CONDITION_ALWAYS, 184, 0, 168, FACING_SOUTH, 0, 24, 24, 280, 256, 0},
	{3, CONDITION_ALWAYS, 280, 0, 344, FACING_SOUTH, 0, 208, 288, 704, 640, 0},
	{4, CONDITION_ALWAYS, 664, 0, 616, FACING_NORTH, 0, 208, 288, 704, 640, 0},
	{ENTRANCE_AFTER_SUHALLA_GATE_BATTLE, CONDITION_ALWAYS, 528, 0, 432, FACING_SOUTH, 0, 208, 288, 704, 640, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(SuhallaGateRoad),
	SCENE_EXIT(SUHALLA_GATE_ROAD_TO_SUHALLA_GATE_AMBUSH_A, SuhallaGateAmbush, SUHALLA_GATE_AMBUSH_FROM_SUHALLA_GATE_ROAD_A),
	SCENE_EXIT(SUHALLA_GATE_ROAD_TO_SUHALLA_GATE_AMBUSH_B, SuhallaGateAmbush, SUHALLA_GATE_AMBUSH_FROM_SUHALLA_GATE_ROAD_B),
	SCENE_EXIT(SUHALLA_GATE_ROAD_TO_VENUS_LIGHTHOUSE_ENTRANCE_EAST, VenusLighthouseEntrance, VENUS_LIGHTHOUSE_ENTRANCE_FROM_SUHALLA_GATE_ROAD_EAST),
	SCENE_EXITS(SuhallaGateAmbush),
	SCENE_EXIT(SUHALLA_GATE_AMBUSH_TO_SUHALLA_GATE_ROAD_A, SuhallaGateRoad, SUHALLA_GATE_ROAD_FROM_SUHALLA_GATE_AMBUSH_A),
	SCENE_EXIT(2, SuhallaGateAmbush, 3),
	SCENE_EXIT(3, SuhallaGateAmbush, 2),
	SCENE_EXIT(SUHALLA_GATE_AMBUSH_TO_SUHALLA_GATE_ROAD_B, SuhallaGateRoad, SUHALLA_GATE_ROAD_FROM_SUHALLA_GATE_AMBUSH_B),
	SCENE_EXIT(SUHALLA_GATE_AMBUSH_TO_SUHALLA_GATE_PSYNERGY_STONE, SuhallaGatePsynergyStone, SUHALLA_GATE_PSYNERGY_STONE_FROM_SUHALLA_GATE_AMBUSH),
	SCENE_EXITS(SuhallaGatePsynergyStone),
	SCENE_EXIT(SUHALLA_GATE_PSYNERGY_STONE_TO_SUHALLA_GATE_AMBUSH, SuhallaGateAmbush, SUHALLA_GATE_AMBUSH_FROM_SUHALLA_GATE_PSYNERGY_STONE),
	SCENE_EXIT(2, SuhallaGatePsynergyStone, 3),
	SCENE_EXIT(3, SuhallaGatePsynergyStone, 2),
	SCENE_EXIT(SUHALLA_GATE_PSYNERGY_STONE_TO_IDEJIMA, Idejima, IDEJIMA_FROM_WORLD_MAP_NORTH_A),
	SCENE_EXITS_END,
};

ACTOR_PLACE SuhallaGateCavePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PUNCH_ANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE SuhallaGateCliffsPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 264, 0, 400, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 280, 0, 368, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 472, 0, 912, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 912, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 552, 0, 880, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 24, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 24, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 24, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE SuhallaGateRoadPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 264, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

CUE SuhallaGateRoadCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, SUHALLA_GATE_ROAD_TO_SUHALLA_GATE_AMBUSH_A},
	{CUE_EXIT, 4, CONDITION_ALWAYS, SUHALLA_GATE_ROAD_TO_SUHALLA_GATE_AMBUSH_B},
	{CUE_EXIT, 5, CONDITION_ALWAYS,
		SUHALLA_GATE_ROAD_TO_VENUS_LIGHTHOUSE_ENTRANCE_EAST},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), ROAD_DOOR_METATILE, CONDITION_ALWAYS,
		SCRIPT_REF(scSuhallaGateEnterRockDoor)},
	{CUE_STEP, RUFFIAN_SPOTS_PARTY_METATILE, FLAG_SUHALLA_GATE_RUFFIAN_BEATEN,
		SCRIPT_REF(scSuhallaGateRuffianSpotsParty)},
	/* Unused: no metatile of the road's map runs it. */
	{CUE_STEP, ROAD_UNUSED_METATILE, CONDITION_ALWAYS,
		SCRIPT_REF(scSuhallaGatePointToMeetingSpot)},
	{CUE_TALK, ACTOR_ROAD_RUFFIAN, CONDITION_ALWAYS, MSGNO(MsgOoooogMaybeI)},
	{CUE_TABLE_END},
};

CUE SuhallaGateCliffsCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, SUHALLA_GATE_AMBUSH_TO_SUHALLA_GATE_ROAD_A},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, SUHALLA_GATE_AMBUSH_TO_SUHALLA_GATE_ROAD_B},
	/* The way into the cave, until the ambush is beaten. */
	{CUE_STEP, 5, FLAG_SUHALLA_GATE_AMBUSH_BEATEN,
		SCRIPT_REF(scSuhallaGateRuffiansAmbushParty)},
	{CUE_EXIT, 5, CONDITION_ALWAYS,
		SUHALLA_GATE_AMBUSH_TO_SUHALLA_GATE_PSYNERGY_STONE},
	{CUE_STEP, KRADEN_POINTS_THE_WAY_METATILE, CONDITION_ALWAYS,
		SCRIPT_REF(scSuhallaGatePointToMeetingSpot)},
	{CUE_TALK, ACTOR_WEST_RUFFIAN, CONDITION_ALWAYS, MSGNO(MsgMaybeIShould)},
	{CUE_TALK, ACTOR_MIDDLE_RUFFIAN, CONDITION_ALWAYS, MSGNO(MsgAllRightSo)},
	{CUE_TALK, ACTOR_EAST_RUFFIAN, CONDITION_ALWAYS, MSGNO(MsgOhManThere)},
	{CUE_TALK, ACTOR_WEST_RUFFIAN, CONDITION_ALWAYS,
		SCRIPT_REF(scSuhallaGateFightDew)},
	{EXAMINE_CUE(EXAMINE_CHEST), CLIFFS_CHEST_METATILE, FLAG_SUHALLA_GATE_MINT,
		FIND_ITEM(ITEM_MINT)},
	{CUE_TABLE_END},
};

CUE SuhallaGateCaveCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS,
		SUHALLA_GATE_PSYNERGY_STONE_TO_SUHALLA_GATE_AMBUSH},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
	{CUE_STEP, SUHALLA_GATE_PSYNERGY_STONE_TO_IDEJIMA, CONDITION_ALWAYS,
		SCRIPT_REF(scSuhallaGateLeaveForIdejima)},
	{CUE_STEP, PUNCH_ANT_METATILE, FLAG_SUHALLA_GATE_PUNCH_ANT_BEATEN,
		SCRIPT_REF(scSuhallaGatePunchAntJumpsOut)},
	{CUE_STEP, CAVE_UNUSED_METATILE, FLAG_SUHALLA_GATE_PUNCH_ANT_BEATEN,
		SCRIPT_REF(scSuhallaGatePunchAntJumpsOutAsLeaderStepsAside)},
	{CUE_TABLE_END},
};
