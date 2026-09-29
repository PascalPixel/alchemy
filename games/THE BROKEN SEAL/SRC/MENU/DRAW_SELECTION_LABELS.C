#include "TYPES.H"

void UiText_DrawMessageAt(s32, s32, s32, s32);
extern u8 MsgStatusAdvice[][2];
extern u8 MsgStatusNormal[];

void CharacterMenu_DrawSelectionLabels(s32 target, s32 selected,
    const u8 *entries)
{
    s32 count = 0;
    s32 index = 0;

    do {
        if (entries[index] != 0) {
            if (selected == count) {
                s32 message = (s32)MsgStatusAdvice[index];
                UiText_DrawMessageAt(message, target, 0, -1);
                UiText_DrawMessageAt(message + 1, target, 0, 15);
            }
            count++;
        }
        index++;
    } while (index <= 4);

    if (count == 0)
        UiText_DrawMessageAt((s32)MsgStatusNormal, target, 0, 0);
}
