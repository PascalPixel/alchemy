#include "trial_road.h"

#include "iwram_call.h"

struct TrialRoadGauge
{
    u16   handle;
    u16   field02;
    u8    unk004[0x1c0];
    void *buffer;
};

extern const u8 gField_Tiles64D[];

void *Runtime_BumpAllocateAlternatePool(s32 size);
s32   Resource_FindFreeEntry(void);
void  VramBlock_LoadCached(s32 handle, s32 size, void *source);
void  MemFree(void *block);
void  Func_02003da0(void);
void  Func_02003f68(void);
void  Func_02005770(const void *source, void *destination);
s32   Func_02005a40(void);

static __inline__ void *Pool_Alloc(s32 size)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Runtime_BumpAllocateAlternatePool(size);
}

static __inline__ void Words_Clear(void *destination, s32 size)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Iwram_ClearWords(destination, size);
}

void Func_02004188(s32 a, s32 b)
{
    struct TrialRoadGauge *gauge = (struct TrialRoadGauge *)(gData_020023c4 + 0x188);
    void                  *buffer;

    if (!GameFlag_Test(0x109))
        Words_Clear(gauge, 0x1c8);
    buffer = Pool_Alloc(0x800);
    Func_02005770(gField_Tiles64D, buffer);
    gauge->handle = Resource_FindFreeEntry();
    VramBlock_LoadCached((s16)gauge->handle, 0x800, buffer);
    MemFree(buffer);
    gauge->buffer = Runtime_BumpAllocateAlternatePool(0x3c0);
    gauge->field02 = b;
    Scheduler_AddOrUpdateCallback(Func_02003da0, 0x48d);
    Scheduler_AddOrUpdateCallback(Func_02003f68, 0x480);
    Object_GetById(Func_02005a40())->priorityFlags |= 0x40;
}
