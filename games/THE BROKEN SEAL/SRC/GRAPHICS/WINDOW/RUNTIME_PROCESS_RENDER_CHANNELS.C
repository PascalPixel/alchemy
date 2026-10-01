#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "DMA.H"
#include "IO_REG.H"
#include "IWRAM_CALL.H"
extern u8 Data_03001e8c[];

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

struct Work;

void UiWork_Finalize(struct Work *, s32);
s32 UiWork_StepChannelScript(void *);
void UiWork_AdvanceChannelTransition(void *);

void UiWork_ProcessRenderChannels(void)
{
    u8 *channel = *(u8 **)((u32)&Data_03001e8c) + RENDER_CHANNEL_OFS;
    s32 channel_no = 0;
    s32 one = 1;

    do {
        u8 *current = *(u8 **)channel;

        if (current != 0 && *(s32 *)(current + 0x18) == 0) {
            u16 flags = *(u16 *)(current + 0x16);

            if (flags == 0) {
                *(u8 **)channel = (u8 *)0;
            } else {
                s32 pending = *(u16 *)(current + 0x12);
                s32 kind;

                if (pending != 0) {
                    UiWork_AdvanceChannelTransition(channel);
                } else {
                    kind = UiWork_StepChannelScript(channel);
                    switch (kind) {
                    case 8:
                        *(u16 *)(*(u8 **)channel + 0x14) = one;
                        break;
                    case 9:
                    {
                        u8 *entity = *(u8 **)channel;

                        UiWork_Finalize(
                            (struct Work *)entity,
                            (u16)(*(u16 *)(entity + 0x16) & 2)
                        );
                        *(u16 *)(channel + 0x04) = pending;
                        *(u16 *)(channel + 0x06) = pending;
                        *(u16 *)(channel + 0x12) = pending;
                        *(u16 *)(channel + 0x14) = pending;
                        *(u16 *)(channel + 0x16) = pending;
                        *(u16 *)(channel + 0x18) = pending;
                        *(u16 *)(channel + 0x1a) = pending;
                        *(u16 *)(*(u8 **)channel + 0x14) = one;
                        break;
                    }
                    }
                }
            }
        }
        channel_no++;
        channel += 0x28;
    } while (channel_no != 3);
}
