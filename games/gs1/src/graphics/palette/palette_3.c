#include "types.h"
#include "scene.h"

/* graphics/palette/reserved_no_op_a.c */
void GraphicsPalette_ReservedNoOpC9BC(void)
{
}

/* graphics/palette/reserved_no_op_b.c */
void GraphicsPalette_ReservedNoOpC9C0(void)
{
}

/* graphics/palette/reserved_no_op_c.c */
void GraphicsPalette_ReservedNoOpC9C4(void)
{
}

/* graphics/palette/set_tint_channels_from_counters.c */
s32 Modulo(s32, s32);
extern u8 gCell[];
extern u8 gRom[];
void GraphicsPalette_SetTintChannelsFromCounters(void *work)
{
    s16 phase; s32 bias; s32 c2, c0, c1;
    phase = Modulo(gCell[0x205] + 0xC, 0x18) * 4;
    bias = gCell[0x206] - 7;
    c0 = gRom[(s16)Modulo(phase, 0x60)];
    c1 = gRom[Modulo(phase + 0x20, 0x60)];
    c2 = gRom[Modulo(phase + 0x40, 0x60)];
    c0 += bias; c1 += bias; c2 += bias;
    if (c0 < 0) c0 = 0; if (c1 < 0) c1 = 0; if (c2 < 0) c2 = 0;
    if (c0 > 0x1F) c0 = 0x1F; if (c1 > 0x1F) c1 = 0x1F; if (c2 > 0x1F) c2 = 0x1F;
    FIELD_AT_OFFSET(work, s16 *, 0x576) = (s16)c0;
    FIELD_AT_OFFSET(work, s16 *, 0x578) = (s16)c1;
    FIELD_AT_OFFSET(work, s16 *, 0x57A) = (s16)c2;
}

/* graphics/palette/write_blended_bank15_entries.c */
void Palette_WriteBlendedBank15Entries(s32 arg0)
{
    *(s16 *)0x050001E8 = Sys_SetMode(arg0, 0xEEEE, 0xCCCC, 0x11110);
    *(s16 *)0x050001EA = Sys_SetMode(arg0, 0xD555, 0xBBBB, 0xEEEE);
    *(s16 *)0x050001EC = Sys_SetMode(arg0, 0xBBBB, 0xAAAA, 0xCCCC);
    *(s16 *)0x050001EE = Sys_SetMode(arg0, 0xA221, 0x9999, 0xAAAA);
    *(s16 *)0x050001F0 = Sys_SetMode(arg0, 0x10888, 0xDDDD, 0x13333);
    *(s16 *)0x050001F2 = Sys_SetMode(arg0, 0x12221, 0xEEEE, 0x15555);
    *(s16 *)0x050001F4 = Sys_SetMode(arg0, 0x13BBB, 0x10000, 0x17777);
}

/* graphics/palette/decrement_selection_wrap.c */
void GraphicsPalette_DecrementSelectionWrap(void *base)
{
    s32 v;
    u16 t;
    s32 cur;

    base = (u8 *)base + 0x574;
    v = FIELD_AT_OFFSET(base, u16 *, 0);
    t = v;
    cur = t;

    if (cur == 0) {
        cur = 2;
    } else {
        cur = v + 0xFFFF;
    }
    FIELD_AT_OFFSET(base, u16 *, 0) = cur;
}
