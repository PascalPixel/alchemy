/* Near miss: differ 14 (listing 83 instructions, draft 83). In the copy loop
 * the native reloads the colour table's address from the pool and indexes it
 * by an offset that starts at 4; the draft hoists the address plus 4 and the
 * stack buffer out of the loop. */
#include "../../../../games/tla/src/scene/jupiter_aerie/jupiter_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

void Func_0200079c(void)
{
    u16 colors[16];
    s32 phase = (gData_02003314 / 4) % 5;
    s32 i;

    if (gData_02003318 != 0) {
        u8 *work = Ram_HeapSlots->eventWork;
        s16 *state = (s16 *)(work + 358);

        if (*state != 0 && (*state & 0x3ff) != 153)
            return;
        if (*(s8 *)(work + 0xca4) != 0)
            return;
        if (*(s16 *)(work + 350) != 0)
            return;
    }
    for (i = 0; i <= 4; i++)
        colors[(phase + i) % 5] = gData_02003340[i + 2];
    Dma_Set(colors, (void *)0x05000184, 0x84000002, REG_DMA3);
    gData_02003314++;
}
