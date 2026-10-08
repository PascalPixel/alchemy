/* Near miss: listing 31 instructions, draft 31, common 30, differ 2; the rest differs in instruction order. */
#include "trackbuf.h"

void Func_081b3fb0(u8 *destination)
{
    struct AudioTrackSlotWork *state = Flash_Handler3;
    s32 i;

    for (i = 0; i != state->out_cnt; i++) {
        destination[state->out_total] = state->out_buf[i];
        state->out_total += 1;
    }
}
