#ifndef ALCHEMY_OAM_H
#define ALCHEMY_OAM_H

/* The window sprites' shapes. How much of OAM the tick's sprites take, and the
   list they are built into, belong to the system library (system.h). */

#include "edition.h"
#include "types.h"
#include "system.h"

/* The OAM shape and size a window sprite is created with (mnWindowSprCreate):
   attribute 0's shape in the low half, attribute 1's size in the high half, as
   the hardware reads them. */
#define WINDOW_SPRITE_SIZE_16X16 0x40000000
#define WINDOW_SPRITE_SIZE_32X32 0x80000000
#define WINDOW_SPRITE_SIZE_32X8	 0x40004000
#define WINDOW_SPRITE_SIZE_32X16 0x80004000
#define WINDOW_SPRITE_SIZE_16X32 0x80008000

#endif
