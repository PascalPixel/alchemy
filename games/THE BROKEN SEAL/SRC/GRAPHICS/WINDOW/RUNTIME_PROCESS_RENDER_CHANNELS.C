#include "TYPES.H"
#include "WINDOW.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "DMA.H"
#include "IO_REG.H"
#include "IWRAM_CALL.H"

/* The sliding panel's tile pixels in VRAM: thirty rows of thirty-two words,
 * of which the last twenty-four are the panel. */
#define PANEL_TILE_ROWS ((u32 *)0x06002500)
#define PANEL_ROW_COUNT 30
#define PANEL_ROW_WORDS 32
#define PANEL_ROW_LEAD 8

/*
 * Slide the panel's pixels left by six words a step: every row's panel
 * words move down over the first ones and the words uncovered at the end
 * of the row are cleared.
 */
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

s32 UiWork_StepChannelScript(void *);
void UiWork_AdvanceChannelTransition(struct UiChannelSlot *);

void UiWork_ProcessRenderChannels(void)
{
    struct UiChannelSlot *channel =
        (struct UiChannelSlot *)(gWindowWork[0] + RENDER_CHANNEL_OFS);
    s32 channel_no = 0;
    s32 one = 1;

    do {
        struct UiWindow *current = channel->work;

        if (current != 0 && *(s32 *)&current->frame == 0) {
            u16 flags = current->flags;

            if (flags == 0) {
                channel->work = NULL;
            } else {
                s32 pending = current->unknown_12;
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
                            (u16)(entity->flags & 2)
                        );
                        channel->x = pending;
                        channel->y = pending;
                        channel->entry = pending;
                        channel->countdown = pending;
                        channel->unknown_16 = pending;
                        channel->unknown_18 = pending;
                        channel->unknown_1a = pending;
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
