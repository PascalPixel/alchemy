#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "FIELD_EVENT.H"
#include "CALLBACK_SCHEDULER.H"

extern u8 Data_02001f10[];
extern u8 Data_02002072[];
extern u8 Data_020022a2[];
extern s16 Data_0200275c;
extern s16 Data_0200275e;
extern struct FieldActor *Data_02002760[];
extern s16 Data_02002860;
extern s16 Data_02002862;

void *Runtime_BumpAllocate(s32 size);
void Func_02001d68(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void Sys_Free(void *block);
void Func_02000e58(void);
struct FieldActor *Object_GetById(s32 id);

static inline void VramLoad(s32 entry, s32 size, void *source)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each size's constant */
    VramBlock_LoadCached(entry, size, source);
}

static inline void *BumpAlloc(s32 size)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each size's constant */
    return Runtime_BumpAllocate(size);
}

void Func_02001014(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_02002760, 64);
    Func_02001d68(Data_02001f10, buffer);
    Data_0200275c = Resource_FindFreeEntry();
    VramLoad(Data_0200275c, 0xc00, buffer);
    Sys_Free(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000e58, 1152);
    Data_0200275e = 0;
    Data_02002860 = 0;
    Data_02002862 = -1;
}

void Func_02001094(void)
{
    u8 *buffer = BumpAlloc(0xc00);

    Iwram_ClearWords(Data_02002760, 64);
    Func_02001d68(Data_02002072 + 1, buffer);
    Data_0200275c = Resource_FindFreeEntry();
    VramLoad(Data_0200275c, 0x800, buffer);
    Sys_Free(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000e58, 1152);
    Data_0200275e = 0;
    Data_02002860 = 0;
    Data_02002862 = -1;
}

void Func_02001114(void)
{
    u8 *buffer = BumpAlloc(0x800);

    Iwram_ClearWords(Data_02002760, 64);
    Func_02001d68(Data_020022a2, buffer);
    Data_0200275c = Resource_FindFreeEntry();
    VramLoad(Data_0200275c, 0x800, buffer);
    Sys_Free(buffer);
    Scheduler_AddOrUpdateCallback(Func_02000e58, 1152);
    Data_0200275e = 0;
    Data_02002860 = 1;
    Data_02002862 = -1;
}

void Func_0200119c(s32 id, s32 value)
{
    struct FieldActor *object = Object_GetById(id);

    if (object != NULL) {
        object->timer = value;
        Data_02002760[Data_0200275e] = object;
        Data_0200275e++;
    }
}

void Func_020011cc(s32 value)
{
    Data_02002862 = value;
}
