#include "TYPES.H"
#include "SYSTEM.H"

/* ☀️'s UiGlyph_ResetWorkState and Resource_ClearOwnerListAndCounters
   keep their listings' names in ⚓️. */
s32 Func_0803dc1c();
void Func_0803dd24(void);

/* The head of ☀️'s WINDOW6.C. */
void Ui_ClearWorkStateAndWaitFrame(void)
{
    Func_0803dc1c();
    Func_0803dd24();
    WaitFrames(1);
}
