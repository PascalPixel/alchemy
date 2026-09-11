#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* runtime/dispatch/reserved_no_op_f.c */
void RuntimeDispatch_ReservedNoOp03008(void)
{
}

/* runtime/set_main_state_19.c */
void Runtime_SetMainState19(void)
{
    *(s32 *)ADDR_03001B00 = 0x13;
}
