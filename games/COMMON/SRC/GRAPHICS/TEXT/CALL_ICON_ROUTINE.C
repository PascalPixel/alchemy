#include "TYPES.H"

void Ui_LoadCharacterEntryForSlotFar(s32 a, s32 b, s32 c);

/* Loads the character entry of slot ARG1 for the Psynergy menu icon. */
void PsynergyMenu_CallIconRoutineWithValue(s32 arg0, s32 arg1)
{
    Ui_LoadCharacterEntryForSlotFar(0, arg1, 0);
}
