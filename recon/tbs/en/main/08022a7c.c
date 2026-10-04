#include "RESOURCE.H"
/* Not-yet-C: complete 200-byte nested stat-arrow owner of
 * DjinnMenu_ShowChangePreview (main:08022b44), which must adopt it in the
 * same object. The current best is the parent's draft (2 off with both
 * addresses; its header has the sched2 dependence analysis).Plain stores in the order active, kind, index, sprite, tile
 * clear, attribute word, sentinel, x, y now give the reference allocation
 * (x r8, chain r9, y sl, rising fp, output r6, sprite r7, zero and window
 * address r5); only sched2 differs: the sentinel/x/y stores follow the
 * window load instead of preceding the sprite stores (6 reordered, score
 * 360 plus the trailing alignment halfword the scorer counts as deleted).
 * Writing sentinel/x/y before the sprite stores lengthens the and-mask zero
 * (CSE'd from the active/kind stores) from 20 to 34 insns; at 2/34 against
 * the static chain's 2/22, global.c evicts it from r5 (20 register edits).
 * All 240 orders of sentinel/x/y/sprite/word/clear, x/y/tile orders and a
 * split index temporary were scored; none keeps both. No adoption.
 * The 34 is local-alloc's doubled length for the zero's REG_EQUIV (raw 17,
 * which would rank above the chain): the same doubling explains the
 * debug glyph browser's mask/work swap (main:0802977c). */
#include "RENDER_INPUT.H"

/* The OAM attribute words embedded at output+16, as UiWindow_PutGlyph's
 * SpriteAttr views them. */
struct ArrowSprite {
    u32 unknown;
    union {
        u32 word;
        struct { u8 y; u8 flags; u16 x : 9; u16 high : 7; } bits;
    } attr;
    union RenderTableValue tile;
};

struct RenderOutput *RenderOutput_AcquireFree(void);
void RenderOutput_AppendToList(struct RenderInput *, struct RenderOutput *);
extern u8 Data_080313a4[];
extern u8 Data_08031424[];

extern void DjinnMenu_DrawStatArrow(s32 x, s32 y, s32 rising)
    __attribute__((alias("Draw_08022a7c.0")));

/* The caller keeps win four bytes below the nested function's static chain. */
static __inline__ void Scope_08022a7c(struct RenderInput *win)
{
    void Draw_08022a7c(s32 x, s32 y, s32 rising)
    {
        struct RenderOutput *work = RenderOutput_AcquireFree();
        struct ArrowSprite *sprite;

        if (work) {
            work->active = 1;
            work->kind = 1;
            work->index = Resource_LoadIntoFreeSlot(128);
            sprite = (struct ArrowSprite *)work->unknown_10;
            sprite->tile.value = 0;
            sprite->attr.word = 0x40000400;
            work->sentinel = 240;
            work->x = 120;
            work->y = 120;
            sprite->attr.bits.x = win->x * 8 + x;
            sprite->attr.bits.y = win->y * 8 + y;
            sprite->tile.bits.index = Resource_GetBuffer(
                (u8)work->index, rising ? Data_080313a4 : Data_08031424);
            RenderOutput_AppendToList(win, work);
        }
    }
}
