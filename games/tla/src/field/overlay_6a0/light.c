#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 Data_02002c9c[];
extern u8 Data_02002dfe[];
extern u8 Data_0200302e[];
extern s16 Data_020035fc;
extern s16 Data_020035fe;
extern struct FieldActor *Data_02003600[];
extern s16 Data_02003700;
extern s16 Data_02003702;

void *MemAlloc(s32 size);
void Func_02002a8c(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02001b74(void);

static inline void *BumpAlloc(s32 size)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each size's constant */
    return MemAlloc(size);
}

static inline void VramLoad(s32 entry, s32 size, void *source)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each size's constant */
    VramBlock_LoadCached(entry, size, source);
}

void Func_02001d30(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_02003600, 64);
    Func_02002a8c(Data_02002c9c, buffer);
    Data_020035fc = Resource_FindFreeEntry();
    VramLoad(Data_020035fc, 0xc00, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02001b74, 1152);
    Data_020035fe = 0;
    Data_02003700 = 0;
    Data_02003702 = -1;
}

void Func_02001db0(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_02003600, 64);
    Func_02002a8c(Data_02002dfe + 1, buffer);
    Data_020035fc = Resource_FindFreeEntry();
    VramLoad(Data_020035fc, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02001b74, 1152);
    Data_020035fe = 0;
    Data_02003700 = 0;
    Data_02003702 = -1;
}

void Func_02001e30(void)
{
    u8 *buffer = BumpAlloc(0x800);

    Iwram_ClearWords(Data_02003600, 64);
    Func_02002a8c(Data_0200302e, buffer);
    Data_020035fc = Resource_FindFreeEntry();
    VramLoad(Data_020035fc, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02001b74, 1152);
    Data_020035fe = 0;
    Data_02003700 = 1;
    Data_02003702 = -1;
}
