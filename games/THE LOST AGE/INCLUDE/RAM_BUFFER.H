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

/* The heap's slot words, which hold the resident work blocks, open IWRAM.
 * GCC reaches a slot from the constant base with the slot's offset in the
 * load, as it does for a field of a structure at a constant address. */
struct HeapSlots {
    void *unknown_00[27];
    /* The event work, as The Broken Seal keeps it in its slot 27. */
    void *event_work;
};

#define Ram_HeapSlots ((struct HeapSlots *)0x03000000) /* gHeapSlots */

#endif
