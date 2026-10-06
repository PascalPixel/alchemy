/* 08021e6a: two zero bytes before this function are alignment, not code: DISPLAY.o ends at 0x08021e6a and a C object aligns .text to 4, so the linker fills them, as it already does at 0x08021dfa. */
/* Shared UiRenderWork, MenuCell, GlyphPalette, BgScroll and MenuSprite replace
 * private views. Original draft: listing 1037, draft 998, differ 1173; shared
 * views preserve that result, then genuine sprite bitfields improve it to
 * draft 1002, differ 1129. Native frame is 60 bytes; draft is 44. Remaining
 * work is loop/spill layout. Whole tutorial message symbols leave the score
 * unchanged; no new devices.
 */
#include "AFFINE.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "IO_WRITE_QUEUE.H"
#include "WINDOW.H"
#include "GLYPH.H"
#include "MENU_LIST.H"
#include "MAP_SCROLL.H"
#include "IO_REG.H"
#include "text/MSG_IDS.H"

struct UiWindow;

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

struct TradeOfferState { u32 available; };

extern u16 Ui_BounceScales[];

extern struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width,
                                           s32 height, s32 style);
extern void UiWork_Finalize(struct UiWindow *work, s32 release);
extern void RenderOutput_PrepareForRedraw(struct UiWindow *work);
extern void UiText_DrawCharacter(u8 *base, s32 index, u32 kind);
extern struct UiWindow *UiText_ShowMessageAndWaitComplete(s32 message,
                                                           s32 x, s32 y);
extern void Runtime_SetIrqHandler(u32 irq, s32 line, void (*handler)(void));
extern void Graphics_SetBg1Priority3(void);
extern void Graphics_ClearBg1ControlBit2(void);
extern struct TradeOfferState *Trade_GetOfferStateFar(s32 index);
extern void BattlePlacement_CountValidEntriesFar(s32 index, u8 *party);
extern void BattlePresentation_WaitForPromptAtFar(s32 x, s32 y);
extern void Audio_PlayCue(s32 cue);

s32 Ui_RunSelectionScreen(s32 mode)
{
    u8 party[4];
    struct UiRenderWork *render;
    struct SelectionWork *work;
    struct MenuCell *runtime;
    struct UiWindow *message;
    s32 rebuild, trade_count, scale, result;
    s32 cnt;
    s32 keys, repeat;
    s32 slot;
    u8 *pos;

    render = (struct UiRenderWork *)gWindowWork[0];
    slot = Resource_LoadIntoFreeSlot(0x400);
    rebuild = 1;
    trade_count = 0;
    work = Runtime_BumpAllocateAlternatePool(sizeof(*work));
    pos = &work->palette.slot[255];
    do {
        *pos-- = 255;
    } while ((s32)pos >= (s32)work);
    work->palette.count = 1;
    if (mode == 0)
        work->window = UiWindow_Create(20, 17, 10, 3, 6);
    else
        work->window = UiWindow_Create(22, 17, 8, 3, 6);
    work->previous = -1;
    if (mode == 0) {
        work->kinds[0] = 14;
        work->kinds[1] = 4;
        work->kinds[2] = 7;
        work->kinds[3] = -1;
        work->selection = gLinkCountdownWork->command_selection[0];
    } else {
        work->selection = gLinkCountdownWork->command_selection[1];
        work->kinds[0] = 0;
        work->kinds[1] = 1;
        cnt = 2;
        if (Trade_GetOfferStateFar(0)->available != 0) {
            BattlePlacement_CountValidEntriesFar(0, party);
            for (pos = party; pos <= &party[3]; pos++)
                trade_count += *pos;
            work->kinds[2] = 15;
            cnt = 3;
            if (trade_count != 0) {
                work->kinds[3] = 16;
                cnt = 4;
            }
        }
        work->kinds[cnt++] = 2;
        work->kinds[cnt++] = 3;
        work->kinds[cnt] = -1;
    }
    work->entries[0].resource = slot;
    cnt = 0;
    while (work->kinds[cnt] != -1) {
        UiText_DrawCharacter((u8 *)work, cnt, work->kinds[cnt]);
        cnt++;
        if (cnt > 5)
            break;
        work->entries[cnt].resource = slot;
    }
    work->count = cnt;
    work->affine.scale_x = 320;
    work->affine.scale_y = 304;
    work->affine.angle = 0;
    for (cnt = 0; cnt < work->count; cnt++) {
        work->entries[cnt].pos_x = (cnt - work->count) * 24 + 155;
        if (mode != 0)
            work->entries[cnt].pos_x = (cnt - work->count) * 24 + 171;
        work->entries[cnt].pos_y = 136;
    }
    Scheduler_AddOrUpdateCallback(Graphics_SetBg1Priority3, 0x480);
    Runtime_SetIrqHandler(2, 136, Graphics_ClearBg1ControlBit2);

    for (;;) {
        scale = ((s32)Ui_BounceScales[(gFrameCount * 2) & 31] - 256) / 4 + 304;
        work->affine.scale_x = scale;
        work->affine.scale_y = scale;
        gBgScroll[1].x = 0;
        gBgScroll[1].y = 32;
        if (rebuild != 0) {
            rebuild = 0;
            render->menu_busy = 1;
            if (mode != 0) {
                s32 panel;
                for (panel = 0; panel < 6 - work->count; panel++) {
                    s32 row;
                    u16 *tile_row = &render->tilemap[panel * 3];
                    for (row = 0; row <= 2; row++, tile_row += 32) {
                        s32 col;
                        for (col = 0; col <= 2; col++)
                            tile_row[0x224 + (col & 3)] = 0xf07f;
                    }
                }
                for (panel = 0; panel < work->count; panel++) {
                    s32 row;
                    s32 count = work->count;
                    s32 tile_index = panel * 3;
                    s32 pattern = panel * 16;
                    u16 *tile_row = &render->tilemap[tile_index];
                    for (row = 0; row <= 2; row++, tile_row += 32, tile_index += 32, pattern += 4) {
                        s32 col;
                        for (col = 0; col <= 2; col++) {
                            ((u16 *)0x0600fd6c)[tile_index + (col & 3) - count * 3] = 0x100 + pattern + col;
                            tile_row[0x236 + (col & 3) - count * 3] = 0;
                        }
                    }
                }
            } else {
                s32 panel;
                for (panel = 0; panel < 6 - work->count; panel++) {
                    s32 row;
                    u16 *tile_row = &render->tilemap[panel * 3];
                    for (row = 0; row <= 2; row++, tile_row += 32) {
                        s32 col;
                        for (col = 0; col <= 2; col++)
                            tile_row[0x222 + (col & 3)] = 0xf07f;
                    }
                }
                for (panel = 0; panel < work->count; panel++) {
                    s32 row;
                    s32 count = work->count;
                    s32 tile_index = panel * 3;
                    s32 pattern = panel * 16;
                    u16 *tile_row = &render->tilemap[tile_index];
                    for (row = 0; row <= 2; row++, tile_row += 32, tile_index += 32, pattern += 4) {
                        s32 col;
                        for (col = 0; col <= 2; col++) {
                            ((u16 *)0x0600fd68)[tile_index + (col & 3) - count * 3] = 0x100 + pattern + col;
                            tile_row[0x234 + (col & 3) - count * 3] = 0;
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
        runtime = gLinkCountdownWork;
        repeat = gKeysRepeat;
        if (runtime->auto_enabled != 0) {
            repeat = keys = 0;
            if (runtime->auto_delay == 0) {
                if (runtime->auto_mode == 1) {
                    if (work->kinds[work->selection] == 3)
                        repeat = keys = 1;
                    else
                        repeat = keys = 32;
                    runtime->auto_delay = 30;
                } else if (runtime->auto_mode == 0) {
                    s32 kind = work->kinds[work->selection];
                    if (kind == 16 || (trade_count == 0 && kind == 15)) {
                        if (runtime->auto_enabled == 1) {
                            if (kind == 15)
                                message = UiText_ShowMessageAndWaitComplete((s32)&MsgDjinnTutorialUnleashCommand, 15, 8);
                            else if (kind == 16)
                                message = UiText_ShowMessageAndWaitComplete((s32)&MsgDjinnTutorialSummon, 15, 8);
                            BattlePresentation_WaitForPromptAtFar(102, 155);
                            UiWork_Finalize(message, 1);
                            runtime->auto_enabled++;
                            runtime->auto_delay = 45;
                        } else {
                            runtime->auto_delay = 200;
                            repeat = keys = 1;
                        }
                    } else {
                        runtime->auto_delay = 40;
                        repeat = keys = 16;
                    }
                } else {
                    runtime->auto_delay = 60;
                    repeat = keys = 1;
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
            Audio_PlayCue(0x71);
            result = -1;
            break;
        }
        if (repeat & 0x90) {
            Audio_PlayCue(0x6f);
            work->selection = __modsi3(work->selection + 1, work->count);
        } else if (repeat & 0x60) {
            Audio_PlayCue(0x6f);
            work->selection = __modsi3(work->selection + work->count - 1, work->count);
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
            s32 row;
            u16 *tile_row = &render->tilemap[panel * 3];
            for (row = 0; row <= 2; row++, tile_row += 32) {
                s32 col;
                for (col = 0; col <= 2; col++)
                    tile_row[0x225 + (col & 3)] = 0xf07f;
            }
        }
    }
    render->dirty = 1;
    WaitFrames(1);
    Scheduler_RemoveCallback(Graphics_SetBg1Priority3);
    Runtime_SetIrqHandler(2, 0, 0);
    {
        struct IoWriteQueue *queue = &gIoWriteQueue;
        volatile u16 *ime = &REG_IME;
        u32 saved = *ime;
        s32 count;
        *ime = (u16)ime;
        count = queue->count;
        if (count <= 31) {
            u32 *dst = queue->entries[count];
            queue->count = count + 1;
            *dst++ = 0x1541;
            *dst++ = 0x04000000;
            *dst = 0x20000;
        }
        *ime = saved;
    }
    Runtime_BumpFree(work);
    render->menu_busy = 0;
    WaitFrames(1);
    return result;
}
