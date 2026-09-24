#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

#include "TYPES.H"

/* Low runtime services every module reaches through the system call table
   at the start of ROM. */

void WaitFrames(s32 frames);

#endif
