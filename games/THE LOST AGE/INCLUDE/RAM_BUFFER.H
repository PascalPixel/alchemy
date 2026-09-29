#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

#include "TYPES.H"

/* Fixed RAM buffers that Camelot's code addressed as constants, as in TBS:
 * code the ROM shows folding a constant address reaches its buffer through
 * the entry here, and the build checks each entry against the object named
 * beside it. Everywhere else the label serves. */

#define Ram_MapCellBuffer ((u8 *)0x02010000) /* gMapCellBuffer */

/* The IWRAM copy of the sound mixer, which the sound set-up fills from ROM.
 * Its label is a Thumb routine, so only a constant gives the even address
 * the copy writes to without an extra instruction. */
#define Ram_SoundMixer ((u8 *)0x03006000) /* SoundDriver_Mixer */

#endif
