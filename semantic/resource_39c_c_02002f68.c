#include "types.h"

/*
 * resource_39c owner at 0x02002f68, 1616 bytes through the four-word tail
 * pool.  This is the scene's long setup sequence: it creates the descriptor
 * used by the local spawner, emits two sixteen-item rows and a four-by-
 * thirty-two particle grid, then selects one of two presentation tails.
 *
 * The saved-register prologue is at 0x02002f68.  The only interworking return
 * is the pop {r0}/bx r0 pair at 0x020035a0-0x020035a2.  The first literal pool
 * is inline at 0x02003250-0x0200328f and is skipped by the conditional branch
 * at 0x0200324e.  The second branch uses the four words at 0x020035a4-
 * 0x020035b7; the next owner starts at 0x020035b8.  The machine span through
 * the return is 1596 bytes; including that tail pool makes the admitted whole
 * owner 0x650 (1616) bytes.
 *
 * All 135 resolved call sites are represented below (31 distinct callees).
 * The old-style imported interfaces are intentionally kept where their ABI
 * is a scene-service boundary; the values and field widths are transcribed
 * from the instructions, while the service names remain unresolved.
 */

extern u8 Data_0200adcd[];
extern void Func_0808a018(void);
extern void Func_0808a010(s32 frames);
extern void Func_02002e08(void);
extern void Func_080000d8(void *callback);
extern void Func_08009180(s32, s32, s32, s32, s32, s32);
extern void Func_080f9010(s32 cue);
extern s32 Func_080000f8(void);
extern void Func_0200013c(s32, s32, s32, s32, s32, s32, u32, void *);
extern void Func_080091f0(s32, s32, s32);
extern void Func_080091f8(void);
extern void Func_0808a5e8(void);
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a090(s32, s32, s32);
extern void Func_0808a0d0(s32, s32, s32);
extern void Func_0808a128(s32, s32, s32);
extern void Func_0808a0c8(s32, s32, s32);
extern void Func_0808a100(s32, s32);
extern void Func_02002e6c(s32);
extern void Func_0808a338(s32, s32);
extern void Func_0808a330(s32, s32);
extern void Func_0808a348(s32);
extern void Func_0808a248(s32);
extern void Func_0808a020(void);
extern void Func_0808a1b8(s32, s32, s32);
extern void Func_0808a0f0(s32, s32, s32);
extern void Func_0808a110(s32, s32);
extern void Func_0808a138(s32, s32);
extern void Func_0808a0e8(s32);
extern void Func_0808a1e8(s32, s32, s32);
extern void Func_0808a130(s32, s32);
extern void Func_0808a150(s32, s32, s32);

/* The original fixed-point expressions use a logical high-half extraction. */
static s32 RowX(s32 randomValue, s32 base)
{
    return base + (s32)(((u32)(randomValue * 48) >> 16) << 16);
}

static s32 RowY(s32 randomValue, s32 base)
{
    return base + (s32)(((u32)(randomValue * 56) >> 16) << 16);
}

void Func_02002f68(void)
{
    u8 descriptor[32];
    s32 randomValue;
    s32 x;
    s32 y;
    s32 offset;
    s32 row;
    s32 item;

    Func_0808a018();
    Func_0808a010(20);
    Func_02002e08();
    Func_080000d8(Data_0200adcd);
    Func_08009180(-45, 77, -45, 73, 9, 4);
    Func_0808a010(30);

    *(s32 *)(descriptor + 4) = 5;
    *(u16 *)(descriptor + 24) = 0x11e;
    *(s32 *)(descriptor + 0) = 1;
    *(u32 *)(descriptor + 28) = 0x0200de70;

    /* First sixteen-item row. */
    for (item = 0; item <= 15; item++) {
        if ((item & 1) != 0) {
            Func_080f9010(246);
        }
        randomValue = Func_080000f8();
        x = RowX(randomValue, 0x03000000);
        randomValue = Func_080000f8();
        y = RowY(randomValue, 0x00880000);
        Func_0200013c(x, 0, y, 0, 0, 0, 0x00330001, descriptor);
        Func_0808a010(2);
    }

    Func_0808a010(40);

    /* Second sixteen-item row. */
    for (item = 0; item <= 15; item++) {
        if ((item & 1) != 0) {
            Func_080f9010(246);
        }
        randomValue = Func_080000f8();
        x = RowX(randomValue, 0x03000000);
        randomValue = Func_080000f8();
        y = RowY(randomValue, 0x00980000);
        randomValue = Func_080000f8();
        offset = -0x3333 * ((s32)(((u32)(randomValue * 10)) >> 16) + 1);
        Func_0200013c(x, 0, y, 0, 0, offset, 0x00330001, descriptor);
        Func_0808a010(2);
    }

    Func_0808a010(60);
    Func_080f9010(141);
    Func_080091f0(0x500000, 0x500000, 0x40000);
    Func_0808a010(60);

    /* Four rows of thirty-two items, with the row-specific vertical offset. */
    for (row = 0; row <= 3; row++) {
        Func_08009180(59, 12 - row, 48, 3, 3, 1);
        for (item = 0; item <= 31; item++) {
            s32 first;
            s32 second;

            randomValue = Func_080000f8();
            x = RowX(randomValue, 0x03000000);
            randomValue = Func_080000f8();
            first = (s32)(((u32)(randomValue * 8)) >> 16);
            y = 0x000d0001 + 0x1999 * first;
            randomValue = Func_080000f8();
            offset = 0x00c00000 - (item >> 1) - (row * 16);
            Func_0200013c(x, 0, offset, y, 0, 0x1999 * second,
                          0x000d0001, descriptor);
            Func_0808a010(2);
            second = (s32)(((u32)(randomValue * 8)) >> 16);
        }
    }

    Func_080f9010(0x121);
    Func_080091f0(-1, -1, 0x0000e666);
    Func_080091f8();
    Func_0808a5e8();
    Func_0808a010(30);

    if (Func_080770c0(0x0881) != 0) {
        Func_0808a090(0, 0x0000cccc, 0x00006666);
        Func_0808a0d0(0, 0x338, 0xe8);
        Func_0808a0d0(0, 0x318, 0xe8);
        Func_0808a0d0(0, 0x318, 0xd0);
        Func_0808a090(0, 0x20000, 0x10000);
        Func_0808a128(0, 4, 0);
        Func_0808a0c8(0, 0x318, 0xc8);
        Func_0808a010(10);
        Func_0808a100(0, 18);
        Func_02002e6c(0);
        Func_0808a010(60);
        Func_080000d8(Data_0200adcd);
        Func_0808a338(0x10000, 0);
        Func_0808a330(0x00010005, 0);
        Func_0808a348(120);
        Func_0808a010(120);
        Func_0808a330(0x00007fff, 0);
        Func_0808a348(60);
        Func_0808a010(60);
        Func_0808a248(9);
        Func_0808a020();
    } else {
        Func_0808a090(0, 0x0000cccc, 0x00006666);
        Func_0808a090(1, 0x0000cccc, 0x00006666);
        Func_0808a090(2, 0x0000cccc, 0x00006666);
        Func_0808a090(3, 0x0000cccc, 0x00006666);
        Func_0808a0d0(0, 0x338, 0xf0);
        Func_0808a1b8(0, 0xa000, 20);
        Func_0808a0f0(3, 0x3380000, 0xf00000);
        Func_0808a0d0(3, 0x318, 0xe8);
        Func_0808a1b8(3, 0xc000, 0);
        Func_0808a010(60);
        Func_0808a1b8(3, 0x2000, 20);
        Func_0808a110(3, 3);
        Func_0808a010(40);
        Func_0808a1b8(3, 0xc000, 20);
        Func_0808a0d0(3, 0x318, 0xc8);
        Func_02002e6c(3);
        Func_0808a010(20);
        Func_0808a138(0, 2);
        Func_0808a010(30);
        Func_0808a0d0(0, 0x318, 0xe8);
        Func_0808a1b8(0, 0xc000, 0);
        Func_0808a0f0(1, 0x3180000, 0xe80000);
        Func_0808a0f0(2, 0x3180000, 0xe80000);
        Func_0808a0c8(1, 0x330, 0xe0);
        Func_0808a0d0(2, 0x300, 0xe0);
        Func_0808a0e8(1);
        Func_0808a1b8(1, 0xa000, 0);
        Func_0808a1b8(2, 0xe000, 20);
        Func_0808a1e8(0, 0x102, 0);
        Func_0808a1e8(1, 0x102, 0);
        Func_0808a1e8(2, 0x102, 80);
        Func_0808a1b8(1, 0x6000, 0);
        Func_0808a1b8(2, 0x2000, 20);
        Func_0808a138(0, 1);
        Func_0808a010(60);
        Func_0808a090(0, 0x8000, 0x4000);
        Func_0808a0d0(0, 0x318, 0xe0);
        Func_0808a1b8(1, 0x8000, 0);
        Func_0808a1b8(2, 0, 0);
        Func_0808a0d0(0, 0x318, 0xd0);
        Func_0808a1b8(1, 0xa000, 0);
        Func_0808a1b8(2, 0xe000, 20);
        Func_0808a110(0, 3);
        Func_0808a010(20);
        Func_0808a090(0, 0x20000, 0x10000);
        Func_0808a128(0, 4, 0);
        Func_0808a0c8(0, 0x318, 0xc8);
        Func_0808a010(10);
        Func_0808a100(0, 18);
        Func_0808a1e8(1, 0x100, 0);
        Func_0808a1e8(2, 0x100, 0);
        Func_0808a130(1, 2);
        Func_0808a130(2, 2);
        Func_02002e6c(0);
        Func_0808a010(60);
        Func_0808a1e8(1, 0x102, 0);
        Func_0808a1e8(2, 0x102, 80);
        Func_0808a150(1, 2, 20);
        Func_0808a100(1, 3);
        Func_0808a110(2, 3);
        Func_0808a010(40);
        Func_0808a0d0(1, 0x318, 0xd8);
        Func_0808a1b8(2, 0xe000, 0);
        Func_0808a0d0(1, 0x318, 0xc8);
        Func_0808a010(30);
        Func_02002e6c(1);
        Func_0808a0d0(2, 0x318, 0xd8);
        Func_0808a0d0(2, 0x318, 0xc8);
        Func_0808a010(30);
        Func_02002e6c(2);
        Func_0808a338(0x10000, 0);
        Func_080000d8(Data_0200adcd);
        Func_0808a330(0x00010005, 0);
        Func_0808a348(120);
        Func_0808a010(120);
        Func_0808a330(0x00007fff, 0);
        Func_0808a348(60);
        Func_0808a010(60);
        Func_0808a020();
        Func_0808a248(8);
    }
}
