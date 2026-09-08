#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000007e;
extern u8 Value_000008c8;
extern u8 Data_0200b2bc[];

s32 Func_0200441c();
void Func_0200443c();
void Func_020044be();
void Func_0200450e();
void Func_020044d2();
void Func_02004486();
void Func_02004584();
void Func_0200454a();

void DispatchPrologueSceneSlot(void)
{
    u8 *table;
    s16 *slot;
    s32 idx;

    table = (u8 *)Data_02000240;
    {
        s32 off = 448;
        slot = (s16 *)(table + off);
    }

    if (Func_0200441c(*slot + ((s32)&Value_000008c8 - (s32)&Value_0000007e)) == 0) {
        Func_0200443c();
        Func_020044be(*slot, 5);
        table[0x22b] = 3;
        idx = *slot - (s32)&Value_0000007e;
        switch (idx) {
        case 0:
            Func_0200450e(63, 0);
            break;
        case 1:
            Func_0200450e(63, 1);
            break;
        case 2:
            Func_0200450e(63, 2);
            break;
        case 3:
            Func_0200450e(63, 3);
            break;
        case 4:
            Func_0200450e(84, 0);
            break;
        case 5:
            Func_0200450e(84, 1);
            break;
        case 6:
            Func_0200450e(84, 2);
            break;
        case 7:
            Func_0200450e(84, 3);
            break;
        case 8:
            Func_0200450e(84, 4);
            break;
        }
        Func_020044d2();
    } else {
        Func_02004486(Data_0200b2bc, 44, 7);
        Func_02004584(183);
        Func_0200454a(3);
    }
}
