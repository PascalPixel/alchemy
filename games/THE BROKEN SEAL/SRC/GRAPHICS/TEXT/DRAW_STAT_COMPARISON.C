#include "TYPES.H"

/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 MsgAgilityLabel[];
extern u8 MsgPanelStatLabel[];
extern u8 MsgPanelDefenseLabel[];
void UiText_DrawCharacterAtOffsetFar();
void UiText_DrawNumberAtOffsetFar();
void UiIcon_CreateStatChangeArrow();

void UiText_DrawStatComparison(s32 alt, s32 base, s32 work)
{
    u32 i;
    s32 tmp2;
    s32 p;
    s32 tmp;
    s32 rec;

    p = alt;
    UiText_DrawCharacterAtOffsetFar((s32)MsgPanelStatLabel, work, 0, 32);
    UiText_DrawNumberAtOffsetFar(*(u16 *)(base + 60), 3, work, 16, 40);
    if (*(u16 *)(p + 60) != *(u16 *)(base + 60)) {
        UiText_DrawNumberAtOffsetFar(*(u16 *)(p + 60), 3, work, 64, 40);
        if (*(u16 *)(p + 60) > *(u16 *)(base + 60)) {
            UiIcon_CreateStatChangeArrow(work, 44, 36, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 36, 1);
        }
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgPanelDefenseLabel, work, 0, 48);
    UiText_DrawNumberAtOffsetFar(*(u16 *)(base + 62), 3, work, 16, 56);
    if (*(u16 *)(p + 62) != *(u16 *)(base + 62)) {
        UiText_DrawNumberAtOffsetFar(*(u16 *)(p + 62), 3, work, 64, 56);
        if (*(u16 *)(p + 62) > *(u16 *)(base + 62)) {
            UiIcon_CreateStatChangeArrow(work, 44, 52, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 52, 1);
        }
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgAgilityLabel, work, 0, 64);
    UiText_DrawNumberAtOffsetFar(*(u16 *)(base + 64), 3, work, 16, 72);
    if (*(u16 *)(p + 64) != *(u16 *)(base + 64)) {
        UiText_DrawNumberAtOffsetFar(*(u16 *)(p + 64), 3, work, 64, 72);
        if (*(u16 *)(p + 64) > *(u16 *)(base + 64)) {
            UiIcon_CreateStatChangeArrow(work, 44, 68, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 68, 1);
        }
    }
    tmp = *(u16 *)(base + 64);
    tmp2 = p + 64;
}
