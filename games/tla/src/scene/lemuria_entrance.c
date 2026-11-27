/* The channels among the cliffs where the party's ship comes into Lemuria. In
   the north-east is the harbor of moored Lemurian ships, whose quay is the way
   ashore to the city. In the north-west a brown rock blocks the channel until
   Grind breaks it up; past it the channel runs south to a second quay, by two
   moored boats, the way ashore to another part of the ruins. Sailing out after
   the audience with King Hydros, Sheba asks Piers how old he is. */

#include "field/field_mode.h"
#include "field/field_sprite.h"
#include "field/script_inline.h"
#include "game/game.h"
#include "field/field_mode_script.h"
#include "field/call.h"
#include "scene/scene.h"
#include "window/window.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

extern Sint32 LemuriaEntrance_ShipSailsOut[];

/* The way in's actors, by the rows of ActorPlacements. */
enum {
	/* The party's ship, placed blank; it sails out of the harbor
	   (LemuriaEntrance_ShipSailsOut). */
	ACTOR_SHIP = ACTOR_FIRST_PLACED,
	/* Boats moored in the harbor: four SPRITE_MOORED_BOAT and six on the winged
	   ship's sprite sheet (SPRITE_WINGED_SHIP). */
	ACTOR_MOORED_BOAT_A,
	ACTOR_MOORED_BOAT_B,
	ACTOR_MOORED_BOAT_C,
	ACTOR_MOORED_BOAT_D,
	ACTOR_MOORED_SHIP_A,
	ACTOR_MOORED_SHIP_B,
	ACTOR_MOORED_SHIP_C,
	ACTOR_MOORED_SHIP_D,
	ACTOR_MOORED_SHIP_E,
	ACTOR_MOORED_SHIP_F,
	/* the brown rock in the north-west channel, which Grind breaks up */
	ACTOR_ROCK
};

/* Who speaks a line of scLemuriaEntranceShebaAsksPiersAge: the party by their
   numbers, and Kraden as 8, who shows his own portrait. */
enum { SPEAKER_KRADEN = 8 };

/* Unused: no exit leads here; it hands the ship to the player and enters again
   by entrance 1. */
enum { ENTRANCE_ABOARD_SHIP = 90 };

void scLemuriaEntranceShowLineWithPortrait(Sint32 speaker, Sint32 frames);

void scLemuriaEntranceShebaAsksPiersAge(void);

extern Sint32 LemuriaEntrance_ShipAtRest[];

/* Grind has broken up the brown rock in the north-west channel. */
#define FLAG_ROCK_GROUND_AWAY 0x909

/* The channel past the rock runs on in the map's south half: its north end
   (exit and entrance 2) leads to its south end (exit and entrance 4), and
   back. */
enum { CHANNEL_NORTH_END = 2, CHANNEL_SOUTH_END = 4 };

/* Starts an actor's animation from its first frame. */
Sint32 scLemuriaEntranceResetAnimationFrame(ACTOR *actor)
{
	((FIELD_SPR *)actor->spr)->flg = 0;
	return 1;
}

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

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

/* The ship sails up to the quay in the harbor of moored Lemurian ships, in the
   north-east, and the party goes ashore on Lemuria's dock, the way to the city.
   Clearing FLAG_LEMURIA_OTHER_WAY_IN makes the dock's ways lead back here and
   on towards the city. */
void scLemuriaEntranceLandAtHarbor(Sint32 exit)
{
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	party->ownerOverride = 0;
	fdScriptClearFlag(FLAG_LEMURIA_OTHER_WAY_IN);
	fdScriptPlaySnd(SE_MAP_EXIT);
	party->leaderActorId = ACTOR_SHIP;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	party->leaderActorId = ACTOR_FELIX;
	syFieldScriptSetExitRequest(exit);
}

/* The ship sails up to the quay by the two moored boats, at the south end of
   the channel past the rock Grind breaks up, and the party goes ashore on
   Lemuria's dock. Setting FLAG_LEMURIA_OTHER_WAY_IN makes the dock's ways lead
   back to this quay and on to the other part of the ruins. */
void scLemuriaEntranceLandAtSouthQuay(Sint32 exit)
{
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	party->ownerOverride = 0;
	fdScriptSetFlag(FLAG_LEMURIA_OTHER_WAY_IN);
	fdScriptPlaySnd(SE_MAP_EXIT);
	party->leaderActorId = ACTOR_SHIP;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	party->leaderActorId = ACTOR_FELIX;
	syFieldScriptSetExitRequest(exit);
}

/* Felix casts Grind on the brown rock in the north-west channel: it breaks up
   and is gone for good, opening the way south to the second quay. */
void scLemuriaEntranceGrindRock(void)
{
	fdScriptSetFlag(FLAG_ROCK_GROUND_AWAY);
	fdActorSetPos(ACTOR_ROCK, 0, 0);
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Unused: once FLAG_TRANSFERRED_DJINN_FIRST is set, the party boards the ship
   and takes the helm; otherwise Felix walks on as before. Nothing calls it. */
UNUSED void scLemuriaEntranceBoardShip(void)
{
	if(fdScriptGetFlag(FLAG_TRANSFERRED_DJINN_FIRST) != 0) {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionFrames();
		{
			MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;

			state->movementMode = LEADER_MOVEMENT_SHIP_ON_MAP;
			state->ownerOverride = 1;
		}
		fdActorSetAnimation(ACTOR_SHIP, 7);
		fdScriptEnd();
	} else {
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		party->leaderActorId = ACTOR_FELIX;
		party->movementMode = LEADER_MOVEMENT_WALK;
		party->ownerOverride = 0;
	}
}

/* The scene's setup: places its actors, then plays what the entrance it was
   reached by calls for. */
Sint32 scInit(void)
{
	Sint32 *screen = &FieldStateWork->transition;
	Sint32	effect = SCENE_TRANSITION(TRANSITION_WINDOW, 4);

	*screen = effect;
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdActorSetSprFlags(fdActorGet(ACTOR_ROCK), 0);
	fdScriptSetFlag(FLAG_LEMURIA_REACHED);
	fdScriptClearFlag(FLAG_SHIP_AT_ATTEKA_INLET);
	if(fdScriptGetFlag(FLAG_ROCK_GROUND_AWAY))
		fdActorSetPos(ACTOR_ROCK, 0, 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_MOORED_BOAT_A), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_MOORED_BOAT_B), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_MOORED_BOAT_C), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_MOORED_BOAT_D), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_MOORED_SHIP_A), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_MOORED_SHIP_B), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_MOORED_SHIP_C), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_MOORED_SHIP_D), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_MOORED_SHIP_E), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_MOORED_SHIP_F), 0);
	if(MainRoutineStateWork.entrance == ENTRANCE_ABOARD_SHIP) {
		MainRoutineStateWork.ownerOverride = 1;
		fdScriptSetDestination(SCENE_LEMURIA_ENTRANCE, 1);
	}
	if(MainRoutineStateWork.entrance ==
			LEMURIA_ENTRANCE_FROM_LEMURIA_DOCK_NORTH ||
		MainRoutineStateWork.entrance ==
			LEMURIA_ENTRANCE_FROM_LEMURIA_DOCK_SOUTH) {
		MainRoutineStateWork.leaderActorId = ACTOR_SHIP;
		FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionFrames();
		MainRoutineStateWork.leaderActorId = ACTOR_FELIX;
		FieldStateWork->transition = effect;
	}
	if(MainRoutineStateWork.entrance ==
			LEMURIA_ENTRANCE_FROM_LEMURIA_DOCK_NORTH &&
		fdScriptGetFlag(FLAG_SHEBA_TO_ASK_PIERS_AGE)) {
		fdScriptClearFlag(FLAG_SHEBA_TO_ASK_PIERS_AGE);
		scLemuriaEntranceShebaAsksPiersAge();
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* Shows the next line of a conversation beside its speaker's portrait (Kraden's
   for SPEAKER_KRADEN), holds it the given frames once typed, and closes it.
   Piers's lines stand further right with his portrait at their right-hand end;
   everyone else's portrait is at the left. */
void scLemuriaEntranceShowLineWithPortrait(Sint32 speaker, Sint32 frames)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Sint32		 height;
	Sint32		 width;
	Sint32		 top;
	Sint32		 left;
	Sint32		 voice;
	Sint32		 character;
	Sint32		 portrait; /* the portrait's left edge, then its window */

	left = 0;
	top = 0;
	character = SPRITE_KRADEN;
	if(speaker != SPEAKER_KRADEN)
		character = speaker;
	voice = fdSpeakerGetVoice(character);
	if(speaker == ACTOR_PIERS) {
		left = 10;
		top = 10;
		mnTextGetMessageSize(state->msg, &left, &top, &width, &height);
		portrait = left + width - 5;
	} else {
		left = 5;
		top = 10;
		portrait = 5;
	}
	Call4((void (*)())mnMessageOpenWindow, state->msg++, left, top,
		MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_UNREAD);
	portrait = mnPortraitOpenWindow(character, 0, portrait, 5);
	while(mnMessageAllTyped() == 0)
		syWait(1);
	syFieldScriptWait(frames);
	mnWindowClose(portrait, 2);
	mnMessageCloseFinishedWindows();
}

/* Coming back aboard after the audience with King Hydros, the ship sails out of
   the harbor and through the channels while Sheba asks Piers how old he really
   is, "Umm... Piers?", and Piers will not say. Then the ship is out on the
   sea. */
void scLemuriaEntranceShebaAsksPiersAge(void)
{
	FIELD_STATE *state;
	Uint8		 saved;

	saved = MainRoutineStateWork.msgSpeed;
	MainRoutineStateWork.msgSpeed = 2;
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetSpeed(ACTOR_SHIP, 0.27);
	fdActorSetScript(ACTOR_SHIP, LemuriaEntrance_ShipSailsOut);
	syFieldScriptWait(80);
	syFieldScriptSetMessageId((Sint32)&MsgFieldAddressCompanion);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_SHEBA, 90);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_PIERS, 90);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_SHEBA, 180);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_PIERS, 90);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_SHEBA, 250);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_PIERS, 180);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_JENNA, 250);
	scLemuriaEntranceShowLineWithPortrait(SPEAKER_KRADEN, 90);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_PIERS, 180);
	scLemuriaEntranceShowLineWithPortrait(SPEAKER_KRADEN, 180);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_PIERS, 180);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_SHEBA, 90);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_PIERS, 250);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_JENNA, 250);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_PIERS, 250);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_SHEBA, 90);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_JENNA, 90);
	scLemuriaEntranceShowLineWithPortrait(SPEAKER_KRADEN, 90);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_PIERS, 250);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_FELIX, 90);
	scLemuriaEntranceShowLineWithPortrait(ACTOR_PIERS, 250);
	scFieldActorWaitForScriptEndById(ACTOR_SHIP);
	syFieldScriptWait(10);
	state = Ram_HeapSlots->fieldState;
	state->transitionFrm = 30;
	state->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	syFieldScriptSetExitRequest(LEMURIA_ENTRANCE_TO_WORLD_MAP);
	MainRoutineStateWork.msgSpeed = saved;
	fdScriptEnd();
}

/* clang-format off */
/* The ship's course out of the harbor. */
Sint32 LemuriaEntrance_ShipSailsOut[] = {
	SC_MOVE(728, 0, 160),
	SC_SYNC,
	SC_MOVE(608, 0, 152),
	SC_SYNC,
	SC_MOVE(488, 0, 152),
	SC_SYNC,
	SC_MOVE(368, 0, 152),
	SC_SYNC,
	SC_MOVE(368, 0, 112),
	SC_SYNC,
	SC_MOVE(336, 0, 88),
	SC_SYNC,
	SC_MOVE(320, 0, 88),
	SC_SYNC,
	SC_MOVE(288, 0, 120),
	SC_SYNC,
	SC_MOVE(288, 0, 176),
	SC_SYNC,
	SC_MOVE(272, 0, 192),
	SC_SYNC,
	SC_MOVE(272, 0, 280),
	SC_SYNC,
	SC_MOVE(344, 0, 280),
	SC_SYNC,
	SC_MOVE(384, 0, 320),
	SC_SYNC,
	SC_MOVE(384, 0, 408),
	SC_SYNC,
	SC_MOVE(416, 0, 440),
	SC_SYNC,
	SC_MOVE(416, 0, 552),
	SC_SYNC,
	SC_END,
};
/* clang-format on */

/* The ship's animation as it rests in the harbor. */
Sint32 LemuriaEntrance_ShipAtRest[] = {
	SC_CALL(scLemuriaEntranceResetAnimationFrame),
	SC_ANIMATION(7),
	SC_END,
};

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(LemuriaEntrance),
	SCENE_EXIT_UNLESS(LEMURIA_ENTRANCE_TO_WORLD_MAP, WorldMap,
		WORLD_MAP_FROM_LEMURIA_ENTRANCE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(
		CHANNEL_NORTH_END, LemuriaEntrance, CHANNEL_SOUTH_END, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIA_ENTRANCE_TO_LEMURIA_DOCK_NORTH, LemuriaDock,
		LEMURIA_DOCK_FROM_LEMURIA_ENTRANCE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(
		CHANNEL_SOUTH_END, LemuriaEntrance, CHANNEL_NORTH_END, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIA_ENTRANCE_TO_LEMURIA_DOCK_SOUTH, LemuriaDock,
		LEMURIA_DOCK_FROM_LEMURIA_ENTRANCE, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS,
		(Sint32)LemuriaEntrance_ShipAtRest, 14, 0, 4, FACING_NORTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MOORED_BOAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		636, 0, 112, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MOORED_BOAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		668, 0, 112, FACING_EAST, TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_MOORED_BOAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		668, 0, 216, FACING_EAST, TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_MOORED_BOAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		732, 0, 216, FACING_WEST, TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_WINGED_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		736, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WINGED_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		640, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_WINGED_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		704, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_WINGED_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		52, 0, 796, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WINGED_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		52, 0, 828, FACING_WEST, TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_WINGED_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		52, 0, 860, FACING_WEST, TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144,
		0, 160, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_EXIT, LEMURIA_ENTRANCE_TO_WORLD_MAP, CONDITION_ALWAYS,
		LEMURIA_ENTRANCE_TO_WORLD_MAP},
	{CUE_EXIT, CHANNEL_NORTH_END, CONDITION_ALWAYS, CHANNEL_NORTH_END},
	{CUE_EXIT, CHANNEL_SOUTH_END, CONDITION_ALWAYS, CHANNEL_SOUTH_END},
	{CUE_STEP, LEMURIA_ENTRANCE_TO_LEMURIA_DOCK_NORTH, CONDITION_ALWAYS,
		SCRIPT_REF(scLemuriaEntranceLandAtHarbor)},
	{CUE_STEP, LEMURIA_ENTRANCE_TO_LEMURIA_DOCK_SOUTH, CONDITION_ALWAYS,
		SCRIPT_REF(scLemuriaEntranceLandAtSouthQuay)},
	{PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK, CONDITION_ALWAYS,
		SCRIPT_REF(scLemuriaEntranceGrindRock)},
	{CUE_TABLE_END},
};
