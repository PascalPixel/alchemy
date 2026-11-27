#ifndef ALCHEMY_SOUND_STUBS_H
#define ALCHEMY_SOUND_STUBS_H

/* The stub calls into the sound module, as the other modules' code calls
   them. */

#include "types.h"

/* Returns the music sySoundPlay last chose. */
Sint32 sySoundGetCurrentBGMStub(void);
/* Brings the music back after a jingle and steps its volume, tempo and pitch
   ramps; called once a VBlank. */
void sySoundUpdateBGMRampsStub(void);
/* Starts the sound driver and resets the music's ramps. */
void sySoundInitStub(void);

#endif
