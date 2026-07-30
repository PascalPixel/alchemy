#include "types.h"

struct RenderObject {
    u8 data[0x70];
};

struct ObjectCacheEntry {
    u32 key;
    void *data;
};

struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
};

struct RenderManager {
    u16 field_00;
    u16 field_02;
    u16 renderPaused;
    u8 maskEdgeValue;
    u8 maskInteriorValue;
    void *multiObjects[4];
    u32 multiObjectCount;
    struct ObjectCacheEntry cache[8];
};

typedef void (*TaskCallback)(void);

void *Func_080048f4(u32 slot, u32 size);
s32 Func_080041d8(TaskCallback callback, u16 flags);
void Func_0800bb20(s32 mode);

void Func_0800c62c(void);
void Func_0800c880(void);
void Func_0800cacc(void);
void Func_0800d340(void);

extern u32 Data_03001cc0;
extern u32 Data_03001d1c;

void Func_0800c004(s32 mode)
{
    struct RenderManager *manager;
    struct RenderObject *objectPool;
    u32 zero;
    struct DmaTransfer *dma;
    const void *source;
    void *destination;
    u32 control;

    manager = Func_080048f4(6, sizeof(*manager));
    objectPool = Func_080048f4(5, 64 * sizeof(*objectPool));
    Func_0800bb20(mode);

    zero = 0;
    dma = (struct DmaTransfer *)0x040000d4;
    source = &zero;
    destination = objectPool;
    control = 0x85000700;
    dma->source = source;
    dma->destination = destination;
    dma->control = control;

    zero = 0;
    source = &zero;
    destination = manager;
    control = 0x85000017;
    dma->source = source;
    dma->destination = destination;
    dma->control = control;

    if (mode == 4)
        Func_080041d8(Func_0800d340, 0xc8a);
    else
        Func_080041d8(Func_0800cacc, 0xc8a);

    if ((u32)(mode - 3) <= 1)
        Func_080041d8(Func_0800c880, 0xc80);
    else {
        Func_080041d8(Func_0800c62c, 0xc80);
        Data_03001d1c = 0;
        Data_03001cc0 = 0;
    }

    manager->maskEdgeValue = 15;
    manager->maskInteriorValue = 0;
}

void Func_0800c0c4(void)
{
}
