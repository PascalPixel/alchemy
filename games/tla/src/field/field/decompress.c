/// @file
/// Resource metadata records and LZ decompression.
// Part of field.c.

#include "../field_internal.h"

struct AnimationMetadata *Resource_GetMetadataRecordFar(u32 id) {
  id &= 0xfff;
  if (id >= 716)
    id = 0;
  return &gResource_Data012[id];
}

/// Decompress source into destination and return destination.
/// source[0-1]: offset to commands; 0 means uncompressed, returning source + 2.
/// source + 2 contains literal bytes. In the command stream,
/// read each control byte from its least significant bit:
/// 1: copy one literal byte.
/// 0: read two bytes, high then low; zero terminates.
/// Low 12 bits give distance; high 4 bits + 2 give length, or the next byte +
/// 18 if those 4 bits are zero. Copy from that distance behind source + 2.
u8 *Decompress_Lz(const u8 *source, u8 *destination) {
  s32 token;
  u32 length;
  u32 control;
  u8 byte;
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
  // A marker bit, shifted down as the flags are read.
  base = literal;
  start = destination;
  command++;
  control |= 0x100;
  // Consume literals or copy runs until the zero token ends the stream.
  for (;;) {
    if ((control & 1) != 0) {
      // Only the marker left means: read the next command byte.
      if (control != 1)
        *destination++ = *literal++;
    } else {
      byte = *command++;
      token = byte << 8;
      token |= *command++;
      if (token == 0)
        break;
      length = token >> 12;
      if (length == 0) {
        length = *command + 16;
        command++;
      }
      length += 2;
      // A run copies from before the literals' start, not from the output.
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
