typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

struct Entry {
    u16 size;
    u16 offset;
};

struct Dma {
    u32 source;
    u32 destination;
    u32 control;
};

extern struct Entry Data_03001b10[];
extern s32 Func_08003e58(s32, s32);
extern void Func_08003f3c(u32);
extern void Func_080072f0(u32, u32, s32, u32);

s32 Func_08003fa4(s32 index, s32 size, s32 source)
{
    u16 *entry;
    s32 offset;
    s32 destination;
    s32 dummy;
    u16 current;

    entry = (u16 *)Data_03001b10 + index * 2;
    destination = 0;
    if ((u32)index > 95)
        return destination;
    if ((u32)size > 0x2000)
        return destination;
    {
        dummy = 0;
        current = entry[0];
        if (current > 16) {
            if (current != size) {
                Func_08003f3c(index);
                goto allocate;
            }
            offset = entry[1];
            goto allocated;
        }

allocate:
        offset = Func_08003e58(index, size);
allocated:
        if (offset != -1) {
            destination = 0x06010000 + offset;
            entry[0] = size;
            entry[1] = offset;

            if (source != 0) {
                if (source == -1) {
                    Func_080072f0(destination, size, source, 0x03000164);
                } else {
                    struct Dma *dma;
                    dma = (struct Dma *)0x040000d4;
                    *dma = (struct Dma) {
                        source,
                        destination,
                        0x84000000 | ((u32)size >> 2),
                    };
                }
            }

            return (u32)offset >> 5;
        }
        return 0;
    }
    return destination;
}
