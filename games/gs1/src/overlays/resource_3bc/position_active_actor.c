#include "types.h"

extern u8 Data_02000240[];

s32 Func_020087a0();           /* veneer to Func_080770c0 */
u8 *Func_02008818();           /* veneer to Func_0808a080 */
void Func_0200876a();          /* veneer to Func_08009150 */
void Func_02008750();          /* veneer to Func_08009098 */
void Func_020086b6();          /* veneer to Func_080000c0 */
s32 Func_02006ca2();           /* local thunk to Func_020020e8, site A */
s32 Func_02006cb4();           /* local thunk to Func_020020e8, site B */
void Func_0200880a();          /* veneer to Func_08015120, site A */
void Func_0200881c();          /* veneer to Func_08015120, site B */
void Func_0200882c();          /* shared veneer, selector refresh + 0x96a */
void Func_020087ca();          /* veneer to Func_08009148 */

s32 Func_02003ddc(s32 handleA, s32 handleB)
{
    u8 *workspace = *(u8 **)0x03001f3c;
    u8 *shared;
    u8 *record;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = Func_020087a0(0x211);

    shared = Data_02000240;
    record = Func_02008818(*(s32 *)(shared + 500));

    if (*(s32 *)(workspace + 232) < *(s32 *)(record + 8)) {
        x = *(s32 *)(workspace + 232) + 0xc0000;
    } else {
        x = *(s32 *)(workspace + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(workspace + 236) + 0x100000;
        cuep = (u16 *)(workspace + 228);
    } else {
        z = *(s32 *)(workspace + 236) - 0x100000;
        cuep = (u16 *)(workspace + 226);
    }

    waitp = (s16 *)(record + 100);
    *waitp = *cuep;
    *(s32 *)(record + 52) = 0x4000;
    *(s32 *)(record + 48) = 0x10000;

    Func_0200876a(record, x, 0, z);
    Func_02008818(0x211);
    Func_02008750(record, (void *)0x0200db24);

    while (*waitp != 0) {
        Func_020086b6(1);
    }

    if (flag == 0) {
        Func_02006ca2(0, handleA);
        Func_0200880a(handleA, 2);
    } else {
        Func_02006cb4(0, handleB);
        Func_0200881c(handleB, 2);
    }

    shared = Data_02000240;
    Func_0200882c(*(s32 *)(shared + 500), 1);
    Func_0200882c(0x96a, 3);
    Func_020087ca(record);

    return flag;
}
