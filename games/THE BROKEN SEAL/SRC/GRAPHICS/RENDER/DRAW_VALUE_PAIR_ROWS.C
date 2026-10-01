#include "TYPES.H"
#if defined(TBS_EDITION_JA)
/* The Japanese rows are labelled "HP    /" and "EP    /", slash included. */
extern const u8 Ui_HpRowString[];
extern const u8 Ui_EpRowString[];
#else
extern const u8 Ui_PpString[];
extern const u8 Ui_SlashString[];
extern const u8 Ui_HpString[];
#endif

void UiText_DrawStringAtOffsetFar(s32 image, s32 layer, s32 x, s32 y);
void UiText_DrawStringInWindowFar(s32 image, s32 layer, s32 x, s32 y);
void UiText_DrawNumberRightAlignedFar(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);
s32 UiWork_SetParamNibbleFar(s32 color);

void Ui_DrawValuePairRows(void *obj, s32 layer)
{
    s16 val;

#if defined(TBS_EDITION_JA)
    /* The Japanese rows draw four-digit fields: the current value before
       the slash, the maximum after it. */
    UiText_DrawStringInWindowFar((s32)Ui_HpRowString, layer, 0, 40);
    val = *(s16 *)((u8 *)obj + 52);
    UiText_DrawNumberInWindowFar(val, 4, layer, 56, 40);
    val = *(s16 *)((u8 *)obj + 56);
    if (val < ((s32)(u16)*(s16 *)((u8 *)obj + 52) << 16) >> 18) {
        UiWork_SetParamNibbleFar(4);
    }
    if (val == 0) {
        UiWork_SetParamNibbleFar(2);
    }
    UiText_DrawNumberAtOffsetFar(val, 4, layer, 16, 40);
    UiWork_SetParamNibbleFar(15);
    UiText_DrawStringInWindowFar((s32)Ui_EpRowString, layer, 0, 48);
    val = *(s16 *)((u8 *)obj + 58);
    UiText_DrawNumberAtOffsetFar(val, 4, layer, 16, 48);
    val = *(s16 *)((u8 *)obj + 54);
    UiText_DrawNumberInWindowFar(val, 4, layer, 56, 48);
#else
    UiText_DrawStringAtOffsetFar((s32)Ui_HpString, layer, 0, 40);
    UiText_DrawStringInWindowFar((s32)Ui_SlashString, layer, 48, 40);
    val = *(s16 *)((u8 *)obj + 52);
    UiText_DrawNumberRightAlignedFar(val, layer, 88, 40);
    val = *(s16 *)((u8 *)obj + 56);
    if (val < ((s32)(u16)*(s16 *)((u8 *)obj + 52) << 16) >> 18) {
        UiWork_SetParamNibbleFar(4);
    }
    if (val == 0) {
        UiWork_SetParamNibbleFar(2);
    }
    UiText_DrawNumberRightAlignedFar(val, layer, 48, 40);
    UiWork_SetParamNibbleFar(15);
    UiText_DrawStringAtOffsetFar((s32)Ui_PpString, layer, 0, 48);
    UiText_DrawStringInWindowFar((s32)Ui_SlashString, layer, 48, 48);
    val = *(s16 *)((u8 *)obj + 58);
    UiText_DrawNumberRightAlignedFar(val, layer, 48, 48);
    val = *(s16 *)((u8 *)obj + 54);
    UiText_DrawNumberRightAlignedFar(val, layer, 88, 48);
#endif
}
