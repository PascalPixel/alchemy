/* Near miss, differ 2: the listing loads the handler before it builds the interrupt number (ldr r2; movs r0;
 * movs r1); this draft builds the number first. A forced temporary for the handler swaps r2 and r3 above it. */
#include "types.h"
#include "io_reg.h"
#include "irq.h"

void Func_08045780(void);

void Func_08045794(void)
{
    u32 zero = 0;

    REG_BG0VOFS = zero;
    Runtime_SetIrqHandler(2, 136, Func_08045780);
}
