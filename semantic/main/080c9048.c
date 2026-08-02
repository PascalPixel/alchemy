#include "layout_guard.h"
#include "types.h"

typedef struct DeferredWrite_080c9048 {
    u32 value;
    u32 address;
    u32 mask;
} DeferredWrite_080c9048;

typedef struct DeferredWriteQueue_080c9048 {
    u16 count;
    u16 padding02;
    DeferredWrite_080c9048 entries[32];
} DeferredWriteQueue_080c9048;

typedef struct WindowDimensions_080c9048 {
    u16 win0_horizontal;
    u16 win1_horizontal;
    u16 win0_vertical;
    u16 win1_vertical;
} WindowDimensions_080c9048;

typedef struct WindowControl_080c9048 {
    u16 inside;
    u16 outside;
} WindowControl_080c9048;

typedef struct BlendControl_080c9048 {
    u16 control;
    u16 alpha;
} BlendControl_080c9048;

LAYOUT_SIZE_GUARD(
    DeferredWrite080c9048_Size,
    DeferredWrite_080c9048,
    0x0c);
LAYOUT_OFFSET_GUARD(
    DeferredWriteQueue080c9048_Entries,
    DeferredWriteQueue_080c9048,
    entries,
    4);

extern DeferredWriteQueue_080c9048 Data_02002090;

void Func_080030f8(u32 frames);

/* Configure the letterbox window and defer the matching display-mode write. */
void Func_080c9048(void)
{
    volatile WindowDimensions_080c9048 *window =
        (volatile WindowDimensions_080c9048 *)0x04000040;
    volatile WindowControl_080c9048 *window_control =
        (volatile WindowControl_080c9048 *)0x04000048;
    volatile BlendControl_080c9048 *blend =
        (volatile BlendControl_080c9048 *)0x04000050;
    volatile u16 *interrupt_master = (volatile u16 *)0x04000208;
    u16 saved_interrupt_master;

    blend->control = 0;
    blend->alpha = 0x100e;
    window->win0_horizontal = 0x00f0;
    window->win0_vertical = 0x1088;
    window->win1_horizontal = 0x00f0;
    window->win1_vertical = 0x1088;
    window_control->inside = 0x3537;
    window_control->outside = 0x3f21;

    saved_interrupt_master = *interrupt_master;
    /* The original stores IME's own even address; bit 0 is therefore clear. */
    *interrupt_master = (u16)(u32)interrupt_master;
    if (Data_02002090.count <= 31) {
        DeferredWrite_080c9048 *write =
            &Data_02002090.entries[Data_02002090.count++];

        write->value = 0x7741;
        write->address = 0x04000000;
        write->mask = 0x00020000;
    }
    *interrupt_master = saved_interrupt_master;

    Func_080030f8(1);
}
