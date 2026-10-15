#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 Field_CachedTilesA[];
extern u8 Field_CachedTilesB[];
extern u8 Field_CachedTilesC[];
extern s16 Data_02005a80;
extern s16 Data_02005a82;
extern struct FieldActor *Data_02005a84[];
extern s16 Data_02005b84;
extern s16 Data_02005b86;

void *MemAlloc(s32 size);
void Func_0200468c(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_0200377c(void);

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

void Func_02003938(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_02005a84, 64);
    Func_0200468c(Field_CachedTilesA, buffer);
    Data_02005a80 = Resource_FindFreeEntry();
    VramLoad(Data_02005a80, 0xc00, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_0200377c, 1152);
    Data_02005a82 = 0;
    Data_02005b84 = 0;
    Data_02005b86 = -1;
}

void Func_020039b8(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_02005a84, 64);
    Func_0200468c(Field_CachedTilesB, buffer);
    Data_02005a80 = Resource_FindFreeEntry();
    VramLoad(Data_02005a80, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_0200377c, 1152);
    Data_02005a82 = 0;
    Data_02005b84 = 0;
    Data_02005b86 = -1;
}

void Func_02003a38(void)
{
    u8 *buffer = BumpAlloc(0x800);

    Iwram_ClearWords(Data_02005a84, 64);
    Func_0200468c(Field_CachedTilesC, buffer);
    Data_02005a80 = Resource_FindFreeEntry();
    VramLoad(Data_02005a80, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_0200377c, 1152);
    Data_02005a82 = 0;
    Data_02005b84 = 1;
    Data_02005b86 = -1;
}
