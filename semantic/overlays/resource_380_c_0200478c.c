typedef int s32;
typedef unsigned char u8;
typedef unsigned int u32;

/*
 * resource_380 owner at 0x0200478c, 200 bytes: build a tracked-object
 * roster.  Allocates a 404-byte array and zero-fills it with an
 * inline DMA3 transfer (0x040000d4 loaded and
 * written src/dst/control 0x85000065 in one stmia -- a 101-word fill
 * from a fixed source, the on-stack zero), clamps the requested count
 * to 10, then per entry: resolves object id (base+i), stores it in a
 * 40-byte slot, clears record byte +38 and object byte +85, pushes the
 * result of Func_020091aa(base+i) into Func_020090f8(.., 1), and seeds
 * slot +28/+32 from two resident tables (0x0200d140, negated
 * 0x0200d168) with slot byte +36 = 3.  Writes the final count at array
 * +400 and installs the resident watcher 0x0200c49d for 3200 ticks
 * (the same install idiom as the main image's Func_080041d8).
 *
 * Complete owner: full high-register prologue at 0x0200478c through
 * `pop {r0} / bx r0` at 0x0200483e, then the five-word literal pool
 * 0x02004840-0x02004853 (0x040000d4, 0x85000065, 0x0200d140,
 * 0x0200d168, 0x0200c49d); next owner's prologue at 0x02004854.
 *
 * Uncertainty: callees unidentified beyond call shape; the three
 * 0x0200xxxx pool constants are resident-module data/code pointers
 * (this overlay executes at 0x02000000 and the resident module sits at
 * 0x02008000+), recorded as raw constants.
 */

extern s32 Func_02009044(s32 arg0, s32 size);
extern s32 Func_02009196(s32 id);
extern s32 Func_020091aa(s32 id);
extern void Func_020090f8(s32 arg0, s32 arg1);
extern void Func_02009098(u32 callback, s32 duration);

void Func_0200478c(s32 base, u32 count)
{
    s32 zero = 0;
    u8 *array = (u8 *)Func_02009044(33, 404);
    volatile u32 *dma = (volatile u32 *)0x040000d4;
    u8 *slot;
    u32 index;

    dma[0] = (u32)&zero;
    dma[1] = (u32)array;
    dma[2] = 0x85000065;

    if (count > 10)
        count = 10;
    slot = array;
    for (index = 0; index < count; index++) {
        s32 object = Func_02009196(base + index);

        *(s32 *)slot = object;
        *(u8 *)(*(s32 *)(object + 80) + 38) = 0;
        *(u8 *)(object + 85) = 0;
        Func_020090f8(Func_020091aa(base + index), 1);
        *(s32 *)(slot + 28) = *(s32 *)(0x0200d140 + index * 4);
        *(s32 *)(slot + 32) = -*(s32 *)(0x0200d168 + index * 4);
        slot[36] = 3;
        slot += 40;
    }
    *(unsigned short *)(array + 400) = count;
    Func_02009098(0x0200c49d, 3200);
}
