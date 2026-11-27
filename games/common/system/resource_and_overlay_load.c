/* syResourceGetTableEntry to syIRQIgnore, the same in both games. Both
   games' system/system_start_up.c include it where the functions stand. */

/* Returns the ROM address of resource `idx` from the resource directory.
   Called throughout the game before decoding a resource. */
void *syResourceGetTableEntry(Sint32 idx)
{
	return syResourceDirectoryTable[idx];
}

/* A Thumb long branch (BL) is two halfwords: the first carries the marker
   THUMB_BL_HIGH and the offset's high eleven bits, from THUMB_BL_HIGH_SHIFT,
   the second the marker THUMB_BL_LOW and the low eleven bits of the halfword
   offset (THUMB_BL_OFFSET_BITS each). */
#define THUMB_BL_HIGH		 0xf000
#define THUMB_BL_LOW		 0xf800
#define THUMB_BL_OFFSET_BITS 0x7ff
#define THUMB_BL_HIGH_SHIFT	 12

/* Unused: resolves the Thumb long branches (BL pairs) of a block of code whose
   branch offsets are stored from the start of the block. Each pair's offset is
   reduced by the pair's own position in the block, which turns it into the
   pc-relative offset the CPU expects; the pairs are rewritten in place.
   `byteLength` is the code size. Nothing calls it: syOverlayLoad uses the ARM
   function that does the same. */
UNUSED void syCodeResolveLongBranches(Uint16 *pair, Sint32 byteLength)
{
	Sint32 entryCnt = byteLength >> 1;
	Sint32 i = 1;

	pair++;
	if(i < entryCnt) {
		Uint32 lowMarker = THUMB_BL_LOW;
		Uint32 ofsBits = THUMB_BL_OFFSET_BITS;
		Uint32 highMarker = THUMB_BL_HIGH;

#if !FIX_POINTER_OUTSIDE_OBJECT
		/* UB: the walk starts a halfword before the block. */
		pair -= 2;
#endif
		/* Each halfword from the second on is tried as the second of a pair. */
		for(; i < entryCnt; i++) {
			Uint16 first, second;
			Uint32 secondMarker, firstMarker;

#if FIX_POINTER_OUTSIDE_OBJECT
			second = pair[0];
#else
			second = pair[2];
#endif
			secondMarker = second & lowMarker;

#if !FIX_POINTER_OUTSIDE_OBJECT
			pair++;
#endif
			if(secondMarker == lowMarker) {
#if FIX_POINTER_OUTSIDE_OBJECT
				first = pair[-1];
#else
				first = pair[0];
#endif
				firstMarker = first & secondMarker;

				if(firstMarker == highMarker) {
					Uint32 ofs = (((first & ofsBits) << THUMB_BL_HIGH_SHIFT) |
									 ((second & ofsBits) * 2)) -
						i * 2;
					Uint32 newFirst =
						((ofs >> THUMB_BL_HIGH_SHIFT) & ofsBits) | firstMarker;
					Uint32 newSecond = ((ofs >> 1) & ofsBits) | secondMarker;

#if FIX_POINTER_OUTSIDE_OBJECT
					pair[-1] = newFirst;
					pair[0] = newSecond;
#else
					pair[0] = newFirst;
					pair[1] = newSecond;
#endif
				}
			}
#if FIX_POINTER_OUTSIDE_OBJECT
			pair++;
#endif
		}
	}
}

/* Decodes the compressed code resource `idx` to `dst` and resolves
   its long branches there, with a RAM copy of the ARM function that does what
   syCodeResolveLongBranches does; the copy is allocated and freed again. */
void syOverlayLoad(Uint32 idx, void *dst)
{
	void (*patchBranches)(void *, Sint32);
	Sint32 codeSize;

	codeSize = syDecompressTaggedLZ(syResourceGetTableEntry(idx), dst);
	RAM_CODE_LOAD(patchBranches, syResourcePatchThumbBranch,
		syResourcePatchThumbBranchCodeSize);
	patchBranches(dst, codeSize);
	syHeapRelease(patchBranches);
}

/* The handler of every interrupt nobody has claimed. */
void syIRQIgnore(void)
{
}
