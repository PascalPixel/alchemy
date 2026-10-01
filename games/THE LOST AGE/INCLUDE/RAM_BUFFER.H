#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

#include "TYPES.H"

/* Fixed RAM buffers that Camelot's code addressed as constants, as in TBS:
 * code the ROM shows folding a constant address reaches its buffer through
 * the entry here, and the build checks each entry against the object named
 * beside it. Everywhere else the label serves. */

#define Ram_MapCellBuffer ((u8 *)0x02010000) /* gMapCellBuffer */
#define Ram_MapCollision ((u8 *)0x0202c000) /* gMapCollision */

/* The IWRAM copy of the sound mixer, which the sound set-up fills from ROM.
 * Its label is a Thumb routine, so only a constant gives the even address
 * the copy writes to without an extra instruction. */
#define Ram_SoundMixer ((u8 *)0x03006000) /* SoundDriver_Mixer */

/* The end of the IWRAM the heap hands blocks out of; the sound mixer's
 * workspace follows it. */
#define Ram_IwramHeapEnd ((u8 *)0x03006fc0) /* IwramSoundMixWorkspace */

/* The heap's slot words, which hold the resident work blocks, open IWRAM.
 * GCC reaches a slot from the constant base with the slot's offset in the
 * load, as it does for a field of a structure at a constant address. */
struct BattleSession;
struct MapWork;

struct HeapSlots {
    void *unknown_00[5];
    /* The field's 64 script objects, which The Broken Seal keeps as
       gObjectSlots. */
    void *script_objects;
    void *unknown_18[2];
    /* The field map's work block. */
    struct MapWork *map_work;
    /* The battle session, which The Broken Seal keeps in its own cell. */
    struct BattleSession *battle_work;
    void *unknown_28[5];
    /* The window tilemap: a dirty flag at 3, then 32 x 32 cells from 8. */
    u8 *window_tiles;
    void *unknown_40[2];
    /* The menu screen work, which The Broken Seal keeps as gResQueueWork. */
    void *menu_work;
    void *unknown_4c[8];
    /* The event work, as The Broken Seal keeps it in its slot 27. */
    void *event_work;
    void *unknown_70[23];
    /* The serial link runtime's work. */
    void *serial_work;
    void *unknown_d0[3];
    /* The item and Psynergy menus' work, which The Broken Seal keeps as
       gMenuWork. */
    void *menu_runtime;
    /* The battle effects' work, which The Broken Seal keeps as gEffectWork. */
    void *effect_work;
    void *unknown_e4[1];
    /* The resource selection menu's work, which The Broken Seal keeps as
       gMenuSelectWork. */
    void *menu_select_work;
    void *unknown_ec[5];
};

#define Ram_HeapSlots ((struct HeapSlots *)0x03000000) /* gHeapSlots */

#endif
