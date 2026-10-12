#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 Field_CachedTilesA[];
extern u8 Field_CachedTilesB[];
extern u8 Field_CachedTilesC[];
extern s16 Data_02002d2c;
extern s16 Data_02002d2e;
extern struct FieldActor *Data_02002d30[];
extern s16 Data_02002e30;
extern s16 Data_02002e32;

void *MemAlloc(s32 size);
void Func_020022a0(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02001388(void);

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

void Func_02001544(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_02002d30, 64);
    Func_020022a0(Field_CachedTilesA, buffer);
    Data_02002d2c = Resource_FindFreeEntry();
    VramLoad(Data_02002d2c, 0xc00, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02001388, 1152);
    Data_02002d2e = 0;
    Data_02002e30 = 0;
    Data_02002e32 = -1;
}

void Func_020015c4(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_02002d30, 64);
    Func_020022a0(Field_CachedTilesB, buffer);
    Data_02002d2c = Resource_FindFreeEntry();
    VramLoad(Data_02002d2c, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02001388, 1152);
    Data_02002d2e = 0;
    Data_02002e30 = 0;
    Data_02002e32 = -1;
}

void Func_02001644(void)
{
    u8 *buffer = BumpAlloc(0x800);

    Iwram_ClearWords(Data_02002d30, 64);
    Func_020022a0(Field_CachedTilesC, buffer);
    Data_02002d2c = Resource_FindFreeEntry();
    VramLoad(Data_02002d2c, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02001388, 1152);
    Data_02002d2e = 0;
    Data_02002e30 = 1;
    Data_02002e32 = -1;
}
