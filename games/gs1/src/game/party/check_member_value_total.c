#include "types.h"
#include "scene.h"

extern u8 gCell[];
extern u8 Value_0000097d;

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

    count = Party_Check(id);
    if (sum < count) {
        offset = 252;
        offset <<= 1;
        p = gCell + offset;
        cnt = count;
        do {
            value = Party_Apply(*p, id);
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
