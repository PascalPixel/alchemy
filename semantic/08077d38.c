#include "layout_guard.h"
#include "types.h"

typedef struct DmaChannel_08077d38 {
    const void *source;
    void *destination;
    u32 control;
} DmaChannel_08077d38;

typedef struct SharedState_08077d38 {
    s32 field_000;
    s32 field_004;
    u8 padding008[8];
    s32 field_010;
    u8 padding014[0x109];
    u8 step_table[15];
    u8 padding12c[0xc8];
    s32 selected_object;
    u8 padding1f8[0x0d];
    u8 field_205;
    u8 field_206;
    u8 padding207[3];
    u8 enabled_20a;
    u8 enabled_20b;
    u8 field_20c;
    u8 padding20d[3];
    s16 timing_table[7];
    u8 padding21e[2];
    s16 field_220;
    s16 field_222;
    u8 padding224[6];
    u8 field_22a;
    u8 padding22b[0x8d];
    s32 field_2b8;
} SharedState_08077d38;

LAYOUT_OFFSET_GUARD(
    SharedState08077d38_StepTable,
    SharedState_08077d38,
    step_table,
    0x11d);
LAYOUT_OFFSET_GUARD(
    SharedState08077d38_SelectedObject,
    SharedState_08077d38,
    selected_object,
    0x1f4);
LAYOUT_OFFSET_GUARD(
    SharedState08077d38_Field205,
    SharedState_08077d38,
    field_205,
    0x205);
LAYOUT_OFFSET_GUARD(
    SharedState08077d38_Enabled20a,
    SharedState_08077d38,
    enabled_20a,
    0x20a);
LAYOUT_OFFSET_GUARD(
    SharedState08077d38_TimingTable,
    SharedState_08077d38,
    timing_table,
    0x210);
LAYOUT_OFFSET_GUARD(
    SharedState08077d38_Field22a,
    SharedState_08077d38,
    field_22a,
    0x22a);
LAYOUT_OFFSET_GUARD(
    SharedState08077d38_Field2b8,
    SharedState_08077d38,
    field_2b8,
    0x2b8);

s32 Func_08077cb8(void);
void Func_08078ee8(void);
void Func_0807961c(s32 mode);

static void StartFill32_08077d38(
    volatile DmaChannel_08077d38 *dma,
    const u32 *value,
    void *destination,
    u32 control)
{
    dma->source = value;
    dma->destination = destination;
    dma->control = control;
}

/* Clear the shared arenas and establish their boot-time defaults. */
s32 Func_08077d38(void)
{
    volatile DmaChannel_08077d38 *dma =
        (volatile DmaChannel_08077d38 *)0x040000d4;
    SharedState_08077d38 *shared =
        (SharedState_08077d38 *)0x02000240;
    u32 zero = 0;
    s32 result;
    s32 group;
    s32 index;

    StartFill32_08077d38(dma, &zero, (void *)0x02000240, 0x850000b0);
    zero = 0;
    StartFill32_08077d38(dma, &zero, (void *)0x02001000, 0x850003e1);
    while ((dma->control & 0x80000000) != 0) {
    }

    zero = 0;
    StartFill32_08077d38(dma, &zero, (void *)0x02000040, 0x85000080);
    *(u8 *)0x02001104 = 0xff;

    zero = 0;
    StartFill32_08077d38(dma, &zero, (void *)0x02000500, 0x85000298);
    Func_08078ee8();

    shared->timing_table[0] = 1;
    shared->timing_table[1] = 2;
    shared->timing_table[2] = 4;
    shared->timing_table[3] = 8;
    shared->timing_table[4] = 0x200;
    shared->timing_table[5] = 0x100;
    shared->timing_table[6] = 2;
    shared->field_220 = 0;
    shared->field_222 = 0;
    shared->selected_object = 0;

    Func_0807961c(0);
    shared->field_010 = 0;
    shared->field_20c = 1;
    shared->enabled_20a = 1;
    shared->enabled_20b = 1;
    shared->field_205 = 0;
    shared->field_206 = 8;
    shared->field_000 = 0;

    result = Func_08077cb8();
    shared->field_2b8 = result;
    *(s32 *)0x03001c9c = 0;
    *(u8 *)0x03001d08 = 0;
    shared->field_004 = 0;
    shared->field_22a = *(u8 *)0x03001d08;
    *(s16 *)0x03001d24 = 0;
    *(s16 *)0x02002004 = -1;

    for (group = 0; group < 5; group++) {
        for (index = 0; index < 3; index++)
            shared->step_table[group * 3 + index] = 4 << group;
    }

    return result;
}
