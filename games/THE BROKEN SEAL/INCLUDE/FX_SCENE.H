#ifndef ALCHEMY_FX_SCENE_H
#define ALCHEMY_FX_SCENE_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "EFFECT_SLOT.H"

/* Heap slot 56: the active field/battle effect keeps its object pair,
   saved target state and 24 screen-space slots in this 0x720-byte block. */
struct BattleFxScene {
    s32 angle;
    s32 x;
    s32 y;
    s32 z;
    void *main_object;
    void *child;
    s16 first_object_id;
    s16 second_object_id;
    u16 action;
    s16 animation;
    s8 enabled;
    u8 mode;
    s8 visible;
    s8 active;
    void (*finish_callback)(void);
    u8 unknown_28[0x0c];
    s8 child_option;
    s8 child_mode;
    u8 unknown_36[2];
    void *saved_callback;
    const s32 *saved_script;
    u8 unknown_40[4];
    u8 saved_palette;
    u8 lit;
    u16 tile_slot;
    u16 unknown_48;
    u16 free_blocks;
    s32 saved_x;
    s32 saved_y;
    s32 saved_z;
    struct EffectSlot slots[24];
    u8 unknown_718[2];
    s16 arc_tile_slot;
    s8 message_mode;
    u8 unknown_71d[3];
};

LAYOUT_SIZE_GUARD(BattleFxScene_Size, struct BattleFxScene, 0x720);
LAYOUT_OFFSET_GUARD(BattleFxScene_MainObject, struct BattleFxScene, main_object, 0x10);
LAYOUT_OFFSET_GUARD(BattleFxScene_Action, struct BattleFxScene, action, 0x1c);
LAYOUT_OFFSET_GUARD(BattleFxScene_SavedCallback, struct BattleFxScene, saved_callback, 0x38);
LAYOUT_OFFSET_GUARD(BattleFxScene_TileSlot, struct BattleFxScene, tile_slot, 0x46);
LAYOUT_OFFSET_GUARD(BattleFxScene_SavedX, struct BattleFxScene, saved_x, 0x4c);
LAYOUT_OFFSET_GUARD(BattleFxScene_Slots, struct BattleFxScene, slots, 0x58);
LAYOUT_OFFSET_GUARD(BattleFxScene_ArcTileSlot, struct BattleFxScene, arc_tile_slot, 0x71a);
LAYOUT_OFFSET_GUARD(BattleFxScene_MessageMode, struct BattleFxScene, message_mode, 0x71c);

#endif
