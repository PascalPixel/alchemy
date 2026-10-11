#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "types.h"
#include "io_reg.h"
#include "window.h"
#include "scene.h"
#include "render_input.h"
#include "resource.h"
#include "tbs_edition.h"
#include "text/msg_ids.h"
#include "vram_block.h"
#include "glyph.h"
#include "affine.h"
#include "uiwindow.h"
#include "resource_ids.h"
#include "iwram_call.h"
#include "global_cells.h"
#include "b5_context.h"
#include "btlsys.h"
#include "lib.h"
#include "dma.h"
#include "ramcode.h"
#include "heap_state.h"
#include "text_render_runtime.h"
#include "irq.h"
#include "fixed_math.h"
#include "io_write_queue.h"
#include "uiflow.h"
#include "menu_list.h"
#include "map_scroll.h"
#include "btlwork.h"
#include "character_menu.h"
#include "battle_unit.h"
#include "vram_tab.h"
#include "edition.h"
#include "djinn_preview.h"
#include "ui.h"
#include "motobj.h"
#include "btlpty.h"
#include "item.h"
#include "btltype.h"
#include "select.h"
#include "effect_step.h"
#include "sound_ids.h"
#include "game_state.h"
#include "resmenu.h"
#include "btlsmn.h"
#include "serial_runtime.h"
#include "save_state.h"

s32 BattleTarget_RunSelection(s32 sel, s32 mode, u32 spread, u32 kind);
void Graphics_ExpandVramTilesByColorTable(u16 *dst);
s32 Menu_BuildLocalizedPatternTiles(void);
void Menu_CenterResourceEntries(s32, s32, s32);
s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index);
s32 Ui_LoadEntryForKind(u32 kind, s32 value);
s32 Ui_RunOwnerStatusScreen(u16 *list, s32 listCount, s32 owner);

u32 Resource_DecodeByteLz(const void *, void *);

extern const u8 *RenderResource_PairSourceTable[];

void RenderResource_LoadPair(s32 group_index, s32 resource_index)
{
    void *staging_buffer = (void *)Runtime_AllocateBlock(14, 0x400);
    const u8 *resource = RenderResource_PairSourceTable[group_index];

    if (resource_index <= 0x5F) {
        Resource_DecodeByteLz(resource, staging_buffer);
        VramBlock_LoadCached(resource_index, 0x200, staging_buffer);
        Runtime_ReleaseHeapBlock(14);
    }
}

struct RenderOutput *RenderResource_CreatePair(
    s32 group,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 index;
    struct RenderOutput *first;
    struct RenderOutput *second;

    index = Resource_FindFreeEntry();
    if (index > 95)
        return NULL;

    RenderResource_LoadPair(group, index);
    first = RenderOutput_Create(index, 0x80004000, input, x, y);
    first->sentinel = 0xFD;
    second = RenderOutput_Create(index, 0x80004000, input, x + 32, y);
    second->sentinel = 0xFD;
    second->oam.index += 8;
    return first;
}

struct UiBobPairOutput {
    struct UiBobPairOutput *next;
    u8 unknown_04[4];
    u8 source;
    u8 unknown_09[11];
    u8 destination;
};

extern volatile u32 gFrameTick;
extern volatile u32 gFrameCount;

struct UiScaleSprite {
    u8 filler0[6];
    u16 src_6;
    u8 src_8;
    u8 filler9[6];
    u8 out_15;
    u8 filler16[4];
    u8 out_20;
    u8 mode_21 : 2;
    u8 rest_21 : 6;
    u16 pos_22 : 9;
    u16 affine_22 : 5;
    u16 rest_22 : 2;
};

static __inline__ u32 XorWord(u32 word, u32 mask)
{
    return word ^ mask;
}

void Ui_ApplyTableOffsetToPair(struct UiBobPairOutput *obj)
{
    s32 value;

    value = obj->source + Ui_PairBobOffsets[(gFrameTick >> 2) & 7];
    obj->destination = value;
    obj = obj->next;
    value = obj->source + Ui_PairBobOffsets[(gFrameTick >> 2) & 7];
    obj->destination = value;
}

void RenderResource_LoadFrame(s32 index, s32 value, s32 flag)
{
    s32 size = 1024;
    void *buffer = Runtime_AllocateBlock(14, size);
    u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

    if (value <= 95) {
        Resource_DecodeByteLz((void *)((u32)base + base[index]), buffer);
        if (flag != 0)
            Runtime_RemapBytesByTableFar(buffer, 768);
        VramBlock_LoadCached(value, size, buffer);
        Runtime_ReleaseHeapBlock(14);
    }
}

struct RenderOutput *RenderResource_CreateFrame(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 index;
    struct RenderOutput *entity;

    index = Resource_FindFreeEntry();
    entity = NULL;
    if (index != 0x60) {
        RenderResource_LoadFrame(arg0, index, arg1);
        entity = RenderOutput_Create(index, 0x80000000, arg2, arg3, arg4);
        entity->oam.palette_256 = 1;
        entity->sentinel = 0xfb;
    }
    return entity;
}

void Ui_ApplyTableScaleToObject(struct UiScaleSprite *obj)
{
    s32 v = Ui_ObjectPulseScales[(gFrameTick >> 1) & 7];
    struct AffineTransform efx;

    if (v < 0)
        v += 255;
    v >>= 8;

    if (obj != 0) {
        efx.scale_x = v;
        efx.scale_y = v;
        efx.angle = 0;
        obj->affine_22 = AffineMatrix_BuildForEffect(&efx);
        obj->mode_21 = 3;
        obj->pos_22 = obj->src_6 + 0xfff0;
        obj->out_20 = obj->src_8 + 0xf0;
        obj->out_15 = 0xfc;
    }
}

s32 Menu_BuildLocalizedPatternTiles(void)
{
    u32 *vram = (u32 *)0x06006280;
    s32 set;
    s32 n;

    for (set = 0; set < 2; set++) {
        for (n = 0; n < 6; n++) {
            u32 *tile = vram + set * 0x60 + n * 0x10;
            s32 x;

            Iwram_FillWords(tile, 64, 0x44444444);
            for (x = 1; x <= 7; x++) {
                s32 mi = n;

                if (set == 1 && x <= 1) {
                    continue;
                }
                if (set == 0 && n > x - 2) {
                    mi = x - 2;
                    if (mi < 0) {
                        mi = 0;
                    }
                }
                tile[x] = XorWord(tile[x], Menu_BarPatternRows[mi].word0);
                tile[x + 8] = XorWord(tile[x + 8], Menu_BarPatternRows[mi].word1);
            }
        }
    }
}

int UiWindow_DrawThreeTileColumn(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    // 値を返さない int 関数 (pop {r1}; bx r1)
    s32 tile_offset = arg3 * 2;
    s32 tile = tile_offset + 0xF315;

    UiWindow_SetTilemapEntry((struct UiWindow *)arg0, 0x400 | tile, arg1, arg2, 0);
    UiWindow_SetTilemapEntry((struct UiWindow *)arg0, tile_offset + 0xF314, arg1 + 1, arg2, 0);
    UiWindow_SetTilemapEntry((struct UiWindow *)arg0, tile, arg1 + 2, arg2, 0);
}

void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output, s32 shift)
{
    s32 neg;
    s32 rshift;
    s32 lshift;
    s32 i;

    neg = -shift;
    i = 0;
    rshift = neg * 4;
    lshift = shift * 4;

    do {
        u32 result;
        u32 shifted;
        u32 fallback;
        s32 j;

        shifted = *second++;
        result = 0;
        fallback = *first++;
        if (shift < 0)
            shifted >>= rshift;
        else
            shifted <<= lshift;

        j = 7;
        do {
            result <<= 4;
            if (shifted > 0x0FFFFFFF)
                result += shifted >> 28;
            else
                result += fallback >> 28;
            j--;
            shifted <<= 4;
            fallback <<= 4;
        } while (j >= 0);

        *output++ = result;
        i++;
    } while (i <= 7);
}

void Link_DrawShiftedTilePair(s32 offset)
{
    s32 phase = (gFrameCount >> 2) & 3;

    if (phase > 2) {
        phase = 2;
    }
    if (phase <= 0) {
        phase = 1;
    }
    phase = phase + 1;
    Graphics_MergeShiftedTileRows((u32 *)0x06000220,
        (u32 *)gRomShiftedTilePair, (u32 *)offset, -phase);
    Graphics_MergeShiftedTileRows((u32 *)0x06000240,
        (u32 *)(gRomShiftedTilePair + 32), (u32 *)(offset + 32), phase);
}

void Graphics_ExpandVramTilesByColorTable(u16 *dst)
{
    s32 bank = 0;
    s32 pal_ofs = 0;
    s32 dst_bank = 0;

    do {
        s32 row = 0;
        s32 dst_ofs = (dst_bank + bank) << 6;

        do {
            u16 *out = (u16 *)((u8 *)dst + dst_ofs);
            u16 *src = (u16 *)((u8 *)0x06000600 + (row << 5));
            s32 col = 0;

            do {
                u32 packed = *src++;
                u32 dec = 0;
                s32 nibble = 0;

                do {
                    u32 color = Graphics_ExpandNibbleTable[
                        (packed & 15) + pal_ofs
                    ] << (nibble * 4);
                    nibble++;
                    packed >>= 4;
                    dec |= color;
                } while (nibble <= 3);

                col++;
                *out++ = dec;
            } while (col <= 15);

            row++;
            dst_ofs += 32;
        } while (row <= 9);

        pal_ofs += 16;
        dst_bank += 4;
        bank++;
    } while (bank <= 1);
}

s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    GlyphTransfer *work;

    work = Runtime_AllocateBlock(17, sizeof(*work));
    Ui_PrepareTransferFromTableEntry(resource);
    result = Resource_GetBuffer(index, (s32)work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
    return result;
}

s32 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    GlyphTransfer *work;

    work = Runtime_AllocateBlock(17, sizeof(*work));
    ItemIcon_Compose(resource, 0x1a);
    result = Resource_GetBuffer(index, (s32)work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
    return result;
}

s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index)
{
    s32 out;
    s32 cur;
    s32 ret;
    GlyphTransfer *work;

    work = Runtime_AllocateBlock(17, sizeof(*work));
    cur = index;
    Ability_LoadGlyph(resource, 0, &cur, &out, 1);
    ret = Resource_GetBuffer(index, (s32)work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
    return ret;
}

s32 Ui_LoadEntryForKind(u32 kind, s32 value)
{
    s32 out;
    s32 cur;
    u32 no;

    no = kind;
    cur = value;
    if (no > 7U) {
        no = 0;
    }
    if (GameFlag_TestFar(0x20) != 0) {
        switch (no) {
        case 0:
            no = 0x38;
            break;
        case 1:
            no = 0x39;
            break;
        }
    }
    UiGlyph_LoadEntryWithPalette(no, 0, &cur, &out, 0xE, 1);
    return out;
}

//--------------------------------------------------------------------------

// get_table_word_zero.c

// ウィンドウ：固定のタイルの表の先頭を返す

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Ui_FixedTileBlocks[0] を返す（0 以外の no も 0 として読む）
//--------------------------------------------------------------------------
s32 Ui_GetTableWordZero(s32 no)
{
	if (no != 0)
		no = 0;
	return Ui_FixedTileBlocks[no];
}

void Ui_RunCopiedRoutine(GlyphTransfer *work)
{

    RamCode_LoadSlot(49, Tile_Decompress4bpp, Tile_Decompress4bppCodeSize);
    ((void (*)(s32, u8 *))((union HeapState *)gWorkSlot)->slots[49])(
        (s32)work->encoded, (u8 *)work);
    Runtime_ReleaseHeapBlock(49);
}

//--------------------------------------------------------------------------

// create_countdown_label_window.c

// 通信：残り時間の見出しのウィンドウを開く

//--------------------------------------------------------------------------

#define UiText_DrawStringAtOffset ((void (*)(u8 *, s32, s32, s32))UiText_DrawStringAtOffset)

void UiText_DrawStringInWindow(u8 *s, s32 arg1, u32 arg2, u32 arg3);

//--------------------------------------------------------------------------
// 左上に 6x4 のウィンドウを開いて見出しを書き、そのハンドルを返す
//--------------------------------------------------------------------------
s32 Link_CreateCountdownLabelWindow(void)
{
	s32 win = UiWindow_Create(0, 0, 6, 4, 6);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TLA_EDITION_ES) \
	|| defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
	UiText_DrawStringAtOffset(Link_TimeLabelString, win, 0, 0);
#else
	UiText_DrawStringInWindow(Link_TimeLabelString, win, 0, 0);
#endif
	return win;
}

typedef struct
{
    u16 code : 10;
    u16 style : 6;
} CharacterCell;

s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1)
{
    GlyphTransfer *buffer = Runtime_AllocateHeapBlock(0x11, sizeof(GlyphTransfer));
    u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
    u8 **slot = &buffer->encoded;
    void *target = (void *)((u32)base + base[arg1]);
    s32 ret;

    *slot = target;
    Resource_DecodeByteLz(target, buffer);
    ret = Resource_GetBuffer(arg0, (s32)buffer);
    Runtime_ReleaseHeapBlock(0x11);
    return ret;
}

void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph, s32 tile)
{
    GlyphTransfer *decoded;
    u8 *font;
    u8 *remapped;
    u8 *src;
    u8 *dst;
    s32 i;
    u32 index;

    decoded = (GlyphTransfer *)Runtime_AllocateHeapBlock(17, sizeof(*decoded));
    font = Resource_GetTableEntry((u32)&ResourceId_CommandIcons);
    decoded->encoded = font + ((u16 *)font)[glyph];
    Resource_DecodeByteLz(decoded->encoded, decoded);
    remapped = MemAlloc(0x400);
    src = decoded->input;
    dst = remapped;
    for (i = 0; i < 0x400; i++) {
        index = *src++;
        if (palette->slot[index] == 255) {
            palette->slot[index] = palette->count;
            if (palette->count < 64) {
                ((u16 *)0x05000000)[palette->count] = ((u16 *)0x05000200)[index];
                palette->count++;
            }
        }
        *dst++ = palette->slot[index];
    }
    Dma_Set(remapped, (void *)(0x06004000 + tile * 64), 0x84000100, REG_DMA3);
    MemFree(remapped);
    Runtime_ReleaseHeapBlock(17);
}

struct SelectionEntry {
    struct MenuSprite sprite;
    u16 resource, unused;
    s32 pos_x, pos_y;
    s32 kind;
};

struct SelectionWork {
    struct GlyphPalette palette;
    struct SelectionEntry entries[6];
    struct UiWindow *window;
    s32 count;
    struct AffineTransform affine;
    s32 matrix, selection, previous;
    s32 kinds[6];
};

void UiText_DrawCharacter(struct SelectionWork *work, s32 index, u32 kind)
{
    struct SelectionEntry *entry = &work->entries[index];

    UiText_LoadRemappedGlyph(&work->palette, kind, index * 16);
    work->entries[index].kind = kind;
    MenuSprite_Init(entry->sprite, 0x80002000);
    entry->sprite.oam.f.tile = Resource_LoadIndexedIntoBuffer(work->entries[index].resource, kind);
}

s32 UiWork_IsComplete(void);
struct UiWindow *UiText_OpenMessageWindow(s32 message, s32 x, s32 y, u32 packed);

void Graphics_SetBg1Priority3(void)
{
    u32 value = REG_BG1CNT;

    value &= ~3;
    value |= 3;
    REG_BG1CNT = value;
}

void Graphics_ClearBg1ControlBit2(void)
{
    u32 value = REG_BG1CNT;

    value &= ~3;
    REG_BG1CNT = value;
}

void Graphics_InstallBg1VCountHandler(void)
{
    u32 address = 0x04000012;
    s32 zero = 0;

    *(volatile u16 *)address = zero;
    /* FAKEMATCH: needed to match: the do-while keeps the handler load ahead of the index. */
    do { address = (u32)Graphics_ClearBg1ControlBit2; } while (0);
    Runtime_SetIrqHandler(2, 136, (InterruptHandler)address);
}

struct UiWindow *UiText_ShowMessageAndWaitComplete(s32 message, s32 x, s32 y)
{
    struct UiWindow *result;

    result = UiText_OpenMessageWindow(message, x, y, 1);
    while (UiWork_IsComplete() == 0)
        WaitFrames(1);
    return result;
}

#if EDITION_INTERNATIONAL
#define MADO_X 22 /* 窓の X */
#else
#define MADO_X 23
#endif

void RenderOutput_PrepareForRedraw(struct UiWindow *work);
s32 BattlePresentation_WaitForPromptAtFar(s32 x, s32 y);
void AudioCommand_PlayFar(s32 cue);

s32 Ui_RunSelectionScreen(s32 mode)
{
	s32 *Trade_GetOfferStateFar(s32 index);
    u8 party[4];
    struct UiRenderWork *render;
    struct SelectionWork *work;
    struct MenuCell *runtime;
    s32 rebuild, trade_count, scale, result;
    s32 cnt;
    s32 keys, repeat;
    s32 slot;
    s32 i, num;

    render = (struct UiRenderWork *)gWindowWork[0];
    slot = Resource_LoadIntoFreeSlot(0x400);
    rebuild = 1;
    trade_count = 0;
    work = Runtime_BumpAllocateAlternatePool(sizeof(*work));
    for (i = 255; i >= 0; i--)
        work->palette.slot[i] = 255;
    work->palette.count = 1;
    if (mode == 0)
        work->window = UiWindow_Create(20, 17, 10, 3, 6);
    else
        work->window = UiWindow_Create(MADO_X, 17, 30 - MADO_X, 3, 6);
    work->previous = -1;
    if (mode == 0) {
        num = 0;
        work->kinds[num++] = 14;
        work->kinds[num++] = 4;
        work->kinds[num++] = 7;
        work->kinds[num] = -1;
        work->selection = gLinkCountdownWork->command_selection[0];
    } else {
        work->selection = gLinkCountdownWork->command_selection[1];
        num = 0;
        work->kinds[num++] = 0;
        work->kinds[num++] = 1;
        if (*Trade_GetOfferStateFar(0) != 0) {
            BattlePlacement_CountValidEntriesFar(0, party);
            for (i = 0; i <= 3; i++)
                trade_count += party[i];
            work->kinds[num++] = 15;
            if (trade_count != 0)
                work->kinds[num++] = 16;
        }
        work->kinds[num++] = 2;
        work->kinds[num++] = 3;
        work->kinds[num] = -1;
    }
    for (cnt = 0; cnt <= 5; cnt++) {
        work->entries[cnt].resource = slot;
        if (work->kinds[cnt] == -1)
            break;
        UiText_DrawCharacter(work, cnt, work->kinds[cnt]);
    }
    work->count = cnt;
    work->affine.scale_x = 320;
    work->affine.scale_y = 304;
    work->affine.angle = 0;
    cnt = 0;
    if (cnt < work->count) {
        do {
            s32 x = (cnt - work->count) * 24;
            work->entries[cnt].pos_x = x + 155;
            if (mode != 0)
                work->entries[cnt].pos_x = x + MADO_X * 8 - 5;
            work->entries[cnt].pos_y = 136;
            cnt++;
        } while (cnt < work->count);
    }
    Scheduler_AddOrUpdateCallback(Graphics_SetBg1Priority3, 0x480);
    Runtime_SetIrqHandler(2, 136, Graphics_ClearBg1ControlBit2);

    for (;;) {
        scale = Ui_BounceScales[(gFrameCount * 2) & 31];
        scale = (scale - 256) / 4 + 304;
        work->affine.scale_y = work->affine.scale_x = scale;
        gBgScroll[1].x = 0;
        gBgScroll[1].y = 32;
        if (rebuild != 0) {
            rebuild = 0;
            render->menu_busy = 1;
            if (mode) {
                s32 panel;
                s32 count;
                for (panel = 0; panel < 6 - work->count; panel++) {
                    for (cnt = 0; cnt <= 2; cnt++) {
                        s32 col;
                        for (col = 0; col <= 2; col++)
                            *(render->tilemap + (cnt + 17) * 32 + panel * 3 + MADO_X - 18 + (col & 3)) = 0xf07f;
                    }
                }
                for (panel = 0; panel < work->count; panel++) {
                    count = work->count;
                    for (cnt = 0; cnt <= 2; cnt++) {
                        s32 col;
                        for (col = 0; col <= 2; col++) {
                            s32 tmp;
                            /* FAKEMATCH: vram, ofs and mask restore two forms the bytes prove. Set ahead of the index, they are three more moves out of the column loop in loop.c's first pass, which drops its threshold so the tile pattern's row part stays in the column loop until the second pass: the row loop reduces the index giv first and copies it first (inits index r6, pattern r5; per row mov ip, r6 and str r5, [sp, #4]). volatile keeps the VRAM base add apart from the store it addresses. Without the three 24, without ofs 564, without mask 470, without volatile 112. */
                            volatile u16 *vram = (volatile u16 *)0x0600f800 + 21 * 32 + MADO_X;
                            s32 ofs = 0x100;
                            s32 mask = 3;
                            /* FAKEMATCH: needed to match: tmp, set twice (the tile, then the column's index), is no giv, so the tile is recomputed in the column loop as the bytes show (adds r3, r7, r0; adds r3, r3, r7); 267 differ without it, 846 in both modes. */
                            vram[panel * 3 + cnt * 32 + (col & mask) - count * 3] = tmp = panel * 16 + cnt * 4 + col + ofs;
                            tmp = col & mask;
                            *(render->tilemap + (cnt + 17) * 32 + panel * 3 + tmp - count * 3 + MADO_X) = 0;
                        }
                    }
                }
            } else {
                s32 panel;
                s32 count;
                for (panel = 0; panel < 6 - work->count; panel++) {
                    for (cnt = 0; cnt <= 2; cnt++) {
                        s32 col;
                        for (col = 0; col <= 2; col++)
                            *(render->tilemap + (cnt + 17) * 32 + panel * 3 + 2 + (col & 3)) = 0xf07f;
                    }
                }
                for (panel = 0; panel < work->count; panel++) {
                    do { } while (0); /* FAKEMATCH: needed to match: the empty loop restores the CSE block boundary at the panel body's head, so the exit test compares count instead of reloading work->count (223 differ without it). */
                    count = work->count;
                    for (cnt = 0; cnt <= 2; cnt++) {
                        s32 col;
                        for (col = 0; col <= 2; col++) {
                            s32 tmp;
                            /* FAKEMATCH: as in mode 1: vram, ofs and mask keep the tile pattern in the column loop through loop.c's first pass, volatile keeps the VRAM add apart from its store. */
                            volatile u16 *vram = (volatile u16 *)0x0600f800 + 21 * 32 + 20;
                            s32 ofs = 0x100;
                            s32 mask = 3;
                            /* FAKEMATCH: needed to match: tmp, as in mode 1 (271 differ without it, 846 in both modes). */
                            vram[panel * 3 + cnt * 32 + (col & mask) - count * 3] = tmp = panel * 16 + cnt * 4 + col + ofs;
                            tmp = col & mask;
                            *(render->tilemap + (cnt + 17) * 32 + panel * 3 + tmp - count * 3 + 20) = 0;
                        }
                    }
                }
            }
        }
        if (work->previous != work->selection) {
            RenderOutput_PrepareForRedraw(work->window);
            UiText_DrawCharacterAtOffset(work->entries[work->selection].kind + (s32)&MsgCommandName,
                                         (struct RenderInput *)work->window, 0, 0);
            work->previous = work->selection;
            UiText_DrawCharacter(work, work->selection, work->kinds[work->selection]);
        }
        work->matrix = AffineMatrix_BuildForEffect(&work->affine);
        for (cnt = 0; cnt < work->count; cnt++) {
            struct SelectionEntry *entry = &work->entries[cnt];
            if (cnt == work->selection) {
                entry->sprite.oam.f.affine_index = work->matrix;
                entry->sprite.oam.f.affine = 3;
                entry->sprite.oam.f.x = entry->pos_x + scale * 7 / 512 - 14;
                entry->sprite.oam.f.y = entry->pos_y + scale * 3 / 256 - 20;
                Runtime_PushSlotEntry(entry, 0xf1);
            } else {
                entry->sprite.oam.f.x = entry->pos_x;
                entry->sprite.oam.f.y = entry->pos_y;
                entry->sprite.oam.f.affine_index = 0;
                entry->sprite.oam.f.affine = 0;
            }
        }
        keys = gKeyState;
        repeat = gKeysRepeat;
        runtime = gLinkCountdownWork;
        if (runtime->auto_enabled != 0) {
            keys = repeat = 0;
            if (runtime->auto_delay == 0) {
                if (runtime->auto_mode == 1) {
                    if (work->kinds[work->selection] == 3)
                        keys = repeat = 1;
                    else
                        keys = repeat = 32;
                    runtime->auto_delay = 30;
                } else if (runtime->auto_mode == 0) {
                    s32 kind = work->kinds[work->selection];
                    if (kind == 16 || (trade_count == 0 && kind == 15)) {
                        if (runtime->auto_enabled == 1) {
                            struct UiWindow *message;
                            if (kind == 15)
                                message = UiText_ShowMessageAndWaitComplete((s32)&MsgDjinnTutorialUnleashCommand, 15, 8);
                            else if (kind == 16)
                                message = UiText_ShowMessageAndWaitComplete((s32)&MsgDjinnTutorialSummon, 15, 8);
                            BattlePresentation_WaitForPromptAtFar(102, 155);
                            UiWork_Finalize(message, 1);
                            runtime->auto_enabled++;
                            runtime->auto_delay = 45;
                        } else {
                            keys = repeat = 1;
                            runtime->auto_delay = 200;
                        }
                    } else {
                        keys = repeat = 16;
                        runtime->auto_delay = 40;
                    }
                } else {
                    runtime->auto_delay = 60;
                    keys = repeat = 1;
                }
            } else {
                runtime->auto_delay--;
            }
        }
        if ((keys & 0x300) && mode != 0) {
            result = -2;
            break;
        }
        if (keys & 1) {
            result = work->kinds[work->selection];
            break;
        }
        if (mode != 0 && (keys & 2)) {
            AudioCommand_PlayFar(0x71);
            result = -1;
            break;
        }
        if (repeat & 0x90) {
            AudioCommand_PlayFar(0x6f);
            work->selection = (work->selection + 1) % work->count;
        } else if (repeat & 0x60) {
            AudioCommand_PlayFar(0x6f);
            work->selection = (work->selection + work->count - 1) % work->count;
        } else if (gLinkCountdownWork->active == 0) {
            result = work->kinds[0];
            break;
        }
        QueueIoWriteDelay2(0x04000000, 0x1741);
        render->menu_busy = 0;
        WaitFrames(1);
    }
    if (mode != 0)
        gLinkCountdownWork->command_selection[1] = work->selection;
    else
        gLinkCountdownWork->command_selection[0] = work->selection;
    for (cnt = 0; cnt < work->count; cnt++)
        Resource_ResetEntry(work->entries[cnt].resource);
    render->menu_busy = 1;
    UiWork_Finalize(work->window, 1);
    {
        s32 panel;
        for (panel = 0; panel <= 6; panel++) {
            for (cnt = 0; cnt <= 2; cnt++) {
                s32 col;
                for (col = 0; col <= 2; col++)
                    *(volatile u16 *)(render->tilemap + (cnt + 17) * 32 + panel * 3 + 5 + (col & 3)) = 0xf07f; /* FAKEMATCH: the add of 0x44a stays apart from the store it addresses (adds r3, r7; strh r6, [r3]); as a plain store combine folds it into strh r6, [r3, r7], 0 -> 33 */
            }
        }
    }
    render->dirty = 1;
    WaitFrames(1);
    Scheduler_RemoveCallback(Graphics_SetBg1Priority3);
    Runtime_SetIrqHandler(2, 0, 0);
    IO_WRITE_QUEUE_PUSH(0x04000000, 0x1541, 0x20000);
    MemFree(work);
    render->menu_busy = 0;
    WaitFrames(1);
    return result;
}

void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 alt)
{
    struct UiRenderWork *render = (struct UiRenderWork *)gWindowWork[0];

    alt &= 1;
    alt <<= 12;

    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29) {
        width = 30 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29) {
        height = 20 - y;
    }

    if (width > 0 && height > 0) {
        do {
            u16 *p = &((u16 (*)[32])render->tilemap)[y][x];
            s32 col = width;

            if (col != 0) {
                do {
                    u32 tile = *p;
                    tile = (tile & ~0x1000) | alt;
                    col--;
                    *p = tile;
                    p++;
                } while (col != 0);
            }

            UiRender_MarkRow(render, y);
            height--;
            y++;
        } while (height != 0);
    }
}

s32 BattleLayout_HighlightPartyPanels(u16 *ids)
{
    struct BtlWork *battle = BtlWk;
    s32 count;
    s32 i;
    s32 j;

    count = BattleParty_ListActorIdsFar(1, 0);
    Ui_SetRectHighlight(29 - count * 6, 0, 25, 5, 15);
    i = 0;
    if (ids[0] != 0xff) {
    next_id:
        for (j = 0; j < 4 && battle->party_units[j] != ids[i]; j++) {
            if (battle->party_units[j] == 0xff) {
                j = 4;
                break;
            }
        }
        if (j != 4)
            Ui_SetRectHighlight(29 - (count - j) * 6, 0, 7, 5, 14);
        if (++i < 4 && ids[i] != 0xff)
            goto next_id;
    }
    return 0;
}

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value)
{
    // 値を返さない int 関数 (pop {r1}; bx r1)
    s16 *dst;
    s32 n;
    u8 *src;

    dst = out;
    Text_FormatSignedDecimalToWork(value);
    src = (u8 *)((u32)&gNumberTextBuffer);
    n = 0xD;
    do {
        n -= 1;
        *dst = *src;
        src += 1;
        dst += 1;
    } while (n >= 0);
}

s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *oldGrid, const struct OwnerActionSlot *newGrid, u16 *out, s32 *gained, s32 *lost)
{
    s32 i;
    s32 j;
    s32 count;
    s32 added;
    s32 removed;

    added = 0;
    removed = 0;
    count = 0;
    for (i = 0; i < 32 && newGrid[i].encoded_action != 0; i++) {
        if (BtlActGet(newGrid[i].encoded_action)->tgt_flg & 0x80) {
            out[count] = newGrid[i].encoded_action & 0x3fff;
            count++;
            for (j = 0; j < 32; j++) {
                if (((newGrid[i].encoded_action ^ oldGrid[j].encoded_action) & 0x3fff) == 0)
                    break;
            }
            if (j == 32) {
                added++;
                out[count - 1] |= 0x8000;
            }
        }
    }
    for (i = 0; i < 32 && oldGrid[i].encoded_action != 0; i++) {
        if (BtlActGet(oldGrid[i].encoded_action)->tgt_flg & 0x80) {
            for (j = 0; j < 32; j++) {
                if (((oldGrid[i].encoded_action ^ newGrid[j].encoded_action) & 0x3fff) == 0)
                    break;
            }
            if (j == 32) {
                removed++;
                out[count] = (oldGrid[i].encoded_action & 0x3fff) | 0x4000;
                count++;
            }
        }
    }
    *gained = added;
    *lost = removed;
    return count;
}

//--------------------------------------------------------------------------

// create_output_from_resource_slot.c

// 表示：わざのアイコンをスプライトとしてウィンドウに置く

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// あいている VRAM キャッシュにアイコンを読み、16x16 のスプライトで出す
// （あきがなければ何もしない）
// input : 置く先のウィンドウ
// x, y  : ウィンドウの中の位置（ドット）
// abil  : わざの番号
//--------------------------------------------------------------------------
s32 Ui_CreateOutputFromResourceSlot(struct RenderInput *input, s32 x, s32 y, s32 abil)
{
	s32 slot;

	slot = Resource_FindFreeEntry();
	if (slot != VRAM_CACHE_ENTRY_COUNT)
	{
		Resource_LoadIndexedEntryToBuffer(abil, slot);
		RenderOutput_Create(slot, 0x40000000, input, x, y); // OBJ サイズ 16x16
	}
}

#undef UiText_DrawStringAtOffset

struct BtlAct * BtlActGet(s32 action_id);
void UnitCalcFar(s32 unit_id);

#define UiWindow_Create ((struct RenderInput * (*)(s32, s32, s32, s32, s32))UiWindow_Create)
#define UiText_DrawCharacterAtOffset ((void (*)(s32, struct RenderInput *, s32, s32))UiText_DrawCharacterAtOffset)

#define UiWindow_SetTilemapEntry ((void (*)(struct RenderInput*,s32,s32,s32,s32))UiWindow_SetTilemapEntry)
#define UiWindow_DrawThreeTileColumn ((s32 (*)(struct RenderInput*,s32,s32,s32,s32))UiWindow_DrawThreeTileColumn)

#define Djinn_DeactivateFar ((void (*)(s32,s32,s32))Djinn_DeactivateFar)
#define BtlActGet ((struct AbilityData * (*)(s32))BtlActGet)

struct AbilityData {
    u8 field_00[2];
    u8 kind;
    u8 field_03[5];
    u8 element;
    u8 cost;
};

struct BattleUnitRecord {
    u8 field_00[0x38];
    s16 hp;
    s16 ep;
    u16 atk;
    u16 def;
    u16 agi;
    u8 luk;
    u8 field_43[0x58 - 0x43];
    u8 grid[0x129 - 0x58];
    u8 element;
};

void UiWindow_DrawDividerLine(struct RenderInput *win, s32 x, s32 y, s32 w, s32 h);
void UiWork_SetParamNibble(s32 nibble);
s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value);
s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *oldGrid, const struct OwnerActionSlot *newGrid, u16 *out, s32 *gained, s32 *lost);

s32 __divsi3(s32 numerator, s32 denominator);
void UiText_DrawNumberInWindow(s32 value, s32 digits, struct RenderInput *win, s32 x, s32 y);

struct RenderInput *DjinnMenu_ShowChangePreview(
    struct RenderInput *win, s32 owner, s32 code, s32 page, s32 *pageCount)
{
    struct BattleUnitRecord *unit;
    struct BattleUnitRecord *snap;
    struct AbilityData *info;
    s16 *buf;
    s32 oldHp;
    s32 oldPp;
    s32 oldAtk;
    s32 oldDef;
    s32 oldAgi;
    u16 *list;
    s32 total;
    s32 n;
    s32 marks;
    s32 rows;
    s32 col;
    s32 row;
    s32 pages;
    s32 gained;
    s32 lost;
    s32 tile;
    s32 val;
    s32 i;

    void DjinnMenu_DrawStatArrow(s32 x, s32 y, s32 rising)
    {
        struct RenderOutput *output = RenderOutput_AcquireFree();
        struct PreviewSprite *entry;
        u32 zero;
        if (output) {
            output->active = 1;
            output->kind = 1;
            /* FAKEMATCH: a char store (alias set 0) keeps the index store ahead of the attribute word in sched2 */
            *(s8 *)&output->index = Resource_LoadIntoFreeSlot(128);
            entry = (struct PreviewSprite *)((u8 *)output + 16);
            output->sentinel = 240;
            output->x = 120;
            output->y = 120;
            /* FAKEMATCH: typed like the window pointer, the word store waits for the window load but not for the x and y stores, so sched2 places the entry copy before them */
            *(struct RenderInput **)&entry->attributes = (struct RenderInput *)0x40000400;
            zero = 0;
            /* FAKEMATCH: a pointer-typed clear does not wait for the x and y stores (sched2 order) */
            *(void **)&entry->tile = (void *)zero;
            entry->attributes.bits.x = win->x * 8 + x;
            entry->attributes.bits.y = win->y * 8 + y;
            /* FAKEMATCH: advancing and restoring the captured pointer keeps
             * the window chain in r9. Without the pair, r8/r9 allocation
             * changes and two extra instructions remain (80 -> 82, 22 rows). */
            win++;
            win--;
            entry->tile.bits.index = Resource_GetBuffer((u8)output->index, (s32)(rising ? DjinnMenu_ArrowUpTiles : DjinnMenu_ArrowDownTiles));
            RenderOutput_AppendToList((struct RenderOutputList *)win, output);
        }
    }

    unit = UnitGetFar(owner);
    if (win != 0)
        UiWork_Finalize(win, 1);

    if (page == 0)
        win = UiWindow_Create(0, 8, 21, 11, 6);
    else
        win = UiWindow_Create(0, 5, 21, 14, 6);
    if (win == 0)
        return 0;

    buf = (s16 *)MemAlloc(128);
    snap = (struct BattleUnitRecord *)MemAlloc(332);
    list = (u16 *)Runtime_BumpAllocateAlternatePool(96);
    Iwram_CopyWords(snap, unit, 332);

    col = (code >> 8) & 15;
    row = code & 255;
    if (Djinn_IsActiveFar(owner, col, row) != 0)
        Djinn_DeactivateFar(owner, col, row);
    else
        Djinn_ActivateFar(owner, col, row);
    UnitCalcFar(owner);

    total = DjinnMenu_ListChangedDjinn((const struct OwnerActionSlot *)snap->grid, (const struct OwnerActionSlot *)unit->grid, list, &gained, &lost);
    pages = __divsi3(total - 1, 5) + 1;
    *pageCount = pages;
    if (page * 5 - 5 >= total)
        page = pages;

    if (page == 0) {
        s32 label = (s32)MsgPreviewStatLabel;

        UiText_DrawCharacterAtOffset(label, win, 0, 8);
        UiText_DrawCharacterAtOffset(label + 1, win, 0, 16);
        UiText_DrawCharacterAtOffset(label + 2, win, 0, 24);
        UiText_DrawCharacterAtOffset(label + 3, win, 0, 32);
        UiText_DrawCharacterAtOffset(label + 4, win, 0, 40);
        UiText_DrawCharacterAtOffset(label + 5, win, 0, 48);

        oldHp = snap->hp;
        UiText_FormatNumberToHalfwords(buf, oldHp);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 1);
#else
        UiText_RenderWideStringInWindow(buf + 7, win, 5, 1);
#endif
        oldPp = snap->ep;
        UiText_FormatNumberToHalfwords(buf, oldPp);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 2);
#else
        UiText_RenderWideStringInWindow(buf + 7, win, 5, 2);
#endif
        oldAtk = snap->atk;
        UiText_FormatNumberToHalfwords(buf, oldAtk);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 3);
#else
        UiText_RenderWideStringInWindow(buf + 8, win, 6, 3);
#endif
        oldDef = snap->def;
        UiText_FormatNumberToHalfwords(buf, oldDef);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 4);
#else
        UiText_RenderWideStringInWindow(buf + 8, win, 6, 4);
#endif
        oldAgi = snap->agi;
        UiText_FormatNumberToHalfwords(buf, oldAgi);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 5);
#else
        UiText_RenderWideStringInWindow(buf + 8, win, 6, 5);
#endif
        UiText_FormatNumberToHalfwords(buf, snap->luk);
        UiText_RenderWideStringInWindow(buf + 8, win, 5, 6);

        UiWindow_DrawDividerLine(win, 0, 8, 19, 8);
        if (gained != 0 || lost != 0)
            UiWork_SetParamNibble(2);
#if !EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffset((s32)MsgBattleAdditionalPsynergyHelp, win, 40, 64);
#else
        UiText_DrawCharacterAtOffset((s32)MsgBattleAdditionalPsynergyHelp, win, 24, 64);
#endif
        UiWork_SetParamNibble(15);
    }

    if (page > 0) {
        marks = 0;
        rows = total;
        if (total > 4)
            rows = 5;
        i = page * 5 - 5;
        for (n = 0; n < rows && i < total; n++, i++) {
            info = BtlActGet(list[i]);
            if (info->kind != 4)
                UiWindow_SetTilemapEntry(win, 0x5001 + info->kind, 15, n * 2, 0);

            tile = info->element;
            if (tile == 255)
                tile = 11;
            else
                tile--;
            UiWindow_DrawThreeTileColumn(win, 16, n * 2, tile, 0);

            Ui_CreateOutputFromResourceSlot(win, 0, n * 16 - 4, list[i] & 0x3fff);
            if ((list[i] & 0x8000) != 0)
                UiWork_SetParamNibble(4);
            else if ((list[i] & 0x4000) != 0)
                UiWork_SetParamNibble(2);
            else
                UiWork_SetParamNibble(15);

            UiText_DrawCharacterAtOffset((list[i] & 0x3fff) + (s32)MsgAbilityName, win, 16, n * 16);
            UiWindow_SetTilemapEntry(win, 0xf01f, 11, n * 2, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            UiWindow_SetTilemapEntry(win, 0xf005, 12, n * 2, 0);
#else
            UiWindow_SetTilemapEntry(win, 0xf01e, 12, n * 2, 0);
#endif
            UiText_DrawNumberInWindow(
                BtlActGet(list[i])->cost, 2, win, 104, n * 16);
        }

        if (gained != 0) {
            UiWork_SetParamNibble(4);
            UiText_DrawCharacterAtOffset((s32)MsgPsynergyGained, win, 32, 80);
            marks = 1;
        }
        if (lost != 0) {
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset((s32)MsgPsynergyLost, win, 32, marks * 8 + 80);
            marks++;
        }
        if (marks == 0)
            UiText_DrawCharacterAtOffset((s32)MsgPsynergyUnchanged, win, 32, 80);
        UiWork_SetParamNibble(15);
        UiWork_SetParamNibble(15);
        UiWindow_DrawDividerLine(win, 0, 10, 19, 10);
    }

    if (page == 0) {
        UiText_DrawCharacterAtOffset((s32)MsgClassName + snap->element, win, 0, 0);
        UiText_DrawCharacterAtOffset((s32)MsgClassName + unit->element, win, 80, 0);
        if (snap->element != unit->element)
            UiWindow_SetTilemapEntry(win, 0xf728, 9, 0, page);
        else
            UiWindow_SetTilemapEntry(win, 0xf729, 9, 0, page);

        UiText_FormatNumberToHalfwords(buf, unit->hp);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 1);
        if (unit->hp != oldHp) {
            val = 0;
            if (unit->hp > oldHp)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 14, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->ep);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 2);
        if (unit->ep != oldPp) {
            val = 0;
            if (unit->ep > oldPp)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 22, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->atk);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 3);
        if (unit->atk != oldAtk) {
            val = 0;
            if (unit->atk > oldAtk)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 30, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->def);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 4);
        if (unit->def != oldDef) {
            val = 0;
            if (unit->def > oldDef)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 38, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->agi);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 5);
        if (unit->agi != oldAgi) {
            val = 0;
            if (unit->agi > oldAgi)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 46, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->luk);
        UiText_RenderWideStringInWindow(buf + 8, win, 12, 6);
        if (unit->luk != snap->luk) {
            val = 0;
            if (unit->luk > snap->luk)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 54, val);
        }
    }

    Iwram_CopyWords(unit, snap, 332);
    MemFree(list);
    MemFree(snap);
    MemFree(buf);
    return win;
}

#if EDITION_INTERNATIONAL
#define OWNER_STATUS_TEXT_MAX 0x80
#define OWNER_STATUS_VALUE_X 184
#else
#define OWNER_STATUS_TEXT_MAX 0x40
#define OWNER_STATUS_VALUE_X 176
#endif

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
#define OWNER_STATUS_LABEL(row) ((s32)MsgOwnerStatusAttack + (row))
#else
#define OWNER_STATUS_LABEL(row) ((s32)MsgOwnerStatusExp - 10 + (row))
#endif

struct StatusPixel {
    s32 x;
    s32 y;
};

struct StatusCursor {
    s8 pos[5];
    s32 entry;
    s32 lastEntry;
    s32 mode;
    s32 limit;
    s32 col;
    s32 row;
};

void UiWork_PushValueSlot(s32 value, s32 slot);
void UiWindow_MarkVisibleTileAttributes(void);
s32 Party_SumDjinnCountsFar(s32 request);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
s32 System_GetLanguage(void);
#endif

s32 Ui_RunOwnerStatusScreen(u16 *list, s32 listCount, s32 owner)
{
    struct MenuSprite objFace;
    struct MenuSprite objCursor;
    s32 tbl[11];
    s8 sel[11];
    struct MenuSprite objIcon[11];
    struct MenuSprite objName;
    struct StatusCursor cur;
    struct UiRenderWork *base;
    struct UiWindow *winMain;
    struct UiWindow *winDesc;
    s32 idx;
    s32 handleCursor;
    struct StatusPixel pix;
    s32 handleFrame;
    s32 cnt;
    s32 extended;
    s32 keys;
    u16 *text;
    s32 iconIdx;
    s32 *q;
    s32 defDelta;
    s32 i;
    s32 j;
    s32 atkDelta;
    s32 redraw;
    struct BtlUnit *object;
    struct BtlUnit *saved;
    const struct OwnerStatusStatCursor *stat;
    struct MenuSprite *p;
    s32 agiDelta;
    s32 code;
    s32 n;
    struct MenuCell *screen;
    s32 x;

    base = (struct UiRenderWork *)gWindowWork[0];
    redraw = 1;
    idx = -1;
    handleFrame = Resource_LoadIntoFreeSlot(0x200);
    cnt = 0;
    extended = Party_SumDjinnCountsFar(-1);

    screen = ((struct MenuCell *)gWindowWork[42]);
    screen->busy = 1;
    if (screen->work != NULL) {
        UiWork_Finalize(screen->work, 1);
        screen->work = NULL;
    }
    AudioCommand_PlayFar(0x70);

    for (i = 4; i >= 0; i--) {
        cur.pos[i] = 0;
    }
    cur.entry = 0;
    cur.lastEntry = 0;
    cur.mode = 0;

    handleCursor = Resource_LoadIntoFreeSlot(0x80);
    for (i = 0; i <= 10; i++) {
        tbl[i] = Resource_LoadIntoFreeSlot(0x80);
        sel[i] = -1;
    }

    if (listCount != 0) {
        for (i = 0; i <= 5; i++) {
            s32 kind = list[i];

            if (kind == 0xFF) {
                break;
            }
            if (kind != 0xFE) {
                if (kind == owner) {
                    idx = i;
                    break;
                }
            }
        }
    }

    pix.x = 0;
    pix.y = 0;
    winMain = (struct UiWindow *)UiWindow_Create(0, 0, 30, 20, 6);
    winDesc = (struct UiWindow *)UiWindow_Create(0, 14, 30, 6, 10);
    UiWindow_MarkVisibleTileAttributes();

    for (;;) {
        keys = gKeysRepeat;

        objName.oam.word.attr01 = 0x80000400;
        objName.oam.word.attr23 = 0;
        objName.oam.f.tile = Ui_LoadEntryForKind(owner, handleFrame);
        objName.oam.f.x = 8;
        objName.oam.f.y = 24;
        objName.oam.f.palette = 14;
        Runtime_PushSlotEntry((s32 *)&objName, 240);

        switch (cur.entry) {
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            cur.mode = 0;
            if (extended != 0) {
                cur.limit = 9;
            } else {
                cur.limit = 7;
            }
            break;
        case 10:
        case 11:
        case 12:
        case 13:
            cur.mode = 1;
            cur.limit = 4;
            break;
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            cur.mode = 2;
            cur.limit = cnt;
            break;
        }

        Ui_SetRectHighlight(
            winMain->x, winMain->y, winMain->width, winMain->height, 15);

        if (cnt != 0) {
            if ((u32)cur.mode <= 1) {
                n = cur.pos[cur.mode];
                if ((keys & 0x80) != 0) {
                    keys = 0;
                    AudioCommand_PlayFar(0x6F);
                    n++;
                    if (n >= cur.limit) {
                        n = 0;
                        if (cur.mode == 1) {
                            cur.mode = 2;
                            n = cur.pos[cur.mode];
                        }
                    }
                } else if ((keys & 0x40) != 0) {
                    keys = 0;
                    AudioCommand_PlayFar(0x6F);
                    n--;
                    if (n < 0) {
                        n = cur.limit - 1;
                        if (cur.mode == 1) {
                            cur.mode = 2;
                            n = cur.pos[cur.mode];
                        }
                    }
                } else if ((keys & 0x31) != 0) {
                    keys = 0;
                    AudioCommand_PlayFar(0x6F);
                    cur.mode ^= 2;
                    if ((gKeysRepeat & KEY_A) != 0) {
                        n = cur.pos[cur.mode];
                    } else if ((gKeysRepeat & KEY_LEFT) != 0) {
                        n = cnt - 1;
                    } else {
                        n = 0;
                    }
                }
                cur.pos[cur.mode] = (s8)n;
            } else if (cur.mode == 2) {
                n = cur.pos[cur.mode];
                if (n >= cur.limit) {
                    n = cur.limit - 1;
                }
                if (n < 0) {
                    cur.mode = 0;
                    n = cur.pos[0];
                } else if ((keys & 0x10) != 0) {
                    keys = 0;
                    n++;
                    if (n >= cur.limit) {
                        cur.mode = 0;
                        n = cur.pos[0];
                    }
                    AudioCommand_PlayFar(0x6F);
                } else if ((keys & 0x20) != 0) {
                    keys = 0;
                    n--;
                    if (n < 0) {
                        cur.mode = 0;
                        n = cur.pos[0];
                    }
                    AudioCommand_PlayFar(0x6F);
                } else if ((keys & 0xC1) != 0) {
                    keys = 0;
                    cur.mode = 0;
                    n = cur.pos[0];
                    AudioCommand_PlayFar(0x6F);
                }
                cur.pos[cur.mode] = (s8)n;
            }
        }

        if (cur.mode == 0) {
            s32 k = cur.pos[cur.mode];

            if (extended == 0) {
                k += 9;
            }
            stat = &OwnerStatus_StatCursors[k];
            cur.entry = stat->entry;
            cur.col = stat->column;
            cur.row = stat->row;
            Ui_SetRectHighlight(
                winMain->x + stat->highlight_column + 1, winMain->y + stat->highlight_row + 1, stat->highlight_width, 1,
                14);
        } else if (cur.mode == 1) {
            cur.entry = OwnerStatus_EquipmentCursors[cur.pos[cur.mode]].entry;
            cur.col = OwnerStatus_EquipmentCursors[cur.pos[cur.mode]].column;
            cur.row = OwnerStatus_EquipmentCursors[cur.pos[cur.mode]].row;
        } else if (cur.mode == 2) {
            cur.entry = OwnerStatus_ConditionCursors[cur.pos[cur.mode]].entry;
            cur.col = OwnerStatus_ConditionCursors[cur.pos[cur.mode]].column;
            cur.row = OwnerStatus_ConditionCursors[cur.pos[cur.mode]].row;
        }

        if (cur.lastEntry != cur.entry) {
            cur.lastEntry = cur.entry;
            redraw = 2;
        }
        pix.x = cur.col * 8;
        pix.y = cur.row * 8;

        {
            struct MenuSprite *face = GetBattleObjectSlotFar(owner)->object->records;

            objFace.oam.word.attr01 = 0xC0002400;
            objFace.oam.word.attr23 = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
            /* FAKEMATCH: the zero lives in sl here; sched2 puts the attr23 store before the move into sl and the 0xFC00 load before the face tile load, which may alias that store; native moves into sl between the two stores and loads the tile first (4 instructions) */
            asm("");
#endif
            objFace.oam.f.tile = face->oam.f.tile;
            objFace.oam.f.x = 0xAC;
            objFace.oam.f.y = 56;
            Runtime_PushSlotEntry((s32 *)&objFace, 240);
        }

        objCursor.oam.word.attr01 = 0x40000400;
        objCursor.oam.word.attr23 = 0;
        /* FAKEMATCH: arm_reorg counts this empty asm as 4 bytes, so the gFrameCount literal falls past the face x literal's 64-byte ldrh range and opens the next pool, as native; the pool and its branch move two instructions earlier, 85 -> 63. */
        /* FAKEMATCH: removing both barriers gives EN 1392/1392, common 1390, differ 4: gFrameCount joins the earlier pool and the branch follows two extra loads. ES 1458/1458, common 1455, differ 6: attr23/mov sl order and the 0x3ff load also move. 2026-10-11: arm_reorg runs before shorten_branches and counts the empty asm at its 4-byte default length while it emits nothing; the halfword constant fix needs that much more estimated length after it, and no C statement or single pattern-length change (strh, str, orr, and tested over all 413 English objects) supplies it. In ES and IT both asms are scheduling barriers only (differ 8 without them). */
        asm("");
        objCursor.oam.f.tile = Resource_GetBuffer(handleCursor, (s32)Resource_FixedBlockBTiles);
        objCursor.oam.f.x = (pix.x + winMain->x * 8
                          - ((gFrameCount & 4) >> 2))
            + 16;
        objCursor.oam.f.y = (u8)((pix.y + winMain->y * 8
                               - ((gFrameCount & 4) >> 2))
            + 16);
        objCursor.oam.f.affine_index = 8;
        Runtime_PushSlotEntry((s32 *)&objCursor, 241);

        if (redraw != 0) {
            object = UnitGetFar(owner);
#if EDITION_INTERNATIONAL
            UiWindow_MarkVisibleTileAttributes();
#endif
            Ui_FillVramBlockPattern();

            if ((redraw & 1) != 0) {
                RenderOutput_RedrawSavedRect(winMain);
                UiWindow_MapTextCanvasTiles(winDesc->x, winDesc->y, winDesc->width,
                    winDesc->height, 0);
                UiWindow_DrawDividerLine((struct RenderInput *)winMain, 0, 14, 29, 14);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                switch (System_GetLanguage()) {
                case 2:
                    UiText_DrawStringAtOffset((u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_NivString, (struct RenderInput *)winMain, 51, 0);
                    UiText_DrawNumberInWindow(object->level, 2, (struct RenderInput *)winMain, 72, 0);
                    break;
                case 3:
                    UiText_DrawStringAtOffset((u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_NvString, (struct RenderInput *)winMain, 56, 0);
                    UiText_DrawNumberInWindow(object->level, 2, (struct RenderInput *)winMain, 72, 0);
                    break;
                default:
                    UiText_DrawStringAtOffset((u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_LevelString, (struct RenderInput *)winMain, 56, 0);
                    UiText_DrawNumberInWindow(object->level, 2, (struct RenderInput *)winMain, 72, 0);
                    break;
                }
#else
                UiText_DrawStringAtOffset((u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
#if defined(TBS_EDITION_FR)
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_LevelString, (struct RenderInput *)winMain, 51, 0);
#else
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_LevelString, (struct RenderInput *)winMain, 56, 0);
#endif
                UiText_DrawNumberInWindow(object->level, 2, (struct RenderInput *)winMain, 72, 0);
#endif
                UiText_DrawCharacterAtOffset((s32)MsgOwnerStatusExp, (struct RenderInput *)winMain, 0, 8);
                UiText_DrawNumberInWindow(
                    object->experience, 8, (struct RenderInput *)winMain, 40, 8);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                switch (System_GetLanguage()) {
                case 1:
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_KpString, (struct RenderInput *)winMain, 40, 24);
                    break;
                case 2:
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_PvString, (struct RenderInput *)winMain, 40, 24);
                    break;
                case 3:
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_PvString, (struct RenderInput *)winMain, 40, 24);
                    break;
                case 4:
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_HpString, (struct RenderInput *)winMain, 40, 24);
                    break;
                }
#else
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_HpString, (struct RenderInput *)winMain, 40, 24);
#endif
                UiText_DrawNumberInWindow(
                    object->hp, 4, (struct RenderInput *)winMain, 56, 24);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 88, 24);
                UiText_DrawNumberInWindow(
                    object->max_hp, 4, (struct RenderInput *)winMain, 96, 24);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_PpString, (struct RenderInput *)winMain, 40, 32);
                UiText_DrawNumberInWindow(
                    object->ep, 4, (struct RenderInput *)winMain, 56, 32);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 88, 32);
                UiText_DrawNumberInWindow(
                    object->max_ep, 4, (struct RenderInput *)winMain, 96, 32);

                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(0), (struct RenderInput *)winMain, 136, 16);
                UiText_DrawNumberInWindow(
                    object->atk, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 16);
                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(1), (struct RenderInput *)winMain, 136, 24);
                UiText_DrawNumberInWindow(
                    object->def, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 24);
                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(2), (struct RenderInput *)winMain, 136, 32);
                UiText_DrawNumberInWindow(
                    object->agi, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 32);
                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(3), (struct RenderInput *)winMain, 136, 40);
                UiText_DrawNumberInWindow(object->luck, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 40);

                UiText_DrawCharacterAtOffset(
                    (s32)MsgClassName + object->class_index, (struct RenderInput *)winMain, 0, 48);
                if (extended != 0) {
                    UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(9), (struct RenderInput *)winMain, 0, 72);
                }
                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(5), (struct RenderInput *)winMain, 0, 80);
                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(6), (struct RenderInput *)winMain, 0, 88);
                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(7), (struct RenderInput *)winMain, 0, 96);

                for (j = 0; j <= 3; j++) {
                    s32 pal;

                    pal = 1;
                    if (extended != 0) {
                        pal = 0;
                    }
                    UiWindow_SetTilemapEntry(
                        (struct RenderInput *)winMain, 0x5001 + j, 7 + j * 4, pal + 8, 0);
                    if (extended != 0) {
                        UiText_DrawNumberInWindow(object->djinn_active_counts[j], 1,
                            (struct RenderInput *)winMain, 40 + j * 32, 72);
                        UiText_DrawStringAtOffset(
                            (u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 48 + j * 32, 72);
                        UiText_DrawNumberInWindow(object->djinn_owned_counts[j], 1,
                            (struct RenderInput *)winMain, 48 + j * 32 + 8, 72);
                    }
                    UiText_DrawNumberInWindow(Owner_GetResistanceValueFar(owner, j), 2,
                        (struct RenderInput *)winMain, 48 + j * 32, 80);
                    UiText_DrawNumberInWindow(
                        object->elm[j].pow, 3, (struct RenderInput *)winMain,
                        40 + j * 32, 88);
                    UiText_DrawNumberInWindow(
                        object->elm[j].res, 3, (struct RenderInput *)winMain,
                        40 + j * 32, 96);
                }

                i = 0;
                if (object->hp == 0) {
                    sel[0] = 16;
                    i = 1;
                }
                do {
                    if (object->restraint != 0) {
                        sel[i++] = 15;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->shiryo != 0) {
                        sel[i++] = 8;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->doku == 1) {
                        sel[i++] = 1;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->doku == 2) {
                        sel[i++] = 2;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->fuuji != 0) {
                        sel[i++] = 4;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->shibire != 0) {
                        sel[i++] = 3;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->nemuri != 0) {
                        sel[i++] = 5;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->akuryo != 0) {
                        sel[i++] = 7;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->maboroshi != 0) {
                        sel[i++] = 6;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->atk_cnt != 0) {
                        if (object->atk_lv > 0) {
                            sel[i++] = 9;
                        }
                        if (object->atk_lv < 0) {
                            sel[i++] = 10;
                        }
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->def_cnt != 0) {
                        if (object->def_lv > 0) {
                            sel[i++] = 11;
                        }
                        if (object->def_lv < 0) {
                            sel[i++] = 12;
                        }
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->res_cnt != 0) {
                        if (object->res_lv > 0) {
                            sel[i++] = 13;
                        }
                        if (object->res_lv < 0) {
                            sel[i++] = 14;
                        }
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->agi_lv > 0) {
                        sel[i++] = 17;
                    }
                    if (object->agi_lv < 0) {
                        sel[i++] = 18;
                    }
                } while (0);

                for (j = 0; j < i; j++) {
                    Resource_LoadTableEntryToBuffer(sel[j], tbl[j]);
                }
                if (i == 0) {
                    sel[0] = 0;
                    i = 1;
                }
                if (i <= 10) {
                    for (j = i; j < 11; j++) {
                        sel[j] = -1;
                    }
                }
                cnt = i;
                if (sel[0] == 0) {
                    UiText_DrawCharacterAtOffset(
                        object->hp != 0 ? (s32)MsgOwnerStatusNormal : (s32)MsgOwnerStatusDowned,
                        (struct RenderInput *)winMain, 112, 0);
                }
            }

            text = (u16 *)MemAlloc(OWNER_STATUS_TEXT_MAX * sizeof(*text));
            if ((u32)cur.entry > 13) {
                iconIdx = sel[cur.entry - 14];
                if (iconIdx == 0 && object->hp == 0) {
                    iconIdx = 16;
                }

                saved = (struct BtlUnit *)MemAlloc(sizeof(*object));
                code = 0;
                Iwram_CopyWords(saved, object, sizeof(*object));

                atkDelta = object->atk;
                defDelta = object->def;
                agiDelta = object->agi;
                object->atk_lv = 0;
                object->def_lv = 0;
                object->agi_lv = 0;
                UnitCalcFar(owner);
                atkDelta -= object->atk;
                defDelta -= object->def;
                agiDelta -= object->agi;
                Iwram_CopyWords(object, saved, sizeof(*object));
                MemFree(saved);

                switch (iconIdx) {
                case 8:
                    code = object->shiryo;
                    break;
                case 9:
                    code = atkDelta;
                    break;
                case 10:
                    code = -atkDelta;
                    break;
                case 11:
                    code = defDelta;
                    break;
                case 12:
                    code = -defDelta;
                    break;
                case 17:
                    code = agiDelta;
                    break;
                case 18:
                    code = -agiDelta;
                    break;
                case 13:
                case 14:
                    code = object->res_lv * 20;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                    if (code < 0) {
                        code = -code;
                    }
#endif
                    break;
                default:
                    break;
                }
                UiWork_PushValueSlot(code, 5);
                UiText_CopyMessageString(iconIdx + (s32)MsgOwnerStatusConditionHelp, text, OWNER_STATUS_TEXT_MAX);
            } else if (cur.entry == 2 && object->level <= 98) {
                UiWork_PushValueSlot(
                    Owner_GetLevelThresholdFar(owner, object->level + 1)
                        - object->experience,
                    5);
                UiText_CopyMessageString((s32)MsgOwnerStatusNextLevel, text, OWNER_STATUS_TEXT_MAX);
            } else {
                UiText_CopyMessageString(cur.entry + (s32)MsgOwnerStatusEntryHelp, text, OWNER_STATUS_TEXT_MAX);
            }

            UiText_RenderWideStringAtOffset(text, (struct UiWindow *)winDesc, 0, 4);
            MemFree(text);
            redraw = 0;
            base->dirty = 1;
        }

        p = objIcon;
        q = tbl;
        {
            struct VramBlockCacheEntry *vc;

            for (i = 0, vc = gVramBlockCache, x = 112; i <= 10; i++) {
                p->oam.word.attr01 = 0x40000400;
                p->oam.word.attr23 = 0;
                p->oam.f.tile = vc[*q++].offset >> 5;
                p->oam.f.x = x;
                p->oam.f.y = (u8)(winMain->y * 8 + 8);
                if (sel[i] > 0) {
                    Runtime_PushSlotEntry((s32 *)p, 240);
                }
                x += 15;
                p++;
            }
        }

        if (gLinkCountdownWork->active == 0) {
            break;
        }
        if ((gKeyState & KEY_B) != 0) {
            break;
        }

        if (listCount != 0) {
            if ((keys & 0x100) != 0) {
                idx++;
                if (idx >= listCount) {
                    idx = 0;
                }
                owner = list[idx];
                redraw = 1;
                AudioCommand_PlayFar(0x6F);
            } else if ((keys & 0x200) != 0) {
                idx--;
                if (idx < 0) {
                    idx = listCount - 1;
                }
                owner = list[idx];
                redraw = 1;
                AudioCommand_PlayFar(0x6F);
            }
        }
        WaitFrames(1);
    }

    {
        s32 *h = tbl;

        for (i = 10; i >= 0; i--) {
            Resource_ResetEntry(*h++);
        }
    }
    Resource_ResetEntry(handleFrame);
    Resource_ResetEntry(handleCursor);
    WaitFrames(1);
#if EDITION_INTERNATIONAL
    UiWindow_MarkVisibleTileAttributes();
#endif
    UiWork_Finalize(winMain, 1);
    UiWork_Finalize(winDesc, 1);
    PtyStDraw(BtlWk->pty_st);
    ((struct MenuCell **)&BtlWk)[HEAP_SLOT_WINDOW + 42 - HEAP_SLOT_BATTLE]->busy = 0;
    WaitFrames(1);
    return 0;
}

#if EDITION_INTERNATIONAL
#define TEXT_COUNT 52
#define TEXT_LOAD UiText_CopyMessageString
#define TURNS_DIGITS 1
#define TURNS_X 48
#else
#define TEXT_COUNT 32
#define TEXT_LOAD UiText_DecodeMessage
#define TURNS_DIGITS 2
#define TURNS_X 40
#endif

struct BattleMenuSelection {
    struct MenuCell menu;
    u32 selected[8];
};

LAYOUT_SIZE_GUARD(BattleMenuSelection_Size, struct BattleMenuSelection, 0x104);
LAYOUT_OFFSET_GUARD(BattleMenuSelection_Selected, struct BattleMenuSelection, selected, 0xe4);

#define MENU_STATE ((struct BattleMenuSelection *)gLinkCountdownWork)
#define CURSOR_OWNER (*(struct PartyStatusWindow **)gPartyStatusWork)

extern const u8 DjinnMenu_BlinkTile[32];
extern const u8 DjinnMenu_ElementXOffsets[4];
extern const u8 DjinnMenu_ElementYOffsets[4];

void RenderOutput_ClearList(void *work);
void UiWork_SetAltFlagAndClearTable(s32 flag);

s32 Battle_SelectAbility(s32 owner, s32 unused)
{
	struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 side);
    struct CursorSprite slot;
    u16 buf[64];
    struct UiRenderWork *ctx;
    struct UiWindow *detail_win;
    s32 prev_row;
    s32 res;
    u32 *tbl;
    s32 cnt;
    s32 prev_page;
    struct UiWindow *msg_win;
    s32 mask;
    s32 saved_row;
    s32 sub_sel;
    s32 redraw;
    s32 is_used;
    s32 sub_cnt;
    struct BattleMenuSelection *menu;
    struct BtlUnit *unit;
    struct DjinnRecoveryList *list;
    struct UiWindow *list_win;
    struct UiWindow *icon_win;
    struct PartyStatusWindow *cursor;
    u32 *pos;
    u32 cur;
    u32 blink;
    s32 page;
    s32 row;
    u32 code;
    s32 pages;
    s32 pal;
    struct MenuPoint cursor_pos;
    s32 trig;
    s32 rep;
    s32 opts;
    s32 x;
    s32 y;
    s32 i;
    s32 j;
    struct CursorSprite *cur_spr;

    ctx = (struct UiRenderWork *)gWindowWork[0];
    detail_win = 0;
    prev_row = -1;
    res = Resource_LoadIntoFreeSlot(0x80);
    tbl = (u32 *)Runtime_BumpAllocateAlternatePool(0xa8 * 2);
    cnt = 0;
    prev_page = -1;
    code = 0;
    mask = 0;
    sub_sel = 0;
    sub_cnt = 0;
    redraw = 0;
    is_used = 0;
    msg_win = (struct UiWindow *)UiWindow_Create(0, 4, 30, 4, 42);
    UiWork_SetAltFlagAndClearTable(1);
    list_win = (struct UiWindow *)UiWindow_Create(21, 9, 9, 11, 6);
    MenuCell_GetCursor(page, row, saved_row);
    unit = UnitGetFar(owner);

    for (x = 0; x <= 3; x++) {
        for (y = 0, pos = (u32 *)((cnt << 2) + (u32)tbl); y <= 19; y++) {
            if (unit->djinn_active[x] & (u32)(1 << y)) {
                *pos = (u32)((x << 8) | y);
                pos++;
                cnt++;
            } else if (unit->djinn_available[x] & (u32)(1 << y)) {
                u32 v;
                s32 k;
                list = &Trade_GetOfferStateFar((u32)owner > 7 ? 1 : 0)->list;
                for (k = 0; k < list->count; k++) {
                    if (list->entries[k].unit_id == owner && list->entries[k].element == x &&
                        list->entries[k].index == y) {
                        break;
                    }
                }
                v = (u32)((x << 8) | y | 0x10000);
                *pos = v;
                if (list->entries[k].turns > 0) {
                    v |= (u32)(list->entries[k].turns << 17);
                    *pos = v;
                }
                cnt++;
                pos++;
            }
        }
    }
    tbl[cnt] = 0x80000000;
    ctx->dirty = 1;

    cur_spr = &slot;
    for (;;) {
        if (page != prev_page || row != prev_row || redraw != 0) {
            cur = tbl[page + row];
            mask = 0;
            ctx->menu_busy = 1;
            Ui_SetRectHighlight(list_win->x + 1, list_win->y + prev_row * 2 + 1,
                          list_win->width - 2, 1, 15);
            if (sub_sel != 0) {
                UiWork_Finalize(msg_win, 1);
                msg_win = (struct UiWindow *)UiWindow_Create(0, 4, 30, 4, 42);
                Ui_FillVramBlockPattern();
            }
            redraw = 0;
            if (cnt != 0) {
                s32 k;
                u32 selected;
                UiWork_ClearValueNameTables();
                /* FAKEMATCH: restores a CSE1 block end the bytes prove (k = redraw survives as the native ldr r1, [sp, #28]): an empty one-pass block, without which CSE1 deletes the second redraw = 0 below as a no-op and folds k to 0 (29 rows) */
                do { } while (0);
                is_used = 0;
                /* FAKEMATCH: restores redraw set in the search block, which the surviving copy proves gcse saw: cprop leaves k = redraw alone only when its source is set earlier in its block, and cse2 deletes this store as a no-op (29 rows without) */
                redraw = 0;
                /* FAKEMATCH: this one-pass loop retains the native redraw
                 * reload after the selected-entry load. A plain assignment
                 * folds k to zero (1201 -> 1200 instructions, 27 rows differ);
                 * reading selected in the comparison moves the reload early
                 * (2 rows differ). No loop instructions survive. */
                selected = MENU_STATE->selected[redraw];
                for (k = redraw; k == redraw;) {
                    k = redraw;
                    break;
                }
                if (selected == cur) {
                    is_used = 1;
                } else {
                    for (;;) {
                        /* FAKEMATCH: needed to match: is_used is zero on every search-loop continuation. Its step dependency prevents loop.c from strength-reducing k into a pointer; CSE later removes the flag term. Plain k differs 35 instructions in EN; this form restores the native reload and indexing (differ 0). */
                        k += 1 - is_used;
                        if (k > 7) break;
                        if ((u32)MENU_STATE->selected[k] == cur) {
                            is_used = 1;
                            break;
                        }
                    }
                }
                if (is_used != 0) {
                    TEXT_LOAD((s32)MsgBattleDjinnTwice, buf, TEXT_COUNT);
                    if (detail_win != 0) {
                        UiWork_Finalize(detail_win, 1);
                        detail_win = 0;
                        sub_sel = 0;
                    }
                } else if (cur & 0x10000) {
                    if (cur & 0x3e0000) {
                        UiWork_PushValueSlot((s32)((cur & 0x3e0000) >> 17), 5);
                        TEXT_LOAD((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + (s32)MsgDjinnEffect, buf, TEXT_COUNT);
                        if (detail_win != 0) {
                            UiWork_Finalize(detail_win, 1);
                            detail_win = 0;
                            sub_sel = 0;
                        }
                    } else {
                        detail_win = (struct UiWindow *)DjinnMenu_ShowChangePreview((struct RenderInput *)detail_win, owner, (s32)cur, sub_sel, &sub_cnt);
                        UiText_CopyMessageString((s32)MsgBattleSummonStandby, buf, TEXT_COUNT);
                        mask = 1 << ((cur & 0xf00) >> 8);
                    }
                } else {
                    detail_win = (struct UiWindow *)DjinnMenu_ShowChangePreview((struct RenderInput *)detail_win, owner, (s32)cur, sub_sel, &sub_cnt);
                    TEXT_LOAD((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + (s32)MsgDjinnEffect, buf, TEXT_COUNT);
                }
            } else {
                TEXT_LOAD((s32)MsgBattleNoDjinn, buf, TEXT_COUNT);
            }
            ctx->menu_busy = 0;
            if (sub_sel == 0) {
                ctx->menu_busy = 1;
                UiWork_Finalize(msg_win, 1);
                msg_win = (struct UiWindow *)UiWindow_Create(0, 4, 30, 4, 42);
                ctx->menu_busy = (u8)sub_sel;
            }
            UiText_RenderWideStringAtOffset(buf, msg_win, 0, 4);
            prev_row = row;
            if (page != prev_page) {
                RenderOutput_RedrawSavedRect(list_win);
                j = 0;
                while ((code = tbl[page + j]) != 0x80000000) {
                    UiWindow_SetTilemapEntry((struct RenderInput *)list_win, (s32)((code & 0xf00) >> 8) + 0x5001, 0, j * 2, 0);
                    if (code & 0x3e0000) {
                        UiWork_SetParamNibble(4);
                    } else if (code & 0x10000) {
                        UiWork_SetParamNibble(2);
                    }
                    UiText_DrawCharacterAtOffset((s32)(((code & 0xf00) >> 8) * 20 + (code & 0xff)) + (s32)MsgDjinnName, (struct RenderInput *)list_win, 8, j * 16);
                    if (code & 0x3e0000) {
                        UiText_DrawNumberAtOffset((s32)((code & 0x3e0000) >> 17), TURNS_DIGITS, (struct RenderInput *)list_win, TURNS_X, j * 16);
                    }
                    UiWork_SetParamNibble(15);
                    j++;
                    if (j > 4) {
                        break;
                    }
                }
                prev_page = page;
            }
            if (cnt > 5) {
                for (j = 0; j < (pages = ((cnt + 4) / 5)); j++) {
                    s32 tile = j + 0xf301;
                    if (j == (page / 5)) {
                        tile = j + 0xf30b;
                    }
                    UiWindow_SetTilemapEntry((struct RenderInput *)list_win, tile, list_win->width - pages + j - 2, -1, 0);
                }
            }
            Ui_SetRectHighlight(list_win->x + 1, list_win->y + row * 2 + 1,
                          list_win->width - 2, 1, 14);
            ctx->dirty = 1;
            ctx->menu_busy = 0;
        }

        if (cnt > 5) {
            for (j = 0; j < (cnt + 4) / 5; j++) {
                s32 tile = j + 0xf301;
                if ((gKeysHeld & 0x100) || (gFrameCount & 15) <= 11) {
                    if (j == (page / 5)) {
                        tile = j + 0xf30b;
                    }
                }
                UiWindow_SetTilemapEntry((struct RenderInput *)list_win, tile,
                              list_win->width - ((cnt + 4) / 5) + j - 2, -1, 0);
            }
            if ((gKeysHeld & 0x100) == 0) {
                UiWindow_SetTilemapEntry((struct RenderInput *)list_win, 0xf334, list_win->width - (cnt + 4) / 5 - 3, -1, 0);
                UiWindow_SetTilemapEntry((struct RenderInput *)list_win, 0xf335, list_win->width - 2, -1, 0);
            } else {
                UiWindow_SetTilemapEntry((struct RenderInput *)list_win, 0xf011, list_win->width - (cnt + 4) / 5 - 3, -1, 0);
                UiWindow_SetTilemapEntry((struct RenderInput *)list_win, 0xf012, list_win->width - 2, -1, 0);
            }
            UiRender_MarkRow(ctx, list_win->y - 1);
        }

        trig = gKeyState;
        rep = gKeysRepeat;
        opts = (s32)gKeysHeld;
        menu = MENU_STATE;
        if (menu->menu.auto_enabled != 0) {
            rep = 0;
            trig = 0;
            opts = 0;
            if (menu->menu.auto_delay == 0) {
                menu->menu.auto_delay = 60;
                rep = 1;
                trig = 1;
            } else {
                menu->menu.auto_delay = menu->menu.auto_delay - 1;
            }
        }
        menu = MENU_STATE;
        if (menu->menu.active == 0 || (trig & 2)) {
            AudioCommand_PlayFar(0x71);
            code = (u32)-1;
            break;
        }
        if (trig & 1) {
            u32 sel;
            u32 turns;
            if (cnt != 0) {
                sel = tbl[page + row];
                turns = sel & 0x3e0000;
                if (turns == 0) {
                    if (is_used == 0) {
                        menu->menu.page = page;
                        menu->menu.row = row;
                        menu->menu.preferred_row = saved_row;
                        code = sel;
                        break;
                    }
                } else if (is_used == 0) {
                    Ui_FillVramBlockPattern();
                    UiWork_ClearValueNameTables();
                    UiWork_PushValueSlot((s32)((turns & 0x3e0000) >> 17), 5);
                    UiText_CopyMessageString((s32)MsgBattleDjinnRecovering, buf, TEXT_COUNT);
                    UiText_RenderWideStringAtOffset(buf, msg_win, 0, 4);
                }
                AudioCommand_PlayFar(0x72);
            } else {
                code = (u32)-1;
                break;
            }
        }

        if (cnt != 0) {
            if (rep & 0x80) {
                AudioCommand_PlayFar(0x6f);
                row++;
                if (row == 5 || page + row == cnt) {
                    row = 0;
                }
                saved_row = row;
            } else if (rep & 0x40) {
                AudioCommand_PlayFar(0x6f);
                row--;
                if (row < 0) {
                    if (page == ((cnt - 1) / 5) * 5) {
                        row = cnt - page - 1;
                    } else {
                        row = 4;
                    }
                }
                saved_row = row;
            } else if (opts & 0x100) {
                if (detail_win != 0) {
                    for (i = 0; i < sub_cnt; i++) {
                        s32 tile = i + 0xf301;
                        if ((gFrameCount & 15) <= 11 && i == sub_sel - 1) {
                            tile = sub_sel - 1 + 0xf30b;
                        }
                        UiWindow_SetTilemapEntry((struct RenderInput *)detail_win, tile,
                                      detail_win->width - sub_cnt + i - 2, -1, 0);
                    }
                    UiWindow_SetTilemapEntry((struct RenderInput *)detail_win, 0xf334, detail_win->width - sub_cnt - 3, -1,
                                  0);
                    UiWindow_SetTilemapEntry((struct RenderInput *)detail_win, 0xf335, detail_win->width - 2, -1, 0);
                    UiRender_MarkRow(ctx, detail_win->y);
                }
                if (sub_sel == 0 && sub_cnt != 0) {
                    if (detail_win != 0) {
                        RenderOutput_ClearList(detail_win);
                    }
                    sub_sel = 1;
                    redraw = 1;
                } else {
                    if (sub_sel > sub_cnt) {
                        sub_sel = sub_cnt;
                    }
                    if (sub_sel != 0) {
                        if (rep & 16) {
                            AudioCommand_PlayFar(0x6f);
                            sub_sel++;
                            if (sub_sel > sub_cnt) {
                                sub_sel = 1;
                            }
                            redraw = 1;
                        } else if (rep & 32) {
                            AudioCommand_PlayFar(0x6f);
                            sub_sel--;
                            if (sub_sel <= 0) {
                                sub_sel = sub_cnt;
                            }
                            redraw = 1;
                        }
                    }
                }
            } else if (sub_sel != 0) {
                if (detail_win != 0) {
                    RenderOutput_ClearList(detail_win);
                }
                sub_sel = 0;
                redraw = 1;
            } else if (rep & 16) {
                AudioCommand_PlayFar(0x6f);
                Runtime_SetMainState19();
                if (page + 5 >= cnt) {
                    if (page != 0) {
                        page = 0;
                        row = saved_row;
                    }
                } else {
                    page = page + 5;
                    row = saved_row;
                    if (page == ((cnt - 1) / 5) * 5) {
                        row = cnt - page - 1;
                        if (row > saved_row) {
                            row = saved_row;
                        }
                    }
                }
            } else if (rep & 32) {
                AudioCommand_PlayFar(0x6f);
                Runtime_SetMainState19();
                if (page != 0) {
                    page -= 5;
                    row = saved_row;
                } else {
                    page = ((cnt - 1) / 5) * 5;
                    row = saved_row;
                    if (page != 0) {
                        row = cnt - page - 1;
                        if (row > saved_row) {
                            row = saved_row;
                        }
                    }
                }
            }
        }

        {
            s32 cx;

            /* FAKEMATCH: the one-pass calculation keeps row's reload after
             * the x shift. Without the block, mov r2, r10 fills the ldrh
             * delay slot before lsls (2 differing instruction rows). */
            do {
                cx = list_win->x * 8;
            } while (0);
            /* FAKEMATCH: instruction order: row read in a low register here is reloaded before the subtract, and row * 2 inherits that reload, so mov r2, r10 precedes subs r3, #2 as native (2 rows without; equal priority, and the reload otherwise has the later luid) */
            asm("" : : "l"(row));
            cursor_pos.x = cx - 2;
        }
        cursor_pos.y = (row * 2 + list_win->y) * 8 + 20;
        cur_spr = &slot;
        ((u32 *)cur_spr)[1] = MENU_SPRITE_SIZE_16;
        ((u32 *)cur_spr)[2] = 0;
        cur_spr->tile = Resource_GetBuffer(res, (s32)Resource_FixedBlockBTiles);
        cur_spr->x = cursor_pos.x + ((gFrameCount & 4) >> 1) - 6;
        cur_spr->y = cursor_pos.y - ((gFrameCount & 4) >> 2) - 8;
        if (cnt != 0) {
            Runtime_PushSlotEntry(cur_spr, 242);
        }
        cursor = CURSOR_OWNER;
        blink = gFrameCount & 4;
        icon_win = cursor->window;
        if (cursor->flags & 2) {
            for (i = 0; i <= 3; i++) {
                pal = 15 - (blink != 0);
                if (((1 << i) & mask) == 0) {
                    pal = 15;
                }
                Ui_SetRectHighlight(icon_win->x + DjinnMenu_ElementXOffsets[i] + 1,
                              icon_win->y + DjinnMenu_ElementYOffsets[i] + 1, 2, 2, pal);
            }
        }
        if (gFrameCount & 4) {
            Iwram_CopyWords((void *)0x06006500, DjinnMenu_BlinkTile, 32);
            Iwram_CopyWords((void *)0x06006520, DjinnMenu_BlinkTile, 32);
        } else {
            Iwram_FillWords((void *)0x06006500, 32, 0x44444444);
            Iwram_CopyWords((void *)0x06006520, DjinnMenu_BlinkTile, 32);
        }
        WaitFrames(1);
    }

    if (CURSOR_OWNER->flags & 2) {
        Ui_SetRectHighlight(CURSOR_OWNER->window->x + 1, CURSOR_OWNER->window->y + 1, 4, 4, 15);
    }
    Resource_ResetEntry(res);
    UiWork_Finalize(msg_win, 1);
    UiWork_Finalize(detail_win, 1);
    UiWork_Finalize(list_win, 1);
    UiWindow_MarkVisibleTileAttributes();
    UiWork_SetAltFlagAndClearTable(0);
    MemFree(tbl);
    WaitFrames(1);
    return (s32)code;
}

//--------------------------------------------------------------------------

// classify_use_ability.c

// アイテム：召喚えらびのメニューと、戦闘でアイテムを使えるかの判定

//--------------------------------------------------------------------------

#undef BtlActGet
#undef UiText_DrawCharacterAtOffset
#undef UiWindow_Create
#undef UiWindow_SetTilemapEntry

#define SENTOU_OK 0x80 // tgt_flg：戦闘で使える

#if !GAME_TLA
#include "callback_scheduler.h"
#include "resource.h"
#include "btlsmn.h"
#include "menu_list.h"
#include "io_reg.h"
#include "sound_ids.h"

#if EDITION_INTERNATIONAL
#define SUMMON_COST_X 13 // 必要ジン数の列
#else
#define SUMMON_COST_X 10
#endif

enum
{
	SUMMON_ELEMENT_TILE = 0x5001,		// 属性アイコン
	SUMMON_PAGE_TILE = 0xf301,			// ページ番号
	SUMMON_SELECTED_PAGE_TILE = 0xf30b, // 表示中のページ番号
	SUMMON_LEFT_ARROW_TILE = 0xf334,	// 左矢印
	SUMMON_RIGHT_ARROW_TILE = 0xf335	// 右矢印
};

#define SUMMON_END 32 // 召喚リストの終わり


void UiWindow_PutGlyph(struct UiWindow *, u32, u32, u32, s32);
s32	 Trade_ListFlaggedEntriesFar(u8 *ids);
struct ItemDefinition *Item_GetDirectFar(s32 item);

//--------------------------------------------------------------------------
// 召喚えらび
// 覚えた召喚を 1 ページ 4 つずつ並べ、えらんだ召喚の番号を返す
// （キャンセルは -1）
// standby : 属性ごとのスタンバイ中のジンの数
//--------------------------------------------------------------------------
s32 SummonMenu_SelectSummon(s32 unused0, s32 unused1, const u8 *standby)
{
	struct MenuSprite			   cursor;
	struct MenuSprite			   sprites[4];
	u8							   available[33]; // 覚えた召喚
	u8							   ordered[33];	  // 呼べるものを先にした並び
	u16							   text[64];
	s32							   handles[4];
	u8							   visible[4];
	s32							   glyph;
	s32							   count;
	struct UiWindow				  *window;
	struct UiRenderWork			  *render;
	s32							   drawn_row, drawn_page, row, page;
	s32							   cursor_handle;
	struct UiWindow				  *description, *counts;
	s32							   mask, preferred;
	struct MenuPoint			   cursor_position;
	const struct SummonDefinition *summon;
	struct MenuCell				  *nav;
	struct MenuSprite			  *sprite, *cur;
	s32							   used;
	s32							   id, tile, affordable;
	s32							   pressed, repeated, result;
	s32							   index;

	/* FAKEMATCH: register choice (spill slots) only: a dead store. GCSE
	   sizes its hash table from the function's length and PRE numbers its
	   reaching registers in bucket order; with this store the table has
	   379 buckets and &ordered and &sprites take their spill slots before
	   count - 1, as in the native frame (377 buckets without it: 80 rows
	   differ, every one a spill slot's offset). */
	result = 0;
	render = (struct UiRenderWork *)gWindowWork[0];
	drawn_row = -1;
	drawn_page = -1;
	cursor_handle = (s16)Resource_LoadIntoFreeSlot(128); // 16x16 スプライト 1 枚分
	description = UiWindow_Create(0, 4, 30, 4, 42);
	counts = UiWindow_Create(20, 8, 10, 3, 6);
	mask = 0;
	MenuCell_GetCursor(page, row, preferred);
#if EDITION_INTERNATIONAL
	window = UiWindow_Create(13, 11, 17, 9, 6);
#else
	window = UiWindow_Create(16, 11, 14, 9, 6);
#endif

	// 行ごとのアイコンのスプライト
	for (index = 0; index < 4; index++)
	{
		s32 y = index * 2;

		sprite = &sprites[index];
		MenuSprite_Init(sprites[index], MENU_SPRITE_SIZE_16);
		sprite->oam.f.x = window->x * 8 + 8;
		sprite->oam.f.y = (y + window->y) * 8 + 4;
	}
	for (index = 0; index < 4; index++)
	{
		s32 slot = Resource_LoadIntoFreeSlot(128);
		handles[index] = slot;
		sprites[index].oam.f.tile = Resource_GetBuffer(slot, -1);
	}

	// 今のスタンバイ数で呼べる召喚を後ろから順に先頭へ、残りをもとの順で後ろへ
	count = Trade_ListFlaggedEntriesFar(available);
	/* FAKEMATCH: instruction order only (block placement and schedule):
	   with the first scan's exit inside this one-pass block, loop.c's
	   find_and_verify_loops finds no barrier at depth 0 and leaves the
	   main loop's select block in place, as the native has it (without
	   the block it moves after the scan: differ 224, 6 branches fewer);
	   inside it, count and used are stored before count - 1 is built
	   (differ 2 with used = 0 before the block). */
	do
	{
		s32 i;

		used = 0;
		for (i = count - 1; i >= 0; i--)
		{
			const struct SummonDefinition *s;
			s32							   n, k;

			k = available[i];
			s = SummonDefinition_Get(k);
			for (n = 0; n <= 3; n++)
			{
				if (s->djinn_required[n] > standby[n])
					break;
			}
			if (n == 4)
			{
				ordered[used++] = k;
				available[i] = SUMMON_END;
			}
		}
	} while (0);
	{
		s32 i, k;

		for (i = 0; i < count; i++)
		{
			k = available[i];
			if (k != SUMMON_END)
				ordered[used++] = k;
		}
	}
	ordered[used] = SUMMON_END;

	for (;;)
	{
		cur = &cursor;

		//------------------------------------------------------------------
		// ページか行が変わったら描きなおす
		//------------------------------------------------------------------
		if (page != drawn_page || row != drawn_row)
		{
			render->menu_busy = 1;
			Ui_SetRectHighlight(
				window->x + 1, window->y + drawn_row * 2 + 1, window->width - 2, 1, 15);
			Ui_FillVramBlockPattern();

			// 説明文と、必要なジンの属性
			{
				const struct SummonDefinition *s;
				s32							   element;

				s = SummonDefinition_Get(ordered[page + row]);
#if EDITION_INTERNATIONAL
				UiText_CopyMessageString(
					s->name_message_id + (s32)&MsgAbilityDescription, text, 52);
#else
				UiText_DecodeMessage(s->name_message_id + (s32)&MsgAbilityDescription, text, 32);
#endif
				UiText_RenderWideStringAtOffset(text, (struct UiWindow *)description, 0, 4);
				drawn_row = row;
				mask = 0;
				for (element = 0; element <= 3; element++)
				{
					if (s->djinn_required[element] != 0)
						mask |= 1 << element;
				}
			}

			// スタンバイ数と、ページの召喚 4 つ
			if (page != drawn_page)
			{
				RenderOutput_RedrawSavedRect(window);
				{
					s32 element;

					for (element = 0; element <= 3; element++)
					{
						UiWindow_SetTilemapEntry(
							counts, element + SUMMON_ELEMENT_TILE, element * 2, 0, 0);
						UiWindow_PutGlyph(counts, standby[element] + '0', element * 2 + 1, 0, 0);
					}
				}
				{
					s32 element, col, y;

					index = 0;
					if ((id = ordered[page + index]) != SUMMON_END)
					{
					next:
						summon = SummonDefinition_Get(id);
						col = 0;
						for (element = 0; element <= 3; element++)
						{
							if (summon->djinn_required[element] > standby[element])
								break;
						}
						affordable = element == 4;
						Ability_LoadGlyph(
							summon->name_message_id & 0x3fff, 0, &handles[index], &glyph, 1);
						y = index * 2;
						sprites[index].oam.f.tile = glyph;
						if (!affordable)
							UiWork_SetParamNibble(2); // 呼べない召喚は色 2
						UiText_DrawCharacterAtOffset(
							SummonDefinition_Get(id)->name_message_id + (s32)&MsgAbilityName,
							(struct RenderInput *)window, 16, index * 16);
						for (element = 0; element <= 3; element++)
						{
							if (summon->djinn_required[element] != 0)
							{
								UiWindow_SetTilemapEntry(window, element + SUMMON_ELEMENT_TILE,
									col * 2 + SUMMON_COST_X, y, 0);
								UiWindow_PutGlyph(window, summon->djinn_required[element] + '0',
									col * 2 + SUMMON_COST_X + 1, y, 0);
								col++;
							}
						}
						UiWork_SetParamNibble(15);
						visible[index++] = 1;
						if (index <= 3 && (id = ordered[page + index]) != SUMMON_END)
							goto next;
					}
					while (index <= 3)
						visible[index++] = 0;
				}
				drawn_page = page;
			}

			// ページ番号
			if (count > 4)
			{
				for (index = 0; index < (count + 3) / 4; index++)
				{
					tile = index + SUMMON_PAGE_TILE;
					if (index == page / 4)
						tile = index + SUMMON_SELECTED_PAGE_TILE;
					UiWindow_SetTilemapEntry(
						window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
				}
			}
			Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1, window->width - 2, 1, 14);
			render->dirty = 1;
			render->menu_busy = 0;
		}

		//------------------------------------------------------------------
		// 毎フレームの表示
		//------------------------------------------------------------------
		for (index = 0; index <= 3; index++)
		{
			if (visible[index] != 0)
				Runtime_PushSlotEntry((s32 *)&sprites[index], 240);
		}

		// カーソルは 4 フレームごとに揺れる
		cursor_position.x = window->x * 8 - 2;
		cursor_position.y = (row * 2 + window->y) * 8 + 20;
		/* FAKEMATCH: instruction order only: as u32 words, not through the
		   union, these stores do not alias cursor_position, so cur is
		   reloaded before cursor_position.y is stored (differ 2). */
		((u32 *)cur)[1] = MENU_SPRITE_SIZE_16;
		((u32 *)cur)[2] = 0;
		cur->oam.f.tile = Resource_GetBuffer((u16)cursor_handle, (s32)Resource_FixedBlockBTiles);
		cur->oam.f.x = cursor_position.x + ((gFrameCount & 4) >> 1) - 4;
		cur->oam.f.y = cursor_position.y - ((gFrameCount & 4) >> 2) + 248;
		sprite = &cursor;
		Runtime_PushSlotEntry((s32 *)sprite, 242);

		// 必要な属性のスタンバイ数を 8 フレームごとに点滅させる
		{
			s32 element, tile, blink;

			blink = gFrameCount & 8;
			for (element = 0; element <= 3; element++)
			{
				tile = 15 - (blink != 0);
				if ((1 << element & mask) == 0)
					tile = 15;
				Ui_SetRectHighlight(counts->x + element * 2 + 1, counts->y + 1, 2, 1, tile);
			}
		}

		// ページ番号（表示中のものは点滅）と左右の矢印
		if (count > 4)
		{
			for (index = 0; index < (count + 3) / 4; index++)
			{
				tile = index + SUMMON_PAGE_TILE;
				if ((gFrameCount & 15) <= 11 && index == page / 4)
					tile = index + SUMMON_SELECTED_PAGE_TILE;
				UiWindow_SetTilemapEntry(
					window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
			}
			UiWindow_SetTilemapEntry(
				window, SUMMON_LEFT_ARROW_TILE, window->width - (count + 3) / 4 - 3, -1, 0);
			UiWindow_SetTilemapEntry(window, SUMMON_RIGHT_ARROW_TILE, window->width - 2, -1, 0);
			UiRender_MarkRow(render, window->y - 1);
		}

		//------------------------------------------------------------------
		// 入力
		//------------------------------------------------------------------
		nav = gLinkCountdownWork;
		/* FAKEMATCH: instruction order only: page, row and preferred are
		   stored in source order before the keys are read (differ 4). */
		do
		{
			nav->page = page;
			nav->row = row;
			nav->preferred_row = preferred;
		} while (0);
		pressed = gKeyState;
		repeated = gKeysRepeat;

		// 自動入力中は 120 フレームごとに決定する
		if (nav->auto_enabled != 0)
		{
			repeated = 0;
			pressed = 0;
			if (nav->auto_delay == 0)
			{
				nav->auto_delay = 120;
				repeated = KEY_A;
				pressed = KEY_A;
			}
			else
			{
				nav->auto_delay--;
			}
		}
		if (pressed & KEY_A)
		{
			result = ordered[page + row];
			break;
		}
		if (gLinkCountdownWork->active == 0 || (pressed & KEY_B))
		{
			AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			result = -1;
			break;
		}
		if (repeated & KEY_DOWN)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			row++;
			if (row == 4 || page + row == count)
				row = 0;
			preferred = row;
		}
		else if (repeated & KEY_UP)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			row--;
			if (row < 0)
			{
				if (page == ((count - 1) / 4) * 4)
					row = count - page - 1; // 最後のページ
				else
					row = 3;
			}
			preferred = row;
		}
		else if (repeated & KEY_RIGHT)
		{
			// 次のページ（最後からは先頭へ）
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			Runtime_SetMainState19();
			if (page + 4 >= count)
			{
				if (page != 0)
				{
					page = 0;
					row = preferred;
				}
			}
			else
			{
				page += 4;
				row = preferred;
				if (page == ((count - 1) / 4) * 4)
				{
					row = count - page - 1;
					if (row > preferred)
						row = preferred;
				}
			}
		}
		else if (repeated & KEY_LEFT)
		{
			// 前のページ（先頭からは最後へ）
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			Runtime_SetMainState19();
			if (page != 0)
			{
				page -= 4;
				row = preferred;
			}
			else
			{
				page = ((count - 1) / 4) * 4;
				row = preferred;
				if (page != 0)
				{
					row = count - page - 1;
					if (row > preferred)
						row = preferred;
				}
			}
		}
		WaitFrames(1);
	}

	// 後かたづけ
	WaitFrames(1);
	for (index = 0; index < 4; index++)
		Resource_ResetEntry(handles[index]);
	Resource_ResetEntry((u16)cursor_handle);
	UiWork_Finalize(counts, 1);
	UiWork_Finalize(description, 1);
	UiWork_Finalize(window, 1);
	WaitFrames(1);
	return result;
}
#endif

//--------------------------------------------------------------------------
// 戦闘でアイテムを使えるか
// 戻り値 : 0 使える / 1 使えない / 2 戦闘では使えない
// unit   : キャラクター番号
// item   : 持ち物の値
//--------------------------------------------------------------------------
s32 Item_ClassifyUseAbility(s32 unit, s32 item)
{
	s32					   ret;
	struct ItemDefinition *def;

	if (item == 0)
	{
		return 1;
	}
#if GAME_TLA
	def = Item_Get(item);
#else
	def = Item_GetDirectFar(item);
#endif
	ret = 1;
	if (def->use_type == 3) // エナジーを授ける装備
	{
		return ret;
	}
	if (def->action_id == 0)
	{
		return ret;
	}
	if ((def->type != 0) && (Item_CanOwnerEquip(unit, item) == 0))
	{
		return ret;
	}
	if ((SENTOU_OK & BtlActGet(def->action_id)->tgt_flg) == 0)
	{
		return 2;
	}
	return 0;
}

#undef UiWindow_DrawThreeTileColumn

void Vram_CopyTile(u32 source_tile, u32 destination_tile)
{
    Dma_Set((u8 *)0x06000000 + (source_tile & 0x3ff) * 32,
            (u8 *)0x06000000 + (destination_tile & 0x3ff) * 32,
            0x84000008, REG_DMA3);
}

struct ItemDefinition *Item_GetDirectFar(s32 item);

enum BattleListTile {
    LIST_TILE_PAGE_NUMBER = 0xf301,
    LIST_TILE_PAGE_NUMBER_LIT = 0xf30b,
    LIST_TILE_PAGES_LEFT = 0xf334,
    LIST_TILE_PAGES_RIGHT = 0xf335
};

s32 ItemList_SelectEntry(s32 owner, u16 *entries, s32 count)
{
    struct UiRenderWork *render;
    s32 drawn_page;
    s32 drawn_row;
    s32 cursor_handle;
    struct UiWindow *message_window;
    s32 icon_count;
    s32 preferred_row;
    struct MenuPoint pos;
    struct MenuCell *cell;
    struct UiWindow *window;
    s32 page;
    s32 row;
    s32 entry;
    s32 tile;
    s32 edge;
    s32 reason;
    s32 result;
    s32 i;
    struct CursorSprite cursor;
    struct MenuSprite *spr;
    struct CursorSprite *cur;
    u16 buffer[64];
    s32 handles[BATTLE_LIST_ROWS];
    struct MenuSprite sprites[BATTLE_LIST_ROWS];

    render = (struct UiRenderWork *)gWindowWork[0];
    drawn_page = -1;
    drawn_row = -1;
    cursor_handle = Resource_LoadIntoFreeSlot(128);
    message_window = UiWindow_Create(0, 5, 30, 4, 42);
    icon_count = 0;
    MenuCell_GetCursor(page, row, preferred_row);
    window = UiWindow_Create(15, 9, 15, BATTLE_LIST_ROWS * 2 + 1, 6);

    for (i = 0; i < BATTLE_LIST_ROWS; i++) {
        s32 y = i * 2;

        spr = &sprites[i];
        MenuSprite_Init(sprites[i], MENU_SPRITE_SIZE_16);
        spr->oam.f.x = window->x * 8 + 8;
        spr->oam.f.y = (y + window->y) * 8 + 4;
    }

    for (i = 0; i < BATTLE_LIST_ROWS; i++) {
        handles[i] = Resource_LoadIntoFreeSlot(128);
        sprites[i].oam.f.tile = Resource_GetBuffer(handles[i], -1);
    }

    Vram_CopyTile(0xf018, 0x200);
    Vram_CopyTile(0xf018, 0x201);
    Vram_CopyTile(0xf019, 0x210);
    Vram_CopyTile(0xf019, 0x211);

    cur = &cursor;
    for (;;) {
        if (page != drawn_page || row != drawn_row) {
            render->menu_busy = 1;
            Ui_SetRectHighlight(
                window->x + 1,
                window->y + drawn_row * 2 + 1,
                window->width - 2,
                1,
                15);
            Ui_FillVramBlockPattern();
            if (count != 0) {
                if (Item_ClassifyUseAbility(owner, entries[page + row]) == 2) {
#if EDITION_INTERNATIONAL
                    UiText_CopyMessageString((s32)MsgBattleItemUnavailable, buffer, 52);
#else
                    UiText_DecodeMessage((s32)MsgBattleItemUnavailable, buffer, 32);
#endif
                } else {
#if EDITION_INTERNATIONAL
                    UiText_CopyMessageString(
                        (entries[page + row] & ITEM_ID_MASK) + (s32)MsgItemPlainName,
                        buffer,
                        52);
#else
                    UiText_DecodeMessage(
                        (entries[page + row] & ITEM_ID_MASK) + (s32)MsgItemPlainName,
                        buffer,
                        32);
#endif
                }
            } else {
#if EDITION_INTERNATIONAL
                UiText_CopyMessageString((s32)MsgBattleNoItems, buffer, 52);
#else
                UiText_DecodeMessage((s32)MsgBattleNoItems, buffer, 32);
#endif
            }
            UiText_RenderWideStringAtOffset(buffer, message_window, 0, 4);
            drawn_row = row;

            if (page != drawn_page) {
                RenderOutput_RedrawSavedRect(window);
                for (i = 0; i < BATTLE_LIST_ROWS; i++) {
                    entry = entries[page + i];
                    if (entry == 0) {
                        break;
                    }
                    Item_GetDirectFar(entry);
                    UiWork_SetParamNibble(15);
                    if (Item_ClassifyUseAbility(owner, entry) != 0) {
                        UiWork_SetParamNibble(4);
                    } else if ((entry & INVENTORY_BROKEN) != 0) {
                        UiWork_SetParamNibble(2);
                    }
                    UiText_DrawCharacterAtOffset(
                        (entry & ITEM_ID_MASK) + (s32)MsgItemName,
                        (struct RenderInput *)window, 16, i * 16);
                    UiWork_SetParamNibble(15);
                    sprites[i].oam.f.tile =
                        Resource_LoadKind26EntryToBuffer(entry, handles[i]);
                }
                icon_count = i;
                drawn_page = page;
            }

            if (count > BATTLE_LIST_ROWS) {
                for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
                    tile = i + LIST_TILE_PAGE_NUMBER;
                    if (i == page / BATTLE_LIST_ROWS) {
                        tile = i + LIST_TILE_PAGE_NUMBER_LIT;
                    }
                    UiWindow_SetTilemapEntry(
                        window,
                        tile,
                        window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2,
                        -1,
                        0);
                }
            }

            Ui_SetRectHighlight(
                window->x + 1,
                window->y + row * 2 + 1,
                window->width - 2,
                1,
                14);
            render->dirty = 1;
            render->menu_busy = 0;
        }

        if (count > BATTLE_LIST_ROWS) {
            for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
                tile = i + LIST_TILE_PAGE_NUMBER;
                if ((gFrameCount & 15) <= 11) {
                    if (i == page / BATTLE_LIST_ROWS) {
                        tile = i + LIST_TILE_PAGE_NUMBER_LIT;
                    }
                }
                UiWindow_SetTilemapEntry(
                    window,
                    tile,
                    window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2,
                    -1,
                    0);
            }
            edge = -1;
            UiWindow_SetTilemapEntry(
                window, LIST_TILE_PAGES_LEFT, window->width - (count + 4) / BATTLE_LIST_ROWS - 3, edge, 0);
            UiWindow_SetTilemapEntry(
                window, LIST_TILE_PAGES_RIGHT, window->width - 2, edge, 0);
            UiRender_MarkRow(render, window->y - 1);
            /* FAKEMATCH: the cursor pointer pair preserves the native
             * spill slots (42 rows differ without the pair); it keeps
             * all 770 instructions in every edition. */
            cur++;
            cur--;
        }

        for (i = 0; i < icon_count; i++) {
            spr = &sprites[i];
            Runtime_PushSlotEntry(spr, 240);
        }

        pos.x = window->x * 8 - 2;
        pos.y = (row * 2 + window->y) * 8 + 20;
        /* FAKEMATCH: re-pointing cur each frame orders the spill slots of the cursor address and row*2 as the native frame does. */
        cur = &cursor;
        ((u32 *)cur)[1] = MENU_SPRITE_SIZE_16;
        ((u32 *)cur)[2] = 0;
        cur->tile =
            Resource_GetBuffer(cursor_handle, (s32)Resource_FixedBlockBTiles);
        cur->x = pos.x + ((gFrameCount & 4) >> 1) - 4;
        cur->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
        if (count != 0) {
            Runtime_PushSlotEntry(cur, 242);
        }

        cell = gLinkCountdownWork;
        cell->page = page;
        cell->row = row;
        /* FAKEMATCH: a one-pass block finishes these stores before input is read. The duplicate final store is removed by flow but preserves the native spill-slot order; with one store, 42 instructions differ only in their stack offsets. */
        cell->preferred_row = preferred_row;
        cell->preferred_row = preferred_row;
        do { } while (0);

        if ((gKeyState & KEY_A) != 0) {
            if (count != 0) {
                result = page + row;
                Item_GetDirectFar(entries[result]);
                reason = 0;
                if ((entries[result] & INVENTORY_BROKEN) == 0) {
                    reason = Item_ClassifyUseAbility(owner, entries[result]);
                    if (reason == 0) {
                        break;
                    }
                }
                AudioCommand_PlayFar(114);
                if (reason == 2) {
#if EDITION_INTERNATIONAL
                    UiText_CopyMessageString((s32)MsgBattleItemUnavailable, buffer, 52);
#else
                    UiText_DecodeMessage((s32)MsgBattleItemUnavailable, buffer, 32);
#endif
                } else if ((entries[result] & INVENTORY_BROKEN) != 0) {
#if EDITION_INTERNATIONAL
                    UiText_CopyMessageString((s32)MsgBattleItemBroken, buffer, 52);
#else
                    UiText_DecodeMessage((s32)MsgBattleItemBroken, buffer, 32);
#endif
                } else {
#if EDITION_INTERNATIONAL
                    UiText_CopyMessageString((s32)MsgBattleItemNoEffect, buffer, 52);
#else
                    UiText_DecodeMessage((s32)MsgBattleItemNoEffect, buffer, 32);
#endif
                }
                Ui_FillVramBlockPattern();
                UiText_RenderWideStringAtOffset(buffer, message_window, 0, 4);
            } else {
                result = -1;
                break;
            }
        } else if (cell->active == 0 || (gKeyState & KEY_B) != 0) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }

        if (count != 0) {
            if ((gKeysRepeat & KEY_DOWN) != 0) {
                AudioCommand_PlayFar(111);
                row++;
                if (row == BATTLE_LIST_ROWS || page + row == count) {
                    row = 0;
                }
                preferred_row = row;
            } else if ((gKeysRepeat & KEY_UP) != 0) {
                AudioCommand_PlayFar(111);
                row--;
                if (row < 0) {
                    if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
                        row = count - page - 1;
                    } else {
                        row = BATTLE_LIST_ROWS - 1;
                    }
                }
                preferred_row = row;
            } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page + BATTLE_LIST_ROWS >= count) {
                    if (page != 0) {
                        page = 0;
                        row = preferred_row;
                    }
                } else {
                    page += BATTLE_LIST_ROWS;
                    row = preferred_row;
                    if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
                        row = count - page - 1;
                        if (row > preferred_row) {
                            row = preferred_row;
                        }
                    }
                }
            } else if ((gKeysRepeat & KEY_LEFT) != 0) {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page != 0) {
                    page -= BATTLE_LIST_ROWS;
                    row = preferred_row;
                } else {
                    page = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
                    row = preferred_row;
                    if (page != 0) {
                        row = count - page - 1;
                        if (row > preferred_row) {
                            row = preferred_row;
                        }
                    }
                }
            }
        }
        WaitFrames(1);
    }

    UiWork_Finalize(message_window, 1);
    UiWork_Finalize(window, 1);
    WaitFrames(1);
    Resource_ResetEntry(cursor_handle);
    for (i = 0; i < BATTLE_LIST_ROWS; i++) {
        Resource_ResetEntry(handles[i]);
    }
    WaitFrames(1);
    return result;
}

#define UiWindow_DrawThreeTileColumn ((void (*)(struct UiWindow*,s32,s32,s32,s32))UiWindow_DrawThreeTileColumn)

#define ACTION_ID_MASK 0x3fff

s32 BattleMenu_RunActionSelection(s32 unit_id, u16 *actions, s32 count)
{
    struct UiRenderWork *render;
    s32 drawn_page;
    s32 page;
    s32 drawn_row;
    s32 cursor_handle;
    struct BtlUnit *unit;
    struct UiWindow *message_window;
    s32 icon_count;
    s32 preferred_row;
    struct MenuPoint pos;
    struct MenuCell *nav;
    struct UiWindow *window;
    s32 row;
    s32 entry;
    s32 tile;
    s32 edge;
    s32 range;
    s32 result;
    s32 i;
    struct MenuSprite cursor;
    struct MenuSprite *spr;
    u16 buffer[64];
    s32 handles[BATTLE_LIST_ROWS];
    struct MenuSprite sprites[BATTLE_LIST_ROWS];
    s32 glyph;

    render = (struct UiRenderWork *)gWindowWork[0];
    drawn_row = -1;
    drawn_page = -1;
    cursor_handle = Resource_LoadIntoFreeSlot(128);
    unit = UnitGetFar(unit_id);
    message_window = UiWindow_Create(0, 5, 30, 4, 42);
    icon_count = BATTLE_LIST_ROWS;
    MenuCell_GetCursor(page, row, preferred_row);
    window = UiWindow_Create(9, 9, 21, BATTLE_LIST_ROWS * 2 + 1, 6);

    for (i = 0; i < BATTLE_LIST_ROWS; i++) {
        s32 y = i * 2;

        spr = &sprites[i];
        MenuSprite_Init(sprites[i], MENU_SPRITE_SIZE_16);
        spr->oam.f.x = window->x * 8 + 8;
        spr->oam.f.y = (y + window->y) * 8 + 4;
    }

    for (i = 0; i < BATTLE_LIST_ROWS; i++) {
        handles[i] = Resource_LoadIntoFreeSlot(128);
        sprites[i].oam.f.tile = Resource_GetBuffer(handles[i], -1);
    }

    Vram_CopyTile(0xf018, 0x200);
    Vram_CopyTile(0xf018, 0x201);
    Vram_CopyTile(0xf019, 0x210);
    Vram_CopyTile(0xf019, 0x211);

    spr = &cursor;
    for (;;) {
        if (page != drawn_page || row != drawn_row) {
            render->menu_busy = 1;
            Ui_SetRectHighlight(
                window->x + 1,
                window->y + drawn_row * 2 + 1,
                window->width - 2,
                1,
                15);
            Ui_FillVramBlockPattern();
            if (count != 0) {
#if EDITION_INTERNATIONAL
                UiText_CopyMessageString(
                    (actions[page + row] & ACTION_ID_MASK) +
                        (s32)MsgAbilityDescription,
                    buffer,
                    52);
#else
                UiText_DecodeMessage(
                    (actions[page + row] & ACTION_ID_MASK) +
                        (s32)MsgAbilityDescription,
                    buffer,
                    32);
#endif
            } else {
#if EDITION_INTERNATIONAL
                UiText_CopyMessageString(MSGNO(MsgBattleNoPsynergy), buffer, 52);
#else
                UiText_DecodeMessage(MSGNO(MsgBattleNoPsynergy), buffer, 32);
#endif
            }
            UiText_RenderWideStringAtOffset(buffer, message_window, 0, 4);
            drawn_row = row;

            if (page != drawn_page) {
                RenderOutput_RedrawSavedRect(window);
                for (i = 0; i < BATTLE_LIST_ROWS; i++) {
                    struct BtlAct *action;

                    entry = actions[page + i];
                    if (entry == 0) {
                        break;
                    }
                    action = BtlActGet(entry);
                    UiWindow_SetTilemapEntry(window, 0xf01f, 11, i * 2, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                    UiWindow_SetTilemapEntry(window, 0xf005, 12, i * 2, 0);
#else
                    UiWindow_SetTilemapEntry(window, 0xf01e, 12, i * 2, 0);
#endif
                    Ability_LoadGlyph(
                        entry & ACTION_ID_MASK, 0, &handles[i], &glyph, 1);
                    sprites[i].oam.f.tile = glyph;

                    if ((action->tgt_flg & ACTION_USABLE_IN_BATTLE) == 0) {
                        UiWork_SetParamNibble(4);
                    } else if (action->pp_cost > unit->ep) {
                        UiWork_SetParamNibble(2);
                    } else if (unit->fuuji != 0) {
                        UiWork_SetParamNibble(9);
                    }

                    render->level = 5;
                    UiText_DrawCharacterAtOffset(
                        entry + (s32)MsgAbilityName,
                        (struct RenderInput *)window, 16, i * 16);
                    UiText_DrawNumberAtOffset(
                        action->pp_cost, 2, (struct RenderInput *)window,
                        104, i * 16);
                    UiWork_SetParamNibble(15);
                    render->level = 15;

                    if (action->damage_class != 4) {
                        UiWindow_SetTilemapEntry(
                            window, action->damage_class + 0x5001, 15, i * 2, 0);
                    }

                    range = action->rng;
                    if (range == 255) {
                        range = 11;
                    } else {
                        range--;
                    }
                    UiWindow_DrawThreeTileColumn(window, 16, i * 2, range, 0);
                }
                icon_count = i;
                drawn_page = page;
            }

            if (count > BATTLE_LIST_ROWS) {
                for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
                    tile = i + 0xf301;
                    if (i == page / BATTLE_LIST_ROWS) {
                        tile = i + 0xf30b;
                    }
                    UiWindow_SetTilemapEntry(
                        window,
                        tile,
                        window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2,
                        -1,
                        0);
                }
            }

            Ui_SetRectHighlight(
                window->x + 1,
                window->y + row * 2 + 1,
                window->width - 2,
                1,
                14);
            render->dirty = 1;
            render->menu_busy = 0;
        }

        if (count > BATTLE_LIST_ROWS) {
            for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
                tile = i + 0xf301;
                if ((gFrameCount & 15) <= 11) {
                    if (i == page / BATTLE_LIST_ROWS) {
                        tile = i + 0xf30b;
                    }
                }
                UiWindow_SetTilemapEntry(
                    window,
                    tile,
                    window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2,
                    -1,
                    0);
            }
            edge = -1;
            UiWindow_SetTilemapEntry(
                window, 0xf334, window->width - (count + 4) / BATTLE_LIST_ROWS - 3, edge, 0);
            UiWindow_SetTilemapEntry(
                window, 0xf335, window->width - 2, edge, 0);
            UiRender_MarkRow(render, window->y - 1);
        }

        for (i = 0; i < icon_count; i++) {
            Runtime_PushSlotEntry(&sprites[i], 240);
        }

        pos.x = window->x * 8 - 4;
        pos.y = (row * 2 + window->y) * 8 + 20;
        ((u32 *)spr)[1] = 0x40000000;
        ((u32 *)spr)[2] = 0;
        /* FAKEMATCH: this dead duplicate is needed to match the native spill
         * slots; removing it changes 42 aligned instructions. */
        ((u32 *)spr)[2] = 0;
        spr->oam.f.tile =
            Resource_GetBuffer(cursor_handle, (s32)Resource_FixedBlockBTiles);
        spr->oam.f.x = pos.x + ((gFrameCount & 4) >> 1) - 4;
        spr->oam.f.y = pos.y - ((gFrameCount & 4) >> 2) - 8;
        if (count != 0) {
            Runtime_PushSlotEntry(spr, 242);
        }

        nav = gLinkCountdownWork;
        nav->page = page;
        nav->row = row;
        nav->preferred_row = preferred_row;

        if ((gKeyState & KEY_A) != 0) {
            if (count != 0) {
                result = page + row;
                if ((BtlActGet(actions[result])->tgt_flg & ACTION_USABLE_IN_BATTLE) != 0) {
                    break;
                }
                /* FAKEMATCH: a dead store here keeps the test above from being threaded past the break. */
                result = -1;
            } else {
                result = -1;
                break;
            }
        } else if (nav->active == 0 || (gKeyState & KEY_B) != 0) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }

        if (count != 0) {
            if ((gKeysRepeat & KEY_DOWN) != 0) {
                AudioCommand_PlayFar(111);
                row++;
                if (row == BATTLE_LIST_ROWS || page + row == count) {
                    row = 0;
                }
                preferred_row = row;
            } else if ((gKeysRepeat & KEY_UP) != 0) {
                AudioCommand_PlayFar(111);
                row--;
                if (row < 0) {
                    if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
                        row = count - page - 1;
                    } else {
                        row = BATTLE_LIST_ROWS - 1;
                    }
                }
                preferred_row = row;
            } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page + BATTLE_LIST_ROWS >= count) {
                    if (page != 0) {
                        page = 0;
                        row = preferred_row;
                    }
                } else {
                    page += BATTLE_LIST_ROWS;
                    row = preferred_row;
                    if (page ==
                        (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
                        row = count - page - 1;
                        if (row > preferred_row) {
                            row = preferred_row;
                        }
                    }
                }
            } else if ((gKeysRepeat & KEY_LEFT) != 0) {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page != 0) {
                    row = preferred_row;
                    page -= BATTLE_LIST_ROWS;
                } else {
                    page = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
                    row = preferred_row;
                    if (page != 0) {
                        row = count - page - 1;
                        if (row > preferred_row) {
                            row = preferred_row;
                        }
                    }
                }
            }
        }
        WaitFrames(1);
    }

    UiWork_Finalize(message_window, 1);
    UiWork_Finalize(window, 1);
    WaitFrames(1);
    for (i = 0; i < BATTLE_LIST_ROWS; i++) {
        Resource_ResetEntry(handles[i]);
    }
    Resource_ResetEntry(cursor_handle);
    WaitFrames(1);
    return result;
}

struct CursorSlot {
    u8 x;
    u8 y;
    u8 flags;
    s8 index;
};

s32 BattleMotion_ProjectConditionalPositionFar(s32 id, s32 *out);
s32 UiText_GetWideStringWidth(u16 *text);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
s32 System_GetLanguage(void);
#endif

s32 BattleTarget_RunSelection(s32 sel, s32 mode, u32 spread, u32 kind)
{
    u16 ids[8];
    struct SelectionSprite entries[6];
    struct CursorSlot tbl[6];
    struct EffectPosition markPos;
    u16 selIds[14];
    u8 selSlot[8];
    struct EffectPosition pos;
    u16 name[15];
    struct EffectPosition namePos;
    struct EffectPosition targetPos;
    struct AffineTransform efx;
    struct BtlUnit *unit;
    struct BtlWork *runtime;
    struct UiWindow *window;
    /* FAKEMATCH: the earlier combined pin/barrier removal gave EN 1585/1585, common 1343, differ 484: infoWin spills and the frame shrinks 324 -> 320. ES 1684/1684, common 1569, differ 230: infoWin/sel swap r10/r9 and spill slots permute. Both extents stayed unchanged. */
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    struct UiWindow *infoWin;
#else
    /* FAKEMATCH: register choice only: as an ordinary local the info window is spilled and the frame shrinks to 320 bytes (differ 492); the native keeps it in r9. 2026-10-11: its live length is doubled by update_equiv_regs (the first set carries a constant REG_EQUAL), giving priority 2104 against &pos 2171 and the hoisted bit-field zero 2142; one duplicated no-effect block reaches 2143 against 2149 (differ 202), two push &pos below the zero (differ 449), and an initializer without the note sends it to r10 as in ES/IT (differ 96). */
    register struct UiWindow *infoWin asm("r9");
#endif
    s32 cursor;
    s32 cnt;
    s32 total;
    s32 redraw;
    s32 slotId;
    s32 pending;
    s32 matrix;
    s32 found;
    s32 column;
    s32 rows;
    s32 count;
    s32 i;
    s32 j;
    s32 n;
    s32 nx;
    s32 ny;
    s32 pressed;
    s32 y;
    s32 width;
    s32 repeat;
    s32 result;
    struct CursorSlot *slot;
    struct SelectionSprite *entry;
    struct SelectionSprite *head;
    struct BtlUnit *who;

    runtime = BtlWk;
    cnt = 0;
    redraw = 0xFFFF;
    slotId = Resource_LoadIntoFreeSlot(256);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    /* FAKEMATCH: this value-preserving null initializer is needed to match
     * infoWin in r10; a direct zero swaps high registers and spill slots. */
    infoWin = (struct UiWindow *)((u32)(u16)sel >> 16);
#else
    infoWin = 0;
#endif
    if (spread == 0)
        spread = 1;
    if (mode == 2 || mode == 4)
        ((struct MenuCell **)&BtlWk)[48]->slide_offset = -2;
    else
        ((struct MenuCell **)&BtlWk)[48]->slide_offset = 16;
    for (i = 0; i < 6; i++)
        tbl[i].flags = 0;
    cursor = -1;
    if (mode == 2) {
        for (i = 0; i <= 5 && runtime->party_units[i] != 0xFF; i++) {
            ids[cnt] = runtime->party_units[i];
            cnt++;
        }
    } else if (mode == 4) {
        ids[0] = (u16)sel;
        cnt = 1;
    } else {
        for (i = 0; i <= 5 && runtime->enm_no[i] != 0xFF; i++) {
            ids[cnt] = runtime->enm_no[i];
            cnt++;
        }
    }
    ids[cnt] = 0xFF;
    total = cnt;
    if (mode == 2) {
        if (spread != 0xFF && kind != 0) {
            found = 0;
            for (j = 0; j < cnt; j++) {
                struct BtlUnit *st;

                if (ids[j] == 0xFE)
                    continue;
                st = UnitGetFar(ids[j]);
                switch (kind) {
                case 5:
                    if (st->hp == 0) {
                        sel = ids[j];
                        found = 1;
                    }
                    break;
                case 3:
                    if (st->doku != 0)
                        found = 1;
                    break;
                case 4:
                    if (st->maboroshi != 0 || st->shibire != 0
                        || st->nemuri != 0 || st->fuuji != 0
                        || st->shiryo != 0)
                        found = 1;
                    break;
                case 6:
                    if (st->maboroshi != 0 || st->shibire != 0
                        || st->nemuri != 0 || st->fuuji != 0
                        || st->shiryo != 0 || st->doku != 0
                        || st->akuryo != 0)
                        found = 1;
                    break;
                }
                if (found != 0) {
                    sel = ids[j];
                    break;
                }
            }
        }
        for (j = 0; j < cnt; j++) {
            if (ids[j] == sel)
                break;
        }
        if (j != cnt)
            cursor = j;
    }
    if (cursor < 0)
        cursor = (cnt - 1) / 2;
    while (ids[cursor] == 0xFE
           || (GameFlag_TestFar(0x16C) && mode == 1 && UnitGetFar(ids[cursor])->hp == 0)) {
        cursor = cursor + cnt - 1;
        cursor %= cnt;
    }
    if (mode != 2) {
        BattleMotion_ProjectConditionalPositionFar(sel, (s32 *)&markPos);
        tbl[0].flags = 8;
        tbl[0].x = (u8)markPos.x;
        tbl[0].y = 0x80;
    }
    window = UiWindow_Create(0, 12, 30, 4, 74);
    while (1) {
        head = entries;
        pending = 0;
        {
            s32 t = cursor * 2;

            /* FAKEMATCH: the separate switch arms end CSE before the
             * projection call, keeping the scaled offset reload ahead of
             * the ids load. Sharing their block reverses two rows; no
             * switch branch remains in any edition. */
            switch (t) {
            case 0:
                t = cursor * 2;
                break;
            default:
                t = cursor * 2;
                break;
            }
            BattleMotion_ProjectConditionalPositionFar(*(u16 *)((u8 *)ids + t), (s32 *)&pos);
        }
        ((s32 *)head)[1] = 0x40002000;
        ((s32 *)head)[2] = 0;
        head->tile = Resource_GetBuffer(slotId, (((gFrameCount >> 2) & 31) << 8) + (s32)Menu_AnimatedCursorTiles);
        pos.y += Trig_Sin(gFrameCount << 12) / 32768;
        y = pos.y;
        if (tbl[0].flags & 1) {
            nx = (pos.x + tbl[0].x) / 2;
            ny = (y + tbl[0].y) / 2;
            if ((pos.x - nx < 0 ? -(pos.x - nx) : pos.x - nx) <= 7)
                pending = 1;
            pos.x = nx;
            tbl[0].x = (u8)nx;
            tbl[0].y = (u8)ny;
            pos.y = ny;
        } else if ((u8)tbl[0].flags <= 3) {
            tbl[0].x = (u8)pos.x;
            pos.y = y;
            tbl[0].y = (u8)y;
            tbl[0].flags = 1;
        } else {
            pos.x = tbl[0].x;
            pos.y = tbl[0].y;
            tbl[0].flags = (u8)(tbl[0].flags - 4);
            if (tbl[0].flags <= 3)
                tbl[0].flags = 1;
        }
        head->x = pos.x - 8;
        head->y = (u8)(pos.y - 16);
        Runtime_PushSlotEntry((s32 *)head, 240);
        if (spread == 0xFF) {
            efx.scale_x = 256;
            efx.scale_y = 256;
        } else {
            efx.scale_x = 176;
            efx.scale_y = 176;
        }
        efx.angle = 0;
        /* FAKEMATCH: register choice (spill slots) only: seventeen dead
           stores, two insns each until flow deletes them. GCSE sizes its
           expression table from the insn count (n / 2 | 1 buckets) and
           PRE numbers its reaching registers in bucket order, which is the
           order their spill slots are assigned; with these the count is
           1451 and the table 725 buckets, so tbl, entries, ids and
           cursor * 2 take sp+36, 32, 28 and 24 before &efx (20) and
           selSlot (16) as in the native frame (1417 and 709 buckets
           without them: every remaining row is a spill slot's offset;
           ES and IT use two additional copies below to restore the slot
           order after their value-preserving null initializer). */
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
        /* FAKEMATCH: these two additional dead stores are needed to match
         * the native spill slots; without them ES differs 136 instructions. */
        efx.angle = 0;
        efx.angle = 0;
#endif
        matrix = AffineMatrix_BuildForEffect(&efx);
        if ((redraw & 1) == 0)
            goto frame_end;
        cnt = 0;
        for (i = 0; i < 6; i++)
            tbl[i].flags &= (u8)~2;
        for (i = 0; (u32)i < spread; i++) {
            {
                n = cursor + i;
                if (n < total && ids[n] != 0xFE) {
                    selIds[cnt] = ids[n];
                    tbl[i].flags |= 2;
                    if (tbl[i].index != i) {
                        tbl[i].flags &= (u8)~1;
                        tbl[i].index = (s8)i;
                    }
                    selSlot[cnt] = (u8)i;
                    cnt++;
                }
                if (i != 0 && (n = cursor - i) >= 0 && ids[n] != 0xFE) {
                    selIds[cnt] = ids[n];
                    tbl[6 - i].flags |= 2;
                    if (tbl[6 - i].index != -i) {
                        tbl[6 - i].flags &= (u8)~1;
                        tbl[6 - i].index = (s8)-i;
                    }
                    selSlot[cnt] = (u8)(6 - i);
                    cnt++;
                }
            }
        }
        for (i = 0; i < 6; i++) {
            if ((tbl[i].flags & 2) == 0)
                tbl[i].index = 6;
        }
        selIds[cnt] = 0xFF;
        BattlePres_SetActorModesFar(selIds, 1);
        if (ids[cursor] > 7)
            goto draw_name;
        if (spread == 0xFF)
            goto frame_tail;
        if (kind == 0)
            goto frame_tail;
        unit = UnitGetFar(ids[cursor]);
        BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&pos);
        if (infoWin != 0)
            UiWork_Finalize(infoWin, 1);
        switch (kind) {
        case 7:
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 4;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                if (System_GetLanguage() == 4) {
                    if (anchor + 6 > 29)
                        column = 20;
                    infoWin = UiWindow_Create(column, 8, 10, 3, 6);
                } else if (System_GetLanguage() == 3) {
                    if (anchor + 7 > 29)
                        column = 19;
                    infoWin = UiWindow_Create(column, 8, 11, 3, 6);
                } else {
                    if (anchor + 5 > 29)
                        column = 21;
                    infoWin = UiWindow_Create(column, 8, 9, 3, 6);
                }
#elif defined(TBS_EDITION_FR)
                if (anchor + 5 > 29)
                    column = 21;
                infoWin = UiWindow_Create(column, 8, 9, 3, 6);
#elif defined(TBS_EDITION_JA)
                if (anchor + 4 > 29)
                    column = 22;
                infoWin = UiWindow_Create(column, 8, 8, 3, 6);
#else
                if (anchor + 4 > 29)
                    column = 22;
                infoWin = UiWindow_Create(column, 8, 9, 3, 6);
#endif
            }
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset((s32)MsgTargetSetDjinni, (struct RenderInput *)infoWin, 0, 0);
            UiWork_SetParamNibble(15);
            goto frame_tail;
        case 1:
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 7;
                if (anchor + 6 > 29)
                    column = 17;
                infoWin = UiWindow_Create(column, 8, 13, 3, 6);
            }
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
            switch (System_GetLanguage()) {
            case 1:
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_KpString, (struct RenderInput *)infoWin, 0, 0);
                break;
            case 2:
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_PvString, (struct RenderInput *)infoWin, 0, 0);
                break;
            case 3:
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_PvString, (struct RenderInput *)infoWin, 0, 0);
                break;
            case 4:
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_HpString, (struct RenderInput *)infoWin, 0, 0);
                break;
            }
#else
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_HpString, (struct RenderInput *)infoWin, 0, 0);
#endif
            UiText_DrawNumberInWindow(unit->hp, 4, (s32)infoWin, 16, 0);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)infoWin, 48, 0);
            UiText_DrawNumberInWindow(unit->max_hp, 4, (s32)infoWin, 56, 0);
            goto frame_tail;
        case 2:
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 7;
                if (anchor + 6 > 29)
                    column = 17;
                infoWin = UiWindow_Create(column, 8, 13, 3, 6);
            }
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_PpString, (struct RenderInput *)infoWin, 0, 0);
            UiText_DrawNumberInWindow(unit->ep, 4, (s32)infoWin, 16, 0);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)infoWin, 48, 0);
            UiText_DrawNumberInWindow(unit->max_ep, 4, (s32)infoWin, 56, 0);
            goto frame_tail;
        case 5:
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 7;
                if (anchor + 5 > 29)
                    column = 18;
                infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            }
            if (unit->hp != 0)
                goto no_condition;
            UiText_DrawCharacterAtOffset((s32)MsgTargetDowned, (struct RenderInput *)infoWin, 0, 0);
            goto frame_tail;
        case 3:
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 7;
                if (anchor + 5 > 29)
                    column = 18;
                infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            }
            if (unit->doku == 0)
                goto no_condition;
            UiText_DrawCharacterAtOffset((s32)MsgTargetCurePoison, (struct RenderInput *)infoWin, 0, 0);
            goto frame_tail;
        case 4:
            count = 0;
            if (unit->maboroshi != 0)
                count = 1;
            if (unit->shibire != 0)
                count++;
            if (unit->nemuri != 0)
                count++;
            if (unit->fuuji != 0)
                count++;
            if (unit->shiryo != 0)
                count++;
            if (count == 0)
                count = 1;
            rows = 9 - count;
            if (rows <= 3)
                rows = 4;
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 7;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                if (System_GetLanguage() == 3) {
                    if (anchor + 10 > 29)
                        column = 13;
                    infoWin = UiWindow_Create(column, rows, 17, count + 2, 6);
                } else {
                    if (anchor + 9 > 29)
                        column = 14;
                    infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
                }
#elif defined(TBS_EDITION_JA)
                if (anchor + 7 > 29)
                    column = 16;
                infoWin = UiWindow_Create(column, rows, 14, count + 2, 6);
#else
                if (anchor + 9 > 29)
                    column = 14;
                infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
#endif
            }
            count = 0;
            if (unit->maboroshi != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureDelusion, (struct RenderInput *)infoWin, 0, 0);
                count = 1;
            }
            if (unit->shibire != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureStun, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->nemuri != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureSleep, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->fuuji != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureSeal, (struct RenderInput *)infoWin, 0, count * 8);
                ++count;
            }
            if (unit->shiryo != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureCurse, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (count == 0)
                goto no_condition;
            goto frame_tail;
        case 6:
            count = 0;
            if (unit->doku != 0)
                count = 1;
            if (unit->maboroshi != 0)
                count++;
            if (unit->shibire != 0)
                count++;
            if (unit->nemuri != 0)
                count++;
            if (unit->fuuji != 0)
                count++;
            if (unit->shiryo != 0)
                count++;
            if (unit->akuryo != 0)
                count++;
            if (count == 0)
                count = 1;
            rows = 9 - count;
            if (rows <= 3)
                rows = 4;
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 7;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                if (System_GetLanguage() == 3) {
                    if (anchor + 10 > 29)
                        column = 13;
                    infoWin = UiWindow_Create(column, rows, 17, count + 2, 6);
                } else {
                    if (anchor + 9 > 29)
                        column = 14;
                    infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
                }
#elif defined(TBS_EDITION_JA)
                if (anchor + 7 > 29)
                    column = 16;
                infoWin = UiWindow_Create(column, rows, 14, count + 2, 6);
#else
                if (anchor + 9 > 29)
                    column = 14;
                infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
#endif
            }
            count = 0;
            if (unit->doku != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCurePoison, (struct RenderInput *)infoWin, 0, 0);
                count = 1;
            }
            if (unit->maboroshi != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureDelusion, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->shibire != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureStun, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->nemuri != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureSleep, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->fuuji != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureSeal, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->shiryo != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureCurse, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->akuryo != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureHaunt, (struct RenderInput *)infoWin, 0, 8 * count);
                count++;
            }
            if (count == 0)
                goto no_condition;
            goto frame_tail;
        default:
            break;
        }
        goto frame_tail;
    no_condition:
        UiWork_SetParamNibble(2);
        UiText_DrawCharacterAtOffset((s32)MsgTargetNoEffect, (struct RenderInput *)infoWin, 0, 0);
        UiWork_SetParamNibble(15);
        goto frame_tail;
    draw_name:
        if (spread == 0xFF)
            goto frame_tail;
        who = UnitGetFar(ids[cursor]);
        BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&namePos);
        namePos.y += Trig_Sin(gFrameCount << 12) / 32768;
        if (who->class_id == 125 || who->class_id == 122) {
            width = (s32)MsgMysteryPersonName;
            if (who->class_id == 125)
                width++;
#if defined(TBS_EDITION_JA)
            UiText_DecodeMessage(width, name, 10);
#else
            UiText_CopyMessageString(width, name, 14);
#endif

        } else {
            for (i = 0; i <= 13; i++) {
                name[i] = n = who->name[i];
                if (!n)
                    break;
            }
            name[i] = 0;
        }
        width = UiText_GetWideStringWidth(name);
        namePos.x -= width / 2;
        namePos.x -= 8;
        if (namePos.x + width > 224)
            namePos.x = 224 - width;
        if (namePos.x < 0)
            namePos.x = 0;
        Ui_ClearVramBlock();
        UiText_RenderWideStringAtOffset(name, window, namePos.x, 4);
    frame_tail:
        redraw &= ~1;
    frame_end:
        if (pending != 0) {
            for (i = 1; i < cnt; i++) {
                entry = &head[i];
                slot = &tbl[selSlot[i]];
                BattleMotion_ProjectConditionalPositionFar(selIds[i], (s32 *)&targetPos);
                targetPos.y = targetPos.y + Trig_Sin(gFrameCount << 12) / 32768;
                *entry = *head;
                if (slot->flags & 1) {
                    s32 x = (targetPos.x + slot->x) / 2;
                    s32 y = (targetPos.y + slot->y) / 2;

                    targetPos.x = x;
                    slot->x = x;
                    targetPos.y = y;
                    slot->y = y;
                } else {
                    s32 x = entry->x;
                    s32 y = entry->y + 8;

                    slot->flags = 1;
                    targetPos.x = x;
                    slot->x = x;
                    targetPos.y = y;
                    slot->y = y;
                }
                entry->mode = 1;
                entry->x = targetPos.x - 8;
                entry->y = targetPos.y - 12;
                if (spread == 0xFF)
                    entry->affine = 0;
                else
                    entry->affine = 1;
                entry->param = matrix;
                Runtime_PushSlotEntry((s32 *)entry, 240);
            }
        }
        pressed = gKeyState;
        repeat = gKeysRepeat;
        if (gLinkCountdownWork->auto_enabled) {
            repeat = 0;
            pressed = 0;
            if (gLinkCountdownWork->auto_delay == 0) {
                gLinkCountdownWork->auto_delay = 60;
                repeat = 1;
                pressed = 1;
            } else {
                gLinkCountdownWork->auto_delay--;
            }
        }
        if (pressed & 1) {
            s32 id;
            s32 k;

            id = ids[cursor];
            redraw = 0;
            result = -1;
            for (k = 0; k <= 5 && runtime->party_units[k] != 0xFF; k++) {
                if (runtime->party_units[k] == id) {
                    result = 0x100 | k;
                    break;
                }
            }
            if (result < 0) {
                for (k = 0; k <= 5 && runtime->enm_no[k] != 0xFF; k++) {
                    if (runtime->enm_no[k] == id) {
                        result = 0x180 | k;
                        break;
                    }
                }
            }
            cursor = result;
        } else if (spread != 0xFF) {
            if (repeat & 0x90) {
                AudioCommand_PlayFar(111);
                do {
                    cursor++;
                    cursor %= total;
                } while (ids[cursor] == 0xFE);
                redraw |= 1;
            }
            if (repeat & 0x60) {
                AudioCommand_PlayFar(111);
                do {
                    cursor = cursor + total - 1;
                    cursor %= total;
                } while (ids[cursor] == 0xFE);
                redraw |= 1;
            }
        }
        if (gLinkCountdownWork->active == 0 || (pressed & 2)) {
            AudioCommand_PlayFar(113);
            cursor = -1;
            break;
        }
        WaitFrames(1);
        if (redraw == 0)
            break;
    }
    WaitFrames(1);
    Resource_ResetEntry(slotId);
    if (infoWin != 0)
        UiWork_Finalize(infoWin, 1);
    UiWork_Finalize(window, 1);
    BattlePres_SetActorModesFar(ids, 0);
    gLinkCountdownWork->slide_offset = 0;
    WaitFrames(1);
    return cursor;
}

#define gLinkCountdownWork (*(struct LinkCountdownState * *)&gLinkCountdownWork)
#define Ui_FillVramBlockPattern ((unsigned char (*)(void))Ui_FillVramBlockPattern)
#define UiWindow_Create ((s32 (*)(s32, s32, s32, s32, s32))UiWindow_Create)
#define UiText_CopyMessageString ((s32 (*)(s32, s16 *, s32))UiText_CopyMessageString)
#define UiText_DecodeMessage ((void (*)(s32, s16 *, s32))UiText_DecodeMessage)

#define UiText_RenderWideStringAtOffset ((void (*)(s16*,s32,s32,s32))UiText_RenderWideStringAtOffset)

#define COUNTDOWN_START_FRAMES 900
#define FRAMES_PER_SECOND 60

struct CountdownDisplayEntry {
    u8 data[12];
};

struct LinkSignature {
    u8 padding_000[8];
    u16 e;
    u16 d;
    u16 v;
    u16 s;
    u8 padding_010[8];
};

struct LinkCountdownState {
    struct CountdownDisplayEntry entries[3];
    u8 entryActive[3];
    u8 padding_027;
    s32 targetOffset;
    s32 currentOffset;
    u8 padding_030[20];
    s32 displayHandle;
    s32 secondaryHandle;
    s32 timer;
    s32 enabled;
};
void AudioCommand_PlayFar(s32 soundId);

void UpdateLinkSessionCountdown(void)
{
	extern struct LinkSignature gLinkPeerSignatures[];
    struct LinkCountdownState *state = gLinkCountdownWork;
    struct BtlWork *runtime;
    struct CountdownDisplayEntry *entry;
    u8 *active;
    struct LinkSignature *signature;
    s32 target;
    s32 current;
    s32 difference;
    s32 step;
    s32 newOffset;
    s32 timer;
    s32 nextTimer;
    s32 seconds;
    s32 i;

    if (state != 0) {
        target = state->targetOffset;
        current = state->currentOffset;
        if (target != current) {
            difference = target - current;
            step = difference / 3;
            if (step == 0) {
                step--;
                if (difference >= 0)
                    step = 1;
            }
            newOffset = current + step;
            state->currentOffset = newOffset;
            Camera_ConfigureSceneFar(newOffset);
        }

        entry = state->entries;
        active = state->entryActive;
        i = 2;
        do {
            if (*active++ != 0)
                Runtime_PushSlotEntry(entry, 240);
            i--;
            entry++;
        } while (i >= 0);
        Link_DrawShiftedTilePair(0x06006680);

        if (state->enabled == 0)
            goto done;

        runtime = BtlWk;
        if (runtime->link_paused == 0)
            goto load_timer;
        state->timer = 0;
        goto done;

signature_tail:
        if (signature->v != 'V')
            goto signature_done;
        if (signature->s != 'S')
            goto signature_done;
        state->timer = COUNTDOWN_START_FRAMES;
        timer = state->timer;
        goto signature_done;

load_timer:
        timer = state->timer;
        if (timer >= 0)
            goto timer_ready;
        signature = &gLinkPeerSignatures[runtime->link_side ^ 1];
        if (signature->e != 'E')
            goto signature_done;
        if (signature->d == 'D')
            goto signature_tail;

signature_done:
        if (timer < 0)
            goto done;

timer_ready:
        if (state->displayHandle == 0 && state->secondaryHandle == 0) {
            state->displayHandle = Link_CreateCountdownLabelWindow();
            timer = state->timer;
        }

        if (timer > 0) {
            nextTimer = timer - 1;
            state->timer = nextTimer;
            timer = nextTimer;
        }
        if (timer < 0)
            goto done;

        seconds = (timer + FRAMES_PER_SECOND - 1) / FRAMES_PER_SECOND;
        if (seconds != 0 && seconds *FRAMES_PER_SECOND == timer)
            AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);

        if (state->displayHandle != 0)
            UiText_DrawNumberInWindow(seconds, 2, state->displayHandle, 16, 8);
    }

done:
    return;
}

#if EDITION_INTERNATIONAL
#define TEXT_COUNT 52
#else
#define TEXT_COUNT 32
#endif

#if EDITION_INTERNATIONAL


#else


#endif

s32 UiText_ShowLocalizedMessageAndWait(void)
{
    s16 buffer[64];
    s32 work;
    s32 result;
    struct BtlWork *battle;
    s32 mode;

    battle = BtlWk;
    mode = gGameState.battle_start;
    if (gGameState.battle_start == 2 || mode <= 2 || (result = 1, mode > 4)) {
        result = 0;
    }
    if (result == 0) {
        if ((s8)battle->chouhatsu_flg != 0) {
            result = 1;
        }
        if (result != 0) {
            goto active;
        }
    } else {
active:
        work = UiWindow_Create(0, 7, 30, 4, 42);
        Ui_FillVramBlockPattern();
#if EDITION_INTERNATIONAL
        UiText_CopyMessageString((s32)MsgNoTimeToRun, buffer, TEXT_COUNT);
#else
        UiText_DecodeMessage((s32)MsgNoTimeToRun, buffer, TEXT_COUNT);
#endif
        UiText_RenderWideStringAtOffset(buffer, work, 0, 4);
        do {
            WaitFrames(1);
        } while ((gKeyState & (KEY_A | KEY_B)) == 0 &&
                 gLinkCountdownWork->timer != 0);
        UiWork_Finalize(work, 1);
    }
    return result;
}

//--------------------------------------------------------------------------

// classify_entry_kind.c

// バトル：わざの対象選びの種類

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// わざの効果から、対象選びで使う種類を返す
// 0 その他、1 HPかいふく、2 EPかいふく、3 毒がきえる、4 状態異常をなおす、
// 5 ふっき、6 状態異常をすべてなおす
// act : わざのデータ
//--------------------------------------------------------------------------
s32 Battle_ClassifyEntryKind(const struct BtlAct *act)
{
	s32 val;
	s32 type;
	s32 kind;
	u8	efx;

	kind = 0;
	type = act->tgt_flg & 15;
	if (type == 1) // HPかいふく
		kind = 1;
	if (type == 11) // EPかいふく
		kind = 2;
	val = (s8)act->efx;
	efx = val;
	if (efx == 3) // 毒がきえる
		kind = 3;
	if (efx == 4) // 状態異常をなおす
		kind = 4;
	if (efx == 64) // 状態異常をすべてなおす
		kind = 6;
	if (BtlEfxRevChkFar(act->efx) != 0) // ふっき
		kind = 5;
	return kind;
}

#undef UiText_CopyMessageString
#undef UiText_DecodeMessage
#undef UiText_RenderWideStringAtOffset
#undef UiWindow_Create
#undef Ui_FillVramBlockPattern
#undef gLinkCountdownWork

s32 GameFlag_TestFar(s32 flag);
struct BtlUnit *UnitGetFar(s32 object_id);

#define UiText_DrawCharacterAtOffset ((void (*)(s32, void *, s32, s32))UiText_DrawCharacterAtOffset)

struct WindowLine {
    u16 left;
    u16 span;
};

struct WindowBuffer {
    u8 unknown_000[4];
    struct WindowLine lines[160];
};
extern u8 *gDisplayTransition;

#define COUNTDOWN ((struct LinkRoundState *)gLinkCountdownWork)

struct LinkRoundState {
    struct MenuSprite entries[3];
    u8 entryActive[3];
    u8 unk_27;
    s32 targetOffset;
    s32 currentOffset;
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    s32 unk_3c;
    s32 unk_40;
    s32 displayHandle;
    s32 secondaryHandle;
    s32 timer;
    s32 enabled;
    u8 cost[8][4];
    u8 pick[34];
    u16 list[33];
    s32 hintCount;
    s32 hintTimer;
    s32 actorId;
    u32 result[8];
};

extern volatile u16 gLinkStatus;
extern u8 gDebugMode;
void UpdateLinkSessionCountdown(void);
s32 Battle_ClassifyEntryKind(const struct BtlAct *entry);
s32 UiText_ShowLocalizedMessageAndWait(void);
struct UiWindow *UiText_ShowMessageAndWaitComplete(s32 id, s32 x, s32 y);
s32 Ui_RunSelectionScreen(s32 mode);
s32 Battle_SelectAbility(s32 actor, s32 mode);
s32 SummonMenu_SelectSummon(s32 a, s32 b, const u8 *standby);
s32 BattleMenu_RunActionSelection(s32 actor, u16 *list, s32 count);
s32 ItemList_SelectEntry(s32 actor, u16 *list, s32 count);

s32 Battle_CollectPartyCommands(struct BattleActionRecord *out, u16 *in, s32 count)
{
	extern u16 gLinkPeerSignatures[][12];
    struct LinkRoundState *state;
    struct BtlWork *link;
    struct BtlWork *session;
    u16 *peer;
    u16 *sync;
    struct BattleActionRecord *ent;
    struct BtlUnit *actor;
    struct UiWindow *win;
    struct MenuSprite *slot;
    u16 hdr[2];
    s32 aux;
    s32 actorId;
    s32 mode;
    s32 kind;
    s32 param;
    s32 res1024;
    s32 res512;
    s32 miss;
    s32 i;
    s32 k;
    s32 order;

    void DrawPartyPanels(void)
    {
        u16 data[2];
        data[0] = 0xff;
        BattleLayout_HighlightPartyPanels(data);
        BattlePres_SetActorModesFar(data, 1);
    }

    aux = 256;
    kind = 0;
    param = 0;
    res1024 = Resource_LoadIntoFreeSlot(1024);
    res512 = Resource_LoadIntoFreeSlot(512);
    state = (struct LinkRoundState *)Runtime_AllocateBlock(57, 260);
    Resource_LoadIndexedIntoBuffer(res1024, 0);
    Graphics_ExpandVramTilesByColorTable(0x06006000);
    Link_DrawShiftedTilePair(0x06006680);
    Menu_BuildLocalizedPatternTiles();
    link = BtlWk;
    for (k = 0; k < 8; k++) {
        state->result[k] = 0x80000000;
    }
    for (k = 0; k < 3; k++) {
        state->entryActive[k] = 0;
    }
    state->targetOffset = 0;
    state->currentOffset = 0;
    state->timer = -1;
    state->unk_3c = 0;
    state->unk_40 = 0;
    state->enabled = 0;
    state->secondaryHandle = 0;
    state->displayHandle = 0;
    if (link->two_sided != 0) {
        state->enabled = 1;
        sync = (u16 *)gSerialTransfer.payload;
        sync[4] = 'V';
        sync[5] = 'S';
        sync[6] = 'S';
        sync[7] = 'T';
        miss = 0;
        /* FAKEMATCH: instruction order only: the one-pass block's loop
           notes before the jump end the schedule there, so miss = 0 and the
           'T' store go out before the copies of the side and pause
           pointers that PRE puts before the jump, as native; its LOOP_CONT
           note keeps jump from copying the check in front (differ 4). */
        do {
            goto handshake_check;
handshake_loop:
            if ((gLinkStatus & 3) != 3) {
                if (++miss > 24) {
                    count = -1;
                    state->enabled = 0;
                    goto handshake_done;
                }
            } else {
                miss = 0;
                if (peer[4] == 'V' && peer[5] == 'S' && peer[6] == 'S' && peer[7] == 'T') {
                    goto handshake_done;
                }
                if (peer[4] == 'E' && peer[5] == 'D' && peer[6] == 'V' && peer[7] == 'S') {
                    goto handshake_done;
                }
                if (!(peer[0] == 'E' && peer[1] == 'X' && peer[2] == 'E' && peer[3] == 'C')) {
                    count = -1;
                    state->enabled = 0;
                    goto handshake_done;
                }
            }
            WaitFrames(1);
handshake_check:
            peer = gLinkPeerSignatures[link->link_side ^ 1];
            if (link->link_paused == 0) {
                goto handshake_loop;
            }
        } while (0);
        count = -1;
        state->enabled = 0;
    }
handshake_done:
    Scheduler_AddOrUpdateCallback(UpdateLinkSessionCountdown, 3200);

    for (;;) {
        DrawPartyPanels();
        state->entryActive[2] = 0;
        state->actorId = -1;
        state->hintCount = 0;
        if (GameFlag_TestFar(366) != 0) {
            state->hintCount = 1;
            state->hintTimer = 60;
        }
        if (count > 0) {
            mode = Ui_RunSelectionScreen(0);
        } else {
            mode = 14;
        }
        if (mode == 7) {
            void *block;
            s32 sel;
            s32 handle;

            block = MemAlloc(12);
            if (gDebugMode != 0 && (gKeysHeld & KEY_START) != 0) {
                sel = 2;
            } else {
                sel = 1;
            }
            handle = BattleParty_ListActorIdsFar(sel, block);
            WaitFrames(1);
            Ui_RunOwnerStatusScreen(block, handle, *(u16 *)block);
            /* FAKEMATCH: instruction order only: the one-pass block's loop
               notes keep MemFree's argument out of the call's schedule, so
               *block is loaded before block is copied to r0, as native
               (differ 2). */
            do {
            } while (0);
            MemFree(block);
            continue;
        }
        if (mode == 4) {
            if (UiText_ShowLocalizedMessageAndWait() != 0) {
                continue;
            }
            count = 1;
            ent = out;
            ent->unit_id = in[0];
            ent->priority = 0x7ffe;
            ent->command = 99;
            ent->parameter = 0;
            ent->target = 256;
            goto finish;
        }
        if (mode != 14) {
            goto finish;
        }
        AudioCommand_PlayFar(154);
        i = 0;
        if (i >= count) {
            goto finish;
        }

        for (;;) {
            if (i == 0) {
                BattlePlacement_CountValidEntriesFar(0, state->cost[0]);
            } else {
                s32 k;

                for (k = 0; k < 4; k++) {
                    state->cost[i][k] = state->cost[i - 1][k];
                }
            }

            ent = &out[i];
            actorId = in[i];
            /* FAKEMATCH: instruction order only: called as returning a
               doubleword, the call also sets r1, so the r1 reload for
               class_id no longer depends on the in reload and sched2 loads
               i * 2 before in, as native (differ 2). */
            actor = (struct BtlUnit *)(s32)((s64 (*)(s32))UnitGetFar)(actorId);
            COUNTDOWN->actorId = actorId;
            COUNTDOWN->unk_40 = 0;
            slot = &COUNTDOWN->entries[2];
            slot->oam.word.attr01 = 0x80000400;
            slot->oam.word.attr23 = 0;
            slot->oam.f.tile = Ui_LoadEntryForKind(actor->class_id, res512);
#if EDITION_INTERNATIONAL
            slot->oam.f.x = 0;
#else
            slot->oam.f.x = 4;
#endif
            slot->oam.f.y = 0x80;
            slot->oam.f.palette = 14;
            state->entryActive[2] = 1;

mark_visible:
            UiWindow_MarkVisibleTileAttributes();
            COUNTDOWN->entryActive[0] = 0;
            COUNTDOWN->result[i] = 0x80000000;
            /* FAKEMATCH: register choice (spill slots) only: twelve dead
               stores, one insn each until flow deletes them. GCSE sizes its
               expression table from the insn count (n / 2 | 1 buckets) and
               PRE numbers its reaching registers in bucket order, which is
               the order their spill slots are assigned. &hdr (sfp - 4)
               hashes to 13922 and i << 1, i << 2, i << 4 to 13966, 13967,
               13969: with these stores the table has 775 buckets (747
               against 16, 17, 19), so the givs take sp+32, 28 and 24
               before &hdr (sp+20) as in the native frame; without them 769
               (80 against 124, 125, 127) puts &hdr above the givs. 775
               needs 11 or 12 stores in every edition (Japanese is two
               insns longer). Without them differ 50 in every edition (en
               96 with the nested helpers' 46); every row they remove is a
               spill slot's offset. */
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = (u16)actorId;
            hdr[1] = 255;
            BattlePres_SetActorModesFar(hdr, 1);
            BattleLayout_HighlightPartyPanels(hdr);
            WaitFrames(1);
            mode = Ui_RunSelectionScreen(1);
            WaitFrames(1);
            if (mode == -2) {
                void *block;
                s32 handle;

                block = MemAlloc(12);
                handle = BattleParty_ListActorIdsFar(1, block);
                state->entryActive[2] = 0;
                WaitFrames(1);
                Ui_RunOwnerStatusScreen(block, handle, actorId);
                state->entryActive[2] = 1;
                MemFree(block);
                continue;
            }
            BattlePres_SetActorModesFar(hdr, 0);
            if (mode == -1) {
                if (i == 0) {
                    goto next_round;
                }
                i--;
                continue;
            }

            if (COUNTDOWN->timer == 0) {
                mode = 3;
            }
            slot = &COUNTDOWN->entries[0];
            slot->oam.word.attr23 = 0;
            slot->oam.word.attr01 = 0x80002400;
            slot->oam.f.tile = Resource_LoadIndexedIntoBuffer(res1024, mode);
            if (mode == 15) {
                slot->oam.f.x = 128;
            } else {
                slot->oam.f.x = 96;
            }
            slot->oam.f.y = 136;
            state->entryActive[0] = 1;
            switch (mode) {
            case 0:
                win = UiWindow_Create(11, 17, 11, 3, 6);
#if defined(TBS_EDITION_FR)
                UiText_DrawCharacterAtOffset((s32)MsgCommandName, (struct RenderInput *)win, 14, 0);
#else
                UiText_DrawCharacterAtOffset((s32)MsgCommandName, (struct RenderInput *)win, 16, 0);
#endif
                state->entries[0].oam.f.x = 64;
                AudioCommand_PlayFar(112);
                mode = BattleTarget_RunSelection(actorId, 1, 1, 0);
                UiWork_Finalize(win, 1);
                if (mode == -1) {
                    goto mark_visible;
                }
                kind = 0;
                aux = mode;
                ent->rng = 1;
                break;

            case 1: {
                struct MenuSprite *slot;
                struct BtlAct *ability;
                struct UiRenderWork *ui;
                u16 *list;
                u8 *pick;
                s32 cost;
                s32 handle;
                s32 cnt;
                s32 k;
                s32 cur;

                AudioCommand_PlayFar(112);
                mode = 0;
                /* FAKEMATCH: instruction order only: the one-pass block's loop
                   notes end the schedule here, so mode = 0 goes out before the
                   countdown base is loaded, as native (differ 4). */
                do {
                } while (0);
                COUNTDOWN->unk_34 = mode;
                COUNTDOWN->unk_30 = mode;
                COUNTDOWN->unk_38 = mode;
psynergy_menu:
                list = state->list;
                pick = state->pick;
                state->entries[0].oam.f.x = 48;
                cnt = 0;
                k = 0;
                cur = actor->action_slots[k].encoded_action & 0x3fff;
                if (cur != 0) {
                    do {
                        if ((BtlActGet(cur)->tgt_flg & 0x80) != 0) {
                            pick[cnt] = k;
                            list[cnt] = cur;
                            cnt++;
                        }
                        k++;
                        if (k == 32) {
                            break;
                        }
                        cur = actor->action_slots[k].encoded_action & 0x3fff;
                    } while (cur != 0);
                }
                pick[cnt] = 0;
                list[cnt] = 0;
                mode = BattleMenu_RunActionSelection(actorId, list, cnt);
                if (mode == -1) {
                    goto mark_visible;
                }
                param = actor->action_slots[pick[mode]].encoded_action & 0x3fff;
                ability = BtlActGet(param);
                cost = ability->rng;
                slot = &state->entries[1];
                handle = Resource_LoadIntoFreeSlot(128);
                ui = (struct UiRenderWork *)gWindowWork[0];
                win = UiWindow_Create(8, 17, 18, 3, 6);
                state->entries[0].oam.f.x = 40;
                slot->oam.word.attr01 = 0x40000400;
                slot->oam.word.attr23 = 0;
                slot->oam.f.x = win->x * 8 + 8;
                slot->oam.f.y = win->y * 8 + 4;
                /* FAKEMATCH: instruction order only (pool placement): arm_reorg counts this empty asm as a 4-byte insn, so the pool goes in after mov r1, r10 as native, whose constant 40 acts as a range-60 (zero_extendhisi2) pool load (differ 14). 2026-10-11: arm_reorg runs before shorten_branches and counts the empty asm at its 4-byte default length while it emits nothing; the halfword constant fix needs that much more estimated length after it, and no C statement or single pattern-length change (strh, str, orr, and tested over all 413 English objects) supplies it. */
                __asm__ volatile ("");
                slot->oam.f.tile = Resource_LoadIndexedEntryToBuffer(param, handle);
                state->entryActive[1] = 1;
                ui->level = 5;
                if ((s32)ability->pp_cost > (s32)actor->ep) {
                    UiWork_SetParamNibble(2);
                } else if (actor->fuuji != 0) {
                    UiWork_SetParamNibble(9);
                }
                UiText_DrawCharacterAtOffset(param + (s32)MsgAbilityName, (struct RenderInput *)win, 16, 0);
                UiText_DrawNumberAtOffset(ability->pp_cost, 2, (struct RenderInput *)win, 104, 0);
                ui->level = 15;
                UiWork_SetParamNibble(15);
                UiWindow_SetTilemapEntry(win, 61471, 11, 0, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                UiWindow_SetTilemapEntry(win, 61445, 12, 0, 0);
#else
                UiWindow_SetTilemapEntry(win, 61470, 12, 0, 0);
#endif
                if (ability->damage_class != 4) {
                    UiWindow_SetTilemapEntry(win, ability->damage_class + 20481, 15, 0, 0);
                }
                ent->rng = cost;
                AudioCommand_PlayFar(112);
                mode = BattleTarget_RunSelection(actorId, ability->target_mode, cost,
                                                 Battle_ClassifyEntryKind((const struct BtlAct *)ability));
                state->entryActive[1] = 0;
                Resource_ResetEntry(handle);
                UiWork_Finalize(win, 1);
                if (mode == -1) {
                    goto psynergy_menu;
                }
                kind = 1;
                aux = mode;
                break;
            }

            case 16: {
                const struct SummonDefinition *djinn;
                struct BtlAct *ability;
                struct MenuSprite *slot;
                s32 handle;
                s32 ok;
                s32 j;
                s32 n;

                AudioCommand_PlayFar(112);
                COUNTDOWN->unk_34 = 0;
                COUNTDOWN->unk_30 = 0;
                COUNTDOWN->unk_38 = 0;
djinn_menu:
#if EDITION_INTERNATIONAL
                state->entries[0].oam.f.x = 80;
#else
                state->entries[0].oam.f.x = 104;
#endif
                BattleLayout_HighlightPartyPanels(hdr);
                mode = SummonMenu_SelectSummon(0, 0, state->cost[i]);
                BattleLayout_HighlightPartyPanels(hdr);
                if (mode == -1) {
                    goto mark_visible;
                }
                kind = 6;
                param = mode;
                WaitFrames(1);
                slot = &state->entries[1];
                djinn = SummonDefinition_Get(mode);
                ability = BtlActGet(djinn->name_message_id);
                handle = Resource_LoadIntoFreeSlot(128);
#if EDITION_INTERNATIONAL
                win = UiWindow_Create(10, 17, 17, 3, 6);
#else
                win = UiWindow_Create(10, 17, 14, 3, 6);
#endif
                n = 0;
                for (j = 0; j <= 3; j++) {
                    if (djinn->djinn_required[j] > state->cost[i][j]) {
                        break;
                    }
                }
                ok = (j == 4);
                state->entries[0].oam.f.x = 56;
                slot->oam.word.attr01 = 0x40000400;
                slot->oam.word.attr23 = 0;
                /* FAKEMATCH: instruction order only (pool placement, schedule): counted as 4 bytes, the empty asm puts the pool after mov r2, r9 as native (constant 56) and keeps ok = 1 - (j != 4) among the slot stores (differ 12). 2026-10-11: arm_reorg runs before shorten_branches and counts the empty asm at its 4-byte default length while it emits nothing; the halfword constant fix needs that much more estimated length after it, and no C statement or single pattern-length change (strh, str, orr, and tested over all 413 English objects) supplies it. */
                __asm__ volatile ("");
                slot->oam.f.x = win->x * 8 + 8;
                slot->oam.f.y = win->y * 8 + 4;
                slot->oam.f.tile = Resource_LoadIndexedEntryToBuffer(djinn->name_message_id & 0x3fff, handle);
                state->entryActive[1] = 1;
                if (ok == 0) {
                    UiWork_SetParamNibble(2);
                }
                UiText_DrawCharacterAtOffset(SummonDefinition_Get(mode)->name_message_id + (s32)MsgAbilityName,
                                             win, 16, 0);
                for (j = 0; j <= 3; j++) {
                    if (djinn->djinn_required[j] != 0) {
#if EDITION_INTERNATIONAL
                        UiWindow_SetTilemapEntry(win, j + 20481, n * 2 + 13, 0, 0);
                        UiWindow_PutGlyph(win, djinn->djinn_required[j] + 48, n * 2 + 14, 0, 0);
#else
                        UiWindow_SetTilemapEntry(win, j + 20481, n * 2 + 10, 0, 0);
                        UiWindow_PutGlyph(win, djinn->djinn_required[j] + 48, n * 2 + 11, 0, 0);
#endif
                        n++;
                    }
                }
                if (ok != 0) {
                    AudioCommand_PlayFar(112);
                } else {
                    AudioCommand_PlayFar(114);
                }
                mode = BattleTarget_RunSelection(actorId, ability->target_mode, ability->rng,
                                                 Battle_ClassifyEntryKind((const struct BtlAct *)ability));
                ent->rng = ability->rng;
                state->entryActive[1] = 0;
                Resource_ResetEntry(handle);
                UiWork_Finalize(win, 1);
                if (mode == -1) {
                    goto djinn_menu;
                }
                for (j = 0; j <= 3; j++) {
                    if (djinn->djinn_required[j] > state->cost[i][j]) {
                        state->cost[i][j] = 0;
                        break;
                    }
                    state->cost[i][j] -= djinn->djinn_required[j];
                }
                aux = mode;
                break;
            }

            case 15: {
                struct BtlAct *ability;
                struct UiWindow *hint;
                s32 cost;
                s32 element;
                s32 index;

                AudioCommand_PlayFar(112);
                COUNTDOWN->unk_34 = 0;
                COUNTDOWN->unk_30 = 0;
                COUNTDOWN->unk_38 = 0;
summon_menu:
                state->entries[0].oam.f.x = 144;
                COUNTDOWN->result[i] = 0x80000000;
                mode = Battle_SelectAbility(actorId, 1);
                ent->rng = 0;
                if (mode == -1) {
                    goto mark_visible;
                }
                kind = 5;
                param = mode;
                COUNTDOWN->result[i] = mode;
                element = (param >> 8) & 15;
                index = param & 255;
                if (Djinn_IsActiveFar(actorId, element, index) != 0) {
                    ability = BtlActGet(Djinn_GetDefinitionHeaderFar(element, index));
                    cost = ability->rng;
                    win = UiWindow_Create(11, 17, 10, 3, 6);
                    state->entries[0].oam.f.x = 64;
                    UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
                    UiText_DrawCharacterAtOffset(element * 20 + index + (s32)MsgDjinnName, win, 16, 0);
                    ent->rng = cost;
                    WaitFrames(1);
                    AudioCommand_PlayFar(112);
                    mode = BattleTarget_RunSelection(actorId, ability->target_mode, cost,
                                                     Battle_ClassifyEntryKind((const struct BtlAct *)ability));
                    if (state->hintCount != 0) {
                        hint = UiText_ShowMessageAndWaitComplete((s32)MsgDjinnUnleashHint, 15, 8);
                        while (UiWork_IsComplete() == 0) {
                            WaitFrames(1);
                        }
                        UiWork_Finalize(hint, 1);
                        state->hintCount++;
                        state->hintTimer = 45;
                    }
                    UiWork_Finalize(win, 1);
                    if (mode == -1) {
                        goto summon_menu;
                    }
                    aux = mode;
                    break;
                }
                win = UiWindow_Create(11, 17, 10, 3, 6);
                state->entries[0].oam.f.x = 64;
                UiWork_SetParamNibble(2);
                UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
                UiText_DrawCharacterAtOffset(element * 20 + index + (s32)MsgDjinnName, win, 16, 0);
                UiWork_SetParamNibble(15);
                ent->rng = 1;
                WaitFrames(1);
                AudioCommand_PlayFar(112);
                mode = BattleTarget_RunSelection(actorId, 4, 0, 7);
                UiWork_Finalize(win, 1);
                if (mode == -1) {
                    goto summon_menu;
                }
                aux = mode;
                break;
            }

            case 2: {
                struct MenuSprite *slot;
                struct BtlAct *ability;
                u16 *list;
                u8 *pick;
                s32 cost;
                s32 handle;
                s32 cnt;
                s32 k;
                s32 cur;
                s32 sel;

                AudioCommand_PlayFar(112);
                mode = 0;
                /* FAKEMATCH: instruction order only: as in the psynergy case,
                   mode = 0 before the countdown base (differ 2). */
                do {
                } while (0);
                COUNTDOWN->unk_34 = mode;
                COUNTDOWN->unk_30 = mode;
                COUNTDOWN->unk_38 = mode;
item_menu:
                state->entries[0].oam.f.x = 96;
                pick = state->pick;
                list = state->list;
                cnt = 0;
                k = 0;
                cur = actor->inventory[k];
                if (cur != 0) {
                    do {
                        Item_GetDirectFar(cur);
                        if (Item_ClassifyUseAbility(actorId, cur) == 0) {
                            list[cnt] = cur;
                            pick[cnt++] = k;
                        }
                        k++;
                        if (k == 15) {
                            break;
                        }
                        cur = actor->inventory[k];
                    } while (cur != 0);
                }
                k = 0;
                cur = actor->inventory[k];
                if (cur != 0) {
                    do {
                        Item_GetDirectFar(cur);
                        if (Item_ClassifyUseAbility(actorId, cur) != 0) {
                            list[cnt] = cur;
                            pick[cnt++] = k;
                        }
                        k++;
                        if (k == 15) {
                            break;
                        }
                        cur = actor->inventory[k];
                    } while (cur != 0);
                }
                list[cnt] = 0;
                mode = ItemList_SelectEntry(actorId, list, cnt);
                if (mode == -1) {
                    goto mark_visible;
                }
                sel = pick[mode];
                param = sel;
                ability = BtlActGet(Item_GetDirectFar(actor->inventory[sel])->action_id);
                cost = ability->rng;
                slot = &state->entries[1];
                handle = Resource_LoadIntoFreeSlot(128);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
                win = UiWindow_Create(9, 17, 16, 3, 6);
#else
                win = UiWindow_Create(9, 17, 15, 3, 6);
#endif
                state->entries[0].oam.f.x = 48;
                slot->oam.word.attr01 = 0x40000400;
                slot->oam.word.attr23 = 0;
                /* FAKEMATCH: instruction order only (pool placement): counted as 4 bytes, the empty asm puts the pool after ldr r1, [sp, #72] as native (constant 48) (differ 10). 2026-10-11: arm_reorg runs before shorten_branches and counts the empty asm at its 4-byte default length while it emits nothing; the halfword constant fix needs that much more estimated length after it, and no C statement or single pattern-length change (strh, str, orr, and tested over all 413 English objects) supplies it. */
                __asm__ volatile ("");
                slot->oam.f.x = win->x * 8 + 8;
                slot->oam.f.y = win->y * 8 + 4;
                slot->oam.f.tile = Resource_LoadKind26EntryToBuffer(actor->inventory[sel], handle);
                state->entryActive[1] = 1;
                UiText_DrawCharacterAtOffset((actor->inventory[sel] & 0x1ff) + (s32)MsgItemName,
                                             (struct RenderInput *)win, 24, 0);
                ent->rng = cost;
                AudioCommand_PlayFar(112);
                mode = BattleTarget_RunSelection(actorId, ability->target_mode, cost,
                                                 Battle_ClassifyEntryKind((const struct BtlAct *)ability));
                state->entryActive[1] = 0;
                Resource_ResetEntry(handle);
                UiWork_Finalize(win, 1);
                if (mode == -1) {
                    goto item_menu;
                }
                kind = 2;
                aux = mode;
                break;
            }

            case 3:
                kind = 3;
                break;
            }

            AudioCommand_PlayFar(110);
            ent->unit_id = (u16)actorId;
            order = actor->agi;
            if (order != 0) {
                order += (u32)(actor->agi * Random16()) >> 20;
            }
            ent->priority = order;
            if (i != 0 && in[i] == in[i - 1]) {
                ent->priority = (s16)order / 2;
            }
            if ((s16)ent->priority < 0) {
                ent->priority = 2000;
            }
            ent->command = kind;
            ent->parameter = param;
            ent->target = aux;
            i++;
            if (i < count) {
                continue;
            }
            break;
        }

next_round:
        if (i < count) {
            continue;
        }
        break;
    }

finish:
    if (state->enabled != 0) {
        sync = (u16 *)gSerialTransfer.payload;
        sync[4] = 'E';
        sync[5] = 'D';
        sync[6] = 'V';
        sync[7] = 'S';
    }
    if (state->displayHandle != 0) {
        UiWork_Finalize((struct UiWindow *)state->displayHandle, 1);
    }
    Resource_ResetEntry(res512);
    Resource_ResetEntry(res1024);
    Scheduler_RemoveCallback(UpdateLinkSessionCountdown);
    if (state->enabled != 0) {
        s32 DrawLocalizedResource(void)
        {
            u16 data[64];
#if EDITION_INTERNATIONAL
            UiText_CopyMessageString((s32)MsgWaitingForOpponent, data, 52);
#else
            UiText_DecodeMessage((s32)MsgWaitingForOpponent, data, 32);
#endif
            UiText_RenderWideStringAtOffset(data, (struct UiWindow *)state->displayHandle, 0, 4);
        }

        session = BtlWk;
        mode = 0;
        if (state->displayHandle == 0 && session->link_paused == 0) {
            state->displayHandle = (s32)UiWindow_Create(0, 16, 30, 4, 42);
            Ui_FillVramBlockPattern();
            DrawLocalizedResource();
        } else {
            state->displayHandle = 0;
        }
        for (;;) {
            peer = gLinkPeerSignatures[session->link_side ^ 1];
            if (session->link_paused != 0) {
                count = -1;
                break;
            }
            if ((gLinkStatus & 3) != 3) {
                mode++;
                if (mode > 24) {
                    count = -1;
                    break;
                }
            } else {
                mode = 0;
                if (peer[4] == 'E' && peer[5] == 'D' && peer[6] == 'V' && peer[7] == 'S') {
                    break;
                }
                if (!(peer[4] == 'V' && peer[5] == 'S' && peer[6] == 'S' && peer[7] == 'T')) {
                    mode = 1;
                }
            }
            WaitFrames(1);
        }
        if (state->displayHandle != 0) {
            UiWork_Finalize((struct UiWindow *)state->displayHandle, 1);
        }
    }
    Camera_ConfigureSceneFar(0);
    Runtime_ReleaseHeapBlock(57);
    return count;
}

void AffineEffect_UpdateFrame(void)
{
    struct ResourceMenuWork *work;
    struct ResourceMenuEntry *sprite;
    struct AffineTransform effect;
    s32 scale;
    s32 matrix;
    s32 edge;

    s32 y;
    u32 end;
    u32 i;
    u16 *line;
    u8 *buffers;

    work = gMenuSelectWork;
    sprite = work->entries;
    scale = Ui_BounceScales[(gFrameCount * 2) & 31];
    scale = (scale - 256) / 4 + 304;
    effect.scale_x = scale;
    effect.scale_y = scale;
    effect.angle = 0;
    matrix = AffineMatrix_BuildForEffect(&effect);
    for (i = 0; i < work->count; i++, sprite++) {
        u32 *attr;
        s32 slot;
        s32 px;

        px = sprite->x;
        if (px == 0)
            continue;
        if (i == work->selection) {
            s32 left = px + scale * 7 / 512 - 20;

            if (sprite->y != 0)
                y = sprite->y + scale * 3 / 256 - 20;
            else
                y = (scale * 15 / 256 - 30) & 255;
            attr = &sprite->link;
            *attr++ = 0;
            *attr++ = (matrix << 25) | y | (left << 16) | 0x80002300;
            *attr = gVramBlockCache[sprite->slot].offset >> 5;
            slot = 246;
        } else {
            u32 base;
            u32 value;

            attr = &sprite->link;
            /* FAKEMATCH: needed to match: reading the y halfword through attr
             * keeps the attribute-pointer copy before its load; sprite->y
             * moves the copy two instructions later. */
            y = ((s16 *)attr)[7];
            *attr++ = 0;
            value = y | (px << 16) | 0x80002000;
            base = gVramBlockCache[sprite->slot].offset;
            *attr++ = value;
            *attr = base >> 5;
            slot = 245;
        }
        Runtime_PushSlotEntry(sprite, slot);
    }
    if (work->row == 0) {
        struct WindowBuffer *window;

        s32 span;

        buffers = gDisplayTransition;
        if (buffers == 0 || work->count == 0)
            return;
        window = (struct WindowBuffer *)(buffers + buffers[0x539] * 644);
        edge = work->entries[work->selection].x;
        span = (scale * 12 - 2817) / 256;
        edge = ((edge - span) << 8) + (edge + span) + 23;
        end = work->entries[work->selection].y + 24;
        end += (scale * 32 - 7937) / 512 + 1;
        line = &window->lines[24].span;
        i = 24;
        if (i < end) {
            do {
                *line = (*line & 0xff) | edge;
                i++;
                line += 2;
            } while (i < end);
        }
        edge = work->entries[0].x;
        if (work->selection == 0)
            edge -= (scale * 12 - 2817) / 256;
        edge <<= 8;
        line = &window->lines[0].span;
        for (i = 0; i <= 23; i++) {
            *line = (*line & 0xff) | edge;
            line += 2;
        }
    } else {
        struct WindowBuffer *window;
        s32 start;

        s32 span;

        buffers = gDisplayTransition;
        if (buffers == 0 || work->count == 0)
            return;
        window = (struct WindowBuffer *)(buffers + buffers[0x539] * 644);
        edge = work->entries[work->selection].x;
        span = (scale * 12 - 2817) / 256;
        edge = ((edge - span) << 8) + (edge + span) + 23;
        start = work->entries[work->selection].y;
        start -= (scale * 32 - 7937) / 512 + 1;
        line = &window->lines[start].span;
        i = start;
        if (i <= 135) {
            do {
                *line = (*line & 0xff) | edge;
                i++;
                line += 2;
            } while (i <= 135);
        }
        edge = work->entries[0].x;
        if (work->selection == 0)
            edge -= (scale * 12 - 2817) / 256;
        edge <<= 8;
        line = &window->lines[136].span;
        for (i = 136; i <= 159; i++) {
            *line = (*line & 0xff) | edge;
            line += 2;
        }
    }
}

void *AffineEffect_InitializeWork(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateBlock(58, 152);
    zero = 0;
    Dma_Set(&zero, work, 0x85000026, REG_DMA3);
    Scheduler_AddOrUpdateCallback(AffineEffect_UpdateFrame, 0xc76);
    return work;
}

void Menu_EndResourceSelection(void)
{
    struct UiWindow *child;
    s32 i;
    struct ResourceMenuWork *work;

    work = gMenuSelectWork;
    Scheduler_RemoveCallback(AffineEffect_UpdateFrame);
    child = work->window;
    if (child != 0) {
        UiWork_Finalize(child, 2);
    }
    i = 0;
    while (i < work->count) {
        Resource_ResetEntry(work->entries[i].slot);
        i += 1;
    }
    Runtime_ReleaseHeapBlock(0x3A);
    WaitFrames(1U);
}

void AudioCommand_PlayFar(s32);

s32 Menu_RunResourceSelectionLoop(s32 initial)
{
    struct ResourceMenuWork *work = gMenuSelectWork;
    s32 resource;

    work->selection = initial;
redraw:
    RenderOutput_PrepareForRedraw(work->window);
    if (work->resource_base != 0)
        resource = work->resource_base + work->selection;
    else
        resource = work->resource_ids[work->selection] + (s32)MsgCommandName;
    UiText_DrawCharacterAtOffset(resource, work->window, 0, 0);
    for (;;) {
        WaitFrames(1);
        if (gKeyState & KEY_A) {
            AudioCommand_PlayFar(112);
            return work->selection;
        }
        if (gKeyState & KEY_B) {
            AudioCommand_PlayFar(113);
            return -1;
        }
        if (gKeyState & KEY_START) {
            AudioCommand_PlayFar(113);
            return -1;
        }
        if ((gKeysRepeat & KEY_LEFT) || (gKeysRepeat & KEY_UP)) {
            AudioCommand_PlayFar(111);
            work->selection--;
            if (work->selection < 0)
                work->selection = work->count - 1;
            goto redraw;
        }
        if ((gKeysRepeat & KEY_RIGHT) || (gKeysRepeat & KEY_DOWN)) {
            AudioCommand_PlayFar(111);
            work->selection++;
            if (work->selection >= work->count)
                work->selection = 0;
            goto redraw;
        }
    }
}

void AudioCommand_PlayFar(s32 sound_id);

static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs)
{
    if (diff >= 0)
        return diff;
    return rhs - lhs;
}

s32 Menu_SelectResource(s32 start, s32 goal)
{
    s16 resource_base;
    s16 cur;
    s32 diff;
    s32 resource_id;
    s32 dist;
    s32 pos;
    s32 step;
    s32 delay;
    const u8 *tbl;
    struct ResourceMenuWork *state;

    state = gMenuSelectWork;
    step = 1;
    delay = 12;
    state->selection = start;
    if (goal < start)
        step = -1;
    pos = start;

    for (;;) {
        RenderOutput_PrepareForRedraw(state->window);
        resource_base = state->resource_base;
        if (resource_base != 0) {
            resource_id = resource_base + state->selection;
        } else {
            resource_id = state->resource_ids[state->selection] + (s32)MsgCommandName;
        }
        UiText_DrawCharacterAtOffset(resource_id, state->window, 0, 0);

        cur = state->selection;
        tbl = Menu_SelectionStepDelays;
        diff = cur - goal;
        dist = AbsoluteDifference(diff, cur, goal);
        WaitFrames(tbl[dist] + delay);

        if (pos == goal)
            break;

        state->selection = state->selection + step;
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        delay = 0;
        pos += step;
    }

    WaitFrames(48);
    AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
    return goal;
}

void Menu_LoadResourceSlot(s32 slot, s32 index)
{
    s32 size = 1024;
    void *buffer = (void *)MemAlloc(size);
    u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

    Resource_DecodeByteLz((void *)((u32)base + base[index]), buffer);
    VramBlock_LoadCached(slot, size, buffer);
    MemFree(buffer);
}

void Menu_AppendResourceEntry(s32 no)
{
    struct ResourceMenuWork *work;
    struct ResourceMenuEntry *entry;
    s16 index;
    s32 slot;
    s32 flags;

    work = gMenuSelectWork;
    index = work->count;
    if (index <= 5) {
        work->count = work->count + 1;
        entry = &work->entries[index];
        slot = Resource_FindFreeEntry();
        Menu_LoadResourceSlot(slot, no);
        entry->x = index * 24 + 32;
        flags = 136;
        entry->y = flags;
        entry->slot = slot;
        work->resource_ids[index] = no;
    }
}

void Menu_CenterResourceEntries(s32 row, s32 width, s32 resource_base)
{
    struct ResourceMenuWork *menu = ((struct ResourceMenuWork *)gMenuSelectWork);
    s32 x;
    s32 i;
    s32 count;

    menu->width = width + 2;
    menu->resource_base = resource_base;
    menu->row = row;
    count = menu->count;
    x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
    for (i = 0; i < menu->count; i++) {
        struct ResourceMenuEntry *entry = &menu->entries[i];

        entry->x = x * 8;
        entry->y = row * 8;
        x += 3;
    }
    menu->window = UiWindow_Create(x, row, menu->width, 3, 2);
}

void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resource_base)
{
    struct ResourceMenuWork *work;
    s32 i;
    struct ResourceMenuEntry *entry;

    work = gMenuSelectWork;
    work->width = width + 2;
    work->resource_base = resource_base;
    work->row = y;
    for (i = 0; i < work->count; i++) {
        entry = &work->entries[i];
        entry->y = y << 3;
        entry->x = x << 3;
        x += 3;
    }
    work->window = UiWindow_Create(x, y, work->width, 3, 2);
}

#define UiWindow_ClearInteriorTiles ((void (*)(void*,s32,s32,s32,s32))UiWindow_ClearInteriorTiles)

static __inline__ s32 TblGet(s8 *tbl, s32 index)
{
    return tbl[index];
}

s32 Menu_SelectTopEntry(s32 sel)
{
    s32 group = 0;
    s32 ofs;
    s32 triple;
    s32 ret;
    s8 *tbl;

    if (Party_SumDjinnCountsFar(-1) == 0) {
        group = 1;
    }

    triple = group * 3;
    tbl = Menu_TopEntryPositionByCommand;
    ofs = triple << 1;
    sel = TblGet(tbl, sel + ofs) - 1;
    if (sel < 0) {
        sel = 0;
    }

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(1);
    if (group == 0) {
        Menu_AppendResourceEntry(15);
    }
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
    ret = Menu_RunResourceSelectionLoop(sel);
    Menu_EndResourceSelection();

    if (ret >= 0) {
        ret = Menu_TopEntryCommandByPosition[ret + ofs + 1];
    }

    return ret;
}

#if EDITION_INTERNATIONAL
#define ANIMATE_ENTRIES_ROW 7
#else
#define ANIMATE_ENTRIES_ROW 5
#endif

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1)
{
    UiWindow_OpenMode1AndWaitFrame();
    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(1);
    Menu_AppendResourceEntry(0xF);
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Menu_CenterResourceEntries(0x11, ANIMATE_ENTRIES_ROW, 0);
    arg1 = Menu_SelectResource(arg0, arg1 - 1);
    Menu_EndResourceSelection();
    UiWork_CloseAndRelease();
    return arg1;
}

s32 Menu_SelectSaveSlotAction(void)
{
    s32 type;
    s32 ret;
    s32 initial;
    u32 group;

    group = 0;
    initial = 0;
    type = SaveState_ScanRecordFlags();
    if (type < 0) {
        return -1;
    }
    if (type == 0) {
        return 0;
    }
    if (type == 3) {
        group = 1;
    } else if (type == 0x67) {
        group = 2;
    } else if (type > 0x64) {
        group = 3;
    } else {
        initial = 1;
    }
    AffineEffect_InitializeWork();
    if ((group == 0) || (group == 3)) {
        Menu_AppendResourceEntry(0x15);
    }
    if (group <= 1U) {
        Menu_AppendResourceEntry(0x16);
    }
    if ((group == 0) || (group == 3)) {
        Menu_AppendResourceEntry(0x17);
    }
    Menu_AppendResourceEntry(0x18);
    if ((*(s16 *)gTitleExtraOptionEnabled) != 0) {
        Menu_AppendResourceEntry(0x1D);
    }
    if (gTitleSendOptionEnabled != 0) {
        Menu_AppendResourceEntry(0x1E);
    }
    Menu_CenterResourceEntries(0x11, TYPE_MENU_WIDTH, 0);
    ret = Menu_RunResourceSelectionLoop(initial);
    Menu_EndResourceSelection();
    if (ret >= 0) {
        ret = Menu_SaveSlotActionByPosition[ret + (group * 6)];
    }
    return ret;
}

#if EDITION_INTERNATIONAL
#define MODE_LABEL_X     18
#define MODE_LABEL_RIGHT 144
#else
#define MODE_LABEL_X     8
#define MODE_LABEL_RIGHT 80
#endif

void Menu_DrawModeLabel(void)
{
    struct ResourceMenuWork *state = gMenuSelectWork;

    if (state->previous_mode != state->selection) {
        state->previous_mode = state->selection;
        UiWindow_ClearInteriorTiles(state->lower_window, 8, 40, MODE_LABEL_RIGHT, 80);

        if (state->selection != 1) {
            if (state->selection > 1)
                goto mode_other;
            if (state->selection != 0)
                goto mode_other;

            {
                s32 text = (s32)MsgPasswordLevel;

                UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 40);
                UiText_DrawCharacterAtOffset(text + 1, state->lower_window, MODE_LABEL_X, 48);
                UiText_DrawCharacterAtOffset(text + 2, state->lower_window, MODE_LABEL_X, 56);
                UiText_DrawCharacterAtOffset(text + 3, state->lower_window, MODE_LABEL_X, 64);
                text += 4;
                UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 72);
                goto done;
            }
        }

        {
            s32 text = (s32)MsgPasswordLevel;

            UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 40);
            UiText_DrawCharacterAtOffset(text + 1, state->lower_window, MODE_LABEL_X, 48);
            text += 2;
            UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 56);
            goto done;
        }

mode_other:
        {
            s32 text = (s32)MsgPasswordLevel;

            UiText_DrawCharacterAtOffset(text++, state->lower_window, MODE_LABEL_X, 40);
            UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 48);
        }
done:
;
    }
}

#if EDITION_INTERNATIONAL
#define PASSWORD_HELP_X 16
#else
#define PASSWORD_HELP_X 0
#endif

#if defined(TBS_EDITION_FR)

void Menu_DrawModeIndicator(void)
{
    struct ResourceMenuWork *state = gMenuSelectWork;

    if (state->previous_mode != state->selection) {
        if (state->selection == 0) {
            RenderOutput_ClearList(state->lower_window);
            UiText_DrawResource((s32)MsgPasswordTransferHelp, (s32)state->lower_window, 16, 4);
            UiText_DrawResource((s32)MsgPasswordTransferHelp + 1, (s32)state->lower_window, 16, 16);
        } else if (state->previous_mode == 0) {
            RenderOutput_ClearList(state->lower_window);
            UiText_DrawResource((s32)MsgCableTransferHelp, (s32)state->lower_window, 0, 4);
            UiText_DrawResource((s32)MsgCableTransferHelp + 1, (s32)state->lower_window, 0, 16);
            UiText_DrawResource((s32)MsgCableTransferHelp + 2, (s32)state->lower_window, 0, 28);
        }
        state->previous_mode = state->selection;
    }
}
#else
void Menu_DrawModeIndicator(void)
{
    struct ResourceMenuWork *state = gMenuSelectWork;
    s16 *shown = &state->previous_mode;
    s16 *current = &state->selection;

    if (*shown != *current) {
        *shown = *current;
        RenderOutput_PrepareForRedraw(state->lower_window);
        if (*current == 0) {
#if defined(TBS_EDITION_DE)
            UiText_DrawCharacterAtOffset((s32)MsgPasswordTransferHelp,
                state->lower_window, 32, 8);
            UiText_DrawCharacterAtOffset((s32)MsgPasswordTransferHelp + 1,
                state->lower_window, 32, 24);
#else
            UiText_DrawResource((s32)MsgPasswordTransferHelp,
                state->lower_window, PASSWORD_HELP_X, 4);
            UiText_DrawResource((s32)MsgPasswordTransferHelp + 1,
                state->lower_window, PASSWORD_HELP_X, 16);
#endif
        } else {
#if defined(TBS_EDITION_DE)
            UiText_DrawCharacterAtOffset((s32)MsgCableTransferHelp,
                state->lower_window, 0, 0);
            UiText_DrawCharacterAtOffset((s32)MsgCableTransferHelp + 1,
                state->lower_window, 0, 16);
            UiText_DrawCharacterAtOffset((s32)MsgCableTransferHelp + 2,
                state->lower_window, 0, 32);
#else
            UiText_DrawResource((s32)MsgCableTransferHelp,
                state->lower_window, 0, 4);
            UiText_DrawResource((s32)MsgCableTransferHelp + 1,
                state->lower_window, 0, 16);
            UiText_DrawResource((s32)MsgCableTransferHelp + 2,
                state->lower_window, 0, 28);
#endif
        }
    }
}
#endif

void WaitFrames(s32);

s32 Menu_SelectResourceLayout(s32 mode)
{
	void UiText_DrawResource(s32, struct UiWindow *, s32, s32);
    struct ResourceMenuWork *work;
    struct UiWindow *window;
    s32 msg;
    struct UiWindow **upper;
    s32 result;

    AffineEffect_InitializeWork();
    work = gMenuSelectWork;
    if (mode == 0) {
        Menu_AppendResourceEntry(44);
        Menu_AppendResourceEntry(45);
    } else {
        Menu_AppendResourceEntry(46);
        Menu_AppendResourceEntry(47);
        Menu_AppendResourceEntry(48);
#if defined(TBS_EDITION_ES)
        Menu_CenterResourceEntries(17, 5, 0);
#endif
    }
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
    Menu_CenterResourceEntries(17, 7, 0);
#elif defined(TBS_EDITION_ES)
#elif defined(TBS_EDITION_FR)
    Menu_CenterResourceEntries(17, 9, 0);
#else
    Menu_CenterResourceEntries(17, 5, 0);
#endif
    if (mode != 0) {
        Scheduler_AddOrUpdateCallback(Menu_DrawModeLabel, 0xc76);
        work->previous_mode = 0xffff;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
        window = UiWindow_Create(7, 0, 17, 4, 2);
#elif defined(TBS_EDITION_ES)
        window = UiWindow_Create(5, 0, 21, 4, 2);
#elif defined(TBS_EDITION_FR)
        window = UiWindow_Create(5, 0, 22, 4, 2);
#else
        window = UiWindow_Create(9, 0, 13, 4, 2);
#endif
        msg = (s32)MsgPasswordSelection;
        upper = &work->upper_window;
        *upper = window;
        UiText_DrawResource(msg, window, 0, 4);
#if EDITION_INTERNATIONAL
#if defined(TBS_EDITION_DE)
        window = UiWindow_Create(1, 4, 28, 12, 2);
#else
        window = UiWindow_Create(3, 4, 25, 12, 2);
#endif
        work->lower_window = window;
        UiText_DrawResource(msg + 1, window, 8, 0);
        UiText_DrawResource(msg + 2, work->lower_window, 8, 11);
        msg += 3;
        UiText_DrawResource(msg, work->lower_window, 8, 22);
#else
        window = UiWindow_Create(5, 4, 21, 12, 2);
        work->lower_window = window;
        UiText_DrawResource(msg + 1, window, 0, 4);
        msg += 2;
        UiText_DrawResource(msg, work->lower_window, 0, 16);
#endif
    } else {
        Scheduler_AddOrUpdateCallback(Menu_DrawModeIndicator, 0xc76);
        work->previous_mode = 0xffff;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
        window = UiWindow_Create(6, 0, 18, 4, 2);
#elif defined(TBS_EDITION_ES)
        window = UiWindow_Create(4, 0, 21, 4, 2);
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        window = UiWindow_Create(5, 0, 20, 4, 2);
#else
        window = UiWindow_Create(9, 0, 13, 4, 2);
#endif
        upper = &work->upper_window;
        *upper = window;
#if EDITION_INTERNATIONAL
        UiText_DrawResource((s32)MsgTransferMethod, window, 2, 4);
#else
        UiText_DrawResource((s32)MsgTransferMethod, window, 0, 4);
#endif
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        work->lower_window = UiWindow_Create(1, 5, 28, 7, 2);
#elif defined(TBS_EDITION_DE)
        work->lower_window = UiWindow_Create(0, 5, 30, 7, 2);
#else
        work->lower_window = UiWindow_Create(5, 5, 21, 7, 2);
#endif
    }
    result = Menu_RunResourceSelectionLoop(0);
    if (mode != 0)
        Scheduler_RemoveCallback(Menu_DrawModeLabel);
    else
        Scheduler_RemoveCallback(Menu_DrawModeIndicator);
    RenderOutput_PrepareForRedraw(*upper);
    RenderOutput_PrepareForRedraw(work->lower_window);
    UiWork_Finalize(*upper, 2);
    UiWork_Finalize(work->lower_window, 2);
    WaitFrames(1);
    Menu_EndResourceSelection();
    return result;
}

//--------------------------------------------------------------------------

// confirm_selection.c

// メニュー：コマンドのアイコンを並べてえらばせる（はい・いいえ など）

//--------------------------------------------------------------------------

// アイコンは ResourceId_CommandIcons の番号。
// えらんでいるコマンドの名前をウィンドウに出す
//--------------------------------------------------------------------------
// アイコン 0x11〜0x14 を画面の中央に並べてえらぶ（行 17）
// sel : はじめにえらんでいる位置。戻り値はえらんだ位置
//--------------------------------------------------------------------------
s32 Menu_SelectEntry11To14(s32 sel)
{
	s32 ret;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(0x11);
	Menu_AppendResourceEntry(0x12);
	Menu_AppendResourceEntry(0x13);
	Menu_AppendResourceEntry(0x14);
#if defined(TBS_EDITION_DE) || defined(TLA_EDITION_DE)
	Menu_CenterResourceEntries(17, 8, 0); // 名前のウィンドウの幅
#elif EDITION_INTERNATIONAL
	Menu_CenterResourceEntries(17, 7, 0);
#else
	Menu_CenterResourceEntries(17, 6, 0);
#endif
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	return ret;
}

//--------------------------------------------------------------------------
// アイコン 0x19〜0x1c を画面の中央に並べてえらぶ（行 17）
//--------------------------------------------------------------------------
s32 Menu_SelectEntry19To1c(s32 sel)
{
	s32 ret;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(0x19);
	Menu_AppendResourceEntry(0x1A);
	Menu_AppendResourceEntry(0x1B);
	Menu_AppendResourceEntry(0x1C);
#if EDITION_INTERNATIONAL
	Menu_CenterResourceEntries(17, 10, 0);
#else
	Menu_CenterResourceEntries(17, 8, 0);
#endif
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	return ret;
}

//--------------------------------------------------------------------------
// はい・いいえ（アイコン 5・6）をえらぶ。キャンセルは 1（いいえ）
// low  : 0 なら行 0、0 でなければ行 17 に出す
// mes  : 名前のメッセージの先頭（0 ならコマンド名）
// w    : 名前のウィンドウの幅（0 なら 3）
// sel  : はじめにえらんでいる位置
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelection(s32 low, s32 mes, s32 w, s32 sel)
{
	s32 row;

	row = 0;

	AffineEffect_InitializeWork();

	if (w == 0)
	{
		w = 3;
	}
	if (low != 0)
	{
		row = 17;
	}

	Menu_AppendResourceEntry(5);
	Menu_AppendResourceEntry(6);
	Menu_CenterResourceEntries(row, w, mes);

	sel = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	if (sel == -1)
	{
		sel = 1;
	}
	return sel;
}

//--------------------------------------------------------------------------
// はい・いいえを (x, y) に出してえらぶ。キャンセルは 1（いいえ）
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 sel)
{
	s32 mes = (s32)MsgCommandYes;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(5);
	Menu_AppendResourceEntry(6);
	Menu_LayoutResourceEntries(x, y, 3, mes);
	sel = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	if (sel == -1)
	{
		sel = 1;
	}
	return sel;
}

//--------------------------------------------------------------------------
// select_entry_20_to_21.c
// メニュー：コマンドのアイコン 0x20・0x21 の 2 つからえらばせる
//--------------------------------------------------------------------------
// 版ごとの名前のウィンドウの幅
#if !EDITION_INTERNATIONAL
#define PAIR_WIDTH 7
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT) || defined(TLA_EDITION_FR) \
	|| defined(TLA_EDITION_IT)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

//--------------------------------------------------------------------------
// アイコン 0x20・0x21 を画面の中央に並べてえらぶ（行 17）
// sel : はじめにえらんでいる位置。戻り値はえらんだ位置
//--------------------------------------------------------------------------
s32 Menu_SelectEntry20To21(s32 sel)
{
	s32 ret;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(0x20);
	Menu_AppendResourceEntry(0x21);
	Menu_CenterResourceEntries(17, PAIR_WIDTH, 0);
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	return ret;
}
