#ifndef ALCHEMY_ANIMSPR_H
#define ALCHEMY_ANIMSPR_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* One scripted layer of the composite object. */
struct AnimationEntry {
    s16 anim_id;    /* 0x00 */
    u16 timer;      /* 0x02 compared as s16, updated as u16 */
    u8 kind;        /* 0x04 direction-table selector */
    u8 param;       /* 0x05 draw parameter */
    u8 priority;    /* 0x06 draw order, 0..3 */
    u8 mode;        /* 0x07 1 = copy, 3 = decode, else draw */
    void **frames;  /* 0x08 */
    void *field_0c;
    u8 *script;     /* 0x10 */
    u8 pos;         /* 0x14 script cursor */
    u8 step;        /* 0x15 timer decrement */
    u8 frame;       /* 0x16 selected frame, 255 = none */
    u8 frame_base;  /* 0x17 */
};

/* Each linked OAM part carries the three hardware attributes. */
struct AnimationSpritePart {
    u8 unknown_00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 object_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 attr : 6;
    u8 unknown_0a[2];
};

/* ResourceObject_Create initializes both parts in the 64-object pool;
   projection and composition operate on this same 56-byte object. */
struct AnimationObject {
    struct AnimationSpritePart part[2];
    s32 scale;      /* 0x18 */
    u8 slot;        /* 0x1c */
    u8 display_flags; /* 0x1d hide and fixed-scale flags */
    u16 rotation;    /* 0x1e */
    u8 width;       /* 0x20 */
    u8 height;      /* 0x21 */
    s8 offset_x;
    s8 offset_y;
    u8 last_no;     /* 0x24 */
    u8 dirty;       /* 0x25 */
    u8 flags;       /* 0x26 bit 1 requests the outline pass */
    u8 count;       /* 0x27 */
    struct AnimationEntry *entries[4]; /* 0x28 */
};

LAYOUT_SIZE_GUARD(AnimationEntry_Size, struct AnimationEntry, 0x18);
LAYOUT_SIZE_GUARD(AnimationSpritePart_Size, struct AnimationSpritePart, 0x0c);
LAYOUT_OFFSET_GUARD(AnimationObject_Scale, struct AnimationObject, scale, 0x18);
LAYOUT_OFFSET_GUARD(AnimationObject_Rotation, struct AnimationObject, rotation, 0x1e);
LAYOUT_OFFSET_GUARD(AnimationObject_Width, struct AnimationObject, width, 0x20);
LAYOUT_SIZE_GUARD(AnimationObject_Size, struct AnimationObject, 0x38);
LAYOUT_OFFSET_GUARD(AnimationObject_Entries, struct AnimationObject, entries, 0x28);

#endif
