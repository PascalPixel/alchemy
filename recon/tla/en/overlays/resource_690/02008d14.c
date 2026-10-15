/* Draft of Func_02000d14 (overlay 690, 240 bytes), differ 71 of 107: the logic
   is right (the palette saved once, four colours rotated by the frame count,
   one queued copy). The native keeps the counter's address in r7 and the zero
   it stores there in r6, and holds the copy routine in r8 for both calls; this
   form allocates them the other way round and reads the counter once. The
   cycle loop as a goto loop is further away (differ 85): the native steps a
   byte offset by 2, as a for loop does. */
#include "../../../../../games/tla/src/scene/settlements/local.h"
#include "io_write_queue.h"

extern s32 gData_02002c5c;
extern u16 gData_02002c60[16];
extern u16 gData_02002c80[16];

void Func_02000d14(void)
{
    u8 *work = Ram_HeapSlots->eventWork;

    if (*(s16 *)(work + 358) == 0 && *(s8 *)(work + 0xca4) == 0) {
        s32 phase;
        s32 i;

        if (gData_02002c5c == -1) {
            Iwram_CopyWords(gData_02002c60, (void *)0x05000060, 32);
            Iwram_CopyWords(gData_02002c80, gData_02002c60, 32);
            gData_02002c5c = 0;
        }
        phase = gData_02002c5c / 8 % 4;
        for (i = 0; i <= 3; i++)
            gData_02002c80[(phase + i) % 4 + 12] = gData_02002c60[i + 12];
        IO_WRITE_QUEUE_PUSH(0x05000060, (u32)gData_02002c80, 0x84000008);
        gData_02002c5c++;
    }
}
