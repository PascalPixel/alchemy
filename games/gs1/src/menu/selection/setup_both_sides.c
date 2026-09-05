#include "types.h"
#include "global_cells.h"

s32 Func_0801b248(s32, s32);

void Menu_SetupSelectionBothSides(void) {
    s32 state;

    state = *(s32 *)ADDR_03001E98;
    Func_0801b248(state, 0);
    Func_0801b248(state, 1);
}
