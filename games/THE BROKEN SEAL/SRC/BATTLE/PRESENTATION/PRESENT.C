#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "SCENE.H"
#include "RAM_BUFFER.H"
#include "BATTLE_PRESENTATION.H"
#include "MAP_SCROLL.H"
#include "HEAP_STATE.H"

static __inline__ void FillWords(void *dst, s32 size, s32 value)
{
    /* FAKEMATCH: a direct call changes BattlePresentation_BuildTilemap from push {r5, r6, lr} to push {r5, r6, r7, lr} (45/44 assembly lines). */
    Iwram_FillWords(dst, size, value);
}

/* Heap slot 44 holds the background view, slot 10 its scanline pages. */

extern u8 gDisp[];

/* Eight 4bpp tiles of a bar that narrows by a column from one to the next. */
extern const u8 BattlePres_TileVariants[];

/* Builds the battle presentation tilemap: 256 blank words, 128 border words,
   240 words of sequential tile pairs from 0x0201, then 640 border words. */
void BattlePresentation_BuildTilemap(s32 *destination)
{
    s32 entry;
    u32 index;

    FillWords(destination, 0x100, -1);
    destination += 0x40;
    FillWords(destination, 0x80, 0x03ff03ff);
    entry = (0x0201 << 16) | 0x0200;
    destination += 0x20;
    index = 0;
    do {
        index++;
        *destination++ = entry;
        entry += 0x00020002;
    } while (index <= 239);
    FillWords(destination, 0x280, 0x03ff03ff);
}

/* While the presentation mode is 2, restart the H-blank DMA that feeds
   the BG2 control register from the current scroll record, and copy
   that record's affine parameters. */
void BattlePres_UpdateHBlankScroll(void)
{
    struct BattleAffineHdma *records;
    u16 *record;
    s32 control;

    union HeapState *heap = (union HeapState *)gWorkSlot;
    void **cache = &heap->slots[44];

    if (((struct BattleBackgroundView *)cache[0])->mode == 2) {
        records = cache[10 - 44];
        record = records->lines[records->page];
        control = record[0];
        /* FAKEMATCH: the do-while keeps the BG2CNT address load after the
           record read. */
        do {
            *(u16 *)0x0400000c = control;
        } while (0);
        Dma_Set(record + 1, (void *)0x0400000c, 0xa2600001, (volatile u32 *)0x040000b0);
        Dma_Set(&records->affine, (void *)0x04000020, 0x84000004, (volatile u32 *)0x040000d4);
    }
}

void BattlePresentation_UploadTileVariant(void)
{
    u32 variant = **(u32 **)gDisp - 1;
    if (variant <= 31)
        Dma_Set(BattlePres_TileVariants + (variant >> 2) * 32, (void *)0x06005000, 0x84000008, (volatile u32 *)0x040000d4);
}

/* Raise the curtain, turn the camera and reduce the transition zoom. */
void BattlePres_AdvanceTransitionTimer(void)
{
    s32 scroll;
    struct BattleCamera *camera;
    u32 *timer;
    struct BgScroll *offset;
    u32 frame;
    u32 next;

    timer = *(u32 **)Ram_Disp;
    frame = *timer;
    camera = *(struct BattleCamera **)Ram_CameraWork;
    scroll = 0x34 - frame;
    if (scroll > 0x20)
        scroll = 0x20;
    offset = (struct BgScroll *)Ram_BgScroll;
    if (scroll < 0)
        scroll = 0;
    offset->y = (s16)scroll;
    if (frame <= 0x50U)
        camera->yaw = (s16)(45 * frame * 8 + 0xaf80);
    next = (*timer = *timer + 1);
    if (next <= 0x50U) {
        BattlePres_SetupTransitionScene(0, 0, 0, 0xb4 - next);
        return;
    }
    BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
}

/* battle/presentation/trans/draw_rows.c */
void BattlePres_DrawTransitionRows(void)
{
    u32 i;
    s32 frame;
    s32 aligned;
    s32 tile;
    u32 row;
    u16 *tiles;

    frame = **(s32 **)gDisp;
    if ((u32)frame <= 79) {
        tile = (7 & frame) + 0xf081;
        if (frame >= 0) {
            aligned = frame;
        } else {
            aligned = frame + 7;
        }
        row = 13 - (aligned >> 3);
        i = 0;
        tiles = (u16 *)((row << 6) + 0x06006000);
        do {
            i++;
            *tiles = tile;
            tiles++;
        } while (i != 32);

        tile = tile | 0x800;
        aligned = frame;
        if (frame < 0) {
            aligned = aligned + 7;
        }
        row = (aligned >> 3) + 13;
        if (row <= 20) {
            i = 0;
            tiles = (u16 *)((row << 6) + 0x06006000);
            do {
                i++;
                *tiles = tile;
                tiles++;
            } while (i != 32);
        }
    }
}

void Graphics_ClearBg0Vofs(void)
{
    u32 zero = 0;

    *(volatile u16 *)0x04000012 = zero;
}
