#include "TYPES.H"

s32 WaitFrames(s32);
s32 UiGlyph_ResetWorkState();
void Resource_ClearOwnerListAndCounters(void);

void Ui_ClearWorkStateAndWaitFrame(void)
{
    UiGlyph_ResetWorkState();
    Resource_ClearOwnerListAndCounters();
    WaitFrames(1);
}
