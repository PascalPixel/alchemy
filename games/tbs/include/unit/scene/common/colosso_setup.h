#ifndef ALCHEMY_UNIT_MAP_COMMON_COLOSSO_SETUP_H
#define ALCHEMY_UNIT_MAP_COMMON_COLOSSO_SETUP_H

#include "types.h"

/// PathRecorder.mode: what the opponent's path does
/// (Colosso_UpdatePathOpponent).
enum PathRecorderMode {
  PATH_RECORDER_PLAY = 1,    ///< the opponent runs the path
  PATH_RECORDER_RECORD = 2,  ///< the object event's own path is recorded
  PATH_RECORDER_FINISHED = 9 ///< the opponent has reached the finish
};

// Native calls precede this definition; see MATCHING.md.
#ifdef UBFIX
void Colosso_StartRaceGauge(s32 playerLocalId, s32 opponentLocalId,
                            s32 segments, s32 finishX, s32 finishZ,
                            s32 finishItemLocalIdA, s32 finishItemLocalIdB);
#endif
void Colosso_StartOpponent(s32 resource);

#endif
