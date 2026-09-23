/* Draft, not exact: 66 differing halfwords, 184-byte candidate for the
   192-byte owner (2026-09-23). Residual: the second DMA fill reuses r0 for
   &fill where the reference recomputes it from r4, and 0x03001cc0 is derived
   from 0x03001d1c by subtraction instead of its own pool entry; the tail
   stores state[6] and state[7] with different registers. */

#include "DMA.H"

u8 *Runtime_AllocateBlock(s32 slot, u32 size);
void Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
void ObjectSystem_Configure(s32 mode);
void Object_UpdateAllMotion(void);
void Object_UpdateAllThumb(void);
void ObjectSystem_UpdateCameraFixed(void);
void ObjectSystem_UpdateCamera(void);

void ObjectSystem_Initialize(s32 mode)
{
    u8 *state;
    u8 *objects;
    volatile u32 fill;

    state = Runtime_AllocateBlock(6, 92);
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
        *(s32 *)0x03001d1c = 0;
        *(s32 *)0x03001cc0 = 0;
    }
    state[6] = 15;
    state[7] = 0;
}
