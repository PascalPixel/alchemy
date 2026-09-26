/* Draft: main:080a6614, complete 384-byte owner.
 * Candidate 372 bytes; 74 aligned halfword edits remain.
 * Control flow agrees; constant sharing adds a stack slot and changes the
 * two measurement calls and repeated global reads. */
#include "PSYNERGY_MENU.H"

extern char Value_00000ae0;
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
s32 Func_08015108(s32 message, s32 *left, s32 *top, s32 *width, s32 *height);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiText_DrawStringAtOffsetFar(void *text, s32 window, s32 x, s32 y);
void *Owner_GetStateFar(s32 owner);
void UiWork_SetParamNibbleFar(s32 value);

static __inline__ void MeasureShortcut(u16 shortcut, s32 *left, s32 *top, s32 *width, s32 *height)
{
    Func_08015108((shortcut & 0x3ff) + 0x333, left, top, width, height);
}

static __inline__ void PushShortcut(u16 shortcut)
{
    UiWork_PushValueSlotFar(shortcut & 0x3ff, 4);
}

/* Show the L and R shortcuts and, when the Psynergy name fits, its owner. */
s32 Func_080a6614(s32 window)
{
    s32 height;
    s32 width;
    s32 top;
    s32 left;
    s32 wide;

    if (Data_02000240.psynergy_shortcuts[0] != 0 &&
        Data_02000240.psynergy_shortcuts[1] != 0)
        UiText_DrawCharacterAtOffsetFar(0xae4, window, 0, -8);
    else
        UiText_DrawCharacterAtOffsetFar((s32)&Value_00000ae0, window, 0, -8);
    MeasureShortcut(Data_02000240.psynergy_shortcuts[0], &left, &top, &width, &height);
    wide = 1;
    if ((u32)width <= 10)
        wide = 0;
    if (Data_02000240.psynergy_shortcuts[0] != 0) {
        PushShortcut(Data_02000240.psynergy_shortcuts[0]);
        UiText_DrawCharacterAtOffsetFar(0xae7, window, 0, 0);
        if (wide == 0)
            UiText_DrawStringAtOffsetFar(Owner_GetStateFar(Data_02000240.psynergy_shortcuts[0] >> 10),
                                         window, 80, 0);
    } else {
        UiText_DrawCharacterAtOffsetFar(0xae5, window, 0, 0);
    }
    MeasureShortcut(Data_02000240.psynergy_shortcuts[1], &left, &top, &width, &height);
    wide = 1;
    if ((u32)width <= 10)
        wide = 0;
    if (Data_02000240.psynergy_shortcuts[1] != 0) {
        PushShortcut(Data_02000240.psynergy_shortcuts[1]);
        UiText_DrawCharacterAtOffsetFar(0xae8, window, 0, 8);
        if (wide == 0)
            UiText_DrawStringAtOffsetFar(Owner_GetStateFar(Data_02000240.psynergy_shortcuts[1] >> 10),
                                         window, 80, 8);
        UiWork_SetParamNibbleFar(15);
    } else {
        UiText_DrawCharacterAtOffsetFar(0xae6, window, 0, 8);
    }
    return 1;
}
