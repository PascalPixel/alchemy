/// @file
/// LZ decompression.
// Part of field.c.

#include "../field_internal.h"

/// Decompresses an LZ stream from source into destination and returns
/// destination. The stream starts with a little-endian halfword: the offset of
/// its command bytes, or 0 for an uncompressed block, whose data (source + 2)
/// is returned as it is. The literal bytes follow that halfword. Each command
/// byte gives eight flags, low bit first: 1 copies the next literal byte; 0
/// reads a big-endian halfword whose low 12 bits are a distance and high 4
/// bits a length - 2 (or, when 0, the next byte + 16 + 2), and copies that many
/// bytes from the literals' start minus the distance. A zero halfword ends the
/// stream. Called only by Sprite_ComposePicture, for a codec 3 (LZ)
/// sprite anim that has a draw parameter, into a 0x400-byte scratch buffer.
u8 *Decompress_Lz(const u8 *source, u8 *destination) {
  s32 token;
  u32 length;
  u32 control;
  u8 *start;
  const u8 *base;
  const u8 *command;
  const u8 *copy;
  const u8 *literal;

  // Uncompressed blocks borrow their payload; compressed blocks use the output.
  token = source[0] | (source[1] << 8);
  literal = source + 2;
  if (token == 0)
    return (u8 *)literal;
  command = source + token;
  control = *command;
  base = literal;
  start = destination;
  command++;
  // A marker bit, shifted down as the flags are read.
  control |= 0x100;
  // Consume literals or copy runs until the zero token ends the stream.
  for (;;) {
    if ((control & 1) != 0) {
      // Only the marker left means: read the next command byte.
      if (control != 1)
        *destination++ = *literal++;
    } else {
      token = *command++ << 8;
      token |= *command++;
      if (token == 0)
        break;
      length = token >> 12;
      if (length == 0) {
        length = *command + 16;
        command++;
      }
      length += 2;
      // A run copies from before the literals' start, not from the output:
      // a sprite's codec 3 pictures are stored one after another, and each is
      // packed against the pictures stored before it, as IwramDecompress also
      // reads them. 253 sprites, Isaac's among them, use codec 3.
      copy = base - (token & 0x0fff);
      while (length != 0) {
        *destination++ = *copy++;
        length--;
      }
    }
    control >>= 1;
    if (control == 0) {
      control = *command;
      command++;
      control |= 0x100;
    }
  }
  return start;
}
