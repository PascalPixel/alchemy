#include "types.h"
#include "scene.h"
#include "abi/menu/selection/setup_both_sides.h"
#include "global_cells.h"

void Menu_SetupSelectionBothSides(void)
{
    s32 state;

    state = *(s32 *)ADDR_03001E98;
    Menu_Apply(state, 0);
    Menu_Apply(state, 1);
}
