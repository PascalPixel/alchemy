/* Goma Cave Entrance (SceneId_GomaCaveEntrance), the foot of the cliffs in the
   Goma Range between Vault and Bilibin, where Ivan joins the party. The cave's
   mouth is high on a column of rock beside a waterfall, hidden behind tall ivy,
   with a stump caught in ivy in front of it.

   The party comes in from the World Map at the bottom of the map, crosses the
   river on a stump sunk in it (ACTOR_SUNKEN_STUMP) and climbs the cliffs. On
   the way a stone pillar stands by a ladder at the foot of a cliff: walked into
   or moved with Move, it slides a step at a time, blocking the ladder while it
   stands in front of it (FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP and
   _TWO_STEPS). Higher up, Move pulls the second pillar off its ledge into the
   gap beside it, where its top makes a bridge
   (FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE).

   On the column by the cave, Move on the ivy-bound stump makes Garet say their
   Psynergy can't free it. Once the thieves in Vault are jailed
   (FLAG_VAULT_THIEVES_JAILED), Ivan comes up instead, blows the ivy off with
   Whirlwind and joins the party with the Shaman's Rod
   (FLAG_GOMA_CAVE_IVAN_JOINED). Move then sends the stump leaping off the
   column into the pool under the waterfall, where it sinks until only its top
   shows and can be jumped across (FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED).
   Whirlwind on the tall ivy blows it away for good and opens the cave's mouth
   (FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY).

   Retreat does not work here. GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM leads into
   Goma Cave (SceneId_GomaCaveStream) and GOMA_CAVE_ENTRANCE_TO_WORLD_MAP out to
   the World Map. */

#include "ability_ids.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script.h"
#include "field/field_psynergy_state.h"
#include "field/map_grid.h"
#include "graphics/graphics.h"
#include "scene/goma_cave.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE GomaCaveEntranceEntrances[];
extern Uint32		  GomaCaveEntranceExits[];
extern ACTOR_PLACE	  GomaCaveEntranceActorPlacements[];
extern CUE			  GomaCaveEntranceCues[];

/* The room's actors.

   Garet's and Ivan's rows go to their party slots, and the rows after them
   number from ACTOR_FIRST_PLACED. */
typedef enum GOMA_CAVE_ENTRANCE_ACTOR_ID {
	/* The stump caught in ivy in front of the cave's mouth. */
	ACTOR_STUMP = ACTOR_FIRST_PLACED,
	/* The pillar by the ladder at the foot of a cliff. */
	ACTOR_WEST_PILLAR,
	/* The pillar on the ledge that Move pulls into the gap beside it. */
	ACTOR_EAST_PILLAR,
	/* A second stump, sunk in the river until only its top shows, that the
	   party crosses on. */
	ACTOR_SUNKEN_STUMP,
	/* Unused: two Move hands and a third stump, placed at the map's origin
	   outside the camera's bounds.

	   Nothing in the overlay addresses them, and Move spawns its own hand
	   (fdEffectSpawnMoveHand), so the player never sees them. */
	ACTOR_UNUSED_FIRST_MOVE_HAND,
	ACTOR_UNUSED_SECOND_MOVE_HAND,
	ACTOR_UNUSED_STUMP,
	/* The ivy holding the stump, until Ivan blows it away. */
	ACTOR_STUMP_IVY,
	/* The tall ivy over the cave's mouth. */
	ACTOR_IVY
} GOMA_CAVE_ENTRANCE_ACTOR_ID;

static void scGomaCaveEntranceWhirlwindOnStumpIvy(void);
static void scLeapStump(Sint32 stumpId);
static void scGomaCaveEntranceGiveShamansRod(void);

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/* Shows the next message over the speaker, then pauses. */
static void scGomaCaveEntranceSayThenWait(Sint32 actorId, Sint32 frames)
{
	fdScriptShowMessage(actorId, 0);
	fdScriptWait(frames);
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return GomaCaveEntranceEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return GomaCaveEntranceExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return GomaCaveEntranceActorPlacements;
}

/* Isaac casts Move on the pillar by the ladder at the foot of the cliff, and
   each push of it ends here: in front of the ladder, metatile (16, 18), it
   blocks the climb, and to either side of it the ladder climbs the cliff again.

   The ladder's two metatiles are copied from a spare stretch of the map:
   climbing metatiles, or flat ground while the pillar stands there. The flags
   keep the pillar's place for the next visit. */
static void scGomaCaveEntranceMoveWestPillar(void)
{
	Sint32 column;

	column = fdActorGet(ACTOR_WEST_PILLAR)->x.fixed / toFIXED(16);
	fdScriptClearFlag(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP);
	fdScriptClearFlag(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS);
	if(column == 15) {
		fdMapGridCopyMetatileAttributes(47, 18, 1, 2, 16, 18);
	} else if(column == 16) {
		fdMapGridCopyMetatileAttributes(48, 18, 1, 2, column, 18);
		gmFlagSet(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP);
	} else {
		fdMapGridCopyMetatileAttributes(47, 18, 1, 2, 16, 18);
		gmFlagSet(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS);
	}
}

/* Isaac casts Move on the pillar on the ledge across the gap: pulled west into
   the gap, metatile (23, 17), it drops in until its top is level with the
   ground, and Isaac can walk across it. */
static void scGomaCaveEntranceMoveEastPillar(void)
{
	Sint32 column;

	column = fdActorGet(ACTOR_EAST_PILLAR)->x.fixed / toFIXED(16);
	if(column == 23) {
		fdScriptWait(10);
		fdActorGet(ACTOR_EAST_PILLAR)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		fdActorGet(ACTOR_EAST_PILLAR)->motionFlg = 0;
		fdActorSetSprFlags(fdActorGet(ACTOR_EAST_PILLAR), 0);
		fdMapGridCopyMetatileAttributes(54, 17, 1, 1, column, 17);
		gmFlagSet(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE);
	}
}

/* The metatiles beside the pillar by the ladder from which walking into it
   pushes it east, from its west side, and west, from its east side. */
enum { CUE_PUSH_WEST_PILLAR_EAST = 5, CUE_PUSH_WEST_PILLAR_WEST = 6 };

/* Isaac walks into the pillar by the ladder from the west or the east, leans on
   it and pushes it one step. */
static void scGomaCaveEntrancePushWestPillar(void)
{
	FIELD_STATE *state;
	Sint32		 offsetX;

	state = FieldStateWork;
	fdScriptStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
	fdActorSetSpeed(ACTOR_WEST_PILLAR, 0.2);
	sySndPlay(SE_PUSH);
	/* A metatile east from CUE_PUSH_WEST_PILLAR_EAST, west from
	   CUE_PUSH_WEST_PILLAR_WEST. */
	offsetX = CUE_PUSH_WEST_PILLAR_EAST + CUE_PUSH_WEST_PILLAR_WEST -
		state->stepCue * 2;
	offsetX *= MAPGRID_METATILE_PIXELS;
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, offsetX, 0);
	fdActorSetDestinationOffset(ACTOR_WEST_PILLAR, offsetX, 0);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorWaitForMoveById(ACTOR_WEST_PILLAR);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	scGomaCaveEntranceMoveWestPillar();
	fdPlayQueuedMusic();
	fdScriptEnd();
}

UNUSED static void scGomaCaveEntranceNoOperationA(void)
{
}

UNUSED static void scGomaCaveEntranceNoOperationB(void)
{
}

/* Isaac casts Whirlwind on the tall ivy over the cave's mouth: it is blown away
   for good, and the way into the cave is open. */
static void scGomaCaveEntranceBlowAwayIvy(void)
{
	fdScriptStart();
	gmFlagSet(FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY);
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	return GomaCaveEntranceCues;
}

/* Sets up Goma Cave Entrance as the party arrives: Retreat is barred, the
   waterfall's sound plays, and the stump, the pillars and the metatiles that go
   with them are where the party left them. */
Sint32 scInit(void)
{
	gmFlagSet(FLAG_RETREAT_BLOCKED);
	fdScriptWait(10);
	fdQueueMusic(SE_AMBIENT_WATER);
	fdActorSetAnimation(ACTOR_SUNKEN_STUMP, STUMP_ANIMATION_SINK);
	fdActorGet(ACTOR_SUNKEN_STUMP)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	{
		ACTOR *stump = fdActorGet(ACTOR_STUMP);

		stump->collisionFlg = ACTOR_NEAR_PSYNERGY | stump->collisionFlg;
	}
	/* The player never targets the stump's ivy; only Ivan's Whirlwind in
	   scGomaCaveEntranceIvanJoins removes it. */
	{
		ACTOR *ivy = fdActorGet(ACTOR_STUMP_IVY);

		ivy->collisionFlg = ACTOR_NOT_TARGETED | ivy->collisionFlg;
	}
	/* The metatile scGomaCaveEntranceIvanJoins copies once the stump's ivy is
	   gone, copied again on each arrival. */
	if(gmFlagGet(FLAG_GOMA_CAVE_IVAN_JOINED) != 0) {
		fdMapGridCopyMetatileAttributes(74, 11, 1, 1, 73, 11);
	}
	if(gmFlagGet(FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED) != 0) {
		fdActorSetPos(ACTOR_STUMP, 136, 196);
		fdActorGet(ACTOR_STUMP)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdActorSetAnimation(ACTOR_STUMP, STUMP_ANIMATION_SINK);
		fdMapGridCopyMetatileAttributes(39, 12, 3, 1, 8, 12);
		fdMapGridCopyMetatileAttributes(43, 11, 3, 1, 12, 11);
	}
	/* One step along, the west pillar stands on column 16, and the two
	   metatiles there are plain ground where they are otherwise a climb; two
	   steps along it has cleared them and the climb is back. */
	if(gmFlagGet(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP) != 0) {
		fdActorSetPos(ACTOR_WEST_PILLAR, 264, 312);
		fdMapGridCopyMetatileAttributes(48, 18, 1, 2, 16, 18);
	} else {
		/* The metatiles scGomaCaveEntranceMoveWestPillar copies as it sets the
		   flag, copied again on each arrival. */
		if(gmFlagGet(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS) != 0) {
			fdActorSetPos(ACTOR_WEST_PILLAR, 280, 312);
			fdMapGridCopyMetatileAttributes(47, 18, 1, 2, 16, 18);
		}
	}
	if(gmFlagGet(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE) != 0) {
		fdActorSetPos(ACTOR_EAST_PILLAR, 376, 280);
		fdActorGet(ACTOR_EAST_PILLAR)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		fdActorGet(ACTOR_EAST_PILLAR)->motionFlg = 0;
		fdActorSetSprFlags(fdActorGet(ACTOR_EAST_PILLAR), 0);
		fdMapGridCopyMetatileAttributes(54, 17, 1, 1, 23, 17);
	}
	return 0;
}

/* Isaac casts Move on the ivy-bound stump in front of the cave's mouth. Garet
   shakes his head: "It's no use. Our Psynergy isn't strong enough to free that
   stump." Once the thieves in Vault are jailed, Ivan comes up from the east
   instead, blows the ivy off with Whirlwind and joins the party. */
static void scGomaCaveEntranceIvanJoins(void)
{
	ACTOR *leader;

#if GOMA_STUMP_WATERFALL_RESUMES_IN_EVENT
	fdPlayQueuedMusic();
#endif
	fdScriptStart();
	fdScriptWait(30);
	fdScriptSetMessageId(MsgGomaNoUsePsynergy);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
	}
	fdActorWalkToAndWait(ACTOR_GARET, 264, 168);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "It's no use. Our Psynergy isn't strong enough to free that stump." */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	if(gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0) {
		/* Garet falls back into line. */
		fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if(leader != NULL) {
			fdActorSetDestination(ACTOR_GARET, leader->x.part[0].integer,
				leader->z.part[0].integer);
		}
		fdActorWaitForMoveById(ACTOR_GARET);
		fdActorSetPos(ACTOR_GARET, 0, 0);
		fdScriptEnd();
	} else {
		/* Ivan walks up from the east. */
		fdActorSetPos(ACTOR_IVAN, 360, 248);
		fdActorSetSpeed(ACTOR_IVAN, 0.8);
		fdActorWalkToAndWait(ACTOR_IVAN, 272, 248);
		fdActorWalkToAndWait(ACTOR_IVAN, 272, 208);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
		fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
		fdScriptWait(20);
		/* "You want to move the stump, right?" */
		fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
		fdScriptWait(20);
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdActorWalkToAndWait(ACTOR_IVAN, 264, 200);
		fdActorWalkTo(ACTOR_PARTY_LEADER, 248, 168);
		fdActorWalkToAndWait(ACTOR_IVAN, 248, 184);
		fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
		fdActorWalkToAndWait(ACTOR_IVAN, 232, 184);
		fdScriptWait(20);
		fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "It looks like it's trapped in the ivy." */
		fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdScriptWait(20);
		fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 20);
		/* "Isn't there anything we can do?" */
		fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 120);
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
		fdActorTurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
		fdScriptWait(60);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
		fdScriptWait(60);
		fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
		fdActorStartRepeatedMotion(ACTOR_IVAN, 1);
		fdScriptWait(30);
		/* "I know!" */
		fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 30);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
		fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdScriptWait(20);
		/* "I have a great idea. Leave it to me!" */
		fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdScriptWait(20);
		fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 20);
		/* He blows the ivy off the stump with Whirlwind. */
		scGomaCaveEntranceWhirlwindOnStumpIvy();
		fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
		fdScriptWait(20);
#if GOMA_STUMP_WATERFALL_RESUMES_IN_EVENT
		fdPlayQueuedMusic();
#endif
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
		fdActorJump(ACTOR_GARET, 2, 0);
		fdScriptWait(20);
		/* "All right, Ivan!" */
		scGomaCaveEntranceSayThenWait(ACTOR_GARET, 20);
		fdActorWalkToAndWait(ACTOR_IVAN, 264, 184);
		fdScriptWait(10);
		fdActorFaceActor(ACTOR_IVAN, ACTOR_GARET, 0);
		fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdScriptWait(20);
		/* "I did it!" */
		scGomaCaveEntranceSayThenWait(ACTOR_IVAN, 60);
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		fdScriptWait(60);
		fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdScriptWait(60);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
		fdScriptWait(10);
		/* "...Thanks for the help. But why are you here, Ivan?" */
		scGomaCaveEntranceSayThenWait(ACTOR_GARET, 20);
		fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
		fdScriptWait(60);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "I couldn't get into Lunpa..." */
		scGomaCaveEntranceSayThenWait(ACTOR_IVAN, 20);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
		fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		fdScriptWait(60);
		fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
		fdScriptWait(20);
		/* "Don't get me wrong! I didn't come here to ask for your help." */
		scGomaCaveEntranceSayThenWait(ACTOR_IVAN, 30);
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		fdScriptWait(80);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdScriptWait(20);
		/* "Your quest has been on my mind ever since I left Vault." */
		scGomaCaveEntranceSayThenWait(ACTOR_IVAN, 20);
		fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
		fdActorStartRepeatedMotion(ACTOR_GARET, 1);
		fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorSetAttachedEffect(
			ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(60);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "Remember? I read everything that happened in your minds." */
		scGomaCaveEntranceSayThenWait(ACTOR_IVAN, 20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
		fdScriptWait(80);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
		fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
		fdScriptWait(30);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdScriptWait(20);
		/* "I couldn't just leave, not with all these terrible things
		   happening." */
		scGomaCaveEntranceSayThenWait(ACTOR_IVAN, 30);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "If I can't rescue Master Hammet, then I want to help you..." */
		scGomaCaveEntranceSayThenWait(ACTOR_IVAN, 20);
		fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdScriptWait(20);
		/* "Please, allow me to join your quest." */
		scGomaCaveEntranceSayThenWait(ACTOR_IVAN, 40);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdScriptWait(20);
		/* Ivan joins the party, and his Shaman's Rod with him. */
		fdScriptAddPartyMember(IVAN, 1);
		fdScriptWait(60);
		scGomaCaveEntranceGiveShamansRod();
		fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
		fdScriptWait(20);
		fdActorWalkToAndWait(ACTOR_IVAN, 248, 184);
		fdScriptWait(20);
		/* "Are you trying to get to Bilibin through this cave?" */
		scGomaCaveEntranceSayThenWait(ACTOR_IVAN, 20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
		fdScriptWait(120);
		/* "Then let's go!" */
		scGomaCaveEntranceSayThenWait(ACTOR_IVAN, 30);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
		fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
		fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
		fdScriptWait(20);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdScriptWait(50);
		fdActorSetSpeed(ACTOR_GARET, 0.8);
		fdActorSetSpeed(ACTOR_IVAN, 0.8);
		fdActorWalkTo(ACTOR_GARET, 248, 168);
		fdActorWalkToAndWait(ACTOR_IVAN, 248, 168);
		fdActorSetPos(ACTOR_IVAN, 0, 0);
		fdActorWaitForMoveById(ACTOR_GARET);
		fdActorSetPos(ACTOR_GARET, 0, 0);
		fdMapGridCopyMetatileAttributes(74, 11, 1, 1, 73, 11);
		gmFlagSet(FLAG_GOMA_CAVE_IVAN_JOINED);
		fdScriptEnd();
	}
}

/* Isaac casts Move on the freed stump in front of the cave's mouth: pushed west
   to the column's edge, it tips, leaps spinning into the pool under the
   waterfall with a crash and a splash, and sinks until only its top shows
   (FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED).

   The metatiles by the pool change so that Isaac can jump across on it. */
static void scGomaCaveEntranceMoveStump(void)
{
	Sint32 column;

	fdScriptStart();
	column = FIELD_METATILE(fdActorGet(ACTOR_STUMP)->x.fixed);
	if(column == 11) {
		scLeapStump(ACTOR_STUMP);
		fdActorGet(ACTOR_STUMP)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdMapGridCopyMetatileAttributes(39, 12, 3, 1, 8, 12);
		fdMapGridCopyMetatileAttributes(43, 11, 3, 1, 12, 11);
		gmFlagSet(FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED);
	}
	fdScriptEnd();
}

/* Ivan casts Whirlwind on the stump's ivy, and the script waits for it. */
static void scGomaCaveEntranceWhirlwindOnStumpIvy(void)
{
	FIELD_EFFECT_STATE *effectState;
	Uint8				flags;

	effectState = FieldEffectStateWork;
	fdPsynergyLoadResources(ABILITY_WHIRLWIND, 1);
	fdPsynergySetupActorPair(ACTOR_IVAN, ACTOR_STUMP_IVY);
	flags = (Uint8)effectState->msgMode | FIELD_MESSAGE_LOWER;
	effectState->msgMode = flags;
	fdPsynergyStartCast();
	fdPsynergyRunHandler(FIELD_PSYNERGY_HANDLER_ON_TARGET);
	fdPsynergyStopCasterUpdate();
}

/* Where the stump lands from the ledge and where it bounces on to and sinks, in
   pixels, and the ground's height there. */
#define STUMP_LANDING_X	   160
#define STUMP_LANDING_Z	   192
#define STUMP_RESTING_X	   139
#define STUMP_RESTING_Z	   196
#define STUMP_FLOOR_HEIGHT toFIXED(32)
#include "shared/stump_leap.c"

#include "shared/item_equip.c"

/* The tries at making room for the Shaman's Rod before a slot is cleared by
   force. */
#define SHAMANS_ROD_TRIES 1000

/* Gives Ivan the Shaman's Rod and equips it.

   While his pack is full, a weapon of his is thrown away to make room, or
   failing that a consumable item; after SHAMANS_ROD_TRIES tries a slot is
   emptied by force. */
static void scGomaCaveEntranceGiveShamansRod(void)
{
	UNIT			*member;
	ITEM_DEFINITION *item;
	Sint32			 tries;
	Sint32			 i;
	Sint32			 id = ITEM_SHAMANS_ROD;
	member = gmUnitGet(IVAN);
	tries = 0;
retry:
	if(++tries > SHAMANS_ROD_TRIES) {
#if SHAMANS_ROD_SPARES_INDISPENSABLE_ITEMS
		for(i = 0; i <= INVENTORY_SLOTS - 1; i++) {
			item = gmItemGetDirect(member->inventory[i]);
			if(item->flg != ITEM_INDISPENSABLE) {
				gmInventoryDiscard(IVAN, i);
				break;
			}
		}
		if(i == INVENTORY_SLOTS)
#endif
			member->inventory[INVENTORY_SLOTS - 1] = 0;
	}
	if(gmInventoryAddItem(IVAN, id) == -1) {
		for(i = 0; i <= INVENTORY_SLOTS - 1; i++) {
			item = gmItemGetDirect(member->inventory[i]);
			if(item->type == ITEM_TYPE_WEAPON) {
				gmInventoryDiscard(IVAN, i);
				goto retry;
			}
		}
		for(i = 0; i <= INVENTORY_SLOTS - 1; i++) {
			item = gmItemGetDirect(member->inventory[i]);
			if(item->type == ITEM_TYPE_NONE &&
				(item->flg & ITEM_INDISPENSABLE) == 0 &&
				item->useType == ITEM_USE_CONSUMED) {
				gmInventoryDiscard(IVAN, i);
				goto retry;
			}
		}
		goto retry;
	}
	for(i = 0; i <= INVENTORY_SLOTS - 1; i++) {
		if(member->inventory[i] == id)
			gmInventoryEquip(IVAN, i);
	}
}

#include "shared/field_effect_duration_scripts.c"

const Sint32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

/* Stretches the tall ivy over the cave's mouth to 1.6 times its height. */
Sint32 GomaCaveEntranceTallIvyScript[] = {
	SC_SET(SW_SCALE_Y, toFIXED(8) / 5),
	SC_END,
};

SCENE_ENTRANCE GomaCaveEntranceEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 192, 0, 197, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{GOMA_CAVE_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 96, 0, 440,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{GOMA_CAVE_ENTRANCE_FROM_GOMA_CAVE_STREAM, CONDITION_ALWAYS, 200, 0, 184,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 GomaCaveEntranceExits[] = {
	SCENE_EXITS(GomaCaveEntrance),
	SCENE_EXIT(GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM, GomaCaveStream,
		GOMA_CAVE_STREAM_FROM_GOMA_CAVE_ENTRANCE),
	SCENE_EXIT(GOMA_CAVE_ENTRANCE_TO_WORLD_MAP, WorldMap,
		WORLD_MAP_FROM_GOMA_CAVE_ENTRANCE),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE GomaCaveEntranceActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		192, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		248, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		392, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		136, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MOVE_HAND, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MOVE_HAND, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, FLAG_GOMA_CAVE_IVAN_JOINED, MOVEMENT_TYPE_RESET_ACTION,
		191, 0, 194, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY, (Sint32)GomaCaveEntranceTallIvyScript,
		200, 0, 168, FACING_NORTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

CUE GomaCaveEntranceCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, GOMA_CAVE_ENTRANCE_TO_WORLD_MAP },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), CUE_PUSH_WEST_PILLAR_EAST, CONDITION_ALWAYS, SCRIPT_REF(scGomaCaveEntrancePushWestPillar) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), CUE_PUSH_WEST_PILLAR_WEST, CONDITION_ALWAYS, SCRIPT_REF(scGomaCaveEntrancePushWestPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GOMA_CAVE_IVAN_JOINED, SCRIPT_REF(scGomaCaveEntranceIvanJoins) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED, SCRIPT_REF(scGomaCaveEntranceMoveStump) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scGomaCaveEntranceMoveWestPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR, FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE, SCRIPT_REF(scGomaCaveEntranceMoveEastPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY, SCRIPT_REF(scGomaCaveEntranceBlowAwayIvy) },
	{ CUE_TABLE_END },
};
/* clang-format on */
