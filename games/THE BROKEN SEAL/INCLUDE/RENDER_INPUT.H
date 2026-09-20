#ifndef ALCHEMY_RENDER_INPUT_H
#define ALCHEMY_RENDER_INPUT_H

#include "TYPES.H"

struct RenderInput {
    u8 unknown_00[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
};

typedef char RenderInput_size[sizeof(struct RenderInput) == 0x10 ? 1 : -1];
typedef char RenderInput_width_offset[
    (u32)&(((struct RenderInput *)0)->width) == 8 ? 1 : -1
];
typedef char RenderInput_x_offset[
    (u32)&(((struct RenderInput *)0)->x) == 12 ? 1 : -1
];

void UiWindow_CopyTilemapRegion(const struct RenderInput *window, const void *source);
void UiWindow_SetTileAttributeRect(const struct RenderInput *window,
    s32 x, s32 y, s32 width, s32 height, u32 field);

/* 描画登録1件。+0x18の下位10bitは表番号として扱う。 */
union RenderTableValue {
    u32 value;
    struct {
        unsigned short index : 10;
        unsigned short reserved : 6;
        u16 high;
    } bits;
    struct {
        u16 low;
        u16 high;
    } half;
};

struct RenderOutput {
    s32 zero;
    s8 one4;
    s8 one5;
    s16 x;
    s16 y;
    u8 unknown_0a[4];
    s8 index;
    u8 sentinel;
    u8 unknown_10[4];
    s32 packed;
    union RenderTableValue table;
};

struct RenderOutput *RenderOutput_Create(
    s32 no,
    s32 flags,
    struct RenderInput *input,
    s32 offset_x,
    s32 offset_y);

#define RENDER_OUTPUT_OFFSET(field) \
    ((u32)&(((struct RenderOutput *)0)->field))

typedef char RenderOutput_sentinel_offset[
    RENDER_OUTPUT_OFFSET(sentinel) == 0x0f ? 1 : -1
];
typedef char RenderOutput_packed_offset[
    RENDER_OUTPUT_OFFSET(packed) == 0x14 ? 1 : -1
];
typedef char RenderOutput_table_offset[
    RENDER_OUTPUT_OFFSET(table) == 0x18 ? 1 : -1
];
typedef char RenderOutput_size[
    sizeof(struct RenderOutput) == 0x1c ? 1 : -1
];

#undef RENDER_OUTPUT_OFFSET

#endif
