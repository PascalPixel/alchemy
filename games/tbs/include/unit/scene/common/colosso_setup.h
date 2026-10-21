#ifndef ALCHEMY_UNIT_SCENE_COMMON_COLOSSO_SETUP_H
#define ALCHEMY_UNIT_SCENE_COMMON_COLOSSO_SETUP_H

#include "types.h"

/// PathRecorder.mode: what the rival's path does (Colosso_UpdatePathRival).
enum PathRecorderMode {
  PATH_RECORDER_PLAY = 1,    ///< the rival runs the path
  PATH_RECORDER_RECORD = 2,  ///< the actor's own path is recorded
  PATH_RECORDER_FINISHED = 9 ///< the rival has reached the finish
};

// Native calls precede this definition; see MATCHING.md.
#ifdef UBFIX
void Colosso_StartRaceGauge(s32 playerActor, s32 rivalActor, s32 segments,
                            s32 finishX, s32 finishZ, s32 finishItemActorA,
                            s32 finishItemActorB);
#endif
void Colosso_StartRival(s32 resource);

#endif
