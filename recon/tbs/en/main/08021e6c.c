#include "TYPES.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "IO_WRITE_QUEUE.H"

/*
 * DRAFT main:08021e6c (2300 bytes, including pools), structural hypothesis 2.
 * Recover the 480-byte work record and natural screen loop before register
 * tuning. Cursor entries share byte/halfword OAM views, not RenderOutput's
 * unrelated 28-byte layout. Trade availability is a word, the party result
 * is four bytes, and this owner returns the selected kind or -1/-2.
 * Residual H1: 2196/2300 bytes; 1099 differing halfwords; topology differs,
 * 1074 wrong instructions, aligned distance 774 halfword edits. Seven-entry
 * prologue prefix agrees, but frame is 44 rather than 60 bytes. Residual runs:
 * 212 (0 register-only, 0 order-only, 8 pool/immediate, 41 copy/rematerialise,
 * 0 pool-placement, 163 other). Typed indexing has not fixed broad lowering.
 * Compiler rounds a standalone halfword union to four bytes: place all four
 * OAM bytes in one word union to keep entries at 28 bytes and work at 480.
 * Residual H2: 2216/2300 bytes; 1107 differing halfwords; topology differs,
 * 1034 wrong instructions, aligned distance 737 halfword edits. Frame 40/60.
 * Runs: 218 (0 register-only, 0 order-only, 9 pool/immediate,
 * 49 copy/rematerialise, 0 pool-placement, 160 other). Row-cursor traversal
 * improves alignment but not the frame. The compiler already hoists natural
 * loop exit stubs between mode-zero and trade setup, as in the reference;
 * adding gotos there would not address the remaining layout difference.
 */
struct UiWindowWork;

union SelectionOam {
    u32 word;
    struct { u8 y, flags, low, high; } bytes;
    struct { u16 y_flags, x; } half;
};

struct SelectionEntry {
    s32 next;
    union SelectionOam oam;
    u32 tile;
    u16 resource, unused;
    s32 pos_x, pos_y;
    s32 kind;
};

struct SelectionAffine {
    u16 scale_x, scale_y, angle, unused;
};

struct SelectionWork {
    u8 glyphs[256];
    s32 state;
    struct SelectionEntry entries[6];
    struct UiWindowWork *window;
    s32 count;
    struct SelectionAffine affine;
    s32 matrix, selection, previous;
    s32 kinds[6];
};

struct SelectionRender {
    u16 tiles[1024];
    u8 unused[0x6a3];
    u8 dirty, mode, menu_state, menu_busy;
};

struct SelectionRuntime {
    u8 unused[0x3c];
    s32 selection[2];
    u8 unused44[8];
    s32 enabled;
    u8 unused50[0x88];
    s32 auto_phase, auto_delay, auto_mode;
};

struct SelectionScroll { u16 unused[2]; s16 x, y; };
struct TradeOfferState { u32 available; };

extern struct SelectionRender *Data_03001e8c;
extern struct SelectionRuntime *Data_03001f34;
extern struct SelectionScroll Data_03001ad0;
extern s32 Data_03001e40, Data_03001c94, Data_03001b04;
extern u16 Data_080366f8[];
extern volatile u16 Data_04000208;
extern u8 Value_0000001f[];

extern s32 Resource_LoadIntoFreeSlot(s32 size);
extern void *Runtime_BumpAllocateAlternatePool(s32 size);
extern void Runtime_BumpFree(void *work);
extern struct UiWindowWork *UiWindow_Create(s32 x, s32 y, s32 width,
                                           s32 height, s32 style);
extern void UiWork_Finalize(struct UiWindowWork *work, s32 release);
extern void RenderOutput_PrepareForRedraw(struct UiWindowWork *work);
extern void UiText_DrawCharacter(u8 *base, s32 index, u32 kind);
extern void UiText_DrawCharacterAtOffset(s32 kind, struct UiWindowWork *work,
                                         s32 x, s32 y);
extern struct UiWindowWork *UiText_ShowMessageAndWaitComplete(s32 message,
                                                           s32 x, s32 y);
extern s32 AffineMatrix_BuildForEffect(struct SelectionAffine *effect);
extern void Runtime_PushSlotEntry(s32 *entry, s32 slot);
extern void Resource_ResetEntry(s32 slot);
extern void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 period);
extern void Scheduler_RemoveCallback(void (*callback)(void));
extern void Runtime_SetIrqHandler(u32 irq, s32 line, void (*handler)(void));
extern void Graphics_SetBg1Priority3(void);
extern void Graphics_ClearBg1ControlBit2(void);
extern struct TradeOfferState *Trade_GetOfferStateFar(s32 index);
extern void Func_080b5130(s32 index, u8 *party);
extern void Func_080b5128(s32 x, s32 y);
extern void Audio_PlayCue(s32 cue);

s32 Ui_RunSelectionScreen(s32 mode)
{
    u8 party[4];
    struct SelectionRender *render;
    struct SelectionWork *work;
    struct SelectionRuntime *runtime;
    struct UiWindowWork *message;
    s32 rebuild, trade_count, scale, result;
    s32 cnt;
    s32 keys, repeat;
    s32 slot;
    u8 *pos;

    render = Data_03001e8c;
    slot = Resource_LoadIntoFreeSlot(0x400);
    rebuild = 1;
    trade_count = 0;
    work = Runtime_BumpAllocateAlternatePool(sizeof(*work));
    pos = &work->glyphs[255];
    do {
        *pos-- = 255;
    } while ((s32)pos >= (s32)work);
    work->state = 1;
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
        work->selection = Data_03001f34->selection[0];
    } else {
        work->selection = Data_03001f34->selection[1];
        work->kinds[0] = 0;
        work->kinds[1] = 1;
        cnt = 2;
        if (Trade_GetOfferStateFar(0)->available != 0) {
            Func_080b5130(0, party);
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
        scale = ((s32)Data_080366f8[(Data_03001e40 * 2) & 31] - 256) / 4 + 304;
        work->affine.scale_x = scale;
        work->affine.scale_y = scale;
        Data_03001ad0.x = 0;
        Data_03001ad0.y = 32;
        if (rebuild != 0) {
            rebuild = 0;
            render->menu_busy = 1;
            if (mode != 0) {
                s32 panel;
                for (panel = 0; panel < 6 - work->count; panel++) {
                    s32 row;
                    u16 *tile_row = &render->tiles[panel * 3];
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
                    u16 *tile_row = &render->tiles[tile_index];
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
                    u16 *tile_row = &render->tiles[panel * 3];
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
                    u16 *tile_row = &render->tiles[tile_index];
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
            UiText_DrawCharacterAtOffset(work->entries[work->selection].kind + (s32)Value_0000001f,
                                         work->window, 0, 0);
            work->previous = work->selection;
            UiText_DrawCharacter((u8 *)work, work->selection, work->kinds[work->selection]);
        }
        work->matrix = AffineMatrix_BuildForEffect(&work->affine);
        for (cnt = 0; cnt < work->count; cnt++) {
            struct SelectionEntry *entry = &work->entries[cnt];
            if (cnt == work->selection) {
                entry->oam.bytes.high = (entry->oam.bytes.high & ~0x3e) | (((u8)work->matrix & 31) * 2);
                entry->oam.bytes.flags |= 3;
                entry->oam.half.x = (entry->oam.half.x & 0xfffffe00) | ((entry->pos_x + scale * 7 / 512 - 14) & 511);
                entry->oam.bytes.y = entry->pos_y + scale * 3 / 256 - 20;
                Runtime_PushSlotEntry((s32 *)entry, 0xf1);
            } else {
                entry->oam.half.x = (entry->oam.half.x & 0xfffffe00) | (entry->pos_x & 511);
                entry->oam.bytes.y = entry->pos_y;
                entry->oam.bytes.high &= ~0x3e;
                entry->oam.bytes.flags &= ~3;
            }
        }
        keys = Data_03001c94;
        runtime = Data_03001f34;
        repeat = Data_03001b04;
        if (runtime->auto_phase != 0) {
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
                        if (runtime->auto_phase == 1) {
                            if (kind == 15)
                                message = UiText_ShowMessageAndWaitComplete(0xc4a, 15, 8);
                            else if (kind == 16)
                                message = UiText_ShowMessageAndWaitComplete(0xc49, 15, 8);
                            Func_080b5128(102, 155);
                            UiWork_Finalize(message, 1);
                            runtime->auto_phase++;
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
            work->selection = Math_Mod(work->selection + 1, work->count);
        } else if (repeat & 0x60) {
            Audio_PlayCue(0x6f);
            work->selection = Math_Mod(work->selection + work->count - 1, work->count);
        } else if (Data_03001f34->enabled == 0) {
            result = work->kinds[0];
            break;
        }
        QueueIoWriteDelay2(0x04000000, 0x1741);
        render->menu_busy = 0;
        WaitFrames(1);
    }
    if (mode != 0)
        Data_03001f34->selection[1] = work->selection;
    else
        Data_03001f34->selection[0] = work->selection;
    for (cnt = 0; cnt < work->count; cnt++)
        Resource_ResetEntry(work->entries[cnt].resource);
    render->menu_busy = 1;
    UiWork_Finalize(work->window, 1);
    {
        s32 panel;
        for (panel = 0; panel <= 6; panel++) {
            s32 row;
            u16 *tile_row = &render->tiles[panel * 3];
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
        volatile u16 *ime = &Data_04000208;
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
