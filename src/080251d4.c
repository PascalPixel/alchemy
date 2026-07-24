typedef unsigned int u32;

void Func_080251d4(u32 source, u32 destination)
{
    u32 mask;
    u32 destination_index;
    u32 destination_address;
    u32 source_address;
    u32 *dma;

    mask = 0x3ff;
    destination_index = destination & mask;
    mask = source & mask;
    source_address = 0x06000000 + (mask << 5);
    destination_address = 0x06000000 + (destination_index << 5);
    dma = (u32 *)0x040000d4;
    dma[0] = source_address;
    dma[1] = destination_address;
    dma[2] = 0x84000008;
}
