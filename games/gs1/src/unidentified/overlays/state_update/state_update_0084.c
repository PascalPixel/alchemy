#include "types.h"

extern u8 Data_0200a1b9[];
extern u8 Data_0200c57e[];
extern u8 Data_0200be4e[];
extern u8 Data_0200c5aa[];
extern u8 Data_0200be76[];
extern u8 Data_0200c628[];

extern u16 Data_0200c790;
extern u16 Data_0200c764;
extern u16 Data_0200c79c;
extern u8 *Data_0200c7a0;
extern u16 Data_0200c7f8;
extern u16 Data_0200c76c;
extern u32 Data_0200c770;

extern void Func_020061d0(u8 *, s32);

void Func_020026a8(s32 a, s32 b)
{
    u8 *p;

    Data_0200c790 = a;
    Data_0200c764 = b << 4;
    Func_020061d0(Data_0200a1b9, 3200);
    p = Data_0200c57e;
    if (a == 2) {
        p = Data_0200be4e;
    }
    if (a == 4) {
        p = Data_0200c5aa;
    }
    if (a == 3) {
        if (b != 0) {
            p = Data_0200be76;
        } else {
            p = Data_0200c628;
        }
    }
    Data_0200c79c = 0;
    Data_0200c7a0 = p;
    Data_0200c7f8 = 0;
    Data_0200c76c = 0;
    Data_0200c770 = 0;
}
