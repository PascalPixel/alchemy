#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SYSTEM.H"

s32 Func_0803dc1c();

void Ui_ClearWorkStateAndWaitFrame(void)
{
    Func_0803dc1c();
    Resource_ClearOwnerListAndCounters();
    WaitFrames(1);
}
