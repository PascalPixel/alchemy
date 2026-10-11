#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 Data_02002334[];
extern u8 Data_02002496[];
extern u8 Data_020026c6[];
extern s16 Data_02002bd8;
extern s16 Data_02002bda;
extern struct FieldActor *Data_02002bdc[];
extern s16 Data_02002cdc;
extern s16 Data_02002cde;

void *MemAlloc(s32 size);
void Func_02002174(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_0200125c(void);

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

void Func_02001418(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_02002bdc, 64);
    Func_02002174(Data_02002334, buffer);
    Data_02002bd8 = Resource_FindFreeEntry();
    VramLoad(Data_02002bd8, 0xc00, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_0200125c, 1152);
    Data_02002bda = 0;
    Data_02002cdc = 0;
    Data_02002cde = -1;
}

void Func_02001498(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_02002bdc, 64);
    Func_02002174(Data_02002496 + 1, buffer);
    Data_02002bd8 = Resource_FindFreeEntry();
    VramLoad(Data_02002bd8, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_0200125c, 1152);
    Data_02002bda = 0;
    Data_02002cdc = 0;
    Data_02002cde = -1;
}

void Func_02001518(void)
{
    u8 *buffer = BumpAlloc(0x800);

    Iwram_ClearWords(Data_02002bdc, 64);
    Func_02002174(Data_020026c6, buffer);
    Data_02002bd8 = Resource_FindFreeEntry();
    VramLoad(Data_02002bd8, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_0200125c, 1152);
    Data_02002bda = 0;
    Data_02002cdc = 1;
    Data_02002cde = -1;
}
