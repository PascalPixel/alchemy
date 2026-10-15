/* Draft (tla-en): listing 74 instructions, draft 74, common 67, differ 14.
 * Two differences left. After the call the native loads 0x784 from the pool
 * before it starts BG2CNT's address (and again for BG2PA); the draft starts
 * the three-instruction address first. And the native's halfword pool sits
 * before the outer loop's label, four instructions earlier: its pool range
 * (64) ran out there. A one-pass block after the five register writes moves
 * the pool to the native place (differ 10) but takes the hoisted 7 out of
 * the call's delay slot; one around the call and all that follows keeps the
 * pool right and leaves only the two address orders (differ 8). The
 * if/else-if order of the level (y < 32, y > 95, else 63) is the native's:
 * jump optimisation turns it into its "level = y * 2" default.
 */
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"

void Func_0815b410(void);

void Func_08179e6c(void)
{
    u8 *canvas = Ram_HeapSlots->battle_fx_work[1];
    s32 y;
    s32 x;

    Func_0815b410();
    REG_BG2CNT = 0x784;
    PLTT_BACKDROP = 0;
    REG_BG2PA = 0x4e;
    REG_BLDALPHA = 0x1010;
    REG_BLDCNT = 0x3f44;
    for (y = 0; y != 128; y++) {
        for (x = 0; x != 128; x++) {
            s32 level;

            if (y < 32)
                level = y * 2;
            else if (y > 95)
                level = 255 - y * 2;
            else
                level = 63;
            canvas[(((x / 8) * 16 + y / 8) * 8 + (x & 7)) * 8 + (y & 7)] = level;
        }
    }
}
