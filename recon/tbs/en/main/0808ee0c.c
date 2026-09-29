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
extern u8 Data_03001ebc[];

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
    register s32 x_delta;
    register s32 y_delta;
    register s32 object_x;
    register s32 object_y;
    register s32 negative_center;
    register u32 maximum;
    register s32 center;
    register s32 index;
    register u8 *object;
    register u8 *entry;

    object = ObjectTable_Get(gGameState.value_1F4);
    entry = *(u8 **)((u32)&Data_03001ebc) + 0x11C;
    index = 0;
    if (entry[4] != 0) {
        object_x = FIELD(object, s32, 8);
        object_y = FIELD(object, s32, 0x10);
        negative_center = -0x80000;
        maximum = 0x1ffffe;
        center = 128;
        center <<= 12;
loop:
        x_offset = entry[6] << 20;
        x_delta = object_x - x_offset;
        rel_x = x_delta + negative_center;
        y_offset = entry[7] << 20;
        y_delta = object_y - y_offset;
        rel_y = y_delta + negative_center;
        if ((u32)(x_delta + 0x7FFFF) <= maximum &&
            (u32)(y_delta + 0x7FFFF) <= maximum) {
            FIELD(object, s32, 8) = x_offset + center;
            FIELD(object, s32, 0x10) = y_offset + center;
            Vector_AddPolarOffset(0x140000,
                          (u16)ArcTan2(rel_y, rel_x),
                          object + 8);
            FIELD(object, s32, 0x38) = 0x80000000;
            FIELD(object, s32, 0x3C) = 0x80000000;
            FIELD(object, s32, 0x40) = 0x80000000;
            return;
        }
        index++;
        entry += 8;
        if (index <= 9 && entry[4] != 0)
            goto loop;
    }
}
