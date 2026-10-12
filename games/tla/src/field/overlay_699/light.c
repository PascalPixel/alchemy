#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 Field_CachedTilesA[];
extern u8 Field_CachedTilesB[];
extern u8 Field_CachedTilesC[];
extern s16 Data_020023f0;
extern s16 Data_020023f2;
extern struct FieldActor *Data_020023f4[];
extern s16 Data_020024f4;
extern s16 Data_020024f6;

void *MemAlloc(s32 size);
void Func_02001bac(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02000c94(void);

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

void Func_02000e50(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_020023f4, 64);
    Func_02001bac(Field_CachedTilesA, buffer);
    Data_020023f0 = Resource_FindFreeEntry();
    VramLoad(Data_020023f0, 0xc00, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000c94, 1152);
    Data_020023f2 = 0;
    Data_020024f4 = 0;
    Data_020024f6 = -1;
}

void Func_02000ed0(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_020023f4, 64);
    Func_02001bac(Field_CachedTilesB, buffer);
    Data_020023f0 = Resource_FindFreeEntry();
    VramLoad(Data_020023f0, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000c94, 1152);
    Data_020023f2 = 0;
    Data_020024f4 = 0;
    Data_020024f6 = -1;
}

void Func_02000f50(void)
{
    u8 *buffer = BumpAlloc(0x800);

    Iwram_ClearWords(Data_020023f4, 64);
    Func_02001bac(Field_CachedTilesC, buffer);
    Data_020023f0 = Resource_FindFreeEntry();
    VramLoad(Data_020023f0, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000c94, 1152);
    Data_020023f2 = 0;
    Data_020024f4 = 1;
    Data_020024f6 = -1;
}
