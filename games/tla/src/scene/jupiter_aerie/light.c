#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8                 gField_CachedTilesA[];
extern u8                 gField_CachedTilesB[];
extern u8                 gField_CachedTilesC[];
extern s16                gData_02003380;
extern s16                gData_02003382;
extern struct FieldActor *gData_02003384[];
extern s16                gData_02003484;
extern s16                gData_02003486;

void *MemAlloc(s32 size);
void  Func_020029a0(const void *source, void *destination);
s32   Resource_FindFreeEntry(void);
void  VramBlock_LoadCached(s32 entry, s32 size, void *source);
void  MemFree(void *block);
void  Func_02001a78(void);

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

void Func_02001c34(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(gData_02003384, 64);
    Func_020029a0(gField_CachedTilesA, buffer);
    gData_02003380 = Resource_FindFreeEntry();
    VramLoad(gData_02003380, 0xc00, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02001a78, 1152);
    gData_02003382 = 0;
    gData_02003484 = 0;
    gData_02003486 = -1;
}

void Func_02001cb4(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(gData_02003384, 64);
    Func_020029a0(gField_CachedTilesB, buffer);
    gData_02003380 = Resource_FindFreeEntry();
    VramLoad(gData_02003380, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02001a78, 1152);
    gData_02003382 = 0;
    gData_02003484 = 0;
    gData_02003486 = -1;
}

void Func_02001d34(void)
{
    u8 *buffer = BumpAlloc(0x800);

    Iwram_ClearWords(gData_02003384, 64);
    Func_020029a0(gField_CachedTilesC, buffer);
    gData_02003380 = Resource_FindFreeEntry();
    VramLoad(gData_02003380, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02001a78, 1152);
    gData_02003382 = 0;
    gData_02003484 = 1;
    gData_02003486 = -1;
}
