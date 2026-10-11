#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 Data_02005fbc[];
extern u8 Data_0200611e[];
extern u8 Data_0200634e[];
extern s16 Data_020074cc;
extern s16 Data_020074ce;
extern struct FieldActor *Data_020074d0[];
extern s16 Data_020075d0;
extern s16 Data_020075d2;

void *MemAlloc(s32 size);
void Func_02005420(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02005048(void);

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

void Func_02005204(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_020074d0, 64);
    Func_02005420(Data_02005fbc, buffer);
    Data_020074cc = Resource_FindFreeEntry();
    VramLoad(Data_020074cc, 0xc00, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02005048, 1152);
    Data_020074ce = 0;
    Data_020075d0 = 0;
    Data_020075d2 = -1;
}

void Func_02005284(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_020074d0, 64);
    Func_02005420(Data_0200611e + 1, buffer);
    Data_020074cc = Resource_FindFreeEntry();
    VramLoad(Data_020074cc, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02005048, 1152);
    Data_020074ce = 0;
    Data_020075d0 = 0;
    Data_020075d2 = -1;
}

void Func_02005304(void)
{
    u8 *buffer = BumpAlloc(0x800);

    Iwram_ClearWords(Data_020074d0, 64);
    Func_02005420(Data_0200634e, buffer);
    Data_020074cc = Resource_FindFreeEntry();
    VramLoad(Data_020074cc, 0x800, buffer);
    MemFree(buffer);
    Scheduler_AddOrUpdateCallback(Func_02005048, 1152);
    Data_020074ce = 0;
    Data_020075d0 = 1;
    Data_020075d2 = -1;
}
