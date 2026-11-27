#ifndef ALCHEMY_SOUND_STUBS_H
#define ALCHEMY_SOUND_STUBS_H

/* @file */
/* The stub calls into the sound module, as the other modules' code calls */
/* them. */

#include "types.h"

/* Returns the music sySndPlay last chose. */
s32 sySndGetCurrentBgmStub(void);
/* Brings the music back after a jingle and steps its volume, tempo and pitch */
/* ramps; called once a VBlank. */
void sySndUpdateBgmRampsStub(void);
/* Starts the sound driver and resets the music's ramps. */
void sySndInitStub(void);

#endif
