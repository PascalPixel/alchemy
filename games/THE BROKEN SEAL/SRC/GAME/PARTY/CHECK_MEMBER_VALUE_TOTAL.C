#include "TYPES.H"
#include "SCENE.H"

extern u8 gGameState[];
extern u8 Value_0000097d;

extern void UiWork_PushValueSlotFar(s32, s32);
extern void UiText_ShowPositionedMessageAndWaitFar(void *, s32);

s32 Party_CheckMemberValueTotal(s32 id)
{
    s32 count;
    s32 value;
    s32 offset = 0;
    s32 cnt;
    s32 sum = offset;
    u8 *p;

    count = Party_CountActiveOwnersFar(id);
    if (sum < count) {
        offset = 252;
        offset <<= 1;
        p = gGameState + offset;
        cnt = count;
        do {
            value = Inventory_CountItemFar(*p, id);
            cnt--;
            p++;
            sum += value;
        } while (cnt != 0);
    }

    if (sum >= count * 30) {
        UiWork_PushValueSlotFar(id, 2);
        UiText_ShowPositionedMessageAndWaitFar(&Value_0000097d, 1);
        UiWork_PushValueSlotFar(id, 2);
        UiText_ShowPositionedMessageAndWaitFar(&Value_0000097d + 1, 1);
        return -1;
    }
    return 0;
}
