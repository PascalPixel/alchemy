/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * Callee signatures here are established, not guessed: 0x03001388 is the
 * word copy declared in the EXACT src/080d40ec.c, and 0x03000168 is the fill
 * documented in semantic/main/080e15e8.c as (destination, size, value).
 *
 * This file's own header already identified the bank correctly, including
 * the index arithmetic, before this audit began -- it is the third place in
 * the tree that had. The four sites below reach 0x03001388 through
 * __call_via_sl, sl taking its value from r3 at 0x0800fdd2.
 */
#include "types.h"

/*
 * Build one entry of the twelve-byte scene table at 0x08013784 into a freshly
 * allocated 404-byte layer block, program the three background control
 * registers from the scene header, optionally stage the four VRAM character
 * blocks through a 16 KiB scratch buffer, and return 2.
 *
 * The owner spans two assembled rows.  The b.n at 0x0800fd20 leaves
 * asm/0800fb38.s for 0x0800fd5c, which does not begin with a push: it reads r0
 * (block + 0x101, set at 0x0800fcde), ip (block + 0x102, set at 0x0800fcfa),
 * r5 (the scene header) and this frame's [sp,#8], and it ends with the
 * epilogue that unwinds exactly the prologue pushed at 0x0800fb38.  So
 * 0x0800fd5c is this owner's continuation, not a tail call, and both code
 * spans are registered as executable ranges of one function.  That epilogue
 * ends pop {r1}; bx r1 with N != 0, so r0 is the result, and the only value
 * reaching it is the movs r0,#2 at 0x0800fe5c.
 *
 * The bl Func_080072f0 at 0x0800fb7a is the runtime _call_via_r3 thunk inside
 * 0x080072e4..0x08007320 - index (0x080072f0 - 0x080072e4) / 4 = 3 - so it is
 * an ordinary indirect call through r3, which holds the pooled 0x03000164.
 * The mov ip,pc ; bx r9 at 0x0800fc9c and 0x0800fcac are ordinary indirect
 * calls to the pooled 0x03000118, the IWRAM-relocated ARM fixed-point
 * multiply, as recorded for 0x0800ebec; the movs r0,r0 before each is inside
 * the instruction stream, aligning the return address that mov ip,pc captures
 * so it lands past the bx.
 *
 * The mov sl,r3 at 0x0800fdd2 loads the pooled 0x03001388 into a register no
 * later instruction reads; it is written as a dead assignment would be, that
 * is, not written at all.
 */

struct Layer_0800fb38 {
    u8 x;
    u8 y;
    s8 scale_x;
    s8 scale_y;
    s8 shear_x;
    s8 shear_y;
    u8 width;
    u8 height;
};

struct Header_0800fb38 {
    u8 corner[4];
    u8 character_base[3];
    u8 priority_bg3;
    u8 priority_bg2;
    u8 priority_bg1;
    u8 unknown_0a[2];
    struct Layer_0800fb38 layers[3];
    u32 palette_offset;
    u32 tile_offset;
    u32 map_offset;
    u32 first_extra_offset;
    u32 second_extra_offset;
    u32 script_offset;
};

struct LayerSlot_0800fb38 {
    s32 origin_x;
    s32 origin_y;
    s32 x;
    s32 y;
    s32 scale_x;
    s32 scale_y;
    s32 shear_x;
    s32 shear_y;
    s32 accumulator_x;
    s32 accumulator_y;
    u16 width;
    u16 height;
    const void *map;
};

struct Block_0800fb38 {
    u8 unknown_00[16];
    const void *script;
    u16 display;
    u8 unknown_16[206];
    s32 scroll_x;
    s32 scroll_y;
    s32 corner[4];
    u8 unknown_fc[4];
    u8 character_base[3];
    u8 unknown_103;
    struct LayerSlot_0800fb38 slots[3];
};

struct Scene_0800fb38 {
    u16 header;
    u16 palette;
    u16 first_character;
    u16 second_character;
    u16 third_character;
    u16 sprite;
};

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_0800fb38)(s32 a, s32 b);
/* 0x03000164, the IWRAM-relocated block clear. */
typedef void (*Clear_0800fb38)(void *block, s32 size);

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

void Func_08003bb4(s32 mode);
struct Block_0800fb38 *Func_080048f4(s32 kind, s32 size);
struct Header_0800fb38 *Func_08002f40(s32 resource);
void Func_08005340(const void *source, void *destination);
void Func_08005394(const void *source, void *destination);
void Func_0800f9f4(void);
void Func_0800fac8(void);
void Func_080118d8(void *buffer);
void Func_08011a84(void *buffer);
s32 Func_080770c0(s32 condition);
void Func_080770d0(s32 condition);
void *Func_08004938(s32 size);
void Func_08002df0(void *buffer);
void Func_080041d8(s32 first, s32 second);

s32 Func_0800fb38(s32 index)
{
    volatile u16 *display_control = (volatile u16 *)0x04000000;
    struct Scene_0800fb38 *scene =
        (struct Scene_0800fb38 *)0x08013784 + index;
    struct Block_0800fb38 *block;
    struct Header_0800fb38 *header;
    struct LayerSlot_0800fb38 *slot;
    const struct Layer_0800fb38 *layer;
    Multiply_0800fb38 multiply;
    void *buffer;
    s32 count;

    *display_control = (u16)(*display_control & 0xc1ff);
    Func_08003bb4(0);

    block = Func_080048f4(8, 404);
    ((Clear_0800fb38)0x03000164)(block, 404);

    header = Func_08002f40(scene->header + 0x128);
    Func_08005340((const u8 *)header + header->palette_offset,
                  (void *)0x02010001);
    Func_0800f9f4();
    Func_08005340((const u8 *)header + header->tile_offset,
                  (void *)0x0202c000);
    Func_08005340((const u8 *)header + header->map_offset,
                  (void *)0x02010000);
    Func_0800fac8();

    if (header->first_extra_offset != 0) {
        Func_08005340((const u8 *)header + header->first_extra_offset,
                      (void *)0x0202d000);
        Func_080118d8((void *)0x0202d000);
    }
    if (header->second_extra_offset != 0) {
        Func_08005340((const u8 *)header + header->second_extra_offset,
                      (void *)0x0202de00);
        Func_08011a84((void *)0x0202de00);
    }

    block->script = (const u8 *)header + header->script_offset;
    block->corner[0] = header->corner[0] << 19;
    block->corner[1] = header->corner[1] << 19;
    block->corner[2] = header->corner[2] << 19;
    block->corner[3] = header->corner[3] << 19;
    block->scroll_x = 0;
    block->scroll_y = 0;
    block->character_base[0] = header->character_base[0];
    block->character_base[1] = header->character_base[1];
    block->character_base[2] = header->character_base[2];

    multiply = (Multiply_0800fb38)0x03000118;
    layer = header->layers;
    slot = block->slots;
    for (count = 2; count >= 0; count--) {
        s32 x = layer->x;
        s32 y = layer->y;
        s32 origin_x = x << 19;
        s32 origin_y = y << 19;
        s32 scale_x = layer->scale_x << 12;
        s32 scale_y = layer->scale_y << 12;

        slot->x = origin_x;
        slot->y = origin_y;
        slot->shear_x = layer->shear_x << 12;
        slot->shear_y = layer->shear_y << 12;
        slot->width = layer->width;
        slot->height = layer->height;
        slot->accumulator_x = 0;
        slot->accumulator_y = 0;
        slot->scale_x = scale_x;
        slot->scale_y = scale_y;
        slot->map = (const u8 *)0x02010000 +
                    ((((u32)y >> 1) << 7) + ((u32)x >> 1)) * 4;
        slot->origin_x = multiply(block->scroll_x, scale_x) + origin_x;
        slot->origin_y = multiply(block->scroll_y, scale_y) + origin_y;
        layer++;
        slot++;
    }

    block->display = 0x1000;
    if (block->character_base[0] != 0)
        block->display = 0x1800;
    if (block->character_base[1] != 0)
        block->display |= 0x400;
    if (block->character_base[2] != 0)
        block->display |= 0x200;

    *(volatile u16 *)0x0400000e =
        (u16)(block->character_base[0] | (header->priority_bg3 << 2) | 0x500);
    *(volatile u16 *)0x0400000c =
        (u16)(block->character_base[1] | (header->priority_bg2 << 2) | 0x600);
    *(volatile u16 *)0x0400000a =
        (u16)(block->character_base[2] | (header->priority_bg1 << 2) | 0x700);

    if (Func_080770c0(368) != 0) {
        Func_080770d0(368);
    } else {
        buffer = Func_08004938(0x4000);
        if (buffer != 0) {
            s16 saved = *(volatile s16 *)0x05000000;

            Func_08005340(Func_08002f40(scene->palette + 0x128), buffer);
            *(s16 *)buffer = saved;
            ((WordCopy)0x03001388)((void *)0x05000000, buffer, 448);

            Func_08005394(Func_08002f40(scene->first_character + 0x128),
                          buffer);
            ((WordCopy)0x03001388)((void *)0x06004000, buffer, 0x4000);

            Func_08005394(Func_08002f40(scene->second_character + 0x128),
                          buffer);
            ((WordCopy)0x03001388)((void *)0x06008000, buffer, 0x4000);

            Func_08005394(Func_08002f40(scene->third_character + 0x128),
                          buffer);
            ((WordCopy)0x03001388)((void *)0x0600c000, buffer, 0x4000);

            Func_08005394(Func_08002f40(scene->sprite + 0x128),
                          (void *)0x02028000);
            Func_08002df0(buffer);
        }
    }

    *(volatile u16 *)0x0400004c = 0;
    *(volatile u16 *)0x04000050 = 0;
    *display_control = 320;
    Func_080041d8(0x08010001, 0x00000c85);
    return 2;
}
