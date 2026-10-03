#ifndef ALCHEMY_CARTBOOT_H
#define ALCHEMY_CARTBOOT_H

#include "TYPES.H"

/* The hardware cartridge entry restarts in ARM state. The build checks
   this constant against the physical label at the cartridge header. */
#define Cart_Restart ((u32)0x08000000) /* Rom_Start */

#endif
