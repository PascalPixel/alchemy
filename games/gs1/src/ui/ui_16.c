#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "gs1_edition.h"
#include "text_render_runtime.h"
#include "render_input.h"

/* ui/runtime/set/set_param_nibble.c */
void UiWork_SetParamNibble(s32 param)
{
  s16 val;
  void *work;
  work = *((void **)ADDR_03001E8C);
  val = (s16)(param & 0xF);
 do { *((s16 *)(work + RENDER_PARAM_OFS)) = val; } while (0);
}

/* ui/runtime/set/set_render_word.c */
void UiWork_SetRenderWord(u16 value)
{
    *(u16 *)(*(u8 **)ADDR_03001E8C + RENDER_WORD_OFS) = value;
}

/* ui/text/draw/draw_resource.c */
void UiText_BuildRenderEntries(s32, s32);

void UiText_DrawResource(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *base = *(u8 **)ADDR_03001E8C;
    u16 *counter = (u16 *)(base + RENDER_ENTRY_COUNT_OFS);
    s32 offset;
    s32 zero = 0;

    *counter = zero;
    UiText_BuildRenderEntries(arg0, 1);
    offset = *counter * 2 + RENDER_ENTRY_TBL_OFS;
    *(u16 *)(base + offset) = zero;
    *counter = (*counter + 1) & RENDER_ENTRY_MASK;
    /* 0xeb0から始まる列を次の処理へ渡す。 */
    Ui_SetMode(base + RENDER_ENTRY_TBL_OFS, arg1, arg2, arg3);
}

/* ui/text/draw/draw_character.c */
struct TextPosition_0801e7c0 {
    u8 padding[12];
    u16 x;
    u16 y;
};

extern u8 *gIw;

s32 UiText_BuildRenderEntries(s32 character, s32 count);

void UiText_DrawCharacter(
    s32 character,
    struct TextPosition_0801e7c0 *position,
    u32 offset_x,
    u32 offset_y)
{
    s32 byte_offset;
    s32 vram_address;
    u16 *text;
    s32 zero;
    u32 cell;
    u8 *base;
    u16 *counter;

    base = gIw;
    counter = (u16 *)(base + RENDER_ENTRY_COUNT_OFS);
    zero = 0;
    *counter = zero;
    UiText_BuildRenderEntries(character, 1);

    byte_offset = *counter * 2 + RENDER_ENTRY_TBL_OFS;
    *(u16 *)(base + byte_offset) = zero;
    *counter = (u16)((*counter + 1) & RENDER_ENTRY_MASK);

    cell = ((position->y + (offset_y >> 3) + 1) << 5)
        + (position->x + (offset_x >> 3)) + 1;
    if (cell < 0x280U) {
        byte_offset = cell * 2;
        vram_address = byte_offset + 0x06002000;
        text = (u16 *)(base + RENDER_ENTRY_TBL_OFS);
        Ui_SetMode(
            text,
            (s32)(base + byte_offset),
            vram_address,
            7 & offset_x);
    }
}

/* ui/text/draw/draw_string.c */
s16 *Runtime_BumpAllocateAlternatePool(s32 arg0);

void UiText_DrawString(u8 *text, s32 arg1, s32 arg2, s32 arg3)
{
    s16 *buffer;
    s16 *output;
    u8 *input;

    input = text;
    buffer = Runtime_BumpAllocateAlternatePool(0x200);
    output = buffer;
    if (*input != 0) {
        do {
            *output = (s16)*input;
            input++;
            output++;
        } while (*input != 0);
    }
    *output = 0;
    Ui_SetMode(buffer, arg1, arg2, arg3);
    Ui_Check(buffer);
}

/* ui/text/draw/draw_string_at_offset.c */
extern u8 *gIw;

s16 *Runtime_BumpAllocateAlternatePool(s32 arg0);

void UiText_DrawStringAtOffset(
    u8 *text,
    struct TextRenderWork *work,
    s32 offset_x,
    s32 offset_y)
{
    s16 *buffer;
    s16 *output;
    u8 *base;
    u32 cell;
    s32 vram;
    s32 src;
    s32 dst;
    s32 phase;

    buffer = Runtime_BumpAllocateAlternatePool(0x200);
    base = gIw;
    output = buffer;
    if (*text != 0) {
        do {
            *output = *text;
            text++;
            output++;
        } while (*text != 0);
    }
    *output = 0;

    /* 座標を8ドット単位のセル番号へ変換する。 */
    cell = ((work->y + ((u32)offset_y >> 3) + 1) << 5)
        + (work->x + ((u32)offset_x >> 3)) + 1;
    if (cell < 0x280) {
        vram = 0x06002000;
        cell *= 2;
        dst = vram + cell;
        src = (s32)base + cell;
        phase = offset_x & 7;
        Ui_SetMode(
            buffer,
            src,
            dst,
            phase);
        Ui_Check(buffer);
    }
}

/* ui/text/draw/draw_string_in_window.c */
s16 *Runtime_BumpAllocateAlternatePool(s32 arg0);

void UiText_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y)
{
    u16 *buffer = Runtime_BumpAllocateAlternatePool(0x200);
    u16 *output = buffer;

    while (*text != 0) {
        *output = *text;
        text++;
        output++;
    }
    *output = 0;
    x >>= 3;
    y >>= 3;
    Ui_SetMode(buffer, arg1, x, y);
    Ui_Do(buffer);
}

/* ui/text/draw/draw_number.c */
u8 *Text_FormatNumber(u8 *, s32, s32);
void UiText_DrawString(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);

void UiText_DrawNumber(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    /* 16バイト一時領域を介して次の処理へ渡す。 */
    UiText_DrawString(Text_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

/* ui/text/draw/draw_number_at_offset.c */
u8 *Text_FormatNumber(u8 *, s32, s32);

void UiText_DrawNumberAtOffset(
    s32 arg0,
    s32 arg1,
    struct TextRenderWork *arg2,
    s32 arg3,
    s32 arg4)
{
    u8 data[16];

    /* 16バイト一時領域を介して次の処理へ渡す。 */
    UiText_DrawStringAtOffset(Text_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

/* ui/text/draw/draw_number_in_window.c */
u8 *Text_FormatNumber(u8 *, s32, s32);
void UiText_DrawStringInWindow(u8 *s, s32 arg1, u32 arg2, u32 arg3);

void UiText_DrawNumberInWindow(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    /* 16バイト一時領域を介して次の処理へ渡す。 */
    UiText_DrawStringInWindow(Text_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

/* ui/text/draw/draw_prefixed_number_at_offset.c */
extern u8 *gIw;

u8 *Text_FormatNumber(u8 *output, s32 value, s32 width);

void UiText_DrawPrefixedNumberAtOffset(
    s32 value,
    struct TextRenderWork *work,
    s32 offset_x,
    s32 offset_y,
    s32 variant)
{
    u8 formatted[16];
    s16 output[8];
    u8 *text;
    s32 index;
    u32 cell;
    u8 *base;
    s32 vram;
    s32 src;
    s32 dst;
    s32 phase;

    base = gIw;
    text = Text_FormatNumber(formatted, value, 4);
    if (variant == 0) {
        output[0] = 0xf01d;
    } else {
        output[0] = 0xf01f;
    }
    output[1] = 0xf01e;
    for (index = 0; index <= 4; index++) {
        output[index + 2] = text[index];
    }
    output[6] = 0;

    cell = ((work->y + ((u32)offset_y >> 3) + 1) << 5)
        + (work->x + ((u32)offset_x >> 3)) + 1;
    if (cell < 0x280) {
        vram = 0x06002000;
        cell *= 2;
        dst = vram + cell;
        src = (s32)base + cell;
        phase = offset_x & 7;
        Ui_SetMode(
            output,
            src,
            dst,
            phase);
    }
}

/* ui/render/create_output.c */
#if defined(GS1_EDITION_DE)
#define TABLE_ADDR 0x03001B20
#else
#define TABLE_ADDR 0x03001B10
#endif

void *RenderOutput_AcquireFree(void);
void Resource_ResetEntry(u32);
s32 RenderOutput_AppendToList(void *, void *);

struct TableEntry {
    u16 unused;
    u16 value;
};

struct RenderOutput *RenderOutput_Create(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 x;
    struct RenderOutput *output;
    s32 y;

    output = RenderOutput_AcquireFree();
    if (output == NULL) {
        Resource_ResetEntry((u32)arg0);
        return 0;
    }
    x = arg3 + (arg2->x * 8) + 8;
    y = arg4 + (arg2->y * 8) + 8;
    x &= 0x1ff;
    y &= 0xff;
    /* Xをbit16～24、Yをbit0～7へ置き、arg1のフラグを重ねる。 */
    output->packed = (x << 16) | y | arg1;
    output->table.value =
        ((struct TableEntry *)TABLE_ADDR)[arg0].value >> 5;
    output->sentinel = 0xff;
    output->zero = 0;
    output->x = x;
    output->y = (s16)y;
    output->index = (s8)arg0;
    output->one4 = 1;
    output->one5 = 1;
    RenderOutput_AppendToList(arg2, output);
    return output;
}

/* ui/render/create_output_from_resource.c */
void *RenderOutput_CreateFromResource(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 no;

    no = Ui_Apply(arg0, arg1);
    if (no < 0) {
        return NULL;
    }
    return RenderOutput_Create(no, 0x40000000, arg2, arg3, arg4);
}

/* ui/render/create_output_loaded.c */
s32 Resource_FindFreeSlot();

void *RenderOutput_CreateLoaded(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 no;
    void *result;

    no = Resource_FindFreeSlot();
    result = NULL;
    if (no != 0x60) {
        Ui_Place(arg0, arg1, no);
        result = RenderOutput_Create(no, 0x40000000, arg2, arg3, arg4);
    }
    return result;
}

/* ui/render/create_output_with_transform.c */
s32 Resource_FindFreeSlot(void);

void *RenderOutput_CreateWithTransform(
    s32 arg0,
    struct RenderInput *arg1,
    s32 arg2,
    s32 arg3)
{
    s32 count;
    s32 unused;
    u8 *result;

    count = Resource_FindFreeSlot();
    if (count == 0x60) {
        return NULL;
    }
    Ui_SetRange(arg0, 1, &count, &unused, 1);
    result = RenderOutput_Create(count, 0x40000000, arg1, arg2, arg3);
    result[15] = 251;
    return result;
}

/* ui/render/create_output_from_table.c */
s32 Resource_FindFreeSlot();
s32 RenderResource_LoadTableEntry(u32 value, s32 unused, void *destination);
void *RenderOutput_CreateFromTable(
    s32 table_entry,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 slot;
    void *output;

    slot = Resource_FindFreeSlot();
    output = NULL;
    if (slot != 0x60) {
        RenderResource_LoadTableEntry(table_entry, 0, slot);
        output = RenderOutput_Create(slot, 0x40000000, input, x, y);
    }
    return output;
}
