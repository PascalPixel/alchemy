/* resource_370 0x02008054..0x02008154 Clear_LoadBackground (256 bytes with
 * pool), formerly MENU/CLEAR/BG_SETUP.C; the listing keeps the rows.
 * Remaining difference: the reference loads resource id 0x1a from its
 * literal pool into r5 before the first call, the shape of a link-time
 * symbol, which this draft spells as the equate Value_0000001a; and GCC
 * keeps gMapCellBuffer in r5 across the decode call where the reference
 * reloads it from the pool (139 bytes differ, 4 bytes longer). Data_03001ad0
 * is gBgScroll. */
#include "TYPES.H"
#include "DMA.H"
#include "FIELD_EVENT.H"
extern struct MapRenderWork *gMapWork;
extern u8 gMapCellBuffer[];

extern u8 Value_0000001a[];
extern u16 Data_03001ad0[];

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

    id = (s32)Value_0000001a;
    Engine_BlendSetDarkenTarget16(0);
    *(volatile u16 *)0x0400000c = 0x681;
    Data_03001ad0[5] = 0;
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
    scroll = (struct ScrollPair *)Data_03001ad0;
    for (y = 0; y <= 3; y++) {
        scroll->y = 0;
        scroll->x = 0;
        scroll++;
    }
    Dma_Set(Data_03001ad0, (void *)0x04000010, 0x84000004, DMA3);
    (*(struct ClearWork **)&gMapWork)->mode = 0x1400;
    {
        struct FieldActor *leader;

        leader = Engine_ActorGet(gGameState.selected_actor);
        /* FAKEMATCH: a halfword zero keeps the reference's short-reach pool. */
        zero = 0;
        leader->motion_flags = zero;
    }
}
