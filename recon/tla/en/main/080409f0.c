/* Near miss, differ 4 of 39 (two reordered rows), 2026-10-15: written from
 * the listing. It opens a 15x9 window at (7, 3), creates an output there
 * from fixed block B, keeps it in the work at 0x5a4 and places it with
 * Func_08108048 at the window's corner. Remaining: the native stores the
 * output and copies its address to r0 right after it forms it; this stores
 * and copies after the window's coordinates are loaded. An empty asm after
 * the store moves the argument set-up of RenderOutput_Create instead
 * (differ 4), and the address pinned to r0 changes the allocation (differ
 * 20). The other editions' scaffold holds this function in one piece with
 * the next.
 */
#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
void Func_08108048(void *entry, s32 x, s32 y);

struct RenderInput *Func_080409f0(void)
{
    u8 *work = Ram_HeapSlots->unkd0[0];
    struct RenderInput *window;
    s32 no;

    window = UiWindow_Create(7, 3, 15, 9, 2);
    no = Resource_LoadFixedBlockBIntoFreeSlot();
    *(struct RenderOutput **)(work + 0x5a4) = RenderOutput_Create(no, 0x40000000, window, 0, 0);
    Func_08108048(work + 0x5a4, ((s16)window->x << 3) - 4, ((s16)window->y << 3) + 12);
    return window;
}
