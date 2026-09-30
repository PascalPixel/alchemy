/*
 * Draft: StatusMenu_DrawCharacterSummary does not yet match; 2 halfwords differ from ☀️'s C, first at +0x9a (data).
 * Links as recon/tla/raw/08043c30.s.
 */
#include "TYPES.H"

void RenderOutput_PrepareForRedraw(void);
void UiWindow_DrawDividerLine(s32, s32, s32, s32, s32);
void UiText_DrawStringAtOffset(void *, s32, s32, s32);
void UiText_DrawStringInWindow(s32, s32, s32, s32);
void UiText_DrawNumberAtOffset(s32, s32, s32, s32, s32);
void UiText_DrawCharacterAtOffset(s32, s32, s32, s32);
s32 Text_FormatPlayTime(s32, void *);
void UiText_DrawNumberInWindow(s32, s32, s32, s32, s32);
extern u8 MsgStatusLabel;
extern u8 MsgClassName;
extern u8 StatusMenu_LevelLetterString[];
extern u8 MsgCoins[];

/* Draws one character entry into the summary surface: its name, the two-digit
 * value at +0x1c, the class message selected by +0x1d, the formatted value at
 * +0x20, the six-digit value at +0x24 and the closing label. */
void StatusMenu_DrawCharacterSummary(s32 surface, u8 *st)
{
    u32 buf[4];
    s32 extra;

    if (surface != 0) {
        RenderOutput_PrepareForRedraw();
        UiWindow_DrawDividerLine(surface, 0, 4, 13, 4);
        UiText_DrawStringAtOffset(st + 16, surface, 0, 0);
        extra = 0;
        UiText_DrawStringInWindow((s32)StatusMenu_LevelLetterString, surface, 72, 0);
        UiText_DrawNumberAtOffset(st[28], 2, surface, 80, extra);
        UiText_DrawCharacterAtOffset(st[29] + (s32)&MsgClassName, surface, 0, 16);
        UiText_DrawCharacterAtOffset((s32)&MsgStatusLabel, surface, 0, 32);
        UiText_DrawStringInWindow(Text_FormatPlayTime(*(s32 *)(st + 32), buf), surface, 48, 40);
        extra = 48;
        UiText_DrawNumberInWindow(*(s32 *)(st + 36), 6, surface, 0, extra);
        UiText_DrawCharacterAtOffset((s32)MsgCoins, surface, 48, 48);
    }
}
