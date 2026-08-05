#include "layout_guard.h"
#include "types.h"

struct DmaTransfer_0809c138 {
    const void *source;
    void *destination;
    u32 control;
};

struct RegisterRestore_0809c138 {
    u32 value;
    volatile void *register_address;
    u32 mask;
};

struct PriorityRecord_0809c138 {
    u8 padding00[0x0a];
    u16 priority;
};

struct PriorityTable_0809c138 {
    u8 padding000[0x18];
    struct PriorityRecord_0809c138 records[16];
};

struct SceneDisplay_0809c138 {
    u8 padding00[0x5b];
    u8 interlude_active;
};

struct SceneState_0809c138 {
    u8 padding000[0x19e];
    s16 mode;
    u8 padding1a0[0x28];
    u32 display_mode;
    u8 padding1cc[0x14];
    struct SceneDisplay_0809c138 *display;
};

struct InterludeRuntime_0809c138 {
    u8 padding00[4];
    u16 active;
};

struct RegisterRestoreQueue_0809c138 {
    volatile u16 count;
    u16 padding02;
    struct RegisterRestore_0809c138 entries[32];
};

LAYOUT_OFFSET_GUARD(
    PriorityRecord0809c138_Priority,
    struct PriorityRecord_0809c138,
    priority,
    0x0a);
LAYOUT_SIZE_GUARD(
    PriorityRecord0809c138_Size,
    struct PriorityRecord_0809c138,
    0x0c);
LAYOUT_OFFSET_GUARD(
    PriorityTable0809c138_Records,
    struct PriorityTable_0809c138,
    records,
    0x18);
LAYOUT_OFFSET_GUARD(
    SceneState0809c138_Mode,
    struct SceneState_0809c138,
    mode,
    0x19e);
LAYOUT_OFFSET_GUARD(
    SceneState0809c138_DisplayMode,
    struct SceneState_0809c138,
    display_mode,
    0x1c8);
LAYOUT_OFFSET_GUARD(
    SceneState0809c138_Display,
    struct SceneState_0809c138,
    display,
    0x1e0);
LAYOUT_OFFSET_GUARD(
    RegisterRestoreQueue0809c138_Entries,
    struct RegisterRestoreQueue_0809c138,
    entries,
    4);

struct SceneState_0809c138 *Func_080048f4(s32, s32);
s32 Func_08002f40(s32);
void Func_08005340(const void *, void *);
void Func_08091df4(void);
void Func_08091dc8(void);
void Func_08091e20(void);
void Func_0809bb64(void);
void Func_0809bcd4(void);
void Func_080041d8(const void *, s32);
void Func_08004278(const void *);
s32 Func_080770c0(s32);
void Func_08015040(const void *, s32);
void Func_080030f8(s32);
void Func_08009238(void);

extern struct PriorityTable_0809c138 *Data_03001e70;
extern struct InterludeRuntime_0809c138 *Data_03001e68;
extern volatile u32 Data_03001b04;

/*
 * Run the palette-and-background interlude used by scene state three.
 * Object priorities are temporarily forced to one, the interlude assets and
 * callback are installed, and every touched display/object state is restored
 * before returning.
 */
void Func_0809c138(void)
{
    struct PriorityTable_0809c138 *priorities = Data_03001e70;
    struct SceneState_0809c138 *scene = Func_080048f4(0x1B, 0xCCC);
    struct InterludeRuntime_0809c138 *runtime = Data_03001e68;
    u8 saved_priorities[16];
    u32 saved_scene_mode;
    s16 saved_blend;
    u32 index;

    if (scene->mode != 3) {
        return;
    }

    scene->display->interlude_active = 1;
    saved_scene_mode = scene->display_mode;
    scene->display_mode = 6;
    Func_08091df4();
    Func_08091e20();

    for (index = 0; index < 16; index++) {
        saved_priorities[index] = (u8)priorities->records[index].priority;
        priorities->records[index].priority = 1;
    }

    runtime->active = 1;
    Func_080030f8(1);
    saved_blend = *(volatile s16 *)0x04000052;

    {
        void *palette = (void *)Func_08002f40(0x1B);
        struct DmaTransfer_0809c138 *dma =
            (struct DmaTransfer_0809c138 *)0x040000D4;

        dma->source = palette;
        dma->destination = (void *)0x05000000;
        dma->control = 0x84000070;
        *(volatile u16 *)0x05000000 = 0;
        Func_08005340((u8 *)palette + 0x1C0, (void *)0x02010000);
        dma->source = (const void *)0x02010000;
        dma->destination = (void *)0x06006A00;
        dma->control = 0x84002580;
    }

    *(volatile u16 *)0x0400000A = 0x682;
    *(volatile u16 *)0x04000000 = 0x1340;
    Func_0809bb64();
    Func_080041d8((const void *)0x0809BCF9, 0xC80);
    if (Func_080770c0(0x11C) != 0) {
        Func_08015040((const void *)0x985, 1);
    }
    do {
        Func_080030f8(1);
    } while ((Data_03001b04 & 3) == 0);

    Func_08004278((const void *)0x0809BCF9);
    Func_0809bcd4();
    *(volatile u16 *)0x04000000 = 0x40;
    Func_08009238();

    {
        volatile u16 *ime = (volatile u16 *)0x04000208;
        u16 saved_ime = *ime;
        volatile struct RegisterRestoreQueue_0809c138 *queue =
            (volatile struct RegisterRestoreQueue_0809c138 *)0x02002090;
        u16 slot = queue->count;

        *ime = (u16)0x04000208;
        if (slot <= 0x1F) {
            volatile struct RegisterRestore_0809c138 *restore =
                &queue->entries[slot];
            queue->count = slot + 1;
            restore->value = (u16)saved_blend;
            restore->register_address = (volatile void *)0x04000052;
            restore->mask = 0x20000;
        }
        *ime = saved_ime;
    }

    for (index = 0; index < 16; index++) {
        priorities->records[index].priority = saved_priorities[index];
    }

    runtime->active = 0;
    Func_08091dc8();
    Func_08091e20();
    scene->display_mode = saved_scene_mode;
    scene->display->interlude_active = 0;
}
