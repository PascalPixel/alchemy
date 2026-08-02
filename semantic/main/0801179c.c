#include "layout_guard.h"
#include "types.h"

struct DmaTransfer_0801179c {
    const void *source;
    void *destination;
    u32 control;
};

struct TileScriptChannel_0801179c {
    const u16 *script;
    const u16 *cursor;
    u16 delay;
    u16 suspended;
};

struct TileScriptRuntime_0801179c {
    u8 padding00[0x16];
    u8 wide_tiles;
    u8 padding17;
    struct TileScriptChannel_0801179c channels[16];
};

LAYOUT_OFFSET_GUARD(
    TileScriptRuntime0801179c_WideTiles,
    struct TileScriptRuntime_0801179c,
    wide_tiles,
    0x16);
LAYOUT_OFFSET_GUARD(
    TileScriptRuntime0801179c_Channels,
    struct TileScriptRuntime_0801179c,
    channels,
    0x18);
LAYOUT_SIZE_GUARD(
    TileScriptChannel0801179c_Size,
    struct TileScriptChannel_0801179c,
    0x0c);

extern struct TileScriptRuntime_0801179c *Data_03001e70;

/*
 * Advance all sixteen tile-transfer scripts.  Ordinary commands contain four
 * halfwords: source tile, transfer span, destination tile, and delay.  0xffff
 * restarts at the script base; 0xfeNN jumps to base + NN*4 bytes; 0xfeff is
 * terminal.  A zero-delay channel may execute several commands in one tick.
 */
void Func_0801179c(void)
{
    struct TileScriptRuntime_0801179c *runtime = Data_03001e70;
    volatile struct DmaTransfer_0801179c *dma =
        (volatile struct DmaTransfer_0801179c *)0x040000d4;
    s32 channel_index;

    for (channel_index = 0; channel_index < 16; channel_index++) {
        struct TileScriptChannel_0801179c *channel =
            &runtime->channels[channel_index];

        if (channel->script == 0 || channel->suspended != 0)
            continue;

        for (;;) {
            const u16 *command;
            u16 opcode;

            if (channel->delay != 0) {
                channel->delay--;
                break;
            }

            command = channel->cursor;
            opcode = command[0];
            if (opcode == 0xffff) {
                channel->cursor = channel->script;
                continue;
            }
            if ((opcode & 0xff00) == 0xfe00) {
                u16 target = opcode & 0xff;

                if (target != 0xff) {
                    channel->cursor = (const u16 *)(
                        (const u8 *)channel->script + target * 4);
                    continue;
                }
                break;
            }

            if (runtime->wide_tiles == 0) {
                dma->source = (const void *)(
                    (opcode >= 0x600 ? 0x0201c000 : 0x06004000) +
                    opcode * 32);
                dma->destination =
                    (void *)(0x06004000 + command[2] * 32);
                dma->control = 0x84000000 | command[1] * 8;
            } else {
                dma->source = (const void *)(
                    (opcode >= 0x200 ? 0x02020000 : 0x06008000) +
                    opcode * 64);
                dma->destination =
                    (void *)(0x06008000 + command[2] * 64);
                dma->control = 0x84000000 | command[1] * 16;
            }

            channel->delay = command[3];
            channel->cursor += 4;
        }
    }
}
