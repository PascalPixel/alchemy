#include "TEXT_RENDER_RUNTIME.H"
#include "RUNTIME_MEM.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

extern u8 *gWindowWork;

s32 UiText_BuildRenderEntries(s32 character, s32 count);
s16 *Runtime_BumpAllocateAlternatePool(s32 size);
u8 *UiText_FormatNumber(u8 *output, s32 value, s32 width);
void UiText_RenderWideStringAtOffset(void *text, s32 work, s32 x, s32 y);
void UiText_RenderWideStringInWindow(u16 *text, s32 work, s32 x, s32 y);
s32 UiText_RenderStringTiles(void *text, s32 source, s32 destination, s32 phase);

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
    UiText_RenderWideStringAtOffset(base + RENDER_ENTRY_TBL_OFS, arg1, arg2, arg3);
}

void UiText_DrawCharacterAtOffset(
    s32 character,
    struct TextRenderWork *position,
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

    base = gWindowWork;
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
        UiText_RenderStringTiles(
            text,
            (s32)(base + byte_offset),
            vram_address,
            7 & offset_x);
    }
}

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
    UiText_RenderWideStringAtOffset(buffer, arg1, arg2, arg3);
    Runtime_BumpFree(buffer);
}

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
    base = gWindowWork;
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
        UiText_RenderStringTiles(
            buffer,
            src,
            dst,
            phase);
        Runtime_BumpFree(buffer);
    }
}

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
    UiText_RenderWideStringInWindow(buffer, arg1, x, y);
    Runtime_BumpFree(buffer);
}

void UiText_DrawNumber(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    /* 16バイト一時領域を介して次の処理へ渡す。 */
    UiText_DrawString(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

void UiText_DrawNumberAtOffset(
    s32 arg0,
    s32 arg1,
    struct TextRenderWork *arg2,
    s32 arg3,
    s32 arg4)
{
    u8 data[16];

    /* 16バイト一時領域を介して次の処理へ渡す。 */
    UiText_DrawStringAtOffset(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

void UiText_DrawNumberInWindow(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    /* 16バイト一時領域を介して次の処理へ渡す。 */
    UiText_DrawStringInWindow(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

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

    base = gWindowWork;
    text = UiText_FormatNumber(formatted, value, 4);
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
        UiText_RenderStringTiles(
            output,
            src,
            dst,
            phase);
    }
}
