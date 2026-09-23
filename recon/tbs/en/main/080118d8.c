/* Draft, not exact (2026-09-24): candidate=172 reference=172 differing_halfwords=65. Constants the reference loads from
   the literal pool are spelled as link-time Value_ symbols, which restores
   the reference size; wraps marked FAKEMATCH only move scheduling. */
#include "TYPES.H"
extern u8 Value_0000ffff;
extern u8 Value_0000ff00;
extern u8 Value_0000fd00;
extern u8 Value_00000c80;
/* Draft, not exact: 65 differing halfwords, 168-byte candidate for the
   172-byte owner (2026-09-23). Residual: the reference builds the looping
   flag with a branch (movs r0, #0; beq; movs r0, #1) and hoists all five
   loop constants (0xff00, 0xfd00, 15, 0x80, 0) into sl, r8, lr, ip and r7;
   here the flag becomes a shift. With an if-form flag (out of 172 bytes, 85
   halfwords) only three constants are hoisted. */

#include "DMA.H"

s32 Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
void MapAnimation_Update(void);

extern u8 *Data_03001e70;

/* One of sixteen tile-animation channels in the map state. */
struct MapAnimation {
    const u16 *start;
    const u16 *cursor;
    u16 timer;
    u16 looping;
};

/* Reads a 0xffff-terminated command list; each 0xfdXX command starts channel
   XX & 15 on the commands that follow it. */
void MapAnimation_Start(const u16 *script)
{
    u8 *state;
    struct MapAnimation *channel;
    s32 count;
    u32 command;
    volatile u32 zero;

    state = Data_03001e70;
    count = 0;
    zero = 0;
    Dma_Set((const void *)&zero, state + 24, 0x85000030, (volatile u32 *)0x040000d4);
    command = *script++;
    while (command != 0xffff) {
        if ((command & 0xff00) == 0xfd00) {
            channel = (struct MapAnimation *)(state + 24) + (command & 15);
            channel->start = script;
            channel->cursor = script;
            channel->timer = 0;
            channel->looping = (command & 0x80) ? 1 : 0;
            count++;
        }
        command = *script++;
    }
    if (count != 0)
        Scheduler_AddOrUpdateCallback(MapAnimation_Update, (s32)&Value_00000c80);
}
