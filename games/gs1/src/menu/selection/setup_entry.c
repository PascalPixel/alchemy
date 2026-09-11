#include "types.h"
#include "scene.h"

/* Sprite record setup for the menu selection list. */

struct Object_0801c0dc {
    u8 filler0[5];
    u8 field_50 : 2;
    u8 field_52 : 2;
    u8 field_54 : 1;
    u8 field_55 : 1;
    u8 field_56 : 2;
    u8 filler6;
    u8 field_70 : 1;
    u8 field_71 : 5;
    u8 field_76 : 2;
    u16 field_80 : 10;
    u16 field_8a : 2;
    u16 field_8c : 4;
};

struct ListNode {
    u8 unk_00[8];
    s16 base;
    s16 kind;
    u16 src;
    s16 tile;
    u8 unk_10[16];
    s16 end;
    s16 field_22;
    u8 unk_24[2];
    s16 field_26;
    struct Object_0801c0dc obj;
};

extern u8 Value_0000001f;
extern u8 Value_00000182;
extern u8 Value_00000333;

/*
 * Load the graphic for kind through one of three tile loaders, record the
 * tile base, source slot and returned tile index in the node, then reset the
 * node's render object.  The object is reached through a pointer so that
 * every byte access stays within Thumb's 5-bit offset range from one base
 * register.  The per-kind tile counts are link-time constants; an integer
 * literal cannot produce 0x1f here.
 */
void Menu_Run(u32 kind, s32 base, struct ListNode *node, s32 reuse)
{
    s32 src;
    s32 tile;
    struct Object_0801c0dc *obj;

    switch (kind) {
    case 1:
    case 6:
        if (reuse != 0)
            src = node->src;
        Menu_SetRange(base, 0, &src, &tile, reuse);
        node->end = base + (s32)&Value_0000001f;
        break;
    case 2:
        if (reuse != 0)
            src = node->src;
        Menu_unk2_5(base, 1, &src, &tile, reuse);
        node->end = base + (s32)&Value_00000182;
        break;
    case 4:
        if (reuse != 0)
            src = node->src;
        Menu_unk3_4(base, 1, (s32)&src, (s32)&tile, reuse);
        node->end = base + (s32)&Value_00000333;
        break;
    }

    node->base = base;
    node->src = src;
    node->tile = tile;
    node->kind = kind;
    node->field_22 = 0x100;
    node->field_26 = 0x100;

    obj = &node->obj;
    obj->field_52 = 0;
    obj->field_55 = 0;
    obj->field_54 = 0;
    /*
     * field_76 = 1 stays before field_56 = 0: both mask with 0x3f, and the
     * mask is copied before the byte-7 store because the byte-5 store still
     * needs it.
     */
    obj->field_76 = 1;
    obj->field_56 = 0;
    obj->field_8c = 0;
    obj->field_80 = tile;
    obj->field_8a = 0;
}
