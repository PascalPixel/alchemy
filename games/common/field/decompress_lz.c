/* fdDecompressLZ, the same in both games. Both games'
   field/field_decompress.c include it where the function stands. */

/* Decompresses an LZ stream from source into destination and returns
   destination. */
Uint8 *fdDecompressLZ(const Uint8 *src, Uint8 *dst)
{
	Sint32		 token;
	Uint32		 length;
	Uint32		 control;
	Uint8		*start;
	const Uint8 *base;
	const Uint8 *command;
	const Uint8 *copy;
	const Uint8 *literal;

	/* Uncompressed blocks borrow their payload; compressed blocks use the
	   output. */
	token = src[0] | (src[1] << 8);
	literal = src + 2;
	if(token == 0)
		return (Uint8 *)literal;
	command = src + token;
	control = *command;
	base = literal;
	start = dst;
	command++;
	/* A marker bit, shifted down as the flags are read. */
	control |= LZ_FLAGS_MARKER;
	/* Consume literals or copy runs until the zero token ends the stream. */
	for(;;) {
		if((control & 1) != 0) {
			/* Only the marker left means: read the next command byte. */
			if(control != 1)
				*dst++ = *literal++;
		} else {
			token = *command++ << 8;
			token |= *command++;
			if(token == 0)
				break;
			length = token >> LZ_LENGTH_SHIFT;
			if(length == 0) {
				length = *command + LZ_LONG_LENGTH_BASE;
				command++;
			}
			length += 2;
			/* A run copies from before the literals' start, not from the
			   output:
			   a sprite's codec 3 pictures are stored one after another, and
			   each is packed against the pictures stored before it, as
			   syIWRAMDecompress also reads them. 253 sprites of The Broken
			   Seal, Isaac's among them, and 441 of The Lost Age, Felix's among
			   them, use codec 3. */
			copy = base - (token & LZ_DISTANCE_MASK);
			while(length != 0) {
				*dst++ = *copy++;
				length--;
			}
		}
		control >>= 1;
		if(control == 0) {
			control = *command;
			command++;
			control |= LZ_FLAGS_MARKER;
		}
	}
	return start;
}
