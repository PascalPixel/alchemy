/* @file */
/* Resource decoding, through RAM copies of ARM code. */
/* Part of system.c. */

/* Decompresses the halfword LZ stream at `source` to `destination`, with a */
/* copy of the ARM decoder on the stack. */
void syDecompressHalfwords(const void *source, void *destination) {
	u8 routine[(u32)syDecompressHalfwordsCodeSize];

	syDMAXCopy((const void *)Decompress_HalfwordLz, routine,
			   DMA_COPY_WORDS((u32)syDecompressHalfwordsCodeSize >> 2),
			   REG_DMA3);
	((void (*)(const void *, void *))routine)(source, destination);
}

/* Decompresses the tagged LZ data at `source` to `destination` with a heap */
/* copy of its ARM decoder, and returns what the decoder returns. */
s32 syDecompressTaggedLz(const void *source, void *destination) {
	s32 (*routine)(const void *, void *);
	s32 result;

	syRamCodeLoad(routine, syDecompressTaggedLzArmCode,
				  syDecompressTaggedLzCodeSize);
	result = routine(source, destination);
	syHeapRelease(routine);
	return result;
}

/* As syDecompressTaggedLz, with the move-to-front LZ decoder. */
s32 syDecompressMoveToFrontLz(const void *source, void *destination) {
	s32 (*routine)(const void *, void *);
	s32 result;

	syRamCodeLoad(routine, Decompress_MoveToFrontLzCommandsCode,
				  Decompress_MoveToFrontLzCommandsCodeSize);
	result = routine(source, destination);
	syHeapRelease(routine);
	return result;
}

/* As syDecompressTaggedLz, with the byte LZ decoder. */
s32 syDecompressByteLz(const void *source, void *destination) {
	s32 (*routine)(const void *, void *);
	s32 result;

	syRamCodeLoad(routine, Resource_DecodeByteLzCode,
				  Resource_DecodeByteLzCodeSize);
	result = routine(source, destination);
	syHeapRelease(routine);
	return result;
}
