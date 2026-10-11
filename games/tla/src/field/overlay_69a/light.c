#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 Data_02001bdc[];
extern u8 Data_02001d3e[];
extern u8 Data_02001f6e[];
extern s16 Data_02002298;
extern s16 Data_0200229a;
extern struct FieldActor *Data_0200229c[];
extern s16 Data_0200239c;
extern s16 Data_0200239e;

void *MemAlloc(s32 size);
void Func_02001a4c(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02000b3c(void);
struct FieldActor *Object_GetById(s32 id);

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

void Func_02000cf8(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_0200229c, 64);
    Func_02001a4c(Data_02001bdc, buffer);
    Data_02002298 = Resource_FindFreeEntry();
    VramLoad(Data_02002298, 0xc00, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000b3c, 1152);
    Data_0200229a = 0;
    Data_0200239c = 0;
    Data_0200239e = -1;
}

void Func_02000d78(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_0200229c, 64);
    Func_02001a4c(Data_02001d3e + 1, buffer);
    Data_02002298 = Resource_FindFreeEntry();
    VramLoad(Data_02002298, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000b3c, 1152);
    Data_0200229a = 0;
    Data_0200239c = 0;
    Data_0200239e = -1;
}

void Func_02000df8(void)
{
    u8 *buffer = BumpAlloc(0x800);

    Iwram_ClearWords(Data_0200229c, 64);
    Func_02001a4c(Data_02001f6e, buffer);
    Data_02002298 = Resource_FindFreeEntry();
    VramLoad(Data_02002298, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000b3c, 1152);
    Data_0200229a = 0;
    Data_0200239c = 1;
    Data_0200239e = -1;
}

