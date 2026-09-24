/* Field: show the world map. The map layers are hidden and the map picture
   is decoded over BG1 until A or B is pressed, then the field comes back.

   FAKEMATCH: two one-pass loops act as scheduling barriers: one around the
   display-control value 64 so it is built before the register address, one
   around reloading the layer table so its spilled pointer loads first. The
   queued blend restore is QueueIoWriteDelay2 (SYSTEM/IO_WRITE_QUEUE.C)
   written out inline with that function's one-pass loop around the IME read
   and its count stored through an explicit u16 pointer. */
#include "TYPES.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"

struct MapLayerEntry {
    u8 padding00[10];
    u16 flag;
};

struct MapWork {
    u8 padding00[0x18];
    struct MapLayerEntry layers[16];
};

struct MenuControl {
    u8 padding00[4];
    u16 suspended;
};

struct FieldState {
    u8 padding000[0x19e];
    s16 mode;
    u8 padding1a0[0x28];
    s32 status;
    u8 padding1cc[0x14];
    u8 *actor;
};

extern u8 gWorkSlot[];
extern u8 Value_0000001b[];
extern u32 gKeysRepeat;
extern volatile u16 Data_04000208;
#define REG_IME Data_04000208

void *Runtime_AllocateBlock(s32 slot, s32 size);
void Event_ClearStatus1c6(void);
void Event_SetStatus1c6(void);
void Event_WaitValue1c8Frames(void);
void WaitFrames(s32 frames);
u8 *Resource_GetTableEntry(s32 index);
s32 Resource_DecodeType01(const void *source, void *destination);
void BattleFx_SetupResourcesAndWindow(void);
void BattleFx_CleanupResourcesAndWindow(void);
s32 GameFlag_TestFar(s32 flag);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void Map_LoadAreaGraphicsFar(void);

void Map_ShowWorldMap(void)
{
    struct MapWork *map = *(struct MapWork **)(gWorkSlot + 8 * 4);
    struct FieldState *field = Runtime_AllocateBlock(27, 0xccc);
    struct MenuControl *menu = *(struct MenuControl **)(gWorkSlot + 6 * 4);
    s32 resource = (s32)Value_0000001b;
    struct MapLayerEntry *layer;
    u8 saved_flags[16];
    s32 i;
    u8 *graphics;
    s32 saved_status;
    s16 blend;

    if (field->mode != 3)
        return;
    field->actor[91] = 1;
    saved_status = field->status;
    field->status = 6;
    Event_ClearStatus1c6();
    Event_WaitValue1c8Frames();
    layer = map->layers;
    for (i = 0; i < 16; i++) {
        saved_flags[i] = layer->flag;
        layer->flag = 1;
        layer++;
    }
    menu->suspended = 1;
    WaitFrames(1);
    blend = *(volatile u16 *)0x04000052;
    graphics = Resource_GetTableEntry(resource);
    Dma_Set(graphics, (void *)0x05000000, 0x84000070, (volatile u32 *)0x040000d4);
    *(u16 *)0x05000000 = 0;
    Resource_DecodeType01(graphics + 448, (void *)0x02010000);
    Dma_Set((void *)0x02010000, (void *)0x06006a00, 0x84002580, (volatile u32 *)0x040000d4);
    {
        s32 v = 0x682;
        *(volatile u16 *)0x0400000a = v;
        v = 0x1340;
        *(volatile u16 *)0x04000000 = v;
    }
    BattleFx_SetupResourcesAndWindow();
    Scheduler_AddOrUpdateCallback(0x0809bcf9, 0xc80);
    if (GameFlag_TestFar(284))
        UiText_ShowPositionedMessageAndWaitFar(0x985, 1);
    do {
        WaitFrames(1);
    } while ((gKeysRepeat & 3) == 0);
    Scheduler_RemoveCallback(0x0809bcf9);
    BattleFx_CleanupResourcesAndWindow();
    {
        s32 v;
        do {
            v = 64;
        } while (0);
        *(volatile u16 *)0x04000000 = v;
    }
    Map_LoadAreaGraphicsFar();
    {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;

        q = &gIoWriteQueue;
        do {
            ime = &REG_IME;
            saved = *ime;
        } while (0);
        *ime = (u16)ime;
        count = q->count;
        if (count <= 31) {
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
            *(u16 *)&q->count = count + 1;
            *destination++ = (u16)blend;
            *destination++ = 0x04000052;
            *destination = 0x20000;
        }
        *ime = saved;
    }
    do {
        layer = map->layers;
    } while (0);
    for (i = 0; i < 16; i++) {
        layer->flag = saved_flags[i];
        layer++;
    }
    menu->suspended = 0;
    Event_SetStatus1c6();
    Event_WaitValue1c8Frames();
    field->status = saved_status;
    field->actor[91] = 0;
}
