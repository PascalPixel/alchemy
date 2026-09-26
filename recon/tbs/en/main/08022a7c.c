/* NONMATCHING: 7 differing halfwords, 7 halfword edits; 198+2 / 200 bytes.
 * At +0x34..+0x38 the entry copy follows the reversed x/y stores; at
 * +0x44..+0x4a the zero store uses work+24 and precedes the attribute store.
 * The static chain, four calls and both literal pools match. */
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

        if (work) {
            work->one5 = 1;
            work->one4 = 1;
            work->index = Resource_LoadIntoFreeSlot(128);
            entry = (struct PreviewSprite *)((u8 *)work + 16);
            work->table.value = 0;
            work->sentinel = 240;
            work->y = 120;
            work->x = 120;
            entry->attributes.word = 0x40000400;
            entry->attributes.bits.x = win->x * 8 + x;
            entry->attributes.bits.y = win->y * 8 + y;
            entry->tile.bits.index = Resource_GetBuffer(
                (u8)work->index, (s32)(rising ? Data_080313a4 : Data_08031424));
            RenderOutput_AppendToList(win, work);
        }
    }
}
