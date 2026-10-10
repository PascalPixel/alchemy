#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "resource.h"
#include "glyph.h"
#include "resource_ids.h"
#include "dma.h"
#include "tbs_edition.h"
#include "io_reg.h"
#include "scene.h"
#include "global_cells.h"
#include "render_input.h"
#include "irq.h"
#include "affine.h"
#include "callback_scheduler.h"
#include "fixed_math.h"
#include "io_write_queue.h"
#include "window.h"
#include "uiflow.h"
#include "menu_list.h"
#include "map_scroll.h"
#include "text/msg_ids.h"
#include "btlwork.h"
#include "character_menu.h"
#include "battle_unit.h"
#include "btlsys.h"

u32 Resource_DecodeByteLz(const void *, void *);

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

void UiText_DrawCharacter(u8 *base, s32 index, u32 value)
{
    u8 *entry;
    s32 offset;
    s32 store_offset;
    s32 load_offset;

    offset = index * 28;
    entry = base + offset + 0x104;
    ((s32 (*)())UiText_LoadRemappedGlyph)(base, value, index * 16, index * 16);
    store_offset = offset + 0x11C;
    *(u32 *)(base + store_offset) = value;
    /* FAKEMATCH: needed to match: the empty do-while around this store only moves the scheduler; the reference sets the constant after the value store. */
    do { *(u32 *)(entry + 4) = 0x80002000; } while (0);
    *(u32 *)(entry + 8) = 0;
    load_offset = offset + 0x110;
    ((CharacterCell *)(entry + 8))->code =
        Resource_LoadIndexedIntoBuffer(*(u16 *)(base + load_offset), value);
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

void RenderOutput_PrepareForRedraw(struct UiWindow *work);
void UiText_DrawCharacter(u8 *base, s32 index, u32 kind);
s32 *Trade_GetOfferStateFar(s32 index);
s32 BattlePresentation_WaitForPromptAtFar(s32 x, s32 y);
void AudioCommand_PlayFar(s32 cue);

s32 Ui_RunSelectionScreen(s32 mode)
{
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
        UiText_DrawCharacter((u8 *)work, cnt, work->kinds[cnt]);
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
        {
            /* FAKEMATCH: needed to match: frame in its own block gives the loop's shared 0 the lifetime at which loop.c hoists it, so gBgScroll[1].x takes the pool-loaded 0 (120 rows differ without it). */
            s32 frame = gFrameCount;
            scale = Ui_BounceScales[(frame * 2) & 31];
            scale = (scale - 256) / 4 + 304;
            work->affine.scale_x = scale;
            work->affine.scale_y = scale;
        }
        {
            /* FAKEMATCH: needed to match: bg in its own block, as frame above (120 rows differ without it). */
            struct BgScroll *bg = gBgScroll;
            bg[1].x = 0;
            bg[1].y = 32;
        }
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
            UiText_DrawCharacter((u8 *)work, work->selection, work->kinds[work->selection]);
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
    u8 *base = gWindowWork[0];

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
            u16 *p = &((u16 (*)[32])base)[y][x];
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

            /* FAKEMATCH: needed to match: the do-while (0) keeps the row counter update after the dirty-byte store instead of filling the load delay. */
            do {
                ((struct UiRenderWork *)base)->dirty |= 2 << ((u32)y >> 2);
            } while (0);
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
    /* FAKEMATCH: no caller uses a result. The original scalar boundary keeps pop {r1}; the ordinary void definition changes it to pop {r0}. */
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
