#include "types.h"

/*
 * Sprite record setup for the menu selection list (0x0801bd98).
 *
 * Loads the graphic for `kind` through one of three tile loaders, records
 * the tile base, source slot and returned tile index in the list node, and
 * resets the node's render object: 2-bit/1-bit attribute fields cleared,
 * size field set to 1, tile index written.  The bitfield aggregate at
 * node + 0x28 is the same layout that the neighbouring owner 0x0801c0dc
 * (games/gs1/src/ui/text/resource/initialize.c) initialises; the source
 * addressed it through a pointer, which is what keeps every byte access
 * within Thumb's 5-bit offset range from one base register.
 *
 * The per-kind tile counts are loaded from the literal pool even for 0x1f,
 * which an integer literal cannot produce, so they are spelled as the
 * established small absolute link-time constants.
 *
 * The bitfield statements keep their original order.  `field_76 = 1` sits
 * before `field_56 = 0`: both share the 0x3f mask, and the reference copies
 * that constant (adds r3, r4, #0) before the byte-7 AND because the byte-5
 * AND still needs it afterwards.  The same byte-7-before-field_56 order
 * appears in the 0x0801c0dc initialiser.
 */

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

void Func_08019ee4(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
void Func_08019fcc(u32 glyph, s32 includeBase, s32 *sourceIndex,
                   s32 *result, s32 reuseSource);
void Func_0801a3d0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

extern u8 Value_0000001f;
extern u8 Value_00000182;
extern u8 Value_00000333;

void Func_0801bd98(u32 kind, s32 base, struct ListNode *node, s32 reuse)
{
    s32 src;
    s32 tile;
    struct Object_0801c0dc *obj;

    switch (kind) {
    case 1:
    case 6:
        if (reuse != 0)
            src = node->src;
        Func_08019ee4(base, 0, &src, &tile, reuse);
        node->end = base + (s32)&Value_0000001f;
        break;
    case 2:
        if (reuse != 0)
            src = node->src;
        Func_08019fcc(base, 1, &src, &tile, reuse);
        node->end = base + (s32)&Value_00000182;
        break;
    case 4:
        if (reuse != 0)
            src = node->src;
        Func_0801a3d0(base, 1, (s32)&src, (s32)&tile, reuse);
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
    obj->field_76 = 1;
    obj->field_56 = 0;
    obj->field_8c = 0;
    obj->field_80 = tile;
    obj->field_8a = 0;
}
