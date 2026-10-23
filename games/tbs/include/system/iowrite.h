#ifndef ALCHEMY_TBS_IOWRITE_H
#define ALCHEMY_TBS_IOWRITE_H

/// @file
/// Io_Write16 and Io_Write32, writes to a register or memory.

#include "types.h"

/// Writes a value to a 16-bit register or memory location.
// A statement macro (S2: a layer the whole game uses, 107 sites and 2 through
// Io_Write32): the value is built in a word before the address and the store
// stays where it is written. An inline function with a u32 parameter builds
// the same value but lets the store move.
#define Io_Write16(v, reg)                                                     \
  do {                                                                         \
    u32 value_ = (v);                                                          \
    *(reg) = value_;                                                           \
  } while (0)

/// Writes a value to a 32-bit register or memory location, in Io_Write16's
/// shape.
#define Io_Write32(v, reg) Io_Write16(v, reg)

#endif
