#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "window.h"
#include "global_cells.h"
#include "tbs_edition.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "glyph.h"
#include "resource.h"
#include "game_state.h"
#include "menu_list.h"

#define PANEL_TILE_ROWS ((u32 *)0x06002500)
#define PANEL_ROW_COUNT 30
#define PANEL_ROW_WORDS 32
#define PANEL_ROW_LEAD 8

void UiWork_ShiftPanelRowsLeft(s32 step)
{
    s32 shift = step * 6;
    u32 *base = PANEL_TILE_ROWS;
    s32 n;

    for (n = 0; n < PANEL_ROW_COUNT; n++) {
        u32 *row = base + n * PANEL_ROW_WORDS;

        Dma_Set(row + PANEL_ROW_LEAD + shift, row + PANEL_ROW_LEAD,
                0x84000000 | (PANEL_ROW_WORDS - PANEL_ROW_LEAD - shift), REG_DMA3);
        Iwram_FillWords(row + (PANEL_ROW_WORDS - shift), shift * 4, 0);
    }
}


void UiWork_ProcessRenderChannels(void)
{
    struct UiChannelSlot *channel =
        ((struct UiRenderWork *)gWindowWork[0])->channels;
    s32 channel_no = 0;
    s32 one = 1;

    do {
        struct UiWindow *current = channel->work;

        if (current != 0 && *(s32 *)&current->frame == 0) {
            u16 flags = current->flags;

            if (flags == 0) {
                channel->work = NULL;
            } else {
                s32 pending = current->effect;
                s32 kind;

                if (pending != 0) {
                    UiWork_AdvanceChannelTransition(channel);
                } else {
                    kind = UiWork_StepChannelScript(channel);
                    switch (kind) {
                    case 8:
                        channel->work->state = one;
                        break;
                    case 9:
                    {
                        struct UiWindow *entity = channel->work;

                        UiWork_Finalize(
                            entity,
                            entity->flags & 2
                        );
                        channel->x = pending;
                        channel->y = pending;
                        channel->entry = pending;
                        channel->countdown = pending;
                        channel->colour = pending;
                        channel->outline = pending;
                        channel->line_spacing = pending;
                        channel->work->state = one;
                        break;
                    }
                    }
                }
            }
        }
        channel_no++;
        channel++;
    } while (channel_no != 3);
}

extern volatile u32 gKeysHeld;
extern u32 gKeysPressedLatch;
extern volatile u16 gLagFramesShown;

void RenderOutput_PrepareForRedraw(struct UiWindow *window);
void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height);

enum {
    TEXT_END = 0,
    TEXT_PAGE = 1,
    TEXT_WAIT_KEY = 2,
    TEXT_NEWLINE = 3,
    TEXT_PAUSE_LONG = 4,
    TEXT_PAUSE_SHORT = 5,
    TEXT_PAUSE_LONGEST = 6,
    TEXT_COLOURS_RESET = 7,
    TEXT_COLOUR = 8,
    TEXT_OUTLINE = 9,
    TEXT_LINE_SPACING = 10,
    TEXT_WINDOW_STATE = 15,
    TEXT_END_ALT = 30,
    TEXT_COMMANDS = 31,
    TEXT_SPACE = 32,
    TEXT_DAKUTEN = 222,
    TEXT_HANDAKUTEN = 223
};

s32 UiWork_StepChannelScript(struct UiChannelSlot *ch)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u32 keys = gKeysHeld;
    s32 count = UiText_SpeedSteps[gGameState.message_speed];
    s32 lag;
    u32 code;
    u32 next;
    struct UiWindow *window;
    s32 x;
    s32 y;
    u32 w;
    u32 h;
    s32 old_w;
    s32 old_h;
    s32 entry;
    s32 pc;
    s32 gx;
    s32 gy;
    s32 cue;
    s32 advance;
    s32 width;

    if (work->menu_state != 0) {
        lag = gLagFramesShown;
        if (lag < 0)
            lag = 0;
        if (lag > 2)
            lag = 2;
        count = lag * 5 + 3;
    }

    if (ch->transition != 0) {
        UiWork_ShiftPanelRowsLeft(1);
        ch->transition = (s16)ch->transition - 1;
        return 0;
    }
    if (gKeysHeld == 0 && ch->glyph_delay != 0) {
        ch->glyph_delay--;
        return 0;
    }

    do {
        code = TEXT_END;
        if (ch->ended == 0)
            code = work->entries[ch->entry];

        if (code < TEXT_COMMANDS) {
            switch (code) {
            case TEXT_NEWLINE:
                ch->x = ch->start_x;
                if (ch->work->flags & 8) {
                    if (ch->y > 0xcff) {
                        UiWork_ResetChannelTransition(ch);
                        count = 1;
                    } else {
                        ch->y += 0xd00;
                    }
                } else {
                    ch->y += 0xf00;
                    if (ch->line <= 2)
                        ch->line++;
                }
                break;

            case TEXT_PAGE:
                if (work->mode != 0 && ch->countdown < 900)
                    gKeysPressedLatch = 0;
                ch->countdown = 919;
                if (UiWork_CheckCancelByModeInput(ch) == 0) {
                    struct UiWindow *pane = ch->work;

#if EDITION_INTERNATIONAL
                    if (pane->width == 0 || pane->height == 0
                        || work->page_arrow_shown != 0)
                        break;
#else
                    if (work->page_arrow_shown != 0)
                        break;
#endif
                    UiText_DrawGlyph(pane, 1, pane->width * 4 - 8,
                                     pane->height * 8 - 16, 1);
                    work->page_arrow_shown = 1;
                    break;
                }
                window = ch->work;
                x = window->x;
                y = window->y;
                old_w = window->width;
                old_h = window->height;
                entry = ch->entry;
                work->page_arrow_shown = 0;
                RenderOutput_PrepareForRedraw(window);
                if (ch->flags == 0 && (window->width | window->height) != 0)
                    UiWindow_EraseBorderRect(window->x, window->y,
                                             window->width, window->height);
                entry = (entry + 1) & RENDER_ENTRY_MASK;
                if (work->entries[entry] != 0
                    && (window->width | window->height) != 0) {
                    if (ch->flags != 0) {
                        UiWindow_EraseBorderRect(window->x, window->y,
                                                 window->width, window->height);
                    } else {
                        UiWindow_FitOnScreen(entry, &x, &y, &w, &h,
                                             ch->spacing, 0);
                        if (window->flags & 0x80) {
                            if (old_h != h)
                                y -= h - old_h;
                            if (y < 0)
                                y = 0;
                        }
                        if (!(window->flags & 0x100)) {
                            x += (s32)(old_w - w) / 4;
                            UiWindow_FitOnScreen(entry, &x, &y, &w, &h,
                                                 ch->spacing, 2);
                        }
                        window->x = x;
                        window->y = y;
                        window->width = w;
                        window->height = h;
                    }
                    UiWindow_DrawFrame(window->x, window->y,
                                       window->width, window->height);
                }
                ch->x = ch->start_x;
                ch->y = 0;
                ch->line = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                ch->colour = 15;
                ch->outline = 0;
                ch->line_spacing = 10;
                UiWork_CopyParamsToRenderWork(ch);
#endif
                Resource_ResetEntry(work->glyph_resource);
                work->glyph_resource = 99;
                break;

            case TEXT_WAIT_KEY:
                if (work->mode != 0 && ch->countdown < 900)
                    gKeysPressedLatch = 0;
                if (UiWork_CheckCancelByModeInput(ch) != 0)
                    return 9;
                ch->countdown = 919;
                break;

            case TEXT_PAUSE_SHORT:
                if (ch->countdown == 0)
                    ch->countdown = 20;
                work->result[1] = 0;
                UiWork_CheckCancelByInput(ch);
                break;

            case TEXT_PAUSE_LONGEST:
                if (ch->countdown == 0)
                    ch->countdown = 120;
                work->result[1] = 0;
                UiWork_CheckCancelByInput(ch);
                break;

            case TEXT_PAUSE_LONG:
                if (ch->countdown == 0)
                    ch->countdown = 60;
                work->result[1] = 0;
                break;

            case TEXT_COLOUR:
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                ch->colour = work->entries[ch->entry];
                UiWork_CopyParamsToRenderWork(ch);
                break;

            case TEXT_OUTLINE:
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                ch->outline = work->entries[ch->entry];
                UiWork_CopyParamsToRenderWork(ch);
                break;

            case TEXT_LINE_SPACING:
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                ch->line_spacing = work->entries[ch->entry];
                UiWork_CopyParamsToRenderWork(ch);
                break;

            case TEXT_COLOURS_RESET:
                ch->colour = 15;
                ch->outline = 0;
                ch->line_spacing = 10;
                UiWork_CopyParamsToRenderWork(ch);
                break;

            case TEXT_WINDOW_STATE:
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                ch->work->effect = work->entries[ch->entry];
                ch->countdown = 10;
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                break;

            case TEXT_END:
            case TEXT_END_ALT:
                ch->ended = 1;
                return 8;
            }

            if (work->menu_state == 0)
                count = 1;
        } else {
            gx = (ch->x + 128) / 256;
            gy = (ch->y + 128) / 256;
            cue = UiText_SpeedCueGaps[gGameState.message_speed];
            pc = ch->entry;
#if !EDITION_INTERNATIONAL || defined(TBS_EDITION_EN)
            if (work->mode != 0)
                gx += 8;
#endif
            pc = (pc + 1) & RENDER_ENTRY_MASK;
            next = work->entries[pc];
#if !EDITION_INTERNATIONAL || defined(TBS_EDITION_EN)
            if (next == TEXT_DAKUTEN) {
                code |= 0x4000;
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
            } else if (next == TEXT_HANDAKUTEN) {
                code |= 0x8000;
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
            }
#endif
#if EDITION_INTERNATIONAL
#if defined(TBS_EDITION_ES)
            if (!(ch->work->flags & 8) && code > TEXT_SPACE
                && next > TEXT_SPACE) {
                s16 pair = UiText_Glyphs[code - TEXT_SPACE].width
                    + UiText_Glyphs[next - TEXT_SPACE].width;

                if (work->outline == 1) {
                    if ((u16)pair <= 14) {
                        code |= next << 8;
                        ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                    }
                } else if ((u16)pair <= 15) {
                    code |= next << 8;
                    ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                }
            }
#else
            if (!(ch->work->flags & 8) && code > TEXT_SPACE
                && next > TEXT_SPACE
                && (u16)(UiText_Glyphs[code - TEXT_SPACE].width
                         + UiText_Glyphs[next - TEXT_SPACE].width) <= 15) {
                code |= next << 8;
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
            }
#endif
#endif
            advance = UiText_DrawGlyph(ch->work, code, gx, gy, 0);
            ch->glyph_delay = UiText_SpeedDelays[gGameState.message_speed];
            if (advance != 0) {
                if (work->result[0] != 0) {
                    if (work->result[1] == 0) {
                        if (code != TEXT_SPACE) {
                            AudioCommand_PlayFar(work->result[0] + (code & 3));
                            work->result[1] = cue;
                        }
                    } else {
                        work->result[1]--;
                    }
                }
                width = advance << 8;
#if EDITION_INTERNATIONAL
                if (code == TEXT_SPACE)
                    width += ch->spacing[ch->line];
#else
                width += ch->spacing[ch->line];
#endif
                ch->x += width;
            }
#if !EDITION_INTERNATIONAL
            else
                return 9;
#endif
            if (code == TEXT_SPACE && work->menu_state == 0)
                count = 1;
        }

        if (ch->countdown != 0) {
            ch->countdown--;
            if (ch->countdown != 0)
                continue;
        }
        ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
    } while (--count != 0);

    return 0;
}
