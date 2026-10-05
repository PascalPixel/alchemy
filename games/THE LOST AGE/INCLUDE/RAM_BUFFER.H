#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

#include "TYPES.H"

/* Fixed RAM buffers used by the engine. */

#define Ram_MapCellBuffer ((u8 *)0x02010000) /* gMapCellBuffer */
#define Ram_MapCollision ((u8 *)0x0202c000) /* gMapCollision */

/* Sound setup copies the mixer from ROM to this even IWRAM address. */
#define Ram_SoundMixer ((u8 *)0x03006000) /* SoundDriver_Mixer */

/* The end of the IWRAM the heap hands blocks out of; the sound mixer's
 * workspace follows it. */
#define Ram_IwramHeapEnd ((u8 *)0x03006fc0) /* IwramSoundMixWorkspace */

/* The heap's slot words hold the resident work blocks at the start of IWRAM. */
struct BattleSession;
struct BattlePresentationTransition;
struct MapWork;
struct ObjectSystemWork;
struct DisplayTransitionState;

struct HeapSlots {
    /* The heap cursors: the next free EWRAM and IWRAM addresses. */
    void *next_ewram;
    void *next_iwram;
    /* The transform stack's block of 48-byte matrices. */
    u8 *transform_stack;
    void *unknown_0c[2];
    /* The field's 64 script objects, which The Broken Seal keeps as
       gObjectSlots. */
    void *script_objects;
    /* The object system's work, which The Broken Seal keeps as
       gMenuCtrlWork. */
    struct ObjectSystemWork *object_system;
    void *unknown_1c[1];
    /* The field map's work block. */
    struct MapWork *map_work;
    /* The battle session, which The Broken Seal keeps in its own cell. */
    struct BattleSession *battle_work;
    void *unknown_28[2];
    /* The scene camera. */
    void *camera_work;
    void *unknown_34[2];
    /* The window tilemap: a dirty flag at 3, then 32 x 32 cells from 8. */
    u8 *window_tiles;
    /* The party status window's handle and place. */
    void *window_layout;
    /* The glyph decoder's transfer work, which The Broken Seal keeps as
       gGlyphWork. */
    void *glyph_work;
    /* The menu screen work, which The Broken Seal keeps as gResQueueWork. */
    void *menu_work;
    void *unknown_4c[4];
    /* The battle effect work pair, The Broken Seal's gBattleFxWork. */
    u8 *battle_fx_work[2];
    void *unknown_64[2];
    /* The event work, as The Broken Seal keeps it in its slot 27. */
    void *event_work;
    void *unknown_70[3];
    /* The display transition's state, which The Broken Seal keeps in its
       slot 31 too. */
    struct DisplayTransitionState *display_transition;
    /* Heap block 32, which each screen fills with its own work: the title
       palette fade's (The Broken Seal keeps it in its own cell) or the
       shop's. */
    void *block_32;
    void *unknown_84[11];
    struct BattlePresentationTransition *transition_work;
    /* The battle canvas's pixel map: for each pixel of a 64-wide half, its
       byte in the 8bpp tiles of battle_fx_work[1]. */
    u16 *canvas_map;
    void *unknown_b8[5];
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
    void *unknown_ec[3];
    /* Heap block 62, the arc effect's work. */
    void *block_62;
    void *unknown_fc[1];
};

#define Ram_HeapSlots ((struct HeapSlots *)0x03000000) /* gHeapSlots */

#endif
