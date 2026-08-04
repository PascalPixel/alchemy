/*
 * The call at 0x03000164 is reached through GCC's call-via-register veneer.
 * Its wider semantics remain uncertain; this owner proves only the two-argument
 * resident entry point represented below.
 */
#include "layout_guard.h"
#include "types.h"

typedef void (*ResidentEntry_080cd594)(void *destination, u32 size);

typedef struct DeferredWrite_080cd594 {
    u32 value;
    u32 address;
    u32 mask;
} DeferredWrite_080cd594;

typedef struct DeferredWriteQueue_080cd594 {
    u16 count;
    u16 padding02;
    DeferredWrite_080cd594 entries[32];
} DeferredWriteQueue_080cd594;

typedef struct PrimaryWork_080cd594 {
    u8 padding0000[0x77a0];
    s32 viewport_width;
    s32 viewport_height;
    s32 background_mode;
    u8 padding77ac[8];
    s32 scroll_step;
    s32 scroll_phase;
} PrimaryWork_080cd594;

typedef struct SyncState_080cd594 {
    u8 padding00[0x0c];
    s32 locked;
} SyncState_080cd594;

typedef struct BackgroundRuntime_080cd594 {
    PrimaryWork_080cd594 *primary;
    void *resident_buffer;
    u8 padding08[0x0c];
    SyncState_080cd594 *sync;
} BackgroundRuntime_080cd594;

typedef struct WorldState_080cd594 {
    u8 padding000[0x648];
    u16 palette_id;
} WorldState_080cd594;

typedef struct DisplayMetrics_080cd594 {
    u8 padding00[4];
    u16 width;
    u16 height;
} DisplayMetrics_080cd594;

typedef struct AffineRegisters_080cd594 {
    u16 pa;
    u16 pb;
    u16 pc;
    u16 pd;
    s32 x;
    s32 y;
} AffineRegisters_080cd594;

typedef struct WindowRegisters_080cd594 {
    u16 win0_horizontal;
    u16 win1_horizontal;
    u16 win0_vertical;
    u16 win1_vertical;
    u16 inside;
    u16 outside;
} WindowRegisters_080cd594;

typedef struct BlendRegisters_080cd594 {
    u16 control;
    u16 alpha;
} BlendRegisters_080cd594;

LAYOUT_SIZE_GUARD(
    DeferredWrite080cd594_Size,
    DeferredWrite_080cd594,
    0x0c);
LAYOUT_OFFSET_GUARD(
    DeferredWriteQueue080cd594_Entries,
    DeferredWriteQueue_080cd594,
    entries,
    4);
LAYOUT_OFFSET_GUARD(
    PrimaryWork080cd594_ViewportWidth,
    PrimaryWork_080cd594,
    viewport_width,
    0x77a0);
LAYOUT_OFFSET_GUARD(
    PrimaryWork080cd594_BackgroundMode,
    PrimaryWork_080cd594,
    background_mode,
    0x77a8);
LAYOUT_OFFSET_GUARD(
    PrimaryWork080cd594_ScrollStep,
    PrimaryWork_080cd594,
    scroll_step,
    0x77b4);
LAYOUT_OFFSET_GUARD(
    BackgroundRuntime080cd594_ResidentBuffer,
    BackgroundRuntime_080cd594,
    resident_buffer,
    4);
LAYOUT_OFFSET_GUARD(
    BackgroundRuntime080cd594_Sync,
    BackgroundRuntime_080cd594,
    sync,
    0x14);
LAYOUT_OFFSET_GUARD(
    WorldState080cd594_PaletteId,
    WorldState_080cd594,
    palette_id,
    0x648);
LAYOUT_SIZE_GUARD(
    AffineRegisters080cd594_Size,
    AffineRegisters_080cd594,
    0x10);

extern BackgroundRuntime_080cd594 Data_03001eec;
extern WorldState_080cd594 *Data_03001e74;
extern DisplayMetrics_080cd594 Data_03001ad0;
extern DeferredWriteQueue_080cd594 Data_02002090;

void Func_080030f8(u32);
void Func_080041d8(const void *, s32);
void Func_080b5028(s32, s32, s32, s32);
void Func_080b5038(s32, u16, s32);
void Func_080cd508(u32);

static void QueueDisplayWrite_080cd594(u32 value)
{
    volatile u16 *interrupt_master = (volatile u16 *)0x04000208;
    u16 saved = *interrupt_master;

    /* The original writes IME's even address, clearing the enable bit. */
    *interrupt_master = (u16)(u32)interrupt_master;
    if (Data_02002090.count <= 31) {
        DeferredWrite_080cd594 *write =
            &Data_02002090.entries[Data_02002090.count++];

        write->value = value;
        write->address = 0x04000000;
        write->mask = 0x00020000;
    }
    *interrupt_master = saved;
}

static void BuildAffineTilemap_080cd594(void)
{
    volatile u16 *tilemap = (volatile u16 *)0x06003800;
    s32 row;

    for (row = 0; row < 16; row++) {
        s32 column;

        for (column = 0; column < 8; column++) {
            tilemap[row * 8 + column] = (u16)(
                (0x100 + row * 0x1000 + column * 0x200) |
                (row * 0x10 + column * 2));
        }
    }
}

/* Configure and clear the first affine battle-background presentation. */
void Func_080cd594(u32 background_bits)
{
    ResidentEntry_080cd594 resident =
        (ResidentEntry_080cd594)0x03000164;
    BackgroundRuntime_080cd594 *runtime = &Data_03001eec;
    PrimaryWork_080cd594 *primary = runtime->primary;
    WorldState_080cd594 *world = Data_03001e74;
    volatile u16 *bg3_control = (volatile u16 *)0x0400000c;
    volatile AffineRegisters_080cd594 *affine =
        (volatile AffineRegisters_080cd594 *)0x04000020;
    volatile WindowRegisters_080cd594 *window =
        (volatile WindowRegisters_080cd594 *)0x04000040;
    volatile BlendRegisters_080cd594 *blend =
        (volatile BlendRegisters_080cd594 *)0x04000050;

    Func_080cd508(background_bits);
    runtime->sync->locked = 1;
    Func_080030f8(1);

    blend->control = 0;
    QueueDisplayWrite_080cd594(0x7741);
    Data_03001ad0.height = 0x20;
    Func_080030f8(1);

    Func_080b5038(1, world->palette_id, 0);
    primary->scroll_step = 0;
    primary->scroll_phase = 0;
    Func_080041d8((const void *)0x080cd4b5, 0x0c80);
    QueueDisplayWrite_080cd594(0x7341);
    Func_080030f8(1);

    *bg3_control = (u16)(background_bits | 0x0784);
    QueueDisplayWrite_080cd594(0x7341);
    Func_080b5028(0, 0, 0, 0x64);
    runtime->sync->locked = 0;
    Func_080030f8(1);

    blend->control = 0x3f44;
    blend->alpha = 0x100e;
    affine->x = 0;
    affine->y = -0x1000;
    affine->pa = 0x80;
    affine->pb = 0;
    affine->pc = 0;
    affine->pd = 0x100;
    window->win0_horizontal = 0x00f0;
    window->win1_horizontal = 0x00f0;
    window->win0_vertical = 0x1088;
    window->win1_vertical = 0x1088;
    window->inside = 0x3537;
    window->outside = 0x3f21;
    QueueDisplayWrite_080cd594(0x7741);

    BuildAffineTilemap_080cd594();
    resident(runtime->resident_buffer, 0x4000);
    resident((void *)0x06004000, 0x4000);
    primary->background_mode = 0;
    primary->viewport_width = Data_03001ad0.width;
    primary->viewport_height = Data_03001ad0.height;
    Func_080030f8(1);
}
