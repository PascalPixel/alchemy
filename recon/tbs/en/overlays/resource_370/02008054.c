/* resource_370 0x02008054..0x02008154 Clear_LoadBackground (256 bytes with
 * pool), formerly MENU/CLEAR/BG_SETUP.C; the listing keeps the rows.
 * The resource it loads is row 0x1a of the resource directory, now
 * ResourceId_GoldenSunLogo, and the scroll buffer is gBgScroll. Remaining
 * difference: GCC keeps gMapCellBuffer in r5 across the decode call where
 * the reference reloads it from the pool (260 bytes against 256, 76
 * halfwords differ). */
#include "TYPES.H"
#include "DMA.H"
#include "FIELD_EVENT.H"
extern struct MapRenderWork *gMapWork;
extern u8 gMapCellBuffer[];

#include "RESOURCE_IDS.H"
extern u16 gBgScroll[];

struct ClearWork {
    u8 unknown_00[20];
    u16 mode;
};

struct ScrollPair {
    u16 x;
    u16 y;
};

static __inline__ void DecodeBackground(const u8 *res)
{
    Engine_ResourceDecodeType01(res, (void *)gMapCellBuffer);
}

#define DMA3 ((volatile u32 *)0x040000d4)

/* Load the clear-screen background: palette, tiles and a 30 x 20 map counting up from
 * tile 0x1a0, then clear the scroll registers. */
void Clear_LoadBackground(void)
{
    u8 *res;
    s32 id;
    u16 *map;
    u32 x;
    u32 y;
    s32 tile;
    struct ScrollPair *scroll;
    s32 blank;
    u16 zero;

    id = (s32)&ResourceId_GoldenSunLogo;
    Engine_BlendSetDarkenTarget16(0);
    *(volatile u16 *)0x0400000c = 0x681;
    gBgScroll[5] = 0;
    blank = 0x1ff;
    res = Engine_ResourceGetTableEntry(id);
    Dma_Set(res, (void *)0x05000000, 0x84000070, DMA3);
    res += 0x1c0;
    DecodeBackground(res);
    Dma_Set((void *)gMapCellBuffer, (void *)0x06006800, 0x84002580, DMA3);
    map = (u16 *)0x06003000;
    tile = 0x1a0;
    y = 0;
col:
    {
        x = 0;
    row:
        {
            s32 old = tile;

            /* FAKEMATCH: keep the signed tile wrap in the high half. */
            tile = ((old << 16) + 0x10000) >> 16;
            *map++ = old;
        }
        if (++x <= 29)
            goto row;
        *map++ = blank;
        *map++ = blank;
    }
    if (++y <= 19)
        goto col;
    scroll = (struct ScrollPair *)gBgScroll;
    for (y = 0; y <= 3; y++) {
        scroll->y = 0;
        scroll->x = 0;
        scroll++;
    }
    Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, DMA3);
    (*(struct ClearWork **)&gMapWork)->mode = 0x1400;
    {
        struct FieldActor *leader;

        leader = Engine_ActorGet(gGameState.selected_actor);
        /* FAKEMATCH: a halfword zero keeps the reference's short-reach pool. */
        zero = 0;
        leader->motion_flags = zero;
    }
}
