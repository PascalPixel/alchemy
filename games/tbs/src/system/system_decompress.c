/* Resource decoding and pixel buffer shading, through RAM copies of ARM code.
   Part of system.c. */

/* Decompresses the halfword LZ stream at `src` to `dest`, with a copy
   of the ARM decoder on the stack. */
void syDecompressHalfwords(const void *src, void *dest)
{
	Uint32 words = (Uint32)syDecompressHalfwordsCodeSize >> 2;
	Uint32 routine[words];
	syDMAXCopy(
		syDecompressHalfwordLZ, routine, DMA_COPY_WORDS(words), REG_DMA3);
	((void (*)(const void *, void *))routine)(src, dest);
}

/* Each decoder copies its ARM function `code` to RAM, runs it and frees the
   copy, and returns the number of bytes it decoded. */
#define RESOURCE_DEFINE_DECODE(name, code, bytes) \
	Sint32 name(const void *src, void *dest) \
	{ \
		Sint32 (*routine)(const void *, void *); \
		Sint32 result; \
\
		RAM_CODE_LOAD(routine, code, bytes); \
		result = routine(src, dest); \
		syHeapRelease(routine); \
		return result; \
	}

/* Decodes a tagged LZ resource (a first byte of 0 for the general form, 1 for
   the palette form) and returns its unpacked byte count. */
RESOURCE_DEFINE_DECODE(
	syDecompressTaggedLZ, syDecompressTaggedLZArm, syDecompressTaggedLZCodeSize)
/* Decodes a move-to-front LZ resource (a first byte of 2) and returns its
   unpacked byte count. */
RESOURCE_DEFINE_DECODE(syDecompressMoveToFrontLZ,
	syDecompressMoveToFrontLZCommands, DecompressMoveToFrontLzCommandsCodeSize)
/* Decodes a byte-oriented LZ resource and returns its unpacked byte count. */
RESOURCE_DEFINE_DECODE(
	syDecompressByteLZ, syDecompressByteLZArm, syDecompressByteLZCodeSize)

/* The canvas fades of battle effects, the element glow and the Lucky Wheels. A
   canvas is a buffer of 8-bit pixels, each a step 0 to 63 of a colour ramp.
   Each function copies the canvas to `dest`, the tiles on screen, 16
   bytes at a time, and shades each group in the canvas as it goes, so what is
   drawn next lands on a dimmer copy of this tick. `amount` holds the step in
   each of its four bytes (CANVAS_FILL_WORD). */
#define CANVAS_DEFINE_FADE_BY(name, code, size) \
	void name(Uint8 *buf, Uint32 amount, Uint8 *dest, Uint32 bytes) \
	{ \
		Uint32 words = (Uint32)size >> 2; \
		Uint32 routine[words]; \
		syDMAXCopy(code, routine, DMA_COPY_WORDS(words), REG_DMA3); \
		((void (*)(Uint8 *, Uint32, Uint8 *, Uint32))routine)( \
			buf, amount, dest, bytes); \
	}
#define CANVAS_DEFINE_FADE(name, code, size) \
	void name(Uint8 *buf, Uint8 *dest, Uint32 bytes) \
	{ \
		Uint32 words = (Uint32)size >> 2; \
		Uint32 routine[words]; \
		syDMAXCopy(code, routine, DMA_COPY_WORDS(words), REG_DMA3); \
		((void (*)(Uint8 *, Uint8 *, Uint32))routine)(buf, dest, bytes); \
	}

/* Raises every pixel in the canvas by `amount`, stopping at 63. */
CANVAS_DEFINE_FADE_BY(syCanvasCopyAndBrighten, syCanvasBrightenArm,
	syCanvasCopyAndBrightenCodeSize)
/* Lowers every pixel in the canvas by `amount`, stopping at 0. */
CANVAS_DEFINE_FADE_BY(
	syCanvasCopyAndDarken, syCanvasDarkenArm, syCanvasCopyAndDarkenCodeSize)
/* Scales every pixel to three quarters. */
CANVAS_DEFINE_FADE(syCanvasCopyAndScaleThreeQuarters,
	syCanvasScaleThreeQuartersArm, syCanvasCopyAndScaleThreeQuartersCodeSize)
/* Halves every pixel. */
CANVAS_DEFINE_FADE(
	syCanvasCopyAndHalve, syCanvasHalveArm, syCanvasCopyAndHalveCodeSize)
/* As syCanvasCopyAndBrighten, skipping groups already all at 63. */
CANVAS_DEFINE_FADE_BY(syCanvasCopyAndBrightenPartial,
	syCanvasBrightenPartialArm, syCanvasCopyAndBrightenPartialCodeSize)
/* As syCanvasCopyAndDarken, skipping groups already all at 0. */
CANVAS_DEFINE_FADE_BY(syCanvasCopyAndDarkenPartial, syCanvasDarkenPartialArm,
	syCanvasCopyAndDarkenPartialCodeSize)
/* Scales every pixel to three quarters, skipping all-zero groups. */
CANVAS_DEFINE_FADE(syCanvasCopyAndScaleNonzeroThreeQuarters,
	syCanvasScaleNonzeroThreeQuartersArm,
	syCanvasCopyAndScaleNonzeroThreeQuartersCodeSize)
/* Halves every pixel, skipping all-zero groups. */
CANVAS_DEFINE_FADE(syCanvasCopyAndHalveNonzero, syCanvasHalveNonzeroArm,
	syCanvasCopyAndHalveNonzeroCodeSize)

#undef RESOURCE_DEFINE_DECODE
#undef CANVAS_DEFINE_FADE_BY
#undef CANVAS_DEFINE_FADE
