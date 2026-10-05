#include "TYPES.H"
#include "RESOURCE.H"

struct WorldMapWork {
    u16 resources[2];
    u8 unknown_04[0x18];
    u32 window;
};

extern struct WorldMapWork gWorldMapWork;

extern void UiWork_FinalizeFar(u32 arg0, u32 arg1);

void BattleFx_CleanupResourcesAndWindow(void)
{
    Resource_ResetEntry(gWorldMapWork.resources[0]);
    Resource_ResetEntry(gWorldMapWork.resources[1]);
    UiWork_FinalizeFar(gWorldMapWork.window, 2);
}
