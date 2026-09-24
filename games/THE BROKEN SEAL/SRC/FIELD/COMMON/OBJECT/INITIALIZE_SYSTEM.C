#include "DMA.H"

/* The object system state block (92 bytes). */
struct ObjectSystem {
    u8 unk_00[6];
    u8 priority;
    u8 flag;
    u8 unk_08[84];
};

u8 *Runtime_AllocateBlock(s32 slot, u32 size);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
void ObjectSystem_Configure(s32 mode);
void Object_UpdateAllMotion(void);
void Object_UpdateAllThumb(void);
void ObjectSystem_UpdateCameraFixed(void);
void ObjectSystem_UpdateCamera(void);

extern s32 Data_03001d1c;
extern s32 Data_03001cc0;

/* Allocates and clears the object system state and the object table,
   configures the system for the mode and schedules the object update (the
   motion-only one in mode 4) and the camera (fixed in modes 3 and 4). */
void ObjectSystem_Initialize(s32 mode)
{
    struct ObjectSystem *state;
    u8 *objects;
    volatile u32 fill;

    state = (struct ObjectSystem *)Runtime_AllocateBlock(6, 92);
    objects = Runtime_AllocateBlock(5, 0x1c00);
    ObjectSystem_Configure(mode);
    fill = 0;
    Dma_Set((const void *)&fill, objects, 0x85000700, (volatile u32 *)0x040000d4);
    fill = 0;
    Dma_Set((const void *)&fill, state, 0x85000017, (volatile u32 *)0x040000d4);
    if (mode == 4)
        Scheduler_AddOrUpdateCallback(Object_UpdateAllMotion, 0xc8a);
    else
        Scheduler_AddOrUpdateCallback(Object_UpdateAllThumb, 0xc8a);
    if ((u32)(mode - 3) <= 1) {
        Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCameraFixed, 0xc80);
    } else {
        Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCamera, 0xc80);
        Data_03001d1c = 0;
        Data_03001cc0 = 0;
    }
    state->priority = 15;
    state->flag = 0;
}
