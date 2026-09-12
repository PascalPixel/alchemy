#include "types.h"

/* menu/psynergy_menu/call_icon_routine_with_value.c */
void Sys_Place(s32 a, s32 b, s32 c);

void PsynergyMenu_CallIconRoutineWithValue(s32 arg0, s32 arg1)
{
    Sys_Place(0, arg1, 0);
}
