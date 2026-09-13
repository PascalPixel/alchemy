#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

/* link/serial/enable_transfer_timer.c */
void SerialRuntime_EnableTransferTimer(void)
{
    s32 state = 0x02002240;
    if (FIELD_AT_OFFSET((void *)state, u8 *, 0) != 0)
        FIELD_AT_OFFSET((void *)state, s8 *, 8) = 1;
}
