#include "IO_REG.H"
#include "TYPES.H"

void Func_081b83c4(void);

/* The far block's entry, as Runtime_BlankDisplayAndRun is its own block's:
   blanks the display to mode 0 with one-dimensional objects, then runs the
   block's scene. */
s32 Runtime_BlankDisplayAndRunScene(void)
{
    REG_DISPCNT = 0x40;
    Func_081b83c4();
    return 0;
}
