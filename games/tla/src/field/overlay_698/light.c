#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 Field_CachedTilesA[];
extern u8 Field_CachedTilesB[];
extern u8 Field_CachedTilesC[];
extern s16 Data_02002200;
extern s16 Data_02002202;
extern struct FieldActor *Data_02002204[];
extern s16 Data_02002304;
extern s16 Data_02002306;

void *MemAlloc(s32 size);
void Func_020019bc(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02000aac(void);

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

void Func_02000c68(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_02002204, 64);
    Func_020019bc(Field_CachedTilesA, buffer);
    Data_02002200 = Resource_FindFreeEntry();
    VramLoad(Data_02002200, 0xc00, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000aac, 1152);
    Data_02002202 = 0;
    Data_02002304 = 0;
    Data_02002306 = -1;
}

void Func_02000ce8(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_02002204, 64);
    Func_020019bc(Field_CachedTilesB, buffer);
    Data_02002200 = Resource_FindFreeEntry();
    VramLoad(Data_02002200, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000aac, 1152);
    Data_02002202 = 0;
    Data_02002304 = 0;
    Data_02002306 = -1;
}

void Func_02000d68(void)
{
    u8 *buffer = BumpAlloc(0x800);

    Iwram_ClearWords(Data_02002204, 64);
    Func_020019bc(Field_CachedTilesC, buffer);
    Data_02002200 = Resource_FindFreeEntry();
    VramLoad(Data_02002200, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000aac, 1152);
    Data_02002202 = 0;
    Data_02002304 = 1;
    Data_02002306 = -1;
}
