#include "types.h"

extern u8 Data_02000240[];

s32 Func_02006d18();           /* veneer to Func_080770c0 */
u8 *Func_02006d88();           /* veneer to Func_0808a080 */
void Func_02006d02();          /* veneer to Func_08009150 */
void Func_02006d90();          /* veneer to Func_080770c8 */
void Func_02006ce8();          /* veneer to Func_08009098 */
void Func_02006c4e();          /* veneer to Func_080000c0 */
s32 Func_02005242();           /* local thunk to Func_020020e8, site A */
s32 Func_02005254();           /* local thunk to Func_020020e8, site B */
void Func_02006d82();          /* veneer to Func_08015120, site A */
void Func_02006d94();          /* veneer to Func_08015120, site B */
void Func_02006da4();          /* shared veneer, selector refresh + 0x96a */
void Func_02006d62();          /* veneer to Func_08009148 */

s32 Func_020030ac(s32 handleA, s32 handleB)
{
    u8 *workspace = *(u8 **)0x03001f3c;
    u8 *shared;
    u8 *record;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = Func_02006d18(0x211);

    shared = Data_02000240;
    record = Func_02006d88(*(s32 *)(shared + 500));

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

    Func_02006d02(record, x, 0, z);
    Func_02006d90(0x211);
    Func_02006ce8(record, (void *)0x0200c6fc);

    while (*waitp != 0) {
        Func_02006c4e(1);
    }

    if (flag == 0) {
        Func_02005242(0, handleA);
        Func_02006d82(handleA, 2);
    } else {
        Func_02005254(0, handleB);
        Func_02006d94(handleB, 2);
    }

    shared = Data_02000240;
    Func_02006da4(*(s32 *)(shared + 500), 1);
    Func_02006da4(0x96a, 3);
    Func_02006d62(record);

    return flag;
}
