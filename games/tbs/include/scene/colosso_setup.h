#ifndef ALCHEMY_COLOSSO_SETUP_H
#define ALCHEMY_COLOSSO_SETUP_H

/* @file */
/* The race at the end of each Colosso finals match (shared/colosso_setup.c): */
/* the gauge at the top of the screen, and the opponent running a recorded */
/* path to the prize. */

#include "fixed_math.h"
#include "types.h"

/* PATH_RECORDER.mode: what the opponent's path does */
/* (scColossoUpdatePathOpponent). */
typedef enum PATH_RECORDER_MODE {
	PATH_RECORDER_PLAY = 1,	   /* the opponent runs the path */
	PATH_RECORDER_RECORD = 2,  /* the actor's own path is recorded */
	PATH_RECORDER_FINISHED = 9 /* the opponent has reached the finish */
} PATH_RECORDER_MODE;

/* UB: the scenes call this before its definition with no prototype, as a */
/* function returning int. */
#ifdef UBFIX
void scColossoStartRaceGauge(s32 playerActorId, s32 opponentActorId,
							 s32 segments, FIXED finishX, FIXED finishZ,
							 s32 finishItemActorIdA, s32 finishItemActorIdB);
#endif
/* Has the opponent run its recorded path (resource) toward the finish. */
void scColossoStartOpponent(s32 resource);

#endif
