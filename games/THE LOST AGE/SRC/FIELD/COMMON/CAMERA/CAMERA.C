#include "PROTO.H"
#include "IO_REG.H"
#include "TYPES.H"


s32 Runtime_BlankDisplayAndRunScene(void)
{
    REG_DISPCNT = 0x40;
    Func_081b83c4();
    return 0;
}
