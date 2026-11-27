/* @file */
/* The leader's step against a pushed thing for each of the sixteen facings, */
/* one cell (16 pixels) along the nearest of the four directions, which the */
/* scenes with pushed things keep a copy of, as ☀️'s do */
/* (its shared/pushed_actor_direction_steps.c). */
/* */
/* An includer whose overlay places the table after its pictures defines */
/* PUSH_LEADER_STEPS_SECTION as LATE_DATA first. */

#ifndef PUSH_LEADER_STEPS_SECTION
#define PUSH_LEADER_STEPS_SECTION
#endif

/* One cell along each facing's nearest direction (PUSH_STEP). */
PUSH_LEADER_STEPS_SECTION s32 PushLeaderSteps[] = {
	PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),	 PUSH_STEP(0, 16),
	PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),	 PUSH_STEP(-16, 0),
	PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),
	PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(16, 0),
};
