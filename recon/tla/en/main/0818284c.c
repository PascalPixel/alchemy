/* Near miss, differ 2 of 31: the native loads the blend word 0x3f44 from the
 * pool before it starts the register address (ldr r3; movs r2, #128); this
 * starts the address first. A forced temporary with an asm barrier splits
 * the constant instead of pooling it (differ 10). */
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"

void BattleFx_BeginCanvasLayer(s32 mode);
void Func_08182898(s32 arg);

void Func_0818284c(s32 arg)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];

    BattleFx_BeginCanvasLayer(0);
    REG_BLDCNT = 0x3f44;
    REG_BLDALPHA = 0x1010;
    *(s32 *)(work + 0x77b4) = 24;
    *(s32 *)(work + 0x77b8) = 0;
    Func_08182898(arg);
}
