#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"

#define PANEL_TILE_ROWS ((u32 *)0x06002500)
#define PANEL_ROW_COUNT 30
#define PANEL_ROW_WORDS 32
#define PANEL_ROW_LEAD 8

struct Work;

void UiWork_Finalize(struct Work *, s32);
s32 Func_08039864(void *);
void Func_0803cbf8(void *);

void UiWork_ProcessRenderChannels(void)
{
    u8 *channel = (u8 *)Ram_HeapSlots->window_tiles + 0x6b8;
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
                    Func_0803cbf8(channel);
                } else {
                    kind = Func_08039864(channel);
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
