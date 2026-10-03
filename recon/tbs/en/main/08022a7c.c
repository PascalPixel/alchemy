#include "RESOURCE.H"
/* Not-yet-C: complete 200-byte nested stat-arrow owner.
 * The typed sprite initializer recovers attributes+4 then tile+8 writes,
 * and x before y, but gives 202 bytes / 19 aligned edits (89 halfwords).
 * Zero is hoisted into r8 across the resource call, moving x to r9; the
 * original 198-byte / seven-edit model remains in the parent revision.
 * Returning the subrecord from the helper also swaps work/entry registers
 * and gives 202 bytes / 35 edits. Stop this interface axis: it repairs the
 * store ownership, not the zero lifetime. No adoption or byte credit. */
#include "DJINN_PREVIEW.H"

static __inline__ void InitializeArrow(struct PreviewSprite *entry)
{
    entry->attributes.word = 0x40000400;
    entry->tile.value = 0;
}
extern void DjinnMenu_DrawStatArrow(s32 x, s32 y, s32 rising)
    __attribute__((alias("Draw_08022a7c.0")));

/* The caller keeps win four bytes below the nested function's static chain. */
static __inline__ void Scope_08022a7c(struct RenderInput *win)
{
    void Draw_08022a7c(s32 x, s32 y, s32 rising)
    {
        struct RenderOutput *work = RenderOutput_AcquireFree();
        struct PreviewSprite *entry;

        if (work) {
            work->active = 1;
            work->kind = 1;
            work->index = Resource_LoadIntoFreeSlot(128);
            entry = (struct PreviewSprite *)((u8 *)work + 16);
            work->sentinel = 240;
            work->x = 120;
            work->y = 120;
            InitializeArrow(entry);
            entry->attributes.bits.x = win->x * 8 + x;
            entry->attributes.bits.y = win->y * 8 + y;
            entry->tile.bits.index = Resource_GetBuffer(
                (u8)work->index, (s32)(rising ? Data_080313a4 : Data_08031424));
            RenderOutput_AppendToList(win, work);
        }
    }
}
