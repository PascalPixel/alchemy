#include "types.h"
#include "scene.h"
#include "abi/psynergy_menu/call_icon_routine_with_value.h"

void PsynergyMenu_CallIconRoutineWithValue(s32 arg0, s32 arg1)
{
    Sys_Place(0, arg1, 0);
}
