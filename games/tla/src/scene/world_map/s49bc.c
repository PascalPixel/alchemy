#include "types.h"
#include "resource.h"

extern const u8 gWorldMapPackedTilesC[];
extern s16 gData_02006e0c;
extern s16 gData_02006e0e;
extern s16 gData_02006e10;
extern u8 gData_02006f20[];

s32 Resource_FindFreeEntry(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Func_020048f4(void);
void Func_02004aa8(const u8 *a, u8 *b);

void Func_020049bc(void)
{
    Func_02004aa8(gWorldMapPackedTilesC, gData_02006f20);
    gData_02006e0e = Resource_FindFreeEntry();
    VramBlock_LoadCached(gData_02006e0e, 0x100, 0);
    gData_02006e0c = -1;
    gData_02006e10 = 10;
    Scheduler_AddOrUpdateCallback(Func_020048f4, 0x476);
}
