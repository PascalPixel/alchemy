/* 2026-10-05: the Japanese UiWork_StepChannelScript, at 080168a0 in the
   Japanese ROM, for games/THE BROKEN SEAL/SRC/GRAPHICS/WINDOW/
   RUNTIME_PROCESS_RENDER_CHANNELS.C, where the five localized editions
   match (compile it with the Japanese edition macro). The Japanese text
   step has the voicing marks and the menu shift English kept, no
   two-glyph cells, a page prompt that only checks the prompt flag, and
   always adds the line's extra space width to the pen.
   Every instruction lines up but one block placement: the reference keeps
   the wait-key cancel's "return 9" (movs r0, #9; b to the epilogue) after
   the pen advance of the glyph path, followed by a literal pool, and jumps
   over both into the code == 32 test; this build leaves it where the
   localized builds have it, before the loop after the delay's return.
   Measured against the Japanese ROM with a ROM-region compare (560 of 587
   instructions common; the rest are that block and the branch and pool
   offsets it moves). A for (;;) loop with the count test at its foot
   changes nothing; return 9 written after a break-guarded wait (552) and
   the pen advance as one expression (533) are worse. */
#include "TYPES.H"
#include "WINDOW.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "GLYPH.H"
#include "RESOURCE.H"

void UiWork_ShiftPanelRowsLeft(s32 step);

extern volatile u32 gKeysHeld;
extern u32 gKeysPressedLatch;
extern volatile u16 gLagFramesShown;
extern u8 gCell[];

/* Indexed by the message speed setting: frames to wait after a glyph,
   glyphs drawn a frame, and glyphs between two voice cues. */
extern const u8 UiText_SpeedDelays[3];
extern const u8 UiText_SpeedSteps[3];
extern const u8 UiText_SpeedCueGaps[3];

void UiWork_ResetChannelTransition(struct UiChannelSlot *channel);
void RenderOutput_PrepareForRedraw(struct UiWindow *window);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height);
void UiWork_CopyParamsToRenderWork(struct UiChannelSlot *channel);
s32 UiWork_CheckCancelByInput(struct UiChannelSlot *channel);
s32 UiWork_CheckCancelByModeInput(struct UiChannelSlot *channel);
void AudioCommand_PlayFar(s32 cue);

/* The message speed setting. */
#define MESSAGE_SPEED gCell[0x20c]

/* Text codes below 31 are commands; the rest are glyphs. */
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

/*
 * Run one frame of a text channel's script: draw the next glyphs, or carry
 * out the command codes met on the way. Returns 8 when the text ended and 9
 * when a key wait was cancelled, otherwise 0.
 */
s32 UiWork_StepChannelScript(struct UiChannelSlot *ch)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u32 keys = gKeysHeld;
    s32 count = UiText_SpeedSteps[MESSAGE_SPEED];
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
    if (gKeysHeld == 0 && ch->unknown_22 != 0) {
        ch->unknown_22--;
        return 0;
    }

    do {
        code = TEXT_END;
        if (ch->unknown_20 == 0)
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
                    if (ch->unknown_10 <= 2)
                        ch->unknown_10++;
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
                        || work->unknown_after_result != 0)
                        break;
#else
                    if (work->unknown_after_result != 0)
                        break;
#endif
                    UiText_DrawGlyph(pane, 1, pane->width * 4 - 8,
                                     pane->height * 8 - 16, 1);
                    work->unknown_after_result = 1;
                    break;
                }
                window = ch->work;
                x = window->x;
                y = window->y;
                old_w = window->width;
                old_h = window->height;
                entry = ch->entry;
                work->unknown_after_result = 0;
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
                                             ch->colours, 0);
                        if (window->flags & 0x80) {
                            if (old_h != h)
                                y -= h - old_h;
                            if (y < 0)
                                y = 0;
                        }
                        if (!(window->flags & 0x100)) {
                            x += (s32)(old_w - w) / 4;
                            UiWindow_FitOnScreen(entry, &x, &y, &w, &h,
                                                 ch->colours, 2);
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
                ch->unknown_10 = 0;
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
                ch->work->unknown_12 = work->entries[ch->entry];
                ch->countdown = 10;
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                break;

            case TEXT_END:
            case TEXT_END_ALT:
                ch->unknown_20 = 1;
                return 8;
            }

            if (work->menu_state == 0)
                count = 1;
        } else {
            gx = (ch->x + 128) / 256;
            gy = (ch->y + 128) / 256;
            cue = UiText_SpeedCueGaps[MESSAGE_SPEED];
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
            /* Two narrow glyphs share one cell. */
#if defined(TBS_EDITION_ES)
            if (!(ch->work->flags & 8) && code > TEXT_SPACE
                && next > TEXT_SPACE) {
                s16 pair = UiText_Glyphs[code - TEXT_SPACE].width
                    + UiText_Glyphs[next - TEXT_SPACE].width;

                /* Outlined text leaves one pixel less. */
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
            ch->unknown_22 = UiText_SpeedDelays[MESSAGE_SPEED];
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
                    width += ch->colours[ch->unknown_10];
#else
                width += ch->colours[ch->unknown_10];
#endif
                ch->x += width;
            }
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
