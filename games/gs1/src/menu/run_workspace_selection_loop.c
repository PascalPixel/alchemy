#include "types.h"
#include "scene.h"
#include "gs1_edition.h"
#include "global_cells.h"
#include "text_render_runtime.h"
#include "render_input.h"
#include "sound_ids.h"

/* menu/misc/run_workspace_selection_loop.c */
/* menu/misc/run_workspace_selection_loop.c */
/* Owner-local field access until this runtime workspace layout is recovered
 * elsewhere; other menu owners reach the same 0x03001EA0 pointer. */

extern void *gIw;
extern u8 gIw2;
extern s8 gRom[];

void *Menu_Run(void);

void *create_pair(s32, void *, s32, s32);

s32 Menu_RunWorkspaceSelectionLoop(void)
{
    void *sp10;
    s32 spC;
    s32 sp8;
    s32 sp4;
    s32 i;
    s32 j;
    s32 var_fp_21;
    s32 temp_r0_22;
    s32 var_r2_101;
    s32 var_r4_35;
    s32 var_r5_144;
    s32 var_r8_51;
    s32 temp_e;
    s32 temp_c;
    s32 temp_e2;
    s8 *tbl;
    void *temp_r5_90;
    void *temp_sl_29;

    spC = 1;
    var_fp_21 = 3;
    temp_r0_22 = GameFlag_IsSet(0x17E);
    sp4 = 0;
    Menu_PrepareWorkspacePage();
    temp_sl_29 = gIw;
    sp10 = Menu_Run();
    var_r4_35 = -0x18;
    if (gIw2 != 0) {
        var_r4_35 = -0x10;
    }
    sp8 = (s32)create_pair(6, sp10, 0x28, var_r4_35);
    WaitFrames(1);
    var_r8_51 = FIELD_AT_OFFSET(temp_sl_29, u16 *, 0x574);
    if (temp_r0_22 != 0) {
        var_fp_21 = 1;
        sp4 = 2;
    }
    if (gIw2 != 0) {
        var_fp_21 += 3;
    }
loop_6:
    if (spC != 0) {
        spC = 0;
        var_r8_51 = Modulo(var_r8_51 + var_fp_21, var_fp_21);
        FIELD_AT_OFFSET(temp_sl_29, u16 *, 0x574) = var_r8_51;
        i = 0;
        if (i < var_fp_21) {
            j = sp4;
            tbl = gRom;
            do {
                temp_r5_90 = ((void **)((u8 *)temp_sl_29 + 0x610))[i];
                FIELD_AT_OFFSET(temp_r5_90, u8 *, 0xF) = 0xFB;
                UiIcon_PrepareObject(temp_r5_90);
                temp_e = FIELD_AT_OFFSET(temp_r5_90, u8 *, 0xE);
                var_r2_101 = 0;
                if (i != FIELD_AT_OFFSET(temp_sl_29, u16 *, 0x574)) {
                    var_r2_101 = 1;
                }
                RenderResource_LoadFrame(*(s8 *)(j + (s32)tbl), temp_e, var_r2_101);
                i++;
                j++;
            } while (i < var_fp_21);
        }
        temp_e2 = FIELD_AT_OFFSET(sp10, u16 *, 0xE);
        temp_c = FIELD_AT_OFFSET(sp10, u16 *, 0xC) * 8;
        var_r4_35 = (((var_r8_51 * 3) + temp_e2) * 8) + 0x10;
        Menu_SetMode(
            (u8 *)temp_sl_29 + 0x5A4,
            temp_c,
            var_r4_35,
            3);
    }
    Ui_ApplyTableOffsetToPair((void *)sp8);
    WaitFrames(1);
    if (*(volatile s32 *)0x03001C94 & 1) {
        var_r5_144 = var_r8_51;
        Audio_PlayCue(SOUND_MENU_CONFIRM);
    } else if (*(volatile s32 *)0x03001C94 & 0xA) {
        var_r5_144 = -1;
        Audio_PlayCue(SOUND_MENU_CANCEL);
    } else {
        if (*(volatile s32 *)0x03001B04 & 0x40) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            var_r8_51 -= 1;
            spC = 1;
        } else if (*(volatile s32 *)0x03001B04 & 0x80) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            var_r8_51 += 1;
            spC = 1;
        }
        goto loop_6;
    }
    UiWork_Finalize(sp10, 2);
    Runtime_ScheduleCallbackAndReleaseBlock20A();
    WaitFrames(1);
    if (var_r5_144 >= 0) {
        var_r5_144 += sp4;
    }
    return var_r5_144;
}

/* ui/window/clear_tile_attributes_in_rect.c */
extern u8 *gIwAttributesInRect;

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height)
{
    u8 *base = gIwAttributesInRect;
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
    u32 alternate = base[RENDER_ALT_OFS];
    u32 row;

    for (row = 0; row < height; row++) {
        u32 column;

        for (column = 0; column < width; column++) {
            u32 tile = *cursor++ & 0x3FF;

            if ((tile - 0x80) <= 0x7F ||
                (alternate != 0 &&
                 tile > 0x1FF &&
                 tile <= 0x27F)) {
                u32 index = ((tile & 0xFF) ^ 0x80) + RENDER_TILE_ATTR_OFS;
                base[index] &= 0xFC;
            }
        }
        cursor += 32 - width;
    }
}

/* ui/runtime/set/set_alt_flag_and_clear_table.c */
void UiWork_SetAltFlagAndClearTable(s32 flag)
{
    s32 i;
    s32 j;
    s8 *p;
    s8 *q;
    void *work;

    work = *(void **)ADDR_03001E8C;
    if (flag != 0) {
        FIELD_AT_OFFSET(work, s8 *, 0xEA2) = 1;
        flag = 0;
        for (i = 0x80, p = work + 0xE20; i <= 0xFF; i += 1) {
            *p = flag;
            p += 1;
        }
        return;
    }
    FIELD_AT_OFFSET(work, s8 *, 0xEA2) = 0;
    flag = 0;
    q = work + 0xE20;
    j = 0x7F;
    do {
        j -= 1;
        *q = flag;
        q += 1;
    } while (j >= 0);
}

/* ui/render/reserved_no_op.c */
void UiRender_ReservedNoOpE418(void)
{
}

/* ui/work_set_param_nibble.c */
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
s32 UiText_BuildRenderEntries(s32, s32);

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
    FunctionHead_08017aa4(base + RENDER_ENTRY_TBL_OFS, arg1, arg2, arg3);
}

/* ui/text/draw/draw_character.c */
struct TextPosition_0801e7c0 {
    u8 padding[12];
    u16 x;
    u16 y;
};

extern u8 *gIwSetParamNibble;

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

    base = gIwSetParamNibble;
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
        FunctionHead_0801de5c(
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
    FunctionHead_08017c8c(buffer, arg1, arg2, arg3);
    Ui_Check(buffer);
}

/* ui/text/draw/draw_string_at_offset.c */


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
    base = gIwSetParamNibble;
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
        FunctionHead_08017c8c(
            buffer,
            src,
            dst,
            phase);
        Ui_Check(buffer);
    }
}

/* ui/text/draw/draw_string_in_window.c */

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
    FunctionHead_08017c8c(buffer, arg1, x, y);
    Ui_Do(buffer);
}

/* ui/text/draw/draw_number.c */
u8 *Text_FormatNumber(u8 *output, s32 value, s32 width);
void UiText_DrawString(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);

void UiText_DrawNumber(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    /* 16バイト一時領域を介して次の処理へ渡す。 */
    UiText_DrawString(Text_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

/* ui/text/draw/draw_number_at_offset.c */

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
void UiText_DrawStringInWindow(u8 *s, s32 arg1, u32 arg2, u32 arg3);

void UiText_DrawNumberInWindow(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    /* 16バイト一時領域を介して次の処理へ渡す。 */
    UiText_DrawStringInWindow(Text_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

/* ui/text/draw/draw_prefixed_number_at_offset.c */


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

    base = gIwSetParamNibble;
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
        FunctionHead_08017c8c(
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
s32 Resource_FindFreeSlot(void);

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
