#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "RESOURCE.H"

struct WorldMapWork {
    u16 resources[2];
    u8 unknown_04[24];
    u32 window;
};


extern void UiWork_FinalizeFar(u32 arg0, u32 arg1);

void BattleFx_CleanupResourcesAndWindow(void)
{
    Resource_ResetEntry(gWorldMapWork.resources[0]);
    Resource_ResetEntry(gWorldMapWork.resources[1]);
    UiWork_FinalizeFar(gWorldMapWork.window, 2);
}
