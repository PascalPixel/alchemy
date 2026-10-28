#ifndef ALCHEMY_MODULE_SCENE_SHARED_COLOSSO_SETUP_H
#define ALCHEMY_MODULE_SCENE_SHARED_COLOSSO_SETUP_H

#include "types.h"

/// PathRecorder.mode: what the opponent's path does
/// (Colosso_UpdatePathOpponent).
enum PathRecorderMode {
  PATH_RECORDER_PLAY = 1,    ///< the opponent runs the path
  PATH_RECORDER_RECORD = 2,  ///< the actor's own path is recorded
  PATH_RECORDER_FINISHED = 9 ///< the opponent has reached the finish
};

// Native calls precede this definition; see MATCHING.md.
#ifdef UBFIX
void Colosso_StartRaceGauge(s32 playerActorId, s32 opponentActorId,
                            s32 segments, s32 finishX, s32 finishZ,
                            s32 finishItemActorIdA, s32 finishItemActorIdB);
#endif
void Colosso_StartOpponent(s32 resource);

#endif
