/* Near miss of Func_020049bc of the world map (26 instructions, 2 differ): the native widens
 * the block number for VramBlock_LoadCached after it builds the size and the null source, the
 * draft before. The number through an s32 local loses the callee-saved address (10 differ). */
#include "types.h"

extern const u8 Data_02005920[];
extern s16 Data_02006e0c;
extern s16 Data_02006e0e;
extern s16 Data_02006e10;
extern u8 Data_02006f20[];

s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 block, s32 size, const u8 *source);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Func_020048f4(void);
void Func_02004aa8(const u8 *a, u8 *b);

void Func_020049bc(void)
{
    Func_02004aa8(Data_02005920, Data_02006f20);
    Data_02006e0e = Resource_FindFreeEntry();
    VramBlock_LoadCached(Data_02006e0e, 0x100, 0);
    Data_02006e0c = -1;
    Data_02006e10 = 10;
    Scheduler_AddOrUpdateCallback(Func_020048f4, 0x476);
}
