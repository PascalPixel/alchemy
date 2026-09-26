/* NONMATCHING: 85 differing halfwords, 27 halfword edits; 202 / 200 bytes.
 * The aggregate uses one constant store, but zero moves to r8, x to r9,
 * and the static chain to r5; the attribute store follows the window load.
 * The four call targets match; allocation, scheduling and both pools differ. */
#include "DJINN_PREVIEW.H"
extern void DjinnMenu_DrawStatArrow(s32 x, s32 y, s32 rising)
    __attribute__((alias("Draw_08022a7c.0")));

/* The caller keeps win four bytes below the nested function's static chain. */
static __inline__ void Scope_08022a7c(struct RenderInput *win)
{
    void Draw_08022a7c(s32 x, s32 y, s32 rising)
    {
        struct RenderOutput *work = RenderOutput_AcquireFree();
        struct PreviewSprite *entry;
        const struct PreviewAttributes attrs = {0, 4, 0, 32};

        if (work) {
            work->one5 = 1;
            work->one4 = 1;
            work->index = Resource_LoadIntoFreeSlot(128);
            entry = (struct PreviewSprite *)((u8 *)work + 16);
            work->sentinel = 240;
            work->x = 120;
            work->y = 120;
            entry->attributes = attrs;
            entry->tile.value = 0;
            entry->attributes.x = win->x * 8 + x;
            entry->attributes.y = win->y * 8 + y;
            entry->tile.bits.index = Resource_GetBuffer(
                (u8)work->index, (s32)(rising ? Data_080313a4 : Data_08031424));
            RenderOutput_AppendToList(win, work);
        }
    }
}
