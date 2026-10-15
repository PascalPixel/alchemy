/* Draft of Func_020006bc (overlay 679, 74 instructions), differ 31: cycles the 15
 * colours it first captured from BG palette 4 (0x05000080, into gData_02002a58) into
 * gData_02002a78, one step every second frame, and queues their DMA to OBJ palette 14 (0x050003c0). Written from
 * the listing (the twin map's ☀️ hint is a false match). Left:
 *  - loop.c moves both array addresses out of the loop and makes a pointer of
 *    gData_02002a58[i + 1]; the native keeps both pool loads in the loop (gData_02002a78
 *    in r8, reused after it for the queue's value) and steps an offset in r6;
 *  - after the capture the native reloads the counter for the base; here the stored
 *    zero's register is reused.
 * Measured: the loop as a goto (no strength reduction at all, 48); the destination as
 * a pointer set in the loop and used after it (32); the base computed inside the loop
 * (51). */
#include "../../../../games/tla/src/scene/apojii_islands/local.h"
#include "io_write_queue.h"

extern s32 gData_020024cc[];
extern u16 gData_02002a58[16];
extern u16 gData_02002a78[16];

s32 __modsi3(s32 a, s32 b);

void Func_020006bc(void)
{
    s32 base;
    s32 i;

    if (gData_020024cc[0] == -1) {
        Iwram_CopyWords(gData_02002a58, (void *)0x05000080, 32);
        gData_020024cc[0] = 0;
    }
    base = (gData_020024cc[0] / 2) % 15;
    for (i = 0; i < 15; i++)
        gData_02002a78[(base + i) % 15 + 1] = gData_02002a58[i + 1];
    IO_WRITE_QUEUE_PUSH(0x050003c0, (u32)gData_02002a78, 0x84000008);
    gData_020024cc[0]++;
}
