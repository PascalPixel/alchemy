#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 Field_CachedTilesA[];
extern u8 Field_CachedTilesB[];
extern u8 Field_CachedTilesC[];
extern s16 Data_02002428;
extern s16 Data_0200242a;
extern struct FieldActor *Data_0200242c[];
extern s16 Data_0200252c;
extern s16 Data_0200252e;

void *MemAlloc(s32 size);
void Func_02001a6c(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02000b5c(void);

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

void Func_02000d18(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_0200242c, 64);
    Func_02001a6c(Field_CachedTilesA, buffer);
    Data_02002428 = Resource_FindFreeEntry();
    VramLoad(Data_02002428, 0xc00, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000b5c, 1152);
    Data_0200242a = 0;
    Data_0200252c = 0;
    Data_0200252e = -1;
}

void Func_02000d98(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_0200242c, 64);
    Func_02001a6c(Field_CachedTilesB, buffer);
    Data_02002428 = Resource_FindFreeEntry();
    VramLoad(Data_02002428, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000b5c, 1152);
    Data_0200242a = 0;
    Data_0200252c = 0;
    Data_0200252e = -1;
}

void Func_02000e18(void)
{
    u8 *buffer = BumpAlloc(0x800);

    Iwram_ClearWords(Data_0200242c, 64);
    Func_02001a6c(Field_CachedTilesC, buffer);
    Data_02002428 = Resource_FindFreeEntry();
    VramLoad(Data_02002428, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000b5c, 1152);
    Data_0200242a = 0;
    Data_0200252c = 1;
    Data_0200252e = -1;
}
