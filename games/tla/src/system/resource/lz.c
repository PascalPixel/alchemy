//--------------------------------------------------------------------------
// lz.c
// Resources: decompress LZ data.
//--------------------------------------------------------------------------

#include "types.h"
#include "scene.h"

//--------------------------------------------------------------------------
// Decompress source into destination and return destination.
// source[0-1]: offset to commands; 0 means uncompressed, returning source + 2.
// source + 2 contains literal bytes. In the command stream,
// read each control byte from its least significant bit:
// 1: copy one literal byte.
// 0: read two bytes, high then low; zero terminates.
// Low 12 bits give distance; high 4 bits + 2 give length, or the next byte + 18
// if those 4 bits are zero. Copy from that distance behind source + 2.
//--------------------------------------------------------------------------
u8 *Resource_DecompressLz(const u8 *source, u8 *destination) {
  s32 token;
  u32 length;
  u32 sentinel;
  u32 control;
  u8 byte;
  u8 *start;
  const u8 *base;
  const u8 *command;
  const u8 *copy;
  const u8 *literal;

  token = source[0] | (source[1] << 8);
  literal = source + 2;
  if (token == 0)
    return (u8 *)literal;
  command = source + token;
  control = *command;
  sentinel = 128;
  sentinel <<= 1; // Marker indicating all eight bits were read.
  base = literal;
  start = destination;
  command++;
  control |= sentinel;
  goto dispatch;

back_reference:
  length = token >> 12;
  if (length == 0) {
    length = *command + 16;
    command++;
  }
  length += 2;
  copy = base - (token & 0x0fff);
  while (length != 0) {
    *destination++ = *copy++;
    length--;
  }

shift:
  control >>= 1;
  if (control == 0) {
    control = *command;
    sentinel = 128;
    sentinel <<= 1;
    command++;
    control |= sentinel;
  }

dispatch:
  if ((control & 1) != 0) {
    if (control !=
        1) // Read the next control byte when only the marker remains.
      *destination++ = *literal++;
    goto shift;
  }

  byte = *command++;
  token = byte << 8;
  token |= *command++;
  if (token != 0)
    goto back_reference;
  return start;
}
