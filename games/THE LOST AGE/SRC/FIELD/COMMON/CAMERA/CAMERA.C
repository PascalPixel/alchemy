#include "IO_REG.H"
#include "TYPES.H"

void Func_081b83c4(void);

s32 Runtime_BlankDisplayAndRunScene(void)
{
    REG_DISPCNT = 0x40;
    Func_081b83c4();
    return 0;
}
