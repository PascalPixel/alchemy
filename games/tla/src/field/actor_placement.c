/* @file */
/* The idle script, and the placement table the field starts with. */

#include "field/scene_tables.h"
#include "field/actor_script.h"

/* The script of an actor that does nothing. */
const s32 Actor_IdleScript[] = {
	SC_END,
};

/* The leader's placement and the end of its table, which the field copies */
/* into its state as it starts. Only the idle script survives the copy: */
/* fdActorResetForLeader writes the leader's own sprite, condition, spot and */
/* heading over the rest of the first row. */
const ACTOR_PLACE Field_DefaultPlacements[] = {
	ACTOR_PLACEMENT(0, CONDITION_ALWAYS, (s32)Actor_IdleScript, 300, 0, 300,
					23472, 0, 0),
	{SCENE_TABLE_END, 0, 0, 0, 0, 0, 0, 0, 0},
};
