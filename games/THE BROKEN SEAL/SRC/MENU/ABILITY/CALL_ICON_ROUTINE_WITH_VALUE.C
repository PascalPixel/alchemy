#include "TYPES.H"

/* menu/psynergy_menu/call_icon_routine_with_value.c */
void Ui_LoadCharacterEntryForSlotFar(s32 a, s32 b, s32 c);

void PsynergyMenu_CallIconRoutineWithValue(s32 arg0, s32 arg1)
{
    Ui_LoadCharacterEntryForSlotFar(0, arg1, 0);
}
