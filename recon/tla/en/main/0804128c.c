/* Near miss, differ 2 of 87 (one reordered pair), 2026-10-15: written
 * from the listing in the form of its twin Func_08040cc4 (menu/screenc.c):
 * eight dividers, then the four columns of the ability grid in nine rows.
 * The divider row and the grid row need their own variables (one shared y
 * trades r5 and r7, differ 31), and the grid loop steps y before the
 * counter (differ 4 the other way). Remaining: before the grid loop the
 * native loads the table's address from the pool (ldr r5) before it zeroes
 * the counter (movs r3, #0; mov r8, r3); this zeroes first (equal priority,
 * the zero has a dependent). The table as a pinned pointer behind a barrier
 * holding r3 fixes the order, but the counter is then reversed (differ 7)
 * or, pinned as an array base, the loop is not reduced (differ 87). A do
 * loop, a table local and three permuter minutes found nothing lower. */
#include "types.h"
#include "ram_buffer.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern const s16 Data_080aa170[4][9];

struct UiWindow;
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 mode);
void UiWindow_DrawDividerLine(struct UiWindow *window, s32 x1, s32 y1, s32 x2, s32 y2);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
s32 RenderOutput_Create(s32 resource, s32 flags, struct UiWindow *window, s32 x, s32 y);
void Func_08041268(s32 ability, struct UiWindow *window, s32 x, s32 y);

struct UiWindow *Func_0804128c(void)
{
    void *work = Ram_HeapSlots->unknown_d0[0];
    struct UiWindow *window = UiWindow_Create(0, 0, 30, 19, 2);
    const s16 *abilities;
    s32 i;
    s32 y;
    s32 row_y;

    for (i = 0, row_y = 2; i < 8; i++, row_y += 2)
        UiWindow_DrawDividerLine(window, 0, row_y, 29, row_y);
    for (y = 0, i = 0; i < 9; y += 16, i++) {
        Func_08041268(Data_080aa170[0][i], window, 8, y);
        Func_08041268(Data_080aa170[1][i], window, 64, y);
        Func_08041268(Data_080aa170[2][i], window, 120, y);
        Func_08041268(Data_080aa170[3][i], window, 176, y);
    }
    WORD_AT(work, 1444) = RenderOutput_Create(Resource_LoadFixedBlockBIntoFreeSlot(), 0x40000000, window, 0, 0);
    return window;
}
