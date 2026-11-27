/* Suhalla Gate and Gateway Cave, the rocky pass Babi's soldiers guard on the
   way from the Suhalla Desert to Lalivero and Venus Lighthouse.

   Three scenes share this overlay, in the order the pass is crossed:

   - SuhallaGateCheckpoint (area name Suhalla Gate): the gate itself, a
	 palisade of logs across the path north from the desert, with a house
	 in the cliff just past it (rock_rooms.c). The same map holds, in its
	 own part to the east, the pass's way out toward Venus Lighthouse. Two
	 soldiers keep the gate.
   - SuhallaGateDew (area name Suhalla Gate): the cliffs beyond the gate, a
	 west part and an east part joined by a path. In the west part the first
	 slide down leads to a chest with a Mint; in the east part the third
	 slide from the west leads down to Dew, the Mercury Djinni, and green
	 vines climb back up the cliffs.
   - Gateway Cave (area name Gateway Cave): a cave entered from the cliffs'
	 east part, two rooms that lead out onto the World Map, with a Psynergy
	 Stone.

   One exit table serves the three scenes. The other getters choose by
   MainRoutineStateWork.next, and SuhallaGateCheckpoint's actor placements also
   by FLAG_BABI_SPOKE_AFTER_COLOSSO. Its cue table answers any scene not
   otherwise named.

   Before Babi's audience after Colosso the guards turn the party away for want
   of travel papers: stepping up to the gate (metatiles (7, 26) to (9, 27))
   brings them together in front of its opening
   (scSuhallaGateGuardsBlockPassage).

   After the audience the party comes with Iodem and finds them lying wounded
   before the gate, and a third soldier lying on the path out toward Venus
   Lighthouse. Iodem's actor waits out of sight. Stepping up to the gate then
   plays scSuhallaGateIodemQuestionsGuards once (FLAG_SUHALLA_GATE_ATTACK_SEEN),
   after which a defeat returns the party to the house past the gate.

   The third soldier, talked to or read, gasps that Venus Lighthouse was
   attacked and fades away for good (scSuhallaGateSoldierVanishes,
   FLAG_SUHALLA_GATE_SOLDIER_VANISHED).

   scInit remembers which side of the gate the party came in by
   (FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH, FLAG_SUHALLA_GATE_ENTERED_FROM_EAST),
   so that Retreat from the house leads back out that way. Leaving for the World
   Map forgets it.

   In the French, Spanish and Italian editions, the editions lay the wounded
   soldiers out without collision and patch the gate's metatiles around them,
   and Retreat always leads back to SuhallaGateCheckpoint.

   Gateway Cave's Psynergy Stone is kept in an area flag
   (FLAG_GATEWAY_CAVE_PSYNERGY_STONE), so it is back whenever the party returns
   from another area. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "graphics/graphics.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE SuhallaGateEntrancesOther[];
extern SCENE_ENTRANCE SuhallaGateDewEntrances[];
extern SCENE_ENTRANCE GatewayCaveEntrances[];
extern Uint32		  SuhallaGateExits[];
extern ACTOR_PLACE	  SuhallaGateActorPlacementsOther[];
extern ACTOR_PLACE	  SuhallaGateDewActorPlacements[];
extern ACTOR_PLACE	  SuhallaGateCheckpointActorPlacements[];
extern ACTOR_PLACE	  SuhallaGateCheckpointActorPlacementsAfterColosso[];
extern CUE			  SuhallaGateCuesOther[];
extern CUE			  SuhallaGateDewCues[];
extern CUE			  GatewayCaveCues[];

/* Gateway Cave's Psynergy Stone has been found.

   It is an area flag, so the stone is back once the party returns from another
   area. */
#define FLAG_GATEWAY_CAVE_PSYNERGY_STONE FLAG_AREA_TEMP_50

/* The metatiles before Suhalla Gate, (7, 26) to (9, 27), where the guards block
   the way or Iodem finds them wounded. The other metatiles' behaviors are the
   numbers of the exits they lead to. */
typedef enum SUHALLA_GATE_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_SUHALLA_GATE_APPROACH = 10
} SUHALLA_GATE_METATILE_BEHAVIOR;

/* SPRITE_SOLDIER's animation lying on his side, held (soldier.tsv). */
enum { SOLDIER_ANIMATION_LIE_WOUNDED = 5 };

/* Where Retreat leads out of the checkpoint house: in the French, Spanish and
   Italian editions the checkpoint, before it the scene the party is in. */
#if SUHALLA_GATE_WOUNDED_SOLDIERS_NO_COLLISION
#define SUHALLA_GATE_RETREAT_MAP(currentMap) (SCENE_SUHALLA_GATE_CHECKPOINT)
#else
#define SUHALLA_GATE_RETREAT_MAP(currentMap) (currentMap)
#endif

/* The actors of the gate itself (SuhallaGateCheckpoint), numbered in the order
   of SuhallaGateCheckpointActorPlacements and, after Babi's audience, of
   SuhallaGateCheckpointActorPlacementsAfterColosso. */
typedef enum SUHALLA_GATE_CHECKPOINT_ACTOR_ID {
	/* The two of Babi's soldiers who keep the gate, standing before Babi's
	   audience and lying wounded after it. The west guard says only those with
	   travel papers may pass, and later asks the party to send help from Babi
	   Lighthouse. */
	ACTOR_WEST_GUARD = ACTOR_FIRST_PLACED,
	/* Asks whether the party wants to go to Babi Lighthouse. */
	ACTOR_EAST_GUARD,
	/* After Babi's audience, the soldier lying on the path out toward Venus
	   Lighthouse, who tells of the attack on it and vanishes. */
	ACTOR_VANISHING_SOLDIER,
	/* After Babi's audience, out of sight until
	   scSuhallaGateIodemQuestionsGuards brings him out of the party. */
	ACTOR_IODEM
} SUHALLA_GATE_CHECKPOINT_ACTOR_ID;

/* The actors of the second scene (SuhallaGateDew), numbered in the order of
   SuhallaGateDewActorPlacements. */
typedef enum SUHALLA_GATE_DEW_ACTOR_ID {
	/* Five green vines up the cliffs, drawn with the sprout Growth grows
	   (SPRITE_SPROUT), three fully grown and two half grown. */
	ACTOR_FIRST_SPROUT = ACTOR_FIRST_PLACED,
	/* A Mercury Djinni. */
	ACTOR_DEW = ACTOR_FIRST_SPROUT + 5,
	/* The eighth placed actor, where the scene places six: scInit stretches it
	   all the same. */
	ACTOR_EIGHTH_PLACED = ACTOR_FIRST_PLACED + 7
} SUHALLA_GATE_DEW_ACTOR_ID;

/* Talking to Dew, the Mercury Djinni at the foot of the third slide in the
   cliffs' east part beyond Suhalla Gate (metatile (16, 55)), starts its
   battle. */
static void scSuhallaGateFightDew(void)
{
	fdRequestDjinnBattle(ACTOR_DEW, DJINNI_DEW);
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_SUHALLA_GATE_DEW) {
		return SuhallaGateDewEntrances;
	}
	if(sceneId == SCENE_GATEWAY_CAVE) {
		return GatewayCaveEntrances;
	}
	return SuhallaGateEntrancesOther;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return SuhallaGateExits;
}

/* Suhalla Gate's guards, standing or wounded, or Dew and the vines of the
   cliffs beyond. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_SUHALLA_GATE_DEW) {
		return SuhallaGateDewActorPlacements;
	}
	if(sceneId == SCENE_SUHALLA_GATE_CHECKPOINT) {
		if(gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
			return SuhallaGateCheckpointActorPlacementsAfterColosso;
		}
		return SuhallaGateCheckpointActorPlacements;
	}
	return SuhallaGateActorPlacementsOther;
}

/* The scripts of the cliffs beyond the gate, of Gateway Cave, or of the gate
   itself for any other scene. */
const CUE *scGetCues(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_SUHALLA_GATE_DEW) {
		return SuhallaGateDewCues;
	}
	if(sceneId == SCENE_GATEWAY_CAVE) {
		return GatewayCaveCues;
	}
	return SuhallaGateCuesOther;
}

/* Sets up whichever of the pass's scenes is entered: after the attack a defeat
   leads back to the house past the gate, the gate remembers which way the party
   came in, and the cliffs' vines are drawn grown. */
Sint32 scInit(void)
{
	Sint32 sceneId;

	if(gmFlagGet(FLAG_SUHALLA_GATE_ATTACK_SEEN)) {
		MainRoutineStateWork.restartScene = SCENE_ROCK_ROOMS;
		MainRoutineStateWork.restartEntrance =
			ROCK_ROOMS_FROM_SUHALLA_GATE_CHECKPOINT;
	}
	sceneId = MainRoutineStateWork.next;
	if(sceneId == SCENE_SUHALLA_GATE_CHECKPOINT) {
		if(gmFlagGet(FLAG_SUHALLA_GATE_SOLDIER_VANISHED))
			fdActorSetPos(ACTOR_VANISHING_SOLDIER, 0, 0);
#if SUHALLA_GATE_WOUNDED_SOLDIERS_BLOCK_BY_WALL_CELLS
		/* The wounded soldiers lie without collision here, so the two metatiles
		   under each are taken off the path, copied from the empty metatile (0,
		   0) at height 0: the third soldier's (29, 18) and (30, 18), the west
		   guard's (8, 25) and (9, 25), the east guard's (9, 26) and (10,
		   26). */
		else if(gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO))
			fdMapGridCopyMetatileAttributes(0, 0, 2, 1, 29, 18);
		if(gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
			fdMapGridCopyMetatileAttributes(0, 0, 2, 1, 8, 25);
			fdMapGridCopyMetatileAttributes(0, 0, 2, 1, 9, 26);
		}
#endif
		/* Out of the house, Retreat leads back to the side the party came in
		   by, and the area's name is not shown again. */
		if(MainRoutineStateWork.entrance ==
			SUHALLA_GATE_CHECKPOINT_FROM_ROCK_ROOMS) {
			if(gmFlagGet(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH)) {
				MainRoutineStateWork.retreatScene =
					SUHALLA_GATE_RETREAT_MAP(sceneId);
				MainRoutineStateWork.retreatEntrance =
					SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH;
			}
			if(gmFlagGet(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST)) {
				MainRoutineStateWork.retreatScene =
					SUHALLA_GATE_RETREAT_MAP(sceneId);
				MainRoutineStateWork.retreatEntrance =
					SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST;
			}
			gmFlagClear(FLAG_SHOW_AREA_NAME);
		}
		if(MainRoutineStateWork.entrance ==
			SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH) {
			gmFlagSet(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH);
			/* Coming up from the desert before the audience, the top of the
			   slope below the gate, (8, 27) and (9, 27), loses the guards'
			   approach behavior for the plain slope kept at (6, 0); the gate's
			   own metatiles still bring them out. */
			if(!gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO))
				fdMapGridCopyMetatileAttributes(6, 0, 2, 1, 8, 27);
		}
		if(MainRoutineStateWork.entrance ==
			SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST)
			gmFlagSet(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST);
	} else if(sceneId == SCENE_SUHALLA_GATE_DEW) {
		fdActorSetAnimation(ACTOR_FIRST_SPROUT, SPROUT_ANIMATION_GROWN);
		fdActorSetAnimation(ACTOR_FIRST_SPROUT + 1, SPROUT_ANIMATION_GROWN);
		fdActorSetAnimation(
			ACTOR_FIRST_SPROUT + 2, SPROUT_ANIMATION_HALF_GROWN);
		fdActorSetAnimation(ACTOR_FIRST_SPROUT + 3, SPROUT_ANIMATION_GROWN);
		fdActorSetAnimation(
			ACTOR_FIRST_SPROUT + 4, SPROUT_ANIMATION_HALF_GROWN);
		/* UB: the scene places only actors up to ACTOR_DEW, so there is no
		   ACTOR_EIGHTH_PLACED and fdActorGet returns NULL; the store goes to
		   the BIOS, which ignores it. */
#if !FIX_NULL_DEREFERENCE
		fdActorGet(ACTOR_EIGHTH_PLACED)->scaleY = toFIXED(8) / 5;
#endif
		/* Map layer 1, whose metatiles begin at the map's column 64, takes
		   Gateway Cave's exit (SUHALLA_GATE_DEW_TO_GATEWAY_CAVE, from its spare
		   metatile (44, 38)) at its metatile (38, 56), just north of the exit
		   at the cave mouth on the ground, (38, 57). */
		fdMapGridCopyMetatileAttributes(108, 38, 1, 1, 102, 56);
	}
	return 0;
}

/* Walking north into the door in the cliff just past Suhalla Gate (metatile (8,
   18)) opens it and takes the party into the house. */
static void scSuhallaGateEnterHouse(void)
{
	Sint32 exitCue;

	exitCue = FieldStateWork->stepCue;
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridCopyMetatilesTo(66, 36, 71, 8, 2, 2);
	sySynch(4);
	fdMapGridCopyMetatilesTo(68, 36, 71, 8, 2, 2);
	sySynch(4);
	fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
	syImportScriptSetExitRequest(exitCue);
}

/* Walking off Suhalla Gate's map, south down the path to the desert or east
   toward Venus Lighthouse, leaves for the World Map.

   The gate forgets which way the party came in. */
static void scSuhallaGateLeaveForWorldMap(void)
{
	Sint16 exitCue = FieldStateWork->stepCue;

	sySndPlay(SE_MAP_EXIT);
	gmFlagClear(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH);
	gmFlagClear(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST);
	syImportScriptSetExitRequest(exitCue);
}

/* Before Babi's audience, stepping up to Suhalla Gate (metatiles (7, 26) to (9,
   27)) makes the two guards, who stand either side of its opening, step
   together in front of it and face the party, blocking the way north.

   Then the cue metatiles at (7, 26), (8, 26) and (7, 27) take the attributes of
   their neighbours. */
static void scSuhallaGateGuardsBlockPassage(void)
{
	fdScriptStart();
	fdActorSetSpeed(ACTOR_WEST_GUARD, 1);
	fdActorSetSpeed(ACTOR_EAST_GUARD, 1);
	fdActorWalkTo(ACTOR_WEST_GUARD, 136, 384);
	fdActorWalkToAndWait(ACTOR_EAST_GUARD, 152, 384);
	fdActorFaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH, 0);
	fdActorSetAnimation(ACTOR_WEST_GUARD, ANIM_STAND);
	/* The gate's approach metatiles go back to plain ground until the party
	   comes again. In the French, Spanish and Italian editions they are copied
	   from (11, 26), clear of the east guard's metatiles, which are taken off
	   the path once he lies wounded. */
	fdMapGridCopyMetatileAttributes(6, 27, 1, 1, 7, 27);
#if SUHALLA_GATE_WOUNDED_SOLDIERS_BLOCK_BY_WALL_CELLS
	fdMapGridCopyMetatileAttributes(11, 26, 1, 1, 7, 26);
	fdMapGridCopyMetatileAttributes(11, 26, 1, 1, 8, 26);
#else
	fdMapGridCopyMetatileAttributes(9, 26, 2, 1, 7, 26);
#endif
	fdScriptEnd();
}

/* After Babi's audience, stepping up to Suhalla Gate (metatiles (7, 26) to (9,
   27)) finds the two guards lying wounded before it; Iodem steps out of the
   party and demands to know what happened.

   They tell of a strange group with no papers and strange powers, Iodem says it
   must have been Felix, and the guards send the party on after them. */
static void scSuhallaGateIodemQuestionsGuards(void)
{
	ACTOR *leader;

	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.6);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 120, 438);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
	}
	sySynch(1);
	fdActorSetSpeed(ACTOR_IODEM, 1.6);
	fdActorWalkToAndWait(ACTOR_IODEM, 108, 431);
	fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHEAST, 10);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 40);
	fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 20);
	fdActorStartRepeatedMotion(ACTOR_IODEM, 2);
	fdScriptSetMessageId(MsgSuhallaMeaning);
	/* "What is the meaning of this!?" */
	fdScriptShowMessageThenWait(ACTOR_IODEM, 0, 40);
	/* The guards describe the attack before Iodem asks who was responsible. */
	fdActorShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorRunRepeatedMotion(ACTOR_WEST_GUARD, 2);
	/* "Uh! Is... Is that Iodem...?" */
	fdScriptShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
	fdActorWalkToAndWait(ACTOR_IODEM, 132, 420);
	fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdActorWalkToAndWait(ACTOR_IODEM, 138, 416);
	fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHWEST, 10);
	fdActorStartRepeatedMotion(ACTOR_IODEM, 2);
	/* "What has happened to the gate?" */
	fdScriptShowMessageThenWait(ACTOR_IODEM, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_WEST_GUARD, 2);
	/* "There was a strange group... No papers... They wanted to pass..." */
	fdScriptShowMessageThenWait(ACTOR_WEST_GUARD, 0, 40);
	fdActorShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorRunRepeatedMotion(ACTOR_EAST_GUARD, 2);
	/* "Uhh... Iodem..." */
	fdScriptShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorWalkToAndWait(ACTOR_IODEM, 144, 420);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_EAST_GUARD, 2);
	/* "We tried to hold them back...but they were too powerful!" */
	fdScriptShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
	fdActorSetAttachedEffect(
		ACTOR_EAST_GUARD, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_EAST_GUARD, 3);
	/* "They had strange powers... We were no match for them." */
	fdScriptShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH_SOUTHWEST, 20);
	/* The party answers Iodem, who, once he has heard the wounded soldiers in
	   Suhalla, adds that they must find Sheba, then track down the monsters.
	   "It must have been Felix!" Yes: "Yeah... No one else is capable of this."
	   No: "Don't you know the work of your own foe's hand? Who else..." */
	fdScriptAskYesNo(ACTOR_IODEM, 0);
	if(gmFlagGet(FLAG_IODEM_MET_WOUNDED_SOLDIERS) != 0) {
		fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHEAST, 40);
		fdActorSetAttachedEffect(
			ACTOR_IODEM, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(40);
		/* "After we find Sheba, we must track down the monsters that..." */
		fdScriptShowMessageThenWait(ACTOR_IODEM, 0, 10);
	} else {
		/* Skips "After we find Sheba...". */
		FieldStateWork->msg += 1;
	}
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH_SOUTHWEST, 10);
	/* "We must get to Babi Lighthouse as soon as possible!" */
	fdScriptShowMessageThenWait(ACTOR_IODEM, 0, 40);
	fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHWEST, 10);
	/* "Yes, but we cannot leave the soldiers like this..." */
	fdScriptShowMessageThenWait(ACTOR_IODEM, 0, 10);
	fdActorWalkToAndWait(ACTOR_IODEM, 138, 416);
	fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHWEST, 20);
	fdActorStartRepeatedMotion(ACTOR_WEST_GUARD, 2);
	/* "We're okay. Don't worry about us! Get after them, please..." */
	fdScriptShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
	fdActorSetAttachedEffect(
		ACTOR_IODEM, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 1);
	fdScriptWait(20);
	/* "But that would be..." */
	fdScriptShowMessageThenWait(ACTOR_IODEM, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_EAST_GUARD, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 10);
	fdActorRunRepeatedMotion(ACTOR_EAST_GUARD, 1);
	fdScriptWait(20);
	/* "We were bested in battle, but we will survive... Now go!" */
	fdScriptShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_WEST_GUARD, 2);
	/* "If they have done anything at the lighthouse... It would be..." */
	fdScriptShowMessageThenWait(ACTOR_WEST_GUARD, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_IODEM, 1);
	fdScriptWait(20);
	/* "He's right. We've got to stop them!" */
	fdScriptShowMessageThenWait(ACTOR_IODEM, 0, 20);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH_SOUTHWEST, 10);
	/* "We're going after them, right, Isaac!" */
	fdScriptShowMessageThenWait(ACTOR_IODEM, 0, 10);
	scWalkIntoLeader(ACTOR_IODEM);
	/* The gate's approach metatiles go back to plain ground, as after the
	   guards block the way. */
#if SUHALLA_GATE_WOUNDED_SOLDIERS_BLOCK_BY_WALL_CELLS
	fdMapGridCopyMetatileAttributes(6, 27, 1, 1, 7, 27);
	fdMapGridCopyMetatileAttributes(11, 26, 1, 1, 7, 26);
	fdMapGridCopyMetatileAttributes(11, 26, 1, 1, 8, 26);
#else
	fdMapGridCopyMetatileAttributes(6, 27, 1, 1, 7, 27);
	fdMapGridCopyMetatileAttributes(9, 26, 2, 1, 7, 26);
#endif
	gmFlagSet(FLAG_SUHALLA_GATE_ATTACK_SEEN);
	fdScriptEnd();
}

/* Talking to the east guard at Suhalla Gate: before Babi's audience he asks
   whether the party wants to go to Babi Lighthouse, and a yes gets a demand for
   travel papers, a no a scoff. After it, lying wounded, he says it is a pity a
   Colosso victor should end up like this and swears to settle the score. */
static void scSuhallaGateTalkToEastGuard(void)
{
	fdScriptStart();

	if(gmFlagGet(FLAG_SUHALLA_GATE_ATTACK_SEEN) != 0) {
		fdScriptSetMessageId(MsgSuhallaPityColossoVictor);
		/* "It's a pity a Colosso victor should end up like this... Next..." */
		fdScriptShowMessage(ACTOR_EAST_GUARD, 0);
	} else {
		fdScriptSetMessageId(MsgSuhallaWantGoBabi);
		/* "Do you want to go to Babi Lighthouse?" */
		fdScriptOpenMessage(ACTOR_EAST_GUARD, 0);

		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			/* Yes: "Show me your travel papers, and I'll let you pass!" */
			fdScriptShowMessage(ACTOR_EAST_GUARD, 0);
			fdActorSetAnimationAndWait(ACTOR_EAST_GUARD, ANIM_SHAKE_HEAD);
			/* "What! You don't have travel papers? Then I cannot let you
			   pass!" */
			fdScriptShowMessage(ACTOR_EAST_GUARD, 0);
		} else {
			FieldStateWork->msg += 2;
			/* No: "Hmmph! You don't want to see Babi Lighthouse? You're a
			   strange one!" */
			fdScriptShowMessage(ACTOR_EAST_GUARD, 0);
		}
	}

	fdScriptEnd();
}

/* The vanishing soldier's blinks: slow ones, four ticks shown and four hidden,
   then fast ones, two and two. */
enum { SOLDIER_SLOW_BLINKS = 6, SOLDIER_FAST_BLINKS = 12 };

/* The wounded soldier on the path out toward Venus Lighthouse, talked to or
   read with Mind Read, gasps that Venus Lighthouse was attacked by strange
   wanderers, then blinks out and is gone (metatile (29, 19)). */
static void scSuhallaGateSoldierVanishes(void)
{
	unsigned int beat;

	fdScriptStart();

	fdScriptSetMessageId(MsgFieldVenusLighthouseWasAttackedBy);
	/* "Venus Lighthouse was... attacked by strange... wanderers..." */
	fdScriptShowMessageThenWait(ACTOR_VANISHING_SOLDIER, 0, 10);

	beat = 0;
	do {
		fdActorSetSprColById(ACTOR_VANISHING_SOLDIER, SPRITE_COLOR_NORMAL);
		fdActorSetSprFlags(
			fdActorGet(ACTOR_VANISHING_SOLDIER), SPRITE_FLAG_SHADOW);
		sySynch(4);

		fdActorSetSprColById(ACTOR_VANISHING_SOLDIER, SPRITE_COLOR_HIDDEN);
		fdActorSetSprFlags(fdActorGet(ACTOR_VANISHING_SOLDIER), 0);
		beat++;
		sySynch(4);
	} while(beat < SOLDIER_SLOW_BLINKS);

	for(beat = 0; beat < SOLDIER_FAST_BLINKS; beat++) {
		fdActorSetSprColById(ACTOR_VANISHING_SOLDIER, SPRITE_COLOR_NORMAL);
		fdActorSetSprFlags(
			fdActorGet(ACTOR_VANISHING_SOLDIER), SPRITE_FLAG_SHADOW);
		sySynch(2);
		fdActorSetSprColById(ACTOR_VANISHING_SOLDIER, SPRITE_COLOR_HIDDEN);
		fdActorSetSprFlags(fdActorGet(ACTOR_VANISHING_SOLDIER), 0);
		sySynch(2);
	}

	fdActorSetPos(ACTOR_VANISHING_SOLDIER, 0, 0);

	gmFlagSet(FLAG_SUHALLA_GATE_SOLDIER_VANISHED);

	/* The path the third soldier lay on is open again. */
#if SUHALLA_GATE_WOUNDED_SOLDIERS_BLOCK_BY_WALL_CELLS
	fdMapGridCopyMetatileAttributes(29, 19, 2, 1, 29, 18);
#endif
	fdScriptEnd();
}

/* How the wounded soldiers lie, the west guard mirrored to lie the other way.
   In the Spanish, French and Italian editions each script first turns collision
   off and runs on into the one after it. */
#if SUHALLA_GATE_WOUNDED_SOLDIERS_BLOCK_BY_WALL_CELLS
Sint32 SuhallaGateWoundedNoCollision[] = {
	SC_SET(SW_COLLISION, 0),
};
#endif

Sint32 SuhallaGateWounded[] = {
	SC_ANIMATION(SOLDIER_ANIMATION_LIE_WOUNDED),
	SC_END,
};

#if SUHALLA_GATE_WOUNDED_SOLDIERS_BLOCK_BY_WALL_CELLS
Sint32 SuhallaGateWoundedFlippedNoCollision[] = {
	SC_SET(SW_COLLISION, 0),
};
#endif

Sint32 SuhallaGateWoundedFlipped[] = {
	SC_SET(SW_SCALE_X, -toFIXED(1)),
	SC_ANIMATION(SOLDIER_ANIMATION_LIE_WOUNDED),
	SC_END,
};

SCENE_ENTRANCE SuhallaGateEntrancesOther[] = {
	{0, CONDITION_ALWAYS, 152, 0, 504, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
	{SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 152, 0,
		520, FACING_NORTH, 0, 32, 32, 272, 544, 0},
	{SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_WEST, CONDITION_ALWAYS, 248,
		0, 280, FACING_WEST, 0, 32, 32, 272, 544, 0},
	{SUHALLA_GATE_CHECKPOINT_FROM_ROCK_ROOMS, CONDITION_ALWAYS, 136, 0, 304,
		FACING_SOUTH, 0, 32, 32, 272, 544, 0},
	{SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_EAST, CONDITION_ALWAYS, 360,
		0, 296, FACING_EAST, 0, 336, 32, 736, 496, 0},
	{SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 664, 0, 424,
		FACING_WEST, 0, 336, 32, 736, 496, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE SuhallaGateDewEntrances[] = {
	{SUHALLA_GATE_DEW_FROM_SUHALLA_GATE_CHECKPOINT_WEST, CONDITION_ALWAYS, 40,
		0, 304, FACING_EAST, 0, 16, 32, 416, 480, 0},
	{SUHALLA_GATE_DEW_WEST_FROM_EAST, CONDITION_ALWAYS, 392, 0, 368,
		FACING_WEST, 0, 16, 32, 416, 480, 0},
	{SUHALLA_GATE_DEW_EAST_FROM_WEST, CONDITION_ALWAYS, 40, 0, 912, FACING_EAST,
		0, 16, 608, 720, 1008, 0},
	{SUHALLA_GATE_DEW_FROM_SUHALLA_GATE_CHECKPOINT_EAST, CONDITION_ALWAYS, 680,
		0, 904, FACING_WEST, 0, 16, 608, 720, 1008, 0},
	{SUHALLA_GATE_DEW_FROM_GATEWAY_CAVE, CONDITION_ALWAYS, 616, 0, 920,
		FACING_SOUTH, 0, 16, 608, 720, 1008, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE GatewayCaveEntrances[] = {
	{GATEWAY_CAVE_FROM_SUHALLA_GATE_DEW, CONDITION_ALWAYS, 72, 0, 264,
		FACING_NORTH, 0, 24, 24, 280, 256, 0},
	{GATEWAY_CAVE_NORTH_FROM_SOUTH, CONDITION_ALWAYS, 184, 0, 168, FACING_SOUTH,
		0, 24, 24, 280, 256, 0},
	{GATEWAY_CAVE_SOUTH_FROM_NORTH, CONDITION_ALWAYS, 280, 0, 344, FACING_SOUTH,
		0, 208, 288, 704, 640, 0},
	{GATEWAY_CAVE_FROM_WORLD_MAP, CONDITION_ALWAYS, 664, 0, 616, FACING_NORTH,
		0, 208, 288, 704, 640, 0},
	{SCENE_TABLE_END},
};

Uint32 SuhallaGateExits[] = {
	SCENE_EXITS(SuhallaGateCheckpoint),
	SCENE_EXIT(SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_SOUTH, WorldMap,
		WORLD_MAP_FROM_SUHALLA_GATE_CHECKPOINT_SOUTH),
	SCENE_EXIT(SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_WEST, SuhallaGateDew,
		SUHALLA_GATE_DEW_FROM_SUHALLA_GATE_CHECKPOINT_WEST),
	SCENE_EXIT(SUHALLA_GATE_CHECKPOINT_TO_ROCK_ROOMS, RockRooms,
		ROCK_ROOMS_FROM_SUHALLA_GATE_CHECKPOINT),
	SCENE_EXIT(SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_EAST, SuhallaGateDew,
		SUHALLA_GATE_DEW_FROM_SUHALLA_GATE_CHECKPOINT_EAST),
	SCENE_EXIT(SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_EAST, WorldMap,
		WORLD_MAP_FROM_SUHALLA_GATE_CHECKPOINT_EAST),
	SCENE_EXITS(SuhallaGateDew),
	SCENE_EXIT(SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_WEST,
		SuhallaGateCheckpoint,
		SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_WEST),
	SCENE_EXIT(SUHALLA_GATE_DEW_WEST_TO_EAST, SuhallaGateDew,
		SUHALLA_GATE_DEW_EAST_FROM_WEST),
	SCENE_EXIT(SUHALLA_GATE_DEW_EAST_TO_WEST, SuhallaGateDew,
		SUHALLA_GATE_DEW_WEST_FROM_EAST),
	SCENE_EXIT(SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_EAST,
		SuhallaGateCheckpoint,
		SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_EAST),
	SCENE_EXIT(SUHALLA_GATE_DEW_TO_GATEWAY_CAVE, GatewayCave,
		GATEWAY_CAVE_FROM_SUHALLA_GATE_DEW),
	SCENE_EXITS(GatewayCave),
	SCENE_EXIT(GATEWAY_CAVE_TO_SUHALLA_GATE_DEW, SuhallaGateDew,
		SUHALLA_GATE_DEW_FROM_GATEWAY_CAVE),
	SCENE_EXIT(GATEWAY_CAVE_NORTH_TO_SOUTH, GatewayCave,
		GATEWAY_CAVE_SOUTH_FROM_NORTH),
	SCENE_EXIT(GATEWAY_CAVE_SOUTH_TO_NORTH, GatewayCave,
		GATEWAY_CAVE_NORTH_FROM_SOUTH),
	SCENE_EXIT(
		GATEWAY_CAVE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_GATEWAY_CAVE),
	SCENE_EXITS_END,
};

ACTOR_PLACE SuhallaGateActorPlacementsOther[] = {
	{SCENE_TABLE_END},
};

/* clang-format off */
ACTOR_PLACE SuhallaGateDewActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		264, 0, 400, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		280, 0, 368, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		472, 0, 912, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		536, 0, 912, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		552, 0, 880, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_DEW), MOVEMENT_TYPE_STAND,
		264, 0, 888, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE SuhallaGateCheckpointActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		120, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		168, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE SuhallaGateCheckpointActorPlacementsAfterColosso[] = {
#if !SUHALLA_GATE_WOUNDED_SOLDIERS_BLOCK_BY_WALL_CELLS
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, (Sint32)SuhallaGateWoundedFlipped,
		136, 0, 408, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, (Sint32)SuhallaGateWounded,
		168, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, (Sint32)SuhallaGateWounded,
		472, 0, 304, FACING_SOUTH, TALK_FACING_PARTY, 0),
#else
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, (Sint32)SuhallaGateWoundedFlippedNoCollision,
		140, 0, 408, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, (Sint32)SuhallaGateWoundedNoCollision,
		164, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, (Sint32)SuhallaGateWoundedNoCollision,
		476, 0, 300, FACING_SOUTH, TALK_FACING_PARTY, 0),
#endif
	ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

CUE SuhallaGateCuesOther[] = {
	{ CUE_STEP, SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaGateLeaveForWorldMap) },
	{ CUE_EXIT, SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_WEST, CONDITION_ALWAYS, SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_WEST },
	{ CUE_EXIT, SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_EAST, CONDITION_ALWAYS, SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_EAST },
	{ CUE_STEP, SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_EAST, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaGateLeaveForWorldMap) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), SUHALLA_GATE_CHECKPOINT_TO_ROCK_ROOMS, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaGateEnterHouse) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_GATE_APPROACH, FLAG_BABI_SPOKE_AFTER_COLOSSO, SCRIPT_REF(scSuhallaGateGuardsBlockPassage) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_GATE_APPROACH, FLAG_SUHALLA_GATE_ATTACK_SEEN, SCRIPT_REF(scSuhallaGateIodemQuestionsGuards) },
	{ CUE_TALK, ACTOR_WEST_GUARD, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaTravelPapersRequired) },
	{ CUE_TALK, ACTOR_WEST_GUARD, CONDITION_ALWAYS, MSGNO(MsgSuhallaSendHelpFromTower) },
	{ CUE_TALK, ACTOR_EAST_GUARD, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaGateTalkToEastGuard) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaObeyBabiOrders) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, CONDITION_ALWAYS, MSGNO(MsgSuhallaAshamedBeforeIodem) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaSeeColossoNextYear) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgSuhallaSurvivedAttack) },
	{ CUE_TALK, ACTOR_VANISHING_SOLDIER, FLAG_SUHALLA_GATE_SOLDIER_VANISHED, SCRIPT_REF(scSuhallaGateSoldierVanishes) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_VANISHING_SOLDIER, FLAG_SUHALLA_GATE_SOLDIER_VANISHED, SCRIPT_REF(scSuhallaGateSoldierVanishes) },
	{ CUE_TABLE_END },
};
/* clang-format on */

CUE SuhallaGateDewCues[] = {
	{CUE_EXIT, SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_WEST,
		CONDITION_ALWAYS, SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_WEST},
	{CUE_EXIT, SUHALLA_GATE_DEW_WEST_TO_EAST, CONDITION_ALWAYS,
		SUHALLA_GATE_DEW_WEST_TO_EAST},
	{CUE_EXIT, SUHALLA_GATE_DEW_EAST_TO_WEST, CONDITION_ALWAYS,
		SUHALLA_GATE_DEW_EAST_TO_WEST},
	{CUE_EXIT, SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_EAST,
		CONDITION_ALWAYS, SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_EAST},
	{CUE_EXIT, SUHALLA_GATE_DEW_TO_GATEWAY_CAVE, CONDITION_ALWAYS,
		SUHALLA_GATE_DEW_TO_GATEWAY_CAVE},
	{CUE_TALK, ACTOR_DEW, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaGateFightDew)},
	{EXAMINE_CUE(EXAMINE_CHEST) | CUE_FLAG_FACING | FACING_NORTH, 100,
		FLAG_SUHALLA_GATE_MINT, FIND_ITEM(ITEM_MINT)},
	{EXAMINE_CUE(EXAMINE_CHEST) | CUE_FLAG_FACING | FACING_EAST, 100,
		FLAG_SUHALLA_GATE_MINT, FIND_ITEM(ITEM_MINT)},
	{EXAMINE_CUE(EXAMINE_CHEST) | CUE_FLAG_FACING | FACING_WEST, 100,
		FLAG_SUHALLA_GATE_MINT, FIND_ITEM(ITEM_MINT)},
	{CUE_TABLE_END},
};

CUE GatewayCaveCues[] = {
	{CUE_EXIT, GATEWAY_CAVE_TO_SUHALLA_GATE_DEW, CONDITION_ALWAYS,
		GATEWAY_CAVE_TO_SUHALLA_GATE_DEW},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, GATEWAY_CAVE_NORTH_TO_SOUTH,
		CONDITION_ALWAYS, GATEWAY_CAVE_NORTH_TO_SOUTH},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, GATEWAY_CAVE_SOUTH_TO_NORTH,
		CONDITION_ALWAYS, GATEWAY_CAVE_SOUTH_TO_NORTH},
	{CUE_EXIT, GATEWAY_CAVE_TO_WORLD_MAP, CONDITION_ALWAYS,
		GATEWAY_CAVE_TO_WORLD_MAP},
	{EXAMINE_CUE(EXAMINE_SILENT), 100, FLAG_GATEWAY_CAVE_PSYNERGY_STONE,
		FIND_PSYNERGY_STONE},
	{CUE_TABLE_END},
};
