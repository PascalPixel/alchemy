/* 2026-09-29 alchemy permute: score 1455 to 1170 on the permuter's scorer
   (0 is exact); remaining 29 register-only, 4 operand, 7 reordered, 3
   inserted, 2 deleted. Kept rewrites: 4x reorder independent statements,
   3x introduce a temporary, 3x add a same-width cast, 2x drop a same-width
   cast, 2x pointer arithmetic or indexing, 1x swap commutative operands,
   1x reorder local declarations, 1x split or join a compound assignment,
   1x toggle register, 1x test truth or compare with zero, then 2x reorder
   independent statements, 2x introduce a temporary, 1x swap commutative
   operands, 1x reorder local declarations, 1x remove a temporary, 1x split
   or join a compound assignment, 1x toggle register. FAKEMATCH: the
   permuter's temporaries, register hints and swapped operand orders below
   only steer allocation and scheduling; no programmer would write them, so
   they stay tagged until a natural spelling replaces them. */
/*
 * main:0808ee0c BattleFx_EmitRandomParticle - draft; the range links as
 * disassembly (recon/tbs/raw/0808ee0c.s).
 *
 * Moves the effect's particle object onto the first of up to ten emitter
 * entries it lies within 0x80000 of, then sends it off 0x140000 along the
 * angle from that entry.
 *
 * Remaining: the reference loads -0x80000 from the literal pool once before
 * the loop and keeps it in lr for both relative offsets. Written as the
 * constant below, GCC folds it into each use and reloads it from the pool
 * inside the loop, which moves every later register; a do/while loop in
 * place of the goto changes the loop layout instead. The unit matched only
 * while the number was a link-time symbol named after itself.
 */
#include "EFFECT_RUNTIME.H"
#include "OBJECT_LOOKUP.H"
#include "GLOBAL_CELLS.H"
#include "FIXED_MATH.H"
extern u8 gEventWork[];

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 ArcTan2(s32, s32);
void Vector_AddPolarOffset(s32, u16, void *);

struct GlobalData {
    u8 filler[0x1F4];
    u32 value_1F4;
};

extern struct GlobalData gGameState;

void BattleFx_EmitRandomParticle(void)
{
    register s32 rel_x;
    register s32 rel_y;
    register s32 x_offset;
    register s32 y_offset;
    s32 x_delta;
    register s32 y_delta;
    register s32 object_x;
    register s32 negative_center;
    register s32 object_y;
    register u32 maximum;
    register s32 index;
    s32 center;
    register u8 *object;
    register u8 *entry;
    void *tmp2;

    tmp2 = ObjectTable_Get(gGameState.value_1F4);
    object = tmp2;
    entry = ((u8 **)(u32)&gEventWork)[0] + 0x11C;
    index = 0;
    if (entry[4]) {
        s32 tmp3;
        u8 tmp4;
        s32 tmp;
        object_x = FIELD(object, s32, 8);
        object_y = FIELD(object, s32, 0x10);
        center = 128;
        (u32)(negative_center = -0x80000);
        center <<= 12;
        maximum = 0x1ffffe;
    loop:
        tmp4 = entry[7];
        x_offset = entry[6] << 20;
        y_offset = tmp4 << 20;
        x_delta = object_x - x_offset;
        tmp = object_y - y_offset;
        y_delta = tmp;
        rel_y = y_delta + negative_center;
        tmp3 = 0x7FFFF + x_delta;
        rel_x = x_delta + negative_center;
        if ((u32)tmp3 <= maximum && (u32)(0x7FFFF + y_delta) <= maximum) {
            FIELD(object, s32, 8) = x_offset + center;
            FIELD(object, s32, 0x10) = y_offset + center;
            Vector_AddPolarOffset(0x140000, (u16)ArcTan2(rel_y, rel_x), object + 8);
            FIELD(object, s32, 0x38) = 0x80000000;
            FIELD(object, s32, 0x3C) = 0x80000000;
            FIELD(object, s32, 0x40) = 0x80000000;
            return;
        }
        ++index;
        entry = &entry[8];
        if (index <= 9 && entry[4] != 0)
            goto loop;
    }
}
