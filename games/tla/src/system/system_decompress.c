// Resource decoding, through RAM copies of ARM code. Part of system.c.

// Decompresses the halfword LZ stream at `src` to `dest`, with a copy of the ARM decoder on the
// stack.
void syDecompressHalfwords(const void *src, void *dest)
{
    Uint8 routine[(Uint32)syDecompressHalfwordsCodeSize];

    syDMAXCopy((const void *)syDecompressHalfwordLZ,
               routine,
               DMA_COPY_WORDS((Uint32)syDecompressHalfwordsCodeSize >> 2),
               REG_DMA3);
    ((void (*)(const void *, void *))routine)(src, dest);
}

// Decompresses the tagged LZ data at `src` to `dest` with a heap copy of its ARM decoder, and
// returns what the decoder returns.
Sint32 syDecompressTaggedLZ(const void *src, void *dest)
{
    Sint32 (*routine)(const void *, void *);
    Sint32 result;

    RAM_CODE_LOAD(routine, syDecompressTaggedLZArmCode, syDecompressTaggedLZCodeSize);
    result = routine(src, dest);
    syHeapRelease(routine);
    return result;
}

// As syDecompressTaggedLZ, with the move-to-front LZ decoder.
Sint32 syDecompressMoveToFrontLZ(const void *src, void *dest)
{
    Sint32 (*routine)(const void *, void *);
    Sint32 result;

    RAM_CODE_LOAD(
        routine, syDecompressMoveToFrontLZCommandsCode, DecompressMoveToFrontLzCommandsCodeSize);
    result = routine(src, dest);
    syHeapRelease(routine);
    return result;
}

// As syDecompressTaggedLZ, with the byte LZ decoder.
Sint32 syDecompressByteLZ(const void *src, void *dest)
{
    Sint32 (*routine)(const void *, void *);
    Sint32 result;

    RAM_CODE_LOAD(routine, ResourceDecodeByteLzCode, ResourceDecodeByteLzCodeSize);
    result = routine(src, dest);
    syHeapRelease(routine);
    return result;
}
