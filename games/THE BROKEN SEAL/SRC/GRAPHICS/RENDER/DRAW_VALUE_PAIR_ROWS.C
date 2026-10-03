#include "EDITION.H"
#include "TYPES.H"
#include "BATTLE_UNIT.H"
#if EDITION_INTERNATIONAL
extern const u8 Ui_PpString[];
extern const u8 Ui_SlashString[];
extern const u8 Ui_HpString[];
#else
/* The Japanese rows are labelled "HP    /" and "EP    /", slash included. */
extern const u8 Ui_HpRowString[];
extern const u8 Ui_EpRowString[];
#endif

void UiText_DrawStringAtOffsetFar(s32 image, s32 layer, s32 x, s32 y);
void UiText_DrawStringInWindowFar(s32 image, s32 layer, s32 x, s32 y);
void UiText_DrawNumberRightAlignedFar(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);
s32 UiWork_SetParamNibbleFar(s32 color);

void Ui_DrawValuePairRows(struct BattleUnit *unit, s32 layer)
{
    s16 val;

#if EDITION_INTERNATIONAL
    UiText_DrawStringAtOffsetFar((s32)Ui_HpString, layer, 0, 40);
    UiText_DrawStringInWindowFar((s32)Ui_SlashString, layer, 48, 40);
    val = unit->max_hp;
    UiText_DrawNumberRightAlignedFar(val, layer, 88, 40);
    val = unit->hp;
    if (val < ((s32)(u16)unit->max_hp << 16) >> 18) {
        UiWork_SetParamNibbleFar(4);
    }
    if (val == 0) {
        UiWork_SetParamNibbleFar(2);
    }
    UiText_DrawNumberRightAlignedFar(val, layer, 48, 40);
    UiWork_SetParamNibbleFar(15);
    UiText_DrawStringAtOffsetFar((s32)Ui_PpString, layer, 0, 48);
    UiText_DrawStringInWindowFar((s32)Ui_SlashString, layer, 48, 48);
    val = unit->pp;
    UiText_DrawNumberRightAlignedFar(val, layer, 48, 48);
    val = unit->max_pp;
    UiText_DrawNumberRightAlignedFar(val, layer, 88, 48);
#else
    /* The Japanese rows draw four-digit fields: the current value before
       the slash, the maximum after it. */
    UiText_DrawStringInWindowFar((s32)Ui_HpRowString, layer, 0, 40);
    val = unit->max_hp;
    UiText_DrawNumberInWindowFar(val, 4, layer, 56, 40);
    val = unit->hp;
    if (val < ((s32)(u16)unit->max_hp << 16) >> 18) {
        UiWork_SetParamNibbleFar(4);
    }
    if (val == 0) {
        UiWork_SetParamNibbleFar(2);
    }
    UiText_DrawNumberAtOffsetFar(val, 4, layer, 16, 40);
    UiWork_SetParamNibbleFar(15);
    UiText_DrawStringInWindowFar((s32)Ui_EpRowString, layer, 0, 48);
    val = unit->pp;
    UiText_DrawNumberAtOffsetFar(val, 4, layer, 16, 48);
    val = unit->max_pp;
    UiText_DrawNumberInWindowFar(val, 4, layer, 56, 48);
#endif
}
