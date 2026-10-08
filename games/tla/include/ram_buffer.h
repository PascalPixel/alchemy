#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

#include "types.h"

#define Ram_MapCellBuffer FIXED_ADDRESS((u8 *), 0x02010000, gMapCellBuffer)
#define Ram_MapCollision FIXED_ADDRESS((u8 *), 0x0202c000, gMapCollision)

#define Ram_SoundMixer FIXED_ADDRESS((u8 *), 0x03006000, SoundDriver_Mixer)

#define Ram_IwramHeapEnd FIXED_ADDRESS((u8 *), 0x03006fc0, IwramSoundMixWorkspace)

struct BtlWork;
struct BattlePresentationTransition;
struct MapWork;
struct ObjectSystemWork;
struct DisplayTransitionState;

struct HeapSlots {
    void *next_ewram;
    void *next_iwram;
    u8 *transform_stack;
    void *unknown_0c[2];
    void *script_objects;
    struct ObjectSystemWork *object_system;
    void *unknown_1c[1];
    struct MapWork *map_work;
    struct BtlWork *battle_work;
    void *unknown_28[2];
    void *camera_work;
    void *unknown_34[2];
    u8 *window_tiles;
    void *window_layout;
    void *glyph_work;
    void *menu_work;
    void *unknown_4c[4];
    u8 *battle_fx_work[2];
    void *unknown_64[2];
    void *event_work;
    void *unknown_70[3];
    struct DisplayTransitionState *display_transition;
    void *block_32;
    void *unknown_84[11];
    struct BattlePresentationTransition *transition_work;
    u16 *canvas_map;
    void *unknown_b8[5];
    void *serial_work;
    void *unknown_d0[3];
    void *menu_runtime;
    void *effect_work;
    void *unknown_e4[1];
    void *menu_select_work;
    void *unknown_ec[3];
    void *block_62;
    void *unknown_fc[1];
};

#define Ram_HeapSlots FIXED_ADDRESS((struct HeapSlots *), 0x03000000, gHeapSlots)

#endif
