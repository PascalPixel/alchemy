/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320.  A `bl` into that range
 * is an indirect call through the named register.
 *
 * `bl 0x080072fc` here is __call_via_r6, and r6 is the RETURN of the
 * Func_08004938(0x318) at 0x08018d1c -- the workspace this function just
 * allocated and then DMA'd 0x318 bytes into from 0x080155D0.  So the call
 * runs the copied bytes: the same idiom as the nine stack kernels closed
 * earlier in this audit, heap-allocated rather than on the stack.
 *
 * 0x080155D0 confirms as ARM and confirms the arity: it opens
 * `push {r5,r6,r7,r8,r9,sl,lr}`, subtracts 44 from sp, and reads
 * `[sp, #72]` -- which after 28 bytes of pushed registers plus that 44-byte
 * frame is the FIRST STACK ARGUMENT, i.e. argument five.  The five-argument
 * call is confirmed from the callee's own side, not just the caller's.
 *
 * The regularity that governed the earlier batches does NOT apply here: the
 * callee occupied the draft's argument slot matching the veneer's register
 * index only for r0-r3, which are the argument registers.  r6 is not one, so
 * the callee never appeared as an argument and all five here are real.
 * What the copied routine DOES is not named -- only located.
 */
#include "types.h"

typedef s32 (*HeapKernel_08018cac)(void *object, s32 character, s32 x, s32 y,
                                   s32 alternate);

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct DmaTransfer_08018cac {
    const void *source;
    void *destination;
    u32 control;
};

void Func_08002df0(void *);
s32 Func_08002f40(s32);
void *Func_08004938(s32);
u8 *Func_08015e8c(void);
void Func_08016584(void *, void *);
s32 Func_080178b0(s32, void *);
s32 Func_08004080(void);

/*
 * Build one display record for a field object's encoded character.
 *
 * Ordinary characters allocate and initialize a record, render a 4bpp
 * 8-by-16 glyph into a stack tile buffer, and upload it to the object's
 * assigned character slot. Special object modes either delegate to the
 * alternate renderer or use the object's existing tile coordinates.
 */
s32 Func_08018cac(void *object, s32 character, s32 x, s32 y, s32 forced)
{
    u8 tile_data[128];
    u8 *state;
    u8 *record;
    u8 *sprite;
    u32 horizontal_offset;
    u16 vertical_offset;
    u16 record_index;
    s32 result;
    s32 tile;

    state = *(u8 **)0x03001E8C;
    horizontal_offset = M2C_FIELD(state, u16 *, 0x12B0);
    vertical_offset = M2C_FIELD(state, u16 *, 0xEA8);

    if (forced != 1 && (M2C_FIELD(object, u16 *, 0x16) & 8) != 0) {
        s32 alternate;

        if (**(void ***)(state + 0x58) == object) {
            Func_08002f40(0x14);
            Func_08002f40(0x13);
            result = 3;
            if (character == 0x20) {
                return result;
            }
        }

        alternate = Func_08002f40(0x13);
        result = 4;
        if (character == 0x20) {
            return result;
        }

        {
            void *workspace = Func_08004938(0x318);
            struct DmaTransfer_08018cac *dma =
                (struct DmaTransfer_08018cac *)0x040000D4;

            dma->source = (const void *)0x080155D0;
            dma->destination = workspace;
            dma->control = 0x84000000 | (0x318 >> 2);
            result = ((HeapKernel_08018cac)workspace)(object, character, x, y, alternate);
            Func_08002df0(workspace);
        }
        return result;
    }

    result = 5;
    if (character == 0x20) {
        return result;
    }

    record = Func_08015e8c();
    if (record == NULL) {
        return 0;
    }

    record_index = (u16)(((record - state) - 0x698) * 0xB6DB6DB7);
    record[5] = 1;
    record[4] = 0;

    if (forced == 1) {
        result = 1;
        record[5] = 2;
    } else {
        switch (M2C_FIELD(state, u16 *, 0xEAC)) {
        case 2:
            record[5] = 4;
            M2C_FIELD(record, u16 *, 0xC) = 0;
            break;
        case 3:
            record[5] = 5;
            break;
        case 4:
            record[5] = 6;
            M2C_FIELD(record, u16 *, 0xC) = 8;
            break;
        case 5:
            record[5] = 7;
            M2C_FIELD(record, u16 *, 0xC) = 0;
            break;
        }

        result = Func_080178b0(character, tile_data);
        if (result == 0) {
            result = 1;
        }
    }

    sprite = record + 0x10;
    if (record[5] == 2) {
        u16 *random_slot = (u16 *)(state + 0x12B6);
        u16 packed;

        if (*random_slot == 99) {
            *random_slot = (u16)Func_08004080();
        }

        packed = M2C_FIELD(sprite, u16 *, 6);
        packed &= 0xFE00;
        packed |= ((M2C_FIELD(object, u16 *, 0xC) +
                    M2C_FIELD(object, u16 *, 8) - 2) * 8 + 4) & 0x1FF;
        M2C_FIELD(sprite, u16 *, 6) = packed;
        sprite[4] = (u8)((M2C_FIELD(object, u8 *, 0xE) +
                          M2C_FIELD(object, u8 *, 0xA) - 2) * 8 - 1);
    } else {
        struct DmaTransfer_08018cac *dma =
            (struct DmaTransfer_08018cac *)0x040000D4;
        tile = M2C_FIELD(state, u16 *, 0x12B8) + record_index;
        dma->source = tile_data;
        dma->destination = (void *)(0x06010000 + (tile << 5));
        dma->control = 0x84000020;

        M2C_FIELD(record, u16 *, 0x14) =
            y + (vertical_offset >> 1) +
            (M2C_FIELD(object, u16 *, 0xE) << 3) - 2;
        M2C_FIELD(record, u16 *, 0x16) =
            (x + (horizontal_offset >> 1) +
             (M2C_FIELD(object, u16 *, 0xC) << 3) + 2) | 0x4000;
        M2C_FIELD(record, u16 *, 0x18) = tile;
    }

    record[0xF] = 0xFE;
    M2C_FIELD(record, u16 *, 6) = M2C_FIELD(sprite, u16 *, 6) & 0x1FF;
    M2C_FIELD(record, u16 *, 8) = sprite[4];
    record[0xE] = (u8)record_index;
    M2C_FIELD(record, u32 *, 0) = 0;
    Func_08016584(object, record);
    return result;
}
