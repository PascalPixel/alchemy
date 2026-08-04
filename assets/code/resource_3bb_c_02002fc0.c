#include "types.h"

/* Seed the marker cycle directly from caller coordinates and install it. */
extern u16 Data_0200cc94;
extern u16 Data_0200cc20;
extern u16 Data_0200cbf8;
extern u16 Data_0200cc14;
extern u16 Data_0200cc2c;

/* Per-site veneers (raw sub_ symbols from the overlay .s). */
extern void Func_02005e32(u32 x, u32 y, u32 style);
extern void Func_02006d9c(s32 task_address, s32 frame_budget);

void Func_02002fc0(u32 x, u32 y, u32 style)
{
    Func_02005e32(x, y, style);

    Data_0200cc94 = (u16)x;
    Data_0200cc20 = (u16)y;
    Data_0200cbf8 = (u16)(style & 3);
    Data_0200cc14 = 0;
    Data_0200cc2c = 0;

    {
        s32 frame_budget = 0xc80;
        Func_02006d9c(0x0200ae85, frame_budget);
    }
}
