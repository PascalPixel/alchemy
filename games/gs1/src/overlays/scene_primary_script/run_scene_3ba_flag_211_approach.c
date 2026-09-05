#include "types.h"

extern u8 Data_02000240[];

s32 Func_02006d18();           /* veneer to GameFlag_IsSet */
u8 *Func_02006d88();           /* veneer to Scene_GetRecord */
void Func_02006d02();          /* veneer to Object_SetPosition */
void Func_02006d90();          /* veneer to GameFlag_Set */
void Func_02006ce8();          /* veneer to Object_SetCallback */
void Func_02006c4e();          /* veneer to Func_080000c0 */
s32 Func_02005242();           /* local thunk to Func_020020e8, site A */
s32 Func_02005254();           /* local thunk to Func_020020e8, site B */
void Func_02006d82();          /* veneer to UiText_DrawQuantity, site A */
void Func_02006d94();          /* veneer to UiText_DrawQuantity, site B */
void Func_02006da4();          /* shared veneer, selector refresh + 0x96a */
void Func_02006d62();          /* veneer to Func_08009148 */

s32 FieldScene_RunFlag211ApproachScene(s32 handle_a, s32 handle_b)
{
    u8 *work = *(u8 **)0x03001f3c;
    u8 *shared;
    u8 *rec;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = Func_02006d18(0x211);

    shared = Data_02000240;
    rec = Func_02006d88(*(s32 *)(shared + 500));

    if (*(s32 *)(work + 232) < *(s32 *)(rec + 8)) {
        x = *(s32 *)(work + 232) + 0xc0000;
    } else {
        x = *(s32 *)(work + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(work + 236) + 0x100000;
        cuep = (u16 *)(work + 228);
    } else {
        z = *(s32 *)(work + 236) - 0x100000;
        cuep = (u16 *)(work + 226);
    }

    waitp = (s16 *)(rec + 100);
    *waitp = *cuep;
    *(s32 *)(rec + 52) = 0x4000;
    *(s32 *)(rec + 48) = 0x10000;

    Func_02006d02(rec, x, 0, z);
    Func_02006d90(0x211);
    Func_02006ce8(rec, (void *)0x0200c6fc);

    while (*waitp != 0) {
        Func_02006c4e(1);
    }

    if (flag == 0) {
        Func_02005242(0, handle_a);
        Func_02006d82(handle_a, 2);
    } else {
        Func_02005254(0, handle_b);
        Func_02006d94(handle_b, 2);
    }

    shared = Data_02000240;
    Func_02006da4(*(s32 *)(shared + 500), 1);
    Func_02006da4(0x96a, 3);
    Func_02006d62(rec);

    return flag;
}
