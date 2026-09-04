#include "types.h"

#define Party_CheckMemberValueTotal Func_080919d8

extern u8 Data_02000240[];
extern u8 Value_0000097d;

extern s32 Func_08077148(s32);
extern s32 Func_080772d8(u8, s32);
extern void UiText_DrawQuantity(s32, s32);
extern void UiText_DrawMessage(void *, s32);

s32 Party_CheckMemberValueTotal(s32 id)
{
    s32 count;
    s32 value;
    s32 offset = 0;
    s32 cnt;
    s32 sum = offset;
    u8 *p;

    count = Func_08077148(id);
    if (sum < count) {
        offset = 252;
        offset <<= 1;
        p = Data_02000240 + offset;
        cnt = count;
        do {
            value = Func_080772d8(*p, id);
            cnt--;
            p++;
            sum += value;
        } while (cnt != 0);
    }

    if (sum >= count * 30) {
        UiText_DrawQuantity(id, 2);
        UiText_DrawMessage(&Value_0000097d, 1);
        UiText_DrawQuantity(id, 2);
        UiText_DrawMessage(&Value_0000097d + 1, 1);
        return -1;
    }
    return 0;
}
