#ifndef ALCHEMY_MODULE_SCENE_MERCURY_LIGHTHOUSE_H
#define ALCHEMY_MODULE_SCENE_MERCURY_LIGHTHOUSE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

void MercuryLighthouse_HearVoices(void);

extern s32 gMercuryLighthouse_MoteScript[];
extern u32 gMercuryLighthouse_MoteFrames;

void BattleEffect_FinishSceneAndReleaseHeapBlock(void);

#endif
