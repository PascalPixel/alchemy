#include "types.h"

extern s32 Data_03001e40;
extern s32 Data_0200d240[3];   /* image offset 0x5240 */

extern u8 *Func_02006652(s32 arg0, s32 arg1, s32 arg2, s32 arg3);  /* site veneer -> Func_080090c8 */
extern void Func_0200559e(u8 *record);                             /* site veneer -> Func_02002a10 */
extern void Func_0200666a(u8 *record, s32 arg1);                   /* site veneer -> Func_08009080 */

void Func_02002b58(void)
{
    u8 *record;

    if ((Data_03001e40 & 3) != 0) {
        return;
    }
    record = Func_02006652(222, Data_0200d240[0], Data_0200d240[1], Data_0200d240[2]);
    if (record == 0) {
        return;
    }
    {
        u16 *p = (u16 *)(record + 100);
        u16 v = 30;
        *p = v;
    }
    {
        u16 *q = (u16 *)(record + 102);
        u16 w = 1;
        *q = w;
    }
    *(s32 *)(record + 104) = 20;
    Func_0200559e(record);
    *(s32 *)(record + 108) = 0x0200aa49;
    Func_0200666a(record, 1);
}
