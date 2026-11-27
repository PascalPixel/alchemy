/* The party leader's actor placement, which fdActorResetForLeader copies before
   a scene places its own actors, and the script it gives the leader. */

#include "field_mode_internal.h"

/* Unused: the camera following the leader, a tick at a time, forever; no code
   names it. */
const Sint32 ActorUnusedCameraScript[] = {
	SC_CAMERA,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

/* A lone SC_END: the leader's script, which the player moves. */
const Sint32 FieldLeaderScript[] = {SC_END};

/* The leader's row and the end of the table; fdActorResetForLeader puts the
   leader's sprite, spot and heading over the row's. */
const ACTOR_PLACE FieldDefaultActorPlacements[2] = {
	ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, (Sint32)FieldLeaderScript,
		300, 0, 300, 23472, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};
