#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "SCENE.H"
#include "RAM_BUFFER.H"
#include "BTLDISP.H"
#include "MAP_SCROLL.H"
#include "IO_REG.H"
#include "HEAP_STATE.H"

static __inline__ void FillWords(void *dst, s32 size, s32 value)
{
    /* FAKEMATCH: needed to match: a direct call changes BattlePresentation_BuildTilemap from push {r5, r6, lr} to push {r5, r6, r7, lr} */
    Iwram_FillWords(dst, size, value);
}

extern void *gWorkSlot[64];

extern u8 gDisp[];


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

void BattlePres_UpdateHBlankScroll(void)
{
    struct BattleAffineHdma *records;
    u16 *record;
    s32 control;

    if (((struct BattleBackgroundView *)*(void **)((u8 *)gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND * sizeof(void *)))->mode == 2) {
        records = *(struct BattleAffineHdma **)&gWorkSlot[10];
        record = records->lines[records->page];
        control = record[0];
        /* FAKEMATCH: needed to match: the do-while keeps the BG2CNT address load after the record read */
        do {
            REG_BG2CNT = control;
        } while (0);
        Dma_Set(record + 1, (void *)0x0400000c, 0xa2600001, &REG_DMA0SAD);
        Dma_Set(&records->affine, (void *)0x04000020, 0x84000004, REG_DMA3);
    }
}

void BattlePresentation_UploadTileVariant(void)
{
    u32 variant = **(u32 **)gDisp - 1;
    if (variant <= 31)
        Dma_Set(BattlePres_TileVariants + (variant >> 2) * 32, (void *)0x06005000, 0x84000008, REG_DMA3);
}

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
    offset->y = scroll;
    if (frame <= 0x50U)
        camera->yaw = 45 * frame * 8 + 0xaf80;
    next = (*timer = *timer + 1);
    if (next <= 0x50U) {
        BattlePres_SetupTransitionScene(0, 0, 0, 0xb4 - next);
        return;
    }
    BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
}

void BattlePres_DrawTransitionRows(void)
{
    u32 i;
    s32 frame;
    s32 aligned;
    s32 tile;
    u32 row;
    u16 *tiles;

    frame = **(s32 **)gDisp;
    if (frame >= 0 && frame <= 79) {
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

    REG_BG0VOFS = zero;
}
